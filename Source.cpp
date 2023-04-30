#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>
#include "memory.h"
#include <string>
#include <fstream>
#include <thread>
#include <chrono>



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
int main(){
    SetConsoleTitle(L"STAR RAIL FPS UNLOCKER BY ELJoOker#8401");

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
        HMODULE LoadliBriray = LoadLibraryA("DLL.dll");
        if (LoadliBriray)
        {
            std::cout << "successfully injected dll\n";
        }
        else
        {
            std::cout << "failed injecting dll\n";
        }

        std::cout << "Made with love by ELJoOker#8401\n\n\n";
        Sleep(2000);
        const wchar_t* processName = L"StarRail.exe";
        std::cout << "Waiting for process to start...\n";
        // Wait for the process to start
        if (!IsProcessRunning(processName)) {
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
            }
            else {
                std::cout << "unknown error\n";
            }
            // Get the current directory
            char currentDirectory[MAX_PATH];
            GetModuleFileNameA(NULL, currentDirectory, MAX_PATH);
            std::string currentDirectoryStr(currentDirectory);
            std::size_t found = currentDirectoryStr.find_last_of("\\/");
            std::string path = currentDirectoryStr.substr(0, found + 1);

            // Process ID of the target process to attach to
           // Name of the process to wait for

             /*   // Specify the name of the executable to run
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

            }
            */
            DWORD procId = GetProcId(L"StarRail.exe");

            int fps = 144;
            const int hotkey = 0x52;
            if (procId) {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
                std::cout << "Successfully attached to the process." << std::endl;

                // Perform operations on the attached process here
                Sleep(10000);
                uintptr_t address = GetModuleBaseAddress(procId, L"unityplayer.dll") + 0x1C4E000;  //unityplayer.dll+1C4E000
                Write<int>(LPVOID(address), fps);
                std::cout << "Successfully wrote value to memory.\ncurrent fps is " << fps << std::endl;
                std::cout << "hold R key to spam space" << std::endl;
                // std::cout << "exiting in 3 seconds\n";
                // Sleep(3000);
                std::thread inputThread(InputThread, std::ref(fps));
                inputThread.detach();
                while (true) {
                    int value1 = Read<int>(LPVOID(address));
                    if (value1 != fps) {
                        Write<int>(LPVOID(address), fps);
                    }

                    // Check if the hotkey is pressed
                    if (GetAsyncKeyState(hotkey) & 0x8000)
                    {
                        // Simulate a left mouse button click
                        keybd_event(VK_SPACE, 0, 0, 0); // Press the "Space" key
                        keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Release the "Space" key

                    }
                    DWORD procId = GetProcId(L"StarRail.exe");
                    if (procId == 0) {
                        std::cout << "Star Rail has been closed" << std::endl;
                        exit(0);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }

            }

        }
        else
        {
            std::cout << "proccess already running , can't inject bypass" << std::endl;
            std::cout << "normal functions will work anyway" << std::endl;
            DWORD procId = GetProcId(L"StarRail.exe");

            int fps = 144;
            const int hotkey = 0x52;
            if (procId) {
                hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
                std::cout << "Successfully attached to the process." << std::endl;

                // Perform operations on the attached process here
                Sleep(10000);
                uintptr_t address = GetModuleBaseAddress(procId, L"unityplayer.dll") + 0x1C4E000;  //unityplayer.dll+1C4E000
                Write<int>(LPVOID(address), fps);
                std::cout << "Successfully wrote value to memory.\ncurrent fps is " << fps << std::endl;
                std::cout << "hold R key to spam space" << std::endl;
                // std::cout << "exiting in 3 seconds\n";
                // Sleep(3000);
                std::thread inputThread(InputThread, std::ref(fps));
                inputThread.detach();
                while (true) {
                    int value1 = Read<int>(LPVOID(address));
                    if (value1 != fps) {
                        Write<int>(LPVOID(address), fps);
                    }

                    // Check if the hotkey is pressed
                    if (GetAsyncKeyState(hotkey) & 0x8000)
                    {
                        // Simulate a left mouse button click
                        keybd_event(VK_SPACE, 0, 0, 0); // Press the "Space" key
                        keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Release the "Space" key

                    }
                    DWORD procId = GetProcId(L"StarRail.exe");
                    if (procId == 0) {
                        std::cout << "Star Rail has been closed" << std::endl;
                        exit(0);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }

            }

        }
        
    }


	return 0;
}