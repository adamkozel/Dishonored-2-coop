"""1374 (#11, stagger_knockdown_recovery): sample the native NPC push-to-ragdoll falls (generic_pushtoragdoll_front/right)
and get-ups from the ground (generic_recovery_front/back/left/right), tall and female skeletons, offline, as
sample_library.py does, for the partner's knockdown presentation (the player's own knockdown is first-person:
ply_body_generic_knockdown_front/land/recovery on the 20-joint body). The exports come from
Read-D2Assets.ps1 (batch native-knockdown-v1374, read-only); d2_sample_edge maps the player's own Dishonored2.exe without
running it and evaluates each clip on its native skeleton. Writes native-samples/<key>.edgepose and
native-knockdown-sample-manifest-v1374.json (rows in the expanded sample manifest's format) for retarget_native.py."""
from pathlib import Path
import json, hashlib, subprocess
P = Path(__file__).resolve().parent; ROOT = P.parent.parent
read = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
exports = ROOT / 'outputs/d2-coop-lab/research/asset-exports'
manifest = read(exports / 'native-knockdown-v1374/manifest.json')
exe = Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe')
helper = P / 'build/Release/d2_sample_edge.exe'; out = P / 'native-samples'; out.mkdir(exist_ok=True)
skroot = exports / 'doppelganger-animation/game1_patch'
rows = []
for r in sorted(manifest['Resources'], key=lambda r: r['Name']):
    asset = r['Name']; part = 'tall' if '/tall/' in asset else 'female'; role = 'corvo' if part == 'tall' else 'emily'
    sk = f'models/characters/{part}/base/base_{part}.md6skl'
    key = hashlib.sha256((asset + '\n' + sk).encode()).hexdigest()[:16]
    alias = asset.split('/')[-1].replace('.md6anim', '')
    assert hashlib.sha256(Path(r['ExportFile']).read_bytes()).hexdigest().lower() == r['Sha256'].lower()
    skeleton = skroot / f'generated.skeleton.models.characters.{part}.base.base_{part}.edgeskl'
    path = out / (key + '.edgepose')
    row = dict(id=key, asset=asset, skeleton=sk, source_role=role, aliases=[alias], families=['combat'], status='missing_asset',
               runtime_verified=False, asset_sha256=r['Sha256'].lower(), export_file=r['ExportFile'])
    run = subprocess.run([str(helper), str(exe), r['ExportFile'], str(skeleton), str(path)], capture_output=True, text=True, timeout=60)
    if run.returncode == 0:
        row.update(status='native_pose_sampled_offline', sampling=json.loads(run.stdout), sample_file=str(path.relative_to(P)),
                   sample_sha256=hashlib.sha256(path.read_bytes()).hexdigest())
    else:
        row.update(status='sampling_failed', exit_code=run.returncode, error=(run.stderr + run.stdout)[-2500:])
    rows.append(row); print(row['status'], role, alias, key)
(P / 'native-knockdown-sample-manifest-v1374.json').write_text(json.dumps(dict(scope='Native push-to-ragdoll falls and ground recoveries for the partner knockdown presentation (1374)', rows=rows), indent=2), encoding='utf-8')
