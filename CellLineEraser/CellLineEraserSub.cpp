#include "CellLineEraser.h"

// 輝度定数
#define LUME_R 30
#define LUME_G 59
#define LUME_B 11

/* 判定用マクロ（命名規則を厳守） */
#define IS_MARKER(P)      ((P)->red == 0 && (P)->green == 0 && (P)->blue == 0)
#define IS_TRANSPARENT(P) ((P)->alpha == 0)


/* テンプレート版白判定 */
/* テンプレート版白判定（ソースコード内での使用用） */
template <typename T>
inline bool IS_WHITE(const T* p) {
	if constexpr (std::is_same_v<T, PF_Pixel8>)
		return (p->red == 255 && p->green == 255 && p->blue == 255);
	else if constexpr (std::is_same_v<T, PF_Pixel16>)
		return (p->red == 32768 && p->green == 32768 && p->blue == 32768);
	else
		return (p->red >= 1.0f && p->green >= 1.0f && p->blue >= 1.0f);
}
/* 16/32bitの色を比較時のみ8bitへ減色して判定する */
template <typename T>
static bool IsTargetColor(const T* p, const FillInfo* info) {
	if (p->alpha == 0) return false;

	PF_Pixel8 p8;
	if constexpr (std::is_same_v<T, PF_Pixel8>) {
		p8 = *p;
	}
	else if constexpr (std::is_same_v<T, PF_Pixel16>) {
		p8.red = static_cast<A_u_char>((p->red * 255 + 16384) / 32768);
		p8.green = static_cast<A_u_char>((p->green * 255 + 16384) / 32768);
		p8.blue = static_cast<A_u_char>((p->blue * 255 + 16384) / 32768);
	}
	else { // PF_PixelFloat
		p8.red = static_cast<A_u_char>(p->red * 255.0f + 0.5f);
		p8.green = static_cast<A_u_char>(p->green * 255.0f + 0.5f);
		p8.blue = static_cast<A_u_char>(p->blue * 255.0f + 0.5f);
	}

	for (int i = 0; i < info->target_count; i++) {
		if (p8.red == info->targets[i].red && p8.green == info->targets[i].green && p8.blue == info->targets[i].blue)
			return true;
	}
	return false;
}

/* パス1：ターゲットをマーカー(0,0,0)に統一 */
template <typename T>
static PF_Err UnifyToMarkerIterate(void* refcon, A_long x, A_long y, T* in, T* out) {
	FillInfo* info = reinterpret_cast<FillInfo*>(refcon);

	if (IsTargetColor<T>(in, info)) {
		out->red = out->green = out->blue = 0;
		out->alpha = in->alpha;
	}
	else {
		if (info->MakeWhiteTransparent && IS_WHITE(in)) {
			out->red = out->green = out->blue = 0;
			out->alpha = 0;
		}
		else if (IS_MARKER(in)) {
			// マーカー(0,0,0)との衝突を回避するために極小値を代入
			if constexpr (std::is_same_v<T, PF_Pixel8>) { out->red = out->green = out->blue = 1; }
			else if constexpr (std::is_same_v<T, PF_Pixel16>) { out->red = out->green = out->blue = 128; }
			else { out->red = out->green = out->blue = 0.004f; }
			out->alpha = in->alpha;
		}
		else { *out = *in; }
	}
	return PF_Err_NONE;
}

/* パス2：暗い色で浸食 */
template <typename T>
static PF_Err DarkestFillIterate(void* refcon, A_long x, A_long y, T* in, T* out) {
	FillInfo* info = reinterpret_cast<FillInfo*>(refcon);
	if (IS_MARKER(in) && in->alpha != 0) {
		T darkest = *in;
		double min_lum = 2000000000.0;
		bool found = false;

		for (A_long ky = -1; ky <= 1; ky++) {
			for (A_long kx = -1; kx <= 1; kx++) {
				A_long tx = x + kx, ty = y + ky;
				if (tx >= 0 && tx < info->src_world->width && ty >= 0 && ty < info->src_world->height) {
					T* neighbor = reinterpret_cast<T*>(reinterpret_cast<char*>(info->src_world->data) + (ty * info->src_world->rowbytes) + (tx * sizeof(T)));
					if (neighbor->alpha != 0 && !IS_MARKER(neighbor) && !IS_WHITE(neighbor)) {
						double lum = (LUME_R * (double)neighbor->red + LUME_G * (double)neighbor->green + LUME_B * (double)neighbor->blue);
						if (lum < min_lum) { min_lum = lum; darkest = *neighbor; found = true; }
					}
				}
			}
		}
		if (found) { *out = darkest; info->pixels_changed->fetch_add(1); return PF_Err_NONE; }
	}
	*out = *in;
	return PF_Err_NONE;
}

