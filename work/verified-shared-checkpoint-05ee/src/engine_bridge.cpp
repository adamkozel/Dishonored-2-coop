#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <atomic>
#include <intrin.h>
#include <isteamnetworkingsockets.h>
#include "engine_bridge.h"
#include "coop_protocol.h"
#include "world_protocol.h"
#include "world_identity.h"
#include "native_ai_state.h"
#include "guard_protocol.h"
#include "native_character.h"

namespace {
unsigned char* gameImage=nullptr;
char commandPath[MAX_PATH]{},statusPath[MAX_PATH]{},consolePath[MAX_PATH]{};
using ExecuteBuffer=void(*)(void*);
ExecuteBuffer realExecuteBuffer=nullptr;
// Native screen update accepts six integer-class arguments. The disassembly
// consumes R8d, R9b, and the sixth argument at entry RSP+0x30; preserve the
// otherwise unused second and fifth argument as well.
using CommonScreenUpdate=void(*)(void*,std::uintptr_t,int,bool,std::uintptr_t,bool);
CommonScreenUpdate realCommonScreenUpdate=nullptr;
using CinematicSkipConfirmed=void(*)(void*);
CinematicSkipConfirmed realCinematicSkipConfirmed=nullptr;
std::uintptr_t sampledCinematicState=0;
bool cinematicSync=false;
bool nativeMutationAllowed=false;
std::atomic<std::uint64_t> nativeSkipCallbacks{0};
std::atomic<DWORD> nativeSkipThread{0};
std::uint64_t skipSent=0,skipReceived=0,skipApplied=0,skipDropped=0,skipSequence=0;
SRWLOCK skipEventLock=SRWLOCK_INIT;
void* pendingSkipState=nullptr;
ULONGLONG pendingSkipTick=0;
std::uint64_t lastSkipSession=0,lastSkipSequence=0;
std::uint64_t lastKnownCinematicSignature=0;
std::uint32_t lastKnownCinematicCount=0;
std::uint64_t lastKnownSequenceSignature=0,lastKnownSequenceHandles=0;
std::uint64_t sampledSequenceHandles=0;
struct CinematicHold {std::uintptr_t game=0,instance=0;std::uint64_t worldEpoch=0;std::uint32_t id=0;ULONGLONG until=0;bool barrier=false,announced=false;};
CinematicHold cinematicHold{};
std::uint64_t cinematicHoldStarted=0,cinematicHoldResumed=0,cinematicHoldAbandoned=0;
bool cinematicBarrier=false;
std::uint64_t barrierStarted=0,barrierReleased=0,barrierTimedOut=0,barrierMissed=0,barrierSent=0,barrierReceived=0,barrierRejected=0;
std::uint64_t barrierSequence=0,lastBarrierSession=0,lastBarrierSequence=0,lastBarrierWorldEpoch=0,lastBarrierHandles=0;
void finishCinematicHold(ULONGLONG tick,bool coordinated=false);
void serviceCinematicBarrier(ULONGLONG tick);
void cinematicSkipConfirmed(void* state);
std::uint64_t screenTransportPumps=0;
using ReferenceFrameUpdate=void(*)(void*);
ReferenceFrameUpdate realReferenceFrameUpdate=nullptr;
std::atomic<void*> controlledFrame{nullptr};
std::atomic<bool> networkFrameOwned{false};
std::atomic<void*> guardControlledFrame{nullptr};
std::atomic<ULONGLONG> guardFrameUntil{0};
bool guardAiOwned=false;
void updateReferenceFrame(void* frame) {
    // Native job workers otherwise integrate the NPC's local motion into its
    // frame after a received pose. Only our registered remote frame is owned
    // by the network; all other reference frames keep their native update.
    if(networkFrameOwned.load(std::memory_order_acquire)&&controlledFrame.load(std::memory_order_relaxed)==frame)return;
    if(guardControlledFrame.load(std::memory_order_acquire)==frame&&GetTickCount64()<guardFrameUntil.load(std::memory_order_acquire))return;
    realReferenceFrameUpdate(frame);
}
std::uint32_t localRole=0,lastCommand=0;
std::atomic<DWORD> engineThread{0};
std::atomic<bool> bridgeInstalled{false};
ULONGLONG lastPump=0,lastTransportPump=0,lastStatus=0,nextSteamAttempt=0,nextConnect=0,lastSend=0,lastReceive=0;
std::uintptr_t observedGame=0,observedPlayer=0;
std::uint64_t observedMapId=0;
bool observedReady=false;
char mapName[192]{};
std::uintptr_t probeActor=0,probeGame=0;
std::uint32_t probeHandle=0;
bool probeFollow=false;
bool phaseFollowInvalidated=false;
std::uint64_t probeRemoteEpoch=0,probeRemoteSession=0;
float probeOffset[3]{},probeRenderPosition[3]{};
CoopPosePacket outgoing{},remote{};
std::uint64_t sent=0,received=0,rejected=0;
ISteamNetworkingSockets* sockets=nullptr;
HSteamListenSocket listener=0;
HSteamNetConnection connection=0;
int connectionState=0;
SRWLOCK eventLock=SRWLOCK_INIT,consoleLock=SRWLOCK_INIT;
SteamNetConnectionStatusChangedCallback_t events[32]{};
unsigned eventCount=0;
thread_local bool invalidConsoleFormat=false;
volatile LONG poseTraceActive=0,poseTraceCount=0;
std::uintptr_t poseTraceAddress=0;
std::uintptr_t modImage=0;DWORD modImageSize=0;
struct PoseWrite {std::uintptr_t rip,rcx,rdx,rbx,rsi;float position[3];void* frames[8];USHORT frameCount;DWORD thread;volatile LONG ready;};
PoseWrite poseWrites[32]{};
struct NativeBodySnapshot {unsigned count=0;std::uint32_t ids[64]{};float positions[64][3]{};};
struct NativeTransformSample {float before[12]{},after[12]{};NativeBodySnapshot bodiesBefore,bodiesAfter;bool deferred=false;};
NativeTransformSample transformSamples[128]{};
unsigned transformSampleCount=0;
ULONGLONG transformTraceUntil=0;
std::uint32_t transformTraceHandle=0,transformTraceCommand=0;
void invalidFormat(const wchar_t*,const wchar_t*,const wchar_t*,unsigned int,std::uintptr_t) {invalidConsoleFormat=true;}
bool memoryRead(std::uintptr_t at,void* data,SIZE_T size) {
    SIZE_T done=0;return ReadProcessMemory(GetCurrentProcess(),reinterpret_cast<void*>(at),data,size,&done)&&done==size;
}
#include "native_locomotion.h"
void consoleOutput(const char* format,...) {
    if(!*consolePath||!format)return;
    char line[4096]{};va_list args;va_start(args,format);
    invalidConsoleFormat=false;
    const auto previous=_set_thread_local_invalid_parameter_handler(invalidFormat);
    const int n=vsnprintf_s(line,sizeof(line),_TRUNCATE,format,args);va_end(args);
    _set_thread_local_invalid_parameter_handler(previous);
    if(invalidConsoleFormat)std::snprintf(line,sizeof(line),"COOP: ignored unsupported native log format: %.500s\n",format);
    const DWORD length=n<0?static_cast<DWORD>(std::strlen(line)):static_cast<DWORD>(n);
    AcquireSRWLockExclusive(&consoleLock);
    HANDLE file=CreateFileA(consolePath,FILE_APPEND_DATA,FILE_SHARE_READ|FILE_SHARE_WRITE,nullptr,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(file!=INVALID_HANDLE_VALUE){DWORD done=0;WriteFile(file,line,length,&done,nullptr);CloseHandle(file);}
    ReleaseSRWLockExclusive(&consoleLock);
}
bool readNativeBodies(std::uintptr_t actor,NativeBodySnapshot& result) {
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t list=0;std::uint32_t flags=0;
    if(!memoryRead(actor+0x268,&list,8)||!list||!memoryRead(actor+0x270,&flags,4)||(flags&0xffffff)>256)return false;
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t component=0,vt=0,instance=0,world=0,ids=0,bodies=0;std::uint32_t count=0;
        if(!memoryRead(list+i*8,&component,8)||!component||!memoryRead(component,&vt,8)||vt!=base+0x1c54008)continue;
        if(!memoryRead(component+0x100,&instance,8)||!instance||!memoryRead(instance+0x20,&world,8)||!world||
           !memoryRead(instance+0x28,&ids,8)||!ids||!memoryRead(instance+0x30,&count,4)||!count||count>64||
           !memoryRead(world+0x28,&bodies,8)||!bodies)return false;
        result.count=count;
        for(unsigned j=0;j<count;++j) {
            if(!memoryRead(ids+j*4,&result.ids[j],4)||(result.ids[j]&0xffffff)==0xffffff||
               !memoryRead(bodies+(result.ids[j]&0xffffff)*0xb0+0x30,result.positions[j],12))return false;
            for(float f:result.positions[j])if(!std::isfinite(f))return false;
        }
        return true;
    }
    return false;
}
void finishTransformTrace(ULONGLONG tick) {
    if(!transformTraceUntil||tick<transformTraceUntil)return;
    transformTraceUntil=0;
    char path[MAX_PATH]{},temporary[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.transform.json",statusPath)>=sizeof(path)||
       std::snprintf(temporary,sizeof(temporary),"%s.tmp",path)>=sizeof(temporary))return;
    FILE* file=nullptr;if(fopen_s(&file,temporary,"wb")||!file)return;
    std::fprintf(file,"{\"command\":%u,\"handle\":%u,\"samples\":[",transformTraceCommand,transformTraceHandle);
    for(unsigned i=0;i<transformSampleCount;++i) {
        const auto& s=transformSamples[i];if(i)std::fputc(',',file);
        std::fprintf(file,"{\"deferred\":%s,\"before\":[",s.deferred?"true":"false");
        for(int j=0;j<12;++j)std::fprintf(file,"%s%.9g",j?",":"",s.before[j]);
        std::fputs("],\"after\":[",file);
        for(int j=0;j<12;++j)std::fprintf(file,"%s%.9g",j?",":"",s.after[j]);
        std::fputs("],\"bodiesBefore\":[",file);
        for(int phase=0;phase<2;++phase) {
            const auto& bodies=phase?s.bodiesAfter:s.bodiesBefore;
            if(phase)std::fputs("],\"bodiesAfter\":[",file);
            for(unsigned j=0;j<bodies.count;++j) {
                const auto* p=bodies.positions[j];
                std::fprintf(file,"%s{\"id\":%u,\"position\":[%.9g,%.9g,%.9g]}",j?",":"",bodies.ids[j],p[0],p[1],p[2]);
            }
        }
        std::fputs("]}",file);
    }
    std::fputs("]}\n",file);const bool ok=std::ferror(file)==0;std::fclose(file);
    if(ok)MoveFileExA(temporary,path,MOVEFILE_REPLACE_EXISTING);
    consoleOutput("COOP: bounded native transform trace handle=%08x samples=%u\n",transformTraceHandle,transformSampleCount);
}
DWORD WINAPI poseTraceWorker(void*) {
    struct WatchedThread{HANDLE handle;DWORD64 dr0,dr6,dr7;} threads[128]{};
    unsigned installed=0;bool restoreFailed=false;
    HANDLE snapshot=CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
    THREADENTRY32 entry{};entry.dwSize=sizeof(entry);
    if(snapshot!=INVALID_HANDLE_VALUE&&Thread32First(snapshot,&entry))do {
        if(entry.th32OwnerProcessID!=GetCurrentProcessId()||entry.th32ThreadID==GetCurrentThreadId()||installed>=128)continue;
        HANDLE thread=OpenThread(THREAD_SUSPEND_RESUME|THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,entry.th32ThreadID);
        bool keep=false;CONTEXT original{};original.ContextFlags=CONTEXT_DEBUG_REGISTERS;
        if(thread&&SuspendThread(thread)!=static_cast<DWORD>(-1)) {
            if(GetThreadContext(thread,&original)&&!(original.Dr7&0xff)) {
                CONTEXT watched=original;watched.Dr0=poseTraceAddress;
                watched.Dr7=(original.Dr7&~0xf0003ULL)|0xd0001ULL;watched.Dr6=0;
                if(SetThreadContext(thread,&watched)) {
                    threads[installed++]={thread,original.Dr0,original.Dr6,original.Dr7};keep=true;
                }
            }
            ResumeThread(thread);
        }
        if(thread&&!keep)CloseHandle(thread);
    }while(Thread32Next(snapshot,&entry));
    if(snapshot!=INVALID_HANDLE_VALUE)CloseHandle(snapshot);
    if(installed) {
        const auto deadline=GetTickCount64()+3000;
        while(GetTickCount64()<deadline&&InterlockedCompareExchange(&poseTraceCount,0,0)<32)Sleep(20);
    }
    for(unsigned i=0;i<installed;++i) {
        const auto& t=threads[i];
        if(SuspendThread(t.handle)!=static_cast<DWORD>(-1)) {
            CONTEXT current{};current.ContextFlags=CONTEXT_DEBUG_REGISTERS;
            if(GetThreadContext(t.handle,&current)&&current.Dr0==poseTraceAddress) {
                current.Dr0=t.dr0;current.Dr7=t.dr7;current.Dr6=t.dr6;
                restoreFailed=SetThreadContext(t.handle,&current)==FALSE||restoreFailed;
            }
            ResumeThread(t.handle);
        }
        CloseHandle(t.handle);
    }
    if(restoreFailed)consoleOutput("COOP: pose watch register restore failed\n");
    const auto count=InterlockedCompareExchange(&poseTraceCount,0,0);
    consoleOutput("COOP: pose write trace threads=%u captured=%ld\n",installed,count);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    for(LONG i=0;i<count&&i<32;++i) {
        const auto& w=poseWrites[i];
        for(unsigned retry=0;retry<20&&!InterlockedCompareExchange(const_cast<volatile LONG*>(&w.ready),0,0);++retry)Sleep(1);
        if(!w.ready)continue;
        consoleOutput("COOP: pose write %ld thread=%lu nextRva=%llx rcx=%llx rdx=%llx rbx=%llx rsi=%llx pos=%.6f,%.6f,%.6f\n",
            i,w.thread,w.rip-base,w.rcx,w.rdx,w.rbx,w.rsi,w.position[0],w.position[1],w.position[2]);
        for(USHORT j=0;j<w.frameCount;++j)consoleOutput("COOP: pose stack %ld.%u rva=%llx\n",i,j,reinterpret_cast<std::uintptr_t>(w.frames[j])-base);
    }
    InterlockedExchange(&poseTraceActive,0);return 0;
}
void startPoseTrace() {
    if(!probeActor||!engineThread||InterlockedCompareExchange(&poseTraceActive,1,0))return;
    poseTraceAddress=probeActor+0x300;InterlockedExchange(&poseTraceCount,0);
    for(auto& w:poseWrites)InterlockedExchange(&w.ready,0);
    HANDLE thread=CreateThread(nullptr,0,poseTraceWorker,nullptr,0,nullptr);
    if(thread)CloseHandle(thread);else InterlockedExchange(&poseTraceActive,0);
}
void statusCallback(SteamNetConnectionStatusChangedCallback_t* event) {
    AcquireSRWLockExclusive(&eventLock);
    if(eventCount<32)events[eventCount++]=*event;
    ReleaseSRWLockExclusive(&eventLock);
}
template<class T>T symbol(HMODULE dll,const char* name){return reinterpret_cast<T>(GetProcAddress(dll,name));}
void initializeSteam(ULONGLONG tick) {
    if(sockets||tick<nextSteamAttempt)return;
    nextSteamAttempt=tick+2000;
    HMODULE steam=GetModuleHandleW(L"steam_api64.dll");if(!steam)return;
    auto user=symbol<int(*)()>(steam,"SteamAPI_GetHSteamUser");
    auto pipe=symbol<int(*)()>(steam,"SteamAPI_GetHSteamPipe");
    auto create=symbol<void*(*)(const char*)>(steam,"SteamInternal_CreateInterface");
    auto generic=symbol<void*(*)(void*,int,int,const char*)>(steam,"SteamAPI_ISteamClient_GetISteamGenericInterface");
    if(!user||!pipe||!create||!generic||!user()||!pipe())return;
    void* client=create("SteamClient017");
    sockets=client?static_cast<ISteamNetworkingSockets*>(generic(client,user(),pipe(),STEAMNETWORKINGSOCKETS_INTERFACE_VERSION)):nullptr;
    if(sockets)consoleOutput("COOP: acquired current native %s on engine thread %lu\n",STEAMNETWORKINGSOCKETS_INTERFACE_VERSION,engineThread.load());
}
void pumpConnections(ULONGLONG tick) {
    if(!sockets)return;
    SteamNetworkingConfigValue_t option;
    option.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,reinterpret_cast<void*>(&statusCallback));
    SteamNetworkingIPAddr address;address.SetIPv4(0x7f000001,31426);
    if(localRole==0&&!listener&&tick>=nextConnect) {
        listener=sockets->CreateListenSocketIP(address,1,&option);nextConnect=tick+2000;
        consoleOutput("COOP: native loopback listener %u\n",listener);
    }else if(localRole==1&&!connection&&tick>=nextConnect) {
        connection=sockets->ConnectByIPAddress(address,1,&option);nextConnect=tick+2000;
        consoleOutput("COOP: native loopback connection attempt %u\n",connection);
    }
    // The game also dispatches Steam callbacks. This pumps the interface queue
    // during our own engine callback, including while native menus are open.
    sockets->RunCallbacks();
    SteamNetConnectionStatusChangedCallback_t pending[32]{};unsigned count=0;
    AcquireSRWLockExclusive(&eventLock);count=eventCount;std::memcpy(pending,events,count*sizeof(*events));eventCount=0;ReleaseSRWLockExclusive(&eventLock);
    for(unsigned i=0;i<count;++i) {
        const auto& e=pending[i];
        if(e.m_info.m_eState==k_ESteamNetworkingConnectionState_Connecting && localRole==0 && e.m_info.m_hListenSocket==listener) {
            // A killed lab client can remain Connected in Steam until its
            // transport timeout. Retire that silent loopback connection before
            // accepting the restarted client; a fresh peer is never displaced.
            if(e.m_info.m_addrRemote.IsLocalHost()&&connection&&connection!=e.m_hConn&&lastReceive&&tick-lastReceive>=5000) {
                sockets->CloseConnection(connection,0,"Silent one-PC lab peer replaced",false);
                connection=0;connectionState=0;lastReceive=0;remote={};
            }
            if(!e.m_info.m_addrRemote.IsLocalHost() || (connection&&connection!=e.m_hConn)) {
                sockets->CloseConnection(e.m_hConn,0,"One-PC lab accepts one loopback peer",false);continue;
            }
            if(sockets->AcceptConnection(e.m_hConn)==k_EResultOK)connection=e.m_hConn;
        }
        if(e.m_hConn!=connection)continue;
        connectionState=e.m_info.m_eState;
        consoleOutput("COOP: connection %u state %d (%s)\n",connection,connectionState,e.m_info.m_szEndDebug);
        if(e.m_info.m_eState==k_ESteamNetworkingConnectionState_ClosedByPeer || e.m_info.m_eState==k_ESteamNetworkingConnectionState_ProblemDetectedLocally) {
            sockets->CloseConnection(connection,0,"Connection ended",false);connection=0;remote={};lastReceive=0;nextConnect=tick+2000;
        }
    }
}
void sampleCinematicIdentity(std::uintptr_t game) {
    outgoing.cinematicSignature=0;outgoing.cinematicNodeCount=0;outgoing.cinematicMetadataValid=0;
    if(!game||(outgoing.gameLogicState!=1&&outgoing.gameLogicState!=5&&outgoing.gameLogicState!=14))return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t player=0,list=0,items=0;std::uint32_t flags=0;
    if(!memoryRead(game+0x3130b0,&player,8)||!player||!memoryRead(player+8,&list,8))return;
    if(!list){outgoing.cinematicMetadataValid=1;return;}
    if(!memoryRead(list,&items,8)||!memoryRead(list+8,&flags,4)||(flags&0xffffff)>256||((flags&0xffffff)&&!items))return;
    CoopCinematicKey keys[8]{};unsigned count=0;
    for(unsigned i=0;i<(flags&0xffffff);++i) {
        std::uintptr_t node=0,vt=0,instance=0,graphNode=0,executable=0,actor=0,name=0;
        std::uint32_t handle=0,actual=0,nodeId=0;unsigned char started=0;
        if(!memoryRead(items+i*8,&node,8)||!node||!memoryRead(node,&vt,8))return;
        if(vt!=base+0x1c63e48)continue;
        if(!memoryRead(node+0x30,&started,1)||started>1)return;
        if(!started)continue;
        if(count==8||!memoryRead(node+0x20,&instance,8)||!instance||!memoryRead(instance,&vt,8)||vt!=base+0x1c69ad8||
           !memoryRead(node+0x28,&graphNode,8)||!graphNode||!memoryRead(graphNode,&vt,8)||vt!=base+0x1c69828||
           !memoryRead(graphNode+0x20,&executable,8)||executable!=node||!memoryRead(graphNode+8,&nodeId,4)||
           !memoryRead(instance+0x60,&handle,4))return;
        const auto slot=static_cast<std::int32_t>(handle)>>16;
        if(slot<0||slot>=16384||!memoryRead(game+0x3130b0+slot*8,&actor,8)||!actor||
           !memoryRead(actor+0xa8,&actual,4)||actual!=handle||!memoryRead(actor+0x20,&name,8)||!name)return;
        char ownerName[192]{};
        if(!memoryRead(name,ownerName,sizeof(ownerName)-1)||!ownerName[0]||!std::memchr(ownerName,0,sizeof(ownerName)-1))return;
        for(const char* p=ownerName;*p;++p)if(static_cast<unsigned char>(*p)<32||static_cast<unsigned char>(*p)>126)return;
        const CoopCinematicKey key{coopMapId(ownerName),nodeId};
        for(unsigned j=0;j<count;++j)if(keys[j].ownerId==key.ownerId&&keys[j].nodeId==key.nodeId)return;
        keys[count++]=key;
    }
    std::uintptr_t finalItems=0;std::uint32_t finalFlags=0;
    if(!memoryRead(list,&finalItems,8)||finalItems!=items||!memoryRead(list+8,&finalFlags,4)||finalFlags!=flags)return;
    outgoing.cinematicNodeCount=count;outgoing.cinematicSignature=coopCinematicSignature(keys,count);outgoing.cinematicMetadataValid=1;
}
bool nativeName(std::uintptr_t address,char* result,unsigned capacity) {
    if(!address||!memoryRead(address,result,capacity-1)||!result[0]||!std::memchr(result,0,capacity-1))return false;
    for(const char* p=result;*p;++p)if(static_cast<unsigned char>(*p)<32||static_cast<unsigned char>(*p)>126)return false;
    return true;
}
void sampleSequenceIdentity(std::uintptr_t game,std::uint64_t map) {
    outgoing.sequenceSignature=outgoing.sequenceBoundaryId=0;outgoing.sequenceCount=outgoing.sequenceMetadataValid=0;sampledSequenceHandles=0;
    outgoing.sequenceTimeMs=outgoing.sequencePaused=0;
    if(!game||(outgoing.gameLogicState!=1&&outgoing.gameLogicState!=5&&outgoing.gameLogicState!=14))return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t player=0,components=0,proxy=0;std::uint32_t componentFlags=0,channels[2]{};
    if(!memoryRead(game+0x3130b0,&player,8)||!player||!memoryRead(player+0x268,&components,8)||!components||
       !memoryRead(player+0x270,&componentFlags,4)||(componentFlags&0xffffff)>256)return;
    for(unsigned i=0;i<(componentFlags&0xffffff);++i) {
        std::uintptr_t component=0,vt=0,owner=0;
        if(!memoryRead(components+i*8,&component,8)||!component||!memoryRead(component,&vt,8))return;
        if(vt!=base+0x1c59ac8)continue;
        if(proxy||!memoryRead(component+0x10,&owner,8)||owner!=player)return;
        proxy=component;
    }
    if(!proxy||!memoryRead(proxy+0x20,&channels[0],4)||!memoryRead(proxy+0x30,&channels[1],4))return;
    const auto manager=game+0x1f5338;
    CoopSequenceKey keys[2]{};unsigned count=0;std::uint32_t found=0;
    std::uintptr_t matched[2]{},matchedScenes[2]{};std::uint32_t states[2]{},times[2]{};
    for(unsigned field:{0u,0x610u}) {
        std::uintptr_t items=0;std::uint32_t flags=0;
        if(!memoryRead(manager+field,&items,8)||!memoryRead(manager+field+8,&flags,4)||(flags&0xffffff)>512||((flags&0xffffff)&&!items))return;
        for(unsigned i=0;i<(flags&0xffffff);++i) {
            std::uintptr_t instance=0;std::uint32_t id=0;
            if(!memoryRead(items+i*8,&instance,8)||!instance||!memoryRead(instance+0x158,&id,4))return;
            if(!id)continue;
            const unsigned mask=(id==channels[0]?1u:0u)|(id==channels[1]?2u:0u);
            if(!mask)continue;
            // A transfer between native lists may expose the same instance
            // twice. A different object with the same handle is not accepted.
            bool seen=false;for(unsigned j=0;j<count;++j)if(matched[j]==instance)seen=true;
            if(seen)continue;
            if(count==2||(found&mask))return;
            std::uintptr_t decl=0,scene=0,vt=0,name=0,subscene=0;std::uint32_t durationMs=0,state=0;
            if(!memoryRead(instance+0x27c,&state,4)||(state!=3&&state!=5)||!memoryRead(instance,&decl,8)||!decl||
               !memoryRead(decl,&vt,8)||vt!=base+0x1affbc8||!memoryRead(decl+8,&name,8)||
               !memoryRead(instance+8,&scene,8)||!scene||!memoryRead(scene,&vt,8)||vt!=base+0x1b00198||
               !memoryRead(scene+0x28,&durationMs,4)||!memoryRead(scene+0x30,&subscene,8))return;
            char assetName[256]{},subsceneName[128]{};
            if(!nativeName(name,assetName,sizeof(assetName))||!nativeName(subscene,subsceneName,sizeof(subsceneName)))return;
            keys[count]={coopMapId(assetName),coopMapId(subsceneName),durationMs,mask};
            states[count]=state;if(!memoryRead(instance+0x160,&times[count],4)||times[count]>86400000)return;
            matched[count]=instance;matchedScenes[count]=scene;++count;found|=mask;
        }
        std::uintptr_t finalItems=0;std::uint32_t finalFlags=0;
        if(!memoryRead(manager+field,&finalItems,8)||finalItems!=items||!memoryRead(manager+field+8,&finalFlags,4)||finalFlags!=flags)return;
    }
    if(found!=((channels[0]?1u:0u)|(channels[1]?2u:0u)))return;
    std::uint32_t finalChannels[2]{};
    if(!memoryRead(proxy+0x20,&finalChannels[0],4)||!memoryRead(proxy+0x30,&finalChannels[1],4)||
       finalChannels[0]!=channels[0]||finalChannels[1]!=channels[1])return;
    for(unsigned i=0;i<count;++i) {
        std::uintptr_t finalScene=0;std::uint32_t finalId=0,finalState=0;
        const unsigned channel=keys[i].channels&1?0:1;
        if(!memoryRead(matched[i]+8,&finalScene,8)||finalScene!=matchedScenes[i]||
           !memoryRead(matched[i]+0x158,&finalId,4)||finalId!=channels[channel]||
           !memoryRead(matched[i]+0x27c,&finalState,4)||finalState!=states[i])return;
    }
    sampledSequenceHandles=static_cast<std::uint64_t>(channels[0])|(static_cast<std::uint64_t>(channels[1])<<32);
    outgoing.sequenceSignature=coopSequenceSignature(keys,count);
    outgoing.sequenceBoundaryId=coopSequenceSignature(keys,count,map,true);
    outgoing.sequenceCount=count;outgoing.sequenceMetadataValid=1;
    if(count==1)outgoing.sequenceTimeMs=times[0];
    for(unsigned i=0;i<count;++i)if(states[i]==5)outgoing.sequencePaused=1;
}
void sampleLocalPlayer() {
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t game=0,player=0;int index=-2,alternate=0;
    outgoing.flags=0;outgoing.mapId=0;std::memset(outgoing.position,0,sizeof(outgoing.position));std::memset(outgoing.orientation,0,sizeof(outgoing.orientation));
    outgoing.gameLogicState=outgoing.pendingGameLogicState=0;
    sampledCinematicState=0;outgoing.cinematicCanSkip=0;outgoing.cinematicSyncEnabled=cinematicSync?1:0;
    std::memset(mapName,0,sizeof(mapName));
    memoryRead(base+0x39a5290,&game,8);
    std::uintptr_t vtable=0,name=0;std::uint32_t state=0;
    bool ready=game&&memoryRead(game,&vtable,8)&&vtable==base+0x1c940a8&&
        memoryRead(game+0x341058,&state,4)&&state==3&&memoryRead(game+0x337138,&name,8)&&name&&
        memoryRead(name,mapName,sizeof(mapName)-1);
    std::uintptr_t logicGame=0,logicGameVtable=0;
    if(memoryRead(base+0x25af110+0x19a8,&logicGame,8)&&logicGame&&
       memoryRead(logicGame,&logicGameVtable,8)&&logicGameVtable==base+0x1c940a8) {
        const auto manager=logicGame+0x1f6ec8;
        std::uintptr_t owner=0,stack=0,current=0,logicVtable=0;
        std::uint32_t count=0,id=0,pending=0;
        // Native CommitSwitch/PushState use this stack; the final item is the
        // active state. A pending switch blocks movement before it commits.
        if(memoryRead(manager+8,&owner,8)&&owner==logicGame&&memoryRead(manager+0x528,&stack,8)&&stack&&
           memoryRead(manager+0x530,&count,4)&&(count&0xffffff)>0&&(count&0xffffff)<=32&&
           memoryRead(stack+((count&0xffffff)-1)*4,&id,4)&&id>0&&id<17&&
           memoryRead(manager+0x18+id*8,&current,8)&&current&&memoryRead(current,&logicVtable,8)&&
           logicVtable>=base&&logicVtable<base+68988928&&
           (id!=1||logicVtable==base+0x1c60478)&&memoryRead(manager+0x538,&pending,4)&&pending<17) {
            outgoing.gameLogicState=id;outgoing.pendingGameLogicState=pending;
            if(id==5&&logicVtable==base+0x1c60128)sampledCinematicState=current;
        }
    }
    sampleCinematicIdentity(logicGame);
    const auto sequenceMap=ready?coopMapId(mapName):0;
    sampleSequenceIdentity(logicGame,sequenceMap);
    if(!outgoing.cinematicEpoch)outgoing.cinematicEpoch=1;
    // Nested native logic phases can alternate within a frame. An epoch tracks
    // authored observer identity changes, not those temporary phase samples.
    if(outgoing.cinematicMetadataValid&&(lastKnownCinematicSignature!=outgoing.cinematicSignature||lastKnownCinematicCount!=outgoing.cinematicNodeCount)) {
        lastKnownCinematicSignature=outgoing.cinematicSignature;lastKnownCinematicCount=outgoing.cinematicNodeCount;++outgoing.cinematicEpoch;
    }
    if(outgoing.sequenceMetadataValid&&(lastKnownSequenceSignature!=outgoing.sequenceSignature||lastKnownSequenceHandles!=sampledSequenceHandles)) {
        lastKnownSequenceSignature=outgoing.sequenceSignature;lastKnownSequenceHandles=sampledSequenceHandles;++outgoing.cinematicEpoch;
    }
    unsigned char cinematicFlags=0;
    outgoing.barrierEnabled=cinematicBarrier?1:0;
    outgoing.barrierHeld=cinematicHold.id&&cinematicHold.barrier&&outgoing.sequenceMetadataValid&&outgoing.sequencePaused&&outgoing.gameLogicState==5&&!outgoing.pendingGameLogicState?1:0;
    if(sampledCinematicState&&!outgoing.pendingGameLogicState&&outgoing.cinematicMetadataValid&&outgoing.cinematicNodeCount&&outgoing.sequenceMetadataValid&&outgoing.sequenceCount&&!outgoing.sequencePaused&&
       memoryRead(sampledCinematicState+0x9c0,&cinematicFlags,1)&&(cinematicFlags&4)&&!(cinematicFlags&8))outgoing.cinematicCanSkip=1;
    if(ready)for(unsigned char c:mapName)if(c&&((c<' '||c>126)||c=='"'||c=='\\')){ready=false;break;}
    ready=ready&&!std::strncmp(mapName,"campaign/",9)&&std::strncmp(mapName,"campaign/menu/",14);
    if(game)memoryRead(game+0x3130b0,&player,8);
    const auto map=ready?coopMapId(mapName):0;
    if(game!=observedGame||player!=observedPlayer||map!=observedMapId||ready!=observedReady) {
        observedGame=game;observedPlayer=player;observedMapId=map;observedReady=ready;++outgoing.worldEpoch;
        networkFrameOwned.store(false,std::memory_order_release);controlledFrame.store(nullptr);
        guardFrameUntil.store(0,std::memory_order_release);guardControlledFrame.store(nullptr,std::memory_order_release);
        probeActor=probeGame=probeHandle=0;probeFollow=false;phaseFollowInvalidated=false;
    }
    outgoing.mapId=map;
    if(!ready||!memoryRead(base+0x39a6088,&index,4)||!memoryRead(base+0x25ae9b8,&alternate,4))return;
    if(index!=0&&(index!=-1||alternate!=0))return;
    if(!player)return;
    float transform[12]{};if(!memoryRead(player+0x300,transform,sizeof(transform)))return;
    for(float f:transform)if(!std::isfinite(f))return;
    std::memcpy(outgoing.position,transform,12);std::memcpy(outgoing.orientation,transform+3,36);outgoing.flags=1;
}
void cinematicSkipConfirmed(void* state) {
    nativeSkipCallbacks.fetch_add(1,std::memory_order_relaxed);nativeSkipThread.store(GetCurrentThreadId(),std::memory_order_relaxed);
    // UI confirmations originate on a native UI thread. Preserve the native
    // callback there, then publish only an opaque event for the engine thread.
    // Never sample game objects or run Steam methods from this UI callback.
    realCinematicSkipConfirmed(state);
    AcquireSRWLockExclusive(&skipEventLock);pendingSkipState=state;pendingSkipTick=GetTickCount64();ReleaseSRWLockExclusive(&skipEventLock);
}
void relayConfirmedSkip(ULONGLONG tick) {
    if(!nativeMutationAllowed)return;
    void* state=nullptr;ULONGLONG confirmedAt=0;
    AcquireSRWLockExclusive(&skipEventLock);state=pendingSkipState;confirmedAt=pendingSkipTick;pendingSkipState=nullptr;ReleaseSRWLockExclusive(&skipEventLock);
    if(!state)return;
    unsigned char flags=0;
    auto confirmed=outgoing;confirmed.cinematicCanSkip=1;
    consoleOutput("COOP: queued native skip UI thread=%lu engine thread=%lu state=%p sampled=%p scene=%llx peerScene=%llx age=%llu\n",
        nativeSkipThread.load(),GetCurrentThreadId(),state,reinterpret_cast<void*>(sampledCinematicState),outgoing.cinematicSignature,remote.cinematicSignature,tick-confirmedAt);
    if(tick-confirmedAt<500&&reinterpret_cast<std::uintptr_t>(state)==sampledCinematicState&&
       memoryRead(sampledCinematicState+0x9c0,&flags,1)&&(flags&8)&&sockets&&connection&&
           connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000&&
           coopSharedSkipReady(confirmed,remote)) {
            CoopSkipPacket request{};request.role=localRole;request.sequence=++skipSequence;
            request.sourceSession=outgoing.session;request.targetSession=remote.session;
            request.sourceWorldEpoch=outgoing.worldEpoch;request.targetWorldEpoch=remote.worldEpoch;
            request.sourceCinematicEpoch=outgoing.cinematicEpoch;request.targetCinematicEpoch=remote.cinematicEpoch;
            request.mapId=outgoing.mapId;request.cinematicSceneId=coopSkipSceneId(outgoing);
            request.sequenceBoundaryId=outgoing.sequenceBoundaryId;
            if(sockets->SendMessageToConnection(connection,&request,sizeof(request),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK) {
                ++skipSent;consoleOutput("COOP: relayed native confirmed skip sequence=%llu scene=%llx\n",request.sequence,request.cinematicSceneId);
            }else ++skipDropped;
    }
}
#include "world_observer.h"
#include "native_ai_hold.h"
#include "guard_link.h"
struct NativeSwitchEvent;
bool checkpointIntercept(unsigned kind,int slot);
bool checkpointOwnsLoading();
bool checkpointLoadingBoundary(const NativeSwitchEvent& event);
#include "checkpoint_trace.h"
#include "character_probe.h"
#include "loading_trace.h"
#include "loading_queue_trace.h"
#include "checkpoint_sync.h"
void exchangePoses(ULONGLONG tick) {
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return;
    SteamNetworkingMessage_t* messages[16]{};
    const int count=sockets->ReceiveMessagesOnConnection(connection,messages,16);
    for(int i=0;i<count;++i) {
        std::uint32_t magic=0;if(messages[i]->m_cbSize>=4)std::memcpy(&magic,messages[i]->m_pData,4);
        if(magic==0x52433244) {
            if(messages[i]->m_cbSize==sizeof(CheckpointPacket)) {
                CheckpointPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));checkpointReceive(p,tick);
            }else ++checkpointSync.rejected;
            messages[i]->Release();continue;
        }
        if(magic==0x47433244) {
            if(messages[i]->m_cbSize==sizeof(CoopGuardPacket)) {
                CoopGuardPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveGuardPacket(p,tick);
            }else ++guardRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x57433244) {
            if(messages[i]->m_cbSize==sizeof(CoopWorldNpcPacket)) {
                CoopWorldNpcPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveWorldNpc(p,tick);
            }else ++worldRejected;
            messages[i]->Release();continue;
        }
        if(messages[i]->m_cbSize==sizeof(CoopBarrierPacket)&&magic==0x42433244) {
            CoopBarrierPacket release{};std::memcpy(&release,messages[i]->m_pData,sizeof(release));++barrierReceived;
            if(nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&cinematicHold.id&&cinematicHold.barrier&&
               tick<cinematicHold.until&&lastReceive&&tick-lastReceive<1000&&validCoopBarrier(release,outgoing,remote)&&
               (release.sourceSession!=lastBarrierSession||release.sequence>lastBarrierSequence)) {
                lastBarrierSession=release.sourceSession;lastBarrierSequence=release.sequence;finishCinematicHold(tick,true);
            }else {
                ++barrierRejected;
                consoleOutput("COOP: rejected rendezvous release mutation=%u held=%u valid=%u sourceEpoch=%llu peerEpoch=%llu targetEpoch=%llu localEpoch=%llu peerState=%u peerSegment=%llx\n",
                    nativeMutationAllowed?1:0,cinematicHold.id?1:0,validCoopBarrier(release,outgoing,remote)?1:0,
                    release.sourceCinematicEpoch,remote.cinematicEpoch,release.targetCinematicEpoch,outgoing.cinematicEpoch,remote.gameLogicState,remote.sequenceBoundaryId);
            }
            messages[i]->Release();continue;
        }
        if(messages[i]->m_cbSize==sizeof(CoopSkipPacket)) {
            CoopSkipPacket request{};std::memcpy(&request,messages[i]->m_pData,sizeof(request));++skipReceived;
            // Only command-buffer execution may perform this engine mutation.
            // The native movie screen fallback can receive messages but never
            // invokes a game callback while a movie is active.
            if(nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&sampledCinematicState&&lastReceive&&tick-lastReceive<1000&&
               validCoopSkip(request,outgoing,remote)&&
               (request.sourceSession!=lastSkipSession||request.sequence>lastSkipSequence)) {
                unsigned char flags=0;
                if(memoryRead(sampledCinematicState+0x9c0,&flags,1)&&(flags&4)&&!(flags&8)) {
                    lastSkipSession=request.sourceSession;lastSkipSequence=request.sequence;++skipApplied;
                    realCinematicSkipConfirmed(reinterpret_cast<void*>(sampledCinematicState));
                    consoleOutput("COOP: applied peer skip through native cinematic callback sequence=%llu scene=%llx\n",request.sequence,request.cinematicSceneId);
                    outgoing.cinematicCanSkip=0;
                }else ++skipDropped;
            }else ++skipDropped;
            messages[i]->Release();continue;
        }
        CoopPosePacket packet{};
        if(messages[i]->m_cbSize==sizeof(packet))std::memcpy(&packet,messages[i]->m_pData,sizeof(packet));
        if(messages[i]->m_cbSize==sizeof(packet)&&validCoopPose(packet,localRole)&&
           (!received||packet.session!=remote.session||static_cast<std::int32_t>(packet.sequence-remote.sequence)>0)) {
            remote=packet;++received;lastReceive=tick;
        }else ++rejected;
        messages[i]->Release();
    }
    if(tick-lastSend>=33) {
        lastSend=tick;++outgoing.sequence;if(!outgoing.sequence)++outgoing.sequence;
        if(sockets->SendMessageToConnection(connection,&outgoing,sizeof(outgoing),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK)++sent;
    }
}
bool spawnProbe(const char* character) {
    if(!coopGameplayReady(outgoing)||!observedGame||probeActor)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vtable=0,handles=0;std::uint32_t flags=0;
    if(!memoryRead(observedGame,&vtable,8)||vtable!=base+0x1c940a8||
       !memoryRead(observedGame+0x601c0,&handles,8)||!handles||
       !memoryRead(observedGame+0x601c8,&flags,4)||(flags&0xffffff)>16384)return false;
    const bool emily=!std::strcmp(character,"emily");
    if(!emily&&std::strcmp(character,"corvo"))return false;
    const char* expected=emily?"models/characters/def/story/emily_kaldwin.def":"models/characters/def/story/corvo_attano.def";
    std::uintptr_t definition=0;
    for(std::uint32_t i=0;i<(flags&0xffffff);++i) {
        std::uint32_t handle=0,actual=0;std::uintptr_t actor=0,actorVtable=0,def=0,name=0;char nameText[128]{};
        if(!memoryRead(handles+i*4,&handle,4))continue;
        const auto slot=static_cast<std::int32_t>(handle)>>16;
        if(slot<=0||slot>=16384||!memoryRead(observedGame+0x3130b0+slot*8,&actor,8)||!actor||
           !memoryRead(actor,&actorVtable,8)||actorVtable!=base+0x1c5e108||
           !memoryRead(actor+0xa8,&actual,4)||actual!=handle||!memoryRead(actor+0x2e8,&def,8)||!def||
           !memoryRead(def+8,&name,8)||!name||!memoryRead(name,nameText,sizeof(nameText)-1))continue;
        if(!std::strcmp(nameText,expected)){definition=def;break;}
    }
    std::uintptr_t spawner=0,spawnerComponent=0;
    const char* spawnerName=emily?"ld_spawner_emily_audience":"ld_spawner_corvo_audience";
    for(std::uint32_t i=0;i<(flags&0xffffff);++i) {
        std::uint32_t handle=0,actual=0;std::uintptr_t actor=0,vt=0,name=0;char text[128]{};
        if(!memoryRead(handles+i*4,&handle,4))continue;
        const auto slot=static_cast<std::int32_t>(handle)>>16;
        if(slot<=0||slot>=16384||!memoryRead(observedGame+0x3130b0+slot*8,&actor,8)||!actor||
           !memoryRead(actor,&vt,8)||vt!=base+0x1c5e948||!memoryRead(actor+0xa8,&actual,4)||actual!=handle||
           !memoryRead(actor+0x20,&name,8)||!name||!memoryRead(name,text,sizeof(text)-1)||std::strcmp(text,spawnerName))continue;
        std::uintptr_t list=0;std::uint32_t count=0;
        if(!memoryRead(actor+0x268,&list,8)||!list||!memoryRead(actor+0x270,&count,4)||(count&0xffffff)>256)continue;
        for(unsigned j=0;j<(count&0xffffff);++j) {
            std::uintptr_t component=0,componentVtable=0;
            if(memoryRead(list+j*8,&component,8)&&component&&memoryRead(component,&componentVtable,8)&&componentVtable==base+0x1c57d78){spawner=actor;spawnerComponent=component;break;}
        }
        if(spawnerComponent)break;
    }
    // Opening completion removes the original story NPCs. Their spawners keep
    // a native NPC variation with the living entity definition at +0x58, as
    // used by the original CreateEntity path at 0x75b250. Never substitute the
    // dead-body variant at +0x60 or an unverified declaration/name.
    if(!definition&&spawnerComponent) {
        std::uintptr_t owner=0,variation=0,vt=0,candidate=0,name=0;char text[192]{};
        if(memoryRead(spawnerComponent+0x10,&owner,8)&&owner==spawner&&
           memoryRead(spawner+0x360,&variation,8)&&variation&&memoryRead(variation,&vt,8)&&vt==base+0x1c5d578&&
           memoryRead(variation+0x58,&candidate,8)&&candidate&&memoryRead(candidate,&vt,8)&&vt==base+0x1ab58f8&&
           memoryRead(candidate+8,&name,8)&&nativeName(name,text,sizeof(text))&&!std::strcmp(text,expected)) {
            definition=candidate;
            consoleOutput("COOP: resolved %s probe definition from retained native spawner variation\n",character);
        }
    }
    if(!definition||!spawnerComponent){consoleOutput("COOP: probe refused; required story definition or presentation source is not loaded\n");return false;}
    float position[3];for(int i=0;i<3;++i)position[i]=outgoing.position[i]+outgoing.orientation[i]*2.5f;
    position[2]+=0.1f;
    // Recovered native spawner path: create a new entity, apply loaded component
    // overrides before initialization, then apply model/material variations.
    // Original story actors and the original spawner's ownership stay intact.
    consoleOutput("COOP: spawning independent %s probe from loaded definition\n",character);
    unsigned char parameters[12]{};
    reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
    void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(definition),parameters);
    if(!actor)return false;
    reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,position);
    reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,outgoing.orientation);
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x759e70)(reinterpret_cast<void*>(spawner),actor);
    reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x759a50)(reinterpret_cast<void*>(spawnerComponent),actor);
    probeActor=reinterpret_cast<std::uintptr_t>(actor);probeGame=observedGame;
    memoryRead(probeActor+0xa8,&probeHandle,4);
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0x87b780)(actor,emily?"d2coop_probe_emily":"d2coop_probe_corvo");
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(actor);
    // Presentation initialization hides the render entity without setting the
    // explicit hidden flag. The native show/update method restores it directly.
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(actor);
    consoleOutput("COOP: independent probe handle %08x at %p\n",probeHandle,actor);
    return true;
}
std::uintptr_t findNativeSequence(std::uintptr_t game,std::uint32_t id) {
    if(!game||!id)return 0;
    std::uintptr_t result=0;
    for(unsigned field:{0u,0x610u}) {
        const auto manager=game+0x1f5338;
        std::uintptr_t items=0;std::uint32_t flags=0;
        if(!memoryRead(manager+field,&items,8)||!memoryRead(manager+field+8,&flags,4)||(flags&0xffffff)>512||((flags&0xffffff)&&!items))return 0;
        for(unsigned i=0;i<(flags&0xffffff);++i) {
            std::uintptr_t instance=0;std::uint32_t current=0;
            if(!memoryRead(items+i*8,&instance,8)||!instance||!memoryRead(instance+0x158,&current,4))return 0;
            if(current==id){if(result&&result!=instance)return 0;result=instance;}
        }
        std::uintptr_t finalItems=0;std::uint32_t finalFlags=0;
        if(!memoryRead(manager+field,&finalItems,8)||finalItems!=items||!memoryRead(manager+field+8,&finalFlags,4)||finalFlags!=flags)return 0;
    }
    return result;
}
void beginCinematicHold(ULONGLONG tick,bool barrier=false) {
    // A bounded research lease uses the engine's own locked pause/resume
    // manager. It is not yet a shared entry barrier or playback controller.
    if(cinematicHold.id||!observedGame||outgoing.gameLogicState!=5||outgoing.pendingGameLogicState||
       !outgoing.sequenceMetadataValid||outgoing.sequenceCount!=1||outgoing.sequencePaused||(!barrier&&!outgoing.cinematicCanSkip))return;
    const std::uint32_t first=static_cast<std::uint32_t>(sampledSequenceHandles),second=static_cast<std::uint32_t>(sampledSequenceHandles>>32);
    if(first&&second&&first!=second)return;
    const auto id=first?first:second;
    const auto instance=findNativeSequence(observedGame,id);std::uint32_t state=0;
    if(!instance||!memoryRead(instance+0x27c,&state,4)||state!=3)return;
    const bool paused=reinterpret_cast<bool(*)(void*,std::uint32_t,int)>(gameImage+0x7d71f0)(reinterpret_cast<void*>(observedGame+0x1f5338),id,0);
    // Even an unexpected return needs a bounded resume attempt if the native
    // call has actually paused the sequence.
    if(findNativeSequence(observedGame,id)!=instance||!memoryRead(instance+0x27c,&state,4)||state!=5) {
        consoleOutput("COOP: native cinematic hold refused id=%u result=%u\n",id,paused?1:0);return;
    }
    const unsigned lease=barrier?5000:2000;
    cinematicHold={observedGame,instance,outgoing.worldEpoch,id,tick+lease,barrier};++cinematicHoldStarted;
    outgoing.sequencePaused=1;outgoing.cinematicCanSkip=0;outgoing.barrierHeld=barrier?1:0;
    if(barrier)++barrierStarted;
    consoleOutput("COOP: native cinematic hold started id=%u lease=%ums barrier=%u clock=%u result=%u\n",id,lease,barrier?1:0,outgoing.sequenceTimeMs,paused?1:0);
}
void finishCinematicHold(ULONGLONG tick,bool coordinated) {
    if(!cinematicHold.id||(!coordinated&&tick<cinematicHold.until))return;
    const auto held=cinematicHold;cinematicHold={};
    outgoing.barrierHeld=0;
    if(held.barrier&&!coordinated)++barrierTimedOut;
    if(observedGame!=held.game||outgoing.worldEpoch!=held.worldEpoch||findNativeSequence(held.game,held.id)!=held.instance) {
        ++cinematicHoldAbandoned;consoleOutput("COOP: cinematic hold invalidated by world or instance change\n");return;
    }
    std::uint32_t state=0;
    if(!memoryRead(held.instance+0x27c,&state,4)||state!=5) {
        ++cinematicHoldAbandoned;consoleOutput("COOP: cinematic hold no longer owns a paused sequence id=%u state=%u\n",held.id,state);return;
    }
    const bool resumed=reinterpret_cast<bool(*)(void*,std::uint32_t,int)>(gameImage+0x7d8950)(reinterpret_cast<void*>(held.game+0x1f5338),held.id,0);
    if(resumed)++cinematicHoldResumed;else ++cinematicHoldAbandoned;
    if(resumed){outgoing.sequencePaused=0;if(held.barrier&&coordinated)++barrierReleased;}
    consoleOutput("COOP: native cinematic hold released id=%u result=%u\n",held.id,resumed?1:0);
}
void serviceCinematicBarrier(ULONGLONG tick) {
    if(!nativeMutationAllowed||!cinematicBarrier)return;
    if(!cinematicHold.id&&coopOpeningBarrierScene(outgoing)&&
       (lastBarrierWorldEpoch!=outgoing.worldEpoch||lastBarrierHandles!=sampledSequenceHandles)) {
        lastBarrierWorldEpoch=outgoing.worldEpoch;lastBarrierHandles=sampledSequenceHandles;
        if(outgoing.sequenceTimeMs<=100&&remote.barrierEnabled&&lastReceive&&tick-lastReceive<1000)beginCinematicHold(tick,true);
        else {++barrierMissed;consoleOutput("COOP: opening rendezvous missed entry window clock=%u peerEnabled=%u\n",outgoing.sequenceTimeMs,remote.barrierEnabled);}
    }
    // A host arriving last can release in the same pump that first observes
    // its segment. Publish that pose reliably ahead of the release so the
    // receiver validates against the matching source epoch, even if an older
    // unreliable pose was the latest snapshot it previously received.
    if(cinematicHold.id&&cinematicHold.barrier&&!cinematicHold.announced&&sockets&&connection&&
       connectionState==k_ESteamNetworkingConnectionState_Connected) {
        ++outgoing.sequence;if(!outgoing.sequence)++outgoing.sequence;
        if(sockets->SendMessageToConnection(connection,&outgoing,sizeof(outgoing),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK) {
            ++sent;lastSend=tick;cinematicHold.announced=true;
        }
    }
    if(localRole==0&&cinematicHold.id&&cinematicHold.barrier&&cinematicHold.announced&&tick<cinematicHold.until&&sockets&&connection&&
       connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000&&coopBarrierReady(outgoing,remote)) {
        CoopBarrierPacket release{};release.role=localRole;release.sequence=++barrierSequence;
        release.sourceSession=outgoing.session;release.targetSession=remote.session;
        release.sourceWorldEpoch=outgoing.worldEpoch;release.targetWorldEpoch=remote.worldEpoch;
        release.sourceCinematicEpoch=outgoing.cinematicEpoch;release.targetCinematicEpoch=remote.cinematicEpoch;
        release.mapId=outgoing.mapId;release.cinematicSceneId=coopSkipSceneId(outgoing);release.sequenceBoundaryId=outgoing.sequenceBoundaryId;
        if(sockets->SendMessageToConnection(connection,&release,sizeof(release),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK) {
            ++barrierSent;finishCinematicHold(tick,true);consoleOutput("COOP: opening rendezvous released through Steam sequence=%llu\n",release.sequence);
        }
    }
}
bool allowedCommand(const char* text) {
    if(!std::strcmp(text,"coop_checkpoint_sync on")||!std::strcmp(text,"coop_checkpoint_sync off")||
       !std::strcmp(text,"coop_checkpoint_delay 0")||!std::strcmp(text,"coop_checkpoint_delay 1500")||
       !std::strcmp(text,"coop_checkpoint_delay 10000")||!std::strcmp(text,"RestartFromCheckpoint")||!std::strcmp(text,"RestartFromMemoryCheckpoint"))return true;
    if(!std::strcmp(text,"coop_loading_probe 2000")||!std::strcmp(text,"coop_loading_probe emily"))return true;
    if(!std::strcmp(text,"coop_character_probe emily")||!std::strcmp(text,"coop_character_probe corvo"))return true;
    if(!std::strcmp(text,"coop_guard_link 8000")||!std::strcmp(text,"coop_guard_link off")||!std::strcmp(text,"coop_guard_nudge 1000")||!std::strcmp(text,"coop_guard_stall 1000"))return true;
    if(!std::strcmp(text,"coop_world_hold_ai 2000")||!std::strcmp(text,"coop_world_hold_ai off"))return true;
    if(!std::strcmp(text,"coop_world_observe on")||!std::strcmp(text,"coop_world_observe off"))return true;
    if(!std::strcmp(text,"coop_probe_locomotion on")||!std::strcmp(text,"coop_probe_locomotion off"))return true;
    if(!std::strcmp(text,"coop_cinematic_barrier on")||!std::strcmp(text,"coop_cinematic_barrier off"))return true;
    if(!std::strcmp(text,"coop_cinematic_hold 2000"))return true;
    if(!std::strcmp(text,"coop_cinematic_sync on")||!std::strcmp(text,"coop_cinematic_sync off"))return true;
    if(!std::strcmp(text,"coop_probe_trace_transform"))return true;
    if(!std::strcmp(text,"coop_probe_trace_pose"))return true;
    if(!std::strcmp(text,"coop_spawn_probe emily")||!std::strcmp(text,"coop_spawn_probe corvo"))return true;
    if(!std::strcmp(text,"coop_probe_follow on")||!std::strcmp(text,"coop_probe_follow off"))return true;
    const char* commands[]={"getviewpos","where","Ansel_PlayerVisible","LoadGame","listDecls","debugEntityByName","trigger","teleport","setviewpos","find","listModels"};
    for(const auto* name:commands){const auto n=std::strlen(name);if(!std::strncmp(text,name,n)&&(text[n]==0||text[n]==' '))return true;}
    return false;
}
void commandPump(void* system) {
    if(!*commandPath)return;
    struct Request{std::uint32_t magic,sequence;std::uint64_t expires;char command[512];} request{};
    HANDLE file=CreateFileA(commandPath,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);
    DWORD size=0;if(file==INVALID_HANDLE_VALUE)return;
    const bool read=ReadFile(file,&request,sizeof(request),&size,nullptr)!=FALSE;CloseHandle(file);
    if(!read||size!=sizeof(request)||request.magic!=0x43433244||!request.sequence||request.sequence==lastCommand)return;
    lastCommand=request.sequence;
    FILETIME time{};GetSystemTimeAsFileTime(&time);
    const std::uint64_t now=(static_cast<std::uint64_t>(time.dwHighDateTime)<<32)|time.dwLowDateTime;
    if(request.expires<=now||request.expires-now>600000000ULL)return;
    if(!std::memchr(request.command,0,sizeof(request.command))||!allowedCommand(request.command))return;
    for(const unsigned char* p=reinterpret_cast<unsigned char*>(request.command);*p;++p)if(*p<32||*p>126||*p==';')return;
    if(!std::strcmp(request.command,"coop_checkpoint_sync on")){checkpointEnable(true);return;}
    if(!std::strcmp(request.command,"coop_checkpoint_sync off")){checkpointEnable(false);return;}
    if(!std::strncmp(request.command,"coop_checkpoint_delay ",22)){checkpointSync.readyDelay=std::strtoul(request.command+22,nullptr,10);return;}
    if(!std::strcmp(request.command,"coop_loading_probe 2000")){armLoadingProbe();return;}
    if(!std::strcmp(request.command,"coop_loading_probe emily")){armLoadingProbe(2);return;}
    if(!std::strcmp(request.command,"coop_character_probe emily")){probeNativeCharacter(2);return;}
    if(!std::strcmp(request.command,"coop_character_probe corvo")){probeNativeCharacter(1);return;}
    if(!std::strcmp(request.command,"coop_guard_link 8000")){beginGuardLink(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_guard_link off")){stopGuardLink("cancelled");return;}
    if(!std::strcmp(request.command,"coop_guard_nudge 1000")){beginGuardNudge(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_guard_stall 1000")) {
        if(!localRole&&guardLink.phase==GuardPhase::Active)guardStreamBlockedUntil=GetTickCount64()+1000;
        return;
    }
    if(!std::strncmp(request.command,"LoadGame",8))stopGuardLink("load_requested");
    if(!std::strcmp(request.command,"coop_world_hold_ai 2000")){beginAiDecisionHold(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_world_hold_ai off")){releaseAiDecisionHold("cancelled");return;}
    if(!std::strncmp(request.command,"LoadGame",8))releaseAiDecisionHold("load_requested");
    if(!std::strcmp(request.command,"coop_cinematic_hold 2000")){beginCinematicHold(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_cinematic_barrier on")||!std::strcmp(request.command,"coop_cinematic_barrier off")) {
        cinematicBarrier=!std::strcmp(request.command,"coop_cinematic_barrier on");outgoing.barrierEnabled=cinematicBarrier?1:0;
        if(!cinematicBarrier&&cinematicHold.barrier){cinematicHold.until=0;finishCinematicHold(GetTickCount64());}
        consoleOutput("COOP: bounded opening rendezvous %s\n",cinematicBarrier?"enabled":"disabled");return;
    }
    if(!std::strcmp(request.command,"coop_cinematic_sync on")||!std::strcmp(request.command,"coop_cinematic_sync off")) {
        cinematicSync=!std::strcmp(request.command,"coop_cinematic_sync on");outgoing.cinematicSyncEnabled=cinematicSync?1:0;
        consoleOutput("COOP: bounded native confirmed-skip relay %s\n",cinematicSync?"enabled":"disabled");return;
    }
    if(!std::strcmp(request.command,"coop_probe_trace_pose")){startPoseTrace();return;}
    if(!std::strcmp(request.command,"coop_world_observe on")||!std::strcmp(request.command,"coop_world_observe off")) {
        worldObservation=!std::strcmp(request.command,"coop_world_observe on");resetWorldObservation();
        consoleOutput("COOP: read-only world NPC transport %s\n",worldObservation?"enabled":"disabled");return;
    }
    if(!std::strcmp(request.command,"coop_probe_locomotion on")||!std::strcmp(request.command,"coop_probe_locomotion off")) {
        locomotionEnabled=!std::strcmp(request.command,"coop_probe_locomotion on");
        locomotionLastSequence=0;locomotionLastTick=0;std::memset(locomotionVelocity,0,sizeof(locomotionVelocity));
        AcquireSRWLockExclusive(&locomotionLock);locomotionPose.expires=0;ReleaseSRWLockExclusive(&locomotionLock);
        consoleOutput("COOP: experimental native locomotion inputs %s\n",locomotionEnabled?"enabled":"disabled");return;
    }
    if(!std::strcmp(request.command,"coop_probe_trace_transform")) {
        if(probeActor&&probeFollow&&!transformTraceUntil) {
            transformSampleCount=0;transformTraceHandle=probeHandle;transformTraceCommand=lastCommand;
            transformTraceUntil=GetTickCount64()+4000;
        }return;
    }
    if(!std::strncmp(request.command,"coop_spawn_probe ",17)) {
        const bool spawned=spawnProbe(request.command+17);
        consoleOutput("COOP: spawn probe request %u %s\n",lastCommand,spawned?"succeeded":"refused");return;
    }
    if(!std::strcmp(request.command,"coop_probe_follow on")) {
        if(probeActor && coopGameplayReady(outgoing) && coopGameplayReady(remote) && outgoing.mapId==remote.mapId && lastReceive && GetTickCount64()-lastReceive<1000) {
            float position[3]{};memoryRead(probeActor+0x300,position,sizeof(position));
            for(int i=0;i<3;++i)probeOffset[i]=position[i]-remote.position[i];
            std::uintptr_t frame=0,owner=0,vt=0;
            const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
            if(!memoryRead(probeActor+0x358,&frame,8)||!frame||!memoryRead(frame,&vt,8)||vt!=base+0x1c573e8||
               !memoryRead(frame+0x10,&owner,8)||owner!=probeActor)return;
            controlledFrame.store(reinterpret_cast<void*>(frame),std::memory_order_relaxed);
            networkFrameOwned.store(true,std::memory_order_release);
            probeFollow=true;phaseFollowInvalidated=false;probeRemoteEpoch=remote.worldEpoch;probeRemoteSession=remote.session;
            reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(probeActor));
            reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(probeActor));
            consoleOutput("COOP: native entity transform follow enabled; animation and interactions are not replicated\n");
        }return;
    }
    if(!std::strcmp(request.command,"coop_probe_follow off")){networkFrameOwned.store(false,std::memory_order_release);probeFollow=false;return;}
    char command[514]{};std::snprintf(command,sizeof(command),"%s\n",request.command);
    // Native AppendText method; called only inside native ExecuteBuffer's thread.
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0xfec70)(system,command);
    consoleOutput("COOP: queued native command %u: %s\n",lastCommand,request.command);
}
void followProbe(ULONGLONG tick) {
    if(!probeActor)return;
    const auto slot=static_cast<std::int32_t>(probeHandle)>>16;
    std::uintptr_t registered=0,render=0,renderVtable=0;std::uint32_t actual=0;
    if(probeGame!=observedGame||slot<=0||slot>=16384||
       !memoryRead(observedGame+0x3130b0+slot*8,&registered,8)||registered!=probeActor||
       !memoryRead(probeActor+0xa8,&actual,4)||actual!=probeHandle||!memoryRead(probeActor+0xf0,&render,8)||!render||
       !memoryRead(render,&renderVtable,8)) {networkFrameOwned.store(false,std::memory_order_release);controlledFrame.store(nullptr);probeActor=probeGame=probeHandle=0;probeFollow=false;return;}
    if(probeFollow) {
        if(phaseFollowInvalidated||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||outgoing.mapId!=remote.mapId||!lastReceive||tick-lastReceive>=1000||remote.session!=probeRemoteSession||remote.worldEpoch!=probeRemoteEpoch) {
            networkFrameOwned.store(false,std::memory_order_release);
            probeFollow=false;reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(probeActor));
            consoleOutput("COOP: native actor follow stopped on stale peer, changed world, or native game-logic transition\n");
        }else {
            float position[3];for(int i=0;i<3;++i)position[i]=remote.position[i]+probeOffset[i];
            publishLocomotion(tick,position);
            // Validate the native reference frame before the entity-wide
            // transform dispatch. This dispatch notifies active components,
            // including physics, before replacing the cached/render pose.
            std::uintptr_t frame=0,frameVtable=0,owner=0,setTransform=0;
            const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
            if(!memoryRead(probeActor+0x358,&frame,8)||!frame||
               !memoryRead(frame,&frameVtable,8)||frameVtable!=base+0x1c573e8||
               !memoryRead(frame+0x10,&owner,8)||owner!=probeActor||
               !memoryRead(frameVtable+22*8,&setTransform,8)||setTransform!=base+0x751d00) {
                networkFrameOwned.store(false,std::memory_order_release);
                probeFollow=false;consoleOutput("COOP: native reference-frame validation failed\n");
            }else {
                NativeTransformSample sample{};bool capture=false;
                if(transformTraceUntil&&tick<transformTraceUntil&&probeHandle==transformTraceHandle&&transformSampleCount<128&&
                   memoryRead(probeActor+0x300,sample.before,sizeof(sample.before))) {
                    bool changed=transformSampleCount==0;
                    for(int i=0;i<3;++i)changed=changed||std::fabs(sample.before[i]-position[i])>0.0001f;
                    for(int i=0;i<9;++i)changed=changed||std::fabs(sample.before[i+3]-remote.orientation[i])>0.0001f;
                    capture=changed&&readNativeBodies(probeActor,sample.bodiesBefore);
                    if(capture)sample.deferred=reinterpret_cast<bool(*)(void*)>(gameImage+0x8a7820)(reinterpret_cast<void*>(observedGame));
                }
                reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(
                    reinterpret_cast<void*>(probeActor),position,remote.orientation,true,0);
                if(capture&&memoryRead(probeActor+0x300,sample.after,sizeof(sample.after))&&readNativeBodies(probeActor,sample.bodiesAfter))
                    transformSamples[transformSampleCount++]=sample;
            }
        }
    }
    memoryRead(render+0x1860,probeRenderPosition,sizeof(probeRenderPosition));
}
void writeStatus(ULONGLONG tick) {
    if(!*statusPath||tick-lastStatus<250)return;lastStatus=tick;
    writeWorldObservation(tick);
    writeGuardStatus(tick);
    writeCheckpointTrace();
    writeSwitchTrace();
    writeReplacementTrace();
    writeCheckpointSync();
    char text[4096]{};
    int length=std::snprintf(text,sizeof(text),
        "{\"role\":%u,\"engineThread\":%lu,\"screenTransportPumps\":%llu,\"nativeInterface\":\"%s\",\"steamAvailable\":%s,\"listener\":%u,\"connection\":%u,\"connectionState\":%d,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"lastCommand\":%u,\"localPoseAvailable\":%s,\"localPosition\":[%.6f,%.6f,%.6f],\"remotePoseAvailable\":%s,\"remoteFresh\":%s,\"remoteSequence\":%u,\"remotePosition\":[%.6f,%.6f,%.6f],\"probeActorHandle\":%u,\"probeFollowEnabled\":%s,\"probeRenderPosition\":[%.6f,%.6f,%.6f],\"probeOffset\":[%.6f,%.6f,%.6f],\"remoteActorImplemented\":false,\"localMap\":\"%s\",\"localMapId\":\"%llx\",\"localEpoch\":%llu,\"remoteMapId\":\"%llx\",\"remoteEpoch\":%llu,\"localGameLogicState\":%u,\"localPendingGameLogicState\":%u,\"remoteGameLogicState\":%u,\"remotePendingGameLogicState\":%u,\"localCinematicMetadataValid\":%u,\"localCinematicNodeCount\":%u,\"localCinematicSignature\":\"%llx\",\"remoteCinematicMetadataValid\":%u,\"remoteCinematicNodeCount\":%u,\"remoteCinematicSignature\":\"%llx\"}\n",
        localRole,engineThread.load(),screenTransportPumps,STEAMNETWORKINGSOCKETS_INTERFACE_VERSION,sockets?"true":"false",listener,connection,connectionState,sent,received,rejected,lastCommand,
        outgoing.flags&1?"true":"false",outgoing.position[0],outgoing.position[1],outgoing.position[2],remote.flags&1?"true":"false",lastReceive&&tick-lastReceive<1000?"true":"false",remote.sequence,remote.position[0],remote.position[1],remote.position[2],probeHandle,probeFollow?"true":"false",probeRenderPosition[0],probeRenderPosition[1],probeRenderPosition[2],probeOffset[0],probeOffset[1],probeOffset[2],mapName,outgoing.mapId,outgoing.worldEpoch,remote.mapId,remote.worldEpoch,
        outgoing.gameLogicState,outgoing.pendingGameLogicState,remote.gameLogicState,remote.pendingGameLogicState,outgoing.cinematicMetadataValid,outgoing.cinematicNodeCount,outgoing.cinematicSignature,remote.cinematicMetadataValid,remote.cinematicNodeCount,remote.cinematicSignature);
    if(length<=0||length>=sizeof(text))return;
    const int extra=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"localCinematicEpoch\":%llu,\"remoteCinematicEpoch\":%llu,\"cinematicSyncEnabled\":%s,\"remoteCinematicSyncEnabled\":%u,\"localCinematicCanSkip\":%u,\"remoteCinematicCanSkip\":%u,\"nativeSkipCallbacks\":%llu,\"skipSent\":%llu,\"skipReceived\":%llu,\"skipApplied\":%llu,\"skipDropped\":%llu,\"localSkipSceneId\":\"%llx\",\"remoteSkipSceneId\":\"%llx\",\"nativeSkipThread\":%lu}\n",
        outgoing.cinematicEpoch,remote.cinematicEpoch,cinematicSync?"true":"false",remote.cinematicSyncEnabled,
        outgoing.cinematicCanSkip,remote.cinematicCanSkip,nativeSkipCallbacks.load(),skipSent,skipReceived,skipApplied,skipDropped,
        coopSkipSceneId(outgoing),coopSkipSceneId(remote),nativeSkipThread.load());
    if(extra<=0||extra>=sizeof(text)-length+2)return;length=length-2+extra;
    const int sequences=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"poseProtocol\":%u,\"posePacketBytes\":%u,\"localSequenceMetadataValid\":%u,\"localSequenceCount\":%u,\"localSequenceSignature\":\"%llx\",\"localSequenceBoundaryId\":\"%llx\",\"remoteSequenceMetadataValid\":%u,\"remoteSequenceCount\":%u,\"remoteSequenceSignature\":\"%llx\",\"remoteSequenceBoundaryId\":\"%llx\"}\n",
        outgoing.version,outgoing.size,outgoing.sequenceMetadataValid,outgoing.sequenceCount,outgoing.sequenceSignature,outgoing.sequenceBoundaryId,
        remote.sequenceMetadataValid,remote.sequenceCount,remote.sequenceSignature,remote.sequenceBoundaryId);
    if(sequences<=0||sequences>=sizeof(text)-length+2)return;length=length-2+sequences;
    const int hold=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"cinematicHoldActive\":%s,\"cinematicHoldId\":%u,\"cinematicHoldStarted\":%llu,\"cinematicHoldResumed\":%llu,\"cinematicHoldAbandoned\":%llu}\n",
        cinematicHold.id?"true":"false",cinematicHold.id,cinematicHoldStarted,cinematicHoldResumed,cinematicHoldAbandoned);
    if(hold<=0||hold>=sizeof(text)-length+2)return;length=length-2+hold;
    const int barrier=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"localSequenceTimeMs\":%u,\"localSequencePaused\":%u,\"remoteSequenceTimeMs\":%u,\"remoteSequencePaused\":%u,\"barrierEnabled\":%u,\"barrierHeld\":%u,\"remoteBarrierEnabled\":%u,\"remoteBarrierHeld\":%u,\"barrierStarted\":%llu,\"barrierReleased\":%llu,\"barrierTimedOut\":%llu,\"barrierMissed\":%llu,\"barrierSent\":%llu,\"barrierReceived\":%llu,\"barrierRejected\":%llu}\n",
        outgoing.sequenceTimeMs,outgoing.sequencePaused,remote.sequenceTimeMs,remote.sequencePaused,outgoing.barrierEnabled,outgoing.barrierHeld,remote.barrierEnabled,remote.barrierHeld,
        barrierStarted,barrierReleased,barrierTimedOut,barrierMissed,barrierSent,barrierReceived,barrierRejected);
    if(barrier<=0||barrier>=sizeof(text)-length+2)return;length=length-2+barrier;
    const int animation=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"locomotionEnabled\":%s,\"locomotionApplied\":%llu,\"locomotionRefused\":%llu,\"locomotionSpeed\":%.6f}\n",
        locomotionEnabled?"true":"false",locomotionApplied.load(),locomotionRefused.load(),locomotionAppliedSpeed.load());
    if(animation<=0||animation>=sizeof(text)-length+2)return;length=length-2+animation;
    const int ai=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"aiHoldActive\":%s,\"aiHoldHandle\":%u,\"aiHoldStarted\":%llu,\"aiHoldReleased\":%llu,\"aiHoldRefused\":%llu,\"aiHoldInvalidated\":%llu,\"aiHoldResult\":\"%s\"}\n",
        aiDecisionHold.expires?"true":"false",aiHoldLastHandle,aiHoldStarted,aiHoldReleased,aiHoldRefused,aiHoldInvalidated,aiHoldResult);
    if(ai<=0||ai>=sizeof(text)-length+2)return;length=length-2+ai;
    char temporary[MAX_PATH]{};std::snprintf(temporary,sizeof(temporary),"%s.tmp",statusPath);
    HANDLE file=CreateFileA(temporary,GENERIC_WRITE,FILE_SHARE_READ,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);
    if(file!=INVALID_HANDLE_VALUE){DWORD done=0;const bool ok=WriteFile(file,text,length,&done,nullptr)&&done==static_cast<DWORD>(length);CloseHandle(file);if(ok)MoveFileExA(temporary,statusPath,MOVEFILE_REPLACE_EXISTING);}
}
void pumpTransport(ULONGLONG tick) {
    if(tick-lastTransportPump<10)return;
    lastTransportPump=tick;initializeSteam(tick);pumpConnections(tick);sampleLocalPlayer();
    if(nativeMutationAllowed){finishCinematicHold(tick);serviceCinematicBarrier(tick);}
    prepareWorldObservation(tick);relayConfirmedSkip(tick);exchangePoses(tick);checkpointService(tick);sendWorldObservation(tick);
    if(probeFollow&&(!coopGameplayReady(outgoing)||!coopGameplayReady(remote))) {
        phaseFollowInvalidated=true;networkFrameOwned.store(false,std::memory_order_release);
    }
}
void executeBuffer(void* system) {
    const DWORD thread=GetCurrentThreadId();
    DWORD unassigned=0;engineThread.compare_exchange_strong(unassigned,thread);
    if(thread==engineThread.load()) {
        const auto tick=GetTickCount64();
        if(tick-lastPump>=10) {
            installCheckpointCommandObservers();
            lastPump=tick;nativeMutationAllowed=true;pumpTransport(tick);nativeMutationAllowed=false;
            serviceLoadingProbe(tick);finishCinematicHold(tick);serviceAiDecisionHold(tick);commandPump(system);checkpointEngineService(system);serviceGuardLink(tick);followProbe(tick);finishTransformTrace(tick);writeStatus(tick);
        }
    }
    realExecuteBuffer(system);
}
void updateNativeScreen(void* common,std::uintptr_t second,int third,bool fourth,std::uintptr_t fifth,bool sixth) {
    // Movie playback bypasses command execution but keeps the native screen
    // callback active. Never run engine work from an input polling worker.
    if(bridgeInstalled.load(std::memory_order_acquire)&&GetCurrentThreadId()==engineThread.load()) {
        const auto tick=GetTickCount64();
        static thread_local bool active=false;
        if(!active&&tick-lastPump>=50) {
            active=true;++screenTransportPumps;pumpTransport(tick);writeStatus(tick);active=false;
        }
    }
    realCommonScreenUpdate(common,second,third,fourth,fifth,sixth);
}
}
long handleCoopPoseTrace(EXCEPTION_POINTERS* event) {
    if(event->ExceptionRecord->ExceptionCode!=EXCEPTION_SINGLE_STEP||
       event->ContextRecord->Dr0!=poseTraceAddress||!(event->ContextRecord->Dr6&1)||!(event->ContextRecord->Dr7&1))return EXCEPTION_CONTINUE_SEARCH;
    void* frames[8]{};const auto frameCount=CaptureStackBackTrace(0,8,frames,nullptr);
    const auto caller=frameCount>6?reinterpret_cast<std::uintptr_t>(frames[6]):0;
    // Ignore our own native teleport call; retain native simulation writers.
    const bool own=caller>=modImage&&caller<modImage+modImageSize;
    const LONG index=own?32:InterlockedIncrement(&poseTraceCount)-1;
    if(index>=0&&index<32) {
        auto& w=poseWrites[index];const auto* c=event->ContextRecord;
        w.rip=c->Rip;w.rcx=c->Rcx;w.rdx=c->Rdx;w.rbx=c->Rbx;w.rsi=c->Rsi;
        memoryRead(poseTraceAddress,w.position,sizeof(w.position));
        w.frameCount=frameCount;std::memcpy(w.frames,frames,sizeof(frames));w.thread=GetCurrentThreadId();
        InterlockedExchange(&w.ready,1);
    }
    event->ContextRecord->Dr6&=~1ULL;
    return EXCEPTION_CONTINUE_EXECUTION;
}
bool installEngineBridge(unsigned char* image,const char* role) {
    auto* slot=reinterpret_cast<void**>(image+0x1aef4a8);
    auto* screenSlot=reinterpret_cast<void**>(image+0x1ac4d88+23*8);
    const unsigned char printBytes[]={0x48,0x89,0x54,0x24,0x10,0x4c,0x89,0x44,0x24,0x18,0x4c,0x89,0x4c,0x24,0x20,0xc3};
    const unsigned char frameBytes[]={0x40,0x55,0x53,0x56,0x57,0x48,0x8d,0x6c,0x24,0xc8,0x48,0x81,0xec,0x38,0x01,0x00,0x00};
    const unsigned char skipBytes[]={0x40,0x57,0x48,0x83,0xec,0x50,0x80,0x89,0xc0,0x09,0x00,0x00,0x08,0x4c,0x8b,0x41,0x10};
    const unsigned char pauseSequenceBytes[]={0x40,0x55,0x56,0x57,0x41,0x56,0x41,0x57,0x48,0x83,0xec,0x40};
    const unsigned char resumeSequenceBytes[]={0x40,0x55,0x56,0x57,0x48,0x83,0xec,0x40};
    const unsigned char animationBytes[]={0x40,0x55,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x81,0xec,0xc0,0,0,0};
    const unsigned char requestVelocityBytes[]={0x8b,0x02,0x89,0x01,0x8b,0x42,0x04,0x89,0x41,0x04};
    const unsigned char requestFacingBytes[]={0x8b,0x02,0x89,0x41,0x0c,0x8b,0x42,0x04,0x89,0x41,0x10};
    if(*slot!=image+0xff8f0 || *screenSlot!=image+0x48ad0 || std::memcmp(image+0x166830,printBytes,sizeof(printBytes))||
       std::memcmp(image+0x7d71f0,pauseSequenceBytes,sizeof(pauseSequenceBytes))||std::memcmp(image+0x7d8950,resumeSequenceBytes,sizeof(resumeSequenceBytes))||
       std::memcmp(image+0x540f20,animationBytes,sizeof(animationBytes))||
       std::memcmp(image+0x486d40,requestVelocityBytes,sizeof(requestVelocityBytes))||std::memcmp(image+0x486bc0,requestFacingBytes,sizeof(requestFacingBytes))||
       std::memcmp(image+0x753c10,frameBytes,sizeof(frameBytes))||std::memcmp(image+0x78a5b0,skipBytes,sizeof(skipBytes)))return false;
    gameImage=image;localRole=std::strcmp(role,"host")?1:0;outgoing.role=localRole;
    HMODULE module=nullptr;
    if(GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,reinterpret_cast<LPCSTR>(&installEngineBridge),&module)) {
        modImage=reinterpret_cast<std::uintptr_t>(module);
        const auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(module);
        modImageSize=reinterpret_cast<IMAGE_NT_HEADERS64*>(modImage+dos->e_lfanew)->OptionalHeader.SizeOfImage;
    }
    outgoing.session=(GetTickCount64()<<32)|GetCurrentProcessId();
    GetEnvironmentVariableA("D2COOP_COMMAND_FILE",commandPath,sizeof(commandPath));
    GetEnvironmentVariableA("D2COOP_STATUS_FILE",statusPath,sizeof(statusPath));
    GetEnvironmentVariableA("D2COOP_ENGINE_LOG",consolePath,sizeof(consolePath));
    GetEnvironmentVariableA("D2COOP_CHECKPOINT_DIR",checkpointDirectory,sizeof(checkpointDirectory));
    realExecuteBuffer=reinterpret_cast<ExecuteBuffer>(*slot);
    realCommonScreenUpdate=reinterpret_cast<CommonScreenUpdate>(*screenSlot);
    if(!installNativeSwitchTrace()||!installReplacementTrace())return false;
    DWORD old=0,unused=0;
    auto* animationTrampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    if(!animationTrampoline)return false;
    std::memcpy(animationTrampoline,animationBytes,sizeof(animationBytes));
    unsigned char animationJump[17]{0xff,0x25,0,0,0,0};void* animationResume=image+0x540f20+sizeof(animationBytes);
    std::memcpy(animationJump+6,&animationResume,8);std::memset(animationJump+14,0x90,3);
    std::memcpy(animationTrampoline+sizeof(animationBytes),animationJump,14);
    if(!VirtualProtect(animationTrampoline,64,PAGE_EXECUTE_READ,&old))return false;
    FlushInstructionCache(GetCurrentProcess(),animationTrampoline,64);
    realEvaluateAnimation=reinterpret_cast<EvaluateAnimation>(animationTrampoline);
    void* animationWrapper=reinterpret_cast<void*>(&evaluateAnimation);std::memcpy(animationJump+6,&animationWrapper,8);
    if(!VirtualProtect(image+0x540f20,sizeof(animationJump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(image+0x540f20,animationJump,sizeof(animationJump));FlushInstructionCache(GetCurrentProcess(),image+0x540f20,sizeof(animationJump));
    if(!VirtualProtect(image+0x540f20,sizeof(animationJump),old,&unused))return false;
    // The copied prologue contains no relative addressing. Preserve whole
    // instructions, then resume the original body immediately after it.
    auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    if(!trampoline)return false;
    std::memcpy(trampoline,frameBytes,sizeof(frameBytes));
    unsigned char jump[17]{0x48,0xb8};void* resume=image+0x753c10+sizeof(frameBytes);
    std::memcpy(jump+2,&resume,8);jump[10]=0xff;jump[11]=0xe0;std::memset(jump+12,0x90,5);
    std::memcpy(trampoline+sizeof(frameBytes),jump,12);
    if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old)){VirtualFree(trampoline,0,MEM_RELEASE);return false;}
    realReferenceFrameUpdate=reinterpret_cast<ReferenceFrameUpdate>(trampoline);
    void* wrapper=reinterpret_cast<void*>(&updateReferenceFrame);std::memcpy(jump+2,&wrapper,8);
    if(!VirtualProtect(image+0x753c10,sizeof(jump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(image+0x753c10,jump,sizeof(jump));FlushInstructionCache(GetCurrentProcess(),image+0x753c10,sizeof(jump));
    if(!VirtualProtect(image+0x753c10,sizeof(jump),old,&unused))return false;
    // Preserve R8 loaded by the copied prologue with register-free FF25 jumps.
    auto* skipTrampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
    if(!skipTrampoline)return false;
    std::memcpy(skipTrampoline,skipBytes,sizeof(skipBytes));
    unsigned char skipJump[17]{0xff,0x25,0,0,0,0};void* skipResume=image+0x78a5b0+sizeof(skipBytes);
    std::memcpy(skipJump+6,&skipResume,8);std::memset(skipJump+14,0x90,3);
    std::memcpy(skipTrampoline+sizeof(skipBytes),skipJump,14);
    if(!VirtualProtect(skipTrampoline,64,PAGE_EXECUTE_READ,&old))return false;
    FlushInstructionCache(GetCurrentProcess(),skipTrampoline,64);
    realCinematicSkipConfirmed=reinterpret_cast<CinematicSkipConfirmed>(skipTrampoline);
    void* skipWrapper=reinterpret_cast<void*>(&cinematicSkipConfirmed);std::memcpy(skipJump+6,&skipWrapper,8);
    if(!VirtualProtect(image+0x78a5b0,sizeof(skipJump),PAGE_EXECUTE_READWRITE,&old))return false;
    std::memcpy(image+0x78a5b0,skipJump,sizeof(skipJump));FlushInstructionCache(GetCurrentProcess(),image+0x78a5b0,sizeof(skipJump));
    if(!VirtualProtect(image+0x78a5b0,sizeof(skipJump),old,&unused))return false;
    if(!VirtualProtect(slot,sizeof(*slot),PAGE_READWRITE,&old))return false;
    InterlockedExchangePointer(slot,reinterpret_cast<void*>(&executeBuffer));
    if(!VirtualProtect(slot,sizeof(*slot),old,&unused))return false;
    if(!VirtualProtect(screenSlot,sizeof(*screenSlot),PAGE_READWRITE,&old))return false;
    InterlockedExchangePointer(screenSlot,reinterpret_cast<void*>(&updateNativeScreen));
    if(!VirtualProtect(screenSlot,sizeof(*screenSlot),old,&unused))return false;
    // Capturing the retail no-op printf stub changes its behavior. Keep it
    // opt-in for diagnostics; mod-owned log messages do not need this hook.
    char capture[8]{};GetEnvironmentVariableA("D2COOP_CAPTURE_NATIVE_LOG",capture,sizeof(capture));
    if(*consolePath&&!std::strcmp(capture,"1")) {
        unsigned char printJump[16]{0x48,0xb8};void* target=reinterpret_cast<void*>(&consoleOutput);
        std::memcpy(printJump+2,&target,8);printJump[10]=0xff;printJump[11]=0xe0;std::memset(printJump+12,0x90,4);
        if(!VirtualProtect(image+0x166830,sizeof(printJump),PAGE_EXECUTE_READWRITE,&old))return false;
        std::memcpy(image+0x166830,printJump,sizeof(printJump));FlushInstructionCache(GetCurrentProcess(),image+0x166830,sizeof(printJump));
        if(!VirtualProtect(image+0x166830,sizeof(printJump),old,&unused))return false;
    }
    bridgeInstalled.store(true,std::memory_order_release);
    return true;
}
