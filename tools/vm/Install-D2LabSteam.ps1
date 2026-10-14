# Installs the Steam client in the lab VM (silent), from Valve's official CDN, inside the guest.
# Download approved by the user on 2026-10-14 (SteamSetup.exe, ~2.4 MB). The Steam login and the
# Dishonored 2 install are the user's step. pwsh 7, member of "Hyper-V Administrators".
$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'D2LabVM.Common.ps1')
Invoke-D2LabVM {
    $ErrorActionPreference = 'Stop'
    $steam = 'C:\Program Files (x86)\Steam\steam.exe'
    if (!(Test-Path $steam)) {
        $setup = 'C:\D2Lab\SteamSetup.exe'
        New-Item -ItemType Directory -Force 'C:\D2Lab' | Out-Null
        Invoke-WebRequest -UseBasicParsing 'https://cdn.cloudflare.steamstatic.com/client/installer/SteamSetup.exe' -OutFile $setup
        $sig = Get-AuthenticodeSignature $setup
        if ($sig.Status -ne 'Valid' -or $sig.SignerCertificate.Subject -notmatch 'Valve') { throw "SteamSetup.exe signature not valid Valve: $($sig.Status) $($sig.SignerCertificate.Subject)" }
        Start-Process $setup -ArgumentList '/S' -Wait
        Remove-Item $setup
    }
    if (!(Test-Path $steam)) { throw 'Steam did not install' }
    [pscustomobject]@{ steam = $steam; version = (Get-Item $steam).VersionInfo.FileVersion; signer = (Get-AuthenticodeSignature $steam).SignerCertificate.Subject }
}
