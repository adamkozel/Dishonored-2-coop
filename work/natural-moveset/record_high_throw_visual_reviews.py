"""Persist the actual temporal-sheet reviews performed in the development chat."""
import json
from pathlib import Path
P=Path(__file__).resolve().parent
for take in ['native-high-throw-emily-v31','native-high-throw-emily-v32','native-high-throw-corvo-v32']:
    f=P/take
    assert (f/'throw-sheet.jpg').exists()
    report=dict(inspected=True,accepted=False,complete=False,method='Actual D2 temporal sheet at 8.2,8.6,8.8,9.0,9.3,10.4 seconds',
        sheet='throw-sheet.jpg',observations='Native corpse raises from the supported shoulder, remains suspended during release, then drops beside the carrier without the required forward throw impulse. Hand/body alignment and recovery are not visually accepted. The health, equipment and cleanup checks do not establish satisfactory throw motion.')
    (f/'visual-review.json').write_text(json.dumps(report,indent=2))
