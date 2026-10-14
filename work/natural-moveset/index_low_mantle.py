"""Index inspected native-input evidence without claiming full traversal acceptance."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
test='low-mantle-v6-recovery-test/result.json';assert read(P/test)['passed']
path=P/'runtime-video-manifest.json';rows=read(path)
for role in ('corvo','emily'):
 name=f'integrated-low-mantle-{role}-v2';take=read(P/name/'take.json');assert not take['failure']
 video=P/f'{name}.mp4';stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=11.8
 row=dict(video=video.name,character=role,label=f'{role.title()} · Native desk mantle, contact revision',actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.',kind='game',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Exact owned native low-mantle scene and replicated scene clock select whole-body climb; network root remains authoritative. Native fast sword recovery is supported.',chapters=take['chapters'],dll_sha256=take['dll'],bank_sha256=take['bank'],sha256=hashlib.sha256(video.read_bytes()).hexdigest(),capture=stream,evidence=f'{name}/take.json',requirements=['ledge_hang_and_pull_up'],tests=[test],
  visual_review=dict(contact_sheet=f'{name}/contact.jpg',observations=['The side view shows the complete body during the climb and the standing recovery.','The revised leading foot lifts onto the surface rather than retaining the original donor trajectory below it.','Supporting hand and staged foot placement are fitted to the measured native desk traversal.','The initial standing feet extend below this camera view; dark clothing and desk props limit fine contact inspection.'],accepted=False),
  limitations=['Only this observed 500 ms low-mantle scene and Dunwall desk path. This is not evidence for sustained ledge hanging, high mantles or vaults.','Contact still needs adaptation to arbitrary ledge height, geometry and approach.','Desk prop intersections and finger contact need further review; unarmed cases are tested but not filmed in these takes.','No complete movement acceptance.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
path.write_text(json.dumps(rows,indent=2),encoding='utf-8');print('Indexed',len(rows),'actual D2 recordings; complete remains false')
