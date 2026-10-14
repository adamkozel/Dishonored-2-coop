"""Select exact aliases from the installed base left-arm animation set."""
import json,re
from pathlib import Path
P=Path(__file__).resolve().parent;R=P.parent.parent
asset='models/characters/player/base/left_arm.animset'
source=R/'outputs/d2-coop-lab/research/asset-exports/moveset-native-sets/game1_patch/generated.decls.animset.models.characters.player.base.left_arm..animset.decl'
rows=[dict(set=asset,alias=name,asset=clip,family='ranged_and_items',status='reference_only') for name,clip in re.findall(r'alias\s*\{\s*name\s+"([^"]+)"\s*anim\s+"([^"]+)"',source.read_text()) if name.startswith('ply_leftarm_grenade_')]
assert rows and len({r['alias'] for r in rows})==len(rows)
(P/'grenade-reference-selection-v53.json').write_text(json.dumps(rows,indent=2));print('Native grenade references selected:',len(rows))
