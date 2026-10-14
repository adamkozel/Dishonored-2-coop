from pathlib import Path
import json,hashlib,datetime,subprocess,collections
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
def wr(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
now=datetime.datetime.now().astimezone().isoformat()
assert rd(W/'native-operation-owner.json')['state']=='finished'
manifest=rd(N/'runtime-video-manifest.json')
takes=('doppelganger-fx-v450','doppelganger-fx-v453','doppelganger-fx-v455','doppelganger-fx-v457','doppelganger-fx-v460')
summaries=[]
for take in takes:
 result=rd(N/take/'result.json');assert 'All14 protected save/settings hashes retained' in result['checks']
 assert any(c.startswith('Both isolated checkpoints restore') for c in result['checks'])
 summaries.append(dict(take=take,passed=result['passed'],failure=result['failure'],checks=len(result['checks']),dll=result['dll']))
 for suffix in ('','-source'):
  video=N/(take+suffix+'.mp4');name=video.name;assert not any(x['video']==name for x in manifest),name
  meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
  inspected=take=='doppelganger-fx-v450' or (take=='doppelganger-fx-v453' and not suffix)
  sheet=N/take/('source-sheet.jpg' if suffix else 'remote-sheet.jpg')
  review=N/take/('visual-review'+suffix+'.json')
  scope='Native source-owned clone FX research; summon burst and body visible on remote453. Disappearance pause and same-world reuse remain unresolved. '+('Checks passed in this limited take.' if result['passed'] else 'Failed take retained: '+result['failure'])
  wr(review,dict(inspected=inspected,contactSheet=sheet.relative_to(N).as_posix() if sheet.exists() else None,scope=scope,complete=False,visualAccepted=False))
  manifest.append(dict(video=name,character='emily',label=f'Doppelganger cosmetic effects · {"source" if suffix else "remote"} ·{take.rsplit("v",1)[1]}',alias='native_doppelganger_cast',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),video_sha256=sha(video),checks_passed=result['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
# Diagnose450 without rewriting its failed result.
rows=rd(N/'doppelganger-fx-v450/states.json')
sent={h['sequence']:h for row in rows for h in row['cloneSource']['history']}
accepted={h['sequence']:h for row in rows for h in row['clonePeer']['history']}
tail={h['sequence']:h for h in rd(N/'doppelganger-fx-v450/source-history-tail-after-cleanup.json')['history']}
missing=[h for sequence,h in accepted.items() if sequence not in sent]
assert len(missing)==2 and all(tail[h['sequence']]['digest']==h['digest'] for h in missing)
assert all(sent[sequence]['digest']==h['digest'] for sequence,h in accepted.items() if sequence in sent)
wr(N/'doppelganger-fx-v450/endpoint-audit-v461.json',dict(missingFromFinalSample=missing,exactlyMatchedByPreservedSenderTail=True,originalResultStillFailed=True,explanation='Receiver status contained the final two clear packets published after the final sender sample.453 captures sender tail after receiver samples.'))
current=rd(R/'work/current-task-state.json');identity=rd(W/'candidate-doppelganger-fx-v459/identity.json')
assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
trace=rd(R/'work/lab-instances/host/coop-status.json.doppelganger-visual.json')['fx'].get('pauseTrace',[])
receipt=dict(at=now,complete=False,diagnosticBuild=True,takes=summaries,pauseTrace=trace,live=identity['dll'],probe=identity['probe'],bodyBaseline='candidate-doppelganger-visual-v427/runtime-verification-v431.json',scope='Native summon FX now visible over Steam; default Emily clone still presentation-only. Disappearance stalls in native paused state5 and must be resolved before promotion. Same-world reuse not completed.')
wr(W/'doppelganger-fx-investigation-v461.json',receipt)
current['DoppelgangerContinuation'].update(at=now,live=identity['dll'],probe=identity['probe'],receipt='work/movement-integration/doppelganger-fx-investigation-v461.json',latest=receipt['scope'],complete=False,next='Inspect459 native pause trace from460, resolve short disappearance scene stalling, then remove diagnostic hook. Re-run source/remote native FX checks and same-actor second summon without checkpoint between.453 summon visuals inspected;455 follow teleport disrupted stream and is withdrawn. Preserve native local first person/UI/Steam, protected14, prior427 body milestone. Partner interaction edge cases remain deferred and native movement collision is allowed.')
current['UpdatedAt']=now;wr(R/'work/current-task-state.json',current)
notes=rd(W/'ghidra-bookmarks-additions-v458.json');books=rd(R/'work/handoff-project-bookmarks-current.json');books=[b for b in books if not any(b['address']==n['address'] and b['category']==n['category'] for n in notes)]+notes;wr(R/'work/handoff-project-bookmarks-current.json',books)
with(D/'35-doppelganger-cast-and-clone-lifecycle.md').open('a',encoding='utf-8') as f:
 f.write('''

## Native cosmetic summon/disappearance implementation448–461

Assets441 were installed into the stopped physical lab copy by449. [Install receipt](../../outputs/d2-coop-lab/research/doppelganger-fx-v441-asset-install.json) records the backup and exact resources; original Steam indices and protected14 remained unchanged. The preceding “staged only” status describes447 and is superseded by this installation.

[448 implementation](../../work/movement-integration/implement_doppelganger_fx_v448.py) extends the separate clone channel fromv2/10696 to **v3/10712** with strict source scene-known/ID/variant/time fields. Semantic protocol52/336 and banks55/322 remain unchanged. Scene incarnation changes send reliably; ordinary pose frames remain the existing stream. Failed scene reads do not interrupt clone root/bone snapshots. Source handles remain opaque on the peer.

The receiver binds only its four-component cosmetic clone to two native-derived FX/material scenes. It deduplicates by stream/world/clone/scene identity, never pooled pointers; initializes native elapsed time from the presented source clock; and stops on confirmed source completion or loss of ownership. It does not execute animation events or create a peer gameplay clone. [Source](../../outputs/d2-coop-lab/src/doppelganger_fx_presentation.h), [native scene wrapper](../../outputs/d2-coop-lab/src/doppelganger_fx_scene.h), [448 archive](../../work/movement-integration/candidate-doppelganger-fx-v448/identity.json).

[450](../../work/natural-moveset/doppelganger-fx-v450/result.json) retained a failed packet-evidence check. Two final clear packets517/518 were published between the final sender and receiver file samples; both exact digests exist in the preserved source tail. [Supplementary audit](../../work/natural-moveset/doppelganger-fx-v450/endpoint-audit-v461.json) documents this without changing the failed result.453 captures the final sender history after receiver sampling and passes exact digest comparison.

[452](../../work/movement-integration/candidate-doppelganger-fx-v452/identity.json) reduces initial cosmetic model wait200→16ms while preserving all actual model/component/rig checks. In453 the native summon starts at source clock54ms, compared with250ms in450. [Inspected remote sheet](../../work/natural-moveset/doppelganger-fx-v453/remote-sheet.jpg) visibly shows native white burst/rings, emergence, body rise and walking. Source and peer recordings remain in the dashboard. This is positive summon evidence, not complete power acceptance.

[453](../../work/natural-moveset/doppelganger-fx-v453/result.json) found a real disappearance problem. The native remote scene starts in running state3 and then is independently observed in **paused state5**, with its clock stalled at38ms. The source scene continues25–458ms. The previously logged `verified` start only meant immediate native start/identity success; it did not establish sustained playback. Do not treat `started[1,1]` as complete birth/death-effect evidence.

[455](../../work/natural-moveset/doppelganger-fx-v455/result.json) attempted to follow the walking clone with observer teleports; transport retired with `native_state_stale` around17s. This camera method is withdrawn. No second reuse take456 was run. [457](../../work/natural-moveset/doppelganger-fx-v457/result.json) keeps the stationary camera and captures the paused native object. Its source-helper assertion also encountered FX retirement between observations.460 requires at least three complete independent helper/FX-map snapshots per phase, records retiring snapshots separately, and retains the original unique scene/owner/key checks. A helper entity or track record can outlive active particles; presence alone never establishes fresh emission.

### Exact-image pause investigation

Ghidra confirms [instance pause7cfa40](../../work/movement-integration/native-midnight-pause-1407cfa40-v454.txt), called by the native sequence manager7d71f0, sets state5. [Initialization](../../work/movement-integration/native-midnight-initialize-1407ce370-v458.txt), [pool reset](../../work/movement-integration/native-midnight-pool-init-1407caae0-v458.txt), [start](../../work/movement-integration/native-midnight-start-1407d2160-v454.txt) and [resume](../../work/movement-integration/native-midnight-resume-1407d8950-v458.txt) are preserved. A bookmark at1407cfa40 distinguishes the confirmed paused state from the unconfirmed visibility/culling explanation. PE instruction/unwind scans are read-only and hash-guarded. Interior function fragments absent from Ghidra were resolved using chained Windows unwind metadata and existing disassembly.

[459 diagnostic build](../../work/movement-integration/candidate-doppelganger-fx-v459/identity.json) temporarily traces native manager pauses only for our exact currently owned cosmetic scene IDs. It passes original arguments/results through unchanged. Its verified12-byte entry trampoline does not suppress or resume scenes. Remove it once the caller is identified. [460 result](../../work/natural-moveset/doppelganger-fx-v460/result.json) and [461 investigation receipt](../../work/movement-integration/doppelganger-fx-investigation-v461.json) preserve the actual trace and failures. These candidates are not promoted as fully verified; the427 body milestone remains available.

Remaining: sustained disappearance emission/fade, clear useful disappearance footage, same-model second summon/material recovery, interrupted/reload/session cleanup, alternate stance/equipment/cast contexts, upgrades/multiple clones and shared clone gameplay. Native first person, original Steam files and protected saves remain preserved. No skill was invoked for this C++/PowerShell/Ghidra/Void diagnostic work.
''')
print(json.dumps(dict(videos=len(manifest),takes=summaries,pauseTrace=trace,complete=False)))
