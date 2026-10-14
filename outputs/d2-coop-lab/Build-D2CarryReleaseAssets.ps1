[CmdletBinding()]
param([switch]$Install)
$ErrorActionPreference='Stop'
if($PSVersionTable.PSEdition -ne 'Core'){throw 'Requires PowerShell 7.'}
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$game=Join-Path $root 'work/asset-game'
$steam='C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
$package=Join-Path $PSScriptRoot 'assets/carry-release-v1'
$stage=Join-Path $root 'work/carry-release-install-stage-v1'
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
if($Install -and (Get-Process -Name Dishonored2 -ErrorAction SilentlyContinue | Where-Object Path -eq "$game\Dishonored2.exe")){throw 'Close isolated asset instances before installation.'}
$tool=Join-Path $root 'work/tools/void-tools-1.2.0/Void Explorer'
if((Get-FileHash "$tool/Void.Utils.dll").Hash -ne '59CCE9A1316157FFE795F0E8F179C6EE2BE3DEA43036A106B38834913AFD4A1F'){throw 'Wrong Void library.'}
Add-Type -Path "$tool/ComponentAce.Compression.Libs.zlib.dll"
Add-Type -Path "$tool/Void.Utils.dll"
$sourceIndices=@(Get-ChildItem "$steam/base" -File -Filter *.index | Get-FileHash)
$cloneIndices=@(Get-ChildItem "$game/base" -File -Filter *.index | Get-FileHash)
$fs=[Void.Utils.Games.GameFileSystem]::new()
$fs.Initialize([IO.DirectoryInfo]::new($game))

$resources=@(@{Name='midnight/d2coop/carry_victim_drop_low_stand.midnightscene';File='assets/carry_victim_drop_low_stand.midnightscene';Kind='midnightscene';Class='midnightScene'},@{Name='midnight/d2coop/carry_victim_drop_low_sneak.midnightscene';File='assets/carry_victim_drop_low_sneak.midnightscene';Kind='midnightscene';Class='midnightScene'})
[void][IO.Directory]::CreateDirectory($package)
[Void.Utils.Mods.Packs.ModInfo]::new('D2Coop remote carried victim release animation experiment','0.1.0','D2Coop lab').ToXml([IO.DirectoryInfo]::new($package))
$writer=[Void.Utils.Mods.Packs.ResourceCollectionWriter]::new([IO.DirectoryInfo]::new($package),'game1')
try {
 foreach($r in $resources){
  $r.Data=[IO.File]::ReadAllBytes((Join-Path $PSScriptRoot $r.File))
  if(!$r.Kind){$r.Kind="entitydef";$r.Class="entityDef"}
  $r.Key="generated/decls/$($r.Kind)/$($r.Name).decl"
  $r.Sha256=[Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($r.Data))
  $writer.WriteEntry([Void.Utils.Core.ResourceKey]::new($r.Key),$r.Name,$r.Class,$r.Data,$false,$true)
 }
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
 $backup=Join-Path $root ('work/carry-release-backup-'+(Get-Date -Format 'yyyyMMdd-HHmmss'))
 Assert-LocalPath $backup
 if(Test-Path -LiteralPath $backup){throw 'Backup exists'}
 New-Item -ItemType Directory -Path $backup|Out-Null
 $preserved=@(foreach($f in $planned){
  if(Test-Path -LiteralPath $f.Target){
   $item=Get-Item -LiteralPath $f.Target
   if($item.Attributes-band[IO.FileAttributes]::ReparsePoint){throw 'Reparse target'}
   if($item.Length-gt1GB){throw 'Unexpected large asset rewrite'}
   $copy=Join-Path $backup ([IO.Path]::GetFileName($f.Target))
   if(Test-Path -LiteralPath $copy){throw 'Ambiguous backup name'}
   Copy-Item -LiteralPath $f.Target -Destination $copy
   $h=(Get-FileHash -LiteralPath $f.Target).Hash
   if((Get-FileHash -LiteralPath $copy).Hash-ne$h){throw 'Backup mismatch'}
   [pscustomobject]@{Target=$f.Target;Backup=$copy;Hash=$h}
  }else{[pscustomobject]@{Target=$f.Target;Backup=$null;Hash=$null}}
 })
 $preserved|ConvertTo-Json -Depth 5|Set-Content (Join-Path $backup 'manifest.json')
 $changed=$installInfo.ReplaceGameFiles()
 $fs.ReloadGame()

 foreach($r in $resources){
  $entries=@($fs.Indices | ForEach-Object { $idx=$_; $_.AllEntries | Where-Object Name -eq $r.Name | ForEach-Object { [pscustomobject]@{Entry=$_;Index=$idx} } })
  if($entries.Count -ne 1){throw "Expected one installed resource: $($r.Name)"}
  $actual=$entries[0].Entry.GetData($entries[0].Index.ResourceCatalog)
  if([Convert]::ToHexString([Security.Cryptography.SHA256]::HashData($actual)) -ne $r.Sha256){throw 'Installed bytes differ.'}
 }

}
foreach($i in $sourceIndices){if((Get-FileHash -LiteralPath $i.Path).Hash -ne $i.Hash){throw "Steam index changed: $($i.Path)"}}
$result=[ordered]@{Time=(Get-Date).ToString('o');Installed=[bool]$Install;ChangedFiles=$changed;Resources=@($resources | ForEach-Object { [pscustomobject]@{Name=$_.Name;Key=$_.Key;Sha256=$_.Sha256} });OriginalSteamIndicesUnchanged=$true;BeforeCloneIndices=$cloneIndices;AfterCloneIndices=@(Get-ChildItem "$game/base" -File -Filter *.index | Get-FileHash);Files=$planned}
$result | ConvertTo-Json -Depth 7 | Set-Content (Join-Path $PSScriptRoot $(if($Install){'research/carry-release-asset-install.json'}else{'research/carry-release-asset-plan.json'})) -Encoding utf8
[pscustomobject]$result | Select-Object Time,Installed,ChangedFiles,Resources,OriginalSteamIndicesUnchanged,Files | ConvertTo-Json -Depth 5
