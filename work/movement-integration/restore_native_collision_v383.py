"""Withdraw collision-only experiment after the user's explicit scope correction."""
from pathlib import Path
import datetime, hashlib, json, shutil
R = Path(__file__).resolve().parents[2]
W = R / 'work/movement-integration'
L = R / 'outputs/d2-coop-lab'
A = W / 'candidate-possession-partner-v374'
E = W / 'candidate-partner-collision-v380'
receipt = W / 'native-collision-restoration-v383.json'
assert not receipt.exists(), 'Already prepared; inspect receipt instead of replaying'
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest().upper()
identity = json.loads((A / 'identity.json').read_text(encoding='utf-8-sig'))
assert identity['dll'] == sha(A / 'dinput8.dll') == 'AD06C1A322AD45C7A511A1D74AB1058156F87DC527041F36CB82C894D6500BF2'
assert identity['probe'] == sha(A / 'd2_runtime_probe.exe')
for name in ('engine_bridge.cpp', 'world_identity_tests.cpp'):
    assert sha(L/'src'/name) == sha(E/'src'/name), 'Preserve unrecognized newer edits'
for name in ('partner_collision_identity.h', 'partner_collision_identity_tests.h', 'partner_collision_filter.h'):
    assert sha(L/'src'/name) == sha(E/'src'/name), 'Research must remain archived'
for name in ('engine_bridge.cpp', 'world_identity_tests.cpp'):
    shutil.copy2(A/'src'/name, L/'src'/name)
for name in ('partner_collision_identity.h', 'partner_collision_identity_tests.h', 'partner_collision_filter.h'):
    (L/'src'/name).unlink()
for name in ('dinput8.dll', 'd2_runtime_probe.exe'):
    shutil.copy2(A/name, L/'bin'/name)
    assert sha(L/'bin'/name) == sha(A/name)
source = {p.name: sha(p) for p in (L/'src').iterdir() if p.is_file()}
baseline = {p.name: sha(p) for p in (A/'src').iterdir() if p.is_file()}
assert source == baseline, 'Current source must exactly match the verified374 archive'
decision = dict(created=datetime.datetime.now().astimezone().isoformat(),
    userInstruction='Movement collision is fine',
    decision='Retain normal native partner movement collision; stop pass-through work. All other partner interaction exclusions remain required.',
    baseline=str(A.relative_to(R)), dll=identity['dll'], probe=identity['probe'],
    sourceFiles=len(source), sourceMatchesVerified374=True, deploymentVerified=False,
    scope='Source and staging binaries restored. Native deployment and read-only verification still required. No bank/assets/save edits.')
receipt.write_text(json.dumps(decision, indent=2), encoding='utf-8')
failure=dict(candidate=380, test=381, collisionExclusionVerified=False,
    outcome='Pair hook suppressed Evolved-body pairs but both players still stopped in front of each other. Collection and checkpoint restoration passed; pass-through behavior failed.',
    additionalFinding='Read-only census382 identifies a separate partner Navigation body, arkCharacterUserData vtable RVA1b977d8. Its relevance was a hypothesis, not a tested fix.',
    disposition='Withdrawn after user accepted native movement collision. No extension to Navigation body or movement collision hooks planned.',
    evidence=['work/natural-moveset/partner-collision-candidate-v381/result.json','work/movement-integration/partner-bodies-host-v382.json','work/movement-integration/partner-bodies-client-v382.json'])
(E/'withdrawal-v383.json').write_text(json.dumps(failure, indent=2), encoding='utf-8')
doc=R/'documentation/handoff-2026-10-11/34-partner-interaction-exclusion.md'
t=doc.read_text(encoding='utf-8')
t=t.replace('interaction prompts and body blocking.', 'interaction prompts. Normal native movement collision between partners is explicitly allowed by the user.')
t=t.replace('| Movement collision | Not yet verified across locomotion/traversal/physics | Prove partners pass through one another without shoving, climbing, blocking or creating physics impulses |', '| Movement collision | User clarification: normal movement collision is fine | Preserve native collision; walk-through behavior is not a requirement. Candidate380 is withdrawn. |')
t=t.replace('| Possession | Test 373 confirmed partner selection and transfer; candidate 374 builds and passes synthetic identity tests | Runtime exclusion and ordinary NPC positive control pending |', '| Possession | Partner test374 and ordinary guard control375 pass; details below | Broader context/lifecycle regression coverage remains |')
t += '\n\n## User clarification and withdrawal of collision candidate, 383\n\nThe user explicitly clarified: **Movement collision is fine.** Normal movement collision is allowed; the exclusion requirement continues to cover combat, powers, targeting, contextual actions and other direct interactions. Do not resume pass-through implementation without a new request. Earlier descriptions of collision as a defect refer to the superseded interpretation.\n\nCandidate380 failed behavioral test381 despite suppressing some ragdoll body pairs. The separate Navigation body found in read-only census382 was a research lead, not a verified correction. The experiment, failed result and exact-image findings remain archived. Active source and staged binaries have been restored byte-for-byte to verified374, preserving all existing damage and power exclusions and bank54/319. Deployment verification is recorded separately in `work/movement-integration/native-collision-restoration-v383.json`.\n'
doc.write_text(t,encoding='utf-8')
road=R/'documentation/handoff-2026-10-11/02-roadmap.md'
t=road.read_text(encoding='utf-8').replace('powers, damage, prompts, contextual actions and body blocking.', 'powers, damage, prompts and contextual actions. Normal native movement collision between partners is allowed, as explicitly clarified by the user.')
road.write_text(t,encoding='utf-8')
print(json.dumps(decision))
