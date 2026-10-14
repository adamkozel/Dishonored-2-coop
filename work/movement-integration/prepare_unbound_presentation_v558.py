from pathlib import Path
import shutil
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';B=W/'unbound-presentation-pre-v558'
B.mkdir(exist_ok=False)
names=['shadow_stream.h','shadow_action_protocol.h','movement_semantics.h','movement_playback.h','engine_bridge.cpp','shadow_action_protocol_tests.cpp']
for n in names:shutil.copy2(S/n,B/n)
def change(n,old,new):
 p=S/n;s=p.read_text(encoding='utf-8-sig');assert s.count(old)==1,(n,old[:100],s.count(old));p.write_text(s.replace(old,new),encoding='utf-8')
change('shadow_action_protocol.h','version=58,size=','version=59,size=')
change('shadow_action_protocol.h','p.version==58','p.version==59')
change('shadow_action_protocol.h','p.state<=17','p.state<=18')
change('shadow_action_protocol.h','// Native semantic states only. No pointers, resource names or damage requests.',
 '// Native semantic states and explicit unbound ground presentation18. No pointers, resource names or damage requests.')
change('shadow_action_protocol.h','return possession&&stun&&grenade&&carry&&ranged&&power&&traversal&&equipment&&combat&&',
 '''// Unbound presentation can never carry a gesture, target, effect or air event.
    const bool unbound=p.state!=18||(p.kind==1&&!p.combatVariant&&!p.equipmentVariant&&!p.traversalVariant&&
        !p.powerVariant&&!p.rangedKind&&!p.grenadeHeld&&!p.carryVariant&&!p.stunVariant&&!p.airStage);
    return unbound&&possession&&stun&&grenade&&carry&&ranged&&power&&traversal&&equipment&&combat&&''')
change('movement_semantics.h','s.state>17','s.state>18')
change('movement_playback.h','const bool actionEligible=(semantic.state==0||','const bool actionEligible=(semantic.state==0||semantic.state==18||')
change('shadow_stream.h','#include "native_player_sword.h"','#include "native_player_sword.h"\n#include "native_presentation_fallback.h"')
change('shadow_stream.h','unsigned shadowSourceReadStage=0,shadowSourceLastFailure=0;',
 '''unsigned shadowSourceReadStage=0,shadowSourceLastFailure=0;
std::uint64_t shadowUnboundSamples=0,shadowUnboundRefused=0;
ULONGLONG shadowUnboundProbeUntil=0; // Explicit bounded missing-binding test, off by default.''')
change('shadow_stream.h','shadowSourceReadStage=shadowSourceLastFailure=0;shadowSourceFailureCounts={};',
 'shadowSourceReadStage=shadowSourceLastFailure=0;shadowSourceFailureCounts={};shadowUnboundSamples=shadowUnboundRefused=0;')
change('shadow_stream.h','shadowStream.until=0;shadowStream.peerReady=false;','shadowUnboundProbeUntil=0;shadowStream.until=0;shadowStream.peerReady=false;')
change('shadow_stream.h','const bool sourceValid=shadowSourceAction(action,&leanSide,&leanUp,&p,&scene);',
 '''bool sourceValid=shadowSourceAction(action,&leanSide,&leanUp,&p,&scene);
    // Fault injection suppresses only this cosmetic equipment binding; the
    // actual native action, controls, root and inventory remain untouched.
    if(sourceValid&&action==7&&tick<shadowUnboundProbeUntil){sourceValid=false;shadowSourceReadStage=5;}''')
change('shadow_stream.h','++shadowSourceFailureCounts[(std::min)(7u,shadowSourceReadStage)];return;} // Missing native evidence never becomes an inferred action.',
 '''++shadowSourceFailureCounts[(std::min)(7u,shadowSourceReadStage)];
        unsigned swordState=0;
        if(!readNativePresentationFallback(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,observedPlayer,localRole+1,swordState)){
            ++shadowUnboundRefused;return;
        }
        // Discard every partially populated action field. Only independently
        // revalidated equipment and ground locomotion cross this path.
        p=shadowStreamPacket(1);p.swordState=swordState;action=18;scene=0;leanSide=leanUp=0;++shadowUnboundSamples;
    }''')
