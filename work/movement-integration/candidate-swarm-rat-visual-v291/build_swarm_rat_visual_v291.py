"""Animation-only native rat experiment, no SwarmAgent/AI/possession components."""
from pathlib import Path
import copy,json,hashlib,struct
from inspect_midnight_events import parse,walk
P=Path(__file__).resolve().parent;LAB=P.parents[1]/'outputs/d2-coop-lab'
read=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
def emit(o,depth=0):
 indent='\t'*depth
 if isinstance(o,dict):return '{\n'+''.join(indent+'\t'+k+' = '+emit(v,depth+1)+(';'if not isinstance(v,dict)else'')+'\n'for k,v in o.items())+indent+'}'
 if isinstance(o,str):return o if o in ('true','false','NULL')else json.dumps(o)
 return str(o)
manifest=[]
resources=read(LAB/'research/asset-exports/native-rat-visual-v291/manifest.json')['Resources']
for phase,alias in [('idle','rat_idle01'),('walk','rat_walk'),('run','rat_run')]:
 source=next(x for x in resources if x['Name'].endswith('/'+alias+'.md6anim'))
 raw=Path(source['ExportFile']).read_bytes();assert raw[:4]==b'80AE'
 frames=struct.unpack_from('<H',raw,16)[0];fps=struct.unpack_from('<f',raw,8)[0];duration=round((frames-1)/fps*1000)
 assert 100<=duration<=20000 and 1<=fps<=240
 d=copy.deepcopy(parse((LAB/'assets/mesmerize_visual_loop.midnightscene').read_text()))
 group=d['edit']['m_groupsInfo']['item[0]'];group['m_name']='Rat';group['m_declsMD6']['item[0]']='models/characters/animals/rat/rat.md6'
 for o in walk(d):
  if 'm_AnimName'in o:o['m_AnimName']=alias
  if 'm_duration'in o:o['m_duration']=duration
  if 'm_length'in o:o['m_length']=duration
 assert {o['m_keyProp']['m_class']for o in walk(d)if'm_keyProp'in o}=={'arkMidnightKeyPropAnim'}
 dest=LAB/f'assets/swarm_rat_{phase}.midnightscene';text=emit(d)+'\n';assert parse(text)==d;dest.write_text(text)
 manifest.append(dict(resource=f'midnight/d2coop/swarm_rat_{phase}.midnightscene',generated=str(dest),kind='midnightscene',classname='midnightScene',alias=alias,frames=frames,fps=fps,durationMs=duration,source=source['Name'],sourceSha256=source['Sha256'],sha256=hashlib.sha256(dest.read_bytes()).hexdigest().upper()))
dest=LAB/'assets/swarm_rat_visual.def'
text=(LAB/'assets/mesmerize_visual.def').read_text().replace('models/interactive/animated/mesmerize_v2/mesmerize_v2.md6','models/characters/animals/rat/rat.md6');dest.write_text(text)
assert 'arkSwarmAgentEntity'not in text and 'cpntanimrat'not in text and 'possession'not in text
manifest.append(dict(resource='models/powers/d2coop/swarm_rat_visual.def',generated=str(dest),kind='entitydef',classname='entityDef',sha256=hashlib.sha256(dest.read_bytes()).hexdigest().upper()))
(P/'swarm-rat-visual-assets-v291.json').write_text(json.dumps(dict(resources=manifest,eventTracks=0,gameplayComponents=0,runtimeVerified=False,note='Existing native MD6/animset retained; animation-embedded sound events may still run through generic audio. No attack/AI/possession components or Midnight gameplay tracks.'),indent=2))
s=(LAB/'Build-D2MesmerizeVisualAssets.ps1').read_text(encoding='utf-8-sig').replace('mesmerize-visual-v76','swarm-rat-visual-v291').replace('cosmetic Mesmerize model animation','cosmetic rat model animation')
start=s.index('$resources=@(');end=s.index('\n',start)
new='$resources=@('+','.join("@{Name='"+m['resource']+"';File='assets/"+Path(m['generated']).name+"';Kind='"+m['kind']+"';Class='"+m['classname']+"'}"for m in manifest)+')'
(LAB/'Build-D2SwarmRatVisualAssets.ps1').write_text(s[:start]+new+s[end:])
print(json.dumps([dict(alias=m.get('alias'),duration=m.get('durationMs'))for m in manifest]))
