#include "RimFill.h"
#include <vector>
#include <type_traits>

#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))

//-----------------------------------------------------------------------------------
// テンプレート用ヘルパー関数
//-----------------------------------------------------------------------------------

template<typename T>
inline bool IsWhitePixel(const T& p, double max_val) {
	if constexpr (std::is_same_v<T, PF_PixelFloat>) {
		return (p.red >= 1.0f && p.green >= 1.0f && p.blue >= 1.0f);
	}
	else {
		A_u_short threshold = static_cast<A_u_short>(max_val);
		return (p.red >= threshold && p.green >= threshold && p.blue >= threshold);
	}
}

template<typename T>
inline T GetTransparentPixel() {
	if constexpr (std::is_same_v<T, PF_PixelFloat>) return { 0.0f, 0.0f, 0.0f, 0.0f };
	return { 0, 0, 0, 0 };
}

//-----------------------------------------------------------------------------------
// コアロジック（テンプレート）
//-----------------------------------------------------------------------------------

template<typename T, int MAX_VAL_INT>
static PF_Err RimFillH(void* refconPV, A_long thread_idxL, A_long y, A_long itrtL) {
	PF_Err err = PF_Err_NONE;
	ParamInfo* infoP = static_cast<ParamInfo*>(refconPV);
	A_long bw = infoP->lineWidth;
	A_long w = infoP->dst->width;

	T* dst = (T*)((A_u_char*)infoP->dst->data + y * infoP->dst->rowbytes);

	std::vector<T> line_buf(w);
	std::vector<A_char> lineD(w, 0);
	std::vector<A_char> lineT(w, 0);

	// アルファ最大値の定義（ピクセル型のalpha型と完全に一致させる）
	using AlphaType = decltype(std::declval<T>().alpha);
	const AlphaType ALPHA_MAX = (std::is_same_v<T, PF_PixelFloat> ? AlphaType(1.0f) : AlphaType(MAX_VAL_INT));

	// 1. バッファ複写と事前判定
	for (A_long x = 0; x < w; x++) {
		line_buf[x] = dst[x];
		if (infoP->isWhite && IsWhitePixel(dst[x], static_cast<double>(ALPHA_MAX))) {
			line_buf[x] = GetTransparentPixel<T>();
			//dst[x].alpha = 0;
		}
		lineT[x] = (line_buf[x].alpha == 0) ? 1 : 0;
	}

	// 2. 境界検出
	for (A_long x = 0; x < w - 1; x++) {
		if (line_buf[x].alpha != ALPHA_MAX && line_buf[x + 1].alpha == ALPHA_MAX) lineD[x] = 1;
	}
	for (A_long x = w - 1; x > 0; x--) {
		if (line_buf[x].alpha != ALPHA_MAX && line_buf[x - 1].alpha == ALPHA_MAX) lineD[x] = 2;
	}

	// 3. 塗りつぶし
	T customCol;
	if constexpr (std::is_same_v<T, PF_Pixel8>) customCol = infoP->customColor;
	else if constexpr (std::is_same_v<T, PF_Pixel16>) customCol = infoP->customColor16;
	else customCol = infoP->customColor32;

	for (A_long x = 0; x < w; x++) {
		if (lineD[x] == 0) continue;

		T col = (infoP->isCustomColor) ? customCol : (lineD[x] == 1 ? line_buf[x + 1] : line_buf[x - 1]);
		col.alpha = ALPHA_MAX;

		if (lineD[x] == 1) {
			A_long xl = AE_CLAMP(x - bw, 0, w - 1);
			for (A_long i = x; i >= xl; i--) {
				if (lineT[i] == 0) break;
				dst[i] = col;
			}
		}
		else if (lineD[x] == 2) {
			A_long xr = AE_CLAMP(x + bw, 0, w - 1);
			for (A_long i = x; i <= xr; i++) {
				if (lineT[i] == 0) break;
				dst[i] = col;
			}
		}
	}
	return err;
}

