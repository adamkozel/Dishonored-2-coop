import glob,os,hashlib,json
P=r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset"
out={}
for p in glob.glob(os.path.join(P,'*','*.bin'))+glob.glob(os.path.join(P,'*','*.edgepose')):
    rel=os.path.relpath(p,P)
    if '-banks-v' in rel or rel.startswith('native-gaits'): continue
    h=hashlib.sha256(open(p,'rb').read()).hexdigest()
    out.setdefault(h,[]).append(rel)
json.dump(out,open('binhash.json','w'),indent=0)
print(len(out))
