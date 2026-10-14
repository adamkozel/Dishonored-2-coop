"""Observe native airborne Windblast before broadening the runtime reader."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Observe-Windblast.ps1').read_text(encoding='utf-8-sig')
def change(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a))
 s=s.replace(a,b)
change("'native-windblast-corvo-v1'","'native-windblast-corvo-air-v1'")
change("I host 255 250;","I host 0 220 A;Start-Sleep -Milliseconds 80;$jump=Q host '--player-states';$jump|ConvertTo-Json -Depth 10|Set-Content -Encoding utf8 \"$out/jump-before-cast-$next.json\";I host 255 250;")
change("action='Native LT press 250ms'","action='Native A jump, then LT cast after observing native state'")
change("wind=$wind;component=", "wind=$wind;effects=(Q host '--windblast-effects');component=")
change("$native=Q host '--player-states';$midnight=Q host '--midnight';$entities=Q host '--entities'", "$native=Q host '--player-states';$midnight=Q host '--midnight'")
change("$wind=@($entities.entities|Where-Object {$_.definitionName-match'windblast'})", "$wind=@() # Avoid an expensive whole-world scan during the short airborne cast.")
change(";component=(Q host '--object' $power.component)", "")
change("if(!$sceneCount){throw 'No native Windblast cast scene observed'};", "if(!@($rows|Where-Object {@($_.player.active|Where-Object {$_.id-in@(2,3)}).Count}).Count){throw 'No native Jump/Fall state observed'};$checks.Add('Native Jump/Fall state observed during controller casting');if(!$sceneCount){throw 'No native Windblast cast scene observed'};")
change("source='host';crouched=","source='host';airborneExperiment=$true;crouched=")
(P/'Observe-WindblastAir.ps1').write_text(s,encoding='utf-8-sig')
print('Created native airborne observation harness')
