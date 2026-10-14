"""Current native moving-turn/stair regression recordings, bounded claims."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='712205BCCB55BCE4D733AB3F910F5338DA476438B1B1138B0926D9FCC4E0329F'
archive=R/'work/movement-integration/candidate-knee-pole-v19-v4';assert sha(archive/'dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json');tests=[];checks=0
for kind in ('moving-turn','terrain'):
 for role in ('corvo','emily'):
  for unarmed in (False,True):
   take=f'{kind}-{role}-'+('unarmed-'if unarmed else '')+'knee-v4'+('b'if kind=='moving-turn'and role=='corvo'and not unarmed else '')
   if kind=='terrain'and role=='emily'and not unarmed:take+='b'
   d=read(P/take/'result.json');validation=read(P/take/'knee-pole-validation.json')
   assert d['failure']is None and d['dll']==digest and d['recorded']and validation['passed']
   assert (P/take/'contact.jpg').exists()and d['bank']==sha(P/'crouch-turn-banks-v1'/f'{role}-crouch-turn-bank.bin')
   if kind=='terrain':assert read(P/take/'native-feet-validation.json')['passed']
   video=P/f'{take}.mp4';capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
   hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
   row=dict(video=video.name,character=role,label=role.title()+' · '+('Curved walk/run/sprint'if kind=='moving-turn'else'Native stairs with authored knee direction')+' · '+('holstered'if unarmed else'sword drawn'),kind='game',actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],timing='Actual capture timestamps; no padding or speedup.',gameplay_bound=True,complete=False,visual_accepted=False,travelling=True,scope=d['scope'],dll_sha256=digest,sha256=sha(video),capture=capture,chapters=d.get('chapters',[]),evidence=f'{take}/result.json',requirements=['moving_turns','start_stop_each_gait']if kind=='moving-turn'else['slopes_and_stairs'],tests=[f'{take}/result.json',f'{take}/knee-pole-validation.json']+([f'{take}/native-feet-validation.json']if kind=='terrain'else[]),visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,scope='Sampled actual D2 frames; whole-library visual acceptance remains separate.'),limitations=['Bounded Dunwall fixture; complete movement and arbitrary geometry/contact coverage remain unfinished.','Native engine owns collision/terrain and first-person controls; remote pose controls no gameplay outcomes.'])
   rows=[r for r in rows if r['video']!=video.name]+[row];tests.append(take);checks+=len(d['checks'])
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
m=read(archive/'manifest.json');m.update(locomotion_tests=tests,locomotion_checks=checks)
(archive/'manifest.json').write_text(json.dumps(m,indent=2),encoding='utf-8')
print('Indexed',len(tests),'new locomotion videos;',len(rows),'actual D2 videos;',checks,'checks; complete:false')
