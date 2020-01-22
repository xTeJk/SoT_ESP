#pragma once
#include "Misc.h"
#include <vector>
#include <map>
#include "Config.h"

struct Color
{
	int r;
	int g;
	int b;
	int a;

	Color()
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->a = 255;
	}

	Color(int r, int g, int b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	Color(int r, int g, int b, int a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	Color operator / (float div)
	{
		Color color = *this;
		color.r = color.r / div;
		color.g = color.g / div;
		color.b = color.b / div;
		return color;
	}

	Color& operator /= (float div)
	{
		Color& color = *this;
		color.r /= div;
		color.g /= div;
		color.b /= div;
		return color;
	}

	Color& operator *= (float coeff)
	{
		Color& color = *this;
		color.r *= coeff;
		color.g *= coeff;
		color.b *= coeff;
		return color;
	}

	static Color FromHSB(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - (int)h;
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 2)
		{
			return Color(
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255)
			);
		}
		else if (h < 3)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255),
				(unsigned char)(t * 255)
			);
		}
		else if (h < 4)
		{
			return Color(
				(unsigned char)(p * 255),
				(unsigned char)(q * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else if (h < 5)
		{
			return Color(
				(unsigned char)(t * 255),
				(unsigned char)(p * 255),
				(unsigned char)(brightness * 255)
			);
		}
		else
		{
			return Color(
				(unsigned char)(brightness * 255),
				(unsigned char)(p * 255),
				(unsigned char)(q * 255)
			);
		}
	}

};
struct TeamMate
{
	std::string PlayerName;
};

struct Team
{
	Color color;
	std::string teamName;
	std::vector<TeamMate> Players;
};



class cCheat
{
public:
	std::vector<Team>Crews;
	bool bAnimals, bShips, bPlayers, bTreasure, bObjects, bLegend;
public:
	void readData();
	void DrawString(const char * szString, float X, float Y, Color COLOR, bool Centered, std::string name);
	void DrawString(const wchar_t * szString, float X, float Y, Color COLOR, bool Centered, std::string name);
	void DrawHealthBar(int health, float maxhealth, int x, int y, int width, int height);
	void DrawBox(int x, int y, int width, int height, Color color);
	void DrawCircle(int x, int y, int radius, int numSides, Color color);
	void DrawLine(float X, float Y, float X2, float Y2, Color color);
private:
	uintptr_t baseModule, baseSize,
		GWorld, tempGNames, GNames, GObjects,
		UWorld, persistentLevel;
	std::map<int, std::string> Names;
private:
	std::string getNameFromIDmem(int ID);
	std::string getNameFromIDmap(int ID);

};

extern cCheat* Cheat;