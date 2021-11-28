#pragma once

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <Windows.h>
#include <WinBase.h>
#include <fstream> 
#include <stdio.h>
#include "resource.h"

void ScreenPayload();
bool ExtractResource(std::uint16_t ResourceID, std::string OutputFileName, const char* ResType);