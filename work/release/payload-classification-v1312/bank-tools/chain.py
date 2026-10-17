import json, os, sys, pickle
idx = pickle.load(open('idx.pkl','rb'))
SRC_KEYS = ('source_bank_sha256','sourceBankSha256','source_sha256','sourceSha256','base_bank_sha256','baseBankSha256','sourceBankSHA256','source')
starts = {'corvo':'51C2DA55723BD0E9D41E00D7EB373032750EAEC617B2143BFC91C71FF58F7F18','emily':'AC5831282A09EC809FB6826ADFA73B97766774D2B675C5237438238AB149583E'}
for role, sha in starts.items():
    print('=====', role)
    seen=set()
    while sha and sha not in seen:
        seen.add(sha)
        ents = idx.get(sha, [])
        # prefer entries that look like bank manifest entries (have source key or clips)
        cands = [e for e in ents if any(k in e[2] for k in SRC_KEYS) or 'clips' in e[2] or 'added' in e[2]]
        if not cands:
            print(sha, 'NO MANIFEST ENTRY; refs:', sorted(set(e[0] for e in ents))[:10])
            break
        # choose manifest-like file names first
        cands.sort(key=lambda e: (0 if 'manifest' in e[0] else 1, len(e[0])))
        fn,path,obj = cands[0]
        src = None
        for k in SRC_KEYS:
            if k in obj and isinstance(obj[k],str) and len(obj[k])==64: src=obj[k].upper(); break
        added = obj.get('added')
        addinfo = (f'list{len(added)} clips {[a.get("clip") for a in added if isinstance(a,dict)]}' if isinstance(added,list) else repr(added)[:80])
        print(f"{obj.get('version')} | {fn}{path} | clips={obj.get('clips')} | role={obj.get('role')} | src={src} | added={addinfo}")
        others = sorted(set(e[0] for e in cands[1:] if 'manifest' in e[0]))
        if others: print('   also in:', others[:6])
        sha = src
