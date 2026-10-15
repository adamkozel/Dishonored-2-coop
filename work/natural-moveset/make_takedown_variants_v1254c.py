"""v1254c: side and front approaches start crouched (1254a/b: a standing attacker beside or in front was noticed).
v1254b: side placements at ~115 degrees from the guard's facing (beside and slightly behind; 1254a's 90 degrees was seen),
and drop/slide chokes tap the choke button while falling or sliding (1254a: a held choke landed into a plain rear choke).
A sword-combat finisher (census-1255 fatality) counts as a lethal takedown (1254a front host lethal played one).
Derives Test-TakedownVariants-v1254b.ps1 from Test-PairedVictim-v1248l.ps1: the same product-controller fixture,
filming, sampling, outcome and restoration, with the attacker placed at the guard's side, in front, above (drop) or
behind at sprint distance (slide). The partner must present every census-1253 takedown scene the source played."""
from pathlib import Path
P = Path(__file__).resolve().parent
s = P.joinpath('Test-PairedVictim-v1248l.ps1').read_bytes().decode('utf-8')
nl = '\r\n' if '\r\n' in s else '\n'
def rep(a, b, count=1):
    global s
    assert s.count(a) == count, (a[:100], s.count(a)); s = s.replace(a, b)
lines = s.split(nl)
param = lines[0]
assert param.startswith('param(')
lines[0] = ("param([ValidatePattern('^[a-z0-9-]+$')][string]$Take='takedown-variants-v1254c',[string]$Candidate='candidate-npc-combat-v1214g',"
            "[ValidateSet('back','left','right','front','drop','slide')][string[]]$Approaches=@('left','right','front'),"
            "[string[]]$Sources=@('client','host'),[string[]]$Kinds=@('choke','lethal'))")
# drop the paired-victim history comments (kept in Test-PairedVictim-v1248*.ps1) and add this harness's header
body = [l for l in lines[2:] if not l.startswith('# ')]
head = [lines[0], lines[1],
        '# 1254c (1214g, #11): side and front approaches start crouched (the source crouches before the first placement).',
        '# 1254b (1214d, #11): side placements at ~115 degrees (beside, slightly behind); drop/slide chokes tap the choke button.',
        '# 1254 (1214c, #11): takedown variants under the product controller (natural guard AI, NPC stream on). Derived from',
        '# Test-PairedVictim-v1248l.ps1 by make_takedown_variants_v1254.py. The attacker is placed at the guard\'s side (0.9 m,',
        '# facing it), in front (1 m), 3 m above and slightly behind (drop; attack/choke pressed while falling) or 4.5 m behind',
        '# (sprint, crouch into a slide, then attack/choke). Pass (runtime): the source plays a native takedown, the partner',
        '# presents every census-1253 scene id it played, both worlds agree on the outcome and the victim ends within 2 m.']
s = nl.join(head + body)
rep("function F($v){([double]$v).ToString('F5',$inv)}",
    "function F($v){([double]$v).ToString('F5',$inv)}" + nl +
    "function PlaceCmd($g,$approach){$fx=[double]$g.orientation[0];$fy=[double]$g.orientation[1];$x=[double]$g.position[0];$y=[double]$g.position[1];$z=68.1;$look=[Math]::Atan2($fy,$fx)*180/[Math]::PI" + nl +
    " switch($approach){'back'{$x-=.9*$fx;$y-=.9*$fy}'left'{$x+=-.85*$fy-.4*$fx;$y+=.85*$fx-.4*$fy;$look=[Math]::Atan2([double]$g.position[1]-$y,[double]$g.position[0]-$x)*180/[Math]::PI}'right'{$x+=.85*$fy-.4*$fx;$y+=-.85*$fx-.4*$fy;$look=[Math]::Atan2([double]$g.position[1]-$y,[double]$g.position[0]-$x)*180/[Math]::PI}'front'{$x+=$fx;$y+=$fy;$look+=180}'drop'{$x-=.35*$fx;$y-=.35*$fy;$z+=3}'slide'{$x-=4.5*$fx;$y-=4.5*$fy}}" + nl +
    " 'setviewpos {0} {1} {2} {3} 0' -f (F $x),(F $y),$z.ToString('F2',$inv),$look.ToString('F4',$inv)}" + nl +
    "function SceneLive($path){try{$t=Get-Content -LiteralPath $path -Tail 1 -ErrorAction Stop;$t-match'/player/base/(assassination|choke)/[^\"]*\",\"subscene\"'}catch{$false}}")
