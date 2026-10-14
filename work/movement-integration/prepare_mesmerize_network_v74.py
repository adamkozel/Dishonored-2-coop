from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';I=R/'work/movement-integration'
s=(P/'Test-MesmerizeNetwork-v73.ps1').read_text()
before=''' if($eventsSeen.Count-ne3){throw "Expected three distinct remote phase events, observed $($eventsSeen.Count)"}
 $checks.Add('All three source phase incarnations drive remote full-body Mesmerize through native Steam')'''
after=''' $phaseCoverage=@(foreach($variant in 16,17,18){
  $nativeIds=@($scenes|Where-Object variant -eq $variant|ForEach-Object handle|Select-Object -Unique)
  $remoteIds=@($rows|Where-Object {$_.movement.powerVariant-eq$variant-and$_.movement.powerWeight-gt.5}|ForEach-Object {$_.movement.powerEvent}|Select-Object -Unique)
  if(!$nativeIds.Count-or$nativeIds.Count-ne$remoteIds.Count){throw "Phase $variant source/remote incarnation mismatch: $($nativeIds.Count)/$($remoteIds.Count)"}
  [pscustomobject]@{variant=$variant;nativeHandles=$nativeIds;remoteEvents=$remoteIds}
 })
 $phaseCoverage|ConvertTo-Json -Depth 5|Set-Content "$out/phase-incarnations.json"
 $checks.Add('All exact source scene incarnations, including native aim-loop renewal, match remote phase events through Steam')'''
assert before in s;s=s.replace(before,after)
(P/'Test-MesmerizeNetwork-v74.ps1').write_text(s)
p=I/'Invoke-NativeLabOperation.ps1';p.write_text(p.read_text().replace('Test-MesmerizeNetwork-v73.ps1','Test-MesmerizeNetwork-v74.ps1'))
