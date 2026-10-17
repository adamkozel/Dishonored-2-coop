import json, os, sys, pickle
root = r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work"
dirs = [os.path.join(root,'natural-moveset'), os.path.join(root,'movement-integration')]
idx = {}  # sha upper -> list of (file, path, dict)
def walk(obj, path, fn):
    if isinstance(obj, dict):
        for k,v in obj.items():
            if isinstance(v,str) and len(v)==64 and k.lower() in ('sha256','banksha256','bank_sha256','outputsha256','output_sha256','sha'):
                try: int(v,16)
                except: continue
                idx.setdefault(v.upper(),[]).append((fn,path,obj))
        for k,v in obj.items():
            walk(v, path+'/'+str(k), fn)
    elif isinstance(obj, list):
        for i,v in enumerate(obj):
            walk(v, path+f'[{i}]', fn)
n=0
for d in dirs:
    for dp, dn, fns in os.walk(d):
        for f in fns:
            if not f.endswith('.json'): continue
            p = os.path.join(dp,f)
            try:
                if os.path.getsize(p) > 20_000_000: continue
                with open(p, encoding='utf-8-sig') as fh:
                    obj = json.load(fh)
            except Exception:
                continue
            n+=1
            walk(obj, '', os.path.relpath(p, root))
pickle.dump(idx, open(sys.argv[1],'wb'))
print(n, len(idx))
