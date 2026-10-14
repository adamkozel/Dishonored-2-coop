from pathlib import Path

R = Path(__file__).resolve().parents[2]
p = R / 'work/natural-moveset/Test-NativeDoorSync-v718.ps1'
s = p.read_text(encoding='utf-8-sig')
def replace(old, new):
    global s
    assert old in s, old
    s = s.replace(old, new)

replace("[ValidateSet('finish','breakout')][string]$Outcome='breakout'", "[ValidateSet('ld_door_350cm_int_rich_wood_01_3','ld_door_350cm_int_rich_wood_01_2')][string]$DoorName='ld_door_350cm_int_rich_wood_01_3'")
replace('native-door-sync-corvo-v718', 'native-door-catalog-corvo-v726')
replace('NativeDoorSync718', 'NativeDoorCatalog726')
replace('candidate-door-sync-v716', 'candidate-door-catalog-v725')
replace('Expected archived716 candidate', 'Expected archived725 candidate')
replace("function DS($r){Get-Content \"$root/work/lab-instances/$r/coop-status.json.door-sync.json\" -Raw|ConvertFrom-Json}", """function DAll($r){Get-Content "$root/work/lab-instances/$r/coop-status.json.door-sync.json" -Raw|ConvertFrom-Json}
function DS($r){$d=@((DAll $r).entries|Where-Object name -eq $DoorName);if($d.Count-ne1){throw 'Selected door channel absent or ambiguous'};return $d[0]}
function CatalogReady($d){$d.enabled-and$d.catalogCount-eq6-and$d.ambiguous-eq0-and$d.ineligible-eq4-and@($d.entries|Where-Object {!$_.enabled-or!$_.ready-or$_.revision-ne1-or$_.applied-ne1}).Count-eq0}
$initialOpen=$DoorName-eq'ld_door_350cm_int_rich_wood_01_2'
$expectedCycle=if($initialOpen){'2,1,0,1,2'}else{'0,1,2,1,0'}""")
replace("   C $peer 'setviewpos -44 -161 68.1 220 0'\n   C $source 'setviewpos -47.05 -163.5 68.1 270 0'", """   if($initialOpen){
    C $peer 'setviewpos -48 -158.5 68.1 215 0'
    C $source 'setviewpos -50.3 -160.75 68.1 180 0'
   }else{
    C $peer 'setviewpos -44 -161 68.1 220 0'
    C $source 'setviewpos -47.05 -163.5 68.1 270 0'
   }""")
replace("Where-Object name -eq 'ld_door_350cm_int_rich_wood_01_3'", 'Where-Object name -eq $DoorName')
replace("   $dw=[Diagnostics.Stopwatch]::StartNew();do{$dh=DS host;$dc=DS client;if($dh.ready-and$dc.ready-and$dh.revision-eq1-and$dc.revision-eq1-and$dh.applied-eq1-and$dc.applied-eq1){break};Start-Sleep -Milliseconds 40}while($dw.ElapsedMilliseconds-lt6000)\n   if(!$dh.ready-or!$dc.ready-or$dh.revision-ne1-or$dc.revision-ne1){throw 'Door sync pair not armed'}", """   $dw=[Diagnostics.Stopwatch]::StartNew();do{$dh=DAll host;$dc=DAll client;if((CatalogReady $dh)-and(CatalogReady $dc)){break};Start-Sleep -Milliseconds 40}while($dw.ElapsedMilliseconds-lt6000)
   $catalogBefore=@{host=$dh;client=$dc};$catalogBefore|ConvertTo-Json -Depth 9|Set-Content "$out/catalog-before.json"
   if(!(CatalogReady $dh)-or!(CatalogReady $dc)){throw 'Six-door catalog not armed'}
   foreach($r in 'host','client'){if((DoorSample $r).current-ne$(if($initialOpen){2}else{0})){throw 'Initial door state differs'}}""")
replace("input='native open'", "input=$(if($initialOpen){'native close'}else{'native open'})")
replace("input='native close';attempt=2", "input=$(if($initialOpen){'native open'}else{'native close'});attempt=2")
replace("-ne'0,1,2,1,0'", '-ne$expectedCycle')
replace("   $checks.Add('Two ordinary native X interactions", """   $catalogAfter=@{host=(DAll host);client=(DAll client)};$catalogAfter|ConvertTo-Json -Depth 9|Set-Content "$out/catalog-after.json"
   foreach($r in 'host','client'){
    $before=$catalogBefore[$r];$after=$catalogAfter[$r]
    if(!$after.enabled-or$after.catalogCount-ne6-or$after.rejected-ne$before.rejected-or$after.unmatched-ne$before.unmatched){throw 'Catalog scope or packet rejection changed'}
    foreach($old in $before.entries){
     $entry=@($after.entries|Where-Object name -eq $old.name)
     if($entry.Count-ne1-or!$entry[0].enabled-or!$entry[0].ready){throw 'Catalog door retired during ordinary interaction'}
     if($old.name-ne$DoorName-and($entry[0].revision-ne1-or$entry[0].applied-ne1-or$entry[0].target-ne$old.target-or$entry[0].nativeCalls-ne0-or$entry[0].rejected-ne$old.rejected)){throw 'Unrelated door changed during selected interaction'}
    }
   }
   $checks.Add('Six eligible authored doors armed; four unsafe doors excluded; five unrelated channels retain initial targets/revisions without native mutations')
   $checks.Add('Two ordinary native X interactions""")
replace('both0->1->2->1->0 door cycles', 'both selected door cycles')
replace("Native Steam sync of one ordinary unlinked unlocked Dunwall door. Source uses real native X inputs; receiver uses74a400 queue. Both engine state cycles and host revision/duplicate counters verified; locks/scripts/collision/campaign-wide doors remain unverified.", "Native Steam authored ordinary-door catalog: six eligible channels, four excluded doors, selected open/close cycle via native X, other five unchanged. Locks/scripts/other maps remain unverified.")
replace('cases=$rows;scope=', 'cases=$rows;door=$DoorName;initialOpen=$initialOpen;scope=')
(R / 'work/natural-moveset/Test-DoorCatalog-v726.ps1').write_text(s, encoding='utf-8')
print('Created catalog test harness726')
