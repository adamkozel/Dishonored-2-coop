"""Prepare isolated cosmetic native-model props, with no weapon callbacks."""
from pathlib import Path
import json
P=Path(__file__).resolve().parents[2];lab=P/'outputs/d2-coop-lab';assets=lab/'assets';rows=[]
for role in ('corvo','emily'):
 for kind in ('gun','crossbow'):
  model=f'models/weapons/crossbow/crossbow_{role}.md6' if kind=='crossbow' else f'models/weapons/gun_player/gun_{"player" if role=="corvo" else "emily"}.md6'
  filename=f'ranged_prop_{role}_{kind}.def';name=f'models/weapons/d2coop/{filename}'
  text='''{
 class = "idEntity";
 edit = {
  renderModelInfo = {
   model = "%s";
   m_castShadows = true;
  }
  m_componentDecls = {
   item_begin = true;
   item_add["D2VisualAnim"] = {
    inheritedDecl = "components/equipment/animevolved_equipment.cpntanimevolved";
   }
  }
 }
}
'''%model
  (assets/filename).write_text(text,encoding='utf-8');rows.append(dict(Name=name,File='assets/'+filename))
s=(lab/'Build-D2WindblastPresentationAssets.ps1').read_text(encoding='utf-8-sig')
s=s.replace('windblast-presentation','ranged-prop-presentation').replace('Windblast visual-only','ranged cosmetic prop')
start=s.index('$resources=@(');end=s.index('\n[void][IO.Directory]',start)
resources='$resources=@(\n'+',\n'.join(" @{Name='%s';File='%s';Kind='entitydef';Class='entityDef'}"%(r['Name'],r['File'])for r in rows)+'\n)'
s=s[:start]+resources+s[end:]
(lab/'Build-D2RangedPropAssets.ps1').write_text(s,encoding='utf-8')
(P/'work/movement-integration/ranged-prop-design-v1.json').write_text(json.dumps(dict(complete=False,resources=rows,
 purpose='Native visual-only gun/crossbow meshes attached through native entity/reference-frame API. No inventory, weapon, projectile, pickup, collision, attention-source, health or damage component.',
 attachment=dict(native_api='140875e80 -> 140752900',query='140879930',draw_tag='left_hand_wpn',source='Native cpntEquipment start 14061aed0 and elite gun declaration'),
 limitations=['Uninstalled/unverified until bounded runtime test.','Mesh mechanism animation, hand grip offsets and streamed gameplay binding remain to be implemented.','Generic cpntEquipment base was not used: no shipped standalone declaration found and base vtable contains abstract slots.']),indent=2),encoding='utf-8')
print('Prepared four isolated cosmetic props; no game files changed')
