#include <cassert>
#include "StartInfo.h"

Ipc* StartInfo::ipc;

int StartInfo::json, StartInfo::w, StartInfo::h, StartInfo::x0, StartInfo::y0, StartInfo::x, StartInfo::y;

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
		int* initmsg = (int*)message->msg;
		json = initmsg[0];
		w = initmsg[1];
		h = initmsg[2];
		x0 = initmsg[3];
		y0 = initmsg[4];
		x = initmsg[5];
		y = initmsg[6];
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
