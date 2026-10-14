"""Reconcile current, historical and experimental evidence without promoting completion."""
import json,hashlib,datetime
from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset';D=R/'documentation/handoff-2026-10-11'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat();state=read(W/'current-task-state.json');m=state['MovementIntegration'];latest=m['LatestContinuation']
latest.update(live=sha(W/'asset-game/dinput8.dll'),liveBuild=sha(W/'asset-game/dinput8.dll'),bank='D2MOVE38 / 238 clips; Bend Time hold authored but unbound',actual_game_videos=len(read(P/'runtime-video-manifest.json')),
 next='Complete Bend Time paired animation/context/VFX verification, then continue remaining movement checklist',
 in_progress='Bend Time body phases and viewer FX; complete movement phase remains unfinished',
 last_validated_compact_release_fix='A46597540535879DE144EE173C18D7F995195B16FC43EC35F35BCD86F3BBB09D',complete=False)
m['NativeCompactReleaseV38']=dict(archive='movement-integration/candidate-compact-hold-v38',
 confirmed='Native compact victim pose retained during mode3-to-ragdoll handoff, eliminating v35 upright pop in both named role recordings. Both high throws retain one impulse. Corvo standing low drop and Emily crouched low drop regressions pass.',
 hypothesis='166ms handoff inferred from native victim animation endpoint, not a native ragdoll event.',
 remaining='Carrier hand contact, recovery acting, arbitrary bodies and interrupted lifecycle remain unaccepted.',complete=False)
cases={}
for take in ['native-bendtime-corvo-v39','native-bendtime-corvo-hold-v39','bendtime-corvo-body-v40-v1','bendtime-corvo-body-v40-v2']:
 if(P/take/'result.json').exists():cases[take]=read(P/take/'result.json')
m['BendTimeV40']=dict(archive='movement-integration/candidate-bendtime-body-v40',bank_manifest='natural-moveset/bendtime-bank-manifest-v40.json',cases=cases,
 scope='Corvo armed standing cast start/release only. Variant13 hold remains refused. No shared world-time authority or remote player FX implemented yet.',complete=False)
state['UpdatedAt']=now;write(W/'current-task-state.json',state)
ann=read(P/'review-evidence-annotations.json');ann['items']['carry_body_lift_idle_move_drop_throw']['notes']='Scoped native pickup, stance-change, low-drop, high-throw and compact-release experiments are preserved by build. v38 removes the compact-release upright pop in both roles; named high throws and low drops regress successfully. Final hand contact, recovery, arbitrary bodies and interrupted lifecycle remain unfinished. Historical failed comparisons remain explicitly labelled.';write(P/'review-evidence-annotations.json',ann)
checks=[]
for f in read(W/'movement-integration/protected-save-hashes-before-v4.json'):
 actual=sha(Path(f['Path']));checks.append(dict(path=f['Path'],expected=f['Hash'],actual=actual,passed=actual==f['Hash']))
original=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe')
integrity=dict(at=now,passed=all(x['passed']for x in checks),checks=checks,liveProxy=sha(W/'asset-game/dinput8.dll'),candidate='candidate-bendtime-body-v40',probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'),isolatedExecutable=sha(W/'asset-game/Dishonored2.exe'),originalExecutable=sha(original))
assert integrity['passed']and integrity['isolatedExecutable']==integrity['originalExecutable']=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180';write(W/'handoff-integrity-verification.json',integrity)
doc=D/'01-current-state.md';s=doc.read_text();s=s.replace('## High-throw continuation (latest)','## Historical high-throw continuation (v34)')
start='<!-- BENDTIME-CURRENT -->';end='<!-- /BENDTIME-CURRENT -->'
section=f'''{start}
## Current continuation: compact release and Bend Time

Updated {now}. Live experimental proxy `{integrity['liveProxy']}`; probe `{integrity['probe']}`. Archived [v40 source/binaries](../../work/movement-integration/candidate-bendtime-body-v40), [D2MOVE38 banks](../../work/natural-moveset/bendtime-bank-manifest-v40.json), [fresh protected-file hashes](../../work/handoff-integrity-verification.json).

The v38 compact-release fix eliminates the upright victim pop in both named role recordings and passes high-throw/low-drop regressions. Hand contact and broad lifecycle acceptance remain pending. See [contextual release findings](14-contextual-body-release.md).

Bend Time's native source acquisition, toggle and cleanup pass. Corvo start/release body phases are now implemented through the native Steam action stream. The first paired take visibly plays them, but its slow external sampling missed one short phase and its test fails; it is not promoted to a passed take. Read the subsequent per-take results in [current state](../../work/current-task-state.json). The asset-only hold is deliberately unbound. Remote viewer effects and shared time authority remain separate unfinished work. See [Bend Time investigation](15-bend-time-integration.md).

Movement is not complete. Evidence counts and authored clips are not acceptance percentages. [Dashboard](http://127.0.0.1:8769/review.html) and generated requirement tables preserve all failures and coverage limits.
{end}'''
if start in s:s=s[:s.index(start)]+section+s[s.index(end)+len(end):]
else:s=s.replace('<!-- HIGH-THROW-CURRENT -->',section+'\n\n<!-- HIGH-THROW-CURRENT -->')
doc.write_text(s)
print(json.dumps(dict(updated=now,protected_files=len(checks),passed=integrity['passed'],cases=len(cases))))
