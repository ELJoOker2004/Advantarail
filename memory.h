#include <Windows.h>
#include <iostream>
#include <tlhelp32.h>

#ifndef MEMORY_H
#define MEMORY_H
HANDLE hProcess;

template <typename T>
T Read(LPVOID Address)
{
    T Data;
    ReadProcessMemory(hProcess, (LPVOID)Address, &Data, sizeof(T), nullptr);
    return Data;
}

template <typename T>
void Write(LPVOID Address, T Data)
{
    WriteProcessMemory(hProcess, (LPVOID)Address, &Data, sizeof(T), nullptr);
}
DWORD GetProcId(const wchar_t* procName)
{
    DWORD procId = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 procEntry;
        procEntry.dwSize = sizeof(procEntry);
        if (Process32First(hSnap, &procEntry)) {
            do {
                if (!_wcsicmp(procEntry.szExeFile, procName)) {
                    procId = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procId;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                if (!_wcsicmp(modEntry.szModule, modName)) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

// Function to check if a process is running
bool IsProcessRunning(const wchar_t* processName) {
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            if (wcscmp(entry.szExeFile, processName) == 0) {
                exists = true;
                break;
            }
        }
    }

    CloseHandle(snapshot);
    return exists;
}
template <typename T>
void write(uintptr_t address, T value) {
    DWORD oldProtection;
    VirtualProtect(reinterpret_cast<void**>(address), sizeof(T), PAGE_EXECUTE_READWRITE, &oldProtection);
    *reinterpret_cast<T*>(address) = value;
    VirtualProtect(reinterpret_cast<void**>(address), sizeof(T), oldProtection, &oldProtection);
}

template <typename K>
K read(uintptr_t address) {
    return *reinterpret_cast<K*>(address);
}

#endif