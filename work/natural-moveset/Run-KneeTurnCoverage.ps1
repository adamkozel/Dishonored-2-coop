param([ValidatePattern('^[a-z0-9-]+$')][string]$Suffix='knee-v4')
$ErrorActionPreference='Stop'
$python='C:/Users/adako/AppData/Local/Programs/Python/Python312/python.exe'
$pillow='C:/Users/adako/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe'
$validator=Join-Path $PSScriptRoot '../movement-integration/verify_knee_pole.py'
foreach($source in 'host','client'){
 $role=if($source-eq'host'){'corvo'}else{'emily'}
 foreach($unarmed in $false,$true){
  $take="crouch-turn-$role-"+$(if($unarmed){'unarmed-'}else{''})+$Suffix
  & "$PSScriptRoot/Test-TurnBankV19.ps1" -Source $source -Take $take -Crouched -AuthoredFeet -CloseCamera -Record -Unarmed:$unarmed
  & $python $validator "$PSScriptRoot/$take"
  if($LASTEXITCODE){throw 'Native knee verification failed'}
  & $pillow "$PSScriptRoot/make_contact_sheet.py" "$PSScriptRoot/$take.mp4" "$PSScriptRoot/$take/contact.jpg" 2.5 6.5 10.5 14.5 18.5 22.5
  if($LASTEXITCODE){throw 'Contact-sheet generation failed'}
 }
}
foreach($source in 'host','client'){
 $role=if($source-eq'host'){'corvo'}else{'emily'}
 foreach($unarmed in $false,$true){
  $take="turn-stance-$role-"+$(if($unarmed){'unarmed-'}else{''})+$Suffix
  & "$PSScriptRoot/Test-TurnStanceV19.ps1" -Source $source -Take $take -AuthoredFeet -CloseCamera -Record -Unarmed:$unarmed
  & $python $validator "$PSScriptRoot/$take"
  if($LASTEXITCODE){throw 'Native knee verification failed'}
  $marks=@();foreach($chapter in ((Get-Content "$PSScriptRoot/$take/result.json" -Raw|ConvertFrom-Json).chapters)){foreach($offset in .8,2.6,5.2){$marks+=[string]($chapter.time+$offset)}}
  & $pillow "$PSScriptRoot/make_contact_sheet.py" "$PSScriptRoot/$take.mp4" "$PSScriptRoot/$take/contact.jpg" @marks
  if($LASTEXITCODE){throw 'Contact-sheet generation failed'}
 }
}
'Knee-turn coverage captured and structurally verified; visual review remains required.'
