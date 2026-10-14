from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset';W=R/'work/movement-integration'
p=S/'runtime_probe.cpp';s=p.read_text();a='(argc==4&&std::strcmp(argv[2],"--watch-possession-scenes")';assert s.count(a)==1;s=s.replace(a,'(argc==4&&std::strcmp(argv[2],"--watch-possession-state")&&std::strcmp(argv[2],"--watch-possession-scenes")');p.write_text(s)
s=(P/'Observe-NativePossessionTarget-v325.ps1').read_text()
s=s.replace(" $dirty=$true\n",''' $preflight=Q host '--watch-possession-state' 100
 $preflight|ConvertTo-Json -Depth 8|Set-Content "$out/probe-preflight.json"
 if(!$preflight.Count){throw 'Read-only target watcher preflight emitted no records'}
 $dirty=$true
''')
needle="if(!$cast-and$w.ElapsedMilliseconds-ge1100){I host 255 1800;"
assert needle in s
s=s.replace(needle,"""if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   $fresh=@((Q host '--world-npcs').npcs|Where-Object {$_.handle-eq$target.handle-and$_.spawnerName-eq$target.spawnerName-and$_.reciprocal-and$_.unique})
   if($fresh.Count-ne1){throw 'Original guard identity changed before cast'}
   $yaw=[Math]::Atan2([double]$fresh[0].position[1]+155.07,[double]$fresh[0].position[0]+48.59)*180/[Math]::PI
   C host ('setviewpos -48.59 -155.07 68.1 {0} 0' -f $yaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
   $fresh[0]|ConvertTo-Json -Depth 8|Set-Content "$out/aim-guard-at-cast.json"
   I host 255 500;""")
s=s.replace('Native Corvo LT1800 then release at verified human target','Native Corvo LT500 then release at freshly revalidated human target')
s=s.replace("if($cast-and!$exitSent-and$w.ElapsedMilliseconds-ge7600){I host 255 120;", "if($cast-and!$exitSent-and$w.ElapsedMilliseconds-ge7600){$exitState=Q host '--player-states';if($exitState.stance-ne18-or$exitState.defaultMode){throw 'No occupied-host mode before leave-host input'};I host 255 120;")
s=s.replace('$_.internalState-eq14-and','$_.internalState-ne14-and')
s=s.replace('internalState14Samples','occupiedInternalStateSamples')
s=s.replace('Internal state14, physics stance18 and active-state IDs are separate fields;','Internal state14 is the inactive sentinel per824e40, not a possessed state. Physics stance18 and active-state IDs are separate fields;')
s=s.replace('under internal state14/physics stance18','under active internal state/physics stance18').replace('native internal state14','native internal state lifecycle')
(P/'Observe-NativePossessionTarget-v326.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Observe-NativePossessionTarget-v325.ps1','Observe-NativePossessionTarget-v326.ps1');p.write_text(s)
print('Registered probe option, added preflight and fresh aim, corrected inactive sentinel14 interpretation')
