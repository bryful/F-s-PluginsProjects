#include "SelectiveColorBlur.h"

// ----------------------------------------------------------------------------
// ピクセル型ごとの特性を定義するテンプレートトレイト
template<typename T> struct PixelTraits;

template<> struct PixelTraits<PF_Pixel8> {
	typedef A_u_char ChannelType;
	static constexpr A_u_char MAX_VAL = 255;
	static inline ChannelType Round(double val) {
		A_long v = static_cast<A_long>(val + 0.5);
		if (v < 0) v = 0; else if (v > 255) v = 255;
		return static_cast<ChannelType>(v);
	}
};

template<> struct PixelTraits<PF_Pixel16> {
	typedef A_u_short ChannelType;
	static constexpr A_u_short MAX_VAL = 32768;
	static inline ChannelType Round(double val) {
		A_long v = static_cast<A_long>(val + 0.5);
		if (v < 0) v = 0; else if (v > 32768) v = 32768;
		return static_cast<ChannelType>(v);
	}
};

template<> struct PixelTraits<PF_PixelFloat> {
	typedef PF_FpShort ChannelType;
	static constexpr PF_FpShort MAX_VAL = 1.0f;
	static inline ChannelType Round(double val) {
		return static_cast<ChannelType>(val);
	}
};

// ----------------------------------------------------------------------------
template<typename T>
struct BlurContext {
	T* data;
	A_long width;
	A_long height;
	A_long rowbytes;
	A_long blur_value;
	double tbl[1024];
};

// ----------------------------------------------------------------------------
// 水平ブラー：境界判定を「MAX_VAL以外なら打ち切り」に修正
template<typename T>
static PF_Err HorizBlur_Kernel(void* refconPV, A_long thread_indexL, A_long i, A_long iterationsL)
{
	BlurContext<T>* ctx = reinterpret_cast<BlurContext<T>*>(refconPV);
	if (!ctx) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	A_long y = i;
	T* row_ptr = ctx->data + (y * ctx->rowbytes);

	std::vector<T> scanline(ctx->width);
	memcpy(scanline.data(), row_ptr, ctx->width * sizeof(T));

	for (A_long x = 0; x < ctx->width; ++x) {
		// 指定色以外はスルー
		if (scanline[x].alpha != PixelTraits<T>::MAX_VAL) continue;

		double sr = 0.0, sg = 0.0, sb = 0.0, count = 0.0;
		double weight = ctx->tbl[0];

		sr += (double)scanline[x].red * weight;
		sg += (double)scanline[x].green * weight;
		sb += (double)scanline[x].blue * weight;
		count += weight;

		// 左方向
		for (A_long k = 1; k <= ctx->blur_value; ++k) {
			A_long ix = x - k;
			if (ix < 0) break;
			// ターゲット外のピクセルに当たったらサンプリングを打ち切る
			if (scanline[ix].alpha != PixelTraits<T>::MAX_VAL) break;

			weight = ctx->tbl[k];
			sr += (double)scanline[ix].red * weight;
			sg += (double)scanline[ix].green * weight;
			sb += (double)scanline[ix].blue * weight;
			count += weight;
		}
		// 右方向
		for (A_long k = 1; k <= ctx->blur_value; ++k) {
			A_long ix = x + k;
			if (ix >= ctx->width) break;
			// ターゲット外のピクセルに当たったらサンプリングを打ち切る
			if (scanline[ix].alpha != PixelTraits<T>::MAX_VAL) break;

			weight = ctx->tbl[k];
			sr += (double)scanline[ix].red * weight;
			sg += (double)scanline[ix].green * weight;
			sb += (double)scanline[ix].blue * weight;
			count += weight;
		}
		if (count > 0.0) {
			row_ptr[x].red = PixelTraits<T>::Round(sr / count);
			row_ptr[x].green = PixelTraits<T>::Round(sg / count);
			row_ptr[x].blue = PixelTraits<T>::Round(sb / count);
		}
	}
	return PF_Err_NONE;
}

