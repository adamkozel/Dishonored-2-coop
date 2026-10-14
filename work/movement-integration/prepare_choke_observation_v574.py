from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-GuardChokeControl-v390.ps1').read_text()
s=s.replace("Take='ordinary-guard-choke-control-v390'","Take='native-guard-choke-v574'")
s=s.replace('PartnerTargetingObserve','NativeChokeObserve574').replace("$capture=$null\n","$capture=$null;$monitor=$null\n",1)
s=s.replace("if($dll-ne'440B0F9FD8BE7D24D22B62DE0BD7FB43C2E9C85FE6AB3551DA9A988AA273C6CD'){throw 'Expected archived387 candidate'}", "if($dll-ne'6BF1C37871BCA7494E968A8B68350200CD2D2DCF06CF50885236AEE079DF3E2F'){throw 'Expected preserved571 candidate'}")
s=s.replace("'-t','8'","'-t','14'").replace('-lt7000','-lt12000').replace('I $source RightShoulder 2000','I $source RightShoulder 6500')
needle="   $filterBefore=Get-Content"
at=s.index(needle)
s=s[:at]+'''   $monitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids[$source],'--watch-midnight','13000') -WindowStyle Hidden -RedirectStandardOutput "$out/scenes.ndjson" -RedirectStandardError "$out/scenes.stderr" -PassThru
'''+s[at:]
s=s.replace('   $capture.WaitForExit(6000)', '   $monitor.WaitForExit(6000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){throw "Native scene monitor incomplete"};$monitor=$null\n   $capture.WaitForExit(6000)')
s=s.replace("   $paired=@($samples.midnight.instances|", "   $allScenes=@(Get-Content \"$out/scenes.ndjson\"|ForEach-Object {$_|ConvertFrom-Json}|ForEach-Object {$_.instances});$paired=@($allScenes|")
s=s.replace("   $checks.Add('Ordinary guard native eligibility accepted", "   $paired|ConvertTo-Json -Depth 8|Set-Content \"$out/choke-scenes.json\"\n   $checks.Add('Ordinary guard native eligibility accepted")
s=s.replace("Native RightShoulder held2000ms", "Native RightShoulder held6500ms")
s=s.replace("  if($capture){", "  if($monitor){$monitor.WaitForExit(15000)|Out-Null;if(!$monitor.HasExited-or$monitor.ExitCode){$failure=($failure+'; scene monitor incomplete').TrimStart('; ')}}\n  if($capture){")
s=s.replace("scope='Ordinary native guard choke positive control for partner filter387. Native source placed behind read-only identified authored guard; target untouched. Require native eligibility accepted, state41, choke_back/loop and checkpoint restoration. Paired target bindings and native recording reviewed separately. Normal native movement collision intentionally retained.'", "scope='Native rear choke phase and target-binding observation using existing ordinary guard fixture; standard controller input only. Captures source scenes, native root/stance/state and actual recording. No remote paired body or shared NPC outcome is claimed. Restore checkpoint inventory/health and14 protected files.'")
s=s.replace('PASS_PARTNER_TARGETING_OBSERVATION','PASS_NATIVE_CHOKE_OBSERVATION')
(P/'Observe-NativeChoke-v574.ps1').write_text(s)
print('Prepared next existing-pipeline native choke observation; not executed.')
