"""Index verified D2 stair recordings, with explicit limits on terrain coverage."""
import json,hashlib,subprocess,statistics
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
archive=R/'work/movement-integration/candidate-terrain-v17-v2'
digest='35049A4D22D96CE612680E8E7FFA6365B548D20CB5D15CCE456D5E38BAE8C76C'
assert sha(archive/'dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json');tests=[];total=0
for role in ('corvo','emily'):
 for unarmed in (False,True):
  take=f'terrain-{role}-'+('unarmed-'if unarmed else '')+'v17-v2'
  d=read(P/take/'result.json');validation=read(P/take/'native-feet-validation.json')
  assert d['failure']is None and d['dll']==digest and d['recorded'] and validation['passed']
  assert len(d['checks'])==(8 if unarmed else 7) and (P/take/'contact.jpg').exists()
  assert d['bank']==sha(P/'terrain-banks-v2'/f'{role}-terrain-bank.bin')
  states=read(P/take/'states.json');rates=[]
  for a,b in zip(states,states[1:]):
   if a['trial']==b['trial'] and min(a['movement']['terrainWeight'],b['movement']['terrainWeight'])>.9 and b['movement']['speed']>.7:
    rates.append(((b['movement']['phase']-a['movement']['phase'])%1)*1000/(b['ms']-a['ms']))
  assert len(rates)>15
  median=statistics.median(rates);assert .5<median<2
  video=P/f'{take}.mp4'
  capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
  hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
  row=dict(video=video.name,character=role,label=role.title()+' · Native stairs · '+('holstered'if unarmed else'sword drawn'),kind='game',actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],
   timing='Actual Windows Graphics Capture timestamps; no padding or speedup.',gameplay_bound=True,complete=False,visual_accepted=False,scope=d['scope'],dll_sha256=digest,sha256=sha(video),capture=capture,
   evidence=f'{take}/result.json',requirements=['slopes_and_stairs'],tests=[f'{take}/result.json',f'{take}/native-feet-validation.json'],measured_median_cycle_hz=median,
   visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,observations=['Dedicated native ascent/descent full-body cycles follow filtered actual root grade.','Measured native stride and cycle duration choose the walk/run blend; physical distance advances the same blended cycle.','Native leg lengths preserved through foot IK; original sword state and player health preserved.','Both characters recover level walking and stop stepping when physical travel stops.']),
   limitations=['One straight native Dunwall staircase at a moderate player speed.','Dim lighting limits detailed sole contact review; final visual acceptance remains open.','Crouched/backward/sideways stair gaits, steeper terrain and explicit sprint trials remain.'])
  rows=[r for r in rows if r['video']!=video.name]+[row];tests.append(take);total+=len(d['checks'])
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
m=read(archive/'manifest.json');m.update(tests=tests,checks=total,gameplayVerified=True,complete=False,visual_accepted=False)
(archive/'manifest.json').write_text(json.dumps(m,indent=2),encoding='utf-8')
print('Indexed stair v2:',len(rows),'actual D2 recordings;',total,'checks; full moveset incomplete')
