#include "Drawing.h"

cDrawing *Draw = new cDrawing();

void cDrawing::Line(float x, float y, float x2, float y2, D3DCOLOR color)
{
	if (!directX->LinePtr)
		D3DXCreateLine(directX->d3ddev, &directX->LinePtr);
	else
	{
		D3DXVECTOR2 Vertex[2];
		Vertex[0].x = x;
		Vertex[0].y = y;
		Vertex[1].x = x2;
		Vertex[1].y = y2;
		directX->LinePtr->Begin();
		directX->LinePtr->Draw(Vertex, 2, color);
		directX->LinePtr->End();
	}
}

void cDrawing::Box(int x, int y, int width, int height, D3DCOLOR colour) {
	Line(x, y, x + width, y, colour);
	Line(x + width, y, x + width, y + height, colour);
	Line(x, y, x, y + height, colour);
	Line(x, y + height, x + width, y + height, colour);
}

void cDrawing::FilledBox(int x, int y, int width, int height, D3DCOLOR colour) {
	sD3DVertex pVertex[4] = { { x, y + height, 0.0f, 1.0f, colour },{ x, y, 0.0f, 1.0f, colour },{ x + width, y + height, 0.0f, 1.0f, colour },{ x + width, y, 0.0f, 1.0f, colour } };
	directX->d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	directX->d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, pVertex, sizeof(sD3DVertex));
}

void cDrawing::GradientRect(float x, float y, float width, float height, D3DCOLOR startCol, D3DCOLOR endCol, gr_orientation orientation)
{
	sD3DVertex vertices[4] = { { 0,0,0,1.0f,0 },{ 0,0,0,1.0f,0 },{ 0,0,0,1.0f,0 },{ 0,0,0,1.0f,0 } };
	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].color = startCol;

	vertices[1].x = x + width;
	vertices[1].y = y;
	vertices[1].color = orientation == horizontal ? endCol : startCol;

	vertices[2].x = x;
	vertices[2].y = y + height;
	vertices[2].color = orientation == horizontal ? startCol : endCol;

	vertices[3].x = x + width;
	vertices[3].y = y + height;
	vertices[3].color = endCol;


	static LPDIRECT3DVERTEXBUFFER9 pVertexObject = NULL;
	static void *pVertexBuffer = NULL;


	directX->d3ddev->CreateVertexBuffer(sizeof(vertices), 0,
		D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &pVertexObject, NULL);

	pVertexObject->Lock(0, sizeof(vertices), &pVertexBuffer, 0);


	memcpy(pVertexBuffer, vertices, sizeof(vertices));
	pVertexObject->Unlock();

	directX->d3ddev->SetStreamSource(0, pVertexObject, 0, sizeof(sD3DVertex));
	directX->d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	directX->d3ddev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	if (pVertexObject != NULL)
		pVertexObject->Release();

}

void cDrawing::Circle(int X, int Y, int radius, int numSides, D3DCOLOR color)
{
	if (!directX->LinePtr)
		D3DXCreateLine(directX->d3ddev, &directX->LinePtr);
	else
	{
		D3DXVECTOR2 Line[128];
		float Step = D3DX_PI * 2.0 / numSides;
		int Count = 0;
		for (float a = 0; a < D3DX_PI*2.0; a += Step)
		{
			float X1 = radius * cos(a) + X;
			float Y1 = radius * sin(a) + Y;
			float X2 = radius * cos(a + Step) + X;
			float Y2 = radius * sin(a + Step) + Y;
			Line[Count].x = X1;
			Line[Count].y = Y1;
			Line[Count + 1].x = X2;
			Line[Count + 1].y = Y2;
			Count += 2;
		}
		directX->LinePtr->Begin();
		directX->LinePtr->Draw(Line, Count, color);
		directX->LinePtr->End();
	}
}

