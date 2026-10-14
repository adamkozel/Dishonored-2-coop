"""Index actual gameplay captures without granting whole-action acceptance."""
import json, hashlib, subprocess, sys
from pathlib import Path
P=Path(__file__).resolve().parent
manifest=P/'runtime-video-manifest.json'
rows=json.loads(manifest.read_text(encoding='utf-8-sig'))
sprint='--sprint' in sys.argv
lean='--lean' in sys.argv
air='--air' in sys.argv
combat='--combat' in sys.argv
slide_jump='--slide-jump' in sys.argv
slide='--slide' in sys.argv or slide_jump
assert sum((sprint,lean,air,slide,combat))<=1
version=5 if air else 3 if slide else 2 if sprint else 1
family='combat' if combat else 'slide-jump' if slide_jump else 'slide' if slide else 'air' if air else 'lean' if lean else 'sprint' if sprint else 'stance'
for role in ('corvo','emily'):
    name=f'integrated-{family}-{role}-v{version}'
    take=json.loads((P/name/'take.json').read_text(encoding='utf-8-sig'))
    assert not take['failure']
    video=P/f'{name}.mp4'
    stream=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=nb_frames,duration,avg_frame_rate,width,height','-of','json',str(video)]))['streams'][0]
    # Capture timestamps are real and variable-rate. Require useful measured
    # cadence, rather than a frame count that assumes ideal 30 Hz capture.
    assert int(stream['nb_frames'])/float(stream['duration'])>=24 and float(stream['duration'])>=(30.9 if combat else 13.9 if slide else 25.9 if air else 37.9)
    row=dict(video=video.name,character=role,label=f'{role.title()} · Live {"standing/crouched lean" if lean else "sprinting, crouching" if sprint else "walking, crouching"} and recovery',
        actual_capture_hz=int(stream['nb_frames'])/float(stream['duration']),container_fps=stream['avg_frame_rate'],
        timing='Real-time Windows Graphics Capture timestamps; no speedup or output frame duplication.',
        kind='game',travelling=True,gameplay_bound=True,complete=False,visual_accepted=False,
        scope=take['scope'],chapters=take['chapters'],dll_sha256=take['dll'],bank_sha256=take['bank'],
        sha256=hashlib.sha256(video.read_bytes()).hexdigest(),capture=stream,evidence=f'{name}/take.json',
        requirements=['armed_idle','walk_8_directions','run_8_directions','crouch_idle','crouch_8_directions',
          'stand_to_crouch','crouch_to_stand','crouch_stop_start','reverse_and_direction_changes','gait_speed_changes'],
        tests=['stance-bank-v1-test-2/result.json','stance-bank-v1-priority/result.json'],
        limitations=['Partial coverage per linked requirement; the video is not acceptance of every variant.',
          'Dark Dunwall room; close-up weapon grip, terrain contacts and arbitrary interruptions need further review.',
          'Attack, block and jump show existing native presentation priority, not completion of those action families.',
          'Persistent test opt-in retires on pause/load; automatic session lifecycle integration remains pending.'])
    if sprint:
        row['requirements']=['sprint_forward_and_diagonals','start_stop_each_gait','gait_speed_changes','reverse_and_direction_changes','stand_to_crouch','crouch_to_stand','crouch_stop_start']
        row['tests']=['sprint-bank-v1-underrun/result.json','stance-bank-v4-regression-2/result.json','stance-bank-v4-priority/result.json']
        row['scope']='Actual native sprint/standing/crouch inputs and Steam semantic state select the whole-body movement bank. Steps follow network-controlled travel. Root timestamps and gradual buffer recovery prevent arrival bursts from compressing movement time. Native block/slash presentation takes priority. Complete movement, terrain, equipment and effects remain unfinished.'
        row['tests'].append('lean-bank-v1-test/result.json')
        row['limitations'].append('Wide view retains sprint travel; initial body is near the right edge and local sword. Later crouch is partly occluded by native furniture.')
    if lean:
        row['requirements']=['stand_lean_left_right','crouch_lean_left_right']
        row['tests']=['lean-bank-v1-test/result.json']
        row['scope']='Exact native Leaning state and read-only horizontal lean amplitude travel over Steam and select authored whole-body balance shifts. Native movement releases the lean. Local first-person camera and controls remain untouched.'
        row['limitations']=['Partial Dunwall coverage, no general visual acceptance.', 'Vertical peeking, collision-limited lean, sword interruptions and simultaneous direction reversals need further runtime coverage.', 'Dark room limits close-up hand and equipment inspection.']
    if slide:
        row['label']=f'{role.title()} · Native slide '+('into authored jump' if slide_jump else 'and crouched recovery')
        row['requirements']=['slide_enter_loop_exit','slide_to_crouch_or_stand']
        row['tests']=['slide-bank-v4-semantic/result.json']
        row['scope']='Exact native Sliding state selects authored full-body entry, hold and recovery. World placement remains controlled by replicated native movement. '+('Native jump interrupts the slide into the height-driven authored air bank.' if slide_jump else 'Native slide retirement selects crouched recovery, then native standing input returns to standing.')
        row['limitations']=['One isolated Dunwall trial per role; no general visual acceptance.','Low ceilings, slopes, attack interruptions, paired victims and arbitrary collision-dependent exits remain unverified.','Dark room limits equipment and contact inspection.']
    if air:
        row['label']=f'{role.title()} · Standing, held, crouched and moving jumps'
        row['requirements']=['standing_jump_takeoff','running_jump_takeoff','crouch_jump_transition','rising_airborne','fall_loop','soft_landing','moving_landing']
        row['tests']=['air-bank-v4-semantic/result.json','air-bank-v2-priority/result.json']
        row['scope']='Read-only native Jumping/Falling states and native start/apex heights control the remote full-body air bank. Rising pose follows replicated root height. Native ground contact starts authored standing/running/crouched landing recovery; local first-person physics and controls remain unchanged.'
        row['limitations']=['Ordinary Dunwall jumps only; no complete family or visual acceptance.','Double jump, high falls, hard landings, slopes, airborne attacks and arbitrary interruptions remain unverified.','Moving takeoff uses observed speed blend; a short walking start does not certify maximum-speed running jumps.']
    if combat:
        row['label']=f'{role.title()} · Native-timed sword combos, heavy attack and guards'
        row['travelling']=False
        row['requirements']=['light_attack_variants','heavy_attack_variants','crouched_attack','block_enter_hold_exit']
        row['tests']=['combat-bank-v1-test/result.json']
        row['scope']=take['scope']
        row['limitations']=['Both-role isolated native-input and Steam timing checks pass; no complete combat-family or visual acceptance.', 'Short/alternating slashes, one heavy windup/hold/release, one standing/crouched guard quadrant. Other directional parries, counters, victim contacts and hit reactions are unfinished.', 'Airborne combat and moving-foot contact remain unfinished. Heavy hold currently retains a static authored windup pose.', 'Complete body remains visible, but this camera distance does not establish fine grip/contact acceptance.']
    for test in row['tests']:
        assert json.loads((P/test).read_text(encoding='utf-8-sig'))['passed'],test
    rows=[x for x in rows if x['video']!=video.name]+[row]
manifest.write_text(json.dumps(rows,indent=2),encoding='utf-8')
print(f'Indexed {len(rows)} game recordings; complete remains false')
