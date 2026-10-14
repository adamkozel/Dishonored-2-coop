$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$refs=Get-Content "$root/work/moveset-growth/native-reference-index.json" -Raw|ConvertFrom-Json
$selected=@($refs|Where-Object {$_.set-match'/left_arm\.animset$'-and$_.alias-match'^ply_leftarm_power_(bendtime|blink|darkvision|decoy|devourswarm|mesmerize|phantasmal|possession|shadowgrapple|windblast)_(fire|select|pull|no_fire)'})
$paths=@($selected.asset|Sort-Object -Unique)+@('models/characters/player/emily/player_leftarm_emily.md6skl')
$pattern='^(?:'+(($paths|ForEach-Object {[regex]::Escape($_)})-join'|')+')$'
$selected|ConvertTo-Json -Depth 6|Set-Content "$PSScriptRoot/power-reference-selection.json"
& "$root/outputs/d2-coop-lab/Read-D2Assets.ps1" -NamePattern $pattern -CatalogPattern '.*' -Export -Batch natural-power-reference -Limit 1000
