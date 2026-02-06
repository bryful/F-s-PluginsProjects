#include "SelectiveColorBlur.h"

// ----------------------------------------------------------------------------
// 合成用コンテキスト：各ビット深度共通
template<typename T>
struct CombineContext {
	T* in_data;
	T* out_data;
	A_long width;
	A_long rowbytes; // ピクセル単位のストライド
};

// ----------------------------------------------------------------------------
// 合成カーネル：Iterate8Suite2準拠
// 引数順序：refcon, thread_index, current_index, iterations
template<typename T>
static PF_Err Combine_Kernel(
	void* refconPV,
	A_long thread_indexL,
	A_long i,
	A_long iterationsL)
{
	CombineContext<T>* ctx = reinterpret_cast<CombineContext<T>*>(refconPV);
	if (!ctx) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	A_long y = i; // 第3引数を現在の行インデックスとして使用
	T* in_ptr = ctx->in_data + (y * ctx->rowbytes);
	T* out_ptr = ctx->out_data + (y * ctx->rowbytes);

	for (A_long x = 0; x < ctx->width; ++x) {
		// outputのアルファが0の部分（ブラー処理されなかった背景）にinputを書き戻す
		if (out_ptr[x].alpha == 0) {
			out_ptr[x] = in_ptr[x];
		}
	}
	return PF_Err_NONE;
}

// ----------------------------------------------------------------------------
template<typename T>
static PF_Err ExecCombine(NF_AE* ae) {
	CombineContext<T> ctx;
	ctx.in_data = reinterpret_cast<T*>(ae->input->data);
	ctx.out_data = reinterpret_cast<T*>(ae->output->data);
	ctx.width = ae->output->width;
	ctx.rowbytes = ae->outputInfo.widthTrue;

	return ae->suitesP->Iterate8Suite2()->iterate_generic(
		ae->output->height,
		&ctx,
		Combine_Kernel<T>);
}

// ----------------------------------------------------------------------------
// 外部公開エントリポイント
PF_Err CombineInput(NF_AE* ae) {
	if (!ae || !ae->input->data || !ae->output->data) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB32:  return ExecCombine<PF_Pixel8>(ae);
	case PF_PixelFormat_ARGB64:  return ExecCombine<PF_Pixel16>(ae);
	case PF_PixelFormat_ARGB128: return ExecCombine<PF_PixelFloat>(ae);
	default: return PF_Err_UNRECOGNIZED_PARAM_TYPE;
	}
}