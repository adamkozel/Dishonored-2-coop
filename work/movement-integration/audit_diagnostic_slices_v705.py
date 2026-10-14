from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
cases=[]
for v,role,candidate in [(701,'host','candidate-native-timing-v700'),(703,'host','candidate-diagnostic-slices-v702'),(704,'client','candidate-diagnostic-slices-v702')]:
 take=f'native-interactions-{"corvo" if role=="host" else "emily"}-crouch-coin-v{v}'
 f=N/take;o=rd(f/'coin-observations.json');r=rd(f/'result.json');w=rd(f/'coin-state-transitions.json');pin=rd(W/candidate/'identity.json')
 checks=[];failures=[]
 def require(ok,label):(checks if ok else failures).append(label)
 require(r['passed'] and r['dll']==pin['dll'] and r['source']==role,'Exact candidate completed and restored both inventories,health and14 protected files')
 require(w['readOnly'] and w['samples']==w['validSamples'] and w['samples']>=500,'Compact native states valid throughout')
 loot=[x for x in w['records'] if any(a['id']==47 for a in x['active'])]
 require(loot and all(x['stance']==1 and x['defaultMode'] and {6,47}<={a['id'] for a in x['active']} for x in loot),'Exact native crouched loot activated')
 require(all(x['motionValid'] and x['motion']==1 for x in w['records']),'Crouch classification continuous')
 require(rd(f/'coin-target-after.json')==[],'Native source coin removed')
 require(all(x['sourceStream']['ready'] and x['stream']['ready'] for x in o),'Both Steam streams remain ready')
 require(all(x['movement']['active'] and x['movement']['weight']>.999 for x in o),'Remote body fully weighted throughout capture')
 require(all(abs(x['movement']['stanceMix']-1)<.001 for x in o),'Remote crouch retained')
 timing={}
 for side in ['sourceTiming','peerTiming']:
  a=o[0][side];b=o[-1][side];elapsed=b['tick']-a['tick'];stages=[]
  for sb in b['stages']:
   sa=next((s for s in a['stages'] if s['name']==sb['name']),{'count':0,'totalMs':0,'slow':0})
   count=sb['count']-sa['count'];total=sb['totalMs']-sa['totalMs']
   stages.append(dict(name=sb['name'],calls=count,totalMs=round(total,3),meanMs=round(total/count,3) if count else 0,over50ms=sb['slow']-sa['slow']))
  events={(e['name'],e['tick'],e['ms']):e for x in o for e in x[side]['events'] if a['tick']<=e['tick']<=b['tick']}
  timing[side]=dict(elapsedMs=elapsed,stages=stages,events=list(events.values()))
  if v!=701:
   require(next(s for s in stages if s['name']=='status_all')['over50ms']==0,f'{side}: no diagnostic status call reaches50ms during measurement')
   require(next(s for s in stages if s['name']=='diagnostic_slice')['over50ms']==0,f'{side}: no staggered diagnostic slice reaches50ms')
 cases.append(dict(take=take,candidate=candidate,passed=not failures,checks=checks,failures=failures,samples=len(o),compactSamples=w['samples'],minimumBodyWeight=min(x['movement']['weight'] for x in o),timing=timing))
audit=dict(at=datetime.datetime.now().astimezone().isoformat(),passed=all(x['passed'] for x in cases[1:]),checks=sum(len(x['checks']) for x in cases),failedChecks=sum(len(x['failures']) for x in cases),cases=cases,complete=False,scope='Measured game-thread diagnostic hitch reduction and unchanged native crouched pickup/body continuity. Does not prove absence of all data gaps or automatic recovery after a retired stream. No network deadlines relaxed.')
(W/'diagnostic-slices-audit-v705.json').write_text(json.dumps(audit,indent=2))
print(json.dumps({k:v for k,v in audit.items() if k!='cases'}))
for c in cases:
 print(json.dumps(dict(take=c['take'],passed=c['passed'],minimumBodyWeight=c['minimumBodyWeight'],failures=c['failures'],timing={side:{'elapsedMs':t['elapsedMs'],'status':next(s for s in t['stages'] if s['name']=='status_all')} for side,t in c['timing'].items()})))
if not audit['passed']:raise SystemExit(1)
