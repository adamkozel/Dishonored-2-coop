from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Observe-NativeLethal-v596.ps1').read_text().replace('NativeLethal596','LethalNetwork605').replace('candidate-choke-breakout-v592','candidate-lethal-v604').replace('archived592','archived604').replace('choke-breakout-bank-manifest-v592','lethal-bank-manifest-v604').replace('version-eq64','version-eq65').replace('version-ne64','version-ne65')
needle='sensors=$objects;midnight='
assert needle in s;s=s.replace(needle,"sword=(Q $source '--player-sword');sensors=$objects;midnight=")
needle="   $checks.Add('Ordinary RT behind native guard starts assassination scene; exact state/ownership still requires analysis')"
assert needle in s;s=s.replace(needle,'''   $checks.Add('Ordinary RT starts the owned native assassination scene')
   $owned=@($samples|Where-Object {$_.player.chokeSceneValid-and$_.player.chokeScene.variant-eq5-and$_.player.chokeScene.durationMs-eq1333})
   if(!$owned.Count){throw 'Independent owned lethal scene reader never validated'};$checks.Add('Exact native state13 and four owned bindings validate fast-back1333')
   if(!@($owned|Where-Object {$_.sword.equipped-and$_.sword.started}).Count){throw 'Native lethal sword-equipped premise not observed'};$checks.Add('Independent native sword equipped/started observed during lethal scene')
   if(!@($samples|Where-Object {$_.movement.chokeVariant-eq5-and$_.movement.chokeWeight-gt.8-and$_.movement.weight-gt.85}).Count){throw 'Remote lethal body phase absent'};$checks.Add('Native Steam remote full-body lethal phase observed')
   $lastEnd=($owned|Measure-Object ms -Maximum).Maximum
   if(!@($samples|Where-Object {$_.ms-gt($lastEnd+300)-and$_.movement.chokeWeight-lt.05-and$_.movement.weight-gt.85}).Count){throw 'Lethal standing recovery absent'};$checks.Add('Native standing recovery restores existing movement')
   $aligned=@($samples|Where-Object {$_.movement.chokeVariant-eq5-and$_.movement.chokeWeight-gt.8-and$_.peer.probeFollowEnabled-and[math]::Abs($_.peer.probeRenderPosition[0]-$_.player.position[0])-lt.3-and[math]::Abs($_.peer.probeRenderPosition[1]-$_.player.position[1])-lt.3})
   if($aligned.Count-lt2){throw 'Remote actor root did not follow native lethal source'};$checks.Add('Remote actor follows independent native source root through lethal animation')''')
s=s.replace('Native ordinary RT250ms behind exact authored guard2; source-only action research, not a remote lethal binding.','Native ordinary RT250ms behind exact authored guard2; independent source and Steam actor checks, victim/world outcome remains separate.')
s=s.replace('Source-only native lethal takedown observation, ordinary RT250ms. Captures exact source phases/state/target and actual game video. No new remote lethal binding or shared victim outcome is claimed.','Fast rear assassination actor-body network binding via existing legacy paired channel, ordinary RT250ms. Source native controls and outcomes unchanged; remote actor animation/root/equipment only. Receiver victim, death/corpse, contact alignment and world outcomes remain unsynchronized by this binding.')
(N/'Test-LethalNetwork-v605.ps1').write_text(s);print('Native/Steam605 lethal validation harness prepared, independent sword-state check included.')
