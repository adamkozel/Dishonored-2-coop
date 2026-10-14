from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
assert '#include "native_mesmerize_targets.h"' not in s
s=s.replace('#include "native_mesmerize_eye.h"','#include "native_mesmerize_eye.h"\n#include "native_mesmerize_targets.h"',1)
s=s.replace('(argc==4&&std::strcmp(argv[2],"--watch-mesmerize-eyes")','(argc==4&&std::strcmp(argv[2],"--watch-mesmerize-targets")&&std::strcmp(argv[2],"--watch-mesmerize-eyes")',1)
i=s.index('    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-eyes"))')
block=r'''    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-targets"))||(argc==4&&!std::strcmp(argv[2],"--watch-mesmerize-targets"))){
        char* end=nullptr;const auto milliseconds=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||milliseconds<100||milliseconds>45000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeMesmerizeTargets s{};unsigned stage=0;const bool valid=readNativeMesmerizeTargets(reader,base,s,&stage);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage;
            if(valid){std::cout<<",\"worldPresent\":"<<(s.world.present?"true":"false")<<",\"worldHandle\":"<<s.world.handle<<",\"worldState\":"<<s.world.state<<",\"targets\":[";
                unsigned n=0;for(const auto& t:s.targets){std::cout<<(n++?",":"")<<"{\"handle\":"<<t.key.handle<<",\"definition\":"<<jsonString(t.name)<<",\"vtable\":\"0x"<<std::hex<<t.vtable-base<<std::dec<<"\",\"anchor\":"<<t.key.anchor<<",\"active\":"<<unsigned(t.key.active)<<",\"removing\":"<<unsigned(t.key.removing)<<",\"effects\":[";
                    unsigned j=0;for(const auto& e:t.effects){std::cout<<(j++?",":"")<<"{\"kind\":"<<e.kind<<",\"id\":"<<e.fx.id<<",\"owner\":"<<e.fx.owner<<",\"name\":"<<jsonString(e.fx.name)<<",\"origin\":[";
                        for(unsigned k=0;k<3;++k)std::cout<<(k?",":"")<<e.fx.origin[k];std::cout<<"],\"axis\":[";for(unsigned k=0;k<9;++k)std::cout<<(k?",":"")<<e.fx.axis[k];std::cout<<"],\"other\":[";for(unsigned k=0;k<12;++k)std::cout<<(k?",":"")<<e.other[k];std::cout<<"]}";}std::cout<<"]}";}std::cout<<']';}
            std::cout<<"}\n";if(!milliseconds)return valid?0:70;Sleep(45);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
'''
s=s[:i]+block+s[i:];p.write_text(s,encoding='utf-8')
p=S/'world_identity_tests.cpp';s=p.read_text(encoding='utf-8-sig');s='#include "native_mesmerize_targets.h"\n'+s
i=s.index('int main() {')
block=r'''void mesmerizeTargetTests(){
    Memory m;constexpr std::uintptr_t c=0x700000000,list=c+0x1000;
    m.q(c+0x1a8,0);m.d(c+0x1b0,0);NativeMesmerizeTargetTable out{};
    auto inspect=[&](Memory& v){return readMesmerizeTargetTable([&](auto a,auto b,auto n){return v.read(a,b,n);},c,out);};
    require(inspect(m)&&out.entries.empty(),"empty native target list is explicit absence");
    std::array<unsigned char,0x70> raw{};m.put(list,raw.data(),raw.size());m.q(c+0x1a8,list);m.d(c+0x1b0,1);m.d(list,0x120014);m.d(list+0x30,55);m.d(list+0x34,~0u);m.d(list+0x38,56);m.d(list+0x60,2);unsigned char one=1;m.put(list+0x6d,&one,1);
    require(inspect(m)&&out.entries.size()==1&&out.entries[0].beam==55&&out.entries[0].head==56,"native target/effect IDs and anchor decoded");
    auto b=m;b.d(c+0x1b0,33);require(!inspect(b),"unbounded target array refused");b=m;b.d(list+0x60,5);require(!inspect(b),"unknown anchor refused");
    b=m;b.d(list+0x38,55);require(!inspect(b),"duplicate effect assigned to different target slots refused");b=m;b.d(list,0x3fff0010);require(!inspect(b),"retired target handle refused");
    b=m;b.d(c+0x1b0,2);std::array<unsigned char,0x70> copy{};m.read(list,copy.data(),copy.size());b.put(list+0x70,copy.data(),copy.size());require(!inspect(b),"duplicate target incarnation refused");
    std::array<float,12> origin={1,2,3,1,0,0,0,1,0,0,0,1},pose=origin;pose[0]+=5;
    require(mesmerizeTargetTransform(pose.data(),origin,10),"finite rigid beam endpoint within scope");pose[0]+=10;require(!mesmerizeTargetTransform(pose.data(),origin,10),"unbounded beam endpoint refused");
    pose=origin;pose[3]=-1;require(!mesmerizeTargetTransform(pose.data(),origin,10),"reflected beam basis refused");pose=origin;pose[2]=NAN;require(!mesmerizeTargetTransform(pose.data(),origin,10),"nonfinite beam endpoint refused");
    std::cout<<"PASS: read-only Mesmerize target identity bounds, duplicate/retired references and rigid beam endpoint validation\n";
}
'''
s=s[:i]+block+s[i:];s=s.replace('int main() {','int main() {\n    mesmerizeTargetTests();',1);p.write_text(s,encoding='utf-8')
print('Read-only target reader and negative tests added')
