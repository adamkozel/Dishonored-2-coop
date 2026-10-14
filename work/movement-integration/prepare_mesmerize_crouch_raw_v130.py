from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeMesmerizeCrouch-v126.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a));s=s.replace(a,b)
rep('$monitor=$null;', '$monitor=$null;$rawMonitor=$null;')
rep(' $w.Restart();$cast=$false', ''' $rawMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-midnight','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/midnight.ndjson" -RedirectStandardError "$out/midnight.stderr" -PassThru
 $w.Restart();$cast=$false''')
rep(" $rawScenes=@($rows|ForEach-Object {$_.midnight.instances}|Where-Object {$_.asset-like'*power_mesmerize_cast_*'})", r''' $rawMonitor.WaitForExit(4000)|Out-Null;if(!$rawMonitor.HasExited-or$rawMonitor.ExitCode){throw 'Raw Midnight watcher failed'}
 $rawSamples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $rawScenes=@($rawSamples|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-like'*power_mesmerize_cast_*'})
 foreach($phase in 'in','loop','out'){
  $scenes=@($rawScenes|Where-Object asset -eq "midnight/gameplay/player/base/powers/power_mesmerize_cast_$phase.midnightscene")
  if(!$scenes.Count-or@($scenes|Where-Object {@($_.bindingsResearch).Count-ne1-or!$_.bindingsResearch[0].reciprocal}).Count){throw "Raw native crouched $phase scene or reciprocal single-arm binding missing"}
 }
 if(@($rows.player|Where-Object {$_.stance-ne1-or!$_.defaultMode-or@($_.active|Where-Object {$_.id-notin6,63}).Count}).Count){throw 'Unexpected native state during crouched cast'}
 $checks.Add('All three native crouched cast phases retain one reciprocal arm binding and observed Sneaking/UsePower state context')''')
rep(' if($capture){$capture.WaitForExit', ''' if($rawMonitor){$rawMonitor.WaitForExit(18000)|Out-Null;if(!$rawMonitor.HasExited-or$rawMonitor.ExitCode){$failure=($failure+'; raw scene watcher failed').TrimStart('; ')}}
 if($capture){$capture.WaitForExit''')
(P/'Observe-NativeMesmerizeCrouch-v130.ps1').write_text(s,encoding='utf-8')
