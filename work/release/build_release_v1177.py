"""Builds the co-op release package from the live lab candidate (1177).
Payload = exactly the files that differ from an original install (read-only
comparison, 1177): dinput8.dll, base/game{1..4}voidmod.{index,resources} and
the modified base/master.index. steam_api64.dll and Dishonored2.exe are never
shipped or replaced."""
from pathlib import Path
import hashlib,json,shutil,datetime,sys
R=Path(__file__).resolve().parents[2]
LAB=R/'work/asset-game';ORIG=Path(r'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2')
CAND=R/'work/movement-integration/candidate-npc-combat-v1171'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest().upper()
ident=json.loads((CAND/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(LAB/'dinput8.dll')==ident['dll'],'live DLL is not candidate 1171'
OUT=R/'outputs/release/d2coop-v1171-r2'
if OUT.exists():sys.exit('release folder exists; never overwrite a published package')
payload=['dinput8.dll','base/master.index']+[f'base/game{i}voidmod.{x}' for i in (1,2,3,4) for x in ('index','resources')]
(OUT/'files/base').mkdir(parents=True)
for rel in payload:shutil.copy2(LAB/rel,OUT/'files'/rel)
manifest=dict(name='Dishonored 2 Corvo/Emily co-op',candidate='candidate-npc-combat-v1171',built=datetime.datetime.now().astimezone().isoformat(),
    requiredExecutableSha256=sha(LAB/'Dishonored2.exe'),requiredSteamApiSha256=sha(LAB/'steam_api64.dll'),
    originalMasterIndexSha256=sha(ORIG/'base/master.index'),
    files=[dict(path=rel,sha256=sha(OUT/'files'/rel),size=(OUT/'files'/rel).stat().st_size,replacesOriginal=rel=='base/master.index') for rel in payload],
    notes=['Never replaces Dishonored2.exe or steam_api64.dll (native Steam).','Install backs up master.index and any existing dinput8.dll; Uninstall restores them byte-identically.',
           'Saves are written by the game in its normal profile; the co-op lab profiles are not shipped.','Two-account online validation is still outstanding.'])
(OUT/'manifest.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
for script in ('Install-D2Coop.ps1','Uninstall-D2Coop.ps1'):shutil.copy2(Path(__file__).parent/script,OUT/script)
print(json.dumps(dict(out=str(OUT),files=len(payload)),indent=1))
