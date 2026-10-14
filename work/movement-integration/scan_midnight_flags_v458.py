from pathlib import Path
import struct,json,hashlib
R=Path(__file__).resolve().parents[2];b=(R/'work/asset-game/Dishonored2.exe').read_bytes()
assert hashlib.sha256(b).hexdigest().upper()=='B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180'
pe=struct.unpack_from('<I',b,0x3c)[0];count=struct.unpack_from('<H',b,pe+6)[0];sec=pe+24+struct.unpack_from('<H',b,pe+20)[0]
sections=[struct.unpack_from('<IIII',b,sec+n*40+8) for n in range(count)]
def off(rva):
 for vs,vr,sz,ro in sections:
  if vr<=rva<vr+sz:return ro+rva-vr
 raise ValueError(hex(rva))
tab,sz=struct.unpack_from('<II',b,pe+24+112+24)
funcs=[struct.unpack_from('<III',b,x)for x in range(off(tab),off(tab)+sz,12)]
def parent(rva):
 for start,end,uw in funcs:
  if start<=rva<end:
   while b[off(uw)]>>3&4:
    u=off(uw);start,end,uw=struct.unpack_from('<III',b,u+4+((b[u+2]+1)&~1)*2)
   return hex(0x140000000+start)
out=[];start=off(0x7c0000);end=off(0x7f0000)
for field in (0x2b2,0x2b3,0x2b4,0x2b5):
 needle=struct.pack('<I',field);pos=b.find(needle,start,end)
 while pos>=0:
  rva=0x7c0000+pos-start;out.append(dict(field=hex(field),address=hex(0x140000000+rva),function=parent(rva),bytes=b[pos-5:pos+9].hex()));pos=b.find(needle,pos+1,end)
(Path(__file__).with_name('midnight-flag-sites-v458.json')).write_text(json.dumps(out,indent=2))
print(json.dumps(out))
