# disasm-7e75b0.txt

Original: [work/disasm-7e75b0.txt](<../../../../work/disasm-7e75b0.txt>)

SHA-256: `5EBF83C6D6C1BB1AF23F0D9F02C167CFEE2C560E0A12C2275BD980498CB5DBCF`; 1460 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001407E75B0: push        rbx
  00000001407E75B2: sub         rsp,20h
  00000001407E75B6: and         dword ptr [rcx+1Ch],0FFFFFFEFh
  00000001407E75BA: mov         rax,qword ptr [rcx]
  00000001407E75BD: or          dword ptr [rcx+1Ch],20h
  00000001407E75C1: mov         rbx,rcx
  00000001407E75C4: call        qword ptr [rax+0000000000000088h]
  00000001407E75CA: mov         rdx,rbx
  00000001407E75CD: mov         rcx,rax
  00000001407E75D0: call        000000014048FF60
  00000001407E75D5: mov         eax,dword ptr [rbx+1Ch]
  00000001407E75D8: test        eax,7FFFFC0h
  00000001407E75DD: jne         00000001407E7600
  00000001407E75DF: or          eax,30h
  00000001407E75E2: mov         rdx,rbx
  00000001407E75E5: mov         dword ptr [rbx+1Ch],eax
  00000001407E75E8: mov         rcx,qword ptr [00000001439A5290h]
  00000001407E75EF: mov         rcx,qword ptr [rcx+00000000001E8960h]
  00000001407E75F6: add         rsp,20h
  00000001407E75FA: pop         rbx
  00000001407E75FB: jmp         00000001407EB3E0
  00000001407E7600: add         rsp,20h
  00000001407E7604: pop         rbx
  00000001407E7605: ret
  00000001407E7606: int         3

  Summary

     1A7D000 .text

````
