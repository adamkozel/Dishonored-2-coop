from pathlib import Path
P=Path(__file__).resolve().parent;s=(P/'Test-BendTimeViewerFx.ps1').read_text();target=P/'Test-FailureHandFx.ps1';assert not target.exists()
s=s.replace("[ValidateSet('bendtime')][string]$Family='bendtime'","[ValidateSet('outofmana')][string]$Family='outofmana'")
s=s.replace("'bendtime-viewer-fx-v41-v1'","'outofmana-hand-fx-v47'").replace("'bendtime_viewer_probe'","'outofmana_hand_probe'").replace("@('10','10')","@('11','11')")
s=s.replace("C $Role 'setviewpos -29 -151 68.1 190 0';Start-Sleep -Milliseconds 250;C $source 'setviewpos -33.5 -153 68.1 0 0'", "C $Role 'setviewpos -33.5 -150 68.1 270 0';Start-Sleep -Milliseconds 250;C $source 'setviewpos -33.5 -153 68.1 90 0'")
s=s.replace('$stage=0;$seen=@{}','$stage=0;$seen=@{};$walkSent=$false')
s=s.replace("if($stage-eq1-and$ms-ge2900)","if($stage-eq1-and!$walkSent-and$ms-ge950){& \"$lab/Send-D2Input.ps1\" -Role $source -Button Neutral -LeftY 12000 -DurationMs 600|Out-Null;$walkSent=$true}\n  if($stage-eq1-and$ms-ge2900)")
s=s.replace('Native Bend Time effects/lifecycle mismatch','Native failure effects/lifecycle mismatch')
s=s.replace('screen declaration create effects','hand declaration creates finite effects')
s=s.replace("$checks.Add('Finite effects retire and explicit cancellation stops only tracked native records')", """$checks.Add('Finite effects retire and explicit cancellation stops only tracked native records')
 $points=@($rows|Where-Object {$_.stage-eq1}|ForEach-Object {$_.fx.effects}|Where-Object kind -eq 11)
 if($points.Count-lt3-or$fx.handUpdates-le$before.handUpdates){throw 'No confirmed moving native hand effect observations'}
 $firstId=$points[0].id;$same=@($points|Where-Object id -eq $firstId);$maxDistance=0.
 foreach($p in $same){$d=0.;for($i=0;$i-lt3;$i++){$d+=[Math]::Pow($p.handPoint[$i]-$same[0].handPoint[$i],2)};$maxDistance=[Math]::Max($maxDistance,[Math]::Sqrt($d))}
 if($maxDistance-lt.05){throw 'Effect did not follow confirmed moving hand'}
 $checks.Add('Same owned native FX record follows the moving left hand with finite world coordinates')""")
s=s.replace('Original executable, Steam API and protected save hashes unchanged','All 14 original settings/save and isolated checkpoint hashes unchanged')
s=s.replace('Isolated manual native Bend Time viewer particle probe. Original screen-particle FX, 1600ms lease or explicit cancellation, owned clone record. No network FX replication, render-factor override, power activation or world time changes. Visual feasibility only; not complete native Bend Time appearance.', 'Isolated manual native out-of-mana hand particle probe on the role-specific remote avatar. Exact native FX/particles, owned left wrist transform, finite 1600ms lease and cancellation. No automatic network trigger, first-person callback, power activation, resource write or damage. Wrist-centred adaptation of the native tattoo socket; placement/scale and camera effects require actual visual review.')
s=s.replace("C $r 'coop_power_fx off';C $r 'coop_movement_fixture off'", "& \"$lab/Send-D2Input.ps1\" -Role $r -Button Neutral -DurationMs 100|Out-Null;C $r 'coop_power_fx off';C $r 'coop_movement_fixture off'")
target.write_text(s);print(target)
