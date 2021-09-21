#include "ipc.h"

#define MESSAGE_LEN_MASK ((uint32_t)0x00ffffff)
#define MESSAGE_SKIP_TAIL_TYPE ((uint8_t)0xfe)

typedef struct IpcMessage {
	uint32_t typeAndLength;
	uint8_t msg[0];
} IpcMessage;

uint32_t messageLen(IpcMessage* message);

uint8_t messageType(IpcMessage* message);

void* prepareNewMessage(Ipc* ipc, uint8_t type, uint32_t length);

void finishNewMessage(Ipc* ipc);

void skipTail(Ipc* ipc);

IpcMessage* peekMessage(Ipc* ipc);

void finishReadMessage(Ipc* ipc);
