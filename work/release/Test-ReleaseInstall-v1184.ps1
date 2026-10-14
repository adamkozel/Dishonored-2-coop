param([string]$Take='release-install-v1184')
# Stage 7 packaging/rollback test on scratch targets only (never the Steam
# install): each target holds read-only copies of the original executable,
# steam_api64.dll and base/master.index.
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
$package=Join-Path $root 'outputs/release/d2coop-v1181';$orig='C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Get-Sha256Hash($p){(Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash}
function Target($name){$t=Join-Path $out $name;New-Item -ItemType Directory -Force (Join-Path $t 'base')|Out-Null
  Copy-Item -LiteralPath (Join-Path $orig 'Dishonored2.exe') $t;Copy-Item -LiteralPath (Join-Path $orig 'steam_api64.dll') $t;Copy-Item -LiteralPath (Join-Path $orig 'base/master.index') (Join-Path $t 'base');$t}
function Snapshot($t){$m=@{};Get-ChildItem -LiteralPath $t -Recurse -File|ForEach-Object {$m[$_.FullName.Substring($t.Length+1).Replace('\','/')]=Get-Sha256Hash $_.FullName};$m}
$cases=[Collections.Generic.List[object]]::new();$failure=$null
try{
 $origHashes=@{exe=(Get-Sha256Hash (Join-Path $orig 'Dishonored2.exe'));api=(Get-Sha256Hash (Join-Path $orig 'steam_api64.dll'));master=(Get-Sha256Hash (Join-Path $orig 'base/master.index'))}
 # 1-3: install, refused reinstall, byte-identical rollback.
 $t=Target 'target-normal';$before=Snapshot $t
 $msg=& (Join-Path $package 'Install-D2Coop.ps1') -GameDirectory $t
 $man=Get-Content (Join-Path $package 'manifest.json') -Raw|ConvertFrom-Json
 $installedOk=@($man.files|Where-Object {(Get-Sha256Hash (Join-Path $t $_.path))-ne$_.sha256}).Count-eq0-and(Test-Path (Join-Path $t 'd2coop-install-receipt.json'))-and(Get-Sha256Hash (Join-Path $t 'Dishonored2.exe'))-eq$origHashes.exe-and(Get-Sha256Hash (Join-Path $t 'steam_api64.dll'))-eq$origHashes.api
 $cases.Add(@{case='install';passed=$installedOk;message="$msg"})
 $refused=$false;try{& (Join-Path $package 'Install-D2Coop.ps1') -GameDirectory $t|Out-Null}catch{$refused=$_.Exception.Message-like'*already installed*'}
 $cases.Add(@{case='reinstall_refused';passed=$refused})
 $msg=& (Join-Path $package 'Uninstall-D2Coop.ps1') -GameDirectory $t
 $after=Snapshot $t
 $same=($before.Count-eq$after.Count)-and(@($before.Keys|Where-Object {$after[$_]-ne$before[$_]}).Count-eq0)
 $cases.Add(@{case='rollback_byte_identical';passed=$same;message="$msg";before=$before;after=$after})
 # 4: wrong executable refused without changes.
 $t=Target 'target-wrong-exe';[IO.File]::AppendAllText((Join-Path $t 'Dishonored2.exe'),'x');$before=Snapshot $t
 $refused=$false;try{& (Join-Path $package 'Install-D2Coop.ps1') -GameDirectory $t|Out-Null}catch{$refused=$_.Exception.Message-like'*Unsupported*'}
 $after=Snapshot $t;$cases.Add(@{case='wrong_exe_refused_unchanged';passed=$refused-and($before.Count-eq$after.Count)-and(@($before.Keys|Where-Object {$after[$_]-ne$before[$_]}).Count-eq0)})
 # 5: modified master.index refused without changes.
 $t=Target 'target-modified-master';[IO.File]::AppendAllText((Join-Path $t 'base/master.index'),'x');$before=Snapshot $t
 $refused=$false;try{& (Join-Path $package 'Install-D2Coop.ps1') -GameDirectory $t|Out-Null}catch{$refused=$_.Exception.Message-like'*not the original*'}
 $after=Snapshot $t;$cases.Add(@{case='modified_master_refused_unchanged';passed=$refused-and($before.Count-eq$after.Count)-and(@($before.Keys|Where-Object {$after[$_]-ne$before[$_]}).Count-eq0)})
 # The real install is only read: its three hashes are unchanged.
 $cases.Add(@{case='steam_install_untouched';passed=((Get-Sha256Hash (Join-Path $orig 'Dishonored2.exe'))-eq$origHashes.exe-and(Get-Sha256Hash (Join-Path $orig 'steam_api64.dll'))-eq$origHashes.api-and(Get-Sha256Hash (Join-Path $orig 'base/master.index'))-eq$origHashes.master-and!(Test-Path (Join-Path $orig 'dinput8.dll'))-and!(Test-Path (Join-Path $orig 'd2coop-install-receipt.json')))})
 if(@($cases|Where-Object {!$_.passed}).Count){$failure='One or more release cases failed'}
}catch{$failure=$_.Exception.Message}
foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$failure='Protected file changed'}}
# Scratch targets hold copies of the executable; remove them after recording.
foreach($d in 'target-normal','target-wrong-exe','target-modified-master'){$p=Join-Path $out $d;if(Test-Path $p){Remove-Item -LiteralPath $p -Recurse -Force}}
@{passed=(!$failure);failure=$failure;cases=@($cases|ForEach-Object {@{case=$_.case;passed=$_.passed;message=$_.message}})}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
Get-Content "$out/result.json";if($failure){throw $failure}
