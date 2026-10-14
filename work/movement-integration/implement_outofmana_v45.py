"""Scoped native failed-cast binding, including wire and bank versions."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(name,pairs):
 p=S/name;s=p.read_text()
 for a,b in pairs:
  assert s.count(a)==1,(name,a,s.count(a));s=s.replace(a,b)
 p.write_text(s)
edit('native_player_power_scene.h',[
 ('if(character==1&&sub=="Scene_1"){','if(character==1&&sub=="Scene_1"){\n        if(asset==prefix+"power_outofmana.midnightscene"&&duration==333)return 15;'),
 ('if(s.instance){if(s.instance==c)continue;', '''// Two actual v44 requests confirmed only standing armed Corvo,
            // selected Bend Time, inactive power and sole Running state.
            if(variant==15&&(states.player.current!=0||!s.power.player.sword.equipped||bend.powerActive||
                states.active.size()!=1||states.active[0].id!=0))continue;
            if(s.instance){if(s.instance==c)continue;'''),
 ('if(!readNativeBendTimeItem(read,base,end,true))return false;finalPower=end;', 'if(!readNativeBendTimeItem(read,base,end,true)||(s.variant==15&&end.powerActive))return false;finalPower=end;')])
edit('power_presentation.h',[
 ('533,300,433};return v<=14?', '533,300,433,333};return v<=15?'),('s.variant<=14&&','s.variant<=15&&')])
edit('shadow_action_protocol.h',[
 ('version=34,','version=35,'),('p.version==34&&','p.version==35&&'),
 ('((p.powerVariant>=10)&&p.swordState==1)','((p.powerVariant>=10&&p.powerVariant<=14)&&p.swordState==1)')])
edit('movement_bank.h',[
 ('32+247*','32+248*'),('235,238,247};return v<=39?','235,238,247,248};return v<=40?'),('for(unsigned v=1;v<=39;++v)','for(unsigned v=1;v<=40;++v)'),
 ('bool hasBendTimeContexts()const','bool hasOutOfMana()const{return version>=40;}\n    bool hasBendTimeContexts()const')])
edit('movement_playback.h',[
 ('(power.variant>=12&&power.variant<=14&&p.bank.hasBendTime()', '(power.variant==15&&p.bank.hasOutOfMana()&&ground&&p.presentedDrawn)||\n            (power.variant>=12&&power.variant<=14&&p.bank.hasBendTime()'),
 ('||(p.powerVariant>=12&&p.powerVariant<=14&&p.bank.hasBendTime()))){','||(p.powerVariant>=12&&p.powerVariant<=14&&p.bank.hasBendTime())||(p.powerVariant==15&&p.bank.hasOutOfMana()))){'),
 ('auto power=p.powerVariant>=12?', 'auto power=p.powerVariant==15?p.bank.sampleParameter(247,j,p.powerAmount):p.powerVariant>=12?')])
edit('shadow_action_protocol_tests.cpp',[
 ('oldDark.powerVariant=15;', 'oldDark.powerVariant=16;'),
 ('auto hold=power;hold.powerVariant=13;', '''auto empty=power;empty.powerVariant=15;empty.powerDurationMs=333;
      check(validShadowAction(empty,c,h),"owned Corvo insufficient-mana response");
      auto invalidEmpty=empty;invalidEmpty.airStage=2;check(!validShadowAction(invalidEmpty,c,h),"unobserved airborne failure refused");
      invalidEmpty=empty;invalidEmpty.swordState=1;check(!validShadowAction(invalidEmpty,c,h),"unobserved empty-hand failure refused");
      invalidEmpty=empty;invalidEmpty.role=1;invalidEmpty.character=2;check(!validShadowAction(invalidEmpty,ec,eh),"Emily failure requires separate observed binding");
      invalidEmpty=empty;invalidEmpty.version=34;check(!validShadowAction(invalidEmpty,c,h),"older peer cannot receive new failure semantic");
      auto hold=power;hold.powerVariant=13;''')])
edit('world_identity_tests.cpp',[
 ('bad=bend;bad.d(instance+0x18,2);require(!inspectCast(bad),"Bend Time release rejects extra player binding");', '''auto empty=bend;empty.str(texts+0x200,"midnight/gameplay/player/base/powers/power_outofmana.midnightscene");empty.d(scene+0x28,333);
                        require(inspectCast(empty)&&observedCast.variant==15,"native out-of-mana response with exact selected power and owned arm");
                        auto invalidEmpty=empty;const unsigned char on=1;invalidEmpty.put(pc+0xd0,&on,1);require(!inspectCast(invalidEmpty),"active power cannot impersonate failed cast");
                        invalidEmpty=empty;invalidEmpty.d(physics+0x50,1);require(!inspectCast(invalidEmpty),"unobserved crouched failed cast refused");
                        invalidEmpty=empty;invalidEmpty.q(bindings+8,arm);require(!inspectCast(invalidEmpty),"failed cast rejects wrong arm");
                        invalidEmpty=empty;invalidEmpty.d(scene+0x28,334);require(!inspectCast(invalidEmpty),"failed cast requires native scene duration");
                        bad=bend;bad.d(instance+0x18,2);require(!inspectCast(bad),"Bend Time release rejects extra player binding");''')])
edit('movement_bank_tests.cpp',[
 ('check(bank.hasBendTimeContexts()==', '''check(bank.hasOutOfMana()==(bank.version>=40),"failed cast requires its own bank version");
        if(bank.hasOutOfMana())for(float phase:{0.f,.5f,1.f}){
            const auto hand=bank.sampleParameter(247,9,phase);check(std::isfinite(hand[0]),"failure samples within appended clip");
        }
        check(bank.hasBendTimeContexts()==''')])
print('OUTOFMANA_CODE_V45')
