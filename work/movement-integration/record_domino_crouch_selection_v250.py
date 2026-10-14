"""Record native provenance and the completed paired result, never infer a pass."""
from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
source=P/'domino-crouch-selection-observe-v249';assert read(source/'result.json')['passed']
proof=read(source/'crouched-selection-raw-proof.json');proof=proof if isinstance(proof,list) else [proof]
t=P/'domino-crouch-network-v250';r=read(t/'result.json');rows=read(t/'states.json');rows=rows if isinstance(rows,list) else [rows]
summary=dict(passed=r['passed'],failure=r['failure'],rows=len(rows),nativeSourceSelectionProofSamples=len(proof),minimumRootWeight=min((x['movement']['weight'] for x in rows),default=None),clockRejections=sorted({x['movement']['clockRejection']['count']for x in rows}),complete=False,visualAccepted=False)
if (t/'crouch-context-summary.json').exists():summary['contexts']=read(t/'crouch-context-summary.json')
write(t/'coverage-summary.json',summary)
write(W/'candidate-domino-crouch-select-v250/runtime-verification.json',dict(at=datetime.datetime.now().astimezone().isoformat(),take=t.name,result=r,summary=summary,complete=False))
p=P/'review-evidence-annotations.json';ann=read(p);item=ann['items']['emily_domino_mark_link_cancel']
item['runtime_binding']='native_domino_body_marks_beam_cancel_deselect_recovery_crouch_partial_verified' if r['passed'] else 'native_domino_body_marks_beam_cancel_deselect_recovery_partial_verified'
item['notes']='Candidate222 passes scoped body/marks, partner exclusion, beams, aim cancellation and target deselection. Candidate241/v244 follows the owned final400ms scene through392ms after state72 retires; receiver body framing limits visual judgment. Native crouch provenance v245 In/Loop/Out and v249 ordinary Select400 confirmed. Pairedv246 failed when the unrecognized ordinary selection retired the source stream; candidate250 adds that exact observed phase without relaxing leases. Pairedv250 '+('passes scoped four-phase armed crouch, continuous root and native standing recovery.' if r['passed'] else 'failed: '+str(r['failure'])+'.')+' Bank48/301 retains all prior289 clip payloads. Crouched final selection, other stances, fire/slide, NPC authority and shared outcomes remain unfinished. Complete power and visual acceptance are not claimed.'
write(p,ann)
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md';s=p.read_text(encoding='utf-8-sig');heading='## Crouched ordinary target selection, v246–v250'
if heading not in s:
 s+=f'''\n\n{heading}

[Paired v246](../../work/natural-moveset/domino-crouch-network-v246/result.json) failed after crouched entry. The strict source observations have an approximately437ms gap before aim. Root presentation then retired through the existing250ms source lease. Source video shows target selection. This was an unsupported native phase, not justification for extending stale-state leases or inferring an idle pose.

[Targeted source v249](../../work/natural-moveset/domino-crouch-selection-observe-v249/result.json) passes7 checks. [Raw selection proof](../../work/natural-moveset/domino-crouch-selection-observe-v249/crouched-selection-raw-proof.json) contains{len(proof)} owned ordinary `Cast_Select`400ms samples, reciprocal Emily left-arm and player-body bindings, with independent Sneaking6/Domino72 observations. Candidate246 correctly refused those samples and accepted52 other observed phases. The actual source video shows the original first-person crouched sword grip and left-hand cast with a native guard mark. No receiver/body or shared outcome conclusion follows from that source-only test.

[Candidate250](../../work/movement-integration/candidate-domino-crouch-select-v250/identity.json) extends the guarded reader to ordinary crouched selection, retaining exact active state72, Sneaking6, ownership, duration and scene incarnation checks. Crouched final selection is still refused. Identity fixtures require the observed phase and reject ordinary selection without casting72; existing standing final recovery and all wire sizes remain unchanged. The bank48 payload is unchanged, so candidate246's full format and both actual-bank checks remain applicable. Build, identity, wire and DLL-load checks pass before deployment.

[Paired v250](../../work/natural-moveset/domino-crouch-network-v250/result.json): **{'PASS' if r['passed'] else 'FAIL'}**, {len(r['checks'])} completed checks. [Coverage summary](../../work/natural-moveset/domino-crouch-network-v250/coverage-summary.json) records the actual root and phase context results. Failure, if any: `{r['failure']}`. Raw Midnight observation and both source/receiver stream diagnostics are retained. The harness permits native target selection and checks its corresponding peer phase instead of incorrectly assuming an empty cast; cosmetic world evidence remains separate from shared NPC gameplay. [Visual review](../../work/natural-moveset/domino-crouch-network-v250/visual-review.json) records inspected camera/sequence limits; a numerical pass is not final visual acceptance. Protected14 hashes and checkpoint recovery are part of the final receipt.
'''
 p.write_text(s,encoding='utf-8')
print(json.dumps(summary))
