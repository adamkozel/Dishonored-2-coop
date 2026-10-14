"""Index the inspected, actual native slide/recovery matrix; retain failed takes."""
import json, hashlib, subprocess, datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v): p.write_text(json.dumps(v,indent=2),encoding='utf-8')
manifest=read(P/'runtime-video-manifest.json'); annotations=read(P/'review-evidence-annotations.json')
tests=[]; videos=[]; checks=0
for take in read(P/'slide-matrix-v28-takes.json'):
 r=read(P/take/'result.json'); assert r['passed'] and len(r['checks'])==7
 reqs=['slide_enter_loop_exit','slide_to_crouch_or_stand']; sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg']
 for s in sheets: assert (P/s).is_file()
 review=dict(inspected=True,accepted=False,method='Temporal contact sheets from the actual source and remote recordings inspected.',contact_sheets=sheets,
  observations=['Native sprint/crouch input drives the complete remote body through slide and authored recovery over Steam.',f"The tested exit is {r['exit']}, with sword {'holstered' if r['holstered'] else 'drawn'}.",'The local native first-person hands, leg, camera banking and movement remain intact.'],
  limitations=['Wide, backlit view and partial initial frame-edge cropping limit fine grip and calf assessment.','Furniture sometimes occludes lower legs near the final position.','Continuous visual acceptance, low ceilings, slopes, combat and arbitrary interruptions remain incomplete.','No audio was captured.'])
 write(P/take/'visual-review.json',review); case=[]
 for suffix in ('','-source'):
  movie=P/f'{take}{suffix}.mp4'
  c=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(c['nb_frames'])/float(c['duration']); assert hz>=24
  row=dict(video=movie.name,character=r['character'],label=take.replace('-',' ')+(' · native source' if suffix else ' · remote body'),kind='game',gameplay_bound=True,travelling=True,complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=c['avg_frame_rate'],capture=c,timing='Actual capture timestamps; no padding, interpolation or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=reqs,scope=r['scope'],visual_review=review,limitations=review['limitations'])
  manifest=[x for x in manifest if x['video']!=movie.name];manifest.append(row);case.append(movie.name)
 for req in reqs:
  item=annotations['items'][req];item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+case));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{take}/result.json']))
 tests.append(f'{take}/result.json');videos+=case;checks+=len(r['checks'])
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
write(P/'slide-matrix-findings-v28.json',dict(complete=False,dll=r['dll'],checks=checks,tests=tests,recordings=videos,confirmed=['Twelve role/equipment/exit cases passed seven checks each.','All twenty-four temporal source/remote contact sheets inspected.'],remaining=review['limitations'],rejected=['Corvo empty jump v8 entered native Vault ID8 beside furniture; v9 uses neutral-stick jump and verifies actual Jump.','Corvo crouch v2 on unsuitable geometry entered distinct native Slipping ID5, not Slide ID7. Slipping remains unsupported.']))
state=P.parent/'current-task-state.json';s=read(state);s['MovementIntegration']['LatestContinuation'].update(actual_game_videos=len(manifest),slide_matrix_checks=checks,complete=False,next='Native ranged equipment/action tracing and reversible acquisition; complete movement port remains in progress.');s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(state,s)
print('Indexed 24 native slide matrix videos; total',len(manifest))
