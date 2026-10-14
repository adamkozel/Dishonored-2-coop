"""Index visually inspected native weaponless air captures with bounded claims."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
path=P/'runtime-video-manifest.json';rows=read(path)
tests=[f'{name}/result.json' for name in ('unarmed-special-v4-test','unarmed-special-v10-priority-2','unarmed-special-v10-pause-2')]
for name in tests:assert read(P/name)['passed'],name
for role in ('corvo','emily'):
 name=f'integrated-unarmed-air-{role}-v3';take=read(P/name/'take.json');assert not take['failure'] and take['unarmed']
 video=P/f'{name}.mp4';stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=25.8
 row=dict(video=video.name,character=role,label=f'{role.title()} · Unarmed standing, crouched and moving jumps',
  actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.',
  kind='game',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Native jump, fall and landing states with Steam root movement select hands-free full-body variants; native sword stays holstered.',
  chapters=take['chapters'],dll_sha256=take['dll'],bank_sha256=take['bank'],sha256=hashlib.sha256(video.read_bytes()).hexdigest(),capture=stream,evidence=f'{name}/take.json',
  requirements=['standing_jump_takeoff','running_jump_takeoff','crouch_jump_transition','rising_airborne','fall_loop','soft_landing','moving_landing'],tests=tests,
  visual_review=dict(contact_sheet=f'{name}/contact.jpg',observations=['Both feet and raised hands remain visible at apex and landing.','Native sword stays at hip, leaving the animated hands free.','Wide Dunwall camera preserves the complete body, with reduced detail at this distance.'],accepted=False),
  limitations=['Single wide view per role; terrain, high falls, contacts and arbitrary interruptions remain unverified.','No double jump or in-air combat acceptance.','No complete movement or whole-family visual acceptance from these takes.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
path.write_text(json.dumps(rows,indent=2),encoding='utf-8');print('Indexed',len(rows),'actual-game recordings; complete remains false')
