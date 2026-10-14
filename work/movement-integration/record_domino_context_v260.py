"""Collect only completed context receipts, preserving source-side failures."""
from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
source=[]
for stance in ('standing','crouched'):
 t=P/f'domino-holstered-{stance}-observe-v253';r=read(t/'result.json');obs=read(t/'holstered-scene-observations.json')
 source.append(dict(take=t.name,passed=r['passed'],failure=r['failure'],stance=stance,castingHolstered=obs['castingHolsteredSamples'],castingDrawn=obs['castingDrawnSamples'],phases=obs['phases']))
cases=[]
for t in sorted(P.glob('domino-context-*-v2*')):
 if not (t/'result.json').exists():continue
 r=read(t/'result.json');rows=read(t/'states.json') if (t/'states.json').exists() else [];rows=rows if isinstance(rows,list)else [rows]
 summary=dict(take=t.name,passed=r['passed'],failure=r['failure'],stance=r.get('stance'),holstered=r.get('holstered'),move=r.get('move'),rows=len(rows),minimumRootWeight=min((x['movement']['weight']for x in rows),default=None),clockRejections=sorted({x['movement']['clockRejection']['count']for x in rows}),complete=False,visualAccepted=False)
 if (t/'crouch-context-summary.json').exists():summary['context']=read(t/'crouch-context-summary.json')
 write(t/'coverage-summary.json',summary);cases.append(dict(result=r,summary=summary))
for archive in sorted(set(W.glob('candidate-domino-context*'))|set(W.glob('candidate-domino-final-context*'))):
 if not (archive/'identity.json').exists():continue
 pins=read(archive/'identity.json');matched=[x for x in cases if x['result']['dll']==pins['dll'] and x['result']['probe']==pins['probe']]
 write(archive/'runtime-verification.json',dict(at=datetime.datetime.now().astimezone().isoformat(),nativeSource=source,cases=matched,complete=False))
p=P/'review-evidence-annotations.json';ann=read(p);item=ann['items']['emily_domino_mark_link_cancel']
item['notes']='Domino scoped body/marks, partner exclusion, beams, aim cancellation, target deselection and standing final recovery remain preserved. Paired armed crouchv250/v256 verifies all ordinary/final gestures, continuous root, target removal and standing recovery; railing/darkness limit fine visual acceptance. Native holsteredv253 confirms ordinary phases with sword remaining unequipped in standing and crouch. Standing source passes; crouched source fails inventory audit because native X pickpockets a nearby key (checkpoint/protected14 restore). Candidate258 validates the same equipment identity across reads and enables bank48 empty-hand contexts, but its first paired run exposed a remaining source/wire armed-only gate. Candidate261 shares that equipment-context rule between sender and receiver and uses semantic protocol45/336bytes. Paired tests look away before holstering. Native v263 standing/crouched holstered final observations each pass7 checks; candidate264 accepts their exact400ms scene and grounded recovery with equipment/ownership guards retained. Bank48/301 and protocol45 remain unchanged. Completed paired contexts: '+('; '.join(f"{x['summary']['take']} {'PASS' if x['summary']['passed'] else 'FAIL: '+str(x['summary']['failure'])}" for x in cases) or 'none yet')+'. Broader air/slide/interruptions, shared NPC authority and gameplay outcomes remain unfinished. No complete-power or final visual acceptance.'
write(p,ann)
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md';s=p.read_text(encoding='utf-8-sig');start='<!-- DOMINO-CONTEXT-V258 -->';end='<!-- /DOMINO-CONTEXT-V258 -->'
section=f'''{start}
## Holstered native contexts and guarded playback, v253–v264

[Standing source observation](../../work/natural-moveset/domino-holstered-standing-observe-v253/result.json) passes7 checks. Native X holsters the sword, then LT casts without redrawing it. Four independent state samples retain Running0/Domino72 and an unequipped sword; raw owned In266/Loop1333/Select400/Out266 scenes are recorded. The armed-only candidate252 accepted zero of these scenes as intended. [Crouched source observation](../../work/natural-moveset/domino-holstered-crouched-observe-v253/result.json) confirms the equivalent four holstered casting samples with Sneaking6/Domino72 but **fails its inventory audit**: native X simultaneously pickpockets `dunwall_key_emilyoffice.inventoryitem` from0 to1. Video visibly shows the Royal Quarters Key notification. The isolated checkpoint restoration and all14 protected hashes pass. The source phase observations are useful evidence; the overall take remains failed.

[Candidate258](../../work/movement-integration/candidate-domino-context-v258/identity.json) permits the observed ordinary holstered phases, retains reciprocal item/player/arm ownership, exact state and scene guards, and requires identical sword equipment identity across the initial and final reads. Initial reader tests caught a leftover drawn-sword-only final check; that failed log is preserved and the complete corrected identity/wire/load suite passes. Holstered final selection is still unobserved and refused. Bank48 already contains the complete standing/crouched empty-hand poses; no payload changes were needed. Old bank47 retains its standing/armed-only playback scope.

The [paired context harness](../../work/natural-moveset/Test-DominoContextNetwork-v258.ps1) uses the established distant source/observer floor positions, aims upward with native look input before holding X, then restores the original measured test pitch. This prevents interaction targeting during holster; unchanged inventory remains a mandatory assertion. It checks actual stance/equipment on both source and peer, native phase IDs and peer events, root clocks, sword state, optional movement, native standing recovery and full cleanup. Recordings remain separate from final visual acceptance.

The [first paired standing run](../../work/natural-moveset/domino-context-standing-holstered-v258/result.json) failed: the strict native reader accepted the scene, but source stage4 still restricted holstered casts to variants10–18. After six refused source reads, the250ms stream lease retired presentation. The receiver's protocol44 predicate also still refused holstered Domino. [Candidate261](../../work/movement-integration/candidate-domino-context-wire-v261/identity.json) centralizes the source/wire equipment-context rule, allows the observed Domino variants19–22, and uses semantic protocol45 with the same336-byte layout. It explicitly rejects old44 packets, unsupported air, absent equipment, wrong character and wrong duration. Full identity/wire/load checks pass; runtime receipts below determine acceptance. No stale lease was enlarged.

| Completed paired take | Stance | Holstered | Movement input | Result |
|---|---|---|---|---|
'''
for case in cases:
 c=case['summary'];section+=f"| [{c['take']}](../../work/natural-moveset/{c['take']}/result.json) | {c['stance']} | {c['holstered']} | {c['move']} | {'PASS' if c['passed'] else 'FAIL: '+str(c['failure'])} |\n"
