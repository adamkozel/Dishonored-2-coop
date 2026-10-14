from pathlib import Path
P=Path(__file__).resolve().parent
source=P.parents[1]/'outputs/d2-coop-lab/src/carry_attachment_fixture.h'
s=source.read_text()
old='    fprintf(file,"{\\"testFixtureOnly\\":true,'
new='    fprintf(file,"{\\"snapshotCaptured\\":%s,\\"snapshotBound\\":%s,\\"snapshotElapsedMs\\":%llu,",f.snapshot.captured?"true":"false",f.snapshot.bound?"true":"false",f.snapshot.started&&tick>=f.snapshot.started?tick-f.snapshot.started:0);\n    fprintf(file,"\\"testFixtureOnly\\":true,'
assert s.count(old)==1;s=s.replace(old,new);source.write_text(s)
h=P/'Record-CarryAttachment.ps1';s=h.read_text()
s=s.replace("[switch]$CrouchedDrop,","[switch]$CrouchedDrop,[switch]$SnapshotBlend,")
old=" C $Role 'setviewpos -33 -153 68.1 90 0';C $peer 'setviewpos -29 -151.5 68.1 180 0'"
new=" if($SnapshotBlend){C $peer 'setviewpos -35 -160 68.1 180 0'}else{"+old.strip()+"}"
assert s.count(old)==1;s=s.replace(old,new)
s=s.replace(" if($NaturalRelease){$schedule[0].command='coop_carry_attachment 16000'}", " if($NaturalRelease){$schedule[0].command='coop_carry_attachment 16000'}\n if($SnapshotBlend){\n  if($NaturalRelease-or$NativeDrop-or$Interruption-ne'none'){throw 'Snapshot proof is a separate bounded case'}\n  $schedule[0].command='coop_carry_attachment snapshot8000'\n  # Hold still for the complete 0.8s blend and 0.35s contact settle.\n  $schedule[1].ms=3700\n }")
s=s.replace('corpseMutations=$true}|ConvertTo-Json -Depth 8|Set-Content "$out/identity.json"','corpseMutations=$true;snapshotBlendRequested=[bool]$SnapshotBlend}|ConvertTo-Json -Depth 8|Set-Content "$out/identity.json"')
h.write_text(s)
