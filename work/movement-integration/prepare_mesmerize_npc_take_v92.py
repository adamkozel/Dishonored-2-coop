from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizePartner-v91.ps1').read_text(encoding='utf-8-sig')
needle=" C client 'setviewpos -33.5 -153 68.1 90 0';C host 'setviewpos -30.7 -151.8 68.1 203 0'"
assert needle in s
s=s.replace(needle,''' $catalog=Q client '--world-npcs';$catalog|ConvertTo-Json -Depth 8|Set-Content "$out/source-npcs-before.json"
 $npc=@($catalog.npcs|Where-Object spawnerName -eq 'ld_spawner_guard_tutorial_1')
 if($npc.Count-ne1-or!$npc[0].unique-or!$npc[0].reciprocal-or$npc[0].definition-ne'models/characters/def/generic/dunwall_guard_male_small_01.def'){throw 'Exact native tutorial guard identity unavailable'}
 $npc=$npc[0];$sx=[double]$npc.position[0];$sy=[double]$npc.position[1]-5;$sz=[double]$npc.position[2]+1.7
 if($sx-lt-48-or$sx-gt-40-or$sy-lt-163-or$sy-gt-150-or$sz-lt67-or$sz-gt69){throw 'Tutorial guard outside known Dunwall corridor'}
 C client ('setviewpos {0} {1} {2} 90 0' -f $sx,$sy,$sz)
 C host ('setviewpos {0} {1} {2} 203 0' -f ($sx+2.8),($sy+1.2),$sz)
''',1)
s=s.replace("   if($cameraX-lt-31.5-or$cameraX-gt-30-or$cameraY-lt-161-or$cameraY-gt-147){throw 'World evidence camera outside tested room bounds'}", "   if([Math]::Abs($committed.transform[0]-$sx)-gt10-or[Math]::Abs($committed.transform[1]-$sy)-gt15){throw 'Committed NPC-control world outside source corridor'}",1)
s=s.replace("   C host ('setviewpos {0} {1} 68.1 126.8699 0' -f $cameraX,$cameraY)", "   C host ('setviewpos {0} {1} {2} 90 0' -f ($sx+0.8),$sy,$sz)",1)
s=s.replace(" if(!$filter.installed-or($filter.blocked-$filterBefore.blocked)-lt1-or$filter.readFailed-ne$filterBefore.readFailed){throw 'Native local partner eligibility was not cleanly excluded'}", " if(!$filter.installed-or($filter.otherAccepted-$filterBefore.otherAccepted)-lt1-or$filter.readFailed-ne$filterBefore.readFailed){throw 'Ordinary native NPC eligibility was not preserved'}",1)
s=s.replace(" if($validTargets.Count-lt20-or@($validTargets|Where-Object {@($_.targets).Count}).Count){throw 'Partner remained a native Mesmerize target'}", " if($validTargets.Count-lt20-or@($validTargets.targets|Where-Object definition -eq $npc.definition).Count-lt10-or@($validTargets.targets|Where-Object {$_.definition-like'*/d2coop/*'}).Count){throw 'Ordinary NPC target missing or partner acquired'}",1)
s=s.replace(" if(@($rows.eyes.eyes).Count){throw 'No-target source invented a receiver eye'}", " if(!@($rows.eyes.eyes).Count-or@((S host '.mesmerize-eyes.json').eyes).Count){throw 'Enemy target did not produce and retire a remote eye'}",1)
s=s.replace(" $checks.Add('Native local Mesmerize eligibility excludes the exact partner before target insertion; valid world snapshots retain zero targets and no remote target eye is invented')", " $checks.Add('Ordinary native tutorial NPC remains eligible and receives source-owned beam/head effects; co-op partner is absent from native targets, remote eye presents and retires')",1)
s=s.replace('Exact local partner is excluded before native target insertion. Ordinary NPC target preservation requires a separate positive control.','Ordinary native tutorial guard targeting is the positive control for the local partner filter. Native NPC effects are observed read-only and eye presentation remains cosmetic.')
(P/'Test-MesmerizeNpc-v92.ps1').write_text(s,encoding='utf-8')
print('NPC positive control prepared')
