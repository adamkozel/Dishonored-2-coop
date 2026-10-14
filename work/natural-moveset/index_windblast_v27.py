"""Index the bounded native Windblast body proof, without claiming world FX."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,o):p.write_text(json.dumps(o,indent=2),encoding='utf-8')
take='windblast-corvo-body-v27-v1';folder=P/take;r=read(folder/'result.json')
assert r['passed'] and len(r['checks'])==8 and len(r['nativeScenes'])==len(r['seenEvents'])==2
assert r['dll']=='C3B2A7717B973CBEFF8A27E0AF480C71F6D0CAA5E16FBB2E2F2C9198E6E41E0F'
review=dict(inspected=True,accepted=False,method='Selected temporal frames of actual source and remote D2 recordings inspected.',
 contact_sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg'],
 observations=['Remote Corvo shows a complete-body casting stance, left-arm extension and standing recovery on both native casts.',
  'Original native first-person cast and screen distortion appear in the source view; observer camera stays unchanged.'],
 limitations=['Only standing, sword drawn, stationary basic Windblast is integrated.',
  'Remote world effects, impacts and gameplay outcomes are not implemented by this build.',
  'Fine hand-to-hilt contact and complete continuous visual quality remain unaccepted.'])
for f in review['contact_sheets']:assert (P/f).is_file()
write(folder/'visual-review.json',review)
manifest=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json');paths=[]
req='corvo_windblast_cast';evidence=f'{take}/result.json'
for suffix in ('','-source'):
 movie=P/f'{take}{suffix}.mp4';capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
 hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
 row=dict(video=movie.name,character='corvo',label='Corvo · Windblast · '+('native caster POV' if suffix else 'remote full body'),kind='game',gameplay_bound=True,travelling=False,complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],capture=capture,timing='Actual capture timestamps; no padding, interpolation or speedup.',evidence=evidence,tests=[evidence],requirements=[req],scope=r['scope'],visual_review=review,limitations=review['limitations'])
 manifest=[x for x in manifest if x['video']!=movie.name];manifest.append(row);paths.append(movie.name)
item=annotations['items'][req];item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+paths));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[evidence]));item['notes']=r['scope']
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
contract=read(P/'movement-phase-contract.json')
for power in contract['power_cases']:
 if power['id']=='windblast':power['partial_runtime_evidence']=dict(scope=r['scope'],recordings=paths,tests=[evidence],dll=r['dll'],checks=r['checks'],remaining=review['limitations'])
write(P/'movement-phase-contract.json',contract)
state=P.parent/'current-task-state.json';s=read(state);c=s['MovementIntegration']['LatestContinuation'];c.update(live=r['dll'],bank='D2MOVE27 /198 clips; windblast-banks-v1; prior197 payload unchanged',actual_game_videos=len(manifest),next='Native Windblast standing armed body passes8 checks. Read-only native effect-map observation in progress before remote effect replication.',complete=False)
write(state,s)
print(f'Indexed Windblast body/source recordings: {len(manifest)} total; movement incomplete.')
