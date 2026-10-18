"""Writes completion-audit-v1335-combat.json (#11): the combat family's decisions without an opponent, on candidate 1214cm."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("Each decision rests on coverage takes on candidate 1214cm under the product controller (both source roles, the partner "
     "checked from its own presentation state on the observer, the checkpoint reloaded after every case): coverage-v1324g "
     "(swings, blocks, moving and jump attacks, holster and draw; 10/10) repeating coverage-v1324f (8/8), on the reviewed "
     "observer videos listed per item, and on the earlier visual sweep 1260 acceptance recorded in the item notes. Lifecycle: "
     "every coverage case re-arms the product controller after a checkpoint reload and waits for the partner presentation to "
     "come back; the 2-hour soak mission-soak-v1316a covers 19 shared restarts and session renewals; the clean-PC take "
     "product-two-account-v1315b covers the two-account Steam session, leave and rejoin. Items that need an opponent (parry, "
     "counter, hit reactions, stagger, kick, deaths, ragdoll) stay open for the mission guard fight.")
def T(take, cases): return {'take': take, 'cases': [f'{r}-{c}' for r in ('host', 'client') for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
G = 'coverage-v1324g'
items = [
 dict(id='sword_draw_holster', decision='complete',
      scope='Native holster and draw while crouched and while walking in both roles: the partner armed mix follows each change in context (crouched stays crouched, walking keeps walking); the standing draw and holster are part of every armed case and of the gait take 1320g.',
      takes=[T(G, ['sheathe'])],
      visual=[V(f'{G}/host-sheathe-observer.mp4', 'Corvo, crouched, takes the hand to the hip and puts the sword away, holds an unarmed crouched idle, then draws it again; walking, he holsters into an unarmed arm swing and later walks on with the sword drawn.')]),
 dict(id='sword_ready_idle', decision='complete',
      scope='The standing and crouched sword-ready idles between every swing, block and jump attack in both roles; each action settles back to the armed idle.',
      takes=[T(G, ['swing', 'block', 'airattack'])],
      visual=[V(f'{G}/client-airattack-observer.mp4', 'Emily lands from each jump strike and settles into the standing sword-ready idle, blade low at her side, before the next jump.')]),
 dict(id='light_attack_variants', decision='complete',
      scope='Light attacks and three-hit combos in both roles: Corvo presents combat variants 1, 2, 4 and 5, Emily 1, 2, 3 and 5 (plus the heavy and crouched variants 7, 8 and 18), each settling afterwards.',
      takes=[T(G, ['swing'])],
      visual=[V('coverage-v1324f/host-swing-observer.mp4', 'Corvo throws the single slash and the three-hit combo from the armed stance, his torso turning into each cut, then settles.')]),
 dict(id='heavy_attack_variants', decision='complete',
      scope='The heavy attack standing and crouched in both roles (wind-up, release, recovery; combat variants 7 and 8, the crouched heavy without a standing pop since audit 1286).',
      takes=[T(G, ['swing'])],
      visual=[V('coverage-v1324f/host-swing-observer.mp4', 'Corvo winds the heavy attack back and lunges with a wide step; crouched, the heavy stays low through the lunge and he recovers crouched.')]),
 dict(id='moving_attack', decision='complete',
      scope='Light attacks while walking forward, right and back and while running in both roles, with the partner still travelling through each.',
      takes=[T(G, ['moveattack'])],
      visual=[V(f'{G}/host-moveattack-observer.mp4', 'Corvo slashes while stepping forward, sidestepping right past the camera and backing away, and swings in a running stride; the legs keep the gait under each strike.')]),
 dict(id='crouched_attack', decision='complete',
      scope='Crouched light and crouched heavy attacks in both roles, presented crouched from entry to recovery (combat variant 18 and the crouched heavy).',
      takes=[T(G, ['swing'])],
      visual=[V('coverage-v1324f/host-swing-observer.mp4', 'Corvo strikes from the crouch with the arm extended and lunges low for the crouched heavy, staying crouched, then stands.')]),
 dict(id='airborne_attack', decision='complete',
      scope='Early, late and running jump attacks in both roles: combat presented in the air, then the landing settles.',
      takes=[T(G, ['airattack'])],
      visual=[V(f'{G}/client-airattack-observer.mp4', 'Emily leaps with her legs tucked and slashes at the top of the jump, lands with the blade still out and recovers to the ready idle.')]),
 dict(id='block_enter_hold_exit', decision='complete',
      scope='Standing, crouched and airborne blocks in both roles: the guard is presented in context for most of each hold, then recovers.',
      takes=[T(G, ['block'])],
      visual=[V(f'{G}/client-block-observer.mp4', 'Emily raises the guard standing, again from the crouch (staying crouched), and once more at the top of a jump, lowering it on landing.')]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1335-combat', family='combat', candidate='candidate-npc-combat-v1214cm', method=M, items=items)
(N / 'completion-audit-v1335-combat.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
