"""Bounded forward walking during the observed crouched native cast."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeCrouchNetwork-v133.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep('$worldFramed=$false;$stood=$false','$worldFramed=$false;$stood=$false;$releaseWalking=$false')
rep('I client 255 1800;', '& "$lab/Send-D2Input.ps1" -Role client -Button Neutral -LeftY 12000 -LeftTrigger 255 -DurationMs 1800|Out-Null;')
rep("input='Native LT held 1800ms then released'", "input='Native crouched forward input12000 with LT held1800ms then released'")
at='  if($cast-and!$manaCommit-and$w.ElapsedMilliseconds-ge5500)'
rep(at,r'''  if(!$releaseWalking-and$w.ElapsedMilliseconds-ge3000){
   & "$lab/Send-D2Input.ps1" -Role client -Button Neutral -LeftY 12000 -DurationMs 1400|Out-Null;$releaseWalking=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Continue native crouched forward movement through release/recovery for1400ms then stop'})
  }
'''+at)
at=" $checks.Add('All three source-native crouched phases drive the complete-body crouch clips; native standing recovery retains the movement bank')"
rep(at,at+r'''
 $moving=@($castRows|Where-Object {$_.movement.speed-gt.12})
 $origin=$rows[0].player.position;$maximumTravel=0.0
 foreach($row in $rows){
  $dx=[double]$row.player.position[0]-$origin[0];$dy=[double]$row.player.position[1]-$origin[1]
  $maximumTravel=[Math]::Max($maximumTravel,[Math]::Sqrt($dx*$dx+$dy*$dy))
  if([Math]::Abs([double]$row.player.position[2]-$origin[2])-gt.3){throw 'Moving crouched cast left its bounded native floor'}
 }
 if(!$releaseWalking-or$moving.Count-lt5-or$maximumTravel-lt.3-or$maximumTravel-gt4){throw 'Native moving crouch coverage or bounded travel not established'}
 [ordered]@{movingPhaseSamples=$moving.Count;maximumTravel=$maximumTravel;maximumRemoteSpeed=($moving.movement.speed|Measure-Object -Maximum).Maximum;floorPreserved=$true}|ConvertTo-Json|Set-Content "$out/moving-crouch-summary.json"
 $checks.Add('Bounded native forward crouch movement overlaps casting; network locomotion continues and stays on the tested native floor')''')
rep('acquisition and crouched armed controller casting', 'acquisition and forward-moving crouched armed controller casting')
rep('Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified.', 'This take uses20-second equipment expiry; native lifetime, standing cancellation and crouched cancellation have separate receipts. Other directions and equipment contexts remain unverified.')
(P/'Test-MesmerizeCrouchMoving-v139.ps1').write_text(s,encoding='utf-8')
