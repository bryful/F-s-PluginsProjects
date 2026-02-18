#include "AlphaFill.h"
struct PixelFloat {
    float r, g, b, a;
};

// チャンネル型を取得するヘルパー
template <typename T>
struct ChannelTypeHelper {
	using type = A_u_char;
};

template <>
struct ChannelTypeHelper<PF_Pixel16> {
	using type = A_u_short;
};

template <>
struct ChannelTypeHelper<PF_PixelFloat> {
	using type = PF_FpShort;
};

/**
 * @brief スピル抑制付きYCbCrキーイング
 * @param pixel      入力ピクセル
 * @param key        キーカラー（RGB）
 * @param thresh     許容値 (0.0 - 1.0)
 * @param soft       ソフトエッジ (0.0 - 1.0)
 * @param spill_amt  スピル抑制の強度 (0.0 - 1.0)
 */
inline PixelFloat ProcessChromaKey(const PixelFloat& pixel, const PixelFloat& key, float thresh, float soft, float spill_amt)
{
    PixelFloat out = pixel;

    // --- 1. アルファ（透過）の計算 ---
    auto get_cb = [](float r, float g, float b) { return -0.1687f * r - 0.3313f * g + 0.5000f * b; };
    auto get_cr = [](float r, float g, float b) { return  0.5000f * r - 0.4187f * g - 0.0813f * b; };

    float p_cb = get_cb(pixel.r, pixel.g, pixel.b);
    float p_cr = get_cr(pixel.r, pixel.g, pixel.b);
    float k_cb = get_cb(key.r, key.g, key.b);
    float k_cr = get_cr(key.r, key.g, key.b);

    float dist = (float)(std::sqrt(std::pow(p_cb - k_cb, 2) + std::pow(p_cr - k_cr, 2)));
    
    // スムーズなマスク作成
    float mask = std::clamp((dist - thresh) / (soft + 0.00001f), 0.0f, 1.0f);
    mask = mask * mask * (3.0f - 2.0f * mask); // Smoothstep
    out.a = mask * pixel.a;

    // --- 2. スピル抑制 (Spill Suppression) ---
    // ここではグリーンバック(G)を想定したロジックの例です
    // キーカラーの中で最も強い成分を特定して動的に切り替えるのが理想的です
    
    // ターゲット成分(G)が他の成分(R, B)の平均より大きい場合、その差を削る
    float other_avg = (pixel.r + pixel.b) * 0.5f;
    
    if (pixel.g > other_avg) {
        // スピル（はみ出し）成分の計算
        float spill = pixel.g - other_avg;
        // 指定した強度で抑制
        out.g -= spill * spill_amt;
    }

    // 注意：スピル抑制をかけると輝度が落ちるため、
    // 削った分を少しだけ他のチャンネルに足して輝度を維持する手法もあります（オプション）

    return out;
}

// *******************************************************************************
// ビット深度ごとの最大値を取得するヘルパー
template <typename T>
inline PF_FpLong GetMaxChannel() {
	if (std::is_same<T, PF_Pixel8>::value) return 255.0;
	if (std::is_same<T, PF_Pixel16>::value) return 32768.0;
	return 1.0; // PF_PixelFloat用
}

