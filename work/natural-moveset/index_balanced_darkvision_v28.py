"""Index inspected native balanced-crouch Dark Vision regressions."""
import json, hashlib, subprocess, datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v): p.write_text(json.dumps(v,indent=2),encoding='utf-8')
manifest=read(P/'runtime-video-manifest.json'); annotations=read(P/'review-evidence-annotations.json')
tests=[]; videos=[]
for take in ('darkvision-corvo-balanced-v28-v1','darkvision-emily-balanced-empty-moving-v28-v1'):
 r=read(P/take/'result.json'); assert r['passed']
 req=f"{r['character']}_dark_vision_cast_end"
 sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg']
 for s in sheets: assert (P/s).is_file()
 review=dict(inspected=True,accepted=False,method='Actual source and remote temporal frames inspected.',contact_sheets=sheets,
  observations=['The complete crouched body casts and returns to the balanced crouch idle.','The casting hand extends forward; the native first-person controls and private reveal remain intact.','Emily also travels forward/backward with the sword holstered; Corvo retains his drawn sword.'],
  limitations=['Only the named equipment and travel contexts were exercised.','Arbitrary interruptions, all directions and continuous visual polish remain incomplete.','No audio was captured.'])
 write(P/take/'visual-review.json',review)
 case=[]
 for suffix in ('','-source'):
  movie=P/f'{take}{suffix}.mp4'
  c=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(c['nb_frames'])/float(c['duration']); assert hz>=24
  row=dict(video=movie.name,character=r['character'],label=take.replace('-',' ')+(' · native source'if suffix else' · remote body'),kind='game',gameplay_bound=True,travelling=r['moving'],complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=c['avg_frame_rate'],capture=c,timing='Actual capture timestamps; no padding, interpolation or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=[req],scope=r['scope'],visual_review=review,limitations=review['limitations'])
  manifest=[x for x in manifest if x['video']!=movie.name]; manifest.append(row); case.append(movie.name)
 item=annotations['items'][req]
 item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+case))
 item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{take}/result.json']))
 tests.append(f'{take}/result.json'); videos+=case
write(P/'runtime-video-manifest.json',manifest); write(P/'review-evidence-annotations.json',annotations)
write(P/'balanced-darkvision-findings-v28.json',dict(complete=False,dll=r['dll'],checks=19,tests=tests,recordings=videos,confirmed=review['observations'],remaining=review['limitations']))
state=P.parent/'current-task-state.json'; s=read(state); s['MovementIntegration']['LatestContinuation'].update(actual_game_videos=len(manifest),next='Balanced Dark Vision regressions passed both roles. Recording native slide recovery/interruption; separately investigate native Slipping state discovered on unsuitable camera-test geometry.',complete=False); s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat(); write(state,s)
print('Indexed four native Dark Vision videos; total',len(manifest))
