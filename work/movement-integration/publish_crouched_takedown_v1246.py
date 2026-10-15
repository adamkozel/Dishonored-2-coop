"""Audit 1246 (#11, crouched_takedown): native probes 1244/1244b, network takes 1245-1245d (candidates 1213p-1213r)
and the observed visual take 1247. Writes the audit, the reviewed videos and the catalog annotations (crouched
takedown, plus the choke-hold regression fix on the rear choke items), updates the live state and rebuilds the
review catalog. Candidate archives are never rewritten."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
TAKES = {'crouched-takedown-v1244': None, 'crouched-takedown-v1244b': None, 'crouched-takedown-network-v1245': 'candidate-npc-combat-v1213p',
         'crouched-takedown-network-v1245b': 'candidate-npc-combat-v1213p', 'crouched-takedown-network-v1245c': 'candidate-npc-combat-v1213q',
         'crouched-takedown-network-v1245d': 'candidate-npc-combat-v1213r', 'crouched-takedown-observed-v1247': 'candidate-npc-combat-v1213r',
         'crouched-takedown-observed-v1247b': 'candidate-npc-combat-v1213r', 'crouched-takedown-observed-v1247c': 'candidate-npc-combat-v1213r', 'crouched-takedown-observed-v1247d': 'candidate-npc-combat-v1213r'}
CAND = 'candidate-npc-combat-v1213r'
# Visual reviews of the observed take (contact-sheet inspection); filled after review.
REVIEW = json.loads((W / 'crouched-takedown-review-v1246.json').read_text(encoding='utf-8')) if (W / 'crouched-takedown-review-v1246.json').exists() else {}
rows = []
for t, c in TAKES.items():
    if not (N / t / 'result.json').exists(): continue
    r = rd(N / t / 'result.json'); rest = rd(N / t / 'restoration.json') if (N / t / 'restoration.json').exists() else {}
    rows.append(dict(take=f'../natural-moveset/{t}', candidate=c, passed=r['passed'], failure=r.get('failure'), restoration=rest.get('passed'),
        cases=[{k: x.get(k) for k in ('source', 'kind', 'passed', 'failure', 'scenes', 'playerStates', 'crouchedAssassinationSamples', 'recoveredSamples', 'restored')} for x in r.get('cases', [])]))
videos = rd(N / 'runtime-video-manifest.json')
for path, (accepted, requirement, review) in REVIEW.items():
    f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/'))
    source = 'host' if '/host-' in path else 'client'; obs = 'client' if source == 'host' else 'host'; kind = 'lethal' if '-lethal-' in path else 'choke'
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=obs, character='corvo' if obs == 'client' else 'emily',
        label=f'Paired / crouched rear {kind} (source {source}) / {path.split("/")[0]}', travelling=False, gameplay_bound=True,
        actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Held observer camera beyond the guard, looking back at the source, while the source performs a rear takedown started from crouch.',
        review=review, candidate=CAND, dll=rd(W / CAND / 'identity.json')['dll'], runtime_test_passed=True, visual_accepted=accepted, requirements=[requirement] if accepted else []))
wr(N / 'runtime-video-manifest.json', videos)
at = datetime.datetime.now().astimezone().isoformat()
accepted = sorted({p.split('/')[1].split('-observer')[0] for p, (ok, _, _) in REVIEW.items() if ok})
wr(W / 'crouched-takedown-audit-v1246.json', dict(at=at, candidate=CAND, takes=rows,
    findings=['Confirmed runtime (probe 1244b, both roles): a rear choke started from crouch stands the player into exclusive Choke41 with the standing scenes (choke_back 600, choke_loop 1400, choke_win 1800 ms); a fast-back lethal started from crouch plays assassination_fast_back (1333 ms) while the player stays crouched (exclusive Assassination13 at physical stance 1), which the reader refused.',
              'Lab fixture: PrepareShadowSword alone drops the lab-restored note note_dw_frommeagan from the client (1244 stopped on the inventory check); every per-case restore now re-runs the note restore.',
              'Confirmed runtime (1245, 1213p): the crouched lethal is presented in both roles (phase 5 at weight > 0.8, locomotion returns).',
              'Confirmed static and runtime (1245-1245c, 1213q engine_call_gap): during every choke hold (exclusive Choke41 at physical stance 12) the wire stance was invalid while the paired-state root motion stayed valid, so validCoopPose rejected each pose packet of the hold (8 rejected in 1245b); no engine stall (no engine-thread call gap of 50 ms or more on either peer), both windows kept rendering. The lab lease then retired the partner about 250 ms into the hold. The same rejection affected the standing rear choke.',
              'Confirmed runtime (1245d, 1213r, passed, restoration passed): with the choke hold mapped to presentation stance 0 and motion only sent with a wire stance, the crouched choke (phases 1/2/3), the crouched lethal (phase 5) and the standing choke regression case are presented in both roles and locomotion returns.',
              f'Visual (1247-1247d, held observer camera; in front of the guard it hid the partner, side spots landed in walls, the view along the corridor from behind and above worked): accepted for {", ".join(accepted) if accepted else "none"}.'],
    complete=True, scope='crouched_takedown runtime binding in both roles (rear choke and fast-back lethal from crouch); other takedown directions, victim alignment and shared outcomes remain under their own items.'))
p = N / 'review-evidence-annotations.json'; an = rd(p)
it = an['items']['crouched_takedown']
it['runtime_binding'] = 'native_crouched_rear_choke_and_fast_back_lethal_presented_both_roles'
if accepted: it['visual_validation'] = 'accepted_' + '_'.join(sorted({a.replace('-', '_') for a in accepted}))
it['notes'] = ('Audit 1246 (1213r): a rear takedown started from crouch is bound in both roles. Native: the choke stands the player into Choke41 '
               '(same scenes as standing); the fast-back lethal keeps the player crouched (Assassination13 at stance 1, admitted by 1213p). The partner presents '
               'every native phase and returns to locomotion. The partner rises into the standing paired clips; a crouched-height lethal clip, victim alignment and '
               'shared outcomes remain open.')
if '../movement-integration/crouched-takedown-audit-v1246.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/crouched-takedown-audit-v1246.json')
for key in ('nonlethal_takedown_front_back_sides', 'struggle_and_escape'):
    it = an['items'][key]
    note = ' Audit 1246 (1213r): the choke hold (Choke41 at stance 12) sent root motion without a wire stance, so peers rejected every pose packet of the hold and the lab lease retired the partner; fixed and re-verified with a standing choke in both roles (1245d).'
    it['notes'] = (it.get('notes') or '').split(' Audit 1246')[0] + note
    if '../movement-integration/crouched-takedown-audit-v1246.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/crouched-takedown-audit-v1246.json')
wr(p, an)
s = rd(R / 'work/current-task-state.json'); s['UpdatedAt'] = at; m = s['MovementExecutionPriority']
m['current'] = ('1213r live (protocol 75): crouched rear choke and fast-back lethal presented in both roles; the choke hold keeps a valid wire stance (audit 1246); '
                'NPC catalog reads on a worker thread (#43); product presentation hold through source gaps (audit 1241).')
wr(R / 'work/current-task-state.json', s)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
