from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
p=S/'runtime_probe.cpp';s=p.read_text().replace('#include "native_player_mesmerize_item.h"','#include "native_player_mesmerize_item.h"\n#include "native_mesmerize_world.h"')
s=s.replace('(argc==4&&std::strcmp(argv[2],"--watch-power-scenes")','(argc==4&&std::strcmp(argv[2],"--watch-mesmerize-world")&&std::strcmp(argv[2],"--watch-power-scenes")',1)
marker='    if(argc==3&&!std::strcmp(argv[2],"--player-mesmerize"))'
assert marker in s
s=s.replace(marker,r'''    if((argc==3&&!std::strcmp(argv[2],"--mesmerize-world"))||(argc==4&&!std::strcmp(argv[2],"--watch-mesmerize-world"))){
        char* end=nullptr;const auto milliseconds=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||milliseconds<100||milliseconds>45000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeMesmerizeWorld w{};const bool valid=readNativeMesmerizeWorld(reader,base,w);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid){std::cout<<",\"present\":"<<(w.present?"true":"false")<<",\"handle\":"<<w.handle<<",\"state\":"<<w.state<<",\"component\":\"0x"<<std::hex<<w.component<<std::dec
                <<"\",\"equipmentHandle\":"<<w.equipment.handle<<",\"equipmentSelected\":"<<(w.equipment.equipped?"true":"false")<<",\"quantity\":"<<w.equipment.quantity<<",\"transform\":[";
                for(unsigned i=0;i<12;++i)std::cout<<(i?",":"")<<w.transform[i];std::cout<<']';}
            std::cout<<"}\n";if(!milliseconds)return valid?0:69;Sleep(25);
        }while(GetTickCount64()-started<milliseconds);return 0;
    }
'''+marker,1);p.write_text(s)
p=S/'world_identity_tests.cpp';s=p.read_text().replace('#include "native_player_mesmerize_item.h"','#include "native_player_mesmerize_item.h"\n#include "native_mesmerize_world.h"')
marker='                        std::cout<<"PASS: Mesmerize native item, Emily scope, ownership, quantity, stale handle and concurrent retirement guards\\n";'
assert marker in s
s=s.replace(marker,r'''                        {
                            auto world=mesmer;world.d(pc+0xd4,0x3fff0010);
                            NativeMesmerizeWorld result{};const auto inspectWorld=[&](Memory& m){return readNativeMesmerizeWorld([&](auto a,auto d,auto n){return m.read(a,d,n);},base,result);};
                            require(inspectWorld(world)&&!result.present,"equipment sentinel is absence, not permission to search preview entities");
                            constexpr auto wa=inv+0x53000,wd=inv+0x54000,wc=inv+0x55000,wl=inv+0x56000,wcd=inv+0x57000,wn=inv+0x58000;
                            constexpr unsigned wh=0x70008;
                            world.q(game+0x3130b0+7*8,wa);world.q(wa,base+0x1c87d58);world.d(wa+0xa8,wh);world.d(wa+0xac,0x3fff0010);
                            world.q(wa+0x2e8,wd);world.q(wd,base+0x1ab58f8);world.q(wd+8,wn);world.str(wn,"models/powers/mesmerize/mesmerize_object.def");
                            world.q(wa+0x268,wl);world.d(wa+0x270,1);world.q(wl,wc);world.q(wc,base+0x1c53958);world.q(wc+8,wcd);world.q(wc+0x10,wa);world.q(wc+0x20,wc);world.d(wc+0x1c,0x2000000a);world.d(wc+0x2c,1);
                            world.q(wcd,base+0x1c53a38);world.q(wcd+8,wn+0x100);world.str(wn+0x100,"components/characters/player/base/powers/mesmerize/mesmerize_object.cpntmesmerize");
                            const float tr[12]={1,2,3,1,0,0,0,1,0,0,0,1};world.put(wa+0x300,tr,sizeof(tr));
                            require(inspectWorld(world)&&!result.present,"existing preview is ignored until native equipment commits its handle");
                            world.d(pc+0xd4,wh);require(inspectWorld(world)&&result.present&&result.handle==wh&&result.state==1,"exact committed world relationship resolves independent native component");
                            auto b=world;b.q(wc+0x10,actor);require(!inspectWorld(b),"world component with foreign owner refused");
                            b=world;b.d(wa+0xa8,wh+0x10000);require(!inspectWorld(b),"recycled committed world incarnation refused");
                            b=world;b.q(wcd,base+0x1b9bd68);require(!inspectWorld(b),"equipment declaration cannot replace world declaration");
                            b=world;b.d(wa+0x270,2);b.q(wl+8,wc);require(!inspectWorld(b),"duplicate world component refused");
                            b=world;b.d(wc+0x2c,5);require(!inspectWorld(b),"unknown world FSM state refused");
                            b=world;b.d(wa+0xac,1);require(!inspectWorld(b),"unobserved parented world entity refused");
                            b=world;b.q(wc+0x20,actor);require(!inspectWorld(b),"invalid state-machine self pointer refused");
                            b=world;b.d(wa+0x30c,0);require(!inspectWorld(b),"degenerate world orientation refused");
                            b=world;unsigned count=0;require(!readNativeMesmerizeWorld([&](auto a,auto d,auto n){const bool ok=b.read(a,d,n);if(a==pc+0xd4&&++count==1)b.d(a,0x3fff0010);return ok;},base,result),"concurrent world relationship retirement refused");
                            b=world;count=0;require(!readNativeMesmerizeWorld([&](auto a,auto d,auto n){const bool ok=b.read(a,d,n);if(a==wc+0x2c&&++count==1)b.d(a,2);return ok;},base,result),"concurrent world FSM transition refused");
                            std::cout<<"PASS: Mesmerize committed world ownership, ignored preview, exact component, state, transform and retirement guards\n";
                        }
'''+marker,1);p.write_text(s)
