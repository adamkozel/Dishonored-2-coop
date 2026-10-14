import json
from pathlib import Path
p=Path('work/handoff-project-bookmarks-current.json')
a=json.loads(p.read_text(encoding='utf-8-sig'))
b={"address":"141c79cf8","category":"D2Coop/Equipment-overlap-v154","comment":"CONFIRMED exact B671F824 runtime v154 context-setup-samples: Emily native holster has Running0 + EquipChange64 + EquipChange53 concurrently. Both64/53 share this vtable but state+24 references distinct scenes206/207. State64 owns power_unequip.midnightscene Scene_1/333ms with one reciprocal owned left arm67225. State53 owns sword_unequip Scene_1/866ms right arm132762+sword198752. Strict two-state reader rejected overlap for >250ms, retiring stream. v155 targeted two-scene ownership reader under validation. Generic entry/exit methods140853d40/1408541a0 do not independently establish meanings. No native timer/first-person mutations.","type":"Note"}
a=[v for v in a if not (v.get('address')==b['address'] and v.get('category')==b['category'])]
a.append(b)
p.write_text(json.dumps(a,indent=2),encoding='utf-8')
print(len(a))
