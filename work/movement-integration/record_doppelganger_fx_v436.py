from pathlib import Path
import json,hashlib,shutil,datetime,subprocess,collections
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';N=R/'work/natural-moveset';D=R/'documentation/handoff-2026-10-11';A=W/'candidate-doppelganger-fx-probe-v434'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p,v):p.write_text(json.dumps(v,indent=2),encoding='utf-8')
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();identity=rd(A/'identity.json');result=rd(N/'doppelganger-fx-v435/result.json')
assert result['passed'] and len(result['checks'])==13 and result['dll']==identity['dll']==sha(R/'work/asset-game/dinput8.dll') and result['probe']==identity['probe']==sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
rows=[json.loads(s) for s in (N/'doppelganger-fx-v435/native-effects.ndjson').read_text(encoding='utf-8-sig').splitlines() if s.strip()]
groups=collections.defaultdict(list)
for row in rows:
    if row['valid']:
        for fx in row['effects']:groups[fx['name']].append(fx)
summary=dict(samples=len(rows),valid=sum(r['valid'] for r in rows),worldStable=sum(r['worldStable'] for r in rows),failures=sorted(set(r['failureLine'] for r in rows)),maxReadMs=max(r['readMs'] for r in rows),effects=[])
for name,fxs in sorted(groups.items()):
    summary['effects'].append(dict(name=name,observations=len(fxs),ids=sorted(set(f['id'] for f in fxs)),owners=sorted(set(f['owner'] for f in fxs)),parents=sorted(set(f['parent'] for f in fxs)),ownerMatches=sum(f['cloneOwnerMatches'] for f in fxs),parentMatches=sum(f['cloneParentMatches'] for f in fxs),ownerTypes=sorted(set(f['ownerType'] for f in fxs)),frameTypes=sorted(set(f['frameType'] for f in fxs))))
assert summary['valid']>100 and len(summary['effects'])==6
assert all(e['ownerMatches']==e['parentMatches']==0 for e in summary['effects'])
assert all(e['parents']==[0x3fff0010] for e in summary['effects'] if '/spawn_despawn/' in e['name'])
scope='Read-only435 FX inventory observes native target, spawn and despawn records. The four clone effect kinds use separate idEntity helpers without clone parent handles; direct owner/parent attribution is invalid. Exact scene/track/helper linkage remains a static hypothesis requiring runtime validation. Existing clone body/Steam/health/checkpoint/protected14 regression passes13 checks. No remote FX yet.'
summary.update(scope=scope,complete=False);wr(N/'doppelganger-fx-v435/effects-summary.json',summary)
wr(A/'runtime-verification-v436.json',dict(at=now,result=result,effects=summary,complete=False,scope=scope));identity.update(runtimeVerified=True,runtimeVerificationScope=scope);wr(A/'identity.json',identity)
for p in [Path(__file__),W/'Run-DoppelgangerFx-v435.ps1',N/'Test-DoppelgangerFx-v435.ps1',W/'doppelganger-fx-cli-preflight-v434.ndjson',W/'midnight-fx-property-rtti-v436.json',W/'midnight-fx-track-rtti-v436.json',W/'ghidra-bookmarks-additions-v436.json',*W.glob('native-doppelganger-fx-*-decompile-v436.txt')]:shutil.copy2(p,A/p.name)
books=rd(R/'work/handoff-project-bookmarks-current.json');notes=rd(W/'ghidra-bookmarks-additions-v436.json');books=[b for b in books if not any(b['address']==n['address'] and b['category']==n['category'] for n in notes)]+notes;wr(R/'work/handoff-project-bookmarks-current.json',books)
manifest=rd(N/'runtime-video-manifest.json')
for version in (433,435):
    take=f'doppelganger-fx-v{version}';res=rd(N/take/'result.json')
    for suffix in ('','-source'):
        name=take+suffix+'.mp4';assert not any(x['video']==name for x in manifest);video=N/name
        meta=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration','-of','json',str(video)],text=True))['streams'][0]
        note=scope if version==435 else '433 retained failed FX-observer invocation: CLI whitelist omitted the new mode. All13 body/world/cleanup checks passed, but the FX watcher exited2 and emitted no observations.434 corrects the argument gate and passes17/17 valid no-cast preflight samples before435.'
        review=N/take/('visual-review'+suffix+'.json');wr(review,dict(inspected=False,contactSheet=None,scope='Recording retained; visual clone comparison is covered by viewed430. '+note,complete=False,visualAccepted=False))
        manifest.append(dict(video=name,character='emily',label=f'Doppelganger native FX observation · {"source" if suffix else "remote"} ·{version}',alias='native_doppelganger_cast',requirements=['emily_doppelganger_cast'],gameplay_bound=False,complete=False,visual_accepted=False,evidence=take+'/result.json',visual_review=review.relative_to(N).as_posix(),proxy_sha256=res['dll'],actual_capture_hz=int(meta['nb_frames'])/float(meta['duration']),video_sha256=sha(video),checks_passed=res['passed'],scope=note))
