#pragma once
#include "SotStuff.h"
#include <locale>
#include <codecvt>
#include <utility>

class  cMisc
{
public:
	bool  WorldToScreen(Vector3  world, Vector2* screen);
	std::string wstringToString(std::wstring wstring);
	std::wstring stringToWString(std::wstring string);
};

extern cMisc* Misc;