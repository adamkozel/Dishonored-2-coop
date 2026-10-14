from pathlib import Path
P=Path(__file__).resolve().parents[2];lab=P/'outputs/d2-coop-lab'
s=(lab/'Build-D2DoppelgangerAssets.ps1').read_text(encoding='utf-8-sig')
s=s.replace("'assets/doppelganger-partner'","'assets/windblast-presentation-v1'").replace("'work/doppelganger-install-stage'","'work/windblast-presentation-install-stage-v1'")
s=s.replace("if(Get-Process -Name Dishonored2", "if($Install -and (Get-Process -Name Dishonored2").replace("-eq \"$game\\Dishonored2.exe\"){", "-eq \"$game\\Dishonored2.exe\")){")
a=s.index('$resources=@(');b=s.index('\n[void][IO.Directory]::CreateDirectory($package)',a)
s=s[:a]+"$resources=@(@{Name='models/effects/d2coop/windblast_remote.fx';File='assets/windblast_remote.fx';Kind='fx';Class='fx'})"+s[b:]
s=s.replace('D2Coop Doppelganger presentation experiment','D2Coop remote Windblast visual-only experiment')
s=s.replace('if($Install){\n $changed=', '''if($Install){
 $backup=Join-Path $root ('work/windblast-presentation-backup-'+(Get-Date -Format 'yyyyMMdd-HHmmss'))
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
 $changed=''')
s=s.replace("'research/doppelganger-asset-install.json'", "$(if($Install){'research/windblast-presentation-asset-install.json'}else{'research/windblast-presentation-asset-plan.json'})")
(lab/'Build-D2WindblastPresentationAssets.ps1').write_text(s,encoding='utf-8')
