"""Index new native air-power evidence and previously recorded double jumps."""
import json,hashlib,subprocess,datetime,struct
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
manifest=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json');windVideos=[];windTests=[]
cases=[]
for role in ('corvo','emily'):
 take=f'double-jump-{role}-v21-v1';bank=P/f'double-jump-banks-v1/{role}-double-jump-bank.bin'
 cases.append(dict(take=take,role=role,bank=sha(bank),reqs=['double_jump'],paired=False,observations=['Both stationary and travelling double jumps are visible, with sword drawn and holstered.','The body tucks during the second ascent, extends into the fall, lands and recovers.','Native Agility and exact second-impulse semantics supplied the root and timing; the temporary upgrade was restored.'],limitations=['This is the preserved v21 build recording, not a new v28 runtime test.','Wide, relatively dark framing limits fine grip and lower-leg inspection.','Continuous transition quality, arbitrary geometry, airborne combat and interruption remain unaccepted.'],sheets=[f'{take}/contact-{context}.jpg'for context in ('armed-standing','armed-moving','empty-standing','empty-moving')]))
for take in ('windblast-corvo-air-network-v28-v1','windblast-corvo-air-empty-moving-v28-v1'):
 cases.append(dict(take=take,role='corvo',reqs=['corvo_windblast_cast'],paired=True,observations=['Native jump precedes casting; the remote body follows the airborne arc and cast, then lands and recovers.','Native wind particles appear once for each cast and retain the independently observed native origin and axis.','The observer view keeps its native camera; source fullscreen radial distortion remains private.'],limitations=['Two ordinary jumping basic casts per take, with the explicitly recorded equipment/travel context.','Double-jump casting, upgrades, physics impacts and full lifecycle remain unverified.','Selected temporal frames inspected; complete continuous motion quality is not accepted.','No recorded audio; sound is not audibly verified.'],sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg']))
for c in cases:
 take=c['take'];r=read(P/take/'result.json');assert r['passed']
 for f in c['sheets']:assert(P/f).is_file()
 review=dict(inspected=True,accepted=False,method='Actual game temporal frames of each recorded context inspected.',contact_sheets=c['sheets'],observations=c['observations'],limitations=c['limitations']);write(P/take/'visual-review.json',review)
 videos=[]
 for suffix in ('','-source')if c['paired']else ('',):
  movie=P/f'{take}{suffix}.mp4';capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0];hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
  row=dict(video=movie.name,character=c['role'],label=take.replace('-',' ')+(' · native source'if suffix else' · remote body'),kind='game',gameplay_bound=True,travelling=r.get('moving',True),complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r.get('bankSha256',c.get('bank')),sha256=sha(movie),actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],capture=capture,timing='Actual capture timestamps; no padding, interpolation or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=c['reqs'],scope=r['scope'],visual_review=review,limitations=c['limitations'])
  manifest=[x for x in manifest if x['video']!=movie.name];manifest.append(row);videos.append(movie.name)
 for req in c['reqs']:
  item=annotations['items'][req];item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+videos));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{take}/result.json']))
 if c['paired']:windVideos+=videos;windTests.append(f'{take}/result.json')
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
contract=read(P/'movement-phase-contract.json')
for power in contract['power_cases']:
 if power['id']=='windblast':power['partial_runtime_evidence']['airborne']=dict(recordings=windVideos,tests=windTests,dll=r['dll'],checks=27,remaining=cases[-1]['limitations'])
write(P/'movement-phase-contract.json',contract)
findings=read(P/'windblast-air-native-findings-v1.json');findings['runtime_integration']=dict(dll=r['dll'],tests=windTests,checks=27,recordings=windVideos,visual_inspected=True,visual_accepted=False);findings['hypotheses']=[];findings['remaining']=cases[-1]['limitations'];write(P/'windblast-air-native-findings-v1.json',findings)
state=P.parent/'current-task-state.json';s=read(state);s['MovementIntegration']['liveBuild']=r['dll'];s['MovementIntegration']['current']='v28 movement library, semantic protocol 24; native Windblast world FX now follows ordinary jumping casts. Complete movement is still in progress.';s['MovementIntegration']['LatestContinuation'].update(live=r['dll'],bank='D2MOVE28 / 201 clips; balanced-crouch-power-banks-v3 candidate',actual_game_videos=len(manifest),next='Crouched Dark Vision regression on balanced idle for both roles, then record slide recovery and interruption. Windblast standing/crouched and ordinary air cases have bounded native body/FX evidence.',complete=False);s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(state,s)
print('Actual game recordings:',len(manifest),'Airborne Windblast checks: 27. Movement complete: false')
