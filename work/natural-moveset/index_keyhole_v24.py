"""Index verified actual-game keyhole takes; keep failed and superseded trials separate."""
import hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(x,indent=2),encoding='utf-8');t.replace(p)
rows=read(P/'runtime-video-manifest.json');added=[]
for take in sys.argv[1:]:
 folder=P/take;result=read(folder/'result.json');assert result['passed'] and result['recorded']
 visual=read(folder/'visual-review.json');assert visual['inspected']
 movie=P/(take+'.mp4');probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
 hz=int(probe['nb_frames'])/float(probe['duration']);assert hz>=24
 pause='reverse'not in result
 role='corvo'if result['source']=='host'else'emily';side='right'if result.get('reverse')else'left';stance='crouched'if result['crouchedStart']else'standing'
 bank=next(b for b in read(P/f"keyhole-bank-manifest-v{result.get('bankRevision',2)}.json")if b['role']==role)
 if pause:
  # These original pause captures predate the explicit SHA field. Their
  # pinned bank checksum must equal a SHA-identified standard capture.
  ref=read(P/f'keyhole-{role}-left-stand-v24-v6/result.json')
  baseline=read(P/f'keyhole-{role}-left-stand-v24-v6/states.json')[0]['movement']['checksum']
  observer='client'if result['source']=='host'else'host'
  states=read(folder/'states.json')
  pinned=[s['movement']['checksum']for s in states if s['role']==observer and s['label']in ('before-pause','resumed')]
  assert len(pinned)==2 and all(c==baseline for c in pinned) and ref['bankSha256']==bank['sha256'].upper()
  result['bankSha256']=ref['bankSha256']
 else:assert result['bankSha256']==bank['sha256'].upper()
 label=f'{role.title()} · keyhole {side} · {stance} · drawn / holstered'
 if pause:label=f'{role.title()} · keyhole {stance} · native pause and resume'
 elif result['exitRequestMs']<2200:label+=' · early exit'
 row=dict(video=movie.name,character=role,label=label,kind='game',gameplay_bound=True,complete=False,visual_accepted=False,dll_sha256=result['dll'],bank_sha256=result['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=probe['avg_frame_rate'],capture=probe,timing='Actual capture timestamps; no padding or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=['keyhole_enter_look_exit'],scope=result['scope'],visual_review=visual,limitations=['One exact Dunwall office door. Other geometry and full death/load/session lifecycle remain to verify.','Known unaccepted earlier crouch donor and reverse-camera fixture trials are preserved outside the verified capture list.','Native first person, world position and gameplay outcomes stay authoritative. The remote pose emits no gameplay events.'])
 rows=[r for r in rows if r['video']!=movie.name];rows.append(row);added.append(row)
write(P/'runtime-video-manifest.json',rows)
a=read(P/'review-evidence-annotations.json');item=a['items']['keyhole_enter_look_exit']
all_keyhole=[r for r in rows if r.get('requirements')==['keyhole_enter_look_exit']]
item.update(runtime_binding='partial_native_keyhole_phases_through_Steam',visual_validation='bounded_actual_game_recordings_inspected',phase_status='work_remaining',notes='Owned native Peeking scenes select full-body NPC-derived entry/look/exit, both door sides and original stances. Native sword holster/recovery and bounded native exit handoff are replicated. Reviewed captures identify the tested roles and variants; global movement completion is not implied.',in_game_recordings=[r['video']for r in all_keyhole],evidence=[r['evidence']for r in all_keyhole])
item['motions']=list(dict.fromkeys(item['motions']+[r['selection']['alias']for r in read(P/'keyhole-source-manifest-v2.json')]))
write(P/'review-evidence-annotations.json',a)
print('Indexed',len(added),'keyhole recordings;',len(rows),'actual game videos; movement phase incomplete')
