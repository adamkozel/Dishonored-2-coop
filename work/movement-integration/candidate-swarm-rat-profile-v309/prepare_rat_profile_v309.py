from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset'
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v308.ps1' in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v308.ps1','Test-SwarmRatVisualNetwork-v309.ps1'))
s=(P/'Test-SwarmRatVisualNetwork-v308.ps1').read_text(encoding='utf-8-sig')
needle=" C host 'coop_shadow_stream off';Start-Sleep -Milliseconds 1200"
block=""" # Quiet timing windows: fixed-duration recorders/watcher must be finished.
 foreach($job in @($recorders)+@($monitor)){$job.WaitForExit(1500)|Out-Null;if(!$job.HasExited-or$job.ExitCode){throw 'Quiet profile requires completed successful recorders and watcher'}}
 $quietActive=@(for($i=0;$i-lt11;$i++){S client '.swarm-rat-visual.json';Start-Sleep -Milliseconds 250})
 $quietActive|ConvertTo-Json -Depth 12|Set-Content "$out/quiet-active.json"
 if(@($quietActive|Where-Object activeModels -gt0).Count-ne11){throw 'Quiet active profile lost cosmetic pool'}
"""
assert needle in s;s=s.replace(needle,block+needle)
needle=" $checks.Add('Source stream retirement hides cached native rat animleaf model')"
block="""
 $quietHidden=@(for($i=0;$i-lt11;$i++){S client '.swarm-rat-visual.json';Start-Sleep -Milliseconds 250})
 $quietHidden|ConvertTo-Json -Depth 12|Set-Content "$out/quiet-hidden.json"
 if(@($quietHidden|Where-Object activeModels -gt0).Count){throw 'Quiet hidden profile reacquired cosmetic pool'}
 $checks.Add('Quiet active and hidden service timings captured after all video and watcher processes finished; timing stages overlap and are not display FPS')
"""
assert needle in s;s=s.replace(needle,needle+block)
(P/'Test-SwarmRatVisualNetwork-v309.ps1').write_text(s)
s=(W/'archive_pose_bootstrap_v308.py').read_text().replace('pose_bootstrap_v308','rat_profile_v309').replace('swarm-pose-bootstrap-v308','swarm-rat-profile-v309').replace('swarm-pose-bootstrap-{kind}-v308','swarm-rat-profile-{kind}-v309').replace('swarm-pose-bootstrap-*-v308','swarm-rat-profile-*-v309').replace('Test-SwarmRatVisualNetwork-v308','Test-SwarmRatVisualNetwork-v309')
start=s.index("scope='")+len("scope='");end=s.index("')\n(A/",start)
s=s[:start]+'Candidate309: bounded per-world cosmetic timing aggregates for identity, gait, scale, live transform and total. Nested stages overlap; service is on10ms transport cadence, not each render frame. Separate source-present/absent categories and callback intervals. Test adds quiet active/hidden windows after all video/watcher processes finish to distinguish capture/probe load.308 startup calibration PASS23 and scale307 retained. No behavior optimization yet, no native content change. Performance attribution and shared gameplay incomplete.'+s[end:]
s=s.replace("assert read(R/'work/natural-moveset/swarm-rat-scale-owner-v307/result.json')['passed']","assert read(R/'work/natural-moveset/swarm-pose-bootstrap-v308/result.json')['passed']")
(W/'archive_rat_profile_v309.py').write_text(s)
