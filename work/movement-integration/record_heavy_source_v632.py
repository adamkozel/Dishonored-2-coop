from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();restored=rd(W/'heavy-fixture-recovery-v631/result.json');assert restored['passed']
receipt=W/'crouched-heavy-source-v632.json';assert not receipt.exists();tests=[];videos=rd(N/'runtime-video-manifest.json')
for role,version,sub,duration in [('corvo',629,'Scene_2',1000),('emily',630,'Scene_1',833)]:
 take=f'native-heavy-{role}-v{version}';f=N/take;r=rd(f/'result.json');source='host'if role=='corvo'else'client';assert not r['passed']
 scenes=[s for line in(f/'scenes.ndjson').read_text(encoding='utf-8-sig').splitlines() for s in json.loads(line).get('instances',[]) if s.get('asset','').endswith('/sword_attack_focus_sneak_out.midnightscene')and s.get('state')==3]
 assert scenes and all(s['subscene']==sub and s['sceneDurationMs']==duration and len(s['bindingsResearch'])==2 and all(b['reciprocal']for b in s['bindingsResearch'])for s in scenes)
 ids={s['id']for s in scenes};rows=rd(f/f'{source}-crouched-heavy-observations.json');owned=[x for x in rows if any(a['id']==39 and a.get('sequence24Research')in ids for a in x['player']['active'])]
 assert len(owned)>=2 and all(x['player']['stance']==1 and sorted(a['id']for a in x['player']['active'])==[6,39]for x in owned)
 tests.append(dict(role=role,take=take,originalPassed=False,failure=r['failure'],observedNativeRelease=dict(subscene=sub,durationMs=duration,stateSamples=len(owned),sceneIds=sorted(ids)),resultSha256=sha(f/'result.json'),remoteBindingVerified=False))
 video=f/f'{source}-crouched-heavy.mp4';rel=video.relative_to(N).as_posix();assert not any(v['video']==rel for v in videos)
 m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
 videos.append(dict(video=rel,kind='game',character=role,label=f'{role.title()} · crouched heavy native source · {version} · retained test failure',alias='heavy_attack_variants',requirements=['heavy_attack_variants','crouched_attack'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=False,scope='Exact source phase/state observation only; native remote heavy-crouch binding pending. Original failure: '+r['failure']+'. Separate631 restoration passes both full inventories, fresh partners, health56/40 and14 protected files. No visual acceptance.'))
wr(receipt,dict(at=at,tests=tests,restoration='work/movement-integration/heavy-fixture-recovery-v631/result.json',sourceObserved=True,remoteBindingVerified=False,complete=False));wr(N/'runtime-video-manifest.json',videos)
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at
latest='629/630/632 native crouched heavy source: focus In266/Loop2000, separate release CorvoScene_2/1000 and EmilyScene_1/833, owned arm+player with Sneaking6/Slash39. Native release binding absent; existing standing heavy50/51/52 need full-body crouched counterparts. Original629 guard-assertion and630 cleanup failures retained. Separate631 restores both full inventories, fresh partners, health56/40 and14files. Live625/628 crouched strikes and Emily right hit recovery stay verified; kick17 and left hit16 remain unverified.'
s['HeavyCombatContinuation']=dict(at=at,latest=latest,candidate='candidate-kick-v625',receipt=str(receipt.relative_to(R)),complete=False,runtimeVerified=False,restorationVerified=True,next='Implement exact crouched-heavy release variant using role-specific native contracts, add complete-body crouched charge/hold/release from existing heavy50/51/52 and current adaptation pipeline. Verify ordinary input on both roles, recordings/root/recovery; defer cosmetic polish. No more random kick retries or third-party co-op research. Continue toward shared world and playable co-op.')
s['MovementExecutionPriority']['current']=latest;wr(p,s)
# Merge newest actual Ghidra notes into the exact file consumed by generator.
p=R/'work/handoff-project-bookmarks-current.json';old=rd(p);new=rd(R/'work/combat-ghidra-bookmarks-v631.json');by={(x['address'],x['category']):x for x in old}
for x in new:by[(x['address'],x['category'])]=x
wr(p,list(by.values()));wr(R/'work/handoff-project-bookmarks-v632.json',list(by.values()))
p=R/'documentation/handoff-2026-10-11/46-native-crouched-heavy-attack.md';p.write_text(p.read_text()+'\nRecovery631 **PASS**: both fresh partners, full inventories, player/partner health56/40 and all14 protected hashes match. [Source audit632](../../work/movement-integration/crouched-heavy-source-v632.json) correlates the release scene IDs to8 Corvo and7 Emily native Slash/Sneaking samples without changing either original failed receipt. Both source videos are indexed as unbound native observations.\n')
print(json.dumps(dict(recordings=len(videos),ghidraNotes=len(by),restoration=True,remoteHeavyBinding=False)))
