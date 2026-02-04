#pragma once
#ifndef NF_SETTINGS_H
#define NF_SETTINGS_H
#include <windows.h>
#include <shlobj.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include "json.hpp"
using json = nlohmann::json;

#include "tinyfiledialogs.h"	
 
std::string GetPluginConfigFilePath(const std::string& filename);

std::string loadTextFile(std::string name);
void saveTextFile(std::string name, std::string txt);
std::string OpenFileDialog(
    std::string title,
    std::string defp, 
    std::vector<std::string> fltp, 
    std::string des);
std::string SaveFileDialog(
    std::string title,
    std::string defp,
    std::vector<std::string> fltp,
    std::string des
);
std::string ChangeFileExtension(const std::string& filename, const std::string& newExtension);
std::string GetDocumentsFolderPath();

#endif // NF_SETTINGS_H

