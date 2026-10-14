from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeNpc-v104.ps1').read_text(encoding='utf-8-sig')
s=s.replace(" $visualBefore=S host '.mesmerize-visual.json';", " $linksBefore=S host '.mesmerize-links.json';$linksBefore|ConvertTo-Json -Depth 6|Set-Content \"$out/links-before.json\"\n $visualBefore=S host '.mesmerize-visual.json';",1)
s=s.replace("eyes=(S host '.mesmerize-eyes.json');effects=$fx", "eyes=(S host '.mesmerize-eyes.json');links=(S host '.mesmerize-links.json');sourceWorld=(S client '.mesmerize-world-stream.json');effects=$fx",1)
needle=" $checks.Add('Ordinary native tutorial NPC remains eligible and receives source-owned beam/head effects; co-op partner is absent from native targets, remote eye presents and retires')"
assert needle in s
extra='''
 $links=S host '.mesmerize-links.json';$links|ConvertTo-Json -Depth 6|Set-Content "$out/links-after.json"
 $linkEntries=@($rows|ForEach-Object {$_.links.links}|Where-Object {$null-ne$_})
 $sourceEffects=@($validTargets|ForEach-Object {$_.targets.effects}|Where-Object {$null-ne$_})
 $sourceTargetHandles=@($validTargets|ForEach-Object {$_.targets.handle}|Where-Object {$null-ne$_}|Select-Object -Unique)
 if($links.refused-ne$linksBefore.refused-or@($links.links).Count){throw 'Remote target effect identity refused or active after retirement'}
 foreach($kind in 1,2,3){
  $ids=@($sourceEffects|Where-Object kind -eq $kind|Select-Object -ExpandProperty id -Unique)
  $received=@($linkEntries|Where-Object kind -eq $kind)
  if(!$ids.Count-or!$received.Count-or$links.started[$kind-1]-le$linksBefore.started[$kind-1]-or
    ($links.started[$kind-1]-$linksBefore.started[$kind-1])-ne($links.stopped[$kind-1]-$linksBefore.stopped[$kind-1])){throw "Native target effect kind $kind did not present and retire"}
  if(@($received|Where-Object {$_.sourceId-notin$ids-or$_.target-notin$sourceTargetHandles}).Count){throw 'Receiver invented an unobserved native effect or target incarnation'}
 }
 foreach($row in $rows){foreach($link in $row.links.links){if($link.owner-ne$row.visual.handle){throw 'Target effect belongs to something other than the cosmetic presentation model'}}}
 $active=Get-Content "$out/observer-effects-active.json" -Raw|ConvertFrom-Json
 $nativeLinks=@($active.effects|Where-Object {$_.name-like'*/mesmerize_v2/link/*'})
 if(!@($nativeLinks|Where-Object name -like '*mesmerize_v2_link_01.fx').Count-or!@($nativeLinks|Where-Object name -like '*mesmerize_v2_link_head_01.fx').Count){throw 'Receiver native FX manager did not confirm beam and head records'}
 [ordered]@{started=@(0..2|ForEach-Object {$links.started[$_]-$linksBefore.started[$_]});stopped=@(0..2|ForEach-Object {$links.stopped[$_]-$linksBefore.stopped[$_]});updated=@(0..2|ForEach-Object {$links.updated[$_]-$linksBefore.updated[$_]});refused=$links.refused-$linksBefore.refused;sourceNativeEffects=@($sourceEffects|Group-Object kind|Select-Object Name,Count);nativeReceiverRecords=$nativeLinks}|ConvertTo-Json -Depth 8|Set-Content "$out/target-links-summary.json"
 $checks.Add('Source-observed beam, trail and target-head effect incarnations drive bounded native receiver FX owned only by the cosmetic model; all retire without NPC writes or duplicate damage calls')
'''
s=s.replace(needle,needle+extra,1)
s=s.replace('AI distraction/target beams/gameplay outcome consistency remain pending.','Native source beam/trail/head transforms are reproduced by cosmetic-only native FX; target gameplay and NPC positions remain separately owned and unsynchronized. Five target identities and fifteen native FX records are the current bound. Continuous visual acceptance and moving-target smoothing remain pending.')
(P/'Test-MesmerizeLinks-v106.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';r=p.read_text(encoding='utf-8-sig')
r=r.replace("'MesmerizePartner','MesmerizeNpc','MesmerizeVisual'", "'MesmerizePartner','MesmerizeNpc','MesmerizeLinks','MesmerizeVisual'")
needle="  'MesmerizeNpc'{& \"$root/work/natural-moveset/Test-MesmerizeNpc-v104.ps1\" -Take $Take -Pitch $Pitch}"
assert needle in r
r=r.replace(needle,needle+"\n  'MesmerizeLinks'{& \"$root/work/natural-moveset/Test-MesmerizeLinks-v106.ps1\" -Take $Take -Pitch $Pitch}")
p.write_text(r)
print('Cosmetic beam/trail/head native test prepared')
