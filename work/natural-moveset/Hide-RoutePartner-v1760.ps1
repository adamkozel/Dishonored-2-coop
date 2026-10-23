param([double]$X,[double]$Y,[double]$Z,[double]$Yaw=0,[switch]$Crouch)
# 1760 (#171): place the VM partner (setviewpos) and crouch her, as Route-MissionVM's 'partner' step does. Called by the
# route action pickpocket_focus before each try: Memories of Training's trainer fails his stealth lesson when he notices
# either player, and every lesson restart puts the partner back at the lesson start, in his sight.
$ErrorActionPreference='Stop';$root=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
. "$root/tools/vm/D2LabVM.Common.ps1";. "$root/tools/vm/D2LabVM.Lab.ps1"
$inv=[Globalization.CultureInfo]::InvariantCulture
Send-D2LabVMCommand -Command ('setviewpos {0} {1} {2} {3} 0'-f$X.ToString('F3',$inv),$Y.ToString('F3',$inv),$Z.ToString('F3',$inv),$Yaw.ToString('F2',$inv)) -WaitConsumed|Out-Null
Start-Sleep -Milliseconds 700
$crouched=$false
if($Crouch){for($k=0;$k-lt4;$k++){$ids=@();try{$ids=@((Invoke-D2LabVMProbe -Mode '--player-states').active|ForEach-Object {$_.id})}catch{}
 if($ids-contains6){$crouched=$true;break};Send-D2LabVMInput -Button B -DurationMs 150|Out-Null;Start-Sleep -Milliseconds 900}}
[ordered]@{placed=@($X,$Y,$Z);crouched=$crouched}|ConvertTo-Json -Compress
