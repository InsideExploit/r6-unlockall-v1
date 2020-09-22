#include <Windows.h>
#include <TlHelp32.h>
#include <PsApi.h>
#include <string>

class Memory
{
public:
	unsigned long long BaseAddress(DWORD pid);
	DWORD GetProcessID(const std::wstring& pName);
};

