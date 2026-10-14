from pathlib import Path
import json
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; S=R/'outputs/d2-coop-lab/src'
read=lambda p:p.read_text(encoding='utf-8-sig')
def change(p,old,new):
    s=read(p); assert s.count(old)==1,(p,old)
    p.write_text(s.replace(old,new),encoding='utf-8')
def create(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
for take in ('native-doppelganger-holstered-standing-v493','native-doppelganger-holstered-crouched-v494'):
    assert json.loads(read(N/take/'result.json'))['passed']
    evidence=json.loads(read(N/take/'holstered-observations.json'))
    assert evidence['swordEquippedWhileCasting']==0 and evidence['swordHolsteredWhileCasting']>=3

change(S/'native_player_doppelganger_scene.h','//406b standing and489 crouched armed Emily:', '//406b/489 armed and493/494 holstered ground contexts for Emily:')
change(S/'native_player_doppelganger_scene.h','||!item.started||!item.player.sword.equipped||','||!item.started||')
change(S/'shadow_action_protocol.h','version=52,','version=53,')
change(S/'shadow_action_protocol.h','p.version==52','p.version==53')
change(S/'shadow_action_protocol.h','if(variant>=29&&variant<=31)return swordState==2;','if(variant>=29&&variant<=31)return swordState==1||swordState==2;')
change(S/'shadow_action_protocol_tests.cpp','bad=cast;bad.swordState=1;check(!validShadowAction(bad,ec,eh),"unobserved empty-hand Doppelganger refused");',
       'bad=cast;bad.swordState=1;check(validShadowAction(bad,ec,eh),"493/494 observed holstered Doppelganger accepted");\n        bad.version=52;check(!validShadowAction(bad,ec,eh),"pre-holstered contract refused");\n        bad=cast;bad.swordState=0;check(!validShadowAction(bad,ec,eh),"unknown Doppelganger equipment refused");')
old='bad=d;bad.q(inv+0x68,0);bad.d(cpnt+0x1c,0x20000002);bad.q(frame+0x3c8,actor);bad.q(frame+0x3c0,names+0x200);bad.d(frame+0x3d0,2);require(!inspect(bad),"unobserved holstered Doppelganger remains refused");'
new='''for(auto context:{d,low}){
                                context.q(inv+0x68,0);context.d(cpnt+0x1c,0x20000002);context.q(frame+0x3c8,actor);context.q(frame+0x3c0,names+0x200);context.d(frame+0x3d0,2);
                                require(inspect(context)&&!observedCast.power.player.sword.equipped,"493/494 native holstered ground contexts retain exact owned phases");
                                bad=context;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"holstered recovery accepts only release");
                                bad=context;bad.q(frame+0x3c8,leftArm);require(!inspect(bad),"holstered sword attachment cannot change parent");
                                bad=context;bad.q(inv+0x70,0);require(!inspect(bad),"holstered cast still needs selected power slot");
                            }'''
change(S/'world_identity_tests.cpp',old,new)
change(S/'movement_bank.h','32+325*','32+331*')
change(S/'movement_bank.h','322,325};return v<=56?','322,325,331};return v<=57?')
change(S/'movement_bank.h','for(unsigned v=1;v<=56;++v)','for(unsigned v=1;v<=57;++v)')
change(S/'movement_bank.h','''    Joint sampleDoppelganger(unsigned variant,unsigned joint,float phase,float crouched=0)const{
        const auto standing=sampleParameter(319+variant-29,joint,phase);
        return hasDoppelgangerCrouch()?blend(standing,sampleParameter(322+variant-29,joint,phase),crouched):standing;
    }''','''    bool hasDoppelgangerHolstered()const{return version>=57;}
    Joint sampleDoppelganger(unsigned variant,unsigned joint,float phase,float crouched=0,float armed=1)const{
        const unsigned v=variant-29;
        const auto standing=sampleParameter(319+v,joint,phase);
        const auto low=hasDoppelgangerCrouch()?sampleParameter(322+v,joint,phase):standing;
        if(!hasDoppelgangerHolstered())return blend(standing,low,crouched);
        return blend(blend(sampleParameter(325+v*2,joint,phase),standing,armed),
                     blend(sampleParameter(326+v*2,joint,phase),low,armed),crouched);
    }''')
change(S/'movement_playback.h','(ground||(crouched&&p.bank.hasDoppelgangerCrouch()))&&p.presentedDrawn;',
       '(ground||(crouched&&p.bank.hasDoppelgangerCrouch()))&&(p.presentedDrawn||p.bank.hasDoppelgangerHolstered());')
change(S/'movement_playback.h','sampleDoppelganger(p.powerVariant,j,p.powerAmount,p.clock.stanceMix)', 'sampleDoppelganger(p.powerVariant,j,p.powerAmount,p.clock.stanceMix,p.armedMix)')
change(S/'movement_bank_tests.cpp','format=(argc==1?1:56);format<=56','format=(argc==1?1:57);format<=57')
needle='        check(bank.hasDoppelganger()==(bank.version>=55),"Doppelganger requires bank55");'
addition='''
        check(bank.hasDoppelgangerHolstered()==(bank.version>=57),"hands-free Doppelganger requires bank57");
        if(bank.hasDoppelgangerHolstered())for(unsigned v=29;v<=31;++v)for(float t:{0.f,.5f,1.f}){
            for(float low:{0.f,.5f,1.f})for(float armed:{0.f,.5f,1.f})for(unsigned j:{2u,9u,16u,17u,20u,24u})
                for(float x:bank.sampleDoppelganger(v,j,t,low,armed))check(std::isfinite(x),"Doppelganger four-context blends finite");
            for(unsigned low=0;low<2;++low)for(unsigned j:{2u,9u,16u,17u,20u,24u}){
                const auto a=bank.sampleDoppelganger(v,j,t,float(low),0),b=bank.sampleParameter(325+(v-29)*2+low,j,t);
                for(unsigned k=0;k<7;++k)check(std::fabs(a[k]-b[k])<1e-6f,"empty-hand endpoint selects its authored complete-body clip");
            }
            if(argc==3){const auto a=bank.sampleDoppelganger(v,2,t,0,0),b=bank.sampleDoppelganger(v,2,t,1,0);
                check(b[6]<a[6]-.15f&&b[6]>.5f,"holstered crouch retains supported full-body pelvis");
                const float duration[]={.466f,1.2f,.666f};for(unsigned low=0;low<2;++low)
                    check(std::fabs(bank.clips[325+(v-29)*2+low].angle-duration[v-29])<1e-6f,"holstered timing matches native source");
            }
        }
'''
change(S/'movement_bank_tests.cpp',needle,needle+addition)

s=read(W/'Build-DoppelgangerCrouch-v490.ps1').replace('v490','v495').replace('Build490','Build495').replace('actual325','actual331').replace('doppelganger-crouch-','doppelganger-holstered-')
create(W/'Build-DoppelgangerHolstered-v495.ps1',s)
s=read(W/'archive_doppelganger_crouch_v490.py').replace('v490','v495')
s=s.replace("previous = W / 'candidate-doppelganger-fx-v481'","previous = W / 'candidate-doppelganger-crouch-v490'")
s=s.replace('doppelganger-crouch-','doppelganger-holstered-').replace('DoppelgangerCrouch','DoppelgangerHolstered').replace('doppelganger_crouch','doppelganger_holstered')
s=s.replace("previous = W / 'candidate-doppelganger-holstered-v490'", "previous = W / 'candidate-doppelganger-crouch-v490'").replace('DoppelgangerHolsteredDeploy490','DoppelgangerHolsteredDeploy495')
s=s.replace('protocol=52, bankVersion=56, clips=325','protocol=53, bankVersion=57, clips=331')
s=s.replace('489 native Sneaking6 plus UsePower63 and exact466/1200/666ms scenes drive full-body armed crouch clips322-324. Original322 payloads unchanged. Supported feet, native rig lengths, grip/socket; no root/FP/event changes.481 clone marker and FX fixes retained. Runtime and visual comparison pending',
            '493/494 confirm holstered standing/crouched native UsePower63 with unchanged466/1200/666ms phases. Six full-body hands-free clips325-330 in bank57 retain all325 old payloads. Semantic53 admits only observed ground sword states; clone4 unchanged. No FP/root/events changes. Runtime and visual verification pending.')
create(W/'archive_doppelganger_holstered_v495.py',s)
print('495 code, regression tests, build/archive inputs prepared from independently observed ground contexts.')
