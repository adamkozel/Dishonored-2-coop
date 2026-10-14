# disasm-6e1e70.txt

Original: [work/disasm-6e1e70.txt](<../../../../work/disasm-6e1e70.txt>)

SHA-256: `313BC8AD7B84C7BA9AFAC3EB9161E4E9073BB1197B9337F809B2024165EE4F30`; 5462 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001406E1E70: add         byte ptr [rax],al
  00000001406E1E72: test        dl,dl
  00000001406E1E74: jne         00000001406E1F7E
  00000001406E1E7A: mov         rdx,qword ptr [rbx+8]
  00000001406E1E7E: call        00000001406E1CC0
  00000001406E1E83: mov         rdx,qword ptr [rbx+10h]
  00000001406E1E87: mov         rcx,qword ptr [rsp+0000000000000080h]
  00000001406E1E8F: mov         rdi,rax
  00000001406E1E92: call        00000001406E1CC0
  00000001406E1E97: mov         rcx,rax
  00000001406E1E9A: movzx       eax,byte ptr [rbx+19h]
  00000001406E1E9E: cmp         al,6
  00000001406E1EA0: jne         00000001406E1F63
  00000001406E1EA6: test        rdi,rdi
  00000001406E1EA9: je          00000001406E1F26
  00000001406E1EAB: test        rcx,rcx
  00000001406E1EAE: jne         00000001406E1EF8
  00000001406E1EB0: movss       xmm1,dword ptr [rbx+1Ch]
  00000001406E1EB5: xorps       xmm0,xmm0
  00000001406E1EB8: comiss      xmm1,xmm0
  00000001406E1EBB: jbe         00000001406E1EE8
  00000001406E1EBD: ucomiss     xmm1,dword ptr [0000000141F697B0h]
  00000001406E1EC4: jp          00000001406E1ECE
  00000001406E1EC6: jne         00000001406E1ECE
  00000001406E1EC8: xor         esi,esi
  00000001406E1ECA: mov         edi,esi
  00000001406E1ECC: jmp         00000001406E1EED
  00000001406E1ECE: movzx       eax,byte ptr [rdi]
  00000001406E1ED1: sub         al,2
  00000001406E1ED3: cmp         al,2
  00000001406E1ED5: ja          00000001406E1EE8
  00000001406E1ED7: mov         rax,qword ptr [rdi+8]
  00000001406E1EDB: test        byte ptr [rax+40h],1
  00000001406E1EDF: je          00000001406E1EE8
  00000001406E1EE1: lea         rcx,[000000014398CCA0h]
  00000001406E1EE8: test        rdi,rdi
  00000001406E1EEB: jne         00000001406E1EF8
  00000001406E1EED: mov         rbp,rcx
  00000001406E1EF0: mov         qword ptr [rsp+0000000000000088h],rcx
  00000001406E1EF8: test        rbp,rbp
  00000001406E1EFB: je          00000001406E1F0C
  00000001406E1EFD: test        rcx,rcx
  00000001406E1F00: cmove       rbp,rdi
  00000001406E1F04: mov         qword ptr [rsp+0000000000000088h],rbp
  00000001406E1F0C: cmp         rbp,rbx
  00000001406E1F0F: jne         00000001406E1F19
  00000001406E1F11: mov         qword ptr [rbx+8],rdi
  00000001406E1F15: mov         qword ptr [rbx+10h],rcx
  00000001406E1F19: mov         rbx,qword ptr [rsp+0000000000000088h]
  00000001406E1F21: jmp         00000001406E1FC8
  00000001406E1F26: test        rcx,rcx
  00000001406E1F29: je          00000001406E1EED
  00000001406E1F2B: movss       xmm1,dword ptr [rbx+1Ch]
  00000001406E1F30: movss       xmm0,dword ptr [0000000141F697B0h]
  00000001406E1F38: comiss      xmm0,xmm1
  00000001406E1F3B: jbe         00000001406E1EED
  00000001406E1F3D: xorps       xmm0,xmm0
  00000001406E1F40: ucomiss     xmm1,xmm0
  00000001406E1F43: jp          00000001406E1F47
  00000001406E1F45: je          00000001406E1EED
  00000001406E1F47: movzx       eax,byte ptr [rcx]
  00000001406E1F4A: sub         al,2
  00000001406E1F4C: cmp         al,2
  00000001406E1F4E: ja          00000001406E1EED
  00000001406E1F50: mov         rax,qword ptr [rcx+8]
  00000001406E1F54: test        byte ptr [rax+40h],1
  00000001406E1F58: je          00000001406E1EED
  00000001406E1F5A: lea         rdi,[000000014398CCA0h]
  00000001406E1F61: jmp         00000001406E1EF8
  00000001406E1F63: cmp         al,3
  00000001406E1F65: jne         00000001406E1EE8
  00000001406E1F67: test        rdi,rdi
  00000001406E1F6A: jne         00000001406E1EF8
  00000001406E1F6C: test        rcx,rcx
  00000001406E1F6F: je          00000001406E1EED
  00000001406E1F75: xor         esi,esi
  00000001406E1F77: mov         ecx,esi
  00000001406E1F79: jmp         00000001406E1EED
  00000001406E1F7E: cmp         dl,5
  00000001406E1F81: jne         00000001406E1F9B
  00000001406E1F83: mov         rdx,qword ptr [rbx+8]
  00000001406E1F87: call        00000001406E1CC0
  00000001406E1F8C: xor         esi,esi
  00000001406E1F8E: test        rax,rax
  00000001406E1F91: mov         qword ptr [rbx+8],rax
  00000001406E1F95: cmove       rbx,rsi
  00000001406E1F99: jmp         00000001406E1FC8
  00000001406E1F9B: lea         eax,[rdx-2]
  00000001406E1F9E: cmp         al,2
  00000001406E1FA0: ja          00000001406E1FC8
  00000001406E1FA2: cmp         qword ptr [rbx+8],0
  00000001406E1FA7: jne         00000001406E1FC8
  00000001406E1FA9: cmp         dl,4
  00000001406E1FAC: jne         00000001406E1FB4
  00000001406E1FAE: cmp         byte ptr [rbx+2Ch],0
  00000001406E1FB2: jne         00000001406E1FC8
  00000001406E1FB4: xor         esi,esi
  00000001406E1FB6: mov         rbx,rsi
  00000001406E1FB9: jmp         00000001406E1FC8
  00000001406E1FBB: mov         rbx,rdx
  00000001406E1FBE: mov         rax,rdx
  00000001406E1FC1: add         rsp,68h
  00000001406E1FC5: pop         rbp
  00000001406E1FC6: pop         rbx
  00000001406E1FC7: ret
  00000001406E1FC8: mov         rsi,qword ptr [rsp+60h]
  00000001406E1FCD: 48
  00000001406E1FCE: 8B
  00000001406E1FCF: jl          00000001406E1FF5

  Summary

     1A7D000 .text

````
