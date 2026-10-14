from pathlib import Path
import json,hashlib,datetime,subprocess,shutil
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert rd(W/'native-operation-owner.json')['state']=='finished'
A=W/'candidate-heart-network-v564';identity=rd(A/'identity.json');assert sha(R/'work/asset-game/dinput8.dll')==identity['dll'];assert sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')==identity['probe']
at=datetime.datetime.now().astimezone().isoformat();tests=[]
for role in ('corvo','emily'):
 p=N/f'heart-network-{role}-v565';r=rd(p/'result.json');assert r['passed'] and len(r['checks'])==11 and r['dll']==identity['dll'];assert (p/'observer-sheet.jpg').is_file();tests.append(dict(role=role,path=str((p/'result.json').relative_to(R)),checks=r['checks']))
limitations=['Fine grip/contact and continuous visual quality unaccepted; temporal sheets only.','Crouch composes the existing lower-body locomotion with authored Heart posture; no new crouch authoring.','Heart internal mechanism/heartbeat, secret-target reactions, dialogue and discovery outcomes are not synchronized.','Airborne, sword-combat combinations, sprint/slide and interruptions beyond tested stream stop remain unverified.','Trap animations still lack runtime binding. Complete moveset and playable co-op remain unfinished.']
receipt=dict(at=at,candidate=A.name,dll=identity['dll'],probe=identity['probe'],protocol=60,bankVersion=60,clips=340,tests=tests,runtimeVerified=True,visualAccepted=False,complete=False,limitations=limitations)
wr(W/'heart-verification-v566.json',receipt);wr(A/'runtime-verification-v566.json',receipt)
shutil.copy2(R/'outputs/d2-coop-lab/research/heart-prop-presentation-asset-install.json',A/'heart-asset-install.json')
videos=rd(N/'runtime-video-manifest.json')
for character in ('corvo','emily'):
 for native in (True,False):
  name=f'native-heart-{character}-v563' if native else f'heart-network-{character}-v565';folder=N/name;r=rd(folder/'result.json');assert r['passed']
  for view in (('source',) if native else ('source','observer')):
   video=folder/f'{view}.mp4';rel=str(video.relative_to(N)).replace('\\','/');assert not any(v['video']==rel for v in videos)
   inspected=not native and view=='observer'
   review=dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=['observer-sheet.jpg'] if inspected else [],observations=['Viewed actual D2 temporal sheet: remote native body holds Heart in left hand, uses it, crouches and returns to standing/holstered state. Prop follows hand; fine grip and continuous timing not certified.'] if inspected else ['Source recording retained as native action evidence; no separate visual quality acceptance.'])
   rp=folder/f'{view}-visual-review.json';wr(rp,review)
   m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
   videos.append(dict(video=rel,character=character,label=character.title()+' · Heart '+('native source observation' if native else 'Steam body and prop · '+view),alias='heart_equip_use_holster',requirements=['heart_equip_use_holster'],gameplay_bound=not native,complete=False,visual_accepted=False,evidence=name+'/result.json',visual_review=name+'/'+rp.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=True,scope=r['scope']))
