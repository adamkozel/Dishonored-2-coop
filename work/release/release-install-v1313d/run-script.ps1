param([string]$Take='release-install-v1313a',[string]$Package='')
# Release 0.7.0 (r7) install/rollback test on scratch targets only (never the Steam install, which is only read).
# The installer runs under Windows PowerShell 5.1 through Install.cmd, as a player would, always with the scratch
# target passed explicitly (auto-detection would find the real install).
# - target-normal: a full copy of the original install (the installer builds every game-derived file from it);
# - target-wrong-exe / target-modified-master: executable, steam_api64.dll and master.index only.
# Cases: install, built files checked (pinned hashes, bank loader validation, table shapes), reinstall refused,
# byte-identical rollback, wrong executable refused unchanged, modified master.index refused unchanged, Steam install
# untouched, protected saves unchanged. Scratch copies are removed after recording.
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
if(!$Package){$Package=Join-Path $root 'outputs/release/d2coop-v0.7.0'}
$orig='C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
function Get-Sha256Hash($p){(Get-FileHash -LiteralPath $p -Algorithm SHA256).Hash}
function Small($name){$t=Join-Path $out $name;New-Item -ItemType Directory -Force (Join-Path $t 'base')|Out-Null
  Copy-Item -LiteralPath (Join-Path $orig 'Dishonored2.exe') $t;Copy-Item -LiteralPath (Join-Path $orig 'steam_api64.dll') $t;Copy-Item -LiteralPath (Join-Path $orig 'base/master.index') (Join-Path $t 'base');$t}
