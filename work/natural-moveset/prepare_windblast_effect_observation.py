from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Observe-Windblast.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$Take='native-windblast-corvo-v1'","$Take='native-windblast-effects-v1'")
s=s.replace("$native=Q host '--player-states';$midnight=Q host '--midnight';$entities=Q host '--entities'", "$native=Q host '--player-states';$effects=Q host '--windblast-effects'")
s=s.replace("  $wind=@($entities.entities|Where-Object {$_.definitionName-match'windblast'})\n",'')
s=s.replace("midnight=$midnight;wind=$wind;component=(Q host '--object' $power.component)","effects=$effects")
s=s.replace("if(($midnight|ConvertTo-Json -Depth 12 -Compress)-match'power_windblast_cast')", "if($native.powerSceneValid-and$native.powerScene.variant-eq11)")
s=s.replace("Native Windblast investigation only; no remote body/effect integration claimed.","Native Windblast effect-record observation. No remote effect integration claimed.")
(P/'Observe-WindblastEffects.ps1').write_text(s,encoding='utf-8')
