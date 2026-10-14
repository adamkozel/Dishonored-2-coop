from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab'
p=L/'Build-D2SwarmRatVisualAssets.ps1';s=p.read_text(encoding='utf-8-sig').replace('swarm-rat-mainclip-v294','swarm-rat-leaf-v296')
needle='[void][IO.Directory]::CreateDirectory($package)';assert s.count(needle)==1
s=s.replace(needle,"$resources+=@(@{Name='animtree/d2coop_swarm_rat_visual.animtree';File='assets/swarm_rat_visual.animtree';Kind='animtree';Class='animTree'},@{Name='components/d2coop/swarm_rat_visual.cpntanimevolved';File='assets/swarm_rat_visual.cpntanimevolved';Kind='cpntanimevolved';Class='cpntAnimEvolved'})\n"+needle);p.write_text(s)
p=P/'Test-SwarmRatVisualNetwork-v295.ps1';s=p.read_text(encoding='utf-8-sig')
needle=" if($rats.Count-ne1){throw 'Expected one cached cosmetic rat entity'}";assert needle in s
s=s.replace(needle,""" $pose0=Get-Content "$out/visual-early-skeleton-0.json" -Raw|ConvertFrom-Json
 $pose1=Get-Content "$out/visual-early-skeleton-1.json" -Raw|ConvertFrom-Json
 if(!$pose0.stable-or!$pose1.stable-or$pose0.joints.Count-ne31-or$pose1.joints.Count-ne31){throw 'Stable31-joint animation observations required'}
 $poseDeltas=@(for($i=1;$i-lt31;$i++){
  if($pose0.joints[$i].nativeName-ne$pose1.joints[$i].nativeName){throw 'Joint identity changed'}
  $delta=0.0;for($j=0;$j-lt12;$j++){$delta=[Math]::Max($delta,[Math]::Abs($pose0.joints[$i].localPose[$j]-$pose1.joints[$i].localPose[$j]))}
  [pscustomobject]@{joint=$pose0.joints[$i].nativeName;maxLocalDelta=$delta}
 })
 $changing=@($poseDeltas|Where-Object maxLocalDelta -gt.01)
 [ordered]@{treeActive=$pose1.treeActive;treeOutput=$pose1.treeHasOutput;changingJoints=$changing.Count;deltas=$poseDeltas;visualAcceptance=$false}|ConvertTo-Json -Depth 5|Set-Content "$out/animation-output-summary.json"
 if($changing.Count-lt4){throw 'Native rat walking did not produce significant joint-local motion'}
 $checks.Add('At least4 joints change locally above0.01 over120ms; root displacement and floating-point noise cannot satisfy this animation check')
"""+needle)
s=s.replace('One owned cosmetic native rat model starts animation-only Midnight walking and follows opaque peer rat identity; native gameplay swarm is not duplicated','One owned cosmetic native rat model follows opaque peer rat identity with a native walking animleaf; no peer gameplay swarm')
s=s.replace('Source stream retirement stops owned rat Midnight scene and hides cached model','Source stream retirement hides cached native rat animleaf model')
s=s.replace('Midnight clock activity alone does not prove animation; joint output and visual acceptance remain separately assessed.','A dedicated native walking animleaf is tested with joint-local motion thresholds; visual acceptance remains separately assessed.')
(P/'Test-SwarmRatVisualNetwork-v296.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v295.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v295.ps1','Test-SwarmRatVisualNetwork-v296.ps1'))
