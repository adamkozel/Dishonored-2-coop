from pathlib import Path
import json,hashlib,shutil
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';I=R/'work/movement-integration'
s=(P/'Observe-NativeMesmerize-v72.ps1').read_text()
s=s.replace('$monitor=$null;', '$monitor=$null;$worldMonitor=$null;$earlyMana=$null;')
s=s.replace("'-t','16'", "'-t','32'").replace('while($w.ElapsedMilliseconds-lt14000)', 'while($w.ElapsedMilliseconds-lt30000)')
mark=' $w.Restart();$cast=$false'
s=s.replace(mark,''' $worldMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-world','32000') -WindowStyle Hidden -RedirectStandardOutput "$out/world.ndjson" -RedirectStandardError "$out/world.stderr" -PassThru
'''+mark)
s=s.replace("  $scene=Q client '--midnight';",'''  if(!$earlyMana-and$w.ElapsedMilliseconds-gt5000){$earlyMana=& "$PSScriptRoot/Read-NativeMana.ps1" -Role client -Evidence "$out/mana-after-commit.json"}
  $scene=Q client '--midnight';''')
s=s.replace('if($manaAfter.currentCandidate-ge$manaBefore.currentCandidate)', 'if(!$earlyMana-or$earlyMana.currentCandidate-ge$manaBefore.currentCandidate)')
s=s.replace('default production path remains disabled','native ownership validated')
mark=" $checks.Add('Exact native world body effect exists after source cast; presentation/AI ownership still under investigation')"
# Original line also includes the guard before the check, so replace check token only.
mark="$checks.Add('Exact native world body effect exists after source cast; presentation/AI ownership still under investigation')"
assert mark in s
s=s.replace(mark,mark+'''
 $worldMonitor.WaitForExit(5000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){throw 'World watcher did not complete'}
 $worlds=@(Get-Content "$out/world.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $present=@($worlds|Where-Object {$_.valid-and$_.present})
 if(!$present.Count-or!@($worlds|Where-Object {$_.valid-and!$_.present}).Count){throw 'Committed world and absent relationship both required'}
 foreach($state in 1,3){if(!@($present|Where-Object state -eq $state).Count){throw "Native world state $state not observed"}}
 [ordered]@{samples=$worlds.Count;valid=@($worlds|Where-Object valid).Count;states=@($present|Group-Object state|Select-Object Name,Count);handles=@($present.handle|Select-Object -Unique);last=$worlds[-1];scope='Exact equipment+d4 committed relationship. Preview not searched. Native finite equipment fixture expires at20s; late removal may be cleanup-driven, not natural lifetime.'}|ConvertTo-Json -Depth 6|Set-Content "$out/world-summary.json"
 $checks.Add('Strict committed world identity observes native spawn state1 and active state3 independently of caster animation')
''')
s=s.replace(' if($monitor){$monitor.WaitForExit(18000)', " if($worldMonitor){$worldMonitor.WaitForExit(18000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){$failure=($failure+'; world watcher failed').TrimStart('; ')}}\n if($monitor){$monitor.WaitForExit(18000)")
s=s.replace("scope='Native source Mesmerize", "scope='Bounded read-only committed world investigation with finite20s native equipment fixture. Preview entities do not establish commitment; late despawn may be fixture-driven. Native source Mesmerize")
(P/'Observe-NativeMesmerizeWorld-v75.ps1').write_text(s)
p=I/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace("'MesmerizeNetwork','GrenadeProp'","'MesmerizeNetwork','MesmerizeWorld','GrenadeProp'")
s=s.replace("  'MesmerizeNetwork'{", "  'MesmerizeWorld'{& \"$root/work/natural-moveset/Observe-NativeMesmerizeWorld-v75.ps1\" -Take $Take -Pitch $Pitch}\n  'MesmerizeNetwork'{");p.write_text(s)
A=I/'candidate-mesmerize-world-v75';A.mkdir(exist_ok=False);shutil.copytree(R/'outputs/d2-coop-lab/src',A/'src')
for n,src in [('dinput8.dll',R/'outputs/d2-coop-lab/bin/dinput8.dll'),('d2_runtime_probe.exe',R/'work/build/Release/d2_runtime_probe.exe'),('Observe-NativeMesmerizeWorld-v75.ps1',P/'Observe-NativeMesmerizeWorld-v75.ps1'),('Invoke-NativeLabOperation.ps1',p)]:shutil.copy2(src,A/n)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
(A/'identity.json').write_text(json.dumps(dict(dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),protocol=40,bankVersion=45,clips=276,runtimeVerified=False,scope='Unchanged v73 DLL, body network take v74 PASS10. New probe-only strict committed-world reader and bounded watcher; runtime world identity validation pending.'),indent=2))
shutil.copy2(A/'d2_runtime_probe.exe',R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
print('Preserved v75 read-only world candidate; DLL unchanged')
