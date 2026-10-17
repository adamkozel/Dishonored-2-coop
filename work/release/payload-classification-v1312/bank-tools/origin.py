import json, os
H=json.load(open('bankhashes.json'))
CHAIN="""native-gaits stance-banks-v1 sprint-banks-v1 lean-banks-v1 slide-banks-v1 air-banks-v1 combat-banks-v1 equipment-banks-v3 unarmed-special-banks-v1 vertical-peek-banks-v1 low-mantle-banks-v1 low-mantle-banks-v2 crouch-mantle-banks-v1 vault-banks-v1 power-banks-v1 air-combat-banks-v1 terrain-banks-v2 turn-banks-v1 crouch-turn-banks-v1 high-mantle-banks-v1 double-jump-banks-v1 hard-landing-banks-v1 hard-landing-crouch-banks-v1 keyhole-banks-v2 darkvision-banks-v1 darkvision-context-banks-v1 windblast-banks-v1 windblast-context-banks-v1 crouch-balance-banks-v1 balanced-crouch-power-banks-v1 balanced-crouch-power-banks-v2 balanced-crouch-power-banks-v3 ranged-banks-v1 crouched-ranged-banks-v3 crouched-ranged-banks-v4 carry-banks-v1 carry-release-banks-v1 carry-contact-banks-v2 carry-pickup-banks-v1 carry-pickup-contact-banks-v5 crouched-pickup-banks-v1 crouched-pickup-banks-v3 high-throw-banks-v31 fetal-banks-v35 bendtime-banks-v40 bendtime-balanced-banks-v42 outofmana-banks-v45 outofmana-context-banks-v46 grenade-banks-v56 stun-banks-v61 crouched-grenade-banks-v62 mesmerize-banks-v77 mesmerize-context-banks-v132 domino-banks-v192 domino-context-banks-v243 swarm-banks-v267 swarm-context-banks-v272 possession-banks-v323 possession-banks-v334 possession-crouch-banks-v363 possession-crouch-release-banks-v371 doppelganger-banks-v407 doppelganger-crouch-banks-v490 doppelganger-holstered-banks-v495 shadowwalk-banks-v523 shadowwalk-sneak-banks-v548 heart-banks-v564 trap-banks-v569 trap-banks-v575 choke-banks-v581 choke-breakout-banks-v592 lethal-banks-v604 kick-banks-v625 heavy-banks-v633 parry-banks-v1022 swim-banks-v1037 climb-banks-v1042 use-banks-v1212""".split()
# Declared-parent corrections (manifests' parent fields): balanced-crouch-power v1/v2/v3 are
# siblings built from crouch-balance; v3 is the one ranged-v1 reads. carry-pickup-contact v5
# was built from v3 (corvo) / v4 (emily), each built from carry-pickup-banks-v1.
def chain_for(role):
    c=[d for d in CHAIN if d not in ('balanced-crouch-power-banks-v1','balanced-crouch-power-banks-v2')]
    i=c.index('carry-pickup-contact-banks-v5')
    c.insert(i,'carry-pickup-contact-banks-v3' if role=='corvo' else 'carry-pickup-contact-banks-v4')
    return c
res={}
for role in ('corvo','emily'):
    seq=[]
    for d in chain_for(role):
        ks=[k for k in H if k.split(chr(92))[0]==d and os.path.basename(k).startswith(role)]
        assert len(ks)==1,(d,role,ks)
        seq.append((d,H[ks[0]]))
    final=[c['ph'] for c in seq[-1][1]['clips']]
    out=[]
    for i,ph in enumerate(final):
        # last change point: earliest index j such that for all k>=j clip i exists and equals ph
        j=len(seq)-1
        while j-1>=0 and len(seq[j-1][1]['clips'])>i and seq[j-1][1]['clips'][i]['ph']==ph:
            j-=1
        # first appearance of index i
        first=next(k for k,(d,b) in enumerate(seq) if len(b['clips'])>i)
        # any intermediate modifications?
        mods=[seq[k][0] for k in range(first+1,len(seq)) if seq[k][1]['clips'][i]['ph']!=seq[k-1][1]['clips'][i]['ph']]
        out.append(dict(clip=i,origin=seq[j][0],first=seq[first][0],modified_by=mods,ph=ph,f1=seq[-1][1]['clips'][i]['f1'],f2=seq[-1][1]['clips'][i]['f2']))
    res[role]=out
    # summary
    from collections import OrderedDict
    g=OrderedDict()
    for o in out: g.setdefault(o['origin'],[]).append(o['clip'])
    print('=====',role)
    for k,v in g.items(): print(k, v)
    print('modified:', [(o['clip'],o['first'],o['modified_by']) for o in out if o['modified_by']])
# also verify chain consistency: each step only appends or changes recorded idx
json.dump(res,open('origins.json','w'),indent=0)
same=[i for i in range(374) if res['corvo'][i]['origin']==res['emily'][i]['origin']]
print('same origin both roles:',len(same))
print('diff:',[(i,res['corvo'][i]['origin'],res['emily'][i]['origin']) for i in range(374) if res['corvo'][i]['origin']!=res['emily'][i]['origin']])
