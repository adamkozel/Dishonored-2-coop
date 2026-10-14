from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-DominoCrouchNetwork-v250.ps1').read_text(encoding='utf-8-sig')
s=s.replace('[int]$Pitch=12)',"[int]$Pitch=12,[ValidateSet('standing','crouched')][string]$Stance='standing',[switch]$Holstered,[ValidateRange(0,12000)][int]$Move=0)")
a=s.index(' & "$lab/Send-D2Input.ps1" -Role client -Button B');b=s.index(' $hwnd=',a)
s=s[:a]+''' $expectedStance=if($Stance-eq'crouched'){1}else{0}
 if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 300}
 if($Holstered){
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch -60 -Evidence "$out/holster-look-away.json" -Quick
  & "$lab/Send-D2Input.ps1" -Role client -Button X -DurationMs 1000|Out-Null;Start-Sleep -Milliseconds 1200
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 12 -Evidence "$out/holster-look-restored.json" -Quick
 }
 $w.Restart();$setupRows=[Collections.Generic.List[object]]::new()
 do{
  $m=S host '.movement.json';$native=Q client '--player-states';$nativeSword=Q client '--player-sword'
  $setupRows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;native=$native;sword=$nativeSword;movement=$m;source=(S client '.shadow-stream.json');peer=(S host '.shadow-stream.json')})
  if($native.stance-eq$expectedStance-and[Math]::Abs($m.stanceMix-$expectedStance)-lt.02-and$m.active-and$m.presentedDrawn-ne[bool]$Holstered-and$nativeSword.equipped-ne[bool]$Holstered-and[Math]::Abs($m.armedMix-$(if($Holstered){0}else{1}))-lt.02){break};Start-Sleep -Milliseconds 40
 }while($w.ElapsedMilliseconds-lt4500)
 $setupRows|ConvertTo-Json -Depth 14|Set-Content "$out/context-setup.json"
 if($native.stance-ne$expectedStance-or[Math]::Abs($m.stanceMix-$expectedStance)-gt.02-or!$m.active-or$m.presentedDrawn-eq[bool]$Holstered-or$nativeSword.equipped-eq[bool]$Holstered){throw 'Native/remote stance and equipment context not ready'}
 [ordered]@{stance=$Stance;holstered=[bool]$Holstered;move=$Move;native=$native;sword=$nativeSword;remote=$m}|ConvertTo-Json -Depth 10|Set-Content "$out/context-ready.json"
 $checks.Add('Native controller stance and holster inputs establish the same complete-body remote context before casting')
''' +s[b:]
s=s.replace('coop_domino_fixture 20000','coop_domino_fixture 40000')
s=s.replace('$cast=$false;$stood=$false','$cast=$false;$stood=$false;$releaseWalking=$false')
s=s.replace('   I client 255 1800;', '   & "$lab/Send-D2Input.ps1" -Role client -Button Neutral -LeftY $Move -LeftTrigger 255 -DurationMs 1800|Out-Null;')
s=s.replace("input='Native LT held 1800ms then released'", 'input="Native $Stance LT1800 with LeftY=$Move; holstered=$Holstered"')
s=s.replace('  if(!$stood-and$w.ElapsedMilliseconds-ge6500)', '''  if($Move-and$cast-and!$releaseWalking-and$w.ElapsedMilliseconds-ge3000){& "$lab/Send-D2Input.ps1" -Role client -Button Neutral -LeftY $Move -DurationMs 1400|Out-Null;$releaseWalking=$true}
  if(!$stood-and$w.ElapsedMilliseconds-ge6500)''')
s=s.replace('{$'+'lab', '{$'+'lab')
s=s.replace('{& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null;$stood=$true;', '{if($expectedStance){& "$lab/Send-D2Input.ps1" -Role client -Button B -DurationMs 180|Out-Null};$stood=$true;')
s=s.replace("player=(Q client '--player-states');movement=", "player=(Q client '--player-states');sword=(Q client '--player-sword');movement=")
s=s.replace("if(!$sword.equipped-or!(Q client '--player-sword').equipped)","if($sword.equipped-eq[bool]$Holstered-or(Q client '--player-sword').equipped-eq[bool]$Holstered)")
s=s.replace('$_.player.stance-ne1-or$_.movement.stanceMix-lt.98', '$_.player.stance-ne$expectedStance-or[Math]::Abs($_.movement.stanceMix-$expectedStance)-gt.02-or$_.movement.presentedDrawn-eq[bool]$Holstered-or$_.sword.equipped-eq[bool]$Holstered')
s=s.replace("throw 'Cast left native or complete-body remote crouched context'", "throw 'Cast left native/remote stance or sword context'")
s=s.replace('observedPhases=$phases;', 'stance=$Stance;holstered=[bool]$Holstered;move=$Move;observedPhases=$phases;')
s=s.replace('crouchedPhaseSamples=$castRows.Count;', 'contextPhaseSamples=$castRows.Count;')
s=s.replace(" $checks.Add('All observed native phases retain whole-body crouched context", " if($Move){$moving=@($castRows|Where-Object {$_.movement.speed-gt.15});if($moving.Count-lt5){throw 'No sustained native moving cast on peer'};$checks.Add('At least five remote cast samples retain native stance/equipment while moving above0.15m/s')}\n $checks.Add('All observed native phases retain whole-body native stance/equipment context")
s=s.replace("source='client';checks=$checks", "source='client';stance=$Stance;holstered=[bool]$Holstered;move=$Move;checks=$checks")
s=s.replace('crouched controller casting drives observed armed Emily complete-body crouched phases', 'native controller casting drives observed Emily complete-body stance/equipment phases')
s=s.replace('This take permits native target selection and records its effects.', 'Explicit stance/holstered/move parameters define this take. It permits native target selection and records its effects. Holstered final selection is still unobserved and refused.')
(P/'Test-DominoContextNetwork-v258.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoHolsteredObserve'","'DominoObserve','DominoContext','DominoHolsteredObserve'")
s=s.replace("  'DominoHolsteredObserve'{", "  'DominoContext'{& \"$root/work/natural-moveset/Test-DominoContextNetwork-v258.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}\n  'DominoHolsteredObserve'{");p.write_text(s,encoding='utf-8')
s=(W/'Archive-DominoCrouchFinal-v252.ps1').read_text(encoding='utf-8-sig').replace('candidate-domino-crouch-final-v252','candidate-domino-context-v258').replace('domino-crouch-final-','domino-context-').replace('-v252.log','-v258.log').replace('prepare_domino_crouch_final_v252.py','prepare_domino_context_network_v258.py').replace('Test-DominoCrouchFinalNetwork-v252.ps1','Test-DominoContextNetwork-v258.ps1')
a=s.index("scope='");b=s.index("'}|ConvertTo-Json",a)
s=s[:a]+"scope='Native v253 standing/crouched holster observations retain sword unequipped while exact Domino In/Loop/Select/Out scenes and state72 run. Reader now allows ordinary holstered phases with unchanged exact item, reciprocal arm/body and state/clock guards. Holstered final selection still unobserved/refused. Context playback enables existing bank48 empty-hand clips; old bank47 remains standing armed only. Bank48/301 unchanged and prior full format/actual-bank checks retained. Armed crouch final recovery from candidate252/v256 retained. Build, identity, wire and load checks pass; paired holstered/moving contexts pending.'"+s[b+1:]
(W/'Archive-DominoContext-v258.ps1').write_text(s,encoding='utf-8')
print('Prepared native Domino stance/equipment/movement harness and candidate archive v258.')
