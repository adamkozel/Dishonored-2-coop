"""Refresh current high-throw evidence while preserving historical records."""
import datetime,hashlib,json,shutil
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent;D=R/'documentation/handoff-2026-10-11'
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest().upper()
def write(p,v):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(v,indent=2));t.replace(p)
stamp=datetime.datetime.now().astimezone().isoformat();statepath=R/'work/current-task-state.json'
backup=R/'work/movement-integration/current-task-state-before-high-throw.json'
if not backup.exists():shutil.copy2(statepath,backup)
state=read(statepath);m=state['MovementIntegration'];live=sha(R/'work/asset-game/dinput8.dll');cases={}
for folder in sorted(P.glob('native-high-throw-*')):
 if not folder.is_dir() or not(folder/'result.json').exists():continue
 result=read(folder/'result.json');identity=read(folder/'identity.json')if(folder/'identity.json').exists()else{}
 validation=read(folder/'high-throw-validation.json')if(folder/'high-throw-validation.json').exists()else None
 cases[folder.name]=dict(harnessPassed=result['passed'],failure=result.get('failure'),identity=identity,
  validation=validation,visual=read(folder/'visual-review.json')if(folder/'visual-review.json').exists()else None)
m['NativeHighThrowContinuation']=dict(at=stamp,liveDll=live,probe=sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'),
 bankManifest='work/natural-moveset/high-throw-bank-manifest-v31.json',cases=cases,complete=False,
 confirmed=['Exact high throw variant8 uses selector2, Drop46, 666ms, flags101 and camera unit direction times declaration strength4.',
 'v33 stops the exact victim scene, requests ragdoll after retirement, waits mode0/pending-1, and queues one native event149. Both role v33 wall-facing cases pass23 pickup plus21 throw checks.',
 'Native829e30 calls829180 before the drop scene; native CarryCorpse declaration names spine03 for carry, spine02 for drop and anchor_jnt for attachment.'],
 hypotheses=['The v31/v32 pre-stop ragdoll request may be ignored or superseded; the transition was not sampled sufficiently to distinguish those mechanisms.',
 'Native-style retained attachment during high release may improve the abrupt victim placement. Current candidate requires its own runtime/visual evidence.'],
 gaps=['Supporting hand/body alignment, natural throw acting and final visual acceptance.',
 'Native soft/fetal/other release variants; away-facing near-furniture case selected variant5 and intentionally received no high-throw impulse.',
 'Arbitrary corpses, pitched throws, cancellation/pause/disconnect, authority and collision outcomes across both worlds.'])
latest=m['LatestContinuation'];latest.update(live=live,liveBuild=live,bank='D2MOVE36 / 234 clips; first233 payloads unchanged',
 actual_game_videos=len(read(P/'runtime-video-manifest.json')),in_progress='Native high-throw physics handoff and retained attachment/visual comparison',
 next='Inspect per-take release/contact evidence. Complete safe native release variants and body contact before generalizing carry ownership.',complete=False)
latest['last_validated_throw_fix']='48A17FA29C9BF0DE418C06F1F5A9BCEBF73E9B30D3F000101C3D4C98A114C520'
state['UpdatedAt']=stamp;state['DocumentationHandoff']['UpdatedAt']=stamp;write(statepath,state)
for version in [33,34]:
 path=R/f'work/movement-integration/candidate-high-throw-v{version}/identity.json'
 if not path.exists():continue
 archive=read(path);valid=[name for name,c in cases.items()if c['validation']and c['validation']['passed']and c['identity'].get('dllSha256')==archive['dllSha256']]
 archive.update(runtimeValidated=bool(valid),runtimeCases=valid,visualAccepted=False,complete=False);write(path,archive)
table='''<!-- HIGH-THROW-CURRENT -->
## High-throw continuation (latest)

Updated: STAMP. Live experimental proxy: `LIVE`.

Probe: `PROBE`. The bank is D2MOVE36 with234clips; all233 prior clip payloads remain byte-identical. See [bank hashes](../../work/natural-moveset/high-throw-bank-manifest-v31.json), [native throw findings](13-native-body-throw.md) and the per-take results in [current state](../../work/current-task-state.json).

The v33 physics milestone passes23 pickup checks and21 throw checks for each named Corvo/Emily standing wall-facing take. One native impulse is queued after scene retirement and ragdoll transition. Actual videos show forward travel, but hand/body alignment and complete throw quality remain unaccepted. The separately archived v34 candidate tests retaining the attachment until the ragdoll boundary. Read its own result before treating it as validated.

Preserved rollback builds: [v33 throw physics](../../work/movement-integration/candidate-high-throw-v33) and [v30 low-drop/stance](../../work/movement-integration/candidate-carry-stance-anchor-v30). Original Steam assets and14 protected save/config files are checked independently; runtime experiments are limited to the physical clone.
<!-- /HIGH-THROW-CURRENT -->

'''.replace('STAMP',stamp).replace('LIVE',live).replace('PROBE',sha(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'))
path=D/'01-current-state.md';text=path.read_text(encoding='utf-8')
if '<!-- HIGH-THROW-CURRENT -->' in text:
 start=text.index('<!-- HIGH-THROW-CURRENT -->');end=text.index('<!-- /HIGH-THROW-CURRENT -->',start)+len('<!-- /HIGH-THROW-CURRENT -->');text=text[:start]+text[end:].lstrip('\n')
text=text.replace('## Current movement candidate','## Preserved v30 movement milestone').replace('| Live v30 proxy SHA-256 |','| Preserved v30 proxy SHA-256 |').replace('| Probe v26 SHA-256 |','| Preserved probe v26 SHA-256 |')
text=text.replace('## Preserved v30 movement milestone',table+'## Preserved v30 movement milestone',1);path.write_text(text,encoding='utf-8')
print(json.dumps(dict(live=live,cases=len(cases),complete=False)))
