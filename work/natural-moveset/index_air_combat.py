"""Index inspected real-game airborne sword/guard takes, with explicit limits."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='6F257EC8CD838F2381E5BF6F0E2E3C652C32304C74C3DFEB4F9910158797FD38'
assert sha(R/'work/movement-integration/candidate-air-clock-v2/dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json')
for role in ('corvo','emily'):
 take=f'integrated-air-combat-{role}-v2';d=read(P/take/'result.json')
 assert d['failure']is None and d['dll']==digest and len(d['checks'])==7 and d['recorded']
 assert(P/take/'contact.jpg').exists();video=P/f'{take}.mp4'
 s=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(s['nb_frames'])/float(s['duration']);assert hz>=24 and float(s['duration'])>=13.8
 row=dict(video=video.name,character=role,label=role.title()+' · Airborne sword, guard and landing',kind='game',actual_capture_hz=hz,container_fps=s['avg_frame_rate'],timing='Actual Windows Graphics Capture timestamps; real elapsed time, no padding or speedup.',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Native jump/fall and owned sword-scene clocks over Steam. Authored airborne combat legs/pelvis and native-role sword acting; landing continues the same source scene. No remote damage events or physics calls.',
  chapters=[dict(time=1.7,label='Jump and early strike'),dict(time=3.1,label='Landing and recovery'),dict(time=5.7,label='Jump with sustained guard'),dict(time=9.7,label='Late airborne strike through landing')],
  dll_sha256=digest,bank_sha256=d['bank'],sha256=sha(video),capture=s,evidence=f'{take}/result.json',requirements=['airborne_attack','block_enter_hold_exit','soft_landing'],tests=[f'{take}/result.json'],
  visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,observations=['Whole native body remains visible through rise, strike or guard, landing and recovery.','Sword action continues through landing instead of dropping the airborne body bank.','The broad motion is visible; this view is too distant for final finger-contact acceptance.']),
  limitations=['Isolated ordinary Dunwall jump; moving jumps, heavy releases, double jumps and target contacts need broader evidence.','Leg and torso balance during every sword variant remains subject to final visual review.','Complete lifecycle and full movement acceptance remain unfinished.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
(R/'work/movement-integration/air-combat-findings.json').write_text(json.dumps(dict(complete=False,visual_accepted=False,dll=digest,bank='D2MOVE16 / 141 clips',tests=[f'integrated-air-combat-{r}-v2/result.json'for r in ('corvo','emily')],checks=14,
 confirmed=['Both native characters perform early/late airborne sword actions and guarding over Steam.','Landing preserves the current native scene phase.','Future landed action state no longer cancels the delayed airborne body.','Native health, first-person controls, sword attachment and protected files remain intact.'],
 pending=['Moving sword gait, broader combat/terrain/variant coverage and final visual acceptance.','Remaining functional movement requirements.'],rollback='work/movement-integration/candidate-air-clock-v2'),indent=2),encoding='utf-8')
print('Indexed actual D2 airborne combat:',len(rows),'game recordings; complete: false')
