$root='C:\Users\adako\Documents\Codex\2026-10-08\i-x20';$lab="$root\outputs\d2-coop-lab";$cand="$root\work\movement-integration\candidate-npc-combat-v1214cd"
foreach($role in 'host','client'){
 & "$lab\Send-D2Command.ps1" -Role $role -Command 'coop_tables_probe' -WaitConsumed | Out-Null
 Start-Sleep -Milliseconds 200
 Copy-Item "$root\work\lab-instances\$role\coop-status.json.native-tables.json" "$cand\native-tables-$role.json"
 $t=Get-Content "$cand\native-tables-$role.json" -Raw|ConvertFrom-Json
 "$role loaded=$($t.loaded) found=$($t.found)/$($t.expected) productMode=$($t.productMode)"
}
