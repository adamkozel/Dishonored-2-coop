// #30: resource container reading and writing on synthetic containers (no game files in CI).
#include "resource_container.h"
#include <cstdio>
#include <cstdlib>

static void require(bool ok,const char* what){if(!ok){std::fprintf(stderr,"FAIL: %s\n",what);std::exit(1);}}
template<class F>static bool throws(F f){try{f();}catch(const std::runtime_error&){return true;}return false;}

int main(){
    using namespace d2res;
    std::vector<Entry> entries(2);
    entries[0].id=7;entries[0].type="entityDef";entries[0].source="models/a.def";entries[0].destination="generated/decls/a.decl";
    entries[0].offset=4;entries[0].size=10;entries[0].packed=10;entries[0].flags2=28;
    entries[1].id=0x01020304;entries[1].type="File";entries[1].source="maps/b.entities";entries[1].destination="";
    entries[1].offset=0x0102030405060708ull;entries[1].size=300;entries[1].packed=120;entries[1].flags1=0xffffffffffffffffull;entries[1].flags2=25;
    const auto bytes=writeIndex(entries);
    require(bytes[0]==5&&bytes[1]=='S'&&bytes[2]=='E'&&bytes[3]=='R',"index magic");
    require(((std::uint32_t(bytes[4])<<24)|(bytes[5]<<16)|(bytes[6]<<8)|bytes[7])==bytes.size()-32,"index size field is the file size - 32");
    require(bytes[0x23]==2,"entry count");
    const auto back=readIndex(bytes);
    require(back.size()==2&&back[1].offset==entries[1].offset&&back[1].flags1==entries[1].flags1&&back[1].flags2==25&&back[0].source=="models/a.def","index round trip");
    require(writeIndex(back)==bytes,"index writes back byte for byte");
    auto bad=bytes;bad[7]^=1;require(throws([&]{readIndex(bad);}),"size field mismatch refused");
    bad=bytes;bad.push_back(0);require(throws([&]{readIndex(bad);}),"trailing byte refused");
    require(resourcesName(28)=="game1voidmod.resources"&&resourcesName(25)=="game2_patch.resources"&&resourcesName(3)=="game4.resources","resources names from flags2");

    // A master.index shaped like the game's: four bases, 24 patches, a tail.
    Master m;m.version=2;
    for(int g=1;g<=4;++g)m.bases.emplace_back("game"+std::to_string(g)+"_patch.index","game"+std::to_string(g)+".resources");
    for(const char* level:{"_001","_002","_003","_004","_005","_patch"})for(std::uint16_t g=0;g<4;++g)m.patches.emplace_back("game"+std::to_string(g+1)+level+".resources",g);
    m.tail={0,0,0,1,0,0,0,2,0,1,0,2,3,0,0,0,'a','b','c'};
    const auto original=writeMaster(m);
    require(writeMaster(readMaster(original))==original,"master round trip");
    const auto mod=readMaster(voidmodMaster(original));
    require(mod.bases[0].first=="game1voidmod.index"&&mod.bases[3].first=="game4voidmod.index"&&mod.bases[2].second=="game3.resources","voidmod bases");
    require(mod.patches.size()==28&&mod.patches[24].first=="game1voidmod.resources"&&mod.patches[27].second==3,"voidmod resources appended with their game numbers");
    require(mod.tail==m.tail,"tail kept");
    require(throws([&]{voidmodMaster(voidmodMaster(original));}),"a modded master is refused");
    std::puts("d2_resource_container_tests passed");
    return 0;
}
