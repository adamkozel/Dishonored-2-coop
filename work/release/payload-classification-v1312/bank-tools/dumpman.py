import json, sys
P=r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset\\"
def short(v, depth=0):
    if isinstance(v, dict):
        return {k: short(x, depth+1) for k, x in v.items() if k not in ('root_track','poses','frames_data')}
    if isinstance(v, list):
        if len(v) > 0 and all(isinstance(x,(int,float)) for x in v) and len(v)>8: return f'<nums{len(v)}>'
        if len(json.dumps(v)) > 3000 and depth>2: return f'<list{len(v)}>'
        return [short(x, depth+1) for x in v]
    return v
for fn in sys.argv[1:]:
    d=json.load(open(P+fn,encoding='utf-8-sig'))
    if isinstance(d,list):
        d=[e for e in d if not isinstance(e,dict) or e.get('role') in (None,'corvo')]
    print('=====',fn)
    print(json.dumps(short(d),indent=None)[:int(1e5)])
