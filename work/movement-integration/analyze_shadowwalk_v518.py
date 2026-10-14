from pathlib import Path
import json,collections,math
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset/native-shadowwalk-standing-v516b'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
rows=rd(N/'states.json');groups={};edges=[];previous=None
for r in rows:
    words={x['offset']:int(x['word'],16) for x in r['component'].get('words',[])}
    phase=words.get('0xe0',0)>>32
    active=tuple((x['id'],x['vtableRva']) for x in r['player']['active'])
    key=(phase,r['player']['stance'],active)
    if key!=previous:edges.append(dict(time=r['videoTime'],phase=phase,stance=r['player']['stance'],states=active,position=r['player']['position'],scenes=[x['asset'] for x in r['midnight'].get('instances',[]) if x['state']==3 and 'shadow_walk' in x['asset']]))
    previous=key;groups.setdefault(phase,[]).append(r)
stats=[]
for phase,rs in groups.items():
    stats.append(dict(phase=phase,samples=len(rs),first=rs[0]['videoTime'],last=rs[-1]['videoTime'],stances=sorted(set(x['player']['stance'] for x in rs)),itemReaderFailures=sum(x['power'].get('stable') is False for x in rs),states=sorted(set(tuple(x['player']['active'][i]['id'] for i in range(len(x['player']['active']))) for x in rs))))
scenes=[json.loads(x) for x in (N/'midnight.ndjson').read_text().splitlines()]
by_scene={}
for r in scenes:
    for x in r.get('instances',[]):
        if x['state']==3 and 'power_shadow_walk' in x['asset'] and x['asset'] not in by_scene:by_scene[x['asset']]=x
result=dict(phases=stats,edges=edges,representativeScenes=by_scene,note='Observational correlation only. Component+0xe4 interpreted as high32 of aligned word+0xe0, exact native object type retained. One normal standing native activation take.')
(R/'work/movement-integration/native-shadowwalk-correlation-v518.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
print(json.dumps(dict(phases=stats,edges=edges,sceneBindings={k:v.get('bindingsResearch') for k,v in by_scene.items()}),indent=2))
