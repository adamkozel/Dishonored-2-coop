import json,sys,subprocess
from pathlib import Path
P=Path(__file__).resolve().parent
take=sys.argv[1];root=P/take
rows=json.loads((root/'states.json').read_text(encoding='utf-8-sig'))
out=[]
for armed in (True,False):
 samples=[r for r in rows if r['armed']==armed and r['trial']==1 and r['movement']['traversalWeight']>.7 and r.get('videoTime')]
 assert samples,(take,armed)
 # Harness wall time is an approximate capture index: encoder startup can
 # offset the first frame. Include the lead-in rather than label an image
 # with a native phase that was observed by a separate asynchronous probe.
 start=min(r['videoTime']for r in samples)-.45
 times=[start+.18*f for f in range(8)]
 filename='contact-armed.jpg'if armed else'contact-empty.jpg'
 subprocess.run([sys.executable,str(P/'make_contact_sheet.py'),str(P/f'{take}.mp4'),str(root/filename),*[str(t)for t in times]],check=True)
 out.append(dict(armed=armed,trial=1,video_times=times,selection='Expanded interval around approximate harness wall time; inspect visible action phases directly. Capture startup is not phase-synchronized.',contact=filename))
(root/'contact-times.json').write_text(json.dumps(out,indent=2))
print('CONTACTS',take)