wr(N/'runtime-video-manifest.json',videos)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['HeartContinuation']=dict(at=at,latest='564/565: Corvo and Emily each pass11 native Steam Heart body/prop checks; exact equip533/use1000/holster333 scenes plus selected idle, existing crouch composition, hide lifecycle and restored checkpoints. Bank60/340 appends four existing clips, prior336 payloads unchanged. Both actual observer sheets viewed; fine polish and internal Heart mechanism remain deferred.',candidate=A.name,receipt='work/movement-integration/heart-verification-v566.json',complete=False,next='Continue broad native binding with existing authored trap clips (stun mine/spring razor), then remaining interactions/traversal. Use current pipeline; third-party co-op research and cosmetic polish remain deferred.')
state['MovementExecutionPriority']['current']=state['HeartContinuation']['latest'];wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');item=ann['items']['heart_equip_use_holster'];item['notes']=state['HeartContinuation']['latest']+' '+ ' '.join(limitations);item['runtime_binding']='partial_native_steam_ground_heart_body_prop';item['complete']=False;item['visual_accepted']=False;wr(N/'review-evidence-annotations.json',ann)
chapter='''# Native Heart: current-pipeline body and held-prop binding

## Result and scope

The existing authored Heart idle/equip/use/holster clips now play on both remote Corvo and Emily through native Steam. Each role passed 11 runtime checks in isolated Dunwall. Actual recordings cover the native source view and remote observer view. Both observer temporal sheets were visually inspected: the native complete body holds and uses the Heart, transitions into the existing crouch, and returns to standing/holstered state. Fine grip, continuous animation quality and internal Heart animation remain unaccepted. This is functional coverage, not complete movement or complete Heart gameplay synchronization.

- [Consolidated runtime receipt](../../work/movement-integration/heart-verification-v566.json)
- [Corvo runtime test](../../work/natural-moveset/heart-network-corvo-v565/result.json) and [viewed sheet](../../work/natural-moveset/heart-network-corvo-v565/observer-sheet.jpg)
- [Emily runtime test](../../work/natural-moveset/heart-network-emily-v565/result.json) and [viewed sheet](../../work/natural-moveset/heart-network-emily-v565/observer-sheet.jpg)
- [Candidate identity](../../work/movement-integration/candidate-heart-network-v564/identity.json), [test script](../../work/natural-moveset/Test-HeartNetwork-v565.ps1), [bank provenance](../../work/natural-moveset/heart-bank-manifest-v564.json)

## Native findings from the exact executable

Executable SHA-256: `B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180`. The [read-only inventory/decompilation evidence](../../work/natural-moveset/native-gadgets-research-v561/ghidra-native-inventory.json) and both-role native563 observations establish:

| Finding | Confirmed evidence |
|---|---|
| Inventory | `models/items/heart/heart.inventoryitem`, type2, maximum1; native item already exists at quantity0 in the test checkpoint |
| Component | `arkCpntHeart` VT RVA `1b99c68`; owner is the reciprocal item entity |
| Declaration | VT `1b99b28`, `components/characters/player/base/items/heart.cpntheart`, slot1 |
| Entity | `idEntity` VT `1c87d58`, parent handle1, component membership verified |
| Attachments | Declaration names `gun_attach_tag` and `heart_attach_tag`; remote authoring track uses native body `left_wrist_jnt`, joint9 |
| Persistent native state | ID67 / VT `1c80b78` is UseHeart; coexists with Running0 or Sneaking6 even during selected idle |
| Equip | `midnight/gameplay/player/base/gadgets/heart_equip.midnightscene`, `Scene_1`, 533ms, owned left-arm binding |
| Holster | `heart_unequip.midnightscene`, `Scene_1`, 333ms, owned left-arm binding |
| Use | `heart_use.midnightscene`, `Scene_1`, 1000ms, owned left arm plus exact Heart entity |

These findings are saved as a bookmark at `141b99c68` in the persistent exact-game Ghidra project, which was saved successfully. Ghidra reconfirmed `618de0` quantity callback, `614b20` native equip and getters `63c890`/`63c8c0`; existing getters do not fabricate equipment. Native source tests [Corvo](../../work/natural-moveset/native-heart-corvo-v563/result.json) and [Emily](../../work/natural-moveset/native-heart-emily-v563/result.json) each pass four checks. Transient reader refusals during equip transitions are retried; they are not interpreted as valid idle.

## Implementation and reproduction

[Native item reader](../../outputs/d2-coop-lab/src/native_player_heart.h) validates exact declaration/type/component/owner/entity/list/selection twice. [Owned scene reader](../../outputs/d2-coop-lab/src/native_player_heart_scene.h) resolves exact native scene clocks and binding actors, then rechecks identities. [Ground state policy](../../outputs/d2-coop-lab/src/native_player_states.h) recognizes observed UseHeart overlays without relaxing airborne or alternate-mode rules.

Semantic protocol60 appends Heart held/variant/elapsed/duration fields; action19 represents owned Heart scenes. The [presentation timeline](../../outputs/d2-coop-lab/src/heart_presentation.h) uses the shared delayed source clock, expires stale data and rejects rewinds within one event. No raw input is transmitted as an animation event. No damage or native Heart-use event is replayed on the remote actor.

[Bank builder](../../work/natural-moveset/build_heart_bank_v564.py) reuses `items-authored-v2/heart_{idle,equip,unequip,use}-{corvo,emily}.bin`. Bank60 has340 clips; the prior336 clip payloads are unchanged. Existing authored wrist-relative attachment tracks are reused. Movement still controls root position; existing lower-body motion blends with Heart presentation during crouch/travel. Crouch has runtime coverage; actual travel with the Heart remains unverified.

The [render-only Heart definition](../../outputs/d2-coop-lab/assets/heart_prop.def) includes native model/render animation only. [Prop service](../../outputs/d2-coop-lab/src/heart_prop.h) reuses existing native entity creation, attachment, ownership checks and Hide/Show lifecycle. It does not contain native Heart, inventory, targeting, damage, pickup or physics components. [Isolated installer](../../outputs/d2-coop-lab/Build-D2HeartPropAssets.ps1) preserves overwritten isolated resource files and verifies original Steam indices unchanged. The [install receipt](../../outputs/d2-coop-lab/research/heart-prop-presentation-asset-install.json) records file targets/hashes. No original Steam assets were edited.

[Build564](../../work/movement-integration/Build-Heart-v564.ps1) passes native ownership, action/protocol, proxy smoke and both actual340-clip bank checks. Added tests cover wrong peer version, malformed duration, unowned gestures, conflicting held items, unsupported air, stale timeline, same-event rewind and wrong native state vtable.

The bounded20s native test lease acquires the existing Heart through native inventory callbacks, selects it, and restores its original zero quantity/empty slot. Test565 performs ordinary native X/LT/B inputs and records both views. Each test ends by restoring both isolated checkpoints and checking all14 protected settings/saves. The prior558 and562 candidates remain preserved. Runtime verification is in a separate receipt; initial candidate metadata intentionally retains its pre-test `runtimeVerified:false` value.

## Gaps and next work

Internal Heart mechanism/heartbeat, secrets/target reactions, dialogue/discovery outcomes, airborne contexts, sword-combat combinations, sprint/slide, broader interruptions and fine visual acceptance remain incomplete. Native first-person controls and presentation are retained. The source alone executes Heart gameplay.

Next is broad trap binding using existing authored stun-mine and spring-razor clips. Exact-file static RTTI candidates are saved under native-gadgets-research-v561: `arkCpntWeaponPlaceable` VT1b54538, declaration VT1b543f8, PlaceWeapon VT1c7c098 and PlaceWeaponReady VT1c3db88. These are static candidates, not runtime-verified trap ownership. `6170b0` calls the existing equipment-creation routine when quantity changes from0 to positive, which provides the next bounded acquisition route. No trap runtime writes have yet been attempted.
'''
(D/'39-native-heart-binding.md').write_text(chapter,encoding='utf-8')
p=D/'README.md';s=p.read_text(encoding='utf-8');link='- [Native Heart body and held-prop binding](39-native-heart-binding.md)';p.write_text(s.rstrip()+'\n'+link+'\n',encoding='utf-8')
p=W/'refresh_grenade_handoff_v58.py';s=p.read_text();mark="m['LatestContinuation']['probeCandidate']=probeArchive.name";assert mark in s
s=s.replace(mark,"heart=state.get('HeartContinuation',{})\nif heart:\n    m['LatestContinuation']['next']=heart.get('next',m['LatestContinuation']['next'])\n    m['LatestContinuation']['in_progress']+=' '+heart.get('latest','')\n"+mark)
p.write_text(s)
print(json.dumps(dict(candidate=A.name,checks=22,recordings=len(videos),complete=False)))
