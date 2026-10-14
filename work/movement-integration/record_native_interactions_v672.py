"""Index actual interaction evidence without converting execution PASS to behavior PASS."""
from pathlib import Path
import json, hashlib, datetime, subprocess
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; N=R/'work/natural-moveset'; D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
at=datetime.datetime.now().astimezone().isoformat()
audit=rd(W/'loot-motion-audit-v668.json'); assert audit['passed'] and audit['checks']==20
specs={
 'native-interactions-corvo-v662':('corvo',False,{'food-source'},'Invalid targeting from fixed eye-height assumption; native pickup activation not accepted. Stream retirement and later note-aim failure retained.'),
 'native-interactions-corvo-v663':('corvo',False,{'food-source','food-observer'},'Native source grapes consumed, despite planned apple target. Receiver fruit remained present; brief body-weight dip0.689115. Later coin startup failed. Partial source observation only.'),
 'native-interactions-corvo-coin-v664':('corvo',False,{'coin-source'},'Native source coin pickup confirmed; LootObject47 rejected by prior movement reader. Baseline for666 fix, not successful continuity proof.'),
 'native-interactions-emily-coin-v665':('emily',False,{'coin-source'},'INVALID comparison: guards attacked source during capture. Execution/restoration PASS does not establish coin pickup or movement coverage. Earlier Ghidra claim corrected.'),
 'native-interactions-corvo-coin-v667':('corvo',True,{'coin-source','coin-observer'},'Confirmed native standing coin pickup and continuous existing standing body; both Steam streams ready. No separate reach gesture or shared inventory/world outcome. Stationary guard partially affects framing.'),
 'native-interactions-emily-coin-v668':('emily',True,{'coin-source','coin-observer'},'Confirmed native standing coin pickup and continuous existing standing body; both Steam streams ready. No separate reach gesture or shared inventory/world outcome.'),
 'native-interactions-corvo-note-v669':('corvo',False,{'note-source'},'Native ReadNote4 UI confirmed. Fixture retired and harness aborted before close; original cleanup failed. Separate ordinary-B and checkpoint recovery receipt passed. No accepted automatic resume.'),
 'native-interactions-corvo-note-v674':('corvo',False,{'note-observer'},'Body remained visible in79 observations and inspected frames, but semantic restart deadlocked because root ownership stayed suspended. Execution/restoration PASS is not recovery acceptance.673 is superseded by corrected676.'),
 'native-interactions-corvo-note-v677':('corvo',False,{'note-source','note-observer'},'676 recovered and remained visible, but returning frame ownership to native physics allowed roughly0.49m horizontal/0.46m vertical drift while reading. Partial recovery proof; root retention superseded by681.'),
 'native-interactions-corvo-note-v682':('corvo',False,{'note-source','note-observer'},'681 body remains visible with stationary reading root and recovers, then a stale root packet at250ms retires the stream near10.65s. Harness fails before planned backward movement. Failure preserved; transport robustness remains open.'),
}
videos=rd(N/'runtime-video-manifest.json'); existing={x['video']:x for x in videos}
noteAudit=rd(W/'readable-resume-audit-v672.json') if (W/'readable-resume-audit-v672.json').exists() else None
if noteAudit:
 assert noteAudit['passed'] and noteAudit['checks']==20
 for take,character in [('native-interactions-corvo-note-v671','corvo'),('native-interactions-emily-note-v672','emily')]:
  specs[take]=(character,True,{'note-source','note-observer'},'Native note opens/closes and movement automatically recovers using fresh stream tokens and pinned bank. Model disappears during reading, then reappears; full bank recovery takes3.65/3.74s. No authored reading gesture or shared document outcome. Retained visibility is a subsequent fix.')
rootAudit=rd(W/'readable-resume-audit-v684.json')
assert rootAudit['passed'] and rootAudit['checks']==28
for take,character in [('native-interactions-corvo-note-v685','corvo'),('native-interactions-emily-note-v683','emily')]:
 specs[take]=(character,True,{'note-source','note-observer'},'681 native reading and subsequent ordinary backward movement pass28 checks across both roles. Body stays visible, measured stationary root error/drift0 during note, fresh Steam tokens and unchanged bank recover. No authored reading gesture/shared document state; furniture contact and acting remain coarse. Failed682 stale-packet interruption remains a stability issue.')
for take,(character,bound,inspected,note) in specs.items():
 f=N/take; r=rd(f/'result.json')
 for video in sorted(f.glob('*.mp4')):
  rel=video.relative_to(N).as_posix(); stem=video.stem; part,view=stem.rsplit('-',1)
  alias={'food':'eat_drink','coin':'pickup_loot_steal','note':'read_object_or_note'}[part]
  reviewed=stem in inspected; sheet=f/(stem+'-sheet.jpg'); assert not reviewed or sheet.exists(),sheet
  review=f/(stem+'-review-v672.json'); wr(review,dict(at=at,inspected=reviewed,visualAccepted=False,complete=False,sheets=[sheet.name] if reviewed else [],observations=[note,'Fine acting/contact polish deferred; this record is not whole-movement acceptance.']))
  stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  with video.open('rb') as h: digest=hashlib.file_digest(h,'sha256').hexdigest().upper()
  binding='../../work/movement-integration/'+('readable-resume-audit-v672.json' if part=='note' else 'loot-motion-audit-v668.json') if bound else None
  if take in ('native-interactions-corvo-note-v685','native-interactions-emily-note-v683'):binding='../../work/movement-integration/readable-resume-audit-v684.json'
  entry=dict(video=rel,kind='game',character=character,label=f'{character.title()} · native {part} · {take[-4:]} · {view}',alias=alias,requirements=[alias],gameplay_bound=bound,complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit=binding,visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=digest,actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=bound and r['passed'],scope=note)
  if rel in existing: existing[rel].update(entry)
  else:videos.append(entry)
