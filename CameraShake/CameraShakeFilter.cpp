#include "CameraShakeFilter.h"

// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}
template <typename T>
inline T SubPix(
	PF_PixelPtr data, 
	A_long xL, A_long yL, 
	A_long width,A_long height,A_long widthtrue,
	A_long mode) 
{
	T retsult = { 0,0,0,0 };
	T* base = reinterpret_cast<T*>(data);
	A_long x2 = xL;
	A_long y2 = yL;
	if (mode == 1) { // タイル
		x2 = (xL % width + width) % width; // 負の値も正しく処理
		y2 = (yL % height + height) % height;
	}
	else if (mode == 2) { // ストレッチ
		x2 = AE_CLAMP(xL, 0, width - 1);
		y2 = AE_CLAMP(yL, 0, height - 1);
	}else { // none
		if (xL < 0 || xL >= width || yL < 0 || yL >= height) {
			return retsult; 
		}
		x2 = xL;
		y2 = yL;
	}
	return *(base + y2 * widthtrue + x2);
}
// --- 5. 共通フィルタテンプレート ---
template <typename T,typename channelType>
static PF_Err CameraShakeT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	CameraShakeInfo* infoP = reinterpret_cast<CameraShakeInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();
	float nx = xL + infoP->offsetX;
	float ny = yL + infoP->offsetY;
	if(infoP->isInt){
		nx = (float)((A_long)(nx + 0.5f));
		ny = (float)((A_long)(ny + 0.5f));
		T px = SubPix<T>(infoP->data, (A_long)nx, (A_long)ny, infoP->width, infoP->height, infoP->widthTrue, infoP->pi->edgeMode);
		*outP = px;
		return PF_Err_NONE;
	}
	A_long x1 = (A_long)floor(nx);
	A_long x2 = x1+1;
	A_long y1 = (A_long)floor(ny);
	A_long y2 = y1 + 1;
	float dx1 = nx - (float)x1;
	float dx2 = 1-dx1;
	float dy1 = ny - (float)y1;
	float dy2 = 1 - dy1;
	T p1 = SubPix<T>(infoP->data, x1, y1, infoP->width, infoP->height, infoP->widthTrue, infoP->pi->edgeMode);
	T p2 = SubPix<T>(infoP->data, x2, y1, infoP->width, infoP->height, infoP->widthTrue, infoP->pi->edgeMode);
	T p3 = SubPix<T>(infoP->data, x1, y2, infoP->width, infoP->height, infoP->widthTrue, infoP->pi->edgeMode);
	T p4 = SubPix<T>(infoP->data, x2, y2, infoP->width, infoP->height, infoP->widthTrue, infoP->pi->edgeMode);

	T result;
	result.alpha = (channelType)AE_CLAMP((p1.alpha * dx2 * dy2 + p2.alpha * dx1 * dy2 + p3.alpha * dx2 * dy1 + p4.alpha * dx1 * dy1),0, max_val);
	result.red = (channelType)AE_CLAMP((p1.red * dx2 * dy2 + p2.red * dx1 * dy2 + p3.red * dx2 * dy1 + p4.red * dx1 * dy1), 0, max_val);
	result.green = (channelType)AE_CLAMP((p1.green * dx2 * dy2 + p2.green * dx1 * dy2 + p3.green * dx2 * dy1 + p4.green * dx1 * dy1), 0, max_val);
	result.blue = (channelType)AE_CLAMP((p1.blue * dx2 * dy2 + p2.blue * dx1 * dy2 + p3.blue * dx2 * dy1 + p4.blue * dx1 * dy1), 0, max_val);

	*outP = result;
	
	return PF_Err_NONE;
}

