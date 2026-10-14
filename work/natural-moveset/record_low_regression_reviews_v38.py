import json,datetime
from pathlib import Path
P=Path(__file__).resolve().parent
for role,stance in [('corvo','standing'),('emily','crouched')]:
 p=P/f'native-low-drop-{role}-regression-v38';assert(p/'release-sheet.jpg').exists()
 (p/'visual-review.json').write_text(json.dumps(dict(inspected=True,visual_accepted=False,complete=False,
  inspected_at=datetime.datetime.now().astimezone().isoformat(),method='Actual-game release contact sheet viewed at 6.2, 6.7, 6.95, 7.2, 7.6, 8.4 seconds',evidence='release-sheet.jpg',
  notes=f'{role.title()} {stance} low-drop regression retains bounded fall and recovery. No compact-release upright pop in this low-drop case. Hand contact and full carry quality remain unaccepted.'),indent=2))
