#include "Menu.h"
#include <ctime>
#include "dwmapi.h"

vars Vars;
c_config g_configs;
c_offsets g_offsets;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class KeyToggle {
public:
	KeyToggle(int key) :mKey(key), mActive(false) {}
	operator bool() {
		if (GetAsyncKeyState(mKey)) {
			if (!mActive) {
				mActive = true;
				return true;
			}
		}
		else
			mActive = false;
		return false;
	}
private:
	int mKey;
	bool mActive;
};

KeyToggle toggleHOME(VK_HOME);

KeyToggle toggleF1(VK_F1);
KeyToggle toggleF2(VK_F2);
KeyToggle toggleF3(VK_F3);
KeyToggle toggleF4(VK_F4);
KeyToggle toggleF5(VK_F5);
KeyToggle toggleF6(VK_F6);
KeyToggle toggleF7(VK_F7);
KeyToggle toggleF8(VK_F8);
KeyToggle toggleF9(VK_F9);
KeyToggle toggleF10(VK_F10);


KeyToggle toggleNUM1(VK_NUMPAD1);
KeyToggle toggleNUM2(VK_NUMPAD2);
KeyToggle toggleNUM3(VK_NUMPAD3);
KeyToggle toggleNUM4(VK_NUMPAD4);
KeyToggle toggleNUM5(VK_NUMPAD5);
KeyToggle toggleNUM6(VK_NUMPAD6);
KeyToggle toggleNUM7(VK_NUMPAD7);
KeyToggle toggleNUM8(VK_NUMPAD8);
KeyToggle toggleNUM9(VK_NUMPAD9);

POINT p;
bool moving = false;
int windowPosX = 0;
int windowPosY = 0;
int clickX = 0;
int clickY = 0;

double clockToMilliseconds(clock_t ticks) {
	// units/(units/time) => time (seconds) * 1000 = milliseconds
	return (ticks / (double)CLOCKS_PER_SEC)*1000.0;
}

clock_t deltaTime = 0;
unsigned int frames = 0;
double  frameRate = 60;
double  averageFrameTimeMilliseconds = 60;


