from pathlib import Path
import hashlib,json
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';F=W/'door-conflict-v749'
names=['v749-door-0.json','v749-door-1.json','v749-use-0.json','v749-use-1.json','v749-use-2.json','bookmark-save-v762.json']
rows=[]
for name in names:
    p=F/name;b=p.read_bytes();v=json.loads(b.decode('utf-8-sig'))
    rows.append(dict(source=p.relative_to(R).as_posix(),sha256=hashlib.sha256(b).hexdigest().upper(),transcript=v))
out=W/'ghidra-native-door-conflict-v749.json'
out.write_text(json.dumps(dict(scope='Original targeted decompilation/xref results from749 plus confirmed project bookmark save762; no new decompilation or runtime acceptance implied by this export.',sources=rows),indent=2),encoding='utf-8')
print(f'Exported {len(rows)} original transcripts with source paths/hashes for Markdown handoff discovery')
