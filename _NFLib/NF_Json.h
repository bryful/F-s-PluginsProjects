#pragma once
#ifndef NF_JSON_H
#define NF_JSON_H

#include "AE_SDK.h"


#include "json.hpp"
using json = nlohmann::json;

inline json PixelToJson(PF_Pixel8 p)
{
	json ret =
	{
		{"r",p.red},
		{"g",p.green},
		{"b",p.blue}
	};
	return ret;
}
inline PF_Pixel JsonToPixel(json jsn)
{
	PF_Pixel ret = { 0,0,0,0 };
	ret.red = ret.green = ret.blue = 0;
	ret.alpha = PF_MAX_CHAN8;
	PF_Pixel8 p = { 0,0,0,0 };
	if (jsn.find("r") != jsn.end()) {
		ret.red = jsn["r"].get<uint8_t>();
	}
	if (jsn.find("g") != jsn.end()) {
		ret.green = jsn["g"].get<uint8_t>();
	}
	if (jsn.find("b") != jsn.end()) {
		ret.blue = jsn["b"].get<uint8_t>();
	}
	return ret;
}

#endif // NF_JSON_H