"""Index completed network takes and their explicit visual review, without acceptance inflation."""
import argparse, json, hashlib, datetime, subprocess
from pathlib import Path
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
    with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
parser=argparse.ArgumentParser();parser.add_argument('take');parser.add_argument('--reviewed',action='store_true');parser.add_argument('--notes',required=True);a=parser.parse_args()
result=read(P/a.take/'result.json');role='corvo' if result['source']=='host' else 'emily';now=datetime.datetime.now().astimezone().isoformat()
if a.reviewed:assert (P/a.take/'native-sheet.jpg').exists() and (P/a.take/'source-sheet.jpg').exists()
review=f'{a.take}/visual-review.json';write(P/review,dict(inspected=a.reviewed,at=now,method='Actual-game temporal sheets compared for source and receiver' if a.reviewed else 'Video retained; visual inspection pending',evidence=['native-sheet.jpg','source-sheet.jpg'] if a.reviewed else [],visual_accepted=False,complete=False,notes=a.notes))
videos=read(P/'runtime-video-manifest.json');added=[]
for source in (False,True):
    file=P/(a.take+('-source' if source else '')+'.mp4')
    if not file.exists():continue
    meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(file)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
    row=dict(video=file.name,character=role,label=f'Grenade native network {"source" if source else "receiver"} · {a.take}',alias='grenade_throw_low' if result['pitch']>=30 else 'grenade_throw_high',requirements=['grenade_equip_aim_throw'],gameplay_bound=bool(result['passed']),complete=False,visual_accepted=False,evidence=f'{a.take}/result.json',visual_review=review,proxy_sha256=result['dll'],actual_capture_hz=n/d,video_sha256=sha(file),checks_passed=result['passed'],scope=result['scope'])
    videos=[v for v in videos if v['video']!=file.name];videos.append(row);added.append(row)
write(P/'runtime-video-manifest.json',videos)
ann=read(P/'review-evidence-annotations.json');item=ann['items']['grenade_equip_aim_throw'];item['native_integration'].update({v['video']:v for v in added})
if result['passed']:
    crouched=any(v.get('checks_passed') and 'grenade-crouch-network-' in v.get('video','') for v in item['native_integration'].values())
    item['runtime_binding']='native_network_ground_grenade_partial_verified' if crouched else 'native_network_standing_grenade_partial_verified'
    item['notes']='Native standing cooking/release/empty recovery drives the isolated remote body and held prop over Steam. '+('Crouched phases have passed in the individually recorded contexts. ' if crouched else '')+'Passed takes also verify six-joint native mechanisms, owned in-hand effects and specific same-world menu recovery cases; consult each result for its exact scope. Release hide, duplicate refusal, inventory rollback and checkpoint restoration checked. Projectile/explosion/world/damage replication, untested movement/stance/air contexts, broader interruption recovery and fine hand/foot acceptance remain incomplete.'
write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(take=a.take,checks_passed=result['passed'],visual_reviewed=a.reviewed,visual_accepted=False,videos=len(videos))))
