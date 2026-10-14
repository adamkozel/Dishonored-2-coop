from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';out=P/'Test-PartnerSwordContexts-v398.ps1';assert not out.exists()
s=(P/'Test-PartnerSwordContexts-v397.ps1').read_text(encoding='utf-8-sig').replace('partner-sword-contexts-host-v397','partner-sword-contexts-host-v398')
s=s.replace('!$filterAfter.installed-or$filterAfter.blocked-le$filterBefore.blocked-or$filterAfter.readFailed-ne$filterBefore.readFailed','!$filterAfter.installed-or$filterAfter.readFailed-ne$filterBefore.readFailed')
s=s.replace("throw 'Exact partner eligibility filter did not reject consistently'","throw 'Assassination filter unavailable or identity validation failed'")
anchor='    $partnerHandle=$row.source.probeActorHandle'
s=s.replace(anchor,anchor+'''
    $global=@($row.sensors|Where-Object {$_.type-eq'.?AVarkCpntPlayerSensorGlobalAssassination@@'})
    if($global.Count-ne1){throw 'Global sensor sample absent'}
    foreach($word in $global[0].object.words){
     if($word.offset-in@('0x60','0x98','0xd0','0x108','0x140')){
      $target=[Convert]::ToUInt64($word.word.Substring(2),16)-band0xffffffffL
      if($target-eq$partnerHandle){throw 'Partner remains in global assassination target records'}
     }
    }''')
out.write_text(s,encoding='utf-8');print('398 validates actual target records, not mandatory downstream block counter')
