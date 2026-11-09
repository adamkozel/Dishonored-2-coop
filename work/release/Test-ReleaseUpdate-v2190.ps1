param([string]$Take='release-update-v2190a',[Parameter(Mandatory)][string]$Package,[string]$OldPackage='')
# 2190 (#244, stage 8 step 6): the update from 0.9.6 to 1.0 on a scratch copy only (never the Steam install, which is only
# read). The installers run under Windows PowerShell 5.1 through Install.cmd, as a player would, with the scratch target
# passed explicitly. Cases, on a full copy of the original install:
# - old_install: 0.9.6's Install.cmd installs (receipt version 0.9.6);
# - update: 1.0's Install.cmd updates it in one step (it removes 0.9.6 through 0.9.6's receipt, then installs): exit 0,
#   the output names the update, the receipt is 1.0's, every pinned built file and every shipped file is 1.0's;
# - reinstall_refused: 1.0's Install.cmd again is refused ("already installed");
# - rollback_byte_identical: 1.0's Uninstall.cmd leaves the copy byte-identical to the original;
# - steam_install_untouched, and the protected files unchanged. The scratch copy is removed after recording.
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
if(!$OldPackage){$OldPackage=Join-Path $root 'outputs/release/d2coop-v0.9.6'}
# The Steam folder has the mod installed since 2026-10-29; the untouched original is its backup 'Dishonored2 - Copy' (read only).
$orig='C:\Program Files (x86)\Steam\steamapps\common\Dishonored2 - Copy'
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Get-Sha256Hash($p){(Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash}
function Snapshot($t){$m=@{};Get-ChildItem -LiteralPath $t -Recurse -File -Force|ForEach-Object {$k=$_.FullName.Substring($t.Length+1).Replace('\','/');$m[$k]=if($_.Length-lt64MB){Get-Sha256Hash $_.FullName}else{"$($_.Length)|$($_.LastWriteTimeUtc.Ticks)"}};$m}
function Same($a,$b){($a.Count-eq$b.Count)-and(@($a.Keys|Where-Object {$b[$_]-ne$a[$_]}).Count-eq0)}
function RunCmd($pkg,$script,$target,$log){
  $env:D2COOP_NO_PAUSE='1';$sw=[Diagnostics.Stopwatch]::StartNew()
  $p=Start-Process -FilePath 'cmd.exe' -ArgumentList '/c',"`"`"$(Join-Path $pkg $script)`" `"$target`" > `"$log`" 2>&1`"" -PassThru -WindowStyle Hidden
  $p.WaitForExit();@{exit=$p.ExitCode;seconds=[math]::Round($sw.Elapsed.TotalSeconds,1);output=(Get-Content $log -Raw)}
}
function Receipt($t){$r=Join-Path $t 'd2coop-install-receipt.json';if(Test-Path $r){Get-Content $r -Raw|ConvertFrom-Json}else{$null}}
$cases=[Collections.Generic.List[object]]::new();$failure=$null
try{
 $origHashes=@{exe=(Get-Sha256Hash (Join-Path $orig 'Dishonored2.exe'));api=(Get-Sha256Hash (Join-Path $orig 'steam_api64.dll'));master=(Get-Sha256Hash (Join-Path $orig 'base/master.index'))}
 $old=Get-Content (Join-Path $OldPackage 'manifest.json') -Raw|ConvertFrom-Json;$new=Get-Content (Join-Path $Package 'manifest.json') -Raw|ConvertFrom-Json
 if($old.version-eq$new.version){throw 'The two packages have the same version'}
 $t=Join-Path $out 'target-normal'
 & robocopy.exe $orig $t /E /COPY:DAT /R:1 /W:1 /NFL /NDL /NJH /NJS /NP *> "$out/copy.log";if($LASTEXITCODE-ge8){throw "Scratch copy failed ($LASTEXITCODE)"}
 $before=Snapshot $t
 $run=RunCmd $OldPackage 'Install.cmd' $t "$out/old-install.log";$r=Receipt $t
 $cases.Add(@{case='old_install';passed=($run.exit-eq0-and$r-and"$($r.version)"-eq"$($old.version)");message=$run.output;seconds=$run.seconds})
 $run=RunCmd $Package 'Install.cmd' $t "$out/update.log";$r=Receipt $t
 $pinned=@($new.generated|Where-Object {$_.sha256}|Where-Object {(Get-Sha256Hash (Join-Path $t $_.path))-ne$_.sha256}).Count-eq0
 $shipped=@($new.files|Where-Object {(Get-Sha256Hash (Join-Path $t $_.path))-ne$_.sha256}).Count-eq0
 $cases.Add(@{case='update';passed=($run.exit-eq0-and$run.output-like"*Updating co-op $($old.version) to $($new.version)*"-and$r-and"$($r.version)"-eq"$($new.version)"-and$pinned-and$shipped-and(Get-Sha256Hash (Join-Path $t 'Dishonored2.exe'))-eq$origHashes.exe-and(Get-Sha256Hash (Join-Path $t 'steam_api64.dll'))-eq$origHashes.api);message=$run.output;seconds=$run.seconds})
 $run=RunCmd $Package 'Install.cmd' $t "$out/reinstall.log"
 $cases.Add(@{case='reinstall_refused';passed=($run.exit-ne0-and$run.output-like'*already installed*');message=$run.output})
 $run=RunCmd $Package 'Uninstall.cmd' $t "$out/uninstall.log";$after=Snapshot $t
 $cases.Add(@{case='rollback_byte_identical';passed=($run.exit-eq0-and(Same $before $after));message=$run.output})
 $cases.Add(@{case='steam_install_untouched';passed=((Get-Sha256Hash (Join-Path $orig 'Dishonored2.exe'))-eq$origHashes.exe-and(Get-Sha256Hash (Join-Path $orig 'steam_api64.dll'))-eq$origHashes.api-and(Get-Sha256Hash (Join-Path $orig 'base/master.index'))-eq$origHashes.master-and!(Test-Path (Join-Path $orig 'd2coop-install-receipt.json')));message=$null})
 if(@($cases|Where-Object {!$_.passed}).Count){$failure='One or more update cases failed'}
}catch{$failure=$_.Exception.Message}
foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$failure='Protected file changed'}}
$p=Join-Path $out 'target-normal';if(Test-Path $p){Remove-Item -LiteralPath $p -Recurse -Force}
@{passed=(!$failure);failure=$failure;package=$Package;oldPackage=$OldPackage;cases=@($cases|ForEach-Object {@{case=$_.case;passed=$_.passed;seconds=$_.seconds;message=$_.message}})}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
Get-Content "$out/result.json";if($failure){throw $failure}
