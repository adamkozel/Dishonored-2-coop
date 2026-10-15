// Plugs one virtual Xbox 360 controller into ViGEmBus and keeps it connected while this process
// runs (the bus unplugs a target when its owning handle closes). The pad's own reports stay
// neutral: the lab proxy's XInputGetState hook supplies the simulated state. Its only job is to
// make Windows (and Dishonored 2's joystick manager) see a real controller in the lab VM (#24).
// Usage: vigem_pad.exe   (runs until killed)
// Build (x64 MSVC): cl /EHsc /O2 vigem_pad.cpp setupapi.lib
#include <windows.h>
#include <setupapi.h>
#include <winioctl.h>
#include <initguid.h>
#include <cstdio>
#include <cwchar>

// ViGEmBus public interface (ViGEmBusShared.h, bus version 1.x).
DEFINE_GUID(GUID_DEVINTERFACE_BUSENUM_VIGEM, 0x96E42B22, 0xF5E9, 0x42F8, 0xB0, 0x43, 0xED, 0x0F, 0x93, 0x2F, 0x01, 0x4F);
#define FILE_DEVICE_BUSENUM FILE_DEVICE_BUS_EXTENDER
#define BUSENUM_W_IOCTL(i) CTL_CODE(FILE_DEVICE_BUSENUM, (i), METHOD_BUFFERED, FILE_WRITE_DATA)
#define IOCTL_BASE 0x801
#define IOCTL_VIGEM_PLUGIN_TARGET BUSENUM_W_IOCTL(IOCTL_BASE + 0x000)
#define IOCTL_VIGEM_CHECK_VERSION BUSENUM_W_IOCTL(IOCTL_BASE + 0x002)
#define IOCTL_VIGEM_WAIT_DEVICE_READY BUSENUM_W_IOCTL(IOCTL_BASE + 0x003)
#define VIGEM_COMMON_VERSION 0x0001
struct VIGEM_CHECK_VERSION { ULONG Size; ULONG Version; };
struct VIGEM_PLUGIN_TARGET { ULONG Size; ULONG SerialNo; ULONG TargetType; USHORT VendorId; USHORT ProductId; };
struct VIGEM_WAIT_DEVICE_READY { ULONG Size; ULONG SerialNo; };

static HANDLE openBus() {
    HDEVINFO set = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_BUSENUM_VIGEM, nullptr, nullptr, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (set == INVALID_HANDLE_VALUE) return INVALID_HANDLE_VALUE;
    SP_DEVICE_INTERFACE_DATA i{ sizeof(i) };
    HANDLE h = INVALID_HANDLE_VALUE;
    if (SetupDiEnumDeviceInterfaces(set, nullptr, &GUID_DEVINTERFACE_BUSENUM_VIGEM, 0, &i)) {
        DWORD need = 0; SetupDiGetDeviceInterfaceDetailW(set, &i, nullptr, 0, &need, nullptr);
        auto* detail = static_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W*>(malloc(need));
        detail->cbSize = sizeof(*detail);
        if (SetupDiGetDeviceInterfaceDetailW(set, &i, detail, need, nullptr, nullptr))
            h = CreateFileW(detail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        free(detail);
    }
    SetupDiDestroyDeviceInfoList(set);
    return h;
}

int wmain() {
    HANDLE bus = openBus();
    if (bus == INVALID_HANDLE_VALUE) { wprintf(L"ViGEmBus not found (%lu); run vigem_setup first\n", GetLastError()); return 3; }
    DWORD got = 0;
    VIGEM_CHECK_VERSION v{ sizeof(v), VIGEM_COMMON_VERSION };
    if (!DeviceIoControl(bus, IOCTL_VIGEM_CHECK_VERSION, &v, sizeof(v), nullptr, 0, &got, nullptr)) { wprintf(L"bus version check failed %lu\n", GetLastError()); return 4; }
    VIGEM_PLUGIN_TARGET p{ sizeof(p), 1, 0 /*Xbox360Wired*/, 0x045E, 0x028E };
    if (!DeviceIoControl(bus, IOCTL_VIGEM_PLUGIN_TARGET, &p, sizeof(p), nullptr, 0, &got, nullptr)) { wprintf(L"plugin failed %lu\n", GetLastError()); return 5; }
    VIGEM_WAIT_DEVICE_READY w{ sizeof(w), 1 };
    DeviceIoControl(bus, IOCTL_VIGEM_WAIT_DEVICE_READY, &w, sizeof(w), nullptr, 0, &got, nullptr);  // absent on old buses: ignore
    wprintf(L"virtual Xbox 360 pad plugged (serial 1); holding\n"); fflush(stdout);
    for (;;) Sleep(60000);
}
