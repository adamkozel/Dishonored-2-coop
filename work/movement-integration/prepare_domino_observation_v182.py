"""Record genuine native Domino phases without presupposing their bindings."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';S=R/'outputs/d2-coop-lab/src'
f=S/'runtime_probe.cpp';s=f.read_text(encoding='utf-8-sig')
a='||!std::strcmp(argv[2],"--mesmerize-effects")))';assert s.count(a)==1
s=s.replace(a,'||!std::strcmp(argv[2],"--mesmerize-effects")||!std::strcmp(argv[2],"--domino-effects")))')
a=':readNativeWindblastFxRecords(reader,base,game,records,&failureLine))'
assert s.count(a)==1
s=s.replace(a,':std::strcmp(argv[2],"--domino-effects")==0?readNativeFilteredFxRecords(reader,base,game,records,[](const std::string& name){return name.find("models/effects/gameplay/powers/phantasmal_link/")==0;},&failureLine,&diagnostic):readNativeWindblastFxRecords(reader,base,game,records,&failureLine))')
f.write_text(s,encoding='utf-8')
s=(P/'Observe-NativeMesmerize-v72.ps1').read_text(encoding='utf-8-sig').replace('Mesmerize','Domino').replace('mesmerize','domino').replace('MESMERIZE','DOMINO')
s=s.replace("C client 'setviewpos -33.5 -153 68.1 270 0'","$dirty=$true;C client 'setviewpos -48.59 -155.07 68.1 90 0'")
s=s.replace("'--watch-power-scenes'","'--watch-midnight'").replace('power-scenes.ndjson','midnight.ndjson').replace('power-scenes.stderr','midnight.stderr')
s=s.replace("$_.definitionName-match'domino'","$_.definitionName-match'phantasmal'")
s=s.replace("'coop_domino_fixture off','coop_movement_fixture off'","'coop_mesmerize_fixture off','coop_domino_fixture off','coop_movement_fixture off'")
a=s.index(' $fastScenes=@(');b=s.index(" C client 'coop_domino_fixture off';",a)
s=s[:a]+''' $rawSamples=@(Get-Content "$out/midnight.ndjson"|ForEach-Object {$_|ConvertFrom-Json})
 $rawScenes=@($rawSamples|ForEach-Object {$_.instances}|Where-Object {$_.state-eq3-and$_.asset-like'*power_phantasmal_cast*'})
 [ordered]@{inputSent=$cast;sceneSamples=$rawScenes.Count;subscenes=@($rawScenes|Group-Object subscene|ForEach-Object {[ordered]@{name=$_.Name;samples=$_.Count;durations=@($_.Group.sceneDurationMs|Sort-Object -Unique)}});manaBefore=$manaBefore.currentCandidate;manaAfter=$manaAfter.currentCandidate;nativeStates=@($rows.player);raw=$rawScenes}|ConvertTo-Json -Depth 16|Set-Content "$out/domino-observations.json"
 if(!$cast-or!$rawScenes.Count){throw 'No native Domino phase observed after controller input'}
 if(@($rawScenes|Where-Object {@($_.bindingsResearch|Where-Object {!$_.reciprocal}).Count}).Count){throw 'Observed Domino scene includes a nonreciprocal entity binding'}
 $checks.Add('Controller input produces recorded native PhantasmalLink scenes with reciprocal bindings; phase names and counts are observed, not inferred')
 $checks.Add('Native player state, equipment, raw component/entity and filtered effect samples retained; mana change and targeting outcome remain observations')
'''+s[b:]
s=s.replace("$checks.Add('Both isolated checkpoints restore clean health/inventory and retire native Domino effects')","$checks.Add('Both isolated checkpoints restore clean health/inventory and no filtered native Domino effects remain')")
s=s.replace('Native source Domino acquisition, controller casting, scenes and effects only. No remote body, VFX, AI or gameplay outcome replication. Mandatory isolated checkpoint reload retires world effects; original assets and saves unchanged.','Initial native Domino observation: exact Emily inventory acquisition, one native LT hold/release, raw PhantasmalLink subscenes, state/equipment/target and filtered effect data. A recorded scene is not proof of successful linking, mana spending or target outcomes. No remote body/VFX or gameplay replication. Full isolated inventory/health and checkpoint recovery plus protected hashes are required.')
(P/'Observe-NativeDomino-v182.ps1').write_text(s,encoding='utf-8')
runner=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=runner.read_text(encoding='utf-8-sig')
s=s.replace("'MesmerizeContext','MesmerizeEmptyAirObserve'","'MesmerizeContext','DominoObserve','MesmerizeEmptyAirObserve'")
a="  'MesmerizeEmptyAirObserve'";assert s.count(a)==1
s=s.replace(a,"  'DominoObserve'{& \"$root/work/natural-moveset/Observe-NativeDomino-v182.ps1\" -Take $Take -Pitch $Pitch}\n"+a)
runner.write_text(s,encoding='utf-8')
