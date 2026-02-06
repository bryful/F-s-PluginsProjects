#include "SelectiveColorBlur.h"
#include <type_traits>

// ----------------------------------------------------------------------------
template<typename T> struct PixelTraits;
template<> struct PixelTraits<PF_Pixel8> { static constexpr A_u_char  MAX_VAL = 255; };
template<> struct PixelTraits<PF_Pixel16> { static constexpr A_u_short MAX_VAL = 32768; };
template<> struct PixelTraits<PF_PixelFloat> { static constexpr PF_FpShort MAX_VAL = 1.0f; };

// ----------------------------------------------------------------------------
// 色判定ロジック (NF_Utilsのマクロを使用して厳密に比較)
template<typename T>
static inline bool IsTarget(const T& pix, const PF_Pixel* targets, A_long count) {
	for (A_long i = 0; i < count; ++i) {
		A_long r, g, b;
		if constexpr (std::is_same_v<T, PF_Pixel8>) {
			r = pix.red; g = pix.green; b = pix.blue;
		}
		else if constexpr (std::is_same_v<T, PF_Pixel16>) {
			r = NF_CONVERT16TO8(pix.red); g = NF_CONVERT16TO8(pix.green); b = NF_CONVERT16TO8(pix.blue);
		}
		else if constexpr (std::is_same_v<T, PF_PixelFloat>) {
			r = NF_CONVERT32TO8(pix.red); g = NF_CONVERT32TO8(pix.green); b = NF_CONVERT32TO8(pix.blue);
		}

		// 誤差を許容せず厳密に一致するか判定
		if (r == (A_long)targets[i].red &&
			g == (A_long)targets[i].green &&
			b == (A_long)targets[i].blue) {
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
template<typename T>
struct SelectCopyContext {
	T* in_data;
	T* out_data;
	A_long width;
	A_long rowbytes;
	A_long target_count;
	const PF_Pixel* targets;
};

// Iterate8Suite2 準拠のカーネル (引数4つ：refcon, thread, idx, total)
template<typename T>
static PF_Err SelectCopy_Kernel(void* refconPV, A_long thread_indexL, A_long i, A_long iterationsL)
{
	SelectCopyContext<T>* ctx = reinterpret_cast<SelectCopyContext<T>*>(refconPV);
	if (!ctx) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	A_long y = i; // 第3引数が現在のインデックス(行)
	T* in_ptr = ctx->in_data + (y * ctx->rowbytes);
	T* out_ptr = ctx->out_data + (y * ctx->rowbytes);

	for (A_long x = 0; x < ctx->width; ++x) {
		if (IsTarget<T>(in_ptr[x], ctx->targets, ctx->target_count)) {
			out_ptr[x].red = in_ptr[x].red;
			out_ptr[x].green = in_ptr[x].green;
			out_ptr[x].blue = in_ptr[x].blue;
			out_ptr[x].alpha = PixelTraits<T>::MAX_VAL;
		}
		else {
			out_ptr[x].red = out_ptr[x].green = out_ptr[x].blue = out_ptr[x].alpha = 0;
		}
	}
	return PF_Err_NONE;
}

// ----------------------------------------------------------------------------
template<typename T>
static PF_Err ExecSelectCopy(NF_AE* ae, ParamInfo* infoP) {
	SelectCopyContext<T> ctx;
	ctx.in_data = reinterpret_cast<T*>(ae->input->data);
	ctx.out_data = reinterpret_cast<T*>(ae->output->data);
	ctx.width = ae->output->width;
	ctx.rowbytes = ae->outputInfo.widthTrue;
	ctx.target_count = infoP->target_count;
	ctx.targets = infoP->target_col;

	// Iterate8Suite2 の iterate_generic は引数3つ
	return ae->suitesP->Iterate8Suite2()->iterate_generic(
		ae->output->height,
		&ctx,
		SelectCopy_Kernel<T>);
}

// ----------------------------------------------------------------------------
PF_Err SelectColorCopy(NF_AE* ae, ParamInfo* infoP) {
	if (!ae || !infoP) return PF_Err_INTERNAL_STRUCT_DAMAGED;
	if (infoP->target_count <= 0) return PF_Err_NONE;

	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB32:  return ExecSelectCopy<PF_Pixel8>(ae, infoP);
	case PF_PixelFormat_ARGB64:  return ExecSelectCopy<PF_Pixel16>(ae, infoP);
	case PF_PixelFormat_ARGB128: return ExecSelectCopy<PF_PixelFloat>(ae, infoP);
	default: return PF_Err_UNRECOGNIZED_PARAM_TYPE;
	}
}