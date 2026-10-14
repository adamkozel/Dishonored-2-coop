from pathlib import Path
W=Path(__file__).resolve().parent;N=W.parent/'natural-moveset'
for name in ('Build-DoppelgangerVisual-v472.ps1','prepare_doppelganger_fx_v472.py'):
    s=(W/name).read_text(encoding='utf-8-sig').replace('v472','v475')
    s=s.replace("previous = W / 'candidate-doppelganger-fx-v468'", "previous = W / 'candidate-doppelganger-fx-v472'")
    s=s.replace('468 native cosmetic scene timing fix with bounded visibility-gate evidence on hide transitions. Existing fail-closed behavior unchanged; no temporary function hooks or hardware watch. Investigates rare pre-retirement hide in460/470.', 'Native finite cosmetic FX force-update retained. Successful clone rediscovery immediately restores33ms active sampling instead of leaving500ms discovery backoff beyond300ms receiver lease. Exact visibility evidence retained; no lease relaxation or extra hook.')
    (W/name.replace('v472','v475')).write_text(s,encoding='utf-8')
s=(N/'Test-DoppelgangerFx-v469.ps1').read_text(encoding='utf-8-sig')
# Preserve the original proven summon-facing camera for two-cast functional
# coverage. A separate camera take will establish unobstructed disappearance.
(N/'Test-DoppelgangerFx-v476.ps1').write_text(s,encoding='utf-8')
s=(W/'Run-DoppelgangerFx-v455.ps1').read_text(encoding='utf-8-sig').replace('v455','v476').replace('v456','v477').replace('v454','v476').replace('Reuse454','Reuse476')
(W/'Run-DoppelgangerFx-v476.ps1').write_text(s,encoding='utf-8')
