from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
p=P/'shadow_action_protocol_tests.cpp';s=p.read_text();s=s.replace('int main(){','#include "grenade_presentation_tests.h"\nint main(){\n    grenadePresentationTests();')
mark='    {auto r=p;r.state=13;'
at=s.index(mark);s=s[:at]+'''    {auto g=p;g.state=15;g.grenadeHeld=1;
      for(unsigned v=1;v<=8;++v){g.grenadeVariant=v;g.grenadeDurationMs=GrenadePresentation::duration(v);g.grenadeTimeMs=g.grenadeDurationMs/2;check(validShadowAction(g,c,h),"known native grenade phases accepted");}
      auto bad=g;bad.version=36;check(!validShadowAction(bad,c,h),"previous grenade-free wire version refused");
      bad=g;bad.rangedKind=1;check(!validShadowAction(bad,c,h),"simultaneous gun and grenade cannot own hand");
      bad=g;bad.grenadeHeld=0;check(!validShadowAction(bad,c,h),"orphan grenade phase refused");
      bad=g;bad.grenadeDurationMs--;check(!validShadowAction(bad,c,h),"wrong grenade duration refused");
      bad=g;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"grenade cannot cross world incarnation");
      bad=g;bad.airStage=2;check(!validShadowAction(bad,c,h),"unverified airborne grenade refused");
      g.state=0;g.grenadeVariant=g.grenadeTimeMs=g.grenadeDurationMs=0;check(validShadowAction(g,c,h),"native standing held grenade is independent of action scene");
      g.role=1;g.character=2;g.sourceSession=22;g.targetSession=11;g.sourceWorldEpoch=7;g.targetWorldEpoch=3;check(validShadowAction(g,h,c),"Emily uses identical native grenade semantics");
    }
'''+s[at:];p.write_text(s)
p=P/'movement_bank_tests.cpp';s=p.read_text().replace('        check(bank.hasOutOfManaContexts()', '''        check(bank.hasGrenade()==(bank.version>=42),"grenade clips unavailable in older banks");
        if(bank.hasGrenade())for(unsigned c=251;c<260;++c)for(float a:{0.f,.5f,1.f}){
            const auto j=bank.sampleParameter(c,9,a);for(float v:j)check(std::isfinite(v),"grenade phase keeps finite wrist pose");
        }
        check(bank.hasOutOfManaContexts()''',1);p.write_text(s)
print('Grenade delayed release, stale state, source identity and packet isolation regressions added')
