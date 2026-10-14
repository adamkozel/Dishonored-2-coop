from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
def edit(name,old,new,count=1):
    p=P/name;s=p.read_text();assert s.count(old)==count,(name,old,s.count(old));p.write_text(s.replace(old,new))
edit('movement_bank.h','32+225*','32+229*')
edit('movement_bank.h','    bool hasCrouchedRanged()','    bool hasCarry()const{return version>=31;}\n    bool hasCrouchedRanged()')
edit('movement_bank.h','213,225};return v<=30','213,225,229};return v<=31')
edit('movement_bank.h','v<=30;++v','v<=31;++v')
edit('movement_bank.h','turn?std::fabs(f[0]-turnArc)>.0001f:terrain?', 'turn?std::fabs(f[0]-turnArc)>.0001f:(c==226||c==228)?(f[0]<.2f||f[0]>3.f):terrain?')
edit('movement_bank_tests.cpp','argc==1?1:30);format<=30','argc==1?1:31);format<=31')
edit('movement_bank_tests.cpp','                if(c>=141&&c<149)', '                if(c==226||c==228)put(bytes,at+16,1.f);\n                if(c>=141&&c<149)')
edit('movement_playback.h','    unsigned turnSerial=0;','''    unsigned carryVariant=0;std::uint64_t carryEvent=0;
    float carryAmount=0,carryWeight=0,carryMove=0,carryPhase[2]{};
    unsigned turnSerial=0;''')
edit('movement_playback.h','        RangedPresentation::Sample ranged{};','''        CarryPresentation::Sample carry{};float carryAmount=0;
        const bool carryValid=p.bank.hasCarry()&&shadowStream.carry.sample(pose.time,carry,carryAmount);
        // Initial carrier-only proof covers loaded idle/forward travel. Do not
        // masquerade unsupported pickup or release as a finished animation.
        const bool carryActive=carryValid&&(carry.variant==2||carry.variant==3)&&pose.motionValid&&(ground||crouched);
        if(combatDt>=0&&combatDt<.25f){
            if(carryActive){p.carryVariant=carry.variant;p.carryAmount=carryAmount;p.carryEvent=carry.event;
                const float dx=position[0]-p.clock.position[0],dy=position[1]-p.clock.position[1],distance=std::sqrt(dx*dx+dy*dy);
                if(p.carryWeight>.001f&&!pose.teleport&&distance<=6.f*combatDt+.01f)
                    for(unsigned i=0;i<2;++i)p.carryPhase[i]=std::fmod(p.carryPhase[i]+distance/p.bank.clips[226+i*2].stride,1.f);
            }
            p.carryWeight+=((carryActive?1.f:0.f)-p.carryWeight)*(1-std::exp(-combatDt/.10f));
            const float moving=carryActive&&carry.variant==3?(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.04f)/.3f)):0.f;
            p.carryMove+=(moving-p.carryMove)*(1-std::exp(-combatDt/.09f));
        }else p.carryWeight=p.carryMove=0;
        RangedPresentation::Sample ranged{};''')
edit('movement_playback.h','||traversalActive||powerActive)&&shadowStream.until','||traversalActive||powerActive||carryActive)&&shadowStream.until')
edit('movement_playback.h','||powerActive||rangedActive||','||powerActive||rangedActive||carryActive||')
edit('movement_playback.h','&&!powerActive&&!rangedActive))','&&!powerActive&&!rangedActive&&!carryActive))')
edit('movement_playback.h','        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());','''        if(p.bank.hasCarry()&&p.carryWeight>.001f){
            const auto stand=MovementBank::blend(p.bank.sample(225,j,p.carryAmount),p.bank.sample(226,j,p.carryPhase[0]),p.carryMove);
            const auto low=MovementBank::blend(p.bank.sample(227,j,p.carryAmount),p.bank.sample(228,j,p.carryPhase[1]),p.carryMove);
            v=MovementBank::blend(v,MovementBank::blend(stand,low,p.clock.stanceMix),p.carryWeight);
        }
        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());''')
