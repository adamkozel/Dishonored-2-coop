from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
C={k:rd(W/f'candidate-npc-combat-{k}/identity.json') for k in ('v1059','v1059b','v1059c')}
assert sha(R/'work/asset-game/dinput8.dll')==C['v1059c']['dll']
names={v['dll']:f'candidate-npc-combat-{k}' for k,v in C.items()}
plan=[
 ('coin-census-dunwall-v1059.json',None,'Read-only --coin-catalog census in the Dunwall fixture on 1059: 51 eligible (19 currency, 4 ammunition, 27 food, 1 other), matching chapter 67.'),
 ('mission-pickup-v1060',False,'1059 (map gate removed, cap 512): Edge of the World census 292/292 eligible (124 currency, 75 ammunition, 61 food, 32 other) but digests differ - character worlds hold different script pickups (Corvo-only script ammo vs Emily-only coin/banana). Whole-catalog equality cannot work on missions.'),
 ('mission-pickup-v1061',False,'1059b hello-paired intersection: ~290 per-entry hellos/250 ms do not scale; asymmetric settles (host 251, client 244) never become ready. Census also varies between loads (293/288).'),
 ('mission-pickup-v1062',False,'1059c identity-list intersection works: both peers 287 shared, same digest, ready after one list exchange each; host ordinary pickup (copper wire, 10) converged and removed on both. Client pickup failed only because the harness chose an underwater item on the canal bed (aim).'),
 ('mission-pickup-v1063',False,'1059c: 285 shared, same digest; host pickup converged again; the chosen client "currency" was an NPC-carried pickpocket pouch (pouch_small_02) so X hit the worker Talk prompt - harness item choice.'),
 ('mission-pickup-v1064',True,'1059c: 285-ish shared intersection agreed; ordinary native pickups (stand at the item, native look aim, X) by alternating roles - host copper wire (currency 10), client pouch (currency 20), host pistol bullet (ammunition 1) - each converged with receipts on both peers and removal from both worlds; shared currency 0 -> 30 on both. Dunwall fixture restored.')]
videos=rd(N/'runtime-video-manifest.json');cases=[]
for take,passed,scope in plan:
    if take.endswith('.json'):cases.append(dict(take=take,passed=True,scope=scope,census=rd(N/take)|{'items':'omitted'}));continue
    T=N/take;r=rd(T/'result.json');assert bool(r['passed'])==passed,take;assert rd(T/'restoration.json')['passed'],take
    ident=rd(T/'identity.json');dll=ident['dll']
    armed={q:{k:v for k,v in rd(T/f'armed-{q}.json').items() if k in ('ready','settled','catalogCount','localOnly','peerOnly','sharedDigest','peerDigest','listsSent','listsReceived','result')} for q in ('host','client') if (T/f'armed-{q}.json').exists()}
    inter=rd(T/'census-intersection.json') if (T/'census-intersection.json').exists() else None
    cases.append(dict(take=take,passed=passed,candidate=names.get(dll,dll),scope=scope,failure=r.get('failure'),cases=r.get('cases'),armed=armed,census=inter))
    for f in sorted(T.glob('*.mp4')):
        path=f'{take}/{f.name}';role='host' if '-host' in f.name else 'client'
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=avg_frame_rate','-of','json',str(f)],text=True))['streams'][0];a,b=map(float,meta['avg_frame_rate'].split('/'))
        videos=[x for x in videos if x['video']!=path]
        videos.append(dict(video=path,role=role,character='corvo' if role=='host' else 'emily',label=f'World authority / shared pickups / {take} / {f.name}',travelling=False,gameplay_bound=True,actual_capture_hz=round(a/b,2) if b else None,container_fps=meta['avg_frame_rate'],timing='Native wall-time recording, no speedup',scope=scope,review='Raw native recording; pickup sheets inspected for failed 1062/1063 cases.',candidate=names.get(dll,dll),dll=dll,runtime_test_passed=passed))
at=datetime.datetime.now().astimezone().isoformat()
wr(W/'mission-pickup-audit-v1065.json',dict(at=at,candidate='candidate-npc-combat-v1059c',dll=C['v1059c']['dll'],probe=C['v1059c']['probe'],coinProtocol=7,coinCatalogListProtocol=1,actionProtocol=73,
  harnesses=['../natural-moveset/Test-MissionPickup-v1060.ps1','../natural-moveset/Test-MissionPickup-v1061.ps1','../natural-moveset/Test-MissionPickup-v1062.ps1','../natural-moveset/Test-MissionPickup-v1063.ps1','../natural-moveset/Test-MissionPickup-v1064.ps1'],cases=cases,
  findings=['Confirmed runtime: mission worlds are character-specific - Corvo and Emily loads of Edge of the World differ by a handful of script-placed pickups (ammunition types vs coins/food), so the shared pickup catalog must be an intersection.','Confirmed runtime: pickup census varies slightly between loads (streaming/scripts); the identity-list exchange happens at bind time on both peers, so both derive the same intersection regardless.','Confirmed runtime: ordinary native interaction pickups route through the installed reward observer and Steam commit path on a mission map; currency is shared (both wallets +30).','Pickpocket pouches carried by NPCs are catalog entries but are taken through the NPC interaction, not world pickup (not yet exercised).'],
  complete=False,outstanding=['Food and stored-potion families on mission maps (Dunwall-verified only); pickpocket pouches; containers/drawers; grapple (Far Reach) pickups.','Checkpoint/reload renewal of the mission catalog (Dunwall-verified only).','Remaining Dunwall-only gates: usable/door-adjacent sync, checkpoint/shared death, Doppelganger visual, props, power FX, guard/damage legacy.']))
wr(N/'runtime-video-manifest.json',videos)
C['v1059c'].update(runtimeVerified=True,scopedMissionPickupVerified=True,runtimeEvidence='../mission-pickup-audit-v1065.json');wr(W/'candidate-npc-combat-v1059c/identity.json',C['v1059c'])
C['v1059'].update(runtimeVerified=False,result='1060: whole-catalog digest cannot match per-character mission worlds; superseded by 1059c',runtimeEvidence='../mission-pickup-audit-v1065.json');wr(W/'candidate-npc-combat-v1059/identity.json',C['v1059'])
C['v1059b'].update(runtimeVerified=False,result='1061: hello-paired intersection did not scale (asymmetric settle); superseded by 1059c',runtimeEvidence='../mission-pickup-audit-v1065.json');wr(W/'candidate-npc-combat-v1059b/identity.json',C['v1059b'])
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
s['MovementExecutionPriority']['current']='1059c live (action protocol73, coin wire7): shared pickups run on mission maps - per-character world intersection via one identity-list exchange, ordinary native pickups verified on Edge of the World (1064); water exits and swim/climb both roles (1056/1057).'
s['MovementExecutionPriority']['next']='Continue lifting Dunwall-only gates in dependency order: usable/door sync, checkpoint/shared death on mission maps, Doppelganger visual/props; then NPC/combat authority on missions (stage 4).'
wr(R/'work/current-task-state.json',s)
print(json.dumps(dict(recordings=len(videos),cases=[(c['take'],c['passed']) for c in cases])))
