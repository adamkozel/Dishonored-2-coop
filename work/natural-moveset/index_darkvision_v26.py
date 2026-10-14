"""Index the inspected v26 stance/equipment/travel matrix without claiming full movement completion."""
import hashlib,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
def write(p,x):
 t=p.with_suffix(p.suffix+'.tmp');t.write_text(json.dumps(x,indent=2),encoding='utf-8');t.replace(p)
TAKES={
 'corvo':['crouched-v26-v1','empty-v26-v1','crouched-empty-v26-v1','moving-v26-v3','crouched-moving-v26-v1','empty-moving-v26-v1','crouched-empty-moving-v26-v1'],
 'emily':['crouched-v26-v1','empty-v26-v1','crouched-empty-v26-v2','moving-v26-v1','crouched-moving-v26-v1','empty-moving-v26-v1','crouched-empty-moving-v26-v1']}
rows=read(P/'runtime-video-manifest.json');annotations=read(P/'review-evidence-annotations.json');all_paths=[];all_tests=[];checks=0
banks=read(P/'darkvision-context-bank-manifest-v1.json')
for role,takes in TAKES.items():
 req=f'{role}_dark_vision_cast_end';item=annotations['items'][req];paths=[];tests=[]
 bank=next(x for x in banks if x['role']==role)
 for tail in takes:
  take=f'darkvision-{role}-{tail}';folder=P/take;r=read(folder/'result.json')
  assert r['passed'] and r['recorded'] and len(r['checks'])==(10 if r['moving'] else 9)
  assert len(r['nativeScenes'])==2 and len(r['seenEvents'])==2
  assert r['bankSha256']==bank['sha256'].upper() and bank['prior_payload_unchanged']
  assert r['dll']=='96C1FB32D85ED33B2F414955CF53E260CE2016523DD54582B98B1E25875CAD62'
  assert (folder/'contact.jpg').is_file() and (folder/'source-contact.jpg').is_file()
  context=', '.join(['crouched' if r['crouched'] else 'standing','sword holstered' if r['holstered'] else 'sword drawn','forward/backward travel' if r['moving'] else 'stationary'])
  review=dict(inspected=True,accepted=False,method='Selected actual D2 temporal frames inspected; full continuous animation approval remains pending.',
   contact_sheets=[f'{take}/contact.jpg',f'{take}/source-contact.jpg'],
   observations=['Native cast and toggle-off visibly animate the remote left arm with torso/shoulder response, while retaining the original full-body stance.',
    'Original drawn/holstered sword state recovers; holstered versions leave the right hand empty.' if r['holstered'] else 'Native sword remains attached through the cast and recovery.',
    'Forward/backward travel and changing foot poses are visible during the cast; the network retains root placement.' if r['moving'] else 'Entry, cast and recovery retain the same world position and stance.',
    'No gross calf/boot separation is apparent in the inspected view. Fine contact quality remains subject to continuous review.'],
   limitations=['This take covers only '+context+'.','No sideways, running, airborne, slide, water, upgrade or lifecycle coverage is implied.',
    'Crouched sword-floor clearance, fine finger contact, foot sliding and continuous blend quality are not fully accepted.',
    'Native caster-only vision remains private; no independent world-visible Dark Vision effect is claimed.',
    'Input/videoTime logs are relative to recorder launch; source and observer recordings are not frame-synchronized.'])
  write(folder/'visual-review.json',review)
  evidence=f'{take}/result.json';tests.append(evidence);checks+=len(r['checks'])
  for suffix in ('','-source'):
   movie=P/(take+suffix+'.mp4');s=json.loads(subprocess.check_output(['ffprobe','-v','error','-select_streams','v:0','-show_entries','stream=width,height,avg_frame_rate,duration,nb_frames','-of','json',str(movie)]))['streams'][0]
   hz=int(s['nb_frames'])/float(s['duration'])
   # The second recorder is supporting first-person/effect evidence. Keep its
   # actual timing and disclose a lower rate; never promote it to motion QA.
   if suffix:
    assert hz>=20
   else:
    assert hz>=24
   row=dict(video=movie.name,character=role,label=f'{role.title()} · Dark Vision · {context} · '+('caster POV' if suffix else 'remote body'),kind='game',gameplay_bound=True,travelling=r['moving'],complete=False,visual_accepted=False,
    dll_sha256=r['dll'],bank_sha256=r['bankSha256'],sha256=hashlib.sha256(movie.read_bytes()).hexdigest().upper(),actual_capture_hz=hz,container_fps=s['avg_frame_rate'],capture=s,
    timing='Actual capture timestamps; no padding or speedup. Separate camera recordings are not frame-synchronized.',evidence=evidence,tests=[evidence],requirements=[req],scope=r['scope'],context=dict(crouched=r['crouched'],holstered=r['holstered'],moving=r['moving']),visual_review=review,limitations=review['limitations'])
   if hz<24:
    row['capture_quality']='supporting_source_view_below_24Hz'
    row['limitations']=review['limitations']+[f'Source POV averages {hz:.2f} captured frames/s. It supports native effect/weapon scope only; use the companion observer recording for remote motion review.']
   rows=[x for x in rows if x['video']!=movie.name];rows.append(row);paths.append(movie.name)
 item.update(runtime_binding='partial_native_Dark_Vision_stance_equipment_and_travel_through_Steam',visual_validation='bounded_actual_game_frames_inspected',phase_status='work_remaining',notes='Native 933ms casts drive complete-body stance/equipment variants and distance-driven walking feet. Both roles pass stationary and forward/backward moving tests with drawn/holstered swords, native private reveal, stance/equipment recovery, health and cleanup. Further contexts and lifecycle remain.',native_effects_status='native_caster_vision_on_off_and_viewer_scope_verified_in_ground_contexts')
 item['in_game_recordings']=list(dict.fromkeys(item.get('in_game_recordings',[])+paths));item['evidence']=list(dict.fromkeys(item.get('evidence',[])+tests))
 item['motions']=list(dict.fromkeys(item['motions']+['native_darkvision_standing_empty','native_darkvision_crouched_armed','native_darkvision_crouched_empty']))
 all_paths+=paths;all_tests+=tests
 bank['partial_runtime_evidence']=tests
write(P/'runtime-video-manifest.json',rows);write(P/'review-evidence-annotations.json',annotations);write(P/'darkvision-context-bank-manifest-v1.json',banks)
contract=read(P/'movement-phase-contract.json')
for p in contract['power_cases']:
 if p['id']=='dark_vision':
  p['partial_runtime_evidence'].update(scope='Both roles standing/crouched, drawn/holstered; stationary and forward/backward walking. Native cast clock, private reveal, recovery, equipment/health/cleanup.',context_recordings=all_paths,context_tests=all_tests,context_checks=checks,remaining='Sideways/run/air/slide/water, upgrades, lifecycle and full continuous visual approval. Screen-effect scope traced in native code and declarations; no independent world-visible effect confirmed.')
write(P/'movement-phase-contract.json',contract)
print(f'Indexed {len(all_paths)} v26 actual-game views, {checks} checks; {len(rows)} total game recordings. Complete movement remains false.')
