"""Writes completion-audit-v1343-lever.json (#11): the Edge of the World turbine lever on candidates 1214cn/cp."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decision rests on the use-variant takes in Edge of the World under the product controller, the observer's free camera "
     "held beside the mechanism: use-variants-eotw-v1228v (Corvo, 1214cn) and use-variants-eotw-v1228w (Emily, 1214cp). The "
     "source uses the real turbine lever with native input (native Interact state 75 on the focused mechanism); the partner "
     "is checked from its own use presentation (the lever variant) on the observer. The Dunwall sink tap (a valve, the same use "
     "family) passes in both roles in use-variants-dunwall-v1228v. The mechanism's own state is world state (stage 5).")
items = [
 dict(id='lever_button_crank_wheel', decision='complete',
      scope='Pulling the turbine lever in both roles and turning the sink tap in both roles: the partner steps in, reaches to the mechanism, operates it and recovers to standing. Cranks and wheels share the use presentation family (use variants 2-4 by mechanism name).',
      takes=[{'take': 'use-variants-eotw-v1228v', 'cases': ['host-script_lever_windturbine_1_1_1']}, {'take': 'use-variants-eotw-v1228w', 'cases': ['client-script_lever_windturbine_1_1_1']},
             {'take': 'use-variants-dunwall-v1228v', 'cases': ['host-esc_env_rich_sink_tap_01_1', 'client-esc_env_rich_sink_tap_01_1']}],
      visual=[{'video': 'use-variants-eotw-v1228v/use-host-script_lever_windturbine_1_1_1-observed.mp4', 'verdict': 'Corvo raises his arm to the lever on the turbine platform, pulls it down and stands back as the blades turn.'},
              {'video': 'use-variants-eotw-v1228w/use-client-script_lever_windturbine_1_1_1-observed.mp4', 'verdict': 'Emily turns to the lever post, works it with her hand and returns to standing.'},
              {'video': 'use-variants-dunwall-v1228v/use-client-esc_env_rich_sink_tap_01_1-observed.mp4', 'verdict': 'Emily reaches up to the tap on the wall and turns it, then lowers her arm.'}],
      lifecycle='See method.'),
]
audit = dict(audit='completion-audit-v1343-lever', family='interactions', candidate='candidate-npc-combat-v1214cp', method=M, items=items)
(N / 'completion-audit-v1343-lever.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
