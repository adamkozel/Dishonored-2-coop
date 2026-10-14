"""Whole-body source inventory; alias names are discovery evidence, not coverage."""
import json,re,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
folder=ROOT/'outputs/d2-coop-lab/research/asset-exports/natural-fullbody-sets'
families={
 'locomotion':r'walk|run|sprint|idle|turn|pivot|start|stop|stair|slope',
 'stance':r'crouch|sneak|undertable|lean|peek|keyhole',
 'traversal':r'jump|fall|land|climb|vault|ladder|slide|swim|dive|window|ledge|rope',
 'combat':r'sword|attack|parry|dodge|hit|block|death|stagger|knock|kick|shove',
 'ranged_and_items':r'pistol|gun|crossbow|shoot|reload|grenade|throw|mine|heart|elixir|drink',
 'paired_actions':r'assass|choke|fatal|kill|takedown|struggle|carry|victim|grip|body',
 'powers':r'cast|power|blink|possession|domino|swarm|shadow|mesmer|doppel|teleport|windblast',
 'interactions':r'door|lever|button|crank|wheel|pickup|pick_up|loot|key|book|note|eat|sit|chair|standup|object|talk|open|close|place|inspect'}
entries=[];seen=set()
for path in folder.rglob('*.decl'):
    text=path.read_text(encoding='utf-8-sig');sk=re.search(r'\bskeleton\s+"([^"]+)"',text)
    if not sk or not any(x in sk[1] for x in ('base_tall','base_female')):continue
    for alias,asset in re.findall(r'alias\s*\{\s*name\s+"([^"]+)"\s+anim\s+"([^"]+)"',text):
        key=(sk[1],alias,asset)
        if key in seen:continue
        seen.add(key)
        group=[f for f,pattern in families.items() if re.search(pattern,alias,re.I)]
        if not group:continue
        entries.append(dict(alias=alias,asset=asset,skeleton=sk[1],role_candidate='corvo' if 'base_tall' in sk[1] else 'emily',families=group,source=str(path.relative_to(ROOT)),source_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),status='candidate_only',runtime_verified=False))
result=dict(scope='Native full-body source inventory, not authored/verified player movements.',count=len(entries),unique_assets=len(set(e['asset'] for e in entries)),family_counts={f:sum(f in e['families'] for e in entries) for f in families},entries=entries)
(P/'native-source-index.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k!='entries'},indent=2))
