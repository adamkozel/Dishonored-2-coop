from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
s=(P/'Observe-NativeDomino-v189.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$hwnd=(Get-Process", """& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null
 Start-Sleep -Milliseconds 500
 $crouch=Q client '--player-states';$crouch|ConvertTo-Json -Depth 10|Set-Content "$out/crouch-before.json"
 if($crouch.stance-ne1-or!$crouch.motionValid-or$crouch.motion-ne1-or!@($crouch.active|Where-Object {$_.id-eq6-and$_.vtableRva-eq'0x1c84258'}).Count){throw 'Native crouch identity not grounded before observation'}
 $hwnd=(Get-Process""",1)
start=s.index(" $strict=@(Get-Content")
end=s.index(" $checks.Add('Native player state",start)
s=s[:start]+''' $strict=@(Get-Content "$out/domino-scenes.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $nativeCrouch=@($rows|Where-Object {$_.player.stance-eq1-and$_.player.defaultMode-and@($_.player.active|Where-Object {$_.id-eq6-and$_.vtableRva-eq'0x1c84258'}).Count-and@($_.player.active|Where-Object {$_.id-eq72-and$_.vtableRva-eq'0x1c3eeb8'}).Count-and!@($_.player.active|Where-Object {$_.id-notin6,72}).Count})
 if($nativeCrouch.Count-lt3){throw 'Too few independent native crouched cast observations'}
 foreach($phase in 'Cast_In','Cast_Loop','Cast_Select_out'){if(!@($rawScenes|Where-Object subscene -eq $phase).Count){throw "Native crouched phase $phase not observed"}}
 if(@($strict|Where-Object valid).Count){throw 'Standing-only strict reader unexpectedly accepts unimplemented crouch context'}
 [ordered]@{rawSceneSamples=$rawScenes.Count;crouchedCastingSamples=$nativeCrouch.Count;productionAccepted=@($strict|Where-Object valid).Count;activeStateSets=@($nativeCrouch|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);scope='Native input/raw scene provenance only; no remote crouch binding or visual acceptance'}|ConvertTo-Json -Depth 8|Set-Content "$out/crouched-phase-summary.json"
 $checks.Add('Native crouch plus state72 independently recorded with raw In/Loop/Out phases and reciprocal bindings; current standing-only production reader correctly refuses this context')

''' + s[end:]
s=s.replace("foreach($r in 'host','client'){try{I $r;C $r 'coop_domino_fixture off';C $r 'coop_movement_fixture off'}catch{}}", "foreach($r in 'host','client'){try{I $r;C $r 'coop_domino_fixture off'}catch{}}")
s=s.replace(' $rows|ConvertTo-Json -Depth 20'," foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}\n $rows|ConvertTo-Json -Depth 20")
s=s.replace('Initial native Domino observation:', 'Crouched native Domino observation (no remote binding):')
(P/'Observe-NativeDominoCrouch-v245.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoNetworkObserve'","'DominoObserve','DominoCrouchObserve','DominoNetworkObserve'")
s=s.replace('Observe-NativeDominoCrouch-v240.ps1','Observe-NativeDominoCrouch-v245.ps1').replace('Observe-NativeDominoCrouch-v242.ps1','Observe-NativeDominoCrouch-v245.ps1')
p.write_text(s,encoding='utf-8')
