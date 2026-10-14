from pathlib import Path
P=Path(__file__).resolve().parent;p=P/'Test-BendTimeContexts.ps1';s=p.read_text(encoding='utf-8-sig')
a=" $start=[DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()"
assert s.count(a)==1
s=s.replace(a,a+'\n $trace=Start-Process -FilePath "$PSHOME/pwsh.exe" -ArgumentList @(\'-NoProfile\',\'-File\',\'"\'+"$PSScriptRoot/Trace-BendTimePresentation.ps1"+\'"\',\'-Take\',$Take) -WindowStyle Hidden -RedirectStandardOutput "$out/trace.stdout" -RedirectStandardError "$out/trace.stderr" -PassThru')
a=' if($seen.Count-ne4-or$nativeScenes.Count-ne4'
assert s.count(a)==1
s=s.replace(a,''' if($trace){$trace.WaitForExit(3000)|Out-Null;if(!$trace.HasExited-or$trace.ExitCode){throw 'Presentation trace did not finalize'}}
 $traceRows=Get-Content "$out/presentation-trace.json" -Raw|ConvertFrom-Json
 $seen=@{};$remotePhases=@{}
 foreach($e in $traceRows){if($e.active-and$e.ready-and$e.variant-in@(12,14)-and$e.weight-gt.5){$seen[[string]$e.event]=$true;$remotePhases[[string]$e.variant]=$true}}
 if($seen.Count-ne4-or$nativeScenes.Count-ne4''')
p.write_text(s)
print('Independent high-frequency presentation observation added; native phase checks retained')
