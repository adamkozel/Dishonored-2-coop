"""Writes completion-audit-v1347-climb.json (#11): chain climbing and the ladder item on candidate 1214cq."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decisions rest on mission-coverage-v1328v (and 1328o/r) on candidate 1214cq in Edge of the World under the product "
     "controller, both source roles: the source grabs chain1 with the ordinary interact (native arkPlayerStateClimb, "
     "alternate-mode state 29), climbs up and down, and in the climbtop case climbs to the top and lets go; the partner is "
     "checked from its own climb presentation (ascending, descending and phases) on the observer. The game has one climbing "
     "state, arkPlayerStateClimb on arkClimbable chains; its only ladders are step-ladder props that are not climbable "
     "(confirmed static: resource index of the player state declarations and of every ladder resource). The mission take ends "
     "with the lab checkpoint restore and the protected-file check.")
def T(take, cases): return {'take': take, 'cases': [f'{r}-{c}' for r in ('host', 'client') for c in cases]}
V = 'mission-coverage-v1328v'
items = [
 dict(id='chain_rope_climb', decision='complete',
      scope='Climbing the Edge of the World chain in both roles: grab, hand-over-hand ascent and descent cycles advanced by the replicated height (about 85-110 climb samples, 9-10 phases per case), climbing to the top and letting go back into the default mode.',
      takes=[T(V, ['climb', 'climbtop'])],
      visual=[{'video': f'{V}/host-climb-observer.mp4', 'verdict': 'Corvo grabs the chain at the street, pulls himself up hand over hand, then climbs back down and steps off.'},
              {'video': f'{V}/client-climbtop-observer.mp4', 'verdict': 'Emily climbs the chain all the way up, hangs at the top and lets go.'}]),
 dict(id='ladder_up_down_entry_exit', decision='complete',
      scope='D2 has no climbable ladders: the only climbing state is arkPlayerStateClimb on arkClimbable (the chains), and the ladder resources are step-ladder props (confirmed static). Entry, ascent, descent and exit of that climb are presented in both roles (chain_rope_climb, the same takes).',
      takes=[T(V, ['climb', 'climbtop'])],
      visual=[{'video': f'{V}/client-climbtop-observer.mp4', 'verdict': 'Emily enters the climb from the street, ascends to the top and exits by letting go.'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1347-climb', family='traversal', candidate='candidate-npc-combat-v1214cq', method=M, items=items)
(N / 'completion-audit-v1347-climb.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
