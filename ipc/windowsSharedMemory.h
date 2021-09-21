#include <Windows.h>

/// <summary>
/// Create a shared memory view.
/// Caller should release the handle.
/// </summary>
/// <param name="size"></param>
/// <param name="name"></param>
/// <param name="hMapFile"></param>
/// <param name="pPtr"></param>
/// <returns>0:OK; 1: Could not create file mapping object; 2:Could not map view of file.</returns>
int createSharedMemory(DWORD size, LPCSTR name, HANDLE* hMapFile, void** pPtr);

/// <summary>
/// Open a shared memory view.
/// Caller should release the handle.
/// </summary>
/// <param name="size"></param>
/// <param name="name"></param>
/// <param name="hMapFile"></param>
/// <param name="pPtr"></param>
/// <returns>0:OK; 1: Could not open file mapping object; 2:Could not map view of file.</returns>
int openSharedMemory(DWORD size, LPCSTR name, HANDLE* hMapFile, void** pPtr);

HANDLE createNamedEvent(LPCSTR name);

HANDLE openNamedEvent(LPCSTR name);
