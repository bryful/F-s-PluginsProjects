#include "NF_RandomMosaic.h"

template <typename T>
static std::vector<std::vector<T>> CopyDataT(
	PF_EffectWorld *world,
	A_long widthTrue,
	A_long x,
	A_long y,
	A_long width,
	A_long height
)
{
	PF_Err err = PF_Err_NONE;
	A_long w = world->width;
	A_long h = world->height;
	T* dataP = reinterpret_cast<T*>(world->data);
	A_long x0 = AE_CLAMP(x, 0, w - 1);
	A_long x1 = AE_CLAMP(x + width, 0, w - 1);
	A_long y0 = AE_CLAMP(y, 0, h - 1);
	A_long y1 = AE_CLAMP(y + height, 0, h - 1);
	if (x1 <= x0 || y1 <= y0) return PF_Err_NONE;
	std::vector<std::vector<T>> result(y1-y0+1, std::vector<T>(x1-x0+1, 0));
	for (A_long j = y0; j <= y1; j++) {
		for (A_long i = x0; i <= x1; i++) {
			result[j][i] = *(dataP + j * widthTrue + i);
		}
	}
	return err;
}
static std::vector<std::vector<PF_Pixel>> CopyData8(
	PF_EffectWorld* world,
	A_long widthTrue,
	A_long x,
	A_long y,
	A_long width,
	A_long height
)
{
	return CopyDataT<PF_Pixel>(world, widthTrue, x, y, width, height);
}
static std::vector<std::vector<PF_Pixel16>> CopyData16(
	PF_EffectWorld* world,
	A_long widthTrue,
	A_long x,
	A_long y,
	A_long width,
	A_long height
)
{
	return CopyDataT<PF_Pixel16>(world, widthTrue, x, y, width, height);
}
static std::vector<std::vector<PF_PixelFloat>> CopyData32(
	PF_EffectWorld* world,
	A_long widthTrue,
	A_long x,
	A_long y,
	A_long width,
	A_long height
)
{
	return CopyDataT<PF_PixelFloat>(world, widthTrue, x, y, width, height);
}

template <typename T>
void PeasteDataT(
	std::vector<std::vector<T>> data,
	PF_EffectWorld* world,
	A_long widthTrue,
	A_long x,
	A_long y
	)
{
	PF_Err err = PF_Err_NONE;
	A_long h = data.size();
	if (h <= 0) return;
	A_long w = data[0].size();
	if (w <= 0) return;
	T* dataP = reinterpret_cast<T*>(world->data);
	if (x + w <0 || x>= world->width || y+h < 0 ||y>=world->height) return;

	for (A_long j = 0; j <= h; j++) {
		A_long yt = y + j;
		if (yt < 0 || yt >= world->height) continue;
		for (A_long i = 0; i < w; i++) {
			A_long xt = x +i;
			if (xt < 0 || xt >= world->width) continue;
			*(dataP + j * widthTrue + i) = result[j][i];
		}
	}
	return err;
}
void PeasteData8(
	std::vector<std::vector<PF_Pixel>> data,
	PF_EffectWorld* world,
	A_long widthTrue,
	A_long x,
	A_long y
)
{
	PeasteDataT<PF_Pixel>(data, world, widthTrue, x, y);
}
void PeasteData16(
	std::vector<std::vector<PF_Pixel>> data,
	PF_EffectWorld* world,
	A_long widthTrue,
	A_long x,
	A_long y
)
{
	PeasteDataT<PF_Pixel>(data, world, widthTrue, x, y);
}