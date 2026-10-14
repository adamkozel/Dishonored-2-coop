# Read-only asset inspection through the pinned Void Explorer library.
# Exports go only to this project's research/asset-exports directory.
[CmdletBinding()]
param(
  [Parameter(Mandatory)][string]$NamePattern,
  [string]$CatalogPattern = '^game1',
  [string]$ClassPattern = '.*',
  [switch]$Export,
  [string]$Batch = 'inventory',
  [int]$Limit = 100
)
$ErrorActionPreference = 'Stop'
if ($PSVersionTable.PSEdition -ne 'Core') { throw 'Run with PowerShell 7.' }
if ($Batch -notmatch '^[a-z0-9][a-z0-9-]{0,63}$') { throw 'Invalid batch name.' }
if ($Limit -lt 1 -or $Limit -gt 1000) { throw 'Limit must be 1..1000.' }
$lab = $PSScriptRoot
$project = [IO.Path]::GetFullPath((Join-Path $lab '..\..'))
$tool = Join-Path $project 'work\tools\void-tools-1.2.0\Void Explorer'
$game = 'C:\Program Files (x86)\Steam\steamapps\common\Dishonored2'
$dll = Join-Path $tool 'Void.Utils.dll'
if ((Get-FileHash -LiteralPath $dll).Hash -ne '59CCE9A1316157FFE795F0E8F179C6EE2BE3DEA43036A106B38834913AFD4A1F') { throw 'Unrecognized Void.Utils library.' }
if ((Get-FileHash -LiteralPath (Join-Path $game 'Dishonored2.exe')).Hash -ne 'B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180') { throw 'Unrecognized game executable.' }
[void][Reflection.Assembly]::LoadFrom((Join-Path $tool 'ComponentAce.Compression.Libs.zlib.dll'))
[void][Reflection.Assembly]::LoadFrom($dll)
# Initialize and GetData are the only game-file operations. No installer/writer API is used.
$indices = @(Get-ChildItem -LiteralPath (Join-Path $game 'base') -File -Filter *.index | Get-FileHash)
$g = [Void.Utils.Games.GameFileSystem]::new()
$g.Initialize([IO.DirectoryInfo]::new($game))
$matches = @(foreach ($idx in $g.Indices) {
  if ($idx.ResourceCatalog.ToString() -notmatch $CatalogPattern) { continue }
  foreach ($e in $idx.AllEntries) {
    if ($e.Name -match $NamePattern -and $e.Classname -match $ClassPattern) { [pscustomobject]@{ Index=$idx; Entry=$e } }
  }
})
if ($matches.Count -gt $Limit) { throw "Matched $($matches.Count) resources; narrow the query or increase Limit." }
$destination = Join-Path $lab "research\asset-exports\$Batch"
if ($Export) {
  $cursor = [IO.DirectoryInfo]::new($destination)
  while ($cursor -and $cursor.FullName.StartsWith($project,[StringComparison]::OrdinalIgnoreCase)) {
    if ($cursor.Exists -and ($cursor.Attributes -band [IO.FileAttributes]::ReparsePoint)) { throw 'Export path contains a reparse point.' }
    $cursor = $cursor.Parent
  }
  [void][IO.Directory]::CreateDirectory($destination)
}
$rows = @(foreach ($m in $matches) {
  $e = $m.Entry
  $resourceFile = $e.GetResourceFile($m.Index.ResourceCatalog)
  $row = [ordered]@{
    Catalog=$m.Index.ResourceCatalog.ToString(); Name=$e.Name; Key=$e.Key
    Class=$e.Classname; Bytes=$e.Size; DiskBytes=$e.DiskSize
    Patch=$e.PatchNumber; SourceFile=$resourceFile.FullName; DiskOffset=$e.DiskOffset
  }
  if ($Export) {
    $leaf = $e.GetFileKey()
    if ([IO.Path]::GetFileName($leaf) -ne $leaf -or $leaf.IndexOfAny([IO.Path]::GetInvalidFileNameChars()) -ge 0) { throw "Unsafe export key: $leaf" }
    $catalogDir = Join-Path $destination ([IO.Path]::GetFileNameWithoutExtension($row.Catalog))
    [void][IO.Directory]::CreateDirectory($catalogDir)
    $out = Join-Path $catalogDir $leaf
    $bytes = $e.GetData($m.Index.ResourceCatalog)
    if (Test-Path -LiteralPath $out) {
      if ((Get-Item -LiteralPath $out).Attributes -band [IO.FileAttributes]::ReparsePoint) { throw 'Export target is a reparse point.' }
    }
    [IO.File]::WriteAllBytes($out,$bytes)
    $row.ExportFile=$out
    $row.Sha256=(Get-FileHash -LiteralPath $out).Hash
  }
  [pscustomobject]$row
})
foreach ($i in $indices) {
  if ((Get-FileHash -LiteralPath $i.Path).Hash -ne $i.Hash) { throw "Source index changed: $($i.Path)" }
}
if ($Export) {
  [ordered]@{
    CreatedAt=(Get-Date).ToString('o'); Tool='Void Explorer 1.2.0.0 / Void.Utils'
    GameExecutableSha256='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
    NamePattern=$NamePattern; CatalogPattern=$CatalogPattern; ClassPattern=$ClassPattern
    SourceIndicesUnchanged=$true; SourceIndices=$indices
    Resources=$rows
  } | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath (Join-Path $destination 'manifest.json') -Encoding utf8
}
$rows

