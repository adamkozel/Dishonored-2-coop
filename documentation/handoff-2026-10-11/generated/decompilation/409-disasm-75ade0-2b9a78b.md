# disasm-75ade0.txt

Original: [work/disasm-75ade0.txt](<../../../../work/disasm-75ade0.txt>)

SHA-256: `C4EBB6CA3FCBB0B1C4D6B81A0A7D3EE5954F4B0DF52D5B93158D844269D5697D`; 6027 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  000000014075ADE0: push        rbx
  000000014075ADE2: sub         rsp,20h
  000000014075ADE6: mov         rbx,rcx
  000000014075ADE9: call        00000001407E7400
  000000014075ADEE: mov         rax,qword ptr [rbx+10h]
  000000014075ADF2: test        rax,rax
  000000014075ADF5: jne         000000014075AE00
  000000014075ADF7: mov         dword ptr [rbx+50h],3FFF0010h
  000000014075ADFE: jmp         000000014075AE09
  000000014075AE00: mov         eax,dword ptr [rax+00000000000000A8h]
  000000014075AE06: mov         dword ptr [rbx+50h],eax
  000000014075AE09: mov         rcx,qword ptr [0000000142286080h]
  000000014075AE10: call        000000014023A2E0
  000000014075AE15: mov         rcx,rax
  000000014075AE18: call        00000001400C7B50
  000000014075AE1D: mov         rcx,rbx
  000000014075AE20: mov         qword ptr [rbx+58h],rax
  000000014075AE24: call        000000014075AA20
  000000014075AE29: test        rax,rax
  000000014075AE2C: jne         000000014075AE53
  000000014075AE2E: mov         rcx,rbx
  000000014075AE31: call        000000014075B250
  000000014075AE36: test        rax,rax
  000000014075AE39: je          000000014075AE53
  000000014075AE3B: mov         eax,dword ptr [rax+00000000000000A8h]
  000000014075AE41: lea         rcx,[rbx+40h]
  000000014075AE45: lea         rdx,[rsp+30h]
  000000014075AE4A: mov         dword ptr [rsp+30h],eax
  000000014075AE4E: call        000000014052E460
  000000014075AE53: add         rsp,20h
  000000014075AE57: pop         rbx
  000000014075AE58: ret
  000000014075AE59: int         3
  000000014075AE5A: int         3
  000000014075AE5B: int         3
  000000014075AE5C: int         3
  000000014075AE5D: int         3
  000000014075AE5E: int         3
  000000014075AE5F: int         3
  000000014075AE60: mov         qword ptr [rsp+8],rbx
  000000014075AE65: mov         qword ptr [rsp+10h],rbp
  000000014075AE6A: mov         qword ptr [rsp+18h],rsi
  000000014075AE6F: push        rdi
  000000014075AE70: sub         rsp,60h
  000000014075AE74: mov         rax,qword ptr [rdx+8]
  000000014075AE78: mov         rbx,rcx
  000000014075AE7B: movaps      xmmword ptr [rsp+50h],xmm6
  000000014075AE80: movzx       ecx,byte ptr [rax+0Ch]
  000000014075AE84: mov         rbp,qword ptr [rax]
  000000014075AE87: mov         rdi,rdx
  000000014075AE8A: movss       xmm6,dword ptr [rax+8]
  000000014075AE8F: mov         rax,qword ptr [rbx+8]
  000000014075AE93: movzx       esi,cl
  000000014075AE96: shr         cl,1
  000000014075AE98: and         sil,1
  000000014075AE9C: and         cl,1
  000000014075AE9F: je          000000014075AEAE
  000000014075AEA1: cmp         byte ptr [rax+00000000000000A4h],0
  000000014075AEA8: je          000000014075AEAE
  000000014075AEAA: mov         dl,1
  000000014075AEAC: jmp         000000014075AEB0
  000000014075AEAE: xor         dl,dl
  000000014075AEB0: test        cl,cl
  000000014075AEB2: jne         000000014075AEC0
  000000014075AEB4: cmp         byte ptr [rax+00000000000000A3h],cl
  000000014075AEBA: je          000000014075AEC0
  000000014075AEBC: mov         al,1
  000000014075AEBE: jmp         000000014075AEC2
  000000014075AEC0: xor         al,al
  000000014075AEC2: test        dl,dl
  000000014075AEC4: jne         000000014075AECE
  000000014075AEC6: test        al,al
  000000014075AEC8: je          000000014075AF5A
  000000014075AECE: mov         rdx,qword ptr [rbx+10h]
  000000014075AED2: lea         rcx,[rsp+40h]
  000000014075AED7: call        00000001405BED20
  000000014075AEDC: movsd       xmm1,mmword ptr [rbx+00000000000000A0h]
  000000014075AEE4: xorps       xmm0,xmm0
  000000014075AEE7: comisd      xmm1,xmm0
  000000014075AEEB: jb          000000014075AF04
  000000014075AEED: movsd       xmm0,mmword ptr [rsp+40h]
  000000014075AEF3: subsd       xmm0,xmm1
  000000014075AEF7: cvtpd2ps    xmm0,xmm0
  000000014075AEFB: comiss      xmm0,dword ptr [0000000141F697B0h]
  000000014075AF02: jbe         000000014075AF5A
  000000014075AF04: mov         rax,qword ptr [rsp+40h]
  000000014075AF09: mov         byte ptr [rsp+30h],sil
  000000014075AF0E: lea         rdx,[rbx+60h]
  000000014075AF12: movss       dword ptr [rsp+28h],xmm6
  000000014075AF18: mov         qword ptr [rbx+00000000000000A0h],rax
  000000014075AF1F: mov         eax,dword ptr [rsp+48h]
  000000014075AF23: mov         qword ptr [rsp+20h],rbp
  000000014075AF28: mov         dword ptr [rbx+00000000000000A8h],eax
  000000014075AF2E: mov         r8,qword ptr [rbx+10h]
  000000014075AF32: mov         rcx,qword ptr [rbx+58h]
  000000014075AF36: lea         r9,[r8+0000000000000300h]
  000000014075AF3D: call        00000001406DF940
  000000014075AF42: cmp         eax,3
  000000014075AF45: je          000000014075AF5A
  000000014075AF47: mov         rcx,rdi
  000000014075AF4A: call        0000000140810210
  000000014075AF4F: mov         rcx,rbx
  000000014075AF52: mov         rdx,rax
  000000014075AF55: call        00000001407598E0
  000000014075AF5A: mov         rbx,qword ptr [rsp+70h]
  000000014075AF5F: mov         rbp,qword ptr [rsp+78h]
  000000014075AF64: mov         rsi,qword ptr [rsp+0000000000000080h]
  000000014075AF6C: movaps      xmm6,xmmword ptr [rsp+50h]
  000000014075AF71: add         rsp,60h
  000000014075AF75: pop         rdi
  000000014075AF76: ret
  000000014075AF77: int         3
  000000014075AF78: int         3
  000000014075AF79: int         3
  000000014075AF7A: int         3
  000000014075AF7B: int         3
  000000014075AF7C: int         3
  000000014075AF7D: int         3
  000000014075AF7E: int         3
  000000014075AF7F: int         3
  000000014075AF80: 48

  Summary

     1A7D000 .text

````
