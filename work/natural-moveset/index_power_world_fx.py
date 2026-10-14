"""Index inspected native power recordings, preserving explicit coverage gaps."""
import json,hashlib,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent;M=R/'work/movement-integration'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='8E83763193BFE1CD54DF13458BEAEC9C499C2138678C6699F3C176387B03724A'
assert sha(R/'work/asset-game/dinput8.dll')==digest
for f in read(M/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
rows=read(P/'runtime-video-manifest.json')
cases=[('integrated-blink-fx-v3','corvo','corvo_blink_aim_cancel_release_arrive'),('integrated-grapple-fx-v3','emily','emily_far_reach_aim_cancel_release_arrive')]
for take,role,req in cases:
 d=read(P/take/'result.json');assert d['failure'] is None and d['dll']==digest and d['recorded'] and len(d['checks'])==8
 assert(P/take/'contact.jpg').exists();video=P/f'{take}.mp4'
 stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=11.8
 blink=role=='corvo'
 row=dict(video=video.name,character=role,label=role.title()+' · Native '+('Blink'if blink else'Far Reach')+' · body + world FX',kind='game',actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Actual Windows Graphics Capture timestamps; real elapsed time, no repeated padding or speedup.',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Standing armed, isolated Dunwall cast and native cancellation. Native Steam transports full-body phases and scoped native world effects. '+('Blink uses native Doppelganger departure/arrival FX and a discrete foot-position transfer.'if blink else'Far Reach uses the native tether from the authored left wrist to the observed static target, plus the native surface impact; gameplay remains source-controlled.'),
  chapters=[dict(time=1.7,label='Aim / full-body preparation'),dict(time=3.0,label='Native release, travel and effects'),dict(time=4.2,label='Standing recovery'),dict(time=6.2,label='Aim and native cancellation')],
  dll_sha256=digest,bank_sha256=d['bank'],sha256=sha(video),capture=stream,evidence=f'{take}/result.json',requirements=[req],tests=[f'{take}/result.json'],
  visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,observations=['Whole-body preparation, travel and recovery are visible.','Native cancellation creates no world effect or travel.']+(['Native blue departure and arrival effects mark the two world endpoints; no eye-height body jump.','Bright effects briefly obscure fine hand/foot detail.']if blink else['The native dark tether extends from the authored left hand toward the floor target; a native impact effect appears during the pull.','The root-update ordering correction removes the measured fast-pull lag; transition attachment tolerance still needs broader visual inspection.'])),
  limitations=['Standing, armed, known static floor target only.','Crouched, moving, airborne, weaponless, upgrade and dynamic-target variants remain unfinished.','Complete interruption/world/session lifecycle and final visual acceptance remain pending.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
facts=dict(dll_sha256=digest,bank='D2MOVE15 / 126 clips',action_protocol='14 / 236 bytes',blink_fx_protocol='1 / 116 bytes',grapple_fx_protocol='1 / 144 bytes',complete=False,visual_accepted=False,
 body_and_fx_checks=16,tests=[f'{t}/result.json'for t,_,_ in cases]+['integrated-grapple-hand-v2/result.json'],
 confirmed=['Both starting powers have native-role body phases and native world effects over Steam.','Each successful cast produces exactly two scoped effects; aim cancellation produces none.','Duplicate reliable event packets cannot replay effects.','Far Reach native endpoints update after remote root presentation, every native game frame.','Two completed-pose and active-record snapshots during the fast pull match the native wrist exactly.'],
 pending=['A recovery-boundary snapshot shows 0.114 m difference; inspect transition/stop timing across more cases.','Power stance/equipment/direction/upgrades and dynamic targets.','Other powers, complete lifecycle, remaining movement bindings and final visual acceptance.'],
 rollback='work/movement-integration/verified-power-fx-8E83',protected_files_unchanged=True)
(M/'power-world-fx-findings.json').write_text(json.dumps(facts,indent=2),encoding='utf-8')
p=P/'movement-phase-contract.json';contract=read(p)
for case in contract['power_cases']:
 if case['id'] in ('blink','far_reach'):
  take=cases[0 if case['id']=='blink'else 1][0]
  case['partial_runtime_evidence']=dict(scope='Standing armed Dunwall body and native world effects, cancellation and replay refusal.',recording=take+'.mp4',remaining='Stance/movement/equipment/upgrades/dynamic targets, full lifecycle and final visual acceptance.')
p.write_text(json.dumps(contract,indent=2),encoding='utf-8')
p=R/'work/current-task-state.json';state=read(p);state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat()
state['MovementIntegration']['LatestContinuation']=dict(complete=False,live_dll=digest,live_bank=facts['bank'],rollback=facts['rollback'],dashboard_game_recordings=len(rows),current_work='Both starting powers now have scoped native world FX; completing current-build lifecycle and movement regressions, then continuing remaining movement requirements.')
p.write_text(json.dumps(state,indent=2),encoding='utf-8')
print('Indexed native power world effects:',len(rows),'actual D2 recordings; movement complete: false')
