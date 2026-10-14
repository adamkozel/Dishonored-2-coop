# Builds a small ISO (ISO9660 + Joliet) from a folder with the built-in IMAPI2 COM API.
# Runs under Windows PowerShell 5.1 (powershell.exe): IMAPI's IStream marshals cleanly there.
# Usage: powershell.exe -NoProfile -File New-AnswerIso.ps1 -Source <folder> -Destination <file.iso> [-Label UNATTEND]
param([Parameter(Mandatory)][string]$Source, [Parameter(Mandatory)][string]$Destination, [string]$Label = 'UNATTEND')
$ErrorActionPreference = 'Stop'
if (!('D2IsoWriter' -as [type])) {
    Add-Type -TypeDefinition @'
using System; using System.IO; using System.Runtime.InteropServices.ComTypes;
public static class D2IsoWriter {
    public static void Save(object comStream, string path) {
        IStream s = (IStream)comStream; byte[] buf = new byte[65536]; IntPtr readPtr = System.Runtime.InteropServices.Marshal.AllocHGlobal(4);
        try { using (FileStream f = File.Create(path)) { while (true) { s.Read(buf, buf.Length, readPtr); int n = System.Runtime.InteropServices.Marshal.ReadInt32(readPtr); if (n <= 0) break; f.Write(buf, 0, n); } } }
        finally { System.Runtime.InteropServices.Marshal.FreeHGlobal(readPtr); }
    }
}
'@
}
$fsi = New-Object -ComObject IMAPI2FS.MsftFileSystemImage
$fsi.FileSystemsToCreate = 3          # ISO9660 | Joliet
$fsi.VolumeName = $Label
$fsi.Root.AddTree((Resolve-Path $Source).Path, $false)
$image = $fsi.CreateResultImage()
[D2IsoWriter]::Save($image.ImageStream, [IO.Path]::GetFullPath($Destination))
Get-Item $Destination | Select-Object FullName, Length
