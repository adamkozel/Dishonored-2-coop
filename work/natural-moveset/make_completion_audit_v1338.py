"""Writes completion-audit-v1338-ranged-items.json (#11): the ranged weapons and items family on candidate 1214cn."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("Each decision rests on the items coverage take coverage-v1330e on candidate 1214cn (15/16; earlier coverage-v1330b and "
     "coverage-v1330d on 1214cm/cn agree), under the product controller in both source roles: each item is granted with the "
     "lab-only coop_grant_item (1214ck; the isolated save has none), selected with the native equip and used with native "
     "input; the partner is checked from its own presentation state on the observer (ranged kind and variants, shots, grenade, "
     "trap and Heart variants, the use reach) and the checkpoint is reloaded after every case. The observer films from a close "
     "side-on free camera, nearly level for standing cases. Remote presentation is cosmetic: shots, traps and grenades act only "
     "in the source world; their outcomes belong to stage 4. Lifecycle: every case re-arms the product controller after a "
     "checkpoint reload; the 2-hour soak mission-soak-v1316a covers 19 shared restarts and session renewals.")
def T(take, cases, roles=('host', 'client')): return {'take': take, 'cases': [f'{r}-{c}' for r in roles for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
E = 'coverage-v1330e'
items = [
 dict(id='pistol_draw_aim_fire_reload_holster', decision='complete',
      scope='The pistol in both roles: native draw, three shots with the reloads they cause, holster (X held) and redraw; the partner presents the gun (kind 1) through variants 0, 3, 4, 5 and 6 with every shot counted.',
      takes=[T(E, ['pistol'])],
      visual=[V(f'{E}/host-pistol-observer.mp4', 'Corvo raises the pistol at arm\'s length for each shot, lowers it to reload, puts it away and draws it again.')]),
 dict(id='crossbow_draw_aim_fire_reload_holster', decision='complete',
      scope='The crossbow in both roles: native draw, three bolts with reloads, holster and redraw; the partner presents the crossbow (kind 2) through variants 0 and 7-10, three shots each.',
      takes=[T(E, ['crossbow'])],
      visual=[V(f'{E}/host-crossbow-observer.mp4', 'Corvo extends the wrist crossbow and holds it level for each bolt, recocks between shots, then lowers the arm to holster and raises it again.')]),
 dict(id='grenade_equip_aim_throw', decision='complete',
      scope='Cooking (left trigger held) and throwing a grenade in both roles; the partner presents grenade variants 0 and 3-6 (hold, wind-up, release, recovery).',
      takes=[T(E, ['grenade'])],
      visual=[V(f'{E}/client-grenade-observer.mp4', 'Emily draws her arm back over her shoulder with the grenade, steps into the throw with her coat swinging, and recovers to the armed stance.')]),
 dict(id='mine_or_razor_place', decision='complete',
      scope='Placing a stun mine and a spring razor on the floor in both roles; the partner presents trap variants 0, 4, 5 and 7 (hold, bend, place, recovery).',
      takes=[T(E, ['stunmine', 'springrazor'])],
      visual=[V(f'{E}/host-stunmine-observer.mp4', 'Corvo looks down, bends and reaches toward the floor to set the mine, then straightens.')]),
 dict(id='heart_equip_use_holster', decision='complete',
      scope='Raising the Heart (left trigger held) and lowering it in both roles; the partner presents Heart variants 0 and 3.',
      takes=[T(E, ['heart'])],
      visual=[V(f'{E}/host-heart-observer.mp4', 'Corvo lifts the Heart in his left hand in front of his chest, holds it there, and lowers it again.')]),
 dict(id='health_elixir', decision='complete',
      scope='Drinking a health elixir in both roles: the native drink is presented as the use reach, hand to mouth (18-19 samples), and recovers.',
      takes=[T(E, ['health'])],
      visual=[V(f'{E}/client-health-observer.mp4', 'Emily brings her hand up to her mouth to drink, holds it briefly and lowers it.')]),
 dict(id='mana_elixir', decision='open',
      scope='Corvo spends mana with Windblast (to 50) and drinks a mana elixir; the drink is presented as the use reach (coverage-v1330e host).',
      takes=[T(E, ['manaelixir'], ('host',))],
      visual=[],
      remaining="Emily's drink: her mana was full at the fixed drink time in 1330e; the case now drinks as soon as her live mana drops after the Doppelganger cast, with the trace recorded."),
 dict(id='bolt_or_ammo_switch', decision='open',
      scope='Not yet recorded.', takes=[], visual=[],
      remaining='The mission quick-slot switch to sleep darts and back with the crossbow drawn (mission harness boltswitch, take 1328j on 1214cp).'),
 dict(id='item_switch_interruptions', decision='open',
      scope='Not yet recorded on the current candidate.', takes=[], visual=[],
      remaining='Switching weapons mid-aim, selecting a power and redrawing (mission harness itemswitch, take 1328j on 1214cp).'),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1338-ranged-items', family='ranged_and_items', candidate='candidate-npc-combat-v1214cn', method=M, items=items)
(N / 'completion-audit-v1338-ranged-items.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
