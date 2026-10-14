from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Record-HighThrow-Open-v33.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$capture=$null;$failure=$null;", "$nativeRelease=[Collections.Generic.List[object]]::new();$capture=$null;$failure=$null;")
s=s.replace("$target=[string]$targets[0].handle;", """$sourceWorld=Q $Role '--world-npcs'
 $sourceTargets=@($sourceWorld.npcs|Where-Object {$_.spawnerName-eq'ld_spawner_alexi_escape'-and$_.definition-eq'models/characters/def/story/alexi_mayhew.def'})
 if($sourceTargets.Count-ne1-or!$sourceTargets[0].unique-or!$sourceTargets[0].reciprocal){throw 'Source corpse identity unavailable'}
 $sourceTarget=[string]$sourceTargets[0].handle
 $sourceParts=Q $Role '--components' $sourceTarget
 $sourcePhysics=@($sourceParts.components|Where-Object type -eq '.?AVarkCpntPhysicsSystemEvolved@@')
 if($sourcePhysics.Count-ne1){throw 'Source corpse physics unavailable'}
 $sourceTargets|ConvertTo-Json -Depth 10|Set-Content "$out/source-target.json"
 $sourcePhysics|ConvertTo-Json -Depth 10|Set-Content "$out/source-physics-component.json"
 $target=[string]$targets[0].handle;""")
s=s.replace("$rows.Add([pscustomobject]", """if($clock.ElapsedMilliseconds-ge8150-and$clock.ElapsedMilliseconds-lt10400){
   $nb=$clock.ElapsedMilliseconds
   $nativeRelease.Add([ordered]@{beginMs=$nb;stateBefore=$st;physics=(Q $Role '--object' $sourcePhysics[0].pointer);retarget=(Q $Role '--retargeting' '1');victim=(Q $Role '--npc-pose' $sourceTarget);stateAfter=(Q $Role '--player-states');endMs=$clock.ElapsedMilliseconds})
  }
  $rows.Add([pscustomobject]""")
s=s.replace("$rows|ConvertTo-Json -Depth 18", "$nativeRelease|ConvertTo-Json -Depth 20|Set-Content \"$out/native-release.json\"\n $rows|ConvertTo-Json -Depth 18")
s=s.replace("scope=$scope}", "scope=$scope;sourceNativeInspection=$true;sourceTarget=$sourceTarget}")
s=s.replace('source-clock remote release queues a single native event149 after physics handoff','source-only release inspection; remote fallback playback is not yet bound')
(P/'Record-ContextualRelease-Reader-v35.ps1').write_text(s,encoding='utf-8')
