"""Reuse paired actor channel for exact native596 fast rear assassination."""
from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src';B=W/'lethal-pre-v604';B.mkdir(exist_ok=False)
def edit(name, pairs):
 p=S/name;shutil.copy2(p,B/name);s=p.read_text()
 for a,b in pairs:assert a in s,(name,a);s=s.replace(a,b)
 p.write_text(s)
edit('choke_presentation.h',[
 ('// Presentation only: observed native rear-choke phases, no victim events.','// Legacy choke field names carry paired actor presentation only.\n// Variants1-4: rear choke;5: native fast-back lethal. Never replay victim events.'),
 ('0,600,1400,1800,1500};return v<=4','0,600,1400,1800,1500,1333};return v<=5'),('s.variant<=4','s.variant<=5')])
edit('native_player_states.h',[
 ('inline bool nativePlayerMotion(','''// Native596 on both roles: exclusive Assassination13, standing0/default mode.
inline bool nativePlayerGroundAssassinationStates(const NativePlayerStates& s,std::uintptr_t base){
    return s.player.logic==1&&s.defaultMode&&s.player.current==0&&s.active.size()==1&&
        s.active[0].id==13&&s.active[0].vtable==base+0x1c83468;
}
inline bool nativePlayerGroundPairedStates(const NativePlayerStates& s,std::uintptr_t base){
    return nativePlayerGroundChokeStates(s,base)||nativePlayerGroundAssassinationStates(s,base);
}
inline bool nativePlayerMotion('''),
 ('if(nativePlayerGroundChokeStates(s,base)){motion=0;', 'if(nativePlayerGroundPairedStates(s,base)){motion=0;')])
edit('native_player_choke_scene.h',[
 ('// first-person and victim animations continue untouched on the source.','// first-person and victim animations continue untouched on the source.\n// The same owned four-actor channel also carries exact native596 fast-back lethal.'),
 ('    if(sub!="Npc")return 0;','    if(sub!="Npc")return 0;\n    if(duration==1333&&asset=="midnight/gameplay/player/base/assassination/assassination_fast_back.midnightscene")return 5;'),
 ('if(!nativePlayerGroundChokeStates(states,base))','if(!nativePlayerGroundPairedStates(states,base))'),
 ('if(asset.find("midnight/gameplay/player/base/choke/")!=0)continue;','if(asset.find("midnight/gameplay/player/base/choke/")!=0&&asset.find("midnight/gameplay/player/base/assassination/")!=0)continue;'),
 ('if(!variant)return false;', 'if(!variant)return false;\n            if(variant==5?!nativePlayerGroundAssassinationStates(states,base):!nativePlayerGroundChokeStates(states,base))return false;'),
 ('!nativePlayerGroundChokeStates(last,base)','!nativePlayerGroundPairedStates(last,base)')])
edit('shadow_stream.h',[('if(sword.sword.equipped||choke.character!=localRole+1','if(sword.sword.equipped!=(choke.variant==5)||choke.character!=localRole+1')])
edit('shadow_action_protocol.h',[('version=64','version=65'),('p.version==64','p.version==65'),('p.state==21&&p.swordState==1','p.state==21&&p.swordState==(p.chokeVariant==5?2u:1u)')])
edit('movement_bank.h',[
 ('32+352*','32+353*'),
 ('version>=63&&(variant<=3||version>=64)','version>=63&&(variant<=3||(variant==4&&version>=64)||(variant==5&&version>=65))'),
 ('348,351,352};return v<=64','348,351,352,353};return v<=65'),('v<=64;++v','v<=65;++v')])
edit('shadow_action_protocol_tests.cpp',[
 ('for(unsigned v=1;v<=4;++v){choke.chokeVariant=v;','for(unsigned v=1;v<=5;++v){choke.swordState=v==5?2u:1u;choke.chokeVariant=v;'),
 ('bad.version=63;', 'bad.version=64;'),
 ('bad.swordState=2;check(!validShadowAction(bad,c,h),"choke cannot leave sword drawn")','bad.swordState=1;check(!validShadowAction(bad,c,h),"lethal paired scene requires presented sword")'),
 ('    ChokePresentation t;', '''    native.active.resize(1);native.active[0].id=13;native.active[0].vtable=base+0x1c83468;native.player.current=0;
    check(nativePlayerGroundAssassinationStates(native,base)&&nativePlayerMotion(native,base,motion)&&motion==0,"native assassination carries independent standing root");
    check(!nativePlayerGroundChokeStates(native,base)&&!nativePresentationFallbackGround(native,base),"lethal identity does not admit choke or generic fallback");
    native.player.current=12;check(!nativePlayerGroundAssassinationStates(native,base),"unobserved assassination stance refused");
    check(nativeChokeVariant("midnight/gameplay/player/base/assassination/assassination_fast_back.midnightscene","Npc",1333)==5&&
          !nativeChokeVariant("midnight/gameplay/player/base/assassination/assassination_fast_front.midnightscene","Npc",1333),"only observed native fast-back lethal admitted");
    ChokePresentation t;''')])
edit('movement_bank_tests.cpp',[
 ('    check(argc==1||argc==3', '    {MovementBank old;old.version=64;check(old.hasChoke(4)&&!old.hasChoke(5),"bank64 cannot address absent lethal clip");}\n    check(argc==1||argc==3'),
 ('        check(bank.hasPossessionCrouch()', '''        if(bank.hasChoke(5))for(unsigned j=0;j<joints;++j)for(float t:{0.f,.2f,.5f,1.f}){
            const auto pose=bank.sampleParameter(352,j,t);for(float x:pose)check(std::isfinite(x),"lethal full-body pose finite");
            if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"lethal cannot move network root");
        }
        check(bank.hasPossessionCrouch()''')])
for a,b in [('Build-Choke-v592.ps1','Build-Lethal-v604.ps1'),('Deploy-Choke-v592.ps1','Deploy-Lethal-v604.ps1')]:
 s=(W/a).read_text().replace('v592','v604').replace('Deploy592','Deploy604').replace('Build592','Build604').replace('stage_choke_v604','stage_lethal_v604').replace('choke-breakout-banks-v604','lethal-banks-v604').replace('352 clip','353 clip');(W/b).write_text(s)
s=(W/'stage_choke_v592.py').read_text().replace("A=W/'candidate-choke-breakout-v592';P=W/'candidate-choke-network-v584'","A=W/'candidate-lethal-v604';P=W/'candidate-choke-breakout-v592'")
s=s.replace('shadowwalk-body-{tag}-v592','shadowwalk-body-{tag}-v604').replace('choke-breakout-banks-v592','lethal-banks-v604').replace("W/'implement_choke_breakout_v592.py'","W/'implement_lethal_v604.py'").replace("W/'Build-Choke-v592.ps1'","W/'Build-Lethal-v604.ps1'").replace('build_choke_breakout_v592.py','build_lethal_bank_v604.py').replace('protocol=64,bankVersion=64,clips=352','protocol=65,bankVersion=65,clips=353')
a=s.index("scope='");b=s.index("')\n",a);s=s[:a]+"scope='Native596 both-role fast-back assassination1333 bound to existing actor bank through paired channel, legacy choke variant5. Exact exclusive state13/standing0, four owned actors and unique NPC. Original352 clips unchanged. Sword-equipped assertion and full runtime validation pending. No remote victim animation, damage, death/corpse or world outcome.'"+s[b+1:]
(W/'stage_lethal_v604.py').write_text(s);print('Candidate604 source prepared; prior592 preserved, native lethal validation pending.')
