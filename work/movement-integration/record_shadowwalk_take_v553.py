"""Index completed Shadow Walk recordings without inferring visual acceptance."""
from pathlib import Path
import argparse,datetime,hashlib,json,subprocess
p=argparse.ArgumentParser();p.add_argument('take');p.add_argument('--source-only',action='store_true');p.add_argument('--sheet');p.add_argument('--source-sheet');p.add_argument('--observation',required=True);a=p.parse_args()
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
assert rd(W/'native-operation-owner.json')['state']=='finished'
take=N/a.take;result=rd(take/'result.json');manifest=rd(N/'runtime-video-manifest.json')
assert any('checkpoints restore' in x for x in result['checks']) and any('protected' in x for x in result['checks'])
at=datetime.datetime.now().astimezone().isoformat()
for suffix in (('',) if a.source_only else ('','-source')):
 video=N/(a.take+suffix+'.mp4');assert video.exists()
 assert not any(v['video']==video.name for v in manifest),video
 source=a.source_only or bool(suffix);sheet=a.source_sheet if source else a.sheet
 if sheet:assert (take/sheet).exists()
 review=dict(at=at,inspected=bool(sheet),sheets=[sheet] if sheet else [],observations=[a.observation] if sheet else ['Recording indexed; no visual inspection claimed for this view.'],visualAccepted=False,complete=False,method='Viewed temporal video sheet; continuous motion quality not certified.' if sheet else 'Unreviewed')
 reviewfile=take/('source-visual-review.json' if source else 'visual-review.json');reviewfile.write_text(json.dumps(review,indent=2),encoding='utf-8')
 meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
 manifest.append(dict(video=video.name,character='emily',label=('Native source' if source else 'Steam remote body')+' · Shadow Walk '+a.take.split('-v')[-1],alias='shadow_walk_native_reference' if source else 'shadow_walk_sneak_integration',requirements=['emily_shadow_walk_enter_move_attack_exit'],gameplay_bound=not source,complete=False,visual_accepted=False,evidence=a.take+'/result.json',visual_review=a.take+'/'+reviewfile.name,proxy_sha256=result['dll'],video_sha256=hashlib.sha256(video.read_bytes()).hexdigest().upper(),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=result['passed'],scope=result['scope']))
(N/'runtime-video-manifest.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
print(json.dumps(dict(indexed=a.take,videos=len(manifest),checksPassed=result['passed'],complete=False)))
