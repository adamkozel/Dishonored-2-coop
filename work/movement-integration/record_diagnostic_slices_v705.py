from pathlib import Path
import json,datetime,subprocess,hashlib
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
a=rd(W/'diagnostic-slices-audit-v705.json');assert a['passed'] and a['checks']==32 and a['failedChecks']==0
at=datetime.datetime.now().astimezone().isoformat();videos=rd(N/'runtime-video-manifest.json');existing={x['video']:x for x in videos}
for c in a['cases']:
 take=c['take'];f=N/take;r=rd(f/'result.json');character='emily' if 'emily' in take else 'corvo'
 note='Actual native crouched coin pickup; source coin disappears and native Coin notification is visible. Remote body remains visible in crouch on the lectern. Rear observer view limits grip/contact judgment; no added reach gesture. Coarse acting unchanged.'
 if '703' in take:note+=' Source NPC walks nearby during recording; source and partner health remained unchanged.'
 note+=' Static contact sheets verify appearance and visibility, not frame pacing. Separate native timing counters establish the measured diagnostic hitch reduction.'
 for view in ['source','observer']:
  video=f/f'coin-{view}.mp4';sheet=f/f'coin-{view}-sheet.jpg';assert sheet.exists()
  review=f/f'coin-{view}-review-v705.json';wr(review,dict(at=at,inspected=True,visualAccepted=False,complete=False,sheets=[sheet.name],observations=[note]))
  stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
  with video.open('rb') as h:sha=hashlib.file_digest(h,'sha256').hexdigest().upper()
  rel=video.relative_to(N).as_posix();entry=dict(video=rel,kind='game',character=character,label=f'{character.title()} · crouched coin / diagnostic timing · {take[-4:]} · {view}',alias='pickup_loot_steal',requirements=['pickup_loot_steal'],gameplay_bound=True,complete=False,visual_accepted=False,evidence=take+'/result.json',binding_audit='../../work/movement-integration/diagnostic-slices-audit-v705.json',visual_review=take+'/'+review.name,proxy_sha256=r['dll'],video_sha256=sha,actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),checks_passed=c['passed'],scope=note)
  if rel in existing:existing[rel].update(entry)
  else:videos.append(entry)
wr(N/'runtime-video-manifest.json',videos)
latest='700/701 measures recurring55–65ms diagnostic batches, with over100ms spikes.702 staggers71 writers across frames (max4 and4ms soft budget), retaining core report cadence and all gameplay/250ms freshness guards.703Corvo/704Emily pass24 scoped runtime checks; baseline701 passes8. Both roles keep Steam ready and body weight>.999 during crouched loot. No status/slice reaches50ms in either new capture, versus52/51 source/observer stalls in baseline. Inventories/health14protected restored. Six game sheets inspected; rear views limit grip/contact judgment. No all-gap recovery or full movement claim.'
nextWork='Continue functional coverage through the current pipeline. This removes the measured periodic diagnostic burst; same-identity stream recovery after other real gaps remains outstanding. Preserve 682/694/699 failures and strict identity/load/death/explicit-stop boundaries. Native source semantic sampling remains costly; profile before optimizing. Then continue missing movement contexts and shared NPC/world progression; fine animation polish stays deferred.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['DiagnosticTimingContinuation']=dict(at=at,latest=latest,candidate='candidate-diagnostic-slices-v702',receipt='work/movement-integration/diagnostic-slices-audit-v705.json',details='documentation/handoff-2026-10-11/55-native-diagnostic-hitches.md',runtimeVerified=True,complete=False,next=nextWork)
state['InteractionBatchContinuation'].update(at=at,candidate='candidate-diagnostic-slices-v702',latest=latest,crouchedLootContinuityVerified=True,stabilityIssue='Scoped703/704 continuity now passes after diagnostic scheduling; earlier682/694/699 failures preserved. Automatic same-identity recovery after other gaps remains unimplemented.',next=nextWork)
state['MovementExecutionPriority']['current']=latest;wr(R/'work/current-task-state.json',state)
annotations=rd(N/'review-evidence-annotations.json');item=annotations['items']['pickup_loot_steal'];item.update(complete=False,runtime_binding='partial_native_standing_and_crouched_pickup_both_roles',notes=latest)
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[x['take']+'/result.json' for x in a['cases']]))
item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+[x['video'] for x in videos if any(x['video'].startswith(c['take']+'/') for c in a['cases'])]))
wr(N/'review-evidence-annotations.json',annotations)
for name in ['candidate-native-timing-v700','candidate-diagnostic-slices-v702']:
 p=W/name/'identity.json';identity=rd(p);identity.update(runtimeVerified=True,runtimeAudit='../diagnostic-slices-audit-v705.json',runtimeScope='Only the recorded diagnostic timings and crouched coin continuity; no broader recovery or movement completion claim.');wr(p,identity)
p=D/'55-native-diagnostic-hitches.md';text=p.read_text();marker='\n## Final runtime comparison705\n';text=text.split(marker)[0]
text=text.replace('Runtime comparison and visual review are pending at the time of this initial entry; consult the final audit and appended results below before claiming acceptance.','Runtime comparison and six contact-sheet inspections are complete; see the bounded results below.')
text+=marker+'''
Audit705 passes32 checks:8 baseline and12 per new character test. In baseline701, source/observer record52/51 status calls of at least50ms. Both703Corvo and704Emily record zero such status or slice calls during their approximately13-second measurement windows. The budget is soft: startup/history includes individual slow writes, so this is not a universal latency guarantee.

Both new takes activate exact native Sneaking6 + LootObject47, remove the source coin and retain crouch, connected Steam presentation and full-body weight. Minimum observed weight is0.999396 forCorvo and1.0 forEmily. Both full inventories, health and14 protected save/settings hashes are restored. The original18-file integrity refresh runs separately.

All six source/observer contact sheets were inspected. The source coin notification/removal is visible; the remote crouched body remains present on the lectern. Rear viewpoints limit grip and foot-contact evaluation. Corvo703 includes an NPC walking nearby, with no measured health change. Static sheets cannot prove smoother motion or frametime; numerical native timing counters establish the narrower hitch result. No new looting gesture is claimed and peer item removal remains unfinished.

Live702 DLL: `AC0ADACDF58FBEC682D28788B01EE224C4D377C1944F657FE4C4A18770D5DB49`. Probe remains `1ECB428D77B93915EA0417F3531E9103BB8E24DEC2336E874A809BA827810B22`.

Next: continue functional movement/shared-world coverage. Same-identity recovery after other data gaps is still outstanding; do not treat logging changes as a general disconnect recovery implementation. Preserve all earlier failures and do not relax native actor/world/session, load/death or explicit-stop checks. Native semantic sampling remains another measurable cost to investigate if useful.
''';p.write_text(text)
p=D/'README.md';text=p.read_text();link='- [Native diagnostic hitches and bounded scheduling](55-native-diagnostic-hitches.md)';
if link not in text:p.write_text(text.rstrip()+'\n\n'+link+'\n')
print(json.dumps(dict(videos=len(videos),checks=a['checks'],currentCandidate='candidate-diagnostic-slices-v702',completeMovement=False)))
