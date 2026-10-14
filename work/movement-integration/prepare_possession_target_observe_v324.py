from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Observe-NativePossession-v322.ps1').read_text()
s=s.replace('[int]$Pitch=30','[int]$Pitch=0')
s=s.replace("function Q($r,$mode,$arg=0)","function O($r,$mode,$arg=0){$raw=if($arg){& \"$lab/bin/d2_runtime_probe.exe\" $pids[$r] $mode $arg}else{& \"$lab/bin/d2_runtime_probe.exe\" $pids[$r] $mode};$code=$LASTEXITCODE;if($code){return [pscustomobject]@{readValid=$false;exitCode=$code}};return($raw|ConvertFrom-Json)}\nfunction Q($r,$mode,$arg=0)")
s=s.replace("C host 'coop_possession_fixture 40000'","C host 'coop_possession_fixture human40000'")
needle=" $checks.Add('Exact Corvo Possession item acquired through native inventory API; no direct spawn, AI or damage invocation')"
assert needle in s
s=s.replace(needle,needle+'''
 $upgrade=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem')
 if($upgrade.Count-ne1-or$upgrade[0].quantity-ne1-or$upgrade[0].type-ne20){throw 'Exact human-Possession upgrade not acquired'}
 $upgrade|ConvertTo-Json -Depth 8|Set-Content "$out/human-upgrade.json"
 $catalog=Q host '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/aim-npc-catalog.json"
 $eligible=@($catalog.npcs|Where-Object {$_.unique-and$_.reciprocal-and$_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')-and$_.definition-eq'models/characters/def/generic/dunwall_guard_male_small_01.def'-and[Math]::Abs([double]$_.position[2]-66.4)-lt.5-and[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)-lt400}|Sort-Object {[Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+155.07,2)})
 if(!$eligible.Count){throw 'No verified upper-floor tutorial guard within twenty metres'}
 $target=$eligible[0];$yaw=[Math]::Atan2([double]$target.position[1]+155.07,[double]$target.position[0]+48.59)*180/[Math]::PI
 C host ('setviewpos -48.59 -155.07 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 [ordered]@{target=$target;yaw=$yaw;placement='Only yaw aims at verified native guard; original player floor retained'}|ConvertTo-Json -Depth 8|Set-Content "$out/aim-guard.json"
 Q host '--components' $target.handle|ConvertTo-Json -Depth 12|Set-Content "$out/target-components-before.json"
 $checks.Add('Exact human-upgrade native quantity acquired and stable reciprocal tutorial guard selected by read-only world identity')
''')
s=s.replace('Native Corvo LT1800 then release at grounded floor','Native Corvo LT1800 then release at verified human target')
s=s.replace("player=(Q host '--player-states');sword=(Q host '--player-sword');power=(Q host '--player-possession');component=(Q host '--object' $power.component)","player=(O host '--player-states');sword=(O host '--player-sword');power=(O host '--player-possession');component=(O host '--object' $power.component);targetComponents=(O host '--components' $target.handle)")
start=s.index(' foreach($variant in 26,27)');end=s.index(' $accepted|ConvertTo-Json',start)
s=s[:start]+" if(!$accepted.Count){throw 'No strict pre-transfer Possession phase observed'}\n"+s[end:]
s=s.replace(" $checks.Add('Strict reader repeatedly accepts native333ms entry and200ms aim with exact Corvo arm/equipment ownership and Running0+UsePower63')"," $checks.Add('Strict reader observes pre-transfer cast; unsupported target-transfer contexts remain excluded from presentation')")
anchor=" $scenes|ConvertTo-Json -Depth 16|Set-Content \"$out/possession-scenes.json\""
s=s.replace(anchor,anchor+'''
 $possessed=@($rows|Where-Object {$_.player.stance-eq14-or14-in@($_.player.active.id)})
 [ordered]@{possessedState14Samples=$possessed.Count;manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;strictPhases=@($accepted.variant|Sort-Object -Unique);nativeScenes=@($scenes.asset|Sort-Object -Unique);scope='State14 is statically identified candidate possession state; raw target ownership and visual transfer still require review'}|ConvertTo-Json -Depth 8|Set-Content "$out/target-observation-summary.json"
 if(!$possessed.Count-or$manaAfter.currentCandidate-ge$manaBefore.currentCandidate){throw 'Valid-target possession state/mana evidence not established'}
 $checks.Add('Native state14 and mana consumption observed after normal input; target ownership remains under investigation')
''')
s=s.replace("  $checks.Add('Both isolated checkpoints", "  $u=@((Q host '--player-inventory').items|Where-Object name -eq 'components/characters/player/base/attributes/possession_parasite.inventoryitem');if($u.Count-ne1-or$u[0].quantity){throw 'Human upgrade remains acquired after checkpoint'}\n  $checks.Add('Both isolated checkpoints")
s=s.replace('empty-target cast only; valid-target possession, remote movement/VFX/gameplay remain unverified','finite native human-upgrade and verified guard-target experiment; transfer ownership, remote movement/VFX/gameplay remain unverified')
(P/'Observe-NativePossessionTarget-v324.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'PossessionNetwork','PossessionObserve'","'PossessionTargetObserve','PossessionNetwork','PossessionObserve'")
anchor="  'PossessionNetwork'{";s=s.replace(anchor,"  'PossessionTargetObserve'{& \"$root/work/natural-moveset/Observe-NativePossessionTarget-v324.ps1\" -Take $Take -Pitch $Pitch}\n"+anchor);p.write_text(s)
print('Prepared source-only human target observation with mandatory native/checkpoint rollback')
