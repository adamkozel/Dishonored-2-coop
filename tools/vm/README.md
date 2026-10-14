# Second computer: the D2 co-op lab VM

A Hyper-V VM (`D2CoopVM`) running Windows 11 Pro gives Claude a second computer with a second Steam account. It is used for two-account co-op testing (board #12 and #20). Control matches the two-instance lab:
- Claude runs commands, the lab scripts, `d2_runtime_probe` and ffmpeg capture *inside* the guest through PowerShell Direct (`Invoke-D2LabVM`).
- The game renders on a partition of the host's RTX 5080 (GPU-P).

| Step | Who | Script |
|---|---|---|
| Enable Hyper-V and join *Hyper-V Administrators*, then restart (one time) | user, elevated | `Enable-HyperV-AsAdmin.ps1` |
| Create the VM and install Windows unattended (about 40 min) | Claude | `New-D2LabVM.ps1 -IsoPath work/vm-lab/<windows 11 iso>` |
| Guest settings: no sleep, lock or update reboots | Claude | `Initialize-D2LabGuest.ps1` |
| GPU partition plus host NVIDIA driver (re-run after host driver updates) | Claude | `Enable-D2LabVMGpu.ps1` |
| Everything after the install in one run: wait for the desktop, guest settings, GPU partition, Steam | Claude | `Complete-D2LabVM.ps1` |
| Steam client, a silent install from Valve's CDN with the Authenticode signature checked | Claude | `Install-D2LabSteam.ps1` |
| Open the VM window | Claude | `Show-D2LabVM.ps1` |
| Steam login with the second account, install Dishonored 2 | user, in the VM window | — |

Helpers in `D2LabVM.Common.ps1`:

| Helper | What it does |
|---|---|
| `Invoke-D2LabVM` | Runs a command inside the guest. |
| `Save-D2LabVMScreen` | Takes a host-side PNG of the console, with no viewer needed. |
| `Send-D2LabVMKey` | Types a key into the console. |
| `Wait-D2LabVMReady` | Waits until PowerShell Direct answers. |

All state lives in `work/vm-lab/`, which is never committed:
- the ISO;
- the VHDX;
- `guest-credential.xml`.

The guest account `d2lab` has a random password that is stored only DPAPI-encrypted for this Windows user. The answer ISO that carried it is deleted after setup.

Programs that must appear on the guest's signed-in desktop, such as Steam and the game, start through `Start-D2LabVMInteractive`. That helper runs a one-shot interactive scheduled task. PowerShell Direct alone runs in a hidden logon session.

Hyper-V's enhanced session mode is switched off on this host (`Set-VMHost -EnableEnhancedSessionMode $false`), for two reasons:
- VMConnect then shows the console the game renders in, and doesn't ask for the guest password over RDP.
- The autologon console session is never taken over.
