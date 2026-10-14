from pathlib import Path
import datetime, hashlib, json, math, subprocess, shutil
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat()
specs=[('native-door-overlap-host-first-v747',False),('native-door-overlap-opposite-host-v748',False),('native-door-close-reopen-host-v750',False),('native-door-close-handoff-host-v751',False),('native-door-busy-use-host-v752',True),('native-door-busy-use-client-v753',True),('native-door-simultaneous-sides-v754',False),('native-door-simultaneous-sides-host-v756',True),('native-door-simultaneous-sides-client-v757',True),('native-door-busy-use-host-v758',True),('native-door-busy-use-client-v759',True),('native-door-simultaneous-sides-reverse-v760',True)]
pin=rd(W/'candidate-door-side-v755/identity.json'); assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
cases=[]
notes={747:'Retained collision-displaced fixture failure: source use never opened the door; source lacked the Open prompt.',748:'Retained failed reversal expectation. Both doors opened; second use during Moving did not reverse them.',750:'Retained invalid second-player fixture: observer faced a wall; no valid reversal coverage.',751:'Retained failed reversal expectation with known front use position handed off during closing. Native close completed unchanged.',754:'Confirmed defect in742: both report Open2/side1/applied3 but actual native Open sides differ0/1. Both opposite-side openings are recorded.',756:'Actual native sides converge after one close/reopen correction on Corvo. The door pushes the nearby source about half a game unit and the camera ends against a surface; this is not seamless interaction or presentation acceptance.',757:'Actual native sides converge after one close/reopen correction on Emily. Source ends close against a surface after native collision; observer retains the room view. This is not seamless interaction or presentation acceptance.',760:'Reverse-side conflict: native correction must converge to actual opening side0. Consult native observations and the paired recording review for visual limits.'}
for take,passed in specs:
    f=N/take;v=int(take.rsplit('v',1)[1]);r=rd(f/'result.json');assert r['passed']==passed,(take,r)
    source='client' if 'client' in take else 'host'; peer='host' if source=='client' else 'client'
    assert any('All14' in s for s in r['checks']) and any('inventor' in s.lower() for s in r['checks'])
    c=dict(take=take,version=v,source=source,passed=passed,dll=r['dll'],checks=r['checks'],failure=r['failure'])
    if passed:
        assert len(r['checks'])==5
        if v>=756:assert r['dll']==pin['dll']
        final=rd(f/'native-sync-final.json');a=final['source'];b=final['peer']
        assert a['enabled'] and b['enabled'] and a['revision']==b['revision']==a['applied']==b['applied']==3
        assert a['target']==b['target']==2 and a['side']==b['side']
        assert a['rejected']==final['baselineSource']['rejected'] and b['rejected']==final['baselinePeer']['rejected']
        c['sync']={role:{k:s.get(k,0) for k in ['role','revision','side','applied','nativeCalls','sideCorrections','requests','rejected']} for role,s in [('source',a),('peer',b)]}
        if 'simultaneous' in take:
            actual=rd(f/'native-final-sides.json'); starts=rd(f/'competing-native-starts.json');assert starts
            assert all(s['current']==s['pending']==2 and s['openSide']==a['side'] for s in actual.values())
            assert a['nativeCalls']+b['nativeCalls']==2 and a['sideCorrections']+b['sideCorrections']==1
            assert a['side']==(0 if v==760 else 1)
            obs=rd(f/f'{source}-unlocked-door-tap-observations.json')
            c['actual']=actual;c['sourceDisplacement']=math.dist(obs[0]['source']['localPosition'],obs[-1]['source']['localPosition'])
            c['nativeCycles']={role:[s['door'+role.title()]['current'] for i,s in enumerate(obs) if i==0 or s['door'+role.title()]['current']!=obs[i-1]['door'+role.title()]['current']] for role in ['source','peer']}
        else:
            busy=rd(f/'after-busy-use.json');assert busy['nativeSource']['current']==busy['nativePeer']['current']==0
            assert busy['source']['revision']==busy['peer']['revision']==2
            assert a['nativeCalls']==b['nativeCalls']==1
            if v>=758:assert a['sideCorrections']==b['sideCorrections']==0
    cases.append(c)
