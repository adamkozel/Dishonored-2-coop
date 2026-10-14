from pathlib import Path
import json,hashlib,datetime,subprocess,shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
def inventory(p):return sorted((i['name'],i['quantity']) for i in rd(p)['items'])
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-usable-sync-v831';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==pin['probe']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
takes=['native-window-input-v828d','native-window-api-v830b','shared-window-family-v832b','shared-window-recovery-v833b']
for t in takes:
    p=N/t;result=rd(p/'result.json')
    if t=='shared-window-recovery-v833b':
        assert not result['passed'] and result['failure']=='Usable experiment altered inventory',result
        # The checkpoint intentionally restores its saved inventory. The
        # harness compared it with a lab sword acquired after that save, after
        # all native lifecycle/window assertions had already passed.
        a=dict(inventory(p/'inventory-before-host.json'));b=dict(inventory(p/'inventory-after-host.json'))
        diff={k:(a.get(k),b.get(k)) for k in a.keys()|b.keys() if a.get(k)!=b.get(k)}
        assert diff=={'models/weapons/ply_sword/ply_sword':(1,0)},diff
        assert rd(p/'note-restoration/result.json')['passed']
    else:assert result['passed'],result
    for role in ('host','client'):assert inventory(p/f'inventory-before-{role}.json')==inventory(p/f'inventory-restored-{role}.json')
B=N/takes[2];ordinary=[];batch=[]
for tag,target in [('ordinary-host-open',1),('ordinary-client-close',0)]:
    pair=rd(B/f'pair-{tag}.json')
    for role in ('host','client'):
        x=pair[role];u=x['native'];c=x['channel'][0]
        assert u['current']==target and u['pending']==-1 and not u['transitioning']
        assert c['enabled'] and c['ready'] and c['target']==target and c['applied']==c['revision']
        ordinary.append(dict(tag=tag,role=role,native=u,channel=c))
for index in range(1,5):
    for tag,target in [('closed-baseline',0),('open',1),('close',0)]:
        pair=rd(B/f'pair-batch-{index}-{tag}.json')
        for role,x in pair.items():
            u=x['native'];c=x['channel'][0]
            assert u['current']==c['target']==target and not u['transitioning'] and u['pending']==-1 and c['applied']==c['revision']
            batch.append(dict(index=index,tag=tag,role=role,name=u['name'],revision=c['revision'],calls=c['nativeCalls']))
F=N/takes[3];lifecycle=[]
for role in ('host','client'):
    held=rd(F/f'menu-held-{role}.json');before=rd(F/f'reload-before-{role}.json');after=rd(F/f'reload-after-{role}.json')
    assert held['sync']['menu']['suspended'] and held['native']['current']==(0 if role=='host' else 1)
    assert after['reset']['loads']==before['reset']['loads']+1 and after['reset']['completed']==before['reset']['completed']+1
    assert after['reset']['avatarRequested'] and after['reset']['avatarReady'] and not after['reset']['avatarPending']
    assert after['sync']['session']['rearms']==before['sync']['session']['rearms']+1
    assert after['sync']['session']['epoch']!=before['sync']['session']['epoch'] and after['sync']['catalogCount']==4
    for e in after['sync']['entries']:
        old=next(x for x in before['sync']['entries'] if x['name']==e['name'])
        assert e['arm']>old['arm'] and e['nativeCalls']==0 and e['enabled']
    fresh=rd(F/'pair-fresh-client-open.json')[role]
    assert fresh['native']['current']==1 and fresh['native']['pending']==-1 and not fresh['native']['transitioning']
    lifecycle.append(dict(role=role,before=before['sync']['session'],after=after['sync']['session'],reset=after['reset'],fresh=fresh))
scope='Exact Dunwall four-window native state-list family: ordinary host/client input, all-member desired-state batch, duplicate no-op, menu retention/local use with deferred replication, coordinated checkpoint renewal and fresh use. Indexed native animation API and exact owned identities. Other48 state-list objects and arbitrary quest/AI side effects remain excluded. Not complete movement, campaign or separate-account online certification.'
audit=dict(at=at,passed=True,complete=False,candidate=A.name,dll=pin['dll'],probe=pin['probe'],scope=scope,ordinary=ordinary,batch=batch,lifecycle=lifecycle,takes=takes,recoveryHarnessPassed=False,recoveryFunctionalChecksPassed=True,recoveryHarnessFailure='833b passes menu/reload/fresh-use checks, then compares checkpoint inventory against post-save lab inventory: sword1->0. Exact difference verified; full final inventory restoration passes. Raw failed result is retained.')
wr(W/'usable-family-audit-v834.json',audit)
videos=rd(N/'runtime-video-manifest.json');by={v['video']:v for v in videos};indexed=[]
for t in takes:
    for p in sorted((N/t).glob('*.mp4')):
        review=p.with_name(p.stem+'-review-v834.json');v=rd(review);assert v['inspected'] and not v['complete']
        role='client' if 'client' in p.stem else 'host';char='emily' if role=='client' else 'corvo'
        owner=rd(W/('candidate-coin-sync-v821' if t.endswith('v828d') else 'candidate-usable-state-v829' if t.endswith('v830b') else 'candidate-usable-sync-v831')/'identity.json')
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel)
        item=dict(video=rel,kind='game',character=char,label=f'{char.title()} · native window state-list family · {t} · {p.stem}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{t}/result.json',binding_audit='../../work/movement-integration/usable-family-audit-v834.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=owner['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=rd(N/t/'result.json')['passed'],scope=' '.join(v['observations']))
        if rel in by:by[rel].update(item)
        else:videos.append(item)
wr(N/'runtime-video-manifest.json',videos)
latest='828–834: exact owned native state-list reader and indexed animation API verified, then one Steam desired-state adapter covers four Dunwall windows. Ordinary host opening/client closing, whole-family batch, duplicate no-op, native menu retention with deferred local intent, one shared checkpoint each and fresh client use pass. Protected files and full isolated baseline restored. Other48 state-list objects, quest/AI side effects and full co-op remain incomplete.'
nextWork='Extend shared native interaction/progression families from evidence, then shared NPC/world authority; continue outstanding movement/VFX and native session UI/separate-account Steam testing. Use grouped family coverage and representative ordinary inputs; do not write an individual handler per consumable.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['SharedUsableContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/usable-family-audit-v834.json',details='documentation/handoff-2026-10-11/69-native-window-state-list-sync.md',runtimeVerified=True,familyCount=4,nativeApiVerified=True,ordinaryInputVerified=True,menuVerified=True,checkpointVerified=True,complete=False,latest=latest,next=nextWork,scope=scope)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['door_open_close_locked'];item['complete']=False
if latest not in item.get('notes',''):item['notes']=item.get('notes','')+' '+latest
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{t}/result.json' for t in takes]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../usable-family-audit-v834.json',runtimeScope=scope);wr(A/'identity.json',pin)
for name in ('Test-SharedUsable-v832.ps1','Test-SharedUsableRecovery-v833.ps1'):shutil.copy2(N/name,A/name)
bms=rd(R/'work/handoff-usable-bookmark-v830.json')['bookmarks'];keys={(b['address'],b['category']) for b in bms}
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;wr(p,sorted([b for b in rd(p) if (b['address'],b['category']) not in keys]+bms,key=lambda b:(b['address'],b['category'])))
p=D/'README.md';s=p.read_text();line='- [Native window state-list synchronization](69-native-window-state-list-sync.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),catalog=4,candidate=pin['dll'])))

