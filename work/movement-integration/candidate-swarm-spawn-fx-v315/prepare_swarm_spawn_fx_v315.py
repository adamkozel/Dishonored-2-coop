from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab';W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=S/'Send-D2Command.ps1';s=p.read_text();assert '|coop_swarm_fx|' not in s;s=s.replace('|coop_swarm_fixture|','|coop_swarm_fixture|coop_swarm_fx|')
needle="$workspaceRoot=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent"
assert needle in s;s=s.replace(needle,"if($Command -like 'coop_swarm_fx*' -and $Command -ne 'coop_swarm_fx duplicate'){throw 'Swarm FX test accepts only duplicate of a previously observed source event.'}\n"+needle);p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert 'Test-SwarmRatVisualNetwork-v314.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v314.ps1','Test-SwarmRatVisualNetwork-v315.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v314.ps1').read_text();needle=" $visualBaseline=S client '.swarm-rat-visual.json'";assert needle in s
s=s.replace(needle," C host 'coop_swarm_fx duplicate' # Idle no-op validates command admission before the cast\n"+needle)
s=s.replace('peer animation does not execute rat, VFX or damage events.','peer body animation does not execute gameplay events. A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration; no remote damage or gameplay swarm.')
(P/'Test-SwarmRatVisualNetwork-v315.ps1').write_text(s)
s=(W/'archive_swarm_spawn_fx_v314.py').read_text().replace('v314','v315')
s=s.replace("S/'Build-D2SwarmSpawnFxAssets.ps1'","S/'Build-D2SwarmSpawnFxAssets.ps1',S/'Send-D2Command.ps1'")
s=s.replace("runtimeVerified=False,scope=","assetHashes={p.name:sha(p)for p in(A/'assets').iterdir()},runtimeVerified=False,scope=")
s=s.replace('Candidate314:', 'Candidate315:314 started one native receiver effect but duplicate-test command was missing from both allowlists. Exact duplicate command admitted in engine and sender; idle preflight added. ')
(W/'archive_swarm_spawn_fx_v315.py').write_text(s)
print('Prepared315 exact duplicate command admission and source-compatible test')
