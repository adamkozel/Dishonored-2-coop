from pathlib import Path
import json, datetime, hashlib, subprocess, shutil

R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
at=datetime.datetime.now().astimezone().isoformat()
takes=['native-door-traversal-emily-v723','native-door-traversal-corvo-v724',
       'native-door-catalog-corvo-v729','native-door-catalog-second-emily-v730',
       'native-door-catalog-second-corvo-v731','native-door-mixed-host-first-v732',
       'native-door-mixed-client-first-v733','native-door-catalog-second-view-v734']
cases=[]
for take in takes:
    f=N/take;r=rd(f/'result.json');assert r['passed'] and len(r['checks'])==5,(take,r)
    source=r['cases'][0]['source'];rows=rd(f/f'{source}-unlocked-door-tap-observations.json');final=rd(f/'native-sync-final.json')
    def cycle(field):
        result=[]
        for x in rows:
            v=x[field]['current']
            if not result or result[-1]!=v:result.append(v)
        return result
    expected=[2,1,0,1,2] if r.get('initialOpen') else [0,1,2,1,0]
    assert cycle('doorSource')==expected and cycle('doorPeer')==expected,take
    mixed='mixed' in take
    assert final['source']['nativeCalls']==(1 if mixed else 0)
    assert final['peer']['nativeCalls']==(1 if mixed else 2)
    for side in ['Source','Peer']:
        a=final[side.lower()];b=final['baseline'+side]
        assert a['revision']==3 and a['applied']==3 and a['rejected']==b['rejected']
    c=dict(take=take,source=source,passed=True,samples=len(rows),cycle=expected,dll=r['dll'],final=final)
    if 'traversal' in take:
        pos=rd(f/'collision-positions.json');c['positions']=pos
        for label in ['closed','reclosed']:assert pos[label+'After'][1]>-164.4
        assert pos['openAfter'][1]<-165 and abs(pos['openAfter'][0]-pos['openBefore'][0])<.35
    else:
        before=rd(f/'catalog-before.json');after=rd(f/'catalog-after.json');c['catalog']={}
        for role in ['host','client']:
            a=after[role];b=before[role]
            assert a['catalogCount']==6 and a['ineligible']==4 and a['ambiguous']==0
            assert a['enabled'] and a['unmatched']==b['unmatched']==0 and a['rejected']==b['rejected']==0
            old={e['name']:e for e in b['entries']}
            for e in a['entries']:
                assert e['enabled'] and e['ready']
                if e['name']!=r['door']:
                    assert e['revision']==e['applied']==1 and e['nativeCalls']==0 and e['target']==old[e['name']]['target']
            c['catalog'][role]=dict(count=a['catalogCount'],ineligible=a['ineligible'],scanMs=a['scanMs'],serviceMaxMs=a['serviceMaxMs'])
    cases.append(c)
failed=['native-door-traversal-emily-v722','native-door-catalog-corvo-v726']
for take in failed:assert not rd(N/take/'result.json')['passed']
audit=dict(at=at,passed=True,cases=cases,retainedFailures=failed,scope='Two716 physical traversal proofs; six728 ordinary authored-catalog proofs across two doors, both source roles and both sequential operator orders. No simultaneous conflicts, loading interruption, lock/key/group or campaign-wide acceptance. Timings use coarse GetTickCount64;16ms values are not precise profiling.')
wr(W/'native-door-catalog-audit-v735.json',audit)

