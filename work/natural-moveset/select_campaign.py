"""Select actor motions found in the other campaign archives, not victim clips."""
import json
from pathlib import Path
P=Path(__file__).resolve().parent
rows=json.loads((P/'native-allcatalog-sample-manifest.json').read_text())['rows']
requests={}
def want(family,aliases,purpose):
 for a in aliases.split():requests[a]=dict(family=family,purpose=purpose)
want('ranged_and_items','sword_grenade_in sword_grenade_launch sword_grenade_launch_down sword_grenade_jumpaway_toss','Native grenade actor motion; held grenade and gameplay-owned release still need contact and timing binding')
want('ranged_and_items','sword_gun_equip','Native pistol equip motion; native pistol attachment is a separate requirement')
want('combat','sword_attack_aerial_interception sword_attack_ground_interception','Native interception attacker motion; victim contact and gameplay outcomes remain separate')
want('stance','empty_distraction_look_keyhole_in empty_distraction_look_keyhole_out','Stationary native keyhole entry/recovery, requiring door-relative alignment')
want('interactions','activity_counter_read_book_in_walkright activity_counter_read_book_loop activity_counter_read_book_out_walk','Native standing reading sequence; book and support surface require contact adaptation')
want('interactions','activity_sitting_table_read_in activity_sitting_table_read_loop activity_sitting_table_read_out','Native seated reading sequence; seat and book must be aligned')
want('interactions','gesture_armcrossed_openhand gesture_openhand_think posture_talkcenter empty_distraction_clipboard_loop_talk','Native full-body dialogue gesture; clipboard prop only for that specific source')
want('interactions','empty_distraction_eat_apple_in empty_distraction_eat_apple_loop empty_distraction_eat_apple_out','Native eating sequence; apple contact and consumption event remain network-owned')
want('interactions','activity_drink_standing_in activity_drink_standing_loop_a activity_drink_standing_out','Native standing drink sequence; hand prop and elixir timing remain separate')
sources={};missing=[]
for a,request in requests.items():
 candidates=[r for r in rows if a in r['aliases'] and r['status']=='native_pose_sampled_offline']
 if not candidates:missing.append(a);continue
 for role in ('corvo','emily'):
  r=min(candidates,key=lambda r:(r['source_role']!=role,'/high/'not in r['asset'],r['asset']))
  s=sources.setdefault(r['id'],dict(alias=a,**request,targets=[]))
  if role not in s['targets']:s['targets'].append(role)
out=dict(scope='Additional native actor motions; contact-aware gameplay binding and visual acceptance remain pending',sources=sources,missing_requested_aliases=missing)
(P/'campaign-source-selection.json').write_text(json.dumps(out,indent=2))
print('SELECTED',len(sources),'sources',sum(len(s['targets'])for s in sources.values()),'role clips','MISSING',missing)
