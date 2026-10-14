import json
from pathlib import Path
p=Path('work/handoff-project-bookmarks-current.json')
a=json.loads(p.read_text(encoding='utf-8-sig'))
b={"address":"141c80ed8","category":"D2Coop/Mesmerize-air-v163","comment":"CONFIRMED exact B671F824 source observations: v151 armed Emily Jump2/Fall3 + UsePower63 holds Mesmerize aim loop (1200ms). v161 early LT release: Jump2-only and Fall3-only overlap exact out666ms; same reciprocal left arm67225, peak1.4026m, same-floor recovery. v163 A+LT entry: two Jump2+UsePower63/in466ms samples then Fall3+UsePower63/loop1200ms; peak1.4415m. Raw Midnight then state reads differ16–31ms, corroborated across adjacent rows and actual-game video. All source cleanup checks pass. This generic UsePower vtable alone does NOT authorize an action: exact owned selected Mesmerize item, scene incarnation/phase and left-arm bindings remain necessary. Empty-hand air and remote air integration are not proven by these observations.","type":"Note"}
a=[v for v in a if not (v.get('address')==b['address'] and v.get('category')==b['category'])]
a.append(b)
p.write_text(json.dumps(a,indent=2),encoding='utf-8')
print(len(a))