# Snapshot: hash every file under 64 MB, size and time for larger ones (the resource containers are never written).
function Snapshot($t){$m=@{};Get-ChildItem -LiteralPath $t -Recurse -File -Force|ForEach-Object {$k=$_.FullName.Substring($t.Length+1).Replace('\','/');$m[$k]=if($_.Length-lt64MB){Get-Sha256Hash $_.FullName}else{"$($_.Length)|$($_.LastWriteTimeUtc.Ticks)"}};$m}
function Same($a,$b){($a.Count-eq$b.Count)-and(@($a.Keys|Where-Object {$b[$_]-ne$a[$_]}).Count-eq0)}
function RunCmd($script,$target,$log){
  $env:D2COOP_NO_PAUSE='1';$sw=[Diagnostics.Stopwatch]::StartNew()
  $p=Start-Process -FilePath 'cmd.exe' -ArgumentList '/c',"`"`"$(Join-Path $Package $script)`" `"$target`" > `"$log`" 2>&1`"" -PassThru -WindowStyle Hidden
  $peak=0;while(!$p.HasExited){foreach($c in Get-CimInstance Win32_Process -Filter "Name='d2coop_setup.exe'" -ErrorAction SilentlyContinue){$peak=[math]::Max($peak,[double]$c.WorkingSetSize)};Start-Sleep -Milliseconds 500}
  $p.WaitForExit();@{exit=$p.ExitCode;seconds=[math]::Round($sw.Elapsed.TotalSeconds,1);peakSetupMB=[math]::Round($peak/1MB);output=(Get-Content $log -Raw)}
}
$cases=[Collections.Generic.List[object]]::new();$failure=$null;$timing=$null
try{
 $origHashes=@{exe=(Get-Sha256Hash (Join-Path $orig 'Dishonored2.exe'));api=(Get-Sha256Hash (Join-Path $orig 'steam_api64.dll'));master=(Get-Sha256Hash (Join-Path $orig 'base/master.index'))}
 $man=Get-Content (Join-Path $Package 'manifest.json') -Raw|ConvertFrom-Json
 # 1-3: install, built files, reinstall refused, byte-identical rollback.
 $t=Join-Path $out 'target-normal'
 & robocopy.exe $orig $t /E /COPY:DAT /R:1 /W:1 /NFL /NDL /NJH /NJS /NP *> "$out/copy.log";if($LASTEXITCODE-ge8){throw "Scratch copy failed ($LASTEXITCODE)"}
 $before=Snapshot $t
 $run=RunCmd 'Install.cmd' $t "$out/install.log";$timing=@{seconds=$run.seconds;peakSetupMB=$run.peakSetupMB}
 $receipt=Join-Path $t 'd2coop-install-receipt.json'
 $pinned=@($man.generated|Where-Object {$_.sha256}|Where-Object {(Get-Sha256Hash (Join-Path $t $_.path))-ne$_.sha256}).Count-eq0
 $shipped=@($man.files|Where-Object {(Get-Sha256Hash (Join-Path $t $_.path))-ne$_.sha256}).Count-eq0
 $cases.Add(@{case='install';passed=($run.exit-eq0-and(Test-Path $receipt)-and$pinned-and$shipped-and(Get-Sha256Hash (Join-Path $t 'Dishonored2.exe'))-eq$origHashes.exe-and(Get-Sha256Hash (Join-Path $t 'steam_api64.dll'))-eq$origHashes.api);message=$run.output})
 & "$root/work/build/Release/d2_movement_bank_tests.exe" (Join-Path $t 'd2coop/corvo-choke-bank.bin') (Join-Path $t 'd2coop/emily-choke-bank.bin') *> "$out/bank-tests.log"
 $cases.Add(@{case='built_banks_load';passed=($LASTEXITCODE-eq0);message=(Get-Content "$out/bank-tests.log" -Raw)})
 $tables=& 'C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe' -I -c "import sys;sys.path.insert(0,r'$root\tools\release');import build_bank_patch as b;t=b.tables(open(r'$t\d2coop\native-tables.bin','rb').read());print(len(t))"
 $cases.Add(@{case='built_tables_parse';passed=("$tables".Trim()-eq'17');message="$tables"})
 $run2=RunCmd 'Install.cmd' $t "$out/reinstall.log"
 $cases.Add(@{case='reinstall_refused';passed=($run2.exit-ne0-and$run2.output-like'*already installed*');message=$run2.output})
 $run3=RunCmd 'Uninstall.cmd' $t "$out/uninstall.log"
 $after=Snapshot $t
 $cases.Add(@{case='rollback_byte_identical';passed=($run3.exit-eq0-and(Same $before $after));message=$run3.output})
 # 4: wrong executable refused without changes.
 $t=Small 'target-wrong-exe';[IO.File]::AppendAllText((Join-Path $t 'Dishonored2.exe'),'x');$before=Snapshot $t
 $run=RunCmd 'Install.cmd' $t "$out/wrong-exe.log";$after=Snapshot $t
 $cases.Add(@{case='wrong_exe_refused_unchanged';passed=($run.exit-ne0-and$run.output-like'*Unsupported*'-and(Same $before $after));message=$run.output})
 # 5: modified master.index refused without changes.
 $t=Small 'target-modified-master';[IO.File]::AppendAllText((Join-Path $t 'base/master.index'),'x');$before=Snapshot $t
 $run=RunCmd 'Install.cmd' $t "$out/modified-master.log";$after=Snapshot $t
 $cases.Add(@{case='modified_master_refused_unchanged';passed=($run.exit-ne0-and$run.output-like'*not the original*'-and(Same $before $after));message=$run.output})
 # The real install is only read.
 $cases.Add(@{case='steam_install_untouched';passed=((Get-Sha256Hash (Join-Path $orig 'Dishonored2.exe'))-eq$origHashes.exe-and(Get-Sha256Hash (Join-Path $orig 'steam_api64.dll'))-eq$origHashes.api-and(Get-Sha256Hash (Join-Path $orig 'base/master.index'))-eq$origHashes.master-and!(Test-Path (Join-Path $orig 'dinput8.dll'))-and!(Test-Path (Join-Path $orig 'd2coop'))-and!(Test-Path (Join-Path $orig 'd2coop-install-receipt.json')))})
 if(@($cases|Where-Object {!$_.passed}).Count){$failure='One or more release cases failed'}
}catch{$failure=$_.Exception.Message}
foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$failure='Protected file changed'}}
foreach($d in 'target-normal','target-wrong-exe','target-modified-master'){$p=Join-Path $out $d;if(Test-Path $p){Remove-Item -LiteralPath $p -Recurse -Force}}
@{passed=(!$failure);failure=$failure;package=$Package;timing=$timing;cases=@($cases|ForEach-Object {@{case=$_.case;passed=$_.passed;message=$_.message}})}|ConvertTo-Json -Depth 6|Set-Content "$out/result.json"
Get-Content "$out/result.json";if($failure){throw $failure}
