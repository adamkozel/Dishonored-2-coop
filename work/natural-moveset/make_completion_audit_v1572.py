"""Writes completion-audit-v1572-body-sizes.json (#11 enemy_body_size_variants): takedowns of every victim body the
campaign's paired takedowns use (small, tall, female, armored, nonhuman wolfhound) and the contextual victim states
(knocked down, seated) in both roles, with the victim streamed to the partner. Gravehound and clockwork outcomes are
stage 4 (#118). Usage: make_completion_audit_v1572.py SEATED.json (the seated takes and verdicts once reviewed)."""
import json, sys
from pathlib import Path
N = Path(__file__).resolve().parent
seated = json.loads(Path(sys.argv[1]).read_text(encoding='utf-8'))
M = ("No native player takedown scene varies by victim body: the same choke and lethal scenes (and their player clip "
     "durations) run for the small Dunwall guard, the tall overseer, the female witch and the armored Grand Guard elite "
     "(census 1259, takes 1426d/e). Only the victim's own body differs, and the partner sees it through the NPC stream "
     "(the host's victim at the priority rate, a client's victim by reverse poses and the host's paired hold). The partner "
     "presents the source's native scene with the authored clip of its kind on the native scene clock (audits v1354, v1369). "
     "Each body was taken under the product controller with natural NPC AI in a mission map, the outcome checked in both "
     "worlds and the partner's window filmed: small (dunwall_guard_male_small_01, the directional chokes of audit v1354), "
     "tall (overseer_male_tall_01_office, 1426b-e), female (witch_female_01_caster, 1488a, 1496a), armored "
     "(elite_male_tall_01, 1562a/c; the elites in Addermire are all this definition), nonhuman (wolfhound_01, its own "
     "assassination_wolfhound_* scenes, 1550a/b). Contextual victim states: knocked down after Windblast "
     "(choke_knockdown_*, the client's replica given the host's down state by 1214en and the host's body held for a "
     "client's victim pose by 1214eo; 1541a/b) and seated (choke_sit_*: the choke rule handler offers them from the back "
     "and the sides, while front_choke_sit has no action; the player's takedown sensor takes a seated NPC only from behind "
     "her shoulder while she is unaware, so the fixture presses at the first ready sample: choke_sit_left in both roles, "
     "1577a, 1578e, 1579a; seated combat NPCs stand up when the source arrives, so seated victims are civilians, whose "
     "own pose in the partner's world needs the scripted-civilian binding of #113). Gravehounds and clockwork "
     "soldiers run native takedowns too (1508b; Emily's clockwork drop behead 1554b), but their outcomes and the behead "
     "state are world authority (#118, stage 4); scripted civilians not bound in the NPC stream are #113.")
items = [dict(id='enemy_body_size_variants', decision='complete',
    scope=("Both roles: takedowns of small, tall, female, armored and wolfhound victims and of knocked-down and seated "
           "victims, each presented on the partner with the outcome shared (the seated victim, a scripted civilian, by its outcome "
           "only until #113). Gravehound and clockwork outcomes are stage 4 (#118); scripted civilians #113."),
    takes=[{'take': 'takedown-variants-v1254i', 'cases': ['client-back-choke', 'host-back-choke']},
           {'take': 'mission-coverage-v1426b', 'cases': ['host-takedown']},
           {'take': 'mission-coverage-v1426c', 'cases': ['client-takedown']},
           {'take': 'mission-coverage-v1426d', 'cases': ['client-takedown']},
           {'take': 'mission-coverage-v1426e', 'cases': ['host-takedown']},
           {'take': 'mission-coverage-v1488a', 'cases': ['client-takedown']},
           {'take': 'mission-coverage-v1496a', 'cases': ['host-takedown']},
           {'take': 'mission-coverage-v1562a', 'cases': ['host-takedown']},
           {'take': 'mission-coverage-v1562c', 'cases': ['client-takedown']},
           {'take': 'mission-coverage-v1550a', 'cases': ['host-takedown']},
           {'take': 'mission-coverage-v1550b', 'cases': ['client-takedown']},
           {'take': 'mission-coverage-v1541a', 'cases': ['client-takedown']},
           {'take': 'mission-coverage-v1541b', 'cases': ['host-takedown']}] + seated['takes'],
    visual=[
        {'video': 'takedown-variants-v1254i/back-client-choke-observer.mp4',
         'verdict': "Corvo's window: Emily takes the small Dunwall guard from behind, holds him in the choke while he struggles and lowers him to the floor (audit v1354). Accepted."},
        {'video': 'mission-coverage-v1426b/host-takedown-observer.mp4',
         'verdict': "Emily's window: Corvo grabs the tall overseer from behind; the overseer struggles with a hand raised, sinks and lies unconscious at Corvo's feet. Accepted."},
        {'video': 'mission-coverage-v1426c/client-takedown-observer.mp4',
         'verdict': "Corvo's window: Emily grabs the tall overseer from behind, holds him while he struggles and he goes down behind the stone block. Accepted."},
        {'video': 'mission-coverage-v1426d/client-takedown-observer.mp4',
         'verdict': "Corvo's window: the overseer is turned toward Emily as she drives the blade in, and he drops at the left edge. Accepted."},
        {'video': 'mission-coverage-v1426e/host-takedown-observer.mp4',
         'verdict': "Emily's window: Corvo takes the overseer from behind, holds him through the stab and the overseer drops behind the stone block. Accepted."},
        {'video': 'mission-coverage-v1488a/client-takedown-observer.mp4',
         'verdict': "Corvo's window: Emily creeps up behind the witch at the balustrade, pulls her back against her shoulder, holds her while she arches and struggles, and lowers her to the ground. Accepted."},
        {'video': 'mission-coverage-v1496a/host-takedown-observer.mp4',
         'verdict': "Emily's window: Corvo creeps up, grabs the witch and pulls her back against him, holds her while she struggles and lowers her to the ground. Accepted."},
        {'video': 'mission-coverage-v1562a/host-takedown-observer.mp4',
         'verdict': "Emily's window: Corvo grabs the red-coated armored elite from behind, holds the choke and lowers him by the railing. The held choke press then starts the native body pickup, and the body jumps half up beside the railing in both worlds (incidental, after the takedown). Accepted."},
        {'video': 'mission-coverage-v1562c/client-takedown-observer.mp4',
         'verdict': "Corvo's window: Emily grabs the armored elite from behind, holds him and lowers him. Accepted."},
        {'video': 'mission-coverage-v1550a/host-takedown-observer.mp4',
         'verdict': "Emily's window: Corvo comes down off the planter, seizes the wolfhound from behind and drives his blade in; the hound rears in his grip and he kneels over it as it dies. Accepted."},
        {'video': 'mission-coverage-v1550b/client-takedown-observer.mp4',
         'verdict': "Corvo's window: Emily drops beside the wolfhound and stabs it, and it collapses dead at her feet. Accepted."},
        {'video': 'mission-coverage-v1541b/host-takedown-observer.mp4',
         'verdict': "Emily's window: the knocked-down overseer gets up on hands and knees and Corvo bends over him reaching down with his left arm while he goes unconscious (the partner's body is the standing choke clip bent toward the victim, the presentation standard for every takedown). Accepted."},
        {'video': 'mission-coverage-v1541a/client-takedown-observer.mp4',
         'verdict': "Corvo's window: Emily stands over the kneeling overseer, steps back and crouches at the edge of the frame while he collapses. Accepted."}] + seated['visual'])]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1572-body-sizes', family='paired_actions', candidate=seated['candidate'], method=M, items=items)
(N / 'completion-audit-v1572-body-sizes.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
