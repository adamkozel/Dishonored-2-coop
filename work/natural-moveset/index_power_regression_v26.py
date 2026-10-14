"""Publish actual v26 starting-power regression videos with their precise scope."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(x,indent=2),encoding='utf-8');t.replace(p)
rows=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json');contract=read(P/'movement-phase-contract.json')
for power,role,req,old in [('blink','corvo','corvo_blink_aim_cancel_release_arrive','integrated-blink-fx-v3.mp4'),('grapple','emily','emily_far_reach_aim_cancel_release_arrive','integrated-grapple-fx-v3.mp4')]:
 take=f'{power}-regression-v26-v2';r=read(P/take/'result.json');assert not r['failure'] and len(r['checks'])==8 and r['recorded']
 row=dict(next(x for x in rows if x['video']==old));movie=P/(take+'.mp4')
 s=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0];hz=int(s['nb_frames'])/float(s['duration']);assert hz>=24
 visual=dict(inspected=True,accepted=False,method='Selected actual D2 temporal frames; full continuous acceptance pending.',contact_sheets=[f'{take}/contact.jpg'],observations=['Whole-body native phase transitions, held sword and standing recovery visible.', 'Departure/arrival smoke is visible; endpoint-only transmission removes the intermediate body location seen in the prior failed regression.' if power=='blink' else 'Native cast and pull body phases, tether/impact and recovery recorded; transient effects require the denser effects contact sheet.'])
 if power=='grapple':visual['contact_sheets'].append(f'{take}/effects-contact.jpg')
 evidence=f'{take}/result.json';row.update(video=movie.name,character=role,label=f'{role.title()} · '+('Blink endpoint fix' if power=='blink' else 'Far Reach native body and VFX regression'),actual_capture_hz=hz,container_fps=s['avg_frame_rate'],timing='Actual Windows capture timestamps; no padding or speedup.',dll_sha256=r['dll'],bank_sha256=r['bank'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),capture=s,evidence=evidence,tests=[evidence],scope=r['scope'],chapters=[],visual_review=visual,complete=False,visual_accepted=False)
 row['limitations']=['Standing, armed Dunwall fixture with one cast and one cancelled aim; upgrades, other stances, moving targets and broader lifecycle remain unverified on this build.','Selected frames and numerical checks are not complete visual acceptance.']
 rows=[x for x in rows if x['video']!=movie.name];rows.append(row);write(P/take/'visual-review.json',visual)
 item=annotations['items'][req];item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+[movie.name]));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[evidence]))
 for case in contract['power_cases']:
  if case['id']==('blink' if power=='blink' else 'far_reach'):case['partial_runtime_evidence']['latest_regression']=dict(recording=movie.name,evidence=evidence,dll=r['dll'],checks=r['checks'])
write(P/'runtime-video-manifest.json',rows);write(P/'review-evidence-annotations.json',annotations);write(P/'movement-phase-contract.json',contract)
print('Indexed starting-power v26 regressions;',len(rows),'actual game recordings; full movement incomplete.')
