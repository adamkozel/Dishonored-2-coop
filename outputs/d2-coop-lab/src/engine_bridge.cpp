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
#include "pose_interpolation.h"
#include "world_protocol.h"
#include "world_identity.h"
#include "native_ai_state.h"
#include "native_npc_authority.h"
#include "npc_authority_protocol.h"
#include "npc_combat_protocol.h"
#include "native_ai_retirement.h"
#include "native_sword_retirement.h"
#include "guard_protocol.h"
#include "damage_protocol.h"
#include "native_character.h"
#include "native_gameover.h"
#include "native_player_stance.h"
#include "native_player_states.h"
#include "native_player_traversal_scene.h"
#include "native_player_blink.h"
#include "native_health.h"
#include "native_faction.h"
#include "body_retarget.h"
#include "arm_retarget.h"
#include "arm_protocol.h"
#include "avatar_weapon_protocol.h"
#include "shadow_action_protocol.h"
#include "guard_body_protocol.h"
#include "native_sword_equipment.h"
#include "native_player_sword.h"
#include "native_player_power_item.h"
#include "guard_weapon_protocol.h"
#include "native_child_frames.h"
#include "enemy_outcome_protocol.h"
#include "enemy_melee_protocol.h"

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
NativePlayerBlink sampledNativeBlink{};
bool sampledBlinkValid=false,sampledBlinkStateKnown=false,sampledBlinkStatePresent=false;
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
bool npcAuthorityHoldActive=false;
bool npcAuthorityStreamActive=false;
bool handleNpcReferenceFrame(void* frame);
bool guardWeaponEnabled=false;
void updateGuardChildren(void* frame);
void updateAvatarChildren(void* frame);
void updateReferenceFrame(void* frame) {
    // Native job workers otherwise integrate the NPC's local motion into its
    // frame after a received pose. Only our registered remote frame is owned
    // by the network; all other reference frames keep their native update.
    if(networkFrameOwned.load(std::memory_order_acquire)&&controlledFrame.load(std::memory_order_relaxed)==frame){updateAvatarChildren(frame);return;}
    if(guardControlledFrame.load(std::memory_order_acquire)==frame&&GetTickCount64()<guardFrameUntil.load(std::memory_order_acquire)){updateGuardChildren(frame);return;}
    if(handleNpcReferenceFrame(frame))return;
    realReferenceFrameUpdate(frame);
}
std::uint32_t localRole=0,lastCommand=0;
// 1181: installed release (no lab environment); data in <game>\d2coop.
bool productMode=false;char productDataDir[MAX_PATH]{};
extern bool productAutoArm;
// 1203d: partner-presentation freshness. The lab keeps its strict 250 ms
// guards; under the product controller ordinary frame/network jitter (1204c:
// 250 ms root gaps every few seconds on the busy mission map) must not retire
// the stream/bank - the partner holds its last pose for up to 1 s, the same
// tolerance native follow already uses.
inline ULONGLONG presentationFreshMs(){return (productMode||productAutoArm)?1000ull:250ull;}
inline ULONGLONG presentationHelloMs(){return (productMode||productAutoArm)?1500ull:350ull;}
// 1203f: per-channel reliable liveness hellos for door/usable catalogs. The lab
// keeps 250 ms hellos with a 1 s readiness window. Under the product controller a
// channel whose peer is already bound sends its hello every 2 s and stays ready
// for 6 s: 145 Edge-of-the-World usables at 4 Hz x 504 B was ~292 KB/s of
// reliable traffic per direction (over half the 512 KB/s budget) and delayed
// partner presentation behind a ~0.45 MB reliable backlog.
inline ULONGLONG channelHelloIntervalMs(bool peerBound){return (productMode||productAutoArm)&&peerBound?2000ull:250ull;}
inline ULONGLONG channelHelloReadyMs(){return (productMode||productAutoArm)?6000ull:1000ull;}
bool partnerAssetsEnabled=false;
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
bool probeFollowExact=false;
bool probeDoppelganger=false; // Set only by this lab's successful native spawn.
bool phaseFollowInvalidated=false;
std::uint64_t probeRemoteEpoch=0,probeRemoteSession=0;
float probeOffset[3]{},probeRenderPosition[3]{};
CoopPosePacket outgoing{},remote{};
PoseInterpolation poseInterpolation{};
CalibratedSourcePoseTimeline sourcePoseTimeline{};
std::uint64_t sourceLateRebases(){return sourcePoseTimeline.lateRebases;}
bool poseSmoothingEnabled=true;
float probeTargetPosition[3]{};
double presentationClock(){static const double factor=[](){LARGE_INTEGER f{};QueryPerformanceFrequency(&f);return 1000.0/static_cast<double>(f.QuadPart);}();LARGE_INTEGER t{};QueryPerformanceCounter(&t);return static_cast<double>(t.QuadPart)*factor;}
struct PoseClockDiagnostic {double arrival=0,source=0,gap=0,offsetBefore=0,offsetAfter=0,mapped=0;std::uint64_t session=0,epoch=0;unsigned sequence=0,kind=0;};
std::array<PoseClockDiagnostic,128> poseClockDiagnostics{};std::uint64_t poseClockDiagnosticCount=0;
std::array<PoseClockDiagnostic,16> poseClockBoundaries{};std::uint64_t poseClockBoundaryCount=0;
void recordPoseClock(const CoopPosePacket& p,double arrival,const SourcePoseTimeline& before,double mapped,unsigned kind){
    const PoseClockDiagnostic value{arrival,double(p.sampleTimeUs)*.001,before.lastArrival?arrival-before.lastArrival:0,before.offset,sourcePoseTimeline.offset,mapped,p.session,p.worldEpoch,p.sequence,kind};
    poseClockDiagnostics[poseClockDiagnosticCount++%poseClockDiagnostics.size()]=value;
    if(kind==1||kind==2||kind==5)poseClockBoundaries[poseClockBoundaryCount++%poseClockBoundaries.size()]=value;
}
void writePoseClockDiagnostics(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.pose-clock.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"count\":%llu,\"offset\":%.6f,\"lastSource\":%llu,\"lastArrival\":%.6f,\"records\":[",poseClockDiagnosticCount,sourcePoseTimeline.offset,sourcePoseTimeline.lastSource,sourcePoseTimeline.lastArrival);
    const auto count=(std::min)(poseClockDiagnosticCount,static_cast<std::uint64_t>(poseClockDiagnostics.size()));
    for(std::uint64_t i=0;i<count;++i){const auto& r=poseClockDiagnostics[(poseClockDiagnosticCount-count+i)%poseClockDiagnostics.size()];
        fprintf(f,"%s{\"kind\":%u,\"arrival\":%.6f,\"source\":%.6f,\"gap\":%.6f,\"offsetBefore\":%.6f,\"offsetAfter\":%.6f,\"mapped\":%.6f,\"session\":%llu,\"epoch\":%llu,\"sequence\":%u}",i?",":"",r.kind,r.arrival,r.source,r.gap,r.offsetBefore,r.offsetAfter,r.mapped,r.session,r.epoch,r.sequence);
    }
    const auto& c=sourcePoseTimeline.calibration;
    fprintf(f,"],\"calibration\":{\"samples\":%u,\"nearSamples\":%u,\"minimumOffset\":%.6f,\"firstArrival\":%.6f,\"lastArrival\":%.6f},\"boundaries\":[",c.samples,c.nearSamples,c.minimumOffset,c.firstArrival,c.lastArrival);
    const auto boundaries=(std::min)(poseClockBoundaryCount,static_cast<std::uint64_t>(poseClockBoundaries.size()));
    for(std::uint64_t i=0;i<boundaries;++i){const auto& r=poseClockBoundaries[(poseClockBoundaryCount-boundaries+i)%poseClockBoundaries.size()];
        fprintf(f,"%s{\"kind\":%u,\"arrival\":%.6f,\"source\":%.6f,\"offsetAfter\":%.6f,\"session\":%llu,\"epoch\":%llu}",i?",":"",r.kind,r.arrival,r.source,r.offsetAfter,r.session,r.epoch);
    }
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
void bufferPresentation(const CoopPosePacket& p){
    const auto before=sourcePoseTimeline;const double arrival=presentationClock();
    if(!coopPresentationReady(p)){poseInterpolation.clear();sourcePoseTimeline.clear();if(before.session)recordPoseClock(p,arrival,before,0,1);return;}
    PresentationPose s{};const bool replaced=before.session!=p.session||before.epoch!=p.worldEpoch||before.map!=p.mapId;
    const auto rebasesBefore=sourcePoseTimeline.lateRebases;
    const bool accepted=sourcePoseTimeline.mapTime(p.sampleTimeUs,arrival,p.session,p.worldEpoch,p.mapId,s.time);
    if(replaced||sourcePoseTimeline.lateRebases!=rebasesBefore)poseInterpolation.clear(); // 1213d: rebase = time discontinuity
    if(!accepted){if(sourcePoseTimeline.session)recordPoseClock(p,arrival,before,s.time,4);
        else if(!before.calibration.samples||before.calibration.session!=p.session||before.calibration.epoch!=p.worldEpoch||before.calibration.map!=p.mapId)recordPoseClock(p,arrival,before,s.time,5);
    }else if(replaced||arrival-before.lastArrival>250.)recordPoseClock(p,arrival,before,s.time,replaced?2:3);
    if(!accepted)return;
    s.session=p.session;s.epoch=p.worldEpoch;s.map=p.mapId;s.sequence=p.sequence;
    s.bodyValid=p.bodyPoseValid!=0;s.motionValid=p.playerMotionValid!=0;s.stance=p.playerStance;s.motion=p.playerMotion;
    if(s.bodyValid)std::memcpy(s.body,p.bodyPose,sizeof(s.body));
    std::memcpy(s.position,p.position,12);std::memcpy(s.orientation,p.orientation,36);poseInterpolation.push(s);
}
struct PoseCadenceSample {double time;std::uint32_t sequence;float remote[3],target[3],native[3],render[3];};
PoseCadenceSample poseCadence[2048]{};unsigned poseCadenceCount=0;double poseCadenceUntil=0,poseCadenceStart=0;
std::uint64_t sent=0,received=0,rejected=0;
std::uint64_t receivedPlayerMotions[4]{};
ISteamNetworkingSockets* sockets=nullptr;
HSteamListenSocket listener=0;
HSteamNetConnection connection=0;
// 1203f: per-message-type send tally (first 4 payload bytes = packet magic),
// to attribute the reliable backlog that delays partner presentation (1204f:
// ~456 KB pending reliable on the host). Diagnostics only; sends unchanged.
struct CoopSendTally {std::uint32_t magic=0;std::uint64_t messages=0,reliableBytes=0,unreliableBytes=0,windowBytes=0,windowPeak=0;};
std::array<CoopSendTally,96> coopSendTally{};ULONGLONG coopSendWindowAt=0;
EResult coopSendMessage(HSteamNetConnection target,const void* data,std::uint32_t size,int flags,std::int64_t* number){
    std::uint32_t magic=0;if(data&&size>=4)std::memcpy(&magic,data,4);
    unsigned i=0;while(i<coopSendTally.size()&&coopSendTally[i].magic&&coopSendTally[i].magic!=magic)++i;
    if(i<coopSendTally.size()){auto& s=coopSendTally[i];s.magic=magic;++s.messages;s.windowBytes+=size;
        if(flags&k_nSteamNetworkingSend_Reliable)s.reliableBytes+=size;else s.unreliableBytes+=size;}
    return sockets->SendMessageToConnection(target,data,size,flags,number);
}
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
std::atomic<std::uint64_t> guardChildrenUpdated{0},guardChildrenRefused{0};
std::atomic<std::uint64_t> avatarChildrenUpdated{0},avatarChildrenRefused{0};
void updateAvatarChildren(void* frame){
    NativeChildFrames children{};
    if(!realReferenceFrameUpdate||!readNativeChildFrames(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),reinterpret_cast<std::uintptr_t>(frame),children)){
        ++avatarChildrenRefused;return;
    }
    for(unsigned i=0;i<children.count;++i){
        if(!networkFrameOwned.load(std::memory_order_acquire)||controlledFrame.load(std::memory_order_acquire)!=frame)return;
        realReferenceFrameUpdate(reinterpret_cast<void*>(children.children[i].frame));++avatarChildrenUpdated;
    }
}
void updateGuardChildren(void* frame){
    NativeChildFrames children{};
    if(!realReferenceFrameUpdate||!readNativeChildFrames(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),reinterpret_cast<std::uintptr_t>(frame),children)){
        ++guardChildrenRefused;return;
    }
    for(unsigned i=0;i<children.count;++i){
        if(guardControlledFrame.load(std::memory_order_acquire)!=frame||GetTickCount64()>=guardFrameUntil.load(std::memory_order_acquire))return;
        // Preserve the native child loop at its original worker scheduling
        // point. Only the parent root is replaced by the host pose.
        realReferenceFrameUpdate(reinterpret_cast<void*>(children.children[i].frame));++guardChildrenUpdated;
    }
}
#include "body_probe.h"
#include "arm_probe.h"
#include "arm_stream.h"
void applyGuardBody(void* component,void* actor);
void applyNpcAuthorityBody(void* component,void* actor);
void stopGuardBody();
void stopGuardEquipment(const char* reason);
void sendGuardBody(ULONGLONG tick);
void sendGuardWeapon(ULONGLONG tick);
void stopGuardWeapon(const char* reason);
#include "shadow_damage.h"
#include "damage_observer.h"
std::uintptr_t shadowActionParameter(std::uintptr_t anim);
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
#include "session_online.h"
#include "session_product.h"
void pumpConnections(ULONGLONG tick) {
    if(!sockets)return;
    serviceOnlineSession(tick);
    SteamNetworkingConfigValue_t options[3];
    options[0].SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,reinterpret_cast<void*>(&statusCallback));
    // Native per-connection budget: compact five-NPC bodies plus player/action
    // streams exceed the SDK's default256KiB/s. This changes no Steam DLL.
    options[1].SetInt32(k_ESteamNetworkingConfig_SendRateMin,512*1024);
    options[2].SetInt32(k_ESteamNetworkingConfig_SendRateMax,512*1024);
    SteamNetworkingIPAddr address;address.SetIPv4(0x7f000001,31426);
    // 1178: online sessions own transport setup (P2P via Steam lobby). 1193: an
    // installed product never opens the one-PC lab loopback; it waits for a
    // Steam session (F7 host, Steam Join/invite or +connect_lobby).
    if(onlineSession.active||productMode){}
    else if(localRole==0&&!listener&&tick>=nextConnect) {
        listener=sockets->CreateListenSocketIP(address,3,options);nextConnect=tick+2000;
        consoleOutput("COOP: native loopback listener %u\n",listener);
    }else if(localRole==1&&!connection&&tick>=nextConnect) {
        connection=sockets->ConnectByIPAddress(address,3,options);nextConnect=tick+2000;
        consoleOutput("COOP: native loopback connection attempt %u\n",connection);
    }
    // The game also dispatches Steam callbacks. This pumps the interface queue
    // during our own engine callback, including while native menus are open.
    sockets->RunCallbacks();
    SteamNetConnectionStatusChangedCallback_t pending[32]{};unsigned count=0;
    AcquireSRWLockExclusive(&eventLock);count=eventCount;std::memcpy(pending,events,count*sizeof(*events));eventCount=0;ReleaseSRWLockExclusive(&eventLock);
    for(unsigned i=0;i<count;++i) {
        const auto& e=pending[i];
        if(e.m_info.m_eState==k_ESteamNetworkingConnectionState_Connecting && onlineSession.active && onlineSession.host && onlineSession.listener && e.m_info.m_hListenSocket==onlineSession.listener) {
            // 1178: accept exactly one P2P peer (not ourselves) for the lobby.
            const auto peer=e.m_info.m_identityRemote.GetSteamID64();
            if(connection||!peer||peer==onlineSession.self){sockets->CloseConnection(e.m_hConn,0,"Co-op session accepts one partner",false);continue;}
            if(sockets->AcceptConnection(e.m_hConn)==k_EResultOK){connection=e.m_hConn;onlineSession.owner=onlineSession.self;onlineSession.result="partner_connecting";}
        }
        else if(e.m_info.m_eState==k_ESteamNetworkingConnectionState_Connecting && localRole==0 && e.m_info.m_hListenSocket==listener) {
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
            if(onlineSession.active&&!onlineSession.host){onlineSession.phase=OnlinePhase::Failed;onlineSession.result="p2p_connection_ended";}
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
// Bounded source-thread diagnostics: preserve the exact context of refused
// physical samples instead of inferring it from a slower external observer.
struct NativeMotionRefusal {
    std::uint64_t sampleUs=0,epoch=0;unsigned stance=0,stateStance=0,reason=0,count=0;
    bool defaultMode=false;std::array<unsigned,8> ids{};std::array<std::uintptr_t,8> tables{};
};
std::array<NativeMotionRefusal,32> nativeMotionRefusals{};
std::uint64_t nativeMotionRefusalCount=0;
void recordNativeMotionRefusal(unsigned reason,const NativePlayerStates& states,unsigned stance,std::uintptr_t base){
    auto& r=nativeMotionRefusals[nativeMotionRefusalCount++%nativeMotionRefusals.size()];r={};
    r.sampleUs=outgoing.sampleTimeUs;r.epoch=outgoing.worldEpoch;r.stance=stance;r.stateStance=states.player.current;r.reason=reason;
    r.defaultMode=states.defaultMode;r.count=unsigned(states.active.size());
    for(unsigned i=0;i<(std::min)(r.count,unsigned(r.ids.size()));++i){r.ids[i]=states.active[i].id;r.tables[i]=states.active[i].vtable-base;}
}
void writeNativeMotionRefusals(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.motion-refusals.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"readOnly\":true,\"count\":%llu,\"records\":[",nativeMotionRefusalCount);
    const auto count=(std::min)(nativeMotionRefusalCount,std::uint64_t(nativeMotionRefusals.size()));
    for(std::uint64_t i=0;i<count;++i){const auto& r=nativeMotionRefusals[(nativeMotionRefusalCount-count+i)%nativeMotionRefusals.size()];
        fprintf(f,"%s{\"sampleUs\":%llu,\"epoch\":%llu,\"stance\":%u,\"stateStance\":%u,\"reason\":%u,\"defaultMode\":%s,\"activeCount\":%u,\"active\":[",i?",":"",r.sampleUs,r.epoch,r.stance,r.stateStance,r.reason,r.defaultMode?"true":"false",r.count);
        for(unsigned j=0;j<(std::min)(r.count,unsigned(r.ids.size()));++j)fprintf(f,"%s{\"id\":%u,\"vtableRva\":\"%llx\"}",j?",":"",r.ids[j],std::uint64_t(r.tables[j]));
        fprintf(f,"]}");
    }fprintf(f,"]}\n");const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
void sampleLocalPlayer() {
    sampledBlinkValid=sampledBlinkStateKnown=sampledBlinkStatePresent=false;
    outgoing.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t game=0,player=0;int index=-2,alternate=0;
    outgoing.flags=0;outgoing.mapId=0;std::memset(outgoing.position,0,sizeof(outgoing.position));std::memset(outgoing.orientation,0,sizeof(outgoing.orientation));
    outgoing.gameLogicState=outgoing.pendingGameLogicState=0;
    outgoing.playerStanceValid=outgoing.playerStance=0;
    outgoing.playerMotionValid=outgoing.playerMotion=0;
    outgoing.bodyPoseValid=0;std::memset(outgoing.bodyPose,0,sizeof(outgoing.bodyPose));
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
           (id!=1||logicVtable==base+0x1c60478)&&(id!=4||logicVtable==base+0x1c61e68)&&
           memoryRead(manager+0x538,&pending,4)&&pending<17) {
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
        stopBodyProbe();mixamoResearch={};stopArmProbe("world_changed");
        observedGame=game;observedPlayer=player;observedMapId=map;observedReady=ready;++outgoing.worldEpoch;
        networkFrameOwned.store(false,std::memory_order_release);controlledFrame.store(nullptr);
        guardFrameUntil.store(0,std::memory_order_release);guardControlledFrame.store(nullptr,std::memory_order_release);
        retireShadowDamageIdentity();probeActor=probeGame=probeHandle=0;probeFollow=false;phaseFollowInvalidated=false;
    }
    outgoing.mapId=map;
    if(!ready||!memoryRead(base+0x39a6088,&index,4)||!memoryRead(base+0x25ae9b8,&alternate,4))return;
    if(index!=0&&(index!=-1||alternate!=0))return;
    if(!player)return;
    float transform[12]{};if(!memoryRead(player+0x300,transform,sizeof(transform)))return;
    outgoing.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);
    for(float f:transform)if(!std::isfinite(f))return;
    std::memcpy(outgoing.position,transform,12);std::memcpy(outgoing.orientation,transform+3,36);outgoing.flags=1;
    if(localRole==0&&outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState){
        NativePlayerStates blinkStates{};
        if(readNativePlayerStates(memoryRead,base,blinkStates)&&blinkStates.player.game==game&&blinkStates.player.player==player){
            sampledBlinkStateKnown=true;for(const auto& s:blinkStates.active)if(s.id==12)sampledBlinkStatePresent=true;
            sampledBlinkValid=readNativePlayerBlink(memoryRead,base,blinkStates,sampledNativeBlink);
            if(sampledBlinkValid)std::memcpy(outgoing.position,nativeBlinkNetworkPoint(sampledNativeBlink).data(),12);
            // Never publish an unverified eye-height/intermediate Blink root.
            // The receiver retains its last coherent sample on this short gap.
            else if(sampledBlinkStatePresent)outgoing.flags=0;
        }
    }
    NativePlayerStance stance{};
    if(outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState&&
       readNativePlayerStance(memoryRead,base,stance)&&stance.game==game&&stance.player==player) {
        NativePlayerStates states{};std::uint32_t motion=0,presentation=0;
        const bool stateValid=readNativePlayerStates(memoryRead,base,states);
        // Existing0/1 wire stays unchanged. Exact native automatic crouch and
        //its owned low-mantle scene additionally map to presentation crouch1.
        if(stance.current<=1){outgoing.playerStance=stance.current;outgoing.playerStanceValid=1;}
        else if(stateValid&&states.player.game==game&&states.player.player==player&&states.player.current==stance.current&&
                readNativePlayerPresentationStance(memoryRead,base,states,presentation)){
            outgoing.playerStance=presentation;outgoing.playerStanceValid=1;
        }
        // 1213r: motion is only valid on the wire with a wire stance (validCoopPose);
        // an unmapped physical stance must not invalidate the whole root packet.
        const unsigned refusal=!stateValid?1u:states.player.game!=game||states.player.player!=player?2u:
            states.player.current!=stance.current?3u:!nativePlayerMotion(states,base,motion)?4u:!outgoing.playerStanceValid?5u:0u;
        if(!refusal) {
            outgoing.playerMotionValid=1;outgoing.playerMotion=motion;
            sampleBodyPose();
        }else recordNativeMotionRefusal(refusal,states,stance.current,base);
    }
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
            if(coopSendMessage(connection,&request,sizeof(request),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK) {
                ++skipSent;consoleOutput("COOP: relayed native confirmed skip sequence=%llu scene=%llx\n",request.sequence,request.cinematicSceneId);
            }else ++skipDropped;
    }
}
// 1203e (product controller only): congestion gate for bulk traffic. The 1186
// soak measured the host send queue at p50 104 ms / p90 285 ms / max 900 ms
// (NPC bodies + world observation + partner streams over the 512 KB/s cap),
// so small partner pose/stream packets waited behind bulk and the client saw
// 1 s+ stale gaps (1204e). Bulk NPC body poses and world observations skip a
// cycle while Steam's queue estimate exceeds 40 ms; partner presentation,
// control and reliable session messages are never gated.
std::uint64_t bulkCongestionSkips=0;long long bulkQueueUsMax=0;
bool bulkTransportCongested(ULONGLONG tick){
    static ULONGLONG checkedAt=0;static bool congested=false;
    if(!(productMode||productAutoArm)||!sockets||!connection)return false;
    if(tick-checkedAt<50)return congested;checkedAt=tick;
    SteamNetConnectionRealTimeStatus_t status{};
    congested=sockets->GetConnectionRealTimeStatus(connection,&status,0,nullptr)==k_EResultOK&&status.m_usecQueueTime>40000;
    if(status.m_usecQueueTime<10000000000ll)bulkQueueUsMax=(std::max)(bulkQueueUsMax,static_cast<long long>(status.m_usecQueueTime));
    return congested;
}
#include "world_observer.h"
#include "native_ai_hold.h"
#include "avatar_ai.h"
#include "avatar_perception.h"
#include "shadow_action.h"
bool enemyMeleeArmed();
bool enemyMeleeHoldsHealth();
#include "guard_link.h"
#include "guard_body.h"
#include "guard_equipment.h"
bool avatarWeaponArmed();
#include "avatar_sword.h"
#include "avatar_weapon.h"
#include "shadow_stream.h"
#include "authoring_proof.h"
#include "movement_playback.h"
#include "movement_native_feet.h"
#include "shadow_equipment.h"
#include "possession_body_visibility.h"
#include "review_playback.h"
#include "guard_weapon.h"
#include "observer_isolation.h"
#include "npc_authority_hold.h"
#include "npc_authority_stream.h"
#include "door_sync.h"
#include "usable_sync.h"
bool movementRangedFixtureBusy();
bool movementGrenadeFixtureBusy();
bool movementPotionFixtureBusy();
bool movementMesmerizeFixtureBusy();
bool movementDominoFixtureBusy();
bool movementSwarmFixtureBusy();
bool movementPossessionFixtureBusy();
bool movementDoppelgangerFixtureBusy();
bool movementShadowWalkFixtureBusy();
#include "power_fixture.h"
#include "darkvision_fixture.h"
#include "windblast_fixture.h"
#include "bendtime_fixture.h"
#include "ranged_fixture.h"
#include "grenade_fixture.h"
#include "heart_fixture.h"
#include "trap_fixture.h"
#include "agility_fixture.h"
#include "potion_fixture.h"
#include "note_restore_fixture.h"
#include "coin_sync.h"
#include "coin_fixture.h"
#include "mesmerize_fixture.h"
#include "domino_fixture.h"
#include "swarm_fixture.h"
#include "possession_fixture.h"
#include "doppelganger_fixture.h"
#include "shadowwalk_fixture.h"
#include "carry_attachment_fixture.h"
#include "native_power_fx.h"
#include "shadowwalk_fx.h"
#include "power_fx_stream.h"
#include "mesmerize_world_stream.h"
#include "domino_world_stream.h"
#include "possession_fx_stream.h"
#include "swarm_world_stream.h"
#include "doppelganger_world_stream.h"
#include "swarm_spawn_fx_stream.h"
#include "swarm_trail_presentation.h"
#include "domino_mark_presentation.h"
#include "domino_link_presentation.h"
#include "grapple_fx_stream.h"
#include "windblast_fx_stream.h"
#include "partner_ai.h"
#include "native_sword_damage.h"
#include "native_enemy_damage.h"
#include "native_player_sword_damage.h"
#include "friendly_fire_probe.h"
#include "mesmerize_partner_filter.h"
#include "domino_partner_filter.h"
#include "possession_partner_filter.h"
#include "assassination_partner_filter.h"
#include "interaction_partner_filter.h"
#include "sword_link.h"
#include "enemy_outcome.h"
#include "enemy_melee.h"
#include "npc_combat.h"
#include "npc_knockout.h"
#include "npc_paired.h"
#include "npc_attack.h"
#include "observer_camera.h"
struct NativeSwitchEvent;
bool checkpointIntercept(unsigned kind,int slot,bool& continuation);
void checkpointContinuationReturned(unsigned char result);
bool checkpointOwnsLoading();
bool checkpointLoadingBoundary(const NativeSwitchEvent& event);
bool missionTransitionBoundary(const NativeSwitchEvent& event);
void observeLevelTransition(const NativeSwitchEvent& e);
void waitMissionTransition();
void checkpointObserveLoadingEntry(const NativeSwitchEvent& event);
bool captureCheckpointPresentation(bool& shadow,bool& exact);
bool restoreCheckpointPresentation(bool shadow,bool exact);
bool prepareCheckpointPresentation(bool shadow);
#include "checkpoint_trace.h"
#include "character_probe.h"
#include "loading_trace.h"
#include "loading_queue_trace.h"
#include "checkpoint_sync.h"
#include "activation_trace.h"
#include "trigger_mirror.h"
#include "mission_end_mirror.h"
#include "kiscule_action_mirror.h"
#include "gameover_probe.h"
void exchangePoses(ULONGLONG tick) {
    if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected)return;
    SteamNetworkingMessage_t* messages[16]{};
    const int count=sockets->ReceiveMessagesOnConnection(connection,messages,16);
    for(int i=0;i<count;++i) {
        std::uint32_t magic=0;if(messages[i]->m_cbSize>=4)std::memcpy(&magic,messages[i]->m_pData,4);
        if(magic==0x484e3244){
            if(messages[i]->m_cbSize==sizeof(CoopNpcAttack)){CoopNpcAttack p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveNpcAttack(p,tick);}else ++npcAttack.rejected;
            messages[i]->Release();continue;
        }
        if(magic==0x434e3244){
            if(messages[i]->m_cbSize==sizeof(CoopNpcCombat)){CoopNpcCombat p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveNpcCombat(p,tick);}else ++npcCombat.rejected;
            messages[i]->Release();continue;
        }
        if(magic==0x414e3244){
            NpcAuthorityHeader h{};const auto bytes=static_cast<unsigned>(messages[i]->m_cbSize);
            if(bytes>=sizeof(h))std::memcpy(&h,messages[i]->m_pData,sizeof(h));
            if(npcPoseKind(h.kind)&&bytes==npcPoseBytes(h.joints)&&bytes<=sizeof(NpcPoseWire)){
                CoopNpcPose p{};if(decodeNpcPose(messages[i]->m_pData,bytes,p))receiveNpcPose(p,tick);else ++npcDecodeRejected;
            }else if(!npcPoseKind(h.kind)&&bytes==sizeof(CoopNpcControl)){
                CoopNpcControl p{};std::memcpy(&p,messages[i]->m_pData,bytes);receiveNpcControl(p,tick);
            }else ++npcPacketsRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x53463244){
            if(messages[i]->m_cbSize==sizeof(CoopSwarmSpawnFxPacket)){CoopSwarmSpawnFxPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveSwarmSpawnFx(p,tick);}
            else ++swarmSpawnFxRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x43443244){
            if(messages[i]->m_cbSize==sizeof(CoopDoppelgangerWorldPacket)){CoopDoppelgangerWorldPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveDoppelgangerWorld(p,tick);}
            else ++doppelgangerWorldRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x53573244){
            CoopSwarmWorldPacket p{};
            if(decodeSwarmWorldPacket(messages[i]->m_pData,static_cast<std::size_t>(messages[i]->m_cbSize),p))receiveSwarmWorld(p,tick);
            else ++swarmWorldRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x50463244){
            if(messages[i]->m_cbSize==sizeof(CoopPossessionFxPacket)){CoopPossessionFxPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receivePossessionFx(p,tick);}
            else ++possessionFxRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x44573244){
            if(messages[i]->m_cbSize==sizeof(CoopDominoWorldPacket)){CoopDominoWorldPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveDominoWorld(p,tick);}
            else ++dominoWorldRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x4d573244){
            if(messages[i]->m_cbSize==sizeof(CoopMesmerizeWorldPacket)){CoopMesmerizeWorldPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveMesmerizeWorld(p,tick);}
            else ++mesmerizeWorldRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x46503244){
            if(messages[i]->m_cbSize==sizeof(CoopPowerFxPacket)){CoopPowerFxPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveNativePowerFx(p,tick);}
            else ++powerFxRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x57463244){
            if(messages[i]->m_cbSize==sizeof(CoopWindblastFxPacket)){CoopWindblastFxPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveNativeWindblastFx(p,tick);}
            else ++windblastFxRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x47503244){
            if(messages[i]->m_cbSize==sizeof(CoopGrappleFxPacket)){CoopGrappleFxPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveNativeGrappleFx(p,tick);}
            else ++grappleFxRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x48413244){
            if(messages[i]->m_cbSize==sizeof(CoopShadowActionPacket)){CoopShadowActionPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveShadowStream(p,tick);}
            else ++shadowStreamRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x57413244) {
            if(messages[i]->m_cbSize==sizeof(CoopAvatarWeaponPacket)){
                CoopAvatarWeaponPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveAvatarWeapon(p,tick);
            }else ++avatarWeaponRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x41433244) {
            if(messages[i]->m_cbSize==sizeof(CoopArmPacket)){
                CoopArmPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveArmStream(p,tick);
            }else ++armStreamRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x4d453244) {
            if(messages[i]->m_cbSize==sizeof(CoopEnemyMeleePacket)){
                CoopEnemyMeleePacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveEnemyMelee(p,tick);
            }else ++meleeRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x4f453244) {
            if(messages[i]->m_cbSize==sizeof(CoopEnemyOutcomePacket)){
                CoopEnemyOutcomePacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveEnemyOutcome(p,tick);
            }else ++enemyOutcomeRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x44433244) {
            if(messages[i]->m_cbSize==sizeof(CoopSwordPacket)){
                CoopSwordPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveSwordPacket(p,tick);
            }else ++swordRejected;
            messages[i]->Release();continue;
        }
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
        if(magic==0x45473244) {
            if(messages[i]->m_cbSize==sizeof(CoopGuardWeaponPacket)) {
                CoopGuardWeaponPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveGuardWeapon(p,tick);
            }else ++guardWeaponRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x41473244) {
            if(messages[i]->m_cbSize==sizeof(CoopGuardBodyPacket)) {
                CoopGuardBodyPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveGuardBody(p,tick);
            }else ++guardBodyRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x43433244) {
            if(messages[i]->m_cbSize==sizeof(CoopCoinPacket)){CoopCoinPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveCoinPacket(p,tick);}
            else ++coinSync.rejected;
            messages[i]->Release();continue;
        }
        if(magic==0x464D3244) {
            if(messages[i]->m_cbSize==sizeof(MissionFollowPacket)){MissionFollowPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveMissionFollow(p);}
            else ++missionFollow.refused;
            messages[i]->Release();continue;
        }
        if(magic==0x4D543244) {
            if(messages[i]->m_cbSize==sizeof(TriggerMirrorPacket)){TriggerMirrorPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveTriggerMirror(p);}
            else ++triggerRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x454D3244) {
            if(messages[i]->m_cbSize==sizeof(MissionEndPacket)){MissionEndPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveMissionEnd(p);}
            else ++missionEnd.refused;
            messages[i]->Release();continue;
        }
        if(magic==0x524D3244) {
            if(messages[i]->m_cbSize==sizeof(MapReadyPacket)){MapReadyPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveMapReady(p);}
            else ++transitionHold.refused;
            messages[i]->Release();continue;
        }
        if(magic==0x48433244) {
            if(messages[i]->m_cbSize==sizeof(ChaosSyncPacket)){ChaosSyncPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveChaosSync(p);}
            else ++chaosSync.refused;
            messages[i]->Release();continue;
        }
        if(magic==0x46473244) {
            if(messages[i]->m_cbSize==sizeof(GameflowMirrorPacket)){GameflowMirrorPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveGameflowMirror(p);}
            else ++gameflowMirror.refused;
            messages[i]->Release();continue;
        }
        if(magic==0x544C3244) {
            if(messages[i]->m_cbSize==sizeof(LevelTransitionPacket)){LevelTransitionPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveLevelTransition(p);}
            else ++levelMirror.refused;
            messages[i]->Release();continue;
        }
        if(magic==0x414B3244) {
            if(messages[i]->m_cbSize==sizeof(KisculeActionPacket)){KisculeActionPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveKisculeAction(p);}
            else{AcquireSRWLockExclusive(&kisculeLock);++kisculeMirror.refused;ReleaseSRWLockExclusive(&kisculeLock);}
            messages[i]->Release();continue;
        }
        if(magic==0x50413244) {
            if(messages[i]->m_cbSize==sizeof(AutosavePairPacket)){AutosavePairPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveAutosavePair(p);}
            else ++autosavePair.rejected;
            messages[i]->Release();continue;
        }
        if(magic==0x4C433244) {
            if(messages[i]->m_cbSize==sizeof(CoopCoinCatalogPacket)){static CoopCoinCatalogPacket p;std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveCoinCatalog(p,tick);}
            else ++coinSync.rejected;
            messages[i]->Release();continue;
        }
        if(magic==0x53553244) {
            if(messages[i]->m_cbSize==sizeof(CoopUsablePacket)){CoopUsablePacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveUsablePacket(p,tick);}
            else ++usableCatalogRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x4f443244) {
            if(messages[i]->m_cbSize==sizeof(CoopDoorPacket)){CoopDoorPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveDoorPacket(p,tick);}
            else ++doorCatalogRejected;
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
            remote=packet;++received;lastReceive=tick;bufferPresentation(packet);
            if(packet.playerMotionValid)++receivedPlayerMotions[packet.playerMotion];
        }else ++rejected;
        messages[i]->Release();
    }
    if(tick-lastSend>=33) {
        lastSend=tick;++outgoing.sequence;if(!outgoing.sequence)++outgoing.sequence;
        if(coopSendMessage(connection,&outgoing,sizeof(outgoing),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK)++sent;
    }
}
constexpr char kEmilyPartnerDefinition[]="models/characters/def/d2coop/emily_partner.def";
std::uintptr_t lookupEntityDefinition(const char* name) {
    if(GetCurrentThreadId()!=engineThread.load()||!coopGameplayReady(outgoing))return 0;
    static const unsigned char typeBytes[]={0x48,0x89,0x6c,0x24,0x18,0x56,0x48,0x83,0xec,0x20,0x48,0x8b,0xea,0x48,0x8b,0xf1};
    static const unsigned char findBytes[]={0x48,0x85,0xd2,0x75,0x03,0x33,0xc0,0xc3,0x45,0x33,0xc9,0xe9,0x60,0xa3,0x01,0x00};
    if(std::memcmp(gameImage+0x1ea30,typeBytes,sizeof(typeBytes))||std::memcmp(gameImage+0x3cc0,findBytes,sizeof(findBytes)))return 0;
    // Recovered from native touchDecl. Lookup only; do not invoke its reparse.
    auto type=reinterpret_cast<std::uintptr_t(*)(void*,const char*)>(gameImage+0x1ea30)(gameImage+0x24367e0,"entityDef");
    std::uintptr_t typeName=0;char text[192]{};
    if(!type||!memoryRead(type+0x78,&typeName,8)||!nativeName(typeName,text,sizeof(text))||std::strcmp(text,"entityDef"))return 0;
    auto def=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,const char*,bool)>(gameImage+0x3cc0)(type,name,false);
    std::uintptr_t vt=0,declName=0;
    if(!def||!memoryRead(def,&vt,8)||vt!=reinterpret_cast<std::uintptr_t>(gameImage)+0x1ab58f8||
       !memoryRead(def+8,&declName,8)||!nativeName(declName,text,sizeof(text))||std::strcmp(text,name))return 0;
    return def;
}
void probePartnerDefinitions() {
    const auto known=lookupEntityDefinition("models/characters/def/story/emily_kaldwin.def");
    const auto missing=lookupEntityDefinition("models/characters/def/d2coop/absent_probe.def");
    const auto partner=lookupEntityDefinition(kEmilyPartnerDefinition);
    consoleOutput("COOP: declaration lookup known=%p missing=%p partner=%p enabled=%u\n",reinterpret_cast<void*>(known),reinterpret_cast<void*>(missing),reinterpret_cast<void*>(partner),partnerAssetsEnabled?1:0);
    // Read-only, fixed native fixture discovery. Do not load/reparse declarations
    // or call the fatal native create path before its spawnable flag is known.
    // Exact executable: 1405be480 returns declaration+58; 1408a78f0 tests +98
    // on that embedded type, hence declaration+f0 bit20.
    const auto chain=lookupEntityDefinition("models/environment/gameplay/climbable/chain_01/usable_sources/chain_01.def");
    unsigned char spawnFlags=0;
    const bool typeRead=chain&&memoryRead(chain+0xf0,&spawnFlags,1);
    char path[MAX_PATH]{},tmp[MAX_PATH]{};
    std::snprintf(path,sizeof(path),"%s.climb-definition.json",statusPath);
    std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;
    if(!fopen_s(&f,tmp,"wb")&&f){
        fprintf(f,"{\"readOnly\":true,\"loaded\":%s,\"declaration\":\"0x%llx\",\"typeRead\":%s,\"spawnable\":%s,\"flags\":%u,\"worldEpoch\":%llu}\n",chain?"true":"false",static_cast<unsigned long long>(chain),typeRead?"true":"false",typeRead&&(spawnFlags&0x20)?"true":"false",spawnFlags,outgoing.worldEpoch);
        const bool ok=!ferror(f);const int closed=fclose(f);
        if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
    }
    consoleOutput("COOP: native climb definition=%p typeRead=%u spawnable=%u\n",reinterpret_cast<void*>(chain),typeRead?1:0,typeRead&&(spawnFlags&0x20)?1:0);
}

