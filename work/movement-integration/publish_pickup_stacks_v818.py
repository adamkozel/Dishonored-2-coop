from pathlib import Path
import json,hashlib,datetime,subprocess,shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-coin-sync-v814';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert sha(p)==sha(A/'src'/p.name),p
takes=[('shared-pickup-stack-batch-v812',True,811),('shared-pickup-recovery-v813',True,811),('shared-pickup-capacity-v815',True,814),('shared-pickup-ammo-input-v816',False,814),('shared-pickup-potion-input-v816b',False,814),('shared-pickup-potion-input-v816c',False,814)]
restorations=[]
for take,passed,version in takes:
    f=N/take;result=rd(f/'result.json');assert result['passed']==passed
    for role in ('host','client'):
        before=rd(f/f'inventory-before-{role}.json')['items'];after=rd(f/f'inventory-restored-{role}.json')['items']
        assert sorted((x['name'],x['quantity']) for x in before)==sorted((x['name'],x['quantity']) for x in after)
        assert rd(f/f'currency-before-{role}.json')['currency']==rd(f/f'currency-restored-{role}.json')['currency']==0
    restorations.append(dict(take=take,passed=passed,restored=True,failure=result.get('failure')))
B=N/'shared-pickup-stack-batch-v812';plan=rd(B/'plan.json');assert len(plan)==24
assert {f:sum(x['family']==f for x in plan) for f in (1,4,14)}=={1:19,4:4,14:1}
checks=[]
for role in ('host','client'):
    done=rd(B/f'committed-{role}.json');repeat=rd(B/f'duplicate-{role}.json');armed=rd(B/f'armed-{role}.json')
    assert done['enabled'] and done['catalogCount']==done['awardedCount']==done['commits']==24
    assert done['catalogId']==armed['catalogId']=='1df4dd38b1c62203'
    assert rd(B/f'currency-after-{role}.json')['currency']==rd(B/f'currency-repeat-{role}.json')['currency']==258
    remaining={e['name'] for e in rd(B/f'entities-after-{role}.json')['entities']}
    for e in plan:
        receipt=next(x for x in done['entries'] if x['name']==e['name']);again=next(x for x in repeat['entries'] if x['name']==e['name']);source=e['role']==role
        assert receipt['awarded'] and receipt['ack']==receipt['revision']==1 and receipt['nativeSource']==source
        assert receipt['family']==e['family'] and receipt['accepted']==receipt['after']-receipt['before']>0
        if e['family']==1:assert receipt['accepted']==e['quantity']
        else:assert receipt['after']<=receipt['maximum']==10
        assert receipt['observations']==int(source) and receipt['nativeCalls']==(0 if source else 5) and receipt['exerciseCalls']==(5 if source else 0)
        assert receipt==again and e['name'] not in remaining
        checks.append(dict(role=role,receipt=receipt))
    actual={i['name']:i['quantity'] for i in rd(B/f'inventory-after-{role}.json')['items']}
    assert actual['components/equipment/ammo/bullet.inventoryitem']==6 and actual['components/interactive/health_potion.inventoryitem']==1
    assert rd(B/f'health-after-{role}.json')['current']==56
    f=N/'shared-pickup-recovery-v813';prior=rd(f/f'reload-before-{role}.json');renew=rd(f/f'reload-after-{role}.json');fresh=rd(f/f'fresh-{role}.json');stale=rd(f/f'stale-{role}.json')
    assert renew['coin']['catalogCount']==24 and renew['coin']['catalogId']==armed['catalogId']
    assert renew['reset']['loads']==prior['reset']['loads']+1 and renew['reset']['completed']==prior['reset']['completed']+1
    assert renew['coin']['rearms']==1 and renew['coin']['arm']>prior['coin']['arm'] and renew['coin']['epoch']!=prior['coin']['epoch']
    assert renew['coin']['awardedCount']==renew['coin']['nativeCalls']==renew['currency']['currency']==0
    assert stale['awardedCount']==stale['nativeCalls']==0
    assert fresh['enabled'] and fresh['awardedCount']==1 and fresh['nativeSource']==(role=='client')
capacity=[];C=N/'shared-pickup-capacity-v815'
for i,e in enumerate(rd(C/'plan.json')):
    for role in ('host','client'):
        done=rd(C/f'committed-{i}-{role}.json');repeat=rd(C/f'duplicate-{i}-{role}.json')
        receipt=next(x for x in done['entries'] if x['name']==e['name']);again=next(x for x in repeat['entries'] if x['name']==e['name'])
        assert receipt==again and receipt['awarded'] and receipt['ack'] and receipt['family']==e['family']
        if role==e['fixtureRole']:assert receipt['before']==e['setup'] and receipt['after']==10 and receipt['accepted']==e['recipientDelta']
        else:assert receipt['accepted']>0 and receipt['nativeSource']
        assert e['name'] not in {x['name'] for x in rd(C/f'entities-{i}-{role}.json')['entities']}
        capacity.append(dict(case=i,role=role,receipt=receipt))
    restore=rd(C/f'restored-{i}.json');assert not restore['active'] and restore['result']=='native_quantity_restored'
V=N/'pickup-catalog-restoration-v817';assert rd(V/'result.json')['passed'] and rd(V/'runtime.json')['passed']
for role in ('host','client'):
    for prefix in ('catalog-restored','outside-fixture'):
        s=rd(V/f'{prefix}-{role}.json');assert s['enabled'] and s['catalogCount']==24 and s['catalogId']=='1df4dd38b1c62203' and s['awardedCount']==s['nativeCalls']==s['commits']==0
        assert all(not e['attempted'] and e['exerciseCalls']==0 for e in s['entries'])
    before=rd(V/f'capacity-before-refusal-{role}.json');after=rd(V/f'capacity-after-refusal-{role}.json')
    assert not after['active'] and after['calls']==before['calls'] and after['refused']==before['refused']+1
