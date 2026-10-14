from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeWorldNetwork-v81.ps1').read_text(encoding='utf-8-sig')
s=s.replace('[int]$Pitch=12)', "[int]$Pitch=12,[ValidateSet('receiver-stop','source-pause','receiver-pause')][string]$Interrupt='source-pause')",1)
s=s.replace('$worldFramed=$false','$worldFramed=$false;$interrupted=$false;$pausedRole=$null',1)
needle="  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'\n  $fx="
assert needle in s
s=s.replace(needle,"""  if(!$interrupted-and$w.ElapsedMilliseconds-ge10000){
   $active=S host '.mesmerize-visual.json';if(!$active.active-or!$active.fxActive){throw 'No active world effect at interruption'}
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input=$Interrupt})
   if($Interrupt-eq'receiver-stop'){C host 'coop_shadow_stream off'}else{
    $pausedRole=if($Interrupt-eq'source-pause'){'client'}else{'host'}
    & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button Start -DurationMs 180|Out-Null
    $pauseWait=[Diagnostics.Stopwatch]::StartNew()
    do{if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 30}while($pauseWait.ElapsedMilliseconds-lt2500)
    if((S $pausedRole).localGameLogicState-ne2){throw 'Native pause did not start'}
    [ordered]@{role=$pausedRole;status=(S $pausedRole);visual=(S host '.mesmerize-visual.json');world=(S host '.mesmerize-world-stream.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/paused.json"
    Start-Sleep -Milliseconds 700
    & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -DurationMs 180|Out-Null
    $pauseWait.Restart()
    do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 30}while($pauseWait.ElapsedMilliseconds-lt4000)
    if((S $pausedRole).localGameLogicState-ne1){throw 'Native pause did not resume'};$pausedRole=$null
   }
   $interrupted=$true
  }
  $m=S host '.movement.json';$stream=S host '.shadow-stream.json'
  $fx=""",1)
s=s.replace("if(!$m.active-or!$stream.ready-or!$stream.armed){throw", "if(!$interrupted-and(!$m.active-or!$stream.ready-or!$stream.armed)){throw",1)
s=s.replace(" if($m.powerWeight-gt.02-or$m.weight-lt.9-or$stream.remoteState-ne0){throw 'Remote cast did not recover to standing'}", " if(!$interrupted){throw 'Interruption stimulus not applied'}\n if($m.powerWeight-gt.02){throw 'Interrupted remote cast retained weight'}")
a=s.index(" if($visual.refused-ne$visualBefore.refused")
b=s.index(" $remainingFx=Q host",a)
s=s[:a]+""" if($visual.refused-ne$visualBefore.refused-or($visual.created-$visualBefore.created)-ne1-or($visual.started[0]-$visualBefore.started[0])-ne1-or($visual.started[1]-$visualBefore.started[1])-ne1){throw 'Interrupted world creation/entry/idle not unique'}
 if($visual.active-or$visual.pendingHide-or$visual.fxActive-or($visual.fxStarted-$visualBefore.fxStarted)-ne1-or($visual.fxStopped-$visualBefore.fxStopped)-ne1-or$world.leased){throw 'Interrupted world lease/effect did not retire'}
 if(@($rows|Where-Object {$_.ms-gt15000-and($_.visual.active-or$_.visual.pendingHide-or$_.world.leased)}).Count){throw 'Old world event revived after interruption'}
 foreach($state in 1,3){if(!@($rows|Where-Object {$_.world.state-eq$state-and$_.world.leased}).Count){throw "Remote pre-interruption state $state missing"}}
 if($world.rejected-le$worldBefore.rejected){throw 'Duplicate packet rejection not observed'}
"""+s[b:]
s=s.replace("Native committed world incarnation and transform drive one cosmetic model, all three scene phases and one owned body effect through native Steam; source retirement hides the model and requests native effect stop; the native particle tail then leaves the manager", "Committed source world drives one model and body effect; explicit stream stop or native pause retires ownership, hides model and lets the native effect tail finish")
s=s.replace("Remote body recovers, original sword selections and all avatar health retained; remote model/effect has retired", "Interrupted cast releases body weight; source/avatar sword selections and avatar health retained; no old world revival")
s=s.replace(" foreach($r in 'host','client'){try{I $r;", " if($pausedRole){try{& \"$lab/Send-D2Input.ps1\" -Role $pausedRole -Button A -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 600}catch{}}\n foreach($r in 'host','client'){try{I $r;",1)
s=s.replace("source='client';checks=$checks", "source='client';interruption=$Interrupt;checks=$checks",1)
s=s.replace("Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified.", "Interruption after commitment checks finite cosmetic retirement and no stale revival. Same-world automatic resume/reacquisition, natural lifetime and interrupted aiming remain unverified.")
(P/'Test-MesmerizeInterrupt-v82.ps1').write_text(s,encoding='utf-8')
