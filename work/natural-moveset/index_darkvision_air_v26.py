"""Index inspected ordinary-jump casts with real capture timing and bounded scope."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
DLL='48A0E8F493C17097455CECF5416AD1DDC095DFC02885FD0DCBC79C50EEB337FD'
TAKES=['darkvision-corvo-air-v26-v3','darkvision-emily-air-v26-v2','darkvision-corvo-air-empty-v26-v1','darkvision-emily-air-empty-v26-v1']
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(x,indent=2),encoding='utf-8');t.replace(p)
rows=read(P/'runtime-video-manifest.json');ann=read(P/'review-evidence-annotations.json');paths=[];tests=[]
for take in TAKES:
 folder=P/take;r=read(folder/'result.json');role=r['character'];req=f'{role}_dark_vision_cast_end'
 assert r['passed'] and r['airborne'] and r['recorded'] and r['dll']==DLL and len(r['checks'])==12
 assert len(r['nativeScenes'])==2 and len(r['seenEvents'])==2
 assert (folder/'contact.jpg').is_file() and (folder/'source-contact.jpg').is_file()
 review=dict(inspected=True,accepted=False,method='Selected actual D2 temporal frames inspected, including wide framing through the jump apex.',
  contact_sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg'],
  observations=['Full remote body retains authored rising/falling/landing leg motion while the native Dark Vision scene drives the torso/left-arm gesture.',
   'Network trajectory carries the jump. Native first-person camera and power effects remain on the caster.',
   'Both casts return to standing with the original sword state. No gross calf/boot separation is apparent in the inspected view.'],
  limitations=['Ordinary stationary jumps only. Moving/directional jumps, double jumps, slide, water and lifecycle contexts remain.',
   'Wide framing proves whole-body continuity but cannot accept fine finger contact or full continuous animation quality.',
   'Source and observer recorder launches are separate, not frame-synchronized.'])
 write(folder/'visual-review.json',review)
 evidence=f'{take}/result.json';tests.append(evidence)
 for suffix in ('','-source'):
  movie=P/f'{take}{suffix}.mp4'
  capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=(20 if suffix else 24),(take,suffix,hz)
  context=dict(airborne=True,holstered=r['holstered'],moving=False)
  row=dict(video=movie.name,character=role,label=f'{role.title()} · Dark Vision during jump · '+('holstered' if r['holstered'] else 'sword drawn')+' · '+('caster POV' if suffix else 'remote body'),kind='game',gameplay_bound=True,travelling=True,complete=False,visual_accepted=False,
   dll_sha256=DLL,bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],capture=capture,
   timing='Actual capture timestamps; no padding, interpolation or speedup.',evidence=evidence,tests=[evidence],requirements=[req],scope=r['scope'],context=context,visual_review=review,limitations=review['limitations'])
  if hz<24:row['capture_quality']='supporting_source_view_below_24Hz'
  rows=[x for x in rows if x['video']!=movie.name];rows.append(row);paths.append(movie.name)
  item=ann['items'][req];item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+[movie.name]));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[evidence]))
  item['notes']='Native Dark Vision casts now cover ordinary jumps as well as tested ground stance/equipment/travel contexts. Exact native cast clock and airborne semantics travel together; native private reveal, sword state and landing recovery are preserved. Other contexts and lifecycle remain.'
write(P/'runtime-video-manifest.json',rows);write(P/'review-evidence-annotations.json',ann)
contract=read(P/'movement-phase-contract.json')
for power in contract['power_cases']:
 if power['id']=='dark_vision':
  power['partial_runtime_evidence']['airborne']=dict(recordings=paths,tests=tests,checks=48,scope='Both roles, drawn and holstered, two ordinary jump casts each.',dll=DLL)
  power['partial_runtime_evidence']['remaining']='Sideways/run/slide/water, moving and double jumps, upgrades, lifecycle and full continuous visual approval.'
write(P/'movement-phase-contract.json',contract)
write(P/'darkvision-air-findings-v1.json',dict(executable_sha256='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180',dll=DLL,protocol=21,packet_bytes=236,
 confirmed=['Same owned 933ms one-left-arm Dark Vision scene occurs during native Jump2/VT1c85028 and Fall3/VT1c845c8 on both roles.',
 'Semantic cast state retains optional validated native jump apex or falling stage. Landing preserves the same power event until its native scene ends.',
 'Remote authored air legs/pelvis blend with the full-body cast torso on the delayed network body clock. Local physics/camera/controls are untouched.'],
 observations=['native-darkvision-corvo-air-v1/result.json','native-darkvision-emily-air-v1/result.json'],integration=tests,recordings=paths,
 superseded=['Both air-v26-v1 integration takes pass numerically but clip the torso at the apex.',
 'Corvo air-v26-v2 failed the minimum native air sample count: 1 sample, with both remote cast events and 6 remote air samples. Faster state observations fixed evidence coverage; game timing unchanged.'],
 complete_movement=False))
print(f'Indexed {len(paths)} airborne game views; {len(rows)} total recordings; movement remains incomplete.')
