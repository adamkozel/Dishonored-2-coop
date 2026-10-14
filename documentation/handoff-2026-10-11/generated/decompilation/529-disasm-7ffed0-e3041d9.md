# disasm-7ffed0.txt

Original: [work/disasm-7ffed0.txt](<../../../../work/disasm-7ffed0.txt>)

SHA-256: `083A276CCDE2A668C2C2616A42E09D04F16E783BEE203412371B9688E505E00A`; 2364 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001407FFED0: or          eax,31A53BBh
  00000001407FFED5: mov         r8,rbx
  00000001407FFED8: mov         rdx,r15
  00000001407FFEDB: add         rcx,1F52E8h
  00000001407FFEE2: call        000000014080E910
  00000001407FFEE7: add         rsi,4
  00000001407FFEEB: dec         r14
  00000001407FFEEE: jne         00000001407FFEB0
  00000001407FFEF0: and         dword ptr [rdi+00000000000003A8h],0FF000000h
  00000001407FFEFA: mov         rcx,rdi
  00000001407FFEFD: call        00000001407FDCB0
  00000001407FFF02: mov         eax,dword ptr [rdi+54h]
  00000001407FFF05: mov         ecx,dword ptr [rdi+50h]
  00000001407FFF08: mov         rsi,qword ptr [rsp+00000000000001C0h]
  00000001407FFF10: mov         rbx,qword ptr [rsp+00000000000001B8h]
  00000001407FFF18: cmp         eax,ecx
  00000001407FFF1A: je          00000001407FFF5E
  00000001407FFF1C: mov         dword ptr [rsp+58h],eax
  00000001407FFF20: lea         rax,[rbp-80h]
  00000001407FFF24: mov         dword ptr [rbp-78h],ecx
  00000001407FFF27: lea         r9,[rsp+50h]
  00000001407FFF2C: lea         r8,[00000001439AB580h]
  00000001407FFF33: lea         rdx,[rsp+68h]
  00000001407FFF38: mov         rcx,r15
  00000001407FFF3B: mov         qword ptr [rsp+20h],rax
  00000001407FFF40: mov         byte ptr [rbp-80h],69h
  00000001407FFF44: mov         byte ptr [rsp+50h],69h
  00000001407FFF49: call        00000001408848A0
  00000001407FFF4E: lea         rcx,[rsp+68h]
  00000001407FFF53: call        0000000140882FC0
  00000001407FFF58: mov         eax,dword ptr [rdi+50h]
  00000001407FFF5B: mov         dword ptr [rdi+54h],eax
  00000001407FFF5E: add         rsp,188h
  00000001407FFF65: pop         r15
  00000001407FFF67: pop         r14
  00000001407FFF69: pop         rdi
  00000001407FFF6A: pop         rbp
  00000001407FFF6B: ret
  00000001407FFF6C: lea         rdx,[rbp-80h]
  00000001407FFF70: mov         rcx,rbx
  00000001407FFF73: call        0000000140878230
  00000001407FFF78: lea         rdx,[rbp-60h]
  00000001407FFF7C: lea         rcx,[rsp+30h]

  Summary

     1A7D000 .text

````
