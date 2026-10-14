import struct,math,json
from pathlib import Path
P=Path(__file__).resolve().parent
for role in ['corvo','emily']:
 b=(P/'native-gaits'/f'{role}-gait-bank.bin').read_bytes();version,n,count,fps,layout=struct.unpack_from('<IIIIQ',b,8);entry=24+121*n*28
 assert len(b)==32+count*entry
 errors=[]
 for c in range(count):
  at=32+c*entry;idx,fc,flags,res,stride,angle=struct.unpack_from('<IIIIff',b,at)
  if idx!=c or fc!=121 or flags or res:errors.append((c,'metadata'))
  if c and not(.2<=stride<=(6 if c>8 else 3)):errors.append((c,'stride',stride))
  expected=((c-1)%8)*math.pi/4 if c else 0
  if abs(math.remainder(angle-expected,2*math.pi))>.0001:errors.append((c,'angle',angle))
  for f in range(121):
   for j in range(n):
    v=struct.unpack_from('<7f',b,at+24+(f*n+j)*28)
    if any(not math.isfinite(x) or abs(x)>3 for x in v):errors.append((c,f,j,'value'))
    norm=sum(x*x for x in v[:4])
    if abs(norm-1)>.001:errors.append((c,f,j,'quaternion',norm))
    if j<2 and any(abs(v[k]-(1 if k==3 else 0))>.00001 for k in range(7)):errors.append((c,f,j,'root'))
    if f==120:
     first=struct.unpack_from('<7f',b,at+24+j*28)
     if abs(sum(first[k]*v[k] for k in range(4)))<.999999:errors.append((c,f,j,'loop rotation',sum(first[k]*v[k] for k in range(4))))
     if any(abs(first[k]-v[k])>.000001 for k in range(4,7)):errors.append((c,f,j,'loop translation'))
 print(role,'errors',len(errors),errors[:12])
