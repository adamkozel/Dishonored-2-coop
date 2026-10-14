from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(name,changes):
 p=S/name;s=p.read_text()
 for before,after in changes:
  assert before in s,(name,before);s=s.replace(before,after)
 p.write_text(s)
edit('power_presentation.h',[
 ('433,333};return v<=15?d[v]:0;','433,333,466,1200,666};return v<=18?d[v]:0;'),
 ('v==8||v==13;','v==8||v==13||v==17;'),('s.variant<=15','s.variant<=18')])
edit('shadow_action_protocol.h',[
 ('version=39','version=40'),('p.version==39','p.version==40'),
 ('p.character==1?(p.powerVariant<=3||p.powerVariant>=11):p.powerVariant>=4&&p.powerVariant<=9','p.character==1?(p.powerVariant<=3||(p.powerVariant>=11&&p.powerVariant<=15)):((p.powerVariant>=4&&p.powerVariant<=9)||(p.powerVariant>=16&&p.powerVariant<=18))')])
edit('shadow_stream.h',[
 ('readNativePlayerPowerScene(memoryRead,base,states,power);','readNativePlayerPowerScene(memoryRead,base,states,power,true);'),
 ('!sword.sword.equipped&&power.variant<10','!sword.sword.equipped&&(power.variant<10||power.variant>=16)')])
edit('runtime_probe.cpp', [('base,s,power);','base,s,power,true);')])
edit('movement_bank.h',[
 ('32+273*','32+276*'),('bool hasCrouchedGrenade()','bool hasMesmerize()const{return version>=45;}\n    bool hasCrouchedGrenade()'),
 ('260,264,273};return v<=44','260,264,273,276};return v<=45'),('v<=44;++v','v<=45;++v')])
edit('movement_playback.h',[
 ('const bool powerActive=powerValid&&power.variant&&(power.variant<=9||','const bool powerActive=powerValid&&power.variant&&((power.variant>=16&&power.variant<=18&&p.bank.hasMesmerize()&&ground&&p.presentedDrawn)||power.variant<=9||'),
 ('||(p.powerVariant==15&&p.bank.hasOutOfMana()))){','||(p.powerVariant==15&&p.bank.hasOutOfMana())||(p.powerVariant>=16&&p.powerVariant<=18&&p.bank.hasMesmerize()))){'),
 ('auto power=p.powerVariant==15?', 'auto power=p.powerVariant>=16?p.bank.sampleParameter(273+p.powerVariant-16,j,p.powerAmount):p.powerVariant==15?'),
 ('(p.powerVariant>=12&&p.bank.hasBendTime()))&&MovementBank::combatLocomotionJoint','(p.powerVariant>=12&&p.powerVariant<=15&&p.bank.hasBendTime())||(p.powerVariant>=16&&p.bank.hasMesmerize()))&&MovementBank::combatLocomotionJoint')])
edit('movement_bank_tests.cpp',[
 ('argc==1?1:44);format<=44','argc==1?1:45);format<=45'),
 ('check(bank.hasGrenade()', '''check(bank.hasMesmerize()==(bank.version>=45),"Mesmerize cannot sample beyond an older bank");
        if(bank.hasMesmerize())for(unsigned c=273;c<276;++c)for(float a:{0.f,.5f,1.f})for(float v:bank.sampleParameter(c,9,a))check(std::isfinite(v),"Mesmerize phase wrist remains finite");
        check(bank.hasGrenade()''')])
edit('shadow_action_protocol_tests.cpp',[
 ('oldDark.powerVariant=16;', 'oldDark.powerVariant=19;'),
 ('      for(unsigned phase:{12u,14u}){', '''      for(unsigned variant=16;variant<=18;++variant){
        auto mesmer=emily;mesmer.powerVariant=variant;mesmer.powerDurationMs=PowerPresentation::duration(variant);
        check(validShadowAction(mesmer,ec,eh),"observed native Emily Mesmerize phase packet");
        auto bad=mesmer;bad.version=39;check(!validShadowAction(bad,ec,eh),"pre-Mesmerize peer refused");
        bad=mesmer;bad.role=0;bad.character=1;check(!validShadowAction(bad,c,h),"Corvo cannot claim Emily Mesmerize");
        bad=mesmer;bad.swordState=1;check(!validShadowAction(bad,ec,eh),"unverified holstered Mesmerize refused");
        bad=mesmer;bad.airStage=2;check(!validShadowAction(bad,ec,eh),"unverified airborne Mesmerize refused");
        bad=mesmer;bad.powerDurationMs++;check(!validShadowAction(bad,ec,eh),"incorrect Mesmerize phase duration refused");
      }
      {PowerPresentation m;PowerPresentation::Sample sampled{};float phase=0;
       check(m.push({100,1,17,1180,1200})&&m.push({160,1,17,1240,1200})&&m.sample(140,sampled,phase)&&std::fabs(phase-20.f/1200)<.00001f,"Mesmerize hold uses the accumulated native loop clock");
       check(!m.push({180,1,18,10,666}),"Mesmerize release requires a new event incarnation");
       check(m.push({180,2,18,0,666})&&m.push({210,2,18,30,666})&&m.sample(210,sampled,phase),"Mesmerize release starts its own timeline");
       check(!m.sample(411,sampled,phase),"stale Mesmerize release retires");}
      for(unsigned phase:{12u,14u}){''')])