videos=rd(N/'runtime-video-manifest.json');byVideo={v['video']:v for v in videos};indexed=[]
for take in [failed[0]]+takes:
    f=N/take;r=rd(f/'result.json');source=r['cases'][0]['source'] if r['cases'] else 'host';character='corvo' if source=='host' else 'emily'
    traversal='traversal' in take;mixed='mixed' in take
    for view,file in [('source',f'{source}-unlocked-door-tap.mp4'),('observer','observer.mp4')]:
        p=f/file;assert p.exists();inspected=not take.endswith('722')
        if inspected:assert (f/f'{view}-sheet.jpg').exists()
        if take.endswith('722'):note='Retained failed off-center traversal fixture: open-door crossing occurred but lateral drift exceeded the0.35 bound. Images not visually inspected; centered723/724 supersede this geometry.'
        elif traversal:note='Native closed and reclosed door blocks forward movement; remotely opened doorway permits crossing on the same floor. Source/receiver contact sheets show door and closet traversal; camera repositioning happens outside measured walking windows.'
        elif take.endswith(('730','731')) and view=='observer':note='Native state assertions pass, but the observer camera is misplaced and shows a bright blank view. This recording is not usable receiver visual evidence.734 records a corrected view.'
        elif mixed:note='Players take turns operating one door without rearming: first opens, second closes. Both native cycles converge at revision3, with one remote API call per process and no measured rejection delta. Contact sheet shows the doorway through the operator handoff.'
        else:note='Native ordinary-door open/close cycle; six eligible channels ready, four excluded doors, five unrelated channels remain unchanged. Both state machines converge at revision3 with exactly two receiver API calls.'
        note+=' No complete-body animation acceptance, audio, linked/locked doors, specific story-event actor validation or campaign-wide acceptance.'
        review=f/f'{view}-review-v735.json';wr(review,dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=[f'{view}-sheet.jpg'] if inspected else [],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        with p.open('rb') as h:sha=hashlib.file_digest(h,'sha256').hexdigest().upper()
        rel=p.relative_to(N).as_posix();indexed.append(rel)
        v=dict(video=rel,kind='game',character=character,label=f'{character.title()} · native door {"traversal" if traversal else "mixed operators" if mixed else "catalog"} · {take[-4:]} · {view}',alias='door_open_close_locked',requirements=['door_open_close_locked'],gameplay_bound=r['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/native-door-catalog-audit-v735.json',visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha,actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=r['passed'],scope=note)
        if rel in byVideo:byVideo[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='723Emily/724Corvo verify physical traversal through the remotely opened ordinary door and closed/reclosed blocking.725 generalizes to authored ordinary-door channels;726 exposed an inactive Open-state side-byte assumption.727/Ghidra established side is only meaningful when Open;728 fixes snapshot interpretation without memory writes.729-734 validate six ordinary channels, four excluded doors, a second initially open door in both source roles, and both sequential mixed-operator orders with one received API call each.730/731 observer videos were unusable due camera placement;734 corrects the view. Eight passing receipts and two retained failures restore full checkpoint inventories/health and14 protected hashes. No full campaign or movement acceptance.'
nextWork='Continue authored-door authority with rapid/conflicting native actions, checkpoint interruption and old-arm/epoch refusal, then lock/key/group/script semantics and shared pickups. Candidate728 stays experimental under its explicit isolated fixture; newly eligible or newly spawned doors require a fresh catalog. Continue functional movement/shared-world development without fine animation polish. Native UI and native Steam remain required.'
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['DoorContinuation']=dict(at=at,candidate='candidate-door-catalog-v728',receipt='work/movement-integration/native-door-catalog-audit-v735.json',details='documentation/handoff-2026-10-11/57-native-door-catalog-and-traversal.md',nativeSourceVerified=True,worldSynchronized=True,worldSynchronizedScope='Six eligible Dunwall channels discovered and retained; two doors operated; both sequential operator orders; physical traversal of original door on716. Explicit isolated fixture only.',complete=False,latest=latest,next=nextWork);s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
a=rd(N/'review-evidence-annotations.json');item=a['items']['door_open_close_locked'];item.update(complete=False,runtime_binding='native_ordinary_door_catalog_and_sequential_mixed_use',notes=latest);item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[t+'/result.json' for t in takes]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',a)
p=W/'candidate-door-catalog-v728/identity.json';v=rd(p);v.update(runtimeVerified=True,runtimeAudit='../native-door-catalog-audit-v735.json',runtimeScope='Six ordinary Dunwall channels; two actually operated doors; both source roles and both sequential operator orders. Explicit fixture only. Locked/linked/campaign/save-load/conflict cases incomplete.');wr(p,v)
p=W/'candidate-door-catalog-v725/identity.json';v=rd(p);v.update(runtimeVerified=False,runtimeAudit='../native-door-catalog-audit-v735.json',runtimeScope='Rejected726: inactive Open-state byte incorrectly excluded one native closed door. Corrected in728.');wr(p,v)
p=R/'work/handoff-project-bookmarks-current.json';b={(x['address'],x['category']):x for x in rd(p)};b.update({(x['address'],x['category']):x for x in rd(R/'work/handoff-project-bookmarks-door-v735.json')});wr(p,sorted(b.values(),key=lambda x:(x['address'],x['category'])))
p=D/'README.md';t=p.read_text();link='- [Native door catalogue, traversal and mixed-player use](57-native-door-catalog-and-traversal.md)'
if link not in t:p.write_text(t.rstrip()+'\n\n'+link+'\n')
shutil.copy2(R/'outputs/d2-coop-lab/research/native-sword-setup-latest.json',W/'native-sword-setup-v735.json')
print(json.dumps(dict(cases=len(cases),retainedFailures=len(failed),newVideos=len(indexed),allGameVideos=len(videos),candidate='candidate-door-catalog-v728',complete=False)))
