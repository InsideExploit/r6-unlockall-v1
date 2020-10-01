#include <Windows.h>
#include <iostream>

#include "Memory.h"
#include "Config.h"

#include <fstream>
#include <filesystem>
#include <direct.h>

#include "Json.h"
using namespace nlohmann;

Memory* memory;

std::string CurrentPath()
{
    char* cwd = _getcwd(0, 0);
    std::string directory(cwd);
    std::free(cwd);
    return directory;
}

int main()
{
    json js;

    std::string configPath = CurrentPath() + "\\config.json";

    std::ifstream configR(configPath);

    if (!configR)
    {

        std::cout << "[LOADING] Creating the json config file because is missing...\n";

        Sleep(300);

        std::ofstream configW;
        configW.open(configPath);
        configW << "{\n";
        configW << "  \"Offset\": 54160164,\n";
        configW << "  \"Enable\": 112,\n";
        configW << "  \"Disable\": 113,\n";
        configW << "  \"Normal\": 114\n";
        configW << "}";

        configW.close();

        Sleep(300);

        std::cout << "[SUCCESS] The json config file got created... please restart application.\n";
    }

    configR >> js;
    
    uintptr_t offsConfig = js["Offset"];

    uintptr_t hotkeyEnable = js["Enable"];
    uintptr_t hotkeyDisable = js["Disable"];
    uintptr_t hotkeyNormal = js["Normal"];

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

    std::cout << "[INFO] Please use an command.\n\n";
    std::cout << "[HOTKEY] F1: Unlock All.\n";
    std::cout << "[HOTKEY] F2: Disable All.\n";
    std::cout << "[HOTKEY] F3: Normal All.\n";

    std::cout << "\n[IMPORTANT] Remember, that one showed key are the default one.\n";


    std::cout << "" << std::endl;

    while (1)
    {
        if (GetAsyncKeyState(hotkeyEnable) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was unlocked. [5.0]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + offsConfig), Config::Offsets::BytepatchEnable, sizeof(Config::Offsets::BytepatchEnable), NULL);
        }

        if (GetAsyncKeyState(hotkeyDisable) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was blocked.  [5.1]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + offsConfig), Config::Offsets::BytepatchDisable, sizeof(Config::Offsets::BytepatchDisable), NULL);
        }

        if (GetAsyncKeyState(hotkeyNormal) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was resetted. [5.2]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + offsConfig), Config::Offsets::BytepatchNormal, sizeof(Config::Offsets::BytepatchNormal), NULL);
        }
    }
    
}
