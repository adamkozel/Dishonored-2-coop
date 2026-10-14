[CmdletBinding()]
param([switch]$Install)
$ErrorActionPreference='Stop'
if($PSVersionTable.PSEdition -ne 'Core'){throw 'Requires PowerShell 7.'}
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$game=Join-Path $root 'work/asset-game'
$steam='C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
$package=Join-Path $PSScriptRoot 'assets/emily-partner'
$stage=Join-Path $root 'work/asset-install-stage'
$expectedExe='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
function Assert-LocalPath([string]$Path) {
 $full=[IO.Path]::GetFullPath($Path)
 if(!$full.StartsWith($root+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)){throw "Outside project: $full"}
 for($p=[IO.DirectoryInfo]::new($full);$p -and $p.FullName.Length -ge $root.Length;$p=$p.Parent){
  if($p.Exists -and ($p.Attributes -band [IO.FileAttributes]::ReparsePoint)){throw "Reparse point: $($p.FullName)"}
 }
}
foreach($p in @($game,(Join-Path $game 'base'),$package,$stage)){Assert-LocalPath $p}
if((Get-FileHash "$game/Dishonored2.exe").Hash -ne $expectedExe){throw 'Wrong isolated executable.'}
if(Get-Process -Name Dishonored2 -ErrorAction SilentlyContinue | Where-Object Path -eq "$game\Dishonored2.exe"){throw 'Close isolated asset instances before installation.'}
$tool=Join-Path $root 'work/tools/void-tools-1.2.0/Void Explorer'
if((Get-FileHash "$tool/Void.Utils.dll").Hash -ne '59CCE9A1316157FFE795F0E8F179C6EE2BE3DEA43036A106B38834913AFD4A1F'){throw 'Wrong Void library.'}
Add-Type -Path "$tool/ComponentAce.Compression.Libs.zlib.dll"
Add-Type -Path "$tool/Void.Utils.dll"
$sourceIndices=@(Get-ChildItem "$steam/base" -File -Filter *.index | Get-FileHash)
$cloneIndices=@(Get-ChildItem "$game/base" -File -Filter *.index | Get-FileHash)
$fs=[Void.Utils.Games.GameFileSystem]::new()
$fs.Initialize([IO.DirectoryInfo]::new($game))
$name='models/characters/def/d2coop/emily_partner.def'
$key="generated/decls/entitydef/$name.decl"
$data=[IO.File]::ReadAllBytes((Join-Path $PSScriptRoot 'assets/emily_partner.def'))
[void][IO.Directory]::CreateDirectory($package)
[Void.Utils.Mods.Packs.ModInfo]::new('D2Coop owned Emily inventory','0.1.0','D2Coop lab').ToXml([IO.DirectoryInfo]::new($package))
$writer=[Void.Utils.Mods.Packs.ResourceCollectionWriter]::new([IO.DirectoryInfo]::new($package),'game1')
try {
 $writer.WriteEntry([Void.Utils.Core.ResourceKey]::new($key),$name,'entityDef',$data,$false,$true)
 $writer.Write()
}finally{$writer.Dispose()}
$pack=[Void.Utils.Mods.Packs.ModPackFileSystem]::Load([IO.DirectoryInfo]::new($package),$fs)
[void][IO.Directory]::CreateDirectory($stage)
$installInfo=$fs.InstallMod($pack,[IO.DirectoryInfo]::new($stage),'voidmod')
$files=@($installInfo.TempFiles)
$planned=@($files | ForEach-Object {
 $dst=[IO.Path]::GetFullPath((Join-Path $_.GameDir.FullName $_.GameFile))
 Assert-LocalPath $dst
 if(!$dst.StartsWith($game+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)){throw "Unexpected installation target: $dst"}
 [pscustomobject]@{Target=$dst;Staged=(Join-Path $_.InstallDir.FullName $_.InstallFile);IsNew=$_.IsNew;Backup=$_.MustBackup}
})
$changed=0
if($Install){
 $changed=$installInfo.ReplaceGameFiles()
 $fs.ReloadGame()
 $entries=@($fs.Indices | ForEach-Object { $idx=$_; $_.AllEntries | Where-Object Name -eq $name | ForEach-Object { [pscustomobject]@{Entry=$_;Index=$idx} } })
 if($entries.Count -ne 1){throw "Expected one installed resource, got $($entries.Count)"}
 $actual=$entries[0].Entry.GetData($entries[0].Index.ResourceCatalog)
 if([Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($actual)) -ne [Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($data))){throw 'Installed bytes differ.'}
}
foreach($i in $sourceIndices){if((Get-FileHash -LiteralPath $i.Path).Hash -ne $i.Hash){throw "Steam index changed: $($i.Path)"}}
$result=[ordered]@{Time=(Get-Date).ToString('o');Installed=[bool]$Install;ChangedFiles=$changed;Asset=$name;Key=$key;AssetSha256=[Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($data));OriginalSteamIndicesUnchanged=$true;BeforeCloneIndices=$cloneIndices;AfterCloneIndices=@(Get-ChildItem "$game/base" -File -Filter *.index | Get-FileHash);Files=$planned}
$result | ConvertTo-Json -Depth 7 | Set-Content (Join-Path $PSScriptRoot 'research/emily-asset-install.json') -Encoding utf8
[pscustomobject]$result | Select-Object Time,Installed,ChangedFiles,Asset,AssetSha256,OriginalSteamIndicesUnchanged,Files | ConvertTo-Json -Depth 5
