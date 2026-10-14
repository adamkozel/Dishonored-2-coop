"""Bounded static inspection. Reads PE bytes; never loads or executes the DLL."""
from pathlib import Path
import datetime, hashlib, json, re, struct, zipfile
R=Path(__file__).resolve().parents[2]
O=R/'work/research/zikq-d2coop-0.1';O.mkdir(parents=True,exist_ok=True)
Z=Path(r'C:\Users\adako\.codex\codex-remote-attachments\01a11a65-16bb-74c3-a717-9821b03e3461\CDAC2459-70D6-4E10-AA94-69E316411D10\1-D2Coop-Beta-0.1-222-0.1-2026-10-03T10-50Z-K8kN6vwV4.zip')
sha=lambda b:hashlib.sha256(b).hexdigest()
archive=Z.read_bytes();assert sha(archive)=='65cc2b01ca65b180e8fbde3439054f9739bab7c565ecdd6924c9c6f035564d15'
members=[]
with zipfile.ZipFile(Z) as z:
 assert set(z.namelist())=={'D2Coop.ini','dinput8.dll','README.txt'}
 for item in z.infolist():
  assert item.file_size<3_000_000
  b=z.read(item);p=(O/'archive'/item.filename).resolve()
  assert p.parent==(O/'archive').resolve()
  p.parent.mkdir(exist_ok=True)
  if p.exists():assert p.read_bytes()==b
  else:p.write_bytes(b)
  members.append(dict(name=item.filename,size=len(b),sha256=sha(b)))
b=(O/'archive/dinput8.dll').read_bytes()
u16=lambda o:struct.unpack_from('<H',b,o)[0]
u32=lambda o:struct.unpack_from('<I',b,o)[0]
u64=lambda o:struct.unpack_from('<Q',b,o)[0]
pe=u32(0x3c);assert b[pe:pe+4]==b'PE\0\0';opt=pe+24
assert u16(opt)==0x20b
sections=[]
for i in range(u16(pe+6)):
 p=opt+u16(pe+20)+i*40
 sections.append(dict(name=b[p:p+8].rstrip(b'\0').decode('ascii'),virtualSize=u32(p+8),rva=u32(p+12),rawSize=u32(p+16),rawOffset=u32(p+20)))
def off(rva):
 if rva<u32(opt+60):return rva
 for s in sections:
  if s['rva']<=rva<s['rva']+s['rawSize']:return s['rawOffset']+rva-s['rva']
 raise ValueError(hex(rva))
def cstr(p):return b[p:b.index(0,p)].decode('utf-8',errors='replace')
def directory(n):return u32(opt+112+n*8),u32(opt+116+n*8)
imports={};rva,size=directory(1)
if rva:
 p=off(rva)
 while any(b[p:p+20]):
  ilt,_,_,name,iat=struct.unpack_from('<IIIII',b,p);q=off(ilt or iat);names=[]
  while (v:=u64(q)):
   names.append('ordinal:'+str(v&0xffff) if v>>63 else cstr(off(v)+2));q+=8
  imports[cstr(off(name))]=names;p+=20
exports=[];rva,size=directory(0)
if rva:
 p=off(rva);base=u32(p+16);count=u32(p+24);functions=off(u32(p+28));names=off(u32(p+32));ordinals=off(u32(p+36))
 for i in range(count):
  ordinal=u16(ordinals+i*2);target=u32(functions+ordinal*4)
  exports.append(dict(name=cstr(off(u32(names+i*4))),ordinal=base+ordinal,rva=hex(target)))
strings=[dict(offset=hex(m.start()),text=m.group().decode('ascii')) for m in re.finditer(rb'[\x20-\x7e]{6,}',b)]
patterns={
 'network':r'(?i)\b(steam|raknet|enet|radmin|hamachi|winsock|connect|packet|socket|lobby|udp|tcp|resync|peer|host|client)\b|[Nn]et(work|Manager|Session)',
 'movement':r'(?i)anim|skeleton|bone|doppel|decoy|shadowwalk|shadow.walk|crouch|stance|replic|avatar|remote.player|interpol',
 'world':r'(?i)checkpoint|savegame|\.save|loadgame|mission|cutscene|midnight|entity|spawn|teleport',
 'debug':r'(?i)\.pdb|\.cpp|\.hpp|github|version|build|offset|signature|pattern|hook',
}
hits={k:[s for s in strings if re.search(p,s['text'])] for k,p in patterns.items()}
report=dict(inspectedAt=datetime.datetime.now().astimezone().isoformat(),sourceUrl='https://www.nexusmods.com/dishonored2/mods/222',archivePath=str(Z),archiveSize=len(archive),archiveSha256=sha(archive),members=members,mode='Static bytes only; no DLL execution, installation, chain-loading or copying into a game directory.',machine=hex(u16(pe+4)),imageBase=hex(u64(opt+24)),linkerTimestampRaw=u32(pe+8),sections=sections,imports=imports,exports=exports,strings=hits,sourceIncluded=False)
(O/'static-inspection.json').write_text(json.dumps(report,indent=2),encoding='utf-8')
for k,v in hits.items():
 (O/(k+'-strings.txt')).write_text('\n'.join(s['offset']+' '+s['text'] for s in v),encoding='utf-8')
print(json.dumps(dict(folder=str(O),members=members,imports=imports,exports=exports,hitCounts={k:len(v) for k,v in hits.items()},selected={k:v[:30] for k,v in hits.items()}),indent=2))
