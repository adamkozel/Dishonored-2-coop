from pathlib import Path
W=Path(__file__).resolve().parent;N=W.parent/'natural-moveset'
s=(N/'Test-DoppelgangerFx-v476.ps1').read_text(encoding='utf-8-sig')
s=s.replace("$cloneStart=S client '.doppelganger-world.json';", "$anchorCaptured=$false;$packetRefusedStart=(S client '.doppelganger-world.json').packetRefused; $cloneStart=S client '.doppelganger-world.json';")
needle="  if(!$m.active-or!$stream.ready-or!$stream.armed)"
insert='''  $recent=$rows[$rows.Count-1].cloneSource
  if(!$anchorCaptured-and$recent.packetRefused-gt$packetRefusedStart-and$recent.present-and$recent.nativeHandle){
   $recent|ConvertTo-Json -Depth 12|Set-Content "$out/refused-source-status.json"
   Q client '--skeleton' $recent.nativeHandle|ConvertTo-Json -Depth 14|Set-Content "$out/refused-native-skeleton.json"
   $anchorCaptured=$true
  }
'''
assert needle in s;s=s.replace(needle,insert+needle)
(N/'Test-DoppelgangerFx-v478.ps1').write_text(s,encoding='utf-8')
(W/'Run-DoppelgangerFx-v478.ps1').write_text((W/'Run-DoppelgangerFx-v473.ps1').read_text(encoding='utf-8-sig').replace('v473','v478'),encoding='utf-8')
