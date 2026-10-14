"""Read-only exact-image body ownership census; no target process writes/calls."""
from pathlib import Path
import argparse,ctypes as C,struct,json,subprocess,hashlib
from ctypes import wintypes as T
p=argparse.ArgumentParser();p.add_argument('role',choices=['host','client']);p.add_argument('output');a=p.parse_args()
R=Path(__file__).resolve().parents[2];out=Path(a.output).resolve();assert out.is_relative_to(R) and not out.exists()
readjson=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
l=readjson(R/f'work/lab-instances/{a.role}/launch.json');s=readjson(R/f'work/lab-instances/{a.role}/coop-status.json')
exe=R/'work/asset-game/Dishonored2.exe';assert l['assetExperiment'] and Path(l['executable'])==exe
assert hashlib.sha256(exe.read_bytes()).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
k=C.WinDLL('kernel32',use_last_error=True);k.OpenProcess.argtypes=[T.DWORD,T.BOOL,T.DWORD];k.OpenProcess.restype=T.HANDLE
k.ReadProcessMemory.argtypes=[T.HANDLE,C.c_void_p,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t)];k.ReadProcessMemory.restype=T.BOOL
k.QueryFullProcessImageNameW.argtypes=[T.HANDLE,T.DWORD,T.LPWSTR,C.POINTER(T.DWORD)];k.QueryFullProcessImageNameW.restype=T.BOOL;k.CloseHandle.argtypes=[T.HANDLE]
h=k.OpenProcess(0x10|0x1000,False,l['pid']);assert h
try:
 path=C.create_unicode_buffer(32768);n=T.DWORD(len(path));assert k.QueryFullProcessImageNameW(h,0,path,C.byref(n));assert Path(path.value)==exe
 def read(at,size):
  assert at and 0<size<=16*1024*1024
  b=C.create_string_buffer(size);done=C.c_size_t()
  if not k.ReadProcessMemory(h,at,b,size,C.byref(done)) or done.value!=size:raise OSError(C.get_last_error(),hex(at))
  return b.raw
 q=lambda at:struct.unpack('<Q',read(at,8))[0];d=lambda at:struct.unpack('<I',read(at,4))[0]
 probe=R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'
 comps=json.loads(subprocess.check_output([str(probe),str(l['pid']),'--components','1'],text=True))
 pc=int(next(c['pointer'] for c in comps['components'] if c['type']=='.?AVarkCpntPlayerPhysics@@'),16);base=q(pc)-0x1c73268
 game=q(base+0x39a5290);assert q(game)==base+0x1c940a8 and d(game+0x341058)==3 and not d(game+0x1f7400)
 player=q(game+0x3130b0);partner=q(game+0x3130b0+(s['probeActorHandle']>>16)*8);assert d(partner+0xa8)==s['probeActorHandle']
 world=q(game+0x60410);assert q(world)==base+0x1ad4f38
 table=q(world+0x28);count=d(world+0x30);assert 0<count<=65536
 bodies=read(table,count*0xb0)
 def typename(at):
  vt=q(at)
  if not base<=vt<base+68988928:return ''
  col=q(vt-8);td=d(col+12);return read(base+td+16,160).split(b'\0')[0].decode('ascii',errors='replace')
 rows=[];errors=0
 for index in range(count):
  b=bodies[index*0xb0:(index+1)*0xb0];bodyId=struct.unpack_from('<I',b,0x70)[0]
  if not b[0x44]&3 or bodyId&0xffffff!=index:continue
  data=struct.unpack_from('<Q',b,0x90)[0]
  if not data:continue
  try:
   component=q(data+8)
   if not component:continue
   owner=q(component+0x10)
   if owner not in (player,partner):continue
   assert q(table+index*0xb0+0x90)==data and d(table+index*0xb0+0x70)==bodyId
   rows.append(dict(bodyId=hex(bodyId),index=index,address=hex(table+index*0xb0),owner='player' if owner==player else 'partner',component=hex(component),componentType=typename(component),userData=hex(data),userDataType=typename(data),userDataVtableRva=hex(q(data)-base),position=list(struct.unpack_from('<fff',b,0x30)),filter=hex(struct.unpack_from('<I',b,0x6c)[0])))
  except OSError:errors+=1
 assert q(base+0x39a5290)==game and q(game+0x60410)==world and q(world+0x28)==table and d(world+0x30)==count and d(partner+0xa8)==s['probeActorHandle']
 out.write_text(json.dumps(dict(role=a.role,pid=l['pid'],base=hex(base),game=hex(game),player=hex(player),partner=hex(partner),world=hex(world),capacity=count,unreadableEntries=errors,bodies=rows,scope='Bounded read-only current Havok table ownership census; generation and world identity rechecked. No mutation or filtering claim.'),indent=2),encoding='utf-8')
 print(json.dumps(dict(role=a.role,bodies=len(rows),types=sorted(set((x['owner'],x['componentType'],x['userDataType']) for x in rows)),output=str(out))))
finally:k.CloseHandle(h)
