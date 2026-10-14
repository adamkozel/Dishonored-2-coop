"""Build an honest, reproducible review index; source aliases are not coverage."""
import json,re,hashlib,datetime,functools
from pathlib import Path
from render_identity import data_identity
P=Path(__file__).resolve().parent
@functools.lru_cache(maxsize=None)
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
sources=['library-retarget-manifest.json','interaction-retarget-manifest.json','mixamo-retarget-manifest-v5.json','gait-review-manifest.json','stance-retarget-manifest.json','campaign-retarget-manifest.json','lean-retarget-manifest.json','crouch-directions-retarget-manifest.json','dynamics-retarget-manifest-v3.json','power-retarget-manifest-v1.json','crouch-actions-retarget-manifest-v2.json']
folders=['library-videos-v3','interaction-videos-v1','mixamo-videos-v5','gait-videos-v1','stance-videos-v1','campaign-videos-v2','lean-videos-v2','crouch-directions-videos-v1','dynamics-videos-v1','dynamics-videos-v3','power-videos-v1','crouch-actions-videos-v2']
sources.append('items-retarget-manifest-v2.json');folders.append('items-videos-v2')
sources.append('paired-native-manifest-v1.json');folders.append('paired-native-videos-v1')
sources.append('jumps-retarget-manifest-v3.json');folders.append('jumps-videos-v3')
sources.append('choke-contact-manifest-v2.json');folders.append('choke-contact-videos-v2')
sources.append('choke-sequences-manifest-v1.json');folders.append('choke-sequences-videos-v1')
sources.append('carry-body-manifest-v3.json');folders.append('carry-body-videos-v3')
sources.append('mechanisms-manifest-v4.json');folders.append('mechanisms-videos-v4')
sources.append('object-carry-manifest-v2.json');folders.extend(['object-carry-videos-v1','object-carry-videos-v2'])
sources.append('guard-sequences-manifest-v1.json');folders.append('guard-sequences-videos-v1')
sources.append('shadow-body-manifest-v1.json');folders.append('shadow-body-videos-v1')
sources.append('choke-approaches-manifest-v1.json');folders.extend(['choke-approaches-pilot-v1','choke-approaches-videos-v1'])
sources.append('item-transitions-manifest-v1.json');folders.append('item-transitions-videos-v1')
sources.append('crouched-pairs-manifest-v1.json');folders.append('crouched-pairs-videos-v1')
sources.append('ladder-contact-manifest-v2.json');folders.append('ladder-contact-videos-v2')
sources.append('water-movement-manifest-v1.json');folders.append('water-movement-videos-v1')
sources.append('drop-pairs-manifest-v3.json');folders.append('drop-pairs-videos-v3')
sources.append('blink-assault-manifest-v1.json');folders.append('blink-assault-videos-v1')
sources.append('slide-pair-manifest-v4.json');folders.append('slide-pair-videos-v4')
sources.append('window-pair-manifest-v5.json');folders.append('window-pair-videos-v5-labelled')
sources.append('ammo-changes-manifest-v1.json');folders.append('ammo-changes-videos-v1')
sources.append('window-choke-manifest-v2.json');folders.append('window-choke-videos-v2')
sources.append('top-transitions-manifest-v1.json');folders.append('top-transitions-videos-v1')
sources.append('water-exit-manifest-v3.json');folders.append('water-exit-videos-v3')
sources.append('trap-surface-manifest-v1.json');folders.append('trap-surface-videos-v1')
sources.append('carry-release-manifest-v3.json');folders.append('carry-release-videos-v3')
sources.append('ladder-top-entry-manifest-v1.json');folders.append('ladder-top-entry-videos-v1')
sources.append('body-lift-manifest-v2.json');folders.append('body-lift-videos-v2')
sources.append('body-throw-manifest-v1.json');folders.append('body-throw-videos-v1')
motions={};pairs={}
for filename in sources:
 if not (P/filename).exists():continue
 for row in read(P/filename):
  alias=row['selection']['alias'];m=motions.setdefault(alias,dict(id=alias,label=alias.replace('_',' '),family=row['selection']['family'],roles={},source_manifest=filename,notes=[],videos=[],visual_accepted=False))
  pairs.setdefault(alias,{})[row['role']]=row
  m['roles'][row['role']]=dict(id=row['id'],sha256=row['sha256'],duration=row['duration'],fps=row['fps'],binary=row['binary'])
  purpose=row['selection'].get('purpose','')
  if purpose and purpose not in m['notes']:m['notes'].append(purpose)
  m['equipment']=row.get('equipment_policy','native source equipment; prop binding may remain incomplete')
