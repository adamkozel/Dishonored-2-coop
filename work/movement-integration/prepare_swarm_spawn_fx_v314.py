from pathlib import Path
import json,hashlib
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab';W=R/'work/movement-integration';P=R/'work/natural-moveset'
def edit(path,old,new):
 s=path.read_text();assert old in s,(path,old);path.write_text(s.replace(old,new))
edit(S/'src/swarm_world_stream.h','#include "native_swarm_world.h"','#include "native_swarm_world.h"\nvoid sampleSwarmSpawnFx(const NativeSwarmWorld&,const CoopSwarmWorldPacket&,ULONGLONG);')
edit(S/'src/swarm_world_stream.h','if(!s.sourceEvent&&!handle)return;','if(!s.sourceEvent&&!handle){sampleSwarmSpawnFx(world,{},tick);return;}')
edit(S/'src/swarm_world_stream.h','s.sent=p;recordSwarmWorld(p);++swarmWorldSent;}','s.sent=p;recordSwarmWorld(p);++swarmWorldSent;sampleSwarmSpawnFx(world,p,tick);}')
edit(S/'src/engine_bridge.cpp','#include "swarm_world_stream.h"','#include "swarm_world_stream.h"\n#include "swarm_spawn_fx_stream.h"')
edit(S/'src/engine_bridge.cpp','        if(magic==0x53573244){','''        if(magic==0x53463244){
            if(messages[i]->m_cbSize==sizeof(CoopSwarmSpawnFxPacket)){CoopSwarmSpawnFxPacket p{};std::memcpy(&p,messages[i]->m_pData,sizeof(p));receiveSwarmSpawnFx(p,tick);}
            else ++swarmSpawnFxRejected;
            messages[i]->Release();continue;
        }
        if(magic==0x53573244){''')
edit(S/'src/engine_bridge.cpp','    if(!std::strcmp(request.command,"coop_power_fx windblast_duplicate"))','    if(!std::strcmp(request.command,"coop_swarm_fx duplicate")){duplicateSwarmSpawnFx();return;}\n    if(!std::strcmp(request.command,"coop_power_fx windblast_duplicate"))')
edit(S/'src/engine_bridge.cpp','writeSwarmWorld();writeSwarmRatVisual();','writeSwarmWorld();writeSwarmSpawnFx();writeSwarmRatVisual();')
edit(S/'src/engine_bridge.cpp','presentSwarmWorld(tick);serviceSwarmRatVisual(tick);','presentSwarmWorld(tick);serviceSwarmRatVisual(tick);serviceSwarmSpawnFx(tick);')
edit(S/'src/native_power_fx.h','        kind==19?','        kind==20?"models/effects/d2coop/swarm_spawn_remote.fx":\n        kind==19?')
edit(S/'src/shadow_action_protocol_tests.cpp','#include "swarm_world_protocol.h"','#include "swarm_world_protocol.h"\n#include "swarm_spawn_fx_protocol.h"')
edit(S/'src/shadow_action_protocol_tests.cpp','#include "swarm_world_protocol_tests.h"','#include "swarm_world_protocol_tests.h"\n#include "swarm_spawn_fx_protocol_tests.h"')
edit(S/'src/shadow_action_protocol_tests.cpp','swarmPacketTests(h,c);','swarmPacketTests(h,c);swarmSpawnFxTests(h,c);')
source=next((S/'research/asset-exports/native-swarm-fx-v311').rglob('*spawn.rswarm_spawn_01..fx.decl'))
text=source.read_text();start=text.index('\t\t\t\tm_rumbles = {');brace=text.index('{',start);depth=1;end=brace+1
while depth:
 if text[end]=='{':depth+=1
 if text[end]=='}':depth-=1
 end+=1
