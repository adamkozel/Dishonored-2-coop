[CmdletBinding()]
param([ValidateSet('host','client')][string]$Role='host',[string]$Take='carry-attachment-native-host-v1')
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$out=Join-Path $PSScriptRoot $Take
if(Test-Path -LiteralPath $out){throw 'Existing take'}
New-Item -ItemType Directory -Path $out|Out-Null
$launch=Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json
if((Get-Process -Id $launch.pid).Path-ne"$root/work/asset-game/Dishonored2.exe".Replace('/','\')){throw 'Wrong process'}
function C($s){& "$lab/Send-D2Command.ps1" -Role $Role -Command $s -WaitConsumed|Out-Null}
function Q($flag,$arg=''){if($arg){$v=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $flag $arg}else{$v=& "$lab/bin/d2_runtime_probe.exe" $launch.pid $flag};if($LASTEXITCODE){throw 'Read-only probe failed'};$v|ConvertFrom-Json}
function Save($name,$o){$o|ConvertTo-Json -Depth 24|Set-Content "$out/$name.json"}
function Word($o,$off){@($o.words|Where-Object offset -eq $off)[0].word}
function I($b,$ms=150){& "$lab/Send-D2Input.ps1" -Role $Role -Button $b -DurationMs $ms|Out-Null}
$held=$false
try{
 foreach($c in 'coop_movement_bank off','coop_shadow_stream off','coop_movement_fixture off'){C $c}
 Save 'player-before' (Q '--player-states');Save 'components-before' (Q '--components' '1');Save 'retarget-before' (Q '--retargeting' '1')
 C 'setviewpos -38.95 -161.5 68.1 270 0'
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $Role -Pitch 50 -Evidence "$out/pickup-camera.json"|Out-Null
 I X 900;Start-Sleep -Milliseconds 1900
 $s=Q '--player-states';Save 'player-held' $s;Save 'retarget-held' (Q '--retargeting' '1')
 if(!$s.carrySceneValid-or$s.carryScene.variant-ne2){throw 'Not carrying'};$held=$true
 $mode=Q '--object' $s.mode;$state=Q '--object' (Word $mode '0x178');Save 'state-held' $state
 $attachment=Q '--object' (Word $state '0xe8');Save 'attachment-held' $attachment
 $records=Q '--object' (Word $attachment '0x20');Save 'record-held' $records
 $frame=Q '--object' (Word $records '0x98');Save 'target-frame-held' $frame
 Save 'components-held' (Q '--components' '1');Save 'corpse-components-held' (Q '--components' ([string]$s.carryScene.corpseHandle))
 Save 'corpse-skeleton-held' (Q '--skeleton' ([string]$s.carryScene.corpseHandle))
 $anim=Q '--animation' ([string]$s.carryScene.corpseHandle);Save 'corpse-animation-held' $anim;Save 'corpse-animation-object-held' (Q '--object' $anim.animation)
 $midnightOffset=($anim.declaredInputs|Where-Object name -eq 'midnightData').offset
 Save 'corpse-midnight-input-held' (Q '--object' ('0x{0:x}'-f([Convert]::ToUInt64($anim.data.Substring(2),16)+[uint64]$midnightOffset)))
 Save 'midnight-held' (Q '--midnight')
 I B;Start-Sleep -Milliseconds 450;Save 'player-crouched' (Q '--player-states');Save 'record-crouched' (Q '--object' (Word $attachment '0x20'));Save 'retarget-crouched' (Q '--retargeting' '1')
 I B;Start-Sleep -Milliseconds 450;I X 500;Start-Sleep -Milliseconds 1400;$held=$false
 Save 'player-released' (Q '--player-states');Save 'attachment-released' (Q '--object' (Word $state '0xe8'));Save 'retarget-released' (Q '--retargeting' '1')
 Save 'target-frame-released' (Q '--object' (Word $records '0x98'))
 Save 'identity' ([ordered]@{role=$Role;exeSha256=(Get-FileHash $launch.executable).Hash;probeSha256=(Get-FileHash "$lab/bin/d2_runtime_probe.exe").Hash;readOnlyResearch=$true;nativePlayerInputs=$true;corpseHandle=$s.carryScene.corpseHandle})
}finally{
 if($held){I X 500};I Neutral
}
