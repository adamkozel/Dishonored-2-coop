"""Reproducible whole-body Windblast context author derived from stance tooling."""
from pathlib import Path
P=Path(__file__).resolve().parent
s=(P/'build_darkvision_contexts_v1.py').read_text(encoding='utf-8-sig')
for a,b in [('darkvision','windblast'),('dark_vision','windblast'),('D2MOVE25','D2MOVE27'),('D2MOVE26','D2MOVE28'),('count==194','count==198'),('26,n,197','28,n,201'),('clips=197','clips=201'),('(194,59','(198,59'),('(195,17','(199,17'),('(196,76','(200,76'),('0.,.933','0.,1.0'),('f/56','f/60'),('range(57)','range(61)'),('duration_ms=933','duration_ms=1000'),('Dark Vision','Windblast')]:s=s.replace(a,b)
s=s.replace('native source owns world movement and private effects.','native source owns world movement and gameplay outcomes. Corvo gameplay availability only; Emily is an offline comparison.')
s=s.replace("shifts=[cg[i].translation-rg[i].translation for i in (21,25)];errors=[]", "shifts=[cg[i].translation-rg[i].translation for i in (21,25)];errors=[]\n  if not crouched:\n   delta={};shifts=[Vector((0,0,0)),Vector((0,0,0))]")
s=s.replace("p[2].translation+=rest[2].translation-reference[2].translation", "if crouched:p[2].translation+=rest[2].translation-reference[2].translation")
s=s.replace("[cg[i].to_3x3()@rg[i].to_3x3().inverted()@g[i].to_3x3()for i in (21,25)]", "[cg[i].to_3x3()@rg[i].to_3x3().inverted()@g[i].to_3x3() if crouched else g[i].to_3x3() for i in (21,25)]")
s=s.replace("clips.append(dict(id=cid", "assert max(errors)<.002, (role,cid,'Unreachable foot trajectory',max(errors))\n  clips.append(dict(id=cid")
(P/'build_windblast_contexts_v1.py').write_text(s,encoding='utf-8')
