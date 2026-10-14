"""Preserve the tested candidate and index only inspected actual-game takes."""
import json,hashlib,shutil,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent;M=ROOT/'work/movement-integration'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
tests=['crouch-mantle-v13c-test','crouch-mantle-v13-priority']
dll=ROOT/'work/asset-game/dinput8.dll';digest=sha(dll)
assert digest=='3A8EE80A3C9FFB248AEAC641B98B45E43E999DF7CB3A0F7660BC6C65324BD147'
for t in tests:
 data=read(P/t/'result.json');assert data['passed'] and data['dll']==digest
for f in read(M/'protected-save-hashes-before-v4.json'):assert sha(Path(f['Path']))==f['Hash']
path=P/'runtime-video-manifest.json';rows=read(path)
for role in ('corvo','emily'):
 name=f'integrated-crouch-mantle-{role}-v3';take=read(P/name/'take.json');assert not take['failure'] and take['dll']==digest
 video=P/f'{name}.mp4';stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(stream['nb_frames'])/float(stream['duration']);assert hz>=24 and float(stream['duration'])>=11.8
 row=dict(video=video.name,character=role,label=f'{role.title()} · Native crouched desk mantle and recovery',actual_capture_hz=hz,container_fps=stream['avg_frame_rate'],timing='Real Windows Graphics Capture timestamps; no speedup or duplicated output frames.',kind='game',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Native crouched low-mantle scene and its scene clock drive complete-body climbing, low recovery and return to standing. Root is network-controlled; exact native fast sword recovery does not interrupt the stream.',
  chapters=take['chapters'],dll_sha256=digest,bank_sha256=take['bank'],sha256=sha(video),capture=stream,evidence=f'{name}/take.json',requirements=['ledge_hang_and_pull_up','crouch_to_stand'],tests=[f'{t}/result.json'for t in tests],
  visual_review=dict(contact_sheet=f'{name}/contact.jpg',observations=['The complete approach, climb, low recovery and final stand stay in frame.','Native crouched exit is preserved; no forced standing at the scene boundary.','The wider view is clear of the bookshelf that obstructed the rejected v1 takes.','Dark clothing, desk props and the camera distance limit fine palm/finger contact inspection.'],accepted=False),
  limitations=['One observed 500 ms low-mantle scene at the Dunwall desk, not evidence for high mantles, vaults or sustained hanging.','Contact is fitted to a measured trajectory; adaptation to varying native climb heights, endpoints and ledge shapes remains incomplete.','Armed footage only; weaponless variants pass runtime checks but are not filmed here.','Visual polish and complete movement acceptance remain pending.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
path.write_text(json.dumps(rows,indent=2),encoding='utf-8')
out=M/'verified-crouch-mantle-3A8E'
if not out.exists():
 out.mkdir();shutil.copytree(ROOT/'outputs/d2-coop-lab/src',out/'src');shutil.copy2(dll,out/'dinput8.dll');shutil.copytree(P/'crouch-mantle-banks-v1',out/'banks')
 for n in ('build_crouch_mantle_bank.py','derive_crouch_mantle_author.py','crouch-mantle-bank-manifest-v1.json','Test-LowMantle.ps1','Record-IntegratedStance.ps1'):shutil.copy2(P/n,out/n)
 for t in tests:shutil.copytree(P/t,out/t)
facts=dict(dll_sha256=digest,bank_format='D2MOVE13 / 115 clips',action_protocol='12 / 208 bytes',tests=tests,checks=sum(len(read(P/t/'result.json')['checks'])for t in tests),complete=False,visual_accepted=False,protected_saves_unchanged=True,
 binding='Native state8 kind1 next6: mantle_low_sneak, Corvo Scene_2 / Emily Scene_1,500ms; actual player only or player plus exact owned right arm. Character comes from the exact player body definition. Crouched recovery sword_equip_sneak_fast333ms binds only the owned right arm.',
 evidence=[f'integrated-crouch-mantle-{r}-v3/take.json'for r in ('corvo','emily')],
 limits=['Arbitrary geometry contacts, other traversals and visual acceptance remain incomplete.','First attempts exposed optional-arm binding and distinct crouched fast recovery; both corrected and the failed evidence retained.','v1 recordings rejected for an obstructed bookshelf camera; Emily observer health changed. Isolated fixture restored, camera moved and NPC isolation moved before camera setup. v3 health checks passed.'],created=datetime.datetime.now().astimezone().isoformat())
(out/'verification.json').write_text(json.dumps(facts,indent=2),encoding='utf-8');(M/'crouch-mantle-integration-findings.json').write_text(json.dumps(facts,indent=2),encoding='utf-8')
path=M/'native-movement-findings.json';d=read(path);d['confirmed_native_activation']['crouched_low_mantle']=facts;path.write_text(json.dumps(d,indent=2),encoding='utf-8')
path=ROOT/'work/current-task-state.json';d=read(path);d['MovementIntegration']['LatestContinuation']=dict(complete=False,live_dll=digest,live_bank=facts['bank_format'],rollback=str(out.relative_to(ROOT)),dashboard_game_recordings=len(rows),current_work='Crouched low mantle and native short sword recovery verified and filmed on both roles. Continuing vault/high-ledge investigation and remaining movement coverage. Complete phase remains unfinished.');path.write_text(json.dumps(d,indent=2),encoding='utf-8')
print('Preserved',out.name,'with',facts['checks'],'checks; indexed',len(rows),'actual game recordings')
