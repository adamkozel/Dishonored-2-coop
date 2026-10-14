"""Preserve historical tests; extend their real-input checks for exact visibility."""
from pathlib import Path
P=Path(__file__).resolve().parent
src=(P/'Test-EquipmentBank.ps1').read_text(encoding='utf-8-sig')
src=src.replace("'equipment-bank-v1-test'","'equipment-visibility-v26'")
src=src.replace('Copy-Item "$PSScriptRoot/equipment-banks-v3/$role-equipment-bank.bin"', '''$bank=Get-Content "$PSScriptRoot/carry-pickup-contact-bank-manifest-v5.json" -Raw|ConvertFrom-Json|Where-Object role -eq $role
  $path=Join-Path $PSScriptRoot $bank.bank;if((Get-FileHash -LiteralPath $path).Hash-ne$bank.sha256){throw 'Bank differs'}
  Copy-Item -LiteralPath $path -Destination''')
src=src.replace('.version-ne9','.version-ne34')
src=src.replace("$eq.equipped-ne$held-or$eq.started-ne$held", "$eq.explicitHidden-ne(!$held)-or$eq.equipped-ne$held-or$eq.started-ne$held")
src=src.replace('settles to matching native sword state','settles to matching native sword state and independent visibility flag')
src=src.replace("$checks.Add(\"${src}: player and avatar health preserved; native outcomes unchanged\")", '''$restored=Q $dst '--avatar-sword' $handles[$dst];$lease=ReadStatus $dst '.shadow-equipment.json'
  if($restored.explicitHidden-or!$restored.equipped-or$lease.active-or$lease.restoring-or$lease.lost-or$lease.visibilityRefused){throw 'Visibility cleanup failed'}
  $checks.Add("${src}: original visible drawn sword restored, no visibility refusal or lost lease; health unchanged")''')
(P/'Test-EquipmentVisibility-v26.ps1').write_text(src)
src=(P/'Test-EquipmentPause.ps1').read_text(encoding='utf-8-sig')
src=src.replace("'equipment-pause-v1'","'equipment-visibility-pause-v26'")
src=src.replace("if((Q $dst '--avatar-sword' $handles[$dst]).equipped-or", "if(!(Q $dst '--avatar-sword' $handles[$dst]).explicitHidden-or(Q $dst '--avatar-sword' $handles[$dst]).equipped-or")
src=src.replace("$eq.lost-or!$sword.equipped", "$eq.lost-or$eq.visibilityRefused-or$sword.explicitHidden-or!$sword.equipped")
src=src.replace('restores original native sword without replaying movement','restores original visible native sword without replaying movement')
(P/'Test-EquipmentVisibilityPause-v26.ps1').write_text(src)