/* 最終パス：塗り残し処理 */
template <typename T>
static PF_Err FinalFillIterate(void* refcon, A_long x, A_long y, T* in, T* out) {
	FillInfo* info = reinterpret_cast<FillInfo*>(refcon);
	if (IS_MARKER(out) && out->alpha != 0) {
		if (!info->KeepPixel) {
			if (info->MakeWhiteTransparent) { out->alpha = 0; }
			else {
				if constexpr (std::is_same_v<T, PF_Pixel8>) { out->red = out->green = out->blue = 255; }
				else if constexpr (std::is_same_v<T, PF_Pixel16>) { out->red = out->green = out->blue = 32768; }
				else { out->red = out->green = out->blue = 1.0f; }
			}
		}
		else if (info->FillUnknownColors) {
			if constexpr (std::is_same_v<T, PF_Pixel8>) *out = info->GiveUpColor8;
			else if constexpr (std::is_same_v<T, PF_Pixel16>) *out = info->GiveUpColor16;
			else *out = info->GiveUpColor32;
		}
	}
	return PF_Err_NONE;
}

/* 実行エンジン */
template <typename T>
static PF_Err ExecuteProcess(CFsAE* ae, FillInfo* infoP) {
	PF_Err err = PF_Err_NONE;
	PF_EffectWorld temp;
	PF_InData* infoP_in = ae->in_data;
	std::atomic<long> changed(0);
	infoP->pixels_changed = &changed;
	PF_InData* in_data = ae->in_data;

	ERR(ae->NewWorld(ae->out->width(), ae->out->height(), ae->pixelFormat(), &temp));
	if (err) return err;

	// STEP 1: マーカー化
	if constexpr (std::is_same_v<T, PF_Pixel8>) err = ae->iterate8(infoP, UnifyToMarkerIterate<T>);
	else if constexpr (std::is_same_v<T, PF_Pixel16>) err = ae->iterate16(infoP, UnifyToMarkerIterate<T>);
	else err = ae->iterate32(infoP, UnifyToMarkerIterate<T>);

	PF_COPY(ae->output, &temp, NULL, NULL);

	// STEP 2: 反復塗りつぶし
	PF_EffectWorld* srcP = &temp;
	PF_EffectWorld* dstP = ae->output;
	for (int i = 0; i < 100; i++) {
		infoP->src_world = srcP;
		changed.store(0);
		if constexpr (std::is_same_v<T, PF_Pixel8>) {
			ERR(ae->suitesP->Iterate8Suite1()->iterate(ae->in_data, 0, ae->output->height, srcP, NULL, infoP, DarkestFillIterate<T>, dstP));
		}
		else if constexpr (std::is_same_v<T, PF_Pixel16>) {
			ERR(ae->suitesP->Iterate16Suite1()->iterate(ae->in_data, 0, ae->output->height, srcP, NULL, infoP, DarkestFillIterate<T>, dstP));
		}
		else {
			ERR(ae->suitesP->IterateFloatSuite1()->iterate(ae->in_data, 0, ae->output->height, srcP, NULL, infoP, DarkestFillIterate<T>, dstP));
		}
		if (err || changed.load() == 0) break;
		std::swap(srcP, dstP);
	}
	if (srcP != ae->output) PF_COPY(srcP, ae->output, NULL, NULL);

	// STEP 3: 最終パス
	if constexpr (std::is_same_v<T, PF_Pixel8>) ae->iterate8(infoP, FinalFillIterate<T>);
	else if constexpr (std::is_same_v<T, PF_Pixel16>) ae->iterate16(infoP, FinalFillIterate<T>);
	else ae->iterate32(infoP, FinalFillIterate<T>);

	ae->suitesP->WorldSuite1()->dispose_world(ae->in_data->effect_ref, &temp);
	return err;
}

PF_Err CellLineEraserSub(CFsAE* ae, FillInfo* infoP) {
	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB32:  return ExecuteProcess<PF_Pixel8>(ae, infoP);
	case PF_PixelFormat_ARGB64:  return ExecuteProcess<PF_Pixel16>(ae, infoP);
	case PF_PixelFormat_ARGB128: return ExecuteProcess<PF_PixelFloat>(ae, infoP);
	default: return PF_Err_BAD_CALLBACK_PARAM;
	}
}
