"""Writes completion-audit-v1377-hit-reactions.json (#11): directional hit reactions (the native stun, state 14) presented
from all four directions in both roles. Usage: make_completion_audit_v1377.py EMILY_FRONT_TAKE EMILY_FRONT_CASE
EMILY_FRONT_VIDEO EMILY_LEFT_TAKE EMILY_LEFT_CASE EMILY_LEFT_VIDEO (the Emily front and left evidence, filled in once
filmed)."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
ef_take, ef_case, ef_video, el_take, el_case, el_video = sys.argv[1:7]
M = ("A heavy hit on the player (PLAYER_HITREACT_BIG: a guard's sword blow or a grenade blast, damage decl census 1372, "
     "confirmed static) puts the player in the native Stun state 14 playing hit_react_stun_right/front/left/back (800 ms, "
     "Scene_1, body only; the native game picks the side from the hit direction). The source reads the scene on its own "
     "clock and sends stun variants 1-4; the partner plays the matching native stagger at full stun weight, with its "
     "locomotion weight at 0 by design while the stun owns the body, then recovers. Evidence comes from guard fights "
     "(Record-MissionCoverage, the source blocking, standing open and attacking, optionally turned away from the guard) "
     "and from the source's own grenade (Record-Coverage-v1363g: thrown at the feet, the view turned 90 degrees left "
     "before the blast). A case passes when the partner presents a stun at full stun weight while its presentation stays "
     "live. 1214de admits a stun taken with the left hand's EquipChange (state 64) beside it, which guard-fight stuns "
     "often carry. Knockdown (state 16) and stumble (17) belong to stagger_knockdown_recovery.")
items = [
 dict(id='hit_reactions_directions', decision='complete',
      scope='The partner staggers from a heavy hit from the front, right, back and left, in both roles, on the native stun clock, and recovers.',
      takes=[{'take': 'mission-coverage-v1328zzo', 'cases': ['host-guardfight']},
             {'take': 'coverage-v1363i', 'cases': ['host-stunright']},
             {'take': 'coverage-v1363g', 'cases': ['host-stunright']},
             {'take': 'mission-coverage-v1371c', 'cases': ['host-guardfight']},
             {'take': ef_take, 'cases': [ef_case]},
             {'take': 'coverage-v1363j', 'cases': ['client-stunright']},
             {'take': 'coverage-v1363h', 'cases': ['client-stunright']},
             {'take': el_take, 'cases': [el_case]}],
      visual=[{'video': 'mission-coverage-v1328zzo/host-guardfight-observer.mp4', 'verdict': 'Front (variant 2): the guard\'s blow rocks Corvo back onto one leg with the knee raised; he recovers upright on the walkway.'},
              {'video': 'coverage-v1363i/host-stunright-observer.mp4', 'verdict': 'Right (variant 1): the grenade blast throws Corvo off balance to his left; he recovers standing.'},
              {'video': 'coverage-v1363g/host-stunright-observer.mp4', 'verdict': 'Back (variant 4): the blast behind him lurches Corvo forward off his feet for a moment before he catches himself and stands.'},
              {'video': 'mission-coverage-v1371c/host-guardfight-observer.mp4', 'verdict': 'Left (variant 3): the guard\'s blow knocks Corvo sideways with legs splayed and arms thrown out; he catches himself and straightens.'},
              {'video': ef_video, 'verdict': 'EMILY_FRONT_VERDICT'},
              {'video': 'coverage-v1363j/client-stunright-observer.mp4', 'verdict': 'Right (variant 1): the blast shoves Emily off balance to her left before she recovers.'},
              {'video': 'coverage-v1363h/client-stunright-observer.mp4', 'verdict': 'Back (variant 4): the blast behind Emily jolts her forward off balance; she recovers standing.'},
              {'video': el_video, 'verdict': 'EMILY_LEFT_VERDICT'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1377-hit-reactions', family='combat', candidate='candidate-npc-combat-v1214de', method=M, items=items)
(N / 'completion-audit-v1377-hit-reactions.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
