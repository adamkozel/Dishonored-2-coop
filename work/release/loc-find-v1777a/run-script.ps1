param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='loc-find-v1777a',[string[]]$Phrases=@('Corvo Attano','Emily Kaldwin','Play as','Choose','character','Corvo','Emily'))
# 1777 (#158, 1214hw): the localization keys whose text holds each phrase, from the lab host instance's in-memory table
# (coop_loc_find, read-only), for the partner's character choice message box (its button labels must be string ids).
# Lab host only; nothing is changed in the game. Each phrase's sidecar is copied into the take folder.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out=Join-Path $PSScriptRoot $Take
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation active'}
 if(Test-Path $out){throw 'Unique take required'};New-Item -ItemType Directory $out|Out-Null;Copy-Item $PSCommandPath "$out/run-script.ps1"
 $side="$root/work/lab-instances/host/coop-status.json.loc-find.json";$summary=[ordered]@{}
 foreach($p in $Phrases){$t0=if(Test-Path $side){(Get-Item $side).LastWriteTimeUtc}else{[DateTime]::MinValue}
  & "$lab/Send-D2Command.ps1" -Role host -Command "coop_loc_find $p" -WaitConsumed|Out-Null
  $d=(Get-Date).AddSeconds(10);do{Start-Sleep -Milliseconds 200}while((Get-Date)-lt$d-and!((Test-Path $side)-and(Get-Item $side).LastWriteTimeUtc-gt$t0))
  $name=($p.ToLowerInvariant() -replace '[^a-z0-9]+','-');Copy-Item $side "$out/$name.json"
  $j=Get-Content "$out/$name.json" -Raw|ConvertFrom-Json;$summary[$p]=[ordered]@{found=$j.found;count=$j.count;keys=@($j.matches|Select-Object -First 12|ForEach-Object{"$($_.key) = $($_.text)"})}}
 $summary|ConvertTo-Json -Depth 6|Set-Content "$out/summary.json"
 Get-Content "$out/summary.json"
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
