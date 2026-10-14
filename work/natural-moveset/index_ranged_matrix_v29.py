"""Index the inspected standing ranged body/prop candidate, without accepting it."""
import json,hashlib,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
manifest=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json')
banks={x['role']:x for x in read(P/'ranged-bank-manifest-v1.json')}
tests=[];videos=[];checks=0
for role in ('corvo','emily'):
 for kind in ('gun','crossbow'):
  take=f'ranged-{role}-{kind}-network-v29-v1';r=read(P/take/'result.json')
  assert r['passed'] and len(r['checks'])==7 and r['dll']=='30AB28F8591D40153679CD52D7B3534B4D3BE6AEF9F7D2DCAEDFDDEEEB45806B'
  req=('pistol'if kind=='gun'else'crossbow')+'_draw_aim_fire_reload_holster'
  sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg']
  assert all((P/s).is_file()for s in sheets)
  review=dict(inspected=True,accepted=False,method='Paired actual-game temporal contact sheets visually inspected.',contact_sheets=sheets,
   observations=['Native input automatically drives remote aim, fire, reload, empty-trigger, holster and draw through Steam.','Native local first-person presentation remains active.','Remote weapon is attached to the native wrist, with cosmetic hand-local action tracks.'],
   limitations=['Standing, base weapon, two-round isolated fixture only.','Wide views limit finger/trigger/grip assessment; continuous naturalness is not accepted.','Crossbow body reload donor needs closer quality review.','This build has no mechanical crossbow pose playback or remote muzzle/projectile effects.','Moving, crouched, airborne, upgraded, switching and interruption cases remain unfinished.','No audio captured.'])
  write(P/take/'visual-review.json',review);case=[]
  for suffix in ('','-source'):
   movie=P/f'{take}{suffix}.mp4'
   c=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
   hz=int(c['nb_frames'])/float(c['duration']);assert hz>=24
   row=dict(video=movie.name,character=role,label=take.replace('-',' ')+(' · native source'if suffix else' · remote body'),kind='game',gameplay_bound=True,travelling=False,complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=banks[role]['sha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=c['avg_frame_rate'],capture=c,timing='Actual capture timestamps; no interpolation, padding or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],requirements=[req],scope=r['scope'],visual_review=review,limitations=review['limitations'])
   manifest=[x for x in manifest if x['video']!=movie.name];manifest.append(row);case.append(movie.name)
  item=annotations['items'][req];item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+case));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{take}/result.json']))
  tests.append(f'{take}/result.json');videos+=case;checks+=len(r['checks'])
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations)
write(P/'ranged-matrix-findings-v29.json',dict(complete=False,visual_accepted=False,dll=r['dll'],checks=checks,tests=tests,recordings=videos,confirmed=['Four role/weapon cases passed seven checks each.','Eight source/remote contact sheets inspected.'],remaining=review['limitations']))
state=P.parent/'current-task-state.json';s=read(state);m=s['MovementIntegration']
m['current']='v29 / 213 clips, Steam action protocol v25; standing base ranged body/prop matrix passes both roles. Movement phase incomplete.'
m['LatestContinuation'].update(actual_game_videos=len(manifest),ranged_matrix_checks=checks,bank='D2MOVE29 / 213 clips; ranged-banks-v1',complete=False,next='Native crossbow mechanism and muzzle presentation, followed by unfinished movement contexts and requirements.')
m['Ranged'].update(remoteGameplayBound=True,remoteGameplayBoundScope='Standing base gun/crossbow draw, fire, reload, empty-trigger and holster body/prop presentation only; four cases.',networkBuild=r['dll'],networkChecks=checks,networkFindings='work/natural-moveset/ranged-matrix-findings-v29.json',visualAccepted=False)
s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(state,s)
print('Indexed',len(videos),'actual-game videos;',checks,'checks; total',len(manifest),'; movement complete=false')
