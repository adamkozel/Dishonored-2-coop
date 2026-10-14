"""Read existing Void exports; candidates only, never an autoplay allowlist."""
import json,re,hashlib
from pathlib import Path
P=Path(__file__).resolve().parent
root=P.parent.parent/'outputs/d2-coop-lab/research/asset-exports'
entries=[];seen=set()
for path in root.rglob('*.decl'):
 if '.animset.' not in path.name:continue
 text=path.read_text(encoding='utf-8-sig');sk=re.search(r'\bskeleton\s+"([^"]+)"',text)
 if not sk or not any(s in sk[1] for s in ('base_tall','base_female')):continue
 for alias,asset in re.findall(r'alias\s*\{\s*name\s+"([^"]+)"\s+anim\s+"([^"]+)"',text):
  if (sk[1],alias,asset) in seen:continue
  seen.add((sk[1],alias,asset))
  families=[family for family,pattern in [('running','run|sprint'),('turning','turn'),('traversal','jump|fall|land|climb|vault|ladder|slide|swim'),('stance','crouch|sneak|undertable|lean'),('combat','attack|parry|dodge|hit|block'),('paired','assass|choke|fatality')] if re.search(pattern,alias,re.I)]
  if not families:continue
  # Event names present in the source set are warnings for a later clip-specific
  # audit, not evidence that this alias necessarily dispatches every event.
  event_names=sorted(set(re.findall(r'\bevent\s+"([^"]+)"',text)))
  entries.append(dict(alias=alias,asset=asset,skeleton=sk[1],role_candidate='corvo' if 'base_tall' in sk[1] else 'emily',families=families,
    source=str(path.relative_to(P.parent.parent)),source_sha256=hashlib.sha256(path.read_bytes()).hexdigest(),set_event_names=event_names,
    status='native_fullbody_source_candidate_only',runtime_verified=False,requires_clip_specific_event_audit=True))
result=dict(scope='Previously exported native full-body sets. Not the complete installed library, not runtime acceptance, and not evidence of a complete player moveset.',count=len(entries),
 family_counts={f:sum(f in e['families'] for e in entries) for f in ('running','turning','traversal','stance','combat','paired')},entries=entries)
(P/'native-fullbody-candidates.json').write_text(json.dumps(result,indent=2));print(json.dumps({k:v for k,v in result.items() if k!='entries'},indent=2))
