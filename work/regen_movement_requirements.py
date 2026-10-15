"""Regenerate only the movement checklist (documentation/handoff-2026-10-11/generated/movement-requirements.md
and generated/requirements/<family>.md) from work/natural-moveset/review-catalog.json, in the same format as
build_handoff.py, without rebuilding the rest of the generated handoff."""
from pathlib import Path
import json, collections, datetime, os
P = Path(__file__).resolve().parents[1]; G = P / 'documentation/handoff-2026-10-11/generated'
STAMP = datetime.datetime.now().astimezone().isoformat()
def data(p): return json.loads(p.read_text(encoding='utf-8-sig'))
def link(p, fromfile, label=None):
    r = os.path.relpath(p, fromfile.parent).replace('\\', '/'); return f'[{label or p.name}](<{r}>)'
def block(v, lang='json'): return f'````{lang}\n' + json.dumps(v, indent=2, ensure_ascii=False) + '\n````\n'
catalogPath = P / 'work/natural-moveset/review-catalog.json'; catalog = data(catalogPath)
contract = data(P / 'work/natural-moveset/movement-phase-contract.json')
req = G / 'movement-requirements.md'
bound = sum(1 for i in catalog['items'] if i.get('runtime_binding') != 'not_verified')
accepted = sum(1 for i in catalog['items'] if i.get('visual_validation') != 'not_accepted')
lines = ['# Complete functional movement requirements\n', f'Captured {STAMP}; source {link(catalogPath, req)}.\n',
         f'{len(catalog["items"])} requirements: {bound} runtime-bound (fully or partially), {accepted} with accepted visuals. Candidate material and recordings do not establish completion.\n',
         '## Acceptance contract\n', block(contract), '## Requirements\n',
         '| id | family | authoring | runtime binding | visual validation |', '|---|---|---|---|---|']
families = collections.defaultdict(list)
for item in catalog['items']:
    families[item['family']].append(item)
    lines.append('| ' + ' | '.join(str(item.get(k, '')) for k in ['id', 'family', 'authoring', 'runtime_binding', 'visual_validation']) + ' |')
lines += ['\n## Full per-family records\n']
for family, items in families.items():
    f = G / 'requirements' / f'{family}.md'; lines.append('- ' + link(f, req, f'{family}: {len(items)} requirements'))
    content = [f'# {family}: complete requirement records\n', f'Source {link(catalogPath, f)}; captured {STAMP}. All fields below retain the catalog wording.\n']
    for item in items:
        content += [f'## {item["id"]}\n', str(item.get('notes', '')) + '\n', block(item)]
        for key in ['evidence', 'in_game_recordings']:
            for x in item.get(key, []) if isinstance(item.get(key, []), list) else []:
                if isinstance(x, str):
                    p = P / 'work/natural-moveset' / x
                    if p.exists(): content.append('- ' + link(p, f, key + ': ' + x))
    content.append('\n'); f.parent.mkdir(parents=True, exist_ok=True); f.write_text('\n'.join(content), encoding='utf-8')
req.write_text('\n'.join(lines) + '\n', encoding='utf-8')
print(f'{len(catalog["items"])} requirements, {bound} bound, {accepted} visually accepted')
