from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
def edit(path,old,new):
    t=path.read_text();assert t.count(old)==1,(path,old,t.count(old));path.write_text(t.replace(old,new))
edit(S/'engine_bridge.cpp','serviceCoinFixture(tick);','serviceCoinFixture(tick);serviceCoinSync(tick);')
edit(S/'engine_bridge.cpp','!installDamageObserver()||','!installDamageObserver()||!installCoinRewardObserver()||')
edit(S/'checkpoint_sync.h','if(doorSession.enabled)stopDoorSession("checkpoint_failed");','if(doorSession.enabled)stopDoorSession("checkpoint_failed");\n    if(coinSync.enabled)stopCoinSync("checkpoint_failed");')
edit(S/'checkpoint_sync.h','))finishDoorCheckpointRecovery();',')){finishDoorCheckpointRecovery();finishCoinCheckpointRecovery();}')
edit(S/'checkpoint_sync.h','    beginDoorCheckpointRecovery();','    beginDoorCheckpointRecovery();beginCoinCheckpointRecovery();')
edit(R/'outputs/d2-coop-lab/Send-D2Command.ps1','|coop_coin_fixture|','|coop_coin_fixture|coop_coin_sync|')
edit(S/'coin_fixture.h','std::memcmp(gameImage+0x841030,grant,sizeof(grant))','!nativeCoinApiAvailable()')
edit(S/'coin_fixture.h','reinterpret_cast<void(*)(void*,void*,unsigned char)>(gameImage+0x841030)','realCoinReward')
edit(S/'world_identity_tests.cpp','#include "native_coin_tests.h"','#include "native_coin_tests.h"\n#include "coin_protocol_tests.h"')
edit(S/'world_identity_tests.cpp','    nativeCoinTests();','    nativeCoinTests();coinProtocolTests();')
W=R/'work/movement-integration'
t=(W/'Build-CoinFixture-v790.ps1').read_text().replace('790','792')
(W/'Build-CoinSync-v792.ps1').write_text(t)
t=(W/'Deploy-CoinFixture-v790.ps1').read_text().replace('790','792').replace('CoinFixture','CoinSync').replace('coin-fixture','coin-sync').replace('stage_coin_fixture','stage_coin_sync')
(W/'Deploy-CoinSync-v792.ps1').write_text(t)
t=(W/'stage_coin_fixture_v790.py').read_text().replace('790','792').replace("P=W/'candidate-coin-fixture-v788'","P=W/'candidate-coin-fixture-v790'").replace("A=W/'candidate-coin-fixture-v792'","A=W/'candidate-coin-sync-v792'").replace('CoinFixture-v792','CoinSync-v792').replace('stage_coin_fixture_v792','stage_coin_sync_v792')
start=t.index("scope='Exact coin");end=t.index("')",start)
t=t[:start]+"scope='Native Steam exact-coin channel with guarded ordinary reward hook, authored identity, per-world once-only receipts, host commits, menu retention and coordinated checkpoint rebind. Runtime pending."+t[end:]
(W/'stage_coin_sync_v792.py').write_text(t)
print('Wired coin channel and generated792 build/deploy scripts')
