from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-DoorOverlap-v748.ps1').read_text(encoding='utf-8-sig')
def rep(a,b):
    global s
    assert a in s,a
    s=s.replace(a,b)
rep('native-door-overlap-opposite-host-v748','native-door-close-reopen-host-v750')
rep('NativeDoorOverlap748','NativeDoorCloseReopen750')
rep("$DoorName='ld_door_350cm_int_rich_wood_01_3'","$DoorName='ld_door_350cm_int_rich_wood_01_2'")
rep("if($initialOpen){throw 'Overlap test requires the closed closet door'}","if(!$initialOpen){throw 'Closing conflict requires the initially open bathroom door'}")
rep("C $peer 'setviewpos -48 -158.5 68.1 215 0'","C $peer 'setviewpos -52.3 -160.75 68.1 0 0'")
rep("input='first player native open'","input='first player native close'")
rep("input='second player uses door during received native motion'","input='second player native reopen during received close'")
rep('$nativeMoving.current-eq1-and$nativeMoving.pending-eq1-and$receiving.inFlight',"$nativeMoving.current-eq1-and$nativeMoving.pending-eq1-and$receiving.target-eq0-and$receiving.inFlight")
rep("$states[0]-ne0-or$states[-1]-ne0","$states[0]-ne2-or$states[-1]-ne2")
rep("$peerStates[0]-ne0-or$peerStates[-1]-ne0","$peerStates[0]-ne2-or$peerStates[-1]-ne2")
rep('finish closed after overlapping native input','finish open after overlapping native input')
rep('converge closed at host revision3','converge open at host revision3')
rep('Host orders the newer closed target','Host orders the newer open target')
(N/'Test-DoorOverlap-v750.ps1').write_text(s)
print('Prepared native close/reopen overlap750 on initially open second door')
