from pathlib import Path
import datetime,json,hashlib,subprocess,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();pin=rd(W/'candidate-door-session-v763/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
takes=[('native-door-session-reload-client-v764','client'),('native-door-session-reload-host-v765','host'),('native-door-session-live-ai-v766','host')];cases=[]
for take,source in takes:
    f=N/take;r=rd(f/'result.json');version=int(take.rsplit('v',1)[1]);assert r['passed']==(version<766) and len(r['checks'])==(5 if version<766 else 4) and r['dll']==pin['dll']
    c=dict(take=take,version=version,source=source,passed=r['passed'],failure=r['failure'],checks=r['checks'],roles={})
    if version<766:
        final=rd(f/'reset-final.json');before=rd(f/'reset-before.json');oldDoors=rd(f/'catalog-before.json');use=rd(f/'postload-door-use.json')
        for role in ['host','client']:
            a=final[role]['reset'];b=before[role];d=final[role]['doors'];s=d['session'];bridge=final[role]['bridge']
            assert a['completed']==b['completed']+1 and a['loads']==b['loads']+1 and a['aborted']==b['aborted'] and a['maxDistance']<=.001
            assert a['avatarReady'] and not a['avatarPending'] and a['avatarRestored']==b['avatarRestored']+1
            assert final[role]['character']['character']==(1 if role=='host' else 2)
            assert s['enabled'] and not s['reloadPending'] and s['reloads']==s['rearms']==1 and s['retiredChannels']==6
            assert s['retiredNativeCalls']==(0 if role==source else 1)
            assert s['epoch']==bridge['localEpoch'] and s['peerEpoch']==bridge['remoteEpoch'] and s['epoch']!=oldDoors[role]['session']['epoch']
            assert d['enabled'] and d['catalogCount']==6 and d['ineligible']==4
            for old in oldDoors[role]['entries']:
                new=next(e for e in d['entries'] if e['name']==old['name'])
                assert new['enabled'] and new['ready'] and new['arm']>old['arm'] and new['revision']==new['applied']==1 and new['nativeCalls']==0
            oldSwitch=rd(f/f'loading-before-{role}.json');newSwitch=rd(f/f'loading-after-{role}.json');events=[e for e in newSwitch['events'] if e['sequence']>oldSwitch['switches']]
            assert sum(e['caller']=='0x787a55' and e['to']==15 for e in events)==1 and sum(e['caller']=='0x7879cd' and e['to']==1 for e in events)==1
            isolation=rd(f/f'postload-isolation-{role}.json');ai=rd(f/f'postload-native-ai-{role}.json')
            assert not isolation['active'] and isolation['heldReferences']==0 and ai['started'] and ai['schedulerActive'] and ai['pauseReferences']==0
            c['roles'][role]=dict(epoch=s['epoch'],rearms=s['rearms'],retired=s['retiredChannels'],retiredNativeCalls=s['retiredNativeCalls'],heldMs=a['releasedAt']-a['heldAt'],maxDistance=a['maxDistance'],nativeAI=ai,loadsDelta=1,completedDelta=1,nativeLoadingEntry=1,nativeLoadingFinish=1)
        for key in ['source','peer']:
            a=use[key];native=use['native'+key.title()];assert a['enabled'] and a['revision']==a['applied']==2 and a['target']==2 and a['nativeCalls']==(0 if key=='source' else 1) and native['current']==2
        h=final['host']['reset'];cl=final['client']['reset'];assert h['transaction']==cl['transaction'] and h['hostReady']==cl['hostReady'] and h['clientReady']==cl['clientReady'] and h['releasedAt']>=cl['readyAt'] and cl['releasedAt']>=h['readyAt']
        c['postloadUse']=use
    else:
        final=rd(f/'native-sync-final.json');catalog=rd(f/'catalog-before.json')
        for role in ['host','client']:
            isolation=rd(f/f'native-isolation-before-{role}.json');ai=rd(f/f'native-ai-before-{role}.json')
            assert catalog[role]['session']['enabled'] and not isolation['active'] and isolation['heldReferences']==0 and ai['started'] and ai['schedulerActive'] and ai['pauseReferences']==0
            c['roles'][role]=dict(nativeAI=ai,initialSession=catalog[role]['session'])
        for key in ['source','peer']:
            a=final[key];assert a['enabled'] and a['revision']==a['applied']==3 and a['target']==2 and a['nativeCalls']==1 and a['sideCorrections']==0
        assert r['failure']=='Health changed during neutral targeting observation'
        c['finalUse']=final;c['interactionChecksPassed']=True;c['visualFailure']='Source recording shows a native guard approaching and striking the stationary player with visible damage feedback; observer remains at the bathroom. Exact damage amount/attacker handle was not saved by this harness.'
    cases.append(c)
scope='Explicit native-console ordinary-door session; fixed-SLOT82 same-map reload in both request roles, new arms/epochs/no old request applied, active native AI after load and fresh native door use. Initial live-AI test766 passes door interaction checks but fails overall health invariant: a native guard attacks the stationary source, visibly recorded. Full inventory/health/protected restoration passes. No shared AI authority, cross-campaign save/load, native session UI or complete movement claim. Same-world menu suspension and repeated loads remain outstanding.'
wr(W/'native-door-session-audit-v767.json',dict(at=at,passed=True,candidate=pin['dll'],cases=cases,scope=scope,complete=False))
videos=rd(N/'runtime-video-manifest.json');byVideo={v['video']:v for v in videos};indexed=[]
for c in cases:
    take=c['take'];f=N/take;source=c['source'];character='corvo' if source=='host' else 'emily'
    note=('Native loading screens and checkpoint room return visible; fresh closet opening appears in both views. Roles/counterparts restored and six channels rebuilt automatically; no door-enable command after load. Native component reads verify AI active and zero isolation references. Pre-load Moving is established by native observations; capture startup often misses it.' if c['version']<766 else 'Retained failed full test. Native bathroom close, busy use and subsequent reopening pass with native AI active from initial session arm. Later a native guard approaches and strikes the stationary source, with visible damage feedback; the observer remains at the bathroom. Exact damage amount/attacker handle was not saved. Fixture relocation is part of the test; no movement-isolation command is used. Cleanup restores inventory/health and protected files.')
    note+=' Paired contact sheets inspected; this is functional interaction evidence, not body/weapon animation acceptance. '+scope
    for view,name in [('source',f'{source}-unlocked-door-tap.mp4'),('observer','observer.mp4')]:
        p=f/name;assert p.exists() and (f/f'{view}-sheet.jpg').exists();review=f/f'{view}-review-v767.json'
        wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[f'{view}-sheet.jpg'],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel);v=dict(video=rel,kind='game',character=character,label=f'{character.title()} · door session recovery · v{c["version"]} · {view}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=c['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/native-door-session-audit-v767.json',visual_review=take+'/'+review.name,proxy_sha256=pin['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=c['passed'],scope=note)
        if rel in byVideo:byVideo[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='Candidate763 adds explicit ordinary-door session intent independent of AI-isolation fixtures.764/765 both request roles pass one actual shared load during native door motion, both-ready hold, roles/counterparts restored, six old bindings retired and six new arms/epochs automatically created without replay. A fresh native use afterward synchronizes. Native AI is active with zero pause references.766 passes initial live-AI door interaction checks but fails the overall health invariant: a native guard visibly attacks the stationary source later in the recording. Retain failure, preserve ordinary enemy damage and continue shared world authority. All inventories/health/protected files restored; six video sheets inspected. No native session-setup UI or general world/movement completion claim.'
nextWork='Verify and implement same-world native-menu suspension/resume for persistent doors; source inspection shows current readiness failure retires channels and wall-time operation deadlines need handling. Then repeated reloads and shared death/retry in one session; shared pickups/world progression, remaining movement contexts, general saves/campaign, native session UI and separate-account Steam validation. Keep animator polish secondary.'
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['DoorContinuation']=dict(at=at,candidate='candidate-door-session-v763',receipt='work/movement-integration/native-door-session-audit-v767.json',details='documentation/handoff-2026-10-11/60-door-session-checkpoint-renewal.md',nativeSourceVerified=True,worldSynchronized=True,worldSynchronizedScope=scope,complete=False,latest=latest,next=nextWork);s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
a=rd(N/'review-evidence-annotations.json');item=a['items']['door_open_close_locked'];item.update(complete=False,runtime_binding='native_ordinary_door_session_with_same_map_checkpoint_renewal',notes=latest);item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[c['take']+'/result.json' for c in cases]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',a)
pin.update(runtimeVerified=True,runtimeAudit='../native-door-session-audit-v767.json',runtimeScope=scope);wr(W/'candidate-door-session-v763/identity.json',pin)
p=D/'README.md';t=p.read_text();line='- [Door-session renewal after shared checkpoints](60-door-session-checkpoint-renewal.md)'
if line not in t:p.write_text(t.rstrip()+'\n\n'+line+'\n')
shutil.copy2(R/'outputs/d2-coop-lab/research/native-sword-setup-latest.json',W/'native-sword-setup-v767.json')
print(json.dumps(dict(passedCases=sum(c['passed'] for c in cases),retainedFailures=sum(not c['passed'] for c in cases),newVideos=len(indexed),allGameVideos=len(videos),candidate=pin['dll'],complete=False)))
