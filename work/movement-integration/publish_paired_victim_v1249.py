"""Audit 1249 (#11, paired_victim_alignment): the victim of a rear choke under the product controller (NPC stream on,
natural guard AI), take 1248 (candidate 1213r). Host-sourced: the client's replica of the host-authoritative guard
is driven by the host's NPC poses through the whole choke. Client-sourced: the host's guard never learns of the
takedown (new issue). Writes the audit, the reviewed video and the catalog annotation, then rebuilds the catalog."""
from pathlib import Path
import json, datetime, subprocess, sys
R = Path(__file__).resolve().parents[2]; W = R / 'work/movement-integration'; N = R / 'work/natural-moveset'
rd = lambda p: json.loads(p.read_text(encoding='utf-8-sig'))
def wr(p, v): p.write_text(json.dumps(v, indent=2), encoding='utf-8')
CAND = 'candidate-npc-combat-v1213r'; TAKE = 'paired-victim-product-v1248'; ISSUE = sys.argv[1] if len(sys.argv) > 1 else None
r = rd(N / TAKE / 'result.json')
REVIEW = {f'{TAKE}/host-choke-observer.mp4': (True, 'Held camera behind the attacker, client window, product controller: remote Corvo chokes the guard and the guard struggles in his arms, collapses and lies at his feet - the replica guard follows the host\'s native victim through the NPC stream. Natural paired takedown in the observer\'s world.'),
          f'{TAKE}/client-choke-observer.mp4': (False, 'Host window: remote Emily presents the choke, but the host\'s guard is host-authoritative and never learns of the client\'s takedown; it keeps walking. Not accepted (victim not shared).')}
videos = rd(N / 'runtime-video-manifest.json')
for path, (accepted, review) in REVIEW.items():
    f = N / path
    if not f.exists(): continue
    meta = json.loads(subprocess.check_output(['ffprobe', '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=avg_frame_rate', '-of', 'json', str(f)], text=True))['streams'][0]
    a, b = map(float, meta['avg_frame_rate'].split('/')); source = 'host' if '/host-' in path else 'client'; obs = 'client' if source == 'host' else 'host'
    videos = [x for x in videos if x['video'] != path]
    videos.append(dict(video=path, role=obs, character='corvo' if obs == 'client' else 'emily', label=f'Paired / rear choke victim under the product controller (source {source}) / {TAKE}',
        travelling=False, gameplay_bound=True, actual_capture_hz=round(a / b, 2) if b else None, container_fps=meta['avg_frame_rate'], timing='Native wall-time recording, no speedup',
        scope='Held observer camera behind the attacker; product controller with the NPC stream; natural guard AI.', review=review, candidate=CAND,
        dll=rd(W / CAND / 'identity.json')['dll'], runtime_test_passed=True, visual_accepted=accepted, requirements=['paired_victim_alignment'] if accepted else []))
wr(N / 'runtime-video-manifest.json', videos)
cases = {c['source']: c for c in r['cases']}
at = datetime.datetime.now().astimezone().isoformat()
wr(W / 'paired-victim-audit-v1249.json', dict(at=at, candidate=CAND, take=dict(take=f'../natural-moveset/{TAKE}', passed=r['passed'], restoration=rd(N / TAKE / 'restoration.json')['passed'],
    cases=[{k: c.get(k) for k in ('source', 'passed', 'scenes', 'victim', 'guardHealthObserverAfter', 'restored')} for c in r['cases']]),
    findings=['Confirmed runtime (1248, product controller, natural guard AI, host source): the client\'s replica of the host-authoritative guard stays a ready client_replica through the choke and is driven by the host\'s native victim poses (94 poses, 442 body applications in ~10 s); it ends where the host\'s guard fell. Visual accepted: the guard struggles in remote Corvo\'s arms, collapses and lies at his feet.',
              'Confirmed runtime (1248, client source): the partner presents every choke phase on the host, but the host\'s guard is host-authoritative and never learns of the client\'s takedown: it walked ~6 m on, and the client\'s channel for it ended in peer_timeout. Client-sourced takedowns on host-owned NPCs are not shared' + (f' (#{ISSUE}).' if ISSUE else '.'),
              'Lab takes 1245-1247 armed the partner by hand with the NPC stream off, so their observer guards never moved; victim sync depends on the product controller\'s NPC stream.'],
    complete=False, scope='paired_victim_alignment: host-sourced rear choke victim synchronized and accepted; client-sourced takedowns need a shared outcome path.'))
p = N / 'review-evidence-annotations.json'; an = rd(p); it = an['items']['paired_victim_alignment']
it['runtime_binding'] = 'partial_host_source_victim_driven_by_npc_stream_product'
it['visual_validation'] = 'accepted_host_source_rear_choke_victim'
it['notes'] = ('Audit 1249 (1213r, product controller): for a host-sourced rear choke the observer\'s guard follows the host\'s native victim through the NPC stream '
               '(struggle, collapse, body at the attacker\'s feet; visual accepted). A client-sourced takedown is not shared: the host\'s authoritative guard never learns of it'
               + (f' (#{ISSUE})' if ISSUE else '') + '. Other takedowns, directions and body sizes remain open.')
if '../movement-integration/paired-victim-audit-v1249.json' not in it.setdefault('evidence', []): it['evidence'].append('../movement-integration/paired-victim-audit-v1249.json')
wr(p, an)
out = subprocess.run([sys.executable, '-X', 'utf8', str(N / 'build_review_catalog.py')], cwd=N, capture_output=True, text=True)
print(out.stdout[-160:], out.stderr[-300:]); assert out.returncode == 0
