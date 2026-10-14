from pathlib import Path
import zipfile,sys,struct,hashlib,json
R=Path(__file__).resolve().parents[2]
wheel=R/'work/tools/ghidra_12.1.4_PUBLIC/Ghidra/Debug/Debugger-agent-dbgeng/pypkg/dist/capstone-5.0.6-py3-none-win_amd64.whl'
dest=R/'work/tools/capstone-5.0.6-local'
if not dest.exists():
    with zipfile.ZipFile(wheel) as z:
        assert all(not Path(i.filename).is_absolute() and '..' not in Path(i.filename).parts for i in z.infolist())
        z.extractall(dest)
sys.path.insert(0,str(dest));from capstone import Cs,CS_ARCH_X86,CS_MODE_64
exe=Path('C:/Program Files (x86)/Steam/steamapps/common/Dishonored2/Dishonored2.exe')
data=exe.read_bytes();assert hashlib.sha256(data).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
pe=struct.unpack_from('<I',data,0x3c)[0];sections=struct.unpack_from('<H',data,pe+6)[0];opt=struct.unpack_from('<H',data,pe+20)[0]
def section(rva,length):
    for i in range(sections):
        at=pe+24+opt+i*40;vs,va,rawsize,raw=struct.unpack_from('<IIII',data,at+8)
        if va<=rva<va+max(vs,rawsize):return data[raw+rva-va:raw+rva-va+length]
    raise ValueError('Unmapped RVA')
rows=[]
for rva,length in [(0x447250,32),(0x4478c0,64)]:
    blob=section(rva,length);ins=[dict(address=hex(i.address),size=i.size,bytes=i.bytes.hex(),mnemonic=i.mnemonic,operands=i.op_str) for i in Cs(CS_ARCH_X86,CS_MODE_64).disasm(blob,0x140000000+rva)]
    rows.append(dict(rva=hex(rva),hex=blob.hex(),instructions=ins))
assert rows[0]['hex']=='405341574883ec284c8bf9488b89b80000000fb6dae86666070080783800750a'
out=dict(exe=str(exe),exeSha256=hashlib.sha256(data).hexdigest().upper(),capstone='5.0.6',wheelSha256=hashlib.sha256(wheel.read_bytes()).hexdigest().upper(),rows=rows)
(R/'work/domino-picker-disk-disassembly-v211.json').write_text(json.dumps(out,indent=2))
print(json.dumps(out,indent=2))
