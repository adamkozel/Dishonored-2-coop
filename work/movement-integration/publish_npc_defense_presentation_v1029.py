from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
A=W/'candidate-npc-combat-v1026';B=W/'candidate-npc-combat-v1022';C=W/'candidate-npc-combat-v1018'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
pin=rd(A/'identity.json');mid=rd(B/'identity.json');old=rd(C/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
for b in pin['banks']:assert sha(R/b['path'])==b['sha256']
names={pin['dll']:A.name,mid['dll']:B.name,old['dll']:C.name}
SCOPES={
 'npc-parry-presentation-v1023':'Candidate1022: receiver timed parry (variant21) presented on the host remote body as bank68 clip358 (8 samples, weight>0.5); no block occurred so clip357 unexercised.',
 'npc-parry-presentation-v1025':'Candidate1022: receiver block impact (sword_parry_hit_2, variant20) and three timed parries (21) presented on the host remote body as clips357/358 (9 and 25 samples); every source event received in order with zero rejections.',
 'observer-camera-v1027c':'Candidate1026 lab observer camera: native render view held at the observer pose while the host body returned to its parked position; overrides grew; released by off.',
 'npc-parry-observed-v1028c':'Candidate1026: defense presentation with the host observer camera watching the remote player: 2 blocks + 1 timed parry relayed natively, host remote body presented clips357 (12 samples) and 358 (7), relay/ack/observer counts consistent, restoration passed.'}
plan=[('npc-parry-scenes-v1021',old,True,'Source observation on 1018 (--watch-midnight): relayed result1 parries start sword_parry_win Scene_1/500 within 0-31 ms; result0 block starts sword_parry_hit_1 Scene_1/533 after 16 ms; both bind player camera, owned right arm and the attacking NPC. The run also passed the full timed-parry relay acceptance.'),
      ('npc-parry-presentation-v1023',mid,True,None),
      ('npc-parry-presentation-v1023b',mid,False,'One relayed block (relay consistent) whose impact scene was not presented on the host (host events 17-19 skipped); cause not determined. Restoration passed.'),
      ('npc-combat-scene-v1024',mid,False,'Diagnostic --watch-combat-scene run: the shared reader accepts sword_parry_hit_1 as variant19 (3 times) and sword_parry_win as 21; stage-6 misses are the normal handle-0 gaps. Cleanup failed (input packet expired; both instances left on the native Press-any-key screen). Recovered separately: npc-combat-scene-v1024/recovery/result.json (gameplay, health56, partners, protected14).'),
      ('npc-parry-presentation-v1025',mid,True,None),
      ('observer-camera-v1027',pin,False,'Harness failure: Send-D2Command.ps1 allowlist lacked coop_observer_camera (hold and cleanup off refused). Host returned to its parked position; protected files unchanged.'),
      ('observer-camera-v1027b',pin,False,'Validator defect: parked check read the (held) native view position instead of the body position; all functional checks passed and the body was parked.'),
      ('observer-camera-v1027c',pin,True,None),
      ('npc-parry-observed-v1028',pin,False,'Setup failure before any relayed contact: native positive control did not detect the host within 4 s. Restoration passed.'),
      ('npc-parry-observed-v1028b',pin,False,'Owned clone 40 -> 37 about 3.8 s into the defense loop; damage records were not saved by that harness version, so the source is unattributed. Restoration passed.'),
      ('npc-parry-observed-v1028c',pin,True,None)]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,lp,passed,note in plan:
    T=N/take;r=rd(T/'result.json') if (T/'result.json').exists() else {}
    if passed and not note:
        assert r.get('passed') and rd(T/'restoration.json')['passed'],take
        summary=dict(result=r.get('scope'))
        if (T/'presentation-results.json').exists():summary.update(presentation=rd(T/'presentation-results.json'),defense=rd(T/'defense-results.json')['blocked'][0])
        if (T/'checks.json').exists():summary.update(checks=rd(T/'checks.json'))
        scope=SCOPES[take]
    else:
        scope=('Passed observation: ' if passed else 'Failed run: ')+note;summary=dict(failure=r.get('failure'))
    cases.append(dict(take=take,candidate=lp['dll'],passed=passed,scope=scope,result=summary))
    for role in ('host','client'):
        for path in (f'{take}/{role}.mp4',f'{take}/{role}-held.mp4'):
            if not (N/path).exists() or (N/path).stat().st_size==0:continue
            meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(N/path)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
            review='Raw native diagnostic, visual review not yet entered.'
            if (T/'visual-review.json').exists():
                vr=rd(T/'visual-review.json').get(role)
                if vr:review=vr['observed']+' Limits: '+vr['limits']
            videos=[x for x in videos if x['video']!=path]
            videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'Remote defense presentation / {take} / {role}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review=review,candidate=names.get(lp['dll']),dll=lp['dll'],runtime_test_passed=passed))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'npc-defense-presentation-audit-v1029.json',dict(at=at,candidates=[B.name,A.name],dll=pin['dll'],bankManifest='work/natural-moveset/parry-bank-manifest-v1022.json',protocol=70,cases=cases,complete=False,
   notes=['Send-D2Command.ps1 allowlist gained coop_observer_camera after candidate1026 was staged; the archived copy predates that one-word change.'],
   outstanding=['Donor deflection clips357/358 are functional stand-ins: acting, block-idle transition and sword/hand contact unreviewed; final visual acceptance belongs to the animation review.','1023b missed impact presentation and 1028b unattributed clone damage (40->37) remain unexplained.','Crouched/sneak block impacts, parry-win (0% native chance) and heavy attacks remain unexercised.']))
wr(N/'runtime-video-manifest.json',videos)
mid.update(runtimeVerified=True,scopedDefensePresentationVerified=True,runtimeEvidence='../npc-defense-presentation-audit-v1029.json')
wr(B/'identity.json',mid)
pin.update(runtimeVerified=True,scopedDefensePresentationVerified=True,scopedObserverCameraVerified=True,runtimeEvidence='../npc-defense-presentation-audit-v1029.json',
   scope='1026 (live): 1022 remote-body presentation of receiver native defense scenes (variants19-21 -> bank68 clips357/358; protocol70) verified 1023/1025/1028c, plus lab-only observer camera (render-view hold) verified 1027c and used for the host view in 1028c. Relay/timed parry from 1018 retained.')
wr(A/'identity.json',pin)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['NpcCombatContinuation'].update(at=at,candidate=A.name,receipt='work/movement-integration/npc-defense-presentation-audit-v1029.json',details='documentation/handoff-2026-10-11/80-native-timed-parry.md',
   latest='1026 live: the remote player body now presents the receiver native block impact and timed parry (bank68 clips357/358 via protocol70) and a lab observer camera lets the host window watch the remote player while its body stays parked (1028c PASS with video).',
   inProgress='Documentation and next combat/world items.',next='Remaining movement bindings (stagger/knockdown, death variants, crouched defense), enemy kicks need native AI defensive-mode perception of the remote player, then shared world authority, campaign progression, native session UI and installation docs.')
s['MovementExecutionPriority']['current']=s['NpcCombatContinuation']['latest'];wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['passed']) for c in cases])))
