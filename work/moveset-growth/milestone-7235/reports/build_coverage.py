"""Create complete source-alias inventory plus actionable moveset coverage.
An alias is a reference channel, not automatically a complete-body animation.
"""
import collections,datetime,hashlib,json,re
from pathlib import Path
P=Path(__file__).resolve().parent
rows=json.loads((P/'native-set-exports.json').read_text(encoding='utf-8-sig'))
rules=[('powers',r'power|blink|windblast|possess|timeshift|shadowgrapple|phantasmal|domino|doppel|mesmer'),
 ('paired_actions',r'assass|takedown|choke|fatality|behead|versus'),('carry_throw',r'carrycorpse|carry|droplow'),
 ('ranged_and_items',r'crossbow|gun|reload|grenade|stunmine|springrazor|heart|potion|health|mana'),
 ('traversal',r'jump|fall|land|climb|ledge|vault|window|ladder|swim|water|slide|cable|rope'),
 ('stance',r'sneak|crouch|lean|keyhole|peek'),('combat',r'sword|attack|parry|block|dodge|hit|stagger'),
 ('locomotion',r'walk|run|sprint|idle|turn'),('interactions',r'door|lever|button|pickup|loot|switch|lock|equip|reach|gesture|sit|eat|drink')]
references=[]
for row in rows:
    text=Path(row['ExportFile']).read_text()
    for alias,asset in re.findall(r'alias\s*\{\s*name\s+"([^"]+)"\s+anim\s+"([^"]+)"',text):
        family=next((f for f,p in rules if re.search(p,alias,re.I)),'other_or_scripted')
        references.append({'set':row['Name'],'alias':alias,'asset':asset,'family':family,'status':'reference_only'})
families={
 'locomotion':['stand_idle','armed_idle','walk_8_directions','run_8_directions','sprint_forward_and_diagonals','start_stop_each_gait','gait_speed_changes','turn_in_place_90_180','moving_turns','reverse_and_direction_changes','slopes_and_stairs'],
 'stance':['crouch_idle','crouch_8_directions','stand_to_crouch','crouch_to_stand','crouched_turns','crouch_stop_start','stand_lean_left_right','crouch_lean_left_right','ledge_peek','keyhole_enter_look_exit'],
 'traversal':['standing_jump_takeoff','running_jump_takeoff','crouch_jump_transition','double_jump','rising_airborne','fall_loop','soft_landing','hard_landing','moving_landing','vault_low_obstacle','climb_high_ledge','ledge_hang_and_pull_up','window_enter_exit','slide_enter_loop_exit','slide_to_crouch_or_stand','ladder_up_down_entry_exit','chain_rope_climb','swim_idle_directions','dive_surface_water_exit'],
 'combat':['sword_draw_holster','sword_ready_idle','light_attack_variants','heavy_attack_variants','moving_attack','crouched_attack','airborne_attack','block_enter_hold_exit','parry_directions','parry_recovery','counterattack','hit_reactions_directions','stagger_knockdown_recovery','kick_or_shove_if_present','death_variants','ragdoll_handoff'],
 'ranged_and_items':['pistol_draw_aim_fire_reload_holster','crossbow_draw_aim_fire_reload_holster','bolt_or_ammo_switch','grenade_equip_aim_throw','mine_or_razor_place','heart_equip_use_holster','health_elixir','mana_elixir','item_switch_interruptions'],
 'paired_actions':['lethal_takedown_front_back_sides','nonlethal_takedown_front_back_sides','crouched_takedown','aerial_drop_kill','aerial_nonlethal_takedown','sliding_takedown','wall_and_window_variants','enemy_body_size_variants','paired_victim_alignment','struggle_and_escape','carry_body_lift_idle_move_drop_throw'],
 'powers':['corvo_blink_aim_cancel_release_arrive','corvo_blink_pull_and_kick_variants','corvo_dark_vision_cast_end','corvo_possession_enter_loop_exit','corvo_bend_time_cast_end','corvo_windblast_cast','corvo_devouring_swarm_cast','emily_far_reach_aim_cancel_release_arrive','emily_far_reach_pull_variants','emily_dark_vision_cast_end','emily_domino_mark_link_cancel','emily_doppelganger_cast','emily_mesmerize_cast','emily_shadow_walk_enter_move_attack_exit','power_upgrade_variants','power_interruptions_and_no_mana'],
 'interactions':['door_open_close_locked','lever_button_crank_wheel','pickup_loot_steal','key_use','read_object_or_note','eat_drink','carry_object_place_throw','shop_dialogue_gestures','sit_stand','scripted_scene_pose_handoff','cutscene_enter_exit','map_load_checkpoint_recovery','incapacitation_and_rescue_if_supported'],
}
coverage=[]
previous={v['id']:v for v in json.loads((P/'coverage.json').read_text())['items']} if (P/'coverage.json').exists() else {}
for family,actions in families.items():
    candidates=sorted({r['asset'] for r in references if r['family']==family})
    for action in actions:
        characters=['corvo'] if action.startswith('corvo_') else ['emily'] if action.startswith('emily_') else ['corvo','emily']
        coverage.append({'id':action,'family':family,'characters':characters,
          'authoring':'not_started','runtime_binding':'not_verified','visual_validation':'not_verified',
          'native_reference_family_count':len(candidates),'priority':1 if family in ('locomotion','stance') else 2 if family in ('traversal','combat') else 3})
        # Rebuilding the source index must not erase accumulated runtime evidence.
        for key,value in previous.get(action,{}).items():
            if key not in ('id','family','characters','native_reference_family_count','priority'):coverage[-1][key]=value
report={'at':datetime.datetime.now().astimezone().isoformat(),'source_sets':len(rows),
 'reference_aliases':len({r['alias'] for r in references}),'reference_channel_entries':len(references),
 'game_executable_sha256':'B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180',
 'scope':'Broad functional coverage plus every alias in the 12 exported base/Corvo/Emily body, left/right arm and camera sets. Situational variants are retained in references; availability is not runtime coverage.',
 'source_files':[{'name':r['Name'],'sha256':hashlib.sha256(Path(r['ExportFile']).read_bytes()).hexdigest()} for r in rows],
 'items':coverage}
(P/'coverage.json').write_text(json.dumps(report,indent=2))
(P/'native-reference-index.json').write_text(json.dumps(references,indent=2))
summary=['D2 COMPLETE REMOTE MOVESET COVERAGE','',report['scope'],'',f"{len(coverage)} functional work items; {report['reference_aliases']} distinct source aliases.",
 'Reference availability, authored clips, runtime binding and visual acceptance are separate states.','',
 'Existing 092E Corvo proof remains preserved; its forward/guard/recovery result is not full family coverage.','']
for family,actions in families.items():
    summary.append(family.upper())
    for action in actions:
        row=next(v for v in coverage if v['id']==action)
        summary.append(f"  {action}: authored={row['authoring']}; runtime={row['runtime_binding']}; visual={row['visual_validation']}")
        if row.get('notes'):summary.append('    '+row['notes'])
    summary.append('')
(P/'COVERAGE.txt').write_text('\n'.join(summary)+'\n')
print(json.dumps({'families':len(families),'work_items':len(coverage),'unique_aliases':report['reference_aliases'],'reference_entries':len(references)}))
