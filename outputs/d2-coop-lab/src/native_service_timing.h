#pragma once
// Bounded, engine-thread-only diagnostics. Never extend a presentation lease
// or move native reads/writes to a worker. Named scopes include nested work.
struct NativeServiceTimings {
    struct Stage {const char* name=nullptr;std::uint64_t count=0,slow=0;double total=0,maximum=0,last=0;};
    struct Event {const char* name=nullptr;ULONGLONG tick=0;double ms=0;};
    std::array<Stage,128> stages{};std::array<Event,32> events{};
    unsigned used=0;std::uint64_t eventCount=0;
    void add(const char* name,double ms){
        unsigned i=0;for(;i<used;++i)if(!std::strcmp(stages[i].name,name))break;
        if(i==used){if(used==stages.size())return;stages[used++].name=name;}
        auto& s=stages[i];++s.count;s.total+=ms;s.last=ms;s.maximum=(std::max)(s.maximum,ms);
        if(ms>=50){++s.slow;events[eventCount++%events.size()]={name,GetTickCount64(),ms};}
    }
} nativeServiceTimings;
struct NativeServiceTimer {
    const char* name;double start;
    explicit NativeServiceTimer(const char* n):name(n),start(presentationClock()){}
    ~NativeServiceTimer(){nativeServiceTimings.add(name,presentationClock()-start);}
};
template<class F> void timedNativeService(const char* name,F fn){NativeServiceTimer timer(name);fn();}
void writeNativeServiceTimings(ULONGLONG tick){
    static ULONGLONG last=0;if(!*statusPath||tick-last<1000)return;last=tick;
    char path[MAX_PATH]{},tmp[MAX_PATH]{};
    std::snprintf(path,sizeof(path),"%s.native-service-timing.json",statusPath);
    std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"tick\":%llu,\"logic\":%u,\"eventCount\":%llu,\"stages\":[",tick,outgoing.gameLogicState,nativeServiceTimings.eventCount);
    for(unsigned i=0;i<nativeServiceTimings.used;++i){const auto& s=nativeServiceTimings.stages[i];
        fprintf(f,"%s{\"name\":\"%s\",\"count\":%llu,\"slow\":%llu,\"totalMs\":%.3f,\"maximumMs\":%.3f,\"lastMs\":%.3f}",i?",":"",s.name,s.count,s.slow,s.total,s.maximum,s.last);}
    fprintf(f,"],\"events\":[");const auto n=(std::min)(nativeServiceTimings.eventCount,std::uint64_t(nativeServiceTimings.events.size()));
    for(std::uint64_t i=0;i<n;++i){const auto& e=nativeServiceTimings.events[(nativeServiceTimings.eventCount-n+i)%nativeServiceTimings.events.size()];
        fprintf(f,"%s{\"name\":\"%s\",\"tick\":%llu,\"ms\":%.3f}",i?",":"",e.name,e.tick,e.ms);}
    fprintf(f,"],\"scope\":\"Read-only engine-thread scope timing; nested durations overlap; no lease or gameplay changes\"}\n");
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
