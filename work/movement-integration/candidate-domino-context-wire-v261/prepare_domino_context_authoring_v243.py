"""Prepare offline stance authoring; does not enable any runtime context."""
from pathlib import Path
R=Path(__file__).resolve().parents[2];P=R/'work/natural-moveset'
s=(P/'build_mesmerize_contexts_v132.py').read_text(encoding='utf-8-sig')
s=s.replace('Mesmerize','Domino').replace('mesmerize-bank-source-manifest-v77.json','domino-bank-source-manifest-v192.json').replace('mesmerize-contexts-v132','domino-contexts-v243').replace('mesmerize-context-banks-v132','domino-context-banks-v243').replace("P/'mesmerize-banks-v77'/f'{role}-mesmerize-bank.bin'","P/'domino-banks-v192'/f'{role}-domino-bank.bin'")
s=s.replace("b'D2MOVE45'and version==45 and count==276", "b'D2MOVE47'and version==47 and count==289").replace("b'D2MOVE46',46,n,285", "b'D2MOVE48',48,n,301")
s=s.replace("'native_mesmerize_in'","'native_domino_entry'").replace("[('in',466),('loop',1200),('out',666)]", "[('entry',266),('aim',1333),('select',400),('exit',266)]")
s=s.replace("'native_mesmerize_'+phase", "'native_domino_'+phase").replace('cid=276+phaseIndex*3+context','cid=289+phaseIndex*3+context')
s=s.replace("phase=='in'", "phase=='entry'").replace("phase=='out'", "phase=='exit'")
s=s.replace('native_mesmerize_', 'native_domino_').replace('variant=16+phaseIndex','variant=19+phaseIndex')
s=s.replace('v77 donor', 'v192 whole-body donor').replace('v77 standing payload','v192 standing payload')
s=s.replace("f'{role}-mesmerize-context-bank.bin'", "f'{role}-domino-context-bank.bin'").replace("format='D2MOVE46',clips=285", "format='D2MOVE48',clips=301")
s=s.replace('mesmerize-context-source-manifest-v132.json','domino-context-source-manifest-v243.json').replace('mesmerize-context-bank-manifest-v132.json','domino-context-bank-manifest-v243.json').replace('MESMERIZE_CONTEXTS_GENERATED','DOMINO_CONTEXTS_GENERATED')
(P/'build_domino_contexts_v243.py').write_text(s,encoding='utf-8')
