$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out="$PSScriptRoot/traversal-research-v648"
$mutex=[Threading.Mutex]::new($false,'Local\D2CoopLab_NativeOperation_01a11a65');$owned=$false
try{
 try{$owned=$mutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$owned=$true};if(!$owned){throw 'Native operation busy'}
 if(Test-Path -LiteralPath $out){throw 'Unique research directory required'};New-Item -ItemType Directory $out|Out-Null
 $launch=Get-Content "$root/work/lab-instances/host/launch.json" -Raw|ConvertFrom-Json
 if(!$launch.assetExperiment-or(Get-Process -Id $launch.pid).Path-ne(Join-Path $root 'work/asset-game/Dishonored2.exe')){throw 'Wrong isolated executable'}
 function Q($mode,$arg=''){
  for($i=0;$i-lt8;$i++){$raw=if($arg){& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode $arg}else{& "$lab/bin/d2_runtime_probe.exe" $launch.pid $mode};if(!$LASTEXITCODE){return($raw|ConvertFrom-Json)};Start-Sleep -Milliseconds 25};throw "Native read failed $mode"
 }
 $s=Q '--player-states';$s|ConvertTo-Json -Depth 8|Set-Content "$out/player-states.json"
 $machine=Q '--object' $s.machine;$machine|ConvertTo-Json -Depth 8|Set-Content "$out/machine-object.json"
 $rows=[Collections.Generic.List[object]]::new()
 foreach($w in $machine.words){
  $offset=[Convert]::ToUInt64($w.offset.Substring(2),16)
  if($offset-lt40-or$offset-gt136-or$w.word-eq'0x0'){continue}
  $o=Q '--object' $w.word
  if(($o.words|Where-Object offset -eq '0x0').word-ne$s.machine){continue}
  $o|ConvertTo-Json -Depth 7|Set-Content "$out/mode-$($w.offset).json"
  $slots=@($o.words|Where-Object {$n=[Convert]::ToUInt64($_.offset.Substring(2),16);$n-ge16-and$n-lt736-and$_.pointerType-match'PlayerState'}|ForEach-Object {[pscustomobject]@{id=([Convert]::ToInt32($_.offset.Substring(2),16)-16)/8;pointer=$_.word;type=$_.pointerType}})
  $rows.Add([pscustomobject]@{offset=$w.offset;pointer=$w.word;header8=($o.words|Where-Object offset -eq '0x8').word;slots=$slots})
 }
 $rows|ConvertTo-Json -Depth 8|Set-Content "$out/mode-slots.json"
 $rows|ForEach-Object {[pscustomobject]@{offset=$_.offset;header8=$_.header8;slots=$_.slots.Count;special=@($_.slots|Where-Object type -match '(?i)(climb|swim|rope|ladder|water)')}}|ConvertTo-Json -Depth 5
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
