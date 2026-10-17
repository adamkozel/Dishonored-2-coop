import json, os, sys, re
root = r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work"
dirs = [os.path.join(root,'natural-moveset'), os.path.join(root,'movement-integration')]
hits = []
def walk(obj, path, fn):
    if isinstance(obj, dict):
        keys = set(obj.keys())
        if ('sha256' in keys or 'bankSha256' in keys or 'sha' in keys) and any(k in keys for k in ('source_bank_sha256','sourceBankSha256','source_sha256','sourceSha256','base_bank_sha256','baseBankSha256','sourceBank','source_bank','added','clips','version','role')):
            hits.append((fn, path, {k:(v if not isinstance(v,(list,dict)) else f'<{type(v).__name__}{len(v)}>') for k,v in obj.items()}))
        for k,v in obj.items():
            walk(v, path+'/'+str(k), fn)
    elif isinstance(obj, list):
        for i,v in enumerate(obj):
            walk(v, path+f'[{i}]', fn)
for d in dirs:
    for dp, dn, fns in os.walk(d):
        # skip deep lab dirs
        for f in fns:
            if not f.endswith('.json'): continue
            p = os.path.join(dp,f)
            try:
                if os.path.getsize(p) > 20_000_000: continue
                with open(p, encoding='utf-8-sig') as fh:
                    obj = json.load(fh)
            except Exception as e:
                continue
            walk(obj, '', os.path.relpath(p, root))
out = sys.argv[1]
with open(out,'w',encoding='utf-8') as fh:
    json.dump(hits, fh, indent=0)
print(len(hits))
