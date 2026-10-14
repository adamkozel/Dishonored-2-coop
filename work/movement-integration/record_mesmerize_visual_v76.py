"""Index inspected isolated cosmetic model proofs separately from gameplay."""
import json,hashlib,datetime,subprocess
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,o:p.write_text(json.dumps(o,indent=2),encoding='utf-8')
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');item=ann['items']['emily_mesmerize_cast']
for suffix in ['v76c','v76d']:
 take='mesmerize-visual-model-'+suffix;t=P/take;r=read(t/'result.json');video=P/(take+'.mp4')
 assert (t/'model-sheet.jpg').exists()
 notes=('Model alone is very faint against this wall; native three-phase animation and hide succeeded but harness failed when an Emily-only reader was used on Corvo.' if suffix=='v76c' else
        'Native body particles form a visible dark vertical shape with drifting particles, absent after cleanup. All three model scenes start and one owned effect starts/stops. Camera/light are insufficient for fine shape acceptance; source/receiver comparison pending. No gameplay power component or AI behavior in the model. Silent recording does not verify sound.')
 write(t/'visual-review.json',dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Actual-game temporal contact sheet',evidence=['model-sheet.jpg'],notes=notes,visual_accepted=False,complete=False))
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
 with video.open('rb')as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=video.name,character='emily',label='Mesmerize isolated cosmetic model '+suffix,alias='mesmerize_visual_model_proof',requirements=['emily_mesmerize_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=take+'/visual-review.json',proxy_sha256=r['dll'],actual_capture_hz=n/d,video_sha256=digest,checks_passed=r['passed'],scope=r['scope'])
 videos=[v for v in videos if v['video']!=video.name];videos.append(row);item.setdefault('native_integration',{})[video.name]=row
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann);print('Indexed',len(videos),'actual game videos')
