from pathlib import Path
import json
# Link mission-map swim/climb recordings to their checklist items (the 1040/1044
# publishers listed them without `requirements`, so the catalog did not attach them).
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
p=N/'runtime-video-manifest.json';v=json.loads(p.read_text(encoding='utf-8-sig'));n=0
for x in v:
    t=x['video'].split('/')[0];name=x['video'].split('/')[-1];req=None
    if t.startswith(('swim-coop-eotw-v1038','native-swim-v1035','traversal-emily-swimview','traversal-corvo-swimview')) or (t=='traversal-emily-v1046' and name.startswith('swim')):req=['swim_idle_directions','dive_surface_water_exit']
    elif t in ('climb-coop-eotw-v1043','native-climb-eotw-v1041','native-climb-eotw-v1041b') or (t=='traversal-emily-v1046' and name.startswith('climb')):req=['ladder_up_down_entry_exit']
    if req and x.get('runtime_test_passed',True):x['requirements']=sorted(set(x.get('requirements',[]))|set(req));n+=1
p.write_text(json.dumps(v,indent=2),encoding='utf-8');print('linked',n)
