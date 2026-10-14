# disasm-469bdd.txt

Original: [work/disasm-469bdd.txt](<../../../../work/disasm-469bdd.txt>)

SHA-256: `E356A05DDBC81A1C8E116AEEEB3AD860F021330D5BD5323A07F9CCFE95FB2A1D`; 3468 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  0000000140469BDD: mov         qword ptr [rax+10h],r14
  0000000140469BE1: movaps      xmmword ptr [rax-38h],xmm7
  0000000140469BE5: movaps      xmmword ptr [rax-48h],xmm8
  0000000140469BEA: movaps      xmmword ptr [rax-58h],xmm9
  0000000140469BEF: mov         eax,dword ptr [rcx+0000000000000AC4h]
  0000000140469BF5: mov         rdi,rcx
  0000000140469BF8: add         rcx,0BD0h
  0000000140469BFF: movss       xmm7,dword ptr [rbp+77h]
  0000000140469C04: mov         dword ptr [rbp-59h],eax
  0000000140469C07: mov         eax,dword ptr [rcx+FFFFFFFFFFFFFEF8h]
  0000000140469C0D: mov         r14,rdx
  0000000140469C10: mov         dword ptr [rbp-55h],eax
  0000000140469C13: mov         eax,dword ptr [rcx+FFFFFFFFFFFFFEFCh]
  0000000140469C19: mov         dword ptr [rbp-51h],eax
  0000000140469C1C: call        0000000140486AA0
  0000000140469C21: lea         rcx,[rdi+0000000000000BD0h]
  0000000140469C28: movss       xmm9,dword ptr [0000000141F697B0h]
  0000000140469C31: xorps       xmm8,xmm8
  0000000140469C35: cmp         al,1
  0000000140469C37: jne         0000000140469C46
  0000000140469C39: call        0000000140486A00
  0000000140469C3E: mov         rdx,rax
  0000000140469C41: jmp         0000000140469CC6
  0000000140469C46: call        00000001404869D0
  0000000140469C4B: movaps      xmm1,xmm8
  0000000140469C4F: mulss       xmm1,xmm8
  0000000140469C54: movss       xmm3,dword ptr [rax]
  0000000140469C58: movss       dword ptr [rbp-69h],xmm3
  0000000140469C5D: movss       xmm4,dword ptr [rax+4]
  0000000140469C62: movaps      xmm2,xmm3
  0000000140469C65: mulss       xmm2,xmm3
  0000000140469C69: movss       dword ptr [rbp-65h],xmm4
  0000000140469C6E: movss       xmm0,dword ptr [rax+8]
  0000000140469C73: movss       dword ptr [rbp-61h],xmm0
  0000000140469C78: movaps      xmm0,xmm4
  0000000140469C7B: mov         dword ptr [rbp-61h],0
  0000000140469C82: mulss       xmm0,xmm4
  0000000140469C86: addss       xmm2,xmm0
  0000000140469C8A: addss       xmm2,xmm1
  0000000140469C8E: movaps      xmm6,xmm2
  0000000140469C91: sqrtss      xmm6,xmm2
  0000000140469C95: comiss      xmm6,dword ptr [0000000141AE8A78h]
  0000000140469C9C: jbe         0000000140469CD8
  0000000140469C9E: movaps      xmm0,xmm9
  0000000140469CA2: lea         rdx,[rbp-69h]
  0000000140469CA6: divss       xmm0,xmm6
  0000000140469CAA: mulss       xmm3,xmm0
  0000000140469CAE: mulss       xmm4,xmm0
  0000000140469CB2: mulss       xmm0,xmm8
  0000000140469CB7: movss       dword ptr [rbp-61h],xmm0
  0000000140469CBC: movss       dword ptr [rbp-69h],xmm3
  0000000140469CC1: movss       dword ptr [rbp-65h],xmm4
  0000000140469CC6: lea         rcx,[rbp-59h]
  0000000140469CCA: call        00000001404875F0
  0000000140469CCF: movaps      xmm6,xmm0
  0000000140469CD2: divss       xmm6,xmm7
  0000000140469CD6: jmp         0000000140469CDC
  0000000140469CD8: movaps      xmm6,xmm8
  0000000140469CDC: mov         rbx,qword ptr [rdi+8]
  0000000140469CE0: lea         rcx,[rdi+0000000000000BD0h]
  0000000140469CE7: call        00000001404869D0
  0000000140469CEC: lea         r9,[rbp-49h]
  0000000140469CF0: 49

  Summary

     1A7D000 .text

````
