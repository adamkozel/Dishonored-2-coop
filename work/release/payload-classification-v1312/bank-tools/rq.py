"""Quick resolve: args are sha prefixes or manifest:alias (corvo)."""
import sys, json, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import resolver as R

BANKS = {}
try:
    H = json.load(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'bankhashes.json')))
    for k, v in H.items():
        BANKS[v['sha256'].lower()] = k
except Exception:
    pass


def full(prefix):
    p = prefix.lower()
    c = [k for k in R.DON if k.startswith(p)] + [k for k in R.BINH if k.startswith(p)] + [k for k in BANKS if k.startswith(p)]
    c += [k.lower() for k in R.glob_idx() if k.lower().startswith(p)]
    c = sorted(set(c))
    return c


for a in sys.argv[1:]:
    if ':' in a:
        fn, alias = a.split(':', 1)
        d = json.load(open(os.path.join(R.P, fn), encoding='utf-8-sig'))
        rows = d if isinstance(d, list) else d.get('rows', [])
        e = [x for x in rows if x.get('role') == 'corvo' and ((x.get('selection') or {}).get('alias') == alias or alias in (x.get('source') or {}).get('aliases', []) if isinstance(x.get('source'), dict) else (x.get('selection') or {}).get('alias') == alias)]
        if not e:
            print(a, 'NOT FOUND'); continue
        s = e[0]['sha256'].lower()
    else:
        c = full(a)
        if len(c) != 1:
            print(a, 'candidates', c[:5]);
            if not c: continue
        s = c[0]
    if s in BANKS:
        print(a, '-> BANK', BANKS[s]); continue
    lv, notes = R.resolve(s)
    print(a, notes, json.dumps(R.summarize(lv))[:1500])
