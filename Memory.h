#pragma once
#include <Windows.h>

class CMemory
{
public:
	CMemory();
	~CMemory();

	uintptr_t PID = 0;
	HANDLE hProcess = 0;

	template <class cData>
	cData Read(uintptr_t dwAddress)
	{
		cData cRead;
		ReadProcessMemory(hProcess, (LPVOID)dwAddress, &cRead, sizeof(cData), NULL); //uses handle to read
		return cRead;
	}

	template <class cData>
	void Write(uintptr_t dwAddress, cData write)
	{
		WriteProcessMemory(hProcess, (LPVOID)dwAddress, &write, sizeof(cData), NULL); //uses handle to write
	}

	virtual HANDLE  Process(const char* ProcessName);
	virtual uintptr_t Module(const char* ModuleName);
	virtual uintptr_t ModuleSize(const char* ModuleName);
	virtual bool  DataCompare(BYTE* data, BYTE* sign, char* mask);
	virtual uintptr_t FindSignature(uintptr_t base, uintptr_t size, BYTE* sign, char* mask);
};

extern CMemory* Mem;