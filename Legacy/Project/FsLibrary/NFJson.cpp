#include "NFJson.h"

json PixelToJson(PF_Pixel p)
{
	json ret =
	{
		{"red",p.red},
		{"green",p.green},
		{"blue",p.blue}
	};
	return ret;
}
PF_Pixel JsonToPixel(json jsn)
{
	PF_Pixel ret = { 255,255,255,255 };
	if (jsn.find("red") != jsn.end()) {
		ret.red = jsn["red"].get<BYTE>();
	}
	if (jsn.find("green") != jsn.end()) {
		ret.green = jsn["green"].get<BYTE>();
	}
	if (jsn.find("blue") != jsn.end()) {
		ret.blue = jsn["blue"].get<BYTE>();
	}
	return ret;
}
