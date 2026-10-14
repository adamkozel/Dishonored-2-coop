from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoNetwork-v196.ps1').read_text(encoding='utf-8-sig')
needle=' Start-Sleep -Milliseconds 750';assert needle in s
s=s.replace(needle,''' $observerYaw=[Math]::Atan2([double]$target.position[1]+153.27,[double]$target.position[0]+44.39)*180/[Math]::PI
 C host ('setviewpos -44.39 -153.27 68.1 {0} 0' -f $observerYaw.ToString('F4',[Globalization.CultureInfo]::InvariantCulture))
 & "$PSScriptRoot/Set-ObserverPitch.ps1" -Role host -Pitch 3 -Evidence "$out/mark-observer-camera.json" -Quick
'''+needle,1)
s=s.replace('movement=$m;stream=$stream;effects=$fx','movement=$m;stream=$stream;effects=$fx;dominoSource=(S client \'.domino-world.json\');dominoPeer=(S host \'.domino-world.json\')',1)
start=s.index(" if(@((Q host '--domino-effects').effects).Count)")
end=s.index(" C client 'coop_domino_fixture off'",start)
s=s[:start]+''' $peerFx=Q host '--domino-effects';$peerFx|ConvertTo-Json -Depth 8|Set-Content "$out/peer-effects.json"
 $sourceTargets=Q client '--domino-targets';$sourceTargets|ConvertTo-Json -Depth 8|Set-Content "$out/source-targets.json"
 $sourceWorld=S client '.domino-world.json';$peerWorld=S host '.domino-world.json'
 [ordered]@{source=$sourceWorld;peer=$peerWorld}|ConvertTo-Json -Depth 8|Set-Content "$out/world-status.json"
 if(!$sourceTargets.valid-or!$sourceTargets.present-or$sourceTargets.targets.Count-ne1-or!$sourceTargets.targets[0].hasMark){throw 'Exact native single-target mark unavailable'}
 if($peerWorld.marks.Count-ne1-or$peerWorld.started-ne1-or$peerWorld.refused-or$peerWorld.rejected-or$peerWorld.updated-lt15-or!$peerWorld.leased){throw 'Owned remote mark stream did not remain stable'}
 $sourceMark=$sourceTargets.targets[0];$peerMark=$peerWorld.marks[0]
 if($peerMark.sourceId-ne$sourceMark.mark-or$peerMark.target-ne$sourceMark.handle-or$peerMark.owner-ne(S host).probeActorHandle){throw 'Remote mark incarnation or local presentation ownership differs'}
 $matches=@($peerFx.effects|Where-Object {$_.id-eq$peerMark.id-and$_.owner-eq$peerMark.owner-and$_.name-eq'models/effects/gameplay/powers/phantasmal_link/target/phlink_validated_01.fx'})
 if($matches.Count-ne1-or$peerFx.effects.Count-ne1){throw 'Expected one local native cosmetic mark without duplicates'}
 $delta=0;for($k=0;$k-lt3;$k++){$delta+=[Math]::Pow([double]$matches[0].origin[$k]-[double]$sourceMark.effectPosition[$k],2)}
 if($delta-gt.25){throw 'Receiver mark is not near observed source target position'}
 $checks.Add('Native validated mark ID and target incarnation traverse Steam; exactly one receiver-owned native effect tracks the observed world position without duplicate target or power methods')
 $checks.Add('Body phases recover and native sword/player/avatar health stay unchanged; target positions remain source-world cosmetic coordinates, not proof of synchronized NPC AI')
 C host 'coop_movement_bank off';Start-Sleep -Milliseconds 700
 $stopped=S host '.domino-world.json';$stopped|ConvertTo-Json -Depth 8|Set-Content "$out/mark-stopped.json"
 if($stopped.marks.Count-or$stopped.stopped-ne1-or@((Q host '--domino-effects').effects).Count){throw 'Receiver-owned mark did not retire with presentation'}
 $checks.Add('Disabling receiver body presentation retires its owned mark; native source mark remains source-owned until checkpoint cleanup')
'''+s[end:]
s=s.replace('Remote target marks/links, AI and shared outcomes remain pending; receiver plays only body poses.','Remote validated target mark is sourced by exact native ownership and recreated as a bounded cosmetic FX; target handles are opaque and no link, NPC or damage method runs on the receiver. Source-world position is preserved; corresponding NPC AI/pose and link gameplay remain unsynchronized.')
s=s.replace('No claim of complete Domino support or visual acceptance.','No claim of complete Domino support, NPC attachment accuracy or visual acceptance.')
(P/'Test-DominoWorld-v203.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');s=s.replace("'DominoNetworkObserve','DominoNetwork'","'DominoNetworkObserve','DominoWorld','DominoNetwork'")
needle="  'DominoNetworkObserve'{";assert needle in s
s=s.replace(needle,"  'DominoWorld'{& \"$root/work/natural-moveset/Test-DominoWorld-v203.ps1\" -Take $Take -Pitch $Pitch}\n"+needle,1);p.write_text(s,encoding='utf-8')
print('Domino remote mark capture and cleanup harness ready')
