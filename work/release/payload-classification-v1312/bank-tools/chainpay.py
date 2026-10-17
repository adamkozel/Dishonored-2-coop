import json, os
H=json.load(open('bankhashes.json'))
for role in ('corvo','emily'):
    banks={k:v for k,v in H.items() if os.path.basename(k).startswith(role)}
    byv={}
    for k,v in banks.items(): byv.setdefault(v['v'],[]).append(k)
    print('=====',role)
    for k,v in sorted(banks.items(), key=lambda kv:(kv[1]['v'],kv[0])):
        ph=[c['ph'] for c in v['clips']]
        best=[]
        for k2,v2 in banks.items():
            if k2==k: continue
            if v2['v'] not in (v['v'],v['v']-1): continue
            ph2=[c['ph'] for c in v2['clips']]
            m=sum(1 for a,b in zip(ph,ph2) if a==b)
            changed=[i for i,(a,b) in enumerate(zip(ph,ph2)) if a!=b]
            best.append((m,k2,changed,len(ph2)))
        best.sort(key=lambda x:-x[0])
        s=f"v{v['v']} {k} n={len(ph)} sha={v['sha256'][:12]}"
        for m,k2,ch,n2 in best[:3]:
            s+=f"\n     <- {k2} (n={n2}) same={m} changed={ch[:20]}{'...' if len(ch)>20 else ''}"
        print(s)
