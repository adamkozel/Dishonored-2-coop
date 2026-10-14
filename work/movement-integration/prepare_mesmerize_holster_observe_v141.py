"""Observe native Mesmerize use starting from holstered equipment."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeMesmerizeCrouch-v130.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep('[int]$Pitch=12)',"[int]$Pitch=12,[ValidateSet('standing','crouched')][string]$Stance='standing')")
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B')
b=s.index(' $hwnd=',a)
s=s[:a]+r''' if($Stance-eq'crouched'){
  & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500
 }
 $nativeState=Q client '--player-states';$nativeState|ConvertTo-Json -Depth 9|Set-Content "$out/state-before.json"
 $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($nativeState.stance-ne$expectedStance-or!$nativeState.defaultMode){throw 'Native stance not established'}
 $checks.Add('Selected native stance established before the equipment observation')
'''+s[b:]
rep('$w.Restart();$cast=$false','$w.Restart();$cast=$false;$holstered=$false')
rep('  if(!$cast-and$w.ElapsedMilliseconds-ge1100)',r'''  if(!$holstered-and$w.ElapsedMilliseconds-ge1000){
   & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;$holstered=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X held1000ms to holster equipment'})
  }
  if(!$cast-and$w.ElapsedMilliseconds-ge4500)''')
rep('player=(Q client \'--player-states\');midnight=$scene;',"player=(Q client '--player-states');sword=(Q client '--player-sword');midnight=$scene;")
a=s.index(' # This take gathers raw scene/state evidence.')
b=s.index(" C client 'coop_mesmerize_fixture off';",a)
s=s[:a]+r''' $rawMonitor.WaitForExit(4000)|Out-Null;if(!$rawMonitor.HasExited-or$rawMonitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $rawSamples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $rawScenes=@($rawSamples|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-like'*power_mesmerize_cast_*'})
 $holsteredRows=@($rows|Where-Object {$_.ms-gt2600-and$_.ms-lt4400-and!$_.sword.equipped})
 if(!$holstered-or!$holsteredRows.Count){throw 'Native X did not establish holstered sword before LT'}
 if(@($rows.player|Where-Object {$_.stance-ne$expectedStance-or!$_.defaultMode}).Count){throw 'Unexpected stance/mode during holstered observation'}
 $checks.Add('Native sword is confirmed holstered before LT, with native stance preserved throughout observation')
 foreach($scene in $rawScenes){if(@($scene.bindingsResearch).Count-ne1-or!$scene.bindingsResearch[0].reciprocal){throw 'Observed power phase has an unexpected native binding'}}
 $phaseAssets=@($rawScenes.asset|Select-Object -Unique)
 [ordered]@{stance=$Stance;strictAccepted=@($fastScenes|Where-Object valid).Count;nativePhaseAssets=$phaseAssets;holsteredBeforeUseSamples=$holsteredRows.Count;equippedAfterUseSamples=@($rows|Where-Object {$_.ms-gt5000-and$_.sword.equipped}).Count;manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;raw=$rawScenes;playerStates=@($rows.player)}|ConvertTo-Json -Depth 14|Set-Content "$out/holster-scene-observations.json"
 $checks.Add('Native phase, reciprocal arm, sword equipment and mana observations saved without inferring automatic draw or a successful cast')
'''+s[b:]
a=s.index("scope='")+len("scope='");b=s.index("'}|ConvertTo-Json",a)
s=s[:a]+"Source-only native holstered Mesmerize observation. Native X holsters, then one LT hold/release tests the actual draw/use behavior in the requested stance. Raw scene/equipment/mana evidence determines whether a cast occurs and which equipment context it uses. No remote integration or complete movement coverage claimed. Isolated native checkpoints and protected hashes restore."+s[b:]
(P/'Observe-NativeMesmerizeHolstered-v141.ps1').write_text(s,encoding='utf-8')
