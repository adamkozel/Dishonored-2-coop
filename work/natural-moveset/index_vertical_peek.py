"""Link the inspected native peek takes to their actual runtime evidence."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
path=P/'runtime-video-manifest.json';rows=read(path)
test='vertical-peek-v2-test/result.json';assert read(P/test)['passed']
for role in ('corvo','emily'):
 name=f'integrated-vertical-peek-{role}-v1';take=read(P/name/'take.json');assert not take['failure']
 video=P/f'{name}.mp4';stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=37.8
 row=dict(video=video.name,character=role,label=f'{role.title()} · Standing and crouched upward peek',
  actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.',
  kind='game',travelling=False,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Exact native upward/lateral lean amplitudes replicated over Steam drive whole-body parameter poses; native look input releases lean.',
  chapters=take['chapters'],dll_sha256=take['dll'],bank_sha256=take['bank'],sha256=hashlib.sha256(video.read_bytes()).hexdigest(),capture=stream,evidence=f'{name}/take.json',
  requirements=['ledge_peek','stand_lean_left_right','crouch_lean_left_right'],tests=[test],
  visual_review=dict(contact_sheet=f'{name}/contact.jpg',observations=['Complete body remains visible.','Crouched upward peek unfolds the body and returns to the low stance.','Standing heel rise is subtle at this camera distance.','Both roles return to standing after the native look/stance controls.'],accepted=False),
  limitations=['Standing lift needs closer visual evaluation.','No obstacle-specific sight-line, collision-limited scale or arbitrary combat-interruption acceptance.','Weaponless variants pass native input tests but are not shown in these armed takes.','No complete movement acceptance.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
path.write_text(json.dumps(rows,indent=2),encoding='utf-8');print('Indexed',len(rows),'actual-game recordings; complete remains false')
