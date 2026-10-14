from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; D=R/'documentation/handoff-2026-10-11'; F=N/'native-gait-batch-v645'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();a=rd(W/'gait-batch-audit-v646.json');r=rd(F/'result.json');assert a['passed']and r['passed']and len(a['checks'])==22
keys=['stand_idle','armed_idle','start_stop_each_gait','gait_speed_changes','moving_turns','reverse_and_direction_changes','sword_draw_holster']
videos=rd(N/'runtime-video-manifest.json')
for source,character in [('host','corvo'),('client','emily')]:
 assert(F/f'{character}-gait-sheet.jpg').exists()
 for view in ('source','observer'):
  video=F/(f'{source}-gait-batch.mp4'if view=='source'else f'{source}-observer.mp4')
  rel=video.relative_to(N).as_posix();assert video.exists()and not any(x['video']==rel for x in videos)
  reviewed=view=='observer';note='Native full-body locomotion, transitions and holster/redraw are visible. Fine foot/contact/body acting deferred.'
  if character=='emily':note+=' Moving-turn/backward sections partly leave the left camera edge; no complete visual acceptance for those intervals.'
  review=F/f'{character}-{view}-review-v646.json';wr(review,dict(at=at,inspected=reviewed,visualAccepted=False,complete=False,sheets=[f'{character}-gait-sheet.jpg']if reviewed else[],observations=[note if reviewed else'Actual first-person reference; no separate visual acceptance.']))
  m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  videos.append(dict(video=rel,kind='game',character=character,label=f'{character.title()} · locomotion batch645 · {view}',alias='start_stop_each_gait',requirements=keys,gameplay_bound=True,complete=False,visual_accepted=False,evidence='native-gait-batch-v645/result.json',visual_review='native-gait-batch-v645/'+review.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=True,scope=r['scope']+' '+note))
wr(N/'runtime-video-manifest.json',videos)
annotations=rd(N/'review-evidence-annotations.json')
latest='645/646: both roles pass11 independent functional checks in one batched native course: walk/run start-stop, speed change, forward/back and diagonal reversals, moving turns, holstered idle/movement and redraw. Steam/body stays ready; minimum settled body weight exceeds0.9998. Full inventories/health and14protected hashes restored. Both observer sheets inspected; fine pose polish deferred, Emily moving-turn framing partly cropped. This is a fixed-course subset, not all movement.'
for key in keys:
 i=annotations['items'][key];i['runtime_binding']='partial_native_steam_batched_locomotion_both_roles';i['complete']=False;i['notes']+=' '+latest
 if 'native-gait-batch-v645/result.json'not in i.setdefault('evidence',[]):i['evidence'].append('native-gait-batch-v645/result.json')
wr(N/'review-evidence-annotations.json',annotations)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['GaitBatchContinuation']=dict(at=at,latest=latest,candidate='candidate-heavy-v635',receipt='work/movement-integration/gait-batch-audit-v646.json',runtimeVerified=True,complete=False,next='Continue missing functional bindings in existing pipeline. Inspect native chain/ladder/swim state availability and current-map fixtures; defer fine posing and external co-op mod research. Keep editable donor/source assets for later animator polish.')
s['MovementExecutionPriority']['current']=latest;s['MovementExecutionPriority']['polishDelegationPreference']='User2026-10-13 explicitly accepts functional-first coverage and later polish by a hired animator. Preserve editable sources and review recordings; do not claim complete gameplay from an attractive clip.';wr(R/'work/current-task-state.json',s)
p=D/'48-batched-locomotion-coverage.md';p.write_text('''# Batched locomotion coverage646

The current pipeline is retained: live candidate635, protocol69, bank67/357. No new production DLL, clip or framework was introduced. [Native run645](../../work/natural-moveset/native-gait-batch-v645/result.json) records20 timed input stages on each character, both game views and one final checkpoint restoration. [Independent audit646](../../work/movement-integration/gait-batch-audit-v646.json) passes22 functional checks. [Test harness](../../work/natural-moveset/Test-GaitBatch-v643.ps1) and [audit code](../../work/movement-integration/audit_gait_batch_v646.py) are reproducible with a new take name.

The course covers armed idle, walking into running, stopping, immediate forward/back reversal, opposing diagonals, translation while turning, native holster, unarmed idle/walking/stopping, redraw and stationary armed recovery. Both source and receiver native sword states are independently checked. Steam stays connected, the bank stays active and settled body weight stays above0.9998. Source movement remains native; network pose owns remote root translation.

## Measurement limits

Corvo214 and Emily215 samples show mean contemporaneous root differences about0.039m and0.029m. Maxima are0.478m and0.830m. The one-metre test bound detects abandoned/wrong following; it does not certify ideal smoothness or latency. Different probe timestamps and deliberate interpolation contribute, but these measurements alone do not isolate causes.

This is one fixed Dunwall course. It does not finish every start/stop gait, sprint/crouched variant, terrain, arbitrary collision, combat interruption or complete movement. Both observer sheets were inspected: native bodies perform the sequence. Emily partly exits the left camera edge during some moving-turn/backward intervals, so those frames cannot establish complete visual acceptance. Fine feet, contact, grip and body acting remain polish for later animator work.

## Preserved and deferred

Full checkpoint inventories, both players/partners' health and14 protected save/settings hashes are restored. Editable bank construction scripts, source/donor clips and the current archive remain available. The user confirmed that functional integration comes first and a hired animator can polish the retained sources later.

[Failed setup643](../../work/natural-moveset/native-gait-batch-v643/result.json) is retained: the harness requested unsupported `coop_shadow_stream 60000`, so the ready check refused to start recordings. Supported continuous mode is `coop_shadow_stream on`; it renews bounded leases internally.645 corrects the harness, not production. The failure's inventory/health/protected-file restoration passed.
''')
p=D/'README.md';t=p.read_text();link='[Batched locomotion coverage and later animator polish](48-batched-locomotion-coverage.md)'
if link not in t:t+='\n49. '+link+'\n'
p.write_text(t)
print(json.dumps(dict(recordings=len(videos),functionalChecks=22,existingCandidate='candidate-heavy-v635',completeMovement=False)))
