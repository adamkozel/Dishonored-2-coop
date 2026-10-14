"""Read-only exact actor/component snapshot for targeted visibility analysis."""
from pathlib import Path
import ctypes as C, ctypes.wintypes as T, hashlib, json, struct, subprocess
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'; out=W/'observable-readonly-v980';out.mkdir(exist_ok=False)
exe=R/'work/asset-game/Dishonored2.exe';assert hashlib.sha256(exe.read_bytes()).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
rd=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
k=C.WinDLL('kernel32',use_last_error=True);k.OpenProcess.argtypes=[T.DWORD,T.BOOL,T.DWORD];k.OpenProcess.restype=T.HANDLE;k.ReadProcessMemory.argtypes=[T.HANDLE,C.c_void_p,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t)];k.ReadProcessMemory.restype=T.BOOL;k.QueryFullProcessImageNameW.argtypes=[T.HANDLE,T.DWORD,T.LPWSTR,C.POINTER(T.DWORD)];k.QueryFullProcessImageNameW.restype=T.BOOL;k.CloseHandle.argtypes=[T.HANDLE]
launch=rd(R/'work/lab-instances/host/launch.json');assert launch['assetExperiment'] and Path(launch['executable'])==exe
h=k.OpenProcess(0x1010,False,launch['pid']);assert h
try:
    name=C.create_unicode_buffer(32768);length=T.DWORD(len(name));assert k.QueryFullProcessImageNameW(h,0,name,C.byref(length)) and Path(name.value)==exe
    def read(at,n):
        b=C.create_string_buffer(n);done=C.c_size_t();assert k.ReadProcessMemory(h,at,b,n,C.byref(done)) and done.value==n;return b.raw
    q=lambda at:struct.unpack('<Q',read(at,8))[0]
    def text(at):
        value=bytearray()
        for i in range(512):
            b=read(at+i,1)
            if b==b'\0':return value.decode('utf-8')
            value.extend(b)
        raise AssertionError('unbounded name')
    status=rd(R/'work/lab-instances/host/coop-status.json');rows=[]
    for handle in [1,status['probeActorHandle']]:
        components=json.loads(subprocess.check_output([str(R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'),str(launch['pid']),'--components',str(handle)],text=True))
        for e in components['components']:
            if e['type'] not in ('.?AVarkCpntObservable@@','.?AVarkCpntAIAttentionSource@@'):continue
            at=int(e['pointer'],16);owner=int(e['owner'],16);header=read(at,32);assert q(at+0x10)==owner and struct.unpack('<I',read(owner+0xa8,4))[0]==handle
            base=q(at)-int(e['vtableRva'],16);decl=q(at+8);raw=read(at,0x80);d=read(decl,0xa0)
            row=dict(handle=handle,component=e,base=hex(base),declaration=hex(decl),declarationVtableRva=hex(q(decl)-base),declarationName=text(q(decl+8)),raw=raw.hex(),declarationRaw=d.hex(),actorRoot=list(struct.unpack('<12f',read(owner+0x300,48))))
            if e['type']=='.?AVarkCpntObservable@@':row.update(nativeReferenceCount=struct.unpack_from('<i',raw,0x40)[0],nativeTimer=struct.unpack_from('<f',raw,0x44)[0],boundHandle=struct.unpack_from('<I',raw,0x20)[0],boundComponent=hex(struct.unpack_from('<Q',raw,0x28)[0]),decl68=struct.unpack_from('<I',d,0x68)[0],decl6c=struct.unpack_from('<I',d,0x6c)[0])
            row['actorFlagsD0']=read(owner+0xd0,1)[0]
            row['actorFlags368']=read(owner+0x368,1)[0]
            frame=q(owner+0xf0);row['renderFrame']=hex(frame)
            if frame:
                row['frameVtableRva']=hex(q(frame)-base)
                row['boundsVirtualRva']=hex(q(q(frame)+0x80)-base)
                row['cachedBoundsMin']=list(struct.unpack('<3f',read(frame+0x1840,12)))
                row['cachedBoundsMax']=list(struct.unpack('<3f',read(frame+0x1850,12)))
            if e['type']=='.?AVarkCpntObservable@@':
                row['declVisibilityOffsets']=list(struct.unpack('<2f',read(decl+0x364,8)))
                bound=q(at+0x28)
                if bound:row['boundComponentVtableRva']=hex(q(bound)-base);row['physicsState238']=hex(struct.unpack('<I',read(bound+0x238,4))[0])
            row['physicsBodies']=[]
            for cp in components['components']:
                if cp['vtableRva'].lower()!='0x1c54008':continue
                phys=int(cp['pointer'],16);instance=q(phys+0x100)
                if not instance:continue
                world=q(instance+0x20);ids=q(instance+0x28);count=struct.unpack('<I',read(instance+0x30,4))[0]
                assert 0<count<=64 and world and ids
                bodies=q(world+0x28)
                for index in range(count):
                    bid=struct.unpack('<I',read(ids+4*index,4))[0];assert bid&0xffffff!=0xffffff
                    row['physicsBodies'].append(dict(id=bid,position=list(struct.unpack('<3f',read(bodies+(bid&0xffffff)*0xb0+0x30,12)))))
            assert read(at,32)==header and q(at+0x10)==owner
            rows.append(row)
    (out/'host.json').write_text(json.dumps(dict(readOnly=True,pid=launch['pid'],rows=rows),indent=2),encoding='utf-8')
    print(json.dumps([{k:v for k,v in r.items() if k not in ('raw','declarationRaw','component')} for r in rows],indent=2))
finally:k.CloseHandle(h)
