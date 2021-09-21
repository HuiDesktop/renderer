#include <stdio.h>
#include <string.h>
#include "ipc.h"
#include "message.h"

int main() {
	char name[1024];
	scanf_s("%s", name, 1023);
	Ipc* ipc = openIpc(name);
	char *initmsg = prepareNewMessage(ipc, 1, 13);
	strcpy(initmsg, "TEST MESSAGE");
	finishNewMessage(ipc);
	setForSend(ipc);
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
				finishReadMessage(ipc);

				char *initmsg = prepareNewMessage(ipc, 1, 13);
				strcpy(initmsg, "LIKE MESSAGE");
				finishNewMessage(ipc);
				setForSend(ipc);
			}
		}
	}
}