"""One-time explicit protocol migration; retain prior candidate archives."""
from pathlib import Path
P=Path(__file__).resolve().parents[2]/'outputs/d2-coop-lab/src'
def edit(file,pairs):
 p=P/file;t=p.read_text()
 for a,b in pairs:
  assert a in t,(file,a);t=t.replace(a,b)
 p.write_text(t)
edit('ranged_presentation.h',[
 ('elapsed=0,duration=0;','elapsed=0,duration=0,mechanism=0;'),
 ('s.variant<=10&&','s.variant<=10&&s.mechanism<=2&&(!s.mechanism||s.kind==2)&&')])
edit('shadow_action_protocol.h',[
 ('version=25','version=26'),('==252','==256'),('p.version==25','p.version==26'),
 ('rangedTimeMs=0,rangedDurationMs=0;','rangedTimeMs=0,rangedDurationMs=0,rangedMechanism=0;'),
 ('p.rangedTimeMs,p.rangedDurationMs}', 'p.rangedTimeMs,p.rangedDurationMs,p.rangedMechanism}')])
edit('shadow_stream.h',[
 ('#include "native_player_ranged_scene.h"','#include "native_player_ranged_scene.h"\n#include "native_crossbow_presentation.h"'),
 ('airPacket->rangedDurationMs=ranged.durationMs;}', 'airPacket->rangedDurationMs=ranged.durationMs;\n            NativeCrossbowPresentation mechanical{};if(readNativeCrossbowPresentation(memoryRead,base,ranged.weapon,mechanical))airPacket->rangedMechanism=mechanical.state;}'),
 ('if(airPacket)airPacket->rangedKind=held.kind;', 'if(airPacket){airPacket->rangedKind=held.kind;NativeCrossbowPresentation mechanical{};\n                if(readNativeCrossbowPresentation(memoryRead,base,held,mechanical))airPacket->rangedMechanism=mechanical.state;}'),
 ('p.rangedTimeMs,p.rangedDurationMs}', 'p.rangedTimeMs,p.rangedDurationMs,p.rangedMechanism}')])
edit('movement_playback.h',[
 ('unsigned rangedKind=0,rangedVariant=0;', 'unsigned rangedKind=0,rangedVariant=0,rangedMechanism=0;'),
 ('p.rangedKind=ranged.kind;p.rangedVariant=ranged.variant;', 'p.rangedKind=ranged.kind;p.rangedVariant=ranged.variant;p.rangedMechanism=ranged.mechanism;')])
edit('ranged_prop.h',[
 ('presentedKind=0,presentedVariant=0;', 'presentedKind=0,presentedVariant=0,presentedMechanism=0;'),
 ('presentedAmount=movement.rangedAmount;', 'presentedAmount=movement.rangedAmount;presentedMechanism=movement.rangedMechanism;'),
 ('publishRangedMechanism(rangedProp.handle,presentedVariant,presentedAmount,rangedProp.until)', 'publishRangedMechanism(rangedProp.handle,presentedVariant,presentedAmount,presentedMechanism,rangedProp.until)')])
edit('ranged_mechanism.h',[
 ('unsigned variant,float phase,ULONGLONG until)', 'unsigned variant,float phase,unsigned mechanism,ULONGLONG until)'),
 ('const auto clip=crossbowMechanism(prop.character,variant);', 'if(variant!=7&&!mechanism){stopRangedMechanism();return true;} // Unknown native state supplies no guessed mechanical pose.\n    const auto clip=crossbowMechanism(prop.character,variant==7?7:mechanism==2?2:0);'),
 ('Idle/empty clips have constant mechanical poses.', 'Idle/fired selection follows the delayed native defaultAnim identity,\n    // including reload contact and an empty weapon held after the action ends.')])
edit('runtime_probe.cpp',[
 ('#include "native_player_ranged_item.h"','#include "native_player_ranged_item.h"\n#include "native_crossbow_presentation.h"'),
 ('const bool ammoValid=readNativeRangedAmmo', 'NativeCrossbowPresentation mechanical{};const bool mechanismValid=readNativeCrossbowPresentation([&](auto a,auto b,auto n){return read(process.value,a,b,n);},base,p,mechanical);\n        const bool ammoValid=readNativeRangedAmmo'),
 ('<<",\\\"ammoValid\\\":"', '<<",\\\"mechanismValid\\\":"<<(mechanismValid?"true":"false")<<",\\\"mechanism\\\":"<<mechanical.state<<",\\\"ammoValid\\\":"')])
print('Migrated to bounded native crossbow state; Steam protocol v26 / 256 bytes')
