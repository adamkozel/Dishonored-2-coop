from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
reviews={
'npc-combat-world-v919':dict(host='Inspected contact-host.jpg at0/1/2/3/4/6s: native close-range assassination and victim falling out of view, surrounding live NPCs. Native source action is visible.',client='Inspected contact-client.jpg at0/1/2/3/4/6s: remote Corvo, victim dropping and corpse on floor, other NPCs. Remote player remains mostly neutral during native paired action; full paired choreography and matching corpse physics are not accepted.'),
'npc-combat-hazard-v920':dict(host='Inspected contact-host.jpg at0/4/8/12/14/16s: native waiting guard then sword contacts with blood. No fire event visible in this sampled interval. Numeric world counters are zero.',client='Inspected contact-client.jpg at0/4/8/12/14/16s: remote Corvo in front of victim with partial victim occlusion. Does not establish matched sword/victim choreography or fire VFX.'),
'npc-combat-trigger-v921':dict(host='Inspected contact-host.jpg at0/4/8/12/14/17s: waiting guard followed by native sword contacts/blood. Console trigger activation produced no observed world damage; positive trigger acceptance is not claimed.',client='Inspected contact-client.jpg at0/4/8/12/14/17s: remote Corvo partly blocks target, native room and combat. No fire outcome visible. Full paired attack choreography remains incomplete.')}
for name,r in reviews.items():(N/name/'visual-review-v924.json').write_text(json.dumps(r,indent=2))
print('Saved3 paired visual reviews with explicit coverage limits.')
