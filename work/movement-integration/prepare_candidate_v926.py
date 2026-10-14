from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
for a,b in [('Build-NpcCombat-v922.ps1','Build-NpcCombat-v926.ps1'),('Deploy-NpcCombat-v922.ps1','Deploy-NpcCombat-v926.ps1')]:
 (W/b).write_text((W/a).read_text().replace('922','926'))
t=(W/'stage_npc_combat_v922.py').read_text().replace('v922','v926').replace("P=W/'candidate-npc-combat-v918'","P=W/'candidate-npc-combat-v922'")
t=t.replace("scope='Adds an explicit", "scope='Accepts both native absent and validated contact geometry for authored trigger effect continuations, including zero-input terminal outcomes. Adds an explicit")
(W/'stage_npc_combat_v926.py').write_text(t)
t=(N/'Test-NpcCombat-v923.ps1').read_text().replace('v923','v927').replace('candidate-npc-combat-v922','candidate-npc-combat-v926')
a=t.index(' $attempt=0\n do{');b=t.index(" $after=Snap 'native-death'",a)
t=t[:a]+''' $attempt=0
 # Native effect continuation is responsible for death; no sword input.
 $watch.Restart();do{
  $hs=S host '.npc-combat.json';$cs=S client '.npc-combat.json'
  if(!$hs.enabled-or!$cs.enabled){break}
  if($hs.deaths-eq1-and$cs.deaths-eq1-and$hs.committed-eq$cs.applied){break}
  Start-Sleep -Milliseconds 80
 }while($watch.ElapsedMilliseconds-lt8000)
'''+t[b:]
t=t.replace('One real controller-triggered native lethal action with exact actor release, native stopped AI and duplicate rejection.', 'Reconstructed native fire event followed by native effect continuation/death, exact actor release, native stopped AI and duplicate rejection; no sword input.')
(N/'Test-NpcCombat-v927.ps1').write_text(t)
