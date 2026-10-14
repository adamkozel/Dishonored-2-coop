from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset/native-doppelganger-source-v406b'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
rows=rd(P/'states.json');out=[]
for row in rows:
 words={x['offset']:int(x['word'],16) for x in row['component']['words']}
 clones=[dict(handle=x['handle'],pointer=x['pointer'],definition=x['definitionName'],position=x['position']) for x in row['entities'] if x['definitionName']=='models/powers/doppelganger/decoy_entity.def']
 d=dict(videoTime=row['videoTime'],preview=hex(words.get('0xb0',0)&0xffffffff),array=hex(words.get('0xb8',0)),count=words.get('0xc0',0)&0xffffff,lastClone=hex((words.get('0xd0',0)>>32)&0xffffffff),sensor=hex(words.get('0xc8',0)),clones=clones,effects=sorted(set(x['name'] for x in row['effects'].get('effects',[]))))
 out.append(d)
result=dict(scope='Static field interpretation aligned with recorded raw component samples; entity enumeration is separate, not a reciprocal live ownership reader. Array contents and clone component lifecycle still need validation before replication.',samples=out)
(P/'clone-lifecycle-candidates.json').write_text(json.dumps(result,indent=2),encoding='utf-8')
last=None
for d in out:
 key=(d['preview'],d['count'],d['lastClone'],tuple(x['handle'] for x in d['clones']))
 if key!=last:print(json.dumps(d));last=key
