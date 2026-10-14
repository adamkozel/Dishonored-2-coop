from pathlib import Path
W=Path(__file__).resolve().parent;N=W.parent/'natural-moveset'
for name in ('Build-DoppelgangerVisual-v475.ps1','prepare_doppelganger_fx_v475.py'):
    s=(W/name).read_text(encoding='utf-8-sig').replace('v475','v479')
    s=s.replace("previous = W / 'candidate-doppelganger-fx-v472'", "previous = W / 'candidate-doppelganger-fx-v475'")
    start=s.find("scope='")
    if start>=0:
        end=s.index("',",start)
        s=s[:start]+"scope='475 plus exact refused joint matrix/sequence capture in source status. Native478 independent later read names leaf anchor_jnt but was already valid again. Bounds unchanged; capture the actual rejected sample before choosing a fix"+s[end:]
    (W/name.replace('v475','v479')).write_text(s,encoding='utf-8')
(N/'Test-DoppelgangerFx-v480.ps1').write_text((N/'Test-DoppelgangerFx-v476.ps1').read_text(encoding='utf-8-sig'),encoding='utf-8')
(W/'Run-DoppelgangerFx-v480.ps1').write_text((W/'Run-DoppelgangerFx-v478.ps1').read_text(encoding='utf-8-sig').replace('v478','v480'),encoding='utf-8')
