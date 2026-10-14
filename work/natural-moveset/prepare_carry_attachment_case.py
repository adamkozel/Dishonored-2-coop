from pathlib import Path
P=Path(__file__).resolve().parent;ROOT=P.parent.parent
p=ROOT/'outputs/d2-coop-lab/Send-D2Command.ps1';s=p.read_text();a='|coop_agility_fixture|'
assert s.count(a)==1;s=s.replace(a,'|coop_agility_fixture|coop_carry_attachment|');p.write_text(s)
s=(P/'Record-CarryCarrier.ps1').read_text()
s=s.replace("'coop_movement_resume off','coop_movement_bank off'","'coop_carry_attachment off','coop_movement_resume off','coop_movement_bank off'")
s=s.replace('Carrier-only native Steam integration. Authored loaded idle and forward walking, standing and crouched. Matched corpse is observed only; attachment, pickup, release and physical outcomes are not ported.',
 'Eight-second explicitly armed native attachment mechanics experiment on the authored Mayhew corpse. No victim Midnight animation; shoulder pose and ragdoll outcome unaccepted.')
s=s.replace('corpseMutations=$false','corpseMutations=$true')
s=s.replace('$schedule=@(@{ms=3000',"$schedule=@(@{ms=2000;command='coop_carry_attachment 8000'},@{ms=3000")
s=s.replace('I $e.button $e.y $e.duration;',"if($e.command){C $peer $e.command}else{I $e.button $e.y $e.duration};")
s=s.replace("movement=(J $peer '.movement.json')","movement=(J $peer '.movement.json');attachment=(J $peer '.carry-attachment.json');retarget=(Q $peer '--retargeting' ([string](J $peer).probeActorHandle))")
s=s.replace("try{C $r 'coop_movement_bank off'","try{C $r 'coop_carry_attachment off';C $r 'coop_movement_bank off'")
(P/'Record-CarryAttachment.ps1').write_text(s)
