from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';N=R/'work/natural-moveset'
def edit(name,changes):
 p=S/name;s=p.read_text()
 for a,b in changes:
  assert a in s,(name,a);s=s.replace(a,b)
 p.write_text(s)
edit('combat_presentation.h',[
 ('return v>=15?48+v-15:', 'return v==17?353:v>=15?48+v-15:'),
 ('s.variant>16','s.variant>17'),
 ('(recovery(s.variant)&&(s.duration!=', '(s.variant==17&&(s.duration!=800||s.phase!=1||s.elapsed>s.duration))||\n           (recovery(s.variant)&&(s.duration!=')])
edit('shadow_action_protocol.h',[
 ('version=66,','version=67,'),('p.version==66','p.version==67'),
 ('p.combatVariant<=16&&(p.combatVariant<15||','p.combatVariant<=17&&(p.combatVariant<15||'),
 ('(p.combatVariant==15?866u:666u)','(p.combatVariant==17?800u:p.combatVariant==15?866u:666u)')])
edit('movement_bank.h',[
 ('32+353*','32+354*'),
 ('bool hasChoke(', 'bool hasKick()const{return version>=66;}\n    bool hasChoke('),
 ('351,352,353};return v<=65','351,352,353,354};return v<=66'),
 ('v<=65;++v','v<=66;++v')])
edit('movement_playback.h',[
 ('combatValid&&combat.variant&&pose.motionValid','combatValid&&combat.variant&&(combat.variant!=17||p.bank.hasKick())&&pose.motionValid'),
 ('if(p.bank.hasAirCombat()&&MovementBank::combatLocomotionJoint', 'if(p.combatVariant!=17&&p.bank.hasAirCombat()&&MovementBank::combatLocomotionJoint')])
edit('world_identity_tests.cpp',[(
 '        std::cout<<"PASS: native combat scene ownership, bounds, generation and concurrent replacement\\n";',
 '''        {
            auto kick=cm;kick.d(component+0x50,1);kick.d(active,6);kick.q(mode+0x10+6*8,state);kick.q(state,base+0x1c84258);kick.d(state+8,6);
            kick.str(names+0x300,"midnight/gameplay/player/base/combat/throwhit.midnightscene");kick.str(names+0x400,"kick");kick.d(scene+0x28,800);
            kick.d(instance+0x18,3);kick.q(bindings+8,arm);kick.q(bindings+0x28,player);kick.q(bindings+0x48,player);
            require(combat(kick)&&result.variant==17&&!result.target.actor,"native kick has one owned arm and two camera/body references to the same player");
            auto wrong=kick;wrong.q(bindings+0x48,arm);require(!combat(wrong),"kick cannot duplicate arm instead of player");
            wrong=kick;wrong.d(instance+0x18,2);require(!combat(wrong),"kick requires both player camera and body groups");
            wrong=kick;wrong.str(names+0x400,"Scene_1");require(!combat(wrong),"kick subscene identity required");
            wrong=kick;wrong.d(scene+0x28,801);require(!combat(wrong),"kick duration must be native800");
            wrong=kick;wrong.d(instance+0x160,801);require(!combat(wrong),"expired kick scene refused");
            wrong=kick;wrong.d(component+0x50,0);require(!combat(wrong),"unobserved standing kick source remains refused");
            wrong=kick;wrong.d(arm+0xac,2);require(!combat(wrong),"kick arm cannot belong to partner");
            wrong=kick;unsigned reads=0;NativePlayerStates ns{};
            require(readNativePlayerStates([&](auto p,auto d,auto n){return kick.read(p,d,n);},base,ns),"kick state fixture");
            require(!readNativePlayerCombatScene([&](auto p,auto d,auto n){const bool ok=wrong.read(p,d,n);if(p==bindings+0x48&&++reads==1)wrong.q(p,arm);return ok;},base,ns,result),"kick actor replacement during read refused");
        }
        std::cout<<"PASS: native combat scene ownership, bounds, generation and concurrent replacement\\n";''')])
