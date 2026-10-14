from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
t=(N/'Test-NativeDoorSync-v717.ps1').read_text(encoding='utf-8-sig').replace('v717','v718').replace('NativeDoorSync717','NativeDoorSync718')
t=t.replace("[ordered]@{source=$ds;peer=$dp}|ConvertTo-Json", "[ordered]@{source=$ds;peer=$dp;baselineSource=$samples[0].syncSource;baselinePeer=$samples[0].syncPeer}|ConvertTo-Json")
t=t.replace('-or$ds.rejected-or$dp.rejected','-or$ds.rejected-ne$samples[0].syncSource.rejected-or$dp.rejected-ne$samples[0].syncPeer.rejected')
t=t.replace('Native Steam door ordering, application or duplicate counters differ','Native Steam door ordering, application, duplicate handling or rejection delta differs')
t=t.replace('duplicate packets do not reapply; both finish host revision3','duplicate packets do not reapply; no rejected packets during measured actions; both finish host revision3')
(N/'Test-NativeDoorSync-v718.ps1').write_text(t,encoding='utf-8')
print('Prepared718;717 retained. Scope rejection assertion to captured action interval, with explicit startup baseline.')
