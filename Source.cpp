#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include "memory.h"
#include <string>
#include <fstream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <vector>
/*
void InputThread(int& fps) {
    while (true) {
        std::cout << "fps now is " << fps << ". Enter a new value or leave it blank to keep the current value: ";
        std::string input;
        std::getline(std::cin, input);
        if (!input.empty()) {
            try {
                fps = std::stoi(input);
                std::cout << "New fps value set to " << fps << std::endl;
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
}
*/

void dllInjection() {
    std::string files[] = { "dll_injector64.exe", "star_rail.dll" };
    bool bypass_files_exist = true;

    for (const auto& file_name : files) {
        std::ifstream file(file_name);
        if (!file.good()) {
            bypass_files_exist = false;
            std::cerr << "Error: '" << file_name << "' not found" << std::endl;
        }
    }
    if (bypass_files_exist) {
        // all files exist, execute the command
        system("dll_injector64.exe /dll star_rail.dll /target StarRail.exe");
        std::cout << "successfully excuted all commands\n";
        Sleep(500);
    }
    else {
        std::cout << "unknown error\n";
    }
}
void MainRoutine() {
    system("cls");
    Sleep(1000);
    std::cout << "Made with love by ELJoOker#8401\n\n\n";
    Sleep(2000);
    DWORD procId = GetProcId(L"StarRail.exe");


    uintptr_t UnityPlayer = GetModuleBaseAddress(procId, L"unityplayer.dll"); //"unityplayer.dll" + 0x1D21D78
    uintptr_t GameAssembly = GetModuleBaseAddress(procId, L"GameAssembly.dll"); //"unityplayer.dll" + 0x1D21D78

    int fps = 144;
    
    if (procId) {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
        std::cout << "Successfully attached to the process." << "\n"<< std::endl;

        // Perform operations on the attached process here
       
        uintptr_t fpsAddress = GetModuleBaseAddress(procId, L"unityplayer.dll") + 0x1C4E000;  //unityplayer.dll+1C4E000
        Write<int>(LPVOID(fpsAddress), fps);
        
        
        
        std::cout << "functions:" << std::endl;
        std::cout << "press Insert to toggle dialoge skip" << std::endl;
        std::cout << "press Home to change fps value (default is 144)"; std::cout << " current fps is " << fps  << std::endl;
        std::cout << "press END to toggle game speed" << std::endl;
        std::cout << "press f9 to enable peeking" << std::endl;
        std::cout << "press f10 to change custom profile" << std::endl;
        std::cout << "press f2 while in battle to unlock battle speedup" << std::endl;
        std::cout << "press f12 for fast close game" << std::endl;
        bool spaceSpam = false;
        bool gamespeedBool=false;
        float gamespeed = 2.0f;
        bool peeking = false;
        bool lastgamespeed = false;
        while (true) {
            //fps unlocker functions
            
            int value1 = Read<int>(LPVOID(fpsAddress));
            if (value1 != fps) {

                Write<int>(LPVOID(fpsAddress), fps);
            }
            if (GetAsyncKeyState(VK_HOME) & 0x8000) {
                std::cout << "enter fps:";
                std::cin >> fps;

            }
            if (GetAsyncKeyState(VK_F12) & 0x8000) {
                if (TerminateProcessByName(L"StarRail.exe")) {
                    std::cout << "StarRail.exe terminated successfully\n";
                }
                else {
                    std::cerr << "Could not terminate StarRail.exe\n";
                }
                Sleep(500);
                
            }
            //peeking function --start
            if (GetAsyncKeyState(VK_F9) & 0x8000) {
                peeking = !peeking;
                std::cout << "peeking is " << (peeking ? "on" : "off") <<  std::endl;
                Sleep(500);
            }
            if (peeking) {
                uintptr_t peekingAdress = GetModuleBaseAddress(procId, L"gameassembly.dll") + 0x51292C0;
                Write<BYTE>(LPVOID(peekingAdress), 0xC3);
               
           }
            else {
                uintptr_t peekingAdress = GetModuleBaseAddress(procId, L"gameassembly.dll") + 0x51292C0;
                Write<BYTE>(LPVOID(peekingAdress), 0x40);
                
            }
            //peeking function --end
             //global game speed function --start
            if (gamespeedBool && GetAsyncKeyState(VK_UP) & 0x8000) {
                gamespeed = gamespeed + 1;
                std::cout << "game speed now is " << gamespeed << std::endl;
                Sleep(500);
            }
            else if (gamespeedBool && GetAsyncKeyState(VK_DOWN) & 0x8000) {
                gamespeed = gamespeed - 1;
                std::cout << "game speed now is " << gamespeed << std::endl;
                Sleep(500);
            }

            {
                
                uintptr_t offsetGameToGlobalSpeedBaseAdress = 0x1D21D78;
                std::vector<uintptr_t> pointsOffsets{ 0xFC };
                uintptr_t baseAddress = NULL;
                ReadProcessMemory(hProcess, (LPVOID)(UnityPlayer + offsetGameToGlobalSpeedBaseAdress), &baseAddress, sizeof(baseAddress), NULL);
                // std::cout << "debugginfo: baseaddress = " << std::hex << baseAddress << std::endl;
                uintptr_t pointsAddress = baseAddress; //the Adress we need -> change now while going through offsets
                for (int i = 0; i < pointsOffsets.size() - 1; i++) // -1 because we dont want the value at the last offset
                {
                    ReadProcessMemory(hProcess, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
                    // std::cout << "debugginfo: Value at offset = " << std::hex << pointsAddress << std::endl;
                }
                pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);

                if (GetAsyncKeyState(VK_END) & 0x8000) {
                    gamespeedBool = !gamespeedBool;
                    std::cout << "game speed " << (gamespeedBool ? "on hold \"LSHIFT\" to use , use arrows up and down to change the speed value" : "off") << std::endl;
                    Sleep(500);
                }
                
                if (gamespeedBool && GetAsyncKeyState(VK_LSHIFT) & 0x8000) {

                    gamespeed;
                    DWORD oldProtect;
                    LPVOID address = (LPVOID)(pointsAddress);
                    SIZE_T size = sizeof(gamespeed);

                    // Change the memory protection to allow writing
                    if (!VirtualProtectEx(hProcess, address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                        // Handle error
                    }

                    // Write the value to the process memory
                    if (!WriteProcessMemory(hProcess, address, &gamespeed, size, NULL)) {
                        // Handle error
                    }

                    // Restore the original memory protection
                    if (!VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect)) {
                        // Handle error
                    }


                    Sleep(100);
                }
                if (gamespeedBool && !GetAsyncKeyState(VK_LSHIFT)){
                    float normalgamespeed = 1;
                    DWORD oldProtect;
                    LPVOID address = (LPVOID)(pointsAddress);
                    SIZE_T size = sizeof(normalgamespeed);

                    // Change the memory protection to allow writing
                    if (!VirtualProtectEx(hProcess, address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                        // Handle error
                    }

                    // Write the value to the process memory
                    if (!WriteProcessMemory(hProcess, address, &normalgamespeed, size, NULL)) {
                        // Handle error
                    }

                    // Restore the original memory protection
                    if (!VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect)) {
                        // Handle error
                    }
                }
            }
            //global game speed function --end
            
             //dialoge skip functions --start

             {
                uintptr_t offsetGameToGlobalSpeedBaseAdress = 0x1D21D78;
                std::vector<uintptr_t> pointsOffsets{ 0xFC };
                uintptr_t baseAddress = NULL;
                ReadProcessMemory(hProcess, (LPVOID)(UnityPlayer + offsetGameToGlobalSpeedBaseAdress), &baseAddress, sizeof(baseAddress), NULL);
                // std::cout << "debugginfo: baseaddress = " << std::hex << baseAddress << std::endl;
                uintptr_t pointsAddress = baseAddress; //the Adress we need -> change now while going through offsets
                for (int i = 0; i < pointsOffsets.size() - 1; i++) // -1 because we dont want the value at the last offset
                {
                    ReadProcessMemory(hProcess, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
                    // std::cout << "debugginfo: Value at offset = " << std::hex << pointsAddress << std::endl;
                }
                pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);
            

            if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
                spaceSpam = !spaceSpam;
                std::cout << "dialoge skip is " << (spaceSpam ? "on hold \"R\" or LCLICK to use" : "off") <<  std::endl;
                Sleep(500);
            }

            if (spaceSpam && GetAsyncKeyState(0x52) & 0x8000|| spaceSpam && GetAsyncKeyState(VK_RBUTTON) & 0x8000)
            {
                if (gamespeedBool) {
                    gamespeedBool = false;
                    lastgamespeed = true;
                }
                keybd_event(VK_SPACE, 0, 0, 0); // Press the "Space" key
                keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Release the "Space" key
                Sleep(10);

            }
            if (spaceSpam && GetAsyncKeyState(0x52) & 0x8000)
            {
                float DialogSpeed = 15.f;
                DWORD oldProtect;
                LPVOID address = (LPVOID)(pointsAddress);
                SIZE_T size = sizeof(DialogSpeed);

                // Change the memory protection to allow writing
                if (!VirtualProtectEx(hProcess, address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                    // Handle error
                }

                // Write the value to the process memory
                if (!WriteProcessMemory(hProcess, address, &DialogSpeed, size, NULL)) {
                    // Handle error
                }

                // Restore the original memory protection
                if (!VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect)) {
                    // Handle error
                }

            }
           if(spaceSpam && !GetAsyncKeyState(0x52)) {
                float normalgamespeed = 1;
                DWORD oldProtect;
                LPVOID address = (LPVOID)(pointsAddress);
                SIZE_T size = sizeof(normalgamespeed);

                // Change the memory protection to allow writing
                if (!VirtualProtectEx(hProcess, address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
                    // Handle error
                }

                // Write the value to the process memory
                if (!WriteProcessMemory(hProcess, address, &normalgamespeed, size, NULL)) {
                    // Handle error
                }

                // Restore the original memory protection
                if (!VirtualProtectEx(hProcess, address, size, oldProtect, &oldProtect)) {
                    // Handle error
                }
                if (lastgamespeed) {
                    gamespeedBool = true;
                }
                

            }

            }
            //dialoge skip functions --end
            
            
           
             
            //battle speed

            {
                uintptr_t offsetGameToBattleSpeedBaseAdress = 0x8CAA6A0;
                std::vector<uintptr_t> pointsOffsets{ 0xC0 , 0x1DC };
                uintptr_t baseAddress = NULL;
                ReadProcessMemory(hProcess, (LPVOID)(GameAssembly + offsetGameToBattleSpeedBaseAdress), &baseAddress, sizeof(baseAddress), NULL);
                // std::cout << "debugginfo: baseaddress = " << std::hex << baseAddress << std::endl;
                uintptr_t pointsAddress = baseAddress; //the Adress we need -> change now while going through offsets
                for (int i = 0; i < pointsOffsets.size() - 1; i++) // -1 because we dont want the value at the last offset
                {
                    ReadProcessMemory(hProcess, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
                    // std::cout << "debugginfo: Value at offset = " << std::hex << pointsAddress << std::endl;
                }
                pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);

                int value1 = Read<int>(LPVOID(pointsAddress));
                if (value1 == 2) {
                    Write<int>(LPVOID(pointsAddress), 50);
                }
                


            if (GetAsyncKeyState(VK_F2) & 0x8000) {
               
                float battlespeed = 100.f;//Add Last offset -> done!!
                WriteProcessMemory(hProcess, (LPVOID)(pointsAddress), &battlespeed, 8, 0);

                //write(BattleSpeedAddress, 12.f);
                std::cout << "battle speed triggered" << std::endl;
                Sleep(1000);
            }
            }
            {
                if (GetAsyncKeyState(VK_F10) & 0x8000) {
                    uintptr_t offsetGameTProgileBaseAdress = 0x8CAA6A0;
                    std::vector<uintptr_t> MainProfile{ 0xC0 , 0x110 , 0x20 , 0x10 };
                    std::vector<uintptr_t> ProfileOffsets{ 0x20 , 0x28 , 0x4C , 0x24 };
                    uintptr_t baseAddress = NULL;
                    ReadProcessMemory(hProcess, (LPVOID)(GameAssembly + offsetGameTProgileBaseAdress), &baseAddress, sizeof(baseAddress), NULL);
                    uintptr_t pointsAddress = baseAddress;
                    for (int i = 0; i < MainProfile.size(); i++)
                    {
                        ReadProcessMemory(hProcess, (LPVOID)(pointsAddress + MainProfile.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
                    }
                    std::vector<uintptr_t> finalAddresses;
                    for (int i = 0; i < ProfileOffsets.size(); i++)
                    {
                        uintptr_t finalAddress = pointsAddress + ProfileOffsets.at(i);
                        finalAddresses.push_back(finalAddress);
                    }
                    // Define the unique values to write to each final address
                    
                    int intValue = 10;
                    unsigned long long ullValue = static_cast<unsigned long long>(intValue);

                    uintptr_t uid, worldlevel, xp, playerlevel;
                    std::cout << "type ur custom profile stats:" << std::endl;
                    std::cout << "uid:"; std::cin >> uid;
                    std::cout << "worldlevel (bugged):"; std::cin >> worldlevel;
                    std::cout << "xp:"; std::cin >> xp;
                    std::cout << "playerlevel:"; std::cin >> playerlevel; 

                    std::vector<uintptr_t> valuesToWrite{ uid, worldlevel, xp, playerlevel };
                    for (int i = 0; i < finalAddresses.size(); i++)
                    {
                        uintptr_t finalAddress = finalAddresses.at(i);
                        uintptr_t valueToWrite = valuesToWrite.at(i);
                        WriteProcessMemory(hProcess, (LPVOID)finalAddress, &valueToWrite, sizeof(valueToWrite), NULL);
                    }


                }}

            DWORD procId = GetProcId(L"StarRail.exe");
            if (procId == 0) {
                std::cout << "Star Rail has been closed" << std::endl;
                exit(0);
            }
           
        }

    }
}
int main(){
    SetConsoleTitle(L"STAR RAIL MENU BY ELJoOker#8401");

    std::string files[] = { "kdu.exe", "Driver.sys", "drv64.dll" , "DLL.dll"};
    bool kdu_files_exist = true;

    for (const auto& file_name : files) {
        std::ifstream file(file_name);
        if (!file.good()) {
            kdu_files_exist = false;
            std::cerr << "Error: '" << file_name << "' not found" << std::endl;
        }
    }

    if (kdu_files_exist) {
        // all files exist, execute the command
        system("kdu.exe -dse 6");
        system("kdu.exe -map Driver.sys");
        system("cls");
        std::cout << "successfully excuted all commands\n";
        Sleep(500);
        HMODULE LoadliBriray = LoadLibraryA("DLL.dll");
        if (LoadliBriray)
        {
            std::cout << "successfully injected dll\n";
            Sleep(500);
        }
        else
        {
            std::cout << "failed injecting dll\n";
        }
        
       
        std::cout << "Waiting for process to start...\n";
        const wchar_t* processName = L"StarRail.exe";
        // Wait for the process to start
        if (!IsProcessRunning(processName)) {
            //dllInjection();
            // Get the current directory
            char currentDirectory[MAX_PATH];
            GetModuleFileNameA(NULL, currentDirectory, MAX_PATH);
            std::string currentDirectoryStr(currentDirectory);
            std::size_t found = currentDirectoryStr.find_last_of("\\/");
            std::string path = currentDirectoryStr.substr(0, found + 1);

            // Process ID of the target process to attach to
           // Name of the process to wait for

                // Specify the name of the executable to run
            std::string exeName = "StarRail.exe";

            // Build the full path to the executable
            std::string exePath = path + exeName;

            // Start the process
            STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
        
            if (CreateProcessA(NULL, const_cast<char*>(exePath.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
            {
                std::cout << "Process started successfully.\n";
                // Optionally, you can wait for the process to finish using WaitForSingleObject
                //WaitForSingleObject(pi.hProcess, INFINITE);
                CloseHandle(pi.hProcess);
                CloseHandle(pi.hThread);
            }
            else
            {
                std::cerr << "Failed to start process. Error code: " << GetLastError() << "\n";
                return 1;
            }

        

            MainRoutine();
        
        }
        else
        {
            std::cout << "proccess already running , can't inject bypass" << std::endl;
            std::cout << "normal functions will work anyway" << std::endl;
            Sleep(2000);
            MainRoutine();

        }
        
    }


	return 0;
}