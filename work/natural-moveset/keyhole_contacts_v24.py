"""Extract actual-time keyhole frames, without changing video timing or color."""
import json,sys,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
take=sys.argv[1];root=P/take
rows=json.loads((root/'states.json').read_text(encoding='utf-8-sig'));out=[]
for repeat in (1,2):
 samples=[r for r in rows if r['repeat']==repeat and r.get('videoTime')]
 phases={}
 for r in samples:
  m=r['movement']
  if 7<=m['traversalVariant']<=16 and m['traversalWeight']>.9:
   phases.setdefault(m['traversalVariant'],[]).append(r['videoTime'])
 if len(phases) not in (1,2,3):continue
 order=sorted(phases,key=lambda v:min(phases[v]));entry,leave=order[0],order[-1];hold=order[1]if len(order)==3 else entry
 start=min(phases[entry]);end=max(phases[leave])
 if len(phases)==1:
  cancel=next((r['videoTime'] for r in samples if any(a.get('id')==24 and a.get('peekingResearch',{}).get('scene24')==0 and a.get('peekingResearch',{}).get('phaseEc')==3 for a in r['native']['active'])),end)
  times=[start-.3,start+.1,cancel,cancel+.15,cancel+.35,cancel+.65,cancel+1,cancel+1.6]
 else:times=[start-.3,start+.25,start+.7,min(phases[hold])+.2,max(phases[hold])-.2,min(phases[leave])+.2,end,end+.7]
 name=f'contact-{repeat}.jpg'
 subprocess.run([sys.executable,str(P/'make_contact_sheet.py'),str(P/f'{take}.mp4'),str(root/name),*[str(t)for t in times]],check=True)
 out.append(dict(repeat=repeat,phases=order,video_times=times,contact=name,selection='Approximate wall-time phase brackets; actual video frames remain authoritative. No interpolation, speedup or recoloring.'))
(root/'contact-times.json').write_text(json.dumps(out,indent=2),encoding='utf-8')
print('KEYHOLE_CONTACTS',take,len(out))
