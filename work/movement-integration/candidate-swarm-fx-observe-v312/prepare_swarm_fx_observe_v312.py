from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab/src'
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig');needle='(argc==4&&std::strcmp(argv[2],"--watch-swarm-world")';assert needle in s;s=s.replace(needle,'(argc==4&&std::strcmp(argv[2],"--watch-swarm-effects")&&std::strcmp(argv[2],"--watch-swarm-world")');p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v311.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v311.ps1','Test-SwarmRatVisualNetwork-v312.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v311.ps1').read_text(encoding='utf-8-sig');needle=' $recorders+=$fxWatcher';s=s.replace(needle,needle+"\n Start-Sleep -Milliseconds 60;if($fxWatcher.HasExited){throw 'Swarm FX observer rejected startup; see stderr'}")
(P/'Test-SwarmRatVisualNetwork-v312.ps1').write_text(s)
s=(W/'archive_swarm_fx_observe_v311.py').read_text().replace('swarm_fx_observe_v311','swarm_fx_observe_v312').replace('swarm-fx-observe-v311','swarm-fx-observe-v312').replace('swarm-fx-observe-{kind}-v311','swarm-fx-observe-{kind}-v312').replace('swarm-fx-observe-*-v311','swarm-fx-observe-*-v312').replace('Test-SwarmRatVisualNetwork-v311','Test-SwarmRatVisualNetwork-v312').replace('Candidate311:','Candidate312:311 missing CLI allowlist rejected new mode before reading game; allowlist corrected and invalid-PID command admission checked before deployment. ')
(W/'archive_swarm_fx_observe_v312.py').write_text(s)
