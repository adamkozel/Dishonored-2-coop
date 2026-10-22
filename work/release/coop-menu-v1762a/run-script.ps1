param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='coop-menu-v1760a')
# 1760 (#156, 1214hq): co-op saves. After CO-OP the save root must switch to savegame_coop (the Campaign screen then
# lists only co-op saves: none yet in the VM lab game), and after CAMPAIGN back to savegame (its saves again). Frames
# 6 s after each choice, once the re-listing and the Campaign screen rebuild are done.
# 1757 (#156, 1214hn): the Co-op entry in the native main menu, in the VM lab game (the synced lab DLL). The VM lab game
# starts at its title (logic 6); Enter shows the main menu, where the mod inserts CO-OP under CAMPAIGN
# (native_menu_ui.h). Then, with the keyboard as a player: Down and Enter choose CO-OP (the Campaign screen opens and
# co-op mode turns on), Escape goes back, Up and Enter choose CAMPAIGN (co-op mode off). Frames of each screen and
# the mod's coop-menu status are recorded. Only the VM game is used; the host lab is not touched. Restores: the VM lab
# role is stopped.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{};$checks=[ordered]@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function Menu{try{Get-D2LabVMStatus -Sidecar 'coop-menu'}catch{$null}}
function Shot($n){try{Save-D2LabVMScreen -Path "$out/$n.png"|Out-Null}catch{}}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 & "$root/tools/vm/Test-D2LabVM.ps1" -Repair|Out-Null;if($LASTEXITCODE){throw 'Lab VM not ready'}
 try{Stop-D2LabVMRole -Role client|Out-Null}catch{}
 Start-D2LabVMRole -Role client|Out-Null
 $d=(Get-Date).AddSeconds(120);do{Start-Sleep 3;$gs=try{Get-D2LabVMStatus}catch{$null}}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne6)
 if($gs.localGameLogicState-ne6){throw 'VM game did not reach its title (logic 6)'}
 Start-Sleep 6;Shot 'title';$notes.title=Menu
 Send-D2LabVMKey 0x0D;$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu}while((Get-Date)-lt$d-and$m.result-ne'inserted')
 Start-Sleep 2;Shot 'menu';$notes.menu=Menu;$checks.inserted=($notes.menu.result-eq'inserted');$checks.entries6=($notes.menu.entries-eq6)
 if(!$checks.inserted){throw "Co-op entry not inserted ($($notes.menu.result) $($notes.menu.failedStep) $($notes.menu.lastCode))"}
 Send-D2LabVMKey 0x28;Start-Sleep 2;Shot 'menu-coop-focus'
 Send-D2LabVMKey 0x0D;Start-Sleep 3;Shot 'coop-campaign-screen';Start-Sleep 6;Shot 'coop-campaign-screen-relisted';$notes.coop=Menu
 $checks.coopSaves=($notes.coop.saves.active-eq$true-and$notes.coop.saves.result-eq'coop')
 $checks.coopMode=($notes.coop.coopMode-eq$true-and$notes.coop.coopSelected-ge1)
 Send-D2LabVMKey 0x1B;Start-Sleep 4;Shot 'back-home';$notes.back=Menu
 Send-D2LabVMKey 0x26;Start-Sleep 2;Send-D2LabVMKey 0x0D;Start-Sleep 3;Shot 'campaign-screen';Start-Sleep 6;Shot 'campaign-screen-relisted';$notes.campaign=Menu
 $checks.campaignSaves=($notes.campaign.saves.active-eq$false-and$notes.campaign.saves.result-eq'campaign')
 $checks.campaignMode=($notes.campaign.coopMode-eq$false-and$notes.campaign.campaignSelected-ge1)
 Send-D2LabVMKey 0x1B;Start-Sleep 3;Shot 'end-home'
 foreach($k in $checks.Keys){if(!$checks[$k]){throw "Check failed: $k"}}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};Shot 'failure'}
finally{try{Stop-D2LabVMRole -Role client|Out-Null}catch{};if($owned){$mutex.ReleaseMutex()}}
Save "$out/result.json" ([ordered]@{passed=!$failure;failure=$failure;checks=$checks;notes=$notes;scope='Co-op main menu entry probe (VM lab game, keyboard as a player): inserted under Campaign, chooses co-op mode, Campaign turns it off; VM lab role stopped.'})
Get-Content "$out/result.json";if($failure){throw $failure}
