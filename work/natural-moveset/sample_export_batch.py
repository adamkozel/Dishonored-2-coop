"""Sample a read-only Read-D2Assets export batch of tall/female animations offline, as sample_sleepdart_v1362.py does,
for retarget_native.py. Usage: sample_export_batch.py <batch> <sample-manifest.json> <family> [scope text].
d2_sample_edge maps the player's own Dishonored2.exe without running it and evaluates each clip on its native skeleton;
writes native-samples/<key>.edgepose (an existing sample of the same key is reused: the output is deterministic) and a
manifest in the expanded sample manifest's format (id, asset, skeleton, source_role, aliases, sample_file)."""
from pathlib import Path
import json, hashlib, subprocess, sys
P = Path(__file__).resolve().parent; ROOT = P.parent.parent
batch, out_manifest, family = sys.argv[1], sys.argv[2], sys.argv[3]
scope = sys.argv[4] if len(sys.argv) > 4 else f'Native samples of export batch {batch}'
read = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
exports = ROOT / 'outputs/d2-coop-lab/research/asset-exports'
manifest = read(exports / batch / 'manifest.json')
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
    row = dict(id=key, asset=asset, skeleton=sk, source_role=role, aliases=[alias], families=[family], status='missing_asset',
               runtime_verified=False, asset_sha256=r['Sha256'].lower(), export_file=r['ExportFile'])
    run = None if path.exists() else subprocess.run([str(helper), str(exe), r['ExportFile'], str(skeleton), str(path)], capture_output=True, text=True, timeout=60)
    if run is None or run.returncode == 0:
        row.update(status='native_pose_sampled_offline', sample_file=str(path.relative_to(P)), sample_sha256=hashlib.sha256(path.read_bytes()).hexdigest())
        if run is not None: row['sampling'] = json.loads(run.stdout)
    else:
        row.update(status='sampling_failed', exit_code=run.returncode, error=(run.stderr + run.stdout)[-2500:])
    rows.append(row); print(row['status'], role, alias, key)
(P / out_manifest).write_text(json.dumps(dict(scope=scope, rows=rows), indent=2), encoding='utf-8')
