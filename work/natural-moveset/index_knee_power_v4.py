"""Pin current full-body/native-effect regressions and their real D2 recordings."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='712205BCCB55BCE4D733AB3F910F5338DA476438B1B1138B0926D9FCC4E0329F'
archive=R/'work/movement-integration/candidate-knee-pole-v19-v4';assert sha(archive/'dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json');tests=[];checks=0
for take in ('air-knee-v4','sprint-knee-v4','lean-knee-v4','slide-knee-v4','combat-knee-v4','blink-knee-v4','grapple-knee-v4'):
 d=read(P/take/'result.json');assert d['failure']is None and d['dll']==digest
 tests.append(take);checks+=len(d['checks'])
 if take.startswith(('blink','grapple')):
  assert d['recorded']and(d['blinkFxTested']or d['grappleFxTested'])
  role=d['character'];assert d['bank']==sha(P/'crouch-turn-banks-v1'/f'{role}-crouch-turn-bank.bin')
  if role=='emily':
   hands=read(P/take/'hand-attachment.json');hands=hands if isinstance(hands,list)else[hands]
   assert hands and all(h['stable']and h['finalPublicationComplete']and h['activeRecord']and h['handError']<.0001 for h in hands)
  video=P/f'{take}.mp4';cap=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
  hz=int(cap['nb_frames'])/float(cap['duration']);assert hz>=24 and (P/take/'contact.jpg').exists()
  row=dict(video=video.name,character=role,label=role.title()+' · '+('Blink'if role=='corvo'else'Far Reach')+' · current body rig / native VFX',kind='game',actual_capture_hz=hz,container_fps=cap['avg_frame_rate'],timing='Actual capture timestamps; no padding or speedup.',gameplay_bound=True,complete=False,visual_accepted=False,scope=d['scope'],dll_sha256=digest,sha256=sha(video),capture=cap,evidence=f'{take}/result.json',requirements=[('corvo_blink'if role=='corvo'else'emily_far_reach')+'_aim_cancel_release_arrive'],tests=[f'{take}/result.json'],visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,scope='Sampled actual D2 frames, including native effects. All power variants and full visual acceptance remain unfinished.'),limitations=['Armed standing cast and aim cancellation at a static Dunwall target.','No claim of moving/crouched casts, dynamic targets, upgrades or other powers.'])
  if role=='emily':row['attachment_evidence']=f'{take}/hand-attachment.json';row['effect_contact_sheet']=f'{take}/fx-contact.jpg'
  rows=[r for r in rows if r['video']!=video.name]+[row]
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
m=read(archive/'manifest.json');m.update(action_regressions=tests,action_regression_checks=checks)
(archive/'manifest.json').write_text(json.dumps(m,indent=2),encoding='utf-8')
print('Indexed power recordings;',len(rows),'actual D2 videos;',checks,'current action checks; complete:false')
