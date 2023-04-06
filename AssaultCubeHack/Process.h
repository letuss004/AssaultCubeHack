#pragma once
#include <iostream>
#include <vector>
#include <windows.h>
#include <TlHelp32.h>

DWORD getProcessdId(const wchar_t* procName);
uintptr_t getModuleBaseAddress(DWORD procId, const wchar_t* modName);
uintptr_t findDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>);

class Process {};