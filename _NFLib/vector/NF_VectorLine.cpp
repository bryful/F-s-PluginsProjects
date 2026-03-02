#include "NF_VectorLine.h"


// ヘルパー関数: ベクトルの外積（2D）
float cross_product(float ax, float ay, float bx, float by) {
    return ax * by - ay * bx;
}

// ヘルパー関数: 点Pから線分ABへの最短距離（符号なし）
float dist_to_line(float px, float py, float x0, float y0, float x1, float y1) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float l2 = dx * dx + dy * dy;
    if (l2 == 0.0f) return std::sqrt((px - x0) * (px - x0) + (py - y0) * (py - y0));
    float t = max(0.0f, min(1.0f, ((px - x0) * dx + (py - y0) * dy) / l2));
    float proj_x = x0 + t * dx;
    float proj_y = y0 + t * dy;
    return std::sqrt((px - proj_x) * (px - proj_x) + (py - proj_y) * (py - proj_y));
}

void draw_a_rect(std::vector<std::vector<float>>& data,
    float x0, float y0, float x1, float y1,
    float x2, float y2, float x3, float y3,
    float value
    ) {
    int height = (int)data.size();
    if (height == 0) return;
    int width = (int)data[0].size();




    float px[] = { x0, x1, x2, x3 };
    float py[] = { y0, y1, y2, y3 };

    // 1. バウンディングボックスの計算（最適化のため）
    int min_x = max(0, (int)std::floor(*std::min_element(px, px + 4) - 1));
    int max_x = min(width - 1, (int)std::ceil(*std::max_element(px, px + 4) + 1));
    int min_y = max(0, (int)std::floor(*std::min_element(py, py + 4) - 1));
    int max_y = min(height - 1, (int)std::ceil(*std::max_element(py, py + 4) + 1));

    // 2. 各ピクセルに対してカバレッジを計算
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            float fx = (float)x;
            float fy = (float)y;

            // 各辺に対する外積を確認（全て同符号なら内部）
            bool inside = true;
            float min_dist = 1e10f;

            for (int i = 0; i < 4; ++i) {
                int next = (i + 1) % 4;
                float cp = cross_product(px[next] - px[i], py[next] - py[i], fx - px[i], fy - py[i]);

                // 頂点の順序が時計回りか反時計回りかで符号が変わるため、簡易的に判定
                // ここでは凸四角形であることを前提に、辺からの距離を保持
                if (cp < 0) inside = false; // 厳密には頂点順に依存

                min_dist = min(min_dist, dist_to_line(fx, fy, px[i], py[i], px[next], py[next]));
            }

            // 3. アンチエイリアス係数（coverage）の決定
            // 境界付近（距離1ピクセル以内）で滑らかに変化させる
            float coverage = 0.0f;
            if (inside) {
                coverage = min(1.0f, min_dist + 0.5f);
            }
            else {
                coverage = max(0.0f, 0.5f - min_dist);
            }

            // 4. データの更新（アルファブレンディング風）
            data[y][x] = data[y][x] * (1.0f - coverage) + value * coverage;
        }
    }
}
void draw_a_rect(std::vector<std::vector<float>>& data,
    PF_Point p0, PF_Point p1, PF_Point p2, PF_Point p3,
    float value)
{
    draw_a_rect(data, (float)p0.h, (float)p0.v, (float)p1.h, (float)p1.v, (float)p2.h, (float)p2.v, (float)p3.h, (float)p3.v, value);
}
// ***************************************************************************
// **************************************************************************************

