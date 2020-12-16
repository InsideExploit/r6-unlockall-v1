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
        configW << "  \"xorOffset\": 45040223,\n";
        configW << "  \"cmpOffset\": 45040232,\n";
        configW << "  \"movOffset\": 45040162,\n";
        configW << "  \"Enable\": 112,\n";
        configW << "  \"Normal\": 113,\n";
        configW << "  \"Version\": 3.5\n";
        configW << "}";

        configW.close();

        Sleep(300);

        std::cout << "[SUCCESS] The json config file got created... please restart application.\n";

        Sleep(500);

        _exit(false);
    }

    configR >> js;
    
    uintptr_t xorOffs = js["xorOffset"];
    uintptr_t cmpOffs = js["cmpOffset"];
    uintptr_t movOffs = js["movOffset"];

    int hotkeyEnable = js["Enable"];
    int hotkeyNormal = js["Normal"];
    float version = js["Version"];

    if (version != Config::Program::version)
    {
        std::cout << "[-] Please delete the old config.json";
        exit(-7);
    }

    SetConsoleTitleA("R6 Unlock All | Neon Dawn | InsideExploit[UC]");

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
    std::cout << "[HOTKEY] F2: Normal All.\n";
    
    std::cout << "\n[IMPORTANT] Remember, that one showed key are the default one.\n";


    std::cout << "" << std::endl;


    while (1)
    {
        if (GetAsyncKeyState(hotkeyEnable) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was unlocked. [5.0]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + xorOffs), Config::Offsets::bytepatch_xor_enb, Config::Offsets::xSize, NULL);
            WriteProcessMemory(hProcess, (LPVOID)(base + cmpOffs), Config::Offsets::bytepatch_cmp_enb, Config::Offsets::xSize, NULL);
            WriteProcessMemory(hProcess, (LPVOID)(base + movOffs), Config::Offsets::bytepatch_mov_enb, Config::Offsets::xSize, NULL);
        }

        if (GetAsyncKeyState(hotkeyNormal) & 0x1)
        {
            std::cout << "[SUCCESS] Everything was resetted. [5.2]\n";
            WriteProcessMemory(hProcess, (LPVOID)(base + xorOffs), Config::Offsets::bytepatch_xor_dis, Config::Offsets::xSize, NULL);
            WriteProcessMemory(hProcess, (LPVOID)(base + cmpOffs), Config::Offsets::bytepatch_cmp_dis, Config::Offsets::xSize, NULL);
            WriteProcessMemory(hProcess, (LPVOID)(base + movOffs), Config::Offsets::bytepatch_mov_dis, Config::Offsets::xSize, NULL);
        }
    }
}
