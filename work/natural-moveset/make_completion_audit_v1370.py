"""Writes completion-audit-v1370-bend-time.json (#11): Corvo's Bend Time cast and end presented on the partner
(coverage-v1359d on 1214cz)."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("Bend Time is Corvo's power only. The coverage harness grants it by name (lab-only grant), and the source casts it "
     "(LT at 2.6 s) and ends it (LT again at 6.2 s) with the product controller in the Dunwall tower safe room. The source "
     "reads the native start and release scene clocks and sends power variants "
     "12 and 14, which the partner presents with the authored left-hand cast and its recovery (visual sweep 1260 accepted "
     "the standing and crouched casts; v42 crouched shoulders, v44 airborne). The case passes when the partner presentation "
     "stays live, the power is presented at full weight and the partner recovers to locomotion afterwards. The world "
     "time effect itself is gameplay, not body motion; the out-of-mana response belongs to power_interruptions_and_no_mana.")
items = [
 dict(id='corvo_bend_time_cast_end', decision='complete',
      scope="Corvo's Bend Time cast and its end on the partner body, on the native scene clocks, recovering to locomotion.",
      takes=[{'take': 'coverage-v1359d', 'cases': ['host-bendtime']}],
      visual=[{'video': 'coverage-v1359d/host-bendtime-observer.mp4',
               'verdict': 'Corvo raises his left hand palm-out for the cast, holds it for about half a second and lowers it with the sword still in his right hand; the second press ends the power with the same gesture, and he settles back into his standing idle each time.'}]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1370-bend-time', family='powers', candidate='candidate-npc-combat-v1214cz', method=M, items=items)
(N / 'completion-audit-v1370-bend-time.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
