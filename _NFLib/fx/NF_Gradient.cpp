#include "NF_Gradient.h"

#ifndef AE_CLAMP
#define AE_CLAMP(VAL, MIN, MAX) ((VAL) < (MIN) ? (MIN) : ((VAL) > (MAX) ? (MAX) : (VAL)))
#endif

typedef struct {
	float x0, y0;
	float ux, uy;
	float inv_len_sq;	// 直線用: 1/長さ^2
	float inv_radius;	// 円形用: 1/半径
	float k_rgb;		// RGB用双曲線係数
	float k_alpha;		// Alpha用双曲線係数
	A_long mode;		// 0:Linear, 1:Radial

	float c0[4];		// 開始色(r,g,b,a) 0.0-1.0
	float c1[4];		// 終了色(r,g,b,a) 0.0-1.0
} GradRenderData;


// 負数対応の双曲線補間
static inline float ApplyHyperbolic(float t, float k) {
	if (k == 0.0f) return t; // 0の時は処理しない

	if (k > 0.0f) {
		// 正数の場合
		return ((1.0f + k) * t) / (1.0f + k * t);
	}
	else {
		// 負数の場合: 反転して正数(-k)で処理し、再度反転
		float pk = -k;
		float inv_t = 1.0f - t;
		float pk2 = pk * inv_t;
		if (pk2 == -1.0f) pk2 = -0.99999f; // 0除算防止	
		float res = ((1.0f + pk) * inv_t) / (1.0f + pk2);
		return 1.0f - res;
	}
}

template <typename PixelType>
static PF_Err GradientFunc(
	void* refcon,
	A_long x,
	A_long y,
	PixelType* in,
	PixelType* out)
{
	GradRenderData* gd = static_cast<GradRenderData*>(refcon);

	float accum_tr = 0.0f;
	float accum_ta = 0.0f;

	const float off[4][2] = { {-0.25f,-0.25f}, {0.25f,-0.25f}, {-0.25f,0.25f}, {0.25f,0.25f} };

	for (int i = 0; i < 4; i++) {
		float px = static_cast<float>(x) + off[i][0];
		float py = static_cast<float>(y) + off[i][1];
		float t = 0.0f;

		if (gd->mode == 0) {
			t = ((px - gd->x0) * gd->ux + (py - gd->y0) * gd->uy) * gd->inv_len_sq;
		}
		else {
			t = sqrtf(powf(px - gd->x0, 2) + powf(py - gd->y0, 2)) * gd->inv_radius;
		}
		t = AE_CLAMP(t, 0.0f, 1.0f);

		// RGB/Alpha個別に負数対応の双曲線補間を適用
		accum_tr += ApplyHyperbolic(t, gd->k_rgb);
		accum_ta += ApplyHyperbolic(t, gd->k_alpha);
	}

	float tr = accum_tr * 0.25f;
	float ta = accum_ta * 0.25f;

	// カラー補間と出力 (32bit float / 16bit / 8bit)
	if constexpr (std::is_same_v<PixelType, PF_PixelFloat>) {
		out->red = gd->c0[0] + tr * (gd->c1[0] - gd->c0[0]);
		out->green = gd->c0[1] + tr * (gd->c1[1] - gd->c0[1]);
		out->blue = gd->c0[2] + tr * (gd->c1[2] - gd->c0[2]);
		out->alpha = gd->c0[3] + ta * (gd->c1[3] - gd->c0[3]);
	}
	else {
		float max_v = std::is_same_v<PixelType, PF_Pixel16> ? 32768.0f : 255.0f;
		out->red = static_cast<decltype(out->red)>((gd->c0[0] + tr * (gd->c1[0] - gd->c0[0])) * max_v + 0.5f);
		out->green = static_cast<decltype(out->green)>((gd->c0[1] + tr * (gd->c1[1] - gd->c0[1])) * max_v + 0.5f);
		out->blue = static_cast<decltype(out->blue)>((gd->c0[2] + tr * (gd->c1[2] - gd->c0[2])) * max_v + 0.5f);
		out->alpha = static_cast<decltype(out->alpha)>((gd->c0[3] + ta * (gd->c1[3] - gd->c0[3])) * max_v + 0.5f);
	}

	return PF_Err_NONE;
}

PF_Err Gradient( 
PF_InData* in_data,
PF_EffectWorld* world,
PF_PixelFormat pixelFormat,
AEGP_SuiteHandler* suitesP,
A_long mode,
A_FloatPoint start_pos,
PF_Pixel start_color,
PF_FpLong	start_opa,
A_FloatPoint end_pos,
PF_Pixel end_color,
PF_FpLong	end_opa,
PF_FpLong	rgb_hyperbolic,
PF_FpLong	alpha_hyperbolic)
{
	PF_Err err = PF_Err_NONE;
	GradRenderData gd;

	if(abs(start_pos.x - end_pos.x)<2 && abs(start_pos.y - end_pos.y)<2) {
		PF_FILL(&end_color,NULL, world);
		return err;
	}


	// 座標と方向ベクトルの準備
	float x0 = static_cast<float>(start_pos.x);
	float y0 = static_cast<float>(start_pos.y);
	float x1 = static_cast<float>(end_pos.x);
	float y1 = static_cast<float>(end_pos.y);
	float ux = x1 - x0;
	float uy = y1 - y0;
	float lensq = ux * ux + uy * uy + 0.000001f;

	gd.x0 = x0; gd.y0 = y0;
	gd.ux = ux; gd.uy = uy;
	gd.inv_len_sq = 1.0f / lensq;
	gd.inv_radius = 1.0f / sqrtf(lensq);
	gd.mode = mode - 1; // Popupは1から始まるため


	gd.k_rgb = static_cast<float>(rgb_hyperbolic);
	gd.k_alpha = static_cast<float>(alpha_hyperbolic);

	// 負の値によるゼロ除算や不連続を防ぐガードのみ
	//if (gd.k_rgb < 0.0f)   gd.k_rgb = 0.0f;
	//if (gd.k_alpha < 0.0f) gd.k_alpha = 0.0f;

	// 色の正規化 (0.0-1.0)
	gd.c0[0] = start_color.red / 255.0f;
	gd.c0[1] = start_color.green / 255.0f;
	gd.c0[2] = start_color.blue / 255.0f;
	gd.c0[3] = (float)start_opa / 100.0f;

	gd.c1[0] = end_color.red / 255.0f;
	gd.c1[1] = end_color.green / 255.0f;
	gd.c1[2] = end_color.blue / 255.0f;
	gd.c1[3] = (float)end_opa / 100.0f;

	// 深度に応じたイテレート実行

	switch (pixelFormat) {
	case PF_PixelFormat_ARGB32:
		err = suitesP->Iterate8Suite2()->iterate(
			in_data, 
			0,
			world->height,
			world,
			NULL, 
			&gd, 
			GradientFunc<PF_Pixel8>,
			world);
		break;
	case PF_PixelFormat_ARGB64:
		err = suitesP->Iterate16Suite2()->iterate(
			in_data,
			0,
			world->height,
			world,
			NULL,
			&gd,
			GradientFunc<PF_Pixel16>,
			world); break;
	case PF_PixelFormat_ARGB128:
		err = suitesP->IterateFloatSuite2()->iterate(
			in_data,
			0,
			world->height,
			world,
			NULL,
			&gd,
			GradientFunc<PF_PixelFloat>,
			world); break;
		break;
	}

	return err;
}