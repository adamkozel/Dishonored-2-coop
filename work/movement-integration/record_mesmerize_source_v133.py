"""Index inspected source-only crouch observations without claiming networking."""
import json,hashlib,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['emily_mesmerize_cast']
for take,note in [('mesmerize-crouch-source-observe-v127','PASS8 observation: slower raw samples captured loop and out but missed entry. Native crouch cast/preview/recovery visible; no remote body evidence.'),('mesmerize-crouch-fast-observe-v131','PASS9 observation: fast Midnight watcher captures in466/loop1200/out666 with a single reciprocal arm binding in native Sneaking context. Source-only crouch/cast visible; ordinary NPC target appears in foreground. No remote body integration claimed.')]:
 t=P/take;r=read(t/'result.json');assert (t/'source-sheet.jpg').exists()
 review=f'{take}/visual-review.json';write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Inspected actual-game source temporal sheet',evidence=['source-sheet.jpg'],visual_accepted=False,complete=False,notes=note))
 video=P/(take+'.mp4');hz=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True))['streams'][0]['avg_frame_rate'];n,d=map(float,hz.split('/'))
 with video.open('rb')as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=video.name,character='emily',label=f'Mesmerize native crouch observation · {take}',alias='mesmerize_native_crouch_observation',requirements=['emily_mesmerize_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=r['passed'],scope=r['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row);item.setdefault('native_integration',{})[video.name]=row
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(videos=len(videos),sourceOnly=2)))
