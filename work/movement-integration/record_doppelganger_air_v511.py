from pathlib import Path
import json,hashlib,datetime,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
assert rd(W/'native-operation-owner.json')['state']=='finished'
identity=rd(W/'candidate-doppelganger-air-v508/identity.json');assert sha(R/'work/asset-game/dinput8.dll')==identity['dll']
now=datetime.datetime.now().astimezone().isoformat()
takes=['native-doppelganger-air-release-v506','native-doppelganger-air-entry-holstered-v507','doppelganger-air-release-armed-v509','doppelganger-air-entry-holstered-v510']
manifest=rd(N/'runtime-video-manifest.json');cases=[]
for take in takes:
    result=rd(N/take/'result.json')
    assert any(x.startswith('Both isolated checkpoints restore') for x in result['checks'])
    assert any(x.startswith('All14 protected') for x in result['checks'])
    native=take.startswith('native-')
    observation=rd(N/take/('air-context-observations.json' if native else 'air-verification.json'))
    observation.pop('airRows',None);observation.pop('fastAir',None)
    cases.append(dict(take=take,passed=result['passed'],failure=result['failure'],checks=len(result['checks']),air=observation))
    for suffix in (('',) if native else ('','-source')):
        video=N/(take+suffix+'.mp4')
        if not video.exists() or any(x['video']==video.name for x in manifest):continue
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        review=N/take/('visual-review'+suffix+'.json')
        scope='Native airborne Doppelganger source observation and candidate508 two-instance Steam presentation. Exact input/native phase and equipment contexts are recorded per take. Air locomotion remains under replicated native motion; clone presentation remains cosmetic. Broader movement, gameplay and final visual quality are unfinished.'
        if not review.exists():wr(review,dict(inspected=False,complete=False,visualAccepted=False,scope=scope))
        manifest.append(dict(video=video.name,character='emily',label=f'Doppelganger air · {take} · {"source" if native or suffix else "remote"}',alias='power_doppelganger_air',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=result['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=result['passed'],scope=scope))
wr(N/'runtime-video-manifest.json',manifest)
passed=all(c['passed'] for c in cases)
receipt=dict(at=now,scopedPassed=passed,complete=False,live=identity['dll'],probe=identity['probe'],protocol=54,bankVersion=57,clips=331,cloneWireVersion=4,cases=cases,scope='Recorded native source and paired armed release/holstered entry cases. No new bank or assets. Does not establish every interruption, moving cast, upgrade, multi-clone or shared gameplay outcome.')
wr(W/'doppelganger-air-verification-v511.json',receipt)
if passed:wr(W/'candidate-doppelganger-air-v508/runtime-verification-v511.json',receipt)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now
latest=state['DoppelgangerContinuation']['latest'];marker=' Native airborne511:'
if marker in latest:latest=latest[:latest.index(marker)]
latest+=marker+(' source506/507 and paired509/510 pass native airborne phase, Steam body/air blend, ground recovery,219-joint cosmetic clone and native FX checks. ' if passed else ' some recorded tests failed; inspect511 receipt. ')
latest+='Candidate508 semantic54/336, bank57/331, clone4/10712. Prior ground/cancellation evidence remains scoped to its recorded build. General movement quality, interrupts, upgrades/multiple clones and shared outcomes remain open.'
state['DoppelgangerContinuation'].update(at=now,receipt='work/movement-integration/doppelganger-air-verification-v511.json',latest=latest,next='Close remaining movement and native power presentation gaps, then shared NPC/world authority. Defer partner-only edge cases and retain normal movement collision.',complete=False)
wr(R/'work/current-task-state.json',state)
annotations=rd(N/'review-evidence-annotations.json');annotations['items']['emily_doppelganger_cast'].update(notes=latest,complete=False);wr(N/'review-evidence-annotations.json',annotations)
chapter=D/'35-doppelganger-cast-and-clone-lifecycle.md';s=chapter.read_text(encoding='utf-8');marker='## Airborne casting506–511'
if marker in s:s=s[:s.index(marker)].rstrip()
s+='''

## Airborne casting506–511

Native source [506](../../work/natural-moveset/native-doppelganger-air-release-v506/result.json) holds LT, then uses A+LT180ms and releases during the jump. Independent raw Midnight/player samples capture the exact reciprocal666ms release while airborne. [507](../../work/natural-moveset/native-doppelganger-air-entry-holstered-v507/result.json) sheathes the sword using native X while looking away, then uses A+LT700ms together. Its raw samples confirm entry466ms during Jump; the strict15ms watcher also records Jump/Fall with UsePower63. Both return to measured floor Z66.4370575, retain health/inventory, and restore both checkpoints and protected14. The prior strict ground-only reader accepts zero airborne samples. Native first-person contact sheets show camera rise, casting hand, placement preview, summon and return; they are not third-person quality approval.

[Candidate508](../../work/movement-integration/candidate-doppelganger-air-v508/identity.json) allows exactly one matching native locomotion class: Running0, Sneaking6, or standing-physics Jump2/Fall3. UsePower63 is optional only for the observed release phase. Exact item/scene/arm ownership, duration checks and final identity rereads remain mandatory. Native airborne metadata travels on semantic54/336; old53 contracts, unknown equipment, invalid apex and stale fall apex are rejected. Playback keeps the existing authored cast and blends pelvis/leg locomotion from the native-air clock. Position remains controlled by the existing root channel. Bank57/331, clone4/10712 and native cosmetic effect assets are unchanged. Build, native ownership, wire, proxy and actual-bank tests pass; none is visual acceptance.

[Armed release509](../../work/natural-moveset/doppelganger-air-release-armed-v509/result.json), [holstered entry510](../../work/natural-moveset/doppelganger-air-entry-holstered-v510/result.json) and [511 receipt](../../work/movement-integration/doppelganger-air-verification-v511.json) record the paired runtime outcomes. The assertions require independently observed native and remote airborne phases, original equipment, continuous body weight, no rejected presentation clocks, landing recovery, the existing219-joint cosmetic clone and native FX lifetime checks, plus inventory/health/checkpoint/protected14 restoration. Consult the actual results and saved visual reviews for outcomes and limitations. These cases do not prove complete shared clone AI/damage, upgrades, multiple clones, moving casts or every interruption.
'''
chapter.write_text(s,encoding='utf-8')
readme=D/'README.md';s=readme.read_text(encoding='utf-8');line='- [Emily Doppelganger casting, clone presentation and native effects](35-doppelganger-cast-and-clone-lifecycle.md)'
if line not in s:readme.write_text(s.rstrip()+'\n'+line+'\n',encoding='utf-8')
print(json.dumps(dict(scopedPassed=passed,complete=False,videos=len(manifest),cases=[{k:v for k,v in c.items() if k!='air'} for c in cases])))
