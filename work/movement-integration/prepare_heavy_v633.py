from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2]; S=R/'outputs/d2-coop-lab/src'; W=R/'work/movement-integration'; N=R/'work/natural-moveset'
backup=W/'heavy-pre-v633';shutil.copytree(S,backup)
def edit(name, changes):
 p=S/name;s=p.read_text()
 for a,b in changes:
  assert a in s,(name,a);s=s.replace(a,b)
 p.write_text(s)
edit('native_player_combat_scene.h',[
 ('"sword_sneak_attack_left_hit","throwhit"','"sword_sneak_attack_left_hit","throwhit","sword_attack_focus_sneak_out"'),
 ('i<17;++i','i<18;++i'),
 ('    unsigned playerCount=0,armCount=0,targetCount=0;', '''    // Native629/630: crouched release binds only owned player and right arm.
    if(s.variant==18&&((bindingFlags&0xffffff)!=2||s.timeMs>s.durationMs||states.player.current!=1||
       !std::any_of(states.active.begin(),states.active.end(),[](const auto& a){return a.id==6;})))return false;
    unsigned playerCount=0,armCount=0,targetCount=0;'''),
 ('s.character=arm.character;','''s.character=arm.character;
    if(s.variant==18&&(s.subscene!=(s.character==1?"Scene_2":"Scene_1")||s.durationMs!=(s.character==1?1000u:833u)))return false;''')])
edit('combat_presentation.h',[
 ('return v==17?353:', 'return v==18?356:v==17?353:'),('s.variant>17','s.variant>18'),
 ('           (s.variant==17&&', '           (s.variant==18&&((s.duration!=1000&&s.duration!=833)||s.phase!=1||s.elapsed>s.duration))||\n           (s.variant==17&&')])
edit('shadow_action_protocol.h',[
 ('version=67,','version=68,'),('p.version==67','p.version==68'),('p.combatVariant<=17','p.combatVariant<=18'),
 ('(p.combatVariant==17?800u:', '(p.combatVariant==18?(p.character==1?1000u:833u):p.combatVariant==17?800u:')])
edit('movement_bank.h',[
 ('32+354*','32+357*'),('    bool hasKick()', '    bool hasCrouchHeavy()const{return version>=67;}\n    bool hasKick()'),
 ('353,354};return v<=66','353,354,357};return v<=67'),('v<=66;++v','v<=67;++v')])
edit('movement_playback.h',[
 ('(combat.variant!=17||p.bank.hasKick())&&','(combat.variant!=17||p.bank.hasKick())&&(combat.variant!=18||p.bank.hasCrouchHeavy())&&'),
 ('            if(p.combatClip>=53&&p.combatClip<=55)', '''            // Reuse the complete authored heavy action with the existing
            // whole-body crouch adaptation. Source stance/root stay native.
            if(p.bank.hasCrouchHeavy()&&p.combatClip>=50&&p.combatClip<=52)
                action=MovementBank::blend(action,p.bank.sampleCombat(354+p.combatClip-50,j,authored,0),p.clock.stanceMix*(1-p.combatAirMix));
            if(p.combatVariant==18)
                action=MovementBank::blend(p.bank.sampleCombat(52,j,authored,0),action,p.clock.stanceMix);
            if(p.combatClip>=53&&p.combatClip<=55)''')])
