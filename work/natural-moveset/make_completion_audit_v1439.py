"""Writes completion-audit-v1439-power-interruptions.json (#11): failed casts without mana, aims interrupted by damage or
cancelled, and casts with no valid target presented on the partner in both roles, with no power pose or effect left
behind. Usage: make_completion_audit_v1439.py "<1438a Corvo verdict>"."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
corvo_blast = sys.argv[1]
M = ("No mana: a press with too little mana plays the native power_outofmana scene (Scene_1, 333 ms, only the left arm bound) "
     "under any selected power (Corvo: Bend Time v44/v45, Windblast and Blink 1425b; Emily: Mesmerize 1390a); the source "
     "sends it as power variant 15 (1214dk Emily, 1214dy Corvo under any power) and the partner plays the native out-of-mana "
     "hand response with its hand effect. Interruption: a lab explosion (coop_lab_hit blast, a knockdown) during an aim "
     "stuns the source; the stun takes over the partner and the power pose fades out with it; a light sword hit does not "
     "interrupt an aim natively (1429a). Cancel: X during an aim cancels it natively and the partner lowers the arm and "
     "returns to locomotion (1350a, 1353a). No target: Possession and Domino cast at nothing play their entry and aim, "
     "then end without an effect, and so does the partner. Takes record the source states and scenes, the partner's "
     "power and stun variants and weights, and the observer film.")
items = [dict(id='power_interruptions_and_no_mana', decision='complete',
    scope=("Both roles: the native failed cast without mana under the selected power, an aim cut off by a knockdown hit, an "
           "aim cancelled with X, and a cast with no valid target, each presented on the partner and ending without a "
           "leftover power pose or effect."),
    takes=[{'take': 'coverage-v1417a', 'cases': ['client-nomana3']}, {'take': 'coverage-v1425c', 'cases': ['host-nomana4']},
           {'take': 'coverage-v1434a', 'cases': ['client-labpowerblast']}, {'take': 'coverage-v1438a', 'cases': ['host-labpowerblast']},
           {'take': 'coverage-v1350a', 'cases': ['host-blinkcancel']}, {'take': 'coverage-v1353a', 'cases': ['client-farreachcancel']},
           {'take': 'coverage-v1332d', 'cases': ['host-possession', 'client-domino']}],
    visual=[{'video': 'coverage-v1417a/client-nomana3-observer.mp4', 'verdict': 'Emily twists slightly and flicks her left hand at the hip, the native out-of-mana response, then settles.'},
            {'video': 'coverage-v1425c/host-nomana4-observer.mp4', 'verdict': 'Corvo flicks his left hand out at the hip with the native out-of-mana wisps curling around it, then the hand drops back.'},
            {'video': 'coverage-v1434a/client-labpowerblast-observer.mp4', 'verdict': 'Emily holds her left hand out aiming Far Reach; the blast throws her back toward the pillar and she recovers with no Far Reach effect left.'},
            {'video': 'coverage-v1438a/host-labpowerblast-observer.mp4', 'verdict': corvo_blast},
            {'video': 'coverage-v1350a/host-blinkcancel-observer.mp4', 'verdict': 'Corvo throws his left arm out to aim, drops it again at the cancel and stays where he stands; a few seconds later he aims again and vanishes in a blue burst.'},
            {'video': 'coverage-v1353a/client-farreachcancel-observer.mp4', 'verdict': 'Emily reaches out with her left hand and holds it, lowers it again without moving when the aim is cancelled, then reaches once more and is pulled away.'},
            {'video': 'coverage-v1332d/host-possession-observer.mp4', 'verdict': 'Corvo raises his left arm up and out to aim Possession at nothing, holds it, then lowers it and stands; nothing is possessed.'},
            {'video': 'coverage-v1332d/client-domino-observer.mp4', 'verdict': 'Emily makes the Domino cast with no target: a short gesture of her left hand at her side and a shift of stance, then she settles (dark, small at this distance).'}])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1439-power-interruptions', family='powers', candidate='candidate-npc-combat-v1214dz', method=M, items=items)
(N / 'completion-audit-v1439-power-interruptions.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
