"""Builds release 0.7.0 (r7, take 1313, #30) from candidate 1214ce. The package ships no game files: everything
game-derived is built by the installer from the player's own install with bin/d2coop_setup.exe:
- base/master.index and base/game{1..4}voidmod.{index,resources}: from data/decl-recipes.bin (D2RCP004) and the
  player's resource containers; byte-identical to the lab's files, so the manifest pins their SHA-256;
- d2coop/{corvo,emily}-choke-bank.bin: from data/<role>-bank.patch (D2BNK001) and the player's animations, sampled
  through the player's own Dishonored2.exe; checked by per-clip sums, since samples may differ in their last bits
  between CPUs;
- d2coop/native-tables.bin: from data/native-tables.recipe (D2NTR001), checked the same way.
Shipped as is: dinput8.dll and d2coop_setup.exe (ours), d2coop/d2coop.cfg, the scripts and the notes.
The original Steam install is only read: to prove the generated files from a clean install and to pin hashes.
Writes outputs/release/d2coop-v0.7.0/ and outputs/release/d2coop-v0.7.0.zip (local; published as a GitHub release).
"""
from pathlib import Path
import hashlib, json, shutil, datetime, subprocess, sys, tempfile, zipfile
R = Path(__file__).resolve().parents[2]
VERSION = '0.7.0'
LAB = R / 'work/asset-game'; ORIG = Path(r'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2')
CAND = R / 'work/movement-integration/candidate-npc-combat-v1214ce'
REL = R / 'work/release'
sha = lambda p: hashlib.sha256(Path(p).read_bytes()).hexdigest().upper()
ident = json.loads((CAND / 'identity.json').read_text(encoding='utf-8-sig'))
assert sha(LAB / 'dinput8.dll') == ident['dll'], 'live DLL is not candidate 1214ce'
assert sha(CAND / 'd2coop_setup.exe') == ident['setup']
OUT = R / f'outputs/release/d2coop-v{VERSION}'; ZIP = OUT.with_suffix('.zip')
if OUT.exists() or ZIP.exists():
    sys.exit('release folder exists; never overwrite a published package')
data = {'data/decl-recipes.bin': REL / 'decl-recipes-v1/decl-recipes.bin',
        'data/corvo-bank.patch': REL / 'bank-patches-v1/corvo-bank.patch',
        'data/emily-bank.patch': REL / 'bank-patches-v1/emily-bank.patch',
        'data/native-tables.recipe': REL / 'native-tables-v1/native-tables.recipe'}
for p in data.values():
    assert p.exists(), p
# Prove the generated packages from the original install (read only) and pin them to the lab's own files.
setup = CAND / 'd2coop_setup.exe'
with tempfile.TemporaryDirectory(dir=REL) as tmp:
    r = subprocess.run([str(setup), 'packages', str(ORIG / 'base'), str(data['data/decl-recipes.bin']), str(Path(tmp) / 'base')], capture_output=True, text=True)
    assert r.returncode == 0, r.stderr
    packages = json.loads(r.stdout.strip().splitlines()[-1])['files']
    generated = []
    for rel, h in sorted(packages.items()):
        assert h.upper() == sha(LAB / rel), f'{rel}: built file differs from the lab game'
        generated.append(dict(path=rel, sha256=h.upper(), builtFrom='data/decl-recipes.bin and the player resource containers', replacesOriginal=rel == 'base/master.index'))
for rel, src in (('d2coop/corvo-choke-bank.bin', 'data/corvo-bank.patch'), ('d2coop/emily-choke-bank.bin', 'data/emily-bank.patch'), ('d2coop/native-tables.bin', 'data/native-tables.recipe')):
    generated.append(dict(path=rel, sha256=None, builtFrom=src + ' and the player animations through the player Dishonored2.exe', verifiedBy='d2coop_setup per-clip or per-table sums'))
# Package.
(OUT / 'files/d2coop').mkdir(parents=True); (OUT / 'bin').mkdir(); (OUT / 'data').mkdir()
shutil.copy2(CAND / 'dinput8.dll', OUT / 'files/dinput8.dll')
cfg = ['[d2coop]', 'enabled=1', '; Optional: the folder holding the SLOTxx save folders, if not the default',
       r'; savegame=C:\Users\<you>\Saved Games\Arkane Studios\Dishonored2\base\savegame', '']
(OUT / 'files/d2coop/d2coop.cfg').write_text('\n'.join(cfg), encoding='ascii')
shutil.copy2(setup, OUT / 'bin/d2coop_setup.exe')
for rel, src in data.items():
    shutil.copy2(src, OUT / rel)
for script in ('Install.cmd', 'Uninstall.cmd', 'Install-D2Coop.ps1', 'Uninstall-D2Coop.ps1'):
    shutil.copy2(REL / script, OUT / script)
shutil.copy2(REL / 'RELEASE-NOTES.md', OUT / 'README.md')
# No package file may be a game file.
originals = {sha(p) for p in ORIG.rglob('*') if p.is_file() and p.stat().st_size < 64 << 20}
shipped = [p for p in OUT.rglob('*') if p.is_file()]
assert not [p for p in shipped if sha(p) in originals], 'a package file equals a game file'
entry = lambda rel: dict(path=rel, sha256=sha(OUT / rel), size=(OUT / rel).stat().st_size)
manifest = dict(name='Dishonored 2 Corvo/Emily co-op', version=VERSION, candidate=CAND.name, built=datetime.datetime.now().astimezone().isoformat(),
                requiredExecutableSha256=sha(LAB / 'Dishonored2.exe'), requiredSteamApiSha256=sha(LAB / 'steam_api64.dll'),
                originalMasterIndexSha256=sha(ORIG / 'base/master.index'),
                files=[entry('files/dinput8.dll') | dict(path='dinput8.dll'), entry('files/d2coop/d2coop.cfg') | dict(path='d2coop/d2coop.cfg')],
                tools=[entry('bin/d2coop_setup.exe')], data=[entry(rel) for rel in data], generated=generated,
                notes=['No game files are shipped: every game-derived file is built by the installer from the player install.',
                       'Never replaces Dishonored2.exe or steam_api64.dll (native Steam).',
                       'Install backs up master.index and any existing dinput8.dll; Uninstall restores them byte-identically.'])
(OUT / 'manifest.json').write_text(json.dumps(manifest, indent=2), encoding='utf-8')
with zipfile.ZipFile(ZIP, 'w', zipfile.ZIP_DEFLATED, compresslevel=6) as z:
    for p in sorted(OUT.rglob('*')):
        if p.is_file():
            z.write(p, f'd2coop-v{VERSION}/' + p.relative_to(OUT).as_posix())
print(json.dumps(dict(out=str(OUT), zip=str(ZIP), zipBytes=ZIP.stat().st_size, zipSha256=sha(ZIP), files=len(shipped) + 1), indent=1))
