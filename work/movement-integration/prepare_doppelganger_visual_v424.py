from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab';A=W/'candidate-doppelganger-visual-v423'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(N/'doppelganger-rig-v422/result.json')['passed'];assert rd(N/'doppelganger-rig-v422/native-clone-skeleton.json')['stable'];assert not A.exists()
logs=[W/f'doppelganger-visual-{k}-v423.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
 t=p.read_text(encoding='utf-8-sig');assert t and not any(s in t for s in ('error C','error LNK','FAIL:','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(W/'candidate-doppelganger-stream-v418/banks',A/'banks');(A/'assets').mkdir()
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
assets=['doppelganger_visual.def','doppelganger_visual.animtree','doppelganger_visual.cpntanimevolved']
for name in assets:shutil.copy2(L/'assets'/name,A/'assets'/name)
for p in [Path(__file__),*logs,W/'Build-DoppelgangerVisual-v423.ps1',W/'implement_doppelganger_pose_v423.py',W/'implement_doppelganger_pose_probe_v423.py',W/'Install-DoppelgangerVisual-v423.ps1',L/'Build-D2DoppelgangerVisualAssets.ps1',N/'doppelganger-rig-v422/native-clone-skeleton.json']:shutil.copy2(p,A/p.name)
x=rd(W/'candidate-doppelganger-stream-v418/identity.json')
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,doppelgangerWorldProtocol=2,doppelgangerWorldBytes=10696,doppelgangerJoints=219,doppelgangerRig='5921d3450a60d7e2',banks=[dict(path=str(p),sha256=sha(p)) for p in (A/'banks').glob('*.bin')],scope='Candidate423: source full219-joint native clone pose over Steam; separate four-component cosmetic Emily model,120ms interpolated root/local pose and phase3 application; no peer AI, health, physics, damage, animation events or power VFX. Runtime not yet verified.',previousVerified=str(W/'candidate-doppelganger-stream-v418'));x.pop('runtimeVerificationScope',None)
x['assetHashes'].update({name:sha(A/'assets'/name) for name in assets});x['doppelgangerAssetsReference']='assets'
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/name,L/'bin'/name)
s=(N/'Test-DoppelgangerStream-v419.ps1').read_text(encoding='utf-8-sig')
s=s.replace("clonePeer=(S host '.doppelganger-world.json')}","clonePeer=(S host '.doppelganger-world.json');cloneVisual=(S host '.doppelganger-visual.json')}")
anchor='    $cloneCaptured=$true';assert s.count(anchor)==1
s=s.replace(anchor,'''    $visual=S host '.doppelganger-visual.json';$visual|ConvertTo-Json -Depth 8|Set-Content "$out/visual-before-rig.json"
    if(!$visual.visible-or!$visual.handle){throw "Clone model not visible: $($visual.result)"}
    $components=Q host '--components' $visual.handle;$components|ConvertTo-Json -Depth 12|Set-Content "$out/visual-components.json"
    if($components.components.Count-ne4-or@($components.components|Where-Object {$_.type-match'(?i)(health|physics|decoy|aiDecision)'}).Count){throw 'Cosmetic clone has unexpected gameplay components'}
    $pose=$null
    for($attempt=0;$attempt-lt30;$attempt++){
     $raw=& "$lab/bin/d2_runtime_probe.exe" $pids.host '--doppelganger-skeleton-final' $visual.handle $visual.publication
     if(!$LASTEXITCODE){$pose=$raw|ConvertFrom-Json;if($pose.stable-and$pose.finalPublicationComplete-and$pose.cloneExpectedSequence-and$pose.cloneExpectedPoseError-lt.0001){break}}
     Start-Sleep -Milliseconds 10
    }
    $pose|ConvertTo-Json -Depth 16|Set-Content "$out/visual-final-skeleton.json"
    if(!$pose.stable-or!$pose.finalPublicationComplete-or$pose.joints.Count-ne219-or$pose.cloneExpectedPoseError-ge.0001){throw 'Complete219-joint native final pose not independently confirmed'}
    $checks.Add('Separate four-component Emily visual and all219 final native joints match the received/interpolated pose')
'''+anchor)
anchor=" $checks.Add('Native clone identity/root/lifetime traverses Steam with exact accepted packet digests and authoritative receiver retirement')";assert s.count(anchor)==1
s=s.replace(anchor,anchor+'''
 $visuals=@($rows.cloneVisual|Where-Object {$_.updated-gt0});$lastVisual=S host '.doppelganger-visual.json'
 $lastVisual|ConvertTo-Json -Depth 8|Set-Content "$out/visual-final.json"
 if(!$visuals.Count-or$lastVisual.created-ne1-or$lastVisual.shown-ne1-or$lastVisual.hidden-ne1-or$lastVisual.visible-or$lastVisual.applied-lt20-or$lastVisual.interpolated-lt20-or$lastVisual.poseRefused-or$lastVisual.maxTransformError-gt.001){throw 'Clone visible pose, interpolation, exact transform or retirement check failed'}
 $checks.Add('One isolated cosmetic clone follows native219-joint pose and interpolated root, then hides on source retirement')
''')
s=s.replace('Receiver creates no model, effect, AI or damage','Receiver creates one cosmetic model; no effect, AI or damage').replace('remote model, VFX/gameplay, alternate contexts','remote VFX/gameplay, alternate contexts')
(N/'Test-DoppelgangerVisual-v424.ps1').write_text(s,encoding='utf-8')
t=(W/'Run-DoppelgangerStream-v419.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-stream-v419','doppelganger-visual-v424').replace('Test-DoppelgangerStream-v419','Test-DoppelgangerVisual-v424')
(W/'Run-DoppelgangerVisual-v424.ps1').write_text(t,encoding='utf-8')
print(json.dumps(dict(dll=x['dll'],probe=x['probe'],candidate=str(A))))
