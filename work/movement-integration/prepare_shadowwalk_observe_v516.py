from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Observe-NativeDoppelgangerAir-v506.ps1').read_text(encoding='utf-8-sig')
s=s.replace(",[ValidateRange(100,800)][int]$ReleaseAfterJumpMs=180",'')
s=s.replace('Doppelganger','ShadowWalk').replace('doppelganger','shadowwalk').replace('DOPPELGANGER','SHADOWWALK')
s=s.replace("'models/powers/shadowwalk/shadowwalk.inventoryitem'","'models/powers/shadow_walk/shadow_walk.inventoryitem'")
s=s.replace("'coop_shadowwalk_fixture off','coop_movement_fixture off'","'coop_doppelganger_fixture off','coop_shadowwalk_fixture off','coop_movement_fixture off'")
a=s.index('function OptionalFx');b=s.index('try{',a)
# Replace the entire original one-line optional FX function, not its internal try.
e=s.index('\n',a)
s=s[:a]+'''function O($r,$mode,$arg=0){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};$code=$LASTEXITCODE;if($code){return [pscustomobject]@{stable=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}'''+s[e:]
s=s.replace("'-t','16'","'-t','28'").replace("'--watch-midnight','15000'","'--watch-midnight','27000'")
a=s.index(' $strictMonitor=Start-Process');b=s.index(" C client 'coop_shadowwalk_fixture off'",a)
s=s[:a]+''' Q client '--object' $power.component|ConvertTo-Json -Depth 14|Set-Content "$out/component-before.json"
 Q client '--components' $power.handle|ConvertTo-Json -Depth 14|Set-Content "$out/components-before.json"
 $w.Restart();$cast=$false;$forward=$false;$back=$false;$entityNext=2000
 while($w.ElapsedMilliseconds-lt26000){
  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 220;$cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native Emily LT220 activation'})}
  $player=Q client '--player-states'
  $form=@($player.active|Where-Object vtableRva -eq '0x1c7ed28').Count-gt0
  if($form-and!$forward-and$w.ElapsedMilliseconds-ge4000){
   & "$lab/Send-D2Input.ps1" -Role client -LeftY 18000 -DurationMs 550|Out-Null;$forward=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Forward550ms only after observed exact native ShadowWalk state'})
  }
  if($form-and$forward-and!$back-and$w.ElapsedMilliseconds-ge5500){
   & "$lab/Send-D2Input.ps1" -Role client -LeftY -18000 -DurationMs 550|Out-Null;$back=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Backward550ms in observed native ShadowWalk state'})
  }
  $entities=$null;if($w.ElapsedMilliseconds-ge$entityNext){$entityNext=$w.ElapsedMilliseconds+2500;$entities=Q client '--entities'}
  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;player=$player;midnight=(Q client '--midnight');sword=(O client '--player-sword');power=(O client '--player-shadowwalk');component=(O client '--object' $power.component);entities=@($entities.entities|Where-Object {$_.definitionName-match'(?i)(shadow_walk|shadowwalk)'})})
  Start-Sleep -Milliseconds 30
 }
 I client;$manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after.json"
 $monitor.WaitForExit(4000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $samples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $scenes=@($samples|ForEach-Object instances|Where-Object {$_.state-eq3-and$_.asset-like'*power_shadow_walk_*'})
 $forms=@($rows|Where-Object {@($_.player.active|Where-Object vtableRva -eq '0x1c7ed28').Count})
 $last=$rows[-1].player
 [ordered]@{scenes=@($scenes|Group-Object asset,subscene|ForEach-Object {[ordered]@{scene=$_.Name;samples=$_.Count;durationMs=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStateSets=@($rows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);stances=@($rows.player.stance|Sort-Object -Unique);formSamples=$forms.Count;forwardInput=$forward;backwardInput=$back;observedEntityDefinitions=@($rows.entities.definitionName|Sort-Object -Unique);scope='Source-only native activation/form/movement/exit observation. No phase enum meaning, remote presentation or gameplay authority assumed.'}|ConvertTo-Json -Depth 14|Set-Content "$out/shadowwalk-observations.json"
 $scenes|ConvertTo-Json -Depth 16|Set-Content "$out/shadowwalk-scenes.json"
 if(!$forms.Count){throw 'Native ShadowWalk state not directly observed'}
 $checks.Add('Exact native ShadowWalk player state observed after normal LT input')
 if(!$scenes.Count){throw 'Native Shadow Walk scene not observed'}
 if(@($scenes|Where-Object {!$_.bindingsResearch.Count-or@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count}).Count){throw 'Observed Shadow Walk scene binding not reciprocal'}
 $checks.Add('Native Shadow Walk scenes observed with reciprocal bindings and recorded clocks')
 if(!$forward-or!$back){throw 'Bounded form movement inputs not exercised'}
 if(@($last.active|Where-Object vtableRva -eq '0x1c7ed28').Count-or!$last.defaultMode-or$last.stance-gt1-or[Math]::Abs([double]$last.position[2]-$context.position[2])-gt.3){throw 'Natural Shadow Walk exit to the same floor not confirmed'}
 $checks.Add('Bounded forward/backward inputs issued during exact form state; natural exit recovers normal mode on same floor')
 if($manaAfter.currentCandidate-ge$manaBefore.currentCandidate){throw 'Native mana expenditure not observed'}
 $checks.Add('Native mana decreases; replenishment prevents interpreting this as exact cost')
'''+s[b:]
# Remove Doppelganger-specific post-reload entity assertions, retain native item reset and health.
a=s.index("  $partnerHandle=");b=s.index("  $checks.Add(",a)
s=s[:a]+s[b:]
s=s.replace('Both isolated checkpoints restore original health/inventory and retire spawned ShadowWalk entities','Both isolated checkpoints restore original health/inventory and unowned Shadow Walk item')
s=s.replace('source=\'client\';airborne=$true;releaseAfterJumpMs=$ReleaseAfterJumpMs;checks=$checks',"source='client';checks=$checks")
s=s.replace('Native source Emily ShadowWalk aim then bounded jump/release observation. Native acquisition, controller input, raw scene clocks/state/equipment and spawned entity observations. AI held in the isolated Dunwall fixture; neither general shadowwalk behavior nor remote movement/VFX/gameplay verified. Checkpoint and protected save restoration mandatory.','Source-only native Shadow Walk acquisition, normal LT activation, conditional bounded movement, natural exit, scene/state/equipment observations. No remote form/VFX/gameplay binding. Checkpoint and protected saves must restore.')
s=s.replace('Exact Emily ShadowWalk item acquired through native inventory API; no direct spawn, AI or damage invocation','Exact Emily Shadow Walk item acquired through native inventory API; no direct state transition or damage invocation')
w=(W/'Run-DoppelgangerAirObserve-v506.ps1').read_text(encoding='utf-8-sig')
w=w.replace('native-doppelganger-air-release-v506','native-shadowwalk-standing-v516').replace('Observe-NativeDoppelgangerAir-v506','Observe-NativeShadowWalk-v516').replace('DoppelgangerObserve','ShadowWalkObserve')
w=w.replace(",[ValidateRange(100,800)][int]$ReleaseAfterJumpMs=180",'').replace(' -ReleaseAfterJumpMs $ReleaseAfterJumpMs','')
for p,text in [(N/'Observe-NativeShadowWalk-v516.ps1',s),(W/'Run-ShadowWalkObserve-v516.ps1',w)]:
    assert not p.exists(),p
    p.write_text(text,encoding='utf-8')
print('Prepared source-only Shadow Walk observation516, with mandatory isolated checkpoint restore.')
