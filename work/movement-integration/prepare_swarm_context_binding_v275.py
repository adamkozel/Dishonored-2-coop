"""Extend only after all three source context receipts finish their cleanup."""
from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset'
for take,low,empty in [('swarm-native-crouched-observe-v271',1,False),('swarm-native-standing-holstered-observe-v273',0,True),('swarm-native-crouched-holstered-observe-v274',1,True)]:
 r=json.loads((P/take/'result.json').read_text(encoding='utf-8-sig'));assert r['passed']
 rows=json.loads((P/take/'states.json').read_text(encoding='utf-8-sig'));assert all(x['player']['stance']==low and x['sword']['equipped']!=empty for x in rows)
def edit(name,a,b):
 p=S/name;s=p.read_text();assert a in s,(name,a);p.write_text(s.replace(a,b))
edit('native_player_swarm_scene.h','states.player.current!=0','states.player.current>1')
edit('native_player_swarm_scene.h','||!item.player.sword.equipped','')
edit('native_player_swarm_scene.h','if(a.id==0&&a.vtable==base+0x1c85628)++ground;', 'if((states.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(states.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;')
edit('native_player_swarm_scene.h','// v266: exact native standing, armed Corvo input. Body and left arm are','// v266 standing armed, v271 crouched armed, v273/v274 holstered standing/crouch.\n// Exact Corvo input; stance must match Running0 or Sneaking6. Body and left arm are')
edit('native_player_swarm_scene.h','// continues. Entry/aim still require exact UsePower63, and release cannot','// continues (Sneaking6 when crouched). Entry/aim still require exact UsePower63, and release cannot')
edit('shadow_action_protocol.h','version=46','version=47');edit('shadow_action_protocol.h','p.version==46','p.version==47')
edit('shadow_action_protocol.h','swordState==1&&variant>=10&&variant<=22','swordState==1&&variant>=10&&variant<=25')
edit('movement_bank.h','32+304*','32+313*')
edit('movement_bank.h','Joint sampleSwarm(unsigned variant,unsigned joint,float phase)const{return sampleParameter(301+variant-23,joint,phase);}', '''bool hasSwarmContexts()const{return version>=50;}
    Joint sampleSwarm(unsigned variant,unsigned joint,float phase,float crouched=0,float armed=1)const{
        const unsigned v=variant-23;
        if(!hasSwarmContexts())return sampleParameter(301+v,joint,phase);
        const auto standing=blend(sampleParameter(304+v*3,joint,phase),sampleParameter(301+v,joint,phase),armed);
        const auto low=blend(sampleParameter(306+v*3,joint,phase),sampleParameter(305+v*3,joint,phase),armed);
        return blend(standing,low,crouched);
    }''')
edit('movement_bank.h','289,301,304};return v<=49','289,301,304,313};return v<=50');edit('movement_bank.h','v<=49;++v','v<=50;++v')
edit('movement_playback.h','p.bank.hasSwarm()&&ground&&p.presentedDrawn','p.bank.hasSwarm()&&(p.bank.hasSwarmContexts()?(ground||crouched):(ground&&p.presentedDrawn))')
edit('movement_playback.h','sampleSwarm(p.powerVariant,j,p.powerAmount)','sampleSwarm(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix)')
edit('movement_bank_tests.cpp','(argc==1?1:49);format<=49','(argc==1?1:50);format<=50')
edit('movement_bank_tests.cpp','check(bank.hasDomino()==','''check(bank.hasSwarmContexts()==(bank.version>=50),"Swarm stance/equipment contexts require bank50");
        if(bank.hasSwarmContexts())for(unsigned v=23;v<=25;++v)for(float t:{0.f,.5f,1.f})for(float low:{0.f,.5f,1.f})for(float armed:{0.f,.5f,1.f}){
            for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float x:bank.sampleSwarm(v,j,t,low,armed))check(std::isfinite(x),"Swarm context blends remain finite");
            if((low==0||low==1)&&(armed==0||armed==1)){
                const unsigned clip=low?(armed?305:306)+(v-23)*3:armed?301+v-23:304+(v-23)*3;
                for(unsigned j:{16u,17u}){const auto a=bank.sampleSwarm(v,j,t,low,armed),b=bank.sampleParameter(clip,j,t);for(unsigned k=0;k<7;++k)check(std::fabs(a[k]-b[k])<1e-6f,"Swarm context keeps corresponding sword-hand/socket pair");}
            }
            if(argc==3&&low==1&&armed==1){const auto a=bank.sampleSwarm(v,2,t,0,1),b=bank.sampleSwarm(v,2,t,1,1);check(b[6]<a[6]-.15f&&b[6]>.5f,"Swarm crouch preserves supported pelvis height");}
        }
        check(bank.hasDomino()==''')
edit('shadow_action_protocol_tests.cpp','check(!validShadowAction(bad,c,h)&&!shadowPowerSwordContext(variant,1),"unobserved holstered Swarm remains refused")','check(validShadowAction(bad,c,h)&&shadowPowerSwordContext(variant,1),"v273/v274 native holstered Swarm accepted by sender and receiver")')
edit('shadow_action_protocol_tests.cpp','bad.version=45;check(!validShadowAction(bad,c,h),"old peer cannot authorize Swarm semantics")','bad.version=46;check(!validShadowAction(bad,c,h),"armed-only Swarm peer cannot authorize expanded contexts")')
edit('world_identity_tests.cpp','require(!inspect(bad),"unobserved holstered Swarm refused")','require(inspect(bad)&&!observedCast.power.player.sword.equipped,"v273 native holstered Swarm preserves exact ownership")')
anchor='                            require(inspect(d)&&observedCast.variant==23+k&&observedCast.arm.character==1,"Swarm phase requires exact duration, Corvo body and owned left arm");'
extra='''
                            for(unsigned low=0;low<2;++low)for(unsigned empty=0;empty<2;++empty){
                                auto context=d;
                                if(low){context.d(physics+0x50,1);context.d(physics+0x54,1);context.d(active,6);context.q(mode+0x10+6*8,ground);context.q(ground,base+0x1c84258);context.d(ground+8,6);}
                                if(empty){context.q(inv+0x68,0);context.d(cpnt+0x1c,0x20000002);context.q(frame+0x3c8,actor);context.q(frame+0x3c0,names+0x200);context.d(frame+0x3d0,2);}
                                require(inspect(context)&&observedCast.variant==23+k&&observedCast.power.player.sword.equipped==!empty,"observed Swarm stance/equipment matrix retains phase and matching hand state");
                                auto bad=context;bad.d(leftArm+0xac,2);require(!inspect(bad),"all Swarm contexts reject foreign arms");
                                bad=context;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"only Swarm release may recover on exact standing/crouched ground state alone");
                                if(low){bad=context;bad.q(ground,base+0x1c85628);require(!inspect(bad),"crouched Swarm cannot use Running vtable with Sneaking ID");}
                            }
'''
edit('world_identity_tests.cpp',anchor,anchor+extra)
print('SWARM_CONTEXT_BINDING_SOURCE_READY')
