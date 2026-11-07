#pragma once
// 2125 (#266): lab command coop_anim_sample <ms> samples the partner avatar's root and feet in the engine frame, every
// frame, for <ms>. The external probe (d2_runtime_probe --anim-sample) reads at about 21 Hz from outside the process,
// with irregular gaps, and reads the root and the pose separately. A contact was then two or three samples, and one
// misread sample decided the skate p95 (animquality-eotw-v2122: a "planted" toe moved 0.305 m in 94 ms, and the twin
// with the start phase off passed while a take with it on failed). The lines have the probe's format, so
// tools/diag/d2_anim_quality.py reads them unchanged; they are written to <status>.anim-sample.jsonl when the time is up.
struct AnimSampleState {ULONGLONG startedAt=0,until=0;unsigned frames=0,failed=0,writes=0;std::string lines;const char* result="idle";};
AnimSampleState animSample{};
inline bool beginAnimSample(unsigned long ms){
    if(!ms||ms>120000)return false;auto& a=animSample;
    a.startedAt=GetTickCount64();a.until=a.startedAt+ms;a.frames=a.failed=0;a.lines.clear();a.lines.reserve(512*ms/16);a.result="sampling";return true;}
void writeAnimSample(){
    auto& a=animSample;char path[MAX_PATH]{},tmp[MAX_PATH]{};
    std::snprintf(path,sizeof(path),"%s.anim-sample.jsonl",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f){a.result="write_failed";return;}
    fwrite(a.lines.data(),1,a.lines.size(),f);
    fprintf(f,"{\"done\":true,\"samples\":%u,\"failed\":%u,\"source\":\"engine_frame\"}\n",a.frames,a.failed);
    const bool ok=!ferror(f);const int closed=fclose(f);
    if(ok&&!closed&&MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING)){++a.writes;a.result="written";}else a.result="write_failed";
    a.lines.clear();a.lines.shrink_to_fit();}
void serviceAnimSample(ULONGLONG tick){
    auto& a=animSample;if(!a.until)return;
    if(tick>=a.until){a.until=0;writeAnimSample();return;}
    NativeSkeleton s{};
    if(!probeHandle||!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s)||s.globalPose.size()<=25||s.parents.size()!=s.globalPose.size()){++a.failed;return;}
    // World point = root[0..2] + sum_c root[3+c*3+r] * globalPose[j][12+c], as the probe computes it.
    const auto world=[&](int j,float* w){for(unsigned r=0;r<3;++r){w[r]=s.root[r];for(unsigned c=0;c<3;++c)w[r]+=s.root[3+c*3+r]*s.globalPose[j][12+c];}};
    char line[1400]{};int n=std::snprintf(line,sizeof(line),"{\"ms\":%llu,\"root\":[%.6f,%.6f,%.6f],\"yaw\":%.6f,\"feet\":[",
        static_cast<unsigned long long>(tick-a.startedAt),s.root[0],s.root[1],s.root[2],std::atan2(s.root[4],s.root[3])*57.29578f);
    for(unsigned f=0;f<2&&n>0&&n<int(sizeof(line));++f){const int ankle=f?25:21;int toe=-1;for(int j=0;j<int(s.parents.size());++j)if(s.parents[j]==ankle){toe=j;break;}
        const int chain[4]={toe,ankle,s.parents[ankle],s.parents[ankle]>=0?s.parents[s.parents[ankle]]:-1};
        n+=std::snprintf(line+n,sizeof(line)-n,"%s{\"ankle\":%d,\"points\":[",f?",":"",ankle);
        for(unsigned k=0;k<4&&n<int(sizeof(line));++k){float w[3]{};if(chain[k]>=0&&chain[k]<int(s.globalPose.size()))world(chain[k],w);
            n+=std::snprintf(line+n,sizeof(line)-n,"%s{\"joint\":%d,\"at\":[%.6f,%.6f,%.6f]}",k?",":"",chain[k],w[0],w[1],w[2]);}
        if(n<int(sizeof(line)))n+=std::snprintf(line+n,sizeof(line)-n,"]}");}
    // 2136 (#271): the head, nose, both arms and wrists, for the held-pistol facing (head 12 to nose 92 against the
    // gun arm, left arm 7 to wrist 9). The shadow rig's indices, the same for both characters.
    if(n>0&&n<int(sizeof(line)))n+=std::snprintf(line+n,sizeof(line)-n,"],\"upper\":[");
    {const int upper[6]={12,92,7,9,14,16};for(unsigned k=0;k<6&&n>0&&n<int(sizeof(line))-80;++k){float w[3]{};if(upper[k]<int(s.globalPose.size()))world(upper[k],w);
        n+=std::snprintf(line+n,sizeof(line)-n,"%s{\"joint\":%d,\"at\":[%.6f,%.6f,%.6f]}",k?",":"",upper[k],w[0],w[1],w[2]);}}
    if(n<=0||n>=int(sizeof(line))-24){++a.failed;return;}
    n+=std::snprintf(line+n,sizeof(line)-n,"],\"joints\":%zu}\n",s.globalPose.size());
    a.lines.append(line,n);++a.frames;}
