from pathlib import Path
R=Path(__file__).resolve().parents[2]; src=R/'outputs/d2-coop-lab/src'
p=src/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
s=s.replace('#include "native_mesmerize_world.h"','#include "native_mesmerize_world.h"\n#include "native_mesmerize_eye.h"',1)
s=s.replace('(argc==4&&std::strcmp(argv[2],"--watch-mesmerize-world")','(argc==4&&std::strcmp(argv[2],"--watch-mesmerize-eyes")&&std::strcmp(argv[2],"--watch-mesmerize-world")',1)
needle='    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-world"))'
i=s.index(needle)
block=r'''    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-eyes"))||(argc==4&&!std::strcmp(argv[2],"--watch-mesmerize-eyes"))){
        char* end=nullptr;const auto milliseconds=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||milliseconds<100||milliseconds>45000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeMesmerizeEyes s{};unsigned stage=0;const bool valid=readNativeMesmerizeEyes(reader,base,s,&stage);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage;
            if(valid){std::cout<<",\"worldPresent\":"<<(s.world.present?"true":"false")<<",\"worldHandle\":"<<s.world.handle<<",\"worldState\":"<<s.world.state<<",\"slots\":[";
                for(unsigned i=0;i<5;++i)std::cout<<(i?",":"")<<'['<<s.slots.entries[i][0]<<','<<s.slots.entries[i][1]<<']';
                std::cout<<"],\"eyes\":[";unsigned n=0;for(const auto& e:s.effects){std::cout<<(n++?",":"")<<"{\"id\":"<<e.id<<",\"owner\":"<<e.owner<<",\"record\":\"0x"<<std::hex<<e.record<<std::dec<<"\",\"origin\":[";
                    for(unsigned k=0;k<3;++k)std::cout<<(k?",":"")<<e.origin[k];std::cout<<"],\"axis\":[";for(unsigned k=0;k<9;++k)std::cout<<(k?",":"")<<e.axis[k];std::cout<<"]}";}std::cout<<']';}
            std::cout<<"}\n";if(!milliseconds)return valid?0:69;Sleep(35);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
'''
s=s[:i]+block+s[i:];p.write_text(s,encoding='utf-8')
p=src/'world_identity_tests.cpp';s=p.read_text(encoding='utf-8-sig');s='#include "native_mesmerize_eye.h"\n'+s
i=s.index('int main() {')
block=r'''void mesmerizeEyeIdentityTests(){
    Memory m;constexpr std::uintptr_t component=0x700000000,list=0x700010000;
    m.q(component+0x140,list);m.d(component+0x148,5);for(unsigned i=0;i<5;++i){m.d(list+i*8,~0u);m.d(list+i*8+4,0);}
    NativeMesmerizeEyeSlots slots{};auto inspect=[&](Memory& x){return readMesmerizeEyeSlots([&](auto a,auto b,auto n){return x.read(a,b,n);},component,slots);};
    require(inspect(m),"empty exact five-eye table readable");auto b=m;b.d(component+0x148,6);require(!inspect(b),"unknown eye table size refused");
    b=m;b.d(list+4,1);require(!inspect(b),"reference cannot point at absent effect ID");
    m.d(list,99);m.d(list+4,1);require(inspect(m),"one referenced native eye slot");b=m;b.d(list+8,99);require(!inspect(b),"duplicate eye incarnation refused");
    b=m;b.d(list+4,~0u);require(!inspect(b),"negative or unbounded reference count refused");require(inspect(m),"restore canonical eye slots");
    NativeMesmerizeWorld w{};w.present=true;w.actor=0x700020000;w.component=component;w.handle=0x880042;w.transform={1,2,3,1,0,0,0,1,0,0,0,1};
    NativeWindblastFxRecord e{};e.id=99;e.owner=w.handle;e.ownerActor=w.actor;e.parent=0x3fff0010;e.declaration=0x700030000;e.name=kNativeMesmerizeEye;e.ownerDefinition="models/powers/mesmerize/mesmerize_object.def";
    e.origin[0]=1;e.origin[1]=2;e.origin[2]=5;e.axis[0]=e.axis[4]=e.axis[8]=1;std::vector<NativeWindblastFxRecord> out;
    auto select=[&](std::vector<NativeWindblastFxRecord> r){return selectNativeMesmerizeEyes(w,slots,e.declaration,r,out);};
    require(select({e})&&out.size()==1,"exact committed world eye selected by live reference ID");
    auto wrong=e;wrong.owner++;require(!select({wrong}),"foreign effect owner refused");wrong=e;wrong.ownerActor++;require(!select({wrong}),"recycled world actor refused");
    wrong=e;wrong.id++;require(!select({wrong}),"missing referenced eye cannot be guessed by name");wrong=e;wrong.declaration++;require(!select({wrong}),"wrong eye declaration refused");
    wrong=e;wrong.name="models/effects/gameplay/powers/mesmerize_v2/link/mesmerize_v2_link_01.fx";require(!select({wrong}),"beam cannot impersonate eye");
    wrong=e;wrong.axis[0]=-1;require(!select({wrong}),"reflected eye basis refused");wrong=e;wrong.origin[0]=NAN;require(!select({wrong}),"invalid eye origin refused");
    require(!select({e,e}),"ambiguous effect incarnation refused");slots.entries[0][1]=0;require(select({e})&&out.empty(),"unreferenced native effect tail is not a live eye");
    std::cout<<"PASS: exact Mesmerize eye references, owner/declaration, finite pose, stale/foreign effects and tail exclusion\n";
}
'''
s=s[:i]+block+s[i:];s=s.replace('int main() {','int main() {\n    mesmerizeEyeIdentityTests();',1);p.write_text(s,encoding='utf-8')
