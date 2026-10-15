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

## GPU load: why the VM game runs at the lab's low settings

The VM's first Dishonored 2 launch (2026-10-14) bugchecked the host (`0x50`).
- **Setup at the time:** the game ran on its defaults (fullscreen 1920×1080, auto quality) inside an 8–10% GPU slice, next to both host lab instances.
- **Evidence:**
  - a GPU-engine timeout (LiveKernelEvent `0x141`) was reported first;
  - `tools/diag/read_kernel_minidump.py` places the fault in the host NVIDIA driver: `nvlddmkm.sys+0x196481E`, with a stack through `dxgmms2.sys` and `dxgkrnl.sys`.
- **Fix:**
  - `Set-D2LabVMGameSettings.ps1` applies the same 944×540 windowed, lowest-preset settings the host instances use, plus vsync;
  - `Enable-D2LabVMGpu.ps1` now gives the VM 40%.
- **Result:**
  - the VM game ran alone for 3 minutes with no GPU or driver events;
  - all three games ran together (host and client lab plus the VM): about 53% GPU load, 4.4 of 16 GB VRAM, 54 °C, no events.

Never launch the VM game at its defaults. Re-run `Set-D2LabVMGameSettings.ps1` if its config is ever reset.

## Running a lab role inside the VM

`Sync-D2LabVMWorkspace.ps1 [-Role client]` mirrors the lab into `C:\D2Lab\ws`, so the lab's own scripts run there unchanged. It copies:
- the pinned pwsh 7 runtime (`C:\D2Lab\pwsh`);
- `outputs/d2-coop-lab/{*.ps1, tooling, bin}`, with the VC++ runtime DLLs app-local (#22);
- a physical game copy made from the guest's own Steam install, plus the lab's voidmod resources and `master.index`;
- the role's instance seed (saves, config, movement bank).

`D2LabVM.Lab.ps1` (dot-source it after `D2LabVM.Common.ps1`) drives the guest instance through PowerShell Direct:

| Function | What it does |
|---|---|
| `Start-D2LabVMRole -Role client` | Runs `Start-D2Lab.ps1 -AssetExperiment` on the guest's visible desktop |
| `Stop-D2LabVMRole` | Stops the guest instance |
| `Send-D2LabVMCommand`, `Send-D2LabVMInput` | The guest's own `Send-D2Command.ps1` / `Send-D2Input.ps1` |
| `Get-D2LabVMStatus [-Sidecar online]` | Reads the guest's `coop-status.json` or a sidecar |
| `Invoke-D2LabVMProbe -Mode --player` | Runs `d2_runtime_probe.exe` against the guest instance |
| `Invoke-D2LabVMLabScript` | Runs any lab script in the guest |

**Controller (#24):** the guest has no controller device, and without one Dishonored 2 ignores the lab's simulated controller. `Enable-D2LabVMPad.ps1` fixes that:
1. It builds `guest/vigem/{vigem_setup,vigem_pad}.cpp` with the local MSVC.
2. It copies the signed ViGEmBus package from the host's DriverStore, with no download.
3. It registers the bus device.
4. It registers a sign-in task that plugs one virtual Xbox 360 pad.

The proxy's `XInputGetState` hook still supplies the state. Start the game after the pad exists. Take `vm-virtual-pad-v1223`: from the menu to gameplay with the controller only, and a 5.40 m stick walk.

The packet expiry clocks match because Hyper-V time synchronization is on.

Lab mode's peer is hard-coded to `127.0.0.1`, so a host instance and the VM instance meet through the Steam lobby instead:
1. On the host: `coop_session host`.
2. Read the lobby id from `host/coop-status.json.online.json`.
3. On the VM: `coop_session join <id>`.

The two accounts are Steam friends, as the friends-only lobby requires. Scripts that drive both roles at once, such as `PrepareShadowSword`, `Restore-D2Lab` and most `Test-*`, assume one machine. Split them into per-machine calls with these functions.
