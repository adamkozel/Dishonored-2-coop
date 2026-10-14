"""Index inspected, real-time D2 recordings without marking powers complete."""
import json,hashlib,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent;M=R/'work/movement-integration'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='3EC71AF4416817FDB70057CC50C7603B30B14F0371A6CE756DF0D7671A2FA47F'
assert sha(R/'work/asset-game/dinput8.dll')==digest
for f in read(M/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
rows=read(P/'runtime-video-manifest.json')
for take,role,req in [('integrated-blink-fx-v1','corvo','corvo_blink_aim_cancel_release_arrive'),('integrated-power-emily-v3','emily','emily_far_reach_aim_cancel_release_arrive')]:
 d=read(P/take/'result.json');assert d['failure'] is None and d['dll']==digest and d['recorded'];assert(P/take/'contact.jpg').exists()
 video=P/f'{take}.mp4';stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=11.8
 blink=role=='corvo'
 row=dict(video=video.name,character=role,label=role.title()+' · '+('Native Blink · body + world FX'if blink else'Native Far Reach · body phases, VFX pending'),kind='game',actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Actual Windows Graphics Capture timestamps; real elapsed time, no repeated padding frames or speedup.',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Native source power phases drive authored full-body motion through Steam. '+('Confirmed native Blink travel sends scoped departure/arrival events, native Doppelganger FX and a discrete foot-position transfer.'if blink else'Exact native Far Reach cast/pull phases drive the body. Native tether, impact effects and target replication remain missing.'),
  chapters=[dict(time=1.7,label='Native aim / full-body preparation'),dict(time=2.9,label='Release and travel'),dict(time=4.0,label='Standing recovery'),dict(time=6.2,label='Aim and native cancellation')],
  dll_sha256=digest,bank_sha256=d['bank'],sha256=sha(video),capture=stream,evidence=f'{take}/result.json',requirements=[req],tests=[f'{take}/result.json'],
  visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,observations=['Complete body remains visible through preparation, travel and recovery.','Cancellation returns to standing without relocating the source.']+(['Native blue departure/arrival particles appear at distinct world endpoints; the body no longer jumps to eye height.','Arrival effect briefly obscures fine hand and foot detail.']if blink else['The world tether is visibly absent; this take establishes body integration only.','Forward pull posture and recovery are visible; other target/direction/stance cases remain untested.'])),
  limitations=['Armed, standing Dunwall fixture only.','Crouched, moving, airborne, equipment variants and upgrades remain incomplete.','Full interruption, map/peer lifecycle and final visual acceptance remain pending.']+([]if blink else['Native world tether/impact/target ownership is not integrated.']))
 rows=[r for r in rows if r['video']!=video.name]+[row]
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
facts=dict(dll_sha256=digest,bank='D2MOVE15 / 126 clips',action_protocol='14 / 236 bytes',blink_fx_protocol='1 / 116 bytes; native Steam reliable events',complete=False,visual_accepted=False,
 body_checks=13,tests=['integrated-blink-fx-v1/result.json','integrated-power-emily-v3/result.json'],
 confirmed=['Both starting powers drive native-role body phases and recover through native cancellation.','Blink state12 eye/foot relationship and completion bit correlate with actual native casts.','Blink departure and arrival effects use the native FX manager with exact owner/ID/declaration identity.','Successful Blink produces two FX events; aim cancellation produces none and duplicate arrival cannot replay effects.'],
 pending=['Far Reach tether/impact/target representation.','Other powers and starting-power variants.','Power interruption/lifecycle matrix, broader movement regressions and final visual acceptance.'],
 rollback='work/movement-integration/verified-blink-network-3EC7',protected_files_unchanged=True)
(M/'starting-power-integration-findings.json').write_text(json.dumps(facts,indent=2),encoding='utf-8')
p=P/'movement-phase-contract.json';contract=read(p)
for case in contract['power_cases']:
 if case['id']=='blink':case['partial_runtime_evidence']={'scope':'Standing armed, scoped Dunwall native Blink departure/arrival, cancellation and replay refusal.','recording':'integrated-blink-fx-v1.mp4','remaining':'Other stances/motion, upgrades, interruptions, maps and complete lifecycle verification.'}
 if case['id']=='far_reach':case['partial_runtime_evidence']={'scope':'Standing armed body phases only.','recording':'integrated-power-emily-v3.mp4','remaining':'Native world tether/impact/targets and the complete power variants/lifecycle.'}
p.write_text(json.dumps(contract,indent=2),encoding='utf-8')
p=R/'work/current-task-state.json';state=read(p);state['UpdatedAt']=datetime.datetime.now().astimezone().isoformat()
state['MovementIntegration']['LatestContinuation']=dict(complete=False,live_dll=digest,live_bank=facts['bank'],rollback=facts['rollback'],dashboard_game_recordings=len(rows),current_work='Starting powers are body-bound; scoped native Blink world FX and endpoint transfer pass eight checks, Emily body path passes five. Far Reach world effects, lifecycle/variant coverage and many movement requirements remain unfinished.')
p.write_text(json.dumps(state,indent=2),encoding='utf-8')
print('Indexed starting powers; actual D2 recordings:',len(rows),'complete: false')
