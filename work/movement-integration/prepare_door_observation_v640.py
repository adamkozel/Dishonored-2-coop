from pathlib import Path
R=Path(__file__).resolve().parents[2]; N=R/'work/natural-moveset'
s=(N/'Observe-NativeDoor-v638.ps1').read_text()
s=s.replace('native-door-corvo-v638','native-unlocked-door-corvo-v640').replace('NativeDoor638','NativeDoor640')
s=s.replace("name='door-tap';yaw=180", "name='unlocked-door-tap';yaw=270")
s=s.replace("setviewpos -32.5 -151 68.1 217 0", "setviewpos -44 -161 68.1 220 0")
s=s.replace("setviewpos -35.9 -153.55 68.1 180 0", "setviewpos -47.05 -163.5 68.1 270 0")
insert='''function W($o,$offset){$w=@($o.words|Where-Object offset -eq $offset);if($w.Count-ne1){throw "Missing object field $offset"};[Convert]::ToUInt64($w[0].word.Substring(2),16)}
function DoorSample($r){
 $d=$doorBindings[$r];$o=Q $r '--object' $d.component
 if($o.type-ne'.?AVarkCpntOpenable@@'-or(W $o '0x10')-ne$d.owner-or((W $o '0x18')-band 0xffffffff)-ne$d.generation){throw 'Door component lifetime changed'}
 $state=W $o '0x48';$flags=W $o '0x1e8'
 [pscustomobject]@{handle=$d.handle;current=[uint32]($state-band 0xffffffff);pending=[uint32]($state-shr 32);locked=[int]($flags-band 255);flags1e8=('0x{0:x}'-f$flags);state90=('0x{0:x}'-f(W $o '0x90'));child=[uint32]((W $o '0xd8')-shr 32)}
}
'''
s=s.replace('try{\n try{$owned=',insert+'try{\n try{$owned=',1)
needle="   $character=if($source-eq'host')"
insert='''   $doorBindings=@{}
   foreach($r in 'host','client'){
    $door=@((Q $r '--entities').entities|Where-Object name -eq 'ld_door_350cm_int_rich_wood_01_3')
    if($door.Count-ne1-or$door[0].type-ne'.?AVarkDoor@@'){throw 'Exact ordinary door missing'};$door=$door[0]
    $cp=@((Q $r '--components' $door.handle).components|Where-Object type -eq '.?AVarkCpntOpenable@@')
    if($cp.Count-ne1-or$cp[0].owner-ne$door.pointer){throw 'Door component owner mismatch'};$cp=$cp[0]
    $obj=Q $r '--object' $cp.pointer;$obj|ConvertTo-Json -Depth 7|Set-Content "$out/openable-before-$r.json"
    $doorBindings[$r]=[pscustomobject]@{handle=$door.handle;component=$cp.pointer;owner=[Convert]::ToUInt64($door.pointer.Substring(2),16);generation=((W $obj '0x18')-band 0xffffffff)}
    if((DoorSample $r).locked-ne0){throw 'Unlocked door required'}
   }
   $doorBindings|ConvertTo-Json -Depth 5|Set-Content "$out/door-bindings.json"
'''
s=s.replace(needle,insert+needle,1)
s=s.replace('ms=$watch.ElapsedMilliseconds;player=', 'ms=$watch.ElapsedMilliseconds;doorSource=(DoorSample $source);doorPeer=(DoorSample $peer);player=')
s=s.replace('at the known native office door','at the adjacent unlocked door')
s=s.replace('Source-only native office door short-X observation.', 'Source-only native unlocked door short-X observation with independently read source/peer Openable states.')
s=s.replace('PASS_NATIVE_CLOSE_COMBAT','PASS_NATIVE_DOOR_OBSERVATION')
p=N/'Observe-NativeDoor-v640.ps1'
if p.exists(): raise SystemExit('Unique script already exists')
p.write_text(s)
print('Prepared640: adjacent unlocked door; native input, both live Openable state samples, no forced door operation.')
