from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeLinksInterrupt-v108.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$pausedRole=$null\n','$pausedRole=$null;$aiBefore=@{}\n',1)
at=" $linksBefore=S host '.mesmerize-links.json';"
assert at in s
s=s.replace(at,""" foreach($r in 'host','client'){
  $aiBefore[$r]=Q $r '--ai-decision' (S $r).probeActorHandle
  $aiBefore[$r]|ConvertTo-Json -Depth 6|Set-Content "$out/avatar-ai-before-$r.json"
  if($aiBefore[$r].pauseReferences-ne1-or$aiBefore[$r].schedulerActive){throw 'Presentation AI is not held exactly once'}
 }
"""+at)
at="   $interrupted=$true"
assert s.count(at)==1
s=s.replace(at,"""   foreach($r in 'host','client'){
    $ai=Q $r '--ai-decision' (S $r).probeActorHandle
    $ai|ConvertTo-Json -Depth 6|Set-Content "$out/avatar-ai-after-menu-$r.json"
    if($ai.pauseReferences-ne1-or$ai.schedulerActive-or$ai.handle-ne$aiBefore[$r].handle-or$ai.component-ne$aiBefore[$r].component){throw 'Menu released or replaced presentation AI ownership'}
   }
"""+at)
at=" I client;$manaAfter="
assert s.count(at)==1
s=s.replace(at,""" foreach($r in 'host','client'){
  $ai=Q $r '--ai-decision' (S $r).probeActorHandle
  $ai|ConvertTo-Json -Depth 6|Set-Content "$out/avatar-ai-end-$r.json"
  if($ai.pauseReferences-ne1-or$ai.schedulerActive-or$ai.handle-ne$aiBefore[$r].handle-or$ai.component-ne$aiBefore[$r].component){throw 'Presentation AI resumed after stream retirement'}
 }
 $checks.Add('Both exact local clone AI components retain one native pause reference and stay outside the active scheduler across menu/stream retirement')
"""+at)
(P/'Test-MesmerizeLinksInterrupt-v112.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert s.count('Test-MesmerizeLinksInterrupt-v108.ps1')==1;p.write_text(s.replace('Test-MesmerizeLinksInterrupt-v108.ps1','Test-MesmerizeLinksInterrupt-v112.ps1'),encoding='utf-8')
archive=R/'work/movement-integration/candidate-avatar-ai-lifetime-v111'
pins=json.loads((archive/'build.json').read_text(encoding='utf-8-sig'))
pins.update(protocol=40,mesmerizeWorldProtocol=3,mesmerizeWorldBytes=2056,bankVersion=45,clips=276,banks='work/natural-moveset/mesmerize-banks-v77',scope=pins['change'])
(archive/'identity.json').write_text(json.dumps(pins,indent=2),encoding='utf-8')
for p in [P/'Test-MesmerizeLinksInterrupt-v112.ps1',R/'work/movement-integration/Invoke-NativeLabOperation.ps1']:(archive/p.name).write_bytes(p.read_bytes())
print('Prepared runtime ownership regression test')
