"""Bounded, read-only exact-image native damage-source identity lookup."""
from pathlib import Path
import argparse, ctypes as C, struct, json, subprocess, hashlib
from ctypes import wintypes as T
p=argparse.ArgumentParser();p.add_argument('role',choices=['host','client']);p.add_argument('handle',type=lambda x:int(x,0));p.add_argument('output');a=p.parse_args()
R=Path(__file__).resolve().parents[2];out=Path(a.output).resolve();assert out.is_relative_to(R) and not out.exists()
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
l=rd(R/f'work/lab-instances/{a.role}/launch.json');exe=R/'work/asset-game/Dishonored2.exe'
assert l['assetExperiment'] and Path(l['executable'])==exe
assert hashlib.sha256(exe.read_bytes()).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
k=C.WinDLL('kernel32',use_last_error=True);k.OpenProcess.argtypes=[T.DWORD,T.BOOL,T.DWORD];k.OpenProcess.restype=T.HANDLE
k.ReadProcessMemory.argtypes=[T.HANDLE,C.c_void_p,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t)];k.ReadProcessMemory.restype=T.BOOL
k.QueryFullProcessImageNameW.argtypes=[T.HANDLE,T.DWORD,T.LPWSTR,C.POINTER(T.DWORD)];k.QueryFullProcessImageNameW.restype=T.BOOL;k.CloseHandle.argtypes=[T.HANDLE]
h=k.OpenProcess(0x10|0x1000,False,l['pid']);assert h
try:
 path=C.create_unicode_buffer(32768);n=T.DWORD(len(path));assert k.QueryFullProcessImageNameW(h,0,path,C.byref(n));assert Path(path.value)==exe
 def read(at,size):
  assert at and 0<size<=4096
  b=C.create_string_buffer(size);done=C.c_size_t()
  if not k.ReadProcessMemory(h,at,b,size,C.byref(done)) or done.value!=size:raise OSError(C.get_last_error(),hex(at))
  return b.raw
 q=lambda at:struct.unpack('<Q',read(at,8))[0];d=lambda at:struct.unpack('<I',read(at,4))[0]
 probe=R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'
 comps=json.loads(subprocess.check_output([str(probe),str(l['pid']),'--components','1'],text=True))
 pc=int(next(c['pointer'] for c in comps['components'] if c['type']=='.?AVarkCpntPlayerPhysics@@'),16);base=q(pc)-0x1c73268
 game=q(base+0x39a5290);assert q(game)==base+0x1c940a8 and d(game+0x341058)==3 and not d(game+0x1f7400)
 def typename(at):
  vt=q(at)
  if not base<=vt<base+68988928:return ''
  col=q(vt-8);td=d(col+12);return read(base+td+16,160).split(b'\0')[0].decode('ascii',errors='replace')
 rows=[];handle=a.handle;seen=set();checks=[]
 for _ in range(12):
  slot=C.c_int32(handle).value>>16
  if not 0<=slot<16384:rows.append(dict(handle=hex(handle),resolved=False,reason='out_of_range'));break
  entity=q(game+0x3130b0+slot*8)
  actual=d(entity+0xa8) if entity else None
  row=dict(handle=hex(handle),slot=slot,entity=hex(entity),actual=hex(actual) if actual is not None else None,resolved=actual==handle)
  rows.append(row);checks.append((slot,entity,actual))
  if actual!=handle or entity in seen:break
  seen.add(entity);row['type']=typename(entity);row['parent']=hex(d(entity+0xac));row['bytes']=read(entity,0x330).hex()
  try:row['name']=read(q(entity+0x20),256).split(b'\0')[0].decode('utf-8',errors='replace')
  except (OSError,AssertionError):row['name']=None
  row['components']=json.loads(subprocess.check_output([str(probe),str(l['pid']),'--components',str(handle)],text=True))
  handle=d(entity+0xac)
 assert q(base+0x39a5290)==game and d(game+0x341058)==3 and not d(game+0x1f7400)
 for slot,entity,actual in checks:
  assert q(game+0x3130b0+slot*8)==entity and (not entity or d(entity+0xa8)==actual)
 out.parent.mkdir(parents=True,exist_ok=True)
 result=dict(role=a.role,pid=l['pid'],base=hex(base),game=hex(game),rows=rows,scope='Read-only current native entity lookup with exact-image/world/handle rechecks. Post-restoration observation does not establish identity at prior callback time.')
 out.write_text(json.dumps(result,indent=2),encoding='utf-8')
 print(json.dumps(dict(output=str(out),rows=[{k:v for k,v in x.items() if k not in ('bytes','components')} for x in rows])))
finally:k.CloseHandle(h)
