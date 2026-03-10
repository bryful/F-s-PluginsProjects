#include "ColorBlockFilter.h"

template <typename T,typename CT,float max_chan>
PF_Err ColorBlockT(
	PF_EffectWorld* worldP,
	A_long widthTrue,
	A_long count,
	A_long width,
	A_long height,
	float value,
	PF_Boolean isAlpha,
	A_long seed
)
{
	PF_Err err = PF_Err_NONE;

	for (int cnt = 0; cnt < count; cnt++) {
		A_long rw = AE_CLAMP((A_long)((float)width * hash_float(cnt * 10, 10, seed)), 2, width-1);
		A_long rh = AE_CLAMP((A_long)((float)height*hash_float(cnt * 10, 50, seed)), 2, height-1);
		A_long tx = (A_long)(-rw/2.0f +  (float)((float)worldP->width + rw/ 2.0f) * hash_float(cnt * 10, 100, seed));
		A_long ty = (A_long)((float)(worldP->height - rh - 1) * hash_float(cnt * 10, 200, seed));
		float v = (0.5f -  hash_float(cnt * 10, 300, seed)) * 2 * value * max_chan;

		for (int j = 0; j < rh; j++) {
			A_long yt = ty + j;
			if (yt < 0 || yt >= worldP->height) continue;
			for (int i = 0; i < rw; i++) {
				A_long xt = tx + i;
				if (xt < 0 || xt >= worldP->width) continue;
				T* p = reinterpret_cast<T*>(worldP->data) + yt * widthTrue + xt;
				float c = 0;
				if (hash_float(cnt * 10, 400, seed) <0.5f) {
					p->red = static_cast<decltype(T::red)>(AE_CLAMP((float)p->red + v, 0, max_chan));
					c+=0.2f* max_chan;
				}
				if (hash_float(cnt * 10, 500, seed) < 0.5f) {
					p->green = static_cast<decltype(T::red)>(AE_CLAMP((float)p->green + v, 0, max_chan));
					c += 0.2f * max_chan;
				}
				if (hash_float(cnt * 10, 600, seed) < 0.5f) {
					p->blue = static_cast<decltype(T::blue)>(AE_CLAMP((float)p->blue + v, 0, max_chan));
					c += 0.2f * max_chan;
				}
				if (c > 0 && isAlpha) {
					p->alpha = static_cast<decltype(T::red)>(AE_CLAMP((float)p->alpha + v, 0, max_chan));
				}
			}
		}


	}

	return err;
}
PF_Err ColorBlockMain(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	A_long count,
	A_long width,
	A_long height,
	float value,
	PF_Boolean alpha,
	A_long seed

)
{
	PF_Err err = PF_Err_NONE;
	if (count <= 0 || width <= 0 || height <= 0) return PF_Err_NONE;

	switch (pixelFormat) {
	case PF_PixelFormat_ARGB32:
		err= ColorBlockT<PF_Pixel,A_u_char,255.0f>(
			worldP,
			worldP->rowbytes / sizeof(PF_Pixel),
			count,
			width,
			height,
			value,
			alpha,
			seed);
		break;
	case PF_PixelFormat_ARGB64:
		err = ColorBlockT<PF_Pixel16,A_u_short,32768.0f>(
			worldP,
			worldP->rowbytes / sizeof(PF_Pixel16),
			count,
			width,
			height,
			value,
			alpha,
			seed);
		break;
	case PF_PixelFormat_ARGB128:
		err = ColorBlockT<PF_PixelFloat,PF_PixelFloat,1.0f>(
			worldP,
			worldP->rowbytes / sizeof(PF_PixelFloat),
			count,
			width,
			height,
			value,
			alpha,
			seed);
		break;
	default:
		err = PF_Err_INVALID_INDEX;
	}


	return err;
}
