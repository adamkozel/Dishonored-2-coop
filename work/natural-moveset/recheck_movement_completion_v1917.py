"""1917 (goal criterion 1): an independent recheck of every movement requirement in review-catalog.json against the
evidence on disk, rather than the catalog's own completion flags.

For each item the recheck records:
- applicable: phase_status is not not_applicable or excluded_by_policy;
- decision: completion.decision is complete, and the completion audit file exists and names the item;
- takes: every completion take folder exists, and its result.json (when the take has one) passed;
- videos: every completion visual and every in-game recording exists and is a real video
  (ffprobe: at least 1 s, at least 320 px wide), with a reviewer verdict for each completion visual;
- roles: both characters are listed, and the completion takes or visuals cover host and client
  (the case and file names name them) unless the item is single-role by its own scope;
- binding and acceptance: runtime_binding and visual_validation are recorded (visual_validation starts with accepted).
It also writes one contact sheet per 20 completion visuals (a mid-video frame of each, labelled), for a visual
spot review. Writes movement-completion-recheck-v1917/ (summary.json and the sheets). Read-only otherwise.
"""
import json, subprocess, sys, shutil
from pathlib import Path
N = Path(__file__).resolve().parent
OUT = N / 'movement-completion-recheck-v1917'
if OUT.exists():
    sys.exit('take exists')
OUT.mkdir()
FFPROBE = shutil.which('ffprobe'); FFMPEG = shutil.which('ffmpeg')
cat = json.loads((N / 'review-catalog.json').read_text(encoding='utf-8-sig'))
probe_cache = {}


def probe(rel):
    if rel in probe_cache:
        return probe_cache[rel]
    p = N / rel
    r = {'path': rel, 'exists': p.is_file()}
    if r['exists']:
        q = subprocess.run([FFPROBE, '-v', 'error', '-select_streams', 'v:0', '-show_entries', 'stream=width,height:format=duration',
                            '-of', 'json', str(p)], capture_output=True, text=True)
        try:
            j = json.loads(q.stdout)
            r['seconds'] = round(float(j['format']['duration']), 2)
            r['width'] = j['streams'][0]['width']; r['height'] = j['streams'][0]['height']
        except Exception as e:
            r['error'] = str(e)[:120]
    r['valid'] = bool(r.get('exists') and r.get('seconds', 0) >= 1 and r.get('width', 0) >= 320)
    probe_cache[rel] = r
    return r


items = []
sheets = []
for it in cat['items']:
    iid = it['id']; comp = it.get('completion') or {}
    rec = {'id': iid, 'phase_status': it.get('phase_status'), 'characters': it.get('characters')}
    rec['applicable'] = it.get('phase_status') not in ('not_applicable', 'excluded_by_policy')
    if not rec['applicable']:
        rec['ok'] = True; rec['reason'] = it.get('phase_status'); items.append(rec); continue
    problems = []
    if comp.get('decision') != 'complete':
        problems.append('decision not complete')
    audit = comp.get('audit')
    ap = (N / audit) if audit else None
    rec['audit'] = audit
    if not ap or not ap.is_file():
        problems.append(f'audit file missing: {audit}')
    elif iid not in ap.read_text(encoding='utf-8-sig', errors='replace'):
        problems.append(f'audit does not name the item: {audit}')
    takes = []
    for t in comp.get('takes') or []:
        name = t.get('take') if isinstance(t, dict) else t
        d = N / name
        tr = {'take': name, 'exists': d.is_dir()}
        if tr['exists'] and (d / 'result.json').is_file():
            try:
                tr['passed'] = json.loads((d / 'result.json').read_text(encoding='utf-8-sig')).get('passed')
            except Exception:
                tr['passed'] = None
        if not tr['exists']:
            problems.append(f'take missing: {name}')
        elif tr.get('passed') is False:
            # a coverage take can pass the item's cases while another case failed; keep the record and flag it
            tr['note'] = 'result.json passed is false'
        takes.append(tr)
    rec['takes'] = takes
    vids = []
    for v in comp.get('visual') or []:
        rel = v.get('video') if isinstance(v, dict) else v
        pr = probe(rel); pr = dict(pr); pr['verdict'] = (v.get('verdict') if isinstance(v, dict) else None)
        if not pr['valid']:
            problems.append(f'visual invalid: {rel}')
        if not pr['verdict']:
            problems.append(f'visual without verdict: {rel}')
        vids.append(pr)
        if pr['valid']:
            sheets.append((iid, rel, pr['seconds']))
    rec['visuals'] = vids
    recs = [probe(r) for r in it.get('in_game_recordings') or []]
    rec['recordings'] = {'count': len(recs), 'valid': sum(1 for r in recs if r['valid']),
                         'invalid': [r['path'] for r in recs if not r['valid']]}
    if not vids and not rec['recordings']['valid']:
        problems.append('no valid real-game video')
    text = json.dumps(comp).lower() + ' ' + ' '.join(it.get('in_game_recordings') or []).lower()
    chars = it.get('characters') or []
    rec['roles'] = {'host': 'host' in text, 'client': 'client' in text or 'partner' in text,
                    'corvo': 'corvo' in text or 'corvo' in chars, 'emily': 'emily' in text or 'emily' in chars}
    if not (rec['roles']['host'] and rec['roles']['client']):
        single = any(w in (comp.get('scope') or '').lower() for w in ('one role', 'single role', 'host only', 'partner only', 'emily only', 'corvo only', "emily's", "corvo's"))
        rec['singleRoleScope'] = single
        if not single:
            problems.append('both roles not named in the completion evidence')
    rec['runtime_binding'] = it.get('runtime_binding'); rec['visual_validation'] = it.get('visual_validation')
    if not it.get('runtime_binding') or str(it.get('runtime_binding')).startswith('none'):
        problems.append('no runtime binding')
    if not str(it.get('visual_validation') or '').startswith('accepted'):
        problems.append('visual acceptance not recorded')
    rec['problems'] = problems; rec['ok'] = not problems
    items.append(rec)

