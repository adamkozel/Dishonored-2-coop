from pathlib import Path
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration'
def new(p,s):
    assert not p.exists(),p
    p.write_text(s,encoding='utf-8')
s=(W/'Build-DoppelgangerHolstered-v495.ps1').read_text(encoding='utf-8-sig').replace('doppelganger-visual-', 'doppelganger-air-').replace('-v495.log','-v508.log').replace('Build495','Build508')
new(W/'Build-DoppelgangerAir-v508.ps1',s)
s=(W/'archive_doppelganger_holstered_v495.py').read_text(encoding='utf-8-sig')
s=s.replace("A = W / 'candidate-doppelganger-holstered-v495'", "A = W / 'candidate-doppelganger-air-v508'")
s=s.replace("previous = W / 'candidate-doppelganger-crouch-v490'", "previous = W / 'candidate-doppelganger-holstered-v495'")
s=s.replace("f'doppelganger-visual-{x}-v495.log'", "f'doppelganger-air-{x}-v508.log'").replace('protocol=53','protocol=54')
a=s.index("                scope='");b=s.index('\n                banks=',a)
s=s[:a]+"                scope='506 armed airborne release and507 holstered airborne entry confirm Jump/Fall with exact owned left-arm phases. Candidate508 admits those states through source and semantic54 validation and retains air locomotion underneath existing full-body casting. Bank57/331, clone4 and native FX assets unchanged. Runtime and visual verification pending.',"+s[b:]
a=s.index('for p in [Path(__file__)');b=s.index('\n    shutil.copy2(p, A / p.name)',a)
s=s[:a]+"for p in [Path(__file__), W / 'Build-DoppelgangerAir-v508.ps1', W/'prepare_doppelganger_air_build_v508.py', *logs]:"+s[b:]
a=s.index("s = (W / 'Deploy-DoppelgangerVisual-v427.ps1')")
s=s[:a]+'''s = (W / 'Deploy-DoppelgangerHolstered-v495.ps1').read_text(encoding='utf-8-sig')
s=s.replace('doppelganger-holstered-v495','doppelganger-air-v508').replace('DoppelgangerHolsteredDeploy495','DoppelgangerAirDeploy508')
(W/'Deploy-DoppelgangerAir-v508.ps1').write_text(s,encoding='utf-8')
print(json.dumps({k:identity[k] for k in ('dll','probe','runtimeVerified','scope')}))
'''
new(W/'archive_doppelganger_air_v508.py',s)
print('Prepared candidate508 build/archive with unchanged495 banks and assets.')
