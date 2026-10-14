param([string]$Take='online-session-v1179')
# Stage 7 (candidate 1178): native Steam session flow in the isolated lab.
# Host creates a native lobby (+rich presence, overlay invite dialog, P2P
# listen); the client (same Steam account in this one-PC lab) attempts to join
# it. Sessions are then turned off and the lab loopback transport must recover.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false;$failure=$null;$rows=@{}
function Save($p,$v){$v|ConvertTo-Json -Depth 10|Set-Content -LiteralPath $p}
function C($r,$cmd){& "$lab/Send-D2Command.ps1" -Role $r -Command $cmd -WaitConsumed -ReceiptTimeoutMs 8000|Out-Null}
function S($r,$suffix=''){for($i=0;$i-lt40;$i++){try{return(Get-Content "$root/work/lab-instances/$r/coop-status.json$suffix" -Raw -ErrorAction Stop|ConvertFrom-Json -ErrorAction Stop)}catch{Start-Sleep -Milliseconds 25}};throw "Status unavailable $r $suffix"}
function Shot($r,$name){$l=Get-Content "$root/work/lab-instances/$r/launch.json" -Raw|ConvertFrom-Json;$hwnd=(Get-Process -Id $l.pid).MainWindowHandle.ToInt64()
 & ffmpeg -hide_banner -loglevel error -y -f lavfi -i "gfxcapture=hwnd=${hwnd}:capture_cursor=0:max_framerate=30,hwdownload,format=bgra" -frames:v 1 "$out/$name-$r.png"}
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $pin=Get-Content "$root/work/movement-integration/candidate-npc-combat-v1178/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Expected 1178 candidate'}
 Save "$out/before.json" @{host=(S host '.online.json');client=(S client '.online.json')}
 C host 'coop_session host'
 $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 250;$h=S host '.online.json'}while($h.phase-lt2-and$h.phase-ne6-and$w.ElapsedMilliseconds-lt15000)
 Start-Sleep -Seconds 2;Shot host 'invite';Shot client 'invite'
 $rows.host=$h;Save "$out/host-online.json" $h
 if($h.phase-ne2-or$h.lobby-eq'0'){throw "Host lobby not created: $($h.result)"}
 C client "coop_session join $($h.lobby)"
 $w.Restart();do{Start-Sleep -Milliseconds 250;$c=S client '.online.json'}while($c.phase-in3,4-and$w.ElapsedMilliseconds-lt20000)
 $rows.client=$c;$rows.hostAfterJoin=(S host '.online.json');Save "$out/client-online.json" $c
 Shot host 'after-join';Shot client 'after-join'
}catch{$failure=$_.Exception.Message}
finally{
 if($owned){
  foreach($r in 'host','client'){try{C $r 'coop_session off'}catch{}}
  $w=[Diagnostics.Stopwatch]::StartNew();do{Start-Sleep -Milliseconds 500;$fresh=(S host).remoteFresh-and(S client).remoteFresh}while(!$fresh-and$w.ElapsedMilliseconds-lt30000)
  $rows.loopbackRecovered=$fresh;$rows.final=@{host=(S host '.online.json');client=(S client '.online.json')}
  if(!$fresh-and!$failure){$failure='Lab loopback transport did not recover'}
  foreach($f in Get-Content "$root/work/movement-integration/protected-save-hashes-before-v4.json" -Raw|ConvertFrom-Json){if((Get-FileHash -LiteralPath $f.Path).Hash-ne$f.Hash){$failure='Protected file changed'}}
  Save "$out/result.json" @{passed=(!$failure);failure=$failure;rows=$rows};$mutex.ReleaseMutex()
 };$mutex.Dispose()
}
Get-Content "$out/result.json";if($failure){throw $failure}
