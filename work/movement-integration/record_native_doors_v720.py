from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
at=datetime.datetime.now().astimezone().isoformat()
verified=['native-door-api-corvo-v714','native-door-api-emily-v715','native-door-sync-emily-v718','native-door-sync-corvo-v719']
cases=[]
for take in verified:
    f=N/take;r=rd(f/'result.json');assert r['passed'] and len(r['checks'])==4,r
    role='client' if 'emily' in take else 'host';rows=rd(f/f'{role}-unlocked-door-tap-observations.json')
    def cycle(field):
        out=[]
        for row in rows:
            v=row[field]['current']
            if not out or out[-1]!=v:out.append(v)
        return out
    c=dict(take=take,passed=True,source=role,samples=len(rows),sourceCycle=cycle('doorSource'),peerCycle=cycle('doorPeer'),dll=r['dll'])
    assert c['sourceCycle']==[0,1,2,1,0]
    if 'sync' in take:
        final=rd(f/'native-sync-final.json');assert c['peerCycle']==[0,1,2,1,0]
        c.update(final=final,localApiCalls=final['source']['nativeCalls'],remoteApiCalls=final['peer']['nativeCalls'],hostRevision=final['source']['revision'],startupRejections=dict(source=final['baselineSource']['rejected'],peer=final['baselinePeer']['rejected']))
        assert c['localApiCalls']==0 and c['remoteApiCalls']==2 and c['hostRevision']==3
        for which in ['Source','Peer']:
            assert final[which.lower()]['rejected']==final['baseline'+which]['rejected']
        assert final['peer']['duplicates']>=2
    else:
        c['api']=rd(f/'native-api-final.json');assert c['peerCycle']==[0] and c['api']['calls']==2 and c['api']['noops']==2 and not c['api']['refused']
    cases.append(c)
audit=dict(at=at,passed=True,cases=cases,retainedFailures=['native-door-api-corvo-v708','native-door-api-corvo-v711','native-door-sync-corvo-v717'],scope='Two local native API proofs and two bidirectional ordinary-door Steam proofs. Each includes native state transitions, duplicate checks, full checkpoint inventory/health restoration and14 protected hashes. No full movement/world/campaign acceptance.')
wr(W/'native-door-audit-v720.json',audit)
videos=rd(N/'runtime-video-manifest.json');byVideo={v['video']:v for v in videos};indexed=[]
for take in audit['retainedFailures']+verified:
    f=N/take;r=rd(f/'result.json');role='client' if 'emily' in take else 'host';character='emily' if role=='client' else 'corvo'
    if take.endswith('708'):note='Retained failed reader proof: all four requests refused before any native call. Native base AnimBase type corrected afterward. Video not visually reviewed.'
    elif take.endswith('711'):note='Retained failed API proof: first open returnsClosed; later open works. The interrupt helper finalizes stale Moving state. Source shows transient bright/unrendered doorway; peer stays closed.'
    elif take.endswith('717'):note='Retained strict-counter harness failure. Both doors actually converge with two receiver calls; one rejection existed at the first sample before any action. No rejection increase during the measured actions.718/719 retain startup baselines explicitly; exact initial rejection cause remains uninstrumented.'
    elif 'api' in take:note='Local queue proof: source door visibly opens onto the furnished closet and closes; observer door remains closed. Exactly two native calls and two duplicate no-ops. No network door state applied.'
    else:note='Ordinary native X input causes both game doors to open and close. Receiver makes exactly two native queue calls; both reach host revision3. Replayed reliable packets do not reapply. Startup rejection is recorded separately; no rejection increases during measured actions.'
    note+=' Character visibility is limited by the doorway/camera and is not accepted as complete-body movement evidence. No audio, collision traversal, lock/key, linked-door or campaign-script validation.'
    for view,name in [('source',f'{role}-unlocked-door-tap.mp4'),('observer','observer.mp4')]:
        file=f/name;assert file.exists();sheet=f/f'{view}-sheet.jpg';inspected=sheet.exists() and not take.endswith('708')
        review=f/f'{view}-review-v720.json';wr(review,dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=[sheet.name] if inspected else [],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(file)],text=True))['streams'][0]
        with file.open('rb') as h:sha=hashlib.file_digest(h,'sha256').hexdigest().upper()
        rel=file.relative_to(N).as_posix();indexed.append(rel)
        entry=dict(video=rel,kind='game',character=character,label=f'{character.title()} · native door {"Steam" if "sync" in take else "API"} · {take[-4:]} · {view}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=bool(r['passed'] or take.endswith('717')),complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/native-door-audit-v720.json',visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha,actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=r['passed'],scope=note)
        if rel in byVideo:byVideo[rel].update(entry)
        else:videos.append(entry)
