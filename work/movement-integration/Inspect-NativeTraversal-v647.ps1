$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab="$root/outputs/d2-coop-lab";$out="$PSScriptRoot/traversal-research-v647"
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
 $mode=Q '--object' $s.mode;$mode|ConvertTo-Json -Depth 8|Set-Content "$out/mode-object.json"
 $slots=@($mode.words|Where-Object {$o=[Convert]::ToUInt64($_.offset.Substring(2),16);$o-ge16-and$o-lt736-and$_.pointerType-match'PlayerState'}|ForEach-Object {[pscustomobject]@{id=([Convert]::ToInt32($_.offset.Substring(2),16)-16)/8;pointer=$_.word;type=$_.pointerType}})
 $slots|ConvertTo-Json -Depth 4|Set-Content "$out/native-state-slots.json"
 $entities=Q '--entities';$entities|ConvertTo-Json -Depth 8|Set-Content "$out/entities.json"
 $candidates=@($entities.entities|Where-Object {($_.name+' '+$_.type+' '+$_.definitionName)-match'(?i)(ladder|chain|rope|water|swim|climb|window)'})
 $candidates|ConvertTo-Json -Depth 7|Set-Content "$out/fixture-candidates.json"
 [pscustomobject]@{modeSlots=$slots.Count;traversalStates=@($slots|Where-Object type -match '(?i)(ladder|chain|rope|water|swim|climb|mantl|vault|div)');entityCandidates=$candidates.Count;entities=@($candidates|Select-Object name,type,position)}|ConvertTo-Json -Depth 7
}finally{if($owned){$mutex.ReleaseMutex()};$mutex.Dispose()}
