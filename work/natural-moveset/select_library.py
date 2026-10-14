"""Curated actor motions. Excludes victim clips, unspecified additives and spell victims."""
import json,re
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
rows=read(P/'native-expanded-sample-manifest.json')['rows']
requests={}
def want(family,aliases,purpose):
 for a in aliases.split():requests[a]=dict(family=family,purpose=purpose)
want('locomotion','empty_idle sword_idle sword_patrol_idle empty_walk_n empty_run_n sword_walk_n sword_run_n','Native whole-body locomotion')
want('locomotion',' '.join(f'sword_{g}_{d}_add' for g in ['walk','run'] for d in 'nesw'),'Directional whole-body gait reconstructed from the verified native neutral-relative representation')
want('locomotion',' '.join(f'sword_{g}_{a}' for g in ['walk','run'] for a in ['startleft','startright','stopleft','stopright','bank_left','bank_right','slopes_up','slopes_down','stairs_up','stairs_down']),'Gait starts, stops and terrain variants')
want('locomotion',' '.join(f'sword_{g}_{d}_{a}' for g in ['walk','run'] for d in 'esw' for a in ['startleft','startright','stopleft','stopright']),'Directional start or stop; only existing sources selected')
want('locomotion',' '.join(f'sword_{prefix}{side}{angle}' for prefix in ['turn','turntowalk','turntorun'] for side in ['left','right'] for angle in [45,90,135,180]),'Authored pivot with root rotation retained as authoring metadata')
want('locomotion',' '.join(f'sword_run_turn{side}180_{foot}' for side in ['left','right'] for foot in ['left','right']),'Moving reversal')
want('traversal',' '.join(f'sword_{x}' for x in ['fall_loop','dropdown_walkleft_in','dropdown_walkright_in','land_walk_out','landfail_walk_out','mantle_walk_out','mantle_vault_walk_out','vaultraise_walk_out','vaultthrough_walk_out','vaultthrough_fall','vaultthrough_fail_walk_out']),'Whole-body traversal donor; collision and obstacle alignment require separate validation')
want('traversal',' '.join(f'sword_{x}_walk{foot}_in' for x in ['mantlelow','mantlemiddle1','mantlemiddle2','mantlemiddle3','mantlehigh','vaultraise','vaultthrough'] for foot in ['left','right']),'Traversal entry variants')
want('combat',' '.join(f'sword_{x}' for x in ['equipsword','unequipsword','attack_fasta','attack_fastb','attack_shorta','attack_shortb','attack_shortc','attack_shortd','attack_shorte','attack_longa','attack_longb','attack_jumpa','attack_kick','attack_punch','attack_stepbacka','attack_left_90a','attack_left_180a','attack_right_90a','attack_right_180a']),'Full-body sword actions; visual events stripped and outcomes remain network-owned')
want('combat',' '.join(f'sword_parry_{d}_{a}' for d in ['ll','lr','rl','rr'] for a in ['in','out','attack']),'Parry entry/recovery/counter attack; incoming attack timing remains separate')
want('combat',' '.join(f'sword_{x}_{d}' for x in ['stinghit','parry_lose'] for d in ['front','back','left','right']),'Hit and guard-break reaction')
want('combat','sword_recovery_fast_back sword_recovery_fast_front sword_recovery_fast_left sword_recovery_a_idle','Knockdown recovery donor')
want('ranged_and_items',' '.join(f'sword_gun_{x}' for x in ['in','idle','fire','reload','out','equip_fast','unequip','snapshot_front','snapshot_back','snapshot_left','snapshot_right']),'Native pistol body motion; native held weapon binding remains separate')
want('ranged_and_items','empty_idlebreak_drink_flask sword_throwrock_floor','Drink and throw whole-body inspiration; actual elixir/grenade contact adaptation pending')
want('paired_actions','sword_doppelganger_assassination_fastback sword_doppelganger_assassination_fastfront','Confirmed Doppelganger attacker assets; victim alignment and contact require paired review')
want('interactions',' '.join(f'sword_walk_door_open_{rotation}_{foot}' for rotation in ['cw','ccw'] for foot in ['left','right']),'Door opening whole-body motion; door contact alignment pending')
want('powers','sword_teleportation_front sword_teleportation_back sword_teleportation_left sword_teleportation_right sword_power_projectileattack sword_power_support_in sword_power_support_loop sword_power_support_out sword_power_summontentacule','Caster motion donor only; not a finished player power animation')
sources={};missing=[]
for alias,request in requests.items():
 candidates=[r for r in rows if alias in r['aliases'] and r['status']=='native_pose_sampled_offline']
 if not candidates:missing.append(alias);continue
 for role in ['corvo','emily']:
  candidates.sort(key=lambda r:(r['source_role']!=role,'/high/' not in r['asset'],r['asset']))
  r=candidates[0];entry=sources.setdefault(r['id'],dict(alias=alias,**request,targets=[]))
  if role not in entry['targets']:entry['targets'].append(role)
result=dict(scope='Curated native body sources, not complete gameplay coverage or accepted animations',sources=sources,missing_requested_aliases=missing)
(P/'library-source-selection.json').write_text(json.dumps(result,indent=2));print('SELECTED',len(sources),'sources',sum(len(r['targets']) for r in sources.values()),'target clips; unavailable aliases',len(missing))
