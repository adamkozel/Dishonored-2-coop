import json, sys, re
BS = chr(92)
P = r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset" + BS
HEX = re.compile(r'^[0-9a-fA-F]{64}$')
KEYS = ('bank_clip', 'modified_bank_clip', 'armed_clip', 'runtime_ground_clip', 'source_clip', 'crouch_reference', 'runtime_phase', 'clip', 'variant', 'parameter')


def short_src(s):
    if isinstance(s, list):
        return [x[:12] if isinstance(x, str) and HEX.match(x) else short_src(x) for x in s]
    if isinstance(s, dict):
        return {k: (v[:12] if isinstance(v, str) and HEX.match(v) else (short_src(v) if isinstance(v, (list, dict)) else v)) for k, v in s.items() if k not in ('finger_mapping',)}
    return s


for fn in sys.argv[1:]:
    d = json.load(open(P + fn, encoding='utf-8-sig'))
    rows = d if isinstance(d, list) else d.get('rows', [d])
    print('=====', fn, len(rows))
    for e in rows:
        if not isinstance(e, dict) or e.get('role') not in (None, 'corvo'):
            continue
        alias = (e.get('selection') or {}).get('alias') or e.get('alias') or e.get('id')
        extra = {k: e[k] for k in KEYS if k in e}
        print('  ', alias, e.get('sha256', '')[:12], json.dumps(short_src(e.get('source')))[:350], extra, (e.get('selection') or {}).get('purpose', '')[:160])
