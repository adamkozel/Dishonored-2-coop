from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-exit-v343'
s=(P/'Test-PossessionVisibilityNetwork-v342.ps1').read_text()
s=s.replace('$returnFramed=$false', '$returnFramed=$false;$moveSent=$false;$returnBeforeMove=$null',1)
anchor='  $rows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;videoTime='
assert s.count(anchor)==1
s=s.replace(anchor,'''  if($returnFramed-and!$moveSent-and$w.ElapsedMilliseconds-ge7200){
   $returnBeforeMove=Q host '--player-states'
   if(!(S client '.shadow-stream.json').ready-or(S client '.possession-body.json').hidden){throw 'Possession exit did not retain live visible peer stream'}
   & "$lab/Send-D2Input.ps1" -Role host -Button Neutral -LeftY -18000 -DurationMs 500|Out-Null
   $moveSent=$true;$events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native half-second backward walk after host exit; no stream rearm'})
  }
'''+anchor)
anchor=" if(!$returnFramed){throw 'Native exit return not reframed for visual review'}"
assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $recovered=@($rows|Where-Object {$_.ms-ge6300})
 $dual=@($rows|Where-Object {$_.player.equipmentSceneValid-and($_.player.active.id-contains64)})
 $equipped=@($rows|Where-Object {$_.ms-gt5200-and$_.movement.equipmentVariant-eq1-and$_.movement.equipmentWeight-gt.3})
 $walk=@($rows|Where-Object {$_.ms-ge7200-and$_.movement.weight-gt.9-and$_.movement.speed-gt.15})
 $finalMove=Q host '--player-states';$distance=if($returnBeforeMove){[Math]::Sqrt([Math]::Pow($finalMove.position[0]-$returnBeforeMove.position[0],2)+[Math]::Pow($finalMove.position[1]-$returnBeforeMove.position[1],2))}else{0}
 [ordered]@{recoveredRows=$recovered.Count;dualOwnedSamples=$dual.Count;remoteDrawSamples=$equipped.Count;walkSamples=$walk.Count;distance=$distance;before=$returnBeforeMove;after=$finalMove;sourceStream=(S host '.shadow-stream.json');peerStream=(S client '.shadow-stream.json')}|ConvertTo-Json -Depth 12|Set-Content "$out/exit-continuity.json"
 if($recovered.Count-lt3-or@($recovered|Where-Object {!$_.stream.ready-or!$_.sourceStream.ready-or$_.visibility.hidden}).Count){throw 'Native exit retired stream or retained hidden body'}
 if(!$dual.Count-or!$equipped.Count-or!$moveSent-or!$walk.Count-or$distance-lt.15){throw 'Owned dual draw or subsequent native movement was not demonstrated'}
 $checks.Add('Native paired sword1066/power333 equip scenes are owned, remote draw plays, both Steam streams stay ready through exit and native backward walk resumes without rearm')
''')
s=s.replace('interruption/alternate contexts pending.', 'Corvo standing dual-equip exit and normal backward walk tested without rearm; interruption/alternate contexts pending.')
(P/'Test-PossessionExitNetwork-v343.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionVisibilityNetwork-v342.ps1','Test-PossessionExitNetwork-v343.ps1');p.write_text(s,encoding='utf-8')
assert not A.exists()
logs=['possession-exit-build-v343.log','possession-exit-world-tests-v343.log','possession-exit-action-tests-v343.log','possession-exit-smoke-v343.log']
for name in logs:
 log=(W/name).read_text(encoding='utf-8-sig');assert log and not any(t in log for t in ('error C','error LNK','FAIL'))
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copytree(P/'possession-banks-v334',A/'banks')
for p in [Path(__file__),W/'Build-PossessionExit-v343.ps1',W/'Invoke-NativeLabOperation.ps1',P/'Test-PossessionExitNetwork-v343.ps1',P/'possession-bank-manifest-v334.json',*[W/x for x in logs]]:shutil.copy2(p,A/p.name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
x=json.loads((W/'candidate-possession-release-body-v338/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Exact Corvo standing native paired sword_equip1066 plus power_equip333 owned-scene reader; allows semantic stream continuity after Possession exit. Every original arm/scene/sword/state lifecycle guard retained. Protocol50 and bank52/316 unchanged. Runtime/visual validation pending.')
x['banks']=[dict(path=str(A/'banks'/p.name),sha256=sha(p)) for p in sorted((A/'banks').glob('*.bin'))];x['banksReference']='banks'
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
(A/'tests.json').write_text(json.dumps(dict(passed=True,logs=logs,scope='Fresh explicit-path build; native memory ownership, protocol, proxy suites pass. Earlier shell attempt failed to find cmake and ran old tests; not validation of343.'),indent=2),encoding='utf-8')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k]for k in ('dll','probe','protocol','bankVersion')}))
