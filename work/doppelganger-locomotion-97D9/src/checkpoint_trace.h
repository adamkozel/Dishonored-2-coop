#pragma once
// Native command observation and pre-teardown cleanup. Command arguments remain
// owned by the native dispatcher; wrappers forward them synchronously, unchanged.
using NativeCheckpointCommand=void(*)(void*);
NativeCheckpointCommand checkpointCommands[3]{};
struct CheckpointTraceEvent {
    std::uint64_t sequence=0,tick=0,epoch=0;
    std::uint32_t kind=0,logic=0,pending=0,thread=0;
    int lastSlot=-1,requestedSlot=-1;
    std::uintptr_t game=0;
};
CheckpointTraceEvent checkpointEvents[16]{};
std::uint64_t checkpointTraceSequence=0;
unsigned checkpointHookMask=0;
bool checkpointHookFailed=false;
void nativeCheckpointCommand(unsigned kind,void* args) {
    // A native command callback executes on the same command-buffer thread.
    // Refuse mod mutations on an unexpected thread, but preserve the game call.
    if(GetCurrentThreadId()==engineThread.load()) {
        auto& e=checkpointEvents[checkpointTraceSequence%16];e={};
        e.sequence=++checkpointTraceSequence;e.tick=GetTickCount64();e.kind=kind;
        e.thread=GetCurrentThreadId();e.epoch=outgoing.worldEpoch;
        e.logic=outgoing.gameLogicState;e.pending=outgoing.pendingGameLogicState;
        const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        std::uintptr_t vt=0;
        if(memoryRead(base+0x39a5290,&e.game,8)&&e.game&&memoryRead(e.game,&vt,8)&&vt==base+0x1c940a8)
            memoryRead(e.game+0x2e75d8+0x1c,&e.lastSlot,4);
        if(kind==2&&args) {
            int argc=0;std::uintptr_t slot=0;char token[12]{};
            if(memoryRead(reinterpret_cast<std::uintptr_t>(args),&argc,4)&&argc>=2&&argc<=3&&
               memoryRead(reinterpret_cast<std::uintptr_t>(args)+0x10,&slot,8)&&slot&&memoryRead(slot,token,sizeof(token)-1)) {
                char* end=nullptr;const long value=std::strtol(token,&end,10);
                if(end&&end!=token&&!*end&&value>=0&&value<=999)e.requestedSlot=static_cast<int>(value);
            }
        }
        if(checkpointIntercept(kind,e.requestedSlot))return;
        stopGuardLink("native_checkpoint_command");
        stopObserverIsolation("native_checkpoint_command");
        releaseAiDecisionHold("native_checkpoint_command");
        releaseAvatarAi("native_checkpoint_command");
        // Native menu reloads now receive the same frame invalidation as lab
        // loads, before the original command can tear down the current world.
        networkFrameOwned.store(false,std::memory_order_release);
        phaseFollowInvalidated=true;
    }
    checkpointCommands[kind](args);
}
void checkpointDisk(void* args){nativeCheckpointCommand(0,args);}
void checkpointMemory(void* args){nativeCheckpointCommand(1,args);}
void checkpointSlot(void* args){nativeCheckpointCommand(2,args);}
void installCheckpointCommandObservers() {
    if(checkpointHookMask==7||checkpointHookFailed)return;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t registry=0,entry=0;
    if(!memoryRead(base+0x228bae0,&registry,8)||!registry||!memoryRead(registry+8,&entry,8))return;
    const char* names[]={"RestartFromCheckpoint","RestartFromMemoryCheckpoint","LoadGame"};
    const std::uintptr_t rvas[]={0x65da50,0x65db00,0x658d00};
    NativeCheckpointCommand wrappers[]={checkpointDisk,checkpointMemory,checkpointSlot};
    std::uintptr_t visited[512]{};unsigned count=0;
    while(entry&&count<512) {
        for(unsigned i=0;i<count;++i)if(visited[i]==entry){checkpointHookFailed=true;return;}
        visited[count++]=entry;
        std::uintptr_t fields[5]{};char name[64]{};
        if(!memoryRead(entry,fields,sizeof(fields))||!fields[1]||!memoryRead(fields[1],name,sizeof(name)-1))return;
        for(unsigned i=0;i<3;++i)if(!std::strcmp(name,names[i])&&!(checkpointHookMask&(1u<<i))) {
            if(fields[2]!=base+rvas[i]){checkpointHookFailed=true;return;}
            checkpointCommands[i]=reinterpret_cast<NativeCheckpointCommand>(fields[2]);
            auto slot=reinterpret_cast<void**>(entry+16);
            // Records are native heap data. Atomic compare avoids replacing a
            // callback another component changed after validation.
            if(InterlockedCompareExchangePointer(slot,reinterpret_cast<void*>(wrappers[i]),reinterpret_cast<void*>(fields[2]))!=reinterpret_cast<void*>(fields[2])) {
                checkpointHookFailed=true;return;
            }
            checkpointHookMask|=1u<<i;
        }
        entry=fields[0];
    }
}
void writeCheckpointTrace() {
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.checkpoint.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"role\":%u,\"hookMask\":%u,\"hookFailed\":%s,\"callbacks\":%llu,\"events\":[",localRole,checkpointHookMask,checkpointHookFailed?"true":"false",checkpointTraceSequence);
    const auto first=checkpointTraceSequence>16?checkpointTraceSequence-16:0;
    for(auto i=first;i<checkpointTraceSequence;++i) {
        const auto& e=checkpointEvents[i%16];
        fprintf(f,"%s{\"sequence\":%llu,\"tick\":%llu,\"kind\":%u,\"epoch\":%llu,\"logic\":%u,\"pending\":%u,\"thread\":%u,\"lastSlot\":%d,\"requestedSlot\":%d}",
            i==first?"":",",e.sequence,e.tick,e.kind,e.epoch,e.logic,e.pending,e.thread,e.lastSlot,e.requestedSlot);
    }
    fprintf(f,"]}\n");const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
