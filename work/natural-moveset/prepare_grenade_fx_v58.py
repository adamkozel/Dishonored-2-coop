"""Retain native in-hand particles/light/audio, excluding observer rumble."""
import json, hashlib, re
from pathlib import Path
R = Path(__file__).resolve().parents[2]
L = R / 'outputs/d2-coop-lab'
manifest = json.loads((L/'research/asset-exports/grenade-fx-reference-v57/manifest.json').read_text(encoding='utf-8-sig'))
source = next(r for r in manifest['Resources'] if r['Name']=='models/effects/weapons/grenade/grenade_trail_inhand.fx')
raw = Path(source['ExportFile']).read_bytes()
assert hashlib.sha256(raw).hexdigest().upper()==source['Sha256']
text = raw.decode('utf-8-sig')
match = re.search(r'\bm_rumbles\s*=\s*\{', text)
assert match and len(re.findall(r'\bm_rumbles\s*=', text))==1
start = match.start(); depth = 1; end = match.end()
while depth:
    assert end<len(text)
    depth += (text[end]=='{')-(text[end]=='}'); end+=1
output = text[:start] + 'm_rumbles = { num = 0; }' + text[end:]
assert 'grenade_trail_inhand.particledt' in output
assert 'bsp_weapon_grenade_ticking_stop' in output
asset = L/'assets/grenade_inhand_remote.fx'
asset.write_text(output, encoding='utf-8')
s = (L/'Build-D2GrenadePropAssets.ps1').read_text(encoding='utf-8-sig')
s=s.replace('grenade-prop-presentation','grenade-fx-presentation').replace('remote grenade cosmetic','remote grenade in-hand effect')
a=s.index('$resources=@('); b=s.index('\n[void][IO.Directory]',a)
s=s[:a]+'''$resources=@(
 @{Name='models/effects/d2coop/grenade_inhand_remote.fx';File='assets/grenade_inhand_remote.fx';Kind='fx';Class='fx'}
)
'''+s[b:]
(L/'Build-D2GrenadeFxAssets.ps1').write_text(s,encoding='utf-8')
(R/'work/natural-moveset/grenade-fx-source-v58.json').write_text(json.dumps(dict(source=source,output=str(asset.relative_to(R)),output_sha256=hashlib.sha256(asset.read_bytes()).hexdigest(),changes=['Disable the native observer rumble group; retain native particles, light and ticking start/stop sound.'],installed=False,runtime_verified=False),indent=2))
print('Prepared native-derived grenade in-hand effect; not installed')
