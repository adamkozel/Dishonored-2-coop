$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$missing=@((Get-Content "$PSScriptRoot/native-expanded-sample-manifest.json" -Raw|ConvertFrom-Json).rows|Where-Object status -eq 'missing_asset'|Select-Object -ExpandProperty asset -Unique)
for($i=0;$i-lt$missing.Count;$i+=60){
 $last=[Math]::Min($i+59,$missing.Count-1)
 $pattern='^('+(@($missing[$i..$last]|ForEach-Object {[regex]::Escape($_)})-join'|')+')$'
 $batch='natural-native-other-'+[int]($i/60)
 $manifest="$root/outputs/d2-coop-lab/research/asset-exports/$batch/manifest.json"
 if(Test-Path $manifest){Write-Output "Already exported $batch";continue}
 $found=@(& "$root/outputs/d2-coop-lab/Read-D2Assets.ps1" -NamePattern $pattern -CatalogPattern '.*' -Export -Batch $batch -Limit 1000)
 Write-Output "$batch : $($found.Count) resources"
}
