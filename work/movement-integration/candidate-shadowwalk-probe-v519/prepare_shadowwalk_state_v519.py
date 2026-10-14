from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration'
def once(s,a,b):
    assert s.count(a)==1,a
    return s.replace(a,b)
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
s=once(s,'#include "native_player_shadowwalk_item.h"','#include "native_player_shadowwalk_item.h"\n#include "native_player_shadowwalk_state.h"')
s=once(s,'(argc==4&&std::strcmp(argv[2],"--watch-doppelganger-effects")','(argc==4&&std::strcmp(argv[2],"--watch-shadowwalk-state")&&std::strcmp(argv[2],"--watch-doppelganger-effects")')
marker='    if(argc==4&&!std::strcmp(argv[2],"--watch-doppelganger-scenes"))'
block=r'''    if((argc==4&&!std::strcmp(argv[2],"--watch-shadowwalk-state"))||(argc==3&&!std::strcmp(argv[2],"--player-shadowwalk-state"))){
        char* end=nullptr;const auto ms=argc==4?std::strtoul(argv[3],&end,10):0;
        if(argc==4&&(!end||*end||ms<100||ms>30000))return 2;
        const auto started=GetTickCount64();const auto reader=[&](auto a,auto b,auto n){return read(process.value,a,b,n);};
        do{NativeShadowWalkState s{};const bool valid=readNativeShadowWalkState(reader,base,s);
            std::cout<<"{\"readOnly\":true,\"tick\":"<<GetTickCount64()<<",\"valid\":"<<(valid?"true":"false");
            if(valid)std::cout<<",\"phase\":"<<s.phase<<",\"form\":"<<(s.form?"true":"false")<<",\"stance\":"<<s.states.player.current
                <<",\"swordState\":"<<(s.item.player.sword.equipped?2:1)<<",\"equipmentHandle\":"<<s.item.handle
                <<",\"mode\":\"0x"<<std::hex<<s.states.mode<<"\",\"component\":\"0x"<<s.item.component<<"\""<<std::dec;
            std::cout<<"}\n";if(!ms)return valid?0:69;Sleep(15);
        }while(GetTickCount64()-started<ms);return 0;
    }
'''
s=once(s,marker,block+marker);p.write_text(s,encoding='utf-8')
p=S/'world_identity_tests.cpp';s=p.read_text(encoding='utf-8-sig')
s=once(s,'#include "native_player_shadowwalk_item.h"','#include "native_player_shadowwalk_item.h"\n#include "native_player_shadowwalk_state.h"')
marker='                    require(inspectCast(cast)&&observedCast.variant==10&&observedCast.timeMs==358&&observedCast.arm.side==1,"native Dark Vision cast requires selected equipment and reciprocal left-arm binding");'
block=r'''
                    {
                        auto sw=cast;
                        sw.str(names+0x400,"models/characters/player/emily/body.def");sw.str(names+0x600,"models/characters/player/emily/right_arm.def");
                        sw.str(texts+0x100,"models/characters/player/emily/left_arm.def");
                        sw.str(strings,"models/powers/shadow_walk/shadow_walk.inventoryitem");sw.str(strings+0x100,"components/characters/player/base/powers/shadow_walk/shadow_walk.cpntpowershadowwalk");
                        sw.q(pc,base+0x1b9c248);sw.q(ped,base+0x1b9c108);sw.q(pc+0xc0,machine);sw.d(pc+0xe4,0);
                        NativeShadowWalkState observed{};
                        const auto inspect=[&](Memory& m){return readNativeShadowWalkState([&](auto a,auto b,auto n){return m.read(a,b,n);},base,observed);};
                        require(inspect(sw)&&!observed.form&&!observed.phase,"Shadow Walk exact selected native ready context");
                        for(unsigned phase:{2u,3u}){auto m=sw;m.d(pc+0xe4,phase);require(inspect(m)&&!observed.form,"native standing Shadow Walk preparation before mode switch");}
                        constexpr auto alternate=inv+0x7d000;
                        auto form=sw;form.q(machine+0x90,alternate);form.q(alternate,machine);form.q(alternate+0x10,ground);form.q(ground,base+0x1c7ed28);
                        form.d(physics+0x50,2);form.d(physics+0x54,2);
                        for(unsigned phase:{3u,4u,5u}){form.d(pc+0xe4,phase);require(inspect(form)&&observed.form&&observed.phase==phase,"native alternate Shadow Walk mode accepts observed transition/form phases");}
                        form.d(pc+0xe4,4);
                        auto bad=form;bad.q(ground,base+0x1c85628);require(!inspect(bad),"slot0 does not identify Shadow Walk without exact class");
                        bad=form;bad.q(machine+0x28,alternate);require(!inspect(bad),"Shadow Walk form cannot masquerade as default native mode");
                        bad=form;bad.q(pc+0xc0,machine+8);require(!inspect(bad),"selected power must refer to same native state machine");
                        bad=form;bad.d(pc+0xe4,1);require(!inspect(bad),"unobserved intermediate phase rejected");
                        bad=form;bad.d(physics+0x50,1);require(!inspect(bad),"sneaking does not imply active Shadow Walk form");
                        for(unsigned phase:{0u,5u}){auto recovery=form;recovery.d(pc+0xe4,phase);recovery.d(physics+0x50,1);require(inspect(recovery)&&observed.form,"native alternate mode persists briefly through crouched exit");}
                        bad=form;unsigned reads=0;require(!readNativeShadowWalkState([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==pc+0xe4&&++reads==1)bad.d(a,5);return ok;},base,observed),"native phase change during read rejected");
                        bad=form;reads=0;require(!readNativeShadowWalkState([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==pc+0xe4&&++reads==1)bad.q(machine+0x90,mode);return ok;},base,observed),"native mode retirement during read rejected");
                        std::cout<<"PASS: native Shadow Walk phase, alternate mode, exact class, owner and concurrent retirement\n";
                    }
'''
s=once(s,marker,marker+block).replace('v404 candidate inline ShadowWalk','v516 native inline ShadowWalk').replace('unowned ShadowWalk inventory does not imply a spawned clone','unowned ShadowWalk inventory does not imply active form')
p.write_text(s,encoding='utf-8')
b=(W/'Build-ShadowWalkInventory-v515.ps1').read_text(encoding='utf-8-sig')
b=b.replace('shadowwalk-inventory','shadowwalk-state').replace('v515','v519').replace('Build515','Build519')
# Only read-only probe and its ownership tests changed; do not interrupt games.
b=b[:b.index(' & "$root/work/build/Release/d2_proxy_smoke.exe"')]+" 'Read-only probe519 and native ownership tests passed'\n}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}\n"
b=b.replace('--target d2_lab_proxy d2_runtime_probe d2_world_identity_tests d2_shadow_action_tests d2_movement_bank_tests','--target d2_runtime_probe d2_world_identity_tests')
b=b.replace("@(@('d2_world_identity_tests','world-tests'),@('d2_shadow_action_tests','action-tests'))","@(@('d2_world_identity_tests','world-tests'))")
(W/'Build-ShadowWalkState-v519.ps1').write_text(b,encoding='utf-8')
print('Prepared explicit native Shadow Walk phase/mode reader, read-only probe and ownership tests; proxy behavior unchanged.')
