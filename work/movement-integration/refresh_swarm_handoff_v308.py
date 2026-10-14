from pathlib import Path
import json,datetime
R=Path(__file__).resolve().parents[2];W=R/'work';P=W/'natural-moveset'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
write=lambda p,v:p.write_text(json.dumps(v,indent=2),encoding='utf-8')
old=read(W/'handoff-project-bookmarks-current.json');new=read(W/'movement-integration/ghidra-bookmarks-v308.json');merged={(x['address'],x['category']):x for x in old}
for x in new:merged[(x['address'],x['category'])]=x
write(W/'handoff-project-bookmarks-current.json',sorted(merged.values(),key=lambda x:(x['address'],x['category'])))
ann=read(P/'review-evidence-annotations.json')
ann['items']['corvo_devouring_swarm_cast']['notes']='Native grounded armed/holstered Swarm body contexts and measured crouch walk retained (bank50/313, semantic47). Native rat world stream and25 cosmetic models replicate strict source identities over Steam. Native walk/idle graph, travel-based gait,120ms interpolation and per-rat scale now have paired evidence.307 PASS23:2358 settled frame/entity/render scale samples,25 sizes,91.37percent interpolation.308 PASS23:startup clock calibration with8+ observations removes reproduced queued-first-packet bias; two native initializations take359/486ms, no steady rejections,93.59percent interpolation.739 actual-game videos indexed; complete visual/movement acceptance remains false.304/305/306 failures and corrected self-driver scale interpretation preserved inChapter31. Peer skins/summonFX/audio, full native rat states,100-rat performance and shared targets/damage remain incomplete.309 timing candidate adds quiet active/hidden windows; no optimization or performance completion claim. Whole movement and playable co-op unfinished; autonomous work continues.'
write(P/'review-evidence-annotations.json',ann)
print(json.dumps(dict(bookmarks=len(merged),at=datetime.datetime.now().astimezone().isoformat())))
