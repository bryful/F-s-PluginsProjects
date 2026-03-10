#include "NF_RandomShift.h"

template <typename T>
static void CopyDataT(
	std::vector<std::vector<T>> *buf,
	PF_EffectWorld *world,
	A_long widthTrue,
	A_long x,
	A_long y,
	A_long width,
	A_long height
)
{
	A_long w = world->width;
	A_long h = world->height;
	T* dataP = reinterpret_cast<T*>(world->data);
	A_long x0 = AE_CLAMP(x, 0, w - 1);
	A_long x1 = AE_CLAMP(x + width - 1, 0, w - 1);  // -1 ‚ð’Ç‰Á
	A_long y0 = AE_CLAMP(y, 0, h - 1);
	A_long y1 = AE_CLAMP(y + height - 1, 0, h - 1); // -1 ‚ð’Ç‰Á
	if (x1 <= x0 || y1 <= y0) return;
	for (A_long j = y0; j <= y1; j++) {
		for (A_long i = x0; i <= x1; i++) {
			buf->at(j - y0).at(i - x0) = *(dataP + j * widthTrue + i);
		}
	}
}

template <typename T>
void PeasteDataT(
	std::vector<std::vector<T>> *data,
	A_long dWidth,
	A_long dHeight,
	PF_EffectWorld* world,
	A_long widthTrue,
	A_long x,
	A_long y
	)
{
	if (dHeight <= 0) return;
	if (dWidth <= 0) return;
	T* dataP = reinterpret_cast<T*>(world->data);
	if (x + dWidth < 0 || x >= world->width || y + dHeight < 0 || y >= world->height) return;

	for (A_long j = 0; j < dHeight; j++) {  // <= ‚ð < ‚ÉC³
		A_long yt = y + j;
		if (yt < 0 || yt >= world->height) continue;
		for (A_long i = 0; i < dWidth; i++) {
			A_long xt = x + i;
			if (xt < 0 || xt >= world->width) continue;
			*(dataP + yt * widthTrue + xt) = data->at(j).at(i);
		}
	}
}

template <typename T>
PF_Err RandomShiftMainT(
	PF_EffectWorld* worldP,
	A_long widthTrue,
	A_long count,
	A_long rWidth,
	A_long rHeight,
	A_long offsetX,
	A_long offsetY,
	A_long seed
)
{
	PF_Err err = PF_Err_NONE;
	std::vector<std::vector<T>> data(rHeight, std::vector<T>(rWidth, T{}));

	for (int cnt = 0; cnt < count; cnt++) {
		A_long rw = AE_CLAMP((A_long)((float)rWidth / 2 + hash_float(cnt * 10, 10, seed) * (float)rWidth / 2), 1, rWidth);
		A_long rh = AE_CLAMP((A_long)((float)rHeight / 2 + hash_float(cnt * 10, 50, seed) * (float)rHeight / 2), 1, rHeight);
		A_long tx = (A_long)((float)(worldP->width - rw - 1) * hash_float(cnt * 10, 100, seed));
		A_long ty = (A_long)((float)(worldP->height - rh - 1) * hash_float(cnt * 10, 200, seed));
		A_long px = AE_CLAMP(tx + (A_long)(offsetX * (0.5f - hash_float(cnt * 10, 300, seed))), 0, worldP->width - 1);  // -1 ‚ð’Ç‰Á
		A_long py = AE_CLAMP(ty + (A_long)(offsetY * (0.5f - hash_float(cnt * 10, 400, seed))), 0, worldP->height - 1); // -1 ‚ð’Ç‰Á
		CopyDataT<T>(&data, worldP, widthTrue, tx, ty, rw, rh);
		PeasteDataT<T>(&data, rw, rh, worldP, widthTrue, px, py);
	}

	return err;
}
PF_Err RandomShiftMain(
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	A_long count,
	A_long rWidth,
	A_long rHeight,
	A_long offsetX,
	A_long offsetY,
	A_long seed

	)
{
	PF_Err err = PF_Err_NONE;
	if (count <= 0 || rWidth <= 0 || rHeight <= 0 || (offsetX == 0 && offsetY == 0)) return PF_Err_NONE;

	switch (pixelFormat) {
		case PF_PixelFormat_ARGB32:
			ERR(RandomShiftMainT<PF_Pixel>(
				worldP, 
				worldP->rowbytes / sizeof(PF_Pixel), 
				count, 
				rWidth, 
				rHeight, 
				offsetX, 
				offsetY, 
				seed));
			break;
		case PF_PixelFormat_ARGB64:
			ERR(RandomShiftMainT<PF_Pixel16>(
				worldP,
				worldP->rowbytes / sizeof(PF_Pixel16),
				count,
				rWidth,
				rHeight,
				offsetX,
				offsetY,
				seed));
			break;
		case PF_PixelFormat_ARGB128:
			ERR(RandomShiftMainT<PF_PixelFloat>(
				worldP,
				worldP->rowbytes / sizeof(PF_PixelFloat),
				count,
				rWidth,
				rHeight,
				offsetX,
				offsetY,
				seed));
			break;
		default:
			err = PF_Err_INVALID_INDEX;
	}


	return err;
}
