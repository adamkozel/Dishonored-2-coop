from pathlib import Path
import hashlib,json,shutil,struct,datetime
R=Path(__file__).resolve().parents[2];W=R/'work/movement-integration';L=R/'outputs/d2-coop-lab';A=W/'candidate-partner-assassination-v387';assert not A.exists()
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest().upper()
exe=R/'work/asset-game/Dishonored2.exe';assert sha(exe)=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
b=exe.read_bytes();pe=struct.unpack_from('<I',b,0x3c)[0];n=struct.unpack_from('<H',b,pe+6)[0];opt=struct.unpack_from('<H',b,pe+20)[0];rva=0x3b00a0;found=[]
for i in range(n):
 p=pe+24+opt+i*40;size,va,raw_size,raw=struct.unpack_from('<IIII',b,p+8)
 if va<=rva and rva+16<=va+raw_size:found.append(raw+rva-va)
assert len(found)==1;prologue=b[found[0]:found[0]+16];assert prologue==bytes.fromhex('4053574883ec38488bfa488bd94885d2')
logs=[W/f'partner-assassination-{k}-v387.log' for k in ('build','world-tests','action-tests','bank-tests','smoke')]
for p in logs:
 t=p.read_text(encoding='utf-8-sig');assert t and not any(s in t for s in ('error C','error LNK','FAIL:','FAIL ','Traceback')),p
A.mkdir();shutil.copytree(L/'src',A/'src');shutil.copytree(W/'candidate-possession-partner-v374/banks',A/'banks')
for name in ('dinput8.dll','d2_runtime_probe.exe'):shutil.copy2(R/'work/build/Release'/name,A/name)
for p in [Path(__file__),*logs,W/'Build-PartnerAssassination-v387.ps1',R/'work/natural-moveset/Observe-PartnerChoke-v386.ps1']:shutil.copy2(p,A/p.name)
x=json.loads((W/'candidate-possession-partner-v374/identity.json').read_text(encoding='utf-8-sig'))
x.update(created=datetime.datetime.now().astimezone().isoformat(),dll=sha(A/'dinput8.dll'),probe=sha(A/'d2_runtime_probe.exe'),runtimeVerified=False,banks=[dict(path=str(p),sha256=sha(p)) for p in sorted((A/'banks').glob('*.bin'))],scope='Candidate387 rejects current partner at native assassination eligibility3b00a0 after reciprocal local PlayerStates context and current registered partner proof. All other NPC calls forwarded unchanged. Bank54/319 and Possession374 unchanged. Native movement collision intentionally unchanged per user. Static tests passed; runtime pending.')
x.pop('runtimeVerificationScope',None)
(A/'identity.json').write_text(json.dumps(x,indent=2),encoding='utf-8')
(A/'prologue.json').write_text(json.dumps(dict(executable=sha(exe),rva=hex(rva),fileOffset=hex(found[0]),instructions=6,length=16,bytes=prologue.hex()),indent=2),encoding='utf-8')
for name in ('dinput8.dll','d2_runtime_probe.exe'):
 shutil.copy2(A/name,L/'bin'/name);assert sha(A/name)==sha(L/'bin'/name)
print(json.dumps(dict(dll=x['dll'],probe=x['probe'],archive=str(A))))
