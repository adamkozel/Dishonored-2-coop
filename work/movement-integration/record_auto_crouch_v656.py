from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb')as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
at=datetime.datetime.now().astimezone().isoformat();audit=rd(W/'auto-crouch-audit-v656.json');assert audit['passed']and len(audit['checks'])==36
cases=[('native-window-corvo-v649','corvo','host','window-open-traverse',False),('native-window-emily-v650','emily','client','window-open-traverse',False),('native-auto-crouch-corvo-v652','corvo','host','auto-crouch-sill',False),('native-auto-crouch-corvo-v653','corvo','host','auto-crouch-sill',True),('native-auto-crouch-emily-v654','emily','client','auto-crouch-sill',True),('native-auto-crouch-corvo-v655','corvo','host','auto-crouch-sill',True)]
videos=rd(N/'runtime-video-manifest.json')
for take,character,role,stem,bound in cases:
 f=N/take;r=rd(f/'result.json')
 for view in ('source','observer'):
  video=f/(f'{role}-{stem}.mp4'if view=='source'else'observer.mp4');rel=video.relative_to(N).as_posix()
  assert video.exists()and not any(x['video']==rel for x in videos)
  inspected=(view=='observer'and take.endswith(('v652','v654','v655')))or(view=='source'and take.endswith('v649'))
  sheet=f'{view}-sheet.jpg';assert not inspected or(f/sheet).exists()
  note='Closed window sill; no window opening or traversal claim. '
  if take.endswith(('v649','v650')):note+='Native discovery only; remote stream never ready.'
  elif take.endswith('v652'):note+='Stream freshness retirement after5.24s; obstructed observer view; continuity failed.'
  else:note+='Sustained native AutoCrouch10, low mantle, fast sword recovery and return to standing. Brief body-blend dips at entry/retreat are retained; foot/sill contact and acting need polish.'
  review=f/f'{view}-review-v656.json';wr(review,dict(at=at,inspected=inspected,visualAccepted=False,complete=False,sheets=[sheet]if inspected else[],observations=[note,'Clear full-body observer coverage is established only for654/655; backlit views do not validate fingers.']))
  stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  videos.append(dict(video=rel,kind='game',character=character,label=f'{character.title()} · automatic crouch{take[-3:]} · {view}',alias='crouch_idle',requirements=['crouch_idle','crouch_to_stand','stand_to_crouch'],gameplay_bound=bound,complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/auto-crouch-audit-v656.json'if bound else'../../work/movement-integration/auto-crouch-audit-v652.json'if take.endswith('v652')else None,visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha(video),actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=bool(bound and r['passed']),scope=r['scope']+' '+note))
wr(N/'runtime-video-manifest.json',videos)
latest='651/656: exact native AutoCrouch10/raw2, low confined mantle8 next10 and fast333ms sword recovery now bind to existing crouch/low-mantle presentation. Corvo653/655 and Emily654 pass36 scoped checks; both streams remain ready, held crouch and final standing fully weighted. Original635 and all357 clips preserved. Clear654/655 observer sheets inspected; foot/sill contact and brief entry/retreat blend dips remain polish/continuity work. No open-window, high confined mantle or auto-crouch combat/power coverage claimed.'
a=rd(N/'review-evidence-annotations.json')
for key in ('crouch_idle','crouch_to_stand','stand_to_crouch'):
 i=a['items'][key];i['complete']=False;i['runtime_binding']='partial_native_manual_and_automatic_crouch_both_roles';i['notes']+=' '+latest
 i.setdefault('evidence',[]).extend(take+'/result.json'for take,_,_,_,bound in cases if bound)
wr(N/'review-evidence-annotations.json',a)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['AutomaticCrouchContinuation']=dict(at=at,latest=latest,candidate='candidate-auto-crouch-v651',receipt='work/movement-integration/auto-crouch-audit-v656.json',runtimeVerified=True,complete=False,next='Continue remaining functional movement in existing pipeline. Inspect chain/climb fixtures and native nondefault swim/climb states from647/648. Fine actor/contact polish remains deferred; no external co-op mod detour.')
s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
p=R/'work/handoff-project-bookmarks-current.json';merged={(x['address'],x['category']):x for x in rd(p)};merged.update({(x['address'],x['category']):x for x in rd(R/'work/handoff-project-bookmarks-v656.json')});wr(p,sorted(merged.values(),key=lambda x:(x['address'],x['category'])))
p=D/'README.md';t=p.read_text()
for label,name in [('Automatic crouch, confined low mantle and retained continuity limits','49-automatic-crouch-and-confined-mantle.md'),('Native swimming/climbing mode and fixture discovery','50-swimming-climbing-mode-discovery.md')]:
 link=f'[{label}]({name})'
 if link not in t:t+='\n- '+link+'\n'
p.write_text(t)
print(json.dumps(dict(videos=len(videos),bookmarks=len(merged),checks=36,candidate='candidate-auto-crouch-v651',completeMovement=False)))
