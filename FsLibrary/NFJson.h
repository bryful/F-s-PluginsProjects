#pragma once
#ifndef NFJSON_H
#define NFJSON_H


#include "json.hpp"

#include "AE_SDK.h"

#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
using json = nlohmann::json;


json PixelToJson(PF_Pixel p);
PF_Pixel JsonToPixel(json jsn);

#endif