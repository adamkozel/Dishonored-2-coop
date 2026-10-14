"""Index bounded actual-game turn/stance evidence, without full-moveset claims."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
archive=R/'work/movement-integration/candidate-knee-pole-v19-v4'
digest='712205BCCB55BCE4D733AB3F910F5338DA476438B1B1138B0926D9FCC4E0329F'
assert sha(archive/'dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json');tests=[];total=0
for kind in ('crouch-turn','turn-stance'):
 for role in ('corvo','emily'):
  for unarmed in (False,True):
   take=f'{kind}-{role}-'+('unarmed-'if unarmed else '')+'knee-v4'
   d=read(P/take/'result.json');validation=read(P/take/'knee-pole-validation.json')
   assert d['failure'] is None and d['dll']==digest and d['recorded'] and validation['passed']
   assert (P/take/'contact.jpg').exists() and d['bank']==sha(P/'crouch-turn-banks-v1'/f'{role}-crouch-turn-bank.bin')
   video=P/f'{take}.mp4'
   cap=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
   hz=int(cap['nb_frames'])/float(cap['duration']);assert hz>=24
   legs=[leg for pose in validation['poses']for leg in pose['legs']]
   row=dict(video=video.name,character=role,label=role.title()+' · '+('Crouched turns'if kind=='crouch-turn'else'Turning: stand / crouch / stand')+' · '+('holstered'if unarmed else'sword drawn'),kind='game',actual_capture_hz=hz,container_fps=cap['avg_frame_rate'],timing='Actual capture timestamps; no padding or speedup.',gameplay_bound=True,complete=False,visual_accepted=False,scope=d['scope'],dll_sha256=digest,sha256=sha(video),capture=cap,chapters=d['chapters'],evidence=f'{take}/result.json',requirements=['crouched_turns']+(['turn_in_place_90_180','stand_to_crouch','crouch_to_stand']if kind=='turn-stance'else[]),tests=[f'{take}/result.json',f'{take}/knee-pole-validation.json'],
    knee_correction=dict(max_pole_error_before_degrees=max(x['pole_error_before_deg']or 0 for x in legs),max_pole_error_after_degrees=max(x['pole_error_after_deg']or 0 for x in legs),max_native_endpoint_error_m=max(x['endpoint_error_m']for x in legs)),
    visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,scope='Six sampled native frames plus structural phase2/phase3 checks; further sustained-motion review remains.'),
    limitations=['Bounded flat-floor Dunwall turns with current native equipment; no claim of complete terrain/contact coverage.','Native floor IK endpoints and ankle rotations retained; authored knee bend plane restored before native helpers.','Fast reversal, collisions and combat interruption need additional coverage.','Original native IK millimeter ankle residuals are separate from this pole correction; legacy flat-floor tolerance is unchanged.'])
   rows=[r for r in rows if r['video']!=video.name]+[row];tests.append(take);total+=len(d['checks'])
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
m=read(archive/'manifest.json');m.update(tests=tests,checks=total,gameplayVerified=True,visual_accepted=False,complete=False)
(archive/'manifest.json').write_text(json.dumps(m,indent=2),encoding='utf-8')
print('Indexed',len(tests),'native turn/stance recordings;',len(rows),'actual D2 videos;',total,'checks; movement complete: false')
