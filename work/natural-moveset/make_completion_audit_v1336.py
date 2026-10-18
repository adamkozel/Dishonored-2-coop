"""Writes completion-audit-v1336-double-jump.json (#11): the double jump on candidate 1214cm."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decision rests on coverage-v1333b on candidate 1214cm under the product controller (both source roles, the partner "
     "checked from its own presentation state on the observer, the checkpoint reloaded after every case; Agility is granted "
     "by name with the lab-only coop_grant_named, 1214cm, since the isolated save has no upgrade), on the reviewed observer "
     "videos listed, and on the earlier visual sweep 1260 acceptance recorded in the item notes. Lifecycle: every coverage case "
     "re-arms the product controller after a checkpoint reload and waits for the partner presentation to come back; the "
     "2-hour soak mission-soak-v1316a covers 19 shared restarts and session renewals; the clean-PC take product-two-account-v1315b "
     "covers the two-account Steam session, leave and rejoin.")
def T(take, cases): return {'take': take, 'cases': [f'{r}-{c}' for r in ('host', 'client') for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
items = [
 dict(id='double_jump', decision='complete',
      scope='Native Agility double jumps standing armed, running on the open course and holstered in both roles: each second impulse is presented in the air (the partner second-jump serial advances once per jump, 3 of 3 in each role) inside an air episode that lands.',
      takes=[T('coverage-v1333b', ['doublejump'])],
      visual=[V('coverage-v1333b/host-doublejump-observer.mp4', 'Corvo springs from a crouched take-off, tucks, kicks into a second tuck near the top and comes down arms spread into a compressed crouched landing, then recovers.'),
              V('coverage-v1333b/client-doublejump-observer.mp4', 'Emily takes off, tucks with her coat flaring, tucks again for the second impulse and lands in a crouch before standing.')]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1336-double-jump', family='traversal', candidate='candidate-npc-combat-v1214cm', method=M, items=items)
(N / 'completion-audit-v1336-double-jump.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
