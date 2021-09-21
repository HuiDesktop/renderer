#pragma once
#include <stdint.h>

#define ACCESS_ONCE(x) (x)

typedef struct IpcChannel {
	uint8_t eventName[32];
	uint32_t length;
	volatile uint32_t readPos;
	volatile uint32_t writePos;
	uint8_t data[0];
} IpcChannel;

typedef struct Ipc {
	IpcChannel* send;
	IpcChannel* receive;
	char name[32];
	char handleInfo[0];
} Ipc;

/// <summary>
/// Create IPC channels as master
/// If you want to be the slave, swap send&receive and (HANDLE*)handleInfo[1]&[2]
/// Release the ipc, using <c>freeIpc</c>
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
Ipc* createIpc(uint32_t masterSendingBufferLength, uint32_t slaveSendingBufferLength);

/// <summary>
/// Open IPC channels as slave
/// If you want to be the master, swap send&receive and (HANDLE*)handleInfo[1]&[2]
/// Release the ipc, using <c>freeIpc</c>
/// </summary>
/// <param name="name"></param>
/// <returns></returns>
Ipc* openIpc(const char* name);

/// <summary>
/// Release all handles and free the memory.
/// </summary>
/// <param name="ipc"></param>
void freeIpc(Ipc* ipc);

/// <summary>
/// Wait there
/// </summary>
/// <param name="ipc"></param>
/// <param name="waitMilliseconds">0 as inf</param>
/// <returns>0: timeout; 1: got</returns>
int waitForRead(Ipc* ipc, uint32_t waitMilliseconds);

void setForSend(Ipc* ipc);
