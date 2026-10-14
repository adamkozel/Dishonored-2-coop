import json,datetime,hashlib,math
from pathlib import Path
P=Path(__file__).resolve().parent
coverage=json.loads((P/'coverage.json').read_text());items={i['id']:i for i in coverage['items']}
for name in ('armed_idle','sword_ready_idle','walk_8_directions'):
 items[name].update(authoring='two_native_rig_candidates',runtime_binding='both_roles_flat_Dunwall_verified',visual_validation='recorded_candidate_polish_remaining',
  evidence=['walk-test-3/result.json','corvo-walking.mp4','emily-walking.mp4'],notes='D2MOVE01 bank: sword-ready idle and eight single-cycle walks; constant native bone lengths, native helpers and rig-specific grips. Distance controls gait phase. Experimental 60-second lease with native Steam action eligibility; not a production moveset.')
for name in ('start_stop_each_gait','reverse_and_direction_changes'):
 items[name].update(authoring='walking_blends_only',runtime_binding='walking_only_both_roles',visual_validation='needs_polish',notes='Walking idle/cycle and adjacent-direction interpolation implemented. No dedicated authored start, brake, pivot or running transitions; turns and foot planting need refinement.')
for name in ('run_8_directions','sprint_forward_and_diagonals'):
 items[name].update(authoring='donors_generated_but_not_accepted_as_gait',notes='Both pretrained sample controllers clamp controls to their training distributions. Forward requests of 3 and 5 m/s produce about 2.1 m/s on Character, 1.95 m/s on NPC. Need suitable running/sprinting source motion or broader training; these are not accepted running/sprint clips.')
items['turn_in_place_90_180'].update(authoring='root_feedback_donors_only',notes='Fixed generator control-root feedback. Four turn requests now end within 0.3 degrees of their requested heading. Not retargeted or played in D2; original open-loop turn donors are rejected.')
for name in ('block_enter_hold_exit','light_attack_variants','crouched_attack'):
 items[name].update(runtime_binding='existing_native_action_priority_verified',notes='Real standing/crouched Slash and Block retain native motion identity. Generated walking yields to existing native action playback. Does not cover every attack/parry variant or validate new combat outcomes.',evidence=['priority-2/result.json'])
for name in ('standing_jump_takeoff','rising_airborne','fall_loop'):
 items[name].update(runtime_binding='existing_airborne_priority_verified',notes='Native Jump/Fall suppresses walking bank. No newly authored jumping/falling sequence is claimed.',evidence=['priority-2/result.json'])
coverage['updated_at']=datetime.datetime.now().astimezone().isoformat();(P/'coverage.json').write_text(json.dumps(coverage,indent=2))
turns=[]
for file in (P/'donors-feedback').glob('turn_*.json'):
 d=json.loads(file.read_text());q=d['frames'][-1]['feedback_rotation'];yaw=math.degrees(math.atan2(2*q[3]*q[2],1-2*q[2]*q[2]));expected=-d['request']['turn']
 turns.append({'name':file.stem,'requested_unreal_yaw':expected,'actual_unreal_yaw':yaw,'wrapped_error_degrees':abs((yaw-expected+180)%360-180),'d2_playback':False})
(P/'turn-feedback-results.json').write_text(json.dumps(turns,indent=2))
manifest=json.loads((P/'retarget-manifest.json').read_text())
state=json.loads((P.parent/'current-task-state.json').read_text(encoding='utf-8-sig'))
state['UpdatedAt']=coverage['updated_at'];state['LoaderSha256']=hashlib.sha256((P.parent.parent/'outputs/d2-coop-lab/bin/dinput8.dll').read_bytes()).hexdigest().upper()
state['Live']=[]
for role in ('host','client'):
 base=P.parent/'lab-instances'/role;s=json.loads((base/'coop-status.json').read_text());launch=json.loads((base/'launch.json').read_text(encoding='utf-8-sig'))
 state['Live'].append({'Role':role,'Pid':launch['pid'],'Epoch':s['localEpoch'],'Handle':s['probeActorHandle'],'Follow':s['probeFollowEnabled'],
  'NativeInterface':s['nativeInterface'],'BodyStream':s.get('bodyStreamEnabled'),'AuthoringProof':json.loads((base/'coop-status.json.authoring.json').read_text()),
  'MovementBank':json.loads((base/'coop-status.json.movement.json').read_text()),'GameLogic':s['localGameLogicState'],'RemoteFresh':s['remoteFresh']})
state['MovesetExpansion']={'scope':'Full remote moveset work in progress; first candidate bank covers armed idle + eight walking directions on both native rigs',
 'complete':False,'default_enabled':False,'nonrenewable_lease_seconds':60,'coverage':'work/moveset-growth/coverage.json','functional_items':len(items),
 'source_aliases':coverage['reference_aliases'],'native_first_person_changed':False,'network':'Existing native Steam root and action streams',
 'banks':[{k:m[k] for k in ('role','sha256','layout_hash','bytes','joints')} for m in manifest],
 'tests':{'direction_checks':18,'priority_checks':13,'bank_validation':'Both actual banks plus malformed inputs and phase math; full existing build regression suite passed'},
 'videos':['work/moveset-growth/corvo-walking.mp4','work/moveset-growth/emily-walking.mp4'],
 'turn_feedback':turns,'remaining':'Dedicated transitions/foot polish, running/sprinting sources, turn runtime, crouch/lean, traversal, combat variants, ranged/items, paired actions, powers and interactions',
 'previous_milestone':'work/animgen-feasibility/milestone-092E','ghidra_note':'Dishonored2.exe 0x140540a90 post comment saved'}
(P.parent/'current-task-state.json').write_text(json.dumps(state,indent=2))
print(json.dumps(state['MovesetExpansion'],indent=2))
