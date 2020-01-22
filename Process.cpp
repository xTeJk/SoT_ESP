#include "Process.h"
#include <dwmapi.h>

#pragma comment(lib, "Dwmapi.lib")


cProcess* Process = new cProcess();

bool cProcess::attachProcess(std::string Process)
{
	while (!Mem->hProcess)
	{
		Mem->Process(Process.c_str());
	}

	return true;
}

bool cProcess::setWindow(std::string Window)
{
	targetWindow = FindWindow(NULL, Window.c_str());

	if (!targetWindow)
		return false;

	if (!getSize())
		return false;

	return true;
}

bool cProcess::getSize()
{
	if (!GetWindowRect(targetWindow, &WindowRect))
		exit(1);

	DwmGetWindowAttribute(targetWindow, DWMWA_EXTENDED_FRAME_BOUNDS, &frame, sizeof(RECT));

	RECT border;
	border.left = frame.left - WindowRect.left;
	border.top = frame.top - WindowRect.top;
	border.right = WindowRect.right - frame.right;
	border.bottom = WindowRect.bottom - frame.bottom;

	WindowRect.left += border.left;
	WindowRect.top += border.top;
	WindowRect.right -= border.right;
	WindowRect.bottom -= border.bottom;

	Size[0] = WindowRect.right - WindowRect.left;
	Size[1] = WindowRect.bottom - WindowRect.top -32;

	Position[0] = WindowRect.left;
	Position[1] = WindowRect.top +32;

	return true;
}

bool cProcess::isWindowActive()
{
	HWND ActiveWindow = GetForegroundWindow();

	if (ActiveWindow != targetWindow  && ActiveWindow != myWindow)
		return false;

	return true;
}

bool cProcess::isWindowMaximized()
{
	if (IsZoomed(targetWindow) && !zoomOnce)
	{
		getSize();
		zoomOnce = true;
		return true;
	}
	else
		return false;
}