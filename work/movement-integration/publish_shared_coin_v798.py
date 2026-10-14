from pathlib import Path
import json,hashlib,datetime,subprocess,shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();A=W/'candidate-coin-sync-v794';pin=rd(A/'identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==pin['dll']
for p in (R/'outputs/d2-coop-lab/src').iterdir():
    if p.is_file():assert (A/'src'/p.name).exists() and sha(p)==sha(A/'src'/p.name),p
takes=[('shared-coin-host-v793',False),('shared-coin-host-v795',True),('shared-coin-client-v795',True),('shared-coin-simultaneous-v796',True),('shared-coin-menu-v796',True),('shared-coin-recovery-v797',False),('shared-coin-recovery-v797b',True)]
checks=[]
for take,passed in takes:
    f=N/take;result=rd(f/'result.json');assert result['passed']==passed
    for role in ('host','client'):
        before=rd(f/f'inventory-before-{role}.json')['items'];after=rd(f/f'inventory-restored-{role}.json')['items']
        assert sorted((i['name'],i['quantity']) for i in before)==sorted((i['name'],i['quantity']) for i in after)
        assert rd(f/f'currency-restored-{role}.json')['currency']==rd(f/f'currency-before-{role}.json')['currency']==0
        if not passed:continue
        first=rd(f/f'committed-{role}.json');repeat=rd(f/f'duplicate-{role}.json')
        source=role==('client' if take=='shared-coin-client-v795' else 'host') or 'simultaneous' in take
        assert first['enabled'] and first['awarded'] and first['revision']==1 and first['commits']==1 and first['ack']==1
        assert first['before']==0 and first['after']==1 and first['nativeSource']==source
        assert first['nativeCalls']==(0 if source else 5) and first['observations']==(1 if source else 0)
        assert repeat['nativeCalls']==first['nativeCalls'] and repeat['before']==0 and repeat['after']==1
        assert rd(f/f'currency-after-{role}.json')['currency']==rd(f/f'currency-repeat-{role}.json')['currency']==1
        if 'menu' in take:
            pause=rd(f/f'paused-{role}.json');assert pause['enabled'] and pause['paused'] and pause['revision']==0 and pause['nativeCalls']==0
            assert rd(f/'paused-peer-wallet.json')['currency']==0
        if take.endswith('v797b'):
            prior=rd(f/f'reload-before-{role}.json');restored=rd(f/f'reload-after-{role}.json');fresh=rd(f/f'fresh-{role}.json');stale=rd(f/f'stale-{role}.json')
            assert restored['reset']['loads']==prior['reset']['loads']+1 and restored['reset']['completed']==prior['reset']['completed']+1
            assert restored['reset']['arm']==prior['reset']['arm'] and restored['coin']['rearms']==1 and restored['coin']['reloads']==1
            assert restored['coin']['arm']>prior['coin']['arm'] and restored['coin']['epoch']!=prior['coin']['epoch']
            assert not restored['coin']['attempted'] and not restored['coin']['awarded'] and restored['coin']['nativeCalls']==restored['coin']['revision']==restored['currency']['currency']==0
            assert not stale['awarded'] and stale['nativeCalls']==stale['revision']==0
            assert fresh['enabled'] and fresh['awarded'] and fresh['revision']==fresh['ack']==1 and fresh['commits']==2
            assert fresh['nativeSource']==(role=='client') and fresh['nativeCalls']==(5 if role=='host' else 0)
        checks.append(dict(take=take,role=role,nativeSource=source,committed=first,restored=True))
scope='One exact ordinary Dunwall coin shared over native Steam. Both ordinary source directions, simultaneous native awards, duplicate packets, peer pause/resume, one coordinated fixed-SLOT82 reload with fresh arms, three retired packet refusals and a new ordinary client pickup pass. Full inventory, currency and health baseline restored. Not a general inventory/shared wallet or complete co-op claim.'
audit=dict(at=at,passed=True,candidate=pin['dll'],checks=checks,scope=scope,failedTakes=[t for t,p in takes if not p])
wr(W/'shared-coin-audit-v798.json',audit)
videos=rd(N/'runtime-video-manifest.json');by={v['video']:v for v in videos};indexed=[]
for take,passed in takes:
    f=N/take
    for p in sorted(f.glob('*.mp4')):
        role='client' if 'client' in p.stem else 'host';char='emily' if role=='client' else 'corvo'
        sheet=f/f'{p.stem}-sheet.jpg';assert sheet.exists(),sheet
        # Every contact sheet is inspected before invoking this publisher.
        if take=='shared-coin-host-v793':note='Retained failed observer experiment: ordinary source pickup/HUD on Corvo, no peer reward. Engine-thread-only observer missed the player-update callback; native wallet reads confirm1/0. Not a successful shared-pickup recording.'
        elif take=='shared-coin-recovery-v797':note='Retained failed setup: Corvo loses the close coin vantage and falls below the pickup while nearby NPCs are active. Coin remains and both wallets remain0. No reload was attempted; not a network convergence failure after a native award.'
        elif p.stem.startswith('reload-'):note='Actual shared native checkpoint/loading course. Receipts prove one load per peer, fresh epochs/arms, restored coin and no award from retired packets. This clip is recovery evidence, not movement polish.'
        elif 'simultaneous' in take:note='Both source views show native Coins notifications for overlapping ordinary pickups. Native hooks establish one source award each and zero extra network application calls. Avatar following was temporarily disabled for equivalent isolated camera placement.'
        elif 'menu' in take:note='Native source pickup and peer pause/resume course with native Coins HUD after resume. The sheet alone does not cover every brief menu frame; native state/owned-wallet receipts prove no recipient award while paused. Nearby NPC activity is visible and was not treated as shared AI proof.'
        else:note='Ordinary source coin pickup with native first-person prompt/reward and native Coins HUD on the recipient. Peer camera is farther away: exact entity removal is independently verified by native registry reads. This does not establish remote pickup-animation quality.'
        if take.endswith('v795'):note+=' Tail frames include NPC awareness/combat after the initial harness released its finite fixture during cleanup; complete restored health and inventory are separately checked.'
        if p.stem.startswith('fresh-'):note+=' Fresh Emily pickup after coordinated reload, without rearming the coin/checkpoint sessions.'
        review=f/f'{p.stem}-review-v798.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet.name],observations=[note]))
        stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(p)],text=True))['streams'][0]
        rel=p.relative_to(N).as_posix();indexed.append(rel);dll=rd(W/('candidate-coin-sync-v792' if take.endswith('v793') else 'candidate-coin-sync-v794')/'identity.json')['dll']
        v=dict(video=rel,kind='game',character=char,label=f'{char.title()} · shared coin · {take} · {p.stem}',alias='pickup_loot_steal',requirements=['pickup_loot_steal'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',binding_audit='../../work/movement-integration/shared-coin-audit-v798.json' if passed else None,visual_review=review.relative_to(N).as_posix(),proxy_sha256=dll,video_sha256=sha(p),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=passed,scope=note)
        if rel in by:by[rel].update(v)
        else:videos.append(v)
wr(N/'runtime-video-manifest.json',videos)
latest='794–798: first bounded native Steam coin channel passes ordinary Corvo and Emily pickups, simultaneous native awards, duplicate packets, pause/resume and coordinated fixed-SLOT82 reload. Both new coin bindings reject retired packets, then a fresh ordinary Emily pickup rewards each wallet once. Worker-thread source observation uses an immutable lease and bounded queue. All test inventories/currency/health restored; actual paired videos retained, including failed793 observer and797 fixture takes. Exact one authored coin only; movement and full shared-world work remain incomplete.'
nextWork='Expand to a bounded authored coin catalog with multiple values and independent host orders; test disconnect/reconnect and death during pickup. Continue generic interactions/progression, world/AI, remaining movement/VFX, native session UI and separate-account Steam validation.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['SharedPickupContinuation']=dict(at=at,candidate=A.name,receipt='work/movement-integration/shared-coin-audit-v798.json',details='documentation/handoff-2026-10-11/64-native-steam-shared-coin.md',runtimeVerified=True,sharedCoinVerified=True,sharedWorldVerified=False,restorationVerified=True,complete=False,latest=latest,next=nextWork,scope=scope)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['pickup_loot_steal'];item['complete']=False
if latest not in item.get('notes',''):item['notes']=item.get('notes','')+' '+latest
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{t}/result.json' for t,p in takes if p]));item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+indexed));wr(N/'review-evidence-annotations.json',ann)
pin.update(runtimeVerified=True,runtimeAudit='../shared-coin-audit-v798.json',runtimeScope=scope);wr(A/'identity.json',pin)
for name in ('Test-SharedCoin-v795.ps1','Test-SharedCoinEdge-v796.ps1','Test-SharedCoinRecovery-v797.ps1'):shutil.copy2(N/name,A/name)
bms=rd(R/'work/handoff-project-bookmarks-coin-v798.json');bms=json.loads(bms['structuredContent']['result'])['bookmarks']
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;data=rd(p);keys={(b['address'],b['category']) for b in bms};data=[b for b in data if (b['address'],b['category']) not in keys]+bms;wr(p,sorted(data,key=lambda b:(b['address'],b['category'])))
p=D/'README.md';s=p.read_text();line='- [Native Steam shared coin, callback threads and checkpoint receipts](64-native-steam-shared-coin.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
p=D/'64-native-steam-shared-coin.md';s=p.read_text();s=s.replace('Shared checkpoint/replay test797 is in progress; do not infer its result from the implementation above.', '[Shared recovery797b](../../work/natural-moveset/shared-coin-recovery-v797b/result.json): host pickup, duplicate replay, one actual load per player, automatic fresh coin binding with new epochs/arms and empty receipts, three old-world commit rejections, then a fresh ordinary Emily pickup without rearming either session. Both wallets0->1 once in the new world; original roles/counterparts and all test quantities are restored. [Audit798](../../work/movement-integration/shared-coin-audit-v798.json) cross-checks the saved receipts. Initial797 never reached reload: the source fell below the coin before its pickup, wallets remained0, and the coin remained present. Native NPC activity is visible; the harness had unnecessarily released/reacquired its finite NPC fixture.797b retains a still-valid fixture and records/validates its readiness before source input. The exact cause of the source displacement in797 was not captured as a native instruction trace.');p.write_text(s)
print(json.dumps(dict(passed=True,newVideos=len(indexed),allVideos=len(videos),candidate=pin['dll'],next=nextWork)))