edit('shadow_action_protocol_tests.cpp',[
 ('for(unsigned v:{15u,16u}){hit.combatVariant=v;hit.combatDurationMs=v==15?866:666;', 'for(unsigned v:{15u,16u,17u}){hit.combatVariant=v;hit.combatDurationMs=v==17?800:v==15?866:666;'),
 ('bad.version=65;check(!validShadowAction(bad,c,h),"previous combat wire refused")','bad.version=66;check(!validShadowAction(bad,c,h),"previous combat wire refused")'),
 ('      check(t.push({100,1,1,1,0,1000})', '''      {CombatPresentation kick;CombatPresentation::Sample k{};float u=0;
       check(kick.push({100,1,17,1,0,800})&&kick.push({200,1,17,1,100,800})&&kick.sample(180,k,u)&&k.variant==17&&std::fabs(u-.1f)<1e-6f,"kick uses the delayed source clock");
       check(CombatPresentation::clip(17,1)==353&&!CombatPresentation::recovery(17),"kick addresses its complete body clip, not hit recovery");
       check(!kick.push({250,2,17,1,0,801})&&!kick.push({250,2,17,2,0,800})&&!kick.push({250,2,17,1,801,800}),"kick duration, loop and expiry guarded");}
      check(t.push({100,1,1,1,0,1000})''')])
edit('movement_bank_tests.cpp',[
 ('    check(argc==1||argc==3', '    {MovementBank old;old.version=65;check(!old.hasKick(),"old bank cannot address absent kick clip");}\n    check(argc==1||argc==3'),
 ('        check(bank.hasPossessionCrouch()', '''        if(bank.hasKick())for(unsigned j=0;j<joints;++j)for(float t:{0.f,.2f,.5f,1.f}){
            const auto pose=bank.sampleCombat(353,j,t,0);for(float x:pose)check(std::isfinite(x),"kick full-body pose finite");
            if(j<2)for(unsigned k=0;k<7;++k)check(std::fabs(pose[k]-(k==3?1.f:0.f))<1e-6f,"kick cannot move network root");
        }
        check(bank.hasPossessionCrouch()''')])
# Existing linear/quaternion sampling appends one already retargeted donor;
# no new rig, FBX import or runtime animation-event path.
s=(N/'build_lethal_bank_v604.py').read_text().replace('lethal-banks-v604','kick-banks-v625').replace("'choke-breakout-banks-v592'","'lethal-banks-v604'")
s=s.replace("(b'D2MOVE64',64,352)","(b'D2MOVE65',65,353)").replace("P/'paired-native-v1'/f'paired_fast_back_tall-{role}.bin'","P/'native-retargeted'/f'0e5cbaf5c5330450-{role}.bin'")
s=s.replace("b'D2MOVE65',65,n,353","b'D2MOVE66',66,n,354").replace("352,121,0,0,0,1.333","353,121,0,0,0,.8")
s=s.replace('version=65,clips=353,prior352PayloadsUnchanged=True,variant=5', 'version=66,clips=354,prior353PayloadsUnchanged=True,variant=17')
s=s.replace("nativeScene='assassination_fast_back.midnightscene/Npc/1333'", "nativeScene='throwhit.midnightscene/kick/800'")
s=s.replace('Existing native Doppelganger paired actor donor;120 samples plus guard. Network root retained; source root track and all events excluded. Native player clock1333ms.', 'Existing retargeted sword_attack_kick donor1.5s normalized to native800ms for a first gameplay candidate;120 samples plus guard. Source roots/events excluded. Timing and contact need actual-game comparison.')
s=s.replace('lethal-bank-manifest-v604','kick-bank-manifest-v625').replace('Bank65/353: existing paired fast-back actor, previous352 unchanged.','Bank66/354: existing full-body kick, previous353 unchanged.')
(N/'build_kick_bank_v625.py').write_text(s)
s=(W/'Build-CrouchStance-v620.ps1').read_text().replace('v620.log','v625.log').replace('Build620','Build625').replace('lethal-banks-v604','kick-banks-v625').replace('actual353','actual354');(W/'Build-Kick-v625.ps1').write_text(s)
print('Prepared625 source guards, wire67, bank66/354 and existing-donor build. Runtime unverified.')