// ----------------------------------------------------------------------------
// 垂直ブラー：境界判定を「MAX_VAL以外なら打ち切り」に修正
template<typename T>
static PF_Err VerticalBlur_Kernel(void* refconPV, A_long thread_indexL, A_long i, A_long iterationsL)
{
	BlurContext<T>* ctx = reinterpret_cast<BlurContext<T>*>(refconPV);
	if (!ctx) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	A_long x = i;
	std::vector<T> scanline(ctx->height);
	for (A_long y = 0; y < ctx->height; ++y) {
		scanline[y] = ctx->data[y * ctx->rowbytes + x];
	}

	for (A_long y = 0; y < ctx->height; ++y) {
		// 指定色以外はスルー
		if (scanline[y].alpha != PixelTraits<T>::MAX_VAL) continue;

		double sr = 0.0, sg = 0.0, sb = 0.0, count = 0.0;
		double weight = ctx->tbl[0];

		sr += (double)scanline[y].red * weight;
		sg += (double)scanline[y].green * weight;
		sb += (double)scanline[y].blue * weight;
		count += weight;

		// 上方向
		for (A_long k = 1; k <= ctx->blur_value; ++k) {
			A_long iy = y - k;
			if (iy < 0) break;
			// ターゲット外のピクセルに当たったらサンプリングを打ち切る
			if (scanline[iy].alpha != PixelTraits<T>::MAX_VAL) break;

			weight = ctx->tbl[k];
			sr += (double)scanline[iy].red * weight;
			sg += (double)scanline[iy].green * weight;
			sb += (double)scanline[iy].blue * weight;
			count += weight;
		}
		// 下方向
		for (A_long k = 1; k <= ctx->blur_value; ++k) {
			A_long iy = y + k;
			if (iy >= ctx->height) break;
			// ターゲット外のピクセルに当たったらサンプリングを打ち切る
			if (scanline[iy].alpha != PixelTraits<T>::MAX_VAL) break;

			weight = ctx->tbl[k];
			sr += (double)scanline[iy].red * weight;
			sg += (double)scanline[iy].green * weight;
			sb += (double)scanline[iy].blue * weight;
			count += weight;
		}
		if (count > 0.0) {
			T* target = ctx->data + (y * ctx->rowbytes + x);
			target->red = PixelTraits<T>::Round(sr / count);
			target->green = PixelTraits<T>::Round(sg / count);
			target->blue = PixelTraits<T>::Round(sb / count);
		}
	}
	return PF_Err_NONE;
}

// ----------------------------------------------------------------------------
template<typename T>
static PF_Err ExecBlur(blurPrm* prm) {
	PF_Err err = PF_Err_NONE;
	if (prm->blur_value <= 0) return err;

	BlurContext<T> ctx;
	ctx.data = reinterpret_cast<T*>(prm->data);
	ctx.width = prm->width; ctx.height = prm->height;
	ctx.rowbytes = prm->rowbytes; ctx.blur_value = prm->blur_value;

	double zone = static_cast<double>(ctx.blur_value) / 3.0;
	for (A_long k = 0; k <= ctx.blur_value; k++) {
		ctx.tbl[k] = exp(-(double)k * k / (2.0 * zone * zone));
	}

	ERR(prm->suitesP->Iterate8Suite2()->iterate_generic(ctx.height, &ctx, HorizBlur_Kernel<T>));
	ERR(prm->suitesP->Iterate8Suite2()->iterate_generic(ctx.width, &ctx, VerticalBlur_Kernel<T>));
	ctx.blur_value /= 4;
	if (ctx.blur_value > 0) {
		ERR(prm->suitesP->Iterate8Suite2()->iterate_generic(ctx.height, &ctx, HorizBlur_Kernel<T>));
	}
	ctx.blur_value /= 4;
	if (ctx.blur_value > 0) {
		ERR(prm->suitesP->Iterate8Suite2()->iterate_generic(ctx.width, &ctx, VerticalBlur_Kernel<T>));
	}
	ctx.blur_value /= 4;
	if (ctx.blur_value > 0) {
		ERR(prm->suitesP->Iterate8Suite2()->iterate_generic(ctx.height, &ctx, HorizBlur_Kernel<T>));
	}

	return err;
}

// ----------------------------------------------------------------------------
PF_Err BlurSub(blurPrm* prm) {
	if (!prm || !prm->data) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	switch (prm->pixelFormat) {
	case PF_PixelFormat_ARGB32:  return ExecBlur<PF_Pixel8>(prm);
	case PF_PixelFormat_ARGB64:  return ExecBlur<PF_Pixel16>(prm);
	case PF_PixelFormat_ARGB128: return ExecBlur<PF_PixelFloat>(prm);
	default: return PF_Err_UNRECOGNIZED_PARAM_TYPE;
	}
}