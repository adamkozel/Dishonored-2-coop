from pathlib import Path
import json,hashlib
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab';W=R/'work/movement-integration';P=R/'work/natural-moveset'
def edit(p,old,new):
 s=p.read_text();assert s.count(old)==1,(p,old,s.count(old));p.write_text(s.replace(old,new),encoding='utf-8')
edit(S/'src/engine_bridge.cpp','#include "domino_world_stream.h"','#include "domino_world_stream.h"\n#include "possession_fx_stream.h"')
edit(S/'src/engine_bridge.cpp','        if(magic==0x44573244){','''        if(magic==0x50463244){
            if(messages[i]->m_cbSize==sizeof(CoopPossessionFxPacket)){CoopPossessionFxPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receivePossessionFx(p,tick);}
            else ++possessionFxRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x44573244){''')
edit(S/'src/engine_bridge.cpp','writeDominoWorld();writeDominoBeams();','writeDominoWorld();writeDominoBeams();writePossessionFx();')
edit(S/'src/engine_bridge.cpp','sampleDominoWorld(tick);presentDominoWorld(tick);','sampleDominoWorld(tick);presentDominoWorld(tick);samplePossessionFx(tick);presentPossessionFx(tick);servicePossessionTargetFx(tick);')
edit(S/'src/native_power_fx.h','        kind==21?','        kind==22?"models/effects/d2coop/possession_target_remote.fx":\n        kind==21?')
edit(S/'src/shadow_action_protocol_tests.cpp','#include "swarm_spawn_fx_protocol.h"','#include "swarm_spawn_fx_protocol.h"\n#include "possession_fx_protocol.h"')
edit(S/'src/shadow_action_protocol_tests.cpp','#include "swarm_spawn_fx_protocol_tests.h"','#include "swarm_spawn_fx_protocol_tests.h"\n#include "possession_fx_protocol_tests.h"')
edit(S/'src/shadow_action_protocol_tests.cpp','swarmPacketTests(h,c);swarmSpawnFxTests(h,c);','swarmPacketTests(h,c);swarmSpawnFxTests(h,c);possessionFxTests(h,c);')
manifest=json.loads((S/'research/asset-exports/possession-fx-v327/manifest.json').read_text(encoding='utf-8-sig'))
source=Path(next(x['ExportFile'] for x in manifest['Resources'] if x['Name']=='models/effects/gameplay/powers/possession/hosts_target/possession_target_human_01.fx'))
text=source.read_text();start=text.index('\t\t\t\tm_rumbles = {');brace=text.index('{',start);depth=1;end=brace+1
while depth:
 if text[end]=='{':depth+=1
 if text[end]=='}':depth-=1
 end+=1
asset=text[:start]+'\t\t\t\tm_rumbles = {\n\t\t\t\t\tnum = 0;\n\t\t\t\t}'+text[end:]
assert asset.count('.particledt')==2 and 'm_highMotor' not in asset and 'm_fullScreen' not in asset
dest=S/'assets/possession_target_remote.fx';dest.write_text(asset,encoding='utf-8')
(S/'assets/possession_target_remote.provenance.json').write_text(json.dumps(dict(source=str(source),sourceSha256=hashlib.sha256(source.read_bytes()).hexdigest().upper(),outputSha256=hashlib.sha256(dest.read_bytes()).hexdigest().upper(),conversion='Only m_rumbles replaced with num=0. Both native target particle declarations and native timing retained. New asset path; no original resource override. No audio/camera or NPC gameplay invocation.',leaseMs=300),indent=2),encoding='utf-8')
builder=(S/'Build-D2SwarmSpawnFxAssets.ps1').read_text().replace('swarm-spawn-fx-presentation','possession-target-fx-presentation').replace('swarm_spawn_remote','possession_target_remote').replace('remote Swarm summon particle and sound experiment','remote Possession target particles experiment')
(S/'Build-D2PossessionTargetFxAssets.ps1').write_text(builder,encoding='utf-8')
edit(W/'Invoke-NativeLabOperation.ps1',' [switch]$InstallSwarmSpawnFx,',' [switch]$InstallSwarmSpawnFx,\n [switch]$InstallPossessionTargetFx,')
edit(W/'Invoke-NativeLabOperation.ps1','if($InstallGrenadeFx-or$InstallMesmerizeVisual-or$InstallSwarmRatVisual-or$InstallSwarmSpawnFx){','if($InstallGrenadeFx-or$InstallMesmerizeVisual-or$InstallSwarmRatVisual-or$InstallSwarmSpawnFx-or$InstallPossessionTargetFx){')
edit(W/'Invoke-NativeLabOperation.ps1','  if($InstallSwarmSpawnFx){','  if($InstallPossessionTargetFx){& "$root/outputs/d2-coop-lab/Build-D2PossessionTargetFxAssets.ps1" -Install;if(!$?){throw "Possession FX installation failed"}}\n  if($InstallSwarmSpawnFx){')
s=(W/'Build-PossessionExit-v343.ps1').read_text().replace('possession-exit-','possession-target-fx-').replace('v343','v351').replace('Build343','Build351')
(W/'Build-PossessionTargetFx-v351.ps1').write_text(s,encoding='utf-8')
s=(P/'Observe-PossessionFx-v350.ps1').read_text()
s=s.replace("visibility=(S client '.possession-body.json')})","visibility=(S client '.possession-body.json');sourceFx=(S host '.possession-fx.json');peerFx=(S client '.possession-fx.json')})")
anchor=" $checks.Add('Scoped selected NPC generation, exact declaration, temporary FX actor and bucket lookup match independently inventoried native human target particles')";assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $sourceFx=S host '.possession-fx.json';$peerFx=S client '.possession-fx.json'
 $activeFx=@($rows|Where-Object {$_.peerFx.sourceId-ne0})
 [ordered]@{source=$sourceFx;peer=$peerFx;activeSamples=$activeFx.Count;sourceIds=@($activeFx.peerFx.sourceId|Sort-Object -Unique);nativeIds=@($activeFx.peerFx.nativeId|Sort-Object -Unique)}|ConvertTo-Json -Depth 8|Set-Content "$out/network-fx-summary.json"
 if($sourceFx.sent-lt3-or$peerFx.received-lt3-or$peerFx.started-ne1-or$peerFx.stopped-ne1-or$peerFx.refused-or$peerFx.retired-or$peerFx.sourceId-or$activeFx.Count-lt2){throw 'Remote native target particle lifecycle not demonstrated'}
 $checks.Add('Exact native human target FX travels through Steam; one native-derived no-rumble particle effect starts on owned Corvo presentation, updates and retires without NPC gameplay invocation')
''')
(P/'Test-PossessionTargetFx-v351.ps1').write_text(s,encoding='utf-8')
edit(W/'Invoke-NativeLabOperation.ps1','Observe-PossessionFx-v350.ps1','Test-PossessionTargetFx-v351.ps1')
print('Prepared native-derived remote Possession target particles and scoped Steam stream')
