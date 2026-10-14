"""Separate empty native target coverage from the positive-NPC fixture case."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeCrouchMoving-v139.ps1').read_text(encoding='utf-8-sig')
a=s.index(" if(!$filter.installed-or($filter.otherAccepted")
b=s.index('\n\n\n}catch',a)
positive=s[a:b]
replacement=r''' if(!$filter.installed-or$filter.readFailed-ne$filterBefore.readFailed-or$validTargets.Count-lt20){throw 'Native target observation or partner identity coverage failed'}
 $nativeTargets=@($validTargets|ForEach-Object {$_.targets}|Where-Object {$null-ne$_})
 if(!$nativeTargets.Count){
  $links=S host '.mesmerize-links.json';$eyes=S host '.mesmerize-eyes.json'
  $linkEntries=@($rows|ForEach-Object {$_.links.links}|Where-Object {$null-ne$_})
  $eyeEntries=@($rows|ForEach-Object {$_.eyes.eyes}|Where-Object {$null-ne$_})
  if($linkEntries.Count-or$eyeEntries.Count-or$links.refused-ne$linksBefore.refused){throw 'Empty native target set invented receiver target effects'}
  foreach($kind in 0..2){if($links.started[$kind]-ne$linksBefore.started[$kind]-or$links.stopped[$kind]-ne$linksBefore.stopped[$kind]){throw 'Unobserved native target caused a cosmetic link lifecycle'}}
  if($rows[-1].sourceWorld.linkReadFailed-ne$rows[0].sourceWorld.linkReadFailed-or$rows[-1].sourceWorld.eyeReadFailed-ne$rows[0].sourceWorld.eyeReadFailed){throw 'Empty target evidence includes unreadable source effect samples'}
  [ordered]@{context='native-empty-targets';validSourceWorldSamples=$validTargets.Count;receiverLinkSamples=$linkEntries.Count;receiverEyeSamples=$eyeEntries.Count;sourceFailedReads=0;positiveNpcEligibilityClaimed=$false}|ConvertTo-Json|Set-Content "$out/target-context-summary.json"
  $checks.Add('Repeated valid native empty target sets produce no receiver eyes/links or invented target effects; positive NPC eligibility is covered separately by v138')
 }else{
'''+positive+r'''
  [ordered]@{context='native-positive-npc-targets';validSourceWorldSamples=$validTargets.Count;positiveNpcEligibilityClaimed=$true}|ConvertTo-Json|Set-Content "$out/target-context-summary.json"
 }
'''
s=s[:a]+replacement+s[b:]
s=s.replace('Native source beam/trail/head transforms are reproduced by cosmetic-only native FX;', 'When native NPC targets exist their exact beam/trail/head transforms drive cosmetic-only FX; otherwise validated empty target sets must produce no receiver target effects;')
(P/'Test-MesmerizeCrouchMoving-v143.ps1').write_text(s,encoding='utf-8')
