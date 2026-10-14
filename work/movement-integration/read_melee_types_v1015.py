"""Read-only: native type ids used by parry-win 140636140's [1439ce350,1439ce354] range.

For each entity, vtable slot 0 must be `lea rax,[typeinfo]; ret`; the type id is
typeinfo+0x60. Reports the actual player, the host's owned clone and Ramsey.
"""
from pathlib import Path
import argparse,ctypes as C,ctypes.wintypes as T,hashlib,json,struct,subprocess
p=argparse.ArgumentParser();p.add_argument('--role',choices=['host','client'],required=True);p.add_argument('--output',required=True);args=p.parse_args()
R=Path(__file__).resolve().parents[2];out=Path(args.output).resolve();assert out.is_relative_to(R) and not out.exists()
exe=R/'work/asset-game/Dishonored2.exe';assert hashlib.sha256(exe.read_bytes()).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
launch=rd(R/f'work/lab-instances/{args.role}/launch.json');assert launch['assetExperiment'] and Path(launch['executable'])==exe
probe=str(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe')
q_=lambda *a:json.loads(subprocess.check_output([probe,str(launch['pid']),*a],text=True))
k=C.WinDLL('kernel32',use_last_error=True);k.OpenProcess.argtypes=[T.DWORD,T.BOOL,T.DWORD];k.OpenProcess.restype=T.HANDLE;k.ReadProcessMemory.argtypes=[T.HANDLE,C.c_void_p,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t)];k.ReadProcessMemory.restype=T.BOOL;k.QueryFullProcessImageNameW.argtypes=[T.HANDLE,T.DWORD,T.LPWSTR,C.POINTER(T.DWORD)];k.QueryFullProcessImageNameW.restype=T.BOOL;k.CloseHandle.argtypes=[T.HANDLE]
h=k.OpenProcess(0x1010,False,launch['pid']);assert h
try:
    name=C.create_unicode_buffer(32768);length=T.DWORD(len(name));assert k.QueryFullProcessImageNameW(h,0,name,C.byref(length)) and Path(name.value)==exe
    def read(at,n):
        b=C.create_string_buffer(n);done=C.c_size_t();assert k.ReadProcessMemory(h,at,b,n,C.byref(done)) and done.value==n;return b.raw
    q=lambda at:struct.unpack('<Q',read(at,8))[0]
    i32=lambda at:struct.unpack('<i',read(at,4))[0]
    u=lambda at:struct.unpack('<I',read(at,4))[0]
    player=q_('--health','1');comp=int(player['component'],16);base=q(comp)-0x1c51588
    status=rd(R/f'work/lab-instances/{args.role}/coop-status.json')
    entities={'player':int(player['actor'],16)}
    if args.role=='host' and status.get('probeActorHandle'):
        entities['clone']=int(q_('--health',str(status['probeActorHandle']))['actor'],16)
    world=q_('--world-npcs')
    for n in world['npcs']:
        if 'mortimer_ramsey' in n.get('definition','') and n.get('unique'):
            entities['ramsey']=int(q_('--health',str(int(n['handle'],16)))['actor'],16)
    def typeid(entity):
        vt=q(entity);slot=q(vt);code=read(slot,8)
        assert code[:3]==b'\x48\x8d\x05' and code[7]==0xc3,f'unexpected slot0 {code.hex()}'
        info=slot+7+struct.unpack('<i',code[3:7])[0]
        return dict(entity=hex(entity),handle=u(entity+0xa8),vtableRva=hex(vt-base),slot0Rva=hex(slot-base),typeInfoRva=hex(info-base),typeId=i32(info+0x60))
    rows={k2:typeid(v) for k2,v in entities.items()}
    lo,hi=i32(base+0x39ce350),i32(base+0x39ce354)
    for r in rows.values():r['inRange']=lo<=r['typeId']<=hi
    out.write_text(json.dumps(dict(readOnly=True,role=args.role,pid=launch['pid'],range=[lo,hi],entities=rows),indent=2),encoding='utf-8')
    print(json.dumps(dict(range=[lo,hi],entities={k2:(v['typeId'],v['inRange'],v['vtableRva']) for k2,v in rows.items()})))
finally:k.CloseHandle(h)
