from pathlib import Path
R=Path(__file__).resolve().parents[2]
p=R/'work/natural-moveset/Test-SwarmContextNetwork-v276.ps1'
s=p.read_text(encoding='utf-8-sig')
s=s.replace("$rows=[Collections.Generic.List[object]]::new();", "$rows=[Collections.Generic.List[object]]::new();$worldRows=[Collections.Generic.List[object]]::new();")
s=s.replace("  Start-Sleep -Milliseconds 30\n", "  $worldRows.Add([pscustomobject]@{ms=$w.ElapsedMilliseconds;source=(S host '.swarm-world.json');peer=(S client '.swarm-world.json')})\n  Start-Sleep -Milliseconds 30\n")
anchor=" $scenes|ConvertTo-Json -Depth 16|Set-Content"
assert s.count(anchor)==1
insert=''' $worldRows|ConvertTo-Json -Depth 16|Set-Content "$out/world-states.json"
 $sourceWorld=S host '.swarm-world.json';$peerWorld=S client '.swarm-world.json'
 [ordered]@{source=$sourceWorld;peer=$peerWorld}|ConvertTo-Json -Depth 12|Set-Content "$out/world-history.json"
 $live=@($worldRows|Where-Object {$_.peer.leased-and$_.peer.present-eq1-and$_.peer.count-gt0})
 if($live.Count-lt20-or$sourceWorld.sent-lt20-or$peerWorld.presented-lt20){throw 'Insufficient live native Steam rat snapshots'}
 $matched=@(foreach($r in $peerWorld.history){$sent=@($sourceWorld.history|Where-Object sequence -eq $r.sequence);if(!$sent.Count){continue};if($sent.Count-ne1-or$sent[0].digest-ne$r.digest-or$sent[0].count-ne$r.count-or$sent[0].handle-ne$r.handle){throw 'Accepted rat wire differs from source'};$r})
 if($matched.Count-lt20){throw 'Insufficient overlapping exact source/accepted rat packets'}
 $checks.Add('More than20 accepted Steam world snapshots exactly match source wire digests, native rat identities and counts')
 if(@($live|Where-Object {$_.peer.bytes-ne(124+72*$_.peer.count)}).Count){throw 'Rat wire length/count mismatch'}
 [ordered]@{liveSamples=$live.Count;matchedPackets=$matched.Count;counts=@($live.peer.count|Sort-Object -Unique);rejected=$peerWorld.rejected;unknown=$sourceWorld.unknown;scopedReadMaxMs=$sourceWorld.scopedReadMaxMs;remoteVisuals=$false;remoteDamage=$false}|ConvertTo-Json|Set-Content "$out/world-summary.json"
 $checks.Add('World samples preserve bounded native agent sets and exact variable payload size; source reader timing retained')
 C host 'coop_shadow_stream off';Start-Sleep -Milliseconds 1200
 $stopped=S client '.swarm-world.json';$stopped|ConvertTo-Json -Depth 12|Set-Content "$out/world-after-source-stop.json"
 if($stopped.leased){throw 'Remote rat lease survives source stream retirement'}
 $checks.Add('Remote rat lease retires when source Steam semantic stream stops')
'''
s=s.replace(anchor,insert+anchor)
s=s.replace("  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Swarm entities')", "  $afterWorld=S client '.swarm-world.json';$afterWorld|ConvertTo-Json -Depth 12|Set-Content \"$out/world-after-checkpoint.json\";if($afterWorld.leased-or$afterWorld.count){throw 'Rat world presentation survives checkpoint'}\n  $checks.Add('Both isolated checkpoints restore original health/inventory and retire spawned Swarm entities and peer world presentation')")
s=s.replace("Remote world/rat behavior, aerial/slide contexts and visual acceptance remain pending.","Strict source rat transforms also travel over native Steam with exact wire comparisons and stream/checkpoint retirement. Remote rat visuals, damage/AI, aerial/slide contexts and visual acceptance remain pending.")
out=R/'work/natural-moveset/Test-SwarmWorldNetwork-v290.ps1';assert not out.exists();out.write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig')
s=s.replace("'SwarmWorldObserve',","'SwarmWorldNetwork','SwarmWorldObserve',")
line="  'SwarmWorldObserve'{"
assert s.count(line)==1
s=s.replace(line,"  'SwarmWorldNetwork'{& \"$root/work/natural-moveset/Test-SwarmWorldNetwork-v290.ps1\" -Take $Take -Pitch $Pitch -Stance $Stance -Holstered:$Holstered -Move $Move}\n"+line)
p.write_text(s,encoding='utf-8')
print('Created serialized Swarm world network proof')
