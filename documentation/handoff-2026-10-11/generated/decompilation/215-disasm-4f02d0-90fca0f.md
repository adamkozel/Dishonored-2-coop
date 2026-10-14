# disasm-4f02d0.txt

Original: [work/disasm-4f02d0.txt](<../../../../work/disasm-4f02d0.txt>)

SHA-256: `B879079F54D7052390BC88018C0CA65C579084372D5B4E0A8FFB6B3D21CDDDE0`; 5673 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001404F02D0: mov         qword ptr [rsp+8],rbx
  00000001404F02D5: mov         qword ptr [rsp+10h],rbp
  00000001404F02DA: mov         qword ptr [rsp+18h],rsi
  00000001404F02DF: mov         qword ptr [rsp+20h],rdi
  00000001404F02E4: push        r14
  00000001404F02E6: sub         rsp,20h
  00000001404F02EA: mov         rbx,rcx
  00000001404F02ED: mov         rcx,qword ptr [rcx+8]
  00000001404F02F1: call        00000001408832B0
  00000001404F02F6: mov         rcx,rax
  00000001404F02F9: mov         r14,rax
  00000001404F02FC: mov         rdx,qword ptr [rax]
  00000001404F02FF: call        qword ptr [rdx+70h]
  00000001404F0302: mov         rbp,qword ptr [rbx+10h]
  00000001404F0306: xor         edi,edi
  00000001404F0308: mov         qword ptr [rbx+0000000000000138h],rax
  00000001404F030F: mov         rcx,qword ptr [00000001432CDD70h]
  00000001404F0316: mov         edx,edi
  00000001404F0318: mov         r9,qword ptr [rcx+10h]
  00000001404F031C: mov         r8d,edi
  00000001404F031F: mov         rcx,qword ptr [r9]
  00000001404F0322: cmp         byte ptr [rcx],dl
  00000001404F0324: je          00000001404F0352
  00000001404F0326: mov         rcx,r9
  00000001404F0329: nop         dword ptr [rax+0000000000000000h]
  00000001404F0330: cmp         qword ptr [rcx+8],rdi
  00000001404F0334: je          00000001404F040D
  00000001404F033A: inc         rdx
  00000001404F033D: inc         r8d
  00000001404F0340: mov         rcx,rdx
  00000001404F0343: shl         rcx,4
  00000001404F0347: add         rcx,r9
  00000001404F034A: mov         rax,qword ptr [rcx]
  00000001404F034D: cmp         byte ptr [rax],dil
  00000001404F0350: jne         00000001404F0330
  00000001404F0352: mov         rcx,qword ptr [00000001432CDD78h]
  00000001404F0359: mov         rbx,qword ptr [r14]
  00000001404F035C: call        00000001401D9090
  00000001404F0361: mov         rcx,r14
  00000001404F0364: mov         rdx,rax
  00000001404F0367: call        qword ptr [rbx+0000000000000208h]
  00000001404F036D: mov         rcx,qword ptr [00000001432CDD70h]
  00000001404F0374: mov         rdx,rdi
  00000001404F0377: mov         r8,qword ptr [rcx+10h]
  00000001404F037B: mov         rsi,rax
  00000001404F037E: mov         rcx,qword ptr [r8]
  00000001404F0381: cmp         byte ptr [rcx],dl
  00000001404F0383: je          00000001404F03B2
  00000001404F0385: mov         rcx,r8
  00000001404F0388: nop         dword ptr [rax+rax+0000000000000000h]
  00000001404F0390: cmp         qword ptr [rcx+8],1
  00000001404F0395: je          00000001404F042B
  00000001404F039B: inc         rdx
  00000001404F039E: inc         edi
  00000001404F03A0: mov         rcx,rdx
  00000001404F03A3: shl         rcx,4
  00000001404F03A7: add         rcx,r8
  00000001404F03AA: mov         rax,qword ptr [rcx]
  00000001404F03AD: cmp         byte ptr [rax],0
  00000001404F03B0: jne         00000001404F0390
  00000001404F03B2: mov         rcx,qword ptr [00000001432CDD78h]
  00000001404F03B9: mov         rbx,qword ptr [r14]
  00000001404F03BC: call        00000001401D9090
  00000001404F03C1: mov         rcx,r14
  00000001404F03C4: mov         rdx,rax
  00000001404F03C7: call        qword ptr [rbx+0000000000000208h]
  00000001404F03CD: mov         rbx,rax
  00000001404F03D0: test        rsi,rsi
  00000001404F03D3: je          00000001404F03E1
  00000001404F03D5: mov         rdx,qword ptr [rsi]
  00000001404F03D8: mov         rcx,rsi
  00000001404F03DB: call        qword ptr [rdx+0000000000000080h]
  00000001404F03E1: test        rbx,rbx
  00000001404F03E4: je          00000001404F03F2
  00000001404F03E6: mov         rdx,qword ptr [rbx]
  00000001404F03E9: mov         rcx,rbx
  00000001404F03EC: call        qword ptr [rdx+0000000000000080h]
  00000001404F03F2: mov         rbx,qword ptr [rsp+30h]
  00000001404F03F7: mov         rbp,qword ptr [rsp+38h]
  00000001404F03FC: mov         rsi,qword ptr [rsp+40h]
  00000001404F0401: mov         rdi,qword ptr [rsp+48h]
  00000001404F0406: add         rsp,20h
  00000001404F040A: pop         r14
  00000001404F040C: ret
  00000001404F040D: movsxd      rax,r8d
  00000001404F0410: lea         rcx,[rax*4+0000000000000011h]
  00000001404F0418: add         rcx,rax
  00000001404F041B: lea         rcx,[rcx*8+0000000000000000h]
  00000001404F0423: add         rcx,rbp
  00000001404F0426: jmp         00000001404F0359
  00000001404F042B: movsxd      rax,edi
  00000001404F042E: lea         rcx,[rax*4+0000000000000011h]
  00000001404F0436: add         rcx,rax
  00000001404F0439: lea         rcx,[rcx*8+0000000000000000h]
  00000001404F0441: add         rcx,rbp
  00000001404F0444: jmp         00000001404F03B9
  00000001404F0449: int         3
  00000001404F044A: int         3
  00000001404F044B: int         3
  00000001404F044C: int         3
  00000001404F044D: int         3
  00000001404F044E: int         3
  00000001404F044F: int         3
  00000001404F0450: movzx       eax,byte ptr [rcx+0000000000000114h]
  00000001404F0457: ret
  00000001404F0458: int         3
  00000001404F0459: int         3
  00000001404F045A: int         3
  00000001404F045B: int         3
  00000001404F045C: int         3
  00000001404F045D: int         3
  00000001404F045E: int         3
  00000001404F045F: int         3
  00000001404F0460: 45

  Summary

     1A7D000 .text

````
