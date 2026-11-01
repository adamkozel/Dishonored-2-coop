#pragma once
// Stage 8.4.3 (#239): one Jindosh lock answer for both players (The Dust District). The answer is five Kiscule ListInt
// variables of the lock's jindosh_enigma.kiscule instance (RandomFamily, RandomColor, RandomHeirloom, RandomBeverage,
// RandomCity), each [-1, x1..x5]: a shuffle of the ids 1..5 by seat, rolled once per world on the map's first load
// (confirmed static; RandomFamily [-1,1,4,3,2,5] in take route-dd-v1306a, confirmed runtime). The riddle and the
// solution notes are journal int lists formatted from them (jindosh-notes, confirmed static): list 15 (the riddle, 25
// ints) holds, for seat s, var 5(s-1)+k = list k[s]-1; lists 14, 19, 20, 21 and 25 (10 ints each) hold var 2(s-1) =
// RandomFamily[s]-1 and var 2(s-1)+1 = RandomHeirloom[s]-1. Every value is 0..4.
#include <cstdint>
#include <vector>
constexpr std::uint32_t kJindoshMagic=0x4c4a3244; // "D2JL"
constexpr unsigned kJindoshLists=5,kJindoshListLength=6,kJindoshSeats=5;
constexpr const char* kJindoshListNames[kJindoshLists]={"RandomFamily","RandomColor","RandomHeirloom","RandomBeverage","RandomCity"};
// The journal decl indices of the Jindosh texts (journal.cpntplayerjournal m_formatedStringList; confirmed static).
constexpr unsigned kJindoshJournalIndex[6]={15,14,19,20,21,25};
#pragma pack(push,1)
struct JindoshPacket {
    std::uint32_t magic=kJindoshMagic;std::uint16_t version=1,size=sizeof(JindoshPacket);
    std::uint32_t role=0;std::uint64_t session=0,sequence=0,mapId=0,epoch=0;
    std::int32_t lists[kJindoshLists][kJindoshListLength]{};
};
#pragma pack(pop)
// A rolled answer: each list is -1 followed by a permutation of 1..5.
inline bool validJindoshLists(const std::int32_t (&l)[kJindoshLists][kJindoshListLength]){
    for(unsigned k=0;k<kJindoshLists;++k){if(l[k][0]!=-1)return false;unsigned seen=0;
        for(unsigned s=1;s<kJindoshListLength;++s){const auto v=l[k][s];if(v<1||v>5||(seen&(1u<<v)))return false;seen|=1u<<v;}}
    return true;
}
inline bool validJindosh(const JindoshPacket& p){
    return p.magic==kJindoshMagic&&p.version==1&&p.size==sizeof(p)&&p.role==0&&p.sequence&&validJindoshLists(p.lists);
}
// The values a journal list holds for this answer: 25 for the riddle (index 15), 10 for the solution texts.
inline unsigned jindoshJournalCount(unsigned index){return index==15?25u:10u;}
inline void jindoshJournalValues(const std::int32_t (&l)[kJindoshLists][kJindoshListLength],unsigned index,std::vector<std::int32_t>& out){
    out.clear();
    for(unsigned s=1;s<=kJindoshSeats;++s){
        if(index==15){for(unsigned k=0;k<kJindoshLists;++k)out.push_back(l[k][s]-1);}
        else{out.push_back(l[0][s]-1);out.push_back(l[2][s]-1);}}
}
