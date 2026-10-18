"""Writes completion-audit-v1357-death.json (#11): death variants on candidates 1214cw/1214cx (player deaths) with the
NPC deaths of takedown-variants-v1254k. Usage: make_completion_audit_v1357.py [emily-finisher-take]"""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
EMILY_FINISHER = sys.argv[1] if len(sys.argv) > 1 else 'player-death-probe-v1361o'
M = ("Player deaths rest on the death probe 1361 (Dunwall tower, product controller, both source roles): the source "
     "attacks tutorial guard 2 once, then stands still with health not held until native GameOver, while the observer "
     "camera is re-aimed at the source after every health drop (1361f on). A player's death is either the native Dead state "
     "(player state 15, arkPlayerStateDead, vtable RVA 0x1c82478 from its RTTI) with no scene, or the guard's finisher "
     "(state 38, arkPlayerStateNpcFatality, vtable RVA 0x1c3d878) playing npc_fatality_guards (subscene npc, 4000 ms) "
     "(confirmed static and runtime). Since 1214cv the source sends either as action 25 with one directional collapse "
     "(combat variants 24-27: front, back, left, right, from the nearest living NPC relative to the facing), which the "
     "partner plays from bank 72 (the native generic_sleepdart falls, sampled offline and retargeted per role from the "
     "role's own skeleton, cut to the 1.8-2.2 s collapse) and holds at the end pose; 1214cw/cx time a finisher's collapse "
     "on the native fatality scene clock to the native player body's fall (npc-fatality-timing-v1362.json), so the "
     "partner is down before GameOver. Each probe case passes when, from the first dead or finisher sample to GameOver, "
     "the partner presents a death collapse at full presentation and combat weight in 80% of the samples. Native "
     "GameOver then retires the dead source's stream and the partner leaves the world until the shared checkpoint "
     "reload (shared death and checkpoint recovery are the requested policy). NPC deaths caused by each source role "
     "are presented and shared through the native takedown scenes and corpse authority (takedown-variants-v1254k, "
     "audits 1257 and 1353).")
items = [
 dict(id='death_variants', decision='complete',
      scope=('Player-character death in both roles, both native kinds (plain death and the guards\' finisher), presented as '
             'a full-body directional collapse held to GameOver; NPC deaths caused by either source role presented and '
             'shared with their corpses.'),
      takes=[{'take': 'player-death-probe-v1361j', 'cases': ['host-death']},
             {'take': 'player-death-probe-v1361i', 'cases': ['client-death']},
             {'take': 'player-death-probe-v1361q', 'cases': ['host-fatality']},
             {'take': 'player-death-probe-v1361s', 'cases': ['host-fatality']},
             {'take': EMILY_FINISHER, 'cases': ['client-fatality']},
             {'take': 'takedown-variants-v1254k', 'cases': ['host-drop-lethal', 'client-drop-lethal']}],
      visual=[{'video': 'player-death-probe-v1361j/observer.mp4', 'verdict': 'Corvo reels back from the guard\'s blow against the bookshelf and drops onto his back across the book stacks, staying down.'},
              {'video': 'player-death-probe-v1361i/observer.mp4', 'verdict': 'Emily is struck, staggers back and folds to the floor beside the book stacks while the guard stands over her.'},
              {'video': 'player-death-probe-v1361q/observer.mp4', 'verdict': 'The guard seizes Corvo and stabs him while he stands held; Corvo sinks to his knees, falls forward and lies face down before GameOver.'},
              {'video': f'{EMILY_FINISHER}/observer.mp4', 'verdict': 'EMILY_FINISHER_VERDICT'},
              {'video': 'takedown-variants-v1254k/drop-host-lethal-observer.mp4', 'verdict': 'Corvo falls from above onto the guard, drives him down and rises with the sword raised over the body.'},
              {'video': 'takedown-variants-v1254k/drop-client-lethal-observer.mp4', 'verdict': 'Emily lands behind the guard from above and he crumples forward onto the floor; she straightens over the body.'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1357-death', family='combat', candidate='candidate-npc-combat-v1214cx', method=M, items=items)
(N / 'completion-audit-v1357-death.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
