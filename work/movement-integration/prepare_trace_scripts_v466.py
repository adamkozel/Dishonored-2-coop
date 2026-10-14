from pathlib import Path
W=Path(__file__).resolve().parent
N=W.parent/'natural-moveset'
for name in ('Build-DoppelgangerVisual-v464.ps1','prepare_doppelganger_fx_v464.py'):
    text=(W/name).read_text(encoding='utf-8-sig').replace('v464','v466')
    text=text.replace("previous = W / 'candidate-doppelganger-fx-v462'", "previous = W / 'candidate-doppelganger-fx-v464'")
    text=text.replace('Diagnostic hardware write watch on the exact owned disappearance scene state for three seconds; previous pause entry hooks removed. Presentation unchanged; remove watch after investigation.', 'Diagnostic owned-scene hardware watch with explicit engine-thread arming before native start. Bounded three seconds, original registers restored. Previous asynchronous watch missed the initial transition; not a release candidate.')
    (W/name.replace('v464','v466')).write_text(text,encoding='utf-8')
(N/'Test-DoppelgangerFx-v467.ps1').write_text((N/'Test-DoppelgangerFx-v465.ps1').read_text(encoding='utf-8-sig'),encoding='utf-8')
(W/'Run-DoppelgangerFx-v467.ps1').write_text((W/'Run-DoppelgangerFx-v465.ps1').read_text(encoding='utf-8-sig').replace('v465','v467'),encoding='utf-8')
