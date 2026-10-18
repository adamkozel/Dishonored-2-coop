"""Writes completion-audit-v1337-powers.json (#11): the powers family's decisions on candidates 1214cm/cn."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("Each decision rests on the powers coverage takes coverage-v1332c (candidate 1214cm, the observer's own view from about "
     "9 m) and coverage-v1332d (candidate 1214cn, a close side-on free camera), 12/12 each, under the product controller: "
     "every power is granted by name (lab-only coop_grant_named, 1214cm), selected with the native equip and cast with the "
     "native left trigger; the partner is checked from its own power presentation state on the observer (power weight above "
     "0.5 for at least three samples, then recovery) and the checkpoint is reloaded after every case. World effects are judged "
     "from the reviewed observer videos listed per item. Remote presentation is cosmetic: the partner never casts, spends mana "
     "or applies damage (partner exclusion, chapter 34). Lifecycle: every case re-arms the product controller after a checkpoint "
     "reload; the 2-hour soak mission-soak-v1316a covers 19 shared restarts and session renewals.")
def T(take, cases, roles=('host', 'client')): return {'take': take, 'cases': [f'{r}-{c}' for r in roles for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
C, D = 'coverage-v1332c', 'coverage-v1332d'
items = [
 dict(id='corvo_dark_vision_cast_end', decision='complete',
      scope='Corvo casts and ends Dark Vision (power variant 10) standing armed; the partner presents both gestures and recovers. Dark Vision has no world-visible effect: its reveal and screen effect stay with the caster (first-person scope).',
      takes=[T(C, ['darkvision'], ('host',)), T(D, ['darkvision'], ('host',))],
      visual=[V(f'{D}/host-darkvision-observer.mp4', 'Corvo raises his left hand to his eye to cast, holds the armed stance, and repeats the gesture to end it; no effect appears in the observer world.')]),
 dict(id='emily_dark_vision_cast_end', decision='complete',
      scope='Emily casts and ends Dark Vision (power variant 10) standing armed; the partner presents both gestures and recovers. The reveal and screen effect stay with the caster.',
      takes=[T(C, ['darkvision'], ('client',)), T(D, ['darkvision'], ('client',))],
      visual=[V(f'{D}/client-darkvision-observer.mp4', 'Emily lifts her hand to her face to cast and again to end Dark Vision, sword held low, then settles.')]),
 dict(id='corvo_windblast_cast', decision='complete',
      scope='Corvo casts Windblast (power variant 11): the partner presents the push and recovery, and the native wind streak plays once in the observer world at the cast.',
      takes=[T(C, ['windblast'], ('host',)), T(D, ['windblast'], ('host',))],
      visual=[V(f'{D}/host-windblast-observer.mp4', 'Corvo braces and thrusts his left arm; a single wind streak sweeps out in front of him on the observer, then he returns to the armed stance.'),
              V(f'{C}/host-windblast-observer.mp4', 'From a distance the streak fans out ahead of Corvo and clears within about a second.')]),
 dict(id='corvo_devouring_swarm_cast', decision='complete',
      scope='Corvo casts Devouring Swarm (power variants 23 and 25): the partner presents the cast and recovery, and the swarm appears in the observer world, spreads across the floor and disperses.',
      takes=[T(C, ['swarm'], ('host',)), T(D, ['swarm'], ('host',))],
      visual=[V(f'{D}/host-swarm-observer.mp4', 'Corvo lunges and throws his left hand forward to summon the swarm, then recovers.'),
              V(f'{C}/host-swarm-observer.mp4', 'A dark mass of rats spreads over the floor ahead of Corvo for about a second and disperses.')]),
 dict(id='corvo_blink_aim_cancel_release_arrive', decision='open',
      scope='Aim, release, departure burst and arrival are presented (power variants 1-3; 1332c/d).',
      takes=[T(C, ['blink'], ('host',)), T(D, ['blink'], ('host',))],
      visual=[V(f'{D}/host-blink-observer.mp4', 'Corvo holds the aim with his arm out; a blue burst marks the departure as he vanishes forward.')],
      remaining='A cancelled aim on the current candidate (the coverage cases always release).'),
 dict(id='corvo_bend_time_cast_end', decision='open',
      scope='Cast and release gestures are presented (power variants 12, 14; 1332c/d).',
      takes=[T(C, ['bendtime'], ('host',)), T(D, ['bendtime'], ('host',))],
      visual=[V(f'{D}/host-bendtime-observer.mp4', 'Corvo raises his left hand to cast and extends it to release; readable but dark.')],
      remaining='What the partner world does during Bend Time (shared world time) is a stage 4 authority decision; the visual of the cast in better light.'),
 dict(id='corvo_possession_enter_loop_exit', decision='open',
      scope='The empty-target entry and aim are presented (power variants 26, 27; 1332c/d). Human-target occupation was verified on the tutorial guard in take 372 (older candidate).',
      takes=[T(C, ['possession'], ('host',))], visual=[],
      remaining='A human-target possession on the current candidate (entry, hidden body, exit and reappearance); control of the possessed NPC in the partner world is stage 4.'),
 dict(id='emily_far_reach_aim_cancel_release_arrive', decision='open',
      scope='The aim is presented (power variant 4); the pull toward a floor point is too short to film from the side.',
      takes=[T(C, ['farreach'], ('client',))], visual=[],
      remaining='A far reach to a distant target with travel and arrival in frame, and a cancelled aim.'),
 dict(id='emily_domino_mark_link_cancel', decision='open',
      scope='The cast gesture is presented (power variants 19, 20, 22) with no target.',
      takes=[T(C, ['domino'], ('client',))], visual=[], remaining='Marking and linking real targets (mission guards) and the link effect on the observer.'),
 dict(id='emily_doppelganger_cast', decision='open',
      scope='The cast is presented (power variants 29-31).',
      takes=[T(C, ['doppelganger'], ('client',))], visual=[],
      remaining='The clone does not appear in the observer world in 1332c or 1332d; it must be presented and retired.'),
 dict(id='emily_mesmerize_cast', decision='open',
      scope='Enter, aim and release gestures are presented (power variants 16-18).',
      takes=[T(C, ['mesmerize'], ('client',))], visual=[],
      remaining='The spirit does not appear in the observer world in 1332c or 1332d; it must be presented with its targets and dismissal.'),
 dict(id='emily_shadow_walk_enter_move_attack_exit', decision='open',
      scope='Entry, crawl and exit are presented (power variants 32-34; 45 samples).',
      takes=[T(C, ['shadowwalk'], ('client',))], visual=[],
      remaining='An attack from shadow form and the transformed material on the observer.'),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1337-powers', family='powers', candidate='candidate-npc-combat-v1214cn', method=M, items=items)
(N / 'completion-audit-v1337-powers.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