edit('movement_playback.h',r'\"rangedEvent\":%llu}\n',r'\"rangedEvent\":%llu,\"carryVariant\":%u,\"carryWeight\":%.6f,\"carryMove\":%.6f,\"carryPhase\":%.6f,\"carryCrouchPhase\":%.6f,\"carryEvent\":%llu}\n')
edit('movement_playback.h','p.rangedWeight,p.rangedEvent);','p.rangedWeight,p.rangedEvent,p.carryVariant,p.carryWeight,p.carryMove,p.carryPhase[0],p.carryPhase[1],p.carryEvent);')
edit('shadow_stream.h',r'\"remotePhase\":%u}\n',r'\"remotePhase\":%u,\"carrySource\":%u,\"carryRemote\":%u,\"carrySourceIncarnation\":%u,\"carryRemoteIncarnation\":%u,\"carryLocalHandle\":%u,\"carrySpawner\":%llu,\"carryDefinition\":%llu,\"corpseMutations\":false}\n')
edit('shadow_stream.h','shadowStream.remoteVariant,shadowStream.remotePhase);','shadowStream.remoteVariant,shadowStream.remotePhase,shadowStream.sourceCarry,shadowStream.remoteCarry,shadowStream.sourceCarryIncarnation,shadowStream.remoteCarryIncarnation,shadowStream.remoteCarryLocalHandle,shadowStream.remoteCarrySpawner,shadowStream.remoteCarryDefinition);')
edit('shadow_action_protocol_tests.cpp','int main(){','''#include "carry_presentation_tests.h"
int main(){
    carryPresentationTests();''')
edit('shadow_action_protocol_tests.cpp','    {auto r=p;r.state=13;', '''    {auto r=p;r.state=14;r.swordState=1;r.carryVariant=2;r.carryDurationMs=3000;r.carryIncarnation=23;
      r.carrySpawner=carryIdentityKey("ld_spawner_alexi_escape");r.carryDefinition=carryIdentityKey("models/characters/def/story/alexi_mayhew.def");
      check(validShadowAction(r,c,h),"owned carry semantic with authored identity");auto bad=r;bad.carrySpawner=0;check(!validShadowAction(bad,c,h),"carry refuses orphan target");
      bad=r;bad.state=0;check(!validShadowAction(bad,c,h),"neutral action cannot hide carry target");
      bad=r;bad.swordState=2;check(!validShadowAction(bad,c,h),"carry requires holstered sword");
      bad=r;bad.version=28;check(!validShadowAction(bad,c,h),"prior wire layout cannot be interpreted as carry");
      bad=r;bad.targetWorldEpoch++;check(!validShadowAction(bad,c,h),"carry refuses old world target");
      r.role=1;r.character=2;r.sourceSession=22;r.targetSession=11;r.sourceWorldEpoch=7;r.targetWorldEpoch=3;
      check(validShadowAction(r,h,c),"Emily carry has identical scoped semantics without handle translation");
    }
    {auto r=p;r.state=13;''')
edit('world_identity_tests.cpp','#include "world_identity.h"','#include "world_identity.h"\n#include "carry_identity.h"')
edit('world_identity_tests.cpp','    auto changed=a;changed.d(0x400004000', '''    {
      const auto sk=carryIdentityKey(ac[0].spawnerName),dk=carryIdentityKey(ac[0].definition);NativeWorldNpc target{};
      auto read=[&](auto p,auto d,auto n){return b.read(p,d,n);};
      require(resolveCarryIdentity(read,base,game,bc,sk,dk,target)&&target.handle==0x70051,"carry target resolves different local generation by authored identity");
      require(!resolveCarryIdentity(read,base,game,bc,sk+1,dk,target),"unknown carry key refused");
      auto ambiguous=bc;ambiguous.push_back(bc[0]);require(!resolveCarryIdentity(read,base,game,ambiguous,sk,dk,target),"ambiguous carry target refused");
      auto wrong=bc;wrong[0].component++;require(!resolveCarryIdentity(read,base,game,wrong,sk,dk,target),"changed spawner component refused");
      b.d(actor+0xa8,0x70052);require(!resolveCarryIdentity(read,base,game,bc,sk,dk,target),"stale cached corpse generation refused");b.d(actor+0xa8,0x70051);
    }
    auto changed=a;changed.d(0x400004000''')
print('Carrier-only v31 playback and regressions added')
