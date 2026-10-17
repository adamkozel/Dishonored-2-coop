import json, sys, re
BS = chr(92)
P = r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset" + BS
HEX = re.compile(r'^[0-9a-fA-F]{64}$')
IDX = ('id', 'clip', 'index', 'cid', 'variant_clip', 'bank_clip', 'modified_bank_clip', 'newClip', 'output_clip', 'target_clip', 'bankClip', 'clip_id')


def items(o, path=''):
    if isinstance(o, dict):
        yield path, o
        for k, v in o.items():
            if k in ('root_track',):
                continue
            yield from items(v, path + '/' + k)
    elif isinstance(o, list):
        for i, v in enumerate(o):
            yield from items(v, path + f'[{i}]')


def show(fn, role='corvo'):
    d = json.load(open(P + fn, encoding='utf-8-sig'))
    tops = d if isinstance(d, list) else [d]
    tops = [t for t in tops if not isinstance(t, dict) or t.get('role') in (None, role)]
    print('=====', fn)
    for t in tops:
        if isinstance(t, dict):
            print('  TOP:', {k: v for k, v in t.items() if isinstance(v, (str, int, float, bool, list)) and k not in ('sha256',) and len(json.dumps(v)) < 400})
        for path, o in items(t):
            idx = [(k, o[k]) for k in IDX if k in o and isinstance(o[k], int)]
            if not idx:
                continue
            shas = {k: v[:12] for k, v in o.items() if isinstance(v, str) and HEX.match(v)}
            srcs = {k: v for k, v in o.items() if isinstance(v, str) and (BS in v or v.endswith('.bin'))}
            other = {k: v for k, v in o.items() if k in ('alias', 'name', 'purpose', 'part', 'phase', 'range', 'source_frames', 'crouch_adaptation', 'duration', 'operation', 'sourceClip', 'scope', 'role_in_bank', 'kind', 'method')}
            print('   ', path, idx, srcs, shas, json.dumps(other)[:400])


for fn in sys.argv[1:]:
    show(fn)
