from pathlib import Path
import json,datetime,hashlib,subprocess,shutil
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat()
takes=['native-door-reload-host-v736','native-door-reload-host-v737','native-door-reload-client-v738','native-door-reload-client-v741','native-door-retry-probe-client-v743','native-door-retry-probe-client-v744','native-door-retry-probe-host-v745']
failures={736,738,743};cases=[]
for take in takes:
    f=N/take;version=int(take.rsplit('v',1)[1]);r=rd(f/'result.json');passed=version not in failures
    assert r['passed']==passed,(take,r)
    source='client' if 'client' in take else 'host';peer='host' if source=='client' else 'client'
    c=dict(take=take,source=source,passed=passed,dll=r['dll'],failure=r['failure'],checks=r['checks'])
    moving=rd(f/'door-at-interruption.json')
    assert moving['source']['current']==moving['peer']['current']==1
    assert moving['peerSync']['nativeCalls']==1 and moving['sourceSync']['nativeCalls']==0
    if passed:
        assert len(r['checks'])==5
        before=rd(f/'reset-before.json');final=rd(f/'reset-final.json');c['roles']={}
        for role in ['host','client']:
            a=final[role]['reset'];b=before[role]
            assert a['loads']==b['loads']+1 and a['completed']==b['completed']+1 and a['aborted']==b['aborted']
            assert a['avatarReady'] and not a['avatarPending'] and a['avatarRestored']==b['avatarRestored']+1
            assert a['maxDistance']<=.001 and final[role]['character']['character']==(1 if role=='host' else 2)
            doors=final[role]['doors'];assert not doors['enabled'] and doors['result']=='all_channels_retired'
            assert len(doors['entries'])==6 and all(not e['enabled'] and e['result']=='world_input_or_connection_scope_changed' for e in doors['entries'])
            chosen=next(e for e in doors['entries'] if e['name']==r['door'])
            assert chosen['nativeCalls']==(1 if role==peer else 0)
            row=dict(heldMs=a['releasedAt']-a['heldAt'],maxDistance=a['maxDistance'],loadsDelta=1,completedDelta=1,avatarRestoredDelta=1)
            if version>=741:
                old=rd(f/f'loading-before-{role}.json');new=rd(f/f'loading-after-{role}.json')
                assert new['switches']-old['switches']<=32
                events=[e for e in new['events'] if e['sequence']>old['switches']]
                assert sum(e['caller']=='0x787a55' and e['to']==15 for e in events)==1
                assert sum(e['caller']=='0x7879cd' and e['to']==1 for e in events)==1
                row.update(nativeLoadingEntries=1,nativeLoadingFinishes=1)
                delta={k:a[k]-b[k] for k in ['continuationCalls','continuationDeferred','continuationAccepted']}
                row.update(delta)
                if version>=744:
                    assert a['retryProbeInjected']-b['retryProbeInjected']==2
                    assert delta['continuationDeferred']>=1 and delta['continuationAccepted']==1
                    trace=rd(f/f'final-{role}-checkpoint.json')
                    callbacks=[e for e in trace['events'] if e['continuation']][-delta['continuationCalls']:]
                    assert len(callbacks)==delta['continuationCalls'] and callbacks[-1]['returnLow']==1
                    assert all(e['returned'] and not e['consumed'] and e['cachedSlot']==82 for e in callbacks)
                    assert all(e['returnLow']==0 for e in callbacks[:-1])
                    assert any(e['ioState']>0 and e['returnLow']==0 for e in callbacks)
                    row['nativeCallbacks']=callbacks
            c['roles'][role]=row
        h=final['host']['reset'];cl=final['client']['reset']
        assert h['transaction']==cl['transaction'] and h['hostReady']==cl['hostReady'] and h['clientReady']==cl['clientReady']
        assert h['releasedAt']>=cl['readyAt'] and cl['releasedAt']>=h['readyAt']
    cases.append(c)
