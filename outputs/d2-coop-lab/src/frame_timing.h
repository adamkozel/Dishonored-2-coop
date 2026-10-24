#pragma once
// 1214jx (#195): frame pacing, to attribute stutters. One engine frame runs one engine-thread command-buffer call, so the
// interval between those calls is the frame time. For the last 3600 frames this keeps the interval, the mod's own time
// in that frame (the bridge's work before native ExecuteBuffer) and the frame's most expensive named mod scope
// (nativeServiceTimings, any hook); an interval of 25 ms or more keeps its attribution as a spike. Engine thread only.
struct FrameTiming {
    static constexpr unsigned kFrames=3600,kSpikes=32;
    struct Spike {ULONGLONG tick=0;float gapMs=0,modMs=0,topMs=0;const char* top=nullptr;};
    std::array<float,kFrames> gap{},mod{};std::array<Spike,kSpikes> spikes{};
    std::uint64_t frames=0,spikes25=0,spikes50=0,spikes100=0,spikeCount=0;
    double lastEntry=0,entry=0,modTotal=0;float lastModMs=0;
} frameTiming;
// At the command-buffer call: close the previous frame (its interval, its mod time, its top scope), open this one.
void frameTimingBegin(){
    auto& f=frameTiming;auto& t=nativeServiceTimings;const double now=presentationClock();
    if(f.lastEntry){
        const float g=static_cast<float>(now-f.lastEntry);const auto i=static_cast<unsigned>(f.frames%FrameTiming::kFrames);
        f.gap[i]=g;f.mod[i]=f.lastModMs;++f.frames;f.modTotal+=f.lastModMs;
        if(g>=25){++f.spikes25;f.spikes[f.spikeCount++%FrameTiming::kSpikes]={GetTickCount64(),g,f.lastModMs,static_cast<float>(t.frameTopMs),t.frameTop};}
        if(g>=50)++f.spikes50;if(g>=100)++f.spikes100;
    }
    f.lastEntry=f.entry=now;f.lastModMs=0;t.frameTop=nullptr;t.frameTopMs=0;
}
// Just before native ExecuteBuffer: the mod's own share of this frame.
void frameTimingEnd(){auto& f=frameTiming;f.lastModMs=static_cast<float>(presentationClock()-f.entry);}
void writeFrameTiming(ULONGLONG tick){
    static ULONGLONG last=0;if(!*statusPath||tick-last<1000)return;last=tick;
    const auto& f=frameTiming;const auto n=static_cast<unsigned>((std::min)(f.frames,static_cast<std::uint64_t>(FrameTiming::kFrames)));
    std::vector<float> gaps(f.gap.begin(),f.gap.begin()+n),mods(f.mod.begin(),f.mod.begin()+n);
    const auto pct=[](std::vector<float> v,double q){if(v.empty())return 0.f;const auto k=static_cast<std::size_t>(q*(v.size()-1));std::nth_element(v.begin(),v.begin()+k,v.end());return v[k];};
    double span=0;unsigned over25=0,over50=0;const float edges[]={8.4f,11.2f,16.8f,20.f,25.f,33.4f,50.f,100.f};std::array<unsigned,9> hist{};
    for(float g:gaps){span+=g;if(g>=25)++over25;if(g>=50)++over50;unsigned b=0;while(b<8&&g>=edges[b])++b;++hist[b];}
    const double minutes=span/60000.0;
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.frame-timing.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* out=nullptr;if(fopen_s(&out,tmp,"wb")||!out)return;
    std::fprintf(out,"{\"diagnostics\":%s,\"tick\":%llu,\"logic\":%u,\"map\":\"%s\",\"frames\":%llu,\"spikes25\":%llu,\"spikes50\":%llu,\"spikes100\":%llu,\"modTotalMs\":%.1f,"
        "\"window\":{\"frames\":%u,\"seconds\":%.1f,\"p50\":%.2f,\"p90\":%.2f,\"p99\":%.2f,\"max\":%.2f,\"spikes25PerMinute\":%.2f,\"spikes50PerMinute\":%.2f,"
        "\"modP50\":%.3f,\"modP99\":%.3f,\"modMax\":%.3f,\"histogram\":[",
        coopDiagnostics?"true":"false",tick,outgoing.gameLogicState,mapName,f.frames,f.spikes25,f.spikes50,f.spikes100,f.modTotal,n,span/1000.0,pct(gaps,.5),pct(gaps,.9),pct(gaps,.99),
        gaps.empty()?0.f:*std::max_element(gaps.begin(),gaps.end()),minutes>0?over25/minutes:0.0,minutes>0?over50/minutes:0.0,pct(mods,.5),pct(mods,.99),
        mods.empty()?0.f:*std::max_element(mods.begin(),mods.end()));
    for(unsigned b=0;b<9;++b)std::fprintf(out,"%s%u",b?",":"",hist[b]);
    std::fputs("],\"histogramEdgesMs\":[8.4,11.2,16.8,20,25,33.4,50,100]},\"spikes\":[",out);
    const auto s=(std::min)(f.spikeCount,static_cast<std::uint64_t>(FrameTiming::kSpikes));
    for(std::uint64_t i=0;i<s;++i){const auto& e=f.spikes[(f.spikeCount-s+i)%FrameTiming::kSpikes];
        std::fprintf(out,"%s{\"tick\":%llu,\"gapMs\":%.2f,\"modMs\":%.3f,\"top\":\"%s\",\"topMs\":%.3f}",i?",":"",e.tick,e.gapMs,e.modMs,e.top?e.top:"",e.topMs);}
    std::fputs("]}\n",out);const bool ok=!ferror(out);const int closed=fclose(out);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
