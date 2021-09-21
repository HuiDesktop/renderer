#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "ipc.h"
#include "message.h"

int main() {
	Ipc* ipc = createIpc(64, 64);
	printf_s("Created IPC channel pair: %s\n", ipc->name);
	while (1)
	{
		int type = waitForRead(ipc, 100);
		if (type == 1) {
			IpcMessage* msg;
			while ((msg = peekMessage(ipc)))
			{
				uint32_t len = messageLen(msg);
				printf("Got a message: type = %d, length = %d\n", messageType(msg), len);
				for (uint32_t i = 0; i < len; ++i) {
					printf("%x ", ((uint8_t*)msg->msg)[i]);
				}
				puts("");
				puts("Wait 1s...");
				finishReadMessage(ipc);

				Sleep(1000);
				char* initmsg = prepareNewMessage(ipc, 1, 16);
				strcpy(initmsg, "GOT MESSAGE!");
				finishNewMessage(ipc);
				setForSend(ipc);
			}
		}
	}
}