param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='coop-saveflow-v1769a',[int]$StopAfter=99)
# 1769 (1214ht): the second CO-OP visit waits for the mod's re-listing (saves.refreshes) before CONTINUE, and records
# the menu status; the campaign check compares the saves exactly (path, size, time, SHA-256) and profile.bin by content
# (coop-saveflow-v1768a: only profile.bin's time changed, from the profile saved at each switch).
# 1768: the co-op folder path in the reset step was broken (coop-saveflow-v1767a: a backspace character where "\b" of
# "\base" stood, "Illegal characters in path"); fixed.
# 1767: the VM lab instance's savegame_coop is removed first (its own test saves). The pause menu does not wrap (coop-saveflow-v1766a: Up stayed on Resume and Enter resumed): eight Downs reach
# Quit Game. The load uses CONTINUE (the newest co-op save, the quicksave) instead of the Load list; then the game is
# left again the same way and CAMPAIGN shows the player's own list.
# 1766: in the mission, logic 3 is the character confirmation "Play as Corvo Attano?" with the focus on No
# (coop-saveflow-v1765c answered No 25 times): there Right then A answers Yes; on the menu A still answers the
# tutorial question No.
# 1765: after the difficulty the game asks "Do you want to start the tutorial?" (logic 3, focus on No;
# coop-saveflow-v1764a waited there): A answers No.
# 1764: the Co-op Campaign screen opens with the focus on CONTINUE, inactive without co-op saves (coop-saveflow-v1763a:
# three Enters did nothing); Down moves to New Game first, each New Game screen is filmed, and while the game is still
# on the menu (logic 6) the steering presses Enter every 4 s to pass the remaining New Game screens.
# 1763 (#156): a co-op save made from the Co-op entry, in the VM lab game, as a player with the keyboard and the pad:
# title, Enter, CO-OP (the co-op saves switch), New Game (the slot, the difficulty, the intro and the character choice
# steered by game logic state as Test-ProductTwoAccount-v1709 does), gameplay, F5 quicksave, pause > Quit Game > Back to
# main menu, CO-OP again, Load: the quicksave is listed and loads; then the main menu and CAMPAIGN. Both save folders
# (savegame and savegame_coop) are listed at the start and the end: the campaign folder must be unchanged and the co-op
# folder must hold the new save. Frames and the game logic state after every step. -StopAfter N ends after phase N
# (1 menu, 2 new game started, 3 saved, 4 back in menu, 5 loaded). Only the VM game is used. Restores: the VM lab role
# is stopped (the VM lab saves are the VM lab instance's own).
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{steps=@();menuEnters=0};$checks=[ordered]@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function Menu{try{Get-D2LabVMStatus -Sidecar 'coop-menu'}catch{$null}}
function St{try{Get-D2LabVMStatus}catch{$null}}
$n=0;function Shot($tag){$script:n++;$name='{0:d2}-{1}' -f $script:n,$tag;try{Save-D2LabVMScreen -Path "$out/$name.png"|Out-Null}catch{};$s=St
 $script:notes.steps+=[ordered]@{step=$name;logic=$s.localGameLogicState;pending=$s.localPendingGameLogicState;map=$s.localMap};$name}
function Key($vk,$wait=1500){Send-D2LabVMKey $vk;Start-Sleep -Milliseconds $wait}
function Folders{Invoke-D2LabVM -ArgumentList "$GuestWs\work\lab-instances\client\base" -ScriptBlock {param($b)
  $r=[ordered]@{};foreach($d in 'savegame','savegame_coop'){$p=Join-Path $b $d;$r[$d]=@(if(Test-Path $p){Get-ChildItem -LiteralPath $p -Recurse -File|ForEach-Object{[ordered]@{path=$_.FullName.Substring($p.Length+1);length=$_.Length;written=$_.LastWriteTimeUtc.ToString('o');sha256=(Get-FileHash -LiteralPath $_.FullName -Algorithm SHA256).Hash}}})};$r}}
