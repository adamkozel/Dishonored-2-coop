// Registers the root-enumerated ViGEmBus device in the lab VM and installs its driver from an INF
// (the signed package copied from the host's DriverStore), like `devcon install <inf> <hwid>`.
// Idempotent: does nothing when a device with the hardware id already exists.
// Usage: vigem_setup.exe <path\ViGEmBus.inf> [hardware id, default Nefarius\ViGEmBus\Gen1]
// Build (x64 MSVC): cl /EHsc /O2 vigem_setup.cpp setupapi.lib newdev.lib
#include <windows.h>
#include <setupapi.h>
#include <newdev.h>
#include <cstdio>
#include <cwchar>
#include <string>
#ifndef MAX_CLASS_NAME_LEN
#define MAX_CLASS_NAME_LEN 32   // SetupAPI's documented class-name buffer length
#endif

static bool deviceExists(const wchar_t* hwid) {
    HDEVINFO set = SetupDiGetClassDevsW(nullptr, nullptr, nullptr, DIGCF_ALLCLASSES | DIGCF_PRESENT);
    if (set == INVALID_HANDLE_VALUE) return false;
    SP_DEVINFO_DATA d{ sizeof(d) };
    bool found = false;
    for (DWORD i = 0; !found && SetupDiEnumDeviceInfo(set, i, &d); ++i) {
        wchar_t buf[2048]{}; DWORD type = 0;
        if (SetupDiGetDeviceRegistryPropertyW(set, &d, SPDRP_HARDWAREID, &type, reinterpret_cast<BYTE*>(buf), sizeof(buf) - 4, nullptr))
            for (const wchar_t* p = buf; *p; p += wcslen(p) + 1) if (_wcsicmp(p, hwid) == 0) { found = true; break; }
    }
    SetupDiDestroyDeviceInfoList(set);
    return found;
}

int wmain(int argc, wchar_t** argv) {
    if (argc < 2) { fwprintf(stderr, L"usage: vigem_setup <inf> [hwid]\n"); return 2; }
    wchar_t inf[MAX_PATH]{};
    if (!GetFullPathNameW(argv[1], MAX_PATH, inf, nullptr)) return 2;
    const wchar_t* hwid = argc > 2 ? argv[2] : L"Nefarius\\ViGEmBus\\Gen1";
    if (deviceExists(hwid)) { wprintf(L"present: %ls\n", hwid); return 0; }
    GUID cls{}; wchar_t className[MAX_CLASS_NAME_LEN]{};
    if (!SetupDiGetINFClassW(inf, &cls, className, MAX_CLASS_NAME_LEN, nullptr)) { wprintf(L"SetupDiGetINFClass failed %lu\n", GetLastError()); return 3; }
    HDEVINFO set = SetupDiCreateDeviceInfoList(&cls, nullptr);
    if (set == INVALID_HANDLE_VALUE) { wprintf(L"SetupDiCreateDeviceInfoList failed %lu\n", GetLastError()); return 3; }
    SP_DEVINFO_DATA d{ sizeof(d) };
    if (!SetupDiCreateDeviceInfoW(set, className, &cls, nullptr, nullptr, DICD_GENERATE_ID, &d)) { wprintf(L"SetupDiCreateDeviceInfo failed %lu\n", GetLastError()); return 3; }
    std::wstring multi(hwid); multi.push_back(L'\0'); multi.push_back(L'\0');
    if (!SetupDiSetDeviceRegistryPropertyW(set, &d, SPDRP_HARDWAREID, reinterpret_cast<const BYTE*>(multi.data()), static_cast<DWORD>(multi.size() * sizeof(wchar_t)))) { wprintf(L"set HARDWAREID failed %lu\n", GetLastError()); return 3; }
    if (!SetupDiCallClassInstaller(DIF_REGISTERDEVICE, set, &d)) { wprintf(L"DIF_REGISTERDEVICE failed %lu\n", GetLastError()); return 3; }
    BOOL reboot = FALSE;
    if (!UpdateDriverForPlugAndPlayDevicesW(nullptr, hwid, inf, INSTALLFLAG_FORCE, &reboot)) {
        const DWORD e = GetLastError();
        SetupDiCallClassInstaller(DIF_REMOVE, set, &d);   // do not leave a driverless phantom device
        wprintf(L"UpdateDriverForPlugAndPlayDevices failed %lu\n", e); return 4;
    }
    SetupDiDestroyDeviceInfoList(set);
    wprintf(L"installed: %ls%ls\n", hwid, reboot ? L" (reboot required)" : L"");
    return 0;
}