for folder in folders:
 for file in (P/folder).glob('*.json'):
  r=read(file)
  if not isinstance(r,dict) or 'alias' not in r or r['alias'] not in motions:continue
  # Only recordings made from the exact current pair belong in the review.
  expected=hashlib.sha256(''.join(motions[r['alias']]['roles'][role]['sha256']for role in ['corvo','emily']).encode()).hexdigest()
  if r.get('identity')!=expected:continue
  if r.get('render_inputs',{}).get('data_identity',data_identity(pairs[r['alias']]))!=data_identity(pairs[r['alias']]):continue
  if r.get('paired_inputs') and r['paired_inputs']!={role:row['partner']for role,row in pairs[r['alias']].items()}:continue
  video=P/r['video']
  if video.is_file():motions[r['alias']]['videos'].append(dict(path=r['video'].replace('\\','/'),kind='authoring',label='Corvo + Emily · authoring preview',character_order='Emily left, Corvo right'if pairs[r['alias']]['corvo'].get('preview_view',[1])[0]<0 else'Corvo left, Emily right',fps=r['fps'],duration=r['duration'],render_identity=r.get('render_identity'),model_variant=r.get('model_variant'),cloth=r.get('cloth')))
# Explicit functional mapping. Broad family matches never mark a requirement done.
patterns={
 'stand_idle':r'^empty_idle$|^mixamo_idle[2345]?$',
 'armed_idle':r'^sword_idle$|^armed_idle$',
 'walk_8_directions':r'^walk_|^sword_walk_[nesw]_add$',
 'run_8_directions':r'^run_|^sword_run_[nesw]_add$',
 'sprint_forward_and_diagonals':r'^sprint_forward',
 'start_stop_each_gait':r'^sword_(walk|run).*?(start|stop)|^mixamo_run_to_stop$',
 'gait_speed_changes':r'^gait_sequence$',
 'turn_in_place_90_180':r'^sword_turn(left|right)(90|180)$|^mixamo_(left|right)_turn$',
 'moving_turns':r'^sword_turnto(walk|run)',
 'reverse_and_direction_changes':r'^sword_run_turn|^walk_.*(left|right|back)|^run_.*(left|right|back)',
 'slopes_and_stairs':r'^sword_(walk|run)_(slopes|stairs)',
 'crouch_idle':r'^crouch_idle$',
 'crouch_8_directions':r'^crouch_walk_|^crouch_forward$|^mixamo_crouched_walking',
 'stand_to_crouch':r'^stand_to_crouch$',
 'crouch_to_stand':r'^crouch_to_stand$',
 'crouched_turns':r'^crouch_turn',
 'crouch_stop_start':r'^crouch_(start|stop|sequence)',
 'stand_lean_left_right':r'^stand_lean_',
 'crouch_lean_left_right':r'^crouch_lean_',
 'ledge_peek':r'^ledge_peek$',
 'keyhole_enter_look_exit':r'^empty_distraction_look_keyhole_',
 'standing_jump_takeoff':r'^mixamo_jumping_up$|^standing_jump_sequence$',
 'running_jump_takeoff':r'^running_jump_sequence$',
 'crouch_jump_transition':r'^crouch_jump_',
 'double_jump':r'^double_jump$',
 'rising_airborne':r'^mixamo_jumping_up$|^standing_jump_sequence$',
 'fall_loop':r'^sword_fall_loop$|^mixamo_falling_idle$',
 'soft_landing':r'^sword_land_walk_out$',
 'hard_landing':r'^mixamo_hard_landing$|^sword_landfail_walk_out$',
 'moving_landing':r'^mixamo_falling_to_roll$|^sword_land_walk_out$',
 'vault_low_obstacle':r'^sword_(vault|vaultraise|mantle_vault)',
 'climb_high_ledge':r'^sword_mantle(high|middle)|^mixamo_freehang_climb$',
 'ledge_hang_and_pull_up':r'^mixamo_freehang_climb$',
 'window_enter_exit':r'^sword_vaultthrough',
 'slide_enter_loop_exit':r'^slide_',
 'slide_to_crouch_or_stand':r'^slide_to_|^slide_exit_to_',
 'ladder_up_down_entry_exit':r'^mixamo_(climbing_ladder|start_climbing_ladder|climbing_to_top)$|^ladder_',
 'chain_rope_climb':r'^mixamo_climbing_a_rope$',
 'swim_idle_directions':r'^mixamo_(swimming|treading_water)$|^water_',
 'dive_surface_water_exit':r'^mixamo_swimming_to_edge$|^water_(dive|surface|ledge)_',
 'sword_draw_holster':r'^sword_(equipsword|unequipsword)$',
 'sword_ready_idle':r'^sword_idle$|^armed_idle$',
 'light_attack_variants':r'^sword_attack_(fast|short)',
 'heavy_attack_variants':r'^sword_attack_long',
 'moving_attack':r'^sword_attack_(left|right|stepback)',
 'crouched_attack':r'^crouch_attack_',
 'airborne_attack':r'^sword_attack_jumpa$',
 'block_enter_hold_exit':r'^sword_parry_.*_(in|out)$|^guard_.*_hold_sequence$',
 'parry_directions':r'^sword_parry_.*_in$',
 'parry_recovery':r'^sword_parry_.*_out$',
 'counterattack':r'^sword_parry_.*_attack$|^guard_.*_counter_sequence$',
 'hit_reactions_directions':r'^sword_stinghit_',
 'stagger_knockdown_recovery':r'^sword_(recovery|parry_lose)',
 'kick_or_shove_if_present':r'^sword_attack_(kick|punch)$',
 'death_variants':r'^generic_deathfast_|^empty_stinghit_death$',
 'pistol_draw_aim_fire_reload_holster':r'^sword_gun_',
 'crossbow_draw_aim_fire_reload_holster':r'^crossbow_',
 'bolt_or_ammo_switch':r'^crossbow_ammo_',
 'heart_equip_use_holster':r'^heart_',
 'mine_or_razor_place':r'^stunmine_|^springrazor_',
 'grenade_equip_aim_throw':r'^sword_grenade_',
 'health_elixir':r'^empty_idlebreak_drink_flask$',
 'mana_elixir':r'^empty_idlebreak_drink_flask$',
 'item_switch_interruptions':r'^item_switch_|^(crossbow|heart|stunmine)_equip_cancel_',
 'crouched_takedown':r'^crouched_(paired|nonlethal)_',
 'aerial_drop_kill':r'^aerial_lethal_back_study$',
 'aerial_nonlethal_takedown':r'^aerial_nonlethal_back_study$',
 'sliding_takedown':r'^sliding_nonlethal_back_study$',
 'wall_and_window_variants':r'^window_rear_lethal_study$|^window_nonlethal_',
 'lethal_takedown_front_back_sides':r'^sword_doppelganger_assassination_fast|^paired_fast_',
 'paired_victim_alignment':r'^paired_fast_',
 'enemy_body_size_variants':r'^paired_fast_',
 'nonlethal_takedown_front_back_sides':r'^paired_nonlethal_hold$|^nonlethal_(back|front|left|right)_',
 'struggle_and_escape':r'^nonlethal_(back|front|left|right)_(hold|release)_sequence$',
 'carry_body_lift_idle_move_drop_throw':r'^carry_body_',
 'lever_button_crank_wheel':r'^interact_(lever|button|crank|wheel)_sequence$',
 'key_use':r'^interact_key_sequence$',
 'carry_object_place_throw':r'^carry_object_',
 'corvo_blink_aim_cancel_release_arrive':r'^sword_teleportation_|^power_blink_',
 'corvo_blink_pull_and_kick_variants':r'^blink_assault_',
 'power_upgrade_variants':r'^blink_assault_',
 'corvo_dark_vision_cast_end':r'^power_dark_vision_',
 'corvo_possession_enter_loop_exit':r'^power_possession_',
 'corvo_bend_time_cast_end':r'^power_bend_time_',
 'corvo_windblast_cast':r'^power_windblast_',
 'corvo_devouring_swarm_cast':r'^power_devouring_swarm_',
 'emily_far_reach_aim_cancel_release_arrive':r'^power_far_reach_',
 'emily_far_reach_pull_variants':r'^power_far_reach_pull',
 'emily_dark_vision_cast_end':r'^power_dark_vision_',
 'emily_domino_mark_link_cancel':r'^power_domino_',
 'emily_doppelganger_cast':r'^power_doppelganger_',
 'emily_mesmerize_cast':r'^power_mesmerize_',
 'emily_shadow_walk_enter_move_attack_exit':r'^shadow_walk_',
 'power_interruptions_and_no_mana':r'^power_.*_cancel$',
 'door_open_close_locked':r'^sword_walk_door_open_',
 'pickup_loot_steal':r'^empty_distraction_loot_ground_',
 'read_object_or_note':r'^activity_.*read_',
 'shop_dialogue_gestures':r'^gesture_|^posture_talkcenter$|^empty_distraction_clipboard_loop_talk$',
 'eat_drink':r'^empty_idlebreak_drink_flask$|^empty_distraction_eat_apple_|^activity_drink_standing_',
 'sit_stand':r'^empty_distraction_sitting_bench_',
}
caveats={
 'bolt_or_ammo_switch':'Regular/sleep-bolt changes are recorded in both directions with native bolt models and exact native attachment tags. The lower/holster/equip choreography is authored, not a confirmed native FP ammo-selection gesture. Inventory state, selection timing, additional ammunition and live equipment binding remain pending.',
 'wall_and_window_variants':'Rear window lethal and nonlethal entry/hold/finish/release studies. Native victim timing accompanies authored full-body stepping and pulling. Neutral sill clearance was checked visually. Fingers, blade contact, native window geometry, ragdoll continuation and live paired control remain unverified. Wall and other window directions remain missing.',
 'carry_body_lift_idle_move_drop_throw':'Floor lift, loaded idle/walk, low/high/soft release and shoulder throw studies are recorded with a grey native target. Lift v2 improves knee direction and recovery stepping but still needs actor/target collision and weight-transfer refinement. Drops stop at their exact native ragdoll markers. Throw flight is illustrative and freezes at floor contact. Fine grip, target settling, equipment transitions and actual NPC attachment/physics ownership remain incomplete.',
 'lever_button_crank_wheel':'Reach, operation and recovery studies with full-body weight transfer and neutral contact fixtures. Palm grip is fitted independently from the wrist. Exact native target geometry, fingertip contact, continuous wheel regripping and game-state binding remain pending.',
 'key_use':'Reach, insert/turn and recovery study using a neutral key fixture. Exact lock geometry, key inventory/visibility and gameplay timing remain pending.',
 'carry_object_place_throw':'Recorded pickup, idle, walking, placement and throw studies with two palm contacts and a neutral box. Placement reverses the lift study; lowering dynamics need refinement. The release trajectory is illustrative, with no native object attachment or physics ownership.',
 'crouch_jump_transition':'Complete-body takeoff/landing candidates with a separated offline jump trajectory. Pose publication passes in both roles; native jump/capsule binding, weapon transition and contact acceptance are pending.',
 'double_jump':'Authored supernatural second impulse within a complete jump sequence. Height/timing are demonstration values, not measured native physics. Both-role pose publication passes; gameplay binding remains pending.',
 'nonlethal_takedown_front_back_sides':'Rear/front/left/right entry, held struggle, finish and release studies. Front/side alignment is inferred from native victim endpoints and is not engine-verified. Initial grabs, precise hand/head contact and native ragdoll handoff remain incomplete; the baked victim finish ends before a full fall.',
 'struggle_and_escape':'Held struggle and release previews use native victim phases. Cancel ends before complete victim recovery; escape outcomes, interruptions and runtime pairing remain unbound.',
 'crossbow_draw_aim_fire_reload_holster':'Native crossbow models and firing mechanisms follow full-body item studies. Normal reload mechanism timing, precise grip/contact, ammunition switching and runtime prop binding remain incomplete.',
 'heart_equip_use_holster':'Native Heart with full-body carrying/gesture studies and hand-local FP references. Native use mechanism is shown; exact grip, investigation targeting and runtime prop binding remain pending.',
 'mine_or_razor_place':'Native stun-mine and spring-razor models accompany held-item sequences and wall reach/press/release studies. Exact native asset data confirms shared placement scenes and the 236 ms fire marker in the 300 ms phase. Surface attachment is offline only; fine fingers, blade clearance, live trap spawning and outcomes remain unverified.',
 'paired_victim_alignment':'Native front/back attacker and matching victim clips are shown together at original speed. Initial placement uses the victim auxiliary anchor as a partner frame; contact looks plausible in the pilot, but production alignment and runtime authority are unverified.',
 'enemy_body_size_variants':'Tall and female native-rig stand-ins only. These are neutral-grey review targets, not the co-op partner. Small, armored, nonhuman and contextual variants remain missing.',
 'running_jump_takeoff':'Authored running-jump candidate with vertical travel separated into offline metadata. Capsule mapping, airborne timing and collision-aware landing remain unbound.',
 'block_enter_hold_exit':'Connected native parry, sustained full-body guard and recovery sequences are recorded for four native quadrant codes. Arbitrary interruption, impact timing and game-state binding remain pending.',
 'ladder_up_down_entry_exit':'Rung-contact ascent, descent, bottom connections, top exit and top descent entry are recorded with neutral fixtures. Descent reverses ascent; bottom exit reverses entry; top entry reverses the top climb more slowly. Exact native rung geometry, fine palm/toe contact, downward weight transfer and gameplay binding remain pending.',
 'swim_idle_directions':'Forward, backward, left, right, up/down and start/stop body studies are recorded. Back/side movement uses upright sculling; propulsion fit and visual acceptance remain pending. Water surface, buoyancy, capsule and native movement binding are not demonstrated.',
 'dive_surface_water_exit':'Dive, surface, swimming-to-edge hold and ledge exit to standing are recorded with separated demonstration travel. The new exit uses obstacle-relative palms and boot clearance; native water physics, surface geometry, capsule motion and gameplay binding remain unverified.',
 'item_switch_interruptions':'Three native item switches and equip cancellation at 30/70 percent are recorded, with at most one left-hand item visible. Arbitrary-time interruption, inventory state and runtime prop binding remain pending.',
 'crouched_takedown':'Crouched approach rises into rear lethal and nonlethal paired actions. This is not a takedown performed entirely at crouch height. Initial contact, terrain and outcome ownership still need work.',
 'aerial_drop_kill':'Revision required: actor/target overlap during impact, sword contact and recovery foot clearance need work. Native victim playback stops at the exact ragdoll event; subsequent physics is NOT simulated. No live paired outcome is implemented.',
 'aerial_nonlethal_takedown':'Revision required: initial body collision and weight transfer need work. Native victim playback stops at the exact ragdoll event; subsequent physics is NOT simulated. No live paired outcome is implemented.',
 'sliding_takedown':'Rear nonlethal prototype only. Revision required: initial sweep/body overlap, recovery and Corvo hand contact (up to 4cm residual) need work. Emily wrist targets fit within 0.3mm, which does not establish acceptable hand/mesh contact. Targets hold their native ragdoll-time pose; physics and live pairing are absent.',
 'lethal_takedown_front_back_sides':'Native front/back Doppelganger attacker and matching tall/female victim previews. Side approaches, exact production alignment and runtime damage/ragdoll ownership remain incomplete.',
 'grenade_equip_aim_throw':'Native grenade preparation and throw donors. Held grenade contact, player equipment choice and release timing remain unbound.',
 'health_elixir':'Drinking donor only. Correct bottle, hand contact and gameplay timing remain missing.',
 'mana_elixir':'Drinking donor only. Correct bottle, hand contact and gameplay timing remain missing.',
 'crouch_8_directions':'Eight full-body directions are authored from forward/back/left/right sources, with shared foot phase and contact-fitted cadence. Diagonal foot contact and consistency need visual acceptance; production crouch binding is pending.',
 'moving_landing':'A roll/landing donor is not proof of collision-aware moving landings.',
 'map_load_checkpoint_recovery':'This is a lifecycle integration requirement, not a standalone animation clip.',
 'ragdoll_handoff':'This requires native ragdoll ownership and a tested transition, not a baked animation alone.',
 'scripted_scene_pose_handoff':'A context-dependent animation ownership transition. Requires a tested blend from the actual scene pose, rather than one universal clip.',
 'cutscene_enter_exit':'Native cutscene animation remains scene-specific. Entry/exit ownership and blends must be tested against real scenes; one generic clip cannot cover them.',
 'incapacitation_and_rescue_if_supported':'No separate rescue moveset is planned under the requested shared-death policy: either death resets both players from the same checkpoint. Keep this original conditional requirement visible without inventing a rescue animation.',
}
items=read(P.parent/'moveset-growth/coverage.json')['items'];assert len(items)==105
for item in items:
 key=item['id'];item['title']=key.replace('_',' ');pat=patterns.get(key)
 item['motions']=[k for k in motions if pat and re.search(pat,k)]
 item['authoring']='candidates' if item['motions'] else 'missing'
 item['runtime_binding']='not_verified';item['visual_validation']='not_accepted'
 item['deliverable_kind']='lifecycle_transition'if key in ['ragdoll_handoff','scripted_scene_pose_handoff','cutscene_enter_exit','map_load_checkpoint_recovery']else'conditional_not_planned'if key=='incapacitation_and_rescue_if_supported'else'animation'
 item['notes']=caveats.get(key,'Candidates require animation review and appropriate game-state/contact binding.' if item['motions'] else 'No new authored candidate recorded yet.')
 if item['family']=='powers' and item['motions']:
  item['notes']='Full-body cast studies with native caster acting and D2 first-person phase timing/finger articulation. Wrists follow the body freely. Effects, target contact, character availability, gameplay binding and visual acceptance remain pending.'
  if key=='power_interruptions_and_no_mana':item['notes']='Authored full-body cancel-to-stance candidates only. No-mana failure and interruption at arbitrary action phases remain missing.'
  if key=='emily_far_reach_pull_variants':item['notes']+=' Pull gesture only; object/enemy attachment, weight, trajectories and variants are not demonstrated.'
  if key=='corvo_blink_pull_and_kick_variants':
   item['title']='Corvo Blink Assault kick and slash';item['notes']='Exact native assets confirm Blink Assault. Full-body kick and slash sequences are recorded. No Corvo pulling variant was confirmed; pulling remains under Emily Far Reach. Teleport/target timing, contact, knockdown and runtime binding are pending. Emily is a comparison adaptation.'
  if key=='power_upgrade_variants':item['notes']='Blink Assault kick/slash body studies only. Other upgrades, transformed bodies, effects and action/state variants are not covered by these clips.'
  if key=='corvo_possession_enter_loop_exit':item['notes']+=' Transformation, controlled-body handoff and reappearance are not demonstrated.'
  if key=='emily_shadow_walk_enter_move_attack_exit':item['notes']='Human-rig low-body movement studies only, using a pinned in-place Low Crawl donor. Entry/exit, idle, forward motion and a low lunge are recorded. Native shadow morphology/effects, attack contact, wall/rathole travel, native-speed calibration and runtime power binding are missing. Exit reverses entry; attack support contacts need refinement. Corvo is a comparison adaptation.'
 if key in ['armed_idle','walk_8_directions','run_8_directions','sword_ready_idle','gait_speed_changes']:
  item['runtime_binding']='isolated_Dunwall_both_roles';item['notes']='D2MOVE02 passes both-role directional, distance-phase and action-priority tests. Visual acceptance and production integration are pending.'
 if key in ['standing_jump_takeoff','rising_airborne','fall_loop','light_attack_variants','crouched_attack','block_enter_hold_exit']:
  item['runtime_priority']='Existing native action suppresses the walking/running bank; this does not validate new donor gameplay binding.'
 for k in item['motions']:motions[k].setdefault('requirements',[]).append(key)
