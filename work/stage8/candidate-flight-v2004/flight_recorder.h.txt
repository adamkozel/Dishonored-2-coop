#pragma once
// Stage 8.2 (#235): the flight recorder, coop-anything's cofr (vendored, MIT, third_party/coop-anything/flight/cpp).
// Every datagram this game sends or receives goes into an 8 MB ring (the first 256 bytes of each; the first 4 are the
// packet magic). `coop_flight dump` writes <status folder>/d2coop-<role>-<unix ms>.cofr, which
// third_party/coop-anything/flight/analyze.py lines up with the other machine's dump to find the first divergence.
// Peers are slots (host 0, partner 1), never SteamIDs. `coop_flight off|on` stops or resumes recording.
#include <memory>
#include <mutex>
#include <string>
#include "cofr.h"

inline cofr::Recorder* flightRecorder(){
    static std::unique_ptr<cofr::Recorder> recorder;static std::once_flag once;
    std::call_once(once,[]{
        cofr::Config c;c.ringBytes=8u<<20;c.maxRawBytes=256;c.label=localRole?"d2coop-partner":"d2coop-host";
        recorder=std::make_unique<cofr::Recorder>(c);
        recorder->setIdentity(localRole?1:0,localRole?cofr::kRoleClient:cofr::kRoleHost);
        recorder->mapPeer(0,localRole?0:1);
    });
    return recorder.get();
}
inline void flightOut(const void* data,std::uint32_t size,int flags){
    if(data&&size)flightRecorder()->netOut(0,static_cast<const std::uint8_t*>(data),size,(flags&k_nSteamNetworkingSend_Reliable)?1:0);
}
inline void flightIn(const void* data,std::uint32_t size){
    if(data&&size)flightRecorder()->netIn(0,static_cast<const std::uint8_t*>(data),size);
}
inline std::string flightDump(){
    std::string dir=statusPath;const auto cut=dir.find_last_of("\\/");if(cut==std::string::npos)return {};
    char name[96]{};std::snprintf(name,sizeof(name),"\\%s-%llu.cofr",localRole?"d2coop-partner":"d2coop-host",
        static_cast<unsigned long long>(GetTickCount64()));
    const auto path=dir.substr(0,cut)+name;
    return flightRecorder()->dump(path,cofr::kDumpManual)?path:std::string();
}
