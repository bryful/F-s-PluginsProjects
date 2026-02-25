#include "ThresholdLine.h"
typedef struct {
	PF_EffectWorld* world;
	A_long width;
	A_long widthTrue;
	A_long height;
	A_long rowbytes;
	PF_Pixel target_color;
	float level;
	std::vector<std::vector<float>>* mask;
} THInfo;

template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}

// ***********************************************************************************
// ***********************************************************************************
template <typename T,typename channelType>
static PF_Err DrawMaskT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	THInfo* infoP = reinterpret_cast<THInfo*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用


	T* outRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));
	for (A_long x = 0; x < infoP->width; x++) {
		//float luma = *(infoP->mask->data() + y * infoP->width + x); // マスクから輝度を取得
		float luma = (float)infoP->mask->at(y).at(x); // マスクから輝度を取得
		T* outP = &outRow[x];
		if (std::is_same<T, PF_PixelFloat>::value) {
			// 32bit float の場合はクランプ不要
			outP->red = static_cast<channelType>(luma);
			outP->green = static_cast<channelType>(luma);
			outP->blue = static_cast<channelType>(luma);
		}
		else {
			// 整数型の場合はクランプしてから適切な型にキャスト
			outP->red = static_cast<channelType>(AE_CLAMP(luma * max_chan, 0, max_chan));
			outP->green = static_cast<channelType>(AE_CLAMP(luma * max_chan, 0, max_chan));
			outP->blue = static_cast<channelType>(AE_CLAMP(luma * max_chan, 0, max_chan));
		}
		outP->alpha = static_cast<channelType>(max_chan);

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
	THInfo info;
	AEFX_CLR_STRUCT(info);
	info.width = output->width;
	info.height = output->height;
	info.rowbytes = output->rowbytes;
	info.world = output;
	info.mask = mask;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		info.widthTrue = output->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_PixelFloat,PF_FpShort>));
		break;
	case PF_PixelFormat_ARGB64:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_Pixel16,A_u_short>));
		break;
	case PF_PixelFormat_ARGB32:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, DrawMaskT<PF_Pixel,A_u_char>));
		break;
	}
	return err;
}
// ***********************************************************************************
// ***********************************************************************************
// 輝度計算ヘルパー (RGBの最大値を基準にする仕様)
template <typename T>
inline float GetLuminance(T* p, PF_FpLong max_chan) {
	PF_FpLong ret = 0;
	PF_FpLong a = (PF_FpLong)p->alpha / max_chan;
	PF_FpLong r = (PF_FpLong)p->red / max_chan;
	PF_FpLong g = (PF_FpLong)p->green / max_chan;
	PF_FpLong b = (PF_FpLong)p->blue / max_chan;
	ret = (0.2126 * r + 0.7152 * g + 0.0722 * b) * a;
	return (float)AE_CLAMP(ret, 0, 1.0f);
}
template <typename T>
static PF_Err ThresholdLumT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	THInfo* infoP = reinterpret_cast<THInfo*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用

	if(y==0|| y== infoP->height-1) {
		for(A_long x=0; x< infoP->width; x++) {
			infoP->mask->at(y).at(x) = 0.0f; // 上下の行は全て0にする
		}
		return PF_Err_NONE;
	}

	// 各行の先頭ポインタをバイト単位で計算してから型キャスト
	T* inRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));
	T* prevRow = reinterpret_cast<T*>((char*)infoP->world->data + ((y - 1) * infoP->rowbytes));
	T* nextRow = reinterpret_cast<T*>((char*)infoP->world->data + ((y + 1) * infoP->rowbytes));
	infoP->mask->at(y).at(0) = 0;
	infoP->mask->at(y).at(infoP->width - 1) = 0;
	for (A_long x = 1; x < infoP->width-1; x++) {
		float cv = GetLuminance<T>(&inRow[x], max_chan);
		// 上下左右の差分（絶対値）
		float v0 = ABS(GetLuminance<T>(&inRow[x - 1], max_chan) - cv); // 左
		float v1 = ABS(GetLuminance<T>(&inRow[x + 1], max_chan) - cv); // 右
		float v2 = ABS(GetLuminance<T>(&prevRow[x], max_chan) - cv);   // 上
		float v3 = ABS(GetLuminance<T>(&nextRow[x], max_chan) - cv);   // 下
		float max_v = MAX(MAX(v0, v1), MAX(v2, v3));

		// 判定結果を 0.0f か 1.0f で代入
		infoP->mask->at(y).at(x) = (max_v > (1-infoP->level)) ? 1.0f : 0.0f;
	}
	//std::string msg = "ThresholdLumT Level: " + std::to_string(infoP->level) + "\n";
	//OutputDebugStringA(msg.c_str());

	return PF_Err_NONE;
}