for m in motions.values():
 m.setdefault('requirements',[])
 issues={
  'carry_body_floor_lift_study':'Revision required: during the 1.5–2.3 second gather, actor/head and target-body overlap and weight transfer need refinement. Hand-target residuals do not prove acceptable surface contact.',
  'carry_body_throw_study':'Revision required: strengthen release anticipation/follow-through and refine hand fit. Target flight is illustrative and deliberately freezes at first floor contact; physical settling is absent.',
  'aerial_lethal_back_study':'Revision required: impact overlap, blade contact and recovery foot clearance remain unresolved.',
  'aerial_nonlethal_back_study':'Revision required: impact collision and weight transfer remain unresolved.',
  'sliding_nonlethal_back_study':'Revision required: sweep/body overlap, recovery and Corvo hand contact remain unresolved.'}
 m['review_status']='revision_required'if m['id']in issues else'awaiting_visual_review'
 if m['id']in issues:m['notes'].append(issues[m['id']])
 m['identity']=hashlib.sha256(''.join(m['roles'][r]['sha256']for r in sorted(m['roles'])).encode()).hexdigest()
 m['complete_movement']=False
 m['pose_publication_evidence']=[]
 for take,report in [('body-lift-v2-pose-test-1','body-lift-v2-roundtrip.json'),('body-throw-v1-pose-test-1','body-throw-v1-roundtrip.json'),('trap-surface-v1-pose-test-1','trap-surface-v1-roundtrip.json'),('carry-release-v3-pose-test-1','carry-release-v3-roundtrip.json'),('ladder-top-entry-v1-pose-test-1','ladder-top-entry-v1-roundtrip.json')]:
  if not(P/take/'result.json').exists()or not(P/report).exists():continue
  result=read(P/take/'result.json');verified=read(P/report)
  if not result['passed'] or not verified['passed']:continue
  for case in result['cases']:
   role=m['roles'].get(case['role'])
   if role and case['bankSha'].lower()==role['sha256'].lower():m['pose_publication_evidence'].append(dict(role=case['role'],take=take,report=report,scope='Isolated D2 pose publication, cancellation and expiry; no gameplay binding or visual acceptance'))
 for take,report in [('window-choke-v2-pose-test-2','window-choke-v2-roundtrip.json'),('top-transition-v1-pose-test-1','top-transition-v1-roundtrip.json'),('water-exit-v3-pose-test-1','water-exit-v3-roundtrip.json'),('ammo-changes-v1-pose-test-1','ammo-changes-v1-roundtrip.json'),('window-v5-pose-test-1','window-v5-roundtrip.json'),('slide-v4-pose-test-1','slide-v4-roundtrip.json'),('blink-assault-v1-pose-test-1','blink-assault-v1-roundtrip.json'),('item-transitions-v1-pose-test-1','item-transitions-v1-roundtrip.json'),('ladder-v2-pose-test-1','ladder-v2-roundtrip.json'),('water-v1-pose-test-1','water-v1-roundtrip.json'),('contextual-v1-pose-test-1','contextual-v1-roundtrip.json'),('crouched-pairs-v1-pose-test-1','crouched-pairs-v1-roundtrip.json'),('shadow-v1-pose-test-1','shadow-v1-roundtrip.json'),('guard-v1-pose-test-1','guard-v1-roundtrip.json'),('mechanisms-v4-pose-test-1','mechanisms-v4-roundtrip.json'),('carry-v3-pose-test-1','carry-v3-roundtrip.json'),('object-carry-v1-pose-test-1','object-carry-v1-roundtrip.json'),('jumps-v3-pose-test-1','jumps-v3-roundtrip.json'),('items-v2-pose-test-1','items-v2-roundtrip.json'),('powers-pose-test-1','powers-roundtrip-v1.json'),('crouch-actions-v2-pose-test-1','crouch-actions-v2-roundtrip.json'),('armed-sprint-v3-pose-test-1','armed-sprint-v3-roundtrip.json'),('stance-pose-test-1','stance-roundtrip.json'),('mixamo-v5-pose-test','mixamo-v5-roundtrip.json')]:
  if not(P/take/'result.json').exists()or not(P/report).exists():continue
  result=read(P/take/'result.json');verified=read(P/report)
  if not result['passed'] or not verified['passed']:continue
  for case in result['cases']:
   role=m['roles'].get(case['role'])
   if role and case['bankSha'].lower()==role['sha256'].lower():m['pose_publication_evidence'].append(dict(role=case['role'],take=take,report=report,scope='Isolated D2 pose publication, cancellation and expiry; no gameplay binding or visual acceptance'))
