"""Retain failed captures and the inspected paired-role equipment regression."""
import datetime, hashlib, json, subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v): p.write_text(json.dumps(v,indent=2),encoding='utf-8')
videos=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json')
item=ann['items']['sword_draw_holster'];evidence=item.setdefault('native_integration',{})
for take in ['equipment-review-both-v48','equipment-review-both-v48-v2','equipment-review-both-v48-v3','equipment-review-both-v48-v4']:
 result=read(P/take/'result.json')
 for role,character in [('host','corvo'),('client','emily')]:
  video=P/f'{take}-{role}.mp4'
  if not video.exists():continue
  reviewed=take.endswith('-v4');review=None
  if reviewed:
   sheet=f'equipment-{role}-sheet.jpg';assert(P/take/sheet).exists()
   review=f'{take}/visual-review-{role}.json'
   write(P/review,dict(inspected=True,method='Viewed actual-game temporal contact sheet',times_seconds=[1.6,2.3,10.4,11,15.8,24.1],evidence=sheet,visual_accepted=False,complete=False,inspected_at=datetime.datetime.now().astimezone().isoformat(),notes='Standing holster/empty idle/draw and low crouched equipment transitions are visible. Hand contact is dark and not accepted from this distance. Directional segments contain deliberate fixture position resets, so this is not a continuous travel-quality test.'))
  meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-show_entries','stream=avg_frame_rate','-of','json',str(video)],text=True));n,d=map(float,meta['streams'][0]['avg_frame_rate'].split('/'))
  row=dict(video=video.name,character=character,label=f'Sword draw/holster · {character} · {take}'+(''if result['passed']else' · failed check'),alias='sword_equipsword',requirements=['sword_draw_holster'],gameplay_bound=True,complete=False,visual_accepted=False,evidence=f'{take}/result.json',visual_review=review,proxy_sha256=result['dll'],actual_capture_hz=n/d,video_sha256=hashlib.sha256(video.read_bytes()).hexdigest().upper(),checks_passed=result['passed'],scope=result['scope'])
  videos=[v for v in videos if v['video']!=video.name];videos.append(row);evidence[video.name]=row
item['runtime_binding']='partial_native_equipment_standing_crouched_both_roles'
item['notes']='Current v48 DLL/v46 banks pass fifteen paired-role equipment/visibility/health/protected-file checks. Actual-game sampled frames reviewed for both roles. Exact hand contact, continuous travel quality, interruption/recovery breadth and final visual acceptance remain open. Earlier failed takes are retained.'
ann['movement_phase']['requested_delivery']='Complete Corvo/Emily moveset library ported to D2 with actual-game recordings. Continue directly into the remaining playable co-op requirements after movement under the active goal; no milestone approval pauses.'
write(P/'runtime-video-manifest.json',videos);write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(videos=len(videos),equipment_complete=False)))
