#include <cassert>
#include "StartInfo.h"

Ipc* StartInfo::ipc;
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
	// nothing to do in test stage
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
