"""Offline hand/holster distances; proximity is a candidate, not contact acceptance."""
import sys,json
from pathlib import Path
P=Path(__file__).resolve().parent;sys.path.insert(0,str(P))
from clip_io import *
L=Library(['library-retarget-manifest.json'],'equipment-contact-v1');report=[]
for role in ('corvo','emily'):
 rig=rig_for(role)
 for alias in ('sword_equipsword','sword_unequipsword'):
  m,fs=L.load(role,alias);samples=[]
  for f,p in enumerate(fs):
   g=rig.fk(p);samples.append(dict(frame=f,time=f/m['fps'],hand_to_holster_m=(g[16].translation-g[195].translation).length,tag_to_holster_m=(g[17].translation-g[195].translation).length))
  best=min(samples,key=lambda x:x['tag_to_holster_m']);print(role,alias,best)
  report.append(dict(role=role,alias=alias,source_sha256=m['sha256'],duration=m['duration'],nearest=best,samples=samples,accepted=False,scope='Relative native-model joints in authored body pose, excluding weapon geometry and attachment offset. No native event timing or usable contact is established by proximity.'))
(P/'equipment-contact-v1/contact.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
