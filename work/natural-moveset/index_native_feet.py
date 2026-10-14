"""Index actual native-foot integration evidence without claiming full coverage."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='E077DE1C0662F2236DF0A9203D3262F27009DC10E4A8D171220452BCFB1A8147'
archive=R/'work/movement-integration/candidate-native-feet-E077'
assert sha(archive/'dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json');tests=[]
for role in ('corvo','emily'):
 for category,take,count,requirements in [
  ('Grounded foot contact',f'native-feet-{role}-close-v5',8,['moving_attack','crouched_attack','block_enter_hold_exit']),
  ('Jump and landing handoff',f'native-feet-air-{role}-v5',7,['standing_jump_takeoff','soft_landing','moving_attack']),
  ('Native staircase',f'native-feet-stairs-{role}-'+('v5b'if role=='corvo'else'v5'),5,['slopes_and_stairs'])]:
  d=read(P/take/'result.json');assert d['failure']is None and d['dll']==digest and len(d['checks'])==count and d['recorded']
  tests.append(take);assert(P/take/'contact.jpg').exists();video=P/f'{take}.mp4'
  s=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
  hz=int(s['nb_frames'])/float(s['duration']);assert hz>=24
  checks=[]
  if 'air-'not in take:
   validation=read(P/take/'native-feet-validation.json');assert validation['passed'];checks.append(f'{take}/native-feet-validation.json')
  observations=['Authored body enters native ground casts and IK before the final attachment/render publication.','Grounded current-foot branches agree through casts, solve and effector reference; continuous network root updates preserve foot/pelvis history.']
  limitations=['Experimental authored-foot policy; full movement package and final visual acceptance remain incomplete.']
  if 'stairs-'in take:
   observations.append('Both ascent and descent cover approximately three metres of height using native player physics; the animation bank stays active and captured leg lengths remain unchanged.')
   limitations+=['One straight native staircase, walking with sword drawn.','Dim wide shot limits precise sole/contact review. Dedicated stair gait, crouch/running/sideways stair cases and varied slopes remain.']
  elif 'air-'in take:
   observations.append('Foot adaptation releases during authored airborne combat and returns after landing/recovery; native first-person action and sword equipment remain intact.')
   limitations.append('Selected jump/strike/guard cases; double jump and hard falls remain separate.')
  else:
   observations.append('Five complete flat-ground pose snapshots preserve authored ankle travel within 3mm after the shared floor correction, and native leg lengths within 0.5mm.')
   limitations.append('Native IK changes knee roll during crouch (up to 10.8mm knee displacement in these samples); anatomical lengths and ankle endpoints are preserved, but close deformation review remains relevant.')
  row=dict(video=video.name,character=role,label=role.title()+' · '+category,kind='game',actual_capture_hz=hz,container_fps=s['avg_frame_rate'],timing='Actual Windows Graphics Capture timestamps; no padding or speedup.',gameplay_bound=True,complete=False,visual_accepted=False,scope=d['scope'],dll_sha256=digest,sha256=sha(video),capture=s,evidence=f'{take}/result.json',requirements=requirements,tests=[f'{take}/result.json']+checks,
   visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,observations=observations),limitations=limitations)
  if 'chapters'in d:row['chapters']=d['chapters']
  rows=[r for r in rows if r['video']!=video.name]+[row]
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
(archive/'manifest.json').write_text(json.dumps(dict(dll=digest,bank='D2MOVE16 /141 clips',complete=False,visual_accepted=False,tests=tests,checks=40,scope='Scoped native foot cast/solve/reference consistency and continuous foot history. Flat grounded combat, airborne handoff and one native staircase on both roles. Experimental default off; broader regressions and full movement remain.'),indent=2),encoding='utf-8')
print('Indexed native feet:',len(rows),'actual game recordings; movement complete: false')