audit=dict(at=at,passed=True,candidate=pin['dll'],cases=cases,complete=False,scope='Explicit isolated ordinary-door catalog only. Native busy-use behavior in both orders; simultaneous opposite sides corrected via native close/reopen in both player roles, and reverse direction760. Retain five failed receipts. No automatic rearm, linked/locked/scripted door support, shared AI or full movement acceptance.')
wr(W/'native-door-side-audit-v762.json',audit)
videos=rd(N/'runtime-video-manifest.json');byVideo={v['video']:v for v in videos};indexed=[]
for c in cases:
    take=c['take'];f=N/take;v=c['version'];source=c['source'];character='corvo' if source=='host' else 'emily'
    note=notes.get(v,'Both doors finish the ordered close despite a use press while Moving; a fresh press after completion reopens both at revision3. Both input orders tested; no implied buffering or arbitrary reversal support.')
    note+=' Paired contact sheets inspected; fixture relocation/collision is not animation acceptance. Full movement and co-op remain incomplete.'
    for view,name in [('source',f'{source}-unlocked-door-tap.mp4'),('observer','observer.mp4')]:
        p=f/name;assert p.exists() and (f/f'{view}-sheet.jpg').exists()
        review=f/f'{view}-review-v762.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[f'{view}-sheet.jpg'],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel)
        item=dict(video=rel,kind='game',character=character,label=f'{character.title()} · native door conflict · v{v} · {view}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=c['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/native-door-side-audit-v762.json',visual_review=take+'/'+review.name,proxy_sha256=c['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=c['passed'],scope=note)
        if rel in byVideo:byVideo[rel].update(item)
        else:videos.append(item)
wr(N/'runtime-video-manifest.json',videos)
latest='Native door busy-use tests752/753 establish that pressing during closing leaves the close unchanged; fresh input afterward reopens.754 proves actual opening-side divergence despite logical Open acknowledgement. Candidate755 checks active native Open side and closes/reopens the wrong-side door through the native queue.756/757 pass opposite-side conflict with each player requiring correction;760 passes reverse direction.758/759 preserve ordinary use without correction. All inventories/health/protected files restored; paired video sheets reviewed with source collision/camera limitations retained.747/748/750/751/754 failed receipts retained. Full movement and playable co-op remain incomplete.'
nextWork='Continue automatic ordinary-door rearming after verified shared checkpoint readiness; preserve fresh world/epoch/arm identity and do not revive old bindings. Then shared pickups and world progression, missing movement contexts, general saves/campaign, native session UI and separate-account Steam verification. Fine animator polish stays deferred.'
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['DoorContinuation']=dict(at=at,candidate='candidate-door-side-v755',receipt='work/movement-integration/native-door-side-audit-v762.json',details='documentation/handoff-2026-10-11/59-native-door-conflicts-and-opening-side.md',nativeSourceVerified=True,worldSynchronized=True,worldSynchronizedScope='Six eligible isolated Dunwall ordinary-door channels; busy-use and opposite-side correction. No automatic rearming or general world authority.',complete=False,latest=latest,next=nextWork);s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
a=rd(N/'review-evidence-annotations.json');item=a['items']['door_open_close_locked'];item.update(complete=False,runtime_binding='native_ordinary_door_catalog_with_direction_convergence',notes=latest);item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[c['take']+'/result.json' for c in cases if c['passed']]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',a)
pin.update(runtimeVerified=True,runtimeAudit='../native-door-side-audit-v762.json',runtimeScope='756/757 both corrected player roles,760 opposite correction direction,758/759 ordinary busy-use regression. Explicit isolated catalog; visible native close/reopen and collision effects retained.');wr(W/'candidate-door-side-v755/identity.json',pin)
p=D/'README.md';t=p.read_text();line='- [Native door conflicts and opening direction](59-native-door-conflicts-and-opening-side.md)'
if line not in t:p.write_text(t.rstrip()+'\n\n'+line+'\n')
shutil.copy2(R/'outputs/d2-coop-lab/research/native-sword-setup-latest.json',W/'native-sword-setup-v762.json')
print(json.dumps(dict(passedCases=sum(c['passed'] for c in cases),retainedFailures=sum(not c['passed'] for c in cases),newVideos=len(indexed),allGameVideos=len(videos),complete=False)))