asset=text[:start]+'\t\t\t\tm_rumbles = {\n\t\t\t\t\tnum = 0;\n\t\t\t\t}'+text[end:]
dest=S/'assets/swarm_spawn_remote.fx';dest.write_text(asset)
assert 'm_highMotor' not in asset and 'm_startSoundEvent' in asset and 'rswarm_spawn_01.particledt' in asset
(S/'assets/swarm_spawn_remote.provenance.json').write_text(json.dumps(dict(source=str(source),sourceSha256=hashlib.sha256(source.read_bytes()).hexdigest().upper(),outputSha256=hashlib.sha256(dest.read_bytes()).hexdigest().upper(),conversion='Only m_rumbles replaced with num=0. Native summon particle and sound declarations unchanged. No original resource override.',emitterDurationSeconds=1,receiverRecordLeaseMs=1600),indent=2))
builder=(S/'Build-D2GrenadeFxAssets.ps1').read_text().replace('grenade-fx-presentation','swarm-spawn-fx-presentation').replace('grenade_inhand_remote','swarm_spawn_remote').replace('remote grenade in-hand effect prop experiment','remote Swarm summon particle and sound experiment')
(S/'Build-D2SwarmSpawnFxAssets.ps1').write_text(builder)
p=W/'Invoke-NativeLabOperation.ps1'
edit(p,' [switch]$InstallSwarmRatVisual,',' [switch]$InstallSwarmRatVisual,\n [switch]$InstallSwarmSpawnFx,')
edit(p,'if($InstallGrenadeFx-or$InstallMesmerizeVisual-or$InstallSwarmRatVisual){','if($InstallGrenadeFx-or$InstallMesmerizeVisual-or$InstallSwarmRatVisual-or$InstallSwarmSpawnFx){')
edit(p,'  if($InstallSwarmRatVisual){','  if($InstallSwarmSpawnFx){& "$root/outputs/d2-coop-lab/Build-D2SwarmSpawnFxAssets.ps1" -Install}\n  if($InstallSwarmRatVisual){')
edit(p,'Test-SwarmRatVisualNetwork-v313.ps1','Test-SwarmRatVisualNetwork-v314.ps1')
s=(P/'Test-SwarmRatVisualNetwork-v313.ps1').read_text()
s=s.replace('$w.Restart();$cast=$false;$stood=$false;$releaseWalking=$false','$w.Restart();$cast=$false;$stood=$false;$releaseWalking=$false;$duplicateFx=$false')
needle="  $m=S client '.movement.json';$stream=S client '.shadow-stream.json'"
assert needle in s
s=s.replace(needle,"  if(!$duplicateFx-and$w.ElapsedMilliseconds-ge8000){C host 'coop_swarm_fx duplicate';$duplicateFx=$true}\n"+needle)
needle="visual=(S client '.swarm-rat-visual.json')})"
assert needle in s
s=s.replace(needle,"visual=(S client '.swarm-rat-visual.json');fxSource=(S host '.swarm-spawn-fx.json');fxPeer=(S client '.swarm-spawn-fx.json');nativeFxPeer=(S client '.power-fx.json')})")
needle=" $checks.Add('Swarm cast preserves root/body continuity without rejected presentation clocks')"
assert needle in s
s=s.replace(needle,needle+'''
 $fxSource=S host '.swarm-spawn-fx.json';$fxPeer=S client '.swarm-spawn-fx.json';$fxNative=S client '.power-fx.json'
 [ordered]@{source=$fxSource;peer=$fxPeer;nativePeer=$fxNative}|ConvertTo-Json -Depth 10|Set-Content "$out/spawn-fx-summary.json"
 if($fxSource.sent-ne1-or$fxPeer.applied-ne1-or$fxPeer.received-ne2-or$fxPeer.rejected-ne1-or$fxSource.reads-gt8){throw 'Swarm summon burst missing, repeated or duplicate not refused'}
 foreach($key in 'sequence','event','nativeFxId','worldEvent','worldHandle','powerEvent','sampleTimeUs','castTimeMs'){
  if($fxSource.$key-ne$fxPeer.$key){throw "Swarm FX packet differs at $key"}
 }
 foreach($key in 'position','axis'){for($j=0;$j-lt$fxSource.$key.Count;$j++){if([Math]::Abs($fxSource.$key[$j]-$fxPeer.$key[$j])-gt.000001){throw 'Swarm FX transform changed over Steam'}}}
 $ownedFx=@($worldRows|ForEach-Object{$_.nativeFxPeer.effects}|Where-Object { $_.kind -eq 20 })
 if(!$ownedFx.Count-or@($ownedFx.id|Sort-Object -Unique).Count-ne1-or$fxNative.active-ne0){throw 'Owned native summon FX lifecycle not observed'}
 $checks.Add('One exact source-owned summon FX record transmits through native Steam and starts one owned remote native particle/sound effect; duplicate rejected and finite effect retired')
''')
(P/'Test-SwarmRatVisualNetwork-v314.ps1').write_text(s)
print('Prepared314 source-owned finite Swarm burst pipeline and isolated native-derived no-rumble asset')
