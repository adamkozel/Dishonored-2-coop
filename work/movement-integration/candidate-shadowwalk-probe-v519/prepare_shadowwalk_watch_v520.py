from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';S=R/'outputs/d2-coop-lab/src';N=R/'work/natural-moveset'
def once(s,a,b):
    assert s.count(a)==1,a
    return s.replace(a,b)
p=S/'runtime_probe.cpp';s=p.read_text(encoding='utf-8-sig')
s=once(s,'||!std::strcmp(argv[2],"--doppelganger-effects")))','||!std::strcmp(argv[2],"--doppelganger-effects")||!std::strcmp(argv[2],"--shadowwalk-effects")))')
needle=':readNativeWindblastFxRecords(reader,base,game,records,&failureLine)'
s=once(s,needle,':std::strcmp(argv[2],"--shadowwalk-effects")==0?readNativeFilteredFxRecords(reader,base,game,records,[](const std::string& name){return name.find("models/effects/gameplay/powers/shadow_walk/")==0;},&failureLine,&diagnostic)'+needle)
p.write_text(s,encoding='utf-8')
s=(N/'Observe-NativeShadowWalk-v516.ps1').read_text(encoding='utf-8-sig')
needle=" $w.Restart();$cast=$false;$forward=$false;$back=$false;$entityNext=2000"
s=once(s,needle,''' $strictMonitor=Start-Process "$lab/bin/d2_runtime_probe.exe" -ArgumentList @($pids.client,'--watch-shadowwalk-state','27000') -WindowStyle Hidden -RedirectStandardOutput "$out/shadowwalk-state.ndjson" -RedirectStandardError "$out/shadowwalk-state.stderr" -PassThru
'''+needle)
s=once(s,"component=(O client '--object' $power.component);entities=","component=(O client '--object' $power.component);effects=(O client '--shadowwalk-effects');entities=")
needle=" $checks.Add('Native mana decreases; replenishment prevents interpreting this as exact cost')"
s=once(s,needle,needle+'''
 $strictMonitor.WaitForExit(4000)|Out-Null;if(!$strictMonitor.HasExited-or$strictMonitor.ExitCode){throw 'Shadow Walk mode watcher failed'}
 $strict=@(Get-Content "$out/shadowwalk-state.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $valid=@($strict|Where-Object valid);$phases=@($valid.phase|Sort-Object -Unique)
 foreach($phase in 0,2,3,4,5){if($phase-notin$phases){throw "Strict phase $phase not observed"}}
 $form=@($valid|Where-Object {$_.form-and$_.phase-eq4-and$_.stance-eq2})
 if(!$form.Count){throw 'Strict alternate native mode not observed'}
 $fx=@($rows|Where-Object {$_.effects.stable-ne$false}|ForEach-Object {$_.effects.records})
 [ordered]@{samples=$strict.Count;accepted=$valid.Count;phases=$phases;formSamples=$form.Count;effectNames=@($fx.name|Sort-Object -Unique);effectOwners=@($fx.owner|Sort-Object -Unique);scope='Read-only exact equipment, reciprocal state-machine owner, native alternate mode and phase guards. FX records independently enumerated; no attribution or remote binding inferred.'}|ConvertTo-Json -Depth 10|Set-Content "$out/strict-state-observations.json"
 $checks.Add('Strict native mode reader observes all recorded phases including exact alternate ShadowWalk form and retirement')
''')
(N/'Observe-NativeShadowWalk-v520.ps1').write_text(s,encoding='utf-8')
w=(W/'Run-ShadowWalkObserve-v516.ps1').read_text(encoding='utf-8-sig').replace('native-shadowwalk-standing-v516','native-shadowwalk-state-v520').replace('Observe-NativeShadowWalk-v516','Observe-NativeShadowWalk-v520')
(W/'Run-ShadowWalkObserve-v520.ps1').write_text(w,encoding='utf-8')
print('Added independent native Shadow Walk FX enumeration and strict phase watcher to next source observation.')
