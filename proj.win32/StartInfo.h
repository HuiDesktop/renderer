#pragma once
#include <string>
extern "C" {
#include "message.h"
}

class StartInfo
{
public:
	static Ipc* ipc;

	static int json, w, h;
	static float x0, y0;
	static int x, y;
	static float scale;

	static std::string atlasFile;
	static std::string skelFile;

	static void init(const char* ipcName);
};
