# disasm-ae3240.txt

Original: [work/disasm-ae3240.txt](<../../../../work/disasm-ae3240.txt>)

SHA-256: `38B8724D56895E30073B14A40A0AF100A01E34FBC360AA403353769B84323191`; 3129 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  0000000140AE3240: movaps      xmm2,xmmword ptr [rdx+20h]
  0000000140AE3244: movaps      xmm3,xmmword ptr [rdx]
  0000000140AE3247: mov         r9,rdx
  0000000140AE324A: mov         r10,rcx
  0000000140AE324D: mov         rax,rcx
  0000000140AE3250: movaps      xmm1,xmm3
  0000000140AE3253: unpckhps    xmm3,xmmword ptr [rdx+10h]
  0000000140AE3257: unpcklps    xmm1,xmmword ptr [rdx+10h]
  0000000140AE325B: shufps      xmm3,xmm2,0E4h
  0000000140AE325F: mov         rdx,r8
  0000000140AE3262: sub         rdx,rcx
  0000000140AE3265: mov         ecx,3
  0000000140AE326A: movaps      xmm5,xmm1
  0000000140AE326D: movlhps     xmm5,xmm2
  0000000140AE3270: movaps      xmm4,xmm5
  0000000140AE3273: movhlps     xmm4,xmm1
  0000000140AE3276: shufps      xmm4,xmm2,0D4h
  0000000140AE327A: nop         word ptr [rax+rax]
  0000000140AE3280: movaps      xmm1,xmmword ptr [rdx+rax]
  0000000140AE3284: add         rax,10h
  0000000140AE3288: movaps      xmm2,xmm1
  0000000140AE328B: movaps      xmm0,xmm1
  0000000140AE328E: shufps      xmm2,xmm1,55h
  0000000140AE3292: shufps      xmm0,xmm1,0
  0000000140AE3296: shufps      xmm1,xmm1,0AAh
  0000000140AE329A: mulps       xmm2,xmm4
  0000000140AE329D: mulps       xmm0,xmm5
  0000000140AE32A0: mulps       xmm1,xmm3
  0000000140AE32A3: addps       xmm2,xmm0
  0000000140AE32A6: addps       xmm2,xmm1
  0000000140AE32A9: movaps      xmmword ptr [rax-10h],xmm2
  0000000140AE32AD: dec         rcx
  0000000140AE32B0: jne         0000000140AE3280
  0000000140AE32B2: movaps      xmm3,xmmword ptr [r9+20h]
  0000000140AE32B7: movaps      xmm4,xmmword ptr [r9]
  0000000140AE32BB: movaps      xmm5,xmmword ptr [r8+30h]
  0000000140AE32C0: subps       xmm5,xmmword ptr [r9+30h]
  0000000140AE32C5: movaps      xmm1,xmm4
  0000000140AE32C8: unpckhps    xmm4,xmmword ptr [r9+10h]
  0000000140AE32CD: unpcklps    xmm1,xmmword ptr [r9+10h]
  0000000140AE32D2: shufps      xmm4,xmm3,0E4h
  0000000140AE32D6: movaps      xmm2,xmm1
  0000000140AE32D9: movlhps     xmm2,xmm3
  0000000140AE32DC: movaps      xmm0,xmm2
  0000000140AE32DF: movhlps     xmm0,xmm1
  0000000140AE32E2: movaps      xmm1,xmm5
  0000000140AE32E5: shufps      xmm1,xmm5,55h
  0000000140AE32E9: shufps      xmm0,xmm3,0D4h
  0000000140AE32ED: mulps       xmm1,xmm0
  0000000140AE32F0: movaps      xmm0,xmm5
  0000000140AE32F3: shufps      xmm0,xmm5,0
  0000000140AE32F7: shufps      xmm5,xmm5,0AAh
  0000000140AE32FB: mulps       xmm0,xmm2
  0000000140AE32FE: mulps       xmm5,xmm4
  0000000140AE3301: addps       xmm1,xmm0
  0000000140AE3304: addps       xmm1,xmm5
  0000000140AE3307: movaps      xmmword ptr [r10+30h],xmm1
  0000000140AE330C: ret
  0000000140AE330D: int         3
  0000000140AE330E: int         3
  0000000140AE330F: int         3
  0000000140AE3310: int         3

  Summary

     1A7D000 .text

````
