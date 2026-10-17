import json, os, re, pickle, hashlib, functools
P=r"C:\Users\adako\Documents\Codex\2026-10-08\i-x20\work\natural-moveset"
T=os.path.dirname(os.path.abspath(__file__))
HEX=re.compile(r'^[0-9a-fA-F]{64}$')
DON=pickle.load(open(os.path.join(T,'donoridx.pkl'),'rb'))
BINH=json.load(open(os.path.join(T,'binhash.json')))
GLOB=None
def glob_idx():
    global GLOB
    if GLOB is None: GLOB=pickle.load(open(os.path.join(T,'idx.pkl'),'rb'))
    return GLOB
TIMING_KEYS={'native_take','native_scene','scene','native_release_ms','native_take_passed','native_release_reader_verified','subscene','scene_sha256'}
def rank(fn):
    b=os.path.basename(fn)
    if b.startswith('skin-review') or b.startswith('rejected'): return 3
    if 'retarget' in b or 'manifest' in b: return 0
    return 1
def lookup(sha):
    sha=sha.lower()
    if sha in DON:
        ents=sorted(DON[sha],key=lambda fe:(0 if fe[1].get('source') is not None else 1, rank(fe[0])))
        return ents[0]
    g=glob_idx().get(sha.upper())
    if g:
        g=sorted(g,key=lambda t:(0 if ('source' in t[2] or 'selection' in t[2]) else 1, rank(t[0])))
        fn,path,obj=g[0]
        if 'source' in obj or 'selection' in obj or 'binary' in obj or 'asset' in obj:
            return (fn+path,obj)
    # file hash
    files=BINH.get(sha)
    if files:
        for rel in files:
            side=os.path.join(P,os.path.splitext(rel)[0]+'.json')
            if rel.endswith('.edgepose'):
                return ('FILE:'+rel,{'source':{'asset':'(native edgepose sample) '+rel+'.md6anim'}})
            if rel.startswith('native-retargeted'):
                return ('FILE:'+rel,{'source':{'asset':'(native-retargeted) '+rel+'.md6anim'}})
            if os.path.exists(side):
                try:
                    o=json.load(open(side,encoding='utf-8-sig'))
                    if isinstance(o,dict): return ('SIDECAR:'+os.path.relpath(side,P),o)
                except Exception: pass
        return ('FILE:'+files[0],{})
    return None
def scan(obj, key, out):
    if isinstance(obj,dict):
        a=obj.get('asset')
        if isinstance(a,str) and a.lower().endswith('.md6anim'):
            out['leaves'].add(('native-sampled',a)); return
        if isinstance(a,str) and a.lower().endswith('.fbx'):
            out['leaves'].add(('mixamo' if ('mixamo' in a.lower() or 'pack' in obj or 'download' in obj) else 'fbx-unknown', os.path.basename(a)+(' ['+str(obj.get('pack'))+']' if obj.get('pack') else ''))); return
        for k,v in obj.items():
            if k in TIMING_KEYS: out['timing'].add(k)
            scan(v, k, out)
    elif isinstance(obj,list):
        for v in obj: scan(v,key,out)
    elif isinstance(obj,str):
        if HEX.match(obj): out['links'].add((key,obj.lower()))
        elif obj.endswith('.json') and 'mixamo' in obj.lower(): out['leaves'].add(('mixamo','via '+obj))
        elif 'md6anim' in obj.lower(): out['leaves'].add(('native-sampled',obj))
@functools.lru_cache(maxsize=None)
def resolve(sha, key='', depth=0):
    sha=sha.lower()
    hit=lookup(sha)
    if hit is None:
        if 'native' in key.lower() or 'scene' in key.lower():
            return frozenset({('native-sampled',f'(unresolved {key}={sha[:12]}, key says native)')}), ()
        return frozenset({('unresolved',f'{key}={sha}')}), ()
    fn,e=hit
    txt=json.dumps(e).lower()
    leaves=set(); notes=[f"{fn}:{e.get('id') or e.get('binary') or ''}"]
    if 'animgen' in txt: leaves.add(('animgen',fn))
    src=e.get('source')
    out={'links':set(),'leaves':set(),'timing':set()}
    if src is not None: scan(src,'source',out)
    for k in ('parent_sha256','previous_sha256','donor_sha256','base_sha256'):
        if isinstance(e.get(k),str) and HEX.match(e[k]): out['links'].add((k,e[k].lower()))
    out['links']={(k,s) for k,s in out['links'] if s!=sha and k not in ('asset_sha256','skeleton_sha256','animation_sha256','sample_sha256')}
    leaves|=out['leaves']
    if not out['links'] and not out['leaves']:
        leaves.add(('authored-procedural',fn))
    if depth>30: return frozenset(leaves|{('unresolved','depth')}), tuple(notes)
    for k,l in sorted(out['links']):
        lv,_=resolve(l,k,depth+1)
        leaves|=lv
    return frozenset(leaves), tuple(notes)
def file_sha(rel):
    p=os.path.join(P,rel)
    if not os.path.exists(p): return None
    return hashlib.sha256(open(p,'rb').read()).hexdigest()
def summarize(leaves):
    cats={}
    for c,l in leaves: cats.setdefault(c,set()).add(l)
    return {c:sorted(v) for c,v in cats.items()}
if __name__=='__main__':
    import sys
    for a in sys.argv[1:]:
        s=a.lower() if HEX.match(a) else file_sha(a)
        lv,notes=resolve(s)
        print(a, notes, json.dumps(summarize(lv),indent=0)[:3000])
