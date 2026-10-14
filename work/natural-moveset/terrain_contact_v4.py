"""Sample native stair traversal while the authored stair gait is engaged."""
import sys,json,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
for take in sys.argv[1:]:
 rows=json.loads((P/take/'states.json').read_text(encoding='utf-8-sig'))
 times=[]
 for trial in ('up','down'):
  samples=[r for r in rows if r['trial']==trial and r['movement']['terrainWeight']>.85 and r['movement']['speed']>.4]
  assert len(samples)>10
  for q in (.15,.45,.75):times.append(samples[round((len(samples)-1)*q)]['videoTime'])
 subprocess.run([sys.executable,str(P/'make_contact_sheet.py'),str(P/f'{take}.mp4'),str(P/take/'contact.jpg'),*map(str,times)],check=True)
 (P/take/'contact-times.json').write_text(json.dumps(dict(times=times,selection='15/45/75 percent of engaged moving stair samples per direction; actual capture timestamps'),indent=2))
