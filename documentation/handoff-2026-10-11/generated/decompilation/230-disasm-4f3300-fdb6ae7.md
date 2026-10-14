# disasm-4f3300.txt

Original: [work/disasm-4f3300.txt](<../../../../work/disasm-4f3300.txt>)

SHA-256: `33C44660FFA6661A25C230F1CF1A6F6AED259906FCB8AE0DB78C33ADFCADD923`; 3451 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001404F3300: push        rbp
  00000001404F3302: push        rsi
  00000001404F3303: push        rdi
  00000001404F3304: push        r12
  00000001404F3306: push        r13
  00000001404F3308: push        r14
  00000001404F330A: push        r15
  00000001404F330C: sub         rsp,5C0h
  00000001404F3313: mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
  00000001404F331C: mov         qword ptr [rsp+0000000000000608h],rbx
  00000001404F3324: mov         rdi,rdx
  00000001404F3327: mov         rbx,rcx
  00000001404F332A: add         rcx,0A50h
  00000001404F3331: call        0000000140395380
  00000001404F3336: mov         rcx,rbx
  00000001404F3339: call        0000000140789710
  00000001404F333E: mov         r12,rax
  00000001404F3341: mov         qword ptr [rsp+40h],rax
  00000001404F3346: mov         esi,dword ptr [rax+18h]
  00000001404F3349: and         esi,0FFFFFFh
  00000001404F334F: mov         r9d,esi
  00000001404F3352: lea         r8,[0000000141B6B4F0h]
  00000001404F3359: mov         rdx,rdi
  00000001404F335C: lea         rcx,[rsp+50h]
  00000001404F3361: call        0000000140027F60
  00000001404F3366: nop
  00000001404F3367: lea         rdi,[rbx+00000000000000E0h]
  00000001404F336E: xor         r8d,r8d
  00000001404F3371: lea         rdx,[rsp+0000000000000318h]
  00000001404F3379: mov         rcx,rdi
  00000001404F337C: call        00000001400283F0
  00000001404F3381: mov         rcx,qword ptr [rbx+8]
  00000001404F3385: call        00000001408832B0
  00000001404F338A: lea         rcx,[rax+0000000000312740h]
  00000001404F3391: call        0000000140515B60
  00000001404F3396: mov         byte ptr [rsp+0000000000000600h],al
  00000001404F339D: xor         ebp,ebp
  00000001404F339F: movsxd      r15,esi
  00000001404F33A2: test        esi,esi
  00000001404F33A4: jle         00000001404F3604
  00000001404F33AA: xor         r14d,r14d
  00000001404F33AD: nop         dword ptr [rax]
  00000001404F33B0: mov         rsi,qword ptr [r12+10h]
  00000001404F33B5: add         rsi,r14
  00000001404F33B8: lea         rcx,[rsi+00000000000000A0h]
  00000001404F33BF: call        00000001401D9090
  00000001404F33C4: mov         rdx,rax
  00000001404F33C7: lea         r8,[rbx+00000000000003D0h]
  00000001404F33CE: mov         rcx,rdi
  00000001404F33D1: call        000000014002A720
  00000001404F33D6: lea         rdx,[rsi+0000000000000098h]
  00000001404F33DD: lea         r8,[rbx+00000000000003E0h]
  00000001404F33E4: mov         rcx,rdi
  00000001404F33E7: call        000000014002A120
  00000001404F33EC: mov         eax,dword ptr [rsi+000000000000016Ch]
  00000001404F33F2: mov         dword ptr [rsp+0000000000000610h],eax
  00000001404F33F9: lea         r8,[rbx+00000000000003F0h]
  00000001404F3400: lea         rdx,[rsp+0000000000000610h]
  00000001404F3408: mov         rcx,rdi
  00000001404F340B: call        000000014002A120
  00000001404F3410: lea         rcx,[rsi+70h]
  00000001404F3414: call        00000001401D9090
  00000001404F3419: mov         rdx,rax
  00000001404F341C: 4C
  00000001404F341D: 8D
  00000001404F341E: add         dword ptr [rax],4

  Summary

     1A7D000 .text

````
