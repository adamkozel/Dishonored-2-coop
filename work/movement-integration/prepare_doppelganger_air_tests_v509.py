from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
def once(s,a,b):
    assert s.count(a)==1,a
    return s.replace(a,b)
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=(N/'Test-DoppelgangerHolstered-v496.ps1').read_text(encoding='utf-8-sig')
s=once(s,"[ValidateSet('standing','crouched')][string]$Stance='standing')", "[ValidateSet('entry','release')][string]$AirMode='release',[switch]$Holstered)")
a=s.index(' $expectedStance=');b=s.index(' $w.Restart();$setupRows=',a)
block=s[a:b];h=block.index(' & {');block=block[h:].replace(' & {',' if($Holstered){',1)
s=s[:a]+" $expectedStance=0;$Stance='standing';$expectedDrawn=!$Holstered\n"+block+s[b:]
s=s.replace('$m.presentedDrawn-ne$true-and$nativeSword.equipped-ne$true-and[Math]::Abs($m.armedMix)-lt.02', '$m.presentedDrawn-eq$expectedDrawn-and$nativeSword.equipped-eq$expectedDrawn-and[Math]::Abs($m.armedMix-[int]$expectedDrawn)-lt.02')
s=s.replace('$m.presentedDrawn-eq$true-or$nativeSword.equipped-eq$true', '$m.presentedDrawn-ne$expectedDrawn-or$nativeSword.equipped-ne$expectedDrawn')
s=s.replace('holstered=$true','holstered=[bool]$Holstered')
s=s.replace('Native controller stance and holster inputs establish the same complete-body remote context before casting','Native standing equipment context matches the remote full body before airborne casting')
s=once(s,'$cast=$false;$stood=$false;', '$cast=$false;$jumped=$false;')
s=s.replace('$sceneNext=2900', "$sceneNext=$(if($AirMode-eq'entry'){1700}else{2600})")
a=s.index('  if(!$cast-and');b=s.index('  if($cast-and!$cloneSceneCaptured',a)
s=s[:a]+'''  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   if($AirMode-eq'entry'){
    & "$lab/Send-D2Input.ps1" -Role client -Button A -LeftTrigger 255 -DurationMs 700|Out-Null;$jumped=$true
   }else{I client 255 1800}
   $cast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input="Native $AirMode airborne cast; holstered=$Holstered"})
  }
  if($AirMode-eq'release'-and$cast-and!$jumped-and$w.ElapsedMilliseconds-ge2300){
   & "$lab/Send-D2Input.ps1" -Role client -Button A -LeftTrigger 255 -DurationMs 180|Out-Null;$jumped=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native A+LT180, then release while airborne'})
  }
'''+s[b:]
a=s.index(' $contextRows=');b=s.index(' $sourceStream=',a)
s=s[:a]+''' $contextRows=@($rows|Where-Object {$_.movement.powerVariant-in29,30,31-and$_.movement.powerWeight-gt.5})
 if(!$contextRows.Count-or@($contextRows|Where-Object {$_.player.stance-ne0-or$_.movement.stanceMix-gt.05-or$_.movement.presentedDrawn-ne$expectedDrawn-or[Math]::Abs($_.movement.armedMix-[int]$expectedDrawn)-gt.05-or$_.sword.equipped-ne$expectedDrawn}).Count){throw 'Airborne cast changed source/remote equipment context'}
 $nativeAir=@($scenes|Where-Object {2-in$_.activeStates-or3-in$_.activeStates})
 $playedAir=@($contextRows|Where-Object {$_.movement.airWeight-gt.5})
 $wanted=if($AirMode-eq'entry'){29}else{31}
 if(!@($nativeAir|Where-Object variant -eq $wanted).Count-or!@($playedAir|Where-Object {$_.movement.powerVariant-eq$wanted}).Count){throw 'Expected phase does not overlap independent source air state and remote air playback'}
 $standing=@($rows|Where-Object {$_.ms-ge6500-and$_.player.motionValid-and$_.player.motion-eq0-and$_.movement.airWeight-lt.02-and$_.movement.powerWeight-lt.02-and$_.movement.weight-gt.95})
 if($standing.Count-lt5-or[Math]::Abs([double]$rows[-1].player.position[2]-$ground.position[2])-gt.05){throw 'Native floor or remote ground recovery missing'}
 [ordered]@{airMode=$AirMode;holstered=[bool]$Holstered;nativeAirSamples=$nativeAir.Count;nativeAirVariants=@($nativeAir.variant|Sort-Object -Unique);playedAirSamples=$playedAir.Count;playedAirVariants=@($playedAir.movement.powerVariant|Sort-Object -Unique);standingRecoverySamples=$standing.Count;minimumBodyWeight=($contextRows.movement.weight|Measure-Object -Minimum).Minimum;nativeFloor=$ground.position[2];finalFloor=$rows[-1].player.position[2]}|ConvertTo-Json|Set-Content "$out/air-verification.json"
 $checks.Add('Native airborne phase overlaps remote cast and air locomotion; matching equipment and original floor recover')

'''+s[b:]
s=once(s,"if($sword.equipped-or(Q client '--player-sword').equipped){throw 'Sheathed sword unexpectedly drawn'}", "if($sword.equipped-ne$expectedDrawn-or(Q client '--player-sword').equipped-ne$expectedDrawn){throw 'Recovered sword context differs'}")
s=s.replace('Remote body recovers to standing with sheathed sword retained', 'Remote body recovers to standing with original sword context retained')
s=s.replace("source='client';stance=$Stance;holstered=[bool]$Holstered;checks=", "source='client';airMode=$AirMode;holstered=[bool]$Holstered;checks=")
s=s.replace('Standing/crouched holstered native Emily Doppelganger phases drive remote full-body bank57 poses', 'Native airborne Emily Doppelganger phases drive remote bank57 full-body casts blended with existing jump/fall locomotion')
new(N/'Test-DoppelgangerAir-v509.ps1',s)
w=(W/'Run-DoppelgangerHolstered-v496.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-holstered-standing-v496','doppelganger-air-release-armed-v509').replace('Test-DoppelgangerHolstered-v496','Test-DoppelgangerAir-v509')
w=once(w,"[ValidateSet('standing','crouched')][string]$Stance='standing')", "[ValidateSet('entry','release')][string]$AirMode='release',[switch]$Holstered)")
w=w.replace('-Take $Take -Stance $Stance', '-Take $Take -AirMode $AirMode -Holstered:$Holstered')
new(W/'Run-DoppelgangerAir-v509.ps1',w)
print('Prepared509 native input/Steam airborne tests with exact phase, body continuity, clone/FX and checkpoint checks.')
