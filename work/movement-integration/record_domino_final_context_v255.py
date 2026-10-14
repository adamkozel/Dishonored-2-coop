from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
source=P/'domino-crouch-final-observe-v251';assert read(source/'result.json')['passed']
raw=read(source/'crouched-final-raw-proof.json');q=read(source/'states.json')
grounded=[]
for row in q:
 for scene in row['midnight']['instances']:
  if scene.get('state')==3 and scene.get('subscene')=='Cast_final_Select' and [v['id']for v in row['player']['active']]==[6]:
   grounded.append(dict(ms=row['ms'],player=row['player'],scene=scene))
provenance=dict(rawFinalSamples=len(raw),maximumRawClock=max(x['clockWords'][1]for x in raw),pairedGroundRecoverySamples=len(grounded),groundRecovery=grounded,complete=False)
write(source/'final-context-provenance.json',provenance)
t=P/'domino-crouch-final-network-v256';r=read(t/'result.json');rows=read(t/'states.json');rows=rows if isinstance(rows,list) else [rows]
summary=dict(passed=r['passed'],failure=r['failure'],rows=len(rows),minimumRootWeight=min((x['movement']['weight']for x in rows),default=None),clockRejections=sorted({x['movement']['clockRejection']['count']for x in rows}),complete=False,visualAccepted=False)
if (t/'crouch-context-summary.json').exists():summary['contexts']=read(t/'crouch-context-summary.json')
write(t/'coverage-summary.json',summary)
write(W/'candidate-domino-crouch-final-v252/runtime-verification.json',dict(at=datetime.datetime.now().astimezone().isoformat(),take=t.name,result=r,summary=summary,earlierFailure='domino-crouch-final-network-v252: unsupported fixture duration35000, corrected to existing40000; no cast or recording occurred',complete=False))
previous='Candidate222 passes scoped body/marks, partner exclusion, beams, aim cancellation and target deselection. Standing final recoveryv244 passes but receiver framing limits visual judgment. Native crouchv245/v249 and pairedv250 confirm four ordinary phases, continuous root and native standing recovery. '
latest='Native crouched final recoveryv251 records14 reciprocal final400 samples through383ms, including Sneaking6 after casting72 retirement. Candidate252 enables the exact phase. Pairedv256 '+('passes native/peer target selection and removal, final crouched gesture/recovery and native standing recovery.' if r['passed'] else 'fails: '+str(r['failure'])+'.')
notes=previous+latest+' The originalv252 harness rejected unsupported35000 fixture duration before casting; v256 uses supported40000. Bank48/301 unchanged. Holstered/moving/air/slide, shared NPC authority and gameplay outcomes remain unfinished. Visual and complete-power acceptance are not claimed.'
p=P/'review-evidence-annotations.json';ann=read(p);item=ann['items']['emily_domino_mark_link_cancel'];item['notes']=notes
if r['passed']:item['runtime_binding']='native_domino_body_marks_beam_cancel_deselect_recovery_crouch_partial_verified'
write(p,ann)
p=R/'documentation/handoff-2026-10-11/30-native-domino-investigation.md';s=p.read_text(encoding='utf-8-sig');heading='## Crouched final selection and native recovery, v251–v256'
if heading not in s:
 s+=f'''\n\n{heading}

[Native source v251](../../work/natural-moveset/domino-crouch-final-observe-v251/result.json) passes7 checks. Native X is not involved: the first LT selects one guard and a second LT on that guard removes it. [Provenance](../../work/natural-moveset/domino-crouch-final-observe-v251/final-context-provenance.json) retains{len(raw)} reciprocal final400ms samples through{provenance['maximumRawClock']}ms. A separately sampled raw scene at375ms has the player still crouched with only exact Sneaking6 active. The source video shows the native hand recover and the guard mark fade. Candidate250 correctly refused the unimplemented final crouched phase while accepting ordinary crouched phases. No receiver outcome is inferred from this observation.

[Candidate252](../../work/movement-integration/candidate-domino-crouch-final-v252/identity.json) allows final selection with the same exact active scene, reciprocal Emily body/arm/equipment and bounded400ms clock used for standing recovery, now matching Sneaking6 when crouched. Tests reject stale scenes, ordinary selection without state72, duration overruns and missing reciprocal bodies. The complete bank48 is unchanged. [v252 test](../../work/natural-moveset/domino-crouch-final-network-v252/result.json) failed before casting because its harness requested unsupported35000ms; the fixture supports only20000/40000/off. Both checkpoint and protected14 restoration passed. [Corrected harness v256](../../work/natural-moveset/Test-DominoCrouchFinalNetwork-v256.ps1) uses supported40000; no production code change was needed for that correction. The intervening [v254 attempt](../../work/natural-moveset/domino-crouch-final-network-v254/result.json) also stopped before casting: the relative east observer position landed on furniture and moved by centimetres. Protected cleanup passed. The v256 harness chooses a known floor anchor with bounded separation and verifies its height and four consecutive stable samples.

[Paired final v256](../../work/natural-moveset/domino-crouch-final-network-v256/result.json): **{'PASS' if r['passed'] else 'FAIL'}**, {len(r['checks'])} completed checks. Failure if any: `{r['failure']}`. [Coverage](../../work/natural-moveset/domino-crouch-final-network-v256/coverage-summary.json) and [visual review](../../work/natural-moveset/domino-crouch-final-network-v256/visual-review.json) separate numerical continuity, mark ownership and image judgment. Source aiming uses a settled native guard and a physically verified floor. Observer placement uses a previously measured balcony floor and four consecutive stable native ground samples; no source NPC transform is changed. A single selected target must match its peer cosmetic target key before reselection. The final scene must span state72 and Sneaking6 recovery past300ms; the peer must play the selected phase and clear the corresponding mark, then return to standing. Shared NPC gameplay remains outside this cosmetic presentation proof.
'''
 p.write_text(s,encoding='utf-8')
print(json.dumps(summary))

