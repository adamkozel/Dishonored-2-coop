from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
source=(P/'Test-PossessionTargetFxView-v355.ps1').read_text()
s=source[:source.index(' $monitor=Start-Process')]
s=s.replace("[int]$Pitch=0)","[int]$Pitch=0,[ValidateSet('cancel','receiver-stop','source-pause','receiver-pause')][string]$Case='cancel')")
s=s.replace('$dirty=$false;', '$pausedRole=$null;$dirty=$false;')
s=s.replace("'-t','11'","'-t','12'")
s+='''
 $fxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-possession-effects','11000') -WindowStyle Hidden -RedirectStandardOutput "$out/possession-effects.ndjson" -RedirectStandardError "$out/possession-effects.stderr" -PassThru
 $peerFxMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-possession-effects','11000') -WindowStyle Hidden -RedirectStandardOutput "$out/peer-effects.ndjson" -RedirectStandardError "$out/peer-effects.stderr" -PassThru
 I host 255 10000
 $activeWait=[Diagnostics.Stopwatch]::StartNew()
 do{$peerFx=S client '.possession-fx.json';if($peerFx.sourceId-and$peerFx.started-$peerFxBefore.started-eq1){break};Start-Sleep -Milliseconds 40}while($activeWait.ElapsedMilliseconds-lt3500)
 if(!$peerFx.sourceId-or$peerFx.started-$peerFxBefore.started-ne1){throw 'No owned peer target mark before lifecycle interruption'}
 $beforeInterrupt=[ordered]@{source=(S host '.possession-fx.json');peer=$peerFx;player=(Q host '--player-states');sourceStream=(S host '.shadow-stream.json');peerStream=(S client '.shadow-stream.json')}
 $beforeInterrupt|ConvertTo-Json -Depth 10|Set-Content "$out/before-interrupt.json"
 if($beforeInterrupt.player.stance-eq18-or!$beforeInterrupt.player.defaultMode){throw 'Expected native aim before interruption, not possession occupation'}
 $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Verified native aim and owned remote target particle before '+$Case})
 Start-Sleep -Milliseconds 500
 if($Case-eq'receiver-stop'){C client 'coop_shadow_stream off'}
 elseif($Case-eq'cancel'){& "$lab/Send-D2Input.ps1" -Role host -Button X -LeftTrigger 255 -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 240;I host}
 else{
  $pausedRole=if($Case-eq'source-pause'){'host'}else{'client'}
  & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button Start -LeftTrigger $(if($pausedRole-eq'host'){255}else{0}) -DurationMs 180|Out-Null
  $menuWait=[Diagnostics.Stopwatch]::StartNew();do{if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 30}while($menuWait.ElapsedMilliseconds-lt2000)
  if((S $pausedRole).localGameLogicState-ne2){throw 'Native pause menu did not open'}
  Start-Sleep -Milliseconds 650
  [ordered]@{role=$pausedRole;status=(S $pausedRole);peerFx=(S client '.possession-fx.json');stream=(S client '.shadow-stream.json')}|ConvertTo-Json -Depth 8|Set-Content "$out/paused.json"
  & "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -LeftTrigger $(if($pausedRole-eq'host'){255}else{0}) -DurationMs 180|Out-Null
  $menuWait.Restart();do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 30}while($menuWait.ElapsedMilliseconds-lt2500)
  if((S $pausedRole).localGameLogicState-ne1){throw 'Native pause menu failed to resume'}
  $pausedRole=$null
 }
 $retire=[Diagnostics.Stopwatch]::StartNew()
 while($retire.ElapsedMilliseconds-lt1500){$rows.Add([pscustomobject]@{ms=$retire.ElapsedMilliseconds;videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;sourceFx=(S host '.possession-fx.json');peerFx=(S client '.possession-fx.json');stream=(S client '.shadow-stream.json')});Start-Sleep -Milliseconds 45}
 $settled=@($rows|Where-Object ms -ge500)
 if($settled.Count-lt4-or@($settled|Where-Object {$_.peerFx.sourceId-or$_.peerFx.started-$peerFxBefore.started-ne1-or$_.peerFx.stopped-$peerFxBefore.stopped-ne1-or$_.peerFx.refused-ne$peerFxBefore.refused-or$_.peerFx.retired-ne$peerFxBefore.retired}).Count){throw 'Target mark did not retire once and remain absent after interruption'}
 if($Case-ne'cancel'-and@($settled|Where-Object {$_.stream.armed-or$_.stream.ready}).Count){throw 'Interrupted stream implicitly rearmed'}
 if($Case-eq'cancel'-and@($settled|Where-Object {!$_.stream.armed-or!$_.stream.ready}).Count){throw 'Native cancellation retired normal movement stream'}
 $checks.Add('One owned target particle retires after '+$Case+' and remains absent across settling interval; no refusal, lost owner or replay')
 # Native cancellation while LT remains held, then neutral input. No synthetic gameplay outcome.
 & "$lab/Send-D2Input.ps1" -Role host -Button X -LeftTrigger 255 -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 250;I host
 Start-Sleep -Milliseconds 600
 $final=Q host '--player-states';$final|ConvertTo-Json -Depth 8|Set-Content "$out/final-player.json"
 if($final.stance-eq18-or!$final.defaultMode){
  if($Case-eq'cancel'){throw 'Cancellation unexpectedly entered native possession'}
  I host 255 120;Start-Sleep -Milliseconds 1300;$final=Q host '--player-states'
 }
 if(!$final.defaultMode-or$final.stance-ne0){throw 'Native input did not restore normal source mode'}
 $manaAfter=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-after.json"
 if($Case-eq'cancel'-and[Math]::Abs([double]$manaAfter.currentCandidate-[double]$manaBefore.currentCandidate)-gt.01){throw 'Aim cancellation unexpectedly spent mana'}
 $checks.Add('Native source recovers normal mode; explicit cancellation preserves mana')
 [ordered]@{case=$Case;before=$beforeInterrupt;afterSource=(S host '.possession-fx.json');afterPeer=(S client '.possession-fx.json');settledRows=$settled.Count}|ConvertTo-Json -Depth 10|Set-Content "$out/lifecycle-summary.json"
'''
tail=source[source.index('}catch{$failure=$_.Exception.Message}finally{'):]
tail=tail.replace('}finally{','}finally{\n if($pausedRole){try{& "$lab/Send-D2Input.ps1" -Role $pausedRole -Button A -DurationMs 180|Out-Null;Start-Sleep -Milliseconds 400}catch{}}',1)
start=tail.index("scope='Native source Corvo Possession acquisition")
end=tail.index("'}|ConvertTo-Json",start)
tail=tail[:start]+"case=$Case;scope='Native human target particles: aim cancellation or stream/menu interruption, source gameplay remains native, no receiver NPC mutation. Complete checkpoint inventory/health and protected save checks mandatory. Body occupation/release is covered by separate successful-cast tests."+tail[end:]
s+=tail
(P/'Test-PossessionFxLifecycle-v357.ps1').write_text(s)
runner=W/'Invoke-NativeLabOperation.ps1';s=runner.read_text();a="   'PossessionFxObserve'{& \"$root/work/natural-moveset/Test-PossessionTargetFxPreview-v356.ps1\" -Take $Take -Pitch $Pitch}"
assert a in s
s=s.replace(a,"   'PossessionFxObserve'{if($Interrupt-eq'none'){& \"$root/work/natural-moveset/Test-PossessionFxLifecycle-v357.ps1\" -Take $Take -Pitch $Pitch -Case cancel}else{& \"$root/work/natural-moveset/Test-PossessionFxLifecycle-v357.ps1\" -Take $Take -Pitch $Pitch -Case $Interrupt}}")
runner.write_text(s)
print('357 isolated cancellation/menu/stream lifecycle harness prepared; DLL351 unchanged')