place_old = "C $source ('setviewpos {0} {1} 68.1 {2} 0' -f (F ([double]$g.position[0]-.9*$fx)),(F ([double]$g.position[1]-.9*$fy)),$look.ToString('F4',$inv))"
rep(place_old, "C $source (PlaceCmd $g $approach)", 2)
rep("$press={if($kind-eq'choke'){I $source RightShoulder 6500}else{& \"$lab/Send-D2Input.ps1\" -Role $source -RightTrigger 255 -DurationMs 250|Out-Null}}",
    "$press={if($approach-eq'slide'){& \"$lab/Send-D2Input.ps1\" -Role $source -Button LeftThumb -DurationMs 1100 -LeftY 32767|Out-Null;Start-Sleep -Milliseconds 350;& \"$lab/Send-D2Input.ps1\" -Role $source -Button B -DurationMs 700 -LeftY 32767|Out-Null;Start-Sleep -Milliseconds 250}" + nl +
    "    elseif($approach-eq'drop'){Start-Sleep -Milliseconds 150}" + nl +
    "    if($kind-eq'choke'-and$approach-in'drop','slide'){foreach($n in 1..4){I $source RightShoulder 200;Start-Sleep -Milliseconds 180}}elseif($kind-eq'choke'){I $source RightShoulder 6500}else{foreach($n in 1..$(if($approach-in'drop','slide'){4}else{1})){& \"$lab/Send-D2Input.ps1\" -Role $source -RightTrigger 255 -DurationMs 200|Out-Null;if($approach-in'drop','slide'){Start-Sleep -Milliseconds 180}}}}")
rep("if(!$pairedSeen-and$attempts-lt3-and$watch.ElapsedMilliseconds-$lastPress-ge1200){",
    "if(!$pairedSeen-and$attempts-lt3-and$watch.ElapsedMilliseconds-$lastPress-ge$(if($approach-in'drop','slide'){2600}else{1200})-and!(SceneLive \"$out/$key-scenes.ndjson\")){")
rep("if(@($player.active|Where-Object {$_.id-eq41-or$_.id-eq13}).Count){$pairedSeen=$true}",
    "if(@($player.active|Where-Object {$_.id-eq41-or$_.id-eq13}).Count-or$player.chokeSceneValid-or(SceneLive \"$out/$key-scenes.ndjson\")){$pairedSeen=$true}")
rep("$want=if($kind-eq'choke'){'*choke_back*'}else{'*/assassination/*'}", "$want=if($kind-eq'choke'){'*/choke/*'}else{'*/assassination/*'}")
lethal_old_start = "$lethalIds=@(foreach($sc in $row.scenes){"
i = s.index(lethal_old_start); j = s.index("$row.checks.Add(\"Remote phase $v presented\")}", i) + len("$row.checks.Add(\"Remote phase $v presented\")}")
s = s[:i] + ("$ids=@(foreach($sc in $row.scenes){$asset=($sc-split'\\|')[0];$k=$asset-replace'^midnight/gameplay/player/base/','' -replace'\\.midnightscene$','';"
             "$t=@($takedownTable|Where-Object {$_.scene-eq$k});if($t.Count-and$t[0].kind-ne4){$t[0].id}});$row.sceneIds=$ids;if(!$ids.Count){throw 'No census takedown scene played'}"
             "foreach($v in $ids){if(!@($samples|Where-Object {$_.remote.chokeVariant-eq$v-and$_.remote.chokeWeight-gt.8}).Count){throw \"Remote phase $v not presented\"};$row.checks.Add(\"Remote phase $v presented\")}") + s[j:]
rep("foreach($source in $Sources){foreach($kind in $Kinds){", "foreach($approach in $Approaches){foreach($source in $Sources){foreach($kind in $Kinds){")
rep(" }}" + nl + " $dirty=$false", " }}}" + nl + " $dirty=$false")
rep("$key=\"$source-$kind\"", "$key=\"$approach-$source-$kind\"")
rep("$row=[ordered]@{source=$source;kind=$kind;", "$row=[ordered]@{approach=$approach;source=$source;kind=$kind;")
rep("operation='PairedVictim1248l'", "operation='TakedownVariants1254'", 2)
rep("\"$($_.source)-$($_.kind): $($_.failure)\"", "\"$($_.approach)-$($_.source)-$($_.kind): $($_.failure)\"")
k = s.index("scope='paired_victim_alignment: rear choke"); e = s.index("'})", k)
s = s[:k] + "scope='Takedown variants (side, front, drop, slide; choke and lethal; both source roles) on a host-owned NPC under the product controller: native scene, partner phases, shared outcome and victim position; fixture restored after every case.'" + s[e + 1:]
rep("\"PASS_PAIRED_VICTIM $($rows.Count) cases\"", "\"PASS_TAKEDOWN_VARIANTS $($rows.Count) cases\"")
rep("native-takedown-scenes-v1253.json", "native-takedown-scenes-v1255.json")
rep("-match'/player/.*/(choke|assassination)/'", "-match'/player/.*/(choke|assassination|fatalities)/'")
rep("if(!@($row.scenes|Where-Object {$_-like$want}).Count)", "if(!@($row.scenes|Where-Object {$_-like$want-or($kind-ne'choke'-and$_-like'*/fatalities/*')}).Count)")
i = s.index('C $source (PlaceCmd $g $approach)'); s = s[:i] + "I $source B 150;Start-Sleep -Milliseconds 500;" + s[i:]
P.joinpath('Test-TakedownVariants-v1254c.ps1').write_bytes(s.encode('utf-8'))
print('ok')
