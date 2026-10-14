"""Index inspected native equipment takes without overstating completeness."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
path=P/'runtime-video-manifest.json';rows=read(path)
tests=['equipment-bank-v3-test/result.json','equipment-pause-v3/result.json','equipment-v3-priority/result.json']
for name in tests:assert read(P/name)['passed'],name
for role in ('corvo','emily'):
 name=f'integrated-equipment-close-{role}-v3';take=read(P/name/'take.json');assert not take['failure']
 video=P/f'{name}.mp4';stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=29.9
 row=dict(video=video.name,character=role,label=f'{role.title()} · Native sword draw/holster, standing and crouched',
  actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.',
  kind='game',travelling=False,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Native first-person inputs and exact owned equipment scenes drive whole-body draw/holster and body-relative native clone sword handoff over Steam.',
  chapters=take['chapters'],dll_sha256=take['dll'],bank_sha256=take['bank'],sha256=hashlib.sha256(video.read_bytes()).hexdigest(),capture=stream,evidence=f'{name}/take.json',
  requirements=['sword_draw_holster','sword_ready_idle','stand_to_crouch','crouch_to_stand'],tests=tests,
  visual_review=dict(contact_sheet=f'{name}/contact.jpg',observations=['Full body visible in native Dunwall room.','Crouched equipment posture no longer adds the complete crouch bend twice.','Native sword changes between the hand and hip at the authored reach.'],accepted=False),
  limitations=['One stationary view per role; fine finger contact, moving draw/holster and arbitrary interruption need additional visual review.','Crouched variants use a bounded posture adaptation of the native standing motion.','No general family or complete movement acceptance from this take.','Pause cleanup is verified; automatic session rearm and full death/load lifecycle remain unfinished.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
path.write_text(json.dumps(rows,indent=2),encoding='utf-8');print('Indexed',len(rows),'actual-game recordings; complete remains false')
