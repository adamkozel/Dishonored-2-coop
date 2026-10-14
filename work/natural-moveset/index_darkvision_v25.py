"""Index inspected native/remote Dark Vision recordings with bounded claims."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(x,indent=2),encoding='utf-8');t.replace(p)
rows=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json')
for role in ('corvo','emily'):
 take=f'darkvision-{role}-v25-v2';folder=P/take;r=read(folder/'result.json')
 assert r['passed'] and r['recorded'] and len(r['checks'])==8 and len(r['nativeScenes'])==2 and len(r['seenEvents'])==2
 bank=next(x for x in read(P/'darkvision-bank-manifest-v1.json')if x['role']==role)
 assert bank['sha256'].upper()==r['bankSha256'] and bank['prior_payload_unchanged']
 review=dict(inspected=True,accepted=False,method='Selected actual D2 frames inspected; complete temporal visual approval remains pending.',
  contact_sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg'],
  observations=['Remote full-body hand raise, torso/shoulder response and standing recovery are visible. Native sword follows the right hand.',
   'Closer views include both boots and knees. No gross boot/trouser separation is apparent in these standing-cast frames; other stances are not covered.',
   'Caster POV shows native hand animation, Dark Vision icon, orange reveal overlay and return to ordinary vision. Observer POV keeps ordinary vision.'],
  limitations=['No crouched, moving, unarmed or airborne cast coverage yet.','Fine finger contact and continuous motion quality require further visual inspection.',
   'Native first-person reveal is confirmed; no independent remote world-effect claim is made.',
   'Logged input/videoTime values are relative to recorder process launch, not exact per-frame camera synchronization. Video timestamps are preserved without retiming.'])
 write(folder/'visual-review.json',review)
 req=f'{role}_dark_vision_cast_end';paths=[];evidence=f'{take}/result.json'
 for suffix in ('','-source'):
  movie=P/(take+suffix+'.mp4');s=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(s['nb_frames'])/float(s['duration']);assert hz>=24
  row=dict(video=movie.name,character=role,label=f'{role.title()} · Dark Vision · '+('native caster POV'if suffix else'remote full-body cast'),
   kind='game',gameplay_bound=True,complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),
   actual_capture_hz=hz,container_fps=s['avg_frame_rate'],capture=s,timing='Actual capture timestamps; no padding or speedup. Separate camera recordings are not frame-synchronized.',
   evidence=evidence,tests=[evidence],requirements=[req],scope=r['scope'],visual_review=review,limitations=review['limitations'])
  rows=[x for x in rows if x['video']!=movie.name];rows.append(row);paths.append(movie.name)
 item=annotations['items'][req]
 item.update(runtime_binding='partial_native_Dark_Vision_cast_clock_through_Steam',visual_validation='bounded_actual_game_frames_inspected',phase_status='work_remaining',
  notes='Armed standing cast and native toggle-off use the actual 933ms source scene and reciprocal left arm. Native Steam carries phase to a whole-body actor performance. Native first-person hand/camera and viewer-only vision remain authoritative. Both roles pass recovery, equipment, health, cleanup and protected-file checks. Remaining stance/motion/weapon/lifecycle cases are not implied by these takes.',
  in_game_recordings=paths,evidence=[evidence],native_effects_status='native_caster_vision_on_off_and_viewer_scope_verified_in_standing_fixture')
 item['motions']=list(dict.fromkeys(item['motions']+['native_phase_dark_vision_fire']))
write(P/'runtime-video-manifest.json',rows);write(P/'review-evidence-annotations.json',annotations)
contract=read(P/'movement-phase-contract.json')
for p in contract['power_cases']:
 if p['id']=='dark_vision':p['partial_runtime_evidence']=dict(scope='Both roles armed/standing; native toggle on/off, private overlay/reveal, remote full body, sword/health/cleanup.',recordings=[f'darkvision-{r}-v25-v2.mp4'for r in ('corvo','emily')],natural_expiry='native-darkvision-corvo-v4/result.json',remaining='Stance/movement/equipment variants, independent world-visible effect determination, broad lifecycle and full visual approval.')
write(P/'movement-phase-contract.json',contract)
print('Indexed 4 actual game views;',len(rows),'actual-game recordings. Complete movement remains false.')
