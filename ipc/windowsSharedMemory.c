#include <Windows.h>
#include "windowsSharedMemory.h"

int createSharedMemory(DWORD size, LPCSTR name, HANDLE* hMapFile, void** pPtr)
{
	*hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		size,               	 // maximum object size (low-order DWORD)
		name);                   // name of mapping object

	if (*hMapFile == NULL)
	{
		return 1;
	}

	*pPtr = MapViewOfFile(*hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS,           // read/write permission
		0,
		0,
		size);

	if (*pPtr == NULL)
	{
		CloseHandle(*hMapFile);
		*hMapFile = 0;
		return 2;
	}
	return 0;
}

int openSharedMemory(DWORD size, LPCSTR name, HANDLE* hMapFile, void** pPtr)
{
	*hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name);

	if (*hMapFile == NULL)
	{
		return 1;
	}

	*pPtr = MapViewOfFile(*hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		size);

	if (*pPtr == NULL)
	{
		CloseHandle(*hMapFile);
		*hMapFile = 0;
		return 2;
	}
	return 0;
}

HANDLE createNamedEvent(LPCSTR name)
{
	return CreateEvent(NULL, FALSE, FALSE, name);
}

HANDLE openNamedEvent(LPCSTR name)
{
	return OpenEvent(NULL, FALSE, name);
}
