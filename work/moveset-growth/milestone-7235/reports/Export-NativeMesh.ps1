[CmdletBinding()]
param([Parameter(Mandatory)][string]$Mesh,[string]$Skeleton,[Parameter(Mandatory)][string]$Output)
$ErrorActionPreference='Stop'
$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$out=[IO.Path]::GetFullPath($Output)
if(!$out.StartsWith((Join-Path $root 'work/moveset-growth'),[StringComparison]::OrdinalIgnoreCase)){throw 'Output outside proof workspace'}
$tool=Join-Path $root 'work/tools/void-tools-1.2.0/Void Explorer'
$dll=Join-Path $tool 'Void.Utils.dll'
if((Get-FileHash $dll).Hash -ne '59CCE9A1316157FFE795F0E8F179C6EE2BE3DEA43036A106B38834913AFD4A1F'){throw 'Unexpected library'}
[void][Reflection.Assembly]::LoadFrom((Join-Path $tool 'ComponentAce.Compression.Libs.zlib.dll'))
[void][Reflection.Assembly]::LoadFrom($dll)
$bones=$null
if($Skeleton){$sk=[Void.Utils.Resources.Meshes.EdgeSkeleton]::Create([IO.File]::ReadAllBytes($Skeleton));$bones=[Void.Utils.Meshes.VoidBone]::GetBones($sk)}
$model=[Void.Utils.Resources.Meshes.BMD6Model]::Create([IO.File]::ReadAllBytes($Mesh))
$data=$model.GetMeshData($bones)
$geo=$data.GeometryData
$verts=@($geo.Vertices)
$weights=@(for($i=0;$i -lt $verts.Count;$i++){
  ,@(if($geo.VertexBoneWeights){foreach($w in $geo.VertexBoneWeights[$i]){,@($w.BoneName,$w.BoneWeightDouble)}})
})
$report=[ordered]@{
  source=(Get-FileHash $Mesh);skeleton=$(if($Skeleton){Get-FileHash $Skeleton}else{$null});library=(Get-FileHash $dll)
  units='metres';axes='native D2 +X forward +Z up';meshSkeleton=$model.Skeleton
  vertices=@(foreach($v in $verts){,@($v.X,$v.Y,$v.Z)})
  indices=@($geo.TriangleIndices);normals=@(foreach($v in $geo.Normals){,@($v.X,$v.Y,$v.Z)})
  uv=@(foreach($v in $geo.GetUVs(0)){,@($v.U,$v.V)})
  weights=$weights;sections=@($geo.Sections);materials=@($data.IndexedMaterials)
  bones=@(foreach($b in $bones){[ordered]@{name=$b.Name;parent=$b.Parent.Name;translation=@($b.Translation.X,$b.Translation.Y,$b.Translation.Z);rotation=@($b.Rotation.X,$b.Rotation.Y,$b.Rotation.Z,$b.Rotation.W)}})
}
$report|ConvertTo-Json -Depth 10 -Compress|Set-Content -LiteralPath $out -Encoding utf8
[pscustomobject]@{Output=$out;Vertices=$verts.Count;Triangles=@($geo.TriangleIndices).Count/3;Bones=@($bones).Count;Bytes=(Get-Item $out).Length}

