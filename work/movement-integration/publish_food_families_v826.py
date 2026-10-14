from pathlib import Path
import json, hashlib, datetime, subprocess, shutil

R = Path(__file__).resolve().parents[2]
N, W, D = R/'work/natural-moveset', R/'work/movement-integration', R/'documentation/handoff-2026-10-11'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
wr = lambda p,v: p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f: return hashlib.file_digest(f,'sha256').hexdigest().upper()
def quantities(p): return sorted((i['name'],i['quantity']) for i in rd(p)['items'])

at = datetime.datetime.now().astimezone().isoformat()
A = W/'candidate-coin-sync-v821'; pin = rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll') == pin['dll']
assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe') == pin['probe']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file(): assert sha(p) == sha(A/'src'/p.name),p

takes = ['shared-food-catalog-batch-v822','shared-food-recovery-v823'] + [f'shared-pickup-{kind}-input-v824' for kind in ('potion','food','ammo')]
restorations=[]
for take in takes:
    f=N/take; result=rd(f/'result.json'); assert result['passed'],result
    for role in ('host','client'):
        assert quantities(f/f'inventory-before-{role}.json') == quantities(f/f'inventory-restored-{role}.json')
        assert rd(f/f'currency-before-{role}.json')['currency'] == rd(f/f'currency-restored-{role}.json')['currency'] == 0
    restorations.append(dict(take=take,passed=True,fullInventoryAndCurrencyRestored=True))

B=N/takes[0]; plan=rd(B/'plan.json'); assert len(plan)==51
families={k:sum(e['family']==k for e in plan) for k in (1,4,6,14)}
assert families == {1:19,4:4,6:27,14:1}
digest='3e76c7c38686d6a2'; checks=[]
for role in ('host','client'):
    done=rd(B/f'committed-{role}.json'); repeat=rd(B/f'duplicate-{role}.json')
    assert done['enabled'] and done['catalogCount']==done['awardedCount']==done['commits']==51 and done['catalogId']==digest
    assert rd(B/f'currency-after-{role}.json')['currency']==rd(B/f'currency-repeat-{role}.json')['currency']==258
    assert quantities(B/f'inventory-after-{role}.json')==quantities(B/f'inventory-repeat-{role}.json')
    assert rd(B/f'food-after-{role}.json')['food']==rd(B/f'food-repeat-{role}.json')['food']
    remaining={e['name'] for e in rd(B/f'entities-after-{role}.json')['entities']}
    for entry in plan:
        e=next(x for x in done['entries'] if x['name']==entry['name']); again=next(x for x in repeat['entries'] if x['name']==e['name']); source=entry['role']==role
        assert e==again and e['awarded'] and e['ack']==e['revision']==1 and e['nativeSource']==source
        assert e['observations']==int(source) and e['nativeCalls']==(0 if source else 5) and e['exerciseCalls']==(5 if source else 0)
        assert e['name'] not in remaining
        if e['family']==6:
            before=e['foodBefore']; after=e['foodAfter']; expected=min(max(before['health'],before['healthTarget'])+before['healthGrant'],before['healthMaximum'])
            assert abs(max(after['health'],after['healthTarget'])-expected)<.01
            assert after['mana']==before['mana']==100 and after['stored']==before['stored']==0
        else: assert e['accepted']==e['after']-e['before']>0
        checks.append(dict(role=role,receipt=e))
    heal=rd(B/f'healing-audit-{role}.json'); assert (heal['foodCount'],heal['healing'],heal['fullHealthNoOps'])==(27,14,24)
    actual=dict(quantities(B/f'inventory-after-{role}.json')); assert actual['components/equipment/ammo/bullet.inventoryitem']==6 and actual['components/interactive/health_potion.inventoryitem']==1
    f=N/takes[1]; before=rd(f/f'reload-before-{role}.json'); after=rd(f/f'reload-after-{role}.json'); stale=rd(f/f'stale-{role}.json')
    assert after['coin']['catalogCount']==51 and after['coin']['catalogId']==digest
    assert after['reset']['loads']==before['reset']['loads']+1 and after['reset']['completed']==before['reset']['completed']+1
    assert after['coin']['epoch']!=before['coin']['epoch'] and after['coin']['rearms']==1
    assert after['coin']['awardedCount']==after['coin']['nativeCalls']==stale['awardedCount']==stale['nativeCalls']==0
    assert rd(f/f'committed-food-{role}.json')['food']['health']==(70 if role=='host' else 61)
    assert rd(f/f'duplicate-food-{role}.json')['food']['health']==(70 if role=='host' else 61)
    assert rd(f/f'fresh-food-{role}.json')['food']['health']==61

ordinary=[]
for kind in ('potion','food','ammo'):
    f=N/f'shared-pickup-{kind}-input-v824'; result=rd(f/'result.json')
    for row in result['cases']:
        e=row['receipt']; assert e['awarded'] and e['ack']==1 and e['exerciseCalls']==0
        assert e['observations']==int(e['nativeSource']) and e['nativeCalls']==(0 if e['nativeSource'] else 5)
        assert rd(f/f'food-after-{row["role"]}.json')['food']['health']==(61 if kind=='food' else 56)
        ordinary.append(dict(family=kind,role=row['role'],receipt=e))
