# disasm-65acb0.txt

Original: [work/disasm-65acb0.txt](<../../../../work/disasm-65acb0.txt>)

SHA-256: `F653CE9D501898105916A6A03866CDF35B63562D7C3746BF1FB886C4369F077C`; 2012 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  000000014065ACB0: push        rdi
  000000014065ACB2: sub         rsp,50h
  000000014065ACB6: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  000000014065ACBF: mov         qword ptr [rsp+60h],rbx
  000000014065ACC4: mov         rbx,r8
  000000014065ACC7: mov         edi,edx
  000000014065ACC9: lea         rdx,[0000000141C34880h]
  000000014065ACD0: lea         rcx,[rsp+28h]
  000000014065ACD5: call        000000014013F9D0
  000000014065ACDA: nop
  000000014065ACDB: cmp         edi,0FFFFFFFFh
  000000014065ACDE: je          000000014065AD0D
  000000014065ACE0: mov         r8d,edi
  000000014065ACE3: lea         rdx,[0000000141AD8924h]
  000000014065ACEA: lea         rcx,[rsp+28h]
  000000014065ACEF: call        0000000140140AA0
  000000014065ACF4: test        rbx,rbx
  000000014065ACF7: je          000000014065AD0D
  000000014065ACF9: mov         r8,rbx
  000000014065ACFC: lea         rdx,[0000000141B0E3D0h]
  000000014065AD03: lea         rcx,[rsp+28h]
  000000014065AD08: call        0000000140140AA0
  000000014065AD0D: mov         rax,qword ptr [000000014228BAE0h]
  000000014065AD14: mov         rbx,qword ptr [rax]
  000000014065AD17: lea         rcx,[rsp+28h]
  000000014065AD1C: call        00000001401D9090
  000000014065AD21: mov         rdx,rax
  000000014065AD24: mov         rcx,qword ptr [000000014228BAE0h]
  000000014065AD2B: call        qword ptr [rbx+70h]
  000000014065AD2E: nop
  000000014065AD2F: lea         rcx,[rsp+28h]
  000000014065AD34: call        000000014013FD10
  000000014065AD39: mov         rbx,qword ptr [rsp+60h]
  000000014065AD3E: add         rsp,50h
  000000014065AD42: pop         rdi
  000000014065AD43: ret
  000000014065AD44: int         3

  Summary

     1A7D000 .text

````