scope='Bounded Dunwall catalog24:19 currency,4 ammo,1 stored health potion. API batch and native modifier outcomes, partial/full recipient capacities, duplicate no-ops and native object retirement pass. Ordinary coin checkpoint/rebind/stale replay passes; ordinary ammo/potion input still unverified after three setup failures. Baseline and protected files restored; not full inventory/progression/campaign/online certification.'
audit=dict(at=at,passed=True,candidate=pin['dll'],catalogId='1df4dd38b1c62203',entries=24,families={1:19,4:4,14:1},batchChecks=checks,capacityChecks=capacity,restorations=restorations,finalValidation=str((V/'runtime.json').relative_to(R)),scope=scope,complete=False)
wr(W/'pickup-stack-audit-v818.json',audit)
videos=rd(N/'runtime-video-manifest.json');by={v['video']:v for v in videos};indexed=[]
for take,passed,version in takes:
    for p in sorted((N/take).glob('*.mp4')):
        role='client' if 'client' in p.stem else 'host';char='emily' if role=='client' else 'corvo';sheet=p.with_name(p.stem+'-sheet.jpg');assert sheet.exists()
        if 'v812' in take:note='Actual fixed first-person game view during24-item API source batch; native ammunition/money HUD and elixir tutorial appear. No ordinary source input. Native receipts and registry reads establish grants and removals; remote pickup animation and distant removals are not visible proof.'
        elif 'v815' in take:note='Actual fixed first-person view during native capacity matrix. Native pistol-bullet notifications and elixir tutorial appear. Receipts prove partial and zero accepted grants plus object retirement; visual framing does not establish remote animation quality.'
        elif p.stem.startswith('reload-'):note='Actual shared native load screen and return to Dunwall. One native load per game, fresh24-member catalog, empty receipts and stale-action rejection are validated separately.'
        else:note='Ordinary coin input on mixed-family catalog, native Coin HUD and adjacent whisky-glass highlight. Peer avatar stands on furniture and nearby NPC activity is visible. Functional pickup/recovery evidence, not animation or shared AI approval.'
        if p.stem.startswith('fresh-'):note+=' Emily is the ordinary source after the coordinated reload.'
        review=p.with_name(p.stem+'-review-v818.json');wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet.name],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel)
        item=dict(video=rel,kind='game',character=char,label=f'{char.title()} · shared pickup families · {take} · {p.stem}',alias='pickup_loot_steal',requirements=['pickup_loot_steal'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',binding_audit='../../work/movement-integration/pickup-stack-audit-v818.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=rd(W/f'candidate-coin-sync-v{version}'/'identity.json')['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=passed,scope=note)
        if rel in by:by[rel].update(item)
        else:videos.append(item)
wr(N/'runtime-video-manifest.json',videos)
latest='810–818: shared pickup catalog expands by native family to24 objects (19 currency,4 ammunition,1 stored health potion). API source/Steam recipient batch gives258 money,6 bullets and1 potion each, with unchanged health; native box modifier2->3 is preserved. Partial ammo9->10 and full ammo/potion zero-grant recipients retire objects and acknowledge once; duplicate no-ops pass. Mixed-catalog ordinary coin shared-checkpoint/rebind/old-packet tests pass. Three ordinary stack approach/isolation failures are retained; ordinary ammo/potion remains unverified. Complete baseline restored. Movement and playable co-op remain incomplete.'
nextWork='Implement immediate-food restorative outcome family and batch its catalog; resolve bounded ordinary stack input setup. Continue containers/quest progression, shared NPC/world authority, remaining movement/VFX, native session UI and separate-account Steam validation.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['SharedPickupContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/pickup-stack-audit-v818.json',details='documentation/handoff-2026-10-11/66-shared-inventory-stack-families.md',runtimeVerified=True,currencyCatalogVerified=True,inventoryStackApiVerified=True,stackCapacityVerified=True,ordinaryStackInputVerified=False,sharedWorldVerified=False,restorationVerified=True,complete=False,latest=latest,next=nextWork,scope=scope)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['pickup_loot_steal'];item['complete']=False
if latest not in item.get('notes',''):item['notes']=item.get('notes','')+' '+latest
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{t}/result.json' for t,p,v in takes]+['pickup-catalog-restoration-v817/result.json']));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../pickup-stack-audit-v818.json',runtimeScope=scope);wr(A/'identity.json',pin)
for name in ('Test-PickupCapacity-v815.ps1','Test-OrdinaryStack-v816.ps1','Test-PickupCatalogRestoration-v817.ps1'):shutil.copy2(N/name,A/name)
bms=json.loads(rd(R/'work/handoff-project-bookmarks-capacity-v818.json')['structuredContent']['result'])['bookmarks'];keys={(b['address'],b['category']) for b in bms}
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    path=R/'work'/name;data=rd(path);data=[b for b in data if (b['address'],b['category']) not in keys]+bms;wr(path,sorted(data,key=lambda b:(b['address'],b['category'])))
p=D/'README.md';s=p.read_text();line='- [Shared inventory stack families and native capacity](66-shared-inventory-stack-families.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),entries=24,candidate=pin['dll'],ordinaryStackInputVerified=False)))
