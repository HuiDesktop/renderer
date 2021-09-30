#include "WinMouseTracker.h"
#include <cassert>

//#define EASY_DEBUG

int volatile MouseTracker::mouseX;
int volatile MouseTracker::mouseY;
int volatile MouseTracker::winX;
int volatile MouseTracker::winY;
std::mutex MouseTracker::winPosMutex;

HINSTANCE MouseTracker::hMod;
HHOOK MouseTracker::hook = NULL;

bool MouseTracker::init(HINSTANCE instance)
{
	assert(hook == NULL);
	hMod = instance;
#ifndef EASY_DEBUG
	hook = SetWindowsHookEx(WH_MOUSE_LL, mouseCallback, instance, 0);
	return hook;
#else
	return true;
#endif
}

LRESULT WINAPI MouseTracker::mouseCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_MOUSEMOVE) {
		MOUSEHOOKSTRUCT* info = (MOUSEHOOKSTRUCT*)lParam;
		mouseX = info->pt.x;
		mouseY = info->pt.y;
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

void MouseTracker::updateWindowPos(HWND handle)
{
	RECT rect;
	if (GetWindowRect(handle, &rect) == TRUE) {
		std::lock_guard<std::mutex> g(winPosMutex);
		winX = rect.left;
		winY = rect.top;

#ifdef EASY_DEBUG
		mouseX = winX + 340;
		mouseY = winY + 200;
#endif
	}
}
