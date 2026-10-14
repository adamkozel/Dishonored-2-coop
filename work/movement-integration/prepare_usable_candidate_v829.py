from pathlib import Path
W=Path(__file__).resolve().parent
s=(W/'stage_coin_sync_v821.py').read_text()
s=s.replace("A=W/'candidate-coin-sync-v821'; P=W/'candidate-coin-sync-v814'", "A=W/'candidate-usable-state-v829'; P=W/'candidate-coin-sync-v821'")
s=s.replace('v821.log','v829.log').replace('Build-CoinSync-v821.ps1','Build-UsableState-v829.ps1').replace('Deploy-CoinSync-v821.ps1','Deploy-UsableState-v829.ps1').replace('stage_coin_sync_v821.py','stage_usable_state_v829.py')
old="Bounded shared currency/ammo/stored-potion catalog with native capacity-aware outcomes, per-item receipts, serialized native reward transactions, Steam host commits and checkpoint rebind; packet version4 with native food queued-healing outcomes, exact owned vital readers and bounded full-health behavior. Runtime pending."
s=s.replace(old,"Preserves821 shared pickup families and all prior co-op behavior. Adds owned read-only native state-list inspection and a finite local indexed-window transition fixture. No shared usable protocol yet; native API runtime verification pending.")
(W/'stage_usable_state_v829.py').write_text(s)
s=(W/'Deploy-CoinSync-v821.ps1').read_text().replace('CoinSyncDeploy821','UsableStateDeploy829').replace('stage_coin_sync_v821.py','stage_usable_state_v829.py').replace('candidate-coin-sync-v821','candidate-usable-state-v829')
(W/'Deploy-UsableState-v829.ps1').write_text(s)
