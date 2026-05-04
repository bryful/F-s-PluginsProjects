#include "SimplexNoiseEngine.h"

struct SNInfo {
	NoiseLUT* lut;
	float scale;
};

// --- 5. 共通フィルタテンプレート ---
template <typename T, typename CT,float max_chan>
static PF_Err SimplexNoiseT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	SNInfo* infoP = static_cast<SNInfo*>(refcon);

	float xx = (float)xL * 0.05f * infoP->scale;
	float yy = (float)yL * 0.05f * infoP->scale;

	// 2. ミラータイリング（Ping-Pong）ロジック
	auto MirrorCoord = [](float v) -> float {
		// 0〜510の範囲でループさせる（2倍の周期）
		float m = fmod(fabs(v), 510.0f);
		// 255を超えたら折り返す
		if (m > 255.0f) m = 510.0f - m;
		// 補間時に255+1を参照しても安全なように、わずかに内側にクランプする
		return std::clamp(m, 0.0f, 254.999f);
		};
	float xx2 = MirrorCoord(xx);
	float yy2 = MirrorCoord(yy);

	float v = infoP->lut->GetValueSmooth(xx2,yy2);
	v = v * 0.5f + 0.5f;
	outP->alpha = (CT)max_chan;

	float r = AE_CLAMP(v * max_chan,0,max_chan);
	outP->red =
	outP->green =
	outP->blue = static_cast<CT>(r);


	return PF_Err_NONE;
}

PF_Err SimplexNoise
(
    PF_InData* in_dataP,
    PF_EffectWorld* input,
    PF_EffectWorld* output,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
    float evolution,
    int octaves,
	float scale
	)
{
    PF_Err err = PF_Err_NONE;
	FastSimplexNoise engine;
	NoiseLUT lut;
	// シーケンス開始時にLUTを更新
	lut.Refresh(engine, evolution, octaves);

	SNInfo info;
	info.lut = &lut;
	info.scale = scale;
	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	try {
		switch (pixelFormat)
		{
		case PF_PixelFormat_ARGB128:
			ERR(suitesP->IterateFloatSuite2()->iterate(in_dataP, 0, output->height, input, nullptr, (refconType)&info, SimplexNoiseT<PF_PixelFloat, PF_FpShort, 1.0f>, output));
			break;
		case PF_PixelFormat_ARGB64:
			ERR(suitesP->Iterate16Suite2()->iterate(in_dataP, 0, output->height, input, nullptr, (refconType)&info, SimplexNoiseT<PF_Pixel16, A_u_short, (float)PF_MAX_CHAN16>, output));
			break;
		case PF_PixelFormat_ARGB32:
			ERR(suitesP->Iterate8Suite2()->iterate(in_dataP, 0, output->height, input, nullptr, (refconType)&info, SimplexNoiseT<PF_Pixel, A_u_char, (float)PF_MAX_CHAN8>, output));

			break;
		}
	}
	catch (PF_Err& thrown_err) {
		err = thrown_err;
	}
	return err;
}
