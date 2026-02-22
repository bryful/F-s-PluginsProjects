#include "TargetWorld.h"

struct TCInfo {
	A_long		target_count;
	PF_Pixel	target_colors[TARGET_COLOR_MAX];
	std::vector<A_u_char>* target_worlds;
	A_long		width;
	A_long		height;
};
struct BorderInfo {
	std::vector<A_u_char>* src;
	std::vector<A_u_char>* dst;
	A_long		width;
	A_long		height;
	PF_Boolean is_border_direction;
	float		direction_angle;
	float		delta_x;
	float		delta_y;

};

// --- ビット深度ごとの型変換を吸収するヘルパー ---
inline PF_Pixel NF_To8(PF_Pixel c) { return c; }
inline PF_Pixel NF_To8(PF_Pixel16 c) { return NF_Pixel16To8(c); }
inline PF_Pixel NF_To8(PF_PixelFloat c) { return NF_Pixel32To8(c); }

template <typename T> struct ECTraits;

template <> struct ECTraits<PF_Pixel8> {
	typedef A_u_char comp;
	static inline comp half() { return 128; }
	static inline comp max_C() { return 255; }
	//static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 255); }
};

template <> struct ECTraits<PF_Pixel16> {
	typedef A_u_short comp;
	static inline comp half() { return 16384; } // AE 16bitは32768が上限
	static inline comp max_C() { return 32768; }
	//static inline comp clamp(float v) { return (comp)AE_CLAMP(v + 0.5f, 0, 32768); }
};

template <> struct ECTraits<PF_PixelFloat> {
	typedef PF_FpShort comp;
	static inline comp half() { return 0.5f; }
	static inline comp max_C() { return 1.0f; }
	//static inline comp clamp(float v) { return (comp)v; } // Floatは基本クランプ不要
};

