"""Index inspected native pistol world-FX evidence, with explicit limits."""
import datetime,hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,value):
    assert isinstance(value,(dict,list))
    pending=p.with_suffix(p.suffix+'.pending')
    pending.write_text(json.dumps(value,indent=2),encoding='utf-8')
    assert read(pending)==value
    pending.replace(p)
manifest=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json')
banks={x['role']:x for x in read(P/'ranged-bank-manifest-v1.json')}
videos=[];tests=[];builds=set()
limitations=[
 'Standing base pistol, isolated two-round fixture only; no remote projectile or impact synchronization.',
 'No audio captured. The exact native character sound event is selected, but spatial audibility and mixing still need verification.',
 'Moving, crouched, airborne, upgrades and weapon switching remain unfinished.',
 'Final natural body motion, reload contact, grip and complete transition quality remain unaccepted.'
]
for role in ('corvo','emily'):
 take=f'ranged-{role}-muzzle-v27-v2';result=read(P/take/'result.json')
 assert result['passed'] and len(result['checks'])==8
 builds.add(result['dll']);sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg']
 assert all((P/s).is_file()for s in sheets)
 review=dict(inspected=True,accepted=False,method='Paired source and remote actual-game temporal contact sheets visually inspected.',contact_sheets=sheets,
  observations=['Exactly two native source flashes yielded two finite remote effects; empty triggers and a duplicate packet did not add effects.',
   'Native character-specific pistol world effect replaces the oversized elite-guard 3p preset.',
   'Flash and smoke render at the cosmetic weapon muzzle; original native first-person presentation continues.'],limitations=limitations)
 write(P/take/'visual-review.json',review)
 case=[]
 for suffix in ('','-source'):
  movie=P/f'{take}{suffix}.mp4'
  capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
  hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
  row=dict(video=movie.name,character=role,label=take.replace('-',' ')+(' · native source'if suffix else' · remote body and muzzle'),kind='game',gameplay_bound=True,
   travelling=False,complete=False,visual_accepted=False,dll_sha256=result['dll'],bank_sha256=banks[role]['sha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),
   actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],capture=capture,timing='Actual capture timestamps; no interpolation, padding or speedup.',evidence=f'{take}/result.json',
   tests=[f'{take}/result.json'],requirements=['pistol_draw_aim_fire_reload_holster'],scope='Standing native gun input drives remote body, cosmetic prop and source-confirmed finite muzzle through Steam; visual quality remains under review.',
   visual_review=review,limitations=limitations)
  manifest=[x for x in manifest if x['video']!=movie.name]+[row];case.append(movie.name)
 item=annotations['items']['pistol_draw_aim_fire_reload_holster']
 item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+case));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{take}/result.json']))
 tests.append(f'{take}/result.json');videos+=case
assert len(builds)==1;dll=next(iter(builds))
assert hashlib.sha256((P.parent/'asset-game/dinput8.dll').read_bytes()).hexdigest().upper()==dll
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
write(P/'ranged-muzzle-findings-v27.json',dict(complete=False,visual_accepted=False,dll=dll,checks=16,tests=tests,recordings=videos,
 superseded_comparison=['ranged-corvo-muzzle-v27-v1/result.json','ranged-emily-muzzle-v27-v1/result.json'],
 confirmed=['Both roles pass eight checks.', 'Native temporary world-effect owners may lack an entity definition; reader verifies owner generation and stable identity.',
  'Fresh exact role effect IDs are correlated with owned gun firing scenes and bounded source positions.',
  'Shot confirmation is discrete on the delayed Steam timeline; one finite visual start per semantic event.',
  'Cosmetic muzzle uses exact native model tag offset (.38,0,.055) on origin joint; no projectile or damage calls.'],limitations=limitations))
state=P.parent/'current-task-state.json';s=read(state);assert isinstance(s,dict) and len(s)>=49
backup=P/'task-state-before-muzzle-index-v27.json'
if not backup.exists():backup.write_bytes(state.read_bytes())
m=s['MovementIntegration'];m['liveBuild']=dll
m['current']='D2MOVE29 / 213 clips, Steam action protocol v27. Standing base ranged body/prop, crossbow state and native gun muzzle verified in bounded tests. Complete movement phase unfinished.'
m['LatestContinuation'].update(live=dll,deploying=None,actual_game_videos=len(manifest),ranged_muzzle_checks=16,complete=False,next='Ranged stance/movement contexts and quality; remaining traversal, combat, items, paired actions, powers and lifecycle coverage.')
m['Ranged'].update(muzzleBuild=dll,muzzleChecks=16,muzzleFindings='work/natural-moveset/ranged-muzzle-findings-v27.json',visualAccepted=False)
s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(state,s)
print('Indexed four native muzzle recordings;',len(manifest),'actual-game videos; movement complete=false')