void cDrawing::CircleFilled(float x, float y, float radius, float rotate, int type, int resolution, D3DCOLOR color, D3DCOLOR color2)
{

	LPDIRECT3DVERTEXBUFFER9 g_pVB2;

	std::vector<sD3DVertex> circle(resolution + 2);

	float angle = rotate * D3DX_PI / 180, pi = D3DX_PI;


	pi = D3DX_PI / type;

	circle[0].x = x;
	circle[0].y = y;
	circle[0].z = 0;
	circle[0].rhw = 1;
	circle[0].color = color2;

	for (int i = 1; i < resolution + 2; i++)
	{
		circle[i].x = (float)(x - radius * cos(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].y = (float)(y - radius * sin(pi*((i - 1) / (resolution / 2.0f))));
		circle[i].z = 0;
		circle[i].rhw = 1;
		circle[i].color = color;
	}

	// Rotate matrix 
	int _res = resolution + 2;
	for (int i = 0; i < _res; i++)
	{
		circle[i].x = x + cos(angle)*(circle[i].x - x) - sin(angle)*(circle[i].y - y);
		circle[i].y = y + sin(angle)*(circle[i].x - x) + cos(angle)*(circle[i].y - y);
	}

	directX->d3ddev->CreateVertexBuffer((resolution + 2) * sizeof(sD3DVertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &g_pVB2, NULL);

	VOID* pVertices;
	g_pVB2->Lock(0, (resolution + 2) * sizeof(sD3DVertex), (void**)&pVertices, 0);
	memcpy(pVertices, &circle[0], (resolution + 2) * sizeof(sD3DVertex));
	g_pVB2->Unlock();

	directX->d3ddev->SetTexture(0, NULL);
	directX->d3ddev->SetPixelShader(NULL);
	directX->d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	directX->d3ddev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	directX->d3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	directX->d3ddev->SetStreamSource(0, g_pVB2, 0, sizeof(sD3DVertex));
	directX->d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	directX->d3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, resolution);
	if (g_pVB2 != NULL)
		g_pVB2->Release();
}

void cDrawing::String(const char * szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name)
{

	auto Font = directX->findFont(name);

	if (Font)
	{
		RECT Center = { 0, 0, 0, 0 }, Position = { 0, 0, 0, 0 };

		if (Centered)
			Font->DrawTextA(0, szString, (INT)strlen(szString), &Center, DT_CALCRECT, NULL);



		Position.left = ((LONG)X + 1) - (Center.right / 2);
		Position.top = ((LONG)Y + 1);
		Font->DrawTextA(0, szString, (INT)strlen(szString), &Position, DT_NOCLIP, D3DCOLOR_XRGB(1,1,1));

		Position.left = (LONG)X - (Center.right / 2);
		Position.top = (LONG)Y;
		Font->DrawTextA(0, szString, (INT)strlen(szString), &Position, DT_NOCLIP, COLOR);
	}
}

void cDrawing::String(const wchar_t * szString, float X, float Y, D3DCOLOR COLOR, bool Centered, std::string name)
{

	auto Font = directX->findFont(name);

	if (Font)
	{
		RECT Center = { 0, 0, 0, 0 }, Position = { 0, 0, 0, 0 };

		if (Centered)
			Font->DrawTextW(0, szString, (INT)wcslen(szString), &Center, DT_CALCRECT, NULL);

		Position.left = ((LONG)X + 1) - (Center.right / 2);
		Position.top = ((LONG)Y + 1);
		Font->DrawTextW(0, szString, (INT)wcslen(szString), &Position, DT_NOCLIP, D3DCOLOR_XRGB(1, 1, 1));

		Position.left = (LONG)X - (Center.right / 2);
		Position.top = (LONG)Y;
		Font->DrawTextW(0, szString, (INT)wcslen(szString), &Position, DT_NOCLIP, COLOR);
	}
}


void cDrawing::FilledTriangleEx(FLOAT PosX, FLOAT PosY, FLOAT Size, D3DCOLOR tColourStart, D3DCOLOR tColourMid, D3DCOLOR tColourEnd)
{
	sD3DVertex pLine[3];

	pLine[0].x = PosX;
	pLine[0].y = PosY;
	pLine[0].z = 1.0f;
	pLine[0].rhw = 1.0f;
	pLine[0].color = tColourStart;

	pLine[1].x = PosX + Size / 2;
	pLine[1].y = PosY + Size / 2;
	pLine[1].z = 0.0f;
	pLine[1].rhw = 1.0f;
	pLine[1].color = tColourMid;

	pLine[2].x = PosX;
	pLine[2].y = PosY + Size;
	pLine[2].z = 0.0f;
	pLine[2].rhw = 1.0f;
	pLine[2].color = tColourEnd;

	HRESULT hRet = D3D_OK;

	DWORD dwFvF;
	directX->d3ddev->GetFVF(&dwFvF);
	directX->d3ddev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	hRet = directX->d3ddev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 3, pLine, sizeof(sD3DVertex));

	directX->d3ddev->SetFVF(dwFvF);
}

