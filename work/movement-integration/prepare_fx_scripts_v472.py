from pathlib import Path
W=Path(__file__).resolve().parent;N=W.parent/'natural-moveset'
for name in ('Build-DoppelgangerVisual-v468.ps1','prepare_doppelganger_fx_v468.py'):
    s=(W/name).read_text(encoding='utf-8-sig').replace('v468','v472')
    s=s.replace("previous = W / 'candidate-doppelganger-fx-v452'", "previous = W / 'candidate-doppelganger-fx-v468'")
    s=s.replace('452 baseline plus native force-update creation option for owned finite FX/material clone scenes, avoiding LOD pause while culled. All temporary pause hooks and hardware watches removed. Runtime confirmation pending.', '468 native cosmetic scene timing fix with bounded visibility-gate evidence on hide transitions. Existing fail-closed behavior unchanged; no temporary function hooks or hardware watch. Investigates rare pre-retirement hide in460/470.')
    (W/name.replace('v468','v472')).write_text(s,encoding='utf-8')
s=(N/'Test-DoppelgangerFx-v469.ps1').read_text(encoding='utf-8-sig')
s=s.replace("C host 'setviewpos -47.39 -156.87 68.1 90 0'", "C host 'setviewpos -54 -127 62.7 45 0'")
(N/'Test-DoppelgangerFx-v473.ps1').write_text(s,encoding='utf-8')
(W/'Run-DoppelgangerFx-v473.ps1').write_text((W/'Run-DoppelgangerFx-v469.ps1').read_text(encoding='utf-8-sig').replace('v469','v473'),encoding='utf-8')
