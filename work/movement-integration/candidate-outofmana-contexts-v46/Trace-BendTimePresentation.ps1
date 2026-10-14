param([Parameter(Mandatory)][ValidatePattern('^[a-z0-9-]+$')][string]$Take,[ValidateRange(1000,15000)][int]$DurationMs=10800)
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$out=Join-Path $PSScriptRoot $Take
if(!(Test-Path "$out/result.json")-and(Test-Path $out)){
 $rows=[Collections.Generic.List[object]]::new();$w=[Diagnostics.Stopwatch]::StartNew()
 while($w.ElapsedMilliseconds-lt$DurationMs){
  try{
   $m=Get-Content "$root/work/lab-instances/client/coop-status.json.movement.json" -Raw|ConvertFrom-Json
   $s=Get-Content "$root/work/lab-instances/host/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json
   $r=Get-Content "$root/work/lab-instances/client/coop-status.json.shadow-stream.json" -Raw|ConvertFrom-Json
   $rows.Add([pscustomobject]@{utcMs=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds();ms=$w.ElapsedMilliseconds;active=$m.active;event=$m.powerEvent;variant=$m.powerVariant;amount=$m.powerAmount;weight=$m.powerWeight;applied=$m.applied;sourceEvent=$s.sourceEvent;sourceState=$s.sourceState;sourceToken=$s.token;targetToken=$r.token;remoteEvent=$r.remoteEvent;ready=$r.ready;epoch=$r.epoch;peerEpoch=$r.peerEpoch})
  }catch{}
  Start-Sleep -Milliseconds 20
 }
 $rows|ConvertTo-Json -Depth 5|Set-Content "$out/presentation-trace.json"
 "PRESENTATION_TRACE $($rows.Count) samples"
}else{throw 'Existing or missing take; trace refused'}
