"""Record actual travelling sword evidence without treating it as full coverage."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
digest='A0B648425DA7C8EB9D5C138BF0550434E124E6574D0085F5021D14691C346471'
assert sha(R/'work/movement-integration/candidate-moving-A0B6/dinput8.dll')==digest
rows=read(P/'runtime-video-manifest.json')
for role in ('corvo','emily'):
 take=f'integrated-moving-combat-{role}-v1';d=read(P/take/'result.json')
 assert d['failure']is None and d['dll']==digest and len(d['checks'])==8 and d['recorded']
 assert(P/take/'contact.jpg').exists();video=P/f'{take}.mp4'
 s=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
 hz=int(s['nb_frames'])/float(s['duration']);assert hz>=24 and float(s['duration'])>=23.8
 row=dict(video=video.name,character=role,label=role.title()+' · Travelling sword actions',kind='game',actual_capture_hz=hz,container_fps=s['avg_frame_rate'],timing='Actual Windows Graphics Capture timestamps; real elapsed time, no padding or speedup.',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
  scope='Native walking/sneaking with sword strikes and guard over Steam. Pelvis, legs and corrective descendants retain the travelled gait while the spine, arms and sword follow the authored action. All semantic transitions now share the delayed root clock.',
  chapters=d['chapters'],dll_sha256=digest,bank_sha256=d['bank'],sha256=sha(video),capture=s,evidence=f'{take}/result.json',requirements=['moving_attack','crouched_attack','block_enter_hold_exit'],tests=[f'{take}/result.json'],
  visual_review=dict(contact_sheet=f'{take}/contact.jpg',accepted=False,observations=['Forward, side and backward strides remain visible during the sword action.','A moving guard and crouched strike recover to the travelling/standing poses.','The wide view demonstrates whole-body travel; closer foot and hand inspection remains needed.']),
  limitations=['Ordinary isolated Dunwall floor with selected strike/guard variants.','Running/heavy/combo/direction-change variants and collision contacts need broader evidence.','No final visual acceptance or complete moveset claim.'])
 rows=[r for r in rows if r['video']!=video.name]+[row]
(P/'runtime-video-manifest.json').write_text(json.dumps(rows,indent=2),encoding='utf-8')
(R/'work/movement-integration/moving-combat-findings.json').write_text(json.dumps(dict(complete=False,visual_accepted=False,dll=digest,bank='D2MOVE16 / 141 clips',tests=[f'integrated-moving-combat-{r}-v1/result.json'for r in ('corvo','emily')],checks=16,
 confirmed=['Both roles perform forward, side, backward and crouched moving sword strikes and forward moving guard.','Movement cadence continues during the authored sword action, with stopped recovery.','Movement semantics use the same delayed timeline as root and native scene phases.','Health, native first person, sword attachment and protected files stay unchanged.'],
 pending=['Close foot/hand review and remaining direction, speed, action and contact cases.','Full movement integration, lifecycle and acceptance.'],rollback='work/movement-integration/candidate-moving-A0B6'),indent=2),encoding='utf-8')
print('Indexed travelling combat:',len(rows),'actual game recordings; movement complete: false')
