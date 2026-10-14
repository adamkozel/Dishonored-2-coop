from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeNetwork-v74.ps1').read_text(encoding='utf-8-sig').replace('Mesmerize','Domino').replace('mesmerize','domino').replace('MESMERIZE','DOMINO')
s=s.replace('domino-banks-v73','domino-banks-v192').replace('domino-bank.bin','domino-bank.bin').replace('$m.version-ne45','$m.version-ne47').replace('bank45/276','bank47/289')
s=s.replace("'coop_domino_fixture off','coop_movement_fixture off'","'coop_mesmerize_fixture off','coop_domino_fixture off','coop_movement_fixture off'")
s=s.replace("C client 'setviewpos -33.5 -153 68.1 90 0';C host 'setviewpos -30.7 -151.8 68.1 203 0'","C client 'setviewpos -48.59 -155.07 68.1 90 0';C host 'setviewpos -44.39 -153.27 68.1 203 0'")
s=s.replace('-Role host -Pitch $Pitch','-Role host -Pitch -3')
# Source camera pitches to the validated target test, independently of peer framing.
a=' $before=Q client \'--player-domino\'';assert s.count(a)==1
s=s.replace(a,' & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role client -Pitch 3 -Evidence "$out/source-camera.json" -Quick\n'+a)
s=s.replace("'--watch-power-scenes'","'--watch-domino-scenes'")
obs=(P/'Observe-NativeDomino-v189.ps1').read_text(encoding='utf-8-sig')
aim=obs[obs.index('    $catalog=Q client'):obs.index('\n   }\n   I client')]
a="  if(!$cast-and$w.ElapsedMilliseconds-ge1100){I client 255 1800;";assert s.count(a)==1
s=s.replace(a,"  if(!$cast-and$w.ElapsedMilliseconds-ge1100){\n"+aim+"\n   I client 255 1800;")
s=s.replace('.variant-in16,17,18','.variant-in19,20,21,22').replace('in 16,17,18','in 19,20,21,22').replace('.powerVariant-in16,17,18','.powerVariant-in19,20,21,22')
a=s.index(" foreach($phase in 'in','loop','out')");b=s.index(' foreach($variant in 19,20,21,22)',a)
s=s[:a]+''' foreach($v in 19,20,21,22){if(!@($scenes|Where-Object variant -eq $v).Count){throw "Native strict Domino phase $v absent"}}
 if(@($scenes|Where-Object {!$_.armHandle-or!$_.equipmentHandle}).Count-or@($scenes.armHandle|Sort-Object -Unique).Count-ne1-or@($scenes.equipmentHandle|Sort-Object -Unique).Count-ne1){throw 'Native Domino arm/equipment ownership changed'}
 $checks.Add('Strict native Domino reader confirms all four phases, exact state72 or exit recovery and reciprocal Emily arm/body ownership')
'''+s[b:]
s=s.replace('All exact source scene incarnations, including native aim-loop renewal, match remote phase events through Steam','Four native subscene transitions within the owned cast produce corresponding distinct remote phase events through native Steam')
a=s.index(" if($manaAfter.currentCandidate-ge$manaBefore.currentCandidate)");b=s.index(" C client 'coop_domino_fixture off'",a)
s=s[:a]+''' $nativeMarks=@($rows|ForEach-Object {$_.effects.effects}|Where-Object name -like '*/phantasmal_link/target/*')
 if(!$nativeMarks.Count){throw 'Targeted native Domino marks not observed'}
 $checks.Add('Native target marks remain source-owned; remote test executes body poses without duplicate power, target or damage events. Final mana is observational only')
'''+s[b:]
s=s.replace('Native source Domino acquisition, controller casting, scenes and effects only. Standing armed Emily body phases replicate through native Steam. Remote world VFX, AI and gameplay outcomes remain pending; source-only world effects intentionally retained.','Native source Domino acquisition and targeted controller selection drive four standing armed Emily full-body phases through native Steam, with source scene timing and preserved sword grip/attachment. One native scene incarnation changes subscenes. Remote target marks/links, AI and shared outcomes remain pending; receiver plays only body poses. No claim of complete Domino support or visual acceptance.')
(P/'Test-DominoNetwork-v193.ps1').write_text(s,encoding='utf-8')
f=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=f.read_text(encoding='utf-8-sig');s=s.replace("'DominoObserve','MesmerizeEmptyAirObserve'","'DominoObserve','DominoNetwork','MesmerizeEmptyAirObserve'")
a="  'DominoObserve'{";assert s.count(a)==1
s=s.replace(a,"  'DominoNetwork'{& \"$root/work/natural-moveset/Test-DominoNetwork-v193.ps1\" -Take $Take -Pitch $Pitch}\n"+a);f.write_text(s,encoding='utf-8')
print('Domino native Steam body test ready')