change('shadow_stream.h','const bool ground=p.state==0||p.state==4||p.state==5;','const bool ground=p.state==0||p.state==4||p.state==5||p.state==18;')
change('shadow_stream.h','fputs("\\\"sourceReadTimings\\\":{",f);',
 'fprintf(f,"\\\"unboundSamples\\\":%llu,\\\"unboundRefused\\\":%llu,\\\"unboundProbeActive\\\":%s,",shadowUnboundSamples,shadowUnboundRefused,tick<shadowUnboundProbeUntil?"true":"false");\n    fputs("\\\"sourceReadTimings\\\":{",f);')
change('engine_bridge.cpp','if(!std::strcmp(request.command,"coop_shadow_stream_duplicate")){duplicateShadowStream();return;}',
 '''if(!std::strcmp(request.command,"coop_shadow_stream_duplicate")){duplicateShadowStream();return;}
    if(!std::strcmp(request.command,"coop_shadow_unbound_probe 6000")){shadowUnboundProbeUntil=shadowStream.until?GetTickCount64()+6000:0;return;}
    if(!std::strcmp(request.command,"coop_shadow_unbound_probe off")){shadowUnboundProbeUntil=0;return;}''')
change('engine_bridge.cpp','if(!std::strcmp(text,"coop_shadow_stream 15000")',
 'if(!std::strcmp(text,"coop_shadow_unbound_probe 6000")||!std::strcmp(text,"coop_shadow_unbound_probe off"))return true;\n    if(!std::strcmp(text,"coop_shadow_stream 15000")')
change('shadow_action_protocol_tests.cpp','#include "movement_semantics.h"','#include "movement_semantics.h"\n#include "native_presentation_fallback.h"')
change('shadow_action_protocol_tests.cpp','int main(){','''void unboundGroundTests(){
    const std::uintptr_t base=0x140000000;NativePlayerStates s{};s.defaultMode=true;s.player.logic=1;
    s.active={{0,1,base+0x1c85628},{53,2,base+0x1c79cf8}};
    check(nativePresentationFallbackGround(s,base),"owned standing physical mode allows unbound equipment presentation");
    s.player.current=1;s.active[0]={6,1,base+0x1c84258};
    check(nativePresentationFallbackGround(s,base),"owned crouched physical mode allows unbound equipment presentation");
    s.defaultMode=false;check(!nativePresentationFallbackGround(s,base),"alternate power/cutscene mode cannot become ground fallback");s.defaultMode=true;
    s.player.logic=2;check(!nativePresentationFallbackGround(s,base),"paused native state cannot become fallback");s.player.logic=1;
    s.active[1].vtable++;check(!nativePresentationFallbackGround(s,base),"unknown native class still rejected");s.active[1].vtable--;
    s.player.current=0;s.active={{14,1,base+0x1c86768}};check(!nativePresentationFallbackGround(s,base),"stun has no confirmed ground locomotion fallback");
    s.active={{2,1,base+0x1c85028}};check(!nativePresentationFallbackGround(s,base),"airborne state cannot become ground fallback");
    unsigned sword=99;check(!readNativePresentationFallback([](auto,auto,auto){return false;},base,1,2,1,sword)&&sword==99,"failed owner read cannot publish fallback equipment");
    MovementSemantics timeline;MovementSemantics::Sample sample{};sample.time=100;sample.event=1;sample.state=18;
    check(timeline.push(sample),"explicit unbound state accepted on normal delayed movement timeline");
    sample.time=133;sample.airStage=2;check(!timeline.push(sample),"unbound state cannot smuggle airborne semantics");
}
int main(){unboundGroundTests();''')
build=(W/'Build-ShadowWalkSneak-v550.ps1').read_text(encoding='utf-8-sig').replace('v550','v558').replace('Build550','Build558')
(W/'Build-UnboundPresentation-v558.ps1').write_text(build,encoding='utf-8')
print('Prepared protocol59 explicit unbound ground presentation; bank59 unchanged')
