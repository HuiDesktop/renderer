#pragma once
#include <string>
extern "C" {
#include "message.h"
}

class StartInfo
{
public:
	static Ipc* ipc;

	static int json, w, h, x0, y0, x, y;

	static std::string atlasFile;
	static std::string skelFile;

	static void init(const char* ipcName);
};