function WaitLogic($want,$sec){$d=(Get-Date).AddSeconds($sec);do{Start-Sleep 2;$s=St}while((Get-Date)-lt$d-and!($s.localGameLogicState-eq$want-and!$s.localPendingGameLogicState));$s}
# Steer the native new-game screens by logic state until gameplay (as Test-ProductTwoAccount-v1709's host start).
function SteerToGameplay($sec){$d=(Get-Date).AddSeconds($sec);$last=Get-Date '2000-01-01';$choices=0
 do{Start-Sleep -Milliseconds 800;$s=St;$lg=$s.localGameLogicState
  if(((Get-Date)-$last).TotalSeconds-ge3.5){
   switch($lg){
    14{$choices++;Start-Sleep -Milliseconds 1500;foreach($k in 1,2){Send-D2LabVMInput -Button Left -LeftX -32000 -DurationMs 250|Out-Null;Start-Sleep -Milliseconds 600};Send-D2LabVMInput -Button A -DurationMs 400|Out-Null;$last=Get-Date}
    5{Send-D2LabVMInput -Button X -DurationMs 3000|Out-Null;$last=Get-Date}
    3{Shot 'question'|Out-Null;if($s.localMap-and$s.localMap-notmatch'menu'){Send-D2LabVMInput -Button Right -DurationMs 250|Out-Null;Start-Sleep -Milliseconds 400};Send-D2LabVMInput -Button A -DurationMs 300|Out-Null;$last=Get-Date}
    {$_-in 8,15,9,11}{Send-D2LabVMInput -Button A -DurationMs 300|Out-Null;$last=Get-Date}
    {$_-in 2,4,12}{Send-D2LabVMInput -Button B -DurationMs 250|Out-Null;$last=Get-Date}
    6{if(!$s.localPendingGameLogicState){Send-D2LabVMKey 0x0D;$script:notes.menuEnters++;Shot 'menu-enter'|Out-Null};$last=Get-Date}}}
 }while((Get-Date)-lt$d-and!($s.localGameLogicState-eq1-and!$s.localPendingGameLogicState-and$s.localMap-and$s.localMap-notmatch'menu'))
 $script:notes.choices=$choices;$s}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 & "$root/tools/vm/Test-D2LabVM.ps1" -Repair|Out-Null;if($LASTEXITCODE){throw 'Lab VM not ready'}
 try{Stop-D2LabVMRole -Role client|Out-Null}catch{}
 # A clean co-op folder (the VM lab instance's own test saves), so New Game starts without an overwrite question.
 $notes.clearedCoop=Invoke-D2LabVM -ArgumentList "$GuestWs\work\lab-instances\client\base\savegame_coop" -ScriptBlock {param($p) if(Test-Path $p){Remove-Item -LiteralPath $p -Recurse -Force;'removed'}else{'absent'}}
 Start-D2LabVMRole -Role client|Out-Null
 if((WaitLogic 6 120).localGameLogicState-ne6){throw 'VM game did not reach its title (logic 6)'}
 $notes.before=Folders;Start-Sleep 5;Shot 'title'|Out-Null
 Key 0x0D 4000;$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu}while((Get-Date)-lt$d-and$m.result-ne'inserted');Shot 'menu'|Out-Null
 if($m.result-ne'inserted'){throw "Co-op entry not inserted ($($m.result))"}
 # Phase 1: CO-OP and its Campaign screen with co-op saves.
 Key 0x28;Key 0x0D 3000;$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu}while((Get-Date)-lt$d-and!($m.saves.active-and$m.saves.phase-eq0));Shot 'coop-campaign'|Out-Null
 $checks.coopSaves=($m.saves.active-eq$true);if(!$checks.coopSaves){throw "Co-op saves not active ($($m.saves.result))"};if($StopAfter-le1){throw 'stop:1'}
 # Phase 2: New Game (the first unlocked entry has the focus), its screens, then gameplay.
 Key 0x28 1000;Shot 'newgame-focus'|Out-Null;Key 0x0D 3000;Shot 'newgame'|Out-Null;Key 0x0D 3000;Shot 'newgame-slot'|Out-Null;Key 0x0D 3000;Shot 'newgame-next'|Out-Null
 $s=SteerToGameplay 420;Shot 'gameplay'|Out-Null;$notes.gameplay=[ordered]@{logic=$s.localGameLogicState;map=$s.localMap}
 $checks.gameplay=($s.localGameLogicState-eq1-and$s.localMap-and$s.localMap-notmatch'menu');if(!$checks.gameplay){throw 'New game did not reach gameplay'};if($StopAfter-le2){throw 'stop:2'}
 # Phase 3: quicksave (F5).
 Start-Sleep 5;Key 0x74 6000;Shot 'quicksaved'|Out-Null;$notes.afterSave=Folders
 $checks.coopSaveWritten=(@($notes.afterSave.savegame_coop|Where-Object{$_.path-match'^SLOT\d\d\\'}).Count-gt0);if($StopAfter-le3){throw 'stop:3'}
 # Phase 4: pause, Quit Game (the last entry), Back to main menu (the second button).
 function QuitToMenu($tag){Key 0x1B 2500;Shot "$tag-pause"|Out-Null;foreach($k in 1..8){Key 0x28 400};Shot "$tag-quit-focus"|Out-Null;Key 0x0D 2000;Shot "$tag-quit-dialog"|Out-Null;Key 0x27 800;Shot "$tag-quit-button"|Out-Null;Key 0x0D 2000;Shot "$tag-quit-confirmed"|Out-Null}
 QuitToMenu 'first'
 if((WaitLogic 6 120).localGameLogicState-ne6){throw 'Did not return to the main menu'};Start-Sleep 4
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu}while((Get-Date)-lt$d-and$m.result-ne'inserted');Shot 'menu-again'|Out-Null;if($StopAfter-le4){throw 'stop:4'}
 # Phase 5: CO-OP, then CONTINUE (the focus; the newest co-op save).
 $r0=[int](Menu).saves.refreshes;Key 0x28;Key 0x0D 3000;$d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu}while((Get-Date)-lt$d-and!($m.saves.active-and$m.saves.phase-eq0-and[int]$m.saves.refreshes-gt$r0));Start-Sleep 2;Shot 'coop-campaign-again'|Out-Null;$notes.coopAgain=Menu
 Key 0x0D 3000;Shot 'continue-chosen'|Out-Null;$s=SteerToGameplay 240;Shot 'loaded'|Out-Null
 $checks.loaded=($s.localGameLogicState-eq1-and$s.localMap-eq$notes.gameplay.map)
 # Leave again and open CAMPAIGN: the player's own list.
 Start-Sleep 4;QuitToMenu 'second';if((WaitLogic 6 120).localGameLogicState-ne6){throw 'Did not return to the main menu (second)'};Start-Sleep 4
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu}while((Get-Date)-lt$d-and$m.result-ne'inserted');Key 0x0D 3000
 $d=(Get-Date).AddSeconds(30);do{Start-Sleep 2;$m=Menu}while((Get-Date)-lt$d-and!(!$m.saves.active-and$m.saves.phase-eq0));Shot 'campaign-list'|Out-Null
 $checks.campaignMode=(!$m.saves.active);$notes.final=Folders
 # The saves exactly (path, size, time, hash); profile.bin by content (each switch saves the player's profile again).
 $sig={param($f)if($f.path-eq'profile.bin'){"$($f.path)|$($f.sha256)"}else{"$($f.path)|$($f.length)|$($f.written)|$($f.sha256)"}}
 $before=@($notes.before.savegame|ForEach-Object{& $sig $_})-join"`n";$after=@($notes.final.savegame|ForEach-Object{& $sig $_})-join"`n"
 $checks.campaignUnchanged=($before-eq$after)
 foreach($k in $checks.Keys){if(!$checks[$k]){throw "Check failed: $k"}}
}catch{$failure=$_.Exception.Message;if($failure-notlike'stop:*'){Save "$out/failure.json" @{message=$failure;stack=$_.ScriptStackTrace};try{Shot 'failure'|Out-Null}catch{}}else{$notes.stoppedAfter=$failure;$failure=$null}}
finally{try{$notes.end=Folders}catch{};try{Stop-D2LabVMRole -Role client|Out-Null}catch{};if($owned){$mutex.ReleaseMutex()}}
Save "$out/result.json" ([ordered]@{passed=!$failure;failure=$failure;checks=$checks;notes=$notes;scope='Co-op save flow from the Co-op entry in the VM lab game (keyboard and pad as a player): new game, quicksave, back to the menu, load; the campaign save folder must stay unchanged; VM lab role stopped.'})
Get-Content "$out/result.json"|Select-Object -First 40;if($failure){throw $failure}
