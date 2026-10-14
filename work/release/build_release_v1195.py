"""Builds the co-op release package r4 from the live lab candidate 1193 (1195).
Adds a README for players. Earlier header (1177):
Payload = exactly the files that differ from an original install (read-only
comparison, 1177): dinput8.dll, base/game{1..4}voidmod.{index,resources} and
the modified base/master.index. steam_api64.dll and Dishonored2.exe are never
shipped or replaced."""
from pathlib import Path
import hashlib,json,shutil,datetime,sys
R=Path(__file__).resolve().parents[2]
LAB=R/'work/asset-game';ORIG=Path(r'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2')
CAND=R/'work/movement-integration/candidate-npc-combat-v1193'
sha=lambda p:hashlib.sha256(Path(p).read_bytes()).hexdigest().upper()
ident=json.loads((CAND/'identity.json').read_text(encoding='utf-8-sig'))
assert sha(LAB/'dinput8.dll')==ident['dll'],'live DLL is not candidate 1193'
OUT=R/'outputs/release/d2coop-v1193'
if OUT.exists():sys.exit('release folder exists; never overwrite a published package')
payload=['dinput8.dll','base/master.index']+[f'base/game{i}voidmod.{x}' for i in (1,2,3,4) for x in ('index','resources')]
(OUT/'files/base').mkdir(parents=True);(OUT/'files/d2coop').mkdir(parents=True)
for rel in payload:shutil.copy2(LAB/rel,OUT/'files'/rel)
# 1181 product activation: config + both partner movement banks (bank 70/369).
cfg_lines=['[d2coop]','enabled=1','; Optional: the folder holding the SLOTxx save folders, if not the default',r'; savegame=C:\Users\<you>\Saved Games\Arkane Studios\Dishonored2\base\savegame','']
(OUT/'files/d2coop/d2coop.cfg').write_text('\n'.join(cfg_lines),encoding='ascii')
for who in ('corvo','emily'):shutil.copy2(R/f'work/natural-moveset/climb-banks-v1042/{who}-choke-bank.bin',OUT/f'files/d2coop/{who}-choke-bank.bin')
payload+=['d2coop/d2coop.cfg','d2coop/corvo-choke-bank.bin','d2coop/emily-choke-bank.bin']
manifest=dict(name='Dishonored 2 Corvo/Emily co-op',candidate='candidate-npc-combat-v1193',built=datetime.datetime.now().astimezone().isoformat(),
    requiredExecutableSha256=sha(LAB/'Dishonored2.exe'),requiredSteamApiSha256=sha(LAB/'steam_api64.dll'),
    originalMasterIndexSha256=sha(ORIG/'base/master.index'),
    files=[dict(path=rel,sha256=sha(OUT/'files'/rel),size=(OUT/'files'/rel).stat().st_size,replacesOriginal=rel=='base/master.index') for rel in payload],
    notes=['Never replaces Dishonored2.exe or steam_api64.dll (native Steam).','Install backs up master.index and any existing dinput8.dll; Uninstall restores them byte-identically.',
           'Saves are written by the game in its normal profile; the co-op lab profiles are not shipped.','Product mode: F7 hosts (Steam lobby + invite overlay), F8 leaves; partners join through Steam Join Game / invite.','Two-account online validation is still outstanding.'])
(OUT/'manifest.json').write_text(json.dumps(manifest,indent=2),encoding='utf-8')
for script in ('Install-D2Coop.ps1','Uninstall-D2Coop.ps1'):shutil.copy2(Path(__file__).parent/script,OUT/script)
readme=r"""Dishonored 2 - Corvo/Emily co-op (build 1193)

Install (PowerShell 7, game closed):
  pwsh -File Install-D2Coop.ps1 -GameDirectory "<your Dishonored2 folder>"
The installer refuses unknown game builds, backs up base\master.index and any
existing dinput8.dll, and writes d2coop-install-receipt.json.
Dishonored2.exe and steam_api64.dll are never replaced (native Steam).

Uninstall (restores the original files byte-identically):
  pwsh -File Uninstall-D2Coop.ps1 -GameDirectory "<your Dishonored2 folder>"

Playing:
  Both players install the same build and start the game from Steam.
  Host (plays Corvo): in game, press F7. A friends-only Steam lobby is
    created and Steam's invite overlay opens.
  Partner (plays Emily): accept the invite, or use "Join Game" on the host in
    the Steam friends list. The partner's game follows the host's mission.
  F8 leaves the session.
  Once both players are in the same mission, every co-op system arms itself.

Status:
  Lab-verified on one PC with two instances. Online play between two Steam
  accounts has not been validated yet.
"""
(OUT/'README.txt').write_text(readme,encoding='utf-8')
print(json.dumps(dict(out=str(OUT),files=len(payload)),indent=1))
