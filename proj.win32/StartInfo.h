#pragma once
#include <string>
extern "C" {
#include "message.h"
}

class StartInfo
{
public:
	static Ipc* ipc;
	static std::string atlasFile;
	static std::string skelFile;

	static void init();
};
