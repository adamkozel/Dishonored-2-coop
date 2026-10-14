"""Decode preserved read-only snapshots; never reads or writes live game memory."""
from pathlib import Path
import json, struct, math
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
take=P/'swarm-native-world-indexed-observe-v282'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def blob(obj):
    out=bytearray(1024)
    for word in obj['words']:
        i=int(word['offset'],16); out[i:i+8]=int(word['word'],16).to_bytes(8,'little')
    return out
def u64(b,o):return struct.unpack_from('<Q',b,o)[0]
def u32(b,o):return struct.unpack_from('<I',b,o)[0]
def fs(b,o,n=3):return list(struct.unpack_from('<'+'f'*n,b,o))
def delta(a,b):return math.dist(a,b)
summary=[]
for index in range(2):
    snap=read(take/f'world-components-{index}.json'); c=blob(snap['ratComponent']); p=blob(snap['pipeline'])
    transform=blob(snap['pipelineTransforms']); agents=[]
    for a in snap['liveAgentObjects']:
        if not a['activeLinks']:continue
        b=blob(a['rat']); e=blob(a['entity']); f=blob(a['frame']); anim=blob(a['animation']); i=a['index']
        row=dict(index=i,handle=hex(u32(e,0xa8)),parent=hex(u32(e,0xac)),
                 ratVtable=hex(u64(b,0)),entityVtable=hex(u64(e,0)),frameVtable=hex(u64(f,0)),animVtable=hex(u64(anim,0)),
                 ratOwnerMatches=u64(b,0x18)==int(snap['ratComponent']['pointer'],16),
                 entityBackPointerMatches=u64(e,0x360)==int(a['rat']['pointer'],16),
                 frameOwnerMatches=u64(f,0x10)==int(a['entity']['pointer'],16),
                 animOwnerMatches=u64(anim,0x10)==int(a['entity']['pointer'],16),
                 frameMember=sum(x['pointer']==a['frame']['pointer'] for x in a['components']['components'])==1,
                 animMember=sum(x['pointer']==a['animation']['pointer'] for x in a['components']['components'])==1,
                 incarnation=u32(b,0x44),state=u32(b,0x48),entityTransform=fs(e,0x300,12),frameTransform=fs(f,0x30,12),
                 pipelinePosition=fs(transform,i*32),pipelineSecondary=fs(transform,i*32+16),pipelineScale=fs(transform,i*32+28,1)[0],
                 rat34=fs(b,0x34),ratD0=fs(b,0xd0))
        row['frameVsEntityDistance']=delta(row['frameTransform'][:3],row['entityTransform'][:3])
        row['pipelineVsEntityDistance']=delta(row['pipelinePosition'],row['entityTransform'][:3])
        row['d0VsEntityDistance']=delta(row['ratD0'],row['entityTransform'][:3])
        agents.append(row)
    summary.append(dict(videoTime=snap['videoTime'],world=snap['entity'],indices=snap['candidateIndices'],
        poolMatchesPipeline=u64(c,0x80)==u64(p,0x1a8),strideComponent=u32(c,0x88),stridePipeline=u32(p,0x1b0),
        transformArrayCount=u32(p,0x140),transformArrayCapacity=u32(p,0x144),agentRows=agents))
result=dict(source=str(take.relative_to(R)),snapshots=summary,
 confirmed='Pipeline pool base/stride match component. Entity +0x360 and rat +0x18 supply reciprocal ownership; entity parent is world handle. Entity +0x300 and reference-frame +0x30 are spatially consistent but sampled at different times (up to 0.072 distance difference). Rat +0x34 also changes, so its earlier description as initial position was unproven. These observations are not synchronized memory transactions.',
 limitations='Only first three indexed agents were traversed in each snapshot; all 25 need strict runtime validation. No remote effects/AI/damage are enabled.')
assert all(s['poolMatchesPipeline'] and s['strideComponent']==s['stridePipeline']==0x108 for s in summary)
assert all(all(a[k] for k in ('ratOwnerMatches','entityBackPointerMatches','frameOwnerMatches','animOwnerMatches','frameMember','animMember')) for s in summary for a in s['agentRows'])
path=R/'work/movement-integration/swarm-indexed-pool-evidence-v283.json'
path.write_text(json.dumps(result,indent=2),encoding='utf-8')
print(json.dumps(result,indent=2))
