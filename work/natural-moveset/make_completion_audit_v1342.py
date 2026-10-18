"""Writes completion-audit-v1342-interactions.json (#11): note reading and eating on candidates 1214cn/cp."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("Each decision rests on interaction takes in the Dunwall safe room under the product controller, both source roles: "
     "the source uses the real native object with native input (the note through the native read state, food through the "
     "native LootObject47 pickup), the partner is checked from its own use presentation on the observer and the checkpoint "
     "is reloaded after every case; reviewed observer videos are listed per item. The objects themselves (the item taken, "
     "the note read) are world and inventory state: stage 5.")
def T(take, cases): return {'take': take, 'cases': [f'{r}-{c}' for r in ('host', 'client') for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
items = [
 dict(id='read_object_or_note', decision='complete',
      scope='Reading the table note in both roles: while the source reads, the partner holds the note out in a held reach and returns to its idle when the source closes it (coverage-v1318i on 1214cn; coverage-v1286e earlier).',
      takes=[T('coverage-v1318i', ['note'])],
      visual=[V('coverage-v1318i/client-note-observer.mp4', 'Emily lifts her left hand and holds the note out in front of her while she reads, then lowers it and stands at ease.')]),
 dict(id='eat_drink', decision='complete',
      scope='Eating native food (grapes) in both roles: the native pickup presents the use reach toward the plate on the partner, and the item is taken. D2 eats food instantly on pickup without a first-person eating motion, so the reach is the faithful gesture. Drinking elixirs is presented hand to mouth (health_elixir).',
      takes=[T('gestures-v1261d', ['food'])],
      visual=[V('gestures-v1261d/client-food-observer.mp4', 'Emily steps to the table and reaches down to the plate, then straightens.')]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1342-interactions', family='interactions', candidate='candidate-npc-combat-v1214cp', method=M, items=items)
(N / 'completion-audit-v1342-interactions.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