edit('world_identity_tests.cpp',[(
 '        std::cout<<"PASS: native combat scene ownership, bounds, generation and concurrent replacement\\n";',
 '''        {
            auto heavy=cm;heavy.d(component+0x50,1);heavy.d(active,6);heavy.q(mode+0x10+6*8,state);heavy.q(state,base+0x1c84258);heavy.d(state+8,6);
            heavy.str(names+0x300,"midnight/gameplay/player/base/combat/sword_attack_focus_sneak_out.midnightscene");
            heavy.str(names+0x400,"Scene_2");heavy.d(scene+0x28,1000);
            require(combat(heavy)&&result.variant==18&&result.character==1,"Corvo owned crouched heavy release");
            auto em=heavy;em.str(names+0x100,"models/characters/player/emily/body.def");em.str(names+0x200,"models/characters/player/emily/right_arm.def");em.str(names+0x400,"Scene_1");em.d(scene+0x28,833);
            require(combat(em)&&result.variant==18&&result.character==2,"Emily owned crouched heavy release");
            auto wrong=heavy;wrong.str(names+0x400,"Scene_1");require(!combat(wrong),"other character heavy subscene refused");
            wrong=heavy;wrong.d(scene+0x28,833);require(!combat(wrong),"other character heavy duration refused");
            wrong=heavy;wrong.d(component+0x50,0);require(!combat(wrong),"unobserved standing sneak release refused");
            wrong=heavy;wrong.d(instance+0x18,3);wrong.q(bindings+0x48,player);require(!combat(wrong),"heavy release cannot acquire kick duplicate binding");
            wrong=heavy;wrong.d(instance+0x160,1001);require(!combat(wrong),"expired heavy release refused");
        }
        std::cout<<"PASS: native combat scene ownership, bounds, generation and concurrent replacement\\n";''')])
edit('shadow_action_protocol_tests.cpp',[
 ('bad.version=66;check(!validShadowAction(bad,c,h),"previous combat wire refused")','bad.version=67;check(!validShadowAction(bad,c,h),"previous combat wire refused")'),
 ('    {auto choke=p;choke.state=21;', '''    {auto heavy=p;heavy.state=1;heavy.combatPhase=1;heavy.combatVariant=18;
      for(unsigned ch:{1u,2u}){heavy.character=ch;heavy.role=ch-1;heavy.combatDurationMs=ch==1?1000:833;
       auto context=c;context.remoteRole=heavy.role;
       check(validShadowAction(heavy,context,h),"character-specific crouched heavy release wire");
       auto bad=heavy;bad.combatDurationMs=ch==1?833:1000;check(!validShadowAction(bad,context,h),"wrong role heavy duration refused");
       bad=heavy;bad.combatTimeMs=heavy.combatDurationMs+1;check(!validShadowAction(bad,context,h),"expired heavy wire refused");
       bad=heavy;bad.state=3;bad.airStage=2;check(!validShadowAction(bad,context,h),"unobserved airborne sneak heavy refused");
      }
      CombatPresentation heavyClock;CombatPresentation::Sample sample{};float amount=0;
      check(heavyClock.push({100,1,18,1,0,833})&&heavyClock.push({200,1,18,1,100,833})&&heavyClock.sample(180,sample,amount)&&std::fabs(amount-80.f/833)<1e-6f,"heavy source clock interpolated");
      check(CombatPresentation::clip(18,1)==356&&!CombatPresentation::recovery(18),"heavy addresses new full-body release");
      check(!heavyClock.push({250,2,18,2,0,833})&&!heavyClock.push({250,2,18,1,0,834}),"heavy loop and duration guarded");
    }
    {auto choke=p;choke.state=21;''')])
edit('movement_bank_tests.cpp',[
 ('    check(argc==1||argc==3','    {MovementBank old;old.version=66;check(!old.hasCrouchHeavy(),"old bank cannot address crouched heavy clips");}\n    check(argc==1||argc==3'),
 ('        if(bank.hasKick())', '''        if(bank.hasCrouchHeavy())for(unsigned clip:{354u,355u,356u})for(unsigned j=0;j<joints;++j)for(float t:{0.f,.2f,.5f,1.f}){
            const auto pose=bank.sampleCombat(clip,j,t,0);for(float x:pose)check(std::isfinite(x),"crouched heavy pose finite");
            if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"crouched heavy cannot move network root");
        }
        if(bank.hasKick())''')])
s=(W/'Build-Kick-v625.ps1').read_text().replace('v625.log','v633.log').replace('Build625','Build633').replace('kick-banks-v625','heavy-banks-v633').replace('actual354','actual357');(W/'Build-Heavy-v633.ps1').write_text(s)
print('Prepared633: exact crouched heavy release18, three existing-pipeline body clips, protocol68/bank67.')
