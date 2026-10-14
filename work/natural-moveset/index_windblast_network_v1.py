"""Index visually inspected native captures, preserving incomplete coverage."""
import json,hashlib,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,o):p.write_text(json.dumps(o,indent=2),encoding='utf-8')
manifest=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json');paths=[];tests=[]
for take in ('windblast-corvo-network-v28-v1','windblast-corvo-empty-network-v28-v1'):
 r=read(P/take/'result.json');assert r['passed']and len(r['checks'])==11 and r['matchedNativeEffects']==2
 review=dict(inspected=True,accepted=False,method='Selected temporal frames in both actual game recordings inspected at matched cast times.',contact_sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg'],
  observations=['The remote avatar performs the whole-body cast and recovers its original standing stance.','Native wind particles appear on the cast; background and body remain crisp in the observer view.','Original native fullscreen radial distortion remains in the source view.','Holstered/drawn selection matches the test case.'],
  limitations=['Standing stationary basic Corvo Windblast only.','Fine hand contact and continuous visual quality still require acceptance.','Crouched, travelling, aerial, upgraded and impact variants remain incomplete.','Remote physics/damage outcomes are outside this cosmetic effect proof.','Recordings contain no audio; sound is not audibly verified.'])
 for f in review['contact_sheets']:assert(P/f).is_file()
 write(P/take/'visual-review.json',review);evidence=f'{take}/result.json';tests.append(evidence)
 for suffix in ('','-source'):
  movie=P/f'{take}{suffix}.mp4';capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
  row=dict(video=movie.name,character='corvo',label='Corvo · Windblast · '+('holstered'if r['holstered']else'sword drawn')+' · '+('native source'if suffix else'remote body + native particles'),kind='game',gameplay_bound=True,travelling=False,complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],capture=capture,timing='Actual capture timestamps; no padding, interpolation or speedup.',evidence=evidence,tests=[evidence],requirements=['corvo_windblast_cast'],scope=r['scope'],visual_review=review,limitations=review['limitations'])
  manifest=[x for x in manifest if x['video']!=movie.name];manifest.append(row);paths.append(movie.name)
item=annotations['items']['corvo_windblast_cast'];item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+paths));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+tests));item['notes']=r['scope']
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
contract=read(P/'movement-phase-contract.json')
for power in contract['power_cases']:
 if power['id']=='windblast':power['partial_runtime_evidence']=dict(scope=r['scope'],recordings=paths,tests=tests,dll=r['dll'],checks=r['checks'],remaining=review['limitations'])
write(P/'movement-phase-contract.json',contract)
findings=dict(exact_game_sha256='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180',dll=r['dll'],confirmed=['Native FX ID map provides distinct basic Windblast effects owned by the source native windblast object.','Native FX record addresses are reused; ID, owner and declaration must be checked.','Original wblast_blast_01.fx produces unwanted observer fullscreen distortion when used remotely.','Separate windblast_remote.fx reuses native world particles/sound and removes private screen effects.','Both standing drawn and holstered native casts produce exactly one remote effect with independently matched native position and orientation.','Replayed reliable packets are rejected; finite clone effects stop and tracked ownership clears.'],tests=tests,asset_install='outputs/d2-coop-lab/research/windblast-presentation-asset-install.json',asset_source='outputs/d2-coop-lab/assets/windblast_remote.fx',remaining=review['limitations'],hypotheses=['The map reader can support upgraded Windblast effects after separately proving owner, phase and visual scope.','Native contact-effect observation may support remote impacts; no office-carpet dust contact was observed.'],complete=False)
write(P/'windblast-network-findings-v1.json',findings)
state=P.parent/'current-task-state.json';s=read(state);s['MovementIntegration']['LatestContinuation'].update(live=r['dll'],bank='D2MOVE28 / 201 clips / windblast-context-banks-v1',actual_game_videos=len(manifest),next='Standing Windblast body/native particle network passes drawn and holstered, 22 checks. Comparing complete-body crouch balance and investigating rendered grip before further contextual integration.',complete=False);s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(state,s)
print('Windblast recordings indexed:',len(manifest),'Movement complete: false')