template<typename T, int MAX_VAL_INT>
static PF_Err RimFillV(void* refconPV, A_long thread_idxL, A_long x, A_long itrtL) {
	PF_Err err = PF_Err_NONE;
	ParamInfo* infoP = static_cast<ParamInfo*>(refconPV);
	A_long bw = infoP->lineWidth;
	A_long h = infoP->dst->height;
	A_long wt = infoP->dst->rowbytes / sizeof(T);

	T* dst = (T*)infoP->dst->data + x;

	std::vector<T> line_buf(h);
	std::vector<A_char> lineD(h, 0);
	std::vector<A_char> lineT(h, 0);

	// アルファ最大値の定義（ピクセル型のalpha型と完全に一致させる）
	using AlphaType = decltype(std::declval<T>().alpha);
	const AlphaType ALPHA_MAX = (std::is_same_v<T, PF_PixelFloat> ? AlphaType(1.0f) : AlphaType(MAX_VAL_INT));

	for (A_long i = 0; i < h; i++) {
		line_buf[i] = dst[i * wt];
		if (infoP->isWhite && IsWhitePixel(line_buf[i], static_cast<double>(ALPHA_MAX))) {
			line_buf[i] = GetTransparentPixel<T>();
			//dst[i * wt].alpha = 0;
		}
		lineT[i] = (line_buf[i].alpha == 0) ? 1 : 0;
	}

	for (A_long i = 0; i < h - 1; i++) {
		if (line_buf[i].alpha != ALPHA_MAX && line_buf[i + 1].alpha == ALPHA_MAX) lineD[i] = 1;
	}
	for (A_long i = h - 1; i > 0; i--) {
		if (line_buf[i].alpha != ALPHA_MAX && line_buf[i - 1].alpha == ALPHA_MAX) lineD[i] = 2;
	}

	T customCol;
	if constexpr (std::is_same_v<T, PF_Pixel8>) customCol = infoP->customColor;
	else if constexpr (std::is_same_v<T, PF_Pixel16>) customCol = infoP->customColor16;
	else customCol = infoP->customColor32;

	for (A_long y = 0; y < h; y++) {
		if (lineD[y] == 0) continue;

		T col = (infoP->isCustomColor) ? customCol : (lineD[y] == 1 ? line_buf[y + 1] : line_buf[y - 1]);
		col.alpha = ALPHA_MAX;

		if (lineD[y] == 1) {
			A_long yt = AE_CLAMP(y - bw, 0, h - 1);
			for (A_long i = y; i >= yt; i--) {
				if (lineT[i] == 0) break;
				dst[i * wt] = col;
			}
		}
		else if (lineD[y] == 2) {
			A_long yb = AE_CLAMP(y + bw, 0, h - 1);
			for (A_long i = y; i <= yb; i++) {
				if (lineT[i] == 0) break;
				dst[i * wt] = col;
			}
		}
	}
	return err;
}

//-----------------------------------------------------------------------------------
// ヘルパー関数（iterate_generic用）
//-----------------------------------------------------------------------------------

static PF_Err RimFillH8(void* r, A_long t, A_long y, A_long i) { return RimFillH<PF_Pixel8, PF_MAX_CHAN8>(r, t, y, i); }
static PF_Err RimFillV8(void* r, A_long t, A_long x, A_long i) { return RimFillV<PF_Pixel8, PF_MAX_CHAN8>(r, t, x, i); }

static PF_Err RimFillH16(void* r, A_long t, A_long y, A_long i) { return RimFillH<PF_Pixel16, PF_MAX_CHAN16>(r, t, y, i); }
static PF_Err RimFillV16(void* r, A_long t, A_long x, A_long i) { return RimFillV<PF_Pixel16, PF_MAX_CHAN16>(r, t, x, i); }

static PF_Err RimFillH32(void* r, A_long t, A_long y, A_long i) { return RimFillH<PF_PixelFloat, 1>(r, t, y, i); }
static PF_Err RimFillV32(void* r, A_long t, A_long x, A_long i) { return RimFillV<PF_PixelFloat, 1>(r, t, x, i); }

//-----------------------------------------------------------------------------------
// 実装メイン
//-----------------------------------------------------------------------------------

static PF_Err RimFill_SubImpl(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	if (infoP->lineWidth == 0) return err;

	infoP->dst = ae->output;
	infoP->in_data = ae->in_data;

	AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scope(
		ae->in_data,
		kPFIterate8Suite,
		kPFIterate8SuiteVersion1,
		ae->out_data
	);

	switch (ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB32:
		ERR(iter_scope->iterate_generic(ae->output->height, infoP, RimFillH8));
		ERR(iter_scope->iterate_generic(ae->output->width, infoP, RimFillV8));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(iter_scope->iterate_generic(ae->output->height, infoP, RimFillH16));
		ERR(iter_scope->iterate_generic(ae->output->width,infoP, RimFillV16));
		break;
	case PF_PixelFormat_ARGB128:
		ERR(iter_scope->iterate_generic(ae->output->height, infoP, RimFillH32));
		ERR(iter_scope->iterate_generic(ae->output->width, infoP,RimFillV32));
		break;
	default:
		err = PF_Err_BAD_CALLBACK_PARAM;
		break;
	}

	return err;
}

PF_Err RimFill_Sub(CFsAE* ae, ParamInfo* infoP)
{
	return RimFill_SubImpl(ae, infoP);
}