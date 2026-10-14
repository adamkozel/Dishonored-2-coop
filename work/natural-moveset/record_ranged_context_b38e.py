"""Preserve the inspected B38E airborne and directional crouched test evidence."""
import datetime
import hashlib
import json
import shutil
from pathlib import Path

P = Path(__file__).resolve().parent
ROOT = P.parents[1]
BUILD = 'B38E7E422D9EC6033D8ED5FF77074F485613E4B20AAD6EB8EA66BCE9DABB689F'
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p, value):
    t = p.with_suffix(p.suffix + '.pending')
    t.write_text(json.dumps(value, indent=2), encoding='utf-8')
    assert read(t) == value
    t.replace(p)
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest().upper()

air = ['ranged-corvo-gun-air-v28-v3', 'ranged-emily-crossbow-air-v28-v1']
moving = ['ranged-corvo-crouch-gun-moving-v30-v4', 'ranged-emily-crouch-crossbow-moving-v30-v4']
for take in moving:
    r = read(P / take / 'result.json')
    assert r['passed'] and r['dll'] == BUILD and r['travelling'] and r['crouched']
    review = dict(inspected=True, accepted=False,
        method='Paired native-source and remote-body temporal sheets inspected at twelve recorded times.',
        contact_sheets=[f'{take}/remote-sheet.jpg', f'{take}/source-sheet.jpg'],
        observations=[
            'Whole remote body remains visible while traversing the scripted directions in a crouch.',
            'Sampled stepping and ranged actions are visible together; stopping returns to a crouched hold.',
            'Source first-person equipment, crouch view and native ammunition display remain visible.',
        ],
        limitations=[
            'Wide oblique view does not establish close finger grip, reload contact or sword alignment.',
            'Temporal sheets do not establish continuous naturalness, cadence or foot sliding quality.',
            'No captured audio; no remote projectile or impact synchronization proof.',
            'Aim pitch, upgrades, continuous native weapon switching and full lifecycle remain incomplete.',
        ])
    if r['kind'] == 'gun':
        review['observations'].append('Both native muzzle flashes are visible; the remote spark fan is conspicuously large.')
        review['limitations'].append('World muzzle scale/orientation requires comparison and refinement; not visually accepted.')
    write(P / take / 'visual-review.json', review)

cases = []
for take in air + moving:
    r = read(P / take / 'result.json')
    v = read(P / take / 'visual-review.json')
    assert r['passed'] and r['dll'] == BUILD and v['inspected'] and not v['accepted']
    cases.append(dict(take=take, checks=len(r['checks']), result=f'work/natural-moveset/{take}/result.json',
                      source=r['source'], kind=r['kind'], airborne=r['airborne'], travelling=r['travelling']))

findings_path = P / 'ranged-air-native-findings-v28.json'
f = read(findings_path)
f['remote_runtime']['failure'] = 'Historical 5E32/D0DE attempts rejected a final falling segment near landing; diagnostic evidence is retained below.'
f['remote_runtime']['scope'] = 'Historical candidates are superseded by the bounded B38E landing fix. These records remain failed; visual acceptance remains false.'
f['landing_fix']['runtime'].update(emily=air[1], checks=sum(x['checks'] for x in cases[:2]), samples=145,
                                 visual_accepted=False, findings='ranged-air-runtime-findings-v28.json')
write(findings_path, f)

archive = ROOT / 'work/movement-integration/candidate-air-landing-B38E-v1'
assert sha(archive / 'dinput8.dll') == BUILD
milestone = ROOT / 'work/movement-integration/verified-ranged-context-B38E-v1'
assert not milestone.exists(), 'Preserve the existing milestone rather than overwrite it'
milestone.mkdir()
files = []
for name in ['Test-RangedNetwork.ps1', 'ranged-air-native-findings-v28.json', 'ranged-air-runtime-findings-v28.json', 'crouched-ranged-bank-manifest-v4.json']:
    src = P / name
    shutil.copy2(src, milestone / name)
    files.append(dict(source=str(src.relative_to(ROOT)), snapshot=name, sha256=sha(src)))
for case in cases:
    dest = milestone / case['take']
    dest.mkdir()
    for name in ['result.json', 'visual-review.json']:
        src = P / case['take'] / name
        shutil.copy2(src, dest / name)
        files.append(dict(source=str(src.relative_to(ROOT)), snapshot=f'{case["take"]}/{name}', sha256=sha(src)))
banks = []
for role in ['corvo', 'emily']:
    src = P / 'crouched-ranged-banks-v4' / f'{role}-crouched-ranged-bank.bin'
    preserved = ROOT / 'work/movement-integration/verified-crouched-ranged-7CD4-v4' / src.name
    assert sha(src) == sha(preserved)
    banks.append(dict(role=role, sha256=sha(src), preserved=str(preserved.relative_to(ROOT))))
write(milestone / 'manifest.json', dict(created=datetime.datetime.now().astimezone().isoformat(),
    dll=BUILD, source_snapshot=str(archive.relative_to(ROOT)), protocol=28, packet_bytes=260,
    bank_version=30, bank_revision=4, banks=banks, files=files, cases=cases,
    checks=sum(x['checks'] for x in cases), complete=False, visual_accepted=False,
    scope='Partial native ranged presentation with single jump/reload/landing and eight-direction crouched travel. Exact test scope and visual limitations are retained; this does not complete movement.'))
state_path = P.parent / 'current-task-state.json'
s = read(state_path)
assert len(s) >= 49
m = s['MovementIntegration']
m['liveBuild'] = BUILD
m['current'] = 'D2MOVE30 / 225 clips; Steam action protocol28 / 260 bytes. Partial movement port with recorded ranged airborne and directional crouched cases. Complete movement and visual acceptance unfinished.'
m['LatestContinuation'].update(live=BUILD, liveBuild=BUILD, complete=False,
    next='Native ranged muzzle presentation comparison and remaining movement contract: traversal, combat, items, paired actions, powers and lifecycle.')
m['AirborneRangedExperiment'].update(runtime='Corvo gun and Emily crossbow pass 23 checks over 145 samples: native airborne fire, reload, empty and landing recovery; no pose/prop refusals. Corvo has two source-confirmed native muzzle effects; Emily cocked/fired mechanism persists. Visual acceptance false.',
    checkedTakes=air, verifiedMilestone=str(milestone.relative_to(ROOT)))
m['CrouchedRangedExperiment']['movingTests'] = moving
m['CrouchedRangedExperiment']['movingChecks'] = sum(x['checks'] for x in cases[2:])
s['UpdatedAt'] = datetime.datetime.now().astimezone().isoformat()
write(state_path, s)
print(f'Preserved {len(cases)} B38E cases / {sum(x["checks"] for x in cases)} checks; complete=false.')
