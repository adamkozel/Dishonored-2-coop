import json, os, sys, pickle
idx = pickle.load(open('idx.pkl','rb'))
rows=[]
seen=set()
for sha, ents in idx.items():
    for fn,path,obj in ents:
        if 'version' in obj and ('clips' in obj or 'clipCount' in obj or 'clip_count' in obj) :
            key=(fn,path)
            if key in seen: continue
            seen.add(key)
            rows.append((obj.get('role'), obj.get('version'), obj.get('clips', obj.get('clipCount', obj.get('clip_count'))), fn, path, sha[:12], sorted(k for k in obj.keys())))
def vkey(r):
    v=r[1]
    return (str(r[0]), v if isinstance(v,int) else -1, str(r[3]))
rows.sort(key=vkey)
for r in rows:
    if 'manifest' in r[3] or True:
        print(r[0], r[1], r[2] if not isinstance(r[2],list) else f'list{len(r[2])}', r[3]+r[4], r[5])
