param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='vault-v14a-test',[ValidateSet('host','client')][string[]]$Sources=@('host','client'))
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab';$out=Join-Path $PSScriptRoot $Take
if(Test-Path $out){throw 'Take exists'};New-Item -ItemType Directory -Path $out|Out-Null
$pids=@{};$handles=@{};$checks=[Collections.Generic.List[string]]::new();$rows=[Collections.Generic.List[object]]::new();$failure=$null;$liveDll=(Get-FileHash "$root/work/asset-game/dinput8.dll").Hash
function C($r,$c){& "$lab/Send-D2Command.ps1" -Role $r -Command $c -WaitConsumed|Out-Null}
function I($r,$button='Neutral',$ms=100,$y=0){& "$lab/Send-D2Input.ps1" -Role $r -Button $button -DurationMs $ms -LeftY $y|Out-Null}
function Status($r,$suffix=''){for($i=0;$i-lt8;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw|ConvertFrom-Json)}catch{Start-Sleep -Milliseconds 15}};throw 'Status unavailable'}
function Q($r,$mode,$id=0){for($i=0;$i-lt10;$i++){$raw=if($id){& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode $id}else{& "$lab/bin/d2_runtime_probe.exe" $pids[$r] $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 15};throw "Probe failed $r $mode"}
function AwaitBank($r){$w=[Diagnostics.Stopwatch]::StartNew();do{$m=Status $r '.movement.json';if($m.active-and$m.version-eq14-and$m.weight-ge.9){return};Start-Sleep -Milliseconds 35}while($w.ElapsedMilliseconds-lt6000);throw 'Bank readiness deadline'}
try{
 foreach($r in 'host','client'){
  $l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;if(!$l.assetExperiment-or(Get-Process -Id $l.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated process'}
  $pids[$r]=$l.pid;$handles[$r]=(Status $r).probeActorHandle;I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off'
  if(!(Q $r '--player-sword').equipped-or(Q $r '--health' 1).current-ne56){throw 'Clean armed fixture required'}
  $role=if($r-eq'host'){'emily'}else{'corvo'};Copy-Item "$PSScriptRoot/vault-banks-v1/$role-vault-bank.bin" "$root/work/lab-instances/$r/coop-status.json.movement.bin"
 }
 foreach($src in $Sources){
  $dst=if($src-eq'host'){'client'}else{'host'}
  foreach($r in 'host','client'){C $r 'coop_movement_fixture off';C $r 'coop_movement_fixture 60000'}
  C $dst 'setviewpos -35.5 -152.5 68.1 58 0';C $src 'setviewpos -30.7 -150.5 68.1 90 0'
  & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $src -Pitch 0 -Evidence "$out/$src-camera.json"
  foreach($r in 'host','client'){C $r 'coop_shadow_stream on'};Start-Sleep -Milliseconds 500;C $dst 'coop_movement_bank on';AwaitBank $dst
  foreach($armed in $true,$false){
   if(!$armed){I $src X 1500;Start-Sleep -Milliseconds 3000}
   foreach($trial in 1,2){
    I $src;if((Q $src '--player-states').stance-eq1){I $src B 180;Start-Sleep -Milliseconds 650}
    C $src 'setviewpos -30.7 -150.5 68.1 90 0';Start-Sleep -Milliseconds 900;I $src LeftThumb 1500 32767
    $w=[Diagnostics.Stopwatch]::StartNew();$nativeSamples=0;$ownedSamples=0;$presented=$false;[double]$maxPhase=0;[double]$minPhase=1;$triggered=$false;$progressByEvent=@{}
    while($w.ElapsedMilliseconds-lt3200){
     $native=Q $src '--player-states';$m=Status $dst '.movement.json';$s=Status $dst '.shadow-stream.json'
     if(!$triggered){$sensor=$native.vaultSensorResearch;$vel=$sensor.velocityCandidate33c
      if($sensor.front38-eq1-and$sensor.clearF0-eq1-and($vel[0]*$vel[0]+$vel[1]*$vel[1])-ge($sensor.minSpeed70*$sensor.minSpeed70)){I $src @('A','LeftThumb') 1000 32767;$triggered=$true}}
     $rows.Add([pscustomobject]@{source=$src;armed=$armed;trial=$trial;ms=$w.ElapsedMilliseconds;triggered=$triggered;native=$native;movement=$m;stream=$s;target=(Status $dst)})
     if(!$m.active-or!$s.armed-or!$s.ready){throw "Vault presentation retired: $src $armed $trial $($s.result)"}
     if($native.active.id-contains9){$nativeSamples++;if($native.vaultSceneValid){$ownedSamples++}}
     if($m.traversalWeight-gt.7-and$m.weight-gt.8-and$m.traversalVariant-eq3){
      $presented=$true;$maxPhase=[Math]::Max($maxPhase,$m.traversalAmount);$minPhase=[Math]::Min($minPhase,$m.traversalAmount);$key=[string]$m.traversalEvent
      if($progressByEvent.ContainsKey($key)-and$m.traversalAmount-lt$progressByEvent[$key]){throw 'Distance progress rewound'};$progressByEvent[$key]=$m.traversalAmount
     }
     Start-Sleep -Milliseconds 15
    }
    if(!$triggered-or!$nativeSamples-or!$ownedSamples-or!$presented-or$maxPhase-lt.8-or$minPhase-gt.3){throw "Vault missing native=$nativeSamples owned=$ownedSamples presented=$presented phase=$minPhase..$maxPhase"}
    $checks.Add("${src}: armed=$armed trial=$trial native vault geometry drives monotonic delayed whole-body playback")
    if($m.traversalWeight-gt.01-or$m.airWeight-gt.01-or$m.weight-lt.9){throw 'Vault did not recover to grounded gait'}
    if([bool](Q $dst '--avatar-sword' $handles[$dst]).equipped-ne$armed){throw 'Vault changed remote equipment'}
    if(@((Q $dst '--player-states').active|Where-Object {$_.id-in8,9,2,3}).Count){throw 'Observer native player entered traversal'}
    foreach($r in 'host','client'){if((Q $r '--health' 1).current-ne56-or(Q $r '--health' $handles[$r]).current-ne40){throw 'Health changed'}}
    $checks.Add("${src}: armed=$armed trial=$trial equipment, native recovery, observer and player/avatar health preserved")
   }
  }
  I $src X 650;Start-Sleep -Milliseconds 3000;C $dst 'coop_movement_bank off';foreach($r in 'host','client'){C $r 'coop_shadow_stream off'}
 }
}catch{$failure=$_.Exception.Message}finally{
 foreach($r in 'host','client'){try{I $r;C $r 'coop_movement_bank off';C $r 'coop_shadow_stream off';if(!(Q $r '--player-sword').equipped){I $r X 650;Start-Sleep -Milliseconds 2500};if((Status $r).localPlayerStance-eq1){I $r B 180};C $r 'setviewpos -33.5 -153 68.1 0 0';C $r 'coop_movement_fixture off'}catch{}}
 $rows|ConvertTo-Json -Depth 13|Set-Content "$out/states.json"
 [ordered]@{passed=(!$failure);failure=$failure;checks=$checks;dll=$liveDll;bankVersion=14;scope='Isolated native vault, both roles and equipment states, two repetitions; distance phase, recovery, health and observer isolation. Visual/contact quality and other obstacle variants remain separate.'}|ConvertTo-Json -Depth 5|Set-Content "$out/result.json"
}
if($failure){throw $failure};"VAULT_VALIDATED $($checks.Count) checks"
