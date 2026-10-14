from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-DominoCrouchNetwork-v250.ps1').read_text(encoding='utf-8-sig')
obs=(P/'Observe-NativeDominoCrouchSelection-v249.ps1').read_text(encoding='utf-8-sig')
a=obs.index(' $dirty=$true\n # Suspending AI');b=obs.index(" $before=Q client '--player-domino'",a)
placement=obs[a:b]+'''
 $ground=Q client '--player-states';$ci=[Globalization.CultureInfo]::InvariantCulture
 $ox=[double]$ground.position[0]+2.5;$oy=[double]$ground.position[1]-1.8;$oz=[double]$ground.position[2]+1.7
 $oyaw=[Math]::Atan2([double]$ground.position[1]-$oy,[double]$ground.position[0]-$ox)*180/[Math]::PI
 C host ('setviewpos {0} {1} {2} {3} 0'-f$ox.ToString('F4',$ci),$oy.ToString('F4',$ci),$oz.ToString('F4',$ci),$oyaw.ToString('F4',$ci))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 18 -Evidence "$out/observer-camera.json" -Quick
 Start-Sleep -Milliseconds 500;$peerFloor=@();foreach($n in 1..4){$peerFloor+=Q host '--player-states';Start-Sleep -Milliseconds 100}
 $peerFloor|ConvertTo-Json -Depth 9|Set-Content "$out/peer-floor-before-stream.json"
 foreach($f in $peerFloor){if(!$f.motionValid-or$f.motion-ne0-or$f.stance-ne0){throw 'Observer not grounded before stream'};for($k=0;$k-lt3;$k++){if([Math]::Abs([double]$f.position[$k]-[double]$peerFloor[0].position[$k])-gt.005){throw 'Observer ground unstable'}}}
'''
a=s.index(" $dirty=$true;C client 'setviewpos");b=s.index(" $before=Q client '--player-domino'",a);s=s[:a]+placement+s[b:]
s=s.replace("[double]$grounded.position[2]-66.4370575", "[double]$grounded.position[2]-[double]$anchor[2]")
s=s.replace('coop_domino_fixture 20000','coop_domino_fixture 40000').replace("'-t','16'","'-t','18'").replace("'15000'","'17000'").replace('ElapsedMilliseconds-lt14000','ElapsedMilliseconds-lt16000')
s=s.replace('$cast=$false;$stood=$false','$cast=$false;$stood=$false;$secondCast=$false')
anchor='  if(!$stood-and$w.ElapsedMilliseconds-ge6500)'
s=s.replace(anchor,'''  if($cast-and!$secondCast-and$w.ElapsedMilliseconds-ge5500){
   $targets=Q client '--domino-targets';$peerMarks=S host '.domino-world.json'
   [ordered]@{source=$targets;peer=$peerMarks}|ConvertTo-Json -Depth 12|Set-Content "$out/targets-before-reselection.json"
   if(!$targets.valid-or$targets.targets.Count-ne1-or$peerMarks.marks.Count-ne1-or$targets.targets[0].handle-ne$peerMarks.marks[0].target){throw 'One corresponding source/peer selected target required'}
   I client 255 1800;$secondCast=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Second native LT1800 toward already marked target while crouched'})
  }
  if(!$stood-and$w.ElapsedMilliseconds-ge9500)''')
s=s.replace('$_.ms-le5500','$_.ms-le11500').replace('$_.ms-gt8500','$_.ms-gt11500')
anchor=" $checks.Add('Strict native Domino reader"
idx=s.index(anchor)
s=s[:idx]+''' $final=@($scenes|Where-Object subscene -eq 'Cast_final_Select')
 $recovery=@($final|Where-Object {$_.activeStates.Count-eq1-and$_.activeStates[0]-eq6-and$_.timeMs-ge150})
 if(!$secondCast-or$final.Count-lt8-or$recovery.Count-lt4-or($recovery.timeMs|Measure-Object -Maximum).Maximum-lt300-or@($final|Where-Object {$_.stance-ne1-or$_.durationMs-ne400-or$_.timeMs-gt400}).Count){throw 'Owned crouched final400 recovery not verified'}
 if(!@($final|Where-Object {$_.activeStates-contains72}).Count){throw 'Native casting state absent during final entry'}
 $final|ConvertTo-Json -Depth 10|Set-Content "$out/final-select-native-proof.json"
 $lateFinal=@($rows|Where-Object {$_.ms-ge7000-and$_.ms-le8500-and$_.movement.powerVariant-eq21-and$_.movement.powerWeight-ge.25})
 if(!$lateFinal.Count){throw 'Crouched final selection gesture absent on peer'}
 $lateFinal|ConvertTo-Json -Depth 16|Set-Content "$out/final-select-peer-proof.json"
 $afterTargets=Q client '--domino-targets';$afterPeer=S host '.domino-world.json'
 [ordered]@{source=$afterTargets;peer=$afterPeer}|ConvertTo-Json -Depth 12|Set-Content "$out/targets-after-reselection.json"
 if(!$afterTargets.valid-or$afterTargets.targets.Count-or$afterPeer.marks.Count){throw 'Native and peer selected mark did not retire after reselection'}
 $checks.Add('Native crouched final400 preserves owned scene through Sneaking6 recovery past300ms and reaches peer; native and cosmetic selected mark retire together')
''' +s[idx:]
s=s.replace('observedPhases=$phases;', 'finalSamples=$final.Count;finalRecoverySamples=$recovery.Count;finalPeerSamples=$lateFinal.Count;observedPhases=$phases;')
s=s.replace('This take permits native target selection and records its effects.', 'This take verifies one native target selection and reselection, exact final400 crouched recovery, corresponding peer gesture and source/cosmetic mark retirement.')
(P/'Test-DominoCrouchFinalNetwork-v254.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace("'DominoObserve','DominoCrouchFinalObserve'","'DominoObserve','DominoCrouchFinalNetwork','DominoCrouchFinalObserve'")
s=s.replace("  'DominoCrouchFinalObserve'{", "  'DominoCrouchFinalNetwork'{& \"$root/work/natural-moveset/Test-DominoCrouchFinalNetwork-v252.ps1\" -Take $Take -Pitch $Pitch}\n  'DominoCrouchFinalObserve'{");p.write_text(s,encoding='utf-8')
s=(W/'Archive-DominoCrouchSelect-v250.ps1').read_text(encoding='utf-8-sig').replace('candidate-domino-crouch-select-v250','candidate-domino-crouch-final-v252').replace('domino-crouch-select-','domino-crouch-final-').replace('-v250.log','-v252.log').replace('prepare_domino_crouch_select_v250.py','prepare_domino_crouch_final_v252.py').replace('Test-DominoCrouchNetwork-v250.ps1','Test-DominoCrouchFinalNetwork-v252.ps1')
a=s.index("scope='");b=s.index("'}|ConvertTo-Json",a)
s=s[:a]+"scope='Native v251 source PASS7 proves crouched final_Select400 and Sneaking6 recovery with14 reciprocal raw samples through383ms. Candidate252 allows exact final scene with matched crouched state, source clock bounded400ms and reciprocal body/arm/equipment, retaining standing and ordinary-selection safeguards. Bank48/301 payloads unchanged; format and actual-bank checks retained from246. Reader fixtures, build, wire and load pass. Paired final-crouch/runtime visual verification pending."+s[b:]
(W/'Archive-DominoCrouchFinal-v252.ps1').write_text(s,encoding='utf-8')
print('Prepared paired crouch final recovery v252, stable source/observer and mandatory single-target identity.')
