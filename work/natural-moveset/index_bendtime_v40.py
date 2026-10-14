"""Index inspected native/paired footage; retain the failed sparse-sampling take."""
import json,hashlib,datetime,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['corvo_bend_time_cast_end'];cases=item.setdefault('native_integration',{})
for take,times,note in [
 ('native-bendtime-corvo-v39',[1.7,2.4,2.8,3.4,5.8,6.5],'Native first-person cast/end and screen response observed; source reference only.'),
 ('bendtime-corvo-body-v40-v1',[1.8,2.5,2.8,3.2,5.8,6.3],'Both remote gestures visible, torso participates and sword stays attached. External observations miss one short start phase; failed harness preserved. No complete quality acceptance.'),
 ('bendtime-corvo-body-v40-v2',[1.6,2.1,2.5,2.9,5.6,6.1],'Remote Corvo full-body start and release are visible and return to standing. Sword remains in the hand in these views; precise finger contact and movement contexts remain separate. Eight scoped checks pass. Viewer FX still absent.')]:
 f=P/take;r=read(f/'result.json');paired=take.startswith('bendtime-corvo-body');sheet='body-sheet.jpg'if paired else'native-sheet.jpg';assert(f/sheet).exists()
 review=dict(inspected=True,method='Viewed actual-game temporal contact sheet',times_seconds=times,evidence=sheet,notes=note,visual_accepted=False,complete=False,inspected_at=datetime.datetime.now().astimezone().isoformat());write(f/'visual-review.json',review)
 for suffix in(['','-source']if paired else['']):
  video=P/(take+suffix+'.mp4');assert video.exists()
  ff=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));num,den=map(float,ff['streams'][0]['avg_frame_rate'].split('/'));hz=num/den
  row=dict(video=video.name,character='corvo',label='Corvo · Bend Time · '+('source reference'if suffix or not paired else'paired body')+(' · sampling check failed'if not r['passed']else''),alias='native_phase_bend_time_in',requirements=['corvo_bend_time_cast_end'],gameplay_bound=paired and not suffix,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/visual-review.json'if not suffix else None,proxy_sha256=r['dll'],bank_sha256=r.get('bankSha256'),actual_capture_hz=hz,video_sha256=sha(video),checks_passed=r['passed'],scope=r['scope'])
  videos=[v for v in videos if v['video']!=row['video']];videos.append(row);cases[video.name]=row
item['notes']='Native Corvo source acquisition/toggle/cleanup verified. Exact armed standing start (533ms, owned left arm plus player) and release (433ms, owned left arm) drive a full-body remote cast over Steam in v40; four phases from two toggles and recovery pass eight scoped checks. Asset-only hold is unbound. Native crouched holstered observations exist; context port, native viewer effects, lifecycle and full visual acceptance remain unfinished.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(actual_game_videos=len(videos),bendtime_videos=len(cases),complete=False)))
