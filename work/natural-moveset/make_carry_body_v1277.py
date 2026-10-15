"""Derives Test-CarryBody-v1277.ps1 from make_kill_unconscious_v1271.py's harness for stage 4 (#14): carrying a choked
body. The source chokes the Dunwall tutorial guard under the product controller (both worlds agree it is unconscious),
picks the body up (X held, native carry state 45/46), walks forward in four 1 s steps and drops it (X held). Both worlds'
body positions are sampled throughout. Pass (runtime): the body ends < 2 m apart in the two worlds. Recorded: whether
the body moved in each world, the largest gap while carried, and the stream entry state on both peers."""
from pathlib import Path
P = Path(__file__).resolve().parent
src = P.joinpath('make_kill_unconscious_v1271.py').read_text(encoding='utf-8')
start = src.index("kill = r'''"); end = src.index("'''", start + 11) + 3
carry = r"""kill = r'''   # 1277: carry the unconscious body forward and drop it; both worlds' body positions are sampled.
   $carryRows=[Collections.Generic.List[object]]::new();$carrying=$false
   $bodyAt={param($tag)$sv=try{(Guard $source).position}catch{$null};$ov=try{(Guard $peer).position}catch{$null};$gap=if($sv-and$ov){[Math]::Round([Math]::Sqrt(([double]$sv[0]-$ov[0])*([double]$sv[0]-$ov[0])+([double]$sv[1]-$ov[1])*([double]$sv[1]-$ov[1])),2)}else{$null}
    $ps=try{Q $source '--player-states'}catch{$null};[pscustomobject]@{tag=$tag;source=$sv;observer=$ov;gap=$gap;active=@($ps.active|ForEach-Object {$_.id})}}
   # 1277c: look down in place, then sweep the yaw around the source (1277b: the client's pitch never applied and it faced a shelf).
   $carryRows.Add((& $bodyAt 'before'));$me=try{(Q $source '--player-states').position}catch{(Q $source '--player').candidatePosition}
   $plan=[Collections.Generic.List[object]]::new();foreach($pv in 50,65){$plan.Add(@{yaw=$null;pitch=$pv})};foreach($yv in 0..7){$plan.Add(@{yaw=$yv*45;pitch=55})}
   for($k=0;$k-lt$plan.Count-and!$carrying;$k++){$step=$plan[$k]
    if($null-ne$step.yaw){C $source ('setviewpos {0} {1} {2} {3} 0' -f (F $me[0]),(F $me[1]),(F ([double]$me[2]+1.7)),([double]$step.yaw).ToString('F4',$inv));Start-Sleep -Milliseconds 350}
    for($t=0;$t-lt2;$t++){try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch $step.pitch -Quick|Out-Null;break}catch{Start-Sleep -Milliseconds 300}}
    I $source X 900;Start-Sleep -Milliseconds 1500;$r=& $bodyAt "pickup$k";$r|Add-Member -NotePropertyName step -NotePropertyValue $step;$carryRows.Add($r);$carrying=@($r.active|Where-Object {$_-eq45-or$_-eq46}).Count-gt0}
   Save "$out/$key-carry.json" @($carryRows)
   if(!$carrying){throw 'The source did not pick up the body (no native carry state)'};$row.checks.Add('Source carries the body (native carry state)')
   try{& "$PSScriptRoot/Set-ObserverPitch.ps1" -Role $source -Pitch 0 -Quick|Out-Null}catch{}
   # 1277e: face the fixture's open spot (-33,-153) and walk 6 s with the body (1277d moved only 0.5 m among shelves).
   $mp=try{(Q $source '--player-states').position}catch{$me};$wy=[Math]::Atan2(-153-[double]$mp[1],-33-[double]$mp[0])*180/[Math]::PI
   C $source ('setviewpos {0} {1} {2} {3} 0' -f (F $mp[0]),(F $mp[1]),(F ([double]$mp[2]+1.7)),$wy.ToString('F4',$inv));Start-Sleep -Milliseconds 400
   for($k=0;$k-lt6;$k++){& "$lab/Send-D2Input.ps1" -Role $source -LeftY 32767 -DurationMs 1000|Out-Null;$carryRows.Add((& $bodyAt "walk$k"))}
   I $source X 900;Start-Sleep -Milliseconds 3000;$carryRows.Add((& $bodyAt 'dropped'));Start-Sleep -Milliseconds 2000;$end=& $bodyAt 'settled';$carryRows.Add($end)
   Save "$out/$key-carry.json" @($carryRows)
   foreach($q in 'host','client'){foreach($suffix in '.npc-stream.json','.npc-combat.json'){try{Copy-Item -LiteralPath "$root/work/lab-instances/$q/coop-status.json$suffix" -Destination "$out/$key-carry-$q$suffix" -ErrorAction Stop}catch{}}}
   $b0=$carryRows[0];$moved=[ordered]@{source=if($b0.source-and$end.source){[Math]::Round([Math]::Sqrt(([double]$end.source[0]-$b0.source[0])*([double]$end.source[0]-$b0.source[0])+([double]$end.source[1]-$b0.source[1])*([double]$end.source[1]-$b0.source[1])),2)}else{$null};
    observer=if($b0.observer-and$end.observer){[Math]::Round([Math]::Sqrt(([double]$end.observer[0]-$b0.observer[0])*([double]$end.observer[0]-$b0.observer[0])+([double]$end.observer[1]-$b0.observer[1])*([double]$end.observer[1]-$b0.observer[1])),2)}else{$null}}
   $row.carry=[ordered]@{moved=$moved;maxGap=($carryRows|ForEach-Object {$_.gap}|Measure-Object -Maximum).Maximum;endGap=$end.gap;stillCarrying=@($end.active|Where-Object {$_-eq45-or$_-eq46}).Count-gt0}
   if($moved.source-gt1-and($null-eq$moved.observer-or$moved.observer-lt0.5*$moved.source)){throw "The observer's body did not follow the carry (source moved $($moved.source) m, observer $($moved.observer) m)"}
   if($null-eq$end.gap-or$end.gap-gt2){throw "Carried body ends $($end.gap) m apart in the two worlds (moved: source $($moved.source) m, observer $($moved.observer) m)"}
   $row.checks.Add("Carried body ends $($end.gap) m apart (moved: source $($moved.source) m, observer $($moved.observer) m; max gap $($row.carry.maxGap) m)")
'''"""
src = src[:start] + carry + src[end:]
def rep(old, new):
    global src
    assert src.count(old) == 1, old[:70]; src = src.replace(old, new)
