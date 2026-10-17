import json, os, glob, re, pickle, collections
P=r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset"
HEX=re.compile(r'\b[0-9a-fA-F]{64}\b')
entries={}   # sha -> list of (file, entry)
srckeys=collections.Counter()
files=glob.glob(os.path.join(P,'*.json'))
for p in files:
    try: d=json.load(open(p,encoding='utf-8-sig'))
    except Exception: continue
    items = d if isinstance(d,list) else (d.get('clips') or d.get('entries') or d.get('items') or [d]) if isinstance(d,dict) else []
    if not isinstance(items,list): items=[d]
    for e in items:
        if not isinstance(e,dict): continue
        s=e.get('sha256')
        if isinstance(s,str) and len(s)==64:
            entries.setdefault(s.lower(),[]).append((os.path.basename(p),e))
            if isinstance(e.get('source'),dict):
                for k in e['source']: srckeys[(os.path.basename(p).split('-manifest')[0],k)]+=1
pickle.dump(entries,open('donoridx.pkl','wb'))
print(len(files),len(entries))
byfile=collections.defaultdict(set)
for (f,k),c in srckeys.items(): byfile[f].add(k)
for f,ks in sorted(byfile.items()): print(f, sorted(ks))
