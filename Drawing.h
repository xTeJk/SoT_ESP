#pragma once
#include "DirectX.h"

class cDrawing
{
public:
	enum gr_orientation {
		horizontal,
		vertical
	};



public:
	void Line(float X, float Y, float X2, float Y2, D3DCOLOR color);
	void FilledBox(int x, int y, int width, int height, D3DCOLOR color);
	void GradientRect(float x, float y, float width, float height, D3DCOLOR startCol, D3DCOLOR endCol, gr_orientation orientation);
	void Circle(int x, int y, int radius, int numSides, D3DCOLOR color);
	void CircleFilled(float x, float y, float radius, float rotate, int type, int resolution, D3DCOLOR color, D3DCOLOR color2);
	void String(const char * szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name);
	void String(const wchar_t * szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name);
	void FilledTriangleEx(FLOAT PosX, FLOAT PosY, FLOAT Size, D3DCOLOR tColourStart, D3DCOLOR tColourMid, D3DCOLOR tColourEnd);
	void HealthBar(int health, float maxhealth, int x, int y, int width, int height);
	void DrawScoreBoard(int x, int y, std::vector<Team> Teams);
	void Box(int x, int y, int width, int height, D3DCOLOR colour);
	void Draw();

private:
	struct sD3DVertex {
		float x, y, z, rhw;
		DWORD color;
	};
};

extern cDrawing* Draw;