from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeNpc-v102.ps1').read_text(encoding='utf-8-sig')
a=s.index(' $npc=@($catalog.npcs|Where-Object')
b=s.index(' $dirty=$true',a)
s=s[:a]+''' # Fixed native floor placements from inspected v99; patrol positions are
 # evidence of eligible nearby targets, never camera placement instructions.
 $sx=-48.59;$sy=-155.07;$sz=68.1
 $npcs=@($catalog.npcs|Where-Object {
  $_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')-and
  [Math]::Abs([double]$_.position[2]-66.4)-lt0.5-and
  [Math]::Pow([double]$_.position[0]+48.59,2)+[Math]::Pow([double]$_.position[1]+150.87,2)-lt100
 })
 if(!$npcs.Count){throw 'No upper-floor tutorial guard near the proven balcony cast placement'}
 foreach($n in $npcs){if(!$n.unique-or!$n.reciprocal-or$n.definition-ne'models/characters/def/generic/dunwall_guard_male_small_01.def'){throw 'Tutorial guard identity ambiguous'}}
 $npc=$npcs[0];$allowedTargetHandles=@($npcs|ForEach-Object {[Convert]::ToUInt32($_.handle.Substring(2),16)})
 $npcs|ConvertTo-Json -Depth 6|Set-Content "$out/eligible-native-guards.json"
'''+s[b:]
old="if($validTargets.Count-lt20-or@($validTargets.targets|Where-Object definition -eq $npc.definition).Count-lt10-or@($validTargets.targets|Where-Object {$_.definition-like'*/d2coop/*'}).Count)"
new="if($validTargets.Count-lt20-or@($validTargets.targets|Where-Object {$_.definition-eq$npc.definition-and$_.handle-in$allowedTargetHandles}).Count-lt10-or@($validTargets.targets|Where-Object {$_.definition-like'*/d2coop/*'}).Count)"
assert old in s
s=s.replace(old,new)
(P/'Test-MesmerizeNpc-v104.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1'
p.write_text(p.read_text(encoding='utf-8-sig').replace('Test-MesmerizeNpc-v102.ps1','Test-MesmerizeNpc-v104.ps1'))
print('Fixed-footprint balcony fixture prepared')
