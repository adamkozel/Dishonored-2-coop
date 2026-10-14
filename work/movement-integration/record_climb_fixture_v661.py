from pathlib import Path
import json, hashlib, datetime, subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
at=datetime.datetime.now().astimezone().isoformat();take='native-climb-corvo-v661';f=N/take;r=rd(f/'result.json');assert r['passed']
obs=rd(f/'observations.json');cp=rd(f/'chain-components.json')['components'];assert len(obs)==136 and not any(x['id']==29 for o in obs for x in o['player']['active'])
assert {x['type'] for x in cp}=={'.?AVarkCpntAudio@@','.?AVarkCpntAnimBase@@'}
audit=dict(at=at,executionAndRestorationPassed=True,climbingActivated=False,remoteBindingVerified=False,complete=False,samples=len(obs),components=[x['type'] for x in cp],blocker='Original spawned arkClimbable lacks required physics-system and climbable components. Cause unproven. No ordinary-input climb activation; obtain effective native campaign fixture before binding.',receipt=take+'/result.json')
wr(W/'native-climb-discovery-v661.json',audit)
review=dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=['source-sheet.jpg'],observations=['No visible usable chain or native climb prompt; normal ground movement and crouch only. Some source framing obstructed by co-op avatar. No remote climbing claimed.'])
wr(f/'source-review-v661.json',review)
videos=rd(N/'runtime-video-manifest.json');video=f/'source.mp4';rel=video.relative_to(N).as_posix()
if not any(x['video']==rel for x in videos):
    stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
    with video.open('rb')as h:videoSha=hashlib.file_digest(h,'sha256').hexdigest().upper()
    videos.append(dict(video=rel,kind='game',character='corvo',label='Corvo · native chain fixture dependency blocker · source',alias='chain_rope_climb',requirements=['chain_rope_climb'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/native-climb-discovery-v661.json',visual_review=take+'/source-review-v661.json',proxy_sha256=r['dll'],video_sha256=videoSha,actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=False,scope=audit['blocker']+' Execution and checkpoint restoration passed; climbing did not activate.'))
    wr(N/'runtime-video-manifest.json',videos)
note='659–661: original native chain definition resolves in both roles, but its isolated creation yields only Audio and AnimBase; PhysicsSystem/Climbable components absent.136 source samples never enter Climb29. Actual source video inspected; no climbing acceptance. Checkpoint fully removes fixture and restores both inventories, health and14 protected hashes. Dependency is documented; do not repeat this incomplete fixture.'
a=rd(N/'review-evidence-annotations.json');i=a['items']['chain_rope_climb'];i.update(complete=False,runtime_binding='blocked_native_fixture_missing_components',notes=note);i['in_game_recordings']=[rel];i['evidence']=[take+'/result.json'];wr(N/'review-evidence-annotations.json',a)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['ClimbSwimInvestigation'].update(at=at,readOnly=False,runtimeActivated=False,fixtureCreated=True,complete=False,candidate='candidate-climb-fixture-v660',latest=note,details='documentation/handoff-2026-10-11/51-native-chain-fixture-blocker.md',receipt='work/movement-integration/native-climb-discovery-v661.json',next='Continue batched native pickup/food/readable observation in existing tower/current animation pipeline. Climbing needs an effective native campaign fixture before binding; swimming needs a real water fixture. Fine acting deferred.')
s['MovementExecutionPriority']['current']=note;wr(R/'work/current-task-state.json',s)
p=R/'work/handoff-project-bookmarks-current.json';merged={(x['address'],x['category']):x for x in rd(p)};merged.update({(x['address'],x['category']):x for x in rd(R/'work/handoff-project-bookmarks-v661.json')});wr(p,sorted(merged.values(),key=lambda x:(x['address'],x['category'])))
print(json.dumps(dict(videos=len(videos),bookmarks=len(merged),climbingActivated=False,restorationPassed=True)))
