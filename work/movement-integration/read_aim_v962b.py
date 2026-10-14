"""Read-only live aim setting discovery; no process writes or cvar setters."""
from pathlib import Path
import json,struct,ctypes as C,hashlib,subprocess
from ctypes import wintypes as T
R=Path(__file__).resolve().parents[2];out=R/'work/movement-integration/aim-readonly-v962b';out.mkdir(exist_ok=False)
exe=R/'work/asset-game/Dishonored2.exe';b=exe.read_bytes();assert hashlib.sha256(b).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
pe=struct.unpack_from('<I',b,60)[0];n=struct.unpack_from('<H',b,pe+6)[0];opt=struct.unpack_from('<H',b,pe+20)[0];secs=[]
for i in range(n):
 o=pe+24+opt+i*40;size,rva,rs,raw=struct.unpack_from('<4I',b,o+8);flags=struct.unpack_from('<I',b,o+36)[0];secs.append((rva,raw,rs,size,flags))
names=['ark_player_autoaim_model_pad','ark_player_autoaim_strength_pad','ark_player_autoaim_model_mouse'];rvas={}
for name in names:
 off=b.index(name.encode()+b'\0');rvas[name]=next(rva+off-raw for rva,raw,rs,sz,flags in secs if raw<=off<raw+rs)
k=C.WinDLL('kernel32',use_last_error=True);k.OpenProcess.argtypes=[T.DWORD,T.BOOL,T.DWORD];k.OpenProcess.restype=T.HANDLE;k.ReadProcessMemory.argtypes=[T.HANDLE,C.c_void_p,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t)];k.ReadProcessMemory.restype=T.BOOL;k.QueryFullProcessImageNameW.argtypes=[T.HANDLE,T.DWORD,T.LPWSTR,C.POINTER(T.DWORD)];k.QueryFullProcessImageNameW.restype=T.BOOL;k.CloseHandle.argtypes=[T.HANDLE]
for role in ['host','client']:
 l=json.loads((R/f'work/lab-instances/{role}/launch.json').read_text(encoding='utf-8-sig'));assert l['assetExperiment'] and Path(l['executable'])==exe
 h=k.OpenProcess(0x1010,False,l['pid']);assert h
 try:
  s=C.create_unicode_buffer(32768);sz=T.DWORD(len(s));assert k.QueryFullProcessImageNameW(h,0,s,C.byref(sz)) and Path(s.value)==exe
  def read(at,n):
   buf=C.create_string_buffer(n);done=C.c_size_t();assert k.ReadProcessMemory(h,at,buf,n,C.byref(done)) and done.value==n;return buf.raw
  q=lambda at:struct.unpack('<Q',read(at,8))[0]
  cp=json.loads(subprocess.check_output([str(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'),str(l['pid']),'--components','1'],text=True));ptr=int(next(x['pointer'] for x in cp['components'] if x['type']=='.?AVarkCpntPlayerPhysics@@'),16);base=q(ptr)-0x1c73268;rows=[]
  for rva,raw,rs,sz,flags in secs:
   if not flags&0x80000000:continue
   data=read(base+rva,sz)
   for name,nrva in rvas.items():
    needle=struct.pack('<Q',base+nrva);start=0
    while True:
     at=data.find(needle,start)
     if at<0:break
     start=at+1
     if at<48:continue
     address=base+rva+at;tp=q(address-48)
     try:current=read(tp,64).split(b'\0')[0].decode('ascii')
     except Exception:current=None
     rows.append(dict(name=name,nameFieldRva=hex(rva+at),text=current,cachedInteger=struct.unpack('<i',read(address-8,4))[0],fields=read(address-48,144).hex()))
  assert q(ptr)==base+0x1c73268
  result=dict(role=role,pid=l['pid'],base=hex(base),rows=rows,scope='Read-only current live cvar candidates located by exact-image string references; no settings changed.');(out/f'{role}.json').write_text(json.dumps(result,indent=2));print(json.dumps(dict(role=role,rows=[{k:v for k,v in x.items() if k!='fields'}for x in rows])))
 finally:k.CloseHandle(h)