void probeShadowDamage(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeHealth h{};NativeSwordEquipment s{};
    if(GetCurrentThreadId()!=engineThread.load()||!swordDamageApiMatches()||!avatarAiHeld||
       !readNativeHealth(memoryRead,base,probeHandle,h)||h.game!=observedGame||h.actor!=probeActor||h.logic!=1||h.current!=40||
       !readNativeSwordEquipment(memoryRead,base,probeHandle,s,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
       !shadowOwnedDamageSource(s.weaponHandle)||shadowOwnedDamageSource(1))return;
    const auto d=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,kSwordDamageDecl,false,false);
    if(!nativeSwordDeclaration(d))return;
    const auto before=shadowDamageBlocked.load();float inflicted=0;
    for(unsigned n=0;n<2;++n){
        alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
        reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(d));
        reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
        const std::uint32_t attacker=n?0:probeHandle,inflictor=n?s.weaponHandle:0;
        std::memcpy(params.data()+0x50,&inflictor,4);std::memcpy(params.data()+0x54,&attacker,4);
        reinterpret_cast<NativeDamage>(gameImage+0x717950)(reinterpret_cast<void*>(h.component),params.data(),result.data());
        float applied=0;std::memcpy(&applied,result.data(),4);inflicted+=applied;
    }
    NativeHealth after{};const bool stable=readNativeHealth(memoryRead,base,probeHandle,after)&&after.component==h.component&&after.current==h.current;
    consoleOutput("COOP: shadow damage firewall actor+weapon blocked=%llu stable=%u inflicted=%.3f playerExempt=%u\n",shadowDamageBlocked.load()-before,stable?1:0,inflicted,shadowOwnedDamageSource(1)?0:1);
}

