from pathlib import Path
import datetime,json,hashlib,subprocess,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();pin=rd(W/'candidate-door-menu-v769/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
names=['native-door-menu-baseline-v768','native-door-menu-host-v770','native-door-menu-client-v771','native-door-menu-moving-host-v772','native-door-menu-moving-client-v773','native-door-menu-use-host-v774','native-door-menu-use-client-v775']
cases=[];door='ld_door_350cm_int_rich_wood_01_3'
selected=lambda d:next(e for e in d['entries'] if e['name']==door)
for take in names:
    f=N/take;r=rd(f/'result.json');version=int(take.rsplit('v',1)[1]);ok=version!=768
    assert r['passed']==ok and len(r['checks'])==(4 if ok else 2)
    assert r['dll']==(pin['dll'] if ok else rd(W/'candidate-door-session-v763/identity.json')['dll'])
    c=dict(take=take,version=version,passed=ok,failure=r['failure'],pauseRole=r['pauseRole'],source=r['source'],moving=r['moving'],useDuringMenu=r.get('useDuringMenu',False),checks=r['checks'],roles={})
    held=rd(f/'menu-end.json');before=rd(f/'catalog-before.json')
    assert held[r['pauseRole']]['bridge']['localGameLogicState']==2
    if not ok:
        assert r['failure']=='Persistent door catalog retired or did not resume for host'
        assert all(not held[role]['doors']['enabled'] and held[role]['doors']['session']['result']=='all_channels_retired' for role in ('host','client'))
    else:
        final=rd(f/'native-final.json');observations=rd(f/'observations.json');entry=next(o for o in observations if o['tag']=='menu-entered')
        for role in ('host','client'):
            h=held[role];a=final[role];d=a['doors'];e=selected(d)
            assert h['doors']['menu']['suspended'] and h['doors']['session']['enabled']
            assert d['enabled'] and d['session']['enabled'] and not d['menu']['suspended']
            assert d['menu']['suspensions']==d['menu']['resumes']==1 and d['menu']['pausedMs']>=6000
            assert d['session']['reloads']==d['session']['rearms']==0
            assert a['bridge']['localEpoch']==h['bridge']['localEpoch'] and a['bridge']['remoteEpoch']==h['bridge']['remoteEpoch']
            assert e['revision']==e['applied']==2 and e['target']==2 and e['nativeCalls']==(1 if role==r['pauseRole'] else 0)
            assert a['native']['current']==a['native']['pending']==2 and a['native']['side']==e['side']
            assert selected(h['doors'])['nativeCalls']==selected(entry[role]['doors'])['nativeCalls']
            for old in before[role]['entries']:
                new=next(x for x in d['entries'] if x['name']==old['name'])
                assert new['enabled'] and new['ready'] and new['arm']==old['arm'] and new['peerArm']==old['peerArm']
                if old['name']!=door:
                    # Some early snapshots have a completed Hello but no host
                    # baseline yet (revision/applied0). Never interpret target0
                    # in that snapshot as a confirmed closed native door.
                    initialized=next(x for x in entry[role]['doors']['entries'] if x['name']==old['name'])
                    authoritative=next(x for x in before['host']['entries'] if x['name']==old['name'])
                    assert initialized['revision']==initialized['applied']==authoritative['revision']==1
                    assert new['revision']==new['applied']==1 and new['nativeCalls']==initialized['nativeCalls']==0
                    assert new['target']==initialized['target']==authoritative['target']
            assert all(x['revision'] for x in entry[role]['doors']['entries'])
            c['roles'][role]=dict(menu=d['menu'],door=e,native=a['native'],epoch=a['bridge']['localEpoch'],initialUninitializedChannels=[x['name'] for x in before[role]['entries'] if not x['revision']])
        if r['moving']:
            assert held[r['pauseRole']]['native']['current']==1 and selected(held[r['pauseRole']]['doors'])['inFlight']==2
            assert held[r['source']]['native']['current']==2
        if r.get('useDuringMenu'):
            assert held[r['pauseRole']]['native']['current']==0 and held[r['source']]['native']['current']==2
    cases.append(c)
take='native-door-menu-reload-client-v776';f=N/take;r=rd(f/'result.json');assert r['passed'] and len(r['checks'])==5 and r['dll']==pin['dll']
final=rd(f/'reset-final.json');before=rd(f/'reset-before.json');oldDoors=rd(f/'catalog-before.json');use=rd(f/'postload-door-use.json')
c=dict(take=take,version=776,passed=True,failure=None,source='client',checks=r['checks'],roles={})
for role in ('host','client'):
    a=final[role]['reset'];b=before[role];d=final[role]['doors'];s=d['session'];bridge=final[role]['bridge']
    assert a['completed']==b['completed']+1 and a['loads']==b['loads']+1 and a['aborted']==b['aborted'] and a['maxDistance']<=.001
    assert a['avatarReady'] and not a['avatarPending'] and a['avatarRestored']==b['avatarRestored']+1
    assert final[role]['character']['character']==(1 if role=='host' else 2)
    assert s['enabled'] and not s['reloadPending'] and s['reloads']==s['rearms']==1 and s['retiredChannels']==6
    assert s['epoch']==bridge['localEpoch'] and s['peerEpoch']==bridge['remoteEpoch'] and s['epoch']!=oldDoors[role]['session']['epoch']
    assert d['enabled'] and d['catalogCount']==6 and d['ineligible']==4
    for old in oldDoors[role]['entries']:
        new=next(e for e in d['entries'] if e['name']==old['name'])
        assert new['enabled'] and new['ready'] and new['arm']>old['arm'] and new['revision']==new['applied']==1 and new['nativeCalls']==0
    oldSwitch=rd(f/f'loading-before-{role}.json');newSwitch=rd(f/f'loading-after-{role}.json');events=[e for e in newSwitch['events'] if e['sequence']>oldSwitch['switches']]
    assert sum(e['caller']=='0x787a55' and e['to']==15 for e in events)==1 and sum(e['caller']=='0x7879cd' and e['to']==1 for e in events)==1
    isolation=rd(f/f'postload-isolation-{role}.json');ai=rd(f/f'postload-native-ai-{role}.json')
    assert not isolation['active'] and isolation['heldReferences']==0 and ai['started'] and ai['schedulerActive'] and ai['pauseReferences']==0
    c['roles'][role]=dict(epoch=s['epoch'],rearms=s['rearms'],nativeLoadingEntries=1,nativeLoadingFinishes=1,maxDistance=a['maxDistance'])
for key in ('source','peer'):
    a=use[key];native=use['native'+key.title()];assert a['enabled'] and a['revision']==a['applied']==2 and a['target']==2 and a['nativeCalls']==(0 if key=='source' else 1) and native['current']==2
cases.append(c)
scope='Same-world native pause2 in both roles, idle and receiver native Moving held over six seconds, plus native use by either unpaused player. Existing arms persist, native queues do not run while menu held, original transition completes once or new desired state synchronizes after resume. Single same-map fixed-SLOT82 shared reload regression also passes. Read-note4/wheel13 retention is code/unit-tested only; not native runtime accepted here. No whole-session pause, shared AI, full movement, campaign, general save or release-UI completion claim.'
wr(W/'native-door-menu-audit-v777.json',dict(at=at,passed=True,candidate=pin['dll'],cases=cases,scope=scope,complete=False))
videos=rd(N/'runtime-video-manifest.json');byVideo={v['video']:v for v in videos};indexed=[]
for c in cases:
    take=c['take'];f=N/take
    if c['version']==776:
        views=[('source','client','client-unlocked-door-tap.mp4'),('observer','host','observer.mp4')]
        note='Native loading screens, room return and fresh closet opening visible; receipt verifies automatic new-epoch bindings, roles/counterparts and native AI active after load. No menu stimulus in this checkpoint regression.'
    else:
        views=[(role,role,role+'.mp4') for role in ('host','client')]
        note=('Retained763 failure: native Corvo pause menu and return visible, closet remains closed; both catalog retirements are recorded.' if not c['passed'] else 'Native pause menu and return visible in selected player view; other view retains gameplay. Closet opening visible in both views after resume. '+('Native reads prove receiver stayed Moving across the full pause while the source completed; final actual state and side agree without another native opening.' if c['moving'] else 'Native use during the partner menu leaves the receiver closed until resume, then both actual open states and sides agree.' if c['useDuringMenu'] else 'New source use occurs after resume; both actual open states and sides agree.'))
    note+=' Contact sheets inspected. Functional world-interaction footage, not character-animation quality acceptance. '+scope
    for view,role,name in views:
        p=f/name;sheet=f'{view}-sheet.jpg';assert p.exists() and (f/sheet).exists()
        review=f/f'{view}-review-v777.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel);character='corvo' if role=='host' else 'emily'
        v=dict(video=rel,kind='game',character=character,label=f'{character.title()} · native door menu lifecycle · v{c["version"]} · {view}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=c['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/native-door-menu-audit-v777.json',visual_review=take+'/'+review.name,proxy_sha256=rd(f/'result.json')['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=c['passed'],scope=note)
        if rel in byVideo:byVideo[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='Candidate769 fixes the native-menu retirement reproduced in768.770–775 pass both-role pause2 idle, moving-door timeout suspension and native use during the other player menu. Six original arms/epochs survive and no door queue runs while the menu is held; fresh peer handshakes recover one native receiver transition.776 preserves the earlier single shared-checkpoint renewal, role/counterpart restoration and fresh use with native AI active afterward. Sixteen paired videos/contact sheets inspected, retained failed baseline, complete restoration/protected checks. Read-note/wheel runtime and continuous repeated loads remain unverified.'
nextWork='Test repeated shared checkpoints and shared death/retry within one continuously armed session, then extend shared pickups and enemy/world authority. Complete remaining movement/native fixtures, general saves/campaign boundaries, native session UI and separate-account Steam validation. Keep animator polish secondary; do not expand paused-state whitelists without native evidence.'
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['DoorContinuation']=dict(at=at,candidate='candidate-door-menu-v769',receipt='work/movement-integration/native-door-menu-audit-v777.json',details='documentation/handoff-2026-10-11/61-door-native-menu-resume.md',nativeSourceVerified=True,worldSynchronized=True,worldSynchronizedScope=scope,complete=False,latest=latest,next=nextWork);s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
a=rd(N/'review-evidence-annotations.json');item=a['items']['door_open_close_locked'];item.update(complete=False,runtime_binding='native_ordinary_door_session_menu_resume_and_same_map_checkpoint_renewal',notes=latest);item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[c['take']+'/result.json' for c in cases]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',a)
pin.update(runtimeVerified=True,runtimeAudit='../native-door-menu-audit-v777.json',runtimeScope=scope);wr(W/'candidate-door-menu-v769/identity.json',pin)
p=D/'README.md';t=p.read_text();line='- [Ordinary doors survive native pause and resume](61-door-native-menu-resume.md)'
if line not in t:p.write_text(t.rstrip()+'\n\n'+line+'\n')
shutil.copy2(R/'outputs/d2-coop-lab/research/native-sword-setup-latest.json',W/'native-sword-setup-v777.json')
print(json.dumps(dict(passedCases=sum(c['passed'] for c in cases),retainedFailures=sum(not c['passed'] for c in cases),newVideos=len(indexed),allGameVideos=len(videos),candidate=pin['dll'],complete=False)))
