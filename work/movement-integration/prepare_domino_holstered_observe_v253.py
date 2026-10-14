from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-NativeDominoCrouchSelection-v249.ps1').read_text(encoding='utf-8-sig')
s=s.replace('[switch]$AimGuard)',"[switch]$AimGuard,[ValidateSet('standing','crouched')][string]$Stance='standing')")
assert '$Stance=' in s[:400]
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B');b=s.index(' $hwnd=',a)
s=s[:a]+''' $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 500}
 $beforeHolster=Q client '--player-states'
 if($beforeHolster.stance-ne$expectedStance-or!$beforeHolster.motionValid-or$beforeHolster.motion-ne$expectedStance){throw 'Native stance not ready for holster observation'}
''' +s[b:]
s=s.replace('$w.Restart();$cast=$false','$w.Restart();$cast=$false;$holstered=$false')
s=s.replace('  if(!$cast-and$w.ElapsedMilliseconds-ge1100){','''  if(!$holstered-and$w.ElapsedMilliseconds-ge1000){
   & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;$holstered=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native X held1000ms to holster equipment'})
  }
  if(!$cast-and$w.ElapsedMilliseconds-ge4500){''')
s=s.replace("player=(Q client '--player-states');midnight=", "player=(Q client '--player-states');sword=(Q client '--player-sword');midnight=")
a=s.index(' $nativeCrouch=@(');b=s.index(" $checks.Add('Native player state",a)
s=s[:a]+''' $holsteredRows=@($rows|Where-Object {$_.ms-gt2600-and$_.ms-lt4400-and!$_.sword.equipped})
 if(!$holstered-or!$holsteredRows.Count){throw 'Native X did not establish holstered sword before LT'}
 if(@($rows|Where-Object {$_.player.stance-ne$expectedStance-or!$_.player.defaultMode}).Count){throw 'Unexpected stance/mode during holstered observation'}
 $castRows=@($rows|Where-Object {@($_.player.active|Where-Object id -eq 72).Count})
 [ordered]@{stance=$Stance;strictAccepted=@($strict|Where-Object valid).Count;holsteredBeforeUseSamples=$holsteredRows.Count;castingSamples=$castRows.Count;castingHolsteredSamples=@($castRows|Where-Object {!$_.sword.equipped}).Count;castingDrawnSamples=@($castRows|Where-Object {$_.sword.equipped}).Count;activeStateSets=@($castRows|ForEach-Object {($_.player.active.id|Sort-Object)-join','}|Sort-Object -Unique);phases=@($rawScenes|Group-Object subscene|ForEach-Object {[ordered]@{name=$_.Name;samples=$_.Count;duration=@($_.Group.sceneDurationMs|Select-Object -Unique)}});raw=$rawScenes;scope='Native holster then LT observation. Sword selection during cast is measured, not assumed; no remote binding.'}|ConvertTo-Json -Depth 16|Set-Content "$out/holstered-scene-observations.json"
 $checks.Add('Native holster before LT and stance confirmed; raw scene, sword selection, state and strict-reader acceptance during actual casting recorded without assuming automatic draw')

''' +s[b:]
s=s.replace("scope='Targeted crouched native Domino selection observation", "stance=$Stance;scope='Targeted native Domino holster and cast observation")
(P/'Observe-NativeDominoHolstered-v253.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoCrouchFinalNetwork'","'DominoObserve','DominoHolsteredObserve','DominoCrouchFinalNetwork'")
s=s.replace("  'DominoCrouchFinalObserve'{", "  'DominoHolsteredObserve'{& \"$root/work/natural-moveset/Observe-NativeDominoHolstered-v253.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance}\n  'DominoCrouchFinalObserve'{");p.write_text(s,encoding='utf-8')
print('Prepared standing/crouched holstered native observation v253.')
