"""Writes completion-audit-v1470-object-carry.json (#11 carry_object_place_throw): a native movable picked up, held, thrown
or put down, presented on the partner with the object in the partner's hand and its throw or placement followed, both roles
(object-carry-v1478a on 1214ee). Usage: make_completion_audit_v1470.py "<Emily throw verdict>" "<Corvo throw verdict>"
"<Emily placement verdict>" "<Corvo placement verdict>"."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
et, ct, ep, cp = sys.argv[1:5]
M = ("Pick-up: the native LootObject 47 and HoldObject 49 start the partner's use reach, which holds at its contact pose while "
     "49 lasts (use variant 6). The object: HoldObject 49 keeps the held entity's handle at +0x48 (1433b/c); the source sends it "
     "with its name key and world transform (protocol 78-79), and the observer pins its own copy of the same map entity by the "
     "partner's presented right wrist, 7 cm along the forearm and 10 cm down so the hand holds its body. Release: a placement "
     "replaces 49 with state 81 in the same read (1457a) and plays the reach's finish (variant 7); otherwise an object that "
     "travels 25 cm within 160 ms is a throw, played as the native grenade overhand throw from its wind-up's end (variant 8). "
     "The observer's copy follows the source's flight and rest, is watched until 10 s after the release, and is broken natively "
     "when the source's breaks (its own health component, the plain arkCpntHealth). Harness 1451/1454/1457/1460: the bottle esc_env_bottle_03_3 picked up with X, held 2.4 s, then "
     "thrown with RT or put down with X; checks: the source's held phases, the observer's copy within 1.2 m of the partner and "
     "0.4-2.0 m up while held (twice), the release gesture (8 for the throw, 7 only for the placement) and both worlds agreeing "
     "at the end (both broken, or resting within 0.75 m).")
items = [dict(id='carry_object_place_throw', decision='complete',
    scope=("Both roles: a held native movable presented in the partner's hand, its throw (arm throw) and its placement (reach "
           "finish), with the object's flight, rest or break the same in both worlds."),
    takes=[{'take': 'object-carry-v1478a', 'cases': ['client-heldbottle', 'client-placebottle', 'host-heldbottle', 'host-placebottle']}],
    visual=[{'video': 'object-carry-v1478a/client-heldbottle-observer.mp4', 'verdict': et},
            {'video': 'object-carry-v1478a/host-heldbottle-observer.mp4', 'verdict': ct},
            {'video': 'object-carry-v1478a/client-placebottle-observer.mp4', 'verdict': ep},
            {'video': 'object-carry-v1478a/host-placebottle-observer.mp4', 'verdict': cp}])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1470-object-carry', family='interactions', candidate='candidate-npc-combat-v1214ee', method=M, items=items)
(N / 'completion-audit-v1470-object-carry.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
