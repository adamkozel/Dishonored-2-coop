from pathlib import Path
import json,datetime,hashlib,subprocess
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();manifest=rd(N/'runtime-video-manifest.json');cases=[]
takes=['native-shadowwalk-scenes-v522','native-shadowwalk-body-v525','native-shadowwalk-body-v527','native-shadowwalk-travel-v529','native-shadowwalk-travel-v532']
for take in takes:
 result=rd(N/take/'result.json');assert result['passed']==(not take.endswith('525'))
 observations={}
 for name in ('remote-body-observations','long-travel-observations','strict-scene-observations'):
  if (p:=N/take/(name+'.json')).exists():observations[name]=rd(p)
 cases.append(dict(take=take,result=result,observations=observations))
 sheets=[]
 if take.endswith('527'):sheets=['body-sheet.jpg']
 if take.endswith('529'):sheets=['travel-sheet.jpg']
 if take.endswith('532'):sheets=['travel-sheet.jpg','source-sheet.jpg']
 if sheets:
  review=dict(at=now,inspected=True,sheets=sheets,method='Viewed temporal sheets from actual game recordings.',observations=['Remote whole body lowers to a hands-and-feet crawl, advances and reverses with native source movement, then returns to native crouch.','During sustained form the actual native peer sword is holstered.','Appearance is still human; Shadow Walk material and particle presentation are not part of these body-only526 recordings.'],limitations=['Temporal sheets do not prove continuous contact quality.','529 leaves the observer frame;532 improves framing but the far-right column still occludes part of the endpoint.','No final aesthetic acceptance; attack, directional clips, morphology, VFX and broader traversal remain incomplete.'],complete=False,visualAccepted=False)
  if take.endswith('532'):review['observations'].append('Reviewed source shows native low camera and vision during the form, native end prompt, and ordinary crouched sword recovery.')
  wr(N/take/'visual-review.json',review)
 for video in ([N/(take+'.mp4')] if take.endswith('522') else [N/(take+'.mp4'),N/(take+'-source.mp4')]):
  assert video.exists() and not any(x['video']==video.name for x in manifest)
  meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
  source=video.stem.endswith('-source') or take.endswith('522')
  entry=dict(video=video.name,character='emily',label='Shadow Walk '+('source' if source else 'remote body')+' · '+take,alias='shadow_walk_native_reference' if source else 'shadow_walk_body',requirements=['emily_shadow_walk_enter_move_attack_exit'],gameplay_bound=not source and result['passed'],complete=False,visual_accepted=False,evidence=take+'/result.json',proxy_sha256=result['dll'],video_sha256=sha(video),actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),checks_passed=result['passed'],scope=result['scope'])
  if sheets and (not source or take.endswith('532')):entry['visual_review']=take+'/visual-review.json'
  manifest.append(entry)
wr(N/'runtime-video-manifest.json',manifest)
receipt=dict(at=now,cases=cases,bodyNativeBound=True,bodyOnlyCandidate='candidate-shadowwalk-body-v526',currentParticleCandidate='candidate-shadowwalk-fx-v533',particleCandidateRuntimeVerified=False,complete=False,visualAccepted=False)
wr(W/'shadowwalk-body-verification-v534.json',receipt);wr(W/'candidate-shadowwalk-body-v526/runtime-verification-v534.json',receipt)
lookup=json.loads(rd(W/'native-shadowwalk-bookmark-v528.json')['lookup']['structuredContent']['result'])['bookmarks']
bookmarks=rd(R/'work/handoff-project-bookmarks-current.json')
for item in lookup:
 bookmarks=[x for x in bookmarks if (x['address'],x['category'])!=(item['address'],item['category'])];bookmarks.append(item)
