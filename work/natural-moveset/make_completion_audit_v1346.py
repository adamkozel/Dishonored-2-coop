"""Writes completion-audit-v1346-mana.json (#11): the mana elixir on candidate 1214cq."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decision rests on coverage-v1330i on candidate 1214cq under the product controller, both source roles: the source "
     "spends mana with a native power that does not regenerate it (Corvo windblast, Emily Mesmerize, granted by name), the "
     "source's live mana is traced, a mana elixir is granted only after the pool drops (a potion already held is auto-used "
     "by the game, mana-drain-probe-v1344b) and drunk 1.2 s later with the native routine; the partner is checked from its "
     "own use presentation on the observer and the checkpoint is reloaded after the case.")
items = [
 dict(id='mana_elixir', decision='complete',
      scope='Drinking a mana elixir in both roles after a power spent mana (Corvo 100 -> 50, Emily 100 -> 40): the partner presents the drink as the hand-to-mouth use reach (18-19 samples) and the pool is refilled to 100.',
      takes=[{'take': 'coverage-v1330i', 'cases': ['host-manaelixir', 'client-manaelixir']}],
      visual=[{'video': 'coverage-v1330i/client-manaelixir-observer.mp4', 'verdict': 'Emily thrusts her hand out to cast Mesmerize, the spirit forms above her, then she turns and lifts her hand to her mouth to drink before lowering it.'}],
      lifecycle='See method.'),
]
audit = dict(audit='completion-audit-v1346-mana', family='ranged_and_items', candidate='candidate-npc-combat-v1214cq', method=M, items=items)
(N / 'completion-audit-v1346-mana.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
