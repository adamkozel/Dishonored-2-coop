"""One-time guarded integration of source-confirmed cosmetic muzzle flashes."""
from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
def edit(name, replacements):
 p=P/name;s=p.read_text()
 for old,new in replacements:
  assert old in s,(name,old)
  s=s.replace(old,new)
 p.write_text(s)
edit('shadow_action_protocol.h',[
 ('version=26','version=27'),('p.version==26','p.version==27'),('==256','==260'),
 ('rangedMechanism=0;','rangedMechanism=0,rangedShot=0;'),
 ('p.rangedDurationMs,p.rangedMechanism}', 'p.rangedDurationMs,p.rangedMechanism,p.rangedShot}')])
edit('ranged_presentation.h',[
 ('mechanism=0;','mechanism=0,shot=0;'),
 ('s.mechanism<=2&&','s.shot<=1&&(!s.shot||(s.kind==1&&s.variant==3))&&s.mechanism<=2&&')])
edit('shadow_stream.h',[
 ('#include "native_player_ranged_scene.h"','#include "native_player_ranged_scene.h"\n#include "ranged_shot_observation.h"\nRangedShotObservation nativeRangedShot;'),
 ('p.state=action;p.event=shadowStream.sourceEvent;p.leanSide=leanSide;p.leanUp=leanUp;', '''p.state=action;p.event=shadowStream.sourceEvent;p.leanSide=leanSide;p.leanUp=leanUp;
    std::vector<NativeWindblastFxRecord> muzzleRecords;
    if(readNativeRangedFxRecords(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),observedGame,muzzleRecords))
        p.rangedShot=nativeRangedShot.observe(shadowStream.token,shadowStream.peerToken,scene,localRole,
            action==13&&p.rangedKind==1&&p.rangedVariant==3&&p.rangedTimeMs<=666,outgoing.position,muzzleRecords);'''),
 ('p.rangedDurationMs,p.rangedMechanism}', 'p.rangedDurationMs,p.rangedMechanism,p.rangedShot}')])
edit('movement_playback.h',[
 ('rangedMechanism=0;', 'rangedMechanism=0,rangedShot=0;'),
 ('p.rangedMechanism=ranged.mechanism;', 'p.rangedMechanism=ranged.mechanism;p.rangedShot=ranged.shot;'),
 ('\\"rangedMechanism\\":%u,','\\"rangedShot\\":%u,\\"rangedMechanism\\":%u,'),
 ('p.landingCrouchMix,p.rangedMechanism,','p.landingCrouchMix,p.rangedShot,p.rangedMechanism,')])
edit('native_power_fx.h',[
 ('kind==8?"models/effects/d2coop/windblast_remote.fx":nullptr;', 'kind==8?"models/effects/d2coop/windblast_remote.fx":\n        kind==9?"models/effects/weapons/gun/muzzle_3p.fx":nullptr;'),
 ('e.expires=tick+1600;', 'e.expires=tick+(kind==9?2500:1600);')])
edit('engine_bridge.cpp',[
 ('#include "ranged_prop.h"', '#include "ranged_prop.h"\n#include "ranged_muzzle.h"'),
 ('serviceNativeWindblastFx(tick);nativeMutationAllowed=false;writeStatus(tick);', 'serviceNativeWindblastFx(tick);serviceRangedMuzzle(tick);nativeMutationAllowed=false;writeStatus(tick);')])
edit('shadow_action_protocol_tests.cpp',[
 ('#include "windblast_fx_protocol.h"','#include "windblast_fx_protocol.h"\n#include "ranged_shot_observation.h"'),
 ('int main(){','int main(){\n    rangedShotTests();'),
 ('bad.version=25;','bad.version=26;')])
