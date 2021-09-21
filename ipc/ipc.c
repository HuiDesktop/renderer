#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "ipc.h"
#include "randomName.h"
#include "windowsSharedMemory.h"

Ipc* createIpc(uint32_t masterSendingBufferLength, uint32_t slaveSendingBufferLength) {
	// length should be 4n
	if (masterSendingBufferLength & 3) {
		masterSendingBufferLength += 4 - (masterSendingBufferLength & 3);
	}
	if (slaveSendingBufferLength & 3) {
		slaveSendingBufferLength += 4 - (slaveSendingBufferLength & 3);
	}
	// calculate total length
	uint32_t memoryBlockSize = sizeof(memoryBlockSize) + sizeof(IpcChannel) * 2 + masterSendingBufferLength + slaveSendingBufferLength;

	// generate name
	Ipc* ipc = malloc(sizeof(Ipc) + sizeof(HANDLE) * 3);
	if (ipc == NULL) return NULL;
	ipc->name[31] = 0;
	setRandomName(ipc->name, ipc->name + 31);

	// create shared memory
	void* ptr;
	if (createSharedMemory(memoryBlockSize, ipc->name, ipc->handleInfo, &ptr) != 0) {
		free(ipc);
		return NULL;
	}

	// init shared memory
	*((uint32_t*)ptr) = memoryBlockSize;
	ipc->send = ((uint8_t*)ptr) + 4;
	ipc->receive = ((uint8_t*)ptr) + 4 + sizeof(IpcChannel) + masterSendingBufferLength;

	// write info
	ipc->send->readPos = ipc->send->writePos = ipc->receive->readPos = ipc->receive->writePos = 0;
	ipc->send->length = masterSendingBufferLength;
	ipc->receive->length = slaveSendingBufferLength;

	// create ipc channel events
	setRandomName(ipc->send->eventName, ipc->send->eventName + 31);
	ipc->send->eventName[31] = 0;
	((HANDLE*)ipc->handleInfo)[1] = createNamedEvent(ipc->send->eventName);
	if (((HANDLE*)ipc->handleInfo)[1] == NULL) {
		CloseHandle(((HANDLE*)ipc->handleInfo)[0]);
		free(ipc);
		return NULL;
	}

	setRandomName(ipc->receive->eventName, ipc->receive->eventName + 31);
	ipc->receive->eventName[31] = 0;
	((HANDLE*)ipc->handleInfo)[2] = createNamedEvent(ipc->receive->eventName);
	if (((HANDLE*)ipc->handleInfo)[2] == NULL) {
		CloseHandle(((HANDLE*)ipc->handleInfo)[0]);
		CloseHandle(((HANDLE*)ipc->handleInfo)[1]);
		free(ipc);
		return NULL;
	}

	return ipc;
}

Ipc* openIpc(const char* name) {
	HANDLE handle;
	void* ptr;
	// try get size
	if (openSharedMemory(sizeof(uint32_t), name, &handle, &ptr) != 0) return NULL;
	uint32_t size = *((uint32_t*)ptr);
	CloseHandle(handle);
	handle = NULL;

	// real open
	Ipc* ipc = malloc(sizeof(Ipc) + sizeof(HANDLE) * 3);
	if (ipc == NULL) return NULL;
	if (openSharedMemory(size, name, ipc->handleInfo, &ptr) != 0) {
		free(ipc);
		return NULL;
	}
	ipc->receive = ((uint8_t*)ptr) + 4;
	ipc->send = ((uint8_t*)ptr) + 4 + sizeof(IpcChannel) + ipc->receive->length;

	// open events
	((HANDLE*)ipc->handleInfo)[1] = createNamedEvent(ipc->send->eventName);
	if (((HANDLE*)ipc->handleInfo)[1] == NULL) {
		CloseHandle(((HANDLE*)ipc->handleInfo)[0]);
		free(ipc);
		return NULL;
	}
	((HANDLE*)ipc->handleInfo)[2] = createNamedEvent(ipc->receive->eventName);
	if (((HANDLE*)ipc->handleInfo)[2] == NULL) {
		CloseHandle(((HANDLE*)ipc->handleInfo)[0]);
		CloseHandle(((HANDLE*)ipc->handleInfo)[1]);
		free(ipc);
		return NULL;
	}

	return ipc;
}

void freeIpc(Ipc* ipc) {
	CloseHandle(((HANDLE*)ipc->handleInfo)[0]);
	CloseHandle(((HANDLE*)ipc->handleInfo)[1]);
	CloseHandle(((HANDLE*)ipc->handleInfo)[2]);
	free(ipc);
}

int waitForRead(Ipc* ipc, uint32_t waitMilliseconds) {
	static_assert(sizeof(waitMilliseconds) == sizeof(DWORD), "Wait milliseconds should change the type");
	if (waitMilliseconds == 0) waitMilliseconds = INFINITE;
	DWORD ret = WaitForSingleObject(((HANDLE*)ipc->handleInfo)[2], waitMilliseconds);
	return ret == WAIT_OBJECT_0;
}

void setForSend(Ipc* ipc) {
	SetEvent(((HANDLE*)ipc->handleInfo)[1]);
}
