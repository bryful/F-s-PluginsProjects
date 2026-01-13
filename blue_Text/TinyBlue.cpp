#include "TinyBlue.h"
/* --- テンプレートによる水平ボックスブラー --- */
template <typename P>
static PF_Err BoxBlurH(
	void* refcon,
	A_long x,
	A_long y,
	P* in,
	P* out)
{
	if (x != 0) return PF_Err_NONE;
	BlurInfo* bi = (BlurInfo*)refcon;
	A_long width = bi->src->width;
	A_long r = bi->radius;
	if (r <= 0) return PF_Err_NONE;


	// 32bit(float)時はfloat、8/16bit時はA_u_longを使用
	double sumR = 0, sumG = 0, sumB = 0, sumA = 0;
	A_long window = (r * 2) + 1;

	// 行の先頭ポインタをrowbytesから正確に計算する
	P* src_row = (P*)((char*)bi->src->data + (y * bi->src->rowbytes));
	P* dst_row = (P*)((char*)bi->src->data + (y * bi->src->rowbytes));
	//P* dst_row = (P*)((char*)out); // iterateが渡すoutはy行目の先頭を指している

	for (A_long i = -r; i <= r; i++) {
		P* p = src_row + AE_CLAMP(i, 0, width - 1);
		sumR += (double)p->red; sumG += (double)p->green; sumB += (double)p->blue; sumA += (double)p->alpha;
	}

	for (A_long i = 0; i < width; i++) {

		// 割り算の結果を一時的に A_long で受ける
		A_long avgR = static_cast<A_long>(sumR / window);
		A_long avgG = static_cast<A_long>(sumG / window);
		A_long avgB = static_cast<A_long>(sumB / window);
		A_long avgA = static_cast<A_long>(sumA / window);
		// AE_CLAMP で 0～255 (または 16bit なら 0～32768) に収める
		// 8bit: 255, 16bit: 32768, 32bit: 1.0
		A_long max_val = (sizeof(typename P::red) == 1) ? 255 : 32768;
		// ※ 32bit float の場合はこのロジックを別途分岐させる
		dst_row[i].red = static_cast<decltype(P::red)>(AE_CLAMP(avgR, 0, max_val));
		dst_row[i].green = static_cast<decltype(P::green)>(AE_CLAMP(avgG, 0, max_val));
		dst_row[i].blue = static_cast<decltype(P::blue)>(AE_CLAMP(avgB, 0, max_val));
		dst_row[i].alpha = static_cast<decltype(P::alpha)>(AE_CLAMP(avgA, 0, max_val));


		A_long left = AE_CLAMP(i - r, 0, width - 1);
		A_long right = AE_CLAMP(i + r + 1, 0, width - 1);

		sumR += ((double)src_row[right].red - (double)src_row[left].red);
		sumG += ((double)src_row[right].green - (double)src_row[left].green);
		sumB += ((double)src_row[right].blue - (double)src_row[left].blue);
		sumA += ((double)src_row[right].alpha - (double)src_row[left].alpha);
	}
	return PF_Err_NONE;
}

/* --- テンプレートによる垂直ボックスブラー (y=0トリガー方式) --- */
template <typename P>
static PF_Err BoxBlurV(
	void* refcon,
	A_long x,
	A_long y,
	P* in,
	P* out)
{
	// 垂直パスでは y=0 の時だけその「列(x)」を完遂させる
	if (y != 0) return PF_Err_NONE;

	BlurInfo* bi = (BlurInfo*)refcon;
	PF_EffectWorld* dst = bi->src;
	A_long height = dst->height;
	A_long r = bi->radius;
	A_long window = (r * 2) + 1;
	A_long stride = dst->rowbytes / sizeof(P);

	P* col_top = out; // y=0なのでoutはx列の先頭を指している
	double sumR = 0, sumG = 0, sumB = 0, sumA = 0;

	for (A_long i = -r; i <= r; i++) {
		P* p = col_top + (AE_CLAMP(i, 0, height - 1) * stride);
		sumR += p->red; sumG += p->green; sumB += p->blue; sumA += p->alpha;
	}

	for (A_long i = 0; i < height; i++) {
		P* target = col_top + (i * stride);
		A_long avgR = static_cast<A_long>(sumR / window);
		A_long avgG = static_cast<A_long>(sumG / window);
		A_long avgB = static_cast<A_long>(sumB / window);
		A_long avgA = static_cast<A_long>(sumA / window);
		A_long max_val = (sizeof(typename P::red) == 1) ? 255 : 32768;
		target->red = static_cast<decltype(P::red)>(AE_CLAMP(avgR, 0, max_val));
		target->green = static_cast<decltype(P::green)>(AE_CLAMP(avgG, 0, max_val));
		target->blue = static_cast<decltype(P::blue)>(AE_CLAMP(avgB, 0, max_val));
		target->alpha = static_cast<decltype(P::alpha)>(AE_CLAMP(avgA, 0, max_val));

		//target->red = (decltype(P::red))(sumR / window);
		//target->green = (decltype(P::green))(sumG / window);
		//target->blue = (decltype(P::blue))(sumB / window);
		//target->alpha = (decltype(P::alpha))(sumA / window);

		A_long top = AE_CLAMP(i - r, 0, height - 1);
		A_long bottom = AE_CLAMP(i + r + 1, 0, height - 1);

		sumR += (col_top + bottom * stride)->red - (col_top + top * stride)->red;
		sumG += (col_top + bottom * stride)->green - (col_top + top * stride)->green;
		sumB += (col_top + bottom * stride)->blue - (col_top + top * stride)->blue;
		sumA += (col_top + bottom * stride)->alpha - (col_top + top * stride)->alpha;
	}
	return PF_Err_NONE;
}

static PF_Err TinyBlueLm(CFsAE* ae, A_long bl)
{
	PF_Err err = PF_Err_NONE;

	BlurInfo bi;
	bi.radius = bl; // 半径を取得
	bi.src = ae->output; // 出力バッファをそのまま加工
	if (bi.radius <= 0) return err;

	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB32: // 8-bit
		for (int n = 0; n < 3; n++) {
			ERR(ae->iterate8(&bi, BoxBlurH<PF_Pixel8>));
			ERR(ae->iterate8(&bi, BoxBlurV<PF_Pixel8>));
		}
		break;

	case PF_PixelFormat_ARGB64: // 16-bit
		for (int n = 0; n < 3; n++) {
			ERR(ae->iterate16(&bi, BoxBlurH<PF_Pixel16>));
			ERR(ae->iterate16(&bi, BoxBlurV<PF_Pixel16>));
		}
		break;

	case PF_PixelFormat_ARGB128: // 32-bit (Float)
		for (int n = 0; n < 3; n++) {
			ERR(ae->iterate32(&bi, BoxBlurH<PF_PixelFloat>));
			ERR(ae->iterate32(&bi, BoxBlurV<PF_PixelFloat>));
		}
		break;
	}
	return err;

}

PF_Err TinyBlue(CFsAE* ae, A_long bl)
{
	return TinyBlueLm(ae, bl);
}