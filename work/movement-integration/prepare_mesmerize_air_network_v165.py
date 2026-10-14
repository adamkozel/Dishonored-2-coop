"""Recorded armed airborne entry/release cases using the v153 recovery contract."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeContextNetwork-v153.ps1').read_text(encoding='utf-8-sig')
s=s[s.index('\n')+1:]
s="param([ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(-15,45)][int]$Pitch=12,[ValidateSet('entry','release')][string]$AirCase='entry')\n$Stance='standing';$Holstered=$false;$Move=0\n"+s
s=s.replace('$stood=$false;$releaseWalking=$false','$stood=$false;$releaseWalking=$false;$jumped=$false')
a=s.index('  if(!$cast-and$w.ElapsedMilliseconds-ge1100)');b=s.index('  if($Move-and!$releaseWalking',a)
s=s[:a]+'''  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   $button=if($AirCase-eq'entry'){'A'}else{'Neutral'}
   & "$lab/Send-D2Input.ps1" -Role client -Button $button -LeftTrigger 255 -DurationMs 1800|Out-Null;$cast=$true
   if($AirCase-eq'entry'){$jumped=$true}
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input="Native air-$AirCase case: $button with LT held1800ms"})
  }
  if($AirCase-eq'release'-and!$jumped-and$w.ElapsedMilliseconds-ge2300){
   & "$lab/Send-D2Input.ps1" -Role client -Button A -LeftTrigger 255 -DurationMs 180|Out-Null;$jumped=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native A during aim; release LT after180ms while still airborne'})
  }
'''+s[b:]
at=" $checks.Add('Strict scene reader validates all three native phases with reciprocal owned Emily left arm')"
assert s.count(at)==1
s=s.replace(at,at+'''
 $requiredVariant=if($AirCase-eq'entry'){16}else{18}
 $sourceAir=@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in2,3}).Count-and$_.player.powerSceneValid-and$_.player.powerScene.variant-eq$requiredVariant})
 $peerAir=@($rows|Where-Object {$_.movement.airState-in1,2-and$_.movement.airWeight-gt.25-and$_.movement.powerWeight-gt.5-and$_.movement.powerVariant-eq$requiredVariant})
 $peak=($rows|ForEach-Object {[double]$_.player.position[2]}|Measure-Object -Maximum).Maximum
 $startHeight=[double]$rows[0].player.position[2]
 [ordered]@{airCase=$AirCase;requiredVariant=$requiredVariant;sourceAirSamples=$sourceAir.Count;peerAirSamples=$peerAir.Count;peakHeight=$peak;startHeight=$startHeight;sourceTimes=@($sourceAir.ms);peerTimes=@($peerAir.ms)}|ConvertTo-Json -Depth 8|Set-Content "$out/air-network-coverage.json"
 if(!$jumped-or!$sourceAir.Count-or!$peerAir.Count-or$peak-$startHeight-lt.25-or$peak-$startHeight-gt3-or[Math]::Abs([double]$rows[-1].player.position[2]-$startHeight)-gt.3){throw 'Direct native and remote airborne phase coverage or same-floor recovery missing'}
 $checks.Add('Exact source airborne phase drives remote cast plus native-air locomotion; jump stays bounded and returns to the same floor')
''')
s=s.replace("stance=$Stance;holstered=[bool]$Holstered;move=$Move;checks=", "stance=$Stance;holstered=[bool]$Holstered;move=$Move;airCase=$AirCase;checks=")
s=s.replace('Native Emily Mesmerize phases drive corresponding whole-body stance/equipment clips through Steam.', 'Armed native Emily Mesmerize air entry or air release drives whole-body cast and air locomotion through Steam. The airCase field identifies the bounded in-place jump sequence.')
(P/'Test-MesmerizeAirNetwork-v165.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';r=runner.read_text(encoding='utf-8-sig')
r=r.replace("'MesmerizeContext','MesmerizeNetwork'", "'MesmerizeContext','MesmerizeAirNetwork','MesmerizeNetwork'")
r=r.replace(" [switch]$AirEntryObserve,", " [switch]$AirEntryObserve,\n [ValidateSet('entry','release')][string]$AirCase='entry',")
at="  'MesmerizeCrouchNetwork'"
assert r.count(at)==1
r=r.replace(at,"  'MesmerizeAirNetwork'{& \"$root/work/natural-moveset/Test-MesmerizeAirNetwork-v165.ps1\" -Take $Take -Pitch $Pitch -AirCase $AirCase}\n"+at)
runner.write_text(r,encoding='utf-8')
