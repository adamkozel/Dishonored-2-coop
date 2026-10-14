"""Expand only after all native standing/crouch/gear observations pass."""
from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset'
for name in ['native-bendtime-corvo-no-mana-v44-v2','native-bendtime-corvo-no-mana-crouched-empty-v45','native-bendtime-corvo-no-mana-standing-empty-v45','native-bendtime-corvo-no-mana-crouched-armed-v45']:
 result=json.loads((P/name/'result.json').read_text(encoding='utf-8-sig'));assert result['passed'],name
 rows=json.loads((P/name/'states.json').read_text(encoding='utf-8-sig'))
 observed=[r for r in rows if any(s['state']==3 and s['asset']=='midnight/gameplay/player/base/powers/power_outofmana.midnightscene' for s in r['midnight']['instances'])]
 assert observed and all(r['player']['stance']==int(result['crouched']) and [a['id']for a in r['player']['active']]==([6]if result['crouched']else[0]) and not r['power']['powerActive']for r in observed),name
def edit(name,pairs):
 p=S/name;s=p.read_text()
 for a,b in pairs:assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 p.write_text(s)
edit('native_player_power_scene.h',[
 ('// Two actual v44 requests confirmed only standing armed Corvo,\n            // selected Bend Time, inactive power and sole Running state.','// Exact v44/v45 failed casts confirm Corvo standing/crouched,\n            // drawn/holstered, inactive selected Bend Time and sole ground state.'),
 ('if(variant==15&&(states.player.current!=0||!s.power.player.sword.equipped||bend.powerActive||\n                states.active.size()!=1||states.active[0].id!=0))continue;', 'if(variant==15&&(states.player.current>1||bend.powerActive||\n                states.active.size()!=1||states.active[0].id!=(states.player.current==1?6u:0u)))continue;')])
edit('shadow_action_protocol.h',[('version=35,','version=36,'),('p.version==35&&','p.version==36&&'),('p.powerVariant<=14)&&p.swordState==1','p.powerVariant<=15)&&p.swordState==1')])
edit('movement_bank.h',[
 ('32+248*','32+251*'),('235,238,247,248};return v<=40?','235,238,247,248,251};return v<=41?'),('for(unsigned v=1;v<=40;++v)','for(unsigned v=1;v<=41;++v)'),
 ('bool hasOutOfMana()const', '''bool hasOutOfManaContexts()const{return version>=41;}
    Joint sampleOutOfMana(unsigned joint,float phase,float crouched,float armed)const{
        if(!hasOutOfManaContexts())return sampleParameter(247,joint,phase);
        const auto stand=blend(sampleParameter(248,joint,phase),sampleParameter(247,joint,phase),armed);
        const auto low=blend(sampleParameter(250,joint,phase),sampleParameter(249,joint,phase),armed);
        return blend(stand,low,crouched);
    }
    bool hasOutOfMana()const''')])
edit('movement_playback.h',[
 ('power.variant==15&&p.bank.hasOutOfMana()&&ground&&p.presentedDrawn','power.variant==15&&p.bank.hasOutOfMana()&&((p.bank.hasOutOfManaContexts()&&(ground||crouched))||(ground&&p.presentedDrawn))'),
 ('p.powerVariant==15?p.bank.sampleParameter(247,j,p.powerAmount)','p.powerVariant==15?p.bank.sampleOutOfMana(j,p.powerAmount,p.clock.stanceMix,p.armedMix)')])
edit('world_identity_tests.cpp',[
 ('invalidEmpty=empty;invalidEmpty.d(physics+0x50,1);require(!inspectCast(invalidEmpty),"unobserved crouched failed cast refused");', '''auto lowEmpty=low;lowEmpty.str(texts+0x200,"midnight/gameplay/player/base/powers/power_outofmana.midnightscene");lowEmpty.d(scene+0x28,333);lowEmpty.d(instance+0x18,1);
                        require(inspectCast(lowEmpty)&&observedCast.variant==15&&!observedCast.power.player.sword.equipped,"observed crouched empty-hand failure keeps exact owned arm");
                        invalidEmpty=empty;invalidEmpty.d(physics+0x50,1);require(!inspectCast(invalidEmpty),"failure refuses mismatched stance and active native state");
                        invalidEmpty=falling;invalidEmpty.str(texts+0x200,"midnight/gameplay/player/base/powers/power_outofmana.midnightscene");invalidEmpty.d(scene+0x28,333);require(!inspectCast(invalidEmpty),"unobserved airborne failure remains refused");''')])
edit('shadow_action_protocol_tests.cpp',[
 ('check(!validShadowAction(invalidEmpty,c,h),"unobserved empty-hand failure refused")','check(validShadowAction(invalidEmpty,c,h),"observed empty-hand failure retains holstered equipment")'),
 ('invalidEmpty.version=34;check(!validShadowAction(invalidEmpty,c,h),"older peer cannot receive new failure semantic")','invalidEmpty.version=35;check(!validShadowAction(invalidEmpty,c,h),"older peer cannot receive expanded failure contexts")')])
edit('movement_bank_tests.cpp',[
 ('(argc==1?1:40);format<=40;','(argc==1?1:41);format<=41;'),
 ('check(bank.hasOutOfMana()==', '''check(bank.hasOutOfManaContexts()==(bank.version>=41),"failure contexts require appended bank version");
        if(bank.hasOutOfManaContexts())for(float low:{0.f,1.f})for(float armed:{0.f,1.f}){
            const unsigned clip=low?(armed?249:250):(armed?247:248);
            const auto a=bank.sampleOutOfMana(16,.5f,low,armed),b=bank.sampleParameter(clip,16,.5f);
            for(unsigned k=0;k<7;++k)check(std::fabs(a[k]-b[k])<1e-6f,"failure context selects matching stance and right-hand grip");
        }
        check(bank.hasOutOfMana()==''')])
print('OUTOFMANA_CONTEXTS_V46')
