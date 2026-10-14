from pathlib import Path
p=Path('work/natural-moveset/Test-MesmerizeWorldNetwork-v80.ps1');s=p.read_text().replace('setviewpos -30.7 -151.8 68.1 230 0','setviewpos -30.7 -151.8 68.1 203 0')
a=s.index("   $sourcePlayer=Q client '--player';");b=s.index('   $worldFramed=$true',a)
s=s[:a]+'''   # Frame the effect separately after the close body phase. Keep the
   # camera within the previously tested room corridor, not inside its east wall.
   $cameraX=[double]$committed.transform[0]+3;$cameraY=[double]$committed.transform[1]-4
   if($cameraX-lt-31.5-or$cameraX-gt-30-or$cameraY-lt-161-or$cameraY-gt-147){throw 'World evidence camera outside tested room bounds'}
   C host ('setviewpos {0} {1} 68.1 126.8699 0' -f $cameraX,$cameraY)
   & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 3 -Evidence "$out/world-camera.json" -Quick
'''+s[b:]
Path('work/natural-moveset/Test-MesmerizeWorldNetwork-v81.ps1').write_text(s)
p=Path('work/movement-integration/Invoke-NativeLabOperation.ps1');s=p.read_text().replace('Test-MesmerizeWorldNetwork-v80.ps1','Test-MesmerizeWorldNetwork-v81.ps1');p.write_text(s)
