from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(N/'Test-SwarmNetwork-v270.ps1').read_text(encoding='utf-8-sig')
s=s.replace('Swarm','Doppelganger').replace('swarm','doppelganger').replace('Corvo','Emily')
s=s.replace('host','ROLE_SWAP').replace('client','host').replace('ROLE_SWAP','client')
s=s.replace('devouring_doppelganger/devouring_doppelganger.inventoryitem','doppelganger/doppelganger.inventoryitem').replace('doppelganger-banks-v267/corvo-doppelganger-bank.bin','doppelganger-banks-v407/emily-doppelganger-bank.bin')
s=s.replace('$m.version-eq49','$m.version-eq55').replace('$m.version-ne49','$m.version-ne55').replace('bank49/304','bank55/322').replace('bank49 poses','bank55 poses').replace('23,24,25','29,30,31')
s=s.replace("'coop_doppelganger_fixture off','coop_movement_fixture off'","'coop_swarm_fixture off','coop_possession_fixture off','coop_doppelganger_fixture off','coop_movement_fixture off'")
s=s.replace("'^models/characters/animals/rat/devouring_doppelganger(_big)?\\.def$'","'^models/powers/doppelganger/(decoy_entity|deadlyshadow_entity)(|_corvo)\\.def$'")
s=s.replace('nativeRats','nativeClones').replace('peerRats','peerClones').replace('native rats','native clones').replace('peer rats','peer clones').replace('source rats','source clones').replace('rat, VFX','clone, VFX').replace('world/rat','world/clone')
# One registered partner uses a separate mod definition. Only native power defs
# count here, and cleanup must retire them through the native checkpoint.
assert 'devouring' not in s and 'corvo-' not in s
p=N/'Test-DoppelgangerNetwork-v408.ps1';assert not p.exists();p.write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerObserve-v406.ps1').read_text(encoding='utf-8-sig').replace('native-doppelganger-source-v406','doppelganger-network-v408').replace('DoppelgangerObserve','DoppelgangerNetwork').replace('Observe-NativeDoppelganger-v406.ps1','Test-DoppelgangerNetwork-v408.ps1')
(W/'Run-DoppelgangerNetwork-v408.ps1').write_text(s,encoding='utf-8')
print('Prepared source/observer native recordings with phase identity and recovery checks408')
