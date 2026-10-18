"""Writes completion-audit-v1349-slopes.json (#11): the sloped street in Edge of the World on candidate 1214cq."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decision rests on two kinds of terrain under both source roles, the partner checked from its own presentation state "
     "on the observer and the source's native player state polled: the Dunwall safe-room staircase (coverage-v1318e, stepped, "
     "earlier audit 1323) and the sloped street in Edge of the World (mission-coverage-v1328zk, candidate 1214cq, product "
     "controller, health held on both peers): the source is placed at the foot of the street (-14.1, 75.3, 11.5), walks up to "
     "(3.2, 57.0, 15.0) at a measured grade of 0.144 and back down, and the partner must present the terrain gait with weight "
     "above 0.9 and the matching positive and negative grade. The free camera holds 3 m behind the start looking up the street. "
     "Each take ends with the lab checkpoint restore and the protected-file check.")
items = [
 dict(id='slopes_and_stairs', decision='complete',
      scope='Stairs (Dunwall safe room) and a non-stepped sloped street (Edge of the World, grade 0.144) up and down in both roles: the partner presents the native terrain gait with positive and negative grade (sloped street: 104 up / 96 down samples with Corvo as source, 112 up / 75 down with Emily).',
      takes=[{'take': 'mission-coverage-v1328zk', 'cases': ['host-slope', 'client-slope']},
             {'take': 'coverage-v1318e', 'cases': ['host-stairs', 'client-stairs']}],
      visual=[{'video': 'mission-coverage-v1328zk/host-slope-observer.mp4', 'verdict': 'Corvo walks away up the sloped cobbled street beside the tram rails, then turns and walks back down toward the camera.'},
              {'video': 'mission-coverage-v1328zk/client-slope-observer.mp4', 'verdict': 'Emily walks away up the same street, then comes back down toward the camera, stepping on the incline throughout.'},
              {'video': 'coverage-v1318a/host-stairs-observer.mp4', 'verdict': 'Corvo steps down the staircase with alternating feet, sword in hand.'},
              {'video': 'coverage-v1318c/client-stairs-observer.mp4', 'verdict': 'Emily climbs the staircase stepping, upright.'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1349-slopes', family='locomotion', candidate='candidate-npc-combat-v1214cq', method=M, items=items)
(N / 'completion-audit-v1349-slopes.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
