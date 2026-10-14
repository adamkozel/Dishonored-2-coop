$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$lab=Join-Path $root 'outputs/d2-coop-lab'
$archive=Join-Path $PSScriptRoot 'candidate-air-trajectory-v170'
if(Test-Path -LiteralPath $archive){throw 'Candidate archive already exists'}
New-Item -ItemType Directory -Path $archive|Out-Null
Copy-Item -LiteralPath "$lab/src" -Destination "$archive/src" -Recurse
Copy-Item -LiteralPath "$root/work/build/Release/dinput8.dll" -Destination "$archive/dinput8.dll"
Copy-Item -LiteralPath "$lab/bin/d2_runtime_probe.exe" -Destination "$archive/d2_runtime_probe.exe"
foreach($file in 'Invoke-NativeLabOperation.ps1','air-trajectory-build-v170.log','air-trajectory-smoke-v170.log','prepare_air_trajectory_v170.py'){
 Copy-Item -LiteralPath (Join-Path $PSScriptRoot $file) -Destination (Join-Path $archive $file)
}
Copy-Item -LiteralPath "$root/work/natural-moveset/Test-MesmerizeAirNetwork-v170.ps1" -Destination $archive
[ordered]@{created=(Get-Date).ToString('o');dll=(Get-FileHash "$archive/dinput8.dll").Hash;probe=(Get-FileHash "$archive/d2_runtime_probe.exe").Hash;protocol=42;mesmerizeWorldProtocol=4;bankVersion=46;clips=285;runtimeVerified=$false;scope='Local interpolation trajectory evidence keeps native motion timestamps while permitting bounded takeoff/contact root steps. Final settlement requires confirmed Fall-to-Ground, coherent descent, <=100ms and <=25cm. Existing ground, horizontal and airborne bounds retained. Pose interpolation tests, full movement bank formats1-46/both roles, and proxy smoke pass. Native replay pending.'}|ConvertTo-Json -Depth 5|Set-Content "$archive/identity.json"
Copy-Item -LiteralPath "$archive/dinput8.dll" -Destination "$lab/bin/dinput8.dll"
Get-Content "$archive/identity.json"
