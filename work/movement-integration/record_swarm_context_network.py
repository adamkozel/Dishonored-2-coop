"""Index completed, visually inspected Swarm context takes without declaring completion."""
from pathlib import Path
import argparse,json,hashlib,datetime,subprocess
a=argparse.ArgumentParser();a.add_argument('take');a.add_argument('--notes',required=True);a.add_argument('--review-times',default='1,2.1,2.6,3.6,4.5,5.5,8.5');a.add_argument('--visual-scope',default='No remote rat/VFX/audio or shared enemy-outcome claim.');args=a.parse_args()
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration';t=P/args.take
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
r=read(t/'result.json');assert r['passed'] and all((t/(side+'-sheet.jpg')).exists() for side in ('peer','source'))
phases=read(t/'phase-incarnations.json');continuity=read(t/'continuity.json');context=read(t/'context-summary.json')
assert continuity['rejections']==[0] and len(phases)==3 and context['contextPhaseSamples']>0
summary=dict(result=r,nativePhases=[dict(variant=x['variant'],nativeScenes=len(x['nativeHandles']),sentEvents=len(x['sourceEvents']),acceptedEvents=len(x['acceptedEvents'])) for x in phases],continuity=continuity,context=context,complete=False,visualAccepted=False)
if (t/'walking-evidence.json').exists():summary['walkingEvidence']=read(t/'walking-evidence.json')
write(t/'coverage-summary.json',summary)
review=f'{args.take}/visual-review.json'
write(P/review,dict(inspected=True,at=datetime.datetime.now().astimezone().isoformat(),method='Viewed source and peer temporal sheets at '+args.review_times+' seconds',evidence=['source-sheet.jpg','peer-sheet.jpg'],visual_accepted=False,complete=False,notes=args.notes+' Contact sheets do not establish native-speed subjective acceptance or fine grip. '+args.visual_scope))
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');req='corvo_devouring_swarm_cast';item=ann['items'][req]
label=f"{r['stance']} {'holstered' if r['holstered'] else 'armed'}"+(f" move{r['move']}" if r.get('move') else '')
for source in (False,True):
 video=P/(args.take+('-source' if source else '')+'.mp4')
 fp=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));x,y=map(float,fp['streams'][0]['avg_frame_rate'].split('/'))
 with video.open('rb') as f:digest=hashlib.file_digest(f,'sha256').hexdigest().upper()
 row=dict(video=video.name,character='corvo',label='Swarm · '+('native source' if source else 'remote full-body')+' · '+label+' · '+args.take.rsplit('-',1)[1],alias='native_swarm_in',requirements=[req],gameplay_bound=not source,complete=False,visual_accepted=False,evidence=f'{args.take}/result.json',visual_review=review,proxy_sha256=r['dll'],actual_capture_hz=x/y,video_sha256=digest,checks_passed=True,scope=r['scope'])
 videos=[x for x in videos if x['video']!=video.name];videos.append(row);item.setdefault('native_integration',{})[video.name]=row
item['runtime_binding']='native_swarm_grounded_contexts_incrementally_verified_world_pending'
item.setdefault('context_receipts',{})[args.take]=dict(evidence=f'{args.take}/coverage-summary.json',label=label,checks=len(r['checks']),complete=False)
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
for archive in W.glob('candidate-swarm-*/identity.json'):
 identity=read(archive)
 if identity['dll']!=r['dll'] or identity.get('probe')!=r.get('probe'):continue
 dest=archive.parent/'runtime-verification.json';v=read(dest) if dest.exists() else dict(complete=False,contexts={})
 v.setdefault('contexts',{})[args.take]=summary;v['updated']=datetime.datetime.now().astimezone().isoformat();write(dest,v)
print(json.dumps(dict(take=args.take,videos=len(videos),summary=summary['nativePhases'],continuity=continuity)))