section+='''
Native final-selection observations now pass7 checks in both [standing](../../work/natural-moveset/domino-holstered-standing-final-observe-v263/result.json) and [crouched](../../work/natural-moveset/domino-holstered-crouched-final-observe-v263/result.json) empty-hand contexts. The exact400ms scene is sampled12 times from25–366ms standing and14 times from0–392ms crouched. Independent probes retain an unequipped sword and show ground-only recovery after casting72 retires; these sequential probes do not claim atomicity. Source recordings show the guard mark appear and clear on reselection. Original inventory, health and protected14 hashes pass.

[Candidate264](../../work/movement-integration/candidate-domino-final-context-v264/identity.json) removes the formerly unobserved holstered-final refusal. It keeps exact scene identity/duration, native ground state, reciprocal arm/body bindings and stable equipment rereads. New negative tests reject a clock past400ms, retired scene, missing body or foreign arm. Build/identity/wire/load pass. Bank48/301 and protocol45 are unchanged; completed paired receipts in the table determine runtime coverage.

The [candidate261 runtime record](../../work/movement-integration/candidate-domino-context-wire-v261/runtime-verification.json) and [candidate264 runtime record](../../work/movement-integration/candidate-domino-final-context-v264/runtime-verification.json) preserve exact completed receipts and numerical summaries. The moving holstered crouchv263 source travels1.2415metres;23 cast samples also move at0.1525–0.4145m/s on the peer with rootweight1. Reviewed sheets show whole-body translation, changing leg support and left-arm casting, followed by standing recovery. Fine foot sliding and finger quality still need a closer useful angle. Shared NPC movement, damage/outcomes and complete campaign power support remain unfinished.
'''+end
if start in s:s=s[:s.index(start)]+section+s[s.index(end)+len(end):]
else:s+='\n\n'+section+'\n'
p.write_text(s,encoding='utf-8')
print(json.dumps(dict(native=source,contexts=[x['summary']for x in cases])))
