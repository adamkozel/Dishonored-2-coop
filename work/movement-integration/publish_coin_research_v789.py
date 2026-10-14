from pathlib import Path
import json, hashlib, datetime, subprocess, shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-coin-fixture-v788';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert (A/'src'/p.name).exists() and sha(p)==sha(A/'src'/p.name),p
baseline=rd(N/'native-coin-reward-v786c/result.json');result=rd(N/'native-coin-api-v788b/result.json')
assert baseline['passed'] and result['passed'] and {x['role'] for x in result['cases']}=={'host','client'}
checks=[]
for role in ('host','client'):
    f=N/'native-coin-api-v788b';scope=rd(f/f'native-scope-refusal-{role}.json');first=rd(f/f'native-first-{role}.json');repeat=rd(f/f'native-repeat-{role}.json')
    assert scope['result']=='native_scope_required' and first['nativeCalls']==scope['nativeCalls']+3
    assert first['result']=='native_coin_reward_and_removal' and first['before']==0 and first['after']==1 and first['flags']==8
    assert repeat['result']=='already_collected' and repeat['nativeCalls']==first['nativeCalls'] and repeat['noops']==first['noops']+1
    assert rd(f/f'currency-before-{role}.json')['currency']==0 and rd(f/f'currency-after-{role}.json')['currency']==1 and rd(f/f'currency-repeat-{role}.json')['currency']==1 and rd(f/f'currency-restored-{role}.json')['currency']==0
    before=rd(f/f'inventory-before-{role}.json')['items'];after=rd(f/f'inventory-restored-{role}.json')['items'];assert sorted((i['name'],i['quantity']) for i in before)==sorted((i['name'],i['quantity']) for i in after)
    checks.append(dict(role=role,passed=True,first=first,duplicate=repeat,inventoryRestored=True,currencyRestored=True))
audit=dict(at=at,passed=True,candidate=pin['dll'],roles=checks,scope='Exact one-coin direct native API feasibility in both roles, with local native UI/reward/removal, duplicate refusal and restored isolated baseline. No network pickup relay, generic mission event, shared wallet, or full co-op completion.')
wr(W/'native-coin-audit-v789.json',audit)
videos=rd(N/'runtime-video-manifest.json');by={v['video']:v for v in videos};indexed=[]
takes=[('native-coin-reward-v786',False,False),('native-coin-reward-v786c',True,False),('native-coin-api-v787',False,True),('native-coin-api-v788',False,True),('native-coin-api-v788b',True,True)]
for take,passed,api in takes:
    f=N/take
    for role in ('host','client'):
        p=f/f'{role}.mp4'
        if not p.exists():continue
        sheet=f/f'{role}-sheet.jpg';assert sheet.exists()
        # These sheets are inspected by the agent before invoking this publisher.
        observations=('Native coin and pickup prompt disappear while the nearby glass remains. No normal source pickup action was requested; direct API delta is in the native receipts.' if api and (passed or (take.endswith('v788') and role=='host')) else 'Normal source coin pickup, native first-person context and object removal. Repeated X can select the adjacent whisky glass; no repeated coin credit.' if passed else 'Retained failed experiment. Consult result.json: wrong glass selected in786, zero-call flag refusal in787, or peer inventory diagnostic failure before Emily application in788.')
        note=observations+' Functional native reward research only; no new remote animation quality or shared progression acceptance.'
        review=f/f'{role}-review-v789.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet.name],observations=[observations]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel);char='corvo' if role=='host' else 'emily'
        dll=rd(W/('candidate-coin-fixture-v788' if 'v788' in take else 'candidate-coin-fixture-v787' if 'v787' in take else 'candidate-death-barrier-v782')/'identity.json')['dll']
        v=dict(video=rel,kind='game',character=char,label=f'{char.title()} · native coin reward · {take}',alias='pickup_loot_steal',requirements=['pickup_loot_steal'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',binding_audit='../../work/movement-integration/native-coin-audit-v789.json' if passed and api else None,visual_review=review.relative_to(N).as_posix(),proxy_sha256=dll,video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=passed,scope=note)
        if rel in by:by[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='785–789: exact native coin lifecycle and currency path traced in Ghidra. Ordinary Corvo/Emily pickup adds one coin, repeated input does not recredit. Guarded direct claim/reward/removal passes both roles with three native calls and duplicate no-op; checkpoint restores currency0, full quantities, health and coin.787 retirement-bit refusal and788 inventory-reader failure retained. No shared pickup relay yet; extra native entity event and reliable host ordering remain.'
nextWork='Trace/reproduce the native two-argument pickup event, then implement a bounded reliable Steam coin channel with host ordering, same-world exactly-once receipts, simultaneous claims and checkpoint retirement. Continue remaining movement/VFX and world/AI/session requirements; keep animation polish deferred.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['SharedPickupContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/native-coin-audit-v789.json',details='documentation/handoff-2026-10-11/63-native-coin-pickup-and-reward.md',runtimeVerified=True,sharedWorldVerified=False,restorationVerified=True,complete=False,latest=latest,next=nextWork,scope=audit['scope'])
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['pickup_loot_steal'];item['complete']=False;item['notes']=item.get('notes','')+' '+latest if latest not in item.get('notes','') else item['notes'];item['evidence']=list(dict.fromkeys(item.get('evidence',[])+['native-coin-reward-v786c/result.json','native-coin-api-v788b/result.json']));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../native-coin-audit-v789.json',runtimeScope=audit['scope']);wr(A/'identity.json',pin)
for name in ('Test-NativeCoinAPI-v788.ps1','Test-NativeCoinReward-v786.ps1'):shutil.copy2(N/name,A/name)
p=D/'63-native-coin-pickup-and-reward.md';s=p.read_text();s=s.replace('Runtime788: pending. Do not infer successful application from build success.', '[Successful take788b](../../work/natural-moveset/native-coin-api-v788b/result.json) and [independent audit789](../../work/movement-integration/native-coin-audit-v789.json) pass both roles: three native calls, money0->1, entity retirement, duplicate with no new calls/credit, outside-fixture zero-call refusal, and full baseline restoration. The epoch receipt permits the restored coin after a later checkpoint. Earlier take788 passed Corvo but stopped before Emily application when the peer full-inventory reader refused; that reader also requires a stable sword attachment. The replacement currency observer validates current player/Inventory ownership directly. The exact cause of that transient sword-dependent refusal was not captured, and is not asserted as a game bug. Both successful source recordings and all retained failed recordings were inspected; nine videos are added to the review dashboard.\n\nLive DLL: `'+pin['dll']+'`; probe: `'+pin['probe']+'`. Sources match archived788. No transport packet or Steam pickup relay is added by this candidate.');p.write_text(s)
p=D/'README.md';s=p.read_text();line='- [Native coin lifecycle, currency and guarded reward experiment](63-native-coin-pickup-and-reward.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),candidate=pin['dll'],next=nextWork)))
