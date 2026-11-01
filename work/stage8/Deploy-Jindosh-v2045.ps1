$ErrorActionPreference='Stop'
# 2045 (#239): deploy candidate jindosh-v2045 to the lab pair and the VM. The lab launcher stages
# outputs/d2-coop-lab/bin/dinput8.dll; Deploy-SourceClock relaunches both lab roles; the VM workspace is resynced.
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'));$archive=Join-Path $PSScriptRoot 'candidate-jindosh-v2045'
$pin=Get-Content "$archive/identity.json" -Raw|ConvertFrom-Json;if((Get-FileHash "$archive/dinput8.dll").Hash-ne$pin.dll){throw 'Archive DLL differs from its identity'}
foreach($f in 'dinput8.dll','d2_runtime_probe.exe'){Copy-Item -LiteralPath "$archive/$f" -Destination "$root/outputs/d2-coop-lab/bin/$f" -Force}
& "$root/work/movement-integration/Deploy-SourceClock.ps1" *> "$archive/deploy.log"
if((Get-FileHash "$root/work/asset-game/dinput8.dll").Hash-ne$pin.dll){throw 'Lab game does not carry the candidate'}
& "$root/tools/vm/Sync-D2LabVMWorkspace.ps1" *> "$archive/vm-sync.log"
"Deployed candidate jindosh-v2045 to the lab and the VM"
