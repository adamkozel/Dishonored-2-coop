from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-PossessionInterruption-v344.ps1').read_text()
s=s.replace("[int]$Pitch=0)","[int]$Pitch=0,[ValidateSet('receiver-pause','source-pause')][string]$Interrupt='receiver-pause')",1)
s=s.replace('$dirty=$false;$failure=$null','$dirty=$false;$failure=$null;$pausedRole=$null',1)
old="   C client 'coop_shadow_stream off';$interruptAt=$w.ElapsedMilliseconds;$interrupted=$true"
assert s.count(old)==1
s=s.replace(old,'''   $pausedRole=if($Interrupt-eq'source-pause'){'host'}else{'client'}
   & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button Start -DurationMs 180|Out-Null
   $menuWait=[Diagnostics.Stopwatch]::StartNew()
   do{if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 30}while($menuWait.ElapsedMilliseconds-lt2000)
   if((S $pausedRole).localGameLogicState-ne2){throw 'Native Possession pause menu not entered'}
   $interruptAt=$w.ElapsedMilliseconds;$interrupted=$true
   Start-Sleep -Milliseconds 700
   $pausedVisibility=S client '.possession-body.json';$pausedStream=S client '.shadow-stream.json'
   [ordered]@{role=$pausedRole;logic=(S $pausedRole).localGameLogicState;visibility=$pausedVisibility;stream=$pausedStream}|ConvertTo-Json -Depth 8|Set-Content "$out/paused-cleanup.json"
   if($pausedVisibility.hidden-or$pausedVisibility.active-or$pausedVisibility.restoring-or$pausedStream.ready-or$pausedStream.armed){throw 'Native pause did not retire clone visibility lease and semantic stream'}
   & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -DurationMs 180|Out-Null
   $menuWait.Restart();do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 30}while($menuWait.ElapsedMilliseconds-lt2500)
   if((S $pausedRole).localGameLogicState-ne1){throw 'Native Possession pause menu did not resume'}
   $pausedRole=$null
''')
s=s.replace('Stop receiver semantic stream during verified human occupation; do not rearm','Native pause/resume during verified human occupation; stream remains retired')
s=s.replace('afterSamples=$afterInterrupt.Count;', 'afterSamples=$afterInterrupt.Count;interrupt=$Interrupt;')
# A source pause naturally extends the native occupation timer; the live
# native-input exit is still required and inventory is never revoked early.
s=s.replace("foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off'", "if($pausedRole){try{& \"$lab/Send-D2Input.ps1\" -Role $pausedRole -Button A -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 400}catch{}}\n foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off'",1)
s=s.replace('Receiver-stop during native occupation restores exact clone visibility within500ms; source remains possessed, receiver remains disarmed, no implicit rearm or leaked visibility lease','Native pause during occupation restores exact clone visibility; menu resumes normally while semantic stream stays disarmed and no visibility lease is leaked')
s=s.replace('Receiver-stop interruption tested during occupation; native menu/newworld/alternate contexts pending.','Native pause interruption tested during occupation; newworld/alternate contexts pending.')
(P/'Test-PossessionMenu-v346.ps1').write_text(s,encoding='utf-8')
print('346 native receiver/source pause cleanup prepared; runner unchanged')
