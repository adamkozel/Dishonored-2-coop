# Ghidra MCP annotations for the native timed-parry path (exact image B671F824).
# Every call goes through the pinned ghidra-mcp bridge; transcripts are kept.
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$py="$root/work/tools/ghidra-mcp-6.0.0/.venv/Scripts/python.exe";$client="$root/outputs/d2-coop-lab/analysis/ghidra/mcp_client.py"
$out=Join-Path $PSScriptRoot 'npc-parry-ghidra-v1015';New-Item -ItemType Directory -Force $out|Out-Null
$n=0
function M($tool,$a){
 $script:n++;$f=Join-Path $out ('{0:d2}-{1}.json' -f $script:n,$tool);$af="$f.args"
 ($a+@{program='Dishonored2.exe'})|ConvertTo-Json -Compress|Set-Content -LiteralPath $af -Encoding utf8
 & $py $client $tool --args-file $af --output $f 2>$null|Out-Null;if($LASTEXITCODE){throw "MCP $tool failed: $f"}
}
$day='2026-10-14 v1015'
foreach($d in 'decompile_function'){foreach($a in '0x1406357d0','0x140636140','0x140859980','0x14060c180','0x1408598c0','0x14071a140','0x140635bf0','0x140635f30'){M $d @{address=$a}}}
$renames=[ordered]@{
 '0x14060c180'='D2_ParryWin_RollPercentChance';
 '0x1408598c0'='D2_AttackerParryable_Check_Hypothesis';
 '0x14071a140'='D2_Health_ApplySignedDelta_Hypothesis';
 '0x140635bf0'='D2_WeaponMeleeNPC_PlayDeclEffect';
 '0x140635f30'='D2_WeaponMeleeNPC_EndAttackImpact'}
foreach($k in $renames.Keys){M 'rename_function_by_address' @{function_address=$k;new_name=$renames[$k]}}
$marks=[ordered]@{
 '0x140859980'="CONFIRMED STATIC ${day}: block result. Parry branch needs (state+58 or state+74>0) and state+59 and attacker cpnt 1439b9bc0 passing 8598c0; else plain block (0). In parry: 60c180 roll -> 2 (presentation 7, 71a140(sm+348,-chance)) or 1 (presentation 0x27). Resolution does NOT stagger the attacker itself.";
 '0x14060c180'="CONFIRMED STATIC ${day}: returns rand(100) < round(table(sm+330)+48[row state+78].f24). Parry-win vs parry-lose is probabilistic inside the parry window.";
 '0x140636140'="CONFIRMED STATIC ${day}: parry-win (result 2) applied by caller 6357d0, not 859980. If weapon decl+128: damage type5 from decl+128 scaled decl+130 sent to ATTACKER (weapon owner) via 884920/event 1439a9d20. Cosmetic branch on type range [1439ce350,354]: victim out & attacker in -> 635bf0 victim decl+158 and event 0x4f to victim; victim in -> 635bf0 attacker decl+168. Co-op 1012 relay skips this entirely (gap).";
 '0x1406357d0'="CONFIRMED STATIC ${day}: after 859980, result2 -> 636140(weapon,victim); 0/1 -> event 0xe6 {victim,result}; always 635f30 + 634890 end attack.";
 '0x14071a140'="HYPOTHESIS ${day}: signed delta; negative builds damage decl 14231f390 amount -x via 884920 to owner; positive -> 719fe0. Called by 859980 parry-win on sm+348."}
foreach($k in $marks.Keys){M 'set_bookmark' @{address=$k;category='D2Coop';comment=$marks[$k]}}
M 'save_program' @{}
Get-ChildItem $out -Filter '*.json'|ForEach-Object{$j=Get-Content $_.FullName -Raw|ConvertFrom-Json;[pscustomobject]@{file=$_.Name;isError=$j.isError}}
