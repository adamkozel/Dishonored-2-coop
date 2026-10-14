from pathlib import Path
import json,datetime,collections
R=Path(__file__).resolve().parents[2]; D=R/'documentation/handoff-2026-10-11'; N=R/'work/natural-moveset/native-interaction-families-v827'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
wr=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
assert rd(N/'result.json')['passed'] and rd(N/'result.json')['readOnly']
at=datetime.datetime.now().astimezone().isoformat(); counts={}
for role in ('host','client'):
    rows=rd(N/f'catalog-{role}.json'); assert len(rows)==113
    counts[role]=dict(collections.Counter(c['type'] for e in rows for c in e['components']))
wr(N/'family-counts.json',counts)
state=rd(R/'work/current-task-state.json');state['UpdatedAt']=at
state['SharedUsableContinuation']=dict(at=at,readOnly=True,runtimeMutationVerified=False,complete=False,liveCandidateUnchanged='candidate-coin-sync-v821',evidence='work/natural-moveset/native-interaction-families-v827/result.json',details='documentation/handoff-2026-10-11/68-native-interaction-families-research.md',latest='Read-only113-entity census per role identifies52 state-list usables,20 static usables and3 sampled NPC loot components. Safe/cabinet, lamps/sinks/audiographs share native state-list code. Ghidra traces state/transition/instigator fields and native animation/script notifications; no new mutation or replication executed.',next='Observe ordinary safe/cabinet use through native input; implement a verified owned state-list reader and host-ordered desired transitions with checkpoint renewal. Keep static scripted triggers, role-specific audio and NPC loot separate until outcomes are established.')
state['SharedPickupContinuation']['next']=state['SharedUsableContinuation']['next']+' Then shared NPC/world authority, remaining movement/VFX and native session UI/separate-account Steam testing.'
wr(R/'work/current-task-state.json',state)
data=rd(R/'work/handoff-project-bookmarks-usable-v827.json'); bms=json.loads(data['structuredContent']['result'])['bookmarks'];keys={(b['address'],b['category']) for b in bms}
for name in ('handoff-project-bookmarks-current.json','handoff-project-bookmarks.json'):
    p=R/'work'/name;wr(p,sorted([b for b in rd(p) if (b['address'],b['category']) not in keys]+bms,key=lambda b:(b['address'],b['category'])))
p=D/'README.md';s=p.read_text();line='- [Native interaction families research](68-native-interaction-families-research.md)'
if line not in s:p.write_text(s.rstrip()+'\n\n'+line+'\n')
print(json.dumps(dict(passed=True,readOnly=True,hostEntities=113,clientEntities=113,liveUnchanged=True)))
