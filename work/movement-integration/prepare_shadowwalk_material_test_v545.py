from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-ShadowWalkMaterial-v540.ps1').read_text(encoding='utf-8-sig')
def once(a,b):
 global s
 assert s.count(a)==1,a;s=s.replace(a,b)
once(" $peerMaterialBefore=Q host", " $observerMaterialBefore=Q host '--material-params' 1\n $observerMaterialBefore|ConvertTo-Json -Depth 8|Set-Content \"$out/observer-material-before.json\"\n $peerMaterialBefore=Q host")
once("player=$player;peerMaterial=", "player=$player;materialState=(S host '.shadowwalk-material.json');peerMaterial=")
once(" $checks.Add('Read-only material observer preserves stable original peer parameter table')",''' $checks.Add('Original peer parameter table restores after bounded native material candidate')
 $observerAfter=Q host '--material-params' 1
 $observerAfter|ConvertTo-Json -Depth 8|Set-Content "$out/observer-material-after.json"
 if(!$observerAfter.stable-or!$observerMaterialBefore.stable-or(Compare-Object @($observerMaterialBefore.parameters|ConvertTo-Json -Compress -Depth 4) @($observerAfter.parameters|ConvertTo-Json -Compress -Depth 4))){throw 'Observer local body material changed'}
 $materialActive=@($rows|Where-Object {$_.materialState.active-and$_.materialState.variant-eq33-and$_.peerMaterial.stable})
 if($materialActive.Count-lt5){throw 'Material loop not observed'}
 foreach($row in $materialActive){
  $effect=@($row.peerMaterial.parameters|Where-Object id -eq 7);$fade=@($row.peerMaterial.parameters|Where-Object id -eq 6)
  if($effect.Count-ne1-or$effect[0].words[0]-ne4-or$fade.Count-ne1-or[Math]::Abs([double]$fade[0].floats[0]-.45)-gt.0001){throw 'Native material values differ from bounded candidate'}
 }
 $lastMaterial=$rows[-1].materialState
 if($lastMaterial.active-or$lastMaterial.refused-or$lastMaterial.starts-ne1-or$lastMaterial.restores-ne1){throw 'Material ownership or retirement did not pass'}
 [ordered]@{loopSamples=$materialActive.Count;state=$lastMaterial;restoredPeer=$true;observerUnchanged=$true;scope='Native setter and exact owner cleanup only; visual acceptance separate'}|ConvertTo-Json -Depth 8|Set-Content "$out/material-observations.json"
 $checks.Add('Native effect4/partial fade apply only to exact peer, original table restored and local observer renderer unchanged')''')
s=s.replace("scope='Read-only native material parameter correlation with verified533 remote body and particles. Native Steam source/observer recordings, original FP, checkpoint and protected saves retained. Material mutation and full Shadow Walk coverage unverified.'", "scope='Isolated544 native partial-fade material feasibility on533 remote body/particles. Native Steam source/observer recording and exact material restoration; visual result pending review. Original FP, checkpoints and protected files retained. Full Shadow Walk incomplete.'")
p=N/'Test-ShadowWalkMaterial-v545.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkMaterial-v540.ps1').read_text().replace('540','545').replace('ShadowWalkMaterialObservation','ShadowWalkMaterialFeasibility');(W/'Run-ShadowWalkMaterial-v545.ps1').write_text(s,encoding='utf-8')
print('Prepared545 candidate ownership/restoration and paired visual comparison')
