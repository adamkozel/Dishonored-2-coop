from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
t=P/'domino-final-body-v239';r=read(t/'result.json');assert r['passed']
strict=[json.loads(x)for x in(t/'native-final-scenes.ndjson').read_text().splitlines()]
raw=[dict(tick=x['tick'],scene=s)for line in(t/'native-deselect-scenes.ndjson').read_text().splitlines()for x in[json.loads(line)]for s in x.get('instances',[])if s.get('state')==3 and s.get('subscene')=='Cast_final_Select']
valid=[x for x in strict if x.get('valid')and x.get('subscene')=='Cast_final_Select']
ground=[x for x in strict if raw[0]['tick']<=x['tick']<=raw[-1]['tick']and x.get('activeStates')==[0]]
summary=dict(take=t.name,checks=len(r['checks']),rawFinalSamples=len(raw),rawFinalSpanMs=raw[-1]['tick']-raw[0]['tick'],strictFinalSamples=len(valid),strictFinalMaxClockMs=max(x['timeMs']for x in valid),groundOnlyWhileRawFinalActive=len(ground),scope='Candidate237 verifies final entry but fades early because state72 retires while native scene is still active. Candidate241 enables exact owned final scene recovery bounded to400ms; further runtime required.')
write(t/'final-select-timing-summary.json',summary)
write(W/'candidate-domino-final-v237/runtime-verification.json',dict(at=datetime.datetime.now().astimezone().isoformat(),result=r,summary=summary,complete=False))
a=P/'review-evidence-annotations.json';ann=read(a);item=ann['items']['emily_domino_mark_link_cancel'];item['runtime_binding']='native_domino_body_marks_beam_cancel_deselect_partial_verified';item['notes']='Candidate222: standing body/markv218, partner exclusionv212, beamsv223/v225, aim cancellationv233 and target deselectionv235 pass. Candidate237/v239 passes14 checks for final gesture entry, links and cleanup, but video/timing proves early body recovery: native state72 ends around100ms inside a400ms scene. Candidate241 follows exact active final scene to its native end; runtime pending. Crouch/air/holstered, fire/slide, shared NPC authority and outcomes remain unfinished.';write(a,ann)
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md';s=p.read_text(encoding='utf-8-sig');heading='## Final selection entry and native recovery, v238–v242'
if heading not in s:
 s+=f'''\n\n{heading}

[v238](../../work/natural-moveset/domino-final-body-v238/result.json) stopped before the final-gesture test: tutorial guard1 occupied the stairs at z64.2325 while the source settled on the lower floor at z61.3049. Same-floor targeting was an invalid harness assumption. [v239 harness](../../work/natural-moveset/Test-DominoFinalSelect-v239.ps1) requires stable native source ground and uses [measured native yaw and elevation](../../work/natural-moveset/Aim-NativeDominoGuard-v239.ps1), without moving an NPC or directly invoking the power. This helper is bounded to5m vertical/20m horizontal separation and verifies source position after native right-stick input.

[v239 final receipt](../../work/natural-moveset/domino-final-body-v239/result.json) passes14 scoped checks. Two native links, target reselection, surviving-mark identity, peer beam removal, independent presentation shutdown, inventories, health, checkpoint cleanup and all14 protected hashes pass. Paired video sheets are indexed. **The receipt is not complete gesture acceptance:** visual/timing inspection found premature remote arm recovery. [Timing summary](../../work/natural-moveset/domino-final-body-v239/final-select-timing-summary.json) has {len(raw)} raw final-scene samples across{summary['rawFinalSpanMs']}ms, but only{len(valid)} strict-reader samples through{summary['strictFinalMaxClockMs']}ms. Native state72 retires while the exact same final scene remains active; {len(ground)} strict observations show grounded state0 during that remaining scene interval. The receiver began recovery before the source hand finished.

[Candidate241](../../work/movement-integration/candidate-domino-recovery-v241/identity.json) now permits only the exact `Cast_final_Select` scene to finish on state0, with the same reciprocal Emily body/left-arm/equipment checks and clock bounded to its400ms native duration. Ordinary `Cast_Select` still requires state72. Wrong subscene, duration, ownership, recycled incarnation, retired scene and a clock past400ms remain rejected. Identity, wire and load tests pass; [the revised runtime test](../../work/natural-moveset/Test-DominoFinalRecovery-v241.ps1) requires samples after state72 retirement and beyond300ms, not merely final-gesture entry. Refer to its completed receipt for runtime outcome.

[v240 crouch observation](../../work/natural-moveset/domino-crouch-observe-v240/result.json) stopped before recording/casting: the harness mistakenly used LeftThumb instead of this setup's native B crouch binding. Inventory/checkpoint/protected-file cleanup passed. [Corrected v242 harness](../../work/natural-moveset/Observe-NativeDominoCrouch-v242.ps1) establishes native B crouch before casting and records raw state6/state72 with their exact native vtables. The current production reader deliberately refuses crouched Domino; raw provenance must precede production support. No footage exists forv240 and it is not indexed as animation evidence.
'''
 p.write_text(s,encoding='utf-8')
print(json.dumps(summary))