wr(R/'work/handoff-project-bookmarks-current.json',bookmarks)
latest='Shadow Walk source522 passes10 checks;524 body attempt525 fails at unrecognized fast holster and is retained.526 recognizes exact Emily333ms fast holster.527 body test passes12 checks;529/532 pass13 each, covering4.61/4.79m and1.90/1.98 distance-driven crawl cycles. Remote native sword holstered, no body clock rejection, source FP retained, checkpoint/inventory/health/protected14 restored. Body-only appearance remains human and is not visually accepted.533 native-particle candidate built/staged for runtime validation; shader/morphology, attacks, directions and wider traversal still incomplete.'
nextWork='Validate533 native arm/hand/ash particles and their ownership/cleanup with paired video; then address shadow material/morphology and remaining Shadow Walk movement/action contexts. Keep509 intermittent airborne landing rejection open. Basic partner exclusions sufficient for now; movement collision allowed.'
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=now;state['ShadowWalkContinuation']=dict(latest=latest,receipt='work/movement-integration/shadowwalk-body-verification-v534.json',complete=False,sourcePassed=True,remoteBound=True,bodyCandidate='candidate-shadowwalk-body-v526',next=nextWork);wr(R/'work/current-task-state.json',state)
ann=rd(N/'review-evidence-annotations.json');ann['items'].setdefault('emily_shadow_walk_enter_move_attack_exit',{}).update(notes=latest,complete=False);wr(N/'review-evidence-annotations.json',ann)
p=D/'36-native-shadow-walk-investigation.md';s=p.read_text(encoding='utf-8')
s+='''

## Native body integration522–534

[522](../../work/natural-moveset/native-shadowwalk-scenes-v522/result.json) passes10 source checks. The exact seven-binding entry reader observes21 samples at25–908ms of the1000ms scene. Sustained form yields139 samples. Exit yields23 samples at33–1108ms of the1133ms scene. Normal LT requests exit. All checkpoints and14 protected files restore. [Reader](../../outputs/d2-coop-lab/src/native_player_shadowwalk_scene.h) distinguishes the native alternate mode from Running0 and preserves scene ownership; sustained-form semantic33 is an owned state, not a synthetic native scene.

[Bank523](../../work/natural-moveset/shadowwalk-bank-manifest-v523.json) adds clips331–334: entry, idle, forward crawl and exit. It uses preserved `shadow-body-v1` complete-body authoring data, normalized shortest-arc quaternion interpolation and120 samples plus the format's wrap sample. Original331 payloads remain byte-identical. Entry uses1000ms; exit uses1133ms and the first60% of the reversed authoring entry to finish crouched. Emily crawl stride is2.421020865m per full authored clip; Corvo's offline comparison stride2.733790874m. Root joints remain identity and no animation event drives damage. D2MOVE58/335 passes actual bank validation. Forward/backward motion phase follows accepted replicated planar distance; backward reverses the initial forward clip. This does not claim dedicated backward/sideways animation coverage.

[Candidate524](../../work/movement-integration/candidate-shadowwalk-body-v524/identity.json) uses semantic55/336 bytes. Its first paired [test525](../../work/natural-moveset/native-shadowwalk-body-v525/result.json) fails before remote transformation. Native preparation uses `sword_unequip_fast.midnightscene`, Emily Scene_1/333ms, with the owned right arm alone. The old equipment reader does not recognize it. Source read stage5 fails six times, then the receiver retires at281ms with `native_state_stale`. The failed source/observer recordings and raw data remain indexed. Cleanup passes; no timeout was relaxed.

[Candidate526](../../work/movement-integration/candidate-shadowwalk-body-v526/identity.json) adds equipment semantic7 and protocol56, preserving336 bytes and bank58. Its exact reader requires Emily, default standing mode0+53, owned right arm, active333ms scene and consistent final identity. It rejects wrong character, duration, arm, retired/expired scene and unsupported overlays. Presentation reuses the complete-body holster clip at the native clock while actual native equipment state controls the attachment contact. The main Shadow Walk body is not mixed with ordinary walking legs.

[527](../../work/natural-moveset/native-shadowwalk-body-v527/result.json) passes12 checks. Entry32, active33 and exit34 arrive over native Steam and reach full remote body weight. The distance phase freezes at rest, advances forward and reverses backward. Actual remote sword is `holster_wpn` during the crawl; normal crouched sword recovers after exit. No body clock rejection occurs. [529](../../work/natural-moveset/native-shadowwalk-travel-v529/result.json) and [532](../../work/natural-moveset/native-shadowwalk-travel-v532/result.json) each pass13 checks with2600ms forward/backward inputs:4.61m/1.90 cycles and4.79m/1.98 cycles respectively. Each restores native inventory/health, both isolated checkpoints and14 protected hashes. [Receipt534](../../work/movement-integration/shadowwalk-body-verification-v534.json) preserves scope and pins.

Viewed actual temporal sheets show the whole-body crawl and recovery.529 loses part of the body outside the observer frame;532 improves the framing but a column still occludes part of the endpoint.532 source review confirms the native low camera/vision/end prompt and crouched sword recovery. The body still appears human, contact quality has no final visual acceptance, and temporal samples do not establish continuous animation quality. Shared attacks/outcomes, shadow morphology/material, native remote VFX, lateral/turning clips, sprint upgrades, rat holes and interruption breadth remain incomplete. The earlier509 airborne landing dropout remains a separate open issue.

## Material and particle investigation528/530; candidate533

Actual Ghidra [string search](../../work/movement-integration/native-shadowwalk-material-strings-v528.json) and [xref](../../work/movement-integration/native-shadowwalk-material-xrefs-v528.json) connect `arkRenderModelAssembly::GetCustomMaterialsPerModel` at141b03d38 to [1401bd420](../../work/movement-integration/native-shadowwalk-1401bd420-decompile-v528.txt). Static dataflow compares/caches custom material lists at `*(this+0x1c40)+0x70/0x78`, builds per-model lists at+0x40, and maps surface records at+0x60/+0x68 from the blueprint at+0x38. It can allocate and rebuild caches; it is not a verified runtime setter. [Bookmark528](../../work/movement-integration/native-shadowwalk-bookmark-v528.json) was actually looked up and the persistent Ghidra program saved.

Read-only [Void export530](../../outputs/d2-coop-lab/research/asset-exports/shadowwalk-particles-v530/manifest.json) preserves seven resources and unchanged original index hashes. The native Emily body material retains ordinary texture maps and enables material effects with a Shadow Walk teleportation parameter block. Simply changing diffuse color is therefore not an established reproduction. Native arm/hand idle, arm/hand locomotion and transformation ashes use separate particle declarations. Full-body material mapping and shader-driver behavior remain hypotheses for runtime investigation.

[Candidate533](../../work/movement-integration/candidate-shadowwalk-fx-v533/identity.json) adds a bounded presentation experiment: up to four native arm/hand/ash effects follow the current published remote Emily elbow/hand matrices. Existing native effect start/stop and exact owned-record validation are reused. A350ms renewable lease, active body/stream/rig ownership and native callback exclusion bound lifetime. Native vision/end-screen FX, power activation, damage and the body material are not invoked. Build/regression/smoke checks pass; runtime and visual results must be recorded separately before treating this as working.
'''
p.write_text(s,encoding='utf-8');print(json.dumps(dict(videos=len(manifest),bookmarks=len(bookmarks),bodyNativeBound=True,complete=False)))
