"""Record human-visible frame inspections actually completed during this turn."""
import json,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
notes={
'native-fetal-release-corvo-reader-v35':'Native compact source action observed; remote binding absent in v34. Not an accepted paired release.',
'native-fetal-release-corvo-v35':'Native source selected high throw, not compact release. Carrier clips observer edge; lease expires during delayed camera setup.',
'native-fetal-release-corvo-turn-v35':'Lifecycle checks pass; carrier mostly outside observer framing. No reliable full-body visual coverage.',
'native-fetal-release-emily-turn-v35':'Lifecycle checks pass; carrier mostly outside observer framing. No reliable full-body visual coverage.',
'native-fetal-release-corvo-framed-v35':'Useful full-body framing reveals victim straightening above carrier before falling; weak hand contact. Reject visual quality.',
'native-fetal-release-emily-framed-v35':'Useful full-body framing reveals the same upright victim pop, falling across the front with poor hand contact. Reject visual quality.',
'native-fetal-release-corvo-blend-v36':'Release snapshot experiment fails: no sampled release-scene activation and body exits frame above carrier. Trajectory exceeds bounded validator. Reject build for this path.'}
for take,note in notes.items():
 f=P/take;assert(f/'release-sheet.jpg').exists()
 (f/'visual-review.json').write_text(json.dumps(dict(inspected=True,visual_accepted=False,
  inspected_at=datetime.datetime.now().astimezone().isoformat(),method='Viewed actual-game contact sheet at 8.3, 8.7, 8.9, 9.1, 9.4, 10.4 seconds',
  evidence='release-sheet.jpg',notes=note,complete=False),indent=2))
print(json.dumps(dict(recorded=len(notes),accepted=0)))
