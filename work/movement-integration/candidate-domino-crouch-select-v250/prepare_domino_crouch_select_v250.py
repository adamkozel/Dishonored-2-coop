from pathlib import Path
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; P=R/'work/natural-moveset'
s=(P/'Test-DominoCrouchNetwork-v246.ps1').read_text(encoding='utf-8-sig')
s=s.replace('$monitor=$null;', '$monitor=$null;$rawMonitor=$null;')
anchor=" $w.Restart();$cast=$false;$stood=$false"
s=s.replace(anchor," $rawMonitor=Start-Process \"$lab/bin/d2_runtime_probe.exe\" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput \"$out/midnight.ndjson\" -RedirectStandardError \"$out/midnight.stderr\" -PassThru\n"+anchor)
s=s.replace('stream=$stream;effects=$fx','stream=$stream;sourceStream=(S client \'.shadow-stream.json\');effects=$fx')
s=s.replace('$_.variant-in19,20,22','$_.variant-in19,20,21,22')
anchor=' foreach($v in 19,20,22)'
s=s.replace(anchor," $phases=@(19,20,22);if(@($scenes|Where-Object variant -eq 21).Count){$phases=@(19,20,21,22)}\n foreach($v in $phases)")
s=s.replace('foreach($variant in 19,20,22)','foreach($variant in $phases)').replace('$_.movement.powerVariant-in19,20,22','$_.movement.powerVariant-in19,20,21,22')
s=s.replace(" if(@((Q host '--domino-effects').effects).Count){throw 'Source-only world effect leaked into observer world'}", " [ordered]@{source=(OptionalFx client);peer=(OptionalFx host)}|ConvertTo-Json -Depth 12|Set-Content \"$out/world-effects-after.json\"")
s=s.replace(" if($nativeMarks.Count){throw 'Empty crouched cast unexpectedly selected a target'}", " # Native target selection is allowed; existing scoped cosmetic replication is recorded separately.")
s=s.replace('crouchedPhaseSamples=$castRows.Count;', 'observedPhases=$phases;nativeTargetEffectSamples=$nativeMarks.Count;crouchedPhaseSamples=$castRows.Count;')
s=s.replace('three observed crouched phases', 'observed crouched phases including ordinary selection when targeted')
s=s.replace('Three native crouched subscene transitions', 'Observed native crouched subscene transitions')
s=s.replace('All three native phases', 'All observed native phases').replace('or native target events','and preserve any native target events')
s=s.replace('no duplicated observer world effect','source and peer world effects recorded without claiming shared NPC outcomes')
s=s.replace('crouched empty-target controller casting drive three observed', 'crouched controller casting drives observed')
s=s.replace('This take exercises no target marks or links. Their earlier scoped presentation proofs remain separate;', 'This take permits native target selection and records its effects. Earlier scoped mark/beam presentation proofs remain separate;')
anchor=" foreach($r in 'host','client'){try{C $r 'coop_movement_fixture off'}catch{}}"
s=s.replace(anchor," if($rawMonitor){$rawMonitor.WaitForExit(18000)|Out-Null;if(!$rawMonitor.HasExited-or$rawMonitor.ExitCode){$failure=($failure+'; raw watcher failed').TrimStart('; ')}}\n"+anchor)
(P/'Test-DominoCrouchNetwork-v250.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig').replace('Test-DominoCrouchNetwork-v246.ps1','Test-DominoCrouchNetwork-v250.ps1');p.write_text(s,encoding='utf-8')
a=(W/'Archive-DominoCrouch-v246.ps1').read_text(encoding='utf-8-sig')
a=a.replace('candidate-domino-crouch-v246','candidate-domino-crouch-select-v250').replace('domino-crouch-build-v246','domino-crouch-select-build-v250').replace('domino-crouch-identity-v246','domino-crouch-select-identity-v250').replace('domino-crouch-wire-v246','domino-crouch-select-wire-v250').replace('domino-crouch-smoke-v246','domino-crouch-select-smoke-v250')
a=a.replace('prepare_domino_crouch_network_v246.py','prepare_domino_crouch_select_v250.py').replace('Test-DominoCrouchNetwork-v246.ps1','Test-DominoCrouchNetwork-v250.ps1')
begin=a.index("scope='");end=a.index("'}|ConvertTo-Json",begin)
a=a[:begin]+"scope='Native v249 directly confirms crouched ordinary Cast_Select400 with reciprocal Emily bindings and Sneaking6/Domino72. Reader now accepts it; crouched final selection remains refused. Standing final recovery retained. Bank48/301 payloads unchanged from candidate246 with its full format/actual-bank checks retained. This candidate adds reader ownership/state fixtures, wire and load checks. Two-player crouch runtime and visual acceptance pending."+a[end:]
(W/'Archive-DominoCrouchSelect-v250.ps1').write_text(a,encoding='utf-8')
print('Prepared crouched ordinary selection harness/archive v250; bank48 unchanged.')
