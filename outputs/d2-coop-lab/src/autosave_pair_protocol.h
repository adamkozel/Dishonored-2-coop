#pragma once
#include <cstdint>
// 1153 paired-autosave wire format and settle rule (pure; unit-tested).
#pragma pack(push,1)
struct AutosavePairPacket {
    std::uint32_t magic=0x50413244; // "D2AP"
    std::uint16_t version=1,size=sizeof(AutosavePairPacket);
    std::uint32_t role=0;
    std::uint64_t session=0,arm=0,epoch=0,mapId=0;
    std::uint32_t saves=0,originated=0,mirrored=0,pending=0;
};
#pragma pack(pop)
static_assert(sizeof(AutosavePairPacket)==60);
inline bool validAutosavePair(const AutosavePairPacket& p,unsigned receiver){
    return p.magic==0x50413244&&p.version==1&&p.size==sizeof(p)&&p.role<=1&&p.role!=receiver&&p.session&&p.arm&&p.epoch&&p.mapId&&
        p.pending<=1&&p.mirrored<=p.saves&&p.originated<=p.saves&&p.saves<100000;
}
// Settled: both peers report the same autosave count, each has mirrored every
// save the other originated, and no native mirror request is in flight.
inline bool autosavePairSettled(unsigned saves,unsigned originated,unsigned mirrored,bool pending,
                                unsigned peerSaves,unsigned peerOriginated,unsigned peerMirrored,bool peerPending){
    return !pending&&!peerPending&&saves==peerSaves&&mirrored==peerOriginated&&peerMirrored==originated;
}
