# disasm-882ca0.txt

Original: [work/disasm-882ca0.txt](<../../../../work/disasm-882ca0.txt>)

SHA-256: `7290F9C1E025A2AE9A5ACA76F7C4DD29E9325F01F966B5F64A2B7D21DBC54A67`; 1004 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  0000000140882CA0: push        rbx
  0000000140882CA2: sub         rsp,20h
  0000000140882CA6: mov         dword ptr [rcx+00000000000004C0h],edx
  0000000140882CAC: cmp         edx,1
  0000000140882CAF: mov         ebx,edx
  0000000140882CB1: lea         rcx,[00000001432CE500h]
  0000000140882CB8: mov         r8b,1
  0000000140882CBB: sete        dl
  0000000140882CBE: call        0000000140102520
  0000000140882CC3: xor         eax,eax
  0000000140882CC5: cmp         ebx,1
  0000000140882CC8: sete        al
  0000000140882CCB: mov         dword ptr [0000000142280A7Ch],eax
  0000000140882CD1: add         rsp,20h
  0000000140882CD5: pop         rbx
  0000000140882CD6: ret
  0000000140882CD7: int         3

  Summary

     1A7D000 .text

````