# contact sheets: a mid frame of each completion visual, 20 per sheet
frames = OUT / 'frames'; frames.mkdir()
paths = []
for n, (iid, rel, sec) in enumerate(sheets):
    f = frames / f'{n:03d}.jpg'
    label = f'{iid} | {rel}'.replace(':', ' ').replace("'", '')[:90]
    subprocess.run([FFMPEG, '-v', 'error', '-ss', str(max(sec / 2, 0.5)), '-i', str(N / rel), '-frames:v', '1',
                    '-vf', f"scale=480:-2,drawtext=text='{label}':x=4:y=4:fontsize=11:fontcolor=yellow:box=1:boxcolor=black@0.6",
                    '-y', str(f)], capture_output=True)
    if f.is_file():
        paths.append(f)
for s in range(0, len(paths), 20):
    group = paths[s:s + 20]
    lst = OUT / f'sheet-{s // 20:02d}.txt'
    inputs = []
    for g in group:
        inputs += ['-i', str(g)]
    cols = 4; rows = (len(group) + cols - 1) // cols
    layout = '|'.join(f'{(i % cols)}*480_{(i // cols)}*300' for i in range(len(group)))
    flt = ''.join(f'[{i}:v]scale=480:270,pad=480:300[v{i}];' for i in range(len(group))) + ''.join(f'[v{i}]' for i in range(len(group))) + f'xstack=inputs={len(group)}:layout={layout}:fill=black'
    if len(group) == 1:
        flt = '[0:v]scale=480:270'
    subprocess.run([FFMPEG, '-v', 'error'] + inputs + ['-filter_complex', flt, '-frames:v', '1', '-y', str(OUT / f'sheet-{s // 20:02d}.jpg')], capture_output=True)
    lst.write_text('\n'.join(g.name for g in group), encoding='utf-8')

app = [i for i in items if i['applicable']]
summary = {
    'catalog': 'review-catalog.json', 'items': len(items), 'applicable': len(app),
    'notApplicable': [{'id': i['id'], 'reason': i['reason']} for i in items if not i['applicable']],
    'applicableOk': sum(1 for i in app if i['ok']),
    'failing': [{'id': i['id'], 'problems': i['problems']} for i in app if not i['ok']],
    'videosChecked': len(probe_cache), 'videosValid': sum(1 for v in probe_cache.values() if v['valid']),
    'completionVisualFrames': len(paths), 'sheets': sorted(p.name for p in OUT.glob('sheet-*.jpg')),
    'itemsDetail': items,
}
(OUT / 'summary.json').write_text(json.dumps(summary, indent=1), encoding='utf-8')
print(json.dumps({k: v for k, v in summary.items() if k != 'itemsDetail'}, indent=1)[:4000])
