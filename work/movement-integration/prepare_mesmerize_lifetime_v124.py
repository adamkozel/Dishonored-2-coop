"""Observe native expiry while equipment remains selected; never write a timer."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-MesmerizeLinks-v106.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep('$targetMonitor=$null;', '$targetMonitor=$null;$worldMonitor=$null;')
rep("'coop_mesmerize_fixture 20000'", "'coop_mesmerize_fixture 40000'")
rep("'-t','34'", "'-t','38'")
rep("'--watch-mesmerize-targets','28000'", "'--watch-mesmerize-targets','36000'")
rep(' while($w.ElapsedMilliseconds-lt32000){', ' while($w.ElapsedMilliseconds-lt36000){')
at=' $w.Restart();$cast='
rep(at,''' $worldMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-mesmerize-world','36000') -WindowStyle Hidden -RedirectStandardOutput "$out/native-world.ndjson" -RedirectStandardError "$out/native-world.stderr" -PassThru
'''+at)
rep("sourceWorld=(S client '.mesmerize-world-stream.json');", "sourceWorld=(S client '.mesmerize-world-stream.json');fixture=(S client '.mesmerize-fixture.json');")
at=" C client 'coop_mesmerize_fixture off';$w.Restart()"
rep(at,r''' $worldMonitor.WaitForExit(4000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){throw 'Native lifetime watcher failed'}
 $nativeRows=@(Get-Content "$out/native-world.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $nativeActive=@($nativeRows|Where-Object {$_.valid-and$_.present-and$_.state-eq3-and$_.lifetimeObserved})
 if($nativeActive.Count-lt20){throw 'Too few native active countdown observations'}
 $last=$nativeActive[-1];$maximum=($nativeActive.remainingSeconds|Measure-Object -Maximum).Maximum
 if($maximum-lt3-or$last.remainingSeconds-gt.2-or$last.remainingSeconds-lt-.1){throw 'Native power retirement is not supported by an observed countdown reaching zero'}
 if(@($nativeRows|Where-Object {$_.valid-and$_.tick-gt$last.tick-and!$_.present}).Count-lt5){throw 'Native world did not leave its equipment after countdown expiry'}
 if(@($rows|Where-Object {!$_.fixture.active-or$_.fixture.restoring}).Count){throw 'Equipment fixture interrupted the native lifetime experiment'}
 $stillOwned=Q client '--player-mesmerize';$stillOwned|ConvertTo-Json -Depth 7|Set-Content "$out/equipment-after-natural-expiry.json"
 if(!$stillOwned.equipped-or!$stillOwned.started-or$stillOwned.quantity-ne1){throw 'Equipment retired before natural lifetime was established'}
 [ordered]@{activeSamples=$nativeActive.Count;maximumSeconds=$maximum;lastActive=$last;firstActive=$nativeActive[0];fixture=(S client '.mesmerize-fixture.json');equipment=$stillOwned}|ConvertTo-Json -Depth 9|Set-Content "$out/natural-expiry-summary.json"
 $checks.Add('Source-native countdown reaches zero and its committed object retires while the original power stays selected, quantity one and fixture active; no forced unequip or timer writes')
'''+at)
at=' foreach($capture in $recorders){'
rep(at,''' if($worldMonitor){$worldMonitor.WaitForExit(45000)|Out-Null;if(!$worldMonitor.HasExited-or$worldMonitor.ExitCode){$failure=($failure+'; native lifetime watcher failed').TrimStart('; ')}}
'''+at)
rep('Native 20-second equipment fixture expiry exercises removal; natural lifetime and interrupted casts remain unverified.', 'Native countdown expiry is observed while a bounded 40-second equipment fixture remains active and the power stays selected. Source alone owns gameplay; the remote model and native particles retire from source lifecycle. Aim cancellation and menu recovery have separate recordings. Other stances, directions and authoritative NPC outcomes remain unverified.')
(P/'Test-MesmerizeLifetime-v124.ps1').write_text(s,encoding='utf-8')
