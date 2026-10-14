"""Preserve tested native vault candidate and inspected actual-game recordings."""
import json,hashlib,shutil,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent;M=ROOT/'work/movement-integration'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
tests=['vault-v14c-test','vault-v14c-priority'];dll=ROOT/'work/asset-game/dinput8.dll';digest=sha(dll)
assert digest=='B1CB32BAF8E581F46759DCFA019D84BD59229DBFE13E546FF36C1459ACC0CF31'
for t in tests:
 d=read(P/t/'result.json');assert d['passed'] and d['dll']==digest
for f in read(M/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
path=P/'runtime-video-manifest.json';rows=read(path);takes=[]
for role in ('corvo','emily'):
 for unarmed in (False,True):
  name=f'integrated-vault-{role}'+('-unarmed'if unarmed else'')+'-v3';take=read(P/name/'take.json');assert not take['failure'] and take['dll']==digest
  assert (P/name/'contact.jpg').exists();takes.append(name)
  video=P/f'{name}.mp4';stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
  hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=11.8
  row=dict(video=video.name,character=role,label=f'{role.title()} · Native desk vault · '+('weaponless'if unarmed else'sword'),actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.',kind='game',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
   scope='Exact native vault state and owned player/arm scene drive whole-body traversal over Steam. Geometric progress controls animation; obstacle height adjusts visual pelvis lift. Network root and original gameplay outcomes retain authority.',
   chapters=take['chapters'],dll_sha256=digest,bank_sha256=take['bank'],sha256=sha(video),capture=stream,evidence=f'{name}/take.json',requirements=['vault_low_obstacle'],tests=[f'{t}/result.json'for t in tests],
   visual_review=dict(contact_sheet=f'{name}/contact.jpg',observations=['Approach, lifted whole-body vault and running recovery remain inside the wider view.','The window-side view reveals more torso/limb detail than the rejected backlit v1 angle.','The desk and props occlude parts of support contact and the final feet during landing.','Sword visibility follows source equipment; weaponless takes do not draw it.'],accepted=False),
   limitations=['One native Dunwall desk trajectory; other obstacles, window vaults and high variants remain unverified.','Palm and foot contacts are not solved for arbitrary geometry.','Some contact and landing detail is occluded, so these recordings establish integration rather than complete visual acceptance.'])
  rows=[r for r in rows if r['video']!=video.name]+[row]
path.write_text(json.dumps(rows,indent=2),encoding='utf-8')
out=M/'verified-vault-B1CB'
if not out.exists():
 out.mkdir();shutil.copytree(ROOT/'outputs/d2-coop-lab/src',out/'src');shutil.copy2(dll,out/'dinput8.dll');shutil.copytree(P/'vault-banks-v1',out/'banks')
 for n in ('build_vault_bank.py','vault-bank-manifest-v1.json','Test-Vault.ps1','Record-IntegratedStance.ps1'):shutil.copy2(P/n,out/n)
 for t in tests:shutil.copytree(P/t,out/t)
facts=dict(dll_sha256=digest,bank_format='D2MOVE14 / 117 clips',action_protocol='13 / 224 bytes',tests=tests,checks=sum(len(read(P/t/'result.json')['checks'])for t in tests),complete=False,visual_accepted=False,protected_saves_unchanged=True,
 binding='Native state9 VT1c86e18. Approach/exit have no scene handle; active vault_1/2 has exact player and both owned arms, Corvo Scene_2 / Emily Scene_1, declared933ms. Animation phase follows start/end-plane distance, not the prematurely ended native scene clock.',
 evidence=[f'{n}/take.json'for n in takes],
 limits=['High/window variants and arbitrary support contacts remain unfinished.','v14a failed because an early action handoff reset blend weight and test progress used integer overloads. v14c corrects both; telemetry now samples movement at50ms while active.','Fine support and landing contact remains partly occluded by desk and props.'],created=datetime.datetime.now().astimezone().isoformat())
(out/'verification.json').write_text(json.dumps(facts,indent=2),encoding='utf-8');(M/'vault-integration-findings.json').write_text(json.dumps(facts,indent=2),encoding='utf-8')
path=M/'native-movement-findings.json';d=read(path);d['confirmed_native_activation']['low_vault']=facts;path.write_text(json.dumps(d,indent=2),encoding='utf-8')
path=ROOT/'work/current-task-state.json';d=read(path);d['MovementIntegration']['LatestContinuation']=dict(complete=False,live_dll=digest,live_bank=facts['bank_format'],rollback=str(out.relative_to(ROOT)),dashboard_game_recordings=len(rows),current_work='Native low vault passes both-role/equipment checks and has four actual D2 recordings. Contact acceptance, other traversals, powers and the complete movement phase remain unfinished. Native power loadout/inventory investigation continuing.');path.write_text(json.dumps(d,indent=2),encoding='utf-8')
print('Preserved',out.name,'with',facts['checks'],'checks; indexed',len(rows),'actual game recordings')