void cDrawing::HealthBar(int health, float maxhealth, int x, int y, int width, int height)
{
	if (health > maxhealth)
		health = maxhealth;
	int r = 255 - health * 2.55;
	int g = health * 2.55;
	float wtf = health / maxhealth;
	float healthBar = height * wtf;
	int healthBarDelta = height - healthBar;

	FilledBox(x - width / 2 - 6, y - 1, 5, height + 1, D3DCOLOR_XRGB(10, 10, 10));
	FilledBox(x - width / 2 - 5, y + healthBarDelta, 3, healthBar, D3DCOLOR_XRGB(r, g, 0));
}

void cDrawing::DrawScoreBoard(int x, int y, std::vector<Team> Teams)
{
	if (Teams.size() == 0)
		return;

	for (int i = 0; i < Teams.size(); ++i)
	{
		auto color D3DCOLOR_XRGB(255, 255, 255);

		i % 2 == 0 ? color = D3DCOLOR_XRGB(77, 77, 77) : color = D3DCOLOR_XRGB(51, 51, 51);

		FilledBox(x, y + 60*i, 70, 60, color);
		Line(x + 70, y + 60 * i, x + 70, y + 60 * (i + 1), D3DCOLOR_XRGB(255, 255, 255));
		String(Teams.at(i).teamName.c_str(), x + 35, y + 20 + 60 * i, D3DCOLOR_XRGB(Teams.at(i).color.r, Teams.at(i).color.g, Teams.at(i).color.b), true, "small");
		for (int j = 0; j < 4; ++j)
		{
			j % 2 == 0 ?  color = D3DCOLOR_XRGB(51, 51, 51): color = D3DCOLOR_XRGB(77, 77, 77);
			FilledBox(x+71, y + (i*4*15) + 15 * j, 70*3, 15, color);
			if (j< Teams.at(i).Players.size())
			String(Teams.at(i).Players.at(j).PlayerName.c_str(), x + 75, y + 2 + (i * 4 * 15) + 15 * j, D3DCOLOR_XRGB(255, 255, 255), false, "small");
		}
		Line(x, y + 60 * i, x + 280, y + 60 * i, D3DCOLOR_XRGB(255, 255, 255));

	}
	   


}

static bool once = false;


void cDirectX::Render(bool  active)
{
	d3ddev->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, NULL);
	d3ddev->BeginScene();

	if (!once)	
	Draw->String("Initializing Please Wait", 100, 100, D3DCOLOR_XRGB(0, 255, 0), false, "default");

	if (once)
	{
		if (active)
			Draw->Draw();

	}

	if (!once)
	once = true;

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void cCheat::DrawString(const char * szString, float X, float Y, Color COLOR, bool Centered, std::string name)
{
	Draw->String(szString, X, Y, D3DCOLOR_XRGB(COLOR.r, COLOR.g, COLOR.b), Centered, name);
}

void cCheat::DrawString(const wchar_t * szString, float X, float Y, Color COLOR, bool Centered, std::string name)
{
	Draw->String(szString, X, Y, D3DCOLOR_XRGB(COLOR.r, COLOR.g, COLOR.b), Centered, name);
}

void cCheat::DrawHealthBar(int health, float maxhealth, int x, int y, int width, int height)
{
	Draw->HealthBar(health, maxhealth, x, y, width, height);
}

void cCheat::DrawBox(int x, int y, int width, int height, Color color)
{
	Draw->Box(x, y, width, height, D3DCOLOR_XRGB(color.r, color.g, color.b));
}

void cCheat::DrawCircle(int x, int y, int radius, int numSides, Color color)
{
	Draw->Circle(x, y, radius, numSides, D3DCOLOR_XRGB(color.r, color.g, color.b));
}

void cCheat::DrawLine(float X, float Y, float X2, float Y2, Color color)
{
	Draw->Line(X, Y, X2, Y2, D3DCOLOR_XRGB(color.r, color.g, color.b));
}