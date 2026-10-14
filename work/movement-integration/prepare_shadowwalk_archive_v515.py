from pathlib import Path
W=Path(__file__).resolve().parent
s=(W/'archive_doppelganger_air_v512.py').read_text(encoding='utf-8-sig')
s=s.replace("A = W / 'candidate-doppelganger-air-v512'", "A = W / 'candidate-shadowwalk-inventory-v515'")
s=s.replace("previous = W / 'candidate-doppelganger-air-v508'", "previous = W / 'candidate-doppelganger-air-v512'")
s=s.replace("f'doppelganger-air-{x}-v512.log'", "f'shadowwalk-inventory-{x}-v515.log'")
a=s.index('                scope=');b=s.index('\n                banks=',a)
s=s[:a]+"                scope='Exact Shadow Walk identity reader/probe and opt-in reversible native inventory fixture. Static514 RTTI/assets plus baseline510 inventory; normal controller input only. Remote Shadow Walk integration is not implemented. Retains512 airborne candidate and bounded diagnostic; intermittent509 landing dropout unresolved despite passing513/510 repetitions.',"+s[b:]
a=s.index('for p in [Path(__file__)');b=s.index('\n    shutil.copy2(p, A / p.name)',a)
s=s[:a]+"for p in [Path(__file__), W / 'Build-ShadowWalkInventory-v515.ps1', W/'prepare_shadowwalk_inventory_v515.py', *W.glob('native-shadowwalk-*-v514.*'), *logs]:"+s[b:]
a=s.index("s = (W / 'Deploy-DoppelgangerHolstered-v495.ps1')")
s=s[:a]+'''s = (W/'Deploy-DoppelgangerAir-v512.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-air-v512','shadowwalk-inventory-v515').replace('DoppelgangerAirDeploy512','ShadowWalkInventoryDeploy515')
(W/'Deploy-ShadowWalkInventory-v515.ps1').write_text(s,encoding='utf-8')
print(json.dumps({k:identity[k] for k in ('dll','probe','runtimeVerified','scope')}))
'''
p=W/'archive_shadowwalk_inventory_v515.py';assert not p.exists();p.write_text(s,encoding='utf-8')