wr(N/'runtime-video-manifest.json',manifest)
state=rd(R/'work/current-task-state.json');state['DoppelgangerContinuation'].update(at=now,live=identity['dll'],probe=identity['probe'],receipt='work/movement-integration/candidate-doppelganger-fx-probe-v434/runtime-verification-v436.json',bodyReceipt='work/movement-integration/candidate-doppelganger-visual-v427/runtime-verification-v431.json',latest=scope,complete=False,next='Validate native clone Midnight scene/group/track ownership.435 effects are unparented helpers. Static84c810/84c9e0 expose track+c0,count+c8 records of12bytes {keyId,helperHandle,stopMode}; track+10 group, group+d0 bound actor. Use native watcher --watch-midnight or strict reader to prove reciprocal clone scene binding, timing and FX linkage. Derive cosmetic-only spawn/despawn scene/FX with native particles; omit AI/screen/first-person side effects. Then integrate source-owned remote effects. Partner edge cases deferred; collision retained.');state['UpdatedAt']=now;wr(R/'work/current-task-state.json',state)
with (D/'35-doppelganger-cast-and-clone-lifecycle.md').open('a',encoding='utf-8') as f:f.write('''

## Native effect ownership investigation432–436

[435 effect summary](../../work/natural-moveset/doppelganger-fx-v435/effects-summary.json) contains286 read-only samples:283 valid FX maps and285 stable clone-world snapshots. Three concurrent map changes return unknown at reader line69; they are not interpreted as effect absence. The external full-map observer reaches4.2714ms and is not installed as an engine-frame scanner. Existing13 clone/body/Steam/health/checkpoint/protected-file checks pass with the unchanged verified427 DLL. The current read-only probe434 is pinned in [verification436](../../work/movement-integration/candidate-doppelganger-fx-probe-v434/runtime-verification-v436.json).

The observer finds target valid/invalid effects, spawn root/body effects and despawn root/body effects. All four clone effect kinds are owned by separate `idEntity` helpers with an ordinary reference-frame component and invalid entity parent `0x3fff0010`. No direct owner or parent matches the native clone. Consequently, a nearby effect or matching name cannot establish which clone owns it. FX records can also persist beyond the one-second scene/particle emission; discovering a record later is not a new power event. Preview effects remain source-local.

Initial433 failed because the new read-only mode was missing from the probe argument whitelist. All13 existing clone/cleanup checks passed, but the observer exited2 with no data. Its failed result, recording and stderr are retained.434 fixes the gate, and a one-second no-input preflight produces17/17 valid snapshots before435. The harness also now rejects an observer that exits before the cast. No game DLL, asset or save correction was needed.

[Void export436](../../outputs/d2-coop-lab/research/asset-exports/native-doppelganger-fx-v436/manifest.json) captures the exact four spawn/despawn `.fx` declarations, with original Steam indices unchanged. The native spawn scene supplies five one-second FX keys: one `decoy_spawn_01.fx` on `decoy_root`, and four `decoy_spawn_body_01.fx` keys on `sl_l_arm`, `sl_r_arm`, `decoy_spawn_l_hand` and `decoy_spawn_r_hand`. Separate tracks also control material effects/fade and sound. The root FX includes cast sound, a `game_defined/bsp_ai_properties_02` sound entry and rumble; cosmetic replication must inspect these rather than assume the whole scene is presentation-only.

Exact-image RTTI identifies [arkMidnightKeyPropFx](../../work/movement-integration/midnight-fx-property-rtti-v436.json), vtable1afdfd0, and [arkMidnightTrackInstFx](../../work/movement-integration/midnight-fx-track-rtti-v436.json), vtable1c7f5a8. A preliminary lookup for the guessed name `arkMidnightTrackFx` found no RTTI and changed nothing; the actual decorated type was then read from the pinned executable.

[84c7a0](../../work/movement-integration/native-doppelganger-fx-14084c7a0-decompile-v436.txt) dispatches key start to [84c810](../../work/movement-integration/native-doppelganger-fx-14084c810-decompile-v436.txt). Static candidates: track+10 points to its group; group+d0 is the bound actor; key+28 is the FX property; property+8 is the declaration and property+10 its socket.84c810 creates a separate helper, binds it through875e80 and appends a12-byte record `{keyId, helperHandle, stopMode}` to track+c0/count+c8. [84c9e0](../../work/movement-integration/native-doppelganger-fx-14084c9e0-decompile-v436.txt) resolves that helper, stops its effect according to the stored mode and removes the record. These layout/ownership relationships are static hypotheses until a reciprocal runtime reader confirms them. The finding is saved in Ghidra at14084c810.

Next: observe the clone's exact native Midnight scene and prove scene→group→track→key/helper linkage and source scene clock. Then create a cosmetic-only native-derived effect scene and bind it to the already verified219-joint clone. Keep animation and gameplay events out of that derivative, preserve source-only outcomes, and suppress duplicate/stale births. Source-side FX observations are not remote playback completion.
''')
print(json.dumps(dict(videos=len(manifest),samples=summary['samples'],valid=summary['valid'],probe=identity['probe'],complete=False)))