PF_Err ThresholdLum(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask,
	float level
)
{
	PF_Err err = PF_Err_NONE;
	THInfo info;
	AEFX_CLR_STRUCT(info);
	info.width = output->width;
	info.height = output->height;
	info.rowbytes = output->rowbytes;
	info.world = output;
	info.mask = mask;
	info.level = level;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		info.widthTrue = output->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdLumT<PF_PixelFloat>));
		break;
	case PF_PixelFormat_ARGB64:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdLumT<PF_Pixel16>));
		break;
	case PF_PixelFormat_ARGB32:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdLumT<PF_Pixel>));
		break;
	}
	return err;
}
// ***********************************************************************************
// ***********************************************************************************
template <typename T>
inline float GetAlpha(T* p, PF_FpLong max_chan) {
	return (float)((PF_FpLong)p->alpha / max_chan);
}
template <typename T>
static PF_Err ThresholdAlphaT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	THInfo* infoP = reinterpret_cast<THInfo*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用

	if (y == 0 || y == infoP->height - 1) {
		for (A_long x = 0; x < infoP->width; x++) {
			infoP->mask->at(y).at(x) = 0.0f; // 上下の行は全て0にする
		}
		return PF_Err_NONE;
	}

	// 各行の先頭ポインタをバイト単位で計算してから型キャスト
	T* inRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));
	T* prevRow = reinterpret_cast<T*>((char*)infoP->world->data + ((y - 1) * infoP->rowbytes));
	T* nextRow = reinterpret_cast<T*>((char*)infoP->world->data + ((y + 1) * infoP->rowbytes));

	infoP->mask->at(y).at(0) = 0;
	infoP->mask->at(y).at(infoP->width - 1) = 0;
	for (A_long x = 1; x < infoP->width - 1; x++) {
		float cv = GetAlpha<T>(&inRow[x], max_chan);
		float v0 = ABS(GetAlpha<T>(&inRow[x - 1], max_chan) - cv);
		float v1 = ABS(GetAlpha<T>(&inRow[x + 1], max_chan) - cv);
		float v2 = ABS(GetAlpha<T>(&prevRow[x], max_chan) - cv);
		float v3 = ABS(GetAlpha<T>(&nextRow[x], max_chan) - cv);
		float max_v = MAX(MAX(v0, v1), MAX(v2, v3));
		infoP->mask->at(y).at(x) = (max_v > (1-infoP->level)) ? 1.0f : 0.0f; // 比較結果を代入
		//infoP->mask->at(y).at(x) = max_v;
	}
	return PF_Err_NONE;
}

PF_Err ThresholdAlpha(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask,
	float level
)
{
	PF_Err err = PF_Err_NONE;
	THInfo info;
	AEFX_CLR_STRUCT(info);
	info.width = output->width;
	info.height = output->height;
	info.rowbytes = output->rowbytes;
	info.world = output;
	info.mask = mask;
	info.level = level;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		info.widthTrue = output->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdAlphaT<PF_PixelFloat>));
		break;
	case PF_PixelFormat_ARGB64:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdAlphaT<PF_Pixel16>));
		break;
	case PF_PixelFormat_ARGB32:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdAlphaT<PF_Pixel>));
		break;
	}
	return err;
}
// ***********************************************************************************
// ***********************************************************************************

// --- ビット深度ごとの型変換を吸収するヘルパー ---
inline PF_Pixel NF_To8(PF_Pixel c) { return c; }
inline PF_Pixel NF_To8(PF_Pixel16 c) { return NF_Pixel16To8(c); }
inline PF_Pixel NF_To8(PF_PixelFloat c) { return NF_Pixel32To8(c); }

