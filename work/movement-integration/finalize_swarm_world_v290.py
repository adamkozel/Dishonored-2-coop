from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';T=P/'swarm-world-network-v290';W=R/'work/movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
r=read(T/'result.json');assert r['passed']
rows=read(T/'world-states.json');live=[x for x in rows if x['peer']['leased'] and x['peer']['present']==1 and x['peer']['count']>0]
summary=read(T/'world-summary.json');summary['counts']=sorted({x['peer']['count'] for x in live});summary['countNote']='Recomputed from each row; initial PowerShell collection .count reported live sample length, not individual agent counts.'
write(T/'world-summary-corrected.json',summary)
p=P/'Test-SwarmWorldNetwork-v290.ps1';s=p.read_text(encoding='utf-8-sig');s=s.replace('@($live.peer.count|Sort-Object -Unique)','@($live|ForEach-Object {$_.peer.count}|Sort-Object -Unique)');p.write_text(s,encoding='utf-8')
a=W/'candidate-swarm-world-v290/runtime-world-verification.json';write(a,dict(at=datetime.datetime.now().astimezone().isoformat(),passed=True,result=str(T/'result.json'),summary=summary,remoteVisuals=False,remoteDamage=False,complete=False))
doc=R/'documentation/handoff-2026-10-11/31-native-devouring-swarm.md'
with doc.open('a',encoding='utf-8') as f:f.write('\n[v290 live paired proof](../../work/natural-moveset/swarm-world-network-v290/result.json) now passes all 14 checks: 117 sampled active peer states and 126 overlapping exact sent/accepted wire digests, zero rejected/unknown snapshots, and a 1.638 ms maximum scoped native read. Actual indexed counts were '+str(summary['counts'])+'. The initial summary used PowerShell collection `.count`; the [corrected derivation](../../work/natural-moveset/swarm-world-network-v290/world-summary-corrected.json) preserves this reporting correction without altering the raw receipt. Source stream stop retires the peer lease; checkpoint reload clears presented agents and restores inventory/health/protected14. Source and peer contact sheets were inspected: source preview/rats are present, peer full-body cast/recovery remain visible, and peer rat models are intentionally absent from this transport-only candidate. This is not a remote VFX or gameplay completion claim.\n')
print(json.dumps(summary))
