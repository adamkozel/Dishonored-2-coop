from pathlib import Path

W = Path(__file__).resolve().parent
N = W.parent / 'natural-moveset'
for source, target in [('Build-DoppelgangerVisual-v462.ps1', 'Build-DoppelgangerVisual-v464.ps1'),
                       ('prepare_doppelganger_fx_v462.py', 'prepare_doppelganger_fx_v464.py')]:
    text = (W / source).read_text(encoding='utf-8-sig').replace('v462', 'v464')
    if source.endswith('.py'):
        text = text.replace("previous = W / 'candidate-doppelganger-fx-v459'", "previous = W / 'candidate-doppelganger-fx-v462'")
        text = text.replace('Diagnostic-only native manager and deferred pause passthrough traces for owned cosmetic FX scenes; presentation unchanged. Remove traces after investigation.',
                            'Diagnostic hardware write watch on the exact owned disappearance scene state for three seconds; previous pause entry hooks removed. Presentation unchanged; remove watch after investigation.')
    (W / target).write_text(text, encoding='utf-8')
text = (N / 'Test-DoppelgangerFx-v463.ps1').read_text(encoding='utf-8-sig')
text = text.replace('}catch{$failure=$_.Exception.Message}finally{', '''}catch{$failure=$_.Exception.Message}finally{
 $traceFile="$root/work/lab-instances/host/coop-status.json.midnight-phase-trace.json"
 if(Test-Path -LiteralPath $traceFile){Copy-Item -LiteralPath $traceFile -Destination "$out/midnight-phase-trace.json"}
''')
(N / 'Test-DoppelgangerFx-v465.ps1').write_text(text, encoding='utf-8')
(W / 'Run-DoppelgangerFx-v465.ps1').write_text((W / 'Run-DoppelgangerFx-v463.ps1').read_text(encoding='utf-8-sig').replace('v463', 'v465'), encoding='utf-8')
