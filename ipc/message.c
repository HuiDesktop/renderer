#include "message.h"

uint32_t messageLen(IpcMessage* message) {
	return (message->typeAndLength & MESSAGE_LEN_MASK) << 2;
}

uint8_t messageType(IpcMessage* message) {
	return message->typeAndLength >> 24;
}

void setMessageTypeAndLength(IpcMessage* message, uint8_t type, uint32_t length) {
	message->typeAndLength = type;
	message->typeAndLength <<= 24;
	message->typeAndLength |= length >> 2;
}

void* prepareNewMessage(Ipc *ipc, uint8_t type, uint32_t length) {
	// length should be 4n
	if (length & 3) {
		length += 4 - (length & 3);
	}

	if (ipc->send->writePos + length + 4 >= ipc->send->length) {
		IpcMessage* message = ipc->send->data + ipc->send->writePos;
		setMessageTypeAndLength(message, MESSAGE_SKIP_TAIL_TYPE, 0);
		ACCESS_ONCE(ipc->send->writePos) = 0;
	}
	IpcMessage* message = ipc->send->data + ipc->send->writePos;
	setMessageTypeAndLength(message, type, length);
	return ipc->send->data + ipc->send->writePos + 4;
}

void finishNewMessage(Ipc* ipc) {
	uint32_t newPos = ipc->send->writePos + messageLen(ipc->send->data + ipc->send->writePos) + 4;
	if (newPos >= ipc->send->length) newPos = 0;
	ACCESS_ONCE(ipc->send->writePos) = newPos;
}

void skipTail(Ipc* ipc) {
	IpcMessage* oldMessage = ipc->send->data + ipc->send->writePos;
	IpcMessage* newMessage = ipc->send->data;
}

IpcMessage* peekMessage(Ipc* ipc) {
	if (ipc->receive->readPos == ACCESS_ONCE(ipc->receive->writePos)) return NULL;
	IpcMessage *msg = ipc->receive->data + ipc->receive->readPos;
	if (messageType(msg) == MESSAGE_SKIP_TAIL_TYPE) {
		ACCESS_ONCE(ipc->receive->readPos) = 0;
		return peekMessage(ipc);
	}
	return msg;
}

void finishReadMessage(Ipc* ipc) {
	uint32_t newPos = ipc->receive->readPos + messageLen(ipc->receive->data + ipc->receive->readPos) + 4;
	if (newPos >= ipc->receive->length) newPos = 0;
	ACCESS_ONCE(ipc->receive->readPos) = newPos;
}
