"""Writes completion-audit-v1414-hit-reactions.json (#11): directional hit reactions (the native stun, state 14) presented
from all four directions in both roles on 1214dp/dq, where a presented stun keeps the partner's body clock (before 1214dp
the clock refused the stun state and the partner fell back to its native pose after the first packet: 1397k/l, so the
earlier v1377 draft and its takes are superseded). Takes: coverage-v1403c/e/f, 1407a/b (Emily), 1403i, 1407c/d/e (Corvo)."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("A heavy hit on the player (PLAYER_HITREACT_BIG: a guard's sword blow or a grenade blast, damage decl census 1372, "
     "confirmed static) puts the player in the native Stun state 14 playing hit_react_stun_right/front/left/back (800 ms, "
     "Scene_1, body only; the game picks the side from the hit direction). The source reads the scene on its own clock "
     "and sends stun variants 1-4; the partner plays the matching authored stagger (bank clips 260-263) at full stun "
     "weight with its body clock kept (1214dp), then recovers. The lab-only hit (coop_lab_hit blast <side>, 1214dp) "
     "applies the grenade's own explosion damage (explosion_grenade.damage) 1.2 m out on a chosen side through the "
     "original native damage function, so each direction is provoked natively and on demand; the blast also throws the "
     "player about a metre, which the partner follows on its network-owned root. Harness 1403 requires the source to "
     "enter Stun 14, the partner to present a stun at full stun weight, and the partner's clock weight to stay at or "
     "above 0.9 throughout; 1407 frames front and back blasts along the push. Every case below passed with the clock "
     "weight at 1.0.")
emily = [('coverage-v1403c', 'client-labblastleft', 'Left (variant 3): the blast from her left flings Emily away from the camera; she staggers with her arms out and recovers upright.'),
         ('coverage-v1407a', 'client-labblastfront', 'Front (variant 2): facing the camera, Emily reels back from the blast in front of her with her arms flung out, then steadies and stands.'),
         ('coverage-v1403e', 'client-labblastright', 'Right (variant 1): thrown toward the camera, she staggers, drops low with one arm out and recovers.'),
         ('coverage-v1407b', 'client-labblastback', 'Back (variant 4): the blast behind her throws Emily toward the camera; she stumbles forward and recovers standing.')]
corvo = [('coverage-v1407e', 'host-labblastleft', 'CORVO_LEFT_VERDICT'),
         ('coverage-v1407c', 'host-labblastfront', 'Front (variant 2): facing the camera, Corvo is thrown back, staggers with his arms out and recovers his stance.'),
         ('coverage-v1403i', 'host-labblastright', 'Right (variant 1): thrown toward the camera, Corvo staggers low with his arms out and straightens up.'),
         ('coverage-v1407d', 'host-labblastback', 'Back (variant 4): the blast behind him pitches Corvo forward toward the camera; he stumbles bent over and recovers.')]
import sys
left = sys.argv[1] if len(sys.argv) > 1 else None
rows = emily + corvo
if left: rows = [(t, c, left if v == 'CORVO_LEFT_VERDICT' else v) for t, c, v in rows]
assert all(v != 'CORVO_LEFT_VERDICT' for _, _, v in rows), 'Corvo left verdict missing'
items = [dict(id='hit_reactions_directions', decision='complete',
              scope='The partner staggers from a heavy hit from the left, front, right and back, in both roles, on the native stun clock with its body kept, and recovers.',
              takes=[{'take': t, 'cases': [c]} for t, c, _ in rows],
              visual=[{'video': f'{t}/{c}-observer.mp4', 'verdict': v} for t, c, v in rows])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1414-hit-reactions', family='combat', candidate='candidate-npc-combat-v1214dq', method=M, items=items)
(N / 'completion-audit-v1414-hit-reactions.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items), len(rows))
