"""Finalize only after the corrected library actually finishes and audits pass."""
import json,sys,subprocess,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
def read(p):return json.loads(p.read_text(encoding='utf-8-sig'))
assert read(P/'boot-seam-v1/rebuild-status.json')['complete']
for script in ['build_review_catalog.py','audit_skin_rebuild.py','audit_recorded_library.py']:
 subprocess.run([sys.executable,str(P/script)],check=True)
qa=read(P/'quality-review-pass07.json')
qa['corvo_calf_correction']=dict(user_report='Persistent chicken-leg deformation at the boot/trouser seam',cause='Offline calf helpers followed the foot pivot',fix='Shin-relative calf helper anchoring in all review renderers; original weights and animation bytes retained',validation='boot-seam-v1/validation.json',rebuild='boot-seam-v1/rebuild-audit.json',visual_comparison='boot-seam-v1/comparison-grid.jpg',native_capture='boot-seam-native-test-1/result.json',limits='Native axial twist remains approximate; not full movement acceptance',finished=datetime.datetime.now().astimezone().isoformat())
(P/'quality-review-pass07.json').write_text(json.dumps(qa,indent=2))
checkpoint=P/'boot-seam-v1/task-state.json';state=read(checkpoint)
state.update(stage='Calf correction and all 482 regenerated recordings validated',active_exec_session=None,completed_checked=482,remaining=['Final browser refresh and user-facing report; production moveset work remains separate'])
checkpoint.write_text(json.dumps(state,indent=2))
text=(P/'REVIEW-PASS07.txt').read_text();text=text.replace('The final pass archive will be written after the calf-correction recording\nrebuild and delivery audit finish; do not assume it exists until verified.','authoring-pass-07/review-package.zip preserves this reviewed pass and evidence.')
catalog=read(P/'review-catalog.json');covered=sum({e['role']for e in m.get('pose_publication_evidence',[])}=={'corvo','emily'}for m in catalog['motions'])
text=text.replace('103 paired candidates',str(covered)+' paired candidates')
(P/'REVIEW-PASS07.txt').write_text(text)
subprocess.run([sys.executable,str(P/'preserve_authoring_pass.py'),'--name','authoring-pass-07'],check=True)
statepath=P.parent/'current-task-state.json';state=read(statepath);state['CorvoCalfPreviewCorrection']=qa['corvo_calf_correction'];statepath.write_text(json.dumps(state,indent=2))
print('CORVO_CALF_REVIEW_FINALIZED',flush=True)
