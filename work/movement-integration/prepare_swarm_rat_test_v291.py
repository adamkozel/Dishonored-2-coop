from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-SwarmWorldNetwork-v290.ps1').read_text(encoding='utf-8-sig')
s=s.replace('-Role client -Pitch -3','-Role client -Pitch 23')
s=s.replace("peer=(S client '.swarm-world.json')", "peer=(S client '.swarm-world.json');visual=(S client '.swarm-rat-visual.json')")
anchor=" $sourceWorld=S host '.swarm-world.json';"
assert s.count(anchor)==1
s=s.replace(anchor,''' $visualRows=@($worldRows|Where-Object {$_.visual.active-and$_.visual.playing-eq2-and!$_.visual.pendingHide})
 $visual=S client '.swarm-rat-visual.json';$visual|ConvertTo-Json -Depth 8|Set-Content "$out/visual-before-stop.json"
 if($visualRows.Count-lt20-or$visual.created-ne1-or$visual.started-lt1-or$visual.updated-lt20-or$visual.refused){throw "Cosmetic rat proof failed: $($visual.result), stage=$($visual.stage), samples=$($visualRows.Count)"}
 Q client '--components' $visual.handle|ConvertTo-Json -Depth 12|Set-Content "$out/visual-components.json"
 Q client '--midnight'|ConvertTo-Json -Depth 20|Set-Content "$out/visual-midnight.json"
 $ratEntities=Q client '--entities';$rats=@($ratEntities.entities|Where-Object definitionName -eq 'models/powers/d2coop/swarm_rat_visual.def')
 $rats|ConvertTo-Json -Depth 8|Set-Content "$out/visual-entities.json"
 if($rats.Count-ne1){throw 'Expected one cached cosmetic rat entity'}
 $checks.Add('One owned cosmetic native rat model starts animation-only Midnight walking and follows opaque peer rat identity; native gameplay swarm is not duplicated')
'''+anchor)
anchor=" $checks.Add('Remote rat lease retires when source Steam semantic stream stops')"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $hidden=S client '.swarm-rat-visual.json';$hidden|ConvertTo-Json -Depth 8|Set-Content "$out/visual-after-source-stop.json"
 if($hidden.active-or$hidden.pendingHide-or$hidden.scene-or$hidden.hidden-lt1){throw 'Cosmetic rat scene/model survives stream stop'}
 $checks.Add('Source stream retirement stops owned rat Midnight scene and hides cached model')
''')
s=s.replace("Remote rat visuals, damage/AI, aerial/slide contexts and visual acceptance remain pending.","One cosmetic rat follows a peer agent using a native walk scene; this proves model/playback compatibility only. Full rat pool, source animation phase, speed, scale, smooth interpolation, VFX, damage/AI and visual acceptance remain pending.")
out=P/'Test-SwarmRatVisualNetwork-v291.ps1';assert not out.exists();out.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig')
s=s.replace("'SwarmWorldNetwork',","'SwarmRatVisualNetwork','SwarmWorldNetwork',")
s=s.replace('[switch]$InstallMesmerizeVisual,','[switch]$InstallMesmerizeVisual,\n [switch]$InstallSwarmRatVisual,')
s=s.replace('if($InstallGrenadeFx-or$InstallMesmerizeVisual){','if($InstallGrenadeFx-or$InstallMesmerizeVisual-or$InstallSwarmRatVisual){')
s=s.replace("  if(!$?){throw 'Isolated effect installation failed'}","  if($InstallSwarmRatVisual){& \"$root/outputs/d2-coop-lab/Build-D2SwarmRatVisualAssets.ps1\" -Install}\n  if(!$?){throw 'Isolated effect installation failed'}")
s=s.replace("  'SwarmWorldNetwork'{","  'SwarmRatVisualNetwork'{& \"$root/work/natural-moveset/Test-SwarmRatVisualNetwork-v291.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}\n  'SwarmWorldNetwork'{")
p.write_text(s)
print('Prepared one-rat native visual test and serialized asset installation')
