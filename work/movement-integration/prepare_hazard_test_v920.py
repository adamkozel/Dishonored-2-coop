from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcCombat-v919.ps1').read_text().replace('v919','v920')
a=' Start-Sleep -Milliseconds 350\n $attempt=0'
b=''' Start-Sleep -Milliseconds 350
 # Observe the active map near the target; no scripted damage injection.
 $watch.Restart();do{
  $natural=Snap ('hazard-'+[int]($watch.ElapsedMilliseconds/1000))
  if(!$natural.host.combat.enabled-or!$natural.client.combat.enabled){throw 'Native map hazard interrupted combat authority'}
  if($natural.host.combat.worldCommitted-gt0-and$natural.host.combat.committed-eq$natural.client.combat.applied){break}
  Start-Sleep -Milliseconds 1000
 }while($watch.ElapsedMilliseconds-lt12000)
 Save "$out/hazard-observation.json" @{host=$natural.host.combat.worldCommitted;client=$natural.client.combat.worldApplied;scope='Natural map wait, no event injection. Zero outcomes is explicitly unexercised.'}
 $attempt=0'''
assert s.count(a)==1;s=s.replace(a,b)
(N/'Test-NpcCombat-v920.ps1').write_text(s)
