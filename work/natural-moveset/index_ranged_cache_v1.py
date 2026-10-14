"""Index inspected game regressions without marking the movement phase complete."""
import datetime
import hashlib
import json
import subprocess
import sys
from pathlib import Path

P = Path(__file__).resolve().parent
def read(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p, value):
    tmp = p.with_suffix(p.suffix + '.pending')
    tmp.write_text(json.dumps(value, indent=2), encoding='utf-8')
    assert read(tmp) == value
    tmp.replace(p)

manifest = read(P / 'runtime-video-manifest.json')
annotations = read(P / 'review-evidence-annotations.json')
air_mode = '--air' in sys.argv[1:]
muzzle_mode = '--muzzle' in sys.argv[1:]
assert not (air_mode and muzzle_mode)
takes = [arg for arg in sys.argv[1:] if arg not in ('--air', '--muzzle')]
findings_path = P / ('remote-muzzle-runtime-findings-v1.json' if muzzle_mode else 'ranged-air-runtime-findings-v28.json' if air_mode else 'ranged-cache-runtime-findings-v1.json')
findings = read(findings_path) if findings_path.exists() else dict(complete=False, visual_accepted=False, cases={})
for take in takes:
    assert Path(take).name == take
    result = read(P / take / 'result.json')
    review = read(P / take / 'visual-review.json')
    assert result['passed'] and result['bankVersion'] == 30 and review['inspected'] and not review['accepted']
    assert bool(result.get('airborne', False)) == air_mode
    if muzzle_mode:
        assert result.get('compactMuzzle') and result.get('assetManifest')
        asset_manifest_path = P.parents[1] / result['assetManifest']
        asset_manifest = read(asset_manifest_path)
        for asset in asset_manifest['files']:
            source = P.parents[1] / 'outputs/d2-coop-lab/assets/ranged-muzzle-presentation-v1-source' / asset['name']
            assert hashlib.sha256(source.read_bytes()).hexdigest().lower() == asset['sha256'].lower()
    assert all((P / sheet).is_file() for sheet in review['contact_sheets'])
    role = 'corvo' if result['source'] == 'host' else 'emily'
    bank = P / f'crouched-ranged-banks-v{result["bankRevision"]}' / f'{role}-crouched-ranged-bank.bin'
    assert hashlib.sha256(bank.read_bytes()).hexdigest().upper() == result['bankSha256']
    req = ('pistol' if result['kind'] == 'gun' else 'crossbow') + '_draw_aim_fire_reload_holster'
    videos = []
    for suffix in ('', '-source'):
        movie = P / f'{take}{suffix}.mp4'
        capture = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=width,height,avg_frame_rate,duration,nb_frames', '-of', 'json', str(movie)]))['streams'][0]
        hz = int(capture['nb_frames']) / float(capture['duration'])
        assert hz >= 24
        row = dict(video=movie.name, character=role, label=take.replace('-', ' ') + (' · native source' if suffix else ' · remote body'), kind='game',
                   gameplay_bound=True, travelling=result['travelling'], complete=False, visual_accepted=False,
                   dll_sha256=result['dll'], bank_sha256=result['bankSha256'], sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),
                   actual_capture_hz=hz, container_fps=capture['avg_frame_rate'], capture=capture,
                   timing='Actual capture timestamps; no interpolation, padding or speedup.', evidence=f'{take}/result.json', tests=[f'{take}/result.json'],
                   requirements=[req], scope=result['scope'], visual_review=review, limitations=review['limitations'])
        if muzzle_mode:
            row['asset_manifest'] = result['assetManifest']
            row['asset_manifest_sha256'] = hashlib.sha256(asset_manifest_path.read_bytes()).hexdigest().upper()
        manifest = [v for v in manifest if v['video'] != movie.name] + [row]
        videos.append(movie.name)
    item = annotations['items'][req]
    item['in_game_recordings'] = list(dict.fromkeys(item.get('in_game_recordings', []) + videos))
    item['evidence'] = list(dict.fromkeys(item.get('evidence', []) + [f'{take}/result.json']))
    findings['cases'][take] = dict(result=f'{take}/result.json', checks=len(result['checks']), videos=videos,
                                  review=f'{take}/visual-review.json', build=result['dll'], empty_only=result.get('emptyOnly', False),
                                  crouched=result['crouched'], airborne=result.get('airborne', False), air_reload=result.get('airReload', False))
write(P / 'runtime-video-manifest.json', manifest)
write(P / 'review-evidence-annotations.json', annotations)
write(findings_path, findings)
state_path = P.parent / 'current-task-state.json'
state = read(state_path)
assert isinstance(state, dict) and len(state) >= 49
target = state['MovementIntegration']['Ranged']['remoteMuzzleExperiment'] if muzzle_mode else state['MovementIntegration']['AirborneRangedExperiment' if air_mode else 'RangedPropCache']
target['indexedFindings'] = 'work/natural-moveset/' + findings_path.name
state['MovementIntegration']['LatestContinuation']['actual_game_videos'] = len(manifest)
state['UpdatedAt'] = datetime.datetime.now().astimezone().isoformat()
write(state_path, state)
print(f'Indexed {len(findings["cases"])} ranged {"muzzle" if muzzle_mode else "air" if air_mode else "cache"} regressions; {len(manifest)} actual-game recordings; movement remains incomplete.')
