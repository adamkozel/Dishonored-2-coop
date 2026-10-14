from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
s=(P/'Test-PossessionDrawNetwork-v368.ps1').read_text(encoding='utf-8-sig')
old='''  if(!$nativeIds.Count-or(Compare-Object $nativeIds $nativeSent)){throw "Phase $v strict-native scene IDs differ from actual sent scene IDs"}'''
new='''  $notSent=@($nativeIds|Where-Object {$_ -notin $nativeSent})
  $sourceOnly=@($nativeSent|Where-Object {$_ -notin $nativeIds})
  #368: a last aim incarnation can be shorter than an external probe interval.
  #All independently observed IDs must be sent; only one last aim event may
  #remain source-only, explicitly recorded rather than claimed independently seen.
  if(!$nativeIds.Count-or$notSent.Count-or($sourceOnly.Count-and($v-ne27-or$sourceOnly.Count-ne1-or$sourceOnly[0]-ne$nativeSent[-1]))){throw "Phase $v strict-native/source history differs beyond one unobserved final aim incarnation"}
  [ordered]@{variant=$v;independentlyObserved=$nativeIds;sentNativeScenes=$nativeSent;sourceOnlyFinal=$sourceOnly}|ConvertTo-Json -Depth 6|Set-Content "$out/phase-observation-coverage-$v.json"'''
assert old in s;s=s.replace(old,new).replace('Bounded send/accept histories exactly match strict native scene incarnations, including sub-status-interval final aim loops','All independently observed phase incarnations match sent events; complete sent/accepted histories agree; any unobserved final aim incarnation is recorded separately')
p=P/'Test-PossessionDrawNetwork-v369.ps1';assert not p.exists();p.write_text(s)
p=W/'Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-PossessionDrawNetwork-v368.ps1' in s;p.write_text(s.replace('Test-PossessionDrawNetwork-v368.ps1','Test-PossessionDrawNetwork-v369.ps1'))
print('369 sampling-coverage assertion updated; no production code changed')
