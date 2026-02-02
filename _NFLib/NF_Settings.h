#pragma once
#ifndef NF_SETTINGS_H
#define NF_SETTINGS_H
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

#include "json.hpp"
using json = nlohmann::json;

std::string GetPluginConfigFilePath(const std::string& filename);


#endif // NF_SETTINGS_H

