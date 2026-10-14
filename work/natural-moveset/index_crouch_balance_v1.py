"""Publish inspected in-game comparisons without claiming moveset completion."""
import json,hashlib,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
manifest=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json');added=[]
cases=[]
for role in ('corvo','emily'):
 for candidate in (False,True):
  take=f'crouch-balance-{role}-{"candidate"if candidate else"baseline"}-v1'
  cases.append((take,['crouch_idle','stand_to_crouch','crouch_to_stand','crouch_stop_start'],False,
   ['Whole-body standing, crouch, forward travel, stopping and standing recovery are visible.',
    'Candidate removes the low asymmetric near-kneeling rest pose and keeps idle closer to the walking pelvis height.' if candidate else 'Baseline shows the low asymmetric crouched rest pose used for comparison.',
    'Existing distance-controlled gait and native first-person animations are unchanged.'],
   ['Forward-only comparison; other crouched actions and directions still need contextual review.','Selected temporal frames inspected; full continuous quality is not accepted.']))
for take in ('grip-closeup-corvo-v2','grip-closeup-emily-v1'):
 cases.append((take,['sword_ready_idle'],False,
  ['The close game view shows the standing hand closed around the native sword handle.','Stable final skeleton and rendered attachment are recorded independently; no new finger edits were needed.'],
  ['Partial-body standing diagnostic; does not establish crouched/combat grip or complete motion quality.']))
for take in ('windblast-corvo-balanced-network-v28-v3','windblast-corvo-balanced-empty-moving-v28-v3'):
 cases.append((take,['corvo_windblast_cast'],True,
  ['The crouched cast extends the casting arm forward and returns to the balanced full-body idle.','Both source and remote cast times show native world wind particles.','Native private screen distortion remains confined to the source view.','Sword state remains consistent with the drawn/holstered test.'],
  ['Basic Corvo Windblast only; all upgrades, impacts and lifecycle cases remain incomplete.','Foot sliding, cancellation and continuous transition quality still require broader review.','No audio is recorded; sound is not audibly verified.']))
for take,requirements,paired,observations,limitations in cases:
 r=read(P/take/'result.json');assert r['passed'],take
 sheets=[f'{take}/frame.png']if take.startswith('grip-')else[f'{take}/contact.jpg']
 if paired:sheets.append(f'{take}/source-contact.jpg')
 for sheet in sheets:assert(P/sheet).is_file(),sheet
 review=dict(inspected=True,accepted=False,method='Actual game temporal-frame comparison and close-up inspection.',contact_sheets=sheets,observations=observations,limitations=limitations)
 write(P/take/'visual-review.json',review)
 videos=[]
 for suffix in ('','-source')if paired else ('',):
  movie=P/f'{take}{suffix}.mp4'
  capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24,(take,hz)
  role=r.get('character','corvo');label=take.replace('-',' ')+(' · native source'if suffix else' · remote view')
  row=dict(video=movie.name,character=role,label=label,kind='game',gameplay_bound=True,travelling=r.get('moving',take.startswith('crouch-balance-')),complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],capture=capture,timing='Actual capture timestamps; no padding, interpolation or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=requirements,scope=r['scope'],visual_review=review,limitations=limitations)
  manifest=[v for v in manifest if v['video']!=movie.name];manifest.append(row);videos.append(movie.name);added.append(movie.name)
 for req in requirements:
  item=annotations['items'][req]
  item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+videos))
  item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{take}/result.json']))
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
findings=dict(complete=False,confirmed=['Both character comparisons favor the complete-body Posture-100 mean idle over the low asymmetric previous rest pose.','Standing close-up views show closed native grips on both characters.','Crouched Windblast revision 3 passes two native casts, matching native world effect placement and cleanup, with a forward casting gesture.','Holstered forward/backward travelling Windblast preserves body ownership, gait advance and source-only private distortion.'],rejected=[dict(candidate='balanced-crouch-power-banks-v1',reason='Native FX numerical checks passed, but the crouched local shoulder offsets pointed the gesture downwards in game.'),dict(candidate='balanced-crouch-power-banks-v2',reason='Offline positional arm targets needed 0.312m/0.261m reach clamps; rejected before runtime testing.')],candidate='balanced-crouch-power-banks-v3',recordings=added,remaining=['Crouched sword orientation needs broader visual comparison across actions.','Other crouched contexts still need rebaking around the balanced rest pose.','Air, lateral travel, interruption and lifecycle cases are not established by these comparisons.'])
write(P/'crouch-balance-findings-v1.json',findings)
state=P.parent/'current-task-state.json';s=read(state);s['MovementIntegration']['LatestContinuation'].update(actual_game_videos=len(manifest),next='Balanced crouch idle and forward crouched Windblast candidate recorded, including holstered travel. Standing grips confirmed in close-up. Investigating native airborne Windblast before expanding support.',complete=False);s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(state,s)
print('Actual game recordings:',len(manifest),'Newly indexed comparisons:',len(added),'Complete: false')
