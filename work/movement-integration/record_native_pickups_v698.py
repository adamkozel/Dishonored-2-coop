"""Preserve partial/failed runs and publish exact native pickup evidence."""
from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
at=datetime.datetime.now().astimezone().isoformat()
coin=rd(W/'crouch-loot-audit-v698.json');food=rd(W/'food-continuity-audit-v698.json')
assert coin['checks']==21 and coin['failedChecks']==1 and not coin['passed']
assert food['passed'] and food['checks']==22
specs={
 'native-interactions-corvo-crouch-coin-v686':('corvo',False,False,{'coin-source','coin-observer'},None,'Invalid pickup comparison: desk occludes crouched sightline; exact coin remains. Body/input execution is not activation proof.'),
 'native-interactions-corvo-crouch-coin-v688':('corvo',False,False,{'coin-source'},None,'Invalid pickup comparison: measured angle settled, but camera faced furniture/wall and coin remained. No native loot state or accepted pickup.'),
 'native-interactions-corvo-crouch-coin-v692':('corvo',False,False,{'coin-source','coin-observer'},None,'Actual crouched coin collected; prior classifier rejects Sneaking6 + LootObject47, with/without Undraw81. Baseline for693; observer upper body cropped.'),
 'native-interactions-emily-crouch-coin-v695':('emily',True,True,{'coin-source','coin-observer'},'crouch-loot-audit-v698.json','Exact crouched coin pickup and retained source/remote crouch pass11 individual checks. Native source alone owns item removal; no added reach gesture. Aggregate audit fails on Corvo body continuity.'),
 'native-interactions-corvo-crouch-coin-v699':('corvo',True,False,{'coin-source','coin-observer'},'crouch-loot-audit-v698.json','Crouched pickup classification and source removal verified, but continuity audit fails: initial body weight0.26822 and later0.522453 dip near10.17s, eligibility mask16/state age250ms. Streams remain ready. No complete continuity acceptance.'),
 'native-interactions-corvo-food-v696':('corvo',True,True,{'food-source','food-observer'},'food-continuity-audit-v698.json','Native grapes consumption, source healing56->61, unchanged peer56, source removal and continuous full body pass11 checks. Receiver fruit remains. No eating gesture or shared inventory/world outcome.'),
 'native-interactions-emily-food-v697':('emily',True,True,{'food-source','food-observer'},'food-continuity-audit-v698.json','Native grapes consumption, source healing56->61, unchanged peer56, source removal and continuous full body pass11 checks. Receiver fruit remains. No eating gesture or shared inventory/world outcome.'),
}
videos=rd(N/'runtime-video-manifest.json');existing={x['video']:x for x in videos}
for take,(character,bound,passed,inspected,audit,note) in specs.items():
 f=N/take;r=rd(f/'result.json')
 for video in sorted(f.glob('*.mp4')):
  rel=video.relative_to(N).as_posix();stem=video.stem;kind,view=stem.rsplit('-',1)
  seen=stem in inspected;sheet=f/(stem+'-sheet.jpg');assert not seen or sheet.exists()
  review=f/(stem+'-review-v698.json');wr(review,dict(at=at,inspected=seen,visualAccepted=False,complete=False,sheets=[sheet.name] if seen else [],observations=[note,'Coarse poses/contact retained; animator polish deferred.']))
  stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  with video.open('rb') as h:digest=hashlib.file_digest(h,'sha256').hexdigest().upper()
  alias='pickup_loot_steal' if kind=='coin' else 'eat_drink'
  entry=dict(video=rel,kind='game',character=character,label=f'{character.title()} · native {"crouched coin" if kind=="coin" else "grapes"} · {take[-4:]} · {view}',alias=alias,requirements=[alias],gameplay_bound=bound,complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/'+audit if audit else None,visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=digest,actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=passed,scope=note)
  if rel in existing:existing[rel].update(entry)
  else:videos.append(entry)
wr(N/'runtime-video-manifest.json',videos)
latest='693 exact crouched LootObject47 now preserves Sneaking6/raw1. Both-role native classification verified;695Emily passes11 checks,699Corvo passes10/11 with body-weight dips retained as failure.696/697 native grapes consumption passes22: source health56->61, peer56 unchanged, source grapes removed, full body/Steam continuity. All source/observer sheets inspected; inventories/health14protected restored. No new reach/eating gesture or shared item/healing outcome. Protocol69 bank67/357 unchanged.'
a=rd(N/'review-evidence-annotations.json')
for key,takes,binding in [('pickup_loot_steal',['native-interactions-emily-crouch-coin-v695','native-interactions-corvo-crouch-coin-v699'],'partial_native_standing_and_crouched_pickup_continuity_gap'),('eat_drink',['native-interactions-corvo-food-v696','native-interactions-emily-food-v697'],'partial_native_source_grapes_and_remote_body_both_roles')]:
 item=a['items'][key];item.update(complete=False,runtime_binding=binding,notes=latest)
 item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[t+'/result.json' for t in takes]))
 item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+[r['video'] for r in videos if r['video'].split('/')[0] in takes]))
wr(N/'review-evidence-annotations.json',a)
s=rd(R/'work/current-task-state.json');s['UpdatedAt']=at
nextWork='Investigate bounded same-identity recovery and presentation continuity after transient native Steam data gaps, using682/694/699 traces. Keep strict actor/world/session/load/death/explicit-stop boundaries and preserve failures. Then continue missing movement and shared-world work; no more unreachable pickup fixtures or fine-pose tuning.'
s['InteractionBatchContinuation'].update(at=at,latest=latest,candidate='candidate-crouch-loot-v693',lootVerified=True,foodVerified=True,crouchedLootClassificationVerified=True,crouchedLootContinuityVerified=False,complete=False,receipt='work/movement-integration/crouch-loot-audit-v698.json',foodReceipt='work/movement-integration/food-continuity-audit-v698.json',details='documentation/handoff-2026-10-11/54-crouched-loot-and-native-food.md',stabilityIssue='682 retired after state/root gap;694 startup retired at state266/root250ms;699 remains connected but body eligibility mask16/stateAge250 resets weight during idle. No thresholds relaxed.',next=nextWork)
s['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',s)
p=R/'work/handoff-project-bookmarks-current.json';bookmarks={(x['address'],x['category']):x for x in rd(p)}
bookmarks.update({(x['address'],x['category']):x for x in rd(R/'work/handoff-project-bookmarks-v698.json')});wr(p,sorted(bookmarks.values(),key=lambda x:(x['address'],x['category'])))
print(json.dumps(dict(videos=len(videos),foodChecks=22,crouchChecks=21,crouchFailedChecks=1,completeMovement=False)))