void draw_aa_line(std::vector<std::vector<float>>& data,
    float x0, float y0, float b0,
    float x1, float y1, float b1,
    float value) {
    if (data.empty()) return;
    int height = (int)data.size();
    int width = (int)data[0].size();

    float r0 = b0 * 0.5f;
    float r1 = b1 * 0.5f;

    // 1. ベクトル計算
    float dx = x1 - x0;
    float dy = y1 - y0;
    float l2 = dx * dx + dy * dy;
    if (l2 < 1e-6f) return; // 長さがほぼ0なら描画しない

    // 2. バウンディングボックス（少し余裕を持つ）
    float max_r = max(r0, r1) + 1.0f;
    int min_x = AE_CLAMP((int)std::floor(min(x0, x1) - max_r), 0, width - 1);
    int max_x = AE_CLAMP((int)std::ceil(max(x0, x1) + max_r), 0, width - 1);
    int min_y = AE_CLAMP((int)std::floor(min(y0, y1) - max_r), 0, height - 1);
    int max_y = AE_CLAMP((int)std::ceil(max(y0, y1) + max_r), 0, height - 1);

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            float fx = (float)x;
            float fy = (float)y;

            // 3. 線分上の射影位置 t を計算
            float t = ((fx - x0) * dx + (fy - y0) * dy) / l2;

            // --- 鋭角（平らな端）にするための判定 ---
            // t < 0 は始点より手前、t > 1 は終点より後ろ
            // ここでもアンチエイリアスを効かせるため、境界付近で coverage を絞る
            float edge_alpha = 1.0f;
            if (t < 0.0f) {
                // 始点の断面：距離を 0.5px で滑らかにする
                edge_alpha = AE_CLAMP(t * std::sqrt(l2) + 0.5f, 0.0f, 1.0f);
                t = 0.0f;
            }
            else if (t > 1.0f) {
                // 終点の断面
                edge_alpha = AE_CLAMP((1.0f - t) * std::sqrt(l2) + 0.5f, 0.0f, 1.0f);
                t = 1.0f;
            }

            // 4. その地点での半径と距離の計算
            float target_r = r0 + t * (r1 - r0);
            float proj_x = x0 + t * dx;
            float proj_y = y0 + t * dy;
            float dist = std::sqrt((fx - proj_x) * (fx - proj_x) + (fy - proj_y) * (fy - proj_y));

            // 5. カバレッジ計算（太さ方向）
            float radius_alpha = AE_CLAMP(target_r + 0.5f - dist, 0.0f, 1.0f);

            // 6. 二つのアルファを合成（断面の鋭さと太さの滑らかさ）
            float final_coverage = radius_alpha * edge_alpha;

            if (final_coverage > 0.0f) {
                data[y][x] = data[y][x] * (1.0f - final_coverage) + value * final_coverage;
            }
        }
    }
}
// **************************************************************************************

/**
 * アンチエイリアス付き可変幅線描画
 * @param data 描画対象の2次元配列
 * @param x0, y0 始点座標
 * @param b0 始点の太さ（直径）
 * @param x1, y1 終点座標
 * @param b1 終点の太さ（直径）
 * @param value 描画する値
 */
void draw_a_line(
    std::vector<std::vector<float>>& data,
    float x0, float y0, float b0,
    float x1, float y1, float b1,
    float value) 
{
    if (data.empty()) return;
    int height = (int)data.size();
    int width = (int)data[0].size();

    float r0 = b0 * 0.5f;
    float r1 = b1 * 0.5f;

    // 1. バウンディングボックスの計算
    float margin = max(r0, r1) + 1.0f;
    int min_x = AE_CLAMP((int)std::floor(min(x0, x1) - margin), 0, width - 1);
    int max_x = AE_CLAMP((int)std::ceil(max(x0, x1) + margin), 0, width - 1);
    int min_y = AE_CLAMP((int)std::floor(min(y0, y1) - margin), 0, height - 1);
    int max_y = AE_CLAMP((int)std::ceil(max(y0, y1) + margin), 0, height - 1);

    float dx = x1 - x0;
    float dy = y1 - y0;
    float l2 = dx * dx + dy * dy;

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            float fx = (float)x;
            float fy = (float)y;

            // 2. 線分に対する射影係数 t を求める
            float t = 0.0f;
            if (l2 > 0.0f) {
                t = ((fx - x0) * dx + (fy - y0) * dy) / l2;
                t = std::clamp(t, 0.0f, 1.0f);
            }

            // 3. その地点での「目標半径」を補間
            float target_r = r0 + t * (r1 - r0);

            // 4. ピクセルと補間された中心点との距離
            float proj_x = x0 + t * dx;
            float proj_y = y0 + t * dy;
            float dist = std::sqrt((fx - proj_x) * (fx - proj_x) + (fy - proj_y) * (fy - proj_y));

            // 5. アンチエイリアス（カバレッジ計算）
            // 半径の内側なら1.0、外側1px以上なら0.0
            float coverage = AE_CLAMP(target_r + 0.5f - dist, 0.0f, 1.0f);

            if (coverage > 0.0f) {
                // 元の値と線形補完（Alpha Blending風）
                data[y][x] = data[y][x] * (1.0f - coverage) + value * coverage;
            }
        }
    }
}
void draw_a_line(std::vector<std::vector<float>>& data,
	PF_Point p0, float b0, PF_Point p1, float b1,
    float value)
{
	draw_a_line(data, (float)p0.h, (float)p0.v, b0, (float)p1.h, (float)p1.v, b1, value);
}
void draw_a_line(std::vector<std::vector<float>>& data,
    a_linePrm p0, a_linePrm p1,
    float value)
{
	draw_a_line(data, 
        p0.x, p0.y, p0.b, 
        p1.x, p1.y, p1.b, 
        value);
}
void draw_a_line(std::vector<std::vector<float>>& data,
    std::vector<a_linePrm> prms,
    float value)
{
	if (prms.size() < 2) return;
    for(int i=0; i<(int)prms.size()-1; i++) {
        draw_a_line(
            data, 
            prms[i].x, prms[i].y, prms[i].b,
            prms[i+1].x, prms[i+1].y, prms[i+1].b,
            value);
	}
}
// **************************************************************************************


