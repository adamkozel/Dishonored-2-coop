from pathlib import Path
import json,hashlib,shutil
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';I=R/'work/movement-integration'
s=(P/'Observe-NativeMesmerize-v71.ps1').read_text()
s=s.replace('--watch-midnight','--watch-power-scenes').replace('midnight.ndjson','power-scenes.ndjson').replace('midnight.stderr','power-scenes.stderr')
s=s.replace('$scenes=@($fastScenes|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-match\'power_mesmerize_cast_\'})',"$scenes=@($fastScenes|Where-Object {$_.valid-and$_.variant-in16,17,18})")
mark=" $checks.Add('Native controller input produces exact Mesmerize enter, aim-loop and release scenes')"
assert mark in s
s=s.replace(mark,""" if(@($scenes|Where-Object {$_.armSide-ne1-or$_.character-ne2-or!$_.armHandle}).Count){throw 'Owned Emily left-arm scope failed'}
 $checks.Add('Strict scene reader validates all three native phases with reciprocal owned Emily left arm; default production path remains disabled')""")
s=s.replace('phase not observed','strict phase not observed')
(P/'Observe-NativeMesmerize-v72.ps1').write_text(s)
p=I/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Observe-NativeMesmerize-v71.ps1','Observe-NativeMesmerize-v72.ps1');p.write_text(s)
A=I/'candidate-mesmerize-scene-v72';A.mkdir(exist_ok=False)
shutil.copytree(R/'outputs/d2-coop-lab/src',A/'src')
for name,src in [('dinput8.dll',R/'outputs/d2-coop-lab/bin/dinput8.dll'),('d2_runtime_probe.exe',R/'work/build/Release/d2_runtime_probe.exe'),('Observe-NativeMesmerize-v72.ps1',P/'Observe-NativeMesmerize-v72.ps1'),('Invoke-NativeLabOperation.ps1',p)]:shutil.copy2(src,A/name)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
(A/'identity.json').write_text(json.dumps(dict(dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),protocol=39,bankVersion=44,clips=273,runtimeVerified=False,scope='Unchanged v70 live DLL. New opt-in strict Mesmerize scene reader in probe only; runtime three-phase ownership validation pending. Initial v72 test build C2664 fixed in v72b; only v72b identity-test binary is current.'),indent=2))
shutil.copy2(A/'d2_runtime_probe.exe',R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
print('Preserved v72 scene candidate and installed read-only probe; live DLL unchanged')
