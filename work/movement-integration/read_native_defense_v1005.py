"""Read-only snapshot of the actual lab player's native damage protections."""
from pathlib import Path
import argparse,ctypes as C,ctypes.wintypes as T,hashlib,json,struct,subprocess
p=argparse.ArgumentParser();p.add_argument('--role',choices=['host','client'],required=True);p.add_argument('--output',required=True);args=p.parse_args()
R=Path(__file__).resolve().parents[2];out=Path(args.output).resolve();assert out.is_relative_to(R) and not out.exists()
exe=R/'work/asset-game/Dishonored2.exe';assert hashlib.sha256(exe.read_bytes()).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
launch=rd(R/f'work/lab-instances/{args.role}/launch.json');assert launch['assetExperiment'] and Path(launch['executable'])==exe
k=C.WinDLL('kernel32',use_last_error=True);k.OpenProcess.argtypes=[T.DWORD,T.BOOL,T.DWORD];k.OpenProcess.restype=T.HANDLE;k.ReadProcessMemory.argtypes=[T.HANDLE,C.c_void_p,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t)];k.ReadProcessMemory.restype=T.BOOL;k.QueryFullProcessImageNameW.argtypes=[T.HANDLE,T.DWORD,T.LPWSTR,C.POINTER(T.DWORD)];k.QueryFullProcessImageNameW.restype=T.BOOL;k.CloseHandle.argtypes=[T.HANDLE]
h=k.OpenProcess(0x1010,False,launch['pid']);assert h
try:
    name=C.create_unicode_buffer(32768);length=T.DWORD(len(name));assert k.QueryFullProcessImageNameW(h,0,name,C.byref(length)) and Path(name.value)==exe
    def read(at,n):
        b=C.create_string_buffer(n);done=C.c_size_t();assert k.ReadProcessMemory(h,at,b,n,C.byref(done)) and done.value==n;return b.raw
    q=lambda at:struct.unpack('<Q',read(at,8))[0]
    u=lambda at:struct.unpack('<I',read(at,4))[0]
    def text(at):
        value=bytearray()
        for i in range(512):
            b=read(at+i,1)
            if b==b'\0':return value.decode('utf-8')
            value.extend(b)
        raise AssertionError('Unbounded native declaration name')
    health=json.loads(subprocess.check_output([str(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'),str(launch['pid']),'--health','1'],text=True))
    at=int(health['component'],16);owner=int(health['actor'],16);base=q(at)-0x1c51588;header=read(at,32)
    assert q(at+0x10)==owner and u(owner+0xa8)==1 and health['logic']==1
    array=q(at+0x88);count=u(at+0x90)&0xffffff;assert count<=128 and (not count or array)
    rows=[]
    for i in range(count):
        entry=q(array+i*8);decl=q(entry+8);entity=q(entry+0x10);assert entry and decl and entity
        vt=q(entry);assert 0<vt-base<0x4000000
        raw=read(decl+0x120,0x30)
        rows.append(dict(component=hex(entry),vtableRva=hex(vt-base),owner=hex(entity),ownerHandle=u(entity+0xa8),linkedHandle=u(entry+0x68),declaration=text(q(decl+8)),decl120=raw.hex(),kind128=u(decl+0x128),direction12c=u(decl+0x12c),basis130=u(decl+0x130),blockMask138=hex(q(decl+0x138)),effectMask140=hex(q(decl+0x140)),flags148=u(decl+0x148)))
    assert read(at,32)==header and q(at+0x88)==array and (u(at+0x90)&0xffffff)==count
    out.write_text(json.dumps(dict(readOnly=True,role=args.role,pid=launch['pid'],health=health,protections=rows),indent=2),encoding='utf-8')
    print(json.dumps(dict(count=count,declarations=[x['declaration'] for x in rows])))
finally:k.CloseHandle(h)
