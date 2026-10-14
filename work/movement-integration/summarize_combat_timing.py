import json
from pathlib import Path
root=Path(__file__).resolve().parents[2]
folder=root/'work/natural-moveset/native-combat-timing-v1'
rows=json.loads((folder/'states.json').read_text(encoding='utf-8-sig'))
groups={}
for row in rows:
    key=(row['role'],row['trial'])
    out=groups.setdefault(key,{'role':key[0],'trial':key[1],'scenes':{},'flags':[]})
    for scene in row['midnight']['instances']:
        asset=scene.get('asset','')
        if '/player/base/combat/' not in asset: continue
        sid=str(scene['id'])
        entry=out['scenes'].setdefault(sid,{'asset':asset,'subscene':scene.get('subscene'),'duration':scene.get('sceneDurationMs'),'first_ms':row['ms'],'last_ms':row['ms'],'state':[],'clock_min':scene['clockWords'][0],'clock_max':scene['clockWords'][0]})
        entry['last_ms']=row['ms']
        if scene['state'] not in entry['state']:entry['state'].append(scene['state'])
        entry['clock_min']=min(entry['clock_min'],scene['clockWords'][0]);entry['clock_max']=max(entry['clock_max'],scene['clockWords'][0])
    for state in row['native']['active']:
        if 'slashResearch' in state:
            flags=state['slashResearch']['flags_d0_db']
            if not out['flags'] or out['flags'][-1]['flags']!=flags:
                out['flags'].append({'ms':row['ms'],'flags':flags})
result={'qualification':'Read-only correlations; fixture ended in player death and native AI crash. Scene ownership must be validated before runtime binding.','groups':list(groups.values())}
(folder/'timing-summary.json').write_text(json.dumps(result,indent=2))
for g in groups.values():
    print(g['role'],g['trial'],[(s['asset'].split('/')[-1],s['subscene'],s['first_ms'],s['last_ms'],s['clock_min'],s['clock_max']) for s in g['scenes'].values()])
