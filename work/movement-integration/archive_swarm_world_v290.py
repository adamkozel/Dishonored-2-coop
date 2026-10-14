from pathlib import Path
import json,shutil,hashlib,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';A=W/'candidate-swarm-world-v290'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
assert not A.exists()
logs=[W/f'swarm-world-{kind}-v290{suffix}.log' for kind,suffix in [('build',''),('build','-final'),('identity',''),('protocol',''),('smoke','')]]
for p in logs:
 s=p.read_text(encoding='utf-8-sig');assert s and 'FAIL:' not in s and 'error C' not in s and 'error LNK' not in s
assert sha(R/'work/asset-game/dinput8.dll')=='06E1DA2C679CFF6E420F9BCA8E0E2B3558F2CFB399A21D982C91DE5D9E1A0747'
A.mkdir();shutil.copytree(S/'src',A/'src')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/b,A/b)
for p in logs+[W/'prepare_swarm_world_network_v290.py',W/'Invoke-NativeLabOperation.ps1',R/'work/natural-moveset/Test-SwarmWorldNetwork-v290.ps1']:shutil.copy2(p,A/p.name)
x=read(W/'candidate-swarm-world-probe-v289/identity.json');x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),swarmWorldProtocol=1,swarmWorldHeaderBytes=124,swarmAgentBytes=72,swarmWorldMaxAgents=100,runtimeVerified=False,scope='Strict native Swarm snapshots over native Steam. Variable bounded agent payload, scoped ownership, incarnation/state/rigid pose/scale guards, paired world/session/token/root clocks and replay order. Telemetry retains exact sent/accepted wire digests. Native absence, stream reset and short peer lease retire stale data. No remote rat spawn, VFX, AI or damage invocation. Existing semantic wire47 and bank50/313 unchanged. Build, native identity, packet and proxy load checks passed; live paired proof pending.')
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
for b in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(A/b,S/'bin'/b)
print(json.dumps({k:x[k] for k in ('dll','probe','protocol','bankVersion','swarmWorldProtocol')}))
