"""Derive a reversible remote-only native muzzle with a shorter streak layer."""
import copy
import hashlib
import json
import re
from pathlib import Path
from inspect_midnight_events import parse

P = Path(__file__).resolve().parent
LAB = P.parents[1] / 'outputs/d2-coop-lab'
particle = LAB / 'research/asset-exports/ranged-muzzle-particle-comparison-v1/game1_patch/generated.decls.particledt.models.effects.weapons.gun.muzzle_1p..particledt.decl'
fx_dir = LAB / 'research/asset-exports/ranged-muzzle-fx-reference-v1/game1_patch'
dest = LAB / 'assets/ranged-muzzle-presentation-v1-source'
dest.mkdir(exist_ok=True)
text = particle.read_text(encoding='utf-8-sig')
before = parse(text)
start = text.index('\n\t\t\titem[5] = {')
end = text.index('\n\t\t\titem[6] = {', start)
stage = text[start:end]
assert 'm_name = "sparkyStreaks";' in stage
changes = []
for old, new in [('y = 3.5;', 'y = 1.05;'), ('y = 1.5;', 'y = 0.45;')]:
    assert stage.count(old) == 1
    stage = stage.replace(old, new)
    changes.append(dict(before=old, after=new))
after_text = text[:start] + stage + text[end:]
after = parse(after_text)
expected = copy.deepcopy(before)
size = expected['edit']['m_stages']['item[5]']['m_sprites']['item[0]']['m_spawn']['m_initialSize']
assert size['m_max']['y'] == 3.5 and size['m_min']['y'] == 1.5
size['m_max']['y'] = 1.05
size['m_min']['y'] = .45
assert after == expected, 'Only the two specified native streak lengths may change'
(dest / 'muzzle_remote.particledt').write_text(after_text, encoding='utf-8')
sources = [dict(path=str(particle.relative_to(LAB)), sha256=hashlib.sha256(particle.read_bytes()).hexdigest())]
for role in ['corvo', 'emily']:
    src = fx_dir / f'generated.decls.fx.models.effects.weapons.gun.muzzle_1p_{role}..fx.decl'
    original = src.read_text(encoding='utf-8-sig')
    needle = 'models/effects/weapons/gun/muzzle_1p.particledt'
    assert original.count(needle) == 1
    modified = original.replace(needle, 'models/effects/d2coop/muzzle_remote.particledt')
    obj = parse(original)
    obj['edit']['m_groups']['item[0]']['m_particleSystems']['item[0]']['m_decls']['item[0]'] = 'models/effects/d2coop/muzzle_remote.particledt'
    assert parse(modified) == obj
    (dest / f'muzzle_remote_{role}.fx').write_text(modified, encoding='utf-8')
    sources.append(dict(path=str(src.relative_to(LAB)), sha256=hashlib.sha256(src.read_bytes()).hexdigest()))
manifest = dict(complete=False, visual_accepted=False, sources=sources, changes=changes,
    factor=.3, scope='Only remote cosmetic muzzle sparkyStreaks sprite length changes. Native materials, timings, light, sound, other particle layers and source first-person assets remain unchanged.',
    files=[dict(name=p.name, sha256=hashlib.sha256(p.read_bytes()).hexdigest()) for p in sorted(dest.iterdir()) if p.is_file()])
(P / 'remote-muzzle-asset-manifest-v1.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
print('Built three isolated native-derived resources; parsed difference verifies exactly two streak lengths and two declaration references.')
