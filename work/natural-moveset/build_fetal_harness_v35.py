from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'Record-HighThrow-Open-v33.ps1').read_text(encoding='utf-8-sig')
s=s.replace('high-throw-bank-manifest-v31.json','fetal-bank-manifest-v35.json')
s=s.replace('This experiment requires standing paired high throw','This experiment requires standing paired contextual release')
s=s.replace('Source native pickup, carry walk and high throw completed','Source native pickup, carry walk and release completed; variant checked separately')
s=s.replace('nativeThrow=$true','nativeThrow=$false;nativeFetalExperiment=$true')
s=s.replace('source-clock remote release queues a single native event149 after physics handoff','native fallback variant5 uses a separate compact victim scene and bounded physics handoff; high-throw impulse is forbidden')
(P/'Record-FetalRelease-v35.ps1').write_text(s,encoding='utf-8')
