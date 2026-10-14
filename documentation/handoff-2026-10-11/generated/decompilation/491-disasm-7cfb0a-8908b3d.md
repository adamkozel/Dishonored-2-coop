# disasm-7cfb0a.txt

Original: [work/disasm-7cfb0a.txt](<../../../../work/disasm-7cfb0a.txt>)

SHA-256: `42980FE8C2F6B68D4852104C9B90A9ADDB1B1E9E3F5E5EF7454166FA583CAC09`; 3927 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001407CFB0A: test        dword ptr [rbx+0000000000000138h],0FFFFFFh
  00000001407CFB14: jle         00000001407CFB45
  00000001407CFB16: mov         rsi,rdi
  00000001407CFB19: nop         dword ptr [rax+0000000000000000h]
  00000001407CFB20: mov         rax,qword ptr [rbx+0000000000000130h]
  00000001407CFB27: mov         rcx,qword ptr [rsi+rax]
  00000001407CFB2B: call        00000001407DD360
  00000001407CFB30: mov         eax,dword ptr [rbx+0000000000000138h]
  00000001407CFB36: inc         edi
  00000001407CFB38: and         eax,0FFFFFFh
  00000001407CFB3D: lea         rsi,[rsi+8]
  00000001407CFB41: cmp         edi,eax
  00000001407CFB43: jl          00000001407CFB20
  00000001407CFB45: mov         rcx,qword ptr [rbx+0000000000000150h]
  00000001407CFB4C: mov         rsi,qword ptr [rsp+60h]
  00000001407CFB51: test        rcx,rcx
  00000001407CFB54: je          00000001407CFB5B
  00000001407CFB56: call        00000001407DD360
  00000001407CFB5B: mov         rcx,qword ptr [rbx+0000000000000290h]
  00000001407CFB62: mov         eax,dword ptr [rbx+0000000000000158h]
  00000001407CFB68: mov         dword ptr [rbx+000000000000027Ch],5
  00000001407CFB72: test        rcx,rcx
  00000001407CFB75: je          00000001407CFBA0
  00000001407CFB77: lea         r9,[rsp+20h]
  00000001407CFB7C: lea         r8,[00000001439A7840h]
  00000001407CFB83: lea         rdx,[rsp+38h]
  00000001407CFB88: mov         byte ptr [rsp+20h],6Dh
  00000001407CFB8D: mov         dword ptr [rsp+28h],eax
  00000001407CFB91: call        0000000140884920
  00000001407CFB96: lea         rcx,[rsp+38h]
  00000001407CFB9B: call        0000000140882FC0
  00000001407CFBA0: mov         al,1
  00000001407CFBA2: mov         rbx,qword ptr [rsp+78h]
  00000001407CFBA7: add         rsp,50h
  00000001407CFBAB: pop         rdi
  00000001407CFBAC: ret
  00000001407CFBAD: int         3
  00000001407CFBAE: int         3
  00000001407CFBAF: int         3
  00000001407CFBB0: mov         qword ptr [rsp+10h],rbx
  00000001407CFBB5: push        rsi
  00000001407CFBB6: sub         rsp,20h
  00000001407CFBBA: xor         ebx,ebx
  00000001407CFBBC: test        dword ptr [rcx+00000000000000E8h],0FFFFFFh
  00000001407CFBC6: mov         rsi,rcx
  00000001407CFBC9: mov         byte ptr [rcx+00000000000002B1h],1
  00000001407CFBD0: jle         00000001407CFC0A
  00000001407CFBD2: mov         qword ptr [rsp+30h],rdi
  00000001407CFBD7: mov         edi,ebx
  00000001407CFBD9: nop         dword ptr [rax+0000000000000000h]
  00000001407CFBE0: mov         rcx,qword ptr [rsi+00000000000000E0h]
  00000001407CFBE7: mov         rcx,qword ptr [rcx+rdi]
  00000001407CFBEB: call        00000001407DD530
  00000001407CFBF0: mov         eax,dword ptr [rsi+00000000000000E8h]
  00000001407CFBF6: inc         ebx
  00000001407CFBF8: and         eax,0FFFFFFh
  00000001407CFBFD: lea         rdi,[rdi+8]
  00000001407CFC01: cmp         ebx,eax
  00000001407CFC03: jl          00000001407CFBE0
  00000001407CFC05: mov         rdi,qword ptr [rsp+30h]
  00000001407CFC0A: mov         rbx,qword ptr [rsp+38h]
  00000001407CFC0F: add         rsp,20h
  00000001407CFC13: pop         rsi
  00000001407CFC14: ret
  00000001407CFC15: int         3
  00000001407CFC16: int         3
  00000001407CFC17: int         3
  00000001407CFC18: int         3
  00000001407CFC19: int         3
  00000001407CFC1A: int         3
  00000001407CFC1B: int         3
  00000001407CFC1C: int         3
  00000001407CFC1D: int         3
  00000001407CFC1E: int         3
  00000001407CFC1F: int         3
  00000001407CFC20: 40

  Summary

     1A7D000 .text

````
