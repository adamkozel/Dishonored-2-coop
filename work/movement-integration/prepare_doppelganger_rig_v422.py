from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(N/'Observe-DoppelgangerWorld-v417.ps1').read_text(encoding='utf-8-sig')
anchor='   $cloneCaptured=$true'
assert s.count(anchor)==1
s=s.replace(anchor,'''   $rig=Q client '--skeleton' $clones[0].handle;$rig|ConvertTo-Json -Depth 15|Set-Content "$out/native-clone-skeleton.json"
   if($rig.model-ne'models/characters/female/emily/head01/emily_head01.md6'-or!$rig.nativeNamesValid-or$rig.joints.Count-lt27-or$rig.joints.Count-gt256){throw 'Native clone rig scope differs'}
   $checks.Add('Native clone model, named skeleton and complete local/global pose captured; external pose stability recorded separately')
'''+anchor)
(N/'Observe-DoppelgangerRig-v422.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerWorld-v417.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-world-ownership-v417','doppelganger-rig-v422').replace('Observe-DoppelgangerWorld-v417','Observe-DoppelgangerRig-v422').replace('DoppelgangerWorldObserve','DoppelgangerRigObserve')
(W/'Run-DoppelgangerRig-v422.ps1').write_text(s,encoding='utf-8')
print('Prepared native clone rig capture422 without modifying live build418')
