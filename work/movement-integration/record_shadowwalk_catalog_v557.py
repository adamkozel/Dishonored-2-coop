"""Correct stale catalog labels using scoped completed runtime evidence."""
from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
receipt=read(R/'work/movement-integration/shadowwalk-sneak-verification-v555.json')
assert receipt['candidate']=='candidate-shadowwalk-sneak-v550'
assert not receipt['complete'] and not receipt['visualAccepted']
for result in receipt['cases'].values():assert result['passed'] and len(result['checks'])==14
p=N/'review-evidence-annotations.json';data=read(p);item=data['items']['emily_shadow_walk_enter_move_attack_exit']
item['runtime_binding']='partial_native_standing_sneaking_entry_crawl_exit_through_Steam'
item['native_effects_status']='partial_six_native_cosmetic_particles_follow_and_retire'
item['visual_validation']='not_accepted';item['complete']=False;item['phase_status']='work_remaining'
paths=[name+'/result.json' for name in receipt['cases']]
item['evidence']=list(dict.fromkeys(item.get('evidence',[])+paths))
item['remaining_behaviors']=[
 'Correct whole-body Shadow Walk appearance; ordinary human body is still visible.',
 'Dedicated sideways/backward/turning motion and continuous hand/foot contact review.',
 'Attack and victim contact/outcomes, upgraded sprint, wall and rat-hole traversal.',
 'Relevant interruptions and recovery; entry/crawl/requested exit tests do not establish all lifecycle cases.',
 '509 airborne landing dropout and540 stream-start freshness refusal remain separate unresolved issues.'
]
power=item['power_visual_requirements'][0];assert power['id']=='shadow_walk'
power['runtime_effects_verified']=False
power['partial_runtime_evidence']={
 'candidate':receipt['candidate'],'receipts':paths,
 'scope':'Emily native standing/sneaking entry, forward/reverse crawl, requested crouched exit, six owned native particles and first-person preservation.',
 'remaining':item['remaining_behaviors'], 'visual_accepted':False,
 'visual_limit':'554 side view improves entry/recovery; body leaves the frame during mid-travel.'
}
p.write_text(json.dumps(data,indent=2),encoding='utf-8')
print('Shadow Walk labels now describe verified partial binding; complete/visual acceptance remain false')
