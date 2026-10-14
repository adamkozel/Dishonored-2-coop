# disasm-7be4f0.txt

Original: [work/disasm-7be4f0.txt](<../../../../work/disasm-7be4f0.txt>)

SHA-256: `BAB920E58AC987EB4ECD442F24B2B65EF2784B094B264FD94288EB597438D71E`; 5362 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001407BE4F0: mov         qword ptr [rsp+18h],rbp
  00000001407BE4F5: mov         qword ptr [rsp+20h],rdi
  00000001407BE4FA: push        r14
  00000001407BE4FC: sub         rsp,40h
  00000001407BE500: mov         rax,qword ptr [rcx+28h]
  00000001407BE504: xor         ebp,ebp
  00000001407BE506: mov         r10,r8
  00000001407BE509: mov         r14,rdx
  00000001407BE50C: mov         rdi,rcx
  00000001407BE50F: mov         dword ptr [rsp+20h],ebp
  00000001407BE513: test        rax,rax
  00000001407BE516: je          00000001407BE626
  00000001407BE51C: mov         qword ptr [rsp+50h],rbx
  00000001407BE521: mov         rbx,qword ptr [rax+40h]
  00000001407BE525: mov         eax,dword ptr [rax+48h]
  00000001407BE528: and         eax,0FFFFFFh
  00000001407BE52D: lea         rax,[rax+rax*8]
  00000001407BE531: lea         rax,[rbx+rax*8]
  00000001407BE535: cmp         rbx,rax
  00000001407BE538: je          00000001407BE621
  00000001407BE53E: nop
  00000001407BE540: mov         rcx,qword ptr [rbx+20h]
  00000001407BE544: test        rcx,rcx
  00000001407BE547: je          00000001407BE555
  00000001407BE549: cmp         qword ptr [rcx+20h],r8
  00000001407BE54D: jne         00000001407BE555
  00000001407BE54F: cmp         qword ptr [rcx+28h],rbp
  00000001407BE553: jne         00000001407BE563
  00000001407BE555: add         rbx,48h
  00000001407BE559: cmp         rbx,rax
  00000001407BE55C: jne         00000001407BE540
  00000001407BE55E: jmp         00000001407BE621
  00000001407BE563: mov         rax,qword ptr [rcx+28h]
  00000001407BE567: mov         r8,r9
  00000001407BE56A: mov         rdx,r10
  00000001407BE56D: mov         rcx,rdi
  00000001407BE570: mov         qword ptr [rsp+58h],rsi
  00000001407BE575: call        rax
  00000001407BE577: mov         rdx,rbx
  00000001407BE57A: mov         rcx,rdi
  00000001407BE57D: movzx       esi,al
  00000001407BE580: call        00000001407BEA70
  00000001407BE585: test        sil,sil
  00000001407BE588: jne         00000001407BE5B7
  00000001407BE58A: mov         rax,qword ptr [rdi+28h]
  00000001407BE58E: lea         r8,[rsp+24h]
  00000001407BE593: mov         r9b,1
  00000001407BE596: mov         ecx,dword ptr [rax+8]
  00000001407BE599: mov         eax,dword ptr [rbx+8]
  00000001407BE59C: mov         rdx,rdi
  00000001407BE59F: mov         dword ptr [rsp+2Ch],ecx
  00000001407BE5A3: mov         rcx,qword ptr [rdi+20h]
  00000001407BE5A7: mov         qword ptr [rsp+24h],rbp
  00000001407BE5AC: mov         dword ptr [rsp+30h],eax
  00000001407BE5B0: call        00000001407BB9D0
  00000001407BE5B5: jmp         00000001407BE61C
  00000001407BE5B7: cmp         sil,1
  00000001407BE5BB: jne         00000001407BE5EB
  00000001407BE5BD: mov         rax,qword ptr [rdi+28h]
  00000001407BE5C1: lea         r8,[rsp+24h]
  00000001407BE5C6: movzx       r9d,sil
  00000001407BE5CA: mov         ecx,dword ptr [rax+8]
  00000001407BE5CD: mov         eax,dword ptr [rbx+8]
  00000001407BE5D0: mov         rdx,rdi
  00000001407BE5D3: mov         dword ptr [rsp+2Ch],ecx
  00000001407BE5D7: mov         rcx,qword ptr [rdi+20h]
  00000001407BE5DB: mov         qword ptr [rsp+24h],rbp
  00000001407BE5E0: mov         dword ptr [rsp+30h],eax
  00000001407BE5E4: call        00000001407BB950
  00000001407BE5E9: jmp         00000001407BE61C
  00000001407BE5EB: cmp         sil,3
  00000001407BE5EF: jne         00000001407BE61C
  00000001407BE5F1: mov         rax,qword ptr [rdi+28h]
  00000001407BE5F5: lea         r8,[rsp+24h]
  00000001407BE5FA: mov         r9b,1
  00000001407BE5FD: mov         ecx,dword ptr [rax+8]
  00000001407BE600: mov         eax,dword ptr [rbx+8]
  00000001407BE603: mov         rdx,rdi
  00000001407BE606: mov         dword ptr [rsp+2Ch],ecx
  00000001407BE60A: mov         rcx,qword ptr [rdi+20h]
  00000001407BE60E: mov         qword ptr [rsp+24h],rbp
  00000001407BE613: mov         dword ptr [rsp+30h],eax
  00000001407BE617: call        00000001407BBB70
  00000001407BE61C: mov         rsi,qword ptr [rsp+58h]
  00000001407BE621: mov         rbx,qword ptr [rsp+50h]
  00000001407BE626: mov         rdi,qword ptr [rsp+68h]
  00000001407BE62B: mov         byte ptr [r14],bpl
  00000001407BE62E: mov         qword ptr [r14+8],rbp
  00000001407BE632: mov         rbp,qword ptr [rsp+60h]
  00000001407BE637: mov         rax,r14
  00000001407BE63A: add         rsp,40h
  00000001407BE63E: pop         r14
  00000001407BE640: ret
  00000001407BE641: int         3
  00000001407BE642: int         3
  00000001407BE643: int         3
  00000001407BE644: int         3
  00000001407BE645: int         3
  00000001407BE646: int         3
  00000001407BE647: int         3
  00000001407BE648: int         3
  00000001407BE649: int         3
  00000001407BE64A: int         3
  00000001407BE64B: int         3
  00000001407BE64C: int         3
  00000001407BE64D: int         3
  00000001407BE64E: int         3
  00000001407BE64F: int         3
  00000001407BE650: 48

  Summary

     1A7D000 .text

````
