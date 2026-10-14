# disasm-484ab0.txt

Original: [work/disasm-484ab0.txt](<../../../../work/disasm-484ab0.txt>)

SHA-256: `2B494B990FA9CFD398F97935B8A16030F4FBDF2302C77078BFD635E540DEAC9A`; 6283 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  0000000140484AB0: push        rbx
  0000000140484AB2: push        rdi
  0000000140484AB3: sub         rsp,38h
  0000000140484AB7: mov         rdi,rdx
  0000000140484ABA: mov         rbx,rcx
  0000000140484ABD: call        0000000140485D40
  0000000140484AC2: cmp         al,1
  0000000140484AC4: je          0000000140484C75
  0000000140484ACA: mov         rax,qword ptr [rbx+0000000000000280h]
  0000000140484AD1: mov         qword ptr [rsp+50h],rbp
  0000000140484AD6: mov         qword ptr [rsp+58h],rsi
  0000000140484ADB: mov         r8,qword ptr [rax]
  0000000140484ADE: mov         qword ptr [rsp+68h],r12
  0000000140484AE3: mov         qword ptr [rsp+30h],r13
  0000000140484AE8: mov         r13d,dword ptr [r8+0000000000000D80h]
  0000000140484AEF: mov         r12d,dword ptr [r8+0000000000000D84h]
  0000000140484AF6: mov         rcx,rdi
  0000000140484AF9: mov         qword ptr [rsp+28h],r14
  0000000140484AFE: mov         qword ptr [rsp+20h],r15
  0000000140484B03: call        0000000140486AB0
  0000000140484B08: mov         rcx,rdi
  0000000140484B0B: movzx       r15d,al
  0000000140484B0F: call        0000000140486A60
  0000000140484B14: xor         bpl,bpl
  0000000140484B17: movzx       r14d,al
  0000000140484B1B: cmp         r15b,1
  0000000140484B1F: jne         0000000140484B38
  0000000140484B21: mov         rcx,rdi
  0000000140484B24: call        0000000140486D70
  0000000140484B29: mov         rcx,rdi
  0000000140484B2C: movzx       ebp,al
  0000000140484B2F: call        0000000140486A20
  0000000140484B34: mov         esi,eax
  0000000140484B36: jmp         0000000140484B3B
  0000000140484B38: mov         esi,r13d
  0000000140484B3B: cmp         r14b,1
  0000000140484B3F: jne         0000000140484B58
  0000000140484B41: mov         rcx,rdi
  0000000140484B44: call        0000000140486D60
  0000000140484B49: mov         rcx,rdi
  0000000140484B4C: or          bpl,al
  0000000140484B4F: call        0000000140486A10
  0000000140484B54: mov         edx,eax
  0000000140484B56: jmp         0000000140484B5B
  0000000140484B58: mov         edx,r12d
  0000000140484B5B: test        r15b,r15b
  0000000140484B5E: mov         r15,qword ptr [rsp+20h]
  0000000140484B63: jne         0000000140484B6E
  0000000140484B65: test        r14b,r14b
  0000000140484B68: je          0000000140484C5C
  0000000140484B6E: test        esi,esi
  0000000140484B70: js          0000000140484C5C
  0000000140484B76: cmp         esi,dword ptr [rbx+00000000000002D8h]
  0000000140484B7C: jge         0000000140484C5C
  0000000140484B82: test        edx,edx
  0000000140484B84: js          0000000140484C5C
  0000000140484B8A: mov         eax,dword ptr [rbx+00000000000002DCh]
  0000000140484B90: cmp         edx,eax
  0000000140484B92: jge         0000000140484C5C
  0000000140484B98: imul        eax,esi
  0000000140484B9B: add         eax,edx
  0000000140484B9D: js          0000000140484C5C
  0000000140484BA3: movsxd      rcx,eax
  0000000140484BA6: mov         rax,qword ptr [rbx+00000000000002D0h]
  0000000140484BAD: mov         rdi,qword ptr [rax+rcx*8]
  0000000140484BB1: test        rdi,rdi
  0000000140484BB4: je          0000000140484C5C
  0000000140484BBA: mov         eax,dword ptr [rbx+0000000000000288h]
  0000000140484BC0: and         eax,0FFFFFFh
  0000000140484BC5: cmp         eax,1
  0000000140484BC8: setg        cl
  0000000140484BCB: cmp         r13d,esi
  0000000140484BCE: je          0000000140484BD5
  0000000140484BD0: cmp         r12d,edx
  0000000140484BD3: je          0000000140484BE0
  0000000140484BD5: cmp         bpl,1
  0000000140484BD9: je          0000000140484BE0
  0000000140484BDB: xor         sil,sil
  0000000140484BDE: jmp         0000000140484BE3
  0000000140484BE0: mov         sil,1
  0000000140484BE3: mov         rax,qword ptr [rbx+0000000000000280h]
  0000000140484BEA: cmp         rdi,qword ptr [rax]
  0000000140484BED: je          0000000140484C3D
  0000000140484BEF: cmp         cl,1
  0000000140484BF2: jne         0000000140484C14
  0000000140484BF4: cmp         bpl,cl
  0000000140484BF7: jne         0000000140484C5C
  0000000140484BF9: xorps       xmm0,xmm0
  0000000140484BFC: mov         rdx,rdi
  0000000140484BFF: mov         rcx,rbx
  0000000140484C02: movss       dword ptr [rsp+60h],xmm0
  0000000140484C08: mov         r8d,dword ptr [rsp+60h]
  0000000140484C0D: call        0000000140484760
  0000000140484C12: jmp         0000000140484C55
  0000000140484C14: cmp         bpl,1
  0000000140484C18: jne         0000000140484C1F
  0000000140484C1A: xorps       xmm0,xmm0
  0000000140484C1D: jmp         0000000140484C27
  0000000140484C1F: movss       xmm0,dword ptr [0000000141ACCF9Ch]
  0000000140484C27: movss       dword ptr [rsp+60h],xmm0
  0000000140484C2D: mov         r8d,dword ptr [rsp+60h]
  0000000140484C32: mov         rdx,rdi
  0000000140484C35: mov         rcx,rbx
  0000000140484C38: call        0000000140484760
  0000000140484C3D: cmp         qword ptr [rbx+0000000000000290h],rdi
  0000000140484C44: je          0000000140484C5C
  0000000140484C46: cmp         sil,1
  0000000140484C4A: je          0000000140484C55
  0000000140484C4C: test        byte ptr [rbx+00000000000002CCh],2
  0000000140484C53: jne         0000000140484C5C
  0000000140484C55: mov         qword ptr [rbx+0000000000000290h],rdi
  0000000140484C5C: mov         r13,qword ptr [rsp+30h]
  0000000140484C61: mov         r12,qword ptr [rsp+68h]
  0000000140484C66: mov         rsi,qword ptr [rsp+58h]
  0000000140484C6B: mov         rbp,qword ptr [rsp+50h]
  0000000140484C70: mov         r14,qword ptr [rsp+28h]
  0000000140484C75: add         rsp,38h
  0000000140484C79: pop         rdi
  0000000140484C7A: pop         rbx
  0000000140484C7B: ret
  0000000140484C7C: int         3
  0000000140484C7D: int         3
  0000000140484C7E: int         3
  0000000140484C7F: int         3
  0000000140484C80: 48

  Summary

     1A7D000 .text

````
