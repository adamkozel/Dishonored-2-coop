"""Retain all native source takes, including failed sampling experiments."""
import datetime,hashlib,json,subprocess
from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset';D=R/'documentation/handoff-2026-10-11'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();videos=read(P/'runtime-video-manifest.json');cases={}
viewed={'native-grenade-equip-corvo-v52-v2','native-grenade-equip-corvo-v52-v3','native-grenade-equip-emily-v52','native-grenade-scene-corvo-v53','native-grenade-scene-emily-v53-v3','native-grenade-actions-corvo-high-v54','native-grenade-actions-corvo-low-v54','native-grenade-actions-emily-high-v54','native-grenade-reader-corvo-high-v54','native-grenade-reader-emily-low-v54'}
for resultfile in sorted(P.glob('native-grenade-*/result.json')):
 take=resultfile.parent.name;result=read(resultfile);cases[take]=result;video=P/f'{take}.mp4'
 if not video.exists():continue
 if (resultfile.parent/'states.json').exists():
  states=read(resultfile.parent/'states.json')
  if not isinstance(states,list):states=[states] if states else []
  scenes=[dict(ms=r['ms'],active=r['player']['active'],scene=s,item=r['item']) for r in states for s in r['midnight']['instances'] if s['state']==3 and '/gadget_grenade_' in s['asset']]
  write(resultfile.parent/'native-grenade-scenes.json',scenes)
 review=None
 if take in viewed:
  assert(resultfile.parent/'native-sheet.jpg').exists();review=f'{take}/visual-review.json'
  write(P/review,dict(inspected=True,at=now,evidence='native-sheet.jpg',method='Viewed actual-game first-person temporal contact sheet',visual_accepted=False,complete=False,notes=('Native cooking, release, sparks/projectile and explosion are visible. Source player can take self-damage; both isolated checkpoints are restored. This is local first-person evidence, not remote third-person integration.' if '-actions-' in take else 'Native local first-person grenade equip/unequip reference. The initial v52-v2 take failed to start arm presentation; later ordinary X input did. Remote body/prop unbound.')))
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
 row=dict(video=video.name,character='corvo' if result['source']=='host' else 'emily',label=f'Native grenade source only · {take}',alias='grenade_equip_aim_throw',requirements=['grenade_equip_aim_throw'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=result['dll'],actual_capture_hz=n/d,video_sha256=sha(video),checks_passed=result['passed'],scope=result['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row)
write(P/'runtime-video-manifest.json',videos)
ann=read(P/'review-evidence-annotations.json');item=ann['items']['grenade_equip_aim_throw']
item['runtime_binding']='native_source_observed_remote_unbound';item['notes']='Native source equip/unequip verified both roles. Cooking/high throw observed both roles; low throw and exact action-reader results are take-specific. Native hand references sampled offline. Remote body/prop, projectile authority, VFX, interruptions and visual quality remain open.'
item['native_integration']={v['video']:v for v in videos if v['video'].startswith('native-grenade-')};write(P/'review-evidence-annotations.json',ann)
state=read(W/'current-task-state.json');m=state['MovementIntegration']
m['GrenadeNativeActionsV54']=dict(cases=cases,gameplay_archive='movement-integration/candidate-grenade-equip-v52',reader='outputs/d2-coop-lab/src/native_player_grenade_scene.h',reference_manifest='natural-moveset/fp-grenade-reference-manifest-v53.json',reference_clips=35,remote_bound=False,complete=False)
m['LatestContinuation'].update(actual_game_videos=len(videos),next='Validate grenade action reader, author body/hand/prop candidate and bind over native Steam; continue full co-op',complete=False)
state['UpdatedAt']=now;write(W/'current-task-state.json',state)
f=D/'18-native-grenade-investigation.md';s=f.read_text(encoding='utf-8');mark='<!-- GRENADE-ACTIONS-V54 -->';end='<!-- /GRENADE-ACTIONS-V54 -->'
section=f'''{mark}
## Native cooking, release and source references v54

Updated {now}. Gameplay remains the preserved v52 equip-fixture DLL. The v53/v54 reader builds are separate read-only probe candidates; their exact hashes are recorded per take and are not the deployed lab-probe pin.

Corvo's high/low and Emily's high native source takes each pass six scoped checks. Normal controller LT is held for 1,300 ms, then released. The game creates and detonates the real grenade. Both isolated checkpoints are reloaded afterward and player/avatar health, inventory and fourteen protected files are checked. Source self-damage is recorded before reload; no world consistency, remote projectile authority or complete friendly-fire coverage follows from these observations.

| Native scene | Duration | Exact binding roles |
|---|---:|---|
| cooking_in | 500 ms | left arm, player, grenade |
| cooking_idle | 1,333 ms | left arm, grenade |
| throw_high | 400 ms | left arm, player, grenade |
| throw_high_ooa | 566 ms | left arm, player |
| throw_low | 400 ms | left arm, player, grenade |
| throw_low_ooa | 400 ms | left arm, player |

All use `Scene_1` and native `ThrowWeapon62`, RTTI `arkPlayerStateThrowWeapon`, vtable RVA `0x1c7f118`, alongside Running0 in these tests. The grenade quantity falls from one to zero at native release while the same equipment identity remains available. Under downward aim, component byte `+0x17c` becomes true during the low throw; it remains false during cooking and the high throw. This confirms the low-throw interpretation within these exact native cases, not a continuously updated aim flag. The authored high-throw scene has a native WEAPON_FIRE event at 293 ms: cosmetic playback must not invoke it.

The guarded reader checks exact scene names/durations, original player/arm/item ownership, stable native lists and supported active states. A later generic probe could miss the 233 ms unequip after the exact reader had seen it. The bounded read-only monitor resolves that sampling issue; failed takes are retained. Successful equip-reader tests pass nine checks for Corvo and Emily. Action-reader validation is recorded separately, per take.

[Thirty-five native first-person references](../../work/natural-moveset/fp-grenade-reference-manifest-v53.json) were sampled offline from the exact base left-arm animation set with the native Edge evaluator. They supply timing, finger articulation and equipment placement relative to a free wrist. They do not anchor a full body to the first-person camera. High/low source action sheets were viewed for Corvo and the high source sheet for Emily. Remote animation/prop attachment, locomotion, crouched actions, reload with additional grenades, cancellation, slide and VFX synchronization are still open.
{end}'''
if mark in s:s=s[:s.index(mark)]+section+s[s.index(end)+len(end):]
else:s+='\n\n'+section+'\n'
f.write_text(s,encoding='utf-8');print(json.dumps(dict(cases=len(cases),videos=len(videos),remote_grenade_bound=False)))
