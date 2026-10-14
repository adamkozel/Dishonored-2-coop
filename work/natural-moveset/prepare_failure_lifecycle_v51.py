"""Derive the shared exact-fixture setup, then test actual failure FX retirement."""
from pathlib import Path
P=Path(__file__).resolve().parent
source=(P/'Test-OutOfManaFx.ps1').read_text(encoding='utf-8-sig')
prefix=source[:source.index(' $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;')]
prefix=prefix[prefix.index('\n')+1:]
prefix=prefix.replace('$captures=@();$trace=$null;', '$captures=@();$trace=$null;$pausedRole=$null;$Crouched=$false;$Holstered=$false;$FrontView=$true;$TriggerMs=250;')
prefix=prefix.replace("'-t','12'", "'-t','20'")
prefix='\n'.join(line for line in prefix.split('\n') if ' $trace=Start-Process' not in line)
prefix=prefix.replace(" $start=[DateTimeOffset]", """ if($Resume){
  $emily=Get-Content "$PSScriptRoot/outofmana-context-bank-manifest-v46.json" -Raw|ConvertFrom-Json|Where-Object role -eq 'emily';$eb=Join-Path $PSScriptRoot $emily.bank
  if((Get-FileHash $eb).Hash-ne$emily.sha256){throw 'Emily bank mismatch'};Copy-Item $eb "$root/work/lab-instances/host/coop-status.json.movement.bin";C host 'coop_movement_bank on'
  $w.Restart();do{$m=S host '.movement.json';if($m.active-and$m.version-eq41-and$m.weight-gt.9){break};Start-Sleep -Milliseconds 40}while($w.ElapsedMilliseconds-lt5000)
  if(!$m.active-or$m.version-ne41-or$m.weight-lt.9){throw 'Both banks required for recovery'}
  $priorTokens=@{};foreach($r in 'host','client'){C $r 'coop_movement_resume on';$priorTokens[$r]=(S $r '.shadow-stream.json').token}
 }
 $start=[DateTimeOffset]""")
