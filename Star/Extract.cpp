#include "Star.h"
typedef struct {
	PF_EffectWorld* world;
	A_long width;
	A_long widthTrue;
	A_long height;
	A_long rowbytes;
	std::vector<std::vector<float>>* mask;
	A_long mode;
} MaskInfo;
typedef struct {
	A_long width;
	A_long height;
	std::vector<std::vector<float>>* src;
	std::vector<std::vector<float>>* dst;
} MaskFInfo;
// 輝度計算ヘルパー (RGBの最大値を基準にする仕様)
template <typename T>
inline PF_FpLong GetLuminance(T* p, A_long mode,PF_FpLong max_chan) {
	PF_FpLong ret = 0;
	if (mode == 1) { // RGBの最大値を基準にする
		PF_FpLong a = (PF_FpLong)p->alpha / max_chan;
		PF_FpLong r = a * (PF_FpLong)p->red / max_chan;
		PF_FpLong g = a * (PF_FpLong)p->green / max_chan;
		PF_FpLong b = a * (PF_FpLong)p->blue / max_chan;
		ret = MAX(MAX(r, g), b);
	}
	else { // 輝度計算式を使用する
		PF_FpLong a = (PF_FpLong)p->alpha / max_chan;
		PF_FpLong r = (PF_FpLong)p->red / max_chan;
		PF_FpLong g = (PF_FpLong)p->green / max_chan;
		PF_FpLong b = (PF_FpLong)p->blue / max_chan;
		ret =  (0.2126 * r + 0.7152 * g + 0.0722 * b)*a;
	}
	ret = AE_CLAMP( (ret-0.1f) / 0.8f,0,1.0f); // 0.9で割って全体的に明るくする
	return ret;
}

// ピクセルごとの抽出処理
template <typename T>
static PF_Err ExtractMaskT(
		void* refconPV,
		A_long thread_idxL,
		A_long y,
		A_long itrtL)
{
	MaskInfo* infoP = reinterpret_cast<MaskInfo*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用

	T* inRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));


	for(A_long x = 0; x < infoP->width; x++) {
		T* inP = &inRow[x];
		PF_FpLong luma = GetLuminance<T>(inP, infoP->mode,max_chan);
		//(*infoP->mask->data() + y*infoP->width +x ) = (float)luma; // 輝度をマスクに保存
		//*(infoP->mask->data() + y * infoP->width + x) = (float)luma; // 輝度をマスクに保存
		infoP->mask->at(y).at(x) = (float)luma; // 輝度をマスクに保存
	}
	
	return PF_Err_NONE;
}

PF_Err ExtractMask(
	PF_InData* in_dataP,
	PF_EffectWorld* inputP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask,
	A_long mode
)
{
	PF_Err err = PF_Err_NONE;
	
	MaskInfo info;
	AEFX_CLR_STRUCT(info);
	info.width = inputP->width;
	info.height = inputP->height;
	info.rowbytes = inputP->rowbytes;
	info.world = inputP;
	info.mask = mask;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		info.widthTrue = inputP->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic( inputP->height,  &info, ExtractMaskT<PF_PixelFloat>));
		break;
	case PF_PixelFormat_ARGB64:
		info.widthTrue = inputP->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(inputP->height, &info, ExtractMaskT<PF_Pixel16>));
		break;
	case PF_PixelFormat_ARGB32:
		info.widthTrue = inputP->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(inputP->height, &info, ExtractMaskT<PF_Pixel>));
		break;
	}
	return err;
}