wr(N/'runtime-video-manifest.json',videos)
latest='666/668: both-role ordinary standing coin pickup passes20 scoped checks, including exact LootObject47 with/without Undraw81, source removal, native Steam continuity and fully weighted existing body.111 observations each plus639/637 compact native samples. Both source/observer sheets inspected; no added reach gesture, shared inventory or receiver removal.669 discovers ReadNote4 and retains its failed fixture/cleanup receipt; separate recovery passed.670 adds exact native note screen to existing suspension/resume, runtime verification ongoing.'
latest='666/668 coin continuity passes20 checks.681/683/685 ReadNote4 and ordinary post-close walking pass28 checks: both bodies remain visible, reading root error/drift0, fresh Steam tokens and pinned357-clip bank recover, inventories/health14protected restored. Both source/observer sheets inspected. Failed682 retained: stream retired after root age250ms despite earlier recovery. No authored reading gesture/shared item or document outcome; fine acting deferred. Routine Mayhew reset now uses verified native skip680 (scene4.619s, full reset20.857s).'
a=rd(N/'review-evidence-annotations.json'); item=a['items']['pickup_loot_steal']; item.update(complete=False,runtime_binding='partial_standing_coin_native_both_roles',notes=latest)
for take in ['native-interactions-corvo-coin-v667','native-interactions-emily-coin-v668']:
 for field,values in [('evidence',[take+'/result.json']),('in_game_recordings',[take+'/coin-source.mp4',take+'/coin-observer.mp4'])]:
  item[field]=list(dict.fromkeys(item.get(field,[])+values))
item=a['items']['eat_drink']; item.update(complete=False,runtime_binding='partial_source_grapes_only',notes='663 source ordinary input consumes grapes with native UI; planned apple was not focused. Existing remote body has brief blend dip and no authored eating gesture. Receiver fruit remains unchanged. Whole-batch coin startup failed; source food observation is not complete eating/drinking coverage.')
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+['native-interactions-corvo-v663/result.json']))
item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+['native-interactions-corvo-v663/food-source.mp4','native-interactions-corvo-v663/food-observer.mp4']))
item=a['items']['read_object_or_note'];item.update(complete=False,runtime_binding='partial_native_note_visible_root_hold_and_resume_both_roles',notes=latest)
for take in ['native-interactions-corvo-note-v685','native-interactions-emily-note-v683']:
 item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[take+'/result.json','../../work/movement-integration/readable-resume-audit-v684.json']))
 item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+[take+'/note-source.mp4',take+'/note-observer.mp4']))
wr(N/'review-evidence-annotations.json',a)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at;s['InteractionBatchContinuation']=dict(at=at,latest=latest,candidate='candidate-read-note-v670',lootVerified=True,readableResumeVerified=False,complete=False,receipt='work/movement-integration/loot-motion-audit-v668.json',details='documentation/handoff-2026-10-11/52-native-pickups-and-readable-recovery.md',next='Verify native note UI suspension/closing/recovery on both roles using671, then continue functional movement coverage in the existing pipeline. Food/other target contexts and shared item outcomes remain tracked; fine acting deferred.')
s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
s['InteractionBatchContinuation'].update(candidate='candidate-readable-body-v681',readableResumeVerified=True,readableVisibleRootVerified=True,receipt='work/movement-integration/readable-resume-audit-v684.json',stabilityIssue='682 recovers then root age250ms retires stream; independent683/685 pair passes. Preserve failure; sustained transport robustness is unresolved.',next='Continue remaining native movement/interaction contexts through existing pipeline; verify other loot stances and food continuity, then remaining traversal/combat fixtures. Shared inventory/document outcomes remain world work. Fine acting deferred.')
s['NativeSetupSkip']=dict(at=at,verified=True,helper='work/PrepareShadowSword.ps1',receipt='work/movement-integration/native-setup-skip-v680/result.json',details='documentation/handoff-2026-10-11/53-native-setup-scene-skip.md',fullPlaybackOption='-PlaySetupScene',sceneElapsedMs=4619,fullResetMs=20857)
wr(R/'work/current-task-state.json',s)
p=R/'work/handoff-project-bookmarks-current.json';merged={(x['address'],x['category']):x for x in rd(p)}
for extra in [R/'work/handoff-project-bookmarks-v673.json',R/'work/handoff-project-bookmarks-v684.json']:
 if extra.exists():merged.update({(x['address'],x['category']):x for x in rd(extra)})
wr(p,sorted(merged.values(),key=lambda x:(x['address'],x['category'])))
p=D/'README.md';t=p.read_text(encoding='utf-8');link='[Native pickups and readable-screen recovery](52-native-pickups-and-readable-recovery.md)'
if link not in t:p.write_text(t+'\n- '+link+'\n',encoding='utf-8')
print(json.dumps(dict(videos=len(videos),lootChecks=20,completeMovement=False)))
