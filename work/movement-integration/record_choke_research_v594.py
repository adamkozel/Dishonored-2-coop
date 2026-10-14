from pathlib import Path
import json,hashlib,datetime,subprocess,collections
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
at=datetime.datetime.now().astimezone().isoformat();videos=rd(N/'runtime-video-manifest.json')
for number in (590,591):
 F=N/f'native-choke-cancel-corvo-v{number}';r=rd(F/'result.json');assert not r['passed']
 video=F/'host-rear-choke.mp4';rel=video.relative_to(N).as_posix();assert not any(v['video']==rel for v in videos)
 note=('Observer fixture failed before input; no native cancellation evidence.' if number==590 else 'Native source sheet inspected in preceding turn: back600, repeated loop1400, then guard breaks free through distinct choke_combat_lose/Npc1500. Expected cancel400 was NOT observed. Source reader captured four reciprocal NPC/player/arm bindings, exact state41 and physical stance0 in combat lose. Remote breakout was unbound on584; no completed remote coverage.')
 review=dict(at=at,inspected=number==591,visualAccepted=False,complete=False,sheets=['source-sheet.jpg'] if number==591 else [],observations=[note]);rp=F/'source-visual-review.json';wr(rp,review)
 if number==591:assert (F/'source-sheet.jpg').is_file()
 m=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=duration,nb_frames','-of','json',str(video)],text=True))['streams'][0]
 videos.append(dict(video=rel,character='corvo',label='Corvo · native choke cancellation research · FAILED expectation',alias='struggle_and_escape',requirements=['struggle_and_escape','nonlethal_takedown_front_back_sides'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=F.name+'/result.json',visual_review=F.name+'/'+rp.name,proxy_sha256=r['dll'],video_sha256=hashlib.sha256(video.read_bytes()).hexdigest().upper(),actual_capture_hz=int(m['nb_frames'])/float(m['duration']),checks_passed=False,scope=note))
wr(N/'runtime-video-manifest.json',videos)
p=R/'work/current-task-state.json';s=rd(p);s['UpdatedAt']=at;s['ChokeContinuation']['latest']+=' Native591 early input release failed expected cancel400 but confirmed separate combat-lose1500, state41/stance0/four owned bindings. Candidate592 adds actor-only body release/recoil using existing donors, bank64/352 with prior351 unchanged; runtime pending. No new tools or third-party co-op investigation.';s['ChokeContinuation']['candidate']='candidate-choke-breakout-v592';s['ChokeContinuation']['next']='Validate breakout593 for both roles and preserve actual recordings, then continue broad functional interactions/traversal; defer fine visual polish.';s['MovementExecutionPriority']['current']=s['ChokeContinuation']['latest'];wr(p,s)
print(json.dumps(dict(recordings=len(videos),nativeCancellationVerified=False,breakoutCandidate='592')))
