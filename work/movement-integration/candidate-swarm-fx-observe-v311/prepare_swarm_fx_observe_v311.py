from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v310.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v310.ps1','Test-SwarmRatVisualNetwork-v311.ps1'))
# No quiet timing claim while an additional effect observer runs. Use308's
# established gameplay harness, supplement with a read-only effect transcript.
s=(P/'Test-SwarmRatVisualNetwork-v308.ps1').read_text(encoding='utf-8-sig')
needle=' $w.Restart();$cast=$false;$stood=$false;$releaseWalking=$false'
replacement=''' $fxWatcher=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.host,'--watch-swarm-effects','15000') -WindowStyle Hidden -RedirectStandardOutput "$out/swarm-effects.ndjson" -RedirectStandardError "$out/swarm-effects.stderr" -PassThru
 $recorders+=$fxWatcher
'''+needle
assert needle in s;s=s.replace(needle,replacement)
(P/'Test-SwarmRatVisualNetwork-v311.ps1').write_text(s)
s=(W/'archive_rat_profile_v309.py').read_text().replace('rat_profile_v309','swarm_fx_observe_v311').replace('swarm-rat-profile-v309','swarm-fx-observe-v311').replace('swarm-rat-profile-{kind}-v309','swarm-fx-observe-{kind}-v311').replace('swarm-rat-profile-*-v309','swarm-fx-observe-*-v311').replace('Test-SwarmRatVisualNetwork-v309','Test-SwarmRatVisualNetwork-v311')
start=s.index("scope='")+len("scope='");end=s.index("')\n(A/",start)
s=s[:start]+'Candidate311: unchanged309 gameplay DLL; new read-only bounded15-second Swarm FX watcher in runtime probe, using existing stable FX-map reader and exact rat_swarm path prefix. Captures observed record IDs, native owner definitions/parents and transforms. Inventory scan is research only and cannot authorize playback. Read-only exact native FX assets exported with provenance. No new peer FX or gameplay mutation. Native source ownership and lifecycle interpretation pending.'+s[end:]
s=s.replace("assert read(R/'work/natural-moveset/swarm-pose-bootstrap-v308/result.json')['passed']","assert read(R/'work/natural-moveset/swarm-rat-profile-v310/result.json')['passed']")
(W/'archive_swarm_fx_observe_v311.py').write_text(s)