V=N/'pickup-catalog-restoration-v825'; assert rd(V/'result.json')['passed'] and rd(V/'runtime.json')['passed']
scope='Exact Dunwall51-member catalog:19 currency,4 ammunition,27 health food,1 stored potion. Grouped native API/Steam batch, queued healing/cap, unequal health, duplicate no-ops, checkpoint renewal/stale rejection and representative ordinary ammo/potion/food input pass. Native mana-food restoration is static/unit covered only; negative foods excluded. Not full inventory, campaign, movement or separate-account online certification.'
audit=dict(at=at,passed=True,complete=False,candidate=pin['dll'],probe=pin['probe'],catalogId=digest,families=families,batchChecks=checks,ordinaryInputChecks=ordinary,restorations=restorations,finalValidation=str((V/'runtime.json').relative_to(R)),scope=scope)
wr(W/'food-family-audit-v826.json',audit)

videos=rd(N/'runtime-video-manifest.json'); by={v['video']:v for v in videos}; indexed=[]
for take in takes:
    for p in sorted((N/take).glob('*.mp4')):
        sheet=p.with_name(p.stem+'-sheet.jpg'); assert sheet.exists()
        role='client' if 'client' in p.stem else 'host'; char='emily' if role=='client' else 'corvo'
        if 'v822' in take: note='Inspected fixed native first-person views during51-member API batch; money/ammunition HUD and elixir tutorial appear. No ordinary source input or visible proof of distant removals; native receipts/entity snapshots establish outcomes.'
        elif p.stem.startswith('reload-'): note='Inspected native loading screen and return to Dunwall, with nearby partner and door prompt. One coordinated native load each and fresh51-member catalog are established by native observations. No remote animation approval.'
        elif 'v823' in take: note='Inspected fixed game views for unequal-health or fresh-after-reload native API food exercise. NPC activity is visible. Native vital/receipt evidence establishes healing, not framing or animation quality.'
        elif 'potion' in take: note='Inspected ordinary health-potion input recording: source looks down at pickup, native elixir HUD/tutorial appears in both views. Close native item outcome confirmed by owned inventory/receipt evidence; peer camera is elsewhere.'
        elif 'food' in take: note='Inspected ordinary food input recording: Emily source looks at fruit plate and native food feedback/highlight; host camera remains elsewhere. Native health56->61 and exact entity removal establish shared outcome. No remote pickup animation claim.'
        else: note='Inspected ordinary ammunition input recording; native bullet pickup prompt/HUD and nearby table remain visible in source view. Owned inventory/receipts establish one grant per player and exact world removal; peer view does not frame the item.'
        review=p.with_name(p.stem+'-review-v826.json'); wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet.name],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix(); indexed.append(rel)
        item=dict(video=rel,kind='game',character=char,label=f'{char.title()} · shared food and pickup families · {take} · {p.stem}',alias='pickup_loot_steal',requirements=['pickup_loot_steal'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',binding_audit='../../work/movement-integration/food-family-audit-v826.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=pin['dll'],video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=True,scope=note)
        if rel in by: by[rel].update(item)
        else: videos.append(item)
wr(N/'runtime-video-manifest.json',videos)
latest='820–826: native food family expands Dunwall catalog to51 objects (27 health food,19 currency,4 ammo,1 stored potion). Batch gives14 health to native cap70,258 money,6 bullets and1 potion each;24 full-health food no-ops retire correctly. Unequal host/client health, duplicate rejection and coordinated checkpoint renewal pass. Representative ordinary ammo, stored potion and food inputs now pass. Previous potion setup failures were test-fixture preconditions, not relay failures. Complete native baseline/protected files restored. Movement/playable co-op remain incomplete.'
nextWork='Continue shared containers and quest/progression semantics, shared NPC/world authority, remaining movement/VFX and native session UI/separate-account Steam testing. Group new items by native behavior; no individual handler per consumable.'
state=rd(R/'work/current-task-state.json'); state['UpdatedAt']=at
state['SharedPickupContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/food-family-audit-v826.json',details='documentation/handoff-2026-10-11/67-food-and-grouped-pickup-validation.md',runtimeVerified=True,currencyCatalogVerified=True,inventoryStackApiVerified=True,stackCapacityVerified=True,ordinaryStackInputVerified=True,ordinaryFoodInputVerified=True,healthFoodVerified=True,manaFoodRuntimeVerified=False,sharedWorldVerified=False,restorationVerified=True,complete=False,latest=latest,next=nextWork,scope=scope)
state['MovementExecutionPriority']['current']=latest; wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json'); item=ann['items']['pickup_loot_steal']; item['complete']=False
if latest not in item.get('notes',''): item['notes']=item.get('notes','')+' '+latest
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{t}/result.json' for t in takes]+['pickup-catalog-restoration-v825/result.json']))
item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed)); wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../food-family-audit-v826.json',runtimeScope=scope); wr(A/'identity.json',pin)
for name in ('Test-FoodCatalogBatch-v822.ps1','Test-FoodRecovery-v823.ps1','Test-OrdinaryFamilies-v824.ps1','Test-PickupCatalogRestoration-v825.ps1'): shutil.copy2(N/name,A/name)
bms=json.loads(rd(R/'work/handoff-project-bookmarks-food-v824.json')['structuredContent']['result'])['bookmarks']; keys={(b['address'],b['category']) for b in bms}
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name; data=[b for b in rd(p) if (b['address'],b['category']) not in keys]+bms; wr(p,sorted(data,key=lambda b:(b['address'],b['category'])))
p=D/'README.md'; s=p.read_text(); line='- [Food family and ordinary pickup validation](67-food-and-grouped-pickup-validation.md)'
if line not in s: p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),catalog=51,candidate=pin['dll'],ordinaryFamilies=['ammo','potion','food'])))