//-------------------------------------------------------------------------------------------------
// テンプレート化したピクセルサンプリング関数
//-------------------------------------------------------------------------------------------------
template <typename T>
static PF_Err
TargetWorld_Template(
	void* refcon,
	A_long		xL,
	A_long		yL,
	T* inP,
	T* outP)
{
	PF_Err			err = PF_Err_NONE;
	TCInfo* infoP = reinterpret_cast<TCInfo*>(refcon);
	typedef ECTraits<T> Traits;
	//if (!infoP) return PF_Err_INVALID_CALLBACK_PARAM;
	A_long index = yL * infoP->width + xL;
	if (infoP->target_count<=0) {
		if (inP->alpha > Traits::half()){
			*(infoP->target_worlds->data() + index) = TARGET_AREA;
		}
		else {
			*(infoP->target_worlds->data() + index) = TARGET_NONE;
		}
		return err;
	}

	// オーバーロードされた NF_To8 により、T の型に応じた関数が呼ばれます
	PF_Pixel src = NF_To8(*inP);
	int idx = -1;
	// 置換対象の色が無い場合は処理しない
	for (A_long i = 0; i < infoP->target_count; i++) {
		if (
			src.red == infoP->target_colors[i].red &&
			src.green == infoP->target_colors[i].green &&
			src.blue == infoP->target_colors[i].blue
			) {
			idx = i;
			break;
		}
	}
	

	if (idx >= 0) {
		*(infoP->target_worlds->data() + index) = TARGET_AREA;
	}

	else
	{
		*(infoP->target_worlds->data() + index) = TARGET_NONE;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err TargetWorldExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long count,
	PF_Pixel* colors,
	std::vector<A_u_char>* target_worlds
)
{
	PF_Err err = PF_Err_NONE;
	TCInfo info;
	AEFX_CLR_STRUCT(info);
	info.target_count = count;
	if (count > 0) {
		for (int i = 0; i < count; i++)
		{
			info.target_colors[i] = colors[i];
		}
	}
	info.target_worlds = target_worlds;
	info.width = input->width;
	info.height = input->height;



	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
	{
		suitesP->IterateFloatSuite2()->iterate(
			in_dataP,
			0,
			input->height,
			input,
			NULL,
			&info,
			TargetWorld_Template<PF_PixelFloat>,
			input);
	}
	break;

	case PF_PixelFormat_ARGB64:
	{
		suitesP->Iterate16Suite2()->iterate(
			in_dataP,
			0,
			input->height,
			input,
			NULL,
			&info,
			TargetWorld_Template<PF_Pixel16>,
			input);
	}
	break;

	case PF_PixelFormat_ARGB32:
	{
		suitesP->Iterate8Suite2()->iterate(
			in_dataP,
			0,
			input->height,
			input,
			NULL,
			&info,
			TargetWorld_Template<PF_Pixel>,
			input);
	}
	break;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
// テンプレート化したピクセルサンプリング関数
//-------------------------------------------------------------------------------------------------
template <typename T>
static PF_Err
FromTargetWorld_Template(
	void* refcon,
	A_long		xL,
	A_long		yL,
	T* inP,
	T* outP)
{
	PF_Err			err = PF_Err_NONE;
	TCInfo* infoP = reinterpret_cast<TCInfo*>(refcon);
	typedef ECTraits<T> Traits;
	//if (!infoP) return PF_Err_INVALID_CALLBACK_PARAM;
	A_long index = yL * infoP->width + xL;

	A_u_char v = *(infoP->target_worlds->data() + index);

	if (v == TARGET_AREA) {
		outP->red = Traits::max_C();
		outP->green = Traits::max_C();
		outP->blue = Traits::max_C();
		outP->alpha = Traits::max_C();
	}
	else if (v == TARGET_BORDER) {
		outP->red = Traits::half();
		outP->green = Traits::half();
		outP->blue = Traits::half();
		outP->alpha = Traits::max_C();
	}
	else {
		outP->red = 0;
		outP->green = 0;
		outP->blue = 0;
		outP->alpha = Traits::max_C();;
	}
	
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err FromTargetWorldExec(
	PF_InData* in_dataP,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	std::vector<A_u_char>* target_worlds
)
{
	PF_Err err = PF_Err_NONE;
	TCInfo info;
	AEFX_CLR_STRUCT(info);
	
	info.target_worlds = target_worlds;
	info.width = output->width;
	info.height = output->height;

	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
	{
		suitesP->IterateFloatSuite2()->iterate(
			in_dataP,
			0,
			output->height,
			output,
			NULL,
			&info,
			FromTargetWorld_Template<PF_PixelFloat>,
			output);
	}
	break;

	case PF_PixelFormat_ARGB64:
	{
		suitesP->Iterate16Suite2()->iterate(
			in_dataP,
			0,
			output->height,
			output,
			NULL,
			&info,
			FromTargetWorld_Template<PF_Pixel16>,
			output);
	}
	break;

	case PF_PixelFormat_ARGB32:
	{
		suitesP->Iterate8Suite2()->iterate(
			in_dataP,
			0,
			output->height,
			output,
			NULL,
			&info,
			FromTargetWorld_Template<PF_Pixel>,
			output);
	}
	break;
	}
	return err;
}
// *************************************************************************************************
// ****************************************************************************************************
static PF_Err BorderFunc(
	void* refconPV, 
	A_long thread_idxL, 
	A_long y, 
	A_long itrtL)
{
	PF_Err err = PF_Err_NONE;
	BorderInfo* infoP = static_cast<BorderInfo*>(refconPV);
	if(infoP->is_border_direction==TRUE)
	{
		for (A_long x = 0; x < infoP->width - 1; x++) {
			A_long index = y * infoP->width + x;
			*(infoP->dst->data() + index) = *(infoP->src->data() + index);
		}
		A_long yy = AE_CLAMP((A_long)(y + infoP->delta_y + 0.5), 0, infoP->height - 1);
		for (A_long x = 0; x < infoP->width - 1; x++)
		{
			A_long indexDst = y * infoP->width + x;
			if (*(infoP->src->data() + indexDst) == TARGET_NONE) {
				continue;
			}
			A_long xx = AE_CLAMP((A_long)(x +infoP->delta_x+0.5),0,infoP->width-1);
			A_long indexSrc = yy * infoP->width + xx;
			if (*(infoP->src->data() + indexSrc) == TARGET_NONE) {
				*(infoP->dst->data() + indexDst) = TARGET_BORDER;
			}
		}
		return PF_Err_NONE;
	}
	//とりあえず全て境界に
	for (A_long x = 1; x < infoP->width-1; x++) {
		A_long index = y * infoP->width + x;
		if (*(infoP->src->data() + index) == TARGET_AREA) {
			*(infoP->dst->data() + index) = TARGET_BORDER;
		}
		else {
			*(infoP->dst->data() + index) = TARGET_AREA;
		}
	}
	if (y>0 && y<infoP->height-1)
	{
		for (A_long x = 1; x < infoP->width - 1; x++) {
			A_long index = y * infoP->width + x;
			A_u_char v0 = *(infoP->src->data() + index);
			if (v0 == TARGET_NONE) {
				*(infoP->dst->data() + index) = TARGET_NONE;
				continue;
			}
			A_u_char v1 = *(infoP->src->data() + index - 1);
			A_u_char v2 = *(infoP->src->data() + index + 1);
			A_u_char v3 = *(infoP->src->data() + index - infoP->width);
			A_u_char v4 = *(infoP->src->data() + index + infoP->width);
			if (v0 == v1 && v0 == v2 && v0 == v3 && v0 == v4) {
				*(infoP->dst->data() + index) = TARGET_AREA;
			}
		}
	}
	return PF_Err_NONE;
}

PF_Err BorderExec(
	PF_InData* in_dataP,
	AEGP_SuiteHandler* suitesP,
	std::vector<A_u_char>* src,
	std::vector<A_u_char>* dst,
	A_long		width,
	A_long		height,
	PF_Boolean is_border_direction,
	float		direction_angle
)
{
	PF_Err err = PF_Err_NONE;

	BorderInfo info;
	info.src = src;
	info.dst = dst;
	info.width = width;
	info.height = height;
	info.is_border_direction = is_border_direction;
	info.direction_angle = direction_angle;
	float length = 1.5f; // 境界の幅（ピクセル単位）
	float rad = (float)((direction_angle) * PF_PI / 180);
	info.delta_x = (float)sin(rad) * length;
	info.delta_y = (float)(-cos(rad) * length);

	ERR(suitesP->Iterate8Suite2()->iterate_generic(
		info.height,     // iterationsL: 実行回数（＝行数）
		&info,                  // refconPV: ユーザー定義データ
		BorderFunc              // fn_func: コールバック関数
	));
	
	return err;
}
