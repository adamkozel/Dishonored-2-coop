from pathlib import Path
S=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
p=S/'world_identity_tests.cpp';s=p.read_text(encoding='utf-8-sig')
assert '#include "native_domino_targets.h"' not in s
s=s.replace('#include "native_player_domino_item.h"','#include "native_player_domino_item.h"\n#include "native_domino_targets.h"',1)
s=s.replace('#include "native_crossbow_presentation_tests.h"','#include "native_domino_targets_tests.h"\n#include "native_crossbow_presentation_tests.h"',1)
needle='                            auto unowned=domino;'
assert needle in s;s=s.replace(needle,'                            dominoTargetsTests(inlineDomino,base,game,pc);\n'+needle,1);p.write_text(s,encoding='utf-8')
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig');assert '#include "native_domino_targets.h"' not in s
s=s.replace('#include "native_player_domino_item.h"','#include "native_player_domino_item.h"\n#include "native_domino_targets.h"',1)
needle='(argc==4&&std::strcmp(argv[2],"--watch-domino-scenes")';assert needle in s
s=s.replace(needle,'(argc==4&&std::strcmp(argv[2],"--watch-domino-targets")&&std::strcmp(argv[2],"--watch-domino-scenes")',1)
needle='    if(argc==4&&!std::strcmp(argv[2],"--watch-domino-scenes"))'
assert needle in s
block=r'''    if((argc==3&&!std::strcmp(argv[2],"--domino-targets"))||(argc==4&&!std::strcmp(argv[2],"--watch-domino-targets"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>18000)return 2;}
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};const auto started=GetTickCount64();
        do{NativeDominoTargets s{};unsigned stage=0;LARGE_INTEGER begin{},end{},frequency{};QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&begin);
            const bool valid=readNativeDominoTargets(reader,base,s,&stage);QueryPerformanceCounter(&end);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage
                <<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/frequency.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(s.present?"true":"false")<<",\"networkHandle\":"<<s.handle<<",\"component\":\"0x"<<std::hex<<s.component<<"\",\"targets\":["<<std::dec;
                bool first=true;for(const auto& t:s.targets){if(!first)std::cout<<',';first=false;
                    std::cout<<"{\"handle\":"<<t.key.handle<<",\"definition\":"<<jsonString(t.name)<<",\"generation\":"<<t.key.generation<<",\"hasMark\":"<<(t.hasMark?"true":"false")
                        <<",\"mark\":"<<t.key.mark<<",\"markOwner\":"<<t.key.markOwner<<",\"node\":\"0x"<<std::hex<<t.key.node<<"\",\"actor\":\"0x"<<t.actor<<"\",\"animation\":\"0x"<<t.key.animation<<"\",\"position\":["<<std::dec;
                    for(unsigned i=0;i<3;++i){if(i)std::cout<<',';std::cout<<t.transform[i];}std::cout<<"]";
                    if(t.hasMark){std::cout<<",\"effectName\":"<<jsonString(t.mark.name)<<",\"effectPosition\":[";for(unsigned i=0;i<3;++i){if(i)std::cout<<',';std::cout<<t.mark.origin[i];}std::cout<<"]";}
                    std::cout<<'}';}std::cout<<']';}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<ms);return 0;
    }
'''
s=s.replace(needle,block+needle,1);p.write_text(s,encoding='utf-8')
print('Domino target probe and tests added; no production DLL stream changes')