wr(N/'runtime-video-manifest.json',videos)
latest='713 native Moving queue fixes the fresh-door failure of interrupt API74d7a0.714Corvo/715Emily pass local native open/close plus duplicate no-ops.716 adds a disabled-by-default authored-door desired-state stream over native Steam, host revisions, client requests, arm/world/session validation and duplicate suppression.718Emily/719Corvo pass both native0->1->2->1->0 cycles with exactly two receiver calls, host revision3, no source API calls and no rejected packets during the measured actions. One initial client rejection predates measurement; its exact cause is not instrumented.717 retains its overly broad absolute-counter failure. Four latest game sheets show both doors opening/closing; character/portal visibility and collision remain separate concerns. Both checkpoint inventories/health and14 protected hashes restored in all takes.'
nextWork='Keep716 experimental and disabled outside its explicit Dunwall fixture. Test collision/traversal after replication, mixed-player and conflicting requests, active checkpoint interruption and stale-scope rejection; then generalize authored ordinary-door identity and key/lock/script semantics. Preserve all failure evidence. Continue functional movement/shared-world work without fine animator polish. Native UI, native Steam and protected original files remain requirements.'
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['DoorContinuation']=dict(at=at,candidate='candidate-door-sync-v716',receipt='work/movement-integration/native-door-audit-v720.json',details='documentation/handoff-2026-10-11/56-native-door-transition-api.md',nativeSourceVerified=True,worldSynchronized=True,worldSynchronizedScope='Only one ordinary unlocked unlinked Dunwall door, both input roles in718/719.',complete=False,latest=latest,next=nextWork)
s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
a=rd(N/'review-evidence-annotations.json');item=a['items']['door_open_close_locked'];item.update(complete=False,runtime_binding='native_ordinary_door_both_roles_bounded_steam_replication',notes=latest)
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[take+'/result.json' for take in verified]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',a)
for candidate,scope in [('candidate-door-api-v713','Local native API714/715 only'),('candidate-door-sync-v716','Only one authored ordinary door in718/719; not generalized door, collision or campaign acceptance')]:
    p=W/candidate/'identity.json';v=rd(p);v.update(runtimeVerified=True,runtimeAudit='../native-door-audit-v720.json',runtimeScope=scope);wr(p,v)
p=R/'work/handoff-project-bookmarks-current.json';b={(x['address'],x['category']):x for x in rd(p)}
b.update({(x['address'],x['category']):x for x in rd(R/'work/handoff-project-bookmarks-door-v720.json')});wr(p,sorted(b.values(),key=lambda x:(x['address'],x['category'])))
p=D/'README.md';t=p.read_text();link='- [Native door transition API and bounded Steam replication](56-native-door-transition-api.md)'
if link not in t:p.write_text(t.rstrip()+'\n\n'+link+'\n')
print(json.dumps(dict(verifiedCases=len(cases),indexedNewDoorVideos=len(indexed),allGameVideos=len(videos),candidate='candidate-door-sync-v716',completeMovement=False,completeWorld=False)))
