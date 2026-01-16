#pragma once
#include "../FsLibrary/json.hpp"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
using json = nlohmann::json;

std::string GetPluginConfigFilePath(const std::string& filename);

