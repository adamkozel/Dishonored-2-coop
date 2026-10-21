param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='title-signin-v1741a')
# 1741 (#146, research, read-only): what changes in the game when a key is pressed at the title screen. A partner who
# joins from the title has no signed-in user and cannot save (product-two-account-v1696a/v1699a). The VM lab game is
# started at its title (logic 6, as Start-D2LabVMRole does), and its game logic state stack (--logic) and every state
# object's words (--object, with the RTTI type of each pointer) are dumped twice at the title (to see what changes on
# its own), then once 4 s after Enter (the main menu). The diff is the finding: a field or pointer (for example to an
# idLocalUser) that only exists after the key press. Only reads the VM game; the host lab is not used. Restores: the VM
# lab role is stopped.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$notes=[ordered]@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 12|Set-Content -LiteralPath $p}
function Dump($tag){
 $logic=Invoke-D2LabVMProbe -Mode '--logic';$states=[ordered]@{}
 foreach($s in @($logic.stack)){$states["$($s.id)"]=[ordered]@{type=$s.type;pointer=$s.pointer;words=$(try{@((Invoke-D2LabVMProbe -Mode '--object' -Arg $s.pointer).words)}catch{"unread: $($_.Exception.Message)"})}}
 $d=[ordered]@{tag=$tag;status=(Get-D2LabVMStatus|Select-Object localGameLogicState,localMap);logic=$logic;states=$states};Save "$out/dump-$tag.json" $d;$d}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 & "$root/tools/vm/Test-D2LabVM.ps1" -Repair|Out-Null;if($LASTEXITCODE){throw 'Lab VM not ready'}
 try{Stop-D2LabVMRole -Role client|Out-Null}catch{}
 Start-D2LabVMRole -Role client|Out-Null
 $d=(Get-Date).AddSeconds(120);do{Start-Sleep 3;$gs=try{Get-D2LabVMStatus}catch{$null}}while((Get-Date)-lt$d-and$gs.localGameLogicState-ne6)
 if($gs.localGameLogicState-ne6){throw 'VM game did not reach its title (logic 6)'}
 Start-Sleep 8;try{Save-D2LabVMScreen -Path "$out/title-1.png"|Out-Null}catch{}
 $a=Dump 'title-1';Start-Sleep 5;$b=Dump 'title-2';try{Save-D2LabVMScreen -Path "$out/title-2.png"|Out-Null}catch{}
 Send-D2LabVMKey 0x0D;Start-Sleep 4;try{Save-D2LabVMScreen -Path "$out/menu.png"|Out-Null}catch{}
 $c=Dump 'menu'
 # Words that are equal in both title dumps and differ after the key press.
 $diff=[Collections.Generic.List[object]]::new()
 foreach($id in $c.states.Keys){$wa=$a.states[$id].words;$wb=$b.states[$id].words;$wc=$c.states[$id].words
  if($wa-is[string]-or$wc-is[string]-or!$wa-or!$wc){continue}
  for($i=0;$i-lt[Math]::Min($wa.Count,$wc.Count);$i++){$x=$wa[$i];$y=$wb[$i];$z=$wc[$i]
   if($x.word-eq$y.word-and$x.word-ne$z.word){$diff.Add([ordered]@{state=$id;type=$c.states[$id].type;offset=$z.offset;title=$x.word;menu=$z.word;titleType=$x.pointerType;menuType=$z.pointerType})}}}
 Save "$out/diff.json" @($diff);$notes.diffCount=$diff.Count;$notes.stateTypes=@($c.logic.stack|ForEach-Object {"$($_.id) $($_.type)"})
}catch{$failure=$_.Exception.Message}finally{
 if($owned){try{Stop-D2LabVMRole -Role client|Out-Null}catch{$failure=($failure+'; stop: '+$_.Exception.Message).TrimStart('; ')}
  if(Test-Path $out){Save "$out/result.json" ([ordered]@{passed=(!$failure);failure=$failure;notes=$notes;scope='Research (#146): the VM game title screen against its main menu, read-only dumps of the game logic state objects; the VM lab role stopped afterwards.'})}
  $mutex.ReleaseMutex()};$mutex.Dispose()}
if($failure){throw $failure};"DIFF $($notes.diffCount)"
