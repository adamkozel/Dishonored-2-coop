"""Index only tested native landing captures; preserve rejected fixture studies."""
import json,hashlib,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,d):
 raw=json.dumps(d,indent=2).encode('utf-8');tmp=p.with_suffix(p.suffix+'.tmp');tmp.write_bytes(raw);tmp.replace(p)
banks={r['role']:r for r in read(P/'hard-landing-crouch-bank-manifest-v1.json')}
rows=read(P/'runtime-video-manifest.json');added=[];tests=[]
for role in ('corvo','emily'):
 for stance,suffix in (('stand','v5'),('crouch','v1')):
  take=f'hard-landing-{role}-{stance}-v23-{suffix}';folder=P/take;result=read(folder/'result.json')
  assert result['passed']and result['recorded']
  movie=P/(take+'.mp4');probe=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(probe['nb_frames'])/float(probe['duration']);assert hz>=24
  sheets=[f'{take}/contact-{equip}-1.jpg'for equip in ('armed','empty')];assert all((P/s).exists()for s in sheets)
  row=dict(video=movie.name,character=role,label=f'{role.title()} · native hard landing · {"direct crouched"if stance=="crouch"else"standing"} recovery · drawn / holstered',kind='game',gameplay_bound=True,complete=False,visual_accepted=False,dll_sha256=result['dll'],bank_sha256=banks[role]['sha256'].upper(),sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=probe['avg_frame_rate'],capture=probe,timing='Actual capture timestamps; no padding or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=['hard_landing'],scope=result['scope'],visual_review=dict(contact_sheets=sheets,accepted=False,scope='Actual game impact, compression, support, equipment and recovery inspected for both equipment states. Direct crouched recovery stays low instead of rising through standing. Complete landing coverage remains unfinished.'),limitations=['Strong/damaging rolling falls, travelling landings and interruption/cancellation coverage remain incomplete.','Standing and crouched recordings use their explicitly documented isolated Dunwall positions.','Native root, camera, input, gameplay outcomes and first-person poses remain authoritative; presentation emits no damage events.'])
  rows=[r for r in rows if r['video']!=movie.name];rows.append(row);added.append(row);tests.append(f'{take}/result.json')
write(P/'runtime-video-manifest.json',rows)
a=read(P/'review-evidence-annotations.json');item=a['items']['hard_landing']
item.update(runtime_binding='native_light_landing_standing_and_crouched_verified_dunwall',visual_validation='reviewed_bounded_cases_full_acceptance_pending',phase_status='work_remaining',notes='Native CatFall light body scene drives complete-body landing on both characters through Steam. Standing and direct crouched recovery, drawn/holstered, are recorded. Native input latch chooses crouch before exit, removing the rejected V22 stand-up dip. Strong/damaging roll, travelling recovery, arbitrary geometry and interruptions remain to complete.',in_game_recordings=[r['video']for r in added],evidence=tests)
item['motions']=list(dict.fromkeys(item['motions']+['armed_native_hard_landing','empty_native_hard_landing','armed_native_hard_landing_crouched','empty_native_hard_landing_crouched']))
write(P/'review-evidence-annotations.json',a)
snapshot=P.parent/'movement-integration/candidate-hard-landing-v23-v1'
write(snapshot/'manifest.json',dict(dll_sha256=added[0]['dll_sha256'],format='D2MOVE23',clips=183,tests=tests,checks=sum(len(read(P/t)['checks'])for t in tests),game_recordings=[r['video']for r in added],source_bank_manifest='hard-landing-crouch-bank-manifest-v1.json',complete=False,limitations=item['notes']))
print('Indexed',len(added),'native hard-landing videos;',len(rows),'total; phase incomplete')
