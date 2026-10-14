from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset';p=N/'Test-NpcPerception-v974.ps1';assert not p.exists()
s=(N/'Test-NpcRecovery-v848.ps1').read_text(encoding='utf-8-sig').replace("'npc-recovery-v848'","'npc-perception-v974'").replace('candidate-npc-stream-v847','candidate-npc-combat-v973')
s=s.replace('function Ready($tag,[int]$count=5){','function Ready($tag,[int]$count=0){').replace('$v.entries.Count-ne$count','($count-and$v.entries.Count-ne$count)-or!$v.entries.Count')
s=s.replace("$initial=Ready 'initial'","$initial=Ready 'initial';$initialCount=$initial.host.entries.Count")
s=s.replace('$v.entries.Count-ne5','$v.entries.Count-ne$initialCount').replace('(10+2*$reloadCount)','(2*$initialCount+2*$reloadCount)').replace('$v.session.released-ne10','$v.session.released-ne(2*$initialCount)')
insert=s.index('try{\n try{$owned=')
s=s[:insert]+'''function P($role='host'){Get-Content "$root/work/lab-instances/$role/coop-status.json.npc-perception.json" -Raw|ConvertFrom-Json}
function Factions($tag){
 $map=@{};$state=N host;$partner=(S host).probeActorHandle
 foreach($e in $state.entries){if($e.enabled){$map[[string]$e.handle]=Q host '--faction' $e.handle}}
 $map['player']=Q host '--faction' 1;$map['partner']=Q host '--faction' $partner
 Save "$out/factions-$tag.json" $map;return $map
}
function PerceptionReady($tag){
 $watch=[Diagnostics.Stopwatch]::StartNew();do{$p=P;if($p.enabled-and!$p.suspended-and$p.entries.Count-and$p.result-eq'active_host_perception'){break};Start-Sleep -Milliseconds 80}while($watch.ElapsedMilliseconds-lt6000)
 Save "$out/perception-$tag.json" $p;if(!$p.enabled-or$p.suspended-or!$p.entries.Count-or$p.lost-or$p.superseded){throw "Perception not ready $tag"};return $p
}
function AssertRelations($before,$after,$owned,[bool]$active){
 foreach($key in $before.Keys){
  $a=$before[$key];$b=$after[$key];if(!$b-or$a.actor-ne$b.actor-or$a.handle-ne$b.handle){throw "Faction identity changed $key"}
  foreach($r in $a.relations){
   $now=@($b.relations|Where-Object handle -eq $r.handle);if($now.Count-ne1){throw 'Faction relation membership changed'}
   $changed=$key-notin@('player','partner')-and@($owned.entries|Where-Object {[string]$_.npc-eq$key-and$_.partner-eq$r.handle}).Count-eq1
   if($active-and$changed){if(!$now[0].overridden-or$now[0].effective-ne2-or$now[0].overrideValue-ne2-or$now[0].base-ne$r.base){throw 'Owned hostility missing or faction base changed'}}
   elseif(($now[0]|ConvertTo-Json -Compress)-ne($r|ConvertTo-Json -Compress)){throw "Unowned/native relation changed $key -> $($r.handle)"}
  }
 }
}
'''+s[insert:]
a=" $index=0\n"
assert s.count(a)==1
s=s.replace(a,''' $factionBefore=Factions 'baseline';$clientBefore=P client
 C client 'coop_npc_perception on';Start-Sleep -Milliseconds 700;$denied=P client;Save "$out/client-refused.json" $denied
 if($denied.enabled-or$denied.entries.Count-or$denied.acquired-or$denied.refused-ne$clientBefore.refused+1){throw 'Client perception mutation was not refused'}
 C host 'coop_npc_perception on';$ownedRelations=PerceptionReady 'initial';$factionActive=Factions 'active';AssertRelations $factionBefore $factionActive $ownedRelations $true
 C host 'coop_npc_perception on';Start-Sleep -Milliseconds 650;$duplicate=P;Save "$out/perception-duplicate.json" $duplicate
 if($duplicate.acquired-ne$ownedRelations.acquired-or$duplicate.refused-ne$ownedRelations.refused+1-or$duplicate.entries.Count-ne$ownedRelations.entries.Count){throw 'Duplicate perception acquired extra overrides'}
 $index=0
''',1)
a='  Save "$out/menu-$source-held.json" $pair;I $source A 180;$resumed=Ready "resumed-$source"'
assert s.count(a)==1
s=s.replace(a,'''  Save "$out/menu-$source-held.json" $pair
  $held=P;Save "$out/perception-menu-$source.json" $held
  if(!$held.enabled-or!$held.suspended-or$held.entries.Count-or$held.acquired-ne$held.released-or$held.lost){throw 'Menu did not release all owned faction overrides'}
  $fm=Factions "menu-$source";AssertRelations $factionBefore $fm $ownedRelations $false
  I $source A 180;$resumed=Ready "resumed-$source";$ownedRelations=PerceptionReady "resumed-$source"
  $fr=Factions "resumed-$source";AssertRelations $factionBefore $fr $ownedRelations $true''',1)
a=' $freshCatalog=Q \'host\' \'--npc-authority\';'
assert s.count(a)==1
s=s.replace(a,''' $retired=P;Save "$out/perception-after-checkpoint.json" $retired
 if($retired.enabled-or$retired.entries.Count-or$retired.acquired-ne$retired.released-or$retired.lost){throw 'Old perception ownership survived checkpoint'}
 $freshCatalog=Q 'host' '--npc-authority';''',1)
a='}catch{$failure=$_.Exception.Message;Save "$out/failure.json"'
assert s.count(a)==1
s=s.replace(a,''' $freshBaseline=Factions 'fresh-baseline';C host 'coop_npc_perception on';$freshPerception=PerceptionReady 'fresh';$freshActive=Factions 'fresh-active';AssertRelations $freshBaseline $freshActive $freshPerception $true
 C host 'coop_npc_perception off';Start-Sleep -Milliseconds 700;$released=P;Save "$out/perception-explicit-off.json" $released
 if($released.enabled-or$released.entries.Count-or$released.acquired-ne$released.released-or$released.lost){throw 'Explicit perception cleanup failed'}
 $afterOff=Factions 'explicit-off';AssertRelations $freshBaseline $afterOff $freshPerception $false
 Save "$out/verified.json" @{passed=$true;initialNpcCount=$initialCount;initialEnemyCount=$ownedRelations.entries.Count;freshNpcCount=$reloadCount;freshEnemyCount=$freshPerception.entries.Count;acquired=$released.acquired;released=$released.released;lost=$released.lost;scope='Independent native directional relation reads across entire bound roster, unrelated/player/partner relationships unchanged; client/duplicate refused, host/client native menus release/reacquire, shared reload retires old ownership, fresh rearm and explicit off restore exact base relations. Active target choice and attack outcomes not yet proved.'}
}catch{$failure=$_.Exception.Message;Save "$out/failure.json"''',1)
s=s.replace("try{C $r 'coop_npc_stream off';", "try{C $r 'coop_npc_perception off';C $r 'coop_npc_stream off';")
s=s.replace("$v=N $r;Save \"$out/final-stream-$r.json\" $v;", "$p=P $r;Save \"$out/final-perception-$r.json\" $p;if($p.enabled-or$p.entries.Count-or$p.acquired-ne$p.released-or$p.lost){throw 'Perception overrides not balanced'};$v=N $r;Save \"$out/final-stream-$r.json\" $v;")
p.write_text(s,encoding='utf-8');print(p)
