from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset';W=R/'work/movement-integration'
t=(P/'Observe-NativeSwarmWorld-v286.ps1').read_text()
t=t.replace("-or$frameLink.Count-ne1-or$frameLink[0].pointerType-ne'.?AVarkCpntReferenceFrame@@'",'')
old="       $frameObject=Q host '--object' $frameLink[0].word";assert old in t
t=t.replace(old,"""       $entityFrame=@($entityObject.words|Where-Object offset -eq '0x358')
       if($entityFrame.Count-ne1-or$entityFrame[0].pointerType-ne'.?AVarkCpntReferenceFrame@@'){throw 'Native entity frame cache does not resolve a reference-frame component'}
       $frameObject=Q host '--object' $entityFrame[0].word""")
old=" $checks.Add('At least20 strict snapshots validate all25 indexed native rats, reciprocal entities/frames/animations and stable generations')";assert old in t
t=t.replace(old,old+"""
 $startLive=($worldRows|Where-Object{$_.valid-and$_.present}|Select-Object -First 1).tick
 $sustained=@($worldRows|Where-Object{$_.tick-gt($startLive+500)})
 $good=@($sustained|Where-Object valid);$ratio=if($sustained.Count){$good.Count/[double]$sustained.Count}else{0}
 [ordered]@{postSpawnSamples=$sustained.Count;validSamples=$good.Count;validRatio=$ratio;ratCounts=@($good|Where-Object present|ForEach-Object{$_.agents.Count}|Sort-Object -Unique)}|ConvertTo-Json -Depth 5|Set-Content "$out/world-reader-continuity.json"
 if($sustained.Count-lt100-or$ratio-lt.8){throw 'Strict Swarm world coverage below80 percent after initial spawn'}
 $checks.Add('Strict world reader remains valid for at least80 percent of more than100 post-spawn samples, including native agent state changes')
""")
(P/'Observe-NativeSwarmWorld-v288.ps1').write_text(t)
p=W/'Invoke-NativeLabOperation.ps1';t=p.read_text();assert 'Observe-NativeSwarmWorld-v286.ps1' in t;p.write_text(t.replace('Observe-NativeSwarmWorld-v286.ps1','Observe-NativeSwarmWorld-v288.ps1'))
t=(W/'Archive-SwarmWorldProbe-v287.ps1').read_text().replace('v287','v288').replace('candidate-swarm-world-probe-v286','candidate-swarm-world-probe-v287').replace('Observe-NativeSwarmWorld-v286.ps1','Observe-NativeSwarmWorld-v288.ps1')
start=t.index("$identity.scope='");end=t.index("'\n",start)
t=t[:start]+"$identity.scope='Read-only Swarm probe adds native contextual frame-driver support: entity+358 supplies the registered reference-frame component; rat+8 may be zero only for state3 with frame+d8 pointing to frame+20. Direct driving requires rat+8=frame and frame+d8=rat. Native6b8fc0 restores the rat driver after attachment. Strict ownership, generation, pool indices and stable pose checks remain. v287 initial full-rat samples passed but219 later rows were refused because rat+8 is not a permanent entity-frame pointer. New tests cover contextual and foreign drivers. Sustained native verification pending. Live proxy, wire47 and bank50 unchanged; remote rat gameplay remains unfinished.'"+t[end+1:]
(W/'Archive-SwarmWorldProbe-v288.ps1').write_text(t)
print('Prepared sustained reader test and archive288.')