template <typename T>
static PF_Err DrawMaskT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	MaskInfo* infoP = reinterpret_cast<MaskInfo*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用


	T* outRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));
	for (A_long x = 0; x < infoP->width; x++) {
		//float luma = *(infoP->mask->data() + y * infoP->width + x); // マスクから輝度を取得
		float luma = (float)infoP->mask->at(y).at(x); // マスクから輝度を取得
		T* outP = &outRow[x];
		if (std::is_same<T, PF_PixelFloat>::value) {
			// 32bit float の場合はクランプ不要
			outP->red = static_cast<typename PixelTraits<T>::channel_type>(luma);
			outP->green = static_cast<typename PixelTraits<T>::channel_type>(luma);
			outP->blue = static_cast<typename PixelTraits<T>::channel_type>(luma);
		}
		else {
			// 整数型の場合はクランプしてから適切な型にキャスト
			outP->red = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(luma* max_chan, 0, max_chan));
			outP->green = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(luma * max_chan, 0, max_chan));
			outP->blue = static_cast<typename PixelTraits<T>::channel_type>(AE_CLAMP(luma * max_chan, 0, max_chan));
		}
		outP->alpha = static_cast<typename PixelTraits<T>::channel_type>(max_chan);

	}


	



	return PF_Err_NONE;
}
PF_Err DrawMask(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask
)
{
	PF_Err err = PF_Err_NONE;
	MaskInfo info;
	AEFX_CLR_STRUCT(info);
	info.width = output->width;
	info.height = output->height;
	info.rowbytes = output->rowbytes;
	info.world = output;
	info.mask = mask;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		info.widthTrue = output->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_PixelFloat>));
		break;
	case PF_PixelFormat_ARGB64:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_Pixel16>));
		break;
	case PF_PixelFormat_ARGB32:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_Pixel>));
		break;
	}
	return err;
}
static PF_Err ClacMaskT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	MaskFInfo* infoP = reinterpret_cast<MaskFInfo*>(refconPV);
	infoP->dst->at(y).at(0) = infoP->src->at(y).at(0);
	infoP->dst->at(y).at(infoP->width-1) = infoP->src->at(y).at(infoP->width - 1);
	if (y == 0) {
		for(A_long x=1; x< infoP->width-1; x++) {
			float p = infoP->src->at(y).at(x);
			float p1 = infoP->src->at(y).at(x-1);
			float p2 = infoP->src->at(y).at(x + 1);
			float p3 = infoP->src->at(y+1).at(x);
			if (p==p1 && p==p2 && p==p3) {
				infoP->dst->at(y).at(x) = 0;
			}
			else {
				infoP->dst->at(y).at(x) = p;
			}
		}
	}
	else if (y == infoP->height - 1) {
		for (A_long x = 1; x < infoP->width - 1; x++) {
			float p = infoP->src->at(y).at(x);
			if (p == 0) continue;
			float p1 = infoP->src->at(y).at(x - 1);
			float p2 = infoP->src->at(y).at(x + 1);
			float p3 = infoP->src->at(y - 1).at(x);
			if (p == p1 && p == p2 && p == p3) {
				infoP->dst->at(y).at(x) = 0;
			}
			else {
				infoP->dst->at(y).at(x) = p;
			}
		}
	}
	else {
		for (A_long x = 1; x < infoP->width - 1; x++) {
			float p = infoP->src->at(y).at(x);
			if (p == 0) continue;
			float p1 = infoP->src->at(y).at(x - 1);
			float p2 = infoP->src->at(y).at(x + 1);
			float p3 = infoP->src->at(y - 1).at(x);
			float p4 = infoP->src->at(y + 1).at(x);
			if (p == p1 && p == p2 && p == p3 && p==p4) {
				infoP->dst->at(y).at(x) = 0;
			}
			else {
				infoP->dst->at(y).at(x) = p;
			}
		}
	}
	return PF_Err_NONE;
}
std::vector<std::vector<float>> CalcMask(
	AEGP_SuiteHandler* suitesP, 
	std::vector<std::vector<float>>* src
)
{

	PF_Err err = PF_Err_NONE;
	MaskFInfo info;
	AEFX_CLR_STRUCT(info);
	info.src = src;
	info.height = (A_long)src->size();
	info.width = (A_long)src->at(0).size();
	std::vector<std::vector<float>> resultMask(info.height, std::vector<float>(info.width));
	info.dst = &resultMask;
	ERR(suitesP->Iterate8Suite2()->iterate_generic(info.height, &info, ClacMaskT));
	return resultMask;
}