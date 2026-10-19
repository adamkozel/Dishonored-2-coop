"""Writes completion-audit-v1431-carry-body.json (#11): the product body carry presented on the partner in both roles:
lift, idle and walk with the body on the shoulder, drop and throw (carry-body-v1277y/z, v1423b/c, v1428a/b on
1214dt-1214dy). Usage: make_completion_audit_v1431.py "<1428b verdict>"."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
corvo_lift = sys.argv[1]
M = ("After a choke the source presses Use on the unconscious body: the native carry (state 45, a pickup scene, the body "
     "attached to the player's shoulder) runs in the source's world. The source streams the carry action with the carried "
     "body's identity (spawner and definition, resolved in the observer's world NPC catalog, kept on large maps since "
     "1214dx); the partner plays the native carry presentation (bank carry clips: lift, shoulder idle and walk, drop, throw), "
     "and the observer attaches its own copy of the body to the partner through the native snapshot blend and attachment "
     "(1214dq-1214dv: the host reuses a flag-only binding, so Emily's carry attaches there too). A drop (state 46) releases "
     "the body to native physics; a throw (RT while carrying) launches it with the source's throw velocity. Harness "
     "Test-CarryBody 1277/1423/1428 chokes the upper-floor guard, picks him up, walks 2.5 s, then drops or throws him over the "
     "railing, compares the body in both worlds after it settles, and films the lift (1428), the carry and the release. A case "
     "passes when the source carried natively, the remote presented every phase and the body ends within 0.5 m in both worlds.")
items = [dict(id='carry_body_lift_idle_move_drop_throw', decision='complete',
    scope=("The partner lifting an unconscious body onto the shoulder, standing and walking with it, dropping it and throwing it, "
           "in both roles, with the observer's own body attached and ending where the source's body ends."),
    takes=[{'take': 'carry-body-v1277y', 'cases': ['host-choke']}, {'take': 'carry-body-v1277z', 'cases': ['client-choke']},
           {'take': 'carry-body-v1423b', 'cases': ['host-choke']}, {'take': 'carry-body-v1423c', 'cases': ['client-choke']},
           {'take': 'carry-body-v1428a', 'cases': ['client-choke']}, {'take': 'carry-body-v1428b', 'cases': ['host-choke']}],
    visual=[{'video': 'carry-body-v1428a/client-choke-pickup-observer.mp4', 'verdict': 'Emily bends over the choked guard, reaches down and rises with him over her shoulder, then stands with him (backlit, silhouette).'},
            {'video': 'carry-body-v1428b/host-choke-pickup-observer.mp4', 'verdict': corvo_lift},
            {'video': 'carry-body-v1277y/host-choke-carry-observer.mp4', 'verdict': 'Corvo walks with the guard over his shoulder, then lowers him to the floor at his left side and stands beside the body.'},
            {'video': 'carry-body-v1277z/client-choke-carry-observer.mp4', 'verdict': 'Emily walks with the guard draped over her shoulder, his legs hanging in front, then drops him.'},
            {'video': 'carry-body-v1423b/host-choke-carry-observer.mp4', 'verdict': 'Corvo walks with the guard over his shoulder, then the body leaves his shoulder to the left and drops past the railing while his left arm swings out.'},
            {'video': 'carry-body-v1423c/client-choke-carry-observer.mp4', 'verdict': 'Emily carries the guard over her shoulder, then the body slides off to her left and falls past the railing while her arm follows.'}])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1431-carry-body', family='paired_actions', candidate='candidate-npc-combat-v1214dy', method=M, items=items)
(N / 'completion-audit-v1431-carry-body.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
