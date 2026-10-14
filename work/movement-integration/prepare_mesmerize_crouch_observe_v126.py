from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeMesmerize-v72.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep(" C client 'setviewpos -33.5 -153 68.1 270 0'", " $dirty=$true;C client 'setviewpos -48.59 -155.07 68.1 90 0'")
at=' $hwnd=(Get-Process -Id $pids.client).MainWindowHandle.ToInt64();'
rep(at,r''' & "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null
 $w.Restart();do{$stance=Q client '--player-stance';if($stance.current-eq1){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt2200)
 $stance|ConvertTo-Json -Depth 7|Set-Content "$out/crouch-before.json"
 $nativeState=Q client '--player-states';$nativeState|ConvertTo-Json -Depth 9|Set-Content "$out/state-before.json"
 if($nativeState.stance-ne1-or!$nativeState.defaultMode-or!@($nativeState.active|Where-Object {$_.id-eq6-and$_.vtableRva-eq'0x1c84258'}).Count){throw 'Native crouch identity not established'}
 $checks.Add('Native controller B establishes crouched source context before any cast')
'''+at)
a=s.index(' $fastScenes=@(');b=s.index(' if($manaAfter.currentCandidate',a)
s=s[:a]+r''' $fastScenes=@(Get-Content "$out/power-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 # This take gathers raw scene/state evidence. The production strict reader
 # still deliberately rejects crouched Mesmerize until this record is audited.
 $rawScenes=@($rows|ForEach-Object {$_.midnight.instances}|Where-Object {$_.asset-like'*power_mesmerize_cast_*'})
 [ordered]@{strictAccepted=@($fastScenes|Where-Object valid).Count;raw=$rawScenes;playerStates=@($rows.player)}|ConvertTo-Json -Depth 14|Set-Content "$out/crouch-scene-observations.json"
 $checks.Add('Read-only native Midnight scenes, arm/equipment data and player state samples retained for crouch provenance analysis; remote integration not claimed')
'''+s[b:]
rep('Native source Mesmerize acquisition, controller casting, scenes and effects only.', 'Native crouched source Mesmerize acquisition and controller cast observation. Existing strict power reader deliberately remains standing-only; raw Midnight/player state evidence must be reviewed before enabling crouch transport.')
(P/'Observe-NativeMesmerizeCrouch-v126.ps1').write_text(s,encoding='utf-8')
