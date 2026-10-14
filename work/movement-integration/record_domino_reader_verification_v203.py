from pathlib import Path
import json
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset';M=W/'movement-integration'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,x:p.write_text(json.dumps(x,indent=2),encoding='utf-8')
for version,take in [('199','domino-target-reader-v200'),('201','domino-target-scoped-v202')]:
    archive=M/('candidate-domino-target-reader-v199' if version=='199' else 'candidate-domino-target-scoped-v201')
    result=read(P/take/'result.json');assert result['passed']
    summary=read(P/take/'target-reader-summary.json')
    write(archive/'runtime-verification.json',dict(take=take,passed=True,checks=len(result['checks']),summary=summary,scope='Read-only ownership observation; receiver effects unverified by this take',complete=False))
raw=read(W/'ghidra-bookmarks-domino-v203.json');new=json.loads(raw['structuredContent']['result'])['bookmarks']
entries=read(W/'handoff-project-bookmarks-current.json')
for row in new:
    entries=[x for x in entries if(x['address'],x['category'])!=(row['address'],row['category'])];entries.append(row)
write(W/'handoff-project-bookmarks-current.json',sorted(entries,key=lambda x:(x['address'],x['category'])))
print(json.dumps(dict(bookmarks=len(entries),readerReceipts=2)))
