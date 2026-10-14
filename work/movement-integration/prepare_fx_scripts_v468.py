from pathlib import Path
W=Path(__file__).resolve().parent;N=W.parent/'natural-moveset'
for name in ('Build-DoppelgangerVisual-v464.ps1','prepare_doppelganger_fx_v464.py'):
    s=(W/name).read_text(encoding='utf-8-sig').replace('v464','v468')
    s=s.replace("previous = W / 'candidate-doppelganger-fx-v462'", "previous = W / 'candidate-doppelganger-fx-v452'")
    s=s.replace('Diagnostic hardware write watch on the exact owned disappearance scene state for three seconds; previous pause entry hooks removed. Presentation unchanged; remove watch after investigation.', '452 baseline plus native force-update creation option for owned finite FX/material clone scenes, avoiding LOD pause while culled. All temporary pause hooks and hardware watches removed. Runtime confirmation pending.')
    (W/name.replace('v464','v468')).write_text(s,encoding='utf-8')
s=(N/'Test-DoppelgangerFx-v463.ps1').read_text(encoding='utf-8-sig')
s=s.replace("if(!$shown.Count){throw 'Remote cosmetic effect scene lacks independent native binding'}", """if($shown.Count-lt3){throw 'Remote cosmetic effect scene lacks independent native binding'}
  $clocks=@($shown|ForEach-Object {[int]$_.clockWords[1]})
  $maximum=($clocks|Measure-Object -Maximum).Maximum;$minimum=($clocks|Measure-Object -Minimum).Minimum
  if($maximum-$minimum-lt100-or$maximum-lt$(if($v-eq1){800}else{370})){throw 'Remote effect scene clock did not progress through its native duration'}
  if(@($peerMidnight.instances|Where-Object {$_.id-eq$h[0].nativeScene-and$_.state-eq5}).Count){throw 'Remote owned cosmetic effect entered paused state'}""")
s=s.replace('observations=$shown.Count;trace=$trace','observations=$shown.Count;minimumClock=$minimum;maximumClock=$maximum;trace=$trace')
(N/'Test-DoppelgangerFx-v469.ps1').write_text(s,encoding='utf-8')
(W/'Run-DoppelgangerFx-v469.ps1').write_text((W/'Run-DoppelgangerFx-v463.ps1').read_text(encoding='utf-8-sig').replace('v463','v469'),encoding='utf-8')
