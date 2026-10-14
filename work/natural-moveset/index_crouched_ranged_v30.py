"""Index explicitly inspected crouched ranged takes; never imply full acceptance."""
import datetime,hashlib,json,subprocess,sys
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,value):
    assert isinstance(value,(dict,list));pending=p.with_suffix(p.suffix+'.pending')
    pending.write_text(json.dumps(value,indent=2),encoding='utf-8');assert read(pending)==value;pending.replace(p)
manifest=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json')
findingsPath=P/'crouched-ranged-findings-v30.json'
findings=read(findingsPath) if findingsPath.exists() else dict(complete=False,visual_accepted=False,cases={})
for take in sys.argv[1:]:
    r=read(P/take/'result.json');review=read(P/take/'visual-review.json')
    assert r['passed'] and r['bankVersion']==30 and review['inspected'] and not review['accepted']
    assert all((P/s).is_file() for s in review['contact_sheets'])
    role='corvo' if r['source']=='host' else 'emily';revision=r['bankRevision']
    bank=P/f'crouched-ranged-banks-v{revision}'/f'{role}-crouched-ranged-bank.bin'
    assert hashlib.sha256(bank.read_bytes()).hexdigest().upper()==r['bankSha256']
    req=('pistol' if r['kind']=='gun' else 'crossbow')+'_draw_aim_fire_reload_holster';videos=[]
    for suffix in ('','-source'):
        movie=P/f'{take}{suffix}.mp4'
        capture=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
        hz=int(capture['nb_frames'])/float(capture['duration']);assert hz>=24
        row=dict(video=movie.name,character=role,label=take.replace('-',' ')+(' · native source' if suffix else ' · remote body'),kind='game',
                 gameplay_bound=True,travelling=r['travelling'],complete=False,visual_accepted=False,dll_sha256=r['dll'],bank_sha256=r['bankSha256'],
                 sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=capture['avg_frame_rate'],capture=capture,
                 timing='Actual capture timestamps; no interpolation, padding or speedup.',evidence=f'{take}/result.json',tests=[f'{take}/result.json'],
                 requirements=[req],scope=r['scope'],visual_review=review,limitations=review['limitations'])
        manifest=[x for x in manifest if x['video']!=movie.name]+[row];videos.append(movie.name)
    item=annotations['items'][req]
    item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+videos))
    item['evidence']=list(dict.fromkeys(item.get('evidence',[])+[f'{take}/result.json']))
    findings['cases'][take]=dict(result=f'{take}/result.json',checks=len(r['checks']),videos=videos,review=f'{take}/visual-review.json',
                               build=r['dll'],revision=revision,stance_transitions=r.get('stanceTransitions',False),travelling=r['travelling'])
write(P/'runtime-video-manifest.json',manifest);write(P/'review-evidence-annotations.json',annotations);write(findingsPath,findings)
state=P.parent/'current-task-state.json';s=read(state);assert isinstance(s,dict) and len(s)>=49
backup=P/'task-state-before-crouched-ranged-index-v30.json'
if not backup.exists():backup.write_bytes(state.read_bytes())
m=s['MovementIntegration'];protocol=m.get('AirborneRangedExperiment',{}).get('protocol',27)
m['current']=f'D2MOVE30 / 225 clips, Steam action protocol v{protocol}. Standing, crouched and bounded airborne ranged cases recorded; complete movement and final visual acceptance unfinished.'
m['LatestContinuation'].update(bank='D2MOVE30 / 225 clips; crouched-ranged-banks-v4 candidate',actual_game_videos=len(manifest),complete=False,
                              next='Ranged stance and moving-context checks, visual quality, then remaining traversal, combat, items, paired actions, powers and lifecycle coverage.')
m['CrouchedRangedExperiment'].update(bankManifest='work/natural-moveset/crouched-ranged-bank-manifest-v4.json',
    runtime='Passed cases are listed individually in crouched-ranged-findings-v30.json, including their exact build, stance-transition and travel scope. Native local-wrist baseline and balanced crouched draw/holster retained as candidates.',
    checkedTakes=list(dict.fromkeys(m['CrouchedRangedExperiment'].get('checkedTakes',[])+list(findings['cases']))),
    findings='work/natural-moveset/crouched-ranged-findings-v30.json',visualRemaining=[
        'Final naturalness, close finger/weapon grip, reload contact and continuous transition quality remain unaccepted.',
        'Prior oblique views are insufficient alone to judge sword direction; front comparison added.',
        'Airborne cases are tracked separately; aim pitch, upgrades, continuous native weapon switching and interruption/lifecycle matrix remain unfinished.'])
s['UpdatedAt']=datetime.datetime.now().astimezone().isoformat();write(state,s)
print('Indexed crouched ranged cases:',len(findings['cases']),'; actual-game recordings:',len(manifest),'; movement complete=false')