rep("$Take='kill-unconscious-v1271a'", "$Take='carry-body-v1277e'")
rep("[int]$KillAttempts=15)", "[int]$KillAttempts=15)")
rep("operation='KillUnconscious1271'", "operation='CarryBody1277'")
rep("\"# 1271 (stage 4, #14, 1214n): after the choke the source kills the unconscious guard; no combat channel binds an unconscious NPC,\"",
    "\"# 1277 (stage 4, #14): after the choke the source carries the unconscious body and drops it; both worlds' body positions compared. Generated by make_carry_body_v1277.py.\"")
src = src.replace("Stage 4 unshared death: the source chokes the Dunwall tutorial guard under the product controller, then kills the unconscious body; the guard must end dead in both worlds (1214n Dead reconciliation); fixture restored after every case.",
                  "Stage 4 carried body: the source chokes the Dunwall tutorial guard under the product controller, carries the unconscious body forward and drops it; the body must end < 2 m apart in both worlds; fixture restored after every case.")
src = src.replace("\"# so the death reaches the peer only by the Dead reconciliation. Generated by make_kill_unconscious_v1271.py from 1248l.\"", "\"# Derived from make_kill_unconscious_v1271.py (1248l base).\"")
src = src.replace("PASS_KILL_UNCONSCIOUS", "PASS_CARRY_BODY").replace("P.joinpath('Test-KillUnconscious-v1271.ps1')", "P.joinpath('Test-CarryBody-v1277.ps1')")
exec(compile(src.split('"""', 2)[2], 'make_carry_inner', 'exec'))
