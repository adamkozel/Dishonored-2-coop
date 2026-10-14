import json,sys,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent;take=sys.argv[1];root=P/take
rows=json.loads((root/'states.json').read_text(encoding='utf-8-sig'));out=[]
for armed in (True,False):
 for repeat in (1,2):
  samples=[r for r in rows if r['armed']==armed and r['repeat']==repeat and r['movement']['traversalVariant']==6 and r['movement']['traversalWeight']>.9 and r.get('videoTime')]
  if not samples:continue
  start=min(r['videoTime']for r in samples)-.65
  times=[max(0,start+.18*f)for f in range(10)]
  filename=f"contact-{'armed'if armed else'empty'}-{repeat}.jpg"
  subprocess.run([sys.executable,str(P/'make_contact_sheet.py'),str(P/f'{take}.mp4'),str(root/filename),*[str(t)for t in times]],check=True)
  out.append(dict(armed=armed,repeat=repeat,video_times=times,selection='Expanded approximate wall-time interval; video frames show actual phases. Encoder startup and status publication are not phase-synchronized.',contact=filename))
(root/'contact-times.json').write_text(json.dumps(out,indent=2),encoding='utf-8');print('CONTACTS',take,len(out))
