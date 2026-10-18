"""Writes completion-audit-v1348-parry.json (#11): parries with a live opponent on candidates 1214cp/cq."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decisions rest on the Edge of the World guard fights under the product controller (health held, lab only): the "
     "source faces a hostile checkpoint guard, blocks until the guard's attacks are deflected, stays unguarded, then attacks "
     "to the kill; the partner is checked from its own combat presentation on the observer (block impact variants 19/20, the "
     "timed parry variant 21, stuns), and both worlds must agree that the guard died. mission-coverage-v1328zh (Corvo, 1214cq: "
     "4 block impacts, 3 timed parries) and mission-coverage-v1328h (Emily, 1214cp: 5 block impacts, 6 timed parries). The "
     "native defense has two impact scenes and one winning parry (sword_parry_hit_1/hit_2, sword_parry_win), presented from "
     "the defender's exact native scene; the camera follows the fight from 3.5 m beside the source. The guard's own reaction "
     "is the NPC stream (stage 4).")
items = [
 dict(id='parry_directions', decision='complete',
      scope="Both roles meet the guard's swings with the native block and the timed parry: the partner presents the block impacts (variants 19, 20) and the winning parry (21) from the defender's native scenes, which are the game's full set of player defense outcomes.",
      takes=[{'take': 'mission-coverage-v1328zh', 'cases': ['host-guardfight']}, {'take': 'mission-coverage-v1328h', 'cases': ['client-guardfight']}],
      visual=[{'video': 'mission-coverage-v1328zh/host-guardfight-observer.mp4', 'verdict': 'Backed against the wall, Corvo snaps his sword up overhead to meet the guard\'s swing; the guard is thrown off balance and stumbles back.'},
              {'video': 'mission-coverage-v1328h/client-guardfight-observer.mp4', 'verdict': 'Emily raises her blade across her body each time the guard swings and holds her ground.'}]),
 dict(id='parry_recovery', decision='complete',
      scope='After each block impact and timed parry the partner returns from the defense scene to the armed stance and keeps fighting (attacks presented afterwards, guard killed) in both roles.',
      takes=[{'take': 'mission-coverage-v1328zh', 'cases': ['host-guardfight']}, {'take': 'mission-coverage-v1328h', 'cases': ['client-guardfight']}],
      visual=[{'video': 'mission-coverage-v1328zh/host-guardfight-observer.mp4', 'verdict': 'After the parry Corvo lowers the blade back to his guard stance and steps in to attack again.'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1348-parry', family='combat', candidate='candidate-npc-combat-v1214cq', method=M, items=items)
(N / 'completion-audit-v1348-parry.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
