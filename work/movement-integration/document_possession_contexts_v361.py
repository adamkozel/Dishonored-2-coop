"""Native source observations, not remote acceptance."""
from pathlib import Path
import json,hashlib,subprocess
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['corvo_possession_enter_loop_exit']
for take,label,note in [
 ('possession-crouch-observe-v361','crouched armed','Source retains lowered native camera/sword combat grip while left arm enters and holds Possession aim. Sneaking6+UsePower63 owns333ms entry/200ms aim. Old standing-only reader accepts0 of390; this is missing context coverage, not absent native animation.'),
 ('possession-holstered-observe-v361','standing initially holstered','Source starts empty-handed, draws native sword while left arm begins Possession, then retains sword. Raw Running0+UsePower63+EquipChange53 overlaps sword_equip Scene_2/1066ms with the left-arm cast. Old reader accepts33 later aim samples and rejects the initial three-state overlap.'),
 ('possession-crouch-holstered-observe-v361','crouched initially holstered','Source begins with lowered empty-hand view, casts with left hand and draws/turns sword into native crouch grip. Raw Sneaking6+UsePower63+EquipChange53 overlaps sword_equip_sneak Scene_2/1133ms with the left-arm cast. Old standing-only reader accepts0 of378.')]:
 t=P/take;r=read(t/'result.json');assert r['passed'] and (t/'source-sheet.jpg').exists()
 review=dict(inspected=True,method='Viewed native source temporal sheet at0.8/2/2.4/2.8/3.3/4.3s',evidence=['source-sheet.jpg'],complete=False,visual_accepted=False,notes=note+' Source-only native evidence. No new remote presentation, NPC control or sound validation.')
 (t/'visual-review.json').write_text(json.dumps(review,indent=2))
 v=P/(take+'.mp4');meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(v)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
 row=dict(video=v.name,character='corvo',label='Possession native context · '+label+' ·361',alias='native_possession_context',requirements=['corvo_possession_enter_loop_exit'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=f'{take}/visual-review.json',proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=hashlib.sha256(v.read_bytes()).hexdigest().upper(),checks_passed=True,scope=review['notes'])
 videos=[x for x in videos if x['video']!=v.name];videos.append(row);item.setdefault('native_integration',{})[v.name]=row
(P/'runtime-video-manifest.json').write_text(json.dumps(videos,indent=2));(P/'review-evidence-annotations.json').write_text(json.dumps(ann,indent=2))
p=R/'documentation/handoff-2026-10-11/32-native-possession-investigation.md';s=p.read_text();heading='## Native crouched and holstered context observations,361'
assert heading not in s
s+='\n\n'+heading+'''

Three source-only tests pass7 checks each: [crouched armed](../../work/natural-moveset/possession-crouch-observe-v361/result.json), [standing initially holstered](../../work/natural-moveset/possession-holstered-observe-v361/result.json), and [crouched initially holstered](../../work/natural-moveset/possession-crouch-holstered-observe-v361/result.json). Their game recordings were inspected and indexed. Both full checkpoint inventories/health and protected14 restore after each run, with final exits consumed sequentially.

Crouch retains native Sneaking6 (vtable RVA1c84258) plus UsePower63 (1c80ed8) while the same left-arm-only Scene_1 entry333ms and aim200ms run.54 raw active samples include1 incomplete initialization; the old standing-only strict reader correctly refuses the crouch context (0 accepted). It should be extended with matching stance/class checks, not by ignoring state ownership. Runtime release from crouch is not established here.

Starting from holstered does not remain empty-handed. Native input equips the sword and adds EquipChange53 alongside UsePower63. The standing draw is `sword_equip.midnightscene`, Corvo Scene_2,1066ms; crouched draw is `sword_equip_sneak.midnightscene`, Scene_2,1133ms. Each has two reciprocal raw bindings. The old reader accepts33 later standing samples after the draw overlay retires; it rejects the initial three-state overlap. Full sword/arm/equipment ownership must be validated before a combined remote draw/cast is allowed. These are observed first-person timings and contexts, not fixed wrist targets for authored bodies.

363 authors two additional complete-body crouched entry/aim clips for the native skeletons; Emily is offline comparison only. Bank53/318 retains the exact previous316 payloads. Native leg lengths/correctives and the crouched sword grip/socket remain; root movement and gameplay effects stay external. Static tests pass, runtime and visual validation are tracked separately in the363 take. Initially holstered overlap remains pending.
''';p.write_text(s)
print(json.dumps(dict(videos=len(videos),native_contexts=3,remote_acceptance=False)))
