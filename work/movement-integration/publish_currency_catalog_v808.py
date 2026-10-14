from pathlib import Path
import json, hashlib, datetime, subprocess, shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-coin-sync-v804';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
takes=[('shared-coin-catalog-v801',False),('shared-coin-catalog-v801b',False),('shared-currency-catalog-v801c',False),('shared-currency-catalog-v801d',False),('shared-currency-recovery-v802',True),('shared-currency-batch-v805',True),('shared-currency-simultaneous-v806',True)]
restorations=[]
for take,passed in takes:
    f=N/take;assert rd(f/'result.json')['passed']==passed
    for role in ('host','client'):
        before=rd(f/f'inventory-before-{role}.json')['items'];after=rd(f/f'inventory-restored-{role}.json')['items']
        assert sorted((i['name'],i['quantity']) for i in before)==sorted((i['name'],i['quantity']) for i in after)
        assert rd(f/f'currency-before-{role}.json')['currency']==rd(f/f'currency-restored-{role}.json')['currency']==0
    restorations.append(dict(take=take,resultPassed=passed,fullInventoryAndCurrencyRestored=True))
B=N/'shared-currency-batch-v805';plan=rd(B/'plan.json');assert len(plan)==19 and sum(e['quantity'] for e in plan)==258
checks=[]
for role in ('host','client'):
    done=rd(B/f'committed-{role}.json');repeat=rd(B/f'duplicate-{role}.json');armed=rd(B/f'armed-{role}.json')
    assert done['enabled'] and done['catalogCount']==done['awardedCount']==done['commits']==19 and done['catalogId']==armed['catalogId']=='5fd266cc323a86fa'
    assert rd(B/f'currency-after-{role}.json')['currency']==rd(B/f'currency-repeat-{role}.json')['currency']==258
    remaining={e['name'] for e in rd(B/f'entities-after-{role}.json')['entities']}
    for e in plan:
        receipt=next(x for x in done['entries'] if x['name']==e['name']);r=next(x for x in repeat['entries'] if x['name']==e['name']);source=e['role']==role
        assert receipt['awarded'] and receipt['ack']==receipt['revision']==1 and receipt['nativeSource']==source
        assert receipt['after']-receipt['before']==e['quantity'] and receipt['observations']==int(source)
        assert receipt['nativeCalls']==(0 if source else 5) and receipt['exerciseCalls']==(5 if source else 0)
        assert receipt==r and e['name'] not in remaining
        checks.append(dict(role=role,name=e['name'],quantity=e['quantity'],sourceExercise=source,receipt=receipt))
    f=N/'shared-currency-recovery-v802';prior=rd(f/f'reload-before-{role}.json');renew=rd(f/f'reload-after-{role}.json');fresh=rd(f/f'fresh-{role}.json');stale=rd(f/f'stale-{role}.json')
    assert renew['coin']['catalogCount']==19 and renew['coin']['catalogId']==armed['catalogId']
    assert renew['reset']['loads']==prior['reset']['loads']+1 and renew['reset']['completed']==prior['reset']['completed']+1
    assert renew['coin']['rearms']==1 and renew['coin']['arm']>prior['coin']['arm'] and renew['coin']['epoch']!=prior['coin']['epoch']
    assert renew['coin']['awardedCount']==renew['coin']['nativeCalls']==renew['currency']['currency']==0
    assert stale['awardedCount']==stale['nativeCalls']==0
    assert fresh['enabled'] and fresh['commits']==2 and fresh['awardedCount']==1 and fresh['nativeSource']==(role=='client')
    f=N/'shared-currency-simultaneous-v806';s=rd(f/f'committed-{role}.json');assert s['enabled'] and s['nativeSource'] and s['awarded'] and s['ack']==s['revision']==s['observations']==s['commits']==1 and s['nativeCalls']==0
    assert rd(f/f'currency-after-{role}.json')['currency']==rd(f/f'currency-repeat-{role}.json')['currency']==1
V=N/'currency-catalog-restoration-v807';assert rd(V/'result.json')['passed'] and rd(V/'runtime.json')['passed']
for role in ('host','client'):
    for prefix in ('catalog-restored','outside-fixture'):
        s=rd(V/f'{prefix}-{role}.json');assert s['enabled'] and s['catalogCount']==19 and s['catalogId']=='5fd266cc323a86fa' and s['awardedCount']==s['nativeCalls']==s['commits']==0
        assert all(not e['attempted'] and e['exerciseCalls']==0 for e in s['entries'])
