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
| Steam login with the second account, install Dishonored 2 | user, in the VM window | — |

All state lives in `work/vm-lab/`, which is never committed:
- the ISO;
- the VHDX;
- `guest-credential.xml`.

The guest account `d2lab` has a random password that is stored only DPAPI-encrypted for this Windows user. The answer ISO that carried it is deleted after setup.
