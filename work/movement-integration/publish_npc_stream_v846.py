"""Publish measured NPC stream scope; retain failed takes without rewriting them."""
from pathlib import Path
import datetime, hashlib, json, math, shutil, subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
A=W/'candidate-npc-stream-v842';T=N/'npc-stream-v846'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
for name in ('result.json','restoration.json'):assert rd(T/name)['passed']
assert rd(N/'npc-camera-recovery-v844/result.json')['passed']
assert not rd(N/'npc-stream-v843b/result.json')['passed']
assert not rd(N/'npc-stream-v843b/restoration.json')['passed']
assert rd(N/'npc-stream-v845/result.json')['passed']
assert all(e['applied']==0 for e in rd(N/'npc-stream-v845/coverage.json')['bodyCallbacks'])
metrics={}
for role in ('host','client'):
    stream=rd(T/f'stream-{role}.json');final=rd(T/f'final-stream-{role}.json')
    assert stream['enabled'] and len(stream['entries'])==5
    assert not stream['decodeRejected'] and not stream['poseScopeRejected'] and not stream['bodyRefused']
    for e in stream['entries']:
        assert e['ready'] and e['enabled'] and e['poses']>=150 and e['rejected']==0
        assert e['references']==(2 if role=='client' else 0)
        if role=='client':assert e['roots']>=1000
    assert sorted(e['joints'] for e in stream['entries'])==[223,223,223,223,224]
    assert not final['enabled'] and all(not e['references'] and not e['lost'] for e in final['entries'])
    if role=='client':assert sum(e['released'] for e in final['entries'])==10
    before=rd(T/f'before-{role}.json');after=rd(T/f'after-{role}.json')
    assert before['localEpoch']==after['localEpoch'] and after['localGameLogicState']==1 and not after['localPendingGameLogicState']
    inv=lambda stage:sorted((i['name'],i['quantity']) for i in rd(T/f'inventory-{stage}-{role}.json')['items'])
    assert inv('before')==inv('after')
    assert rd(T/f'health-before-{role}.json')['current']==rd(T/f'health-after-{role}.json')['current']==56
    assert len(rd(T/f'catalog-after-{role}.json')['entries'])==5
    metrics[role]={'streams':stream['entries'],'bodyCallbacks':stream['bodyEntries'],'transport':stream['transport'],'released':sum(e['released'] for e in final['entries'])}
coverage=rd(T/'coverage.json');assert sum(e['applied']>30 for e in coverage['bodyCallbacks'])==2
assert not coverage['allActorBodyCallbacks']
client=rd(T/'stream-client.json');counter=rd(T/'counters-before-client.json')
assert client['bodyApplied']>counter['bodyApplied'] and client['framesOwned']>counter['framesOwned']
start=rd(T/'view-target.json')['root'];end=rd(T/'host-ld_spawner_guard_tutorial_2-skeleton.json')['root']
distance=math.dist(start[:3],end[:3]);assert distance>2
scope='Five authored Dunwall NPC rigs negotiated and streamed over native Steam, host AI retained, client roots owned and ten AI references released cleanly. Two visible 223-joint guards received fresh body callbacks. Paired footage shows one guard idle-to-patrol movement; three other NPCs including224-joint Ramsey have packet/root coverage only. No general damage/death, equipment, spawning, campaign or online-account certification.'
audit=dict(at=at,passed=True,complete=False,candidate=A.name,dll=pin['dll'],probe=pin['probe'],scope=scope,metrics=metrics,patrolDisplacementMeters=distance,evidence='work/natural-moveset/npc-stream-v846/result.json',visualEvidence='work/natural-moveset/npc-stream-v846/contact.png',visualJudgment='Paired sampled frames show matching guard posture and patrol progression. No conspicuous body deformation in these views. Full smoothness, foot contacts and weapons remain unevaluated; this is not a complete combat demonstration.',failedAndInsufficientTakes=[{'take':'npc-stream-v841','reason':'Uncompressed pose traffic exceeded native transport budget; pose timeout.'},{'take':'npc-stream-v843','reason':'Compact streams sustained but actors culled; no current body callbacks.'},{'take':'npc-stream-v843b','reason':'Harness gave root height and reversed angles to native eye-position command; host fell and died, stream retired correctly. Raw restoration failed; separate844 recovery passed.'},{'take':'npc-stream-v845','reason':'Raw PASS incorrectly accepted lifetime body counters; all current-arm body counters zero. Rejected as body proof;846 fixes assertion and follows current target.'}])
wr(W/'npc-stream-audit-v846.json',audit)
pin.update(runtimeVerified=True,runtimeAudit='../npc-stream-audit-v846.json',runtimeScope=scope);wr(A/'identity.json',pin)
shutil.copy2(N/'Test-NpcStream-v843.ps1',A/'Test-NpcStream-v843.ps1')
shutil.copy2(N/'Recover-NpcCamera-v844.ps1',A/'Recover-NpcCamera-v844.ps1')
latest='839–846: generic host-authored NPC root/body protocol2 streams five223/224-joint rigs over native Steam. Compact14byte joints and512KiB/s budget replace oversized840 packets. Test846 delivers all five roots; two visible guards receive1423/1343 current-arm body applications; paired frames show one idle-to-patrol transition. Ten client AI references released, health/inventory retained. Failed/insufficient841/843/843b/845 retained and explained.'
next_work='Extend generic NPC authority through menu/checkpoint and actor spawn/despawn/rearm, then integrate repeated hits/health/deaths, equipment and AI perception of both players. Validate the224-joint and remaining visible rigs, campaign/progression, remaining movement/VFX gaps and native session UI/separate-account Steam connections. Group item families; do not implement consumables individually.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['NpcAuthorityContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/npc-stream-audit-v846.json',details='documentation/handoff-2026-10-11/71-generic-npc-steam-root-and-body-stream.md',runtimeVerified=True,networkAuthorityVerified=False,presentationStreamVerified=True,complete=False,latest=latest,next=next_work,scope=scope)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
bms=rd(R/'work/handoff-npc-stream-bookmark-v846.json')['bookmarks'];keys={(b['address'],b['category']) for b in bms}
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;wr(p,sorted([b for b in rd(p) if (b['address'],b['category']) not in keys]+bms,key=lambda b:(b['address'],b['category'])))
videos=rd(N/'runtime-video-manifest.json')
for role in ('host','client'):
    path=T/f'{role}.mp4';info=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate','-of','json',str(path)]))['streams'][0]
    rate=int(info['nb_frames'])/float(info['duration']);rel=path.relative_to(N).as_posix();videos=[v for v in videos if v['video']!=rel]
    videos.append(dict(video=rel,character='corvo' if role=='host' else 'emily',role=role,label='Shared NPC patrol · '+('Host native AI' if role=='host' else 'Client Steam replica'),travelling=True,gameplay_bound=True,actual_capture_hz=rate,container_fps=info['avg_frame_rate'],timing='Native wall-time capture, no speedup',scope=scope,review='Paired contact sheet inspected: one guard idle-to-patrol; two223-joint body callbacks; no all-rig or combat claim.'))
wr(N/'runtime-video-manifest.json',videos)
p=D/'README.md';s=p.read_text();line='- [Generic NPC Steam root and body stream](71-generic-npc-steam-root-and-body-stream.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,candidate=pin['dll'],actors=5,visibleRigCallbacks=2,patrolDisplacementMeters=distance,videos=len(videos))))
