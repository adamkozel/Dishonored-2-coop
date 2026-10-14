"""Index source-only native grenade evidence and refresh measured live state."""
import datetime, hashlib, json, subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset';D=R/'documentation/handoff-2026-10-11'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat()
videos=read(P/'runtime-video-manifest.json');cases={}
for take,char in [('native-grenade-equip-corvo-v52-v2','corvo'),('native-grenade-equip-corvo-v52-v3','corvo'),('native-grenade-equip-emily-v52','emily')]:
 result=read(P/take/'result.json');cases[take]=result
 scenes=[dict(ms=r['ms'],active=r['player']['active'],scene=s,item=r['item']) for r in read(P/take/'states.json') for s in r['midnight']['instances'] if s['state']==3 and '/gadget_grenade_' in s['asset']]
 write(P/take/'native-grenade-scenes.json',scenes)
 print(take,[(x['scene']['id'],x['scene']['asset'].split('/')[-1],[(a['id'],a.get('sequence24Research')) for a in x['active']]) for x in scenes])
 sheet=P/take/'native-sheet.jpg';assert sheet.exists()
 review=f'{take}/visual-review.json'
 write(P/review,dict(inspected=True,at=now,evidence=sheet.name,method='Viewed actual-game first-person temporal contact sheet',visual_accepted=False,complete=False,notes=('Native grenade appears in the local left hand after ordinary holster/draw, including crouch. This is source first-person evidence only; remote body and prop are not bound.' if result['passed'] else 'Inventory selection succeeded but did not initiate native hand presentation. Failed take retained.')))
 video=P/f'{take}.mp4';meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
 row=dict(video=video.name,character=char,label=f'Native grenade equip source only · {char} · {take}',alias='grenade_equip',requirements=['grenade_equip_aim_throw'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=result['dll'],actual_capture_hz=n/d,video_sha256=sha(video),checks_passed=result['passed'],scope=result['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row)
write(P/'runtime-video-manifest.json',videos)
archive=W/'movement-integration/candidate-grenade-equip-v52';identity=read(archive/'identity.json')
live=sha(W/'asset-game/dinput8.dll');probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
assert live==identity['dll'] and probe==identity['probe']
checks=[]
for f in read(W/'movement-integration/protected-save-hashes-before-v4.json'):
 actual=sha(Path(f['Path']));checks.append(dict(path=f['Path'],expected=f['Hash'],actual=actual,passed=actual==f['Hash']))
steam=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2')
for name,expected in [('Dishonored2.exe','B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'),('steam_api64.dll','71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8')]:
 for folder in [steam,W/'asset-game']:
  f=folder/name;actual=sha(f);checks.append(dict(path=str(f),expected=expected,actual=actual,passed=actual==expected))
assert all(c['passed'] for c in checks)
write(W/'handoff-integrity-verification.json',dict(at=now,passed=True,checks=checks,liveProxy=live,probe=probe,candidate=archive.name))
state=read(W/'current-task-state.json');m=state['MovementIntegration']
m['GrenadeEquipV52']=dict(archive=str(archive.relative_to(W)),cases=cases,complete=False,scope='Source native equip/unequip and crouched holding only; remote body/prop and cooking/throw remain unbound')
m['GrenadeReaderV52'].update(gameplay_deployed=True,next='Exact native arm and equipment scene reader, then remote body/prop binding')
m['LatestContinuation'].update(live=live,liveBuild=live,probe=probe,actual_game_videos=len(videos),next='Native grenade scene ownership and remote body/prop binding; remaining movement and full co-op',complete=False)
state['UpdatedAt']=now;write(W/'current-task-state.json',state)
f=D/'01-current-state.md';s=f.read_text(encoding='utf-8')
start='<!-- EQUIPMENT-GRENADE-V52 -->';end='<!-- /EQUIPMENT-GRENADE-V52 -->'
section=f'''{start}
## Latest measured live build: grenade equip v52

Updated {now}. This supersedes earlier live-build descriptions on this page. Live proxy `{live}` and probe `{probe}` match [the preserved v52 candidate](../../work/movement-integration/candidate-grenade-equip-v52/identity.json). Protocol 36 and standard D2MOVE41/251 v46 banks are unchanged. [All eighteen protected file checks pass](../../work/handoff-integrity-verification.json).

Both characters pass seven scoped native grenade equip/unequip checks, using ordinary holster/draw input. The local first-person recordings are reviewed and indexed; they do not demonstrate remote grenade movement. Exact native scene ownership is the next binding step. Failed takes are preserved. [Grenade findings](18-native-grenade-investigation.md).

Earlier paired-role equipment, v50 crouched cast guard candidate and v51 failure-effect lifecycle evidence remains valid within each take. The pause visibility gap remains open. There are {len(videos)} indexed actual-game videos; this is not a completion percentage. Movement and playable co-op remain incomplete, with the goal active beyond movement.
{end}'''
assert start in s and end in s;s=s[:s.index(start)]+section+s[s.index(end)+len(end):];f.write_text(s,encoding='utf-8')
f=D/'18-native-grenade-investigation.md';s=f.read_text(encoding='utf-8').replace('This investigation begins the grenade movement binding. No native grenade equip, cooking, throw or remote animation binding has been validated yet. The live gameplay DLL remains v48.','Native equip/unequip has now been observed and validated on both roles in the isolated Dunwall fixture. The live gameplay DLL is the preserved v52 equip candidate. Cooking, throwing and remote body/prop binding remain unvalidated.').replace('the deployed lab probe remains the v48 pin.','the deployed lab probe now uses the v52 pin.')
mark='## Native equip experiment v52'
if mark not in s:s+='''

## Native equip experiment v52

The first publisher attempt rejected the new command before granting an item. The next take proved native selection and rollback, but selection alone did not start first-person presentation. The fixed harness uses normal X holster/draw input, as the previous ranged-weapon investigation did.

[Corvo v3](../../work/natural-moveset/native-grenade-equip-corvo-v52-v3/result.json) and [Emily](../../work/natural-moveset/native-grenade-equip-emily-v52/result.json) each pass seven checks: native equip/unequip scenes, crouched holding, no observed cooking/throw, exact original item rollback, full inventory/health preservation and fourteen save/settings hashes. Both local first-person sheets were visually inspected. The native grenade is visibly held after ordinary draw; this is not evidence of a remote grenade prop or full-body animation.

Exact `gadget_grenade_equip` lasts 500 ms and binds the owned native left arm plus grenade entity. `gadget_grenade_unequip` lasts 233 ms and binds only the owned left arm. Both use `Scene_1`. During combined sword/grenade equipment changes, states 53 and 64 share the EquipChange vtable; state 53 can refer to the sword scene while the grenade unequip scene still runs separately. Scene ownership must therefore be checked through exact arm/equipment bindings, not inferred from the state name or item-selected bit. Full scene/state observations and exact harness copies are retained beside each result.

The twelve-second fixture uses native quantity/equip/unequip calls, single-attempt transitions and bounded rollback. No cook/fire/projectile/effect call is made. No grenade source asset or original Steam file is modified. Next work is a guarded read-only scene reader, followed by isolated remote presentation. Projectile creation, detonation and damage must stay authoritative and must not be replayed from cosmetic animation events.
'''
f.write_text(s,encoding='utf-8');print('Current v52 pins, native source evidence and 18-file integrity saved')
