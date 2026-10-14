from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';P=R/'work/natural-moveset';L=R/'outputs/d2-coop-lab'
p=L/'src/swarm_rat_visual_fixture.h';s=p.read_text()
s=s.replace('// One cached cosmetic rat proves model/animation compatibility before a pool.','// Bounded cosmetic rat pool, one cached model per validated source pool index.')
s=s.replace('} swarmRatVisual;','};\nstd::array<SwarmRatVisualFixture,100> swarmRatVisualPool{};\ndouble swarmRatServiceMaxMs=0;')
s=s.replace('bool swarmRatVisualIdentity(){\n    const auto& p=swarmRatVisual;','bool swarmRatVisualIdentity(const SwarmRatVisualFixture& p){\n    ')
s=s.replace('bool swarmRatSetLiveTransform(){\n    auto& p=swarmRatVisual;','bool swarmRatSetLiveTransform(SwarmRatVisualFixture& p){\n    ')
s=s.replace('swarmRatVisualIdentity()','swarmRatVisualIdentity(p)').replace('swarmRatSetLiveTransform()','swarmRatSetLiveTransform(p)')
s=s.replace('void serviceSwarmRatVisual(ULONGLONG tick){','void serviceSwarmRatSlot(ULONGLONG tick,unsigned index,bool& mayCreate){')
s=s.replace('auto& p=swarmRatVisual;const auto& s=swarmWorldStream;','auto& p=swarmRatVisualPool[index];const auto& s=swarmWorldStream;')
s=s.replace('s.presented.agents[i].handle==p.sourceHandle&&s.presented.agents[i].incarnation==p.incarnation','s.presented.agents[i].index==index&&s.presented.agents[i].handle==p.sourceHandle&&s.presented.agents[i].incarnation==p.incarnation')
s=s.replace('}else if(!p.until&&!p.pendingHide)source=&s.presented.agents[0];','}else if(!p.until&&!p.pendingHide){for(unsigned i=0;i<s.presented.count;++i)if(s.presented.agents[i].index==index)source=&s.presented.agents[i];}')
s=s.replace('if(!p.target.actor){\n            const auto definition=', 'if(!p.target.actor){\n            if(!mayCreate)return;mayCreate=false; // Bound allocation cost to one new model per engine frame.\n            const auto definition=')
start=s.index('void writeSwarmRatVisual(){');s=s[:start]+r'''
void serviceSwarmRatVisual(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto began=presentationClock();bool mayCreate=true;
    for(unsigned i=0;i<swarmRatVisualPool.size();++i)serviceSwarmRatSlot(tick,i,mayCreate);
    swarmRatServiceMaxMs=(std::max)(swarmRatServiceMaxMs,presentationClock()-began);
}
void writeSwarmRatVisual(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.swarm-rat-visual.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    unsigned active=0,cached=0,hiding=0;float error=0;std::uint64_t verified=0;
    for(const auto& p:swarmRatVisualPool){if(p.target.actor)++cached;if(p.until&&!p.pendingHide&&p.playing)++active;if(p.pendingHide)++hiding;error=(std::max)(error,p.maxPositionError);verified+=p.verifiedTransforms;}
    // Keep first-slot diagnostic fields for the established closeup/skeleton harness.
    const auto& p=swarmRatVisualPool[0];
    fprintf(f,"{\"experiment\":true,\"maximumModels\":100,\"active\":%s,\"activeModels\":%u,\"cachedModels\":%u,\"pendingHide\":%s,\"hidingModels\":%u,\"serviceMaxMs\":%.4f,\"handle\":%u,\"sourceHandle\":%u,\"incarnation\":%u,\"scene\":0,\"playing\":%u,\"created\":%llu,\"started\":%llu,\"hidden\":%llu,\"updated\":%llu,\"refused\":%llu,\"retired\":%llu,\"event\":%llu,\"result\":\"%s\",\"sourceTransform\":[",active?"true":"false",active,cached,hiding?"true":"false",hiding,swarmRatServiceMaxMs,p.target.handle,p.sourceHandle,p.incarnation,p.playing,swarmRatCreated,swarmRatStarted,swarmRatHidden,swarmRatUpdated,swarmRatRefused,swarmRatRetired,p.event,swarmRatResult);
    for(unsigned i=0;i<12;++i)fprintf(f,"%s%.6f",i?",":"",p.sourceTransform[i]);
    fprintf(f,"],\"verifiedTransforms\":%llu,\"positionError\":%.7f,\"maxPositionError\":%.7f,\"actualTransform\":[",verified,p.positionError,error);
    for(unsigned i=0;i<12;++i)fprintf(f,"%s%.6f",i?",":"",p.actualTransform[i]);
    fputs("],\"models\":[",f);bool comma=false;
    for(unsigned i=0;i<swarmRatVisualPool.size();++i){const auto& a=swarmRatVisualPool[i];if(!a.target.actor)continue;
        fprintf(f,"%s{\"index\":%u,\"handle\":%u,\"sourceHandle\":%u,\"incarnation\":%u,\"active\":%s,\"pendingHide\":%s,\"verifiedTransforms\":%llu,\"maxPositionError\":%.7f,\"actualTransform\":[",comma?",":"",i,a.target.handle,a.sourceHandle,a.incarnation,a.until&&!a.pendingHide&&a.playing?"true":"false",a.pendingHide?"true":"false",a.verifiedTransforms,a.maxPositionError);comma=true;
        for(unsigned j=0;j<12;++j)fprintf(f,"%s%.6f",j?",":"",a.actualTransform[j]);fputs("]}",f);
    }
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
'''
p.write_text(s)
p=P/'Test-SwarmRatVisualNetwork-v296.ps1';s=p.read_text(encoding='utf-8-sig')
s=s.replace('($visual.created-$visualBaseline.created)-ne1','($visual.created-$visualBaseline.created)-ne25')
s=s.replace('($visual.started-$visualBaseline.started)-lt1','($visual.started-$visualBaseline.started)-lt25')
s=s.replace('if($rats.Count-eq1){','if($rats.Count-eq25){')
s=s.replace("if($rats.Count-ne1){throw 'Expected one cached cosmetic rat entity'}","if($rats.Count-ne25){throw 'Expected25 cached cosmetic rat entities'}")
s=s.replace(" $sourceWorld=S host '.swarm-world.json';$peerWorld=S client '.swarm-world.json'",r''' $fullPool=@($worldRows|Where-Object {$_.visual.activeModels-eq25-and$_.peer.count-eq25})
 if($fullPool.Count-lt20){throw 'Full25-rat pool did not remain active'}
 foreach($row in $fullPool){
  $models=@($row.visual.models|Where-Object active)
  if($models.Count-ne25-or@($models.handle|Sort-Object -Unique).Count-ne25){throw 'Cosmetic local handles not unique'}
  foreach($model in $models){
   $source=@($row.peer.agents|Where-Object {$_.index-eq$model.index-and$_.handle-eq$model.sourceHandle-and$_.incarnation-eq$model.incarnation})
   if($source.Count-ne1-or$model.maxPositionError-gt.001-or!$model.verifiedTransforms){throw 'Cosmetic slot source identity/readback mismatch'}
  }
 }
 [ordered]@{fullPoolSamples=$fullPool.Count;maximumActive=($worldRows.visual.activeModels|Measure-Object -Maximum).Maximum;created=$visual.created-$visualBaseline.created;serviceMaxMs=$visual.serviceMaxMs;fullGameplay=$false}|ConvertTo-Json|Set-Content "$out/pool-summary.json"
 $checks.Add('25 unique owned cosmetic models match25 opaque source indices/handles/incarnations for at least20 snapshots, all with verified live transforms')
 $sourceWorld=S host '.swarm-world.json';$peerWorld=S client '.swarm-world.json' ''')
s=s.replace('($hidden.hidden-$visualBaseline.hidden)-lt1','($hidden.hidden-$visualBaseline.hidden)-lt25')
s=s.replace('One owned cosmetic native rat model follows opaque peer rat identity with a native walking animleaf; no peer gameplay swarm','25 owned cosmetic native rat models follow opaque peer identities with native walking animleafs; no peer gameplay swarm')
s=s.replace('One cosmetic rat receives live transforms','A bounded cached cosmetic pool receives live transforms').replace('Full rat pool, source animation phase, speed, scale','Upgraded100-rat capacity, source animation phase, speed, scale')
(P/'Test-SwarmRatVisualNetwork-v298.ps1').write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-SwarmRatVisualNetwork-v296.ps1'in s;p.write_text(s.replace('Test-SwarmRatVisualNetwork-v296.ps1','Test-SwarmRatVisualNetwork-v298.ps1'))
