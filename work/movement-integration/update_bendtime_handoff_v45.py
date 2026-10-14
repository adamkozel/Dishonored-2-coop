"""Refresh movement handoff with measured live identities and scoped results."""
import datetime,hashlib,json,shutil
from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work'; P=W/'natural-moveset'; D=R/'documentation/handoff-2026-10-11'
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x): p.write_text(json.dumps(x,indent=2),encoding='utf-8')
def sha(p):
 with p.open('rb') as f:return hashlib.file_digest(f,'sha256').hexdigest().upper()
now=datetime.datetime.now().astimezone().isoformat(); live=sha(W/'asset-game/dinput8.dll'); probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
code_archive={'296BC0E7ACCB700A86D0D4BCCC7E7EAE1926B813D07892627A219511EB2E74F7':'candidate-outofmana-v45'}[live]
banks=read(P/'bendtime-balanced-bank-manifest-v42.json')
archive=W/'movement-integration/candidate-bendtime-balanced-v42'
if not archive.exists():
 archive.mkdir()
 files=['build_bendtime_balance_v42.py','bendtime-balanced-bank-manifest-v42.json','bendtime-balanced-source-manifest-v42.json','Test-BendTimeContexts.ps1','Trace-BendTimePresentation.ps1']
 manifest=[]
 for f in files+[x['bank']for x in banks]:
  source=P/f; dest=archive/source.name; shutil.copy2(source,dest); manifest.append(dict(source=str(source.relative_to(R)),archive=dest.name,sha256=sha(dest)))
 write(archive/'identity.json',dict(created=now,base_source_archive='../candidate-bendtime-contexts-v41',dll='459C82C0A41C9EA11DDB1C91A4AF673724DFF5ABA3B47793BB262C2CB4A7F1F7',probe='4F275F0D33134E47D25CEA7A1E6A8D2643AF5314DE2450506BBA35A0BF7A1A57',files=manifest,complete=False))
checks=[]
for f in read(W/'movement-integration/protected-save-hashes-before-v4.json'):
 actual=sha(Path(f['Path'])); checks.append(dict(path=f['Path'],expected=f['Hash'],actual=actual,passed=actual==f['Hash']))
steam=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2')
for name,expected in [('Dishonored2.exe','B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'),('steam_api64.dll','71AF8666392ACAD2080A816B9F1196FE9CCB950E1DB8A549F8312C4D231F55A8')]:
 for folder in [steam,W/'asset-game']:
  f=folder/name; actual=sha(f);checks.append(dict(path=str(f),expected=expected,actual=actual,passed=actual==expected))
integrity=dict(at=now,passed=all(x['passed']for x in checks),checks=checks,liveProxy=live,candidate=code_archive+' (code) / candidate-bendtime-balanced-v42 (bank)',probe=probe,isolatedExecutable=sha(W/'asset-game/Dishonored2.exe'),originalExecutable=sha(steam/'Dishonored2.exe'))
assert integrity['passed']; write(W/'handoff-integrity-verification.json',integrity)
state=read(W/'current-task-state.json'); m=state['MovementIntegration']; latest=m['LatestContinuation']
latest.update(live=live,liveBuild=live,bank='D2MOVE40 / 248 clips, out-of-mana v45 candidate; prior balanced v42 banks remain supported',actual_game_videos=len(read(P/'runtime-video-manifest.json')),next='Finish movement contexts and failure presentation, then remaining full movement checklist',in_progress='Native power failure, travel/air combinations and lifecycle review',complete=False)
cases={}
for pattern in ['bendtime-corvo-*/result.json','bendtime-viewer-*/result.json','native-bendtime-corvo-*/result.json','bendtime-source-*/result.json','bendtime-receiver-*/result.json','outofmana-*/result.json']:
 for f in P.glob(pattern):cases[f.parent.name]=read(f)
m['PowerFailureV45']=dict(code_archive='movement-integration/'+code_archive,bank_archive='movement-integration/candidate-outofmana-v45',bank_manifest='natural-moveset/outofmana-bank-manifest-v45.json',cases=cases,scope='Scoped Corvo standing armed native failure; prior ground and airborne cases; pause ownership recovery. No full quality acceptance, observer camera effect replication or shared time authority.',complete=False)
state['UpdatedAt']=now; write(W/'current-task-state.json',state)
s=(D/'01-current-state.md').read_text(encoding='utf-8'); start='<!-- BENDTIME-CURRENT -->'; end='<!-- /BENDTIME-CURRENT -->'
section=f'''{start}
## Current movement continuation

Updated {now}. Measured live proxy `{live}` and probe `{probe}`. [Current code milestone](../../work/movement-integration/{code_archive}), [balanced crouch candidate](../../work/movement-integration/candidate-bendtime-balanced-v42), [latest bank pins](../../work/natural-moveset/outofmana-bank-manifest-v45.json), [fresh integrity report](../../work/handoff-integrity-verification.json).

Corvo's Bend Time cast/end runs through native Steam using actual scene timing. Ground stance/equipment, walking and armed airborne cases have scoped passing takes. Either player's native pause now has an automatic bank/ownership recovery test; videos still show a temporary avatar visibility gap. v45 adds the exact native 333ms insufficient-mana response and passes eleven checks in the standing armed Corvo case. Frontal visual review, broader contexts and the complete movement checklist remain active work. Asset-only hold remains unbound; native viewer blur is refused by the network path. See [Bend Time findings](15-bend-time-integration.md) and [failure/recovery evidence](16-power-failure-and-resume.md).

The v38 compact-release fix and earlier working milestones are retained. The whole movement phase is unfinished; clip and recording counts do not mean acceptance. Every named take, including failures, is preserved in [current state](../../work/current-task-state.json), the [dashboard](http://127.0.0.1:8769/review.html) and the generated evidence index.
{end}'''
assert start in s and end in s;s=s[:s.index(start)]+section+s[s.index(end)+len(end):];(D/'01-current-state.md').write_text(s,encoding='utf-8')
print(json.dumps(dict(at=now,cases=len(cases),integrity_checks=len(checks),passed=integrity['passed'],live=live)))