// 8-bit用
static PF_Err CameraShake8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return CameraShakeT<PF_Pixel8,A_u_char>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err CameraShake16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return CameraShakeT<PF_Pixel16,A_u_short>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err CameraShakeFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return CameraShakeT<PF_PixelFloat,PF_FpShort>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}
template <typename T>
inline T LERP(T a, T b, T t) {
	return a + t * (b - a);
}
inline float StaticRandom(int i) {
	int n = i * 12345; // 適当な大きな素数
	n = (n << 13) ^ n;
	float res = (float)(1.0 - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
	return res;
}

inline float GetSmoothNoise(double t) {
	int i = (int)floor(t);      // 整数部（グリッドの左側）
	float f = (float)(t - i);            // 小数部（グリッド内の位置）

	// Smoothstep: 3t^2 - 2t^3 (加速・減速をつけて滑らかに)
	float u = f * f * (3.0f - 2.0f * f);

	// 左側のランダム値と右側のランダム値を、滑らかな重みuで混ぜる
	return LERP(StaticRandom(i), StaticRandom(i + 1), u);
}
PF_Err CameraShakeExec(
	PF_InData* in_dataP,
	PF_EffectWorld* input,
	PF_EffectWorld* output,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	ParamInfo *pi

)
{
	PF_Err err = PF_Err_NONE;
	CameraShakeInfo info;
	AEFX_CLR_STRUCT(info);
	info.pi = pi;
	info.data = input->data;
	info.width = input->width;
	info.height = input->height;
	PF_FpLong pixelValue = ((PF_FpLong)MIN(output->width, output->height) * pi->camera_value*0.05f);
	pixelValue = pixelValue * (1.0f - pi->camera_valueRand) + hash_float(pi->frame,10,30)* pi->camera_valueRand * pixelValue;
	if (hash_float(pi->frame, 15, 35) < pi->camera_subShake) {
		pixelValue += hash_float(pi->frame, 25, 45) * pixelValue*0.5;
	}
	
	info.isInt = (pixelValue > 10 && pi->hand_Value > 0) ? TRUE : FALSE; // 128以上は整数型で処理する

	if (pixelValue > 0) {
		PF_FpLong angleR = (pi->camera_angleRand - 2 * hash_float(pi->frame, 20, 40) * pi->camera_angleRand) * 45;
		angleR += pi->camera_angle;
		if (hash_float(pi->frame, 30, 50) < pi->camera_subShake) {
			if (hash_float(pi->frame, 40, 60) < 0.5) {
				angleR += 90;
			}
			else {
				angleR -= 90;
			}
		}
		PF_FpLong rad = (angleR + 180 * (pi->frame % 2)) * (PF_PI / 180.0);
		info.offsetX = (float)(pixelValue * -sin(rad));
		info.offsetY = (float)(pixelValue * cos(rad));
		if (info.isInt) {
			info.offsetX = (float)((A_long)(info.offsetX + 0.5f));
			info.offsetY = (float)((A_long)(info.offsetY + 0.5f));
		}
	}

	if(pi->hand_Value >0&&(pi->hand_X_value>0|| pi->hand_Y_value > 0)){
		float shakePixel = (float)((float)MIN(output->width, output->height) * pi->hand_amplitude * 0.05f);
		// XとYで StaticRandom に入る値をずらすのがコツです
		float ax = 0;
		ax += (float)(GetSmoothNoise(pi->sec * pi->hand_X_speed1 * pi->hand_speed + pi->hand_X_seed +pi->hand_seed) * shakePixel * pi->hand_X_value * pi->hand_Value);
		ax += (float)(GetSmoothNoise(pi->sec * pi->hand_X_speed2 * pi->hand_speed + pi->hand_X_seed + 50 +pi->hand_seed) * shakePixel * pi->hand_X_value * pi->hand_Value*0.5);
		float ay = 0;
		ay += (float)(GetSmoothNoise(pi->sec * pi->hand_Y_speed1 * pi->hand_speed + pi->hand_Y_seed + 100 + pi->hand_seed) * shakePixel * pi->hand_Y_value * pi->hand_Value);
		ay += (float)(GetSmoothNoise(pi->sec * pi->hand_Y_speed2 * pi->hand_speed + pi->hand_Y_seed + 150 + pi->hand_seed) * shakePixel * pi->hand_Y_value * pi->hand_Value*0.5);
		info.offsetX += ax;
		info.offsetY += ay;

	}


	switch (pixelFormat) {
	case PF_PixelFormat_ARGB128:
		info.widthTrue = input->rowbytes / sizeof(PF_PixelFloat);
		err = suitesP->IterateFloatSuite2()->iterate(
			in_dataP,
			0,
			output->width,
			input,
			NULL,
			&info,
			CameraShakeFloat,
			output);
		break;
	case PF_PixelFormat_ARGB64:
		info.widthTrue = input->rowbytes / sizeof(PF_Pixel16);
		err = suitesP->Iterate16Suite2()->iterate(
			in_dataP,
			0,
			output->width,
			input,
			NULL,
			&info,
			CameraShake16,
			output);
		break;
	case PF_PixelFormat_ARGB32:
		info.widthTrue = input->rowbytes / sizeof(PF_Pixel);
		err = suitesP->Iterate8Suite2()->iterate(
			in_dataP,
			0,
			output->width,
			input,
			NULL,
			&info,
			CameraShake8,
			output);
		break;
	}
	return err;
}