header="""param([ValidateSet('receiver-stop','source-pause','receiver-pause')][string]$Mode='receiver-stop',[ValidatePattern('^[a-z0-9-]+$')][string]$Take='failure-fx-lifecycle-v51',[switch]$Resume)
if($Resume-and$Mode-eq'receiver-stop'){throw 'Resume is scoped to native pause'}
"""
tail=r'''
 $watch=[Diagnostics.Stopwatch]::StartNew();$next=0;$times=@(1000,3000,6000);$failed=$false
 while($watch.ElapsedMilliseconds-lt8500){
  if($next-lt$times.Count-and$watch.ElapsedMilliseconds-ge$times[$next]){
   if($next-eq2){$mana=& "$PSScriptRoot/Read-NativeMana.ps1" -Role host -Evidence "$out/mana-before-failure.json"}
   I host 255 $TriggerMs;$events.Add([pscustomobject]@{seconds=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;action='Native LT press';index=$next});$next++
  }
  $native=Q host '--player-states';$fx=S client '.power-fx.json';$m=S client '.movement.json'
  $rows.Add([pscustomobject]@{stage='casting';videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;native=$native;fx=$fx;movement=$m;stream=(S client '.shadow-stream.json')})
  if($native.powerSceneValid-and$native.powerScene.variant-eq15){$failed=$true}
  if($next-eq3-and$failed-and$fx.started-eq($fxBefore.started+1)-and$fx.active-eq1){break}
  Start-Sleep -Milliseconds 15
 }
 if(!$failed-or$fx.started-ne($fxBefore.started+1)-or$fx.active-ne1-or@($fx.effects|Where-Object {$_.kind-eq11-and$_.owner-eq$avatar-and$_.record-ne'0x0'}).Count-ne1){throw 'Owned native failed scene and live automatic hand effect required'}
 if((Q host '--player-bendtime').powerActive){throw 'Failure activated native power'}
 $checks.Add('One exact native failure scene produces one owned live hand effect before interruption')
 $interruptionAt=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;$beforeInterrupt=$fx
 if($Mode-eq'receiver-stop'){C client 'coop_shadow_stream off'}else{
  $pausedRole=if($Mode-eq'source-pause'){'host'}else{'client'};I $pausedRole 0 180 Start
  $w.Restart();do{if((S $pausedRole).localGameLogicState-eq2){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt2500)
  if((S $pausedRole).localGameLogicState-ne2){throw 'Native pause not reached'}
  $rows.Add([pscustomobject]@{stage='paused';videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;fx=(S client '.power-fx.json');movement=(S client '.movement.json');stream=(S client '.shadow-stream.json')})
  if($Resume){foreach($r in 'host','client'){if((S $r '.movement.json').active-or(S $r '.shadow-stream.json').armed-or(S $r '.movement-resume.json').phase-ne2){throw 'Pause did not suspend both owners'}}}
  Start-Sleep -Milliseconds 500;I $pausedRole 0 180 A
  $w.Restart();do{if((S $pausedRole).localGameLogicState-eq1){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt4000)
  if((S $pausedRole).localGameLogicState-ne1){throw 'Native menu resume failed'};$pausedRole=$null
 }
 $w.Restart();do{$fx=S client '.power-fx.json';if(!$fx.active){break};Start-Sleep -Milliseconds 25}while($w.ElapsedMilliseconds-lt1800)
 if($fx.active-or$fx.started-ne$beforeInterrupt.started-or$fx.stopped-ne($beforeInterrupt.stopped+1)){throw 'Owned effect did not stop exactly once after interruption'}
 $checks.Add('Interruption stops the owned effect exactly once; observer pause defers native cleanup until resume')
 if($Resume){
  $w.Restart();do{$ready=$true;foreach($r in 'host','client'){$m=S $r '.movement.json';$s=S $r '.shadow-stream.json';if(!$m.active-or$m.version-ne41-or$m.weight-lt.9-or!$s.ready-or$s.token-eq$priorTokens[$r]-or(S $r '.movement-resume.json').phase-ne1){$ready=$false}};if($ready){break};Start-Sleep -Milliseconds 60}while($w.ElapsedMilliseconds-lt14000)
  if(!$ready){throw 'Pinned bodies did not resume through fresh tokens'};$checks.Add('Both pinned banks resume through fresh Steam tokens after native menu resume')
 }else{
  if((S client '.shadow-stream.json').armed-or(S client '.movement.json').weight-gt.001){throw 'Retired stream retained body ownership'};$checks.Add('Stopped stream retires body ownership')
 }
 Start-Sleep -Milliseconds 1800;$fx=S client '.power-fx.json'
 if($fx.active-or$fx.started-ne$beforeInterrupt.started){throw 'Failure effect replayed after retirement'}
 $rows.Add([pscustomobject]@{stage='settled';videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;fx=$fx;movement=(S client '.movement.json');stream=(S client '.shadow-stream.json')})
 $checks.Add('No delayed failure effect restarts after retirement or fresh-token recovery')
 I host;C host 'coop_bendtime_fixture off';$w.Restart();do{$f=S host '.bendtime-fixture.json';if(!$f.active){break};Start-Sleep -Milliseconds 50}while($w.ElapsedMilliseconds-lt7000)
 $after=Q host '--player-bendtime';if($f.active-or$f.result-ne'quantity_and_selection_restored'-or$after.quantity-or$after.equipped-or$after.started-or$after.powerActive){throw 'Power cleanup incomplete'}
 $checks.Add('Inactive native power fixture restores quantity and selection')
 $afterInventory=Q host '--player-inventory';$peerAfter=Q client '--player-inventory'
 if(Compare-Object @($inventory.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($afterInventory.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Source inventory changed'}
 if(Compare-Object @($peerBefore.items|ForEach-Object {"$($_.name):$($_.quantity)"}) @($peerAfter.items|ForEach-Object {"$($_.name):$($_.quantity)"})){throw 'Peer inventory changed'}
 foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' (S $r).probeActorHandle).current-ne40){throw 'Health changed'}}
 foreach($f in(Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json)){if((Get-FileHash $f.Path).Hash-ne$f.Hash){throw 'Protected file changed'}}
 $checks.Add('Both inventories, player/avatar health and fourteen protected hashes retained')
}catch{$failure=$_.Exception.Message}finally{
 if($pausedRole){try{I $pausedRole 0 180 A;Start-Sleep -Milliseconds 700}catch{}}
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_resume off';C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';C $r 'coop_bendtime_fixture off';C $r 'coop_movement_fixture off'}catch{}}
 foreach($rec in $captures){$rec.WaitForExit(25000)|Out-Null;if(!$rec.HasExited-or$rec.ExitCode){$failure='Capture incomplete'}}
 $rows|ConvertTo-Json -Depth 15|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;mode=$Mode;automaticResume=[bool]$Resume;interruptionAt=$interruptionAt;checks=$checks;events=$events;dll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash;bankSha256=(Get-FileHash $bank).Hash;scope='Corvo standing armed out-of-mana native hand effect interruption. Exact owned native scene and Steam body clock; cleanup and optional same-world menu recovery only. Death, reload, disconnect and broader contexts remain open.'}|ConvertTo-Json -Depth 7|Set-Content "$out/result.json"
}
if($failure){throw $failure};"PASS_FAILURE_FX_LIFECYCLE $Mode $($checks.Count) checks"
'''
out=P/'Test-FailureFxLifecycle-v51.ps1';assert not out.exists();out.write_text(header+prefix+tail,encoding='utf-8')
print(out)
