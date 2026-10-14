from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab';P=R/'work/natural-moveset'
def edit(p,a,b):
 s=p.read_text();assert a in s,(p,a);p.write_text(s.replace(a,b))
edit(S/'src/engine_bridge.cpp','writeSwarmWorld();writeSwarmSpawnFx();writeSwarmRatVisual();','writeSwarmWorld();writeSwarmSpawnFx();writeSwarmTrails();writeSwarmRatVisual();')
edit(S/'src/engine_bridge.cpp','presentSwarmWorld(tick);serviceSwarmRatVisual(tick);serviceSwarmSpawnFx(tick);','presentSwarmWorld(tick);serviceSwarmRatVisual(tick);serviceSwarmSpawnFx(tick);serviceSwarmTrails(tick);')
edit(W/'Invoke-NativeLabOperation.ps1','Test-SwarmRatVisualNetwork-v317.ps1','Test-SwarmRatVisualNetwork-v318.ps1')
s=(P/'Test-SwarmRatVisualNetwork-v317.ps1').read_text().replace('(124+72*$_.peer.count)','(124+128*$_.peer.count)')
s=s.replace(" $visualBaseline=S client '.swarm-rat-visual.json'"," $trailsSourceBaseline=S host '.swarm-trails.json';$trailsPeerBaseline=S client '.swarm-trails.json'\n [ordered]@{source=$trailsSourceBaseline;peer=$trailsPeerBaseline}|ConvertTo-Json -Depth 10|Set-Content \"$out/trails-baseline.json\"\n $visualBaseline=S client '.swarm-rat-visual.json'")
s=s.replace("nativeFxPeer=(S client '.power-fx.json')})","nativeFxPeer=(S client '.power-fx.json');trailsSource=(S host '.swarm-trails.json');trailsPeer=(S client '.swarm-trails.json')})")
needle=" $checks.Add('One exact source-owned summon FX record transmits through native Steam and starts one owned remote native particle/sound effect; duplicate rejected and finite effect retired')"
assert needle in s
s=s.replace(needle,needle+'''
 $trailsSource=S host '.swarm-trails.json';$trailsPeer=S client '.swarm-trails.json'
 [ordered]@{source=$trailsSource;peer=$trailsPeer}|ConvertTo-Json -Depth 12|Set-Content "$out/trails-summary.json"
 if(($trailsSource.sourceDiscovered-$trailsSourceBaseline.sourceDiscovered)-ne7-or($trailsPeer.started-$trailsPeerBaseline.started)-ne7-or
    ($trailsPeer.refused-$trailsPeerBaseline.refused)-ne0-or($trailsPeer.updated-$trailsPeerBaseline.updated)-lt100-or
    ($trailsPeer.interpolated-$trailsPeerBaseline.interpolated)-lt50-or$trailsPeer.maxReadbackError-gt.00001-or$trailsPeer.active-ne0){throw 'Rat trail presentation count, transform, interpolation or retirement failed'}
 $trailRecords=@($worldRows|ForEach-Object{$_.trailsPeer.effects}|Where-Object{$_.active})
 if(@($trailRecords.sourceId|Sort-Object -Unique).Count-ne7-or@($trailRecords.nativeId|Sort-Object -Unique).Count-ne7){throw 'Seven unique native trail owners were not observed'}
 $checks.Add('Seven strictly rat-attached source trails travel in version2 native Steam world snapshots, start once each on peer, interpolate source effect transforms with exact native record readback, and retire without gameplay calls')
''')
s=s.replace('A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration; no remote damage or gameplay swarm.','A separate exact-source-owned channel plays the finite native-derived summon VFX with sound configuration. Version2 world packets also carry7 strictly rat-attached native spawn trails and their interpolated source transforms; no remote damage or gameplay swarm.')
(P/'Test-SwarmRatVisualNetwork-v318.ps1').write_text(s)
s=(W/'archive_swarm_spawn_fx_v315.py').read_text().replace('swarm-spawn-fx-v315','swarm-trails-v318').replace('swarm-spawn-fx-{kind}-v315','swarm-trails-{kind}-v318').replace('prepare_swarm_spawn_fx_v315.py','prepare_swarm_trails_v318.py').replace('Test-SwarmRatVisualNetwork-v315.ps1','Test-SwarmRatVisualNetwork-v318.ps1').replace('swarm-spawn-fx-*-v315.log','swarm-trails-*-v318.log')
begin=s.index("scope='Candidate315:");end=s.index("')",begin)
s=s[:begin]+"scope='Candidate318: version2 rat world packet carries optional strictly attributed native rat spawn-trail IDs, ages and transforms. Bounded source discovery7/16scans/750ms, read-only native frame/pool ownership checks, cached record samples expire at2500ms. Receiver starts each source ID once within200ms, interpolates matching IDs only, updates guarded owned particle record with no callback, preserves tombstones through lease gaps, and retires on source absence/scope changes. Native particle declaration reused unchanged; no AI/damage or first-person modification. Runtime verification pending."+s[end:]
(W/'archive_swarm_trails_v318.py').write_text(s)
print('Prepared318 bounded source trail snapshots and peer-only native particle presentation')
