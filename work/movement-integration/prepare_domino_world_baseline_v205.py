from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'Test-DominoWorld-v203.ps1').read_text(encoding='utf-8-sig')
needle=" $checks.Add('Native Steam transport and exact bank47/289 remote Emily standing armed context ready')"
assert needle in s
s=s.replace(needle,needle+'''
 Start-Sleep -Milliseconds 350
 $worldBaseline=S host '.domino-world.json';$worldBaseline|ConvertTo-Json -Depth 8|Set-Content "$out/world-before-capture.json"
''',1)
old='$peerWorld.refused-or$peerWorld.rejected-or$peerWorld.updated-lt15';assert old in s
s=s.replace(old,'$peerWorld.refused-or$peerWorld.rejected-ne$worldBaseline.rejected-or$peerWorld.updated-lt15',1)
(P/'Test-DominoWorld-v205.ps1').write_text(s,encoding='utf-8')
p=R/'work/movement-integration/Invoke-NativeLabOperation.ps1';s=p.read_text(encoding='utf-8-sig');assert 'Test-DominoWorld-v203.ps1' in s
p.write_text(s.replace('Test-DominoWorld-v203.ps1','Test-DominoWorld-v205.ps1'),encoding='utf-8')
p=R/'work/movement-integration/record_domino_network_v196.py';s=p.read_text(encoding='utf-8-sig')
s=s.replace('Index inspected native/remote Domino body captures without VFX claims.','Index inspected native/remote Domino cosmetic mark captures without gameplay claims.')
s=s.replace("alias='domino_native_body_network'","alias='domino_native_mark_network'").replace("'Steam body presentation'","'Steam body and mark presentation'")
s=s.replace("native_domino_standing_armed_body_partial_verified","native_domino_standing_armed_body_and_mark_partial_verified")
old="Native Emily PhantasmalLink state72 and exact entry/aim/select/exit scene clocks drive the remote full body through native Steam. One native cast changes subscene; semantic events identify individual phases. No native power or damage method is invoked on the receiver. Latest inspected passing take "
assert old in s;s=s.replace(old,"Native Emily phases and strictly owned target marks traverse native Steam. Receiver recreates validated native FX on owned presentation objects, with no native Domino/NPC/damage calls. Latest inspected passing take ")
s=s.replace(' Remote target marks/links, partner exclusion, shared NPC outcomes, additional stances/interruptions and visual acceptance remain incomplete.',' Receiver NPC attachment alignment, multi-target links, partner exclusion, shared NPC outcomes, additional stances/interruptions and visual acceptance remain incomplete.')
(R/'work/movement-integration/record_domino_world_v205.py').write_text(s,encoding='utf-8')
print('Ready-state rejection baseline and cosmetic-world indexer prepared')
