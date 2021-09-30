#include "WinMouseTracker.h"
#include <cassert>

//#define EASY_DEBUG

int volatile MouseTracker::mouseX;
int volatile MouseTracker::mouseY;
int volatile MouseTracker::winX;
int volatile MouseTracker::winY;
bool volatile MouseTracker::state;
std::mutex MouseTracker::winPosMutex;

HINSTANCE MouseTracker::hMod;
HHOOK MouseTracker::hook = NULL;
HWND MouseTracker::hwnd;
LONG MouseTracker::exStyle;
bool MouseTracker::dragging;
int MouseTracker::beginX, MouseTracker::beginY, MouseTracker::offsetX, MouseTracker::offsetY, MouseTracker::lastX, MouseTracker::lastY;
bool volatile MouseTracker::lastState;

bool MouseTracker::init(HINSTANCE instance)
{
	dragging = false;
	state = lastState = true;
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
	auto r = CallNextHookEx(hook, nCode, wParam, lParam);
	if (dragging) {
		tryFlushDraggingWindowPos();
	}
	if (lastState != state) {
		if (state) {
			bool t = SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
			assert(t);
			lastState = true;
		}
		else {
			SetWindowLong(hwnd, GWL_EXSTYLE, exStyle ^ WS_EX_TRANSPARENT);
			lastState = false;
		}
	}
	return r;
}

void MouseTracker::setWindow(HWND handle)
{
	hwnd = handle;
	updateWindowPos();
	exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (exStyle & WS_EX_TRANSPARENT) {
		exStyle ^= WS_EX_TRANSPARENT;
	}
}

void MouseTracker::updateWindowPos()
{
	RECT rect;
	if (GetWindowRect(hwnd, &rect) == TRUE) {
		std::lock_guard<std::mutex> g(winPosMutex);
		winX = rect.left;
		winY = rect.top;
#ifdef EASY_DEBUG
		mouseX = winX + 340;
		mouseY = winY + 200;
#endif
	}
}

void MouseTracker::setWindowPos(int x, int y)
{
	SetWindowPos(hwnd, NULL, x, y, 114, 514, SWP_NOSIZE | SWP_NOZORDER);
	std::lock_guard<std::mutex> g(winPosMutex);
	winX = x;
	winY = y;
}

void MouseTracker::beginDrag()
{
	SetCapture(hwnd);
	updateWindowPos();
	dragging = true;
	beginX = mouseX, beginY = mouseY;
	offsetX = mouseX - winX, offsetY = mouseY - winY;
}

void MouseTracker::endDrag()
{
	ReleaseCapture();
	dragging = false;
}

void MouseTracker::tryFlushDraggingWindowPos()
{
	if (!dragging) {
		return;
	}
	if (lastX == mouseX && lastY == mouseY) {
		return;
	}
	lastX = mouseX, lastY = mouseY;
	setWindowPos(mouseX - offsetX, mouseY - offsetY);
}
