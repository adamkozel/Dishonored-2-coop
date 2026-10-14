param([switch]$Expanded)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$source=if($Expanded){"$PSScriptRoot/native-source-index.json"}else{"$root/work/moveset-growth/native-fullbody-candidates.json"}
$c=Get-Content $source -Raw|ConvertFrom-Json
$assets=@($c.entries.asset|Sort-Object -Unique)
for($i=0;$i-lt$assets.Count;$i+=800){
 $end=[Math]::Min($i+799,$assets.Count-1)
 $pattern='^('+(@($assets[$i..$end]|ForEach-Object {[regex]::Escape($_)})-join'|')+')$'
 $batch=if($Expanded){'natural-native-expanded-'+([int]($i/800))}else{'natural-native-library'}
 & "$root/outputs/d2-coop-lab/Read-D2Assets.ps1" -NamePattern $pattern -Export -Batch $batch -Limit 1000 | Select-Object Name,Bytes
}
