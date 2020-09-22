#include <Windows.h>
#include <iostream>

#include "Memory.h"
#include "Config.h"

Memory* memory;

int main()
{

    SetConsoleTitleA("R6 Unlock All | Shadow Legacy | InsideExploit[UC]");

    DWORD ProcessID = memory->GetProcessID(L"RainbowSix.exe");
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
    unsigned long long base = memory->BaseAddress(ProcessID);

    if (!ProcessID)
    {
        std::cout << "[ERROR] Could not get process id. [-1]\n";
        Sleep(1500);
        exit(-1);
    }

    if (!hProcess)
    {
        std::cout << "[ERROR] Could not handle the process. [-2]\n";
        Sleep(1500);
        exit(-2);
    }

    if (!base)
    {
        std::cout << "[ERROR] Could not get base address. [-3]\n";
        Sleep(1500);
        exit(-3);
    }

    std::cout << "[WARNING] Please use an command.\n\n";
    std::cout << "[INFO] F1: Unlock All.\n";
    std::cout << "[INFO] F2: Disable All.\n";
    std::cout << "[INFO] F3: Normal All.\n";

    std::cout << "" << std::endl;

    while (1)
    {
        if (GetAsyncKeyState(VK_F1) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was unlocked. [5.0]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + Config::Offsets::unlock), Config::Offsets::BytepatchEnable, sizeof(Config::Offsets::BytepatchEnable), NULL);
        }

        if (GetAsyncKeyState(VK_F2) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was blocked.  [5.1]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + Config::Offsets::unlock), Config::Offsets::BytepatchDisable, sizeof(Config::Offsets::BytepatchDisable), NULL);
        }

        if (GetAsyncKeyState(VK_F3) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was resetted. [5.2]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + Config::Offsets::unlock), Config::Offsets::BytepatchNormal, sizeof(Config::Offsets::BytepatchNormal), NULL);
        }
    }
}