void draw_polyline(
    std::vector<std::vector<float>>& data, 
    const std::vector<a_linePrm>& points, 
    float v
) {
    if (points.size() < 2 || data.empty()) return;
    int height = (int)data.size();
    int width = (int)data[0].size();

    for (size_t i = 0; i < points.size() - 1; ++i) {
        float x0 = points[i].x, y0 = points[i].y, r0 = points[i].b * 0.5f;
        float x1 = points[i + 1].x, y1 = points[i + 1].y, r1 = points[i + 1].b * 0.5f;

        // 1. 線分のベクトル計算
        float dx = x1 - x0;
        float dy = y1 - y0;
        float l2 = dx * dx + dy * dy;

        // 2. この線分が影響する範囲（バウンディングボックス）を計算
        float max_r = max(r0, r1) + 1.5f;
        int min_x = AE_CLAMP((int)(min(x0, x1) - max_r), 0, width - 1);
        int max_x = AE_CLAMP((int)(max(x0, x1) + max_r), 0, width - 1);
        int min_y = AE_CLAMP((int)(min(y0, y1) - max_r), 0, height - 1);
        int max_y = AE_CLAMP((int)(max(y0, y1) + max_r), 0, height - 1);

        for (int y = min_y; y <= max_y; ++y) {
            for (int x = min_x; x <= max_x; ++x) {
                float fx = (float)x, fy = (float)y;

                // 3. 点(fx, fy)から線分への最短距離を求める
                float t = 0.0f;
                if (l2 > 0.0f) {
                    t = AE_CLAMP(((fx - x0) * dx + (fy - y0) * dy) / l2, 0.0f, 1.0f);
                }

                // 線分上の最短地点の座標
                float proj_x = x0 + t * dx;
                float proj_y = y0 + t * dy;

                // その地点での「太さ（半径）」を線形補間
                float target_r = r0 + t * (r1 - r0);

                // ピクセルと線分の距離
                float dist = std::sqrt((fx - proj_x) * (fx - proj_x) + (fy - proj_y) * (fy - proj_y));

                // 4. アンチエイリアス計算
                float alpha = AE_CLAMP(target_r + 0.5f - dist, 0.0f, 1.0f);

                if (alpha > 0.0f) {
                    // 5. 既存の描画内容と「最大値」で合成（これで重なりも隙間も解決）
                    data[y][x] = max(data[y][x], v * alpha);
                }
            }
        }
    }
}

std::vector<a_linePrm> offsetLinePrm(const std::vector<a_linePrm>& points, A_long ox,A_long oy) 
{
    std::vector<a_linePrm> result;
    for (const auto& p : points) {
        result.push_back({ p.x + ox, p.y + oy, p.b });
    }
    return result;
}
std::vector<a_linePrm> offsetLinePrm(A_long size,a_linePrm *points, A_long ox, A_long oy)
{
    std::vector<a_linePrm> result;
	if (size <= 0) return result;
    for (int i=0; i<size;i++) {
        result.push_back({ points[i].x + ox, points[i].y + oy, points[i].b });
    }
    return result;
}