template <typename T>
static PF_Err ThresholdColorT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	THInfo* infoP = reinterpret_cast<THInfo*>(refconPV);
	PF_FpLong max_chan = GetMaxChannel<T>(); // _SkeletonFilter.cppにあるヘルパーを利用

	T* inRow = reinterpret_cast<T*>((char*)infoP->world->data + (y * infoP->rowbytes));
	for (A_long x = 0; x < infoP->width; x++) {
		PF_Pixel pixel8 = NF_To8(inRow[x]); // ビット深度に応じた型変換を行う
		double r_diff = (ABS((double)pixel8.red - (double)infoP->target_color.red))/255;
		double g_diff = (ABS((double)pixel8.green - (double)infoP->target_color.green)) / 255;
		double b_diff = (ABS((double)pixel8.blue - (double)infoP->target_color.blue)) / 255;
		double d = sqrt(pow(r_diff, 2) + pow(g_diff, 2) + pow(b_diff, 2)); // RGBの差分を距離として計算
		infoP->mask->at(y).at(x) = ((1 - d) > (1 - infoP->level)) ? 1.0f : 0.0f; // 比較結果を代入

	}

	return PF_Err_NONE;
}
PF_Err ThresholdColor(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<std::vector<float>>* mask, 
	PF_Pixel targetColor,
	float level
)
{
	PF_Err err = PF_Err_NONE;
	THInfo info;
	AEFX_CLR_STRUCT(info);
	info.width = output->width;
	info.height = output->height;
	info.rowbytes = output->rowbytes;
	info.world = output;
	info.target_color = targetColor;
	info.mask = mask;
	info.level = level;
	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		info.widthTrue = output->rowbytes / sizeof(PF_PixelFloat);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdColorT<PF_PixelFloat>));
		break;
	case PF_PixelFormat_ARGB64:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel16);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdColorT<PF_Pixel16>));
		break;
	case PF_PixelFormat_ARGB32:
		info.widthTrue = output->rowbytes / sizeof(PF_Pixel);
		ERR(suitesP->Iterate8Suite2()->iterate_generic(output->height, &info, ThresholdColorT<PF_Pixel>));
		break;
	}
	return err;
}
// ************************************************************************************
// ************************************************************************************
typedef struct {
	A_long width;
	A_long height;
	std::vector<std::vector<float>>* src;
	std::vector<std::vector<float>>* dst;
} CalcMaskInfo;
static PF_Err ClacMaskT(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	CalcMaskInfo* infoP = reinterpret_cast<CalcMaskInfo*>(refconPV);
	infoP->dst->at(y).at(0) = infoP->src->at(y).at(0);
	infoP->dst->at(y).at(infoP->width - 1) = infoP->src->at(y).at(infoP->width - 1);
	if (y == 0) {
		for (A_long x = 1; x < infoP->width - 1; x++) {
			float p = infoP->src->at(y).at(x);
			float p1 = infoP->src->at(y).at(x - 1);
			float p2 = infoP->src->at(y).at(x + 1);
			float p3 = infoP->src->at(y + 1).at(x);
			if (p == p1 && p == p2 && p == p3) {
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
			if (p == p1 && p == p2 && p == p3 && p == p4) {
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
	CalcMaskInfo info;
	AEFX_CLR_STRUCT(info);
	info.src = src;
	info.height = (A_long)src->size();
	info.width = (A_long)src->at(0).size();
	std::vector<std::vector<float>> resultMask(info.height, std::vector<float>(info.width));
	info.dst = &resultMask;
	ERR(suitesP->Iterate8Suite2()->iterate_generic(info.height, &info, ClacMaskT));
	return resultMask;
}
// ************************************************************************************
// ************************************************************************************

std::vector<PF_Point> TargetMask(
	std::vector<std::vector<float>>* src,
	double value,
	A_long seed
)
{

	PF_Err err = PF_Err_NONE;
	std::vector<PF_Point> resultMask;
	if (src->size() == 0 || src->at(0).size() == 0) return resultMask;
	resultMask.reserve(src->size() * src->at(0).size());

	for(A_long y=0; y< (A_long)src->size(); y++) {
		for(A_long x=0; x< (A_long)src->at(0).size(); x++) {
			if(src->at(y).at(x) > 0) {
				if (hash_float(x, y, seed) < value) {
					PF_Point p;
					p.h = x;
					p.v = y;
					resultMask.push_back(p);
				}
			}
		}
	}

	return resultMask;
}