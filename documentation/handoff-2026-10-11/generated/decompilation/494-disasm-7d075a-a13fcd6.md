# disasm-7d075a.txt

Original: [work/disasm-7d075a.txt](<../../../../work/disasm-7d075a.txt>)

SHA-256: `359BE48198FE0873537838AE6FDFE25AC1A4EDFFC072156AE6934B6E41FF7B2E`; 4288 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001407D075A: test        dword ptr [rbx+0000000000000138h],0FFFFFFh
  00000001407D0764: jle         00000001407D0795
  00000001407D0766: mov         rsi,rdi
  00000001407D0769: nop         dword ptr [rax+0000000000000000h]
  00000001407D0770: mov         rcx,qword ptr [rbx+0000000000000130h]
  00000001407D0777: mov         rcx,qword ptr [rcx+rsi]
  00000001407D077B: call        00000001407DD690
  00000001407D0780: mov         eax,dword ptr [rbx+0000000000000138h]
  00000001407D0786: inc         edi
  00000001407D0788: and         eax,0FFFFFFh
  00000001407D078D: lea         rsi,[rsi+8]
  00000001407D0791: cmp         edi,eax
  00000001407D0793: jl          00000001407D0770
  00000001407D0795: mov         rcx,qword ptr [rbx+0000000000000150h]
  00000001407D079C: mov         rdi,qword ptr [rsp+40h]
  00000001407D07A1: mov         rsi,qword ptr [rsp+38h]
  00000001407D07A6: test        rcx,rcx
  00000001407D07A9: je          00000001407D07B0
  00000001407D07AB: call        00000001407DD690
  00000001407D07B0: mov         dword ptr [rbx+000000000000027Ch],3
  00000001407D07BA: add         rsp,20h
  00000001407D07BE: pop         rbx
  00000001407D07BF: ret
  00000001407D07C0: mov         qword ptr [rsp+10h],rbx
  00000001407D07C5: push        rsi
  00000001407D07C6: sub         rsp,20h
  00000001407D07CA: xor         ebx,ebx
  00000001407D07CC: test        dword ptr [rcx+00000000000000E8h],0FFFFFFh
  00000001407D07D6: mov         rsi,rcx
  00000001407D07D9: mov         byte ptr [rcx+00000000000002B1h],0
  00000001407D07E0: jle         00000001407D081A
  00000001407D07E2: mov         qword ptr [rsp+30h],rdi
  00000001407D07E7: mov         edi,ebx
  00000001407D07E9: nop         dword ptr [rax+0000000000000000h]
  00000001407D07F0: mov         rcx,qword ptr [rsi+00000000000000E0h]
  00000001407D07F7: mov         rcx,qword ptr [rcx+rdi]
  00000001407D07FB: call        00000001407DBAB0
  00000001407D0800: mov         eax,dword ptr [rsi+00000000000000E8h]
  00000001407D0806: inc         ebx
  00000001407D0808: and         eax,0FFFFFFh
  00000001407D080D: lea         rdi,[rdi+8]
  00000001407D0811: cmp         ebx,eax
  00000001407D0813: jl          00000001407D07F0
  00000001407D0815: mov         rdi,qword ptr [rsp+30h]
  00000001407D081A: mov         rbx,qword ptr [rsp+38h]
  00000001407D081F: add         rsp,20h
  00000001407D0823: pop         rsi
  00000001407D0824: ret
  00000001407D0825: int         3
  00000001407D0826: int         3
  00000001407D0827: int         3
  00000001407D0828: int         3
  00000001407D0829: int         3
  00000001407D082A: int         3
  00000001407D082B: int         3
  00000001407D082C: int         3
  00000001407D082D: int         3
  00000001407D082E: int         3
  00000001407D082F: int         3
  00000001407D0830: mov         qword ptr [rsp+10h],rbx
  00000001407D0835: push        rsi
  00000001407D0836: sub         rsp,20h
  00000001407D083A: xor         ebx,ebx
  00000001407D083C: test        dword ptr [rcx+18h],0FFFFFFh
  00000001407D0843: mov         rsi,rcx
  00000001407D0846: jle         00000001407D0874
  00000001407D0848: mov         qword ptr [rsp+30h],rdi
  00000001407D084D: mov         edi,ebx
  00000001407D084F: nop
  00000001407D0850: mov         rcx,qword ptr [rsi+10h]
  00000001407D0854: mov         rcx,qword ptr [rcx+rdi]
  00000001407D0858: call        00000001407DD770
  00000001407D085D: mov         eax,dword ptr [rsi+18h]
  00000001407D0860: inc         ebx
  00000001407D0862: and         eax,0FFFFFFh
  00000001407D0867: lea         rdi,[rdi+20h]
  00000001407D086B: cmp         ebx,eax
  00000001407D086D: jl          00000001407D0850
  00000001407D086F: mov         rdi,qword ptr [rsp+30h]
  00000001407D0874: mov         rbx,qword ptr [rsp+38h]
  00000001407D0879: add         rsp,20h
  00000001407D087D: pop         rsi
  00000001407D087E: ret
  00000001407D087F: int         3
  00000001407D0880: 44

  Summary

     1A7D000 .text

````
