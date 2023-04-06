#pragma once
#include <iostream> 
#include <vector> 
#include <windows.h>
namespace mem {
	void patchEx(BYTE* dst, BYTE* src, unsigned int, HANDLE hProcess);
	void nopEx(BYTE* dst, unsigned int size, HANDLE hProcess);
}
