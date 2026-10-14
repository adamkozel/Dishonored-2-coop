"""Prepare one isolated visual-only grenade resource; never install here."""
from pathlib import Path
P=Path(__file__).resolve().parents[2];lab=P/'outputs/d2-coop-lab'
(lab/'assets/grenade_prop.def').write_text('''{
 class = "idEntity";
 edit = {
  renderModelInfo = {
   model = "models/weapons/grenade/grenade.md6";
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
''')
s=(lab/'Build-D2RangedPropAssets.ps1').read_text(encoding='utf-8-sig')
s=s.replace('ranged-prop-presentation','grenade-prop-presentation').replace('remote ranged cosmetic','remote grenade cosmetic')
a=s.index('$resources=@(');b=s.index('\n[void][IO.Directory]',a)
s=s[:a]+'''$resources=@(
 @{Name='models/weapons/d2coop/grenade_prop.def';File='assets/grenade_prop.def';Kind='entitydef';Class='entityDef'}
)
'''+s[b:]
(lab/'Build-D2GrenadePropAssets.ps1').write_text(s,encoding='utf-8');print('Grenade cosmetic resource and isolated installer prepared')
