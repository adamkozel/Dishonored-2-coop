from pathlib import Path
R=Path(__file__).resolve().parents[2];N=R/'work/natural-moveset'
s=(N/'Test-NpcNativePerception-v985.ps1').read_text().replace('npc-native-perception-v985','npc-native-attack-v989').replace('candidate-npc-combat-v983','candidate-npc-combat-v988')
a=" $avatar=(S host).probeActorHandle;";assert a in s;s=s.replace(a,''' foreach($r in 'host','client'){C $r 'coop_npc_attack on'}
 $attackReady=[Diagnostics.Stopwatch]::StartNew();do{
  $both=$true;foreach($r in 'host','client'){$a=S $r '.npc-attack.json';if(!$a.enabled-or!$a.entries.Count-or@($a.entries|Where-Object {!$_.ready}).Count){$both=$false}}
  if(!$both){Start-Sleep -Milliseconds 50}
 }while(!$both-and$attackReady.ElapsedMilliseconds-lt8000)
 if(!$both){throw 'NPC sword attack handshake failed'}
 foreach($r in 'host','client'){Save "$out/attack-ready-$r.json" (S $r '.npc-attack.json')}
 $playerInitial=Q client '--health' 1;Save "$out/player-initial.json" $playerInitial
'''+a)
a="  if($row.avatarHealth.current-le0){break}";assert a in s;s=s.replace(a,'''  if($row.clientPlayerHealth.current-lt$playerInitial.current){
   C client 'setviewpos -33 -148 68.1 270 0'
   Save "$out/retreated-after-native-hit.json" @{atMs=$watch.ElapsedMilliseconds;health=$row.clientPlayerHealth.current}
   break
  }
  if($row.avatarHealth.current-lt40){throw 'Owned clone absorbed gameplay damage instead of routing it'}''')
a=' if($selected-lt1)';i=s.index(a);j=s.index('\n Save "$out/verified.json"',i)
s=s[:i]+''' $settle=[Diagnostics.Stopwatch]::StartNew();do{
  $hs=S host '.npc-attack.json';$cs=S client '.npc-attack.json'
  if($hs.event-and$hs.acked-eq$hs.event-and$cs.applied-eq$hs.event-and!$hs.pending){break};Start-Sleep -Milliseconds 80
 }while($settle.ElapsedMilliseconds-lt5000)
 $beforeHealth=Q client '--health' 1;$avatarAfter=Q host '--health' $avatar
 Save "$out/native-hit-result.json" @{host=$hs;client=$cs;player=$beforeHealth;avatar=$avatarAfter}
 foreach($r in 'host','client'){Save "$out/damage-final-$r.json" (S $r '.damage.json')}
 if(!$hs.enabled-or!$cs.enabled-or!$hs.event-or$hs.acked-ne$hs.event-or$cs.applied-ne$hs.event-or$hs.pending-or$beforeHealth.current-ge$playerInitial.current-or$beforeHealth.current-le0-or$avatarAfter.current-ne40){throw 'Native enemy sword contact, health ownership or acknowledgement failed'}
 C host 'coop_npc_attack duplicate';$dups=[Diagnostics.Stopwatch]::StartNew();do{$afterDup=S client '.npc-attack.json';if($afterDup.duplicates-gt$cs.duplicates){break};Start-Sleep -Milliseconds 80}while($dups.ElapsedMilliseconds-lt5000)
 $dupHealth=Q client '--health' 1;Save "$out/duplicate.json" @{attack=$afterDup;health=$dupHealth}
 if($afterDup.duplicates-le$cs.duplicates-or$afterDup.applied-ne$cs.applied-or$dupHealth.current-ne$beforeHealth.current){throw 'Duplicate native enemy hit was not idempotent'}
''' +s[j:]
s=s.replace("scope='Candidate983 owned continuous-transform observable retention. Native host NPC combat-target selection of remote Doppelganger with ordinary Player faction, while NPC root/body stream owns client AI. Attack scheduler observations are separate from landed damage and enemy-to-client health relay; those are not accepted by this test.'", "scope='Opt-in988: native bound-enemy sword contact to owned host clone routes through Steam to actual client player, native damage/health/reaction once, clone health40 retained, duplicate rejected. Not all damage types or full lifecycle/combat.'")
s=s.replace("'coop_avatar_perception off','coop_shadow_stream off'","'coop_npc_attack off','coop_avatar_perception off','coop_shadow_stream off'")
s=s.replace("scope='Bounded native enemy perception of remote player, no custom faction override or enemy damage relay.'", "scope='Bounded generic native enemy sword routing to actual remote player; restored isolated lab.'")
(N/'Test-NpcNativeAttack-v989.ps1').write_text(s)