recovery=rd(W/'door-reload-failure-v738/recovery.json');assert recovery['passed'] and recovery['protectedFiles']==14
pin=rd(W/'candidate-checkpoint-retry-v742/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
audit=dict(at=at,passed=True,cases=cases,recovery738=recovery,scope='Fixed-SLOT82 shared reset during native door motion. Four passing receipts; three retained failures. Native deferred continuation followed by one accepted load verified under both request roles on742. No full campaign save/load acceptance, automatic door rearming, general world authority or movement polish acceptance.')
wr(W/'native-checkpoint-retry-audit-v746.json',audit)

videos=rd(N/'runtime-video-manifest.json');byVideo={v['video']:v for v in videos};indexed=[]
for c in cases:
    take=c['take'];f=N/take;source=c['source'];character='corvo' if source=='host' else 'emily';version=int(take.rsplit('v',1)[1])
    if version==736:note='Retained premature counterpart-readiness assertion. Native load/room return visible; restored inventory/health and protected files passed.737 fixes readiness polling.'
    elif version==738:note='Retained real shared-load failure: client waits at loading while host stays in room; both subsequently enter native pause. Restart recovery is separately verified. Original callback trace was not preserved, so its exact cause remains unproven.'
    elif version==743:note='Retained probe-readiness timing failure before reload. Cleanup released AI while waiting for recording; source was attacked and reached game over, observer stayed in room. This is not shared-death evidence because checkpoint sync was already disabled. Both inventories/health and protected files were restored. Receipt also reports observer wait timeout; media is indexed as failed evidence.'
    elif version in (744,745):note='Actual native memory-checkpoint callbacks defer while save I/O is busy, then accept exactly one load. Both native loading entries/finishes counted, both-ready hold releases together, roles and counterparts restored, all old door channels retired. Native loading/room return visible in both views.'
    else:note='Shared fixed-SLOT82 reset during active native door movement. Both native loading/room return views recorded; roles/counterparts restored and all old door channels retired.741 also counts actual native loading entry/finish; its busy-return branch did not occur.'
    note+=' This contact-sheet review is not whole-body animation acceptance. Pre-load Moving is established by component observations; capture startup often misses it. Shared spawn separation and automatic door rearming remain incomplete.'
    for view,name in [('source',f'{source}-unlocked-door-tap.mp4'),('observer','observer.mp4')]:
        p=f/name;assert p.exists() and (f/f'{view}-sheet.jpg').exists()
        # These exact sheets have been inspected in the main conversation.
        review=f/f'{view}-review-v746.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[f'{view}-sheet.jpg'],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel)
        v=dict(video=rel,kind='game',character=character,label=f'{character.title()} · shared checkpoint during door motion · v{version} · {view}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=c['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/native-checkpoint-retry-audit-v746.json',visual_review=take+'/'+review.name,proxy_sha256=c['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=c['passed'],scope=note)
        if rel in byVideo:byVideo[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='737 verified host shared reload during native door motion.738 exposed a real pre-loading stall; recovered exact isolated pair and preserved failed evidence. Ghidra plus pinned-executable assembly found native memory restart returns AL=0 and requeues itself during save I/O.740 retains continuation permission until AL=1 or actual loading entry;741 passes client reload.742 adds a bounded native retry probe.743 was a retained diagnostic-readiness harness failure;744/745 wait for probe readiness and prove real I/O deferrals followed by one accepted native load under both request roles. Both restore roles/counterparts after both-ready hold and retire all old door channels. Original738 cause is not retroactively proven. Full movement and playable co-op remain incomplete.'
nextWork='Resume overlapping native door inputs using the prepared739 experiment updated to pin742; repair observed ownership conflicts, then automatic door rearming at a verified new-world readiness boundary and lock/key/group/script semantics. Continue shared pickups/world and remaining functional movement coverage. Keep animator polish secondary. Shared checkpoint spawn separation, general saves/campaign, native session UI and separate-account Steam sessions remain outstanding.'
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['DoorContinuation']=dict(at=at,candidate='candidate-checkpoint-retry-v742',receipt='work/movement-integration/native-checkpoint-retry-audit-v746.json',details='documentation/handoff-2026-10-11/58-native-checkpoint-retry-and-door-interruption.md',nativeSourceVerified=True,worldSynchronized=True,worldSynchronizedScope='Previously verified six ordinary Dunwall channels/two operated doors; fixed-SLOT82 interruption retires old channels. Native busy retry correction in both request roles. Explicit isolated fixtures only.',complete=False,latest=latest,next=nextWork);s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
a=rd(N/'review-evidence-annotations.json');item=a['items']['door_open_close_locked'];item.update(complete=False,runtime_binding='native_ordinary_door_catalog_with_shared_reload_retirement',notes=latest);item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[c['take']+'/result.json' for c in cases if c['passed']]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',a)
for version in (740,742):
    p=W/f'candidate-checkpoint-retry-v{version}/identity.json';v=rd(p);v.update(runtimeVerified=True,runtimeAudit='../native-checkpoint-retry-audit-v746.json',runtimeScope=('Client shared reset passes741, no busy callback in that take.' if version==740 else 'Both native request roles744/745 exercise actual busy deferrals and one accepted load, roles/avatars restored, old door channels retired,14 protected hashes retained.')+' Fixed-SLOT82 isolated fixture only.');wr(p,v)
p=R/'work/handoff-project-bookmarks-current.json';book={(x['address'],x['category']):x for x in rd(p)};book.update({(x['address'],x['category']):x for x in rd(R/'work/handoff-project-bookmarks-checkpoint-v746.json')});wr(p,sorted(book.values(),key=lambda x:(x['address'],x['category'])))
p=D/'README.md';txt=p.read_text();line='- [Native checkpoint retry correction and door interruption](58-native-checkpoint-retry-and-door-interruption.md)'
if line not in txt:p.write_text(txt.rstrip()+'\n\n'+line+'\n')
shutil.copy2(R/'outputs/d2-coop-lab/research/native-sword-setup-latest.json',W/'native-sword-setup-v746.json')
print(json.dumps(dict(passedCases=4,retainedFailures=3,newVideos=len(indexed),allGameVideos=len(videos),candidate=pin['dll'],complete=False)))
