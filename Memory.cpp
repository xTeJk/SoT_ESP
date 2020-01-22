#include "Memory.h"
#include <TlHelp32.h>


CMemory* Mem = new CMemory(); 

CMemory::CMemory()
{
}

CMemory::~CMemory()
{
	CloseHandle(hProcess);
}

HANDLE CMemory::Process(const char* ProcessName) 
{
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); 
	PROCESSENTRY32 ProcEntry;
	ProcEntry.dwSize = sizeof(ProcEntry);

	do
		if (!strcmp(ProcEntry.szExeFile, ProcessName)) 
		{
			PID = ProcEntry.th32ProcessID;
			CloseHandle(hPID); 

			return hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
			
		}
	while (Process32Next(hPID, &ProcEntry)); 

}

bool CMemory::DataCompare(BYTE* data, BYTE* sign, char* mask)
{
	for (; *mask; mask++, sign++, data++)
	{
		if (*mask == 'x' && *data != *sign)
		{
			return false;
		}
	}
	return true;
}

uintptr_t CMemory::FindSignature(uintptr_t base, uintptr_t size, BYTE* sign, char* mask)
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	uintptr_t offset = 0;
	while (offset < size)
	{
		VirtualQueryEx(hProcess, (LPCVOID)(base + offset), &mbi, sizeof(MEMORY_BASIC_INFORMATION));
		if (mbi.State != MEM_FREE)
		{
			BYTE* buffer = new BYTE[mbi.RegionSize];
			ReadProcessMemory(hProcess, mbi.BaseAddress, buffer, mbi.RegionSize, NULL);
			for (int i = 0; i < mbi.RegionSize; i++)
			{
				if (DataCompare(buffer + i, sign, mask))
				{
					delete[] buffer;
					return (uintptr_t)mbi.BaseAddress + i;
				}
			}

			delete[] buffer;
		}
		offset += mbi.RegionSize;
	}
	return 0;
}

uintptr_t CMemory::Module(const char* ModuleName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
		if (!strcmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);
			return (uintptr_t)mEntry.modBaseAddr;
		}
	while (Module32Next(hModule, &mEntry));

	return 0;
}

uintptr_t CMemory::ModuleSize(const char* ModuleName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);

	do
		if (!strcmp(mEntry.szModule, ModuleName))
		{
			CloseHandle(hModule);
			return (uintptr_t)mEntry.modBaseSize;
		}
	while (Module32Next(hModule, &mEntry));

	return 0;
}