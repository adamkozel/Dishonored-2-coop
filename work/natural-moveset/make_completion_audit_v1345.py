"""Writes completion-audit-v1345-paired.json (#11): paired victim alignment and crouched takedowns on candidates 1214cn-cq."""
import json
from pathlib import Path
N = Path(__file__).resolve().parent
M = ("Each decision rests on the takedown variant takes takedown-variants-v1254d-g (candidates 1214cn, cp, cq) under the "
     "product controller with natural guard AI and the NPC stream on, both source roles: the source performs a native "
     "takedown on the upper-floor Dunwall guard (side and front approaches start crouched), the partner presents every "
     "census-1253 takedown scene the source played, both worlds must reach the same outcome (unconscious or dead) and the "
     "victim must end within 2 m in both worlds; the checkpoint is reloaded after every case. Reviewed observer videos are "
     "listed per item.")
def V(video, verdict): return {'video': video, 'verdict': verdict}
G = 'takedown-variants-v1254g'
items = [
 dict(id='paired_victim_alignment', decision='complete',
      scope='Every passing takedown in both roles (back, side, front, drop and slide; chokes and lethals; 15 cases in 1254g alone) ends with the same outcome in both worlds and the victim within 0-0.03 m of the same place, the partner presenting the source side of the paired scene.',
      takes=[{'take': G, 'cases': ['host-back-choke', 'client-back-choke', 'host-back-lethal', 'client-back-lethal', 'host-left-lethal', 'client-left-lethal', 'host-drop-lethal', 'client-drop-lethal']}],
      visual=[V(f'{G}/back-host-choke-observer.mp4', 'Corvo grabs the guard from behind and holds the choke while the guard struggles; the guard sags and collapses at his feet in the same place.')]),
 dict(id='crouched_takedown', decision='complete',
      scope='Takedowns started from a crouch in both roles: the side lethals (approach crouched; native scenes 31 and 108) and Corvo\'s crouched front choke (62) are presented with the shared outcome; the crouched rear choke and fast-back lethal were bound in both roles in audit 1246.',
      takes=[{'take': G, 'cases': ['host-left-lethal', 'client-left-lethal', 'host-front-choke']}],
      visual=[V(f'{G}/left-host-lethal-observer.mp4', 'Corvo comes in low from the guard\'s side, drives the blade in and the guard folds to the floor of the walkway.'),
              V('takedown-variants-v1254f/right-client-lethal-observer.mp4', 'Emily closes on the guard from the side on the balcony and stabs him; he drops and she straightens over the body.')]),
]
for i in items:
    i['lifecycle'] = 'See method.'
audit = dict(audit='completion-audit-v1345-paired', family='paired_actions', candidate='candidate-npc-combat-v1214cq', method=M, items=items)
(N / 'completion-audit-v1345-paired.json').write_text(json.dumps(audit, indent=2) + '\n', encoding='utf-8')
print('ok', len(items))
