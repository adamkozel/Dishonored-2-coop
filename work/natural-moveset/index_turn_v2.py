"""Index actual standing turn integration with bounded, auditable claims."""
import json,hashlib,subprocess,statistics
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
archive=R/'work/movement-integration/candidate-turn-v18-v2'
digest='94B180B9467EAD05E70A23180554FBFDEF203B48CC65B653376EF5C28FF91A11'
assert sha(archive/'dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json');tests=[];total=0
for role in ('corvo','emily'):
 for unarmed in (False,True):
  take=f'turn-{role}-'+('unarmed-'if unarmed else '')+'v18-v2'
  d=read(P/take/'result.json');validation=read(P/take/'native-feet-validation.json')
  assert d['failure']is None and d['dll']==digest and d['recorded'] and validation['passed']
  assert len(d['checks'])==(10 if unarmed else 9) and (P/take/'contact.jpg').exists()
  assert d['bank']==sha(P/'turn-banks-v1'/f'{role}-turn-bank.bin')
  states=read(P/take/'states.json');speeds={}
  for trial in ('left-slow','right-slow','left-medium','right-medium','left-fast','right-fast'):
   active=[s['movement']for s in states if s['trial']==trial and 950<s['ms']<1600]
   assert len(active)>5 and min(s['turnWeight']for s in active)>.8
   speeds[trial]=statistics.median(abs(s['turnVelocity'])for s in active)
   if 'fast'in trial:assert sum(s['turnClip']in (153,154)for s in active)/len(active)>.8,'Fast spin still overcranks a short turn'
  video=P/f'{take}.mp4'
  capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
  hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
  row=dict(video=video.name,character=role,label=role.title()+' · Standing turns · '+('holstered'if unarmed else'sword drawn'),kind='game',actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],timing='Actual capture timestamps; no padding or speedup.',
   gameplay_bound=True,complete=False,visual_accepted=False,scope=d['scope'],dll_sha256=digest,sha256=sha(video),capture=capture,chapters=d['chapters'],evidence=f'{take}/result.json',requirements=['turn_in_place_90_180'],
   tests=[f'{take}/result.json',f'{take}/native-feet-validation.json'],measured_turn_radians_per_second=speeds,
   visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,observations=['Actual native look input drives complete-body footwork through replicated facing; no movement input or actor-root animation.','Native root-yaw phase map keeps turning poses aligned with angle, including yaw wrap.','Step-boundary arc reselection handles controller acceleration; fast spins use larger native steps.','Stopping recovers standing; native sword states, health and leg lengths preserved.']),
   limitations=['Standing turns on flat Dunwall floor, six directed look cases per role/equipment state.','Native donor root translation is omitted; horizontal foot planting and rapid reversals need further visual review.','Crouched turns, moving turns and action/stance interruptions remain separate.'])
  rows=[r for r in rows if r['video']!=video.name]+[row];tests.append(take);total+=len(d['checks'])
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
m=read(archive/'manifest.json');m.update(tests=tests,checks=total,gameplayVerified=True,visual_accepted=False,complete=False)
(archive/'manifest.json').write_text(json.dumps(m,indent=2),encoding='utf-8')
print('Indexed standing turns:',len(rows),'actual D2 recordings;',total,'checks; movement complete: false')