// --- 5. 共通フィルタテンプレート ---
template <typename T>
static PF_Err ChromaKeyT(
	refconType refcon,
	A_long xL,
	A_long yL,
	T* inP,
	T* outP)
{
	using ChannelType = typename ChannelTypeHelper<T>::type;
	
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_FpLong max_val = GetMaxChannel<T>();

	if (std::is_same<T, PF_PixelFloat>::value) {
		if (outP->red == infoP->colorFloat.red &&
			outP->green == infoP->colorFloat.green &&
			outP->blue == infoP->colorFloat.blue) {
			*outP = { 0,0,0,0 }; // 完全に透明
			return PF_Err_NONE;
		}
	} else if (std::is_same<T, PF_Pixel16>::value) {
		if (outP->red == infoP->color16.red &&
			outP->green == infoP->color16.green &&
			outP->blue == infoP->color16.blue) {
			*outP = { 0,0,0,0 }; // 完全に透明
			return PF_Err_NONE;
		}
	}
	else if (std::is_same<T, PF_Pixel>::value) {
		if (outP->red == infoP->color.red &&
			outP->green == infoP->color.green &&
			outP->blue == infoP->color.blue) {
			*outP = { 0,0,0,0 }; // 完全に透明
			return PF_Err_NONE;
		}
	}

	PixelFloat src = {
		static_cast<float>(outP->red / max_val) ,
		static_cast<float>(outP->green / max_val) ,
		static_cast<float>(outP->blue / max_val) ,
		static_cast<float>(outP->alpha / max_val)
	};
	
	// PF_PixelFloatをPixelFloatに変換
	PixelFloat key = {
		infoP->colorFloat.red,
		infoP->colorFloat.green,
		infoP->colorFloat.blue,
		infoP->colorFloat.alpha
	};
	
	PixelFloat dst = ProcessChromaKey( 
		src,
		key,
		static_cast<float>(infoP->thresh), 
		static_cast<float>(infoP->soft), 
		static_cast<float>(infoP->Spill_amt));
	
	if (std::is_same<T, PF_PixelFloat>::value) {
		outP->alpha = static_cast<ChannelType>(dst.a);
		outP->red = static_cast<ChannelType>(dst.r);
		outP->green = static_cast<ChannelType>(dst.g);
		outP->blue = static_cast<ChannelType>(dst.b);
	}
	else {
		outP->alpha = static_cast<ChannelType>(AE_CLAMP(dst.a*max_val, 0, max_val));
		outP->red = static_cast<ChannelType>(AE_CLAMP(dst.r * max_val, 0, max_val));
		outP->green = static_cast<ChannelType>(AE_CLAMP(dst.g * max_val, 0, max_val));
		outP->blue = static_cast<ChannelType>(AE_CLAMP(dst.b * max_val, 0, max_val));
	}

	return PF_Err_NONE;
}

// 8-bit用
static PF_Err ChromaKey8(refconType refcon, A_long xL, A_long yL, PF_Pixel* inP, PF_Pixel* outP) {
	return ChromaKeyT<PF_Pixel8>(refcon, xL, yL, reinterpret_cast<PF_Pixel8*>(inP), reinterpret_cast<PF_Pixel8*>(outP));
}

// 16-bit用
static PF_Err ChromaKey16(refconType refcon, A_long xL, A_long yL, PF_Pixel16* inP, PF_Pixel16* outP)
{
	return ChromaKeyT<PF_Pixel16>(refcon, xL, yL, reinterpret_cast<PF_Pixel16*>(inP), reinterpret_cast<PF_Pixel16*>(outP));
}
// 32-bit用 (Float)
static PF_Err ChromaKeyFloat(refconType refcon, A_long xL, A_long yL, PF_PixelFloat* inP, PF_PixelFloat* outP)
{
	return ChromaKeyT<PF_PixelFloat>(refcon, xL, yL, reinterpret_cast<PF_PixelFloat*>(inP), reinterpret_cast<PF_PixelFloat*>(outP));
}

PF_Err ChromaKeyExec(
	NF_AE *ae,
	ParamInfo* infoP
)
{
	PF_Err err = PF_Err_NONE;

	// ピクセルフォーマットに応じてテンプレート関数を呼び出す
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
		ERR(ae->suitesP->IterateFloatSuite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			infoP,
			ChromaKeyFloat,
			ae->output));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->suitesP->Iterate16Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			infoP,
			ChromaKey16,
			ae->output));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->suitesP->Iterate8Suite2()->iterate(
			ae->in_data,
			0,
			ae->output->width,
			ae->input,
			NULL,
			infoP,
			ChromaKey8,
			ae->output));
		break;
	}
	return err;
}
