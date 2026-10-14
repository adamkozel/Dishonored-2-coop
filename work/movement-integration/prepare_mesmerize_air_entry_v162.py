"""Observe native Mesmerize entry launched together with one in-place jump."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Observe-NativeMesmerizeAirRelease-v157.ps1').read_text(encoding='utf-8-sig')
a=s.index('  if(!$cast-and$w.ElapsedMilliseconds-ge1100)')
b=s.index("  $scene=Q client '--midnight'",a)
s=s[:a]+'''  if(!$cast-and$w.ElapsedMilliseconds-ge1100){
   & "$lab/Send-D2Input.ps1" -Role client -Button A -LeftTrigger 255 -DurationMs 1800|Out-Null;$cast=$true;$jumped=$true
   $events.Add([pscustomobject]@{videoTime=([DateTimeOffset]::UtcNow.ToUnixTimeMilliseconds()-$start)/1000;input='Native jump and Mesmerize aim together; LT held1800ms then released'})
  }
'''+s[b:]
s=s.replace('$airRelease','$airEntry').replace('power_mesmerize_cast_out.midnightscene\'-and$_.sceneDurationMs-eq666','power_mesmerize_cast_in.midnightscene\'-and$_.sceneDurationMs-eq466')
s=s.replace('native airborne release','native airborne entry').replace('exact666ms reciprocal Mesmerize release','exact466ms reciprocal Mesmerize entry')
s=s.replace('Native source in-place jump while holding Mesmerize aim, releasing LT180ms after A.','Native source in-place jump and Mesmerize aim begin together with1800ms hold; directly inspect airborne entry and later grounded release.')
(P/'Observe-NativeMesmerizeAirEntry-v162.ps1').write_text(s,encoding='utf-8')
