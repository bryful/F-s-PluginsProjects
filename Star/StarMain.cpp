#include "Star.h"


// ピクセル型ごとの最大チャンネル値
template <typename T> inline float GetMaxChan()                  { return 255.0f;   }
template <>           inline float GetMaxChan<PF_Pixel16>()      { return 32768.0f; }
template <>           inline float GetMaxChan<PF_PixelFloat>()   { return 1.0f;     }

// 座標クランプ付きピクセル参照
template <typename T>
inline T* SubPixT(
	T* data,
	A_long xL, A_long yL,
	A_long width, A_long height, A_long widthtrue
)
{
	A_long x2 = AE_CLAMP(xL, 0, width  - 1);
	A_long y2 = AE_CLAMP(yL, 0, height - 1);
	return (data + y2 * widthtrue + x2);
}

// バイリニア書き込み (スクリーン合成、r/g/b/a は 0-1 正規化)
template <typename T>
inline void SetPixT(
	T* data,
	float r, float g, float b, float a,
	float x, float y,
	A_long width, A_long height, A_long widthtrue
)
{
	const float mc = GetMaxChan<T>();
	A_long x0 = (A_long)x,  x1 = x0 + 1;
	A_long y0 = (A_long)y,  y1 = y0 + 1;
	float dx1 = x - (float)x0,  dx0 = 1.0f - dx1;
	float dy1 = y - (float)y0,  dy0 = 1.0f - dy1;

	// スクリーン合成: D + S - D*S  (正規化空間で計算)
	auto screenBlend = [&](T* p, float sr, float sg, float sb, float sa) {
		float dr = (float)p->red   / mc;
		float dg = (float)p->green / mc;
		float db = (float)p->blue  / mc;
		float da = (float)p->alpha / mc;
		p->red   = static_cast<decltype(p->red)>  (AE_CLAMP((dr + sr - dr * sr) * mc, 0.0f, mc));
		p->green = static_cast<decltype(p->green)>(AE_CLAMP((dg + sg - dg * sg) * mc, 0.0f, mc));
		p->blue  = static_cast<decltype(p->blue)> (AE_CLAMP((db + sb - db * sb) * mc, 0.0f, mc));
		p->alpha = static_cast<decltype(p->alpha)>(AE_CLAMP((da + sa - da * sa) * mc, 0.0f, mc));
	};

	if (x0 >= 0 && x0 < width  && y0 >= 0 && y0 < height)
		screenBlend(data + y0 * widthtrue + x0, r*dx0*dy0, g*dx0*dy0, b*dx0*dy0, a*dx0*dy0);
	if (x1 >= 0 && x1 < width  && y0 >= 0 && y0 < height)
		screenBlend(data + y0 * widthtrue + x1, r*dx1*dy0, g*dx1*dy0, b*dx1*dy0, a*dx1*dy0);
	if (x0 >= 0 && x0 < width  && y1 >= 0 && y1 < height)
		screenBlend(data + y1 * widthtrue + x0, r*dx0*dy1, g*dx0*dy1, b*dx0*dy1, a*dx0*dy1);
	if (x1 >= 0 && x1 < width  && y1 >= 0 && y1 < height)
		screenBlend(data + y1 * widthtrue + x1, r*dx1*dy1, g*dx1*dy1, b*dx1*dy1, a*dx1*dy1);
}

// ピクセル型 T に対するレンダリング本体
template <typename T>
static PF_Err StarMainT(
	NF_AE* ae,
	std::vector<StarSource>& mask,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	const float mc = GetMaxChan<T>();

	for (A_long ch = 0; ch < 4; ch++) {
		float len = (float)(infoP->lines[ch].len_per * infoP->overall_length);
		float opa = (float)(infoP->lines[ch].int_per * infoP->overall_intensity);
		float k   = (float)infoP->hyperbolic;
		for (float angle_dir : {0.0f, 180.0f}) {
			float rad = (float)((infoP->overall_angle + infoP->lines[ch].angle_offset + angle_dir) * M_PI / 180);
			float dx = (float)sin(rad);
			float dy = (float)-cos(rad);
			for (A_long idx = 0; idx < (A_long)mask.size(); idx++) {
				StarSource& src = mask[idx];
				if (src.brightness <= 0) continue;
				float len2 = len * src.brightness;
				float opa2 = opa * src.brightness;

				float ar = infoP->cross_color.red;
				float ag = infoP->cross_color.green;
				float ab = infoP->cross_color.blue;
				if (infoP->color_mode == 1) {
					T* oP = SubPixT<T>(
						(T*)ae->input->data,
						static_cast<A_long>(src.x),
						static_cast<A_long>(src.y),
						ae->inputInfo.width, ae->inputInfo.height, ae->inputInfo.widthTrue);
					ar = (float)oP->red   / mc;
					ag = (float)oP->green / mc;
					ab = (float)oP->blue  / mc;
				}
				ar *= opa2;
				ag *= opa2;
				ab *= opa2;
				float aa = opa2;

				float curX = static_cast<float>(src.x);
				float curY = static_cast<float>(src.y);
				for (int i = 0; i < static_cast<int>(len2); ++i) {
					float falloff = 1.0f - (static_cast<float>(i) / len2);
					if (curX >= 0 && curX < ae->outputInfo.width && curY >= 0 && curY < ae->outputInfo.height) {
						float ar2 = ar * falloff;
						float ag2 = ag * falloff;
						float ab2 = ab * falloff;
						float aa2 = aa * falloff;
						ar2 = 1.0f - ((1.0f + k) * (1.0f - ar2)) / (1.0f + k * (1.0f - ar2));
						ag2 = 1.0f - ((1.0f + k) * (1.0f - ag2)) / (1.0f + k * (1.0f - ag2));
						ab2 = 1.0f - ((1.0f + k) * (1.0f - ab2)) / (1.0f + k * (1.0f - ab2));
						aa2 = 1.0f - ((1.0f + k) * (1.0f - aa2)) / (1.0f + k * (1.0f - aa2));
						SetPixT<T>(
							(T*)ae->output->data,
							ar2, ag2, ab2, aa2,   // 0-1 正規化のまま渡す
							curX, curY,
							ae->outputInfo.width, ae->outputInfo.height, ae->outputInfo.widthTrue);
					}
					else {
						break;
					}
					curX += dx;
					curY += dy;
				}
				if (err) return err;
			}
		}
	}
	return err;
}

PF_Err StarMain(
	NF_AE* ae,
	std::vector<StarSource> mask,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;
	if (mask.empty()) return PF_Err_NONE;

	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
		err = StarMainT<PF_PixelFloat>(ae, mask, infoP);
		break;
	case PF_PixelFormat_ARGB64:
		err = StarMainT<PF_Pixel16>(ae, mask, infoP);
		break;
	case PF_PixelFormat_ARGB32:
		err = StarMainT<PF_Pixel>(ae, mask, infoP);
		break;
	default:
		break;
	}
	return err;
}