"""1917 (criterion 1): contact sheets of the completion videos for a visual spot review. A frame at 25, 50 and 75 percent
of each completion visual named in review-catalog.json, three frames per row (one video per row), six rows per sheet,
with index.json naming each row's item and video. Reads the catalog and the videos only; writes
movement-completion-sheets-v1917/."""
import json, subprocess, shutil, sys
from pathlib import Path
N = Path(__file__).resolve().parent
OUT = N / 'movement-completion-sheets-v1917'
if OUT.exists():
    sys.exit('take exists')
OUT.mkdir()
FFMPEG = shutil.which('ffmpeg'); FFPROBE = shutil.which('ffprobe')
cat = json.loads((N / 'review-catalog.json').read_text(encoding='utf-8-sig'))
rows = []
for it in cat['items']:
    for v in (it.get('completion') or {}).get('visual') or []:
        rel = v['video'] if isinstance(v, dict) else v
        rows.append({'item': it['id'], 'video': rel, 'verdict': v.get('verdict') if isinstance(v, dict) else None})
frames = OUT / 'frames'; frames.mkdir()
for n, r in enumerate(rows):
    p = N / r['video']
    q = subprocess.run([FFPROBE, '-v', 'error', '-show_entries', 'format=duration', '-of', 'csv=p=0', str(p)], capture_output=True, text=True)
    try:
        dur = float(q.stdout.strip())
    except ValueError:
        dur = 0
    r['seconds'] = round(dur, 2); r['frames'] = []
    for k, f in enumerate((0.25, 0.5, 0.75)):
        o = frames / f'{n:03d}-{k}.jpg'
        subprocess.run([FFMPEG, '-v', 'error', '-ss', f'{dur * f:.2f}', '-i', str(p), '-frames:v', '1', '-vf', 'scale=400:-2', '-y', str(o)], capture_output=True)
        if o.is_file():
            r['frames'].append(o.name)
sheets = []
for s in range(0, len(rows), 6):
    group = rows[s:s + 6]
    files = [frames / f for r in group for f in (r['frames'] + [r['frames'][-1]] * (3 - len(r['frames'])) if r['frames'] else [])]
    if len(files) != 3 * len(group):
        continue
    inputs = []
    for f in files:
        inputs += ['-i', str(f)]
    layout = '|'.join(f'{(i % 3) * 400}_{(i // 3) * 230}' for i in range(len(files)))
    flt = ''.join(f'[{i}:v]scale=400:225,pad=400:230[v{i}];' for i in range(len(files))) + ''.join(f'[v{i}]' for i in range(len(files))) + f'xstack=inputs={len(files)}:layout={layout}:fill=black'
    name = f'sheet-{s // 6:02d}.jpg'
    subprocess.run([FFMPEG, '-v', 'error'] + inputs + ['-filter_complex', flt, '-frames:v', '1', '-q:v', '4', '-y', str(OUT / name)], capture_output=True)
    if (OUT / name).is_file():
        sheets.append({'sheet': name, 'rows': [{'item': r['item'], 'video': r['video']} for r in group]})
(OUT / 'index.json').write_text(json.dumps({'videos': len(rows), 'sheets': sheets, 'rows': rows}, indent=1), encoding='utf-8')
print(len(rows), 'videos,', len(sheets), 'sheets')
