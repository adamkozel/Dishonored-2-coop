# disasm-872bb0.txt

Original: [work/disasm-872bb0.txt](<../../../../work/disasm-872bb0.txt>)

SHA-256: `458574189E6005B2BAF0CFA352C0FA2DAC5657A6FD3EBAE4374E01036B023E41`; 3564 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  0000000140872BB0: mov         qword ptr [rsp+10h],rbx
  0000000140872BB5: mov         qword ptr [rsp+18h],rbp
  0000000140872BBA: push        rdi
  0000000140872BBB: sub         rsp,30h
  0000000140872BBF: mov         rbx,rcx
  0000000140872BC2: mov         rcx,qword ptr [rcx]
  0000000140872BC5: xor         ebp,ebp
  0000000140872BC7: test        rcx,rcx
  0000000140872BCA: je          0000000140872BDE
  0000000140872BCC: cmp         rcx,0C350h
  0000000140872BD3: jbe         0000000140872BDB
  0000000140872BD5: mov         rax,qword ptr [rcx]
  0000000140872BD8: call        qword ptr [rax+30h]
  0000000140872BDB: mov         qword ptr [rbx],rbp
  0000000140872BDE: test        dword ptr [rbx+70h],0FFFFFFh
  0000000140872BE5: mov         edi,ebp
  0000000140872BE7: jle         0000000140872C1F
  0000000140872BE9: mov         qword ptr [rsp+40h],rsi
  0000000140872BEE: mov         rsi,rbp
  0000000140872BF1: mov         rax,qword ptr [rbx+68h]
  0000000140872BF5: mov         rcx,qword ptr [rsi+rax]
  0000000140872BF9: cmp         rcx,0C351h
  0000000140872C00: jb          0000000140872C08
  0000000140872C02: mov         rax,qword ptr [rcx]
  0000000140872C05: call        qword ptr [rax+30h]
  0000000140872C08: mov         eax,dword ptr [rbx+70h]
  0000000140872C0B: inc         edi
  0000000140872C0D: add         rsi,8
  0000000140872C11: and         eax,0FFFFFFh
  0000000140872C16: cmp         edi,eax
  0000000140872C18: jl          0000000140872BF1
  0000000140872C1A: mov         rsi,qword ptr [rsp+40h]
  0000000140872C1F: mov         ecx,dword ptr [rbx+74h]
  0000000140872C22: mov         eax,ecx
  0000000140872C24: shr         eax,8
  0000000140872C27: test        eax,eax
  0000000140872C29: jns         0000000140872C73
  0000000140872C2B: test        cl,40h
  0000000140872C2E: jne         0000000140872C6C
  0000000140872C30: mov         rdi,qword ptr [rbx+68h]
  0000000140872C34: test        rdi,rdi
  0000000140872C37: je          0000000140872C61
  0000000140872C39: call        00000001401548E0
  0000000140872C3E: lea         rdx,[0000000141AB2230h]
  0000000140872C45: mov         r9,rdi
  0000000140872C48: mov         r10,qword ptr [rax]
  0000000140872C4B: mov         r8d,1ECh
  0000000140872C51: mov         rcx,rax
  0000000140872C54: mov         qword ptr [rsp+20h],10h
  0000000140872C5D: call        qword ptr [r10+10h]
  0000000140872C61: and         dword ptr [rbx+74h],0FFh
  0000000140872C68: mov         qword ptr [rbx+68h],rbp
  0000000140872C6C: and         dword ptr [rbx+70h],0FF000000h
  0000000140872C73: and         dword ptr [rbx+70h],0FF000000h
  0000000140872C7A: mov         eax,dword ptr [rbx+74h]
  0000000140872C7D: shr         eax,8
  0000000140872C80: test        eax,eax
  0000000140872C82: cmovg       eax,ebp
  0000000140872C85: mov         rbp,qword ptr [rsp+50h]
  0000000140872C8A: and         eax,0FFFFFFh
  0000000140872C8F: or          dword ptr [rbx+70h],eax
  0000000140872C92: mov         rbx,qword ptr [rsp+48h]
  0000000140872C97: add         rsp,30h
  0000000140872C9B: pop         rdi
  0000000140872C9C: ret
  0000000140872C9D: int         3
  0000000140872C9E: int         3
  0000000140872C9F: int         3
  0000000140872CA0: 8B

  Summary

     1A7D000 .text

````
