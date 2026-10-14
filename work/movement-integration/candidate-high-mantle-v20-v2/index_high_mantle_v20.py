"""Index real native high-climb takes without claiming general traversal coverage."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='9393D00E68310D25D1E185BDA46B32F76EDC112189E4863D26B86E43B458B4E9'
archive=P.parent/'movement-integration/candidate-high-mantle-v20-v2'
assert sha(archive/'dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json');tests=[];checks=0
for role,source in [('corvo','host'),('emily','client')]:
 for crouched in [False,True]:
  take=f'high-mantle-{role}'+('-crouched'if crouched else'')+'-v20-lit-v1'
  result=read(P/take/'result.json')
  assert result['passed'] and result['dll']==digest and result['sources']==[source]
  assert result['high'] and result['bankVersion']==20 and result['recorded'] and result['crouched']==crouched
  tests.append(take);checks+=len(result['checks'])
  for name in ['contact-armed.jpg','contact-empty.jpg']:assert(P/take/name).is_file()
  video=P/f'{take}.mp4'
  cap=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
  hz=int(cap['nb_frames'])/float(cap['duration']);assert hz>=24
  row=dict(video=video.name,character=role,label=role.title()+' · high climb → '+('crouch'if crouched else'stand')+' · drawn / holstered',kind='game',gameplay_bound=True,complete=False,visual_accepted=False,dll_sha256=digest,bank_sha256=sha(P/'high-mantle-banks-v1'/f'{role}-high-mantle-bank.bin'),sha256=sha(video),actual_capture_hz=hz,container_fps=cap['avg_frame_rate'],capture=cap,timing='Actual capture timestamps; no padding or speedup. Contact-sheet times are video timestamps, not synchronized native phase measurements.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=['climb_high_ledge'],scope=result['scope'],visual_review=dict(contact_sheets=[f'{take}/contact-armed.jpg',f'{take}/contact-empty.jpg'],accepted=False,scope='Sampled actual D2 frames show rise, tuck and the requested stance recovery. Fine support-hand fit is not resolved at this camera distance.'),limitations=['Observed native kind-2, 833 ms Dunwall canopy climb only.','Authored contact is calibrated to this ledge; arbitrary height, depth, lateral approach and interruption remain unverified.','Full visual acceptance, close grip/contact evaluation and complete movement integration remain unfinished.'])
  rows=[r for r in rows if r['video']!=video.name]+[row]
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
manifest=read(archive/'manifest.json')
manifest.update(runtime_tests=tests,runtime_checks=checks,recordings=[r['video']for r in rows if r.get('dll_sha256')==digest],complete=False,visual_accepted=False)
(archive/'manifest.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
print('Indexed',len(tests),'high-climb recordings;',checks,'checks;',len(rows),'total actual D2 videos; complete:false')
