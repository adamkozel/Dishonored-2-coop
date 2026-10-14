"""Read-only verification; caller holds the serialized native-operation mutex."""
from pathlib import Path
import ctypes as C, datetime, hashlib, json, struct, subprocess
from ctypes import wintypes as T
R=Path(__file__).resolve().parents[2]; W=R/'work/movement-integration'
readjson=lambda p:json.loads(p.read_text(encoding='utf-8-sig'))
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
receipt=W/'native-collision-preserved-v391.json'; assert not receipt.exists(); result=readjson(W/'candidate-partner-assassination-v387/identity.json');result['deploymentVerified']=False
assert not result['deploymentVerified']
exe=R/'work/asset-game/Dishonored2.exe'
assert sha(exe)=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
assert sha(exe.parent/'dinput8.dll')==result['dll']
assert sha(R/'outputs/d2-coop-lab/bin/dinput8.dll')==result['dll']
for f in readjson(W/'protected-save-hashes-before-v4.json'): assert sha(Path(f['Path']))==f['Hash']
k=C.WinDLL('kernel32',use_last_error=True)
k.OpenProcess.argtypes=[T.DWORD,T.BOOL,T.DWORD]; k.OpenProcess.restype=T.HANDLE
k.ReadProcessMemory.argtypes=[T.HANDLE,C.c_void_p,C.c_void_p,C.c_size_t,C.POINTER(C.c_size_t)]; k.ReadProcessMemory.restype=T.BOOL
k.QueryFullProcessImageNameW.argtypes=[T.HANDLE,T.DWORD,T.LPWSTR,C.POINTER(T.DWORD)]; k.QueryFullProcessImageNameW.restype=T.BOOL
k.CloseHandle.argtypes=[T.HANDLE]
rows=[];probe=R/'outputs/d2-coop-lab/bin/d2_runtime_probe.exe'
for role in ('host','client'):
    l=readjson(R/f'work/lab-instances/{role}/launch.json');assert l['assetExperiment'] and Path(l['executable'])==exe
    h=k.OpenProcess(0x1010,False,l['pid']); assert h
    try:
        path=C.create_unicode_buffer(32768); n=T.DWORD(len(path)); assert k.QueryFullProcessImageNameW(h,0,path,C.byref(n)); assert Path(path.value)==exe
        def read(at,size):
            b=C.create_string_buffer(size); done=C.c_size_t()
            assert k.ReadProcessMemory(h,at,b,size,C.byref(done)) and done.value==size
            return b.raw
        def q(at): return struct.unpack('<Q',read(at,8))[0]
        def query(*args): return json.loads(subprocess.check_output([str(probe),str(l['pid']),*map(str,args)],text=True,encoding='utf-8'))
        components=query('--components',1)
        pc=int(next(c['pointer'] for c in components['components'] if c['type']=='.?AVarkCpntPlayerPhysics@@'),16)
        base=q(pc)-0x1c73268
        native=read(base+0x93b00,14).hex();assert native=='448bca8bc2418bd081e2ffffff00', 'Collision interception is still active'
        s=readjson(R/f'work/lab-instances/{role}/coop-status.json')
        assert s['localGameLogicState']==1 and s['localPendingGameLogicState']==0 and s['remoteFresh'] and s['connectionState']==3 and s['steamAvailable']
        hp=query('--health',1); partner=query('--health',s['probeActorHandle'])
        assert hp['current']==56 and partner['current']==40
        filters={}
        for name in ('mesmerize','domino','possession','assassination'):
            f=readjson(R/f'work/lab-instances/{role}/coop-status.json.{name}-partner-filter.json'); assert f['installed'];filters[name]=f
        damage=readjson(R/f'work/lab-instances/{role}/coop-status.json.damage.json')
        assert damage['ownedAvatarValid'] and damage['friendlyFireFilterEnabled']
        rows.append(dict(role=role,pid=l['pid'],base=hex(base),nativeCollisionPrologue=native,health=hp['current'],partnerHealth=partner['current'],steamInterface=s['nativeInterface'],connectionState=s['connectionState'],powerFilters=filters,damageFilterEnabled=True))
    finally:k.CloseHandle(h)
for f in readjson(W/'protected-save-hashes-before-v4.json'): assert sha(Path(f['Path']))==f['Hash']
result.update(deploymentVerified=True,verifiedAt=datetime.datetime.now().astimezone().isoformat(),roles=rows,protectedFilesUnchanged=14,
    scope='Verified387 isolated DLL active; exact original collision prologue in both live game processes, native Steam session connected, damage, three power and assassination filters installed, health and protected files unchanged. This read-only check is not a new blanket gameplay exclusion test.')
receipt.write_text(json.dumps(result,indent=2),encoding='utf-8')
print(json.dumps(dict(passed=True,roles=[r['role'] for r in rows],nativeCollisionRestored=True,protectedFilesUnchanged=14,receipt=str(receipt))))