scope='Bounded Dunwall type1 currency catalog:19 authored objects,6 values,258 total per wallet. Entire native API source/Steam recipient batch, removal and duplicate rejection pass. Representative ordinary pickups, simultaneous ordinary same-coin awards and catalog checkpoint/stale replay pass. All19 members, inventories, currency and health restored. Not generic inventory, container sharing or full ordinary reachability.'
audit=dict(at=at,passed=True,candidate=pin['dll'],catalogId='5fd266cc323a86fa',entries=19,values=sorted({e['quantity'] for e in plan}),deltaEach=258,checks=checks,restorations=restorations,finalValidation=str((V/'runtime.json').relative_to(R)),scope=scope)
wr(W/'currency-catalog-audit-v808.json',audit)
videos=rd(N/'runtime-video-manifest.json');by={v['video']:v for v in videos};indexed=[]
for take,passed in takes:
    f=N/take
    for p in sorted(f.glob('*.mp4')):
        role='client' if 'client' in p.stem else 'host';char='emily' if role=='client' else 'corvo';sheet=f/f'{p.stem}-sheet.jpg';assert sheet.exists()
        if 'v805' in take:note='Fixed first-person view during developer native API batch of19 currency pickups. Native money HUD appears. No ordinary input or visible remote pickup sequence is claimed; native readers establish individual retirement and sums. This is functional API/Steam evidence, not animation approval.'
        elif 'v806' in take:note='Both ordinary source views show native Coin HUD; post-pickup whisky glass is highlighted. Native observer receipts confirm one local award per player and zero extra recipient calls. Avatar following temporarily disabled for equivalent close camera placement. Nearby NPC activity is visible; this is not shared AI proof.'
        elif 'reload-' in p.stem:note='Actual shared native loading and return to Dunwall. Receipts verify one load per game, rebuilt19-entry catalog, fresh receipts and old-packet rejection. No movement-quality claim.'
        elif 'v802' in take:note='Ordinary source pickup and native Coin HUD on peer with full currency catalog active. Peer view shows remote avatar on furniture; close object removal is validated by native registry, not peer framing. Nearby NPC activity remains visible. This is not remote animation approval.'
        elif 'v801b' in take:note='Retained unsuccessful mixed-value attempt. Original coin converges, then client approaches closed cupboard. Temporary NPC fixture releases on native state change before the second source input. No second award; cleanup passed.'
        else:note='Retained unsuccessful mixed-value approach. Original coin converges; client camera is obstructed by room geometry near pouch, with no pouch native award. No network application failure observed. Cleanup passed; not successful concurrency evidence.'
        if p.stem.startswith('fresh-'):note+=' New Emily pickup after coordinated reload without rearming the catalog.'
        review=f/f'{p.stem}-review-v808.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet.name],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel);version=804 if ('v805' in take or 'v806' in take) else 800
        v=dict(video=rel,kind='game',character=char,label=f'{char.title()} · currency catalog · {take} · {p.stem}',alias='pickup_loot_steal',requirements=['pickup_loot_steal'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',binding_audit='../../work/movement-integration/currency-catalog-audit-v808.json' if passed else None,visual_review=review.relative_to(N).as_posix(),proxy_sha256=rd(W/f'candidate-coin-sync-v{version}'/'identity.json')['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=passed,scope=note)
        if rel in by:by[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='800–808: one bounded native currency catalog replaces the single-item binding. Both games match19 ordinary type1 objects across6 values. Native API batch alternates sources and verifies all19 awards/removals (258 each), individual receipts and duplicate no-ops. Ordinary same-coin simultaneous pickup gives1 each with0 extra recipient calls. Catalog shared checkpoint/rebind/stale packets and fresh Emily pickup pass. Full catalog/inventory/currency/health baseline restored; outside-fixture exercise makes0 calls. Failed ordinary pouch/cupboard approaches retained. Functional family coverage; movement and playable co-op remain incomplete.'
nextWork='Reuse catalog/session/receipt logic for inventory-stack/ammunition and restorative families with capacity and actual outcome checks; finish consumption policy, containers and quest interactions. Continue shared world/AI, remaining movement/VFX, native session UI and separate-account Steam validation.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['SharedPickupContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/currency-catalog-audit-v808.json',details='documentation/handoff-2026-10-11/65-currency-catalog-and-pickup-families.md',runtimeVerified=True,sharedCoinVerified=True,currencyCatalogVerified=True,sharedWorldVerified=False,restorationVerified=True,complete=False,latest=latest,next=nextWork,scope=scope)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['pickup_loot_steal'];item['complete']=False
if latest not in item.get('notes',''):item['notes']=item.get('notes','')+' '+latest
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{t}/result.json' for t,p in takes if p]+['currency-catalog-restoration-v807/result.json']));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../currency-catalog-audit-v808.json',runtimeScope=scope);wr(A/'identity.json',pin)
old=rd(W/'candidate-coin-sync-v800/identity.json');old.update(runtimeVerified=True,runtimeAudit='../currency-catalog-audit-v808.json',runtimeScope='Matching19-entry catalog with representative ordinary pickup, coordinated checkpoint retirement/rebind, stale replay and fresh Emily source verified802. Whole-catalog native API batch was verified on successor804.');wr(W/'candidate-coin-sync-v800/identity.json',old)
for name in ('Test-CurrencyBatch-v805.ps1','Test-CurrencyCatalogEdge-v806.ps1','Test-CurrencyCatalogRestoration-v807.ps1'):shutil.copy2(N/name,A/name)
bms=json.loads(rd(R/'work/handoff-project-bookmarks-currency-v808.json')['structuredContent']['result'])['bookmarks']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;data=rd(p);keys={(b['address'],b['category']) for b in bms};data=[b for b in data if (b['address'],b['category']) not in keys]+bms;wr(p,sorted(data,key=lambda b:(b['address'],b['category'])))
p=D/'README.md';s=p.read_text();line='- [Currency catalog, native batch validation and pickup families](65-currency-catalog-and-pickup-families.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),catalogEntries=19,deltaEach=258,candidate=pin['dll'])))
