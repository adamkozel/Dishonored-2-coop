param([string]$Take='product-boot-v1187',[int]$WaitSeconds=90)
# Stage 7 product-mode boot: a standalone game process from the lab copy
# (work/asset-game) with NO lab environment and a temporary d2coop\d2coop.cfg.
# Saves go to an isolated fs_savepath under this take (never the user's real
# saves). Product mode must activate by itself (status files written in
# d2coop, Steam interfaces ready, solo host, no lab command file). The process
# is then closed and the temporary config/data removed; the lab instances are
# only checked for continued health.
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
$game=Join-Path $root 'work/asset-game';$data=Join-Path $game 'd2coop';$exe=Join-Path $game 'Dishonored2.exe'
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$proc=$null;$rows=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 8|Set-Content -LiteralPath $p}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 if(Test-Path $data){throw 'asset-game already has a d2coop folder; refusing to overwrite'}
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1188/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash (Join-Path $game 'dinput8.dll')).Hash-ne$pin.dll){throw 'Expected 1188 candidate'}
 $saves=Join-Path $out 'profile';New-Item -ItemType Directory -Force (Join-Path $saves 'base/savegame')|Out-Null
 New-Item -ItemType Directory $data|Out-Null
 "[d2coop]`nenabled=1`nsavegame=$((Join-Path $saves 'base/savegame').Replace('/','\'))`n"|Set-Content -LiteralPath (Join-Path $data 'd2coop.cfg') -Encoding ascii
 foreach($who in 'corvo','emily'){Copy-Item "$root/work/natural-moveset/climb-banks-v1042/$who-choke-bank.bin" (Join-Path $data "$who-choke-bank.bin")}
 $protectedBefore=@{};foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){$protectedBefore[$f.Path]=$f.Hash}
 $args="+set fs_savepath `"$($saves.Replace('\','/'))`" +set r_fullscreen 0 +set r_windowWidth 640 +set r_windowHeight 360 +set r_windowPosX 980 +set r_windowPosY 40 +set win_pauseOnFocusLoss 0"
 # A clean environment: none of the lab variables are inherited.
 $psi=[Diagnostics.ProcessStartInfo]::new($exe,$args);$psi.WorkingDirectory=$game;$psi.UseShellExecute=$false
 foreach($k in @($psi.Environment.Keys)){if($k-like'D2COOP_*'){$psi.Environment.Remove($k)|Out-Null}}
 $psi.Environment['SteamAppId']='403640';$psi.Environment['SteamGameId']='403640'
 $proc=[Diagnostics.Process]::Start($psi);$rows.pid=$proc.Id
 $w=[Diagnostics.Stopwatch]::StartNew();$status=$null
 do{Start-Sleep -Seconds 2;if($proc.HasExited){throw "Product process exited early (code $($proc.ExitCode))"}
  $pf=Join-Path $data 'coop-status.json.product.json';if(Test-Path $pf){try{$status=Get-Content $pf -Raw|ConvertFrom-Json}catch{}}}while(!$status-and$w.ElapsedMilliseconds-lt($WaitSeconds*1000))
 Start-Sleep -Seconds 5
 $files=@(Get-ChildItem -LiteralPath $data -File|ForEach-Object Name);$rows.dataFiles=$files
 foreach($n in 'coop-status.json.product.json','coop-status.json.online.json','coop-status.json'){$p=Join-Path $data $n;if(Test-Path $p){Copy-Item $p (Join-Path $out $n)}}
 $product=if(Test-Path (Join-Path $out 'coop-status.json.product.json')){Get-Content (Join-Path $out 'coop-status.json.product.json') -Raw|ConvertFrom-Json}else{$null}
 $online=if(Test-Path (Join-Path $out 'coop-status.json.online.json')){Get-Content (Join-Path $out 'coop-status.json.online.json') -Raw|ConvertFrom-Json}else{$null}
 $rows.product=$product;$rows.online=$online;$rows.aliveMs=$w.ElapsedMilliseconds
 $hwnd=$proc.MainWindowHandle.ToInt64();if($hwnd){& ffmpeg -hide_banner -loglevel error -y -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30,hwdownload,format=bgra" -frames:v 1 "$out/product-window.png"}
 if(!$product-or!$product.productMode){throw 'Product mode did not activate'}
 if(!$online-or!$online.steamReady){throw 'Steam interfaces not ready in product mode'}
}catch{$failure=$_.Exception.Message}
finally{
 if($proc-and!$proc.HasExited){try{$proc.CloseMainWindow()|Out-Null;if(!$proc.WaitForExit(15000)){$proc.Kill();$proc.WaitForExit(15000)|Out-Null}}catch{}}
 $rows.closed=($proc-eq$null)-or$proc.HasExited
 if(Test-Path $data){Remove-Item -LiteralPath $data -Recurse -Force}
 $rows.dataRemoved=!(Test-Path $data)
 $errors=@();foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$errors+='Protected file changed'}}
 foreach($r in 'host','client'){try{$s=Get-Content "$root/work/lab-instances/$r/coop-status.json" -Raw|ConvertFrom-Json;if(!$s.remoteFresh){$errors+="lab $r not fresh"}}catch{$errors+="lab $r status unreadable"}}
 if($errors.Count-and!$failure){$failure=$errors-join '; '}
 if($owned){Save "$out/result.json" @{passed=(!$failure);failure=$failure;rows=$rows;errors=$errors};$mutex.ReleaseMutex()};$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
