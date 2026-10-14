from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
s=(N/'Test-ShadowWalkFx-v535.ps1').read_text(encoding='utf-8-sig')
def once(a,b):
 global s
 assert s.count(a)==1,a;s=s.replace(a,b)
once(" $initialMovement=$m;",''' $peerMaterialBefore=Q host '--material-params' (S host).probeActorHandle
 if(!$peerMaterialBefore.stable){throw 'Stable peer material baseline required'}
 $peerMaterialBefore|ConvertTo-Json -Depth 8|Set-Content "$out/peer-material-before.json"
 $initialMovement=$m;''')
once("player=$player;shadowFx=", "player=$player;peerMaterial=(O host '--material-params' (S host).probeActorHandle);shadowFx=")
once(" C client 'coop_shadowwalk_fixture off';$w.Restart()",''' $materialAfter=Q host '--material-params' (S host).probeActorHandle
 $materialAfter|ConvertTo-Json -Depth 8|Set-Content "$out/peer-material-after.json"
 if(!$materialAfter.stable-or(Compare-Object @($peerMaterialBefore.parameters|ConvertTo-Json -Compress -Depth 4) @($materialAfter.parameters|ConvertTo-Json -Compress -Depth 4))){throw 'Observed peer material changed during read-only experiment'}
 $checks.Add('Read-only material observer preserves stable original peer parameter table')
 C client 'coop_shadowwalk_fixture off';$w.Restart()''')
s=s.replace("scope='Body-only native Shadow Walk entry, idle, bounded forward/backward crawl and requested exit through native Steam. Original FP preserved. Material, VFX, attacks and untested traversal remain unfinished. Checkpoint and protected saves restored; actual remote/source videos require visual review.'", "scope='Read-only native material parameter correlation with verified533 remote body and particles. Native Steam source/observer recordings, original FP, checkpoint and protected saves retained. Material mutation and full Shadow Walk coverage unverified.'")
p=N/'Test-ShadowWalkMaterial-v540.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-ShadowWalkFx-v535.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-fx-v535','native-shadowwalk-material-v540').replace('Test-ShadowWalkFx-v535','Test-ShadowWalkMaterial-v540').replace("operation='ShadowWalkBody'","operation='ShadowWalkMaterialObservation'")
(W/'Run-ShadowWalkMaterial-v540.ps1').write_text(s,encoding='utf-8')
print('Prepared540 read-only material correlation with prior native fixture recovery')
