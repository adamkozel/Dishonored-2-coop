param([Parameter(Mandatory)][ValidateSet('host','client')][string]$Role,[ValidateSet(80,81)][int]$Slot=80,[string]$EvidencePath='')
# 1790 (#159, #160): a lab instance enters a pairing fixture: LoadGame <Slot> (80: the other character at the Dunwall
# fixture, Prepare-PairingFixtures-v1784), then the native Mayhew scene hands it the sword exactly as the restore
# fixture does (PrepareShadowSword: stand at -39.4 -161.4 facing 270, X, the scene, back to gameplay). The checkpoint
# itself holds no sword (diag-partner-sword-v1789a: ply_sword quantity 0 after the load, so the shadow source refused
# and the stream never armed). Fails unless the instance plays the slot's character with ply_sword quantity 1. Run while
# the caller holds the lab mutex; restores nothing (the caller reloads SLOT81).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab"
function C($cmd){& "$lab/Send-D2Command.ps1" -Role $Role -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function I([string]$b='Neutral',[int]$ms=200){& "$lab/Send-D2Input.ps1" -Role $Role -Button $b -DurationMs $ms|Out-Null}
function S(){for($a=0;$a-lt40;$a++){try{return(Get-Content "$root/work/lab-instances/$Role/coop-status.json" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};$null}
function Inv(){$p=(Get-Content "$root/work/lab-instances/$Role/launch.json" -Raw|ConvertFrom-Json).pid;& "$lab/bin/d2_runtime_probe.exe" $p '--player-inventory'|ConvertFrom-Json}
$log=[ordered]@{role=$Role;slot=$Slot;steps=@()}
C "LoadGame $Slot"
$w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 700;$st=S;if($st.localGameLogicState-in8,15){I A 300}}while(($st.localGameLogicState-ne1-or$st.localPendingGameLogicState)-and$w.ElapsedMilliseconds-lt120000)
if($st.localGameLogicState-ne1){throw "LoadGame $Slot did not reach gameplay"};Start-Sleep 2
$log.steps+=[ordered]@{at='loaded';ms=$w.ElapsedMilliseconds}
# The Mayhew sword scene, as the restore fixture plays it (hold-to-skip once it shows).
C 'setviewpos -39.4 -161.4 68.1 270 20';I X 400
$w=[Diagnostics.Stopwatch]::StartNew();$seen=$false;$skipped=$false
do{Start-Sleep -Milliseconds 300;$st=S;if($st.localGameLogicState-eq5){$seen=$true;if(!$skipped-and$w.ElapsedMilliseconds-gt1500){I X 3000;$skipped=$true}}}while(!($seen-and$st.localGameLogicState-eq1-and!$st.localPendingGameLogicState)-and$w.ElapsedMilliseconds-lt45000)
$log.steps+=[ordered]@{at='scene';seen=$seen;skipped=$skipped;ms=$w.ElapsedMilliseconds;logic=$st.localGameLogicState}
if(!$seen){throw 'The Mayhew sword scene did not play'}
$inv=Inv;$sword=@($inv.items|Where-Object{$_.name-eq'models/weapons/ply_sword/ply_sword'})[0]
$log.character=$inv.character;$log.swordQuantity=$sword.quantity
if($EvidencePath){$log|ConvertTo-Json -Depth 6|Set-Content $EvidencePath}
if([int]$sword.quantity-ne1){throw "The fixture sword was not handed over (ply_sword quantity $($sword.quantity))"}
$log