bool enableProbeFollow(bool exact);
bool spawnProbe(const char* character) {
    if(!coopGameplayReady(outgoing)||!observedGame||probeActor)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vtable=0,handles=0;std::uint32_t flags=0;
    if(!memoryRead(observedGame,&vtable,8)||vtable!=base+0x1c940a8||
       !memoryRead(observedGame+0x601c0,&handles,8)||!handles||
       !memoryRead(observedGame+0x601c8,&flags,4)||(flags&0xffffff)>16384)return false;
    const bool doppelganger=!std::strcmp(character,"shadow_emily")||!std::strcmp(character,"shadow_corvo");
    if(doppelganger&&!partnerAssetsEnabled)return false;
    const bool emily=!std::strcmp(character,"emily")||!std::strcmp(character,"shadow_emily");
    if(!emily&&std::strcmp(character,"corvo")&&std::strcmp(character,"shadow_corvo"))return false;
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
    if(emily&&partnerAssetsEnabled) {
        definition=lookupEntityDefinition(kEmilyPartnerDefinition);
        consoleOutput("COOP: dedicated Emily partner definition %p\n",reinterpret_cast<void*>(definition));
    }
    if(doppelganger)definition=lookupEntityDefinition(emily?"models/characters/def/d2coop/doppelganger_emily.def":"models/characters/def/d2coop/doppelganger_corvo.def");
    if(!definition||(!doppelganger&&!spawnerComponent)){consoleOutput("COOP: probe refused; required story definition or presentation source is not loaded\n");return false;}
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
    if(!doppelganger)reinterpret_cast<void(*)(void*,void*)>(gameImage+0x759e70)(reinterpret_cast<void*>(spawner),actor);
    reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);
    if(!doppelganger)reinterpret_cast<void(*)(void*,void*)>(gameImage+0x759a50)(reinterpret_cast<void*>(spawnerComponent),actor);
    probeActor=reinterpret_cast<std::uintptr_t>(actor);probeGame=observedGame;probeDoppelganger=doppelganger;
    memoryRead(probeActor+0xa8,&probeHandle,4);
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0x87b780)(actor,emily?"d2coop_probe_emily":"d2coop_probe_corvo");
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(actor);
    // Presentation initialization hides the render entity without setting the
    // explicit hidden flag. The native show/update method restores it directly.
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(actor);
    consoleOutput("COOP: independent probe handle %08x at %p\n",probeHandle,actor);
    if(doppelganger){
        retireShadowDamageIdentity();shadowDamageGame.store(probeGame);shadowDamageHandle.store(probeHandle);shadowDamageActor.store(probeActor,std::memory_order_release);
        // No story spawner overrides: retain the native clone model, tree and inventory.
        // Lifecycle, reflex, attack, psychic sensor and crew components are absent.
        const bool held=enableProbeFollow(true);
        consoleOutput("COOP: Doppelganger presentation immediate follow=%u\n",held?1:0);
    }
    return true;
}
#include "ranged_prop.h"
#include "climb_fixture.h"
#include "door_fixture.h"
#include "usable_fixture.h"
#include "grenade_prop.h"
#include "heart_prop.h"
#include "trap_prop.h"
#include "grenade_fx.h"
#include "mesmerize_visual_fixture.h"
#include "swarm_rat_visual_fixture.h"
#include "doppelganger_visual.h"
#include "ranged_muzzle.h"
#include "power_failure_fx.h"
bool enableProbeFollow(bool exact) {
    if(!probeActor||probeGame!=observedGame||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
       outgoing.mapId!=remote.mapId||!lastReceive||GetTickCount64()-lastReceive>=1000)return false;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const auto slot=static_cast<std::int32_t>(probeHandle)>>16;
    std::uintptr_t registered=0,frame=0,owner=0,vt=0;unsigned actual=0;float position[3]{};
    if(slot<=0||slot>=16384||!memoryRead(observedGame+0x3130b0+slot*8,&registered,8)||registered!=probeActor||
       !memoryRead(probeActor+0xa8,&actual,4)||actual!=probeHandle||!memoryRead(probeActor+0x300,position,sizeof(position))||
       !memoryRead(probeActor+0x358,&frame,8)||!frame||!memoryRead(frame,&vt,8)||vt!=base+0x1c573e8||
       !memoryRead(frame+0x10,&owner,8)||owner!=probeActor)return false;
    for(float p:position)if(!std::isfinite(p))return false;
    if(!acquireAvatarAi())return false;
    for(int i=0;i<3;++i)probeOffset[i]=exact?0.f:position[i]-remote.position[i];
    // Re-arming follow does not receive a new root packet. Rebasing the source
    // clock here against cached remote data adds its age to the offset, making
    // later live action samples appear in the future. Preserve the established
    // clock and refill the render buffer only on actual packet receipt.
    probeFollowExact=exact;poseInterpolation.clear();
    controlledFrame.store(reinterpret_cast<void*>(frame),std::memory_order_relaxed);
    networkFrameOwned.store(true,std::memory_order_release);
    probeFollow=true;phaseFollowInvalidated=false;probeRemoteEpoch=remote.worldEpoch;probeRemoteSession=remote.session;
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(probeActor));
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(probeActor));
    consoleOutput("COOP: native entity transform follow enabled; animation and interactions are not replicated\n");
    return true;
}
#include "movement_resume.h"
// 1203: product partner-presentation state for the session controller watchdog.
bool productPresentationState(ULONGLONG tick,bool& resuming,unsigned& missing){
    resuming=movementResume.policy.enabled()&&movementResume.policy.phase!=MovementResumePolicy::Running;
    // Bit mask of what is missing: 1 avatar, 2 follow, 4 bank (incl. loading), 8 stream.
    missing=0;
    if(!probeActor||probeGame!=observedGame)missing|=1;
    if(!probeFollow)missing|=2;
    if(!movementBankActive())missing|=movementLoad.state.load(std::memory_order_acquire)?0u:4u;
    if(!shadowStreamCurrent(tick))missing|=8;
    return !missing&&movementBankActive();
}
bool productPeerStreamWaiting(ULONGLONG tick){return shadowPeerHelloAt&&tick-shadowPeerHelloAt<1000&&!shadowStreamCurrent(tick);}
bool captureCheckpointPresentation(bool& shadow,bool& exact) {
    if(!probeActor||!probeFollow||probeGame!=observedGame||
       worldIdentityEntity(memoryRead,observedGame,probeHandle)!=probeActor||
       (probeDoppelganger&&(!partnerAssetsEnabled||!shadowOwnedDamageSource(probeHandle))))return false;
    shadow=probeDoppelganger;exact=probeFollowExact;return true;
}
bool prepareCheckpointPresentation(bool shadow) {
    // 1153b: any shared map (mission checkpoints restore the partner too).
    if(GetCurrentThreadId()!=engineThread.load()||
       !coopGameplayReady(outgoing)||!coopGameplayReady(remote)||outgoing.mapId!=remote.mapId)return false;
    // Restore the explicitly captured presentation, never silently substitute
    // a story body when a clone definition or its asset pack is unavailable.
    if(shadow&&!partnerAssetsEnabled)return false;
    if(!probeActor&&!spawnProbe(shadow?(localRole?"shadow_corvo":"shadow_emily"):(localRole?"corvo":"emily")))return false;
    return probeDoppelganger==shadow;
}
bool restoreCheckpointPresentation(bool shadow,bool exact){
    if(probeDoppelganger!=shadow)return false;
    if(shadow){NativeSwordEquipment sword{};
        if(!shadowOwnedDamageSource(probeHandle)||!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,
            localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
            sword.actor!=probeActor||!sword.equipped||!sword.started)return false;
    }
    return enableProbeFollow(exact);
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
        if(coopSendMessage(connection,&outgoing,sizeof(outgoing),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK) {
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
        if(coopSendMessage(connection,&release,sizeof(release),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK) {
            ++barrierSent;finishCinematicHold(tick,true);consoleOutput("COOP: opening rendezvous released through Steam sequence=%llu\n",release.sequence);
        }
    }
}
bool allowedCommand(const char* text) {
    if(!std::strcmp(text,"coop_sword_link 15000")||!std::strcmp(text,"coop_sword_link 60000")||!std::strcmp(text,"coop_sword_link off")||!std::strcmp(text,"coop_sword_duplicate"))return true;
    if(!std::strcmp(text,"coop_damage_sample sword")||!std::strcmp(text,"coop_damage_sample sword_back"))return true;
    if(!std::strcmp(text,"coop_enemy_damage assassination"))return true;
    if(!std::strcmp(text,"coop_enemy_outcome 60000")||!std::strcmp(text,"coop_enemy_outcome off")||!std::strcmp(text,"coop_enemy_duplicate"))return true;
    if(!std::strcmp(text,"coop_enemy_melee 15000")||!std::strcmp(text,"coop_enemy_melee off")||!std::strcmp(text,"coop_enemy_melee_duplicate"))return true;
    if(!std::strcmp(text,"coop_partner_enemy 8000")||!std::strcmp(text,"coop_partner_enemy escape 8000")||!std::strcmp(text,"coop_partner_enemy escape 60000")||!std::strcmp(text,"coop_partner_enemy off"))return true;
    if(!std::strcmp(text,"coop_guard_health 35"))return true;
    if(!std::strcmp(text,"coop_damage_observe 60000")||!std::strcmp(text,"coop_damage_observe off"))return true;
    if(!std::strcmp(text,"coop_gameover_probe")||!std::strcmp(text,"coop_gameover_retry"))return true;
    if(!std::strcmp(text,"coop_product_auto on")||!std::strcmp(text,"coop_product_auto off")||!std::strcmp(text,"coop_follow_host on")||!std::strcmp(text,"coop_follow_host off"))return true;
    if(!std::strcmp(text,"coop_session host")||!std::strcmp(text,"coop_session host_quiet")||!std::strcmp(text,"coop_session off")||!std::strncmp(text,"coop_session join ",18))return true;
    if(!std::strcmp(text,"coop_autosave_probe")||!std::strcmp(text,"coop_trigger_mirror on")||!std::strcmp(text,"coop_trigger_mirror off"))return true;
    if(!std::strcmp(text,"coop_mission_end_mirror on")||!std::strcmp(text,"coop_mission_end_mirror off"))return true;
    if(!std::strcmp(text,"coop_gameflow_mirror on")||!std::strcmp(text,"coop_gameflow_mirror off"))return true;
    if(!std::strcmp(text,"coop_kiscule_mirror on")||!std::strcmp(text,"coop_kiscule_mirror off"))return true;
    if(!std::strncmp(text,"coop_chaos_points ",18)&&std::strlen(text)<=24&&std::strspn(text+18,"0123456789")==std::strlen(text+18)&&text[18])return true;
    if(!std::strcmp(text,"coop_checkpoint_sync on")||!std::strcmp(text,"coop_checkpoint_sync off")||!std::strcmp(text,"coop_checkpoint_sync retry_probe")||
       !std::strcmp(text,"coop_checkpoint_delay 0")||!std::strcmp(text,"coop_checkpoint_delay 1500")||
       !std::strcmp(text,"coop_checkpoint_delay 10000")||!std::strcmp(text,"RestartFromCheckpoint")||!std::strcmp(text,"RestartFromMemoryCheckpoint"))return true;
    if(!std::strcmp(text,"coop_loading_probe 2000")||!std::strcmp(text,"coop_loading_probe emily"))return true;
    if(!std::strcmp(text,"coop_character_probe emily")||!std::strcmp(text,"coop_character_probe corvo"))return true;
    if(!std::strcmp(text,"coop_guard_link 8000")||!std::strcmp(text,"coop_guard_link off")||!std::strcmp(text,"coop_guard_nudge 1000")||!std::strcmp(text,"coop_guard_stall 1000"))return true;
    if(!std::strcmp(text,"coop_guard_link escape 60000"))return true;
    if(!std::strcmp(text,"coop_npc_hold 12000")||!std::strcmp(text,"coop_npc_hold off"))return true;
    if(!std::strcmp(text,"coop_npc_combat fixture40000")||!std::strcmp(text,"coop_npc_combat fixture_off"))return true;
    if(!std::strcmp(text,"coop_npc_combat ordinary40000"))return true;
    if(!std::strcmp(text,"coop_npc_combat fixture_ai40000"))return true;
    if(!std::strcmp(text,"coop_npc_combat trigger_fixture_once"))return true;
    if(!std::strcmp(text,"coop_npc_combat knockout_test")||!std::strcmp(text,"coop_npc_combat knockout_reaction_test")||!std::strcmp(text,"coop_npc_combat knockout_generic_test"))return true;
    if(!std::strcmp(text,"coop_npc_combat hostworld_test")||!std::strcmp(text,"coop_npc_combat hostnative_test"))return true;
    if(!std::strcmp(text,"coop_npc_combat on")||!std::strcmp(text,"coop_npc_combat off")||!std::strcmp(text,"coop_npc_combat duplicate"))return true;
    if(!std::strcmp(text,"coop_npc_stream on")||!std::strcmp(text,"coop_npc_stream off"))return true;
    if(!std::strcmp(text,"coop_guard_body on")||!std::strcmp(text,"coop_guard_body off"))return true;
    if(!std::strcmp(text,"coop_guard_equipment 2000")||!std::strcmp(text,"coop_guard_equipment off"))return true;
    if(!std::strcmp(text,"coop_guard_weapon on")||!std::strcmp(text,"coop_guard_weapon off"))return true;
    if(!std::strcmp(text,"coop_observer_isolation 30000")||!std::strcmp(text,"coop_observer_isolation off"))return true;
    if(!std::strcmp(text,"coop_movement_fixture 60000")||!std::strcmp(text,"coop_movement_fixture off"))return true;
    if(!std::strcmp(text,"coop_observer_camera hold")||!std::strcmp(text,"coop_observer_camera off"))return true;
    if(!std::strcmp(text,"coop_power_fixture 20000")||!std::strcmp(text,"coop_power_fixture off"))return true;
    if(!std::strcmp(text,"coop_darkvision_fixture 20000")||!std::strcmp(text,"coop_darkvision_fixture off"))return true;
    if(!std::strcmp(text,"coop_windblast_fixture 20000")||!std::strcmp(text,"coop_windblast_fixture off"))return true;
    if(!std::strcmp(text,"coop_mesmerize_fixture 20000")||!std::strcmp(text,"coop_mesmerize_fixture 40000")||!std::strcmp(text,"coop_mesmerize_fixture off"))return true;
    if(!std::strcmp(text,"coop_doppelganger_fixture 20000")||!std::strcmp(text,"coop_doppelganger_fixture 40000")||!std::strcmp(text,"coop_doppelganger_fixture off"))return true;
    if(!std::strcmp(text,"coop_shadowwalk_fixture 20000")||!std::strcmp(text,"coop_shadowwalk_fixture 40000")||!std::strcmp(text,"coop_shadowwalk_fixture off"))return true;
    if(!std::strcmp(text,"coop_domino_fixture 20000")||!std::strcmp(text,"coop_domino_fixture 40000")||!std::strcmp(text,"coop_domino_fixture off"))return true;
    if(!std::strcmp(text,"coop_possession_fixture human40000")||!std::strcmp(text,"coop_possession_fixture 20000")||!std::strcmp(text,"coop_possession_fixture 40000")||!std::strcmp(text,"coop_possession_fixture off"))return true;
    if(!std::strcmp(text,"coop_swarm_fixture 20000")||!std::strcmp(text,"coop_swarm_fixture 40000")||!std::strcmp(text,"coop_swarm_fixture off"))return true;
    if(!std::strcmp(text,"coop_bendtime_fixture 20000")||!std::strcmp(text,"coop_bendtime_fixture off"))return true;
    if(!std::strcmp(text,"coop_ranged_fixture gun")||!std::strcmp(text,"coop_ranged_fixture crossbow")||!std::strcmp(text,"coop_ranged_fixture off"))return true;
    if(!std::strcmp(text,"coop_ranged_fixture gun_loaded")||!std::strcmp(text,"coop_ranged_fixture crossbow_loaded"))return true;
    // Explicit isolated research only; native console setter, no default change.
    // Harness records the live value and restores it before load and on failure.
    if(!std::strcmp(text,"ark_player_autoaim_model_pad 0")||!std::strcmp(text,"ark_player_autoaim_model_pad 1")||!std::strcmp(text,"ark_player_autoaim_model_pad 2"))return true;
    if(!std::strcmp(text,"coop_grenade_fixture 12000")||!std::strcmp(text,"coop_grenade_fixture off"))return true;
    if(!std::strcmp(text,"coop_heart_fixture 20000")||!std::strcmp(text,"coop_heart_fixture off"))return true;
    if(!std::strcmp(text,"coop_trap_fixture stun")||!std::strcmp(text,"coop_trap_fixture razor")||!std::strcmp(text,"coop_trap_fixture off"))return true;
    if(!std::strcmp(text,"coop_coin_fixture collect"))return true;
    if(!std::strcmp(text,"coop_coin_sync on")||!std::strcmp(text,"coop_coin_sync off")||!std::strcmp(text,"coop_coin_sync duplicate")||!std::strcmp(text,"coop_coin_sync stale"))return true;
    if(!std::strncmp(text,"coop_coin_sync exercise ",24))return coinCanonicalName(std::string(text+24));
    if(!std::strcmp(text,"coop_coin_sync capacity off"))return true;
    if(!std::strncmp(text,"coop_coin_sync capacity ",24)){std::string name;int quantity=-1;return parseCoinCapacityCommand(text,name,quantity);}
    if(!std::strcmp(text,"coop_note_fixture restore_absent")||!std::strcmp(text,"coop_note_fixture restore_present"))return true;
    if(!std::strcmp(text,"coop_potion_fixture health")||!std::strcmp(text,"coop_potion_fixture mana")||!std::strcmp(text,"coop_potion_fixture off")||!std::strcmp(text,"coop_potion_fixture use_health")||!std::strcmp(text,"coop_potion_fixture use_mana"))return true;
    if(!std::strcmp(text,"coop_ranged_prop gun")||!std::strcmp(text,"coop_ranged_prop crossbow")||!std::strcmp(text,"coop_ranged_prop off"))return true;
    if(!std::strcmp(text,"coop_mesmerize_visual duplicate"))return true;
    if(!std::strcmp(text,"coop_mesmerize_visual in")||!std::strcmp(text,"coop_mesmerize_visual loop")||!std::strcmp(text,"coop_mesmerize_visual out")||!std::strcmp(text,"coop_mesmerize_visual off"))return true;
    if(!std::strcmp(text,"coop_grenade_prop 15000")||!std::strcmp(text,"coop_grenade_prop off"))return true;
    if(!std::strcmp(text,"coop_agility_fixture 30000")||!std::strcmp(text,"coop_agility_fixture off"))return true;
    if(!std::strcmp(text,"coop_carry_attachment 8000")||!std::strcmp(text,"coop_carry_attachment 16000")||!std::strcmp(text,"coop_carry_attachment snapshot8000")||!std::strcmp(text,"coop_carry_attachment pickup8000")||!std::strcmp(text,"coop_carry_attachment off"))return true;
    if(!std::strcmp(text,"coop_power_fx blink_start")||!std::strcmp(text,"coop_power_fx blink_out")||!std::strcmp(text,"coop_power_fx off"))return true;
    if(!std::strcmp(text,"coop_power_fx duplicate"))return true;
    if(!std::strcmp(text,"coop_power_fx grapple_beam")||!std::strcmp(text,"coop_power_fx grapple_impact"))return true;
    if(!std::strcmp(text,"coop_power_fx grapple_duplicate"))return true;
    if(!std::strcmp(text,"coop_power_fx windblast_probe")||!std::strcmp(text,"coop_power_fx windblast_ground_probe"))return true;
    if(!std::strcmp(text,"coop_power_fx windblast_remote_probe"))return true;
    if(!std::strcmp(text,"coop_power_fx windblast_duplicate"))return true;
    if(!std::strcmp(text,"coop_swarm_fx duplicate"))return true;
    if(!std::strcmp(text,"coop_power_fx bendtime_viewer_probe"))return true;
    if(!std::strcmp(text,"coop_power_fx outofmana_hand_probe"))return true;
    if(!std::strcmp(text,"coop_source_isolation 30000")||!std::strcmp(text,"coop_source_isolation off"))return true;
    if(!std::strcmp(text,"coop_world_hold_ai 2000")||!std::strcmp(text,"coop_world_hold_ai off"))return true;
    if(!std::strcmp(text,"coop_world_observe on")||!std::strcmp(text,"coop_world_observe off"))return true;
    if(!std::strcmp(text,"coop_probe_locomotion on")||!std::strcmp(text,"coop_probe_locomotion off"))return true;
    if(!std::strcmp(text,"coop_body_probe 2000")||!std::strcmp(text,"coop_body_probe off"))return true;
    if(!std::strcmp(text,"coop_arm_probe 2000")||!std::strcmp(text,"coop_arm_probe off"))return true;
    if(!std::strcmp(text,"coop_probe_arm_space on")||!std::strcmp(text,"coop_probe_arm_space off"))return true;
    if(!std::strcmp(text,"coop_probe_arm on")||!std::strcmp(text,"coop_probe_arm off")||!std::strcmp(text,"coop_arm_duplicate"))return true;
    if(!std::strcmp(text,"coop_shadow_equipment 2500")||!std::strcmp(text,"coop_shadow_equipment off"))return true;
    if(!std::strcmp(text,"coop_avatar_sword 2000")||!std::strcmp(text,"coop_avatar_sword off"))return true;
    if(!std::strcmp(text,"coop_shadow_unbound_probe 6000")||!std::strcmp(text,"coop_shadow_unbound_probe off"))return true;
    if(!std::strcmp(text,"coop_shadow_stream 15000")||!std::strcmp(text,"coop_shadow_stream on")||!std::strcmp(text,"coop_shadow_stream off")||!std::strcmp(text,"coop_shadow_stream_duplicate"))return true;
    if(!std::strcmp(text,"coop_avatar_weapon 15000")||!std::strcmp(text,"coop_avatar_weapon off")||!std::strcmp(text,"coop_avatar_weapon_duplicate"))return true;
    if(!std::strcmp(text,"coop_probe_body on")||!std::strcmp(text,"coop_probe_body off"))return true;
    if(!std::strcmp(text,"coop_shadow_crouch capture")||!std::strcmp(text,"coop_shadow_crouch off")||!std::strcmp(text,"coop_shadow_crouch upright")||!std::strcmp(text,"coop_shadow_crouch native"))return true;
    {char pose[24]{};unsigned milliseconds=0;if(parseShadowPose(text,pose,milliseconds))return true;}
    if(!std::strcmp(text,"coop_cinematic_barrier on")||!std::strcmp(text,"coop_cinematic_barrier off"))return true;
    if(!std::strcmp(text,"coop_cinematic_hold 2000"))return true;
    if(!std::strcmp(text,"coop_cinematic_sync on")||!std::strcmp(text,"coop_cinematic_sync off"))return true;
    if(!std::strcmp(text,"coop_probe_trace_transform"))return true;
    if(!std::strcmp(text,"coop_probe_trace_pose"))return true;
    if(!std::strcmp(text,"coop_shadow_damage_probe"))return true;
    if(!std::strcmp(text,"coop_friendly_fire_probe"))return true;
    if(!std::strcmp(text,"coop_mixamo_crouch 30000")||!std::strcmp(text,"coop_mixamo_crouch on")||!std::strcmp(text,"coop_mixamo_crouch off"))return true;
    if(!std::strcmp(text,"coop_probe_smoothing on")||!std::strcmp(text,"coop_probe_smoothing off")||!std::strcmp(text,"coop_pose_trace 3000"))return true;
    if(!std::strcmp(text,"coop_shadow_action crouchpose")||!std::strcmp(text,"coop_shadow_action crouchref")||!std::strcmp(text,"coop_shadow_action airborne")||!std::strcmp(text,"coop_shadow_action land")||!std::strcmp(text,"coop_shadow_action attack")||!std::strcmp(text,"coop_shadow_action parry")||!std::strcmp(text,"coop_shadow_action recover")||!std::strcmp(text,"coop_shadow_action idle")||!std::strcmp(text,"coop_shadow_action off"))return true;
    if(!std::strcmp(text,"coop_shadow_loco sword")||!std::strcmp(text,"coop_shadow_loco combat")||!std::strcmp(text,"coop_shadow_loco off"))return true;
    if(!std::strcmp(text,"coop_authoring_proof play")||!std::strcmp(text,"coop_authoring_proof hold_ready")||!std::strcmp(text,"coop_authoring_proof hold_guard")||!std::strcmp(text,"coop_authoring_proof off"))return true;
    if(!std::strcmp(text,"coop_review_clip play")||!std::strcmp(text,"coop_review_clip off"))return true;
    if(!std::strncmp(text,"coop_review_clip hold ",22)){char* end=nullptr;const auto frame=std::strtol(text+22,&end,10);return end!=text+22&&!*end&&frame>=0&&frame<=1800;}
    if(!std::strcmp(text,"coop_movement_bank 60000")||!std::strcmp(text,"coop_movement_bank on")||!std::strcmp(text,"coop_movement_bank off"))return true;
    if(!std::strcmp(text,"coop_movement_resume on")||!std::strcmp(text,"coop_movement_resume off"))return true;
    if(!std::strcmp(text,"coop_movement_feet on")||!std::strcmp(text,"coop_movement_feet off")||!std::strcmp(text,"coop_movement_feet authored"))return true;
    if(!std::strcmp(text,"coop_decl_probe"))return true;
    if(!std::strcmp(text,"coop_door_sync on")||!std::strcmp(text,"coop_door_sync session")||!std::strcmp(text,"coop_door_sync off")||!std::strcmp(text,"coop_door_sync duplicate"))return true;
    if(!std::strcmp(text,"coop_usable_sync on")||!std::strcmp(text,"coop_usable_sync session")||!std::strcmp(text,"coop_usable_sync off")||!std::strcmp(text,"coop_usable_sync duplicate"))return true;
    if(!std::strcmp(text,"coop_door_fixture open")||!std::strcmp(text,"coop_door_fixture close"))return true;
    if(!std::strncmp(text,"coop_observer_camera hold_at ",29)){float x=0,y=0,z=0;char extra=0;
        return sscanf_s(text+29,"%f %f %f %c",&x,&y,&z,&extra,1u)==3&&std::isfinite(x)&&std::isfinite(y)&&std::isfinite(z)&&std::fabs(x)<100000.f&&std::fabs(y)<100000.f&&std::fabs(z)<100000.f;}
    if(!std::strncmp(text,"coop_usable_fixture ",20)){char name[192]{},extra=0;unsigned target=0;return sscanf_s(text+20,"%191s %u %c",name,static_cast<unsigned>(sizeof(name)),&target,&extra,1u)==2&&target<32;}
    if(!std::strcmp(text,"coop_climb_fixture spawn"))return true;
    if(!std::strcmp(text,"coop_spawn_probe emily")||!std::strcmp(text,"coop_spawn_probe corvo")||!std::strcmp(text,"coop_spawn_probe shadow_emily")||!std::strcmp(text,"coop_spawn_probe shadow_corvo"))return true;
    if(!std::strcmp(text,"coop_npc_attack on")||!std::strcmp(text,"coop_npc_attack off")||!std::strcmp(text,"coop_npc_attack duplicate"))return true;
    if(!std::strcmp(text,"coop_avatar_perception on")||!std::strcmp(text,"coop_avatar_perception off"))return true;
    if(!std::strcmp(text,"coop_probe_follow on")||!std::strcmp(text,"coop_probe_follow exact")||!std::strcmp(text,"coop_probe_follow off"))return true;
    // Native developer campaign loader (exact-image usage string: "campaign
    // <mapinfo> [low|high] [emily|corvo]"). Only packaged campaign mapinfos,
    // for isolated fixtures beyond the Dunwall tower; no other arguments.
    if(!std::strncmp(text,"campaign campaign/",18)){
        const char* end=std::strstr(text,".mapinfo");if(!end)return false;
        for(const char* c=text+9;c<end;++c)if(!((*c>='a'&&*c<='z')||(*c>='0'&&*c<='9')||*c=='_'||*c=='/'))return false;
        if(std::strstr(text,".."))return false;
        const char* rest=end+8;
        if(!std::strncmp(rest," low",4))rest+=4;else if(!std::strncmp(rest," high",5))rest+=5;
        if(!std::strncmp(rest," emily",6))rest+=6;else if(!std::strncmp(rest," corvo",6))rest+=6;
        return *rest==0;
    }
    if(!std::strcmp(text,"coop_player_health_hold on")||!std::strcmp(text,"coop_player_health_hold off"))return true;
    if(!std::strcmp(text,"coop_grant_item grenade")||!std::strcmp(text,"coop_grant_item health")||!std::strcmp(text,"coop_grant_item mana"))return true;
    if(!std::strcmp(text,"coop_use_item health")||!std::strcmp(text,"coop_use_item mana"))return true;
    const char* commands[]={"getviewpos","where","Ansel_PlayerVisible","LoadGame","listDecls","debugEntityByName","trigger","teleport","setviewpos","find","listModels"};
    for(const auto* name:commands){const auto n=std::strlen(name);if(!std::strncmp(text,name,n)&&(text[n]==0||text[n]==' '))return true;}
    return false;
}
void dispatchCoopCommand(void* system,const char* text);
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
    dispatchCoopCommand(system,request.command);
}
// 1181: validated dispatch shared by the lab command file and the product
// session controller (same allowlist, same handlers).
void dispatchCoopCommand(void* system,const char* text) {
    struct {char command[512];} request{};
    if(!text||std::strlen(text)>=sizeof(request.command)||!allowedCommand(text))return;
    std::snprintf(request.command,sizeof(request.command),"%s",text);
    for(const unsigned char* p=reinterpret_cast<unsigned char*>(request.command);*p;++p)if(*p<32||*p>126||*p==';')return;
    if(!std::strcmp(request.command,"coop_gameover_probe")){probeGameOver(false);return;}
    if(!std::strcmp(request.command,"coop_sword_link 15000")){beginSwordLink(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_sword_link 60000")){beginSwordLink(GetTickCount64(),true);return;}
    if(!std::strcmp(request.command,"coop_sword_link off")){stopSwordLink("cancelled");return;}
    if(!std::strcmp(request.command,"coop_sword_duplicate")){repeatSwordPacket();return;}
    if(!std::strcmp(request.command,"coop_gameover_retry")){probeGameOver(true);return;}
    if(!std::strcmp(request.command,"coop_follow_host on")){followHostEnabled=true;missionFollow={};return;}
    if(!std::strcmp(request.command,"coop_follow_host off")){followHostEnabled=false;missionFollow={};missionFollow.result="disabled";return;}
    if(!std::strcmp(request.command,"coop_product_auto on")){productAutoArm=true;productArm={};return;}
    if(!std::strcmp(request.command,"coop_product_auto off")){productAutoArm=false;productArm={};productArm.result="disabled";return;}
    if(!std::strcmp(request.command,"coop_session host")){startOnlineHost();return;}
    if(!std::strcmp(request.command,"coop_session host_quiet")){if(productMode)return;startOnlineHost(true);return;}
    if(!std::strcmp(request.command,"coop_session off")){stopOnlineSession("disabled");return;}
    if(!std::strncmp(request.command,"coop_session join ",18)){startOnlineJoin(std::strtoull(request.command+18,nullptr,10));return;}
    if(!std::strcmp(request.command,"coop_autosave_probe")){autosaveProbeRequested=true;return;}
    if(!std::strcmp(request.command,"coop_trigger_mirror on")){triggerMirrorEnabled=true;triggerPeerSequence=0;triggerMirrorResult="armed";return;}
    if(!std::strcmp(request.command,"coop_trigger_mirror off")){triggerMirrorEnabled=false;triggerMirrorResult="disabled";return;}
    // Idempotent: re-arming in a new world (product controller) keeps a pending or sent mission end.
    if(!std::strcmp(request.command,"coop_mission_end_mirror on")){if(!missionEndMirrorEnabled){missionEndMirrorEnabled=true;missionEnd={};missionEnd.result="armed";}return;}
    if(!std::strcmp(request.command,"coop_mission_end_mirror off")){missionEndMirrorEnabled=false;missionEnd={};missionEnd.result="disabled";return;}
    if(!std::strncmp(request.command,"coop_chaos_points ",18)){labSetChaosPoints(request.command+18);return;}
    if(!std::strcmp(request.command,"coop_gameflow_mirror on")){if(!gameflowMirrorEnabled){gameflowMirrorEnabled=true;gameflowMirror={};gameflowMirror.result="armed";}return;}
    if(!std::strcmp(request.command,"coop_gameflow_mirror off")){gameflowMirrorEnabled=false;gameflowMirror={};gameflowMirror.result="disabled";return;}
    if(!std::strcmp(request.command,"coop_kiscule_mirror on")){AcquireSRWLockExclusive(&kisculeLock);if(!kisculeMirrorEnabled){kisculeMirrorEnabled=true;kisculeMirror={};kisculeMirror.result="armed";}ReleaseSRWLockExclusive(&kisculeLock);return;}
    if(!std::strcmp(request.command,"coop_kiscule_mirror off")){AcquireSRWLockExclusive(&kisculeLock);kisculeMirrorEnabled=false;kisculeMirror={};kisculeMirror.result="disabled";ReleaseSRWLockExclusive(&kisculeLock);return;}
    if(!std::strcmp(request.command,"coop_checkpoint_sync on")){checkpointEnable(true);return;}
    if(!std::strcmp(request.command,"coop_checkpoint_sync off")){checkpointEnable(false);return;}
    if(!std::strcmp(request.command,"coop_checkpoint_sync retry_probe")){checkpointArmRetryProbe();return;}
    if(!std::strncmp(request.command,"coop_checkpoint_delay ",22)){checkpointSync.readyDelay=std::strtoul(request.command+22,nullptr,10);return;}
    if(!std::strcmp(request.command,"coop_loading_probe 2000")){armLoadingProbe();return;}
    if(!std::strcmp(request.command,"coop_partner_enemy 8000")){beginPartnerEnemy(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_partner_enemy escape 8000")){beginPartnerEnemy(GetTickCount64(),true);return;}
    if(!std::strcmp(request.command,"coop_partner_enemy escape 60000")){beginPartnerEnemy(GetTickCount64(),true,60000);return;}
    if(!std::strcmp(request.command,"coop_partner_enemy off")){stopPartnerEnemy("cancelled");return;}
    if(!std::strcmp(request.command,"coop_guard_health 35")){beginGuardHealthStimulus();return;}
    if(!std::strcmp(request.command,"coop_damage_sample sword")){beginSwordDamageSample();return;}
    if(!std::strcmp(request.command,"coop_damage_sample sword_back")){beginSwordDamageSample(true);return;}
    if(!std::strcmp(request.command,"coop_enemy_damage assassination")){beginEnemyDamageSample();return;}
    if(!std::strcmp(request.command,"coop_enemy_outcome 60000")){beginEnemyOutcome(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_enemy_outcome off")){stopEnemyOutcome("cancelled");return;}
    if(!std::strcmp(request.command,"coop_enemy_duplicate")){repeatEnemyOutcome();return;}
    if(!std::strcmp(request.command,"coop_enemy_melee 15000")){beginEnemyMelee(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_enemy_melee off")){stopEnemyMelee("cancelled");return;}
    if(!std::strcmp(request.command,"coop_enemy_melee_duplicate")){repeatEnemyMelee();return;}
    if(!std::strcmp(request.command,"coop_damage_observe 60000")){armDamageObservation(true);return;}
    if(!std::strcmp(request.command,"coop_damage_observe off")){armDamageObservation(false);return;}
    if(!std::strcmp(request.command,"coop_body_probe 2000")){beginBodyProbe();return;}
    if(!std::strcmp(request.command,"coop_body_probe off")){stopBodyProbe();return;}
    if(!std::strcmp(request.command,"coop_probe_arm_space on")||!std::strcmp(request.command,"coop_probe_arm_space off")){
        stopArmProbe("coordinate_mode_changed");armModelSpaceEnabled=!std::strcmp(request.command,"coop_probe_arm_space on");return;
    }
    if(!std::strcmp(request.command,"coop_arm_probe 2000")){beginArmProbe();return;}
    if(!std::strcmp(request.command,"coop_arm_probe off")){stopArmProbe("cancelled");return;}
    if(!std::strcmp(request.command,"coop_probe_arm on")||!std::strcmp(request.command,"coop_probe_arm off")){
        stopArmProbe("stream_changed");armStreamEnabled=!std::strcmp(request.command,"coop_probe_arm on");return;
    }
    if(!std::strcmp(request.command,"coop_arm_duplicate")){duplicateArmStream();return;}
    if(!std::strcmp(request.command,"coop_shadow_equipment 2500")){beginShadowEquipment();return;}
    if(!std::strcmp(request.command,"coop_shadow_equipment off")){stopShadowEquipment("cancelled");return;}
    if(!std::strcmp(request.command,"coop_avatar_sword 2000")){beginAvatarSword();return;}
    if(!std::strcmp(request.command,"coop_avatar_sword off")){stopAvatarSword("cancelled");return;}
    if(!std::strcmp(request.command,"coop_shadow_stream 15000")){beginShadowStream();return;}
    if(!std::strcmp(request.command,"coop_shadow_stream on")){beginShadowStream(true);return;}
    if(!std::strcmp(request.command,"coop_shadow_stream off")){stopShadowStream("cancelled");return;}
    if(!std::strcmp(request.command,"coop_shadow_stream_duplicate")){duplicateShadowStream();return;}
    if(!std::strcmp(request.command,"coop_shadow_unbound_probe 6000")){shadowUnboundProbeUntil=shadowStream.until?GetTickCount64()+6000:0;return;}
    if(!std::strcmp(request.command,"coop_shadow_unbound_probe off")){shadowUnboundProbeUntil=0;return;}
    if(!std::strcmp(request.command,"coop_power_fx blink_start")){powerFxRequest=1;return;}
    if(!std::strcmp(request.command,"coop_power_fx blink_out")){powerFxRequest=2;return;}
    if(!std::strcmp(request.command,"coop_power_fx off")){powerFxRequest=3;return;}
    if(!std::strcmp(request.command,"coop_power_fx duplicate")){duplicateNativePowerFx();return;}
    if(!std::strcmp(request.command,"coop_power_fx grapple_beam")){powerFxRequest=4;return;}
    if(!std::strcmp(request.command,"coop_power_fx grapple_impact")){powerFxRequest=5;return;}
    if(!std::strcmp(request.command,"coop_power_fx grapple_duplicate")){duplicateNativeGrappleFx();return;}
    if(!std::strcmp(request.command,"coop_power_fx windblast_probe")){powerFxRequest=6;return;}
    if(!std::strcmp(request.command,"coop_power_fx windblast_ground_probe")){powerFxRequest=7;return;}
    if(!std::strcmp(request.command,"coop_power_fx windblast_remote_probe")){powerFxRequest=8;return;}
    if(!std::strcmp(request.command,"coop_swarm_fx duplicate")){duplicateSwarmSpawnFx();return;}
    if(!std::strcmp(request.command,"coop_power_fx windblast_duplicate")){duplicateNativeWindblastFx();return;}
    if(!std::strcmp(request.command,"coop_power_fx bendtime_viewer_probe")){powerFxRequest=10;return;}
    if(!std::strcmp(request.command,"coop_power_fx outofmana_hand_probe")){powerFxRequest=11;return;}
    if(!std::strcmp(request.command,"coop_avatar_weapon 15000")){beginAvatarWeapon();return;}
    if(!std::strcmp(request.command,"coop_avatar_weapon off")){stopAvatarWeapon("cancelled");return;}
    if(!std::strcmp(request.command,"coop_avatar_weapon_duplicate")){duplicateAvatarWeapon();return;}
    if(!std::strcmp(request.command,"coop_probe_body on")||!std::strcmp(request.command,"coop_probe_body off")){
        if(!std::strcmp(request.command,"coop_probe_body on")&&(shadowAction.actor||shadowStream.until))return;
        stopBodyProbe();bodyStreamEnabled=!std::strcmp(request.command,"coop_probe_body on");return;
    }
    if(!std::strcmp(request.command,"coop_shadow_crouch capture")){consoleOutput("COOP: complete native crouch capture %s\n",captureNativeCrouch()?"accepted":"refused");return;}
    if(!std::strcmp(request.command,"coop_mixamo_crouch off")){mixamoResearch={};clearBodyProbe();return;}
    if(!std::strcmp(request.command,"coop_mixamo_crouch 30000")||!std::strcmp(request.command,"coop_mixamo_crouch on")){
        if(!bodyStreamEnabled||!nativeCrouchReferenceReady()||!probeFollow||!guardPairReady(GetTickCount64())||mixamoResearch.active(GetTickCount64()))return;
        NativeSwordEquipment sword{};
        if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||sword.actor!=probeActor||!sword.equipped||!sword.started)return;
        clearBodyProbe();mixamoResearch={};mixamoResearch.persistent=!std::strcmp(request.command,"coop_mixamo_crouch on");mixamoResearch.until=GetTickCount64()+30000;mixamoResearch.actor=probeActor;mixamoResearch.epoch=outgoing.worldEpoch;mixamoResearch.peerEpoch=remote.worldEpoch;mixamoResearch.peerSession=remote.session;return;
    }
    if(!std::strcmp(request.command,"coop_shadow_crouch upright")||!std::strcmp(request.command,"coop_shadow_crouch native")){
        clearBodyProbe();bodyCrouchUpright=!std::strcmp(request.command,"coop_shadow_crouch upright");return;
    }
    if(!std::strcmp(request.command,"coop_shadow_crouch off")){clearBodyProbe();bodyStreamEnabled=false;nativeCrouchReference={};return;}
    if(!std::strcmp(request.command,"coop_loading_probe emily")){armLoadingProbe(2);return;}
    if(!std::strcmp(request.command,"coop_character_probe emily")){probeNativeCharacter(2);return;}
    if(!std::strcmp(request.command,"coop_character_probe corvo")){probeNativeCharacter(1);return;}
    if(!std::strcmp(request.command,"coop_guard_link 8000")){beginGuardLink(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_npc_hold 12000")){npcAuthorityHoldRequest=1;return;}
    if(!std::strcmp(request.command,"coop_npc_hold off")){npcAuthorityHoldRequest=0;return;}
    if(!std::strcmp(request.command,"coop_npc_combat fixture40000")){npcCombatFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_npc_combat fixture_ai40000")){npcCombatFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_npc_combat trigger_fixture_once")){if(npcCombat.enabled)npcCombatTriggerFixtureRequested=true;return;}
    if(!std::strcmp(request.command,"coop_npc_combat knockout_test")){npcKnockoutTest.decl=kNpcKnockoutDecl;npcKnockoutTest.requested=true;return;}
    if(!std::strcmp(request.command,"coop_npc_combat hostworld_test")){npcHostWorldTest.player=false;npcHostWorldTest.requested=true;return;}
    if(!std::strcmp(request.command,"coop_npc_combat hostnative_test")){npcHostWorldTest.player=true;npcHostWorldTest.requested=true;return;}
    if(!std::strcmp(request.command,"coop_npc_combat knockout_reaction_test")){npcKnockoutTest.decl="components/base/damage/sleep_damage.damage";npcKnockoutTest.requested=true;return;}
    if(!std::strcmp(request.command,"coop_npc_combat knockout_generic_test")){npcKnockoutTest.decl="code_referenced/damage/generic_sleep_damage.damage";npcKnockoutTest.requested=true;return;}
    if(!std::strcmp(request.command,"coop_npc_combat fixture_off")){npcCombatFixtureRequest=0;return;}
    if(!std::strcmp(request.command,"coop_npc_combat ordinary40000")){if(npcCombat.enabled)npcCombatOrdinaryTestUntil.store(GetTickCount64()+40000);return;}
    if(!std::strcmp(request.command,"coop_npc_combat on")){npcCombatRequest=1;return;}
    if(!std::strcmp(request.command,"coop_npc_combat off")){npcCombatRequest=0;return;}
    if(!std::strcmp(request.command,"coop_npc_combat duplicate")){repeatNpcCombat();return;}
    if(!std::strcmp(request.command,"coop_npc_stream on")){npcStreamRequest=1;return;}
    if(!std::strcmp(request.command,"coop_npc_stream off")){npcStreamRequest=0;return;}
    if(!std::strcmp(request.command,"coop_guard_link escape 60000")){beginGuardLink(GetTickCount64(),true);return;}
    if(!std::strcmp(request.command,"coop_guard_body on")||!std::strcmp(request.command,"coop_guard_body off")){
        stopGuardBody();guardBodyEnabled=!std::strcmp(request.command,"coop_guard_body on");return;
    }
    if(!std::strcmp(request.command,"coop_guard_link off")){stopGuardLink("cancelled");return;}
    if(!std::strcmp(request.command,"coop_guard_equipment 2000")){beginGuardEquipment(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_guard_equipment off")){stopGuardEquipment("cancelled");return;}
    if(!std::strcmp(request.command,"coop_observer_isolation 30000")){beginObserverIsolation(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_movement_fixture 60000")){beginObserverIsolation(GetTickCount64(),localRole==0,true);return;}
    if(!std::strcmp(request.command,"coop_movement_fixture off")){stopObserverIsolation("movement_fixture_cancelled");return;}
    if(!std::strcmp(request.command,"coop_observer_camera hold")){observerCameraRequest=1;return;}
    if(!std::strcmp(request.command,"coop_player_health_hold on")){playerHealthHold=true;return;}
    if(!std::strcmp(request.command,"coop_player_health_hold off")){playerHealthHold=false;return;}
    if(!std::strcmp(request.command,"coop_grant_item grenade")){labGrantRequest=1;return;}
    if(!std::strcmp(request.command,"coop_grant_item health")){labGrantRequest=2;return;}
    if(!std::strcmp(request.command,"coop_grant_item mana")){labGrantRequest=3;return;}
    if(!std::strcmp(request.command,"coop_use_item health")){labUseRequest=1;return;}
    if(!std::strcmp(request.command,"coop_use_item mana")){labUseRequest=2;return;}
    if(!std::strncmp(request.command,"coop_observer_camera hold_at ",29)){float at[3]{};char extra=0;
        if(sscanf_s(request.command+29,"%f %f %f %c",&at[0],&at[1],&at[2],&extra,1u)==3){std::memcpy(observerCameraAt,at,12);observerCameraRequest=2;}return;}
    if(!std::strcmp(request.command,"coop_observer_camera off")){observerCameraRequest=0;return;}
    if(!std::strcmp(request.command,"coop_power_fixture 20000")){powerFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_power_fixture off")){powerFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_darkvision_fixture 20000")){darkVisionFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_darkvision_fixture off")){darkVisionFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_windblast_fixture 20000")){windblastFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_windblast_fixture off")){windblastFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_mesmerize_fixture 20000")){mesmerizeFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_doppelganger_fixture 20000")){doppelgangerFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_doppelganger_fixture 40000")){doppelgangerFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_doppelganger_fixture off")){doppelgangerFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_shadowwalk_fixture 20000")){shadowwalkFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_shadowwalk_fixture 40000")){shadowwalkFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_shadowwalk_fixture off")){shadowwalkFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_domino_fixture 20000")){dominoFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_possession_fixture 20000")){possessionFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_possession_fixture 40000")){possessionFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_possession_fixture human40000")){possessionFixtureRequest=4;return;}
    if(!std::strcmp(request.command,"coop_possession_fixture off")){possessionFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_swarm_fixture 20000")){swarmFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_mesmerize_fixture 40000")){mesmerizeFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_domino_fixture 40000")){dominoFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_swarm_fixture 40000")){swarmFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_mesmerize_fixture off")){mesmerizeFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_domino_fixture off")){dominoFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_swarm_fixture off")){swarmFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_bendtime_fixture 20000")){bendtimeFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_bendtime_fixture off")){bendtimeFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_ranged_fixture gun")){rangedFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_ranged_fixture crossbow")){rangedFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_ranged_fixture off")){rangedFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_ranged_fixture gun_loaded")){rangedFixtureRequest=4;return;}
    if(!std::strcmp(request.command,"coop_grenade_fixture 12000")){grenadeFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_grenade_fixture off")){grenadeFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_heart_fixture 20000")){heartFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_heart_fixture off")){heartFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_trap_fixture stun")){trapFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_trap_fixture razor")){trapFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_trap_fixture off")){trapFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_coin_fixture collect")){coinFixtureRequest=true;return;}
    if(!std::strncmp(request.command,"coop_coin_sync exercise ",24)){coinExerciseRequest=request.command+24;return;}
    if(!std::strcmp(request.command,"coop_coin_sync capacity off")){coinCapacityOff=true;return;}
    if(!std::strncmp(request.command,"coop_coin_sync capacity ",24)){parseCoinCapacityCommand(request.command,coinCapacityName,coinCapacityQuantity);return;}
    if(!std::strcmp(request.command,"coop_coin_sync on")){coinEnableRequest=1;return;}
    if(!std::strcmp(request.command,"coop_coin_sync off")){coinEnableRequest=0;return;}
    if(!std::strcmp(request.command,"coop_coin_sync duplicate")){coinReplayRequest=1;return;}
    if(!std::strcmp(request.command,"coop_coin_sync stale")){coinReplayRequest=2;return;}
    if(!std::strcmp(request.command,"coop_note_fixture restore_absent")){noteRestoreRequest=0;return;}
    if(!std::strcmp(request.command,"coop_note_fixture restore_present")){noteRestoreRequest=1;return;}
    if(!std::strcmp(request.command,"coop_potion_fixture health")){potionFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_potion_fixture mana")){potionFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_potion_fixture off")){potionFixtureRequest=3;return;}
    if(!std::strcmp(request.command,"coop_potion_fixture use_health")){potionFixtureRequest=4;return;}
    if(!std::strcmp(request.command,"coop_potion_fixture use_mana")){potionFixtureRequest=5;return;}
    if(!std::strcmp(request.command,"coop_ranged_fixture crossbow_loaded")){rangedFixtureRequest=5;return;}
    if(!std::strcmp(request.command,"coop_ranged_prop gun")){rangedPropRequest=1;return;}
    if(!std::strcmp(request.command,"coop_ranged_prop crossbow")){rangedPropRequest=2;return;}
    if(!std::strcmp(request.command,"coop_ranged_prop off")){rangedPropRequest=3;return;}
    if(!std::strcmp(request.command,"coop_mesmerize_visual duplicate")){duplicateMesmerizeWorld();return;}
    if(!std::strcmp(request.command,"coop_mesmerize_visual in")){mesmerizeVisualRequest=1;return;}
    if(!std::strcmp(request.command,"coop_mesmerize_visual loop")){mesmerizeVisualRequest=2;return;}
    if(!std::strcmp(request.command,"coop_mesmerize_visual out")){mesmerizeVisualRequest=3;return;}
    if(!std::strcmp(request.command,"coop_mesmerize_visual off")){mesmerizeVisualRequest=4;return;}
    if(!std::strcmp(request.command,"coop_grenade_prop 15000")){grenadePropRequest=1;return;}
    if(!std::strcmp(request.command,"coop_grenade_prop off")){grenadePropRequest=2;return;}
    if(!std::strcmp(request.command,"coop_agility_fixture 30000")){agilityFixtureRequest=1;return;}
    if(!std::strcmp(request.command,"coop_agility_fixture off")){agilityFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_carry_attachment 8000")){carryAttachmentPickupRequested=false;carryAttachmentSnapshotRequested=false;carryAttachmentDuration=8000;carryAttachmentRequest=1;return;}
    if(!std::strcmp(request.command,"coop_carry_attachment 16000")){carryAttachmentPickupRequested=false;carryAttachmentSnapshotRequested=false;carryAttachmentDuration=16000;carryAttachmentRequest=1;return;}
    if(!std::strcmp(request.command,"coop_carry_attachment snapshot8000")){carryAttachmentPickupRequested=false;carryAttachmentSnapshotRequested=true;carryAttachmentDuration=8000;carryAttachmentRequest=1;return;}
    if(!std::strcmp(request.command,"coop_carry_attachment pickup8000")){carryAttachmentPickupRequested=true;carryAttachmentSnapshotRequested=true;carryAttachmentDuration=8000;carryAttachmentRequest=1;return;}
    if(!std::strcmp(request.command,"coop_carry_attachment off")){carryAttachmentRequest=2;return;}
    if(!std::strcmp(request.command,"coop_observer_isolation off")){stopObserverIsolation("cancelled");return;}
    if(!std::strcmp(request.command,"coop_source_isolation 30000")){beginObserverIsolation(GetTickCount64(),true);return;}
    if(!std::strcmp(request.command,"coop_source_isolation off")){stopObserverIsolation("cancelled");return;}
    if(!std::strcmp(request.command,"coop_guard_weapon on")||!std::strcmp(request.command,"coop_guard_weapon off")){
        const bool enable=!std::strcmp(request.command,"coop_guard_weapon on");
        if(enable&&(guardEquipment.until||guardEquipment.restoreUntil)){++guardWeaponRejected;guardWeaponResult="equipment_busy";return;}
        stopGuardWeapon("local_toggle");guardWeaponEnabled=enable;return;
    }
    if(!std::strcmp(request.command,"coop_guard_nudge 1000")){beginGuardNudge(GetTickCount64());return;}
    if(!std::strcmp(request.command,"coop_guard_stall 1000")) {
        if(!localRole&&guardLink.phase==GuardPhase::Active)guardStreamBlockedUntil=GetTickCount64()+1000;
        return;
    }
    if(!std::strncmp(request.command,"LoadGame",8)){stopSwordLink("load_requested");stopPartnerEnemy("load_requested");stopGuardLink("load_requested");}
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
    if(!std::strncmp(request.command,"coop_authoring_proof ",21)){consoleOutput("COOP: authored proof %s\n",beginAuthoringProof(request.command+21)?"accepted":"refused");return;}
    if(!std::strcmp(request.command,"coop_movement_bank off")){stopMovementBank();return;}
    if(!std::strcmp(request.command,"coop_movement_resume on")){beginMovementResume();return;}
    if(!std::strcmp(request.command,"coop_movement_resume off")){stopMovementResume();return;}
    if(!std::strcmp(request.command,"coop_movement_feet on")||!std::strcmp(request.command,"coop_movement_feet off")||!std::strcmp(request.command,"coop_movement_feet authored")){
        if(!movementBankActive()&&!movementLoad.state.load()){
            movementNativeFeetRequested=std::strcmp(request.command,"coop_movement_feet off")!=0;
            movementCurrentFeetRequested=!std::strcmp(request.command,"coop_movement_feet authored");
        }
        return;
    }
    if(!std::strcmp(request.command,"coop_movement_bank 60000")){consoleOutput("COOP: movement bank %s\n",beginMovementBank()?"accepted":"refused");return;}
    if(!std::strcmp(request.command,"coop_movement_bank on")){const bool accepted=beginMovementBank(true);consoleOutput("COOP: continuous movement bank %s (reason %u)\n",accepted?"accepted":"refused",accepted?0u:movementBeginRefusal);return;}
    if(!std::strcmp(request.command,"coop_review_clip off")){stopReviewClip();return;}
    if(!std::strcmp(request.command,"coop_review_clip play")){consoleOutput("COOP: review clip %s\n",beginReviewClip(-1)?"accepted":"refused");return;}
    if(!std::strncmp(request.command,"coop_review_clip hold ",22)){consoleOutput("COOP: review hold %s\n",beginReviewClip(std::atoi(request.command+22))?"accepted":"refused");return;}
    if(!std::strncmp(request.command,"coop_shadow_pose ",17)){
        char pose[24]{};unsigned milliseconds=0;
        consoleOutput("COOP: native pose sample %s\n",(!shadowStream.until&&parseShadowPose(request.command,pose,milliseconds)&&playShadowAction(pose,milliseconds*.001f))?"accepted":"refused");return;
    }
    if(!std::strncmp(request.command,"coop_shadow_action ",19)){consoleOutput("COOP: shadow action request %s\n",(!shadowStream.until&&playShadowAction(request.command+19))?"accepted":"refused");return;}
    if(!std::strcmp(request.command,"coop_shadow_damage_probe")){probeShadowDamage();return;}
    if(!std::strcmp(request.command,"coop_friendly_fire_probe")){probeFriendlyFire();return;}
    if(!std::strcmp(request.command,"coop_pose_trace 3000")){if(probeFollow&&armProbePairReady(GetTickCount64())){poseCadenceCount=0;poseCadenceStart=presentationClock();poseCadenceUntil=poseCadenceStart+3000;}return;}
    if(!std::strncmp(request.command,"coop_probe_smoothing ",21)){poseSmoothingEnabled=!std::strcmp(request.command+21,"on");poseInterpolation.clear();return;}
    if(!std::strncmp(request.command,"coop_shadow_loco ",17)){
        const bool off=!std::strcmp(request.command+17,"off");
        shadowLocoUntil=0;shadowLocoCombat=!std::strcmp(request.command+17,"combat");
        NativeSwordEquipment sword{};
        if(!off&&avatarAiHeld&&probeFollow&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&lastReceive&&GetTickCount64()-lastReceive<250&&readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)&&sword.actor==probeActor&&sword.equipped&&sword.started){
            shadowLocoGame=probeGame;shadowLocoActor=probeActor;shadowLocoHandle=probeHandle;shadowLocoEpoch=outgoing.worldEpoch;
            shadowLocoRemoteEpoch=remote.worldEpoch;shadowLocoRemoteSession=remote.session;shadowLocoUntil=GetTickCount64()+15000;
            reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(sword.entity));
            reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(sword.entity));
        }return;
    }
    if(!std::strcmp(request.command,"coop_decl_probe")){probePartnerDefinitions();return;}
    if(!std::strcmp(request.command,"coop_door_sync on")){doorSyncEnableRequest=1;return;}
    if(!std::strcmp(request.command,"coop_usable_sync on")){usableSyncEnableRequest=1;return;}
    if(!std::strcmp(request.command,"coop_door_sync session")){doorSyncEnableRequest=2;return;}
    if(!std::strcmp(request.command,"coop_usable_sync session")){usableSyncEnableRequest=2;return;}
    if(!std::strcmp(request.command,"coop_door_sync off")){doorSyncEnableRequest=0;return;}
    if(!std::strcmp(request.command,"coop_usable_sync off")){usableSyncEnableRequest=0;return;}
    if(!std::strcmp(request.command,"coop_door_sync duplicate")){doorSyncDuplicateRequest=true;return;}
    if(!std::strcmp(request.command,"coop_usable_sync duplicate")){usableSyncDuplicateRequest=true;return;}
    if(!std::strcmp(request.command,"coop_door_fixture open")){doorFixtureRequest=2;return;}
    if(!std::strcmp(request.command,"coop_door_fixture close")){doorFixtureRequest=0;return;}
    if(!std::strncmp(request.command,"coop_usable_fixture ",20)){char name[192]{},extra=0;unsigned target=0;if(sscanf_s(request.command+20,"%191s %u %c",name,static_cast<unsigned>(sizeof(name)),&target,&extra,1u)==2&&target<32){usableFixtureName=name;usableFixtureTarget=target;}return;}
    if(!std::strcmp(request.command,"coop_climb_fixture spawn")){climbFixtureRequest=true;return;}
    if(!std::strncmp(request.command,"coop_spawn_probe ",17)) {
        const bool spawned=spawnProbe(request.command+17);
        consoleOutput("COOP: spawn probe request %u %s\n",lastCommand,spawned?"succeeded":"refused");return;
    }
    if(!std::strcmp(request.command,"coop_npc_attack on")){npcAttackRequest=1;return;}
    if(!std::strcmp(request.command,"coop_npc_attack off")){npcAttackRequest=0;return;}
    if(!std::strcmp(request.command,"coop_npc_attack duplicate")){repeatNpcAttack();return;}
    if(!std::strcmp(request.command,"coop_avatar_perception on")||!std::strcmp(request.command,"coop_avatar_perception off")){avatarPerceptionEnabled=!std::strcmp(request.command,"coop_avatar_perception on");writeAvatarPerception();return;}
    if(!std::strcmp(request.command,"coop_probe_follow on")||!std::strcmp(request.command,"coop_probe_follow exact")) {
        stopAvatarWeapon("follow_changed");stopAvatarSword("follow_changed");stopArmProbe("follow_changed");enableProbeFollow(!std::strcmp(request.command,"coop_probe_follow exact"));return;
    }
    if(!std::strcmp(request.command,"coop_probe_follow off")){stopAvatarWeapon("follow_disabled");stopAvatarSword("follow_disabled");stopArmProbe("follow_disabled");networkFrameOwned.store(false,std::memory_order_release);probeFollow=false;releaseAvatarAi("follow_disabled");return;}
    char command[514]{};std::snprintf(command,sizeof(command),"%s\n",request.command);
    // Native AppendText method; called only inside native ExecuteBuffer's thread.
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0xfec70)(system,command);
    consoleOutput("COOP: queued native command %u: %s\n",lastCommand,request.command);
}
void followProbe(ULONGLONG tick) {
    const bool retainedReadable=movementRetainsReadableBody(tick);
    if(!retainedReadable&&avatarAiHeld&&(!probeActor||!probeFollow||phaseFollowInvalidated||!coopPresentationReady(outgoing)||!coopPresentationReady(remote)||
       !lastReceive||tick-lastReceive>=1000||remote.session!=probeRemoteSession||remote.worldEpoch!=probeRemoteEpoch))releaseAvatarAi("follow_scope_changed");
    if(!probeActor)return;
    const auto slot=static_cast<std::int32_t>(probeHandle)>>16;
    std::uintptr_t registered=0,render=0,renderVtable=0;std::uint32_t actual=0;
    if(probeGame!=observedGame||slot<=0||slot>=16384||
       !memoryRead(observedGame+0x3130b0+slot*8,&registered,8)||registered!=probeActor||
       !memoryRead(probeActor+0xa8,&actual,4)||actual!=probeHandle||!memoryRead(probeActor+0xf0,&render,8)||!render||
       !memoryRead(render,&renderVtable,8)) {networkFrameOwned.store(false,std::memory_order_release);controlledFrame.store(nullptr);retireShadowDamageIdentity();probeActor=probeGame=probeHandle=0;probeFollow=false;return;}
    if(retainedReadable){
        // Keep the already visible native clone at its last position. Never
        // sample/apply a new root, action, damage or body pose from a menu.
        // Retain its validated frame ownership so native physics cannot move
        // that frame while gameplay pose publication is suspended.
        memoryRead(render+0x1860,probeRenderPosition,sizeof(probeRenderPosition));return;
    }
    if(probeFollow) {
        if(phaseFollowInvalidated||!coopPresentationReady(outgoing)||!coopPresentationReady(remote)||outgoing.mapId!=remote.mapId||!lastReceive||tick-lastReceive>=1000||remote.session!=probeRemoteSession||remote.worldEpoch!=probeRemoteEpoch) {
            networkFrameOwned.store(false,std::memory_order_release);
            probeFollow=false;reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(probeActor));
            consoleOutput("COOP: native actor follow stopped on stale peer, changed world, or native game-logic transition bits=%u local=%u/%u/%u/%u/%u remote=%u/%u/%u/%u/%u age=%llu\n",
                (phaseFollowInvalidated?1u:0u)|(!coopPresentationReady(outgoing)?2u:0u)|(!coopPresentationReady(remote)?4u:0u)|(outgoing.mapId!=remote.mapId?8u:0u)|(!lastReceive||tick-lastReceive>=1000?16u:0u)|(remote.session!=probeRemoteSession?32u:0u)|(remote.worldEpoch!=probeRemoteEpoch?64u:0u),
                outgoing.gameLogicState,outgoing.pendingGameLogicState,outgoing.flags,outgoing.cinematicMetadataValid,outgoing.cinematicNodeCount,
                remote.gameLogicState,remote.pendingGameLogicState,remote.flags,remote.cinematicMetadataValid,remote.cinematicNodeCount,lastReceive?tick-lastReceive:0ull);
        }else {
            PresentationPose sampled{};sampled.bodyValid=remote.bodyPoseValid!=0;sampled.motionValid=remote.playerMotionValid!=0;sampled.stance=remote.playerStance;sampled.motion=remote.playerMotion;
            std::memcpy(sampled.position,remote.position,12);std::memcpy(sampled.orientation,remote.orientation,36);
            if(poseSmoothingEnabled)poseInterpolation.sample(presentationClock(),sampled);
            presentNativeBlink(sampled,tick);
            presentNativeGrapple(sampled,tick);
            presentNativeWindblastFx(sampled,tick);
            float position[3];for(int i=0;i<3;++i)position[i]=sampled.position[i]+probeOffset[i];
            std::memcpy(probeTargetPosition,position,12);
            publishLocomotion(tick,position,sampled.orientation);
            publishMovementBank(sampled,position,presentationClock());
            publishBodyStream(tick);
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
                    for(int i=0;i<9;++i)changed=changed||std::fabs(sample.before[i+3]-sampled.orientation[i])>0.0001f;
                    capture=changed&&readNativeBodies(probeActor,sample.bodiesBefore);
                    if(capture)sample.deferred=reinterpret_cast<bool(*)(void*)>(gameImage+0x8a7820)(reinterpret_cast<void*>(observedGame));
                }
                dispatchAvatarTransform(position,sampled.orientation);
                if(capture&&memoryRead(probeActor+0x300,sample.after,sizeof(sample.after))&&readNativeBodies(probeActor,sample.bodiesAfter))
                    transformSamples[transformSampleCount++]=sample;
            }
        }
    }
    memoryRead(render+0x1860,probeRenderPosition,sizeof(probeRenderPosition));
    if(poseCadenceUntil){const auto now=presentationClock();
        if(now<poseCadenceUntil&&poseCadenceCount<2048&&probeFollow){auto& sample=poseCadence[poseCadenceCount];sample.time=now-poseCadenceStart;sample.sequence=remote.sequence;std::memcpy(sample.remote,remote.position,12);std::memcpy(sample.target,probeTargetPosition,12);std::memcpy(sample.render,probeRenderPosition,12);if(memoryRead(probeActor+0x300,sample.native,12))++poseCadenceCount;}
        if(now>=poseCadenceUntil){poseCadenceUntil=0;char path[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.pose-cadence.json",statusPath);FILE* f=nullptr;
            if(!fopen_s(&f,path,"wb")&&f){fprintf(f,"{\"smoothing\":%s,\"delayMs\":%.1f,\"samples\":[",poseSmoothingEnabled?"true":"false",PoseInterpolation::delayMs);
                for(unsigned i=0;i<poseCadenceCount;++i){const auto& s=poseCadence[i];fprintf(f,"%s{\"t\":%.4f,\"sequence\":%u,\"remote\":[%.8f,%.8f,%.8f],\"target\":[%.8f,%.8f,%.8f],\"native\":[%.8f,%.8f,%.8f],\"render\":[%.8f,%.8f,%.8f]}",i?",":"",s.time,s.sequence,s.remote[0],s.remote[1],s.remote[2],s.target[0],s.target[1],s.target[2],s.native[0],s.native[1],s.native[2],s.render[0],s.render[1],s.render[2]);}fprintf(f,"]}\n");fclose(f);}
        }
    }
}
#include "native_service_timing.h"
#include "diagnostic_schedule.h"
void writeDiagnosticSlice(ULONGLONG tick){
    if(!*statusPath)return;NativeServiceTimer timer("diagnostic_slice");
    static DiagnosticSchedule<72> schedule;const auto start=presentationClock();
    for(unsigned work=0;work<4;++work){unsigned index=0;if(!schedule.next(tick,index))break;
        switch(index){
        case 0:timedNativeService("writeNativeMotionRefusals",[&]{writeNativeMotionRefusals();});break;
        case 1:timedNativeService("writeAuthoringStatus",[&]{writeAuthoringStatus();});break;
        case 2:timedNativeService("writeReviewStatus",[&]{writeReviewStatus();});break;
        case 3:timedNativeService("writeWorldObservation",[&]{writeWorldObservation(tick);});break;
        case 4:timedNativeService("writeDamageObservation",[&]{writeDamageObservation(tick);});break;
        case 5:timedNativeService("writeFriendlyFireProbe",[&]{writeFriendlyFireProbe();});break;
        case 6:timedNativeService("writeMesmerizePartnerFilter",[&]{writeMesmerizePartnerFilter();});break;
        case 7:timedNativeService("writeDominoPartnerFilter",[&]{writeDominoPartnerFilter();});break;
        case 8:timedNativeService("writePossessionPartnerFilter",[&]{writePossessionPartnerFilter();});break;
        case 9:timedNativeService("writeAssassinationPartnerFilter",[&]{writeAssassinationPartnerFilter();});break;
        case 10:timedNativeService("writeInteractionPartnerFilter",[&]{writeInteractionPartnerFilter();});break;
        case 11:timedNativeService("writeSwordSampleStatus",[&]{writeSwordSampleStatus();});break;
        case 12:timedNativeService("writeEnemySampleStatus",[&]{writeEnemySampleStatus();});break;
        case 13:timedNativeService("writeEnemyOutcomeStatus",[&]{writeEnemyOutcomeStatus(tick);});break;
        case 14:timedNativeService("writeEnemyMeleeStatus",[&]{writeEnemyMeleeStatus(tick);});break;
        case 15:timedNativeService("writeArmProbe",[&]{writeArmProbe(tick);});break;
        case 16:timedNativeService("writeArmStream",[&]{writeArmStream(tick);});break;
        case 17:timedNativeService("writeAvatarSword",[&]{writeAvatarSword(tick);});break;
        case 18:timedNativeService("writeShadowEquipment",[&]{writeShadowEquipment(tick);});break;
        case 19:timedNativeService("writePossessionBodyVisibility",[&]{writePossessionBodyVisibility();});break;
        case 20:timedNativeService("writeAvatarWeapon",[&]{writeAvatarWeapon(tick);});break;
        case 21:timedNativeService("writeSwordLinkStatus",[&]{writeSwordLinkStatus(tick);});break;
        case 22:timedNativeService("writeGuardStatus",[&]{writeGuardStatus(tick);writeNpcAuthorityHold();writeNpcStream();writeNpcCombat();writeNpcKnockoutTest();writeNpcHostWorldTest();});break;
        case 23:timedNativeService("writeGuardBody",[&]{writeGuardBody(tick);});break;
        case 24:timedNativeService("writeGuardEquipment",[&]{writeGuardEquipment(tick);});break;
        case 25:timedNativeService("writeGuardWeapon",[&]{writeGuardWeapon(tick);});break;
        case 26:timedNativeService("writePowerFixture",[&]{writePowerFixture(tick);});break;
        case 27:timedNativeService("writeDarkVisionFixture",[&]{writeDarkVisionFixture(tick);});break;
        case 28:timedNativeService("writeWindblastFixture",[&]{writeWindblastFixture(tick);});break;
        case 29:timedNativeService("writeMesmerizeFixture",[&]{writeMesmerizeFixture(tick);});break;
        case 30:timedNativeService("writeDominoFixture",[&]{writeDominoFixture(tick);});break;
        case 31:timedNativeService("writeSwarmFixture",[&]{writeSwarmFixture(tick);});break;
        case 32:timedNativeService("writePossessionFixture",[&]{writePossessionFixture(tick);});break;
        case 33:timedNativeService("writeDoppelgangerFixture",[&]{writeDoppelgangerFixture(tick);});break;
        case 34:timedNativeService("writeShadowWalkFixture",[&]{writeShadowWalkFixture(tick);});break;
        case 35:timedNativeService("writeBendTimeFixture",[&]{writeBendTimeFixture(tick);});break;
        case 36:timedNativeService("writeRangedFixture",[&]{writeRangedFixture(tick);});break;
        case 37:timedNativeService("writeGrenadeFixture",[&]{writeGrenadeFixture(tick);});break;
        case 38:timedNativeService("writeHeartFixture",[&]{writeHeartFixture(tick);});break;
        case 39:timedNativeService("writeTrapFixture",[&]{writeTrapFixture(tick);});break;
        case 40:timedNativeService("writePotionFixture",[&]{writePotionFixture(tick);});break;
        case 41:timedNativeService("writeRangedProp",[&]{writeRangedProp();});break;
        case 42:timedNativeService("writeGrenadeProp",[&]{writeGrenadeProp();});break;
        case 43:timedNativeService("writeHeartProp",[&]{writeHeartProp();});break;
        case 44:timedNativeService("writeTrapProp",[&]{writeTrapProp();});break;
        case 45:timedNativeService("writeGrenadeFx",[&]{writeGrenadeFx();});break;
        case 46:timedNativeService("writeMesmerizeVisual",[&]{writeMesmerizeVisual();});break;
        case 47:timedNativeService("writeMesmerizeWorldStream",[&]{writeMesmerizeWorldStream();});break;
        case 48:timedNativeService("writeDominoWorld",[&]{writeDominoWorld();});break;
        case 49:timedNativeService("writeDominoBeams",[&]{writeDominoBeams();});break;
        case 50:timedNativeService("writePossessionFx",[&]{writePossessionFx();});break;
        case 51:timedNativeService("writeSwarmWorld",[&]{writeSwarmWorld();});break;
        case 52:timedNativeService("writeDoppelgangerWorld",[&]{writeDoppelgangerWorld();});break;
        case 53:timedNativeService("writeDoppelgangerVisual",[&]{writeDoppelgangerVisual();});break;
        case 54:timedNativeService("writeSwarmSpawnFx",[&]{writeSwarmSpawnFx();});break;
        case 55:timedNativeService("writeSwarmTrails",[&]{writeSwarmTrails();});break;
        case 56:timedNativeService("writeSwarmRatVisual",[&]{writeSwarmRatVisual();});break;
        case 57:timedNativeService("writePoseClockDiagnostics",[&]{writePoseClockDiagnostics();});break;
        case 58:timedNativeService("writeAgilityFixture",[&]{writeAgilityFixture(tick);});break;
        case 59:timedNativeService("writeCarryAttachmentFixture",[&]{writeCarryAttachmentFixture(tick);});break;
        case 60:timedNativeService("writeNativePowerFx",[&]{writeNativePowerFx();});break;
        case 61:timedNativeService("writeShadowWalkFx",[&]{writeShadowWalkFx();});break;
        case 62:timedNativeService("writePowerFxStream",[&]{writePowerFxStream();});break;
        case 63:timedNativeService("writeWindblastFxStream",[&]{writeWindblastFxStream();});break;
        case 64:timedNativeService("writeGrappleFxStream",[&]{writeGrappleFxStream();});break;
        case 65:timedNativeService("writePartnerEnemyStatus",[&]{writePartnerEnemyStatus(tick);});break;
        case 66:timedNativeService("writeCheckpointTrace",[&]{writeCheckpointTrace();writeActivationTrace();writeTriggerTrace();writeMissionEndMirror();writeKisculeMirror();writeOnlineSession();writeProductSession();});break;
        case 67:timedNativeService("writeSwitchTrace",[&]{writeSwitchTrace();});break;
        case 68:timedNativeService("writeReplacementTrace",[&]{writeReplacementTrace();});break;
        case 69:timedNativeService("writeCheckpointSync",[&]{writeCheckpointSync();});break;
        case 71:timedNativeService("writeDoorSync",[&]{writeDoorSync();writeCoinSync();writeUsableSync();writeAvatarPerception();writeNpcAttack();writeObserverCamera();});break;
        case 70:timedNativeService("writeMixamoStatus",[&]{{char path[MAX_PATH]{},temp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.mixamo.json",statusPath);std::snprintf(temp,sizeof(temp),"%s.tmp",path);FILE* f=nullptr;
     if(!fopen_s(&f,temp,"wb")&&f){fprintf(f,"{\"experimental\":true,\"active\":%s,\"persistent\":%s,\"until\":%llu,\"phase\":%.6f,\"applied\":%llu,\"gripApplied\":%llu,\"actionApplied\":%llu,\"epoch\":%llu}\n",mixamoResearch.active(tick)?"true":"false",mixamoResearch.persistent?"true":"false",mixamoResearch.until,mixamoResearch.phase,mixamoApplied.load(),mixamoGripApplied.load(),mixamoActionApplied.load(),mixamoResearch.epoch);const bool ok=!ferror(f);const auto closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);}}});break;
        }
        // Soft budget: finish the one atomic file write already in progress.
        if(presentationClock()-start>=4.)break;
    }
}
void writeStatus(ULONGLONG tick) {
    NativeServiceTimer scope("status_all");
    // A vault lasts ~450 ms. The ordinary 250 ms status cadence misses most
    // of its progression; sample only the small movement report at 50 ms
    // while the explicit experimental bank is active.
    static ULONGLONG lastMovementStatus=0;
    if(*statusPath&&tick-lastMovementStatus>=(movementBankActive()?50u:250u)){timedNativeService("writeMovementStatus",[&]{writeMovementStatus();});lastMovementStatus=tick;}
    writeDiagnosticSlice(tick);
    if(!*statusPath||tick-lastStatus<250)return;lastStatus=tick;
    timedNativeService("writeMovementResumeStatus",[&]{writeMovementResumeStatus();});
    timedNativeService("writeShadowStream",[&]{writeShadowStream(tick);});
    timedNativeService("writeObserverIsolation",[&]{writeObserverIsolation(tick);});
    char text[6144]{};
    int length=std::snprintf(text,sizeof(text),
        "{\"role\":%u,\"engineThread\":%lu,\"screenTransportPumps\":%llu,\"nativeInterface\":\"%s\",\"steamAvailable\":%s,\"listener\":%u,\"connection\":%u,\"connectionState\":%d,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"lastCommand\":%u,\"localPoseAvailable\":%s,\"localPosition\":[%.6f,%.6f,%.6f],\"remotePoseAvailable\":%s,\"remoteFresh\":%s,\"remoteSequence\":%u,\"remotePosition\":[%.6f,%.6f,%.6f],\"probeActorHandle\":%u,\"probeFollowEnabled\":%s,\"probeFollowExact\":%s,\"probeRenderPosition\":[%.6f,%.6f,%.6f],\"probeOffset\":[%.6f,%.6f,%.6f],\"remoteActorImplemented\":false,\"localMap\":\"%s\",\"localMapId\":\"%llx\",\"localEpoch\":%llu,\"remoteMapId\":\"%llx\",\"remoteEpoch\":%llu,\"localGameLogicState\":%u,\"localPendingGameLogicState\":%u,\"remoteGameLogicState\":%u,\"remotePendingGameLogicState\":%u,\"localCinematicMetadataValid\":%u,\"localCinematicNodeCount\":%u,\"localCinematicSignature\":\"%llx\",\"remoteCinematicMetadataValid\":%u,\"remoteCinematicNodeCount\":%u,\"remoteCinematicSignature\":\"%llx\"}\n",
        localRole,engineThread.load(),screenTransportPumps,STEAMNETWORKINGSOCKETS_INTERFACE_VERSION,sockets?"true":"false",listener,connection,connectionState,sent,received,rejected,lastCommand,
        outgoing.flags&1?"true":"false",outgoing.position[0],outgoing.position[1],outgoing.position[2],remote.flags&1?"true":"false",lastReceive&&tick-lastReceive<1000?"true":"false",remote.sequence,remote.position[0],remote.position[1],remote.position[2],probeHandle,probeFollow?"true":"false",probeFollowExact?"true":"false",probeRenderPosition[0],probeRenderPosition[1],probeRenderPosition[2],probeOffset[0],probeOffset[1],probeOffset[2],mapName,outgoing.mapId,outgoing.worldEpoch,remote.mapId,remote.worldEpoch,
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
    const int stance=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"localPlayerStanceValid\":%u,\"localPlayerStance\":%u,\"remotePlayerStanceValid\":%u,\"remotePlayerStance\":%u,\"localPlayerMotionValid\":%u,\"localPlayerMotion\":%u,\"remotePlayerMotionValid\":%u,\"remotePlayerMotion\":%u}\n",
        outgoing.playerStanceValid,outgoing.playerStance,remote.playerStanceValid,remote.playerStance,
        outgoing.playerMotionValid,outgoing.playerMotion,remote.playerMotionValid,remote.playerMotion);
    if(stance<=0||stance>=sizeof(text)-length+2)return;length=length-2+stance;
    const int motions=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"receivedPlayerMotions\":[%llu,%llu,%llu,%llu]}\n",
        receivedPlayerMotions[0],receivedPlayerMotions[1],receivedPlayerMotions[2],receivedPlayerMotions[3]);
    if(motions<=0||motions>=sizeof(text)-length+2)return;length=length-2+motions;
    const int hold=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"cinematicHoldActive\":%s,\"cinematicHoldId\":%u,\"cinematicHoldStarted\":%llu,\"cinematicHoldResumed\":%llu,\"cinematicHoldAbandoned\":%llu}\n",
        cinematicHold.id?"true":"false",cinematicHold.id,cinematicHoldStarted,cinematicHoldResumed,cinematicHoldAbandoned);
    if(hold<=0||hold>=sizeof(text)-length+2)return;length=length-2+hold;
    const int barrier=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"localSequenceTimeMs\":%u,\"localSequencePaused\":%u,\"remoteSequenceTimeMs\":%u,\"remoteSequencePaused\":%u,\"barrierEnabled\":%u,\"barrierHeld\":%u,\"remoteBarrierEnabled\":%u,\"remoteBarrierHeld\":%u,\"barrierStarted\":%llu,\"barrierReleased\":%llu,\"barrierTimedOut\":%llu,\"barrierMissed\":%llu,\"barrierSent\":%llu,\"barrierReceived\":%llu,\"barrierRejected\":%llu}\n",
        outgoing.sequenceTimeMs,outgoing.sequencePaused,remote.sequenceTimeMs,remote.sequencePaused,outgoing.barrierEnabled,outgoing.barrierHeld,remote.barrierEnabled,remote.barrierHeld,
        barrierStarted,barrierReleased,barrierTimedOut,barrierMissed,barrierSent,barrierReceived,barrierRejected);
    if(barrier<=0||barrier>=sizeof(text)-length+2)return;length=length-2+barrier;
    const int smooth=std::snprintf(text+length-2,sizeof(text)-length+2,",\"poseSmoothingEnabled\":%s,\"poseInterpolationMs\":%.1f,\"probeTargetPosition\":[%.6f,%.6f,%.6f]}\n",poseSmoothingEnabled?"true":"false",PoseInterpolation::delayMs,probeTargetPosition[0],probeTargetPosition[1],probeTargetPosition[2]);
    if(smooth<=0||smooth>=sizeof(text)-length+2)return;length=length-2+smooth;
    const int animation=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"locomotionEnabled\":%s,\"locomotionApplied\":%llu,\"locomotionRefused\":%llu,\"locomotionSpeed\":%.6f}\n",
        locomotionEnabled?"true":"false",locomotionApplied.load(),locomotionRefused.load(),locomotionAppliedSpeed.load());
    if(animation<=0||animation>=sizeof(text)-length+2)return;length=length-2+animation;
    const int body=std::snprintf(text+length-2,sizeof(text)-length+2,
        ",\"bodyProbeApplied\":%llu,\"bodyProbeRefused\":%llu,\"bodyProbeThread\":%lu,\"bodyStreamEnabled\":%s,\"bodyStreamApplied\":%llu,\"localBodyPoseValid\":%u,\"remoteBodyPoseValid\":%u,\"nativeCrouchReferenceReady\":%s,\"nativeCrouchJointCount\":%u,\"bodyCrouchHelpersApplied\":%llu,\"bodyCrouchUpright\":%s}\n",
        bodyProbeApplied.load(),bodyProbeRefused.load(),bodyProbeThread.load(),bodyStreamEnabled?"true":"false",bodyStreamApplied.load(),outgoing.bodyPoseValid,remote.bodyPoseValid,nativeCrouchReferenceReady()?"true":"false",static_cast<unsigned>(nativeCrouchReference.local.size()),bodyCrouchHelpersApplied.load(),bodyCrouchUpright?"true":"false");
    if(body<=0||body>=sizeof(text)-length+2)return;length=length-2+body;
    const int publication=std::snprintf(text+length-2,sizeof(text)-length+2,",\"bodyPublicationAddress\":\"0x%llx\"}\n",static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(&bodyPublication)));
    if(publication<=0||publication>=sizeof(text)-length+2)return;length=length-2+publication;
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
    lastTransportPump=tick;NativeServiceTimer scope("transport_all");timedNativeService("initializeSteam",[&]{initializeSteam(tick);});timedNativeService("pumpConnections",[&]{pumpConnections(tick);});timedNativeService("sampleLocalPlayer",[&]{sampleLocalPlayer();});
    if(nativeMutationAllowed){finishCinematicHold(tick);serviceCinematicBarrier(tick);}
    timedNativeService("prepareWorldObservation",[&]{prepareWorldObservation(tick);});relayConfirmedSkip(tick);timedNativeService("exchangePoses",[&]{exchangePoses(tick);});timedNativeService("checkpointService",[&]{checkpointService(tick);});timedNativeService("sendWorldObservation",[&]{sendWorldObservation(tick);});timedNativeService("sendArmStream",[&]{sendArmStream(tick);});timedNativeService("sendAvatarWeapon",[&]{sendAvatarWeapon(tick);});timedNativeService("sendShadowStream",[&]{sendShadowStream(tick);});timedNativeService("sampleNativeBlinkFx",[&]{sampleNativeBlinkFx(tick);});timedNativeService("sampleNativeGrappleFx",[&]{sampleNativeGrappleFx(tick);});
    timedNativeService("sampleNativeWindblastFx",[&]{sampleNativeWindblastFx(tick);});
    if(probeFollow&&(!coopPresentationReady(outgoing)||!coopPresentationReady(remote))) {
        if(!phaseFollowInvalidated)consoleOutput("COOP: follow phase invalidated local=%u/%u/%u/%u/%u remote=%u/%u/%u/%u/%u\n",
            outgoing.gameLogicState,outgoing.pendingGameLogicState,outgoing.flags,outgoing.cinematicMetadataValid,outgoing.cinematicNodeCount,
            remote.gameLogicState,remote.pendingGameLogicState,remote.flags,remote.cinematicMetadataValid,remote.cinematicNodeCount);
        phaseFollowInvalidated=true;
        if(!movementRetainsReadableBody(tick))networkFrameOwned.store(false,std::memory_order_release);
    }
}
void executeBuffer(void* system) {
    const DWORD thread=GetCurrentThreadId();
    DWORD unassigned=0;engineThread.compare_exchange_strong(unassigned,thread);
    if(thread==engineThread.load()) {
        // 1213q: interval between engine-thread command-buffer calls, which drive the
        // whole bridge pump; a long gap here means nothing was sent or received.
        {static double lastEngineCall=0;const double engineNow=presentationClock();if(lastEngineCall)nativeServiceTimings.add("engine_call_gap",engineNow-lastEngineCall);lastEngineCall=engineNow;}
        const auto tick=GetTickCount64();setPresentationHold(productMode||productAutoArm);
        if(tick-lastPump>=10) {
            installCheckpointCommandObservers();
            lastPump=tick;nativeMutationAllowed=true;pumpTransport(tick);nativeMutationAllowed=false;
            {NativeServiceTimer scope("native_services_all");serviceLoadingProbe(tick);finishCinematicHold(tick);serviceAiDecisionHold(tick);serviceObserverIsolation(tick);commandPump(system);serviceProductSession(system,tick);serviceMissionFollow(system,tick);nativeMutationAllowed=true;servicePowerFixture(tick);serviceDarkVisionFixture(tick);serviceWindblastFixture(tick);serviceBendTimeFixture(tick);serviceRangedFixture(tick);serviceGrenadeFixture(tick);serviceHeartFixture(tick);serviceTrapFixture(tick);servicePotionFixture(tick);serviceNoteRestoreFixture();serviceCoinFixture(tick);serviceCoinSync(tick);serviceNpcAuthorityHold(tick);timedNativeService("serviceNpcStream",[&]{serviceNpcStream(tick);});serviceNpcCombat(tick);serviceNpcKnockoutTest();serviceNpcHostWorldTest();serviceNpcAttack(tick);serviceObserverCamera(tick);serviceMesmerizeFixture(tick);serviceDominoFixture(tick);serviceSwarmFixture(tick);servicePossessionFixture(tick);serviceDoppelgangerFixture(tick);serviceShadowWalkFixture(tick);serviceClimbFixture(tick);serviceDoorFixture(tick);serviceUsableFixture(tick);timedNativeService("serviceDoorSync",[&]{serviceDoorSync(tick);});timedNativeService("serviceUsableSync",[&]{serviceUsableSync(tick);});serviceTriggerMirror(tick);serviceMissionEndMirror(tick);serviceChaosSync(tick);serviceGameflowMirror(tick);serviceLevelTransitionMirror(tick);serviceKisculeMirror(tick);serviceRangedProp(tick);serviceGrenadeProp(tick);serviceHeartProp(tick);serviceTrapProp(tick);serviceGrenadeFx(tick);sampleMesmerizeWorld(tick);presentMesmerizeWorld(tick);sampleDominoWorld(tick);presentDominoWorld(tick);samplePossessionFx(tick);presentPossessionFx(tick);servicePossessionTargetFx(tick);sampleSwarmWorld(tick);presentSwarmWorld(tick);sampleDoppelgangerWorld(tick);presentDoppelgangerWorld(tick);serviceDoppelgangerVisual(tick);serviceSwarmRatVisual(tick);serviceSwarmSpawnFx(tick);serviceSwarmTrails(tick);serviceDominoMarks(tick);serviceDominoBeams(tick);serviceMesmerizeVisual(tick);serviceAgilityFixture(tick);serviceCarryAttachmentFixture(tick);serviceShadowWalkFx(tick);serviceNativePowerFx(tick);servicePowerFxStream(tick);nativeMutationAllowed=false;checkpointEngineService(system);serviceEnemyOutcome(tick);serviceEnemyMelee(tick);serviceGuardLink(tick);serviceGuardWeapon(tick);serviceGuardEquipment(tick);serviceAvatarWeapon(tick);serviceAvatarSword(tick);serviceShadowEquipment(tick);serviceShadowStream(tick);servicePossessionBodyVisibility(tick);serviceShadowAction(tick);serviceArmProbe(tick);servicePartnerEnemy(tick);serviceSwordLink(tick);finishTransformTrace(tick);}
        }
        // Render presentation at each native engine frame, independent of
        // transport/gameplay service cadence. QPC avoids coarse tick stepping.
        {NativeServiceTimer scope("presentation_all");serviceAuthoringProof(tick);timedNativeService("serviceMovementBank",[&]{serviceMovementBank(tick);});serviceReviewClip(tick);nativeMutationAllowed=true;timedNativeService("serviceMovementResume",[&]{serviceMovementResume(tick);});nativeMutationAllowed=false;timedNativeService("followProbe",[&]{followProbe(tick);});nativeMutationAllowed=true;serviceNativeGrappleFx(tick);serviceNativeWindblastFx(tick);serviceRangedMuzzle(tick);servicePowerFailureFx(tick);timedNativeService("presentNpcStream",[&]{presentNpcStream(tick);});nativeMutationAllowed=false;}writeStatus(tick);writeNativeServiceTimings(tick);
    }
    if(thread==engineThread.load()){NativeServiceTimer scope("native_execute_buffer");realExecuteBuffer(system);}else realExecuteBuffer(system);
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
    {char product[8]{};productMode=GetEnvironmentVariableA("D2COOP_PRODUCT",product,sizeof(product))==1&&product[0]=='1';
     GetEnvironmentVariableA("D2COOP_DATA_DIR",productDataDir,sizeof(productDataDir));if(!*productDataDir)productMode=false;}
    char partnerFlag[8]{};partnerAssetsEnabled=GetEnvironmentVariableA("D2COOP_PARTNER_ASSETS",partnerFlag,sizeof(partnerFlag))==1&&partnerFlag[0]=='1';
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
    if(!installEntityActivateObserver()||!installTriggerEventObserver()||!installKisculeDispatchObserver()||!installAvatarPerception()||!installNativeSwitchTrace()||!installReplacementTrace()||!installDamageObserver()||!installMeleeDefenseObserver()||!installObserverCamera()||!installNativeHealthSubtractObserver()||!installCoinRewardObserver()||!installMesmerizePartnerFilter()||!installDominoPartnerFilter()||!installPossessionPartnerFilter()||!installAssassinationPartnerFilter()||!installInteractionPartnerFilter()||!installBodyPoseCapture()||!installMovementFootHooks())return false;
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

