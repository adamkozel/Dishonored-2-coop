"""Writes completion-audit-v1441-stagger-knockdown.json (#11): the parried or blocked swing's recoil stagger and the
knockdown with its recovery presented on the partner in both roles. Usage:
make_completion_audit_v1441.py "<1440a Emily verdict>" "<1440a Corvo verdict>"."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
emily, corvo = sys.argv[1], sys.argv[2]
M = ("Stagger: a swing that is parried by a guard or strikes a wall enters the native Recoil 40 over Running 0 (vtable RVA "
     "0x1c83c98) with a sword_parry_* scene (666 or 733 ms); the source sends it as combat variant 28 (left) or 29 (right) "
     "with the scene clock, and the partner plays the native NPC parry-lose stagger (bank 73 clips 378-379). Until 1214dz the "
     "motion classifier did not accept Recoil 40 as an overlay, so the observer's playback refused the stagger as motion "
     "invalid (1418k, confirmed runtime). Knockdown: the lab shove (coop_lab_hit shove) knocks the source down natively "
     "(KnockedDown 16) and the partner plays the knockdown, the lying phase and the get-up (stun variant 5, bank 74 clip 380). "
     "Takes record the source states, the partner's combat and stun variants with their weights, and the observer film; "
     "1435b/1440a swing into the lab room wall four times per role, 1403a/b shove the source from the front.")
items = [dict(id='stagger_knockdown_recovery', decision='complete',
    scope=("Both roles: the recoil stagger after a parried or wall-struck swing (left and right) and the knockdown with the "
           "lying phase and the recovery to standing, presented on the partner from the native states."),
    takes=[{'take': 'coverage-v1403a', 'cases': ['client-labknock']}, {'take': 'coverage-v1403b', 'cases': ['host-labknock']},
           {'take': 'coverage-v1435b', 'cases': ['client-wallswing', 'host-wallswing']},
           {'take': 'coverage-v1440a', 'cases': ['client-wallswing', 'host-wallswing']}],
    visual=[{'video': 'coverage-v1403a/client-labknock-observer.mp4', 'verdict': 'Emily recoils, drops to her knees, lies face down, pushes up to a crouch and rises to her guard.'},
            {'video': 'coverage-v1403b/host-labknock-observer.mp4', 'verdict': 'Corvo drops to his knees, lies face down, pushes up to a crouch and rises to standing.'},
            {'video': 'coverage-v1440a/client-wallswing-observer.mp4', 'verdict': emily},
            {'video': 'coverage-v1440a/host-wallswing-observer.mp4', 'verdict': corvo}])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1441-stagger-knockdown', family='combat', candidate='candidate-npc-combat-v1214dz', method=M, items=items)
(N / 'completion-audit-v1441-stagger-knockdown.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
