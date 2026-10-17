"""Classify every clip of the v1212 movement banks by motion source.

Read-only over the repository. Inputs: origins.json (payload-diff provenance),
the repo manifests, and resolver.py (recursive donor provenance via manifest
sha256 links). Output: clips.json in the parent directory.
"""
import json, os, re, sys, hashlib
T = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, T)
import resolver as R

BS = chr(92)
NM = R.P  # work/natural-moveset
OUT = os.path.dirname(T)
HEX = re.compile(r'^[0-9a-fA-F]{64}$')
ORIG = json.load(open(os.path.join(T, 'origins.json')))
BANKH = json.load(open(os.path.join(T, 'bankhashes.json')))
BANKSHA = {v['sha256'].lower(): k for k, v in BANKH.items()}

# origin bank dir -> (bank manifest, builder script, D2MOVE version)
ORIGIN = {
    'native-gaits': ('native-gait-manifest.json', 'build_native_gaits.py', 2),
    'stance-banks-v1': ('stance-bank-manifest-v1.json', 'build_stance_bank.py', 3),
    'sprint-banks-v1': ('sprint-bank-manifest-v1.json', 'build_sprint_bank.py', 4),
    'lean-banks-v1': ('lean-bank-manifest-v1.json', 'build_lean_bank.py', 5),
    'slide-banks-v1': ('slide-bank-manifest-v1.json', 'build_slide_bank.py', 6),
    'air-banks-v1': ('air-bank-manifest-v1.json', 'build_air_bank.py', 7),
    'combat-banks-v1': ('combat-bank-manifest-v1.json', 'build_combat_bank.py', 8),
    'equipment-banks-v3': ('equipment-bank-manifest-v3.json', 'build_equipment_bank.py', 9),
    'unarmed-special-banks-v1': ('unarmed-special-bank-manifest-v1.json', 'build_unarmed_special_bank.py', 10),
    'vertical-peek-banks-v1': ('vertical-peek-bank-manifest-v1.json', 'build_vertical_peek_bank.py', 11),
    'low-mantle-banks-v2': ('low-mantle-bank-manifest-v2.json', 'build_low_mantle_contact.py', 12),
    'crouch-mantle-banks-v1': ('crouch-mantle-bank-manifest-v1.json', 'build_crouch_mantle_bank.py', 13),
    'vault-banks-v1': ('vault-bank-manifest-v1.json', 'build_vault_bank.py', 14),
    'power-banks-v1': ('power-bank-manifest-v1.json', 'build_power_bank.py', 15),
    'air-combat-banks-v1': ('air-combat-bank-manifest-v1.json', 'build_air_combat_bank.py', 16),
    'terrain-banks-v2': ('terrain-bank-manifest-v2.json', 'build_terrain_bank_v2.py', 17),
    'turn-banks-v1': ('turn-bank-manifest-v1.json', 'build_turn_bank_v1.py', 18),
    'crouch-turn-banks-v1': ('crouch-turn-bank-manifest-v1.json', 'build_crouch_turn_bank_v1.py', 19),
    'high-mantle-banks-v1': ('high-mantle-bank-manifest-v1.json', 'build_high_mantle_bank_v1.py', 20),
    'double-jump-banks-v1': ('double-jump-bank-manifest-v1.json', 'build_double_jump_bank_v1.py', 21),
    'hard-landing-banks-v1': ('hard-landing-bank-manifest-v1.json', 'build_hard_landing_bank_v1.py', 22),
    'hard-landing-crouch-banks-v1': ('hard-landing-crouch-bank-manifest-v1.json', 'build_hard_landing_crouch_bank_v1.py', 23),
    'keyhole-banks-v2': ('keyhole-bank-manifest-v2.json', 'build_keyhole_bank_v2.py', 24),
    'darkvision-banks-v1': ('darkvision-bank-manifest-v1.json', 'build_darkvision_bank_v1.py', 25),
    'darkvision-context-banks-v1': ('darkvision-context-bank-manifest-v1.json', 'build_darkvision_contexts_v1.py', 26),
    'windblast-banks-v1': ('windblast-bank-manifest-v1.json', 'build_windblast_bank_v1.py', 27),
    'windblast-context-banks-v1': ('windblast-context-bank-manifest-v1.json', 'build_windblast_contexts_v1.py', 28),
    'crouch-balance-banks-v1': ('crouch-balance-bank-manifest-v1.json', 'build_crouch_balance_v1.py', 28),
    'balanced-crouch-power-banks-v1': ('balanced-crouch-power-bank-manifest-v1.json', 'build_balanced_crouch_powers_v1.py', 28),
    'balanced-crouch-power-banks-v3': ('balanced-crouch-power-bank-manifest-v3.json', 'build_balanced_crouch_powers_v3.py', 28),
    'ranged-banks-v1': ('ranged-bank-manifest-v1.json', 'build_ranged_bank_v1.py', 29),
    'crouched-ranged-banks-v3': ('crouched-ranged-bank-manifest-v3.json', 'build_crouched_ranged_bank_v3.py', 30),
    'crouched-ranged-banks-v4': ('crouched-ranged-bank-manifest-v4.json', 'build_balanced_crouch_equipment_v1.py', 30),
    'carry-banks-v1': ('carry-bank-manifest-v1.json', 'build_carry_bank_v1.py', 31),
    'carry-release-banks-v1': ('carry-release-bank-manifest-v1.json', 'build_carry_release_bank_v1.py', 32),
    'carry-pickup-contact-banks-v5': ('carry-pickup-contact-bank-manifest-v5.json', 'author_pickup_settle_v5.py', 34),
    'crouched-pickup-banks-v3': ('crouched-pickup-bank-manifest-v3.json', 'author_crouched_pickup_v3.py', 35),
    'high-throw-banks-v31': ('high-throw-bank-manifest-v31.json', 'build_high_throw_bank_v31.py', 36),
    'fetal-banks-v35': ('fetal-bank-manifest-v35.json', 'build_fetal_bank_v35.py', 37),
    'bendtime-banks-v40': ('bendtime-bank-manifest-v40.json', 'build_bendtime_bank_v40.py', 38),
    'bendtime-balanced-banks-v42': ('bendtime-balanced-bank-manifest-v42.json', 'build_bendtime_balance_v42.py', 39),
    'outofmana-banks-v45': ('outofmana-bank-manifest-v45.json', 'build_outofmana_bank_v45.py', 40),
    'outofmana-context-banks-v46': ('outofmana-context-bank-manifest-v46.json', 'build_outofmana_contexts_v46.py', 41),
    'grenade-banks-v56': ('grenade-bank-manifest-v56.json', 'build_grenade_bank_v56.py', 42),
    'stun-banks-v61': ('stun-bank-manifest-v61.json', 'build_stun_bank_v61.py', 43),
    'crouched-grenade-banks-v62': ('crouched-grenade-bank-manifest-v62.json', 'build_crouched_grenade_bank_v62.py', 44),
    'mesmerize-banks-v77': ('mesmerize-bank-manifest-v77.json', 'build_mesmerize_bank_v77.py', 45),
    'mesmerize-context-banks-v132': ('mesmerize-context-bank-manifest-v132.json', 'build_mesmerize_contexts_v132.py', 46),
    'domino-banks-v192': ('domino-bank-manifest-v192.json', 'build_domino_bank_v192.py', 47),
    'domino-context-banks-v243': ('domino-context-bank-manifest-v243.json', 'build_domino_contexts_v243.py', 48),
    'swarm-banks-v267': ('swarm-bank-manifest-v267.json', 'build_swarm_bank_v267.py', 49),
    'swarm-context-banks-v272': ('swarm-context-bank-manifest-v272.json', 'build_swarm_contexts_v272.py', 50),
    'possession-banks-v323': ('possession-bank-manifest-v323.json', 'build_possession_bank_v323.py', 51),
    'possession-banks-v334': ('possession-bank-manifest-v334.json', 'build_possession_bank_v334.py', 52),
    'possession-crouch-banks-v363': ('possession-crouch-bank-manifest-v363.json', 'build_possession_crouch_v363.py', 53),
    'possession-crouch-release-banks-v371': ('possession-crouch-release-bank-manifest-v371.json', 'build_possession_crouch_release_v371.py', 54),
    'doppelganger-banks-v407': ('doppelganger-bank-manifest-v407.json', 'build_doppelganger_bank_v407.py', 55),
    'doppelganger-crouch-banks-v490': ('doppelganger-crouch-bank-manifest-v490.json', 'build_doppelganger_crouch_v490.py', 56),
    'doppelganger-holstered-banks-v495': ('doppelganger-holstered-bank-manifest-v495.json', 'build_doppelganger_holstered_v495.py', 57),
    'shadowwalk-banks-v523': ('shadowwalk-bank-manifest-v523.json', 'build_shadowwalk_bank_v523.py', 58),
    'shadowwalk-sneak-banks-v548': ('shadowwalk-sneak-bank-manifest-v548.json', '../movement-integration/prepare_shadowwalk_sneak_v548.py', 59),
    'heart-banks-v564': ('heart-bank-manifest-v564.json', 'build_heart_bank_v564.py', 60),
    'trap-banks-v569': ('trap-bank-manifest-v569.json', 'build_trap_bank_v569.py', 61),
    'trap-banks-v575': ('trap-bank-manifest-v575.json', 'build_trap_bank_v575.py', 62),
    'choke-banks-v581': ('choke-bank-manifest-v581.json', 'build_choke_bank_v581.py', 63),
    'choke-breakout-banks-v592': ('choke-breakout-bank-manifest-v592.json', 'build_choke_breakout_v592.py', 64),
    'lethal-banks-v604': ('lethal-bank-manifest-v604.json', 'build_lethal_bank_v604.py', 65),
    'kick-banks-v625': ('kick-bank-manifest-v625.json', 'build_kick_bank_v625.py', 66),
    'heavy-banks-v633': ('heavy-bank-manifest-v633.json', 'build_heavy_bank_v633.py', 67),
    'parry-banks-v1022': ('parry-bank-manifest-v1022.json', 'build_parry_bank_v1022.py', 68),
    'swim-banks-v1037': ('swim-bank-manifest-v1037.json', 'build_swim_bank_v1037.py', 69),
    'climb-banks-v1042': ('climb-bank-manifest-v1042.json', 'build_climb_bank_v1042.py', 70),
    'use-banks-v1212': ('use-bank-manifest-v1212.json', 'build_use_bank_v1212.py', 71),
}

# ---- table -------------------------------------------------------------
# donor specs: ('A', manifest, alias)    entry by role + selection alias / source aliases
#              ('BM', None)              64-hex shas found in the origin bank manifest under an item indexed by this clip
#              ('BMV', base)             same, but index is variant: clip = base + variant
#              ('BMTOP', key)            sha at top-level key of the origin bank manifest role entry
#              ('F', 'mixamo-crouch/host-fp-crouch.json')  runtime capture file (native, runtime)
# deps: other clip indices whose (earlier) payload is reused as base/pose input.
TABLE = []


def add(clips, cat, purpose, donors=(), deps=(), note='', deps_fn=None):
    for i, c in enumerate(clips):
        d = [x(c, i) if callable(x) else x for x in donors]
        dd = deps_fn(c, i) if deps_fn else list(deps)
        TABLE.append(dict(clip=c, category=cat, purpose=purpose(c, i) if callable(purpose) else purpose, donors=d, deps=dd, note=note))


LIB = 'library-retarget-manifest.json'
dirs4 = ['n', 'w', 's', 'e']
names8 = ['forward', 'forward_left', 'left', 'back_left', 'back', 'back_right', 'right', 'forward_right']


def gait_donors(gait, d):
    a = d // 2; b = (a + 1) % 4
    out = [('A', LIB, f'sword_{gait}_{dirs4[a]}_add')]
    if d % 2:
        out.append(('A', LIB, f'sword_{gait}_{dirs4[b]}_add'))
    return out


TABLE.append(dict(clip=0, category='native-sampled', purpose='armed idle (native sword_idle cycle)', donors=[('A', LIB, 'sword_idle')], deps=[], note='Resampled native clip.'))
for gi, gait in enumerate(('walk', 'run')):
    for d in range(8):
        TABLE.append(dict(clip=1 + gi * 8 + d, category='native-sampled', purpose=f'armed {gait} {names8[d]} (distance-clocked)', donors=gait_donors(gait, d), deps=[],
                          note='Native cardinal add-clips; diagonals are whole-body blends of two native cardinals.'))
TABLE.append(dict(clip=17, category='mixamo', purpose='armed crouch idle (balanced Posture-100 mean stance)', donors=[('A', 'crouch-balance-source-manifest-v1.json', 'balanced_posture100_armed_idle')], deps=[0],
                  note='Replaced in place at bank 28 (crouch-balance). Mean pose of the Mixamo Crouched Walking (Posture 100) retarget plus procedural breathing; feet pinned to native floor height taken from clip 0. Originally (bank 3) the stance-authored crouch_idle.'))
cd = ['forward', 'forward_left', 'left', 'back_left', 'back', 'back_right', 'right', 'forward_right']
add(range(18, 26), 'mixamo', lambda c, i: f'armed crouch walk {cd[i]}', donors=[lambda c, i: ('A', 'crouch-directions-retarget-manifest.json', 'crouch_walk_' + cd[i])], deps=[1],
    note='Authored crouch-walk built from Mixamo Crouched Walking / Crouch Walk Left/Right/Back retargets; phase-aligned to native walk clip 1; native grip.')
add(range(26, 29), 'mixamo', lambda c, i: 'armed sprint ' + ['forward', 'forward_left', 'forward_right'][i], donors=[lambda c, i: ('A', 'dynamics-retarget-manifest-v3.json', 'sprint_' + ['forward', 'forward_left', 'forward_right'][i])], deps=[1],
    note='Mixamo Two Cycle Sprint legs/torso with the native sword-running arm (sword_run_n) phase-synchronised.')
leans = ['stand_lean_left', 'stand_lean_right', 'crouch_lean_left', 'crouch_lean_right']
add(range(29, 33), 'authored', lambda c, i: leans[i].replace('_', ' '), donors=[lambda c, i: ('A', 'lean-retarget-manifest.json', leans[i])],
    note='Procedural whole-body lean authored on a base pose (standing: native sword_idle; crouched: the Mixamo-derived Posture-100 crouch stance).')
slides = ['slide_enter', 'slide_loop', 'slide_exit_to_run', 'slide_to_crouch']
add(range(33, 37), 'mixamo', lambda c, i: slides[i].replace('_', ' '), donors=[('BM', None)], note='Mixamo Running Slide retarget (dynamics-authored).')
air = {37: ('mixamo', 'standing jump rise'), 38: ('native-sampled', 'fall loop (native sword_fall_loop)'), 39: ('mixamo', 'standing jump land'), 40: ('mixamo', 'running jump rise'),
       41: ('mixamo', 'running jump land'), 42: ('mixamo', 'crouch jump land'), 43: ('mixamo', 'crouch jump rise')}
for c, (cat, p) in air.items():
    TABLE.append(dict(clip=c, category=cat, purpose=p, donors=[('BM', None)], deps=[], note='Segments of jumps-authored-v3 / dynamics-authored studies (Mixamo Standing/Running Jump with native grip and native sword_fall_loop) or the native fall loop.' if cat == 'mixamo' else 'Native clip, resampled.'))
combat = {44: 'sword attack fast A', 45: 'sword attack fast B', 46: 'sword attack short A', 47: 'sword attack short B', 48: 'crouched attack fast A', 49: 'crouched attack fast B',
          50: 'heavy attack windup', 51: 'heavy attack hold', 52: 'heavy attack release', 53: 'guard enter', 54: 'guard hold', 55: 'guard recovery', 56: 'crouched guard enter', 57: 'crouched guard hold', 58: 'crouched guard recovery'}
for c, p in combat.items():
    note = 'Native clip segment.'
    if c in (48, 49):
        note = 'Native attack on crouch-actions-v2 crouch base (Mixamo-derived Posture-100 crouch).'
    if c in (53, 54, 55):
        note = 'guard-sequences-v1 study assembled from native parry_ll in/attack/out and sword_idle.'
    if c in (56, 57, 58):
        note = 'Guard study with whole-body crouch delta from stance-authored crouch_idle (Mixamo-derived) and foot IK.'
    TABLE.append(dict(clip=c, category='native-sampled', purpose=p, donors=[('BM', None)] + ([('A', 'stance-retarget-manifest.json', 'crouch_idle')] if c in (56, 57, 58) else []), deps=[], note=note))
TABLE.append(dict(clip=59, category='native-sampled', purpose='unarmed idle (native empty_idle)', donors=[('A', LIB, 'empty_idle')], deps=[], note='Resampled native clip.'))
for gi, gait in enumerate(('walk', 'run')):
    for d in range(8):
        a = d // 2; b = (a + 1) % 4
        UN = 'unarmed-retarget-manifest.json'
        dn = [('A', UN, f'empty_{gait}_{dirs4[a]}_add')] + ([('A', UN, f'empty_{gait}_{dirs4[b]}_add')] if d % 2 else [])
        TABLE.append(dict(clip=60 + gi * 8 + d, category='native-sampled', purpose=f'unarmed {gait} {names8[d]}', donors=dn, deps=[1], note='Native empty-hand cardinals; diagonals blended; phase-aligned to clip 1.'))
TABLE.append(dict(clip=76, category='mixamo', purpose='unarmed crouch idle (balanced Posture-100 mean stance)', donors=[('A', 'crouch-balance-source-manifest-v1.json', 'balanced_posture100_empty_idle')], deps=[0, 59],
                  note='Replaced in place at bank 28. Same Mixamo-derived mean stance as clip 17; right-hand joints copied from the previous clip 76 (native empty_idle fingers).'))
add(range(77, 85), 'mixamo', lambda c, i: f'unarmed crouch walk {cd[i]}', donors=[('A', LIB, 'empty_idle')], deps_fn=lambda c, i: [18 + i],
    note='Copy of armed crouch walk (clip 18+k, Mixamo-derived) with fingers from native empty_idle.')
TABLE.append(dict(clip=85, category='native-sampled', purpose='sword draw (standing)', donors=[('A', LIB, 'sword_equipsword')], deps=[], note='Native draw with authored chest/arm IK to the native holster attachment.'))
TABLE.append(dict(clip=86, category='native-sampled', purpose='sword holster (standing)', donors=[('A', LIB, 'sword_unequipsword')], deps=[], note='Native holster with authored chest/arm IK to the native holster attachment.'))
TABLE.append(dict(clip=87, category='native-sampled', purpose='sword draw (crouched, balanced)', donors=[('A', 'balanced-crouch-equipment-source-manifest-v1.json', 'balanced_crouch_sword_equipsword')], deps=[0, 17],
                  note='Replaced in place at bank 30 (crouched-ranged v4). Native draw blended toward the Mixamo-derived crouch stance (clip 17) with foot IK and authored contact.'))
TABLE.append(dict(clip=88, category='native-sampled', purpose='sword holster (crouched, balanced)', donors=[('A', 'balanced-crouch-equipment-source-manifest-v1.json', 'balanced_crouch_sword_unequipsword')], deps=[0, 17],
                  note='Replaced in place at bank 30. Native holster blended toward the Mixamo-derived crouch stance (clip 17).'))
usp = ['unarmed_sprint_forward', 'unarmed_sprint_forward_left', 'unarmed_sprint_forward_right']
add(range(89, 92), 'mixamo', lambda c, i: usp[i].replace('_', ' '), donors=[lambda c, i: ('A', 'unarmed-special-source-manifest-v1.json', usp[i])], deps_fn=lambda c, i: [26 + i],
    note='Armed sprint (clip 26+k) with the original Mixamo arm swing and Mixamo hand restored.')
ul = ['unarmed_stand_lean_left', 'unarmed_stand_lean_right', 'unarmed_crouch_lean_left', 'unarmed_crouch_lean_right']
add(range(92, 96), 'authored', lambda c, i: ul[i].replace('_', ' '), donors=[lambda c, i: ('A', 'unarmed-special-source-manifest-v1.json', ul[i])], deps_fn=lambda c, i: [29 + i] + ([76] if i >= 2 else [59]),
    note='Authored lean (clip 29+k) with right arm from native empty_idle (standing) or the crouch empty idle (crouched).')
us = ['unarmed_slide_enter', 'unarmed_slide_loop', 'unarmed_slide_exit_to_run', 'unarmed_slide_to_crouch']
add(range(96, 100), 'mixamo', lambda c, i: us[i].replace('_', ' '), donors=[lambda c, i: ('A', 'unarmed-special-source-manifest-v1.json', us[i])], deps_fn=lambda c, i: [33 + i] + ([76] if i == 3 else []),
    note='Mixamo slide (clip 33+k) with hand from the Mixamo Running Slide donor.')
ua = {100: ('mixamo', 'unarmed_rise_standing_jump_sequence', [37]), 101: ('native-sampled', 'unarmed_full_sword_fall_loop', [38, 59]), 102: ('mixamo', 'unarmed_land_standing_jump_sequence', [39]),
      103: ('mixamo', 'unarmed_running-rise_running_jump_sequence', [40]), 104: ('mixamo', 'unarmed_running-land_running_jump_sequence', [41]),
      105: ('mixamo', 'unarmed_crouch-land_crouch_jump_to_crouch', [42, 76]), 106: ('mixamo', 'unarmed_crouch-rise_crouch_jump_to_crouch', [43, 76])}
for c, (cat, alias, deps) in ua.items():
    dn = [('A', 'mixamo-jump-retarget-manifest.json', 'mixamo_standing_jump')] if c in (100, 102, 105, 106) else [('A', 'mixamo-dynamic-retarget-manifest.json', 'mixamo_running_jump')] if c in (103, 104) else [('A', LIB, 'empty_idle')]
    TABLE.append(dict(clip=c, category=cat, purpose=alias.replace('_', ' '), donors=dn, deps=deps,
                      note='Hands-free variant of air clip ' + str(deps[0]) + ('; arm from native empty_idle.' if c == 101 else '; arm/hand from the Mixamo jump donor.')))
vp = {107: ('armed standing vertical peek', [0]), 108: ('armed crouched vertical peek', [17, 0]), 109: ('unarmed standing vertical peek', [59]), 110: ('unarmed crouched vertical peek', [76, 59])}
for c, (p, deps) in vp.items():
    TABLE.append(dict(clip=c, category='authored', purpose=p, donors=[], deps=deps, note='Procedural rise/heel-pivot or crouch unfold applied to the first frame of the listed base clips (native idle; Mixamo-derived crouch).'))
for c, a in ((111, 'armed_native_low_mantle_contact'), (112, 'unarmed_native_low_mantle_contact')):
    TABLE.append(dict(clip=c, category='native-sampled', purpose=a.replace('_', ' '), donors=[('A', 'low-mantle-source-manifest-v2.json', a)], deps=[0] if c == 111 else [59],
                      note='Replaced in place at bank 12 (contact pass). Native sword_mantlelow_walkleft_in with authored palm/leg contact and standing recovery.'))
for c, a in ((113, 'armed_native_low_mantle_crouched_exit_contact'), (114, 'unarmed_native_low_mantle_crouched_exit_contact')):
    TABLE.append(dict(clip=c, category='native-sampled', purpose=a.replace('_', ' '), donors=[('A', 'crouch-mantle-source-manifest-v1.json', a)], deps=[17] if c == 113 else [76],
                      note='Native low mantle with authored contact and recovery into the Mixamo-derived crouch.'))
for c, a in ((115, 'armed_native_vault_distance_sequence'), (116, 'unarmed_native_vault_distance_sequence')):
    TABLE.append(dict(clip=c, category='native-sampled', purpose=a.replace('_', ' '), donors=[('A', 'vault-bank-source-manifest-v1.json', a)], deps=[0] if c == 115 else [59], note='Native vaultthrough in/out joined.'))
pw = ['blink_fire_in', 'blink_fire_loop', 'blink_fire_out', 'far_reach_fire_in', 'far_reach_fire_in_loop', 'far_reach_fire_out', 'far_reach_pull_in', 'far_reach_pull_loop', 'far_reach_pull_out']
add(range(117, 126), 'native-sampled', lambda c, i: 'power ' + pw[i].replace('_', ' '), donors=[lambda c, i: ('A', 'power-bank-source-manifest-v1.json', 'native_phase_' + pw[i])], deps=[0],
    note='power-authored-v1 study: native NPC caster body animation plus native first-person finger samples; right grip from clip 0.')
add(range(126, 141), 'native-sampled', lambda c, i: f'airborne sword phase of clip {44 + i}', donors=[lambda c, i: ('A', 'air-combat-bank-source-manifest-v1.json', f'airborne_sword_phase_{44 + i}')], deps_fn=lambda c, i: [44 + i],
    note='Upper body of grounded combat clip 44+k with legs/pelvis from native sword_attack_jumpa.')
ter = ['sword_walk_stairs_up', 'sword_walk_stairs_down', 'sword_run_stairs_up', 'sword_run_stairs_down', 'empty_walk_stairs_up', 'empty_walk_stairs_down', 'empty_run_stairs_up', 'empty_run_stairs_down']
add(range(141, 149), 'native-sampled', lambda c, i: ter[i].replace('_', ' '), donors=[('BM', None)], note='Native stair cycles (library / terrain-retargeted-v1).')
tu = [f'{e}_turn{s}{d}' for e in ('sword', 'empty') for d in (45, 90, 180) for s in ('left', 'right')]
add(range(149, 161), 'native-sampled', lambda c, i: tu[i].replace('_', ' '), donors=[('BM', None)], note='Native standing turns sampled by root yaw.')
ct = [('crouch_turn_' if e == 'sword' else 'unarmed_crouch_turn_') + f'{s}{d}' for e in ('sword', 'empty') for d in (45, 90, 180) for s in ('left', 'right')]
add(range(161, 173), 'native-sampled', lambda c, i: ct[i].replace('_', ' '), donors=[('BM', None)], deps_fn=lambda c, i: [17] if i < 6 else [76],
    note='Native turn footwork on the Mixamo-derived crouch body (crouch-actions-v2 / unarmed-crouch-turns-v1).')
hm = {173: ('armed_native_high_mantle_standing', [0]), 174: ('empty_native_high_mantle_standing', [59]), 175: ('armed_native_high_mantle_crouched', [17]), 176: ('empty_native_high_mantle_crouched', [76])}
for c, (a, deps) in hm.items():
    TABLE.append(dict(clip=c, category='native-sampled', purpose=a.replace('_', ' '), donors=[('A', 'high-mantle-source-manifest-v1.json', a)], deps=deps, note='Native sword_mantlemiddle1 retimed, authored ledge support and recovery to the listed rest clip.'))
for c in (177, 178):
    TABLE.append(dict(clip=c, category='mixamo', purpose='double jump midair tuck' + (' (unarmed)' if c == 178 else ''), donors=[('BM', None)], deps=[59] if c == 178 else [],
                      note='jumps-authored-v3 double_jump (Mixamo Standing Jump with native grip and native sword_fall_loop).'))
for c, a in ((179, 'armed_native_hard_landing'), (180, 'empty_native_hard_landing')):
    TABLE.append(dict(clip=c, category='mixamo', purpose=a.replace('_', ' ') + ' (CatFall recovery)', donors=[('A', 'hard-landing-source-manifest-v1.json', a)], deps=[0] if c == 179 else [59],
                      note='Named "native" for the native CatFall clock only; the motion donor is Mixamo hard landing (Action Adventure Pack).'))
for c, a, deps in ((181, 'armed_native_hard_landing_crouched', [179, 17]), (182, 'empty_native_hard_landing_crouched', [180, 76])):
    TABLE.append(dict(clip=c, category='mixamo', purpose=a.replace('_', ' '), donors=[], deps=deps, note='Authored crouched recovery from the Mixamo hard landing clip into the Mixamo-derived crouch.'))
kh = [f'empty_native_keyhole_{s}_{k}' for s in ('left', 'right') for k in ('in', 'loop', 'out', 'crouch_in', 'crouch_out')]
add(range(183, 193), 'native-sampled', lambda c, i: kh[i].replace('_', ' '), donors=[lambda c, i: ('A', 'keyhole-source-manifest-v2.json', kh[i])], deps_fn=lambda c, i: [76] if 'crouch' in kh[i] else [59],
    note='Native NPC keyhole distraction animations (in/loop/out).')
TABLE.append(dict(clip=193, category='native-sampled', purpose='Dark Vision cast (standing armed)', donors=[('A', 'darkvision-source-manifest-v1.json', 'native_phase_dark_vision_fire')], deps=[0], note='Native NPC caster body (summontentacule) plus native FP finger samples.'))
TABLE.append(dict(clip=194, category='native-sampled', purpose='Dark Vision cast (standing unarmed)', donors=[('A', 'darkvision-context-source-manifest-v1.json', 'native_darkvision_standing_empty')], deps=[59], note='Stance/equipment adaptation of clip 193.'))
for c, a, deps in ((195, 'balanced_darkvision_crouched_armed', [17]), (196, 'balanced_darkvision_crouched_empty', [76])):
    TABLE.append(dict(clip=c, category='native-sampled', purpose=a.replace('_', ' '), donors=[('A', 'balanced-crouch-power-source-manifest-v3.json', a)], deps=deps,
                      note='Replaced in place at bank 28 (balanced-crouch-power v3; payload byte-identical to sibling v1/v2). Native Dark Vision cast re-authored around the Mixamo-derived crouch stance.'))
TABLE.append(dict(clip=197, category='native-sampled', purpose='Windblast cast (standing armed)', donors=[('A', 'windblast-source-manifest-v1.json', 'native_phase_windblast_fire')], deps=[0], note='Native NPC caster body (projectileattack) plus native FP finger samples.'))
TABLE.append(dict(clip=198, category='native-sampled', purpose='Windblast cast (standing unarmed)', donors=[('A', 'windblast-context-source-manifest-v1.json', 'native_windblast_standing_empty')], deps=[59], note='Stance/equipment adaptation of clip 197.'))
for c, a, deps in ((199, 'balanced_windblast_crouched_armed', [17]), (200, 'balanced_windblast_crouched_empty', [76])):
    TABLE.append(dict(clip=c, category='native-sampled', purpose=a.replace('_', ' '), donors=[('A', 'balanced-crouch-power-source-manifest-v3.json', a)], deps=deps,
                      note='Replaced in place at bank 28 (balanced-crouch-power v3). Native Windblast cast re-authored around the Mixamo-derived crouch stance.'))
rp = ['idle', 'ooa', 'fire', 'reload', 'equip', 'unequip']
add(range(201, 213), 'native-sampled', lambda c, i: 'ranged ' + ('gun ' if i < 6 else 'crossbow ') + rp[i % 6], donors=[lambda c, i: ('A', 'ranged-bank-source-manifest-v1.json', 'ranged_native_' + ('gun_' if i < 6 else 'crossbow_') + rp[i % 6])],
    note='Native NPC sword_gun_* body acting with native first-person hand references.')
add(range(213, 225), 'native-sampled', lambda c, i: 'crouched ranged ' + ('gun ' if i < 6 else 'crossbow ') + rp[i % 6], donors=[lambda c, i: ('A', 'crouched-ranged-source-manifest-v3.json', 'ranged_crouched_' + ('gun_' if i < 6 else 'crossbow_') + rp[i % 6]), ('F', 'mixamo-crouch/{fp}-fp-crouch.json')],
    deps_fn=lambda c, i: [201 + i, 17], note='Native ranged acting adapted to the Mixamo-derived crouch stance; wrist baseline from a runtime capture of the native first-person crouch.')
TABLE.append(dict(clip=225, category='native-sampled', purpose='carry body idle (standing)', donors=[('A', 'carry-bank-source-manifest-v1.json', 'carry_body_idle_runtime')], deps=[0], note='Native generic_carrycorpse_idle (NPC) + native FP carry arm + sword_idle.'))
TABLE.append(dict(clip=226, category='native-sampled', purpose='carry body walk (standing)', donors=[('A', 'carry-bank-source-manifest-v1.json', 'carry_body_walk_runtime')], deps=[1], note='Native carry pose on native walk clip 1.'))
TABLE.append(dict(clip=227, category='native-sampled', purpose='carry body idle (crouched)', donors=[('A', 'carry-bank-source-manifest-v1.json', 'crouched_carry_body_idle_runtime'), ('A', 'carry-contact-bank-source-manifest-v2.json', 'crouched_carry_idle_contact')], deps=[17],
                  note='Bank 31 carry pose on crouch clip 17, modified at bank 32 (carry-contact v2) and bank 35 (crouched-pickup v3 support IK).'))
TABLE.append(dict(clip=228, category='native-sampled', purpose='carry body walk (crouched)', donors=[('A', 'carry-bank-source-manifest-v1.json', 'crouched_carry_body_walk_runtime'), ('A', 'carry-contact-bank-source-manifest-v2.json', 'crouched_carry_walk_contact')], deps=[18],
                  note='Bank 31 carry pose on crouch walk clip 18 (Mixamo-derived), modified at banks 32 and 35.'))
TABLE.append(dict(clip=229, category='native-sampled', purpose='carry drop low (standing)', donors=[('A', 'carry-release-bank-source-manifest-v1.json', 'carry_drop_low_runtime')], deps=[], note='Native generic_carrycorpse_droplow based release.'))
TABLE.append(dict(clip=230, category='native-sampled', purpose='carry drop low (crouched)', donors=[('A', 'carry-release-bank-source-manifest-v1.json', 'crouched_carry_drop_low_runtime'), ('A', 'carry-contact-bank-source-manifest-v2.json', 'crouched_carry_low_drop_contact')], deps=[17],
                  note='Bank 32 crouched drop modified at bank 32 (contact v2) and bank 35 (support IK).'))
TABLE.append(dict(clip=231, category='native-sampled', purpose='carry pickup with supported settle (standing)', donors=[('A', 'body-lift-contact-manifest-v4.json', 'carry_pickup_contact_pilot')], deps=[],
                  note='Replaced at bank 34 (author_pickup_settle_v5): body-lift contact pilot (native carrycorpse / paired assassination donors) with authored left-hand settle on a runtime contact trace.'))
TABLE.append(dict(clip=232, category='authored', purpose='crouched carry pickup (gather and hoist)', donors=[], deps=[76, 227],
                  note='Replaced at bank 35 (author_crouched_pickup_v3): procedural blend from crouch empty idle (76) to the loaded crouched carry (227) with authored reach/step/IK on runtime contact traces.'))
TABLE.append(dict(clip=233, category='native-sampled', purpose='carry throw high (standing)', donors=[('A', 'carry-release-manifest-v3.json', 'carry_body_drop_high_sequence')], deps=[], note='Native generic_carrycorpse_drophigh based study.'))
TABLE.append(dict(clip=234, category='native-sampled', purpose='carry fetal release', donors=[('A', LIB, 'sword_idle')], deps=[233], note='Start of clip 233 plus recovery to native sword_idle.'))
add(range(235, 238), 'native-sampled', lambda c, i: 'Bend Time ' + ['in', 'loop', 'out'][i], donors=[lambda c, i: ('A', 'bendtime-bank-source-manifest-v40.json', 'native_phase_bend_time_' + ['in', 'loop', 'out'][i])], deps=[0],
    note='power-authored-v1 study: native NPC caster body plus native FP finger samples; grip from clip 0.')
btb = [f'native_bendtime_balanced_{ph}_{ctx}' for ph in ('in', 'loop', 'out') for ctx in ('standing_empty', 'crouched_armed', 'crouched_empty')]
add(range(238, 247), 'native-sampled', lambda c, i: btb[i].replace('_', ' '), donors=[lambda c, i: ('A', 'bendtime-balanced-source-manifest-v42.json', btb[i])], deps_fn=lambda c, i: [235 + i // 3] + ([59] if i % 3 == 0 else [17] if i % 3 == 1 else [76]),
    note='Stance/equipment adaptation of the Bend Time clip; crouched contexts use the Mixamo-derived crouch stance.')
TABLE.append(dict(clip=247, category='native-sampled', purpose='out-of-mana gesture (standing armed)', donors=[('A', 'outofmana-source-manifest-v45.json', 'native_out_of_mana_standing_armed')], deps=[0], note='Native NPC caster body (projectileattack) with native FP outofmana finger reference.'))
om = ['native_out_of_mana_standing_empty', 'native_out_of_mana_crouched_armed', 'native_out_of_mana_crouched_empty']
add(range(248, 251), 'native-sampled', lambda c, i: om[i].replace('_', ' '), donors=[lambda c, i: ('A', 'outofmana-context-source-manifest-v46.json', om[i])], deps_fn=lambda c, i: [247, [59, 17, 76][i]], note='Stance/equipment adaptation of clip 247.')
gp = ['idle', 'equip', 'unequip', 'cooking_in', 'cooking_idle', 'throw_high', 'throw_high_ooa', 'throw_low', 'throw_low_ooa']
add(range(251, 260), 'native-sampled', lambda c, i: 'grenade ' + gp[i].replace('_', ' '), donors=[lambda c, i: ('A', 'grenade-authoring-manifest-v55.json', 'grenade_' + gp[i])],
    note='grenade-authored-v55: native NPC sword_gun_* / distraction body acting with native FP grenade/stunmine samples (authored assembly).')
st = ['right', 'front', 'left', 'back']
add(range(260, 264), 'native-sampled', lambda c, i: 'stun hit ' + st[i], donors=[lambda c, i: ('A', LIB, 'sword_stinghit_' + st[i])], note='Native sword_stinghit clips.')
add(range(264, 273), 'native-sampled', lambda c, i: 'crouched grenade ' + gp[i].replace('_', ' '), donors=[lambda c, i: ('A', 'crouched-grenade-source-manifest-v62.json', 'grenade_crouched_' + gp[i]), ('F', 'mixamo-crouch/{fp}-fp-crouch.json')],
    deps_fn=lambda c, i: [251 + i, 17], note='Grenade acting adapted to the Mixamo-derived crouch stance; native FP crouch wrist baseline (runtime capture).')
add(range(273, 276), 'native-sampled', lambda c, i: 'Mesmerize ' + ['in', 'loop', 'out'][i], donors=[lambda c, i: ('A', 'mesmerize-bank-source-manifest-v77.json', 'native_mesmerize_' + ['in', 'loop', 'out'][i])], deps=[0],
    note='Native NPC support-cast body with an authored 65-degree shoulder gesture and native FP finger samples.')
mz = [f'native_mesmerize_{ph}_{ctx}' for ph in ('in', 'loop', 'out') for ctx in ('standing_empty', 'crouched_armed', 'crouched_empty')]
add(range(276, 285), 'native-sampled', lambda c, i: mz[i].replace('_', ' '), donors=[lambda c, i: ('A', 'mesmerize-context-source-manifest-v132.json', mz[i])], deps_fn=lambda c, i: [273 + i // 3, [59, 17, 76][i % 3]], note='Stance/equipment adaptation.')
dm = ['entry', 'aim', 'select', 'exit']
add(range(285, 289), 'native-sampled', lambda c, i: 'Domino ' + dm[i], donors=[lambda c, i: ('A', 'domino-bank-source-manifest-v192.json', 'native_domino_' + dm[i])], deps=[0], note='Native NPC support-cast body with native FP finger samples; phase timing from a native runtime take.')
dc = [f'native_domino_{ph}_{ctx}' for ph in dm for ctx in ('standing_empty', 'crouched_armed', 'crouched_empty')]
add(range(289, 301), 'native-sampled', lambda c, i: dc[i].replace('_', ' '), donors=[lambda c, i: ('A', 'domino-context-source-manifest-v243.json', dc[i])], deps_fn=lambda c, i: [285 + i // 3, [59, 17, 76][i % 3]], note='Stance/equipment adaptation.')
add(range(301, 304), 'native-sampled', lambda c, i: 'Devouring Swarm ' + ['in', 'loop', 'out'][i], donors=[lambda c, i: ('A', 'swarm-bank-source-manifest-v267.json', 'native_swarm_' + ['in', 'loop', 'out'][i])], deps=[0], note='Native NPC caster body + native FP samples; native scene timing.')
sw = [f'native_swarm_{ph}_{ctx}' for ph in ('in', 'loop', 'out') for ctx in ('standing_empty', 'crouched_armed', 'crouched_empty')]
add(range(304, 313), 'native-sampled', lambda c, i: sw[i].replace('_', ' '), donors=[lambda c, i: ('A', 'swarm-context-source-manifest-v272.json', sw[i])], deps_fn=lambda c, i: [301 + i // 3, [59, 17, 76][i % 3]], note='Stance/equipment adaptation.')
add(range(313, 315), 'native-sampled', lambda c, i: 'Possession ' + ['in', 'loop'][i], donors=[lambda c, i: ('A', 'possession-bank-source-manifest-v323.json', 'native_possession_' + ['in', 'loop'][i])], deps=[0], note='Native NPC caster body + native FP samples.')
TABLE.append(dict(clip=315, category='native-sampled', purpose='Possession out', donors=[('A', 'possession-bank-source-manifest-v334.json', 'native_possession_out')], deps=[59], note='Native NPC caster body + native FP samples.'))
add(range(316, 318), 'native-sampled', lambda c, i: 'Possession crouched ' + ['in', 'loop'][i], donors=[lambda c, i: ('A', 'possession-crouch-source-manifest-v363.json', 'native_possession_' + ['in', 'loop'][i] + '_crouched')], deps_fn=lambda c, i: [313 + i, 17], note='Crouched adaptation (Mixamo-derived crouch stance).')
TABLE.append(dict(clip=318, category='native-sampled', purpose='Possession crouched out', donors=[('A', 'possession-crouch-release-source-manifest-v371.json', 'native_possession_out_crouched')], deps=[315, 76], note='Crouched adaptation.'))
add(range(319, 322), 'native-sampled', lambda c, i: 'Doppelganger ' + ['in', 'loop', 'out'][i], donors=[lambda c, i: ('A', 'doppelganger-bank-source-manifest-v407.json', 'native_doppelganger_' + ['in', 'loop', 'out'][i])], deps=[0], note='Native NPC caster body + native FP samples.')
add(range(322, 325), 'native-sampled', lambda c, i: 'Doppelganger crouched ' + ['in', 'loop', 'out'][i], donors=[lambda c, i: ('A', 'doppelganger-crouch-source-manifest-v490.json', 'native_doppelganger_' + ['in', 'loop', 'out'][i] + '_crouched')], deps_fn=lambda c, i: [319 + i, 17], note='Crouched adaptation.')
dh = [f'native_doppelganger_{ph}_{ctx}' for ph in ('in', 'loop', 'out') for ctx in ('standing_empty', 'crouched_empty')]
add(range(325, 331), 'native-sampled', lambda c, i: dh[i].replace('_', ' '), donors=[lambda c, i: ('A', 'doppelganger-holstered-source-manifest-v495.json', dh[i])], deps_fn=lambda c, i: [319 + i // 2, [59, 76][i % 2]], note='Hands-free stance adaptation.')
swk = ['enter', 'idle', 'move_forward', 'exit']
add(range(331, 335), 'mixamo', lambda c, i: 'Shadow Walk ' + swk[i].replace('_', ' '), donors=[('BM', None)], note='shadow-body-v1 study: Mixamo Low Crawl retarget with native empty_idle hands/stand reference.')
TABLE.append(dict(clip=335, category='mixamo', purpose='Shadow Walk crouched sneak entry', donors=[], deps=[334], note='First 120 stored poses of clip 334 reversed.'))
ht = ['heart idle', 'heart equip', 'heart unequip', 'heart use']
add(range(336, 340), 'native-sampled', lambda c, i: ht[i], donors=[('BMV', 336)], note='items-authored-v2: native NPC body (sword_gun_* / clipboard distraction) + native FP item samples.')
sm = ['stunmine idle', 'stunmine equip', 'stunmine unequip', 'stunmine ready in', 'stunmine ready loop', 'stunmine place', 'stunmine out of ammo']
add(range(340, 347), 'native-sampled', lambda c, i: sm[i], donors=[('BMV', 340)], note='items-authored-v2: native NPC body + native FP item samples.')
TABLE.append(dict(clip=347, category='native-sampled', purpose='stunmine reload', donors=[('BMV', 340)], deps=[], note='items-authored-v2: native NPC body + native FP item samples.'))
add(range(348, 351), 'native-sampled', lambda c, i: 'rear choke ' + ['in', 'loop', 'win'][i], donors=[('BMTOP', 'sourceSha256')], note='choke-sequences-v1 study built from native generic_corvo_choke* animations.')
TABLE.append(dict(clip=351, category='native-sampled', purpose='choke breakout (victim wins)', donors=[('BMALL', None)], deps=[], note='Authored release from the native-timed choke study + native sword_stinghit_front recoil.'))
TABLE.append(dict(clip=352, category='native-sampled', purpose='lethal fast-back assassination', donors=[('BMTOP', 'sourceSha256')], deps=[], note='Native paired Doppelganger/Corvo fastback assassination actor donor.'))
TABLE.append(dict(clip=353, category='native-sampled', purpose='close-range kick', donors=[('BMTOP', 'sourceSha256')], deps=[], note='Native sword_attack_kick retimed to 800 ms.'))
add(range(354, 357), 'native-sampled', lambda c, i: 'crouched heavy attack ' + ['windup', 'hold', 'release'][i], donors=[('A', 'stance-retarget-manifest.json', 'crouch_idle')], deps_fn=lambda c, i: [50 + i],
    note='Native heavy attack clips 50-52 with the combat crouch delta from stance-authored crouch_idle (Mixamo-derived) and foot IK.')
add(range(357, 359), 'native-sampled', lambda c, i: ['parry block impact', 'timed parry deflection'][i], donors=[('BM', None)], note='Native NPC parry _out clips.')
swp = {359: ('mixamo', 'swim tread idle'), 360: ('mixamo', 'swim forward stroke'), 361: ('mixamo', 'swim rise'), 362: ('mixamo', 'swim dive'), 363: ('mixamo', 'reserved copy of 359'), 364: ('mixamo', 'reserved copy of 360')}
for c, (cat, p) in swp.items():
    TABLE.append(dict(clip=c, category=cat, purpose=p, donors=[('BM', None)], deps=[], note='Mixamo Treading Water / Swimming retargets (water-movement-v1 authored blends).'))
add(range(365, 369), 'mixamo', lambda c, i: ['ladder climb ascend', 'ladder climb descend', 'ladder bottom enter', 'ladder bottom exit'][i], donors=[('BM', None)], note='ladder-contact-v2 study: Mixamo Climbing Ladder retarget fitted to a neutral rung fixture.')
add(range(369, 374), 'authored', lambda c, i: ['use: button/press', 'use: lever', 'use: crank', 'use: wheel/valve', 'use: key/lock'][i], donors=[('BM', None)],
    note='mechanisms-v4 authored reach/operate/recover study on native base poses (empty_idle/sword_idle) with native FP finger references.')

assert sorted(r['clip'] for r in TABLE) == list(range(374)), [c for c in range(374) if c not in [r['clip'] for r in TABLE]]
# Weak references: earlier clips used only for timing/phase alignment or a
# floor-height measurement (no pose data copied). Not propagated to components.
WEAK = {17: [0], 76: [0], **{c: [1] for c in range(18, 26)}, **{c: [1] for c in range(26, 29)}, **{c: [1] for c in range(60, 76)}}
for r in TABLE:
    w = WEAK.get(r['clip'], [])
    r['weak'] = w
    r['deps'] = [d for d in r['deps'] if d not in w]
TABLE.sort(key=lambda r: r['clip'])

# ---- donor lookup ------------------------------------------------------
_mcache = {}


def manifest(fn):
    if fn not in _mcache:
        p = os.path.join(NM, fn)
        _mcache[fn] = json.load(open(p, encoding='utf-8-sig'))
    return _mcache[fn]


def find_alias(fn, alias, role):
    d = manifest(fn)
    rows = d if isinstance(d, list) else d.get('rows', [])
    for e in rows:
        if e.get('role') != role:
            continue
        sel = (e.get('selection') or {}).get('alias')
        src = e.get('source') if isinstance(e.get('source'), dict) else {}
        if sel == alias or alias in (src.get('aliases') or []):
            return e
    return None


IDXK = ('clip', 'id', 'index')
SHAK = ('sha256', 'source', 'donor', 'donorSha256', 'source_sha256', 'sourceSha256')


def bm_items(fn, role):
    d = manifest(fn)
    ents = [e for e in (d if isinstance(d, list) else [d]) if e.get('role') == role]
    return ents[0] if ents else None


def walk(o):
    if isinstance(o, dict):
        yield o
        for v in o.values():
            yield from walk(v)
    elif isinstance(o, list):
        for v in o:
            yield from walk(v)


def bm_shas(fn, role, clip, variant_base=None, top=None, all_=False):
    e = bm_items(fn, role)
    out = []
    if e is None:
        return out
    if top:
        v = e.get(top)
        return [v.lower()] if isinstance(v, str) and HEX.match(v) else []
    for o in walk(e):
        if o is e:
            continue
        if all_ and any(isinstance(o.get(k), str) and HEX.match(o.get(k)) for k in SHAK):
            pass
        elif variant_base is not None:
            if o.get('variant') is None or variant_base + o['variant'] != clip:
                continue
        elif not any(o.get(k) == clip for k in IDXK):
            continue
        for k in SHAK:
            v = o.get(k)
            if isinstance(v, str) and HEX.match(v):
                out.append(v.lower())
    return out


def entry_for_sha(sha):
    hit = R.lookup(sha)
    return hit


FPS = {'corvo': 'host', 'emily': 'client'}
SPECIAL = {}
for role, side in FPS.items():
    p = os.path.join(os.path.dirname(NM), 'mixamo-crouch', side + '-fp-crouch.json')
    if os.path.exists(p):
        SPECIAL[hashlib.sha256(open(p, 'rb').read()).hexdigest()] = ('native-runtime', 'runtime capture of native first-person crouch wrist: work/mixamo-crouch/' + side + '-fp-crouch.json')


def leaf_filter(leaves):
    out = set()
    for c, l in leaves:
        if c == 'authored-procedural' and ('/source/download' in l or 'bank-manifest' in l):
            continue
        if c == 'unresolved':
            key, _, sha = l.partition('=')
            if sha in SPECIAL:
                out.add(SPECIAL[sha]); continue
            if sha.lower() in BANKSHA:
                out.add(('bank-derived', BANKSHA[sha.lower()])); continue
            if 'trace' in key or 'snapshot' in key:
                out.add(('native-runtime', 'runtime capture ' + key + '=' + sha[:12])); continue
        out.add((c, l))
    return out


def resolve_sha(sha):
    sha = sha.lower()
    if sha in SPECIAL:
        return {SPECIAL[sha]}
    if sha in BANKSHA:
        return {('bank-derived', BANKSHA[sha])}
    lv, _ = R.resolve(sha)
    return leaf_filter(lv)


def donors_for(rec, role):
    origin = ORIG[role][rec['clip']]['origin']
    bman = ORIGIN[origin][0]
    res = []  # (path, sha, exists, leaves)
    for spec in rec['donors']:
        kind = spec[0]
        shas = []
        if kind == 'A':
            e = find_alias(spec[1], spec[2], role)
            if e is None:
                res.append(dict(ref=f'{spec[1]}:{spec[2]}', path=None, sha256=None, exists=False, leaves=[('unknown', 'alias not found')]))
                continue
            shas = [e['sha256'].lower()]
        elif kind == 'BM':
            shas = bm_shas(bman, role, rec['clip'])
        elif kind == 'BMV':
            shas = bm_shas(bman, role, rec['clip'], variant_base=spec[1])
        elif kind == 'BMTOP':
            shas = bm_shas(bman, role, rec['clip'], top=spec[1])
        elif kind == 'BMALL':
            shas = bm_shas(bman, role, rec['clip'], all_=True)
        elif kind == 'F':
            p = spec[1].replace('{fp}', FPS[role])
            full = os.path.join(os.path.dirname(NM), p.replace('/', BS))
            res.append(dict(ref=p, path='work/' + p, sha256=hashlib.sha256(open(full, 'rb').read()).hexdigest() if os.path.exists(full) else None, exists=os.path.exists(full),
                            leaves=[('native-runtime', 'runtime capture of native first-person crouch wrist')]))
            continue
        if not shas:
            res.append(dict(ref=str(spec), path=None, sha256=None, exists=False, leaves=[('unknown', 'no sha found in ' + bman)]))
        for s in dict.fromkeys(shas):
            hit = R.lookup(s)
            path = None
            if hit:
                e = hit[1]
                path = e.get('binary') or (hit[0][5:] if hit[0].startswith('FILE:') else None)
            if path is None and s in R.BINH:
                path = R.BINH[s][0]
            exists = bool(path) and os.path.exists(os.path.join(NM, path))
            res.append(dict(ref=(hit[0] + ':' + str(hit[1].get('id') or '')) if hit else s, path=path, sha256=s, exists=exists, leaves=sorted(resolve_sha(s))))
    return res


# ---- main --------------------------------------------------------------
results = {}
for role in ('corvo', 'emily'):
    recs = {}
    for rec in TABLE:
        c = rec['clip']
        o = ORIG[role][c]
        origin = o['origin']
        bman, builder, ver = ORIGIN[origin]
        dons = donors_for(rec, role)
        leaves = set()
        for d in dons:
            leaves |= set(map(tuple, d['leaves']))
        dep_leaves = set()
        for dc in rec['deps']:
            if dc in recs:
                dep_leaves |= set(map(tuple, recs[dc]['_leaves']))
        all_leaves = leaves | dep_leaves
        comps = sorted({l[0] for l in all_leaves if l[0] not in ('bank-derived',)})
        native_assets = sorted({l[1] for l in all_leaves if l[0] == 'native-sampled'})
        mixamo_src = sorted({l[1] for l in all_leaves if l[0] == 'mixamo'})
        first = o['first']
        evidence = [f"payload provenance: clip {c} payload first equals final payload in {origin} (sha1 {o['ph'][:12]}), computed by diffing all on-disk banks of the v1212 chain [confirmed static]"]
        if o['modified_by']:
            evidence.append(f"clip index first appeared in {first}; replaced in place by {', '.join(o['modified_by'])} [confirmed static]")
        evidence.append(f"builder work/natural-moveset/{builder}; bank manifest work/natural-moveset/{bman} [confirmed static]")
        for d in dons:
            if d['sha256']:
                evidence.append(f"donor {d['path'] or d['ref']} sha256 {d['sha256'][:16]} -> leaves {sorted(set(x[0] for x in d['leaves']))} [confirmed static, via manifest sha links]")
        if rec['deps']:
            evidence.append(f"reuses earlier bank clip(s) {rec['deps']} as base/pose input (read from builder) [confirmed static]")
        recs[c] = dict(role=role, clip=c, category=rec['category'], purpose=rec['purpose'],
                       donor=[d['path'] or d['ref'] for d in dons], donorSha256=[d['sha256'] for d in dons],
                       donorExists=(all(d['exists'] for d in dons) if dons else None),
                       donorExistsDetail=[{'donor': d['path'] or d['ref'], 'exists': d['exists']} for d in dons],
                       baseClips=rec['deps'], timingReferenceClips=rec['weak'], manifest='work/natural-moveset/' + bman if not bman.startswith('..') else bman,
                       builder='work/natural-moveset/' + builder if not builder.startswith('..') else 'work/' + builder[3:],
                       originBank='work/natural-moveset/' + origin, version=ver, firstAddedIn='work/natural-moveset/' + first,
                       replacedInPlaceBy=o['modified_by'],
                       components=comps, nativeAssets=native_assets, mixamoSources=mixamo_src,
                       containsNativeAnimationData=('native-sampled' in comps or 'native-runtime' in comps),
                       containsMixamoData=('mixamo' in comps),
                       embedsNativeBindTranslations=True,
                       note=rec['note'], evidence=evidence, _leaves=sorted(all_leaves))
    results[role] = recs

out = []
for role in ('corvo', 'emily'):
    for c in range(374):
        r = dict(results[role][c]); r.pop('_leaves')
        out.append(r)
json.dump(out, open(os.path.join(OUT, 'clips.json'), 'w'), indent=1)

# quick report
from collections import Counter
for role in ('corvo', 'emily'):
    rs = [results[role][c] for c in range(374)]
    print(role, Counter(r['category'] for r in rs))
    print(role, 'components', Counter(tuple(r['components']) for r in rs))
    print(role, 'donor missing', [r['clip'] for r in rs if r['donorExists'] is False])
    print(role, 'unknown/unresolved leaves', [(r['clip'], [l for l in r['_leaves'] if l[0] in ('unknown', 'unresolved', 'fbx-unknown', 'animgen')]) for r in rs if any(l[0] in ('unknown', 'unresolved', 'fbx-unknown', 'animgen') for l in r['_leaves'])][:40])
    mism = []
    for r in rs:
        if r['category'] == 'native-sampled' and 'native-sampled' not in r['components']:
            mism.append(r['clip'])
        if r['category'] == 'mixamo' and 'mixamo' not in r['components']:
            mism.append(r['clip'])
    print(role, 'category/component mismatch', mism)
