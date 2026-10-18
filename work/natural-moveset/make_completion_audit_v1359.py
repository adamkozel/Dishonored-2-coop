"""Writes completion-audit-v1359-scene-handoff.json (#11): the hand-back from native scenes to locomotion, across the scene
families that take over the player's body, both roles."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("A native scene owns the player's body for its length (use mechanisms, traversal scenes, landings, paired takedowns, "
     "cinematics); afterwards the partner must return to its locomotion from the scene's actual end pose without a pop. "
     "The receiver blends from the presented scene pose to locomotion over about 250 ms (audit 1241: traversal weight "
     "0.34/0.07/0.02 while locomotion returns 0.49/0.89/0.97, both roles), keeps the authored body through a native state "
     "no binding classifies for a packet or two at a scene end (1214cg, the protocol-73 hold; 1320c: zero low-weight "
     "samples where 1320b had 2-4 per role) and holds the presented pose for at most 150 ms on a transient ineligible "
     "packet (1214ch). Cinematics retire the presentation for the scene and re-arm it after (1236c). The cited cases are "
     "the passing coverage of each family; the videos are reviewed for the hand-back at the end of each scene.")
items = [
 dict(id='scripted_scene_pose_handoff', decision='complete',
      scope='Hand-back from a native scene to locomotion in both roles for every family that takes over the body: use mechanisms (lever, tap), traversal scenes (high mantle), the damaging landing, paired drop takedowns and cinematics (partner retired and restored).',
      takes=[{'take': 'use-variants-eotw-v1228v', 'cases': ['host-script_lever_windturbine_1_1_1']},
             {'take': 'use-variants-eotw-v1228w', 'cases': ['client-script_lever_windturbine_1_1_1']},
             {'take': 'coverage-v1326d', 'cases': ['host-highmantle', 'client-highmantle']},
             {'take': 'mission-coverage-v1328q', 'cases': ['host-hardfall', 'client-hardfall']},
             {'take': 'takedown-variants-v1254k', 'cases': ['host-drop-lethal', 'client-drop-lethal']},
             {'take': 'cutscene-transition-v1236c', 'cases': ['host-cutscene', 'client-cutscene']}],
      visual=[{'video': 'use-variants-eotw-v1228v/use-host-script_lever_windturbine_1_1_1-observed.mp4', 'verdict': 'Corvo reaches up, pulls the lever down and lets his arm fall back as he settles into his standing idle; no pop at the hand-back.'},
              {'video': 'use-variants-eotw-v1228w/use-client-script_lever_windturbine_1_1_1-observed.mp4', 'verdict': 'Emily steps to the lever, pulls it and returns smoothly to standing beside it.'},
              {'video': 'coverage-v1326c/client-highmantle-observer.mp4', 'verdict': 'Emily hauls herself up the high ledge and straightens into a stand on top without a jump between the climb and the idle.'},
              {'video': 'mission-coverage-v1328q/host-hardfall-observer.mp4', 'verdict': 'Corvo lands from the long fall in a deep crouch, rises and walks straight on along the dock.'},
              {'video': 'takedown-variants-v1254k/drop-host-lethal-observer.mp4', 'verdict': 'Corvo falls from above onto the guard, drives him down and rises with the sword raised over the body.'},
              {'video': 'cutscene-transition-v1236c/after-scene-host.mp4', 'verdict': 'CUTSCENE_VERDICT'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1359-scene-handoff', family='interactions', candidate='candidate-npc-combat-v1214cz', method=M, items=items)
(N / 'completion-audit-v1359-scene-handoff.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
