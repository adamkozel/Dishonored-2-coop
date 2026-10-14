from pathlib import Path
import json
P=Path(__file__).resolve().parents[1]/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
for stance,ground in [('crouched',6),('standing',0)]:
 t=P/f'domino-holstered-{stance}-final-observe-v263'
 if not(t/'result.json').exists():continue
 result=read(t/'result.json');raw=read(t/'crouched-final-raw-proof.json');rows=read(t/'states.json')
 assert result['passed'] and all(not x['sword']['equipped']for x in rows)
 final=[]
 for x in rows:
  for scene in x['midnight']['instances']:
   if scene.get('state')==3 and scene.get('subscene')=='Cast_final_Select':
    final.append(dict(timeMs=scene['clockWords'][1],durationMs=scene['sceneDurationMs'],states=[a['id']for a in x['player']['active']],holstered=not x['sword']['equipped'],scene=scene))
 assert len(raw)>=8 and all(x['sceneDurationMs']==400 for x in raw)
 assert all(len(x['bindingsResearch'])==2 and all(b['reciprocal']for b in x['bindingsResearch'])for x in raw)
 out=dict(take=t.name,stance=stance,holstered=True,rawFinalSamples=len(raw),rawClockMs=[min(x['clockWords'][1]for x in raw),max(x['clockWords'][1]for x in raw)],independentFinalRows=final,groundState=ground,passed=result['passed'],complete=False,scope='Native source provenance. Independent state/scene probes are sequential, not atomic. Reader must keep reciprocal ownership, equipment rereads, exact ground state and400ms bound. No peer proof in this observation.')
 (t/'final-context-provenance.json').write_text(json.dumps(out,indent=2))
 print(json.dumps({k:v for k,v in out.items()if k not in ('independentFinalRows','scope')}|{'independentFinalStates':[{k:v for k,v in x.items()if k!='scene'}for x in final]}))
