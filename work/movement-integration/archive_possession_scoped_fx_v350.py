from pathlib import Path
import json,hashlib,shutil,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab';A=W/'candidate-possession-scoped-fx-v350'
assert not A.exists()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
for name in ['possession-scoped-fx-build-v350.log','possession-scoped-fx-tests-v350.log']:
 text=(W/name).read_text(encoding='utf-8-sig');assert text and not any(t in text for t in ['error C','error LNK','FAIL:'])
assert 'PASS: selected Possession target' in (W/'possession-scoped-fx-tests-v350.log').read_text(encoding='utf-8-sig')
s=(P/'Observe-PossessionFx-v349.ps1').read_text();anchor=" $checks.Add('Read-only source Possession FX records captured with target-context stability and candidate controller slots; no remote VFX invocation')";assert anchor in s
s=s.replace(anchor,anchor+'''
 $scoped=@($fx|Where-Object {$_.scoped.valid-and$_.scoped.present})
 $matched=@($scoped|Where-Object {$r=$_; $r.valid-and@($r.effects|Where-Object {$_.id-eq$r.scoped.id-and$_.owner-eq$r.scoped.owner-and$_.name-eq'models/effects/gameplay/powers/possession/hosts_target/possession_target_human_01.fx'}).Count-eq1})
 [ordered]@{samples=$fx.Count;valid=@($fx|Where-Object {$_.scoped.valid}).Count;selected=@($fx|Where-Object {$_.scoped.valid-and$_.scoped.selected}).Count;present=$scoped.Count;fullScanMatched=$matched.Count;maxScopedMs=($fx.scoped.ms|Measure-Object -Maximum).Maximum;stages=@($fx|Group-Object {$_.scoped.stage}|Select-Object Name,Count);scope='Read-only exact selected human NPC component and native FX bucket lookup; no remote invocation'}|ConvertTo-Json -Depth 6|Set-Content "$out/scoped-fx-summary.json"
 if($scoped.Count-lt3-or$matched.Count-lt3){throw 'Scoped human target FX ownership not demonstrated'}
 $checks.Add('Scoped selected NPC generation, exact declaration, temporary FX actor and bucket lookup match independently inventoried native human target particles')
''');(P/'Observe-PossessionFx-v350.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Observe-PossessionFx-v349.ps1')==1;s=s.replace('Observe-PossessionFx-v349.ps1','Observe-PossessionFx-v350.ps1');p.write_text(s,encoding='utf-8')
A.mkdir();shutil.copytree(S/'src',A/'src');shutil.copy2(W/'candidate-possession-exit-v343/dinput8.dll',A/'dinput8.dll');shutil.copy2(R/'work/build/Release/d2_runtime_probe.exe',A/'d2_runtime_probe.exe')
for p in [Path(__file__),W/'prepare_possession_scoped_fx_v350.py',W/'Build-PossessionScopedFx-v350.ps1',W/'possession-scoped-fx-build-v350.log',W/'possession-scoped-fx-tests-v350.log',P/'Observe-PossessionFx-v350.ps1']:shutil.copy2(p,A/p.name)
x=json.loads((W/'candidate-possession-fx-research-v348/identity.json').read_text());x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,scope='Production DLL343 remains unchanged. Read-only probe350 adds generation-validated selected human NPC target-effect reader and direct FX ID bucket lookup; native ownership/race tests pass. Runtime correlation pending. No effect invocation or broad game-hook scan.')
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8');shutil.copy2(A/'d2_runtime_probe.exe',S/'bin/d2_runtime_probe.exe')
print(json.dumps(dict(archive=A.name,probe=x['probe'],dll=x['dll'])))
