#include <cassert>
#include "StartInfo.h"

Ipc* StartInfo::ipc;

int StartInfo::json, StartInfo::w, StartInfo::h, StartInfo::x, StartInfo::y;
float StartInfo::x0, StartInfo::y0, StartInfo::scale;

std::string StartInfo::atlasFile;
std::string StartInfo::skelFile;

void StartInfo::init(const char* ipcName)
{
	ipc = openIpc(ipcName);

	prepareNewMessage(ipc, 0, 0);
	finishNewMessage(ipc);
	setForSend(ipc);

	waitForRead(ipc, 0);
	auto message = peekMessage(ipc);
	assert(messageType(message) == 0);
	{
		int* ints = (int*)message->msg;
		float* floats = (float*)message->msg;
		json = ints[0];
		w = ints[1];
		h = ints[2];
		x0 = floats[3];
		y0 = floats[4];
		x = ints[5];
		y = ints[6];
		scale = floats[7];
	}
	finishReadMessage(ipc);

	message = peekMessage(ipc);
	if (message == nullptr) {
		waitForRead(ipc, 0);
		message = peekMessage(ipc);
	}
	assert(messageType(message) == 1);
	atlasFile = std::string((char*)message->msg);
	finishReadMessage(ipc);

	message = peekMessage(ipc);
	if (message == nullptr) {
		waitForRead(ipc, 0);
		message = peekMessage(ipc);
	}
	assert(messageType(message) == 2);
	skelFile = std::string((char*)message->msg);
	finishReadMessage(ipc);
}
