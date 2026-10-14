from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
p=P/'grenade_prop.h';s=p.read_text()
def sub(a,b):
 global s
 assert s.count(a)==1,(a[:80],s.count(a));s=s.replace(a,b)
sub('#include "grenade_prop_calibration.h"','#include "grenade_prop_calibration.h"\n#include "grenade_hand_tracks.h"')
sub('void serviceGrenadeProp(ULONGLONG tick){','''bool updateGrenadePropTrack(unsigned variant,float amount){
    auto& p=grenadeProp;
    if(variant>8||!rangedPropIdentityFor(p,true)||!rangedPropApiMatches())return false;
    const auto& track=grenadeHandTracks[localRole?0:1][variant];
    const float f=(std::max)(0.f,(std::min)(1.f,amount))*119;const auto a=unsigned(f),b=(std::min)(119u,a+1);
    MovementBank::Joint x{},y{};std::copy(track[a],track[a]+7,x.begin());std::copy(track[b],track[b]+7,y.begin());
    const auto q=MovementBank::blend(x,y,f-a);std::array<float,12> qts{};std::copy(q.begin(),q.end(),qts.begin());qts[8]=qts[9]=qts[10]=1;
    BodyMatrix m{};if(!bodyFromQts(qts,m))return false;
    float offset[3]{m.v[12],m.v[13],m.v[14]},axis[9]{};for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)axis[c*3+r]=m.v[c*4+r];
    reinterpret_cast<void(*)(void*,void*,const char**,const float*,const float*,bool)>(gameImage+0x875e80)(reinterpret_cast<void*>(p.parent),reinterpret_cast<void*>(p.actor),&p.tag,offset,axis,true);
    return rangedPropIdentityFor(p,true);
}
void serviceGrenadeProp(ULONGLONG tick){''')
sub('auto& p=grenadeProp;const auto request=grenadePropRequest;grenadePropRequest=0;', '''auto& p=grenadeProp;auto request=grenadePropRequest;grenadePropRequest=0;
    unsigned variant=0;float amount=0;bool presented=false;
    AcquireSRWLockShared(&movementLock);const auto& movement=movementPlayback;
    if(movement.end>tick&&movement.lease>tick&&movement.bank.hasGrenade()&&movement.clock.weight>.5f&&movement.grenadeWeight>.1f&&
       movement.actor==probeActor&&movement.game==observedGame&&movement.epoch==outgoing.worldEpoch&&
       shadowStreamCurrent(tick)&&shadowStream.peerReady&&shadowStream.lastState&&tick-shadowStream.lastState<250){
        variant=movement.grenadeVariant;amount=movement.grenadeAmount;presented=GrenadePresentation::visible(movement.grenadeHeld,variant,amount);
    }
    ReleaseSRWLockShared(&movementLock);
    const bool automatic=presented&&!request;
    if(p.network&&!automatic)hideGrenadeProp();
    if(automatic&&p.network&&p.until)p.until=tick+250;''')
sub('    if(request!=1)return;', '''    if(automatic&&!p.until&&!p.pendingHide)request=1;
    if(automatic&&p.network&&p.until){
        if(!updateGrenadePropTrack(variant,amount)){hideGrenadeProp();++grenadePropRefused;grenadePropResult="network_attachment_changed";}
        return;
    }
    if(request!=1)return;''')
sub('observerIsolation.until<tick+16000','observerIsolation.until<tick+(automatic?300:16000)')
sub('    p.until=tick+15000;grenadePropResult="native_grenade_prop_attached";', '''    p.network=automatic;p.until=tick+(automatic?250:15000);
    if(automatic&&!updateGrenadePropTrack(variant,amount)){hideGrenadeProp();refuse("network_track_unconfirmed");return;}
    grenadePropResult="native_grenade_prop_attached";''')
sub('    fprintf(f,"{\\"testFixtureOnly\\":true,','    fprintf(f,"{\\"network\\":%s,",p.network?"true":"false");\n    fprintf(f,"\\"testFixtureOnly\\":true,')
p.write_text(s)
p=P/'movement_playback.h';s=p.read_text();needle='    fprintf(f,"\\"carrySampleAmount\\"';at=s.index(needle)
s=s[:at]+'''    fprintf(f,"\\"grenadeHeld\\":%u,\\"grenadeVariant\\":%u,\\"grenadeAmount\\":%.7f,\\"grenadeWeight\\":%.7f,\\"grenadeEvent\\":%llu,",p.grenadeHeld,p.grenadeVariant,p.grenadeAmount,p.grenadeWeight,p.grenadeEvent);
'''+s[at:];p.write_text(s)
p=P/'movement_bank_tests.cpp';s=p.read_text().replace('(argc==1?1:41);format<=41','(argc==1?1:42);format<=42');p.write_text(s)
print('Grenade cosmetic prop now consumes guarded delayed movement state')
