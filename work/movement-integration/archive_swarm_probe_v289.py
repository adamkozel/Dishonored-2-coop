from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-world-probe-v289'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
for kind in ('build','identity'):
 text=(W/f'swarm-world-probe-{kind}-v289.log').read_text(encoding='utf-8-sig');assert text and 'FAIL:' not in text and 'error C' not in text and 'error LNK' not in text
assert sha(R/'work/asset-game/dinput8.dll')=='06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747'
A.mkdir();shutil.copytree(S/'src',A/'src')
shutil.copy2(R/'work/asset-game/dinput8.dll',A/'dinput8.dll');shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
for p in [*W.glob('swarm-world-probe-*-v289.log'),W/'prepare_swarm_scoped_reader_v289.py',W/'Invoke-NativeLabOperation.ps1',R/'work/natural-moveset/Observe-NativeSwarmWorld-v289.ps1']:shutil.copy2(p,A/p.name)
shutil.copy2(R/'work/natural-moveset/swarm-native-world-reader-v288/result.json',A/'retained-full25-test-failure-v288.json')
x=read(W/'candidate-swarm-world-probe-v288/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),probe=sha(A/'d2_runtime_probe.exe'),scope='Read-only Swarm world reader retains contextual/native frame-driver guards and adds scoped current-item ownership validation, avoiding unrelated first-person pose rescans. Current Corvo/inventory membership, equipment entity/declaration/inline parent, map readiness, quantity and selection still checked every frame. Identity tests pass including selection changes, pending transitions and foreign owners. v288 validated221/235 samples but its obsolete fixed25-count gate failed when native agents disappeared; v289 requires an initial25 snapshot plus sustained post-spawn coverage of currently indexed agents. Live verification pending. Proxy/wire47/bank50 unchanged; no remote rat/VFX/gameplay implementation.')
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8');shutil.copy2(A/'d2_runtime_probe.exe',S/'bin/d2_runtime_probe.exe')
print(json.dumps({k:x[k] for k in ('dll','probe','protocol','bankVersion')}))
