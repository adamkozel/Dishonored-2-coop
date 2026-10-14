from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionTargetFxView-v353.ps1').read_text()
assert s.count('66.40365 {2}')==1
s=s.replace('66.40365 {2}','68.1 {2}')
a=" & \"$PSScriptRoot/Set-ObserverPitch.ps1\" -Role client -Pitch 0"
assert a in s
s=s.replace(a,''' Start-Sleep -Milliseconds 650
 $observerFinal=Q client '--player-states';$observerFinal|ConvertTo-Json -Depth 8|Set-Content "$out/observer-final-floor.json"
 if(!$observerFinal.defaultMode-or$observerFinal.stance-ne0-or!$observerFinal.motionValid-or$observerFinal.motion-ne0-or[Math]::Abs([double]$observerFinal.position[2]-66.4)-gt.1){throw 'Camera yaw reframe changed verified observer floor'}
'''+a)
a="if(!$m.active-or$m.version-ne52-or$m.weight-lt.9-or!$stream.ready-or!$stream.armed){throw 'Release bank/native Steam stream not ready'}"
assert a in s
s=s.replace(a,"if(!$m.active-or$m.version-ne52-or$m.weight-lt.9-or!$stream.ready-or!$stream.armed){[ordered]@{movement=$m;stream=$stream;sourceStream=(S host '.shadow-stream.json');observer=(Q client '--player-states');source=(Q host '--player-states');isolation=(S client '.observer-isolation.json')}|ConvertTo-Json -Depth 12|Set-Content \"$out/preflight-failure.json\";throw 'Release bank/native Steam stream not ready'}")
(P/'Test-PossessionTargetFxView-v354.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text();assert s.count('Test-PossessionTargetFxView-v353.ps1')==1;s=s.replace('Test-PossessionTargetFxView-v353.ps1','Test-PossessionTargetFxView-v354.ps1');p.write_text(s)
print('354 preserves native eye-height convention and revalidates post-yaw floor')
