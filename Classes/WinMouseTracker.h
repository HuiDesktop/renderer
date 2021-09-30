#include <mutex>
#include <Windows.h>

class MouseTracker {
public:
	static volatile int mouseX, mouseY, winX, winY;
	static std::mutex winPosMutex;
	static bool init(HINSTANCE instance);
	static LRESULT WINAPI mouseCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static void updateWindowPos(HWND handle);

private:
	static HINSTANCE hMod;
	static HHOOK hook;
};