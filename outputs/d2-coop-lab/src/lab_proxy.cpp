#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <unknwn.h>
#include <cstring>
#include <cstdio>
#include "lab_policy.h"
#include "lab_input.h"
#include "engine_bridge.h"

// A local test loader with a scoped mutex and opt-in native lab window policy.
// Steam APIs are untouched.
namespace {
using CreateMutexProc = HANDLE (WINAPI*)(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR);
using DirectInputProc = HRESULT (WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
CreateMutexProc realCreateMutex = nullptr;
INIT_ONCE inputOnce = INIT_ONCE_STATIC_INIT;
DirectInputProc realDirectInput = nullptr;
char instanceRole[16]{};
char logPath[MAX_PATH]{};
char inputPath[MAX_PATH]{};
using InputStateProc = DWORD (WINAPI*)(DWORD,XINPUT_STATE*);
InputStateProc realInputState = nullptr;
using InputVibrationProc = DWORD (WINAPI*)(DWORD,XINPUT_VIBRATION*);
InputVibrationProc realInputVibration = nullptr;
using ExitProcessProc=void(WINAPI*)(UINT);
ExitProcessProc realExitProcess=nullptr;
DWORD WINAPI labInputVibration(DWORD index,XINPUT_VIBRATION* vibration) {
    // The simulated controller has no physical motors. Acknowledge its native
    // rumble requests; preserve Windows behavior for physical controller slots.
    if(index==0 && vibration)return ERROR_SUCCESS;
    return realInputVibration(index,vibration);
}
SRWLOCK inputLock = SRWLOCK_INIT;
XINPUT_STATE virtualState{};
LabInputPacket lastPacket{};
ULONGLONG lastInputRead=0;
std::uint32_t lastAcknowledged=0;
void keepControllerAcquired(void* controller) {
    // Only the native joystick manager uses this patched virtual method.
    // Keyboard/mouse acquisition and actual window focus remain native.
    *reinterpret_cast<unsigned char*>(static_cast<unsigned char*>(controller)+8)=1;
}
bool retainControllerAcquisition(unsigned char* image) {
    auto* slot=reinterpret_cast<void**>(image+0x1ae06f8);
    if(*slot!=image+0xd1cb0) return false;
    DWORD old=0,unused=0;
    if(!VirtualProtect(slot,sizeof(*slot),PAGE_READWRITE,&old)) return false;
    InterlockedExchangePointer(slot,reinterpret_cast<void*>(&keepControllerAcquired));
    return VirtualProtect(slot,sizeof(*slot),old,&unused)!=FALSE;
}

void log(const char* line) {
    if (!*logPath) return;
    HANDLE file = CreateFileA(logPath, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return;
    DWORD written = 0;
    WriteFile(file, line, static_cast<DWORD>(std::strlen(line)), &written, nullptr);
    CloseHandle(file);
}
LONG CALLBACK traceNativeException(EXCEPTION_POINTERS* event) {
    if(handleCoopPoseTrace(event)==EXCEPTION_CONTINUE_EXECUTION)return EXCEPTION_CONTINUE_EXECUTION;
    const auto code=event->ExceptionRecord->ExceptionCode;
    // 1214jy (#195): a guarded native read's own fault; its __except returns the failure.
    if(coopGuardedRead&&(code==EXCEPTION_ACCESS_VIOLATION||code==EXCEPTION_IN_PAGE_ERROR||code==EXCEPTION_GUARD_PAGE))return EXCEPTION_CONTINUE_SEARCH;
    if(code==EXCEPTION_ACCESS_VIOLATION||code==EXCEPTION_ILLEGAL_INSTRUCTION||code==EXCEPTION_STACK_OVERFLOW) {
        char line[256]{};
        const auto base=reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
        std::snprintf(line,sizeof(line),"Native exception %08lx at RVA %llx; address %llx; operation %llu\r\n",code,
            event->ContextRecord->Rip-base,event->ExceptionRecord->NumberParameters>1?event->ExceptionRecord->ExceptionInformation[1]:0,
            event->ExceptionRecord->NumberParameters?event->ExceptionRecord->ExceptionInformation[0]:0);
        log(line);
        // Targeted, read-only evidence for the observed CombatGiveUp crash.
        // RBX is the task in 5a4b00; task+20 is its actor. Never repair a
        // native graph or consume the exception from this diagnostic path.
        if(event->ContextRecord->Rip==base+0x584660) {
            const auto task=event->ContextRecord->Rbx;
            std::uintptr_t taskVt=0,actor=0,actorVt=0,contextRef=0;
            unsigned handle=0,phase=0;SIZE_T done=0;
            const auto read=[&](std::uintptr_t at,void* out,SIZE_T size){
                return ReadProcessMemory(GetCurrentProcess(),reinterpret_cast<void*>(at),out,size,&done)&&done==size;
            };
            if(read(task,&taskVt,8)&&read(task+0x20,&actor,8)&&read(task+0xb0,&phase,4)&&read(task+0xb8,&contextRef,8)) {
                read(actor,&actorVt,8);read(actor+0xa8,&handle,4);
                std::snprintf(line,sizeof(line),"CombatGiveUp task=%llx vtRva=%llx actor=%llx actorVtRva=%llx handle=%08x phase=%u context=%llx\r\n",
                    task,taskVt-base,actor,actorVt-base,handle,phase,contextRef);log(line);
            }
        }
        if(code!=EXCEPTION_STACK_OVERFLOW) {
            CONTEXT context=*event->ContextRecord;
            for(unsigned i=0;i<20&&context.Rip;++i) {
                std::snprintf(line,sizeof(line),"Fault stack %u RVA %llx\r\n",i,context.Rip-base);log(line);
                DWORD64 imageBase=0;auto* fn=RtlLookupFunctionEntry(context.Rip,&imageBase,nullptr);
                if(fn){void* handler=nullptr;DWORD64 establisher=0;RtlVirtualUnwind(UNW_FLAG_NHANDLER,imageBase,context.Rip,fn,&context,&handler,&establisher,nullptr);}
                else {SIZE_T done=0;DWORD64 next=0;if(!ReadProcessMemory(GetCurrentProcess(),reinterpret_cast<void*>(context.Rsp),&next,8,&done)||done!=8)break;context.Rip=next;context.Rsp+=8;}
            }
        }
        HMODULE faultModule=nullptr;char modulePath[MAX_PATH]{};
        if(GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCSTR>(event->ContextRecord->Rip),&faultModule)&&GetModuleFileNameA(faultModule,modulePath,sizeof(modulePath))) {
            std::snprintf(line,sizeof(line),"Fault module %.180s offset %llx\r\n",modulePath,event->ContextRecord->Rip-reinterpret_cast<std::uintptr_t>(faultModule));log(line);
        }
    }
    return EXCEPTION_CONTINUE_SEARCH;
}
void WINAPI traceNativeExit(UINT code) {
    char line[256]{};void* frames[8]{};
    const auto count=CaptureStackBackTrace(0,8,frames,nullptr);
    std::snprintf(line,sizeof(line),"Native ExitProcess %u\r\n",code);log(line);
    const auto base=reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
    for(USHORT i=0;i<count;++i){std::snprintf(line,sizeof(line),"Exit frame %u RVA %llx\r\n",i,reinterpret_cast<std::uintptr_t>(frames[i])-base);log(line);}
    realExitProcess(code);
}
HANDLE WINAPI instanceMutex(LPSECURITY_ATTRIBUTES attributes, BOOL owner, LPCSTR name) {
    if (!name || std::strcmp(name, "Dishonored 2") != 0 || !*instanceRole) return realCreateMutex(attributes, owner, name);
    char scopedName[128];
    std::snprintf(scopedName, sizeof(scopedName), "Dishonored 2-D2CoopLab-%s", instanceRole);
    log("Scoped the game instance mutex; Steam APIs unchanged.\r\n");
    return realCreateMutex(attributes, owner, scopedName);
}
DWORD WINAPI labInputState(DWORD index, XINPUT_STATE* output) {
    if (index != 0 || !output) return realInputState(index,output);
    // One diagnostic per polling thread, including the native caller. This
    // identifies movie input paths without moving engine work across threads.
    static thread_local bool pollingThreadRecorded=false;
    if(!pollingThreadRecorded) {
        pollingThreadRecorded=true;
        void* frames[5]{};const auto count=CaptureStackBackTrace(0,5,frames,nullptr);
        const auto base=reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));
        char line[160]{};
        std::snprintf(line,sizeof(line),"Input poll thread %lu, caller RVAs %llx %llx %llx\r\n",GetCurrentThreadId(),
            count>1?reinterpret_cast<std::uintptr_t>(frames[1])-base:0,
            count>2?reinterpret_cast<std::uintptr_t>(frames[2])-base:0,
            count>3?reinterpret_cast<std::uintptr_t>(frames[3])-base:0);log(line);
    }
    AcquireSRWLockExclusive(&inputLock);
    const ULONGLONG tick=GetTickCount64();
    if (tick-lastInputRead>=20) {
        lastInputRead=tick;
        LabInputPacket packet{};
        HANDLE file=CreateFileA(inputPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
        DWORD got=0;
        if(file!=INVALID_HANDLE_VALUE) {
            if(ReadFile(file,&packet,sizeof(packet),&got,nullptr) && got==sizeof(packet)) lastPacket=packet;
            else lastPacket={};
            CloseHandle(file);
        } else lastPacket={};
    }
    FILETIME time{}; GetSystemTimeAsFileTime(&time);
    const std::uint64_t now=(static_cast<std::uint64_t>(time.dwHighDateTime)<<32)|time.dwLowDateTime;
    XINPUT_GAMEPAD pad{};
    const bool valid=decodeLabInput(lastPacket,now,pad);
    if(std::memcmp(&pad,&virtualState.Gamepad,sizeof(pad))) {
        ++virtualState.dwPacketNumber;
        virtualState.Gamepad=pad;
    }
    if(valid && lastAcknowledged!=lastPacket.sequence) {
        lastAcknowledged=lastPacket.sequence;
        char line[100]{};
        std::snprintf(line,sizeof(line),"Consumed simulated controller packet %lu.\r\n",static_cast<unsigned long>(lastAcknowledged));
        log(line);
    }
    *output=virtualState;
    ReleaseSRWLockExclusive(&inputLock);
    return ERROR_SUCCESS;
}
bool hookImport(HMODULE module,const char* library,const char* symbol,void* hook,void** original) {
    auto* base = reinterpret_cast<unsigned char*>(module);
    auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE) return false;
    auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos->e_lfanew);
    if (nt->Signature != IMAGE_NT_SIGNATURE || nt->FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64) return false;
    const auto imports = nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    if (!imports.VirtualAddress) return false;
    auto* descriptor = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base + imports.VirtualAddress);
    for (; descriptor->Name; ++descriptor) {
        if (_stricmp(reinterpret_cast<char*>(base + descriptor->Name), library) != 0 || !descriptor->OriginalFirstThunk) continue;
        auto* names = reinterpret_cast<IMAGE_THUNK_DATA64*>(base + descriptor->OriginalFirstThunk);
        auto* slots = reinterpret_cast<IMAGE_THUNK_DATA64*>(base + descriptor->FirstThunk);
        for (; names->u1.AddressOfData; ++names, ++slots) {
            if (IMAGE_SNAP_BY_ORDINAL64(names->u1.Ordinal)) continue;
            auto* import = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base + names->u1.AddressOfData);
            if (std::strcmp(reinterpret_cast<char*>(import->Name), symbol) != 0) continue;
            *original = reinterpret_cast<void*>(slots->u1.Function);
            DWORD old = 0;
            if (!VirtualProtect(&slots->u1.Function, sizeof(slots->u1.Function), PAGE_READWRITE, &old)) return false;
            InterlockedExchangePointer(reinterpret_cast<void* volatile*>(&slots->u1.Function), hook);
            DWORD unused = 0;
            VirtualProtect(&slots->u1.Function, sizeof(slots->u1.Function), old, &unused);
            return true;
        }
    }
    return false;
}
BOOL CALLBACK initializeInput(PINIT_ONCE, PVOID, PVOID*) {
    wchar_t directory[MAX_PATH]{};
    const UINT size = GetSystemDirectoryW(directory, MAX_PATH);
    if (!size || size + 13 >= MAX_PATH) return FALSE;
    wcscat_s(directory, L"\\dinput8.dll");
    // Called from DirectInput8Create, outside DllMain's loader lock.
    HMODULE systemInput = LoadLibraryW(directory);
    if (!systemInput) return FALSE;
    realDirectInput = reinterpret_cast<DirectInputProc>(GetProcAddress(systemInput, "DirectInput8Create"));
    return realDirectInput != nullptr;
}
}
extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(HINSTANCE instance, DWORD version, REFIID id, LPVOID* output, LPUNKNOWN outer) {
    if (!InitOnceExecuteOnce(&inputOnce, initializeInput, nullptr, nullptr) || !realDirectInput) return E_FAIL;
    return realDirectInput(instance, version, id, output, outer);
}
BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID) {
    if (reason != DLL_PROCESS_ATTACH) return TRUE;
    DisableThreadLibraryCalls(dll);
    char role[16]{};
    GetEnvironmentVariableA("D2COOP_LAB_ROLE", role, sizeof(role));
    if (std::strcmp(role, "host") != 0 && std::strcmp(role, "client") != 0) {
        // 1181 product activation: an installed release (no lab environment)
        // is active only when <game>\d2coop\d2coop.cfg exists. No lab window
        // policy, mutex rename, simulated input or command file; the engine
        // bridge starts as a solo host and roles come from the Steam session.
        char exe[MAX_PATH]{};if(!GetModuleFileNameA(nullptr,exe,MAX_PATH))return TRUE;
        char* slash=std::strrchr(exe,'\\');if(!slash)return TRUE;*slash=0;
        char data[MAX_PATH]{},cfg[MAX_PATH]{},status[MAX_PATH]{},assets[MAX_PATH]{};
        if(std::snprintf(data,sizeof(data),"%s\\d2coop",exe)>=static_cast<int>(sizeof(data))||std::snprintf(cfg,sizeof(cfg),"%s\\d2coop.cfg",data)>=static_cast<int>(sizeof(cfg)))return TRUE;
        if(GetFileAttributesA(cfg)==INVALID_FILE_ATTRIBUTES)return TRUE;
        std::snprintf(status,sizeof(status),"%s\\coop-status.json",data);std::snprintf(assets,sizeof(assets),"%s\\base\\game1voidmod.index",exe);
        SetEnvironmentVariableA("D2COOP_PRODUCT","1");SetEnvironmentVariableA("D2COOP_DATA_DIR",data);SetEnvironmentVariableA("D2COOP_STATUS_FILE",status);
        SetEnvironmentVariableA("D2COOP_PARTNER_ASSETS",GetFileAttributesA(assets)!=INVALID_FILE_ATTRIBUTES?"1":"0");
        // 1214jz (#195): the diagnostic status files are opt-in for players (troubleshooting).
        if(GetPrivateProfileIntA("d2coop","diagnostics",0,cfg)==1)SetEnvironmentVariableA("D2COOP_DIAGNOSTICS","1");
        // Stage 8.4.1 (#237): host_story=0 turns host story off; the host's setting holds for both players.
        if(GetPrivateProfileIntA("d2coop","host_story",1,cfg)==0)SetEnvironmentVariableA("D2COOP_HOST_STORY","0");
        // Mission checkpoints read the game's own autosave details (read only).
        char savegame[MAX_PATH]{};
        if(!GetPrivateProfileStringA("d2coop","savegame","",savegame,sizeof(savegame),cfg)){
            char profile[MAX_PATH]{};if(GetEnvironmentVariableA("USERPROFILE",profile,sizeof(profile)))
                std::snprintf(savegame,sizeof(savegame),"%s\\Saved Games\\Arkane Studios\\Dishonored2\\base\\savegame",profile);
        }
        if(*savegame){char slot[MAX_PATH]{};std::snprintf(slot,sizeof(slot),"%s\\SLOT82",savegame);SetEnvironmentVariableA("D2COOP_CHECKPOINT_DIR",slot);}
        strcpy_s(instanceRole,"product");
        return installEngineBridge(reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr)),"host")?TRUE:TRUE;
    }
    strcpy_s(instanceRole, role);
    GetEnvironmentVariableA("D2COOP_LAB_LOG", logPath, sizeof(logPath));
    AddVectoredExceptionHandler(0,traceNativeException);
    hookImport(GetModuleHandleW(nullptr),"KERNEL32.dll","ExitProcess",reinterpret_cast<void*>(&traceNativeExit),reinterpret_cast<void**>(&realExitProcess));
    const bool installed = hookImport(GetModuleHandleW(nullptr),"KERNEL32.dll","CreateMutexA",reinterpret_cast<void*>(&instanceMutex),reinterpret_cast<void**>(&realCreateMutex));
    log(installed ? "Lab loader active.\r\n" : "ERROR: game instance mutex import was not found.\r\n");
    char policy[8]{};
    GetEnvironmentVariableA("D2COOP_LAB_POLICY", policy, sizeof(policy));
    if (std::strcmp(policy,"1") == 0) {
        const bool applied = installLabPolicy(reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr)));
        log(applied ? "Native lab placement and focus-pause policy installed.\r\n" : "ERROR: native lab policy fingerprint mismatch or patch failure.\r\n");
        if (!applied) return FALSE;
        char bridge[8]{};
        GetEnvironmentVariableA("D2COOP_ENGINE_BRIDGE",bridge,sizeof(bridge));
        if(!std::strcmp(bridge,"1")) {
            const bool bridgeInstalled=installEngineBridge(reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr)),role);
            log(bridgeInstalled?"Engine-thread bridge and current native Steam transport installed.\r\n":"ERROR: engine bridge fingerprint mismatch.\r\n");
            if(!bridgeInstalled)return FALSE;
        }
        GetEnvironmentVariableA("D2COOP_LAB_INPUT",inputPath,sizeof(inputPath));
        if(*inputPath) {
            const bool inputInstalled=hookImport(GetModuleHandleW(nullptr),"XINPUT9_1_0.dll","XInputGetState",reinterpret_cast<void*>(&labInputState),reinterpret_cast<void**>(&realInputState));
            const bool vibrationInstalled=hookImport(GetModuleHandleW(nullptr),"XINPUT9_1_0.dll","XInputSetState",reinterpret_cast<void*>(&labInputVibration),reinterpret_cast<void**>(&realInputVibration));
            const bool retained=retainControllerAcquisition(reinterpret_cast<unsigned char*>(GetModuleHandleW(nullptr)));
            log(inputInstalled && retained ? "Per-instance simulated controller active; background acquisition retained.\r\n" : "ERROR: controller interface fingerprint mismatch.\r\n");
            if(!inputInstalled || !vibrationInstalled || !retained) return FALSE;
        }
    }
    return TRUE;
}
