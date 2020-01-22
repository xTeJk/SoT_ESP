#pragma once
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include <array>
#include <vector>
#include <string>
#include <iostream>
#include "Cheat.h"


struct sFont
{
	ID3DXFont* Font;
	std::string Name;
};

class cDirectX
{
public:
	int windowWidth = 800;
	int windowHeight = 600;
	int windowX = 300;
	int windowY = 300;
	int frames = 30;
	bool resetLock = false;

public:
	void initD3D(HWND hWnd);    // sets up and initializes Direct3D
	void cleanD3D();    // closes Direct3D and releases memory
	void getSize();
	void Reset();
	void Render(bool active);
	void initFonts();
	ID3DXFont* findFont(std::string name);

	LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
	LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
	std::vector<sFont>Fonts;
	ID3DXLine * LinePtr;
	bool addFont(std::string name, std::string font, int size);

};

extern cDirectX* directX;