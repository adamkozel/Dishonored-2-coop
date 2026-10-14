from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionReleaseNetwork-v334.ps1').read_text()
s=s.replace("$_.spawnerName-in@('ld_spawner_guard_tutorial_1','ld_spawner_guard_tutorial_2')", "$_.spawnerName-eq'ld_spawner_guard_tutorial_2'")
# Reject a target that has moved to another floor; do not infer an unseen target.
needle="   if($fresh.Count-ne1){throw 'Original guard identity changed before cast'}"
assert s.count(needle)==1
s=s.replace(needle,needle+"\n   if([Math]::Abs([double]$fresh[0].position[2]-66.4)-gt.5){throw 'Verified target left the measured test floor'}")
# Preserve setup placement and finite duration. More time for native target transfer.
s=s.replace("'-t','9'","'-t','11'").replace("'8000'","'10000'").replace('$w.ElapsedMilliseconds-lt7000','$w.ElapsedMilliseconds-lt9000').replace('$w.ElapsedMilliseconds-ge4000','$w.ElapsedMilliseconds-ge5200')
s=s.replace("power=(O host '--player-possession');movement=", "power=(O host '--player-possession');isolation=(S host '.observer-isolation.json');movement=")
(P/'Test-PossessionReleaseNetwork-v335.ps1').write_text(s,encoding='utf-8')
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text().replace('Test-PossessionReleaseNetwork-v334.ps1','Test-PossessionReleaseNetwork-v335.ps1');p.write_text(s,encoding='utf-8')
print('335 targets only observed upper-floor tutorial guard2 and records AI lease;334 target1 descended stairs before cast')
