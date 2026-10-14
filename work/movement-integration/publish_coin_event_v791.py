from pathlib import Path
import json, hashlib, datetime, subprocess, shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-coin-fixture-v790';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert (A/'src'/p.name).exists() and sha(p)==sha(A/'src'/p.name),p
baseline=rd(N/'native-coin-reward-v786c/result.json');result=rd(N/'native-coin-api-v790b/result.json')
assert baseline['passed'] and result['passed'] and {x['role'] for x in result['cases']}=={'host','client'}
checks=[]
for role in ('host','client'):
    f=N/'native-coin-api-v790b';scope=rd(f/f'native-scope-refusal-{role}.json');first=rd(f/f'native-first-{role}.json');repeat=rd(f/f'native-repeat-{role}.json')
    assert scope['result']=='native_scope_required' and first['nativeCalls']==scope['nativeCalls']+5
    assert first['result']=='native_coin_reward_event_and_removal' and first['before']==0 and first['after']==1 and first['flags']==8
    assert repeat['result']=='already_collected' and repeat['nativeCalls']==first['nativeCalls'] and repeat['noops']==first['noops']+1
    assert rd(f/f'currency-before-{role}.json')['currency']==0 and rd(f/f'currency-after-{role}.json')['currency']==1 and rd(f/f'currency-repeat-{role}.json')['currency']==1 and rd(f/f'currency-restored-{role}.json')['currency']==0
    before=rd(f/f'inventory-before-{role}.json')['items'];after=rd(f/f'inventory-restored-{role}.json')['items'];assert sorted((i['name'],i['quantity']) for i in before)==sorted((i['name'],i['quantity']) for i in after)
    checks.append(dict(role=role,passed=True,first=first,duplicate=repeat,inventoryRestored=True,currencyRestored=True))
audit=dict(at=at,passed=True,candidate=pin['dll'],roles=checks,scope='Exact one-coin direct native API feasibility in both roles, with local native UI/reward/removal, duplicate refusal and restored isolated baseline. Includes the native pickedUp event for this exact coin. No network pickup relay, arbitrary mission-effect coverage, shared wallet, or full co-op completion.')
wr(W/'native-coin-audit-v791.json',audit)
videos=rd(N/'runtime-video-manifest.json');by={v['video']:v for v in videos};indexed=[]
takes=[('native-coin-api-v790b',True,True)]
for take,passed,api in takes:
    f=N/take
    for role in ('host','client'):
        p=f/f'{role}.mp4'
        if not p.exists():continue
        sheet=f/f'{role}-sheet.jpg';assert sheet.exists()
        # These sheets are inspected by the agent before invoking this publisher.
        observations=('Native coin and pickup prompt disappear while the nearby glass remains. No normal source pickup action was requested; direct API delta is in the native receipts.' if api and (passed or (take.endswith('v788') and role=='host')) else 'Normal source coin pickup, native first-person context and object removal. Repeated X can select the adjacent whisky glass; no repeated coin credit.' if passed else 'Retained failed experiment. Consult result.json: wrong glass selected in786, zero-call flag refusal in787, or peer inventory diagnostic failure before Emily application in788.')
        if role=='client':observations='Native Coins notification is visible, but the camera points at the ceiling and does not show the coin disappearing. Removal and money delta are native-reader assertions; this video is HUD evidence only. Earlier788b client footage shows the three-call baseline coin removal.'
        note=observations+' Functional native reward research only; no new remote animation quality or shared progression acceptance.'
        review=f/f'{role}-review-v791.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet.name],observations=[observations]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel);char='corvo' if role=='host' else 'emily'
        dll=pin['dll']
        v=dict(video=rel,kind='game',character=char,label=f'{char.title()} · native coin reward · {take}',alias='pickup_loot_steal',requirements=['pickup_loot_steal'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',binding_audit='../../work/movement-integration/native-coin-audit-v791.json' if passed and api else None,visual_review=review.relative_to(N).as_posix(),proxy_sha256=dll,video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=passed,scope=note)
        if rel in by:by[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='790/791: exact native coin fixture now includes pickedUp(true,false) entity/script event and variant cleanup, matching the decompiled native ordering before removal. Both roles pass currency0->1, five direct calls, duplicate no-op, outside-fixture refusal, and checkpoint/full inventory/health/coin restoration. Original788 feasibility and failed attempts are preserved. Native UI money notifications recorded in both views; coin removal visible for Corvo, while Emily camera points away and removal is established by native reads. No shared Steam pickup channel yet.'
nextWork='Implement bounded reliable native Steam coin synchronization using authored identity, host ordering, per-world exactly-once receipts, simultaneous claims, duplicate/delayed packets and checkpoint retirement. Preserve native pickedUp event. Continue world/AI, remaining movement/VFX, native session UI and separate-account validation.'

state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['SharedPickupContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/native-coin-audit-v791.json',details='documentation/handoff-2026-10-11/63-native-coin-pickup-and-reward.md',runtimeVerified=True,sharedWorldVerified=False,restorationVerified=True,complete=False,latest=latest,next=nextWork,scope=audit['scope'])
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['pickup_loot_steal'];item['complete']=False;item['notes']=item.get('notes','')+' '+latest if latest not in item.get('notes','') else item['notes'];item['evidence']=list(dict.fromkeys(item.get('evidence',[])+['native-coin-api-v790b/result.json']));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../native-coin-audit-v791.json',runtimeScope=audit['scope']);wr(A/'identity.json',pin)
for name in ('Test-NativeCoinAPI-v790.ps1','Test-NativeCoinReward-v786.ps1'):shutil.copy2(N/name,A/name)
p=D/'63-native-coin-pickup-and-reward.md';s=p.read_text();s=s.replace('Build and runtime790 are pending; no new Steam packet is implemented.', '[Runtime790](../../work/natural-moveset/native-coin-api-v790b/result.json) and [audit791](../../work/movement-integration/native-coin-audit-v791.json) pass both roles: money0->1, five direct native calls including pickedUp and return cleanup, one native removal, duplicate without new calls/credit, and outside-fixture refusal. Both first-person recordings were inspected and show the native coin HUD. Corvo shows removal; Emily points at the ceiling, so her clip does not visually establish removal. Her currency/removal checks are native-reader evidence, and788b remains the earlier Emily visual baseline. Complete inventory quantities, money0, health56/40 and the exact coin restore after cleanup;14 protected files match. This validates return through the native event call for this coin, not all quest-script effects. No new Steam packet is implemented. Current DLL `'+pin['dll']+'`; prior788 remains preserved.');p.write_text(s)

p=D/'README.md';s=p.read_text();line='- [Native coin lifecycle, currency and guarded reward experiment](63-native-coin-pickup-and-reward.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),candidate=pin['dll'],next=nextWork)))
