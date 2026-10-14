# disasm-78a6c0.txt

Original: [work/disasm-78a6c0.txt](<../../../../work/disasm-78a6c0.txt>)

SHA-256: `FE4F430C1A85C6F898663D8D76D84F834E3D3A5F9C1B0F3D045157ABB25401AB`; 5615 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  000000014078A6C0: mov         qword ptr [rsp+10h],rbx
  000000014078A6C5: mov         qword ptr [rsp+18h],rsi
  000000014078A6CA: push        rdi
  000000014078A6CB: sub         rsp,20h
  000000014078A6CF: mov         rsi,rdx
  000000014078A6D2: mov         rdi,rcx
  000000014078A6D5: mov         r9,rcx
  000000014078A6D8: lea         rdx,[0000000141AC71E0h]
  000000014078A6DF: lea         r8,[000000014078A5B0h]
  000000014078A6E6: mov         rcx,rsi
  000000014078A6E9: call        000000014002CD20
  000000014078A6EE: lea         rdx,[rsp+30h]
  000000014078A6F3: lea         rcx,[rdi+60h]
  000000014078A6F7: mov         dword ptr [rsp+30h],eax
  000000014078A6FB: call        000000014001D6B0
  000000014078A700: lea         r8,[000000014078A680h]
  000000014078A707: lea         rdx,[0000000141AC7200h]
  000000014078A70E: mov         r9,rdi
  000000014078A711: mov         rcx,rsi
  000000014078A714: call        000000014002CD20
  000000014078A719: lea         rdx,[rsp+30h]
  000000014078A71E: lea         rcx,[rdi+60h]
  000000014078A722: mov         dword ptr [rsp+30h],eax
  000000014078A726: call        000000014001D6B0
  000000014078A72B: mov         rbx,qword ptr [rsp+38h]
  000000014078A730: mov         rsi,qword ptr [rsp+40h]
  000000014078A735: add         rsp,20h
  000000014078A739: pop         rdi
  000000014078A73A: ret
  000000014078A73B: int         3
  000000014078A73C: int         3
  000000014078A73D: int         3
  000000014078A73E: int         3
  000000014078A73F: int         3
  000000014078A740: and         byte ptr [rcx+00000000000009C0h],0EFh
  000000014078A747: ret
  000000014078A748: int         3
  000000014078A749: int         3
  000000014078A74A: int         3
  000000014078A74B: int         3
  000000014078A74C: int         3
  000000014078A74D: int         3
  000000014078A74E: int         3
  000000014078A74F: int         3
  000000014078A750: push        rbx
  000000014078A752: sub         rsp,70h
  000000014078A756: cmp         qword ptr [rcx+20h],0
  000000014078A75B: mov         rbx,rcx
  000000014078A75E: je          000000014078A7A4
  000000014078A760: lea         r8,[rsp+58h]
  000000014078A765: lea         rdx,[rsp+38h]
  000000014078A76A: lea         rcx,[rsp+20h]
  000000014078A76F: mov         r9d,1
  000000014078A775: call        0000000140028980
  000000014078A77A: lea         rdx,[rbx+00000000000003E0h]
  000000014078A781: lea         rcx,[rbx+0000000000000688h]
  000000014078A788: lea         r8,[rsp+20h]
  000000014078A78D: call        0000000140028A90
  000000014078A792: and         byte ptr [rbx+00000000000009C0h],0DFh
  000000014078A799: and         al,1
  000000014078A79B: shl         al,5
  000000014078A79E: or          byte ptr [rbx+00000000000009C0h],al
  000000014078A7A4: add         rsp,70h
  000000014078A7A8: pop         rbx
  000000014078A7A9: ret
  000000014078A7AA: int         3
  000000014078A7AB: int         3
  000000014078A7AC: int         3
  000000014078A7AD: int         3
  000000014078A7AE: int         3
  000000014078A7AF: int         3
  000000014078A7B0: push        rbx
  000000014078A7B2: sub         rsp,0C0h
  000000014078A7B9: cmp         qword ptr [rcx+20h],0
  000000014078A7BE: mov         rbx,rcx
  000000014078A7C1: je          000000014078A857
  000000014078A7C7: lea         r8,[rsp+58h]
  000000014078A7CC: lea         rdx,[rsp+38h]
  000000014078A7D1: lea         rcx,[rsp+20h]
  000000014078A7D6: mov         r9d,1
  000000014078A7DC: call        0000000140028980
  000000014078A7E1: call        0000000140034920
  000000014078A7E6: lea         rdx,[rsp+00000000000000D0h]
  000000014078A7EE: lea         rcx,[rsp+20h]
  000000014078A7F3: mov         dword ptr [rsp+00000000000000D0h],eax
  000000014078A7FA: call        0000000140028700
  000000014078A7FF: lea         rdx,[rbx+00000000000006E8h]
  000000014078A806: lea         rcx,[rbx+0000000000000990h]
  000000014078A80D: lea         r8,[rsp+20h]
  000000014078A812: call        0000000140028A90
  000000014078A817: lea         r8,[rsp+00000000000000A8h]
  000000014078A81F: lea         rdx,[rsp+0000000000000088h]
  000000014078A827: lea         rcx,[rsp+70h]
  000000014078A82C: mov         r9d,1
  000000014078A832: call        0000000140028980
  000000014078A837: lea         rdx,[rbx+78h]
  000000014078A83B: lea         rcx,[rbx+0000000000000320h]
  000000014078A842: lea         r8,[rsp+70h]
  000000014078A847: call        0000000140028A90
  000000014078A84C: mov         al,1
  000000014078A84E: add         rsp,0C0h
  000000014078A855: pop         rbx
  000000014078A856: ret
  000000014078A857: xor         al,al
  000000014078A859: add         rsp,0C0h
  000000014078A860: pop         rbx
  000000014078A861: ret
  000000014078A862: int         3
  000000014078A863: int         3
  000000014078A864: int         3
  000000014078A865: int         3
  000000014078A866: int         3
  000000014078A867: int         3
  000000014078A868: int         3
  000000014078A869: int         3
  000000014078A86A: int         3
  000000014078A86B: int         3
  000000014078A86C: int         3
  000000014078A86D: int         3
  000000014078A86E: int         3
  000000014078A86F: int         3
  000000014078A870: 48

  Summary

     1A7D000 .text

````
