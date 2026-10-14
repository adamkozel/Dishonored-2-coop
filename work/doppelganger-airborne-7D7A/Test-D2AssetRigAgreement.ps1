[CmdletBinding()]
param()
$ErrorActionPreference='Stop'
if ($PSVersionTable.PSEdition -ne 'Core') { throw 'Run with PowerShell 7.' }
$project=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..'))
$dll=Join-Path $project 'work\tools\void-tools-1.2.0\Void Explorer\Void.Utils.dll'
if ((Get-FileHash -LiteralPath $dll).Hash -ne '59CCE9A1316157FFE795F0E8F179C6EE2BE3DEA43036A106B38834913AFD4A1F') {throw 'Unrecognized library.'}
[void][Reflection.Assembly]::LoadFrom($dll)
$dir=Join-Path $PSScriptRoot 'research\asset-exports\avatar-rigs'
$manifest=Get-Content -Raw -LiteralPath (Join-Path $dir 'manifest.json') | ConvertFrom-Json
$decoded=@{}
foreach($r in $manifest.Resources) {
  if((Get-FileHash -LiteralPath $r.ExportFile).Hash -ne $r.Sha256){throw 'Export hash mismatch.'}
  $s=[Void.Utils.Resources.Meshes.EdgeSkeleton]::Create([IO.File]::ReadAllBytes($r.ExportFile))
  $bones=@([Void.Utils.Meshes.VoidBone]::GetBones($s))
  if($bones.Count -ne $s.BoneCount){throw 'Incomplete skeleton decode.'}
  $names=@{}
  for($i=0;$i -lt $bones.Count;$i++){
    if($names.ContainsKey($bones[$i].Name)){throw 'Duplicate bone name.'}
    $names[$bones[$i].Name]=$i
  }
  $rows=@(for($i=0;$i -lt $bones.Count;$i++){
    $b=$bones[$i]
    [pscustomobject]@{
      Index=$i;Name=$b.Name;Parent=if($b.Parent){$names[$b.Parent.Name]}else{-1}
      Rotation=@($b.Rotation.X,$b.Rotation.Y,$b.Rotation.Z,$b.Rotation.W)
      Translation=@($b.Translation.X,$b.Translation.Y,$b.Translation.Z)
    }
  })
  $decoded[$r.Name]=[pscustomobject]@{Name=$r.Name;Sha256=$r.Sha256;BoneCount=$s.BoneCount;Bones=$rows}
}
$decoded.Values | Sort-Object Name | ConvertTo-Json -Depth 7 | Set-Content -LiteralPath (Join-Path $dir 'decoded-bones.json') -Encoding utf8
$observationPath=Join-Path $PSScriptRoot 'research\arm-stream-observations.json'
$observations=Get-Content -Raw -LiteralPath $observationPath | ConvertFrom-Json
if(!$observations.Passed){throw 'Runtime observation was not validated.'}
$results=@(foreach($case in $observations.Cases){
 foreach($which in @('SourceRig','TargetRig')){
  $native=$case.$which
  $asset=$decoded[$native.skeleton]
  if(!$asset -or $asset.BoneCount -ne $native.joints.Count){throw 'Rig absent or count mismatch.'}
  $maxError=0.0;$mismatches=@()
  foreach($j in $native.joints){
    $b=$asset.Bones[$j.index]
    if($b.Name -ne $j.nativeName -or $b.Parent -ne $j.parent){$mismatches+= $j.index}
    $values=@($b.Rotation)+@($b.Translation)
    for($c=0;$c -lt 7;$c++){
      $delta=[Math]::Abs([double]$values[$c]-[double]$j.bindLocalQTS[$c])
      if([double]::IsNaN($delta)-or[double]::IsInfinity($delta)){throw 'Invalid transform.'}
      $maxError=[Math]::Max($maxError,$delta)
    }
  }
  [pscustomobject]@{Role=$case.Role;Scope=$which;Skeleton=$native.skeleton;Joints=$native.joints.Count;NameOrParentMismatches=$mismatches;MaxQtsError=$maxError;Passed=($mismatches.Count -eq 0 -and $maxError -lt 0.000002)}
 }
})
$result=[ordered]@{
 ObservedAt=(Get-Date).ToString('o')
 Passed=(@($results | Where-Object {!$_.Passed}).Count -eq 0)
 Source='Void Explorer 1.2.0.0 EdgeSkeleton.Create / VoidBone.GetBones'
 RuntimeRecord='arm-stream-observations.json'
 RuntimeRecordSha256=(Get-FileHash -LiteralPath $observationPath).Hash
 Checks=$results
 Scope='Independent asset decoder agrees with previously recorded native runtime joint names, parents, quaternion and translation bind data. Does not establish visual grip or correct retargeting.'
 VisualGripPassed=$false
}
$result | ConvertTo-Json -Depth 7 | Set-Content -LiteralPath (Join-Path $PSScriptRoot 'research\asset-rig-agreement-verification.json') -Encoding utf8
if(!$result.Passed){throw 'Native/asset skeleton agreement failed; inspect report.'}
[pscustomobject]@{Passed=$true;RigComparisons=$results.Count;Joints=($results|Measure-Object Joints -Sum).Sum;MaxQtsError=($results|Measure-Object MaxQtsError -Maximum).Maximum;VisualGripPassed=$false}

