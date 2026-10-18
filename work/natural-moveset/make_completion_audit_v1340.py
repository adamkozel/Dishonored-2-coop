"""Writes completion-audit-v1340-switching.json (#11): ammo and item switching on candidate 1214cp."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decisions rest on mission-coverage-v1328j on candidate 1214cp in Edge of the World under the product controller, "
     "both source roles: the mission's native quick slots (D-pad Left pistol, Right crossbow, Up sleep darts, Down Blink; "
     "chapter 85) drive the switches, the partner is checked from its own presentation state on the observer (ranged kind "
     "and variants, presentation weight at every sample) and the source's native player states are polled. Lifecycle: the "
     "mission take re-arms the product controller after its map load, resets the source between cases and ends with the "
     "lab checkpoint restore and protected-file check.")
def T(take, cases): return {'take': take, 'cases': [f'{r}-{c}' for r in ('host', 'client') for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
J = 'mission-coverage-v1328j'
items = [
 dict(id='bolt_or_ammo_switch', decision='complete',
      scope='With the crossbow drawn, selecting sleep darts (the native ammo switch, equip change 64) and switching back to bolts in both roles: the partner presents the crossbow through both switches (59 samples, variants 0 and 7-10), fires one dart, and never drops the presentation.',
      takes=[T(J, ['boltswitch'])],
      visual=[V(f'{J}/host-boltswitch-observer.mp4', 'Corvo holds the crossbow aimed through the dart selection, raises it to fire a dart, re-aims after switching back and finally lowers his arm to holster.')]),
 dict(id='item_switch_interruptions', decision='complete',
      scope='Aiming the pistol and switching to the crossbow mid-aim, selecting Blink, holstering and redrawing in both roles: the partner presents the pistol (20-21 samples), then the crossbow (24-26), lowers it for Blink and presents the redrawn crossbow (27-28), with no presentation drop.',
      takes=[T(J, ['itemswitch'])],
      visual=[V(f'{J}/client-itemswitch-observer.mp4', 'Emily raises the pistol, swaps to the wrist crossbow without lowering her arm fully, drops the arm when Blink is selected, then raises the crossbow again and puts it away.')]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1340-switching', family='ranged_and_items', candidate='candidate-npc-combat-v1214cp', method=M, items=items)
(N / 'completion-audit-v1340-switching.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
