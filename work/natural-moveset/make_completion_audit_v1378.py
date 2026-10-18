"""Writes completion-audit-v1378-doors.json (#11): door use in both roles: the locked office door (a refused native use,
"key needed") and the adjacent unlocked door opened and closed with two taps, filmed over the partner's right shoulder.
Usage: make_completion_audit_v1378.py [verdict overrides are edited in place once the open/close takes are reviewed]."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("A door use has no native player animation: an ordinary tap runs Interact75 for about a frame beside Running0 (and "
     "Undraw81 around a second tap), with no first-person reach scene (chapter 47, 638-642). The source reads the native "
     "use event and the partner plays the authored reach as use variant 1 (generic) on the use clock (protocol 74); the "
     "door leaf itself is native and its state is shared by the door desired-state adapter (chapters 56-57). The locked "
     "office door (ld_door_350cm_int_rich_wood_01) answers a tap with the native 'Royal Quarters Key needed' message: the use "
     "registers and the partner reaches for the handle, the leaf stays shut. The adjacent unlocked door "
     "(ld_door_350cm_int_rich_wood_01_3) opens on the first tap and closes on the second (Test-UseVariants-v1228r "
     "-SecondUse). The observer's free camera stands over the partner's right shoulder (back 0.9 m, side -0.6 m, eye 2.1 m, "
     "pitch 28). A case passes when the native use registers and the observer presents the reach at full weight.")
items = [
 dict(id='door_open_close_locked', decision='complete',
      scope='Opening and closing an unlocked door and trying a locked one, in both roles: the partner reaches for the handle on the native use clock and settles back; the door leaf and its state are native and shared.',
      takes=[{'take': 'use-variants-dunwall-v1228zf', 'cases': ['host-ld_door_350cm_int_rich_wood_01']},
             {'take': 'use-variants-dunwall-v1228zg', 'cases': ['client-ld_door_350cm_int_rich_wood_01']},
             {'take': 'use-variants-dunwall-v1228zh', 'cases': ['host-ld_door_350cm_int_rich_wood_01_3']},
             {'take': 'use-variants-dunwall-v1228zi', 'cases': ['client-ld_door_350cm_int_rich_wood_01_3']}],
      visual=[{'video': 'use-variants-dunwall-v1228zf/use-host-ld_door_350cm_int_rich_wood_01-observed.mp4', 'verdict': 'Locked door: Corvo reaches his right hand forward to the handle of the office door, which stays shut, and lowers it again.'},
              {'video': 'use-variants-dunwall-v1228zg/use-client-ld_door_350cm_int_rich_wood_01-observed.mp4', 'verdict': 'Locked door: Emily steps to the office door, reaches her right hand to the handle and returns to stand when it does not open.'},
              {'video': 'use-variants-dunwall-v1228zh/use-host-ld_door_350cm_int_rich_wood_01_3-observed.mp4', 'verdict': 'CORVO_OPEN_CLOSE'},
              {'video': 'use-variants-dunwall-v1228zi/use-client-ld_door_350cm_int_rich_wood_01_3-observed.mp4', 'verdict': 'EMILY_OPEN_CLOSE'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1378-doors', family='interactions', candidate='candidate-npc-combat-v1214df', method=M, items=items)
(N / 'completion-audit-v1378-doors.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