void CheckMenuStatus(int key)
{
	if (key == 1) {
		if (Vars.ESP.MenuV2.F2)
		{
			Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
		}
		if (Vars.ESP.MenuV2.F3)
		{
			Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
		}
		if (Vars.ESP.MenuV2.F4)
		{
			Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
		}
		if (Vars.ESP.MenuV2.F5)
		{
			Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
		}
		if (Vars.ESP.MenuV2.F6)
		{
			Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
		}
		if (Vars.ESP.MenuV2.F7)
		{
			Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
		}
	}
	else if (key == 2) {
		if (Vars.ESP.MenuV2.F1)
		{
			Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
		}
		if (Vars.ESP.MenuV2.F3)
		{
			Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
		}
		if (Vars.ESP.MenuV2.F4)
		{
			Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
		}
		if (Vars.ESP.MenuV2.F5)
		{
			Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
		}
		if (Vars.ESP.MenuV2.F6)
		{
			Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
		}
		if (Vars.ESP.MenuV2.F7)
		{
			Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
		}
	}
	else if (key == 3) {
		if (Vars.ESP.MenuV2.F1)
		{
			Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
		}
		if (Vars.ESP.MenuV2.F2)
		{
			Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
		}
		if (Vars.ESP.MenuV2.F4)
		{
			Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
		}
		if (Vars.ESP.MenuV2.F5)
		{
			Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
		}
		if (Vars.ESP.MenuV2.F6)
		{
			Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
		}
		if (Vars.ESP.MenuV2.F7)
		{
			Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
		}
	}
	else if (key == 4) {
		if (Vars.ESP.MenuV2.F1)
		{
			Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
		}
		if (Vars.ESP.MenuV2.F2)
		{
			Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
		}
		if (Vars.ESP.MenuV2.F3)
		{
			Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
		}
		if (Vars.ESP.MenuV2.F5)
		{
			Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
		}
		if (Vars.ESP.MenuV2.F6)
		{
			Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
		}
		if (Vars.ESP.MenuV2.F7)
		{
			Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
		}
	}
	else if (key == 5) {
		if (Vars.ESP.MenuV2.F1)
		{
			Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
		}
		if (Vars.ESP.MenuV2.F2)
		{
			Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
		}
		if (Vars.ESP.MenuV2.F3)
		{
			Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
		}
		if (Vars.ESP.MenuV2.F4)
		{
			Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
		}
		if (Vars.ESP.MenuV2.F6)
		{
			Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
		}
		if (Vars.ESP.MenuV2.F7)
		{
			Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
		}
	}
	else if (key == 6) {
		if (Vars.ESP.MenuV2.F1)
		{
			Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
		}
		if (Vars.ESP.MenuV2.F2)
		{
			Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
		}
		if (Vars.ESP.MenuV2.F3)
		{
			Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
		}
		if (Vars.ESP.MenuV2.F4)
		{
			Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
		}
		if (Vars.ESP.MenuV2.F5)
		{
			Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
		}
		if (Vars.ESP.MenuV2.F7)
		{
			Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
		}
	}
	else if (key == 7) {
		if (Vars.ESP.MenuV2.F1)
		{
			Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
		}
		if (Vars.ESP.MenuV2.F2)
		{
			Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
		}
		if (Vars.ESP.MenuV2.F3)
		{
			Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
		}
		if (Vars.ESP.MenuV2.F4)
		{
			Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
		}
		if (Vars.ESP.MenuV2.F5)
		{
			Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
		}
		if (Vars.ESP.MenuV2.F6)
		{
			Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
		}
	}
	else if (key == 0) {
		if (Vars.ESP.MenuV2.F1)
		{
			Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
		}
		if (Vars.ESP.MenuV2.F2)
		{
			Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
		}
		if (Vars.ESP.MenuV2.F3)
		{
			Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
		}
		if (Vars.ESP.MenuV2.F4)
		{
			Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
		}
		if (Vars.ESP.MenuV2.F5)
		{
			Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
		}
		if (Vars.ESP.MenuV2.F6)
		{
			Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
		}
		if (Vars.ESP.MenuV2.F7)
		{
			Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
		}
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	if (!Process->setWindow("Sea of Thieves"))
	{
		MessageBoxA(NULL, "Failed To Find The Window", "Failed To Find The Window", MB_OK);
		return 1;
	}

	if (!Process->attachProcess("SoTGame.exe"))
	{
		MessageBoxA(NULL, "Failed To Find The Process", "Failed To Find The Process", MB_OK);
		return 1;
	}

	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW| WS_EX_TRANSPARENT,
		"WindowClass",
		"RandomTitle",
		WS_POPUP,
		Process->Position[0], Process->Position[1],
		Process->Size[0], Process->Size[1],
		NULL,
		NULL,
		hInstance,
		NULL);
	SetLayeredWindowAttributes(hWnd, RGB(0, 0, 0), 255,  LWA_ALPHA);

	MARGINS margins = { -1 };
	DwmExtendFrameIntoClientArea(hWnd, &margins);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	directX->initD3D(hWnd);
	Process->myWindow= hWnd;

	g_offsets.init();
	if (!g_offsets.load("offsets"))
	{
		MessageBoxA(NULL, "Failed To Find The Offsets File", "Please place the offsets file in the same folder as the Application", MB_OK);
		return 1;
	}

	g_configs.init();
	if (!g_configs.load("config"))
		g_configs.save("config");

	g_configs.save("config");
	MSG msg;

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}

		if (msg.message == WM_QUIT)
			break;

		if (Vars.ESP.MenuV2.MenuActive)
		{
			if (toggleF1) {
				CheckMenuStatus(1);
				Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
			}
			if (toggleF2) {
				CheckMenuStatus(2);
				Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
			}
			if (toggleF3) {
				CheckMenuStatus(3);
				Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
			}
			if (toggleF4) {
				CheckMenuStatus(4);
				Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
			}
			if (toggleF5) {
				CheckMenuStatus(5);
				Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
			}
			if (toggleF6) {
				CheckMenuStatus(6);
				Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
			}
			if (toggleF7) {
				CheckMenuStatus(7);
				Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
			}
		}

		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F1)
		{
			if (toggleF1)
				Vars.ESP.MenuV2.F1 = !Vars.ESP.MenuV2.F1;
			if (toggleNUM1)
				Menu->menuOpen = !Menu->menuOpen;
			if (toggleNUM2)
				Vars.ESP.MenuTake.MapPins = !Vars.ESP.MenuTake.MapPins;
			if (toggleNUM3)
				Vars.ESP.MenuTake.ColorsLegend = !Vars.ESP.MenuTake.ColorsLegend;
			if (toggleNUM4)
				Vars.ESP.MenuTake.LoreBooks = !Vars.ESP.MenuTake.LoreBooks;
			if (toggleNUM5)
				g_configs.save("config");
			if (toggleNUM6)
				g_configs.load("config");
			if (toggleNUM7)
				Vars.ESP.MenuTake.Debug = !Vars.ESP.MenuTake.Debug;
		}

		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F2)
		{
			if (toggleF2)
				Vars.ESP.MenuV2.F2 = !Vars.ESP.MenuV2.F2;
			if (toggleNUM1)
				Vars.ESP.MenuTake.PlayerEnemy = !Vars.ESP.MenuTake.PlayerEnemy;
			if (toggleNUM2)
				Vars.ESP.MenuTake.PlayerAlly = !Vars.ESP.MenuTake.PlayerAlly;
			if (toggleNUM3)
				Vars.ESP.MenuTake.PlayerHPBar = !Vars.ESP.MenuTake.PlayerHPBar;
			if (toggleNUM4)
				Vars.ESP.MenuTake.PlayerName = !Vars.ESP.MenuTake.PlayerName;
			if (toggleNUM5)
				Vars.ESP.MenuTake.PlayerItem = !Vars.ESP.MenuTake.PlayerItem;
			if (toggleNUM6)
				Vars.ESP.MenuTake.Skeleton = !Vars.ESP.MenuTake.Skeleton;
			if (toggleNUM7)
				Vars.ESP.MenuTake.SkeletonName = !Vars.ESP.MenuTake.SkeletonName;
			if (toggleNUM8)
				Vars.ESP.MenuTake.SkeletonItem = !Vars.ESP.MenuTake.SkeletonItem;
			if (toggleNUM9)
				Vars.ESP.MenuTake.WaterMobs = !Vars.ESP.MenuTake.WaterMobs;
		}

		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F3)
		{
			if (toggleF3)
				Vars.ESP.MenuV2.F3 = !Vars.ESP.MenuV2.F3;
			if (toggleNUM1)
				Vars.ESP.MenuTake.AnimalsCommon = !Vars.ESP.MenuTake.AnimalsCommon;
			if (toggleNUM2)
				Vars.ESP.MenuTake.AnimalsRare = !Vars.ESP.MenuTake.AnimalsRare;
			if (toggleNUM3)
				Vars.ESP.MenuTake.AnimalsMythical = !Vars.ESP.MenuTake.AnimalsMythical;
			if (toggleNUM4)
				Vars.ESP.MenuTake.AnimalsLegendary = !Vars.ESP.MenuTake.AnimalsLegendary;
			if (toggleNUM5)
				Vars.ESP.MenuTake.AnimalsSnakes = !Vars.ESP.MenuTake.AnimalsSnakes;
			if (toggleNUM6)
				Vars.ESP.MenuTake.AnimalsChickens = !Vars.ESP.MenuTake.AnimalsChickens;
			if (toggleNUM7)
				Vars.ESP.MenuTake.AnimalsPigs = !Vars.ESP.MenuTake.AnimalsPigs;
		}

		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F4)
		{
			if (toggleF4)
				Vars.ESP.MenuV2.F4 = !Vars.ESP.MenuV2.F4;
			if (toggleNUM1)
				Vars.ESP.MenuTake.ItemsCommon = !Vars.ESP.MenuTake.ItemsCommon;
			if (toggleNUM2)
				Vars.ESP.MenuTake.ItemsRare = !Vars.ESP.MenuTake.ItemsRare;
			if (toggleNUM3)
				Vars.ESP.MenuTake.ItemsMythical = !Vars.ESP.MenuTake.ItemsMythical;
			if (toggleNUM4)
				Vars.ESP.MenuTake.ItemsLegendary = !Vars.ESP.MenuTake.ItemsLegendary;
			if (toggleNUM5)
				Vars.ESP.MenuTake.ItemsUtility = !Vars.ESP.MenuTake.ItemsUtility;
			if (toggleNUM6)
				Vars.ESP.MenuTake.ItemsBarrels = !Vars.ESP.MenuTake.ItemsBarrels;
			if (toggleNUM7)
				Vars.ESP.MenuTake.ItemsCages = !Vars.ESP.MenuTake.ItemsCages;
			if (toggleNUM8)
				Vars.ESP.MenuTake.ItemsOther = !Vars.ESP.MenuTake.ItemsOther;
			if (toggleNUM9)
				Vars.ESP.MenuTake.ItemsHarpoonSupport = !Vars.ESP.MenuTake.ItemsHarpoonSupport;
		}

		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F5)
		{
			if (toggleF5)
				Vars.ESP.MenuV2.F5 = !Vars.ESP.MenuV2.F5;
			if (toggleNUM1)
				Vars.ESP.MenuTake.EventsFort = !Vars.ESP.MenuTake.EventsFort;
			if (toggleNUM2)
				Vars.ESP.MenuTake.EventsCloud = !Vars.ESP.MenuTake.EventsCloud;
			if (toggleNUM3)
				Vars.ESP.MenuTake.EventsReapers = !Vars.ESP.MenuTake.EventsReapers;
		}

		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F6)
		{
			if (toggleF6)
				Vars.ESP.MenuV2.F6 = !Vars.ESP.MenuV2.F6;
			if (toggleNUM1)
				Vars.ESP.MenuTake.ShipsEnemy = !Vars.ESP.MenuTake.ShipsEnemy;
			if (toggleNUM2)
				Vars.ESP.MenuTake.ShipsAlly = !Vars.ESP.MenuTake.ShipsAlly;
			if (toggleNUM3)
				Vars.ESP.MenuTake.ShipsRowboat = !Vars.ESP.MenuTake.ShipsRowboat;
			if (toggleNUM4)
				Vars.ESP.MenuTake.ShipsGhost = !Vars.ESP.MenuTake.ShipsGhost;
		}

		if (Vars.ESP.MenuV2.MenuActive && Vars.ESP.MenuV2.F7)
		{
			if (toggleF7)
				Vars.ESP.MenuV2.F7 = !Vars.ESP.MenuV2.F7;
			if (toggleNUM1)
				Vars.ESP.MenuTake.ObjectsSiren = !Vars.ESP.MenuTake.ObjectsSiren;
			if (toggleNUM2)
				Vars.ESP.MenuTake.ObjectsStatues = !Vars.ESP.MenuTake.ObjectsStatues;
			if (toggleNUM3)
				Vars.ESP.MenuTake.ObjectsWrecks = !Vars.ESP.MenuTake.ObjectsWrecks;
		}

			/*
			if (toggleF1)
				Menu->menuOpen = !Menu->menuOpen;
			if (toggleF2)
				Vars.ESP.Animals.bActive = !Vars.ESP.Animals.bActive;
			if (toggleF3)
				Vars.ESP.Player.bActive = !Vars.ESP.Player.bActive;
			if (toggleF4)
				Vars.ESP.Ships.bActive = !Vars.ESP.Ships.bActive;
			if (toggleF5)
				Vars.ESP.Treasure.bActive = !Vars.ESP.Treasure.bActive;
			if (toggleF6)
				Vars.ESP.Objects.bActive = !Vars.ESP.Objects.bActive;
			if (toggleF7)
				Vars.ESP.Legend.bActive = !Vars.ESP.Legend.bActive;
			*/

		if (toggleHOME)
			Vars.ESP.MenuV2.MenuActive = !Vars.ESP.MenuV2.MenuActive;
			//CheckMenuStatus(0);
			//g_configs.load("config");

		if (GetAsyncKeyState(VK_END))
			break;

		Process->getSize();
		MoveWindow(hWnd, Process->Position[0], Process->Position[1], Process->Size[0], Process->Size[1], false);

		if (Process->isWindowMaximized())
			directX->Reset();
		else
		{
			clock_t beginFrame = clock();
			if (!directX->resetLock)
			directX->Render(Process->isWindowActive());
			clock_t endFrame = clock();

			deltaTime += endFrame - beginFrame;
			frames++;

			if (clockToMilliseconds(deltaTime) > 1000.0) { //every second
				directX->frames = frames;//(double)frames * 0.5 + frameRate * 0.5; //more stable
				frames = 0;
				deltaTime -= CLOCKS_PER_SEC;
				averageFrameTimeMilliseconds = 1000.0 / (frameRate == 0 ? 0.001 : frameRate);
			}

		}
	}

	// clean up DirectX and COM
	directX->cleanD3D();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:
		Process->zoomOnce = false;
		directX->Reset();
		break;
	case WM_MOUSEMOVE:
		Menu->curPos.x = GET_X_LPARAM(lParam);
		Menu->curPos.y = GET_Y_LPARAM(lParam);
		break;
	case WM_LBUTTONDOWN:
	{

		break;
	}
	case WM_LBUTTONUP:
	{

		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
