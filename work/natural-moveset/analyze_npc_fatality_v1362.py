"""1362 (#11 death_variants): when does the player's body fall in a native NPC fatality? Probe 1361c found Emily killed
by a guard's finisher: native player state 38 (arkPlayerStateNpcFatality) with the player scene npc_fatality_guards
(subscene npc, 4000 ms), which binds PlayerBody to ply_body_generic_npc_guard_fatality (scene decl, read-only export
native-npcfatality-scene-v1362). This samples that clip (and ply_body_generic_fatality_knockdown) offline on the native
first-person player body skeleton with d2_sample_edge and reports the pelvis height over time, to time the partner's
collapse; the other NPC types' fatality bodies are sampled the same way. Writes npc-fatality-timing-v1362.json."""
from pathlib import Path
import json, subprocess, sys
P = Path(__file__).resolve().parent; ROOT = P.parent.parent
sys.path.insert(0, str(P))
from edge_data import poses, skeleton
from rig_math import NativeRig, qts
exports = ROOT / 'outputs/d2-coop-lab/research/asset-exports'
skl = exports / 'natural-fp-window-bodies/game1_patch/generated.skeleton.models.characters.player.emily.player_body_emily.edgeskl'
exe = 'C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe'
helper = P / 'build/Release/d2_sample_edge.exe'
sk = skeleton(skl); rig = NativeRig(sk['joints']); names = [j['nativeName'] for j in sk['joints']]
report = dict(skeleton=str(skl.relative_to(ROOT)), skeletonSha256=sk['sha256'], clips=[])
# Batches native-npcfatality-body-v1362 (guard, knockdown) and -body2- (clockwork a/b, elite, howler, overseer, witch;
# the game2/game3 catalogs hold identical-name copies, the first found is used).
anims = {}
for batch in ('native-npcfatality-body-v1362', 'native-npcfatality-body2-v1362'):
    for a in sorted((exports / batch).glob('*/*.edgeanim')):
        anims.setdefault(a.name.split('.body.')[-1].replace('.edgeanim', ''), a)
for name, anim in sorted(anims.items()):
    out = P / 'native-samples' / f'{name}-player-body-emily.edgepose'
    run = None if out.exists() else subprocess.run([str(helper), exe, str(anim), str(skl), str(out)], capture_output=True, text=True, timeout=60)  # deterministic: reuse
    if run and run.returncode: report['clips'].append(dict(name=name, error=(run.stderr + run.stdout)[-800:])); continue
    sp = poses(out); fps = sp['fps']
    pelvis = next(i for i, n in enumerate(names) if n == 'center_root_jnt')
    heads = [i for i, n in enumerate(names) if n == 'center_head_jnt']
    rows = []
    for f, frame in enumerate(sp['frames']):
        g = rig.fk([qts(v) for v in frame])
        z = lambda j: float(g[j].to_translation().z) if hasattr(g[j], 'to_translation') else float(g[j][2][3])
        rows.append(dict(t=round(f / fps, 3), pelvis=round(z(pelvis), 3), head=round(z(heads[0]), 3) if heads else None))
    p0 = rows[0]['pelvis']; low = min(r['pelvis'] for r in rows)
    drop = next((r['t'] for r in rows if r['pelvis'] < p0 - .5 * (p0 - low)), None)
    report['clips'].append(dict(name=name, frames=len(rows), fps=fps, duration=round((len(rows) - 1) / fps, 3), pelvisJoint=names[pelvis],
                                pelvisStart=p0, pelvisMin=low, halfDropAt=drop, samples=rows[::max(1, len(rows) // 40)]))
(P / 'npc-fatality-timing-v1362.json').write_text(json.dumps(report, indent=1), encoding='utf-8')
for c in report['clips']: print({k: v for k, v in c.items() if k != 'samples'})
