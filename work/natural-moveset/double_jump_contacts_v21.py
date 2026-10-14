import json,sys,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
take=sys.argv[1];root=P/take
rows=json.loads((root/'states.json').read_text(encoding='utf-8-sig'));out=[]
for armed in (True,False):
 for moving in (False,True):
  samples=[r for r in rows if r['armed']==armed and r['moving']==moving and r['movement'].get('secondJump') and r.get('videoTime')]
  if not samples:continue
  start=min(r['videoTime']for r in samples)-.65
  times=[max(0,start+.24*f)for f in range(10)]
  filename=f"contact-{'armed'if armed else'empty'}-{'moving'if moving else'standing'}.jpg"
  subprocess.run([sys.executable,str(P/'make_contact_sheet.py'),str(P/f'{take}.mp4'),str(root/filename),*[str(t)for t in times]],check=True)
  out.append(dict(armed=armed,moving=moving,video_times=times,selection='Expanded approximate wall-time interval; video frames show actual phases. Encoder startup and status publication are not phase-synchronized.',contact=filename))
(root/'contact-times.json').write_text(json.dumps(out,indent=2));print('CONTACTS',take,len(out))
