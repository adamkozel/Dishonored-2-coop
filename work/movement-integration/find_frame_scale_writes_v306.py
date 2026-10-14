from pathlib import Path
import sys,struct,json
R=Path(__file__).resolve().parents[2];sys.path.insert(0,str(R/'work/tools/capstone-5.0.6-local'))
import capstone
data=(R/'work/asset-game/Dishonored2.exe').read_bytes();u16=lambda o:struct.unpack_from('<H',data,o)[0];u32=lambda o:struct.unpack_from('<I',data,o)[0]
pe=u32(0x3c);opt=pe+24;sections=[]
for i in range(u16(pe+6)):
 o=opt+u16(pe+20)+i*40;sections.append((u32(o+12),u32(o+8),u32(o+20)))
def offset(rva):
 for va,size,raw in sections:
  if va<=rva<va+size:return raw+rva-va
 raise ValueError(hex(rva))
exr,exs=struct.unpack_from('<II',data,opt+112+3*8);xo=offset(exr);functions=[struct.unpack_from('<III',data,xo+i)[:2]for i in range(0,exs,12)]
md=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_64);md.detail=True;md.skipdata=True
matches=[]
for start,end in [(0x74f5e0,0x754600),(0x875000,0x87f000)]:
 instructions=list(md.disasm(data[offset(start):offset(start)+end-start],0x140000000+start))
 for i,ins in enumerate(instructions):
  if not ins.id or not ins.operands:continue
  dst=ins.operands[0]
  if dst.type==capstone.CS_OP_MEM and dst.mem.disp in (0x60,0x330,0x1890) and ins.reg_name(dst.mem.base) not in ('rsp','rbp') and dst.access&capstone.CS_AC_WRITE:
   matches.append(dict(instruction=hex(ins.address),context=[f'{x.address:x} {x.mnemonic} {x.op_str}' for x in instructions[max(0,i-5):i+5]]))
(R/'work/movement-integration/frame-scale-writes-v306.json').write_text(json.dumps(matches,indent=2));print(json.dumps(matches,indent=2))
