"""Writes completion-audit-v1344-traversal-2.json (#11): hard landing, window, swimming and water exit on 1214cp/cq."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("Each decision rests on Edge of the World takes under the product controller in both source roles, the partner checked "
     "from its own presentation state on the observer and the source's native player states polled: mission-coverage-v1328q "
     "(hard landing, candidate 1214cq), mission-coverage-v1328h and 1328o (swimming, water exit, candidates 1214cp/cq) and "
     "window-traversal-v1235i (the climbable window, 1214cq). Each mission take ends with the lab checkpoint restore and the "
     "protected-file check; reviewed observer videos are listed per item.")
def T(take, cases): return {'take': take, 'cases': [f'{r}-{c}' for r in ('host', 'client') for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
items = [
 dict(id='hard_landing', decision='complete',
      scope='A damaging ~12 m fall onto the dock in both roles (health held): the source enters the native CatFall with the strong landing scene (cat_fall_strong, 1066 ms, bound to the body and both arms), the partner presents it as the hard landing (traversal variant 6, 15 samples) and walks on. The light landing after 5-6 m falls was accepted earlier (audit 1283).',
      takes=[T('mission-coverage-v1328q', ['hardfall'])],
      visual=[V('mission-coverage-v1328q/host-hardfall-observer.mp4', 'Corvo drops onto the dock and lands in a deep crouch with one hand on the planks, holds it a moment, rises and walks on.'),
              V('mission-coverage-v1328q/client-hardfall-observer.mp4', 'Emily lands from the fall in the same braced crouch, pushes up and walks away along the dock.')]),
 dict(id='window_enter_exit', decision='complete',
      scope='Crossing the climbable Edge of the World window in both roles: the native mantle through the window (crossed from one side to the other) is presented on the partner (traversal weight 1, presentation weight 1 throughout).',
      takes=[{'take': 'window-traversal-v1235i', 'cases': ['host-script_rich_small_01_sndctrl', 'client-script_rich_small_01_sndctrl']}],
      visual=[V('window-traversal-v1235i/cross-client-observed.mp4', 'Emily steps up to the window, plants her hands on the sill, swings through the opening and drops out of sight on the far side.')]),
 dict(id='swim_idle_directions', decision='complete',
      scope='Native swimming in both roles: forward, lateral and backward strokes, treading, sinking and rising are presented (about 184 samples per role, strokes 82-98 with 37-39 lateral, sink and rise episodes).',
      takes=[T('mission-coverage-v1328o', ['swim']), T('mission-coverage-v1328h', ['swim'])],
      visual=[V('mission-coverage-v1328o/client-swim-observer.mp4', 'From behind, Emily strokes away through the murky water, slips under and surfaces again, then treads water with her arms out.')]),
 dict(id='dive_surface_water_exit', decision='complete',
      scope='Swimming to the dock and climbing out in both roles: the partner swims, then presents the native ledge exit (traversal variants 1, 4) and stands on land; dives and surfacing are presented within swimming (sink and rise).',
      takes=[T('mission-coverage-v1328h', ['waterexit'])],
      visual=[V('mission-coverage-v1328h/host-waterexit-observer.mp4', 'Corvo swims up to the dock edge, pulls himself up onto it in one movement, stands and steps forward.')]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1344-traversal-2', family='traversal', candidate='candidate-npc-combat-v1214cq', method=M, items=items)
(N / 'completion-audit-v1344-traversal-2.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
