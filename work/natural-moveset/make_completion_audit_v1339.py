"""Writes completion-audit-v1339-emily-clone-spirit.json (#11, #104): Doppelganger and Mesmerize on candidate 1214cp."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("The decisions rest on coverage-v1332e on candidate 1214cp (Emily as source, product controller, the observer filming "
     "from a wider camera 5.6 m away so the caster and the effect point share the frame), with the body presentation already "
     "passing in coverage-v1332c/d. 1214cp lets the partner world's clone and spirit visuals run in an armed product session "
     "(#104); the host status records the clone created, shown and hidden once with 278 pose updates, and the spirit created, "
     "its native effect started and stopped, and hidden. Both are cosmetic: NPC perception of the clone and the spirit's "
     "mesmerized targets are NPC authority (stage 4, #14). Lifecycle: every case re-arms the product controller after a "
     "checkpoint reload; the 2-hour soak mission-soak-v1316a covers 19 shared restarts and session renewals.")
def T(take, cases): return {'take': take, 'cases': [f'client-{c}' for c in cases]}
def V(video, verdict): return {'video': video, 'verdict': verdict}
E = 'coverage-v1332e'
items = [
 dict(id='emily_doppelganger_cast', decision='complete',
      scope='Emily casts Doppelganger (power variants 29-31): the partner presents the cast, the native summon effect, a separate clone beside her that moves off on its own, and its retirement.',
      takes=[T(E, ['doppelganger']), T('coverage-v1332d', ['doppelganger'])],
      visual=[V(f'{E}/client-doppelganger-observer.mp4', 'A bright summon burst rises beside Emily; a second Emily appears next to her, runs off, and is gone about two seconds later while Emily stands armed.')]),
 dict(id='emily_mesmerize_cast', decision='complete',
      scope='Emily casts Mesmerize without targets (power variants 16-18): the partner presents the enter, aim and release gestures and the native spirit appearing ahead of her, then its dismissal. The mesmerized behaviour of NPC targets is stage 4.',
      takes=[T(E, ['mesmerize']), T('coverage-v1332d', ['mesmerize'])],
      visual=[V(f'{E}/client-mesmerize-observer.mp4', 'Emily thrusts her hand forward; a dark, tendrilled spirit forms a few metres ahead of her and hovers there for the rest of the cast.')]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1339-emily-clone-spirit', family='powers', candidate='candidate-npc-combat-v1214cp', method=M, items=items)
(N / 'completion-audit-v1339-emily-clone-spirit.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
