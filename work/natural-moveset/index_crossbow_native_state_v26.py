"""Publish bounded, visually inspected crossbow evidence; never mark complete."""
import datetime, hashlib, json, subprocess
from pathlib import Path
P = Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
manifest = read(P/'runtime-video-manifest.json')
annotations = read(P/'review-evidence-annotations.json')
banks = {x['role']:x for x in read(P/'ranged-bank-manifest-v1.json')}
dll = '0E70FBC248D0B9FA406D95B8A2916CB8CD2743CD1714031CE189E469F5C1ACE3'
tests, videos = [], []
limitations = [
    'Standing base crossbow, isolated two-round fixture only.',
    'Native source graph reports no output and source rig snapshots stay at bind pose; exact mechanical pose equivalence remains unproven.',
    'Wide shots establish attachment continuity, not final finger contact or natural reload quality.',
    'Remote projectiles, sounds, upgrades, weapon switching, moving/crouched/airborne contexts and interruption coverage remain unfinished.',
    'No audio captured; full movement phase remains incomplete.'
]
for role in ('corvo', 'emily'):
    take = f'ranged-{role}-crossbow-native-state-v26-v5'
    result = read(P/take/'result.json')
    assert result['passed'] and result['dll'] == dll and len(result['checks']) == 9
    sheets = [f'{take}/contact.jpg', f'{take}/source-contact.jpg']
    assert all((P/x).is_file() for x in sheets)
    review = dict(inspected=True, accepted=False, method='Paired actual-game temporal contact sheets inspected.', contact_sheets=sheets,
        observations=['Source native cocked/fired state is replicated through Steam, including held empty state after the shot scene ends.',
                      'Exact native crossbow skeleton used for the cosmetic remote prop; attachment remains with the left hand in the inspected frames.',
                      'Reload hand path and full-body naturalness still need closer review.'], limitations=limitations)
    write(P/take/'visual-review.json', review)
    case = []
    for suffix in ('', '-source'):
        movie = P/f'{take}{suffix}.mp4'
        capture = json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
        hz = int(capture['nb_frames']) / float(capture['duration'])
        assert hz >= 24
        row = dict(video=movie.name, character=role, label=take.replace('-',' ')+(' · native source' if suffix else ' · remote body'), kind='game',
            gameplay_bound=True, travelling=False, complete=False, visual_accepted=False, dll_sha256=dll, bank_sha256=banks[role]['sha256'],
            sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(), actual_capture_hz=hz, container_fps=capture['avg_frame_rate'], capture=capture,
            timing='Actual capture timestamps; no interpolation, padding or speedup.', evidence=f'{take}/result.json', tests=[f'{take}/result.json'],
            requirements=['crossbow_draw_aim_fire_reload_holster'], scope=result['scope'], visual_review=review, limitations=limitations)
        manifest = [x for x in manifest if x['video'] != movie.name] + [row]
        case.append(movie.name)
    item = annotations['items']['crossbow_draw_aim_fire_reload_holster']
    item['in_game_recordings'] = list(dict.fromkeys(item.get('in_game_recordings', []) + case))
    item['evidence'] = list(dict.fromkeys(item.get('evidence', []) + [f'{take}/result.json']))
    tests.append(f'{take}/result.json'); videos += case
write(P/'runtime-video-manifest.json', manifest)
write(P/'review-evidence-annotations.json', annotations)
write(P/'crossbow-native-state-findings-v26.json', dict(complete=False, visual_accepted=False, dll=dll, checks=18, tests=tests, recordings=videos,
    confirmed=['Both roles passed nine checks.', 'Source-owned native defaultAnim distinguishes cocked and fired crossbow, including empty held state.',
               'Exact 15-joint Corvo and 33-joint Emily native mechanical clips play only on cosmetic remote equipment.'], limitations=limitations))
state = P.parent/'current-task-state.json'; s = read(state); m = s['MovementIntegration']
m['liveBuild'] = dll
m['current'] = 'D2MOVE29 / 213 clips, Steam action protocol v26. Standing crossbow source-state matrix passes; visual acceptance and complete movement phase unfinished.'
m['LatestContinuation'].update(live=dll, actual_game_videos=len(manifest), crossbow_native_state_checks=18, complete=False,
    next='Native muzzle effect observation and cosmetic replication; ranged quality/context gaps and remaining movement families.')
m['Ranged'].update(nativeMechanismBuild=dll, nativeMechanismChecks=18, nativeMechanismFindings='work/natural-moveset/crossbow-native-state-findings-v26.json', visualAccepted=False)
s['UpdatedAt'] = datetime.datetime.now().astimezone().isoformat()
write(state,s)
print(f'Indexed four crossbow videos; {len(manifest)} actual-game videos; 18 checks; complete=false')
