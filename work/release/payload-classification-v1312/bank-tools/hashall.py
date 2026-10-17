import glob, os, json, sys, hashlib
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from parse import parse
P=r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset"
out={}
files=sorted(glob.glob(os.path.join(P,'*-banks-v*','*.bin'))+glob.glob(os.path.join(P,'native-gaits','*-gait-bank.bin')))
for p in files:
    rel=os.path.relpath(p,P)
    try:
        r=parse(p)
    except Exception as e:
        print('ERR',rel,e,flush=True); continue
    sha=hashlib.sha256(open(p,'rb').read()).hexdigest().upper()
    r['sha256']=sha
    r['magic']=r['magic'].decode('latin1')
    out[rel]=r
    print(rel, r['magic'], r['count'], sha[:12], flush=True)
json.dump(out, open(sys.argv[1],'w'), indent=0)
