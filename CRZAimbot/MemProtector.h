#pragma once

#include <Windows.h>
#include <intrin.h>
#include "Config.h"
#pragma intrinsic(_ReturnAddress)

#define SLog(text) //MessageBox(NULL, text, L"SUPER DEBUG", 0);

struct ProtectedFunction {
	void* address;
	size_t size;
	BYTE lastXor;
	bool crypted;
};
extern int funcCount;
extern ProtectedFunction functions[50];
void addFunc(ProtectedFunction func);
void unsafe_unprotect(int index);
void unsafe_protect(int index);
int GetFunctionIndex(void* FunctionAddress);
void Unprotect(void* FunctionAddress);
void Protect(void* FunctionAddress);
void XOR(BYTE* data, size_t size, BYTE XOR_KEY = STRING_XOR_KEY);
void ProtectedSleep(int ms);