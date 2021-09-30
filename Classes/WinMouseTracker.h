#include <mutex>
#include <Windows.h>

class MouseTracker {
public:
	static volatile int mouseX, mouseY, winX, winY;
	static volatile bool state;
	static std::mutex winPosMutex;
	static bool init(HINSTANCE instance);
	static LRESULT WINAPI mouseCallback(int nCode, WPARAM wParam, LPARAM lParam);
	static void setWindow(HWND handle);
	static void updateWindowPos();
	static void setWindowPos(int x, int y);
	static void beginDrag();
	static void endDrag();
	static void tryFlushDraggingWindowPos();

private:
	static HINSTANCE hMod;
	static HHOOK hook;
	static HWND hwnd;
	static LONG exStyle;
	static bool dragging;
	static int beginX, beginY, offsetX, offsetY, lastX, lastY;
	static volatile bool lastState;
};