recordings=[]
for r in read(P/'runtime-video-manifest.json'):
 if (P/r['video']).exists():
  label=r['character'].title()+' · '+('Steam-driven walk/run/directions'if r['video'].startswith('gait-')else 'Native crouch sequence · pose-only'if r.get('alias')=='crouch_sequence'else 'Ten full-body power studies · pose-only'if r.get('alias')=='power_cast_showcase'else 'Blink Assault kick body study · pose-only'if r.get('alias')=='blink_assault_kick_sequence'else 'D2 turn/run/attack · pose-only')
  if r.get('alias')=='springrazor_wall_place_sequence':label=r['character'].title()+' · Wall-placement body motion · pose-only'
  recordings.append(dict(**r,label=label,kind='game',travelling=r['video'].startswith('gait-')))
report=dict(generated=datetime.datetime.now().astimezone().isoformat(),complete=False,scope='Work-in-progress full-body review library. Every original functional requirement remains listed; candidate source clips do not count as a complete moveset.',first_person_constraints=False,items=items,motions=list(motions.values()),game_recordings=recordings,rejected=['library-videos','library-videos-v2','mixamo-videos-v1','mixamo-videos-v2','mixamo-videos-v3','mixamo-videos-v4','campaign-videos-v1','lean-videos-v1','gait-corvo-video-1','gait-corvo-video-2','stance-emily-video-1'])
(P/'review-catalog.json').write_text(json.dumps(report,indent=2));(P/'review-catalog.js').write_text('window.REVIEW_CATALOG='+json.dumps(report,separators=(',',':'))+';\n')
print('CATALOG',len(items),'requirements',len(motions),'paired-role candidates',sum(bool(m['videos'])for m in motions.values()),'recorded',sum(bool(i['motions'])for i in items),'requirements have candidates')


