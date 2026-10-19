"""Writes completion-audit-v1443-counterattack.json (#11): the counter-kill after a timed parry win presented on the
partner in both roles in a mission fight (mission-coverage-v1418a Emily, v1418l Corvo)."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("A guard's swing met by a timed block plays sword_parry_win on the player and sword_parry_lose_* on the guard; an attack "
     "pressed at once runs a counter-kill finisher (fatality_*_fast_a, player state 13) that kills the staggered guard. "
     "The source streams the parry (combat variants 10/12/19-21) and the finisher through the takedown channel (census "
     "finisher scenes), and the guard's death and corpse come from the host's NPC authority (host-sourced: the client's "
     "replica follows the host's native finisher; client-sourced: the host follows the client's paired poses). Harness "
     "1418 fights a checkpoint guard on Edge of the World with block presses of 400 ms, answers the partner-presented "
     "parry win at once with three attack presses, and films from a raised camera following the fight; the take passes "
     "when the guard is dead in both worlds, and the counter-kill is a fatality starting within 3 s of a counter press.")
items = [dict(id='counterattack', decision='complete',
    scope=("Both roles in a mission fight: the timed parry win, the immediate counter-kill finisher on the staggered guard, and "
           "the guard dead in both worlds, presented on the partner."),
    takes=[{'take': 'mission-coverage-v1418a', 'cases': ['client-guardfight']}, {'take': 'mission-coverage-v1418l', 'cases': ['host-guardfight']}],
    visual=[{'video': 'mission-coverage-v1418a/client-guardfight-observer.mp4', 'verdict': 'From above the street: the guard lunges, Emily catches him and the two lock together through the finisher, and he drops as other guards close in.'},
            {'video': 'mission-coverage-v1418l/host-guardfight-observer.mp4', 'verdict': 'From above the street: Corvo parries the guard\'s swing, steps in and drives his sword forward into him with the arm fully extended, and the guard falls back onto the street.'}])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1443-counterattack', family='combat', candidate='candidate-npc-combat-v1214dz', method=M, items=items)
(N / 'completion-audit-v1443-counterattack.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
