from pathlib import Path
R=Path(__file__).resolve().parents[2]; S=R/'outputs/d2-coop-lab/src'
p=S/'world_identity_tests.cpp';t=p.read_text()
old='#include "native_player_swarm_item.h"';assert t.count(old)==1
t=t.replace(old,old+'\n#include "native_swarm_world.h"')
old='#include "native_domino_targets_tests.h"';assert t.count(old)==1
t=t.replace(old,old+'\n#include "native_swarm_world_tests.h"')
old='require(inspectSwarm(inlineSwarm)&&item.parentDeclaration==swarmParent,"candidate inline Swarm declaration requires its exact named parent");';assert t.count(old)==1
t=t.replace(old,old+'\n                            swarmWorldTests(inlineSwarm,base,game,pc);');p.write_text(t)
p=S/'runtime_probe.cpp';t=p.read_text()
old='#include "native_player_swarm_item.h"'
if old not in t:old='#include "native_player_swarm_scene.h"'
assert t.count(old)==1;t=t.replace(old,old+'\n#include "native_swarm_world.h"')
old='(argc==4&&std::strcmp(argv[2],"--watch-swarm-scenes")';assert t.count(old)==1
t=t.replace(old,'(argc==4&&std::strcmp(argv[2],"--watch-swarm-world")&&std::strcmp(argv[2],"--watch-swarm-scenes")')
old='    if(argc==4&&!std::strcmp(argv[2],"--watch-swarm-scenes")){' ;assert t.count(old)==1
new=r'''    if((argc==3&&!std::strcmp(argv[2],"--swarm-world"))||(argc==4&&!std::strcmp(argv[2],"--watch-swarm-world"))){
        unsigned ms=0;if(argc==4){char* end=nullptr;ms=std::strtoul(argv[3],&end,10);if(!end||*end||ms<100||ms>45000)return 2;}
        const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};const auto started=GetTickCount64();
        std::cout.precision(9);
        do{NativeSwarmWorld w{};unsigned stage=0;LARGE_INTEGER begin{},end{},frequency{};QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&begin);
            const bool valid=readNativeSwarmWorld(reader,base,w,&stage);QueryPerformanceCounter(&end);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false")<<",\"stage\":"<<stage
                <<",\"readMs\":"<<(end.QuadPart-begin.QuadPart)*1000.0/frequency.QuadPart;
            if(valid){std::cout<<",\"present\":"<<(w.present?"true":"false")<<",\"handle\":"<<w.handle<<",\"capacity\":"<<w.capacity
                <<",\"component\":\"0x"<<std::hex<<w.component<<"\",\"pool\":\"0x"<<w.pool<<"\",\"pipeline\":\"0x"<<w.pipeline<<"\""<<std::dec<<",\"agents\":[";
                bool first=true;for(const auto& a:w.agents){if(!first)std::cout<<',';first=false;
                    std::cout<<"{\"index\":"<<a.index<<",\"handle\":"<<a.handle<<",\"incarnation\":"<<a.incarnation<<",\"state\":"<<a.state
                        <<",\"frameGeneration\":"<<a.frameGeneration<<",\"animationGeneration\":"<<a.animationGeneration<<",\"scale\":"<<a.scale<<",\"transform\":[";
                    for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<a.transform[i];std::cout<<"]}";
                }std::cout<<']';}
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<ms);return 0;
    }
'''
t=t.replace(old,new+old);p.write_text(t)
print('Prepared isolated read-only Swarm world probe and identity tests; proxy source unchanged.')
