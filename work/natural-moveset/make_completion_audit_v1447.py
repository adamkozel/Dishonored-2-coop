"""Writes completion-audit-v1447-struggle-escape.json (#11): the rear choke's struggle and the victim's escape presented
in both roles on a mission NPC, the victim's side included (mission-coverage-v1448a Emily, v1448b Corvo; 1444a/b first produced it).
Usage: make_completion_audit_v1447.py "<1448a Emily verdict>" "<1448b Corvo verdict>"."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
emily, corvo = sys.argv[1], sys.argv[2]
M = ("A rear choke runs choke_back (600 ms) and choke_loop (1400 ms) with the victim struggling; when the choker is hit "
     "during the loop the victim breaks free natively (choke_combat_lose, 1500 ms, census variant 4) and stays alive and "
     "alert. Harness 1444/1448 places the source behind the bound tall overseer on Edge of the World, presses the choke and "
     "lands the lab-only sword hit (coop_lab_hit sword back) 1.2 s later. The source streams the choke variants (1-4); the "
     "partner plays the choke and the breakout (candidate 592's combat-lose clip). The victim's side: for a client-sourced "
     "choke the host's NPC follows the client's paired poses (PairedHold, then released at the escape); for a host-sourced "
     "choke the client's replica follows the host's native NPC. The case records the source states, the choke scenes, the "
     "NPC authority status during and after the choke and the outcome (alive in both worlds).")
items = [dict(id='struggle_and_escape', decision='complete',
    scope=("Both roles: the victim's struggle in the rear choke and its escape (the choker thrown off, the victim free and "
           "alive), presented on the partner with the victim's side following in the observer's world."),
    takes=[{'take': 'mission-coverage-v1448a', 'cases': ['client-takedown']}, {'take': 'mission-coverage-v1448b', 'cases': ['host-takedown']}],
    visual=[{'video': 'mission-coverage-v1448a/client-takedown-observer.mp4', 'verdict': emily},
            {'video': 'mission-coverage-v1448b/host-takedown-observer.mp4', 'verdict': corvo}])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1447-struggle-escape', family='paired_actions', candidate='candidate-npc-combat-v1214dz', method=M, items=items)
(N / 'completion-audit-v1447-struggle-escape.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
