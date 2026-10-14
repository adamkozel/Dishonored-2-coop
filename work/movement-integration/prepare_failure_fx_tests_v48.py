"""Preserve v46 harness and create v48 paired effect assertions."""
from pathlib import Path
R=Path(__file__).resolve().parents[2]; P=R/'work/natural-moveset'
s=(P/'Test-OutOfManaContexts.ps1').read_text()
def replace(a,b):
 global s
 assert s.count(a)==1,(a,s.count(a))
 s=s.replace(a,b)
replace("$Take='outofmana-corvo-standing-armed-v45'", "$Take='outofmana-corvo-standing-armed-fx-v48'")
replace("stream=(S client '.shadow-stream.json')", "stream=(S client '.shadow-stream.json');fx=(S client '.power-fx.json')")
replace("$fxAfter=S client '.power-fx.json';if($fxAfter.started-ne$fxBefore.started-or$fxAfter.active){throw 'Failure created unsolicited observer effect'}", """$fxAfter=S client '.power-fx.json'
 $failureFx=@($rows|ForEach-Object {$_.fx.effects}|Where-Object kind -eq 11)
 $ids=@($failureFx|Select-Object -ExpandProperty id -Unique)
 if($fxAfter.started-ne($fxBefore.started+2)-or$fxAfter.active-or$ids.Count-ne2-or$fxAfter.handUpdates-le$fxBefore.handUpdates){throw 'Expected exactly two finite native hand effects'}
 if(@($failureFx|Where-Object {$_.owner-ne$avatar-or$_.record-eq'0x0'}).Count){throw 'Failure FX ownership mismatch'}
 if(@($rows|Where-Object {$_.ms-lt$events[2].ms-and$_.fx.started-ne$fxBefore.started}).Count){throw 'Successful cast triggered failure FX'}
 $checks.Add('Exactly one finite owned native hand effect per failed scene, with no effect on successful casts and no replay')""")
replace("Remote failure recovers to original stance with original sword, health and no observer screen effect", "Remote failure recovers to original stance with original sword and health; both owned hand effects finish")
replace("Native time dilation remains source-local; no remote power activation or animation damage.", "Two source-confirmed failures trigger two native world hand effects at delayed presentation time. Wrist-centred placement; native time remains source-local and no remote power activation or animation damage occurs.")
dest=P/'Test-OutOfManaFx.ps1';assert not dest.exists();dest.write_text(s)
print(dest)
