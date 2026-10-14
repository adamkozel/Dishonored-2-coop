# disasm-803f50.txt

Original: [work/disasm-803f50.txt](<../../../../work/disasm-803f50.txt>)

SHA-256: `634DCBFCBB5751904CB5F6E2A6E929C139CA1F9D50DE6D48AEE7B6E33ED7FAA6`; 6183 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  0000000140803F50: mov         r11,rsp
  0000000140803F53: mov         qword ptr [r11+8],rbx
  0000000140803F57: mov         qword ptr [r11+10h],rbp
  0000000140803F5B: mov         qword ptr [r11+18h],rsi
  0000000140803F5F: push        rdi
  0000000140803F60: sub         rsp,40h
  0000000140803F64: xor         eax,eax
  0000000140803F66: mov         rbx,rcx
  0000000140803F69: mov         qword ptr [r11-18h],rax
  0000000140803F6D: mov         qword ptr [r11-10h],rax
  0000000140803F71: lea         rax,[00000001408065E0h]
  0000000140803F78: mov         qword ptr [r11-10h],rax
  0000000140803F7C: mov         qword ptr [r11-18h],rcx
  0000000140803F80: call        0000000140810240
  0000000140803F85: lea         r8,[rsp+30h]
  0000000140803F8A: mov         edx,0F2h
  0000000140803F8F: mov         rcx,rax
  0000000140803F92: call        0000000140811040
  0000000140803F97: mov         rdi,qword ptr [rbx+10h]
  0000000140803F9B: xor         eax,eax
  0000000140803F9D: mov         qword ptr [rsp+30h],rax
  0000000140803FA2: mov         qword ptr [rsp+38h],rax
  0000000140803FA7: lea         rax,[00000001408065F0h]
  0000000140803FAE: mov         qword ptr [rsp+38h],rax
  0000000140803FB3: mov         qword ptr [rsp+30h],rbx
  0000000140803FB8: call        0000000140810240
  0000000140803FBD: lea         r9,[rsp+30h]
  0000000140803FC2: mov         r8d,178h
  0000000140803FC8: mov         rdx,rdi
  0000000140803FCB: mov         rcx,rax
  0000000140803FCE: call        0000000140811160
  0000000140803FD3: mov         eax,dword ptr [rbx+00000000000000B0h]
  0000000140803FD9: xor         ebp,ebp
  0000000140803FDB: and         eax,0FFFFFFh
  0000000140803FE0: mov         edi,ebp
  0000000140803FE2: movsxd      rsi,eax
  0000000140803FE5: test        eax,eax
  0000000140803FE7: jle         0000000140804024
  0000000140803FE9: nop         dword ptr [rax+0000000000000000h]
  0000000140803FF0: mov         rax,qword ptr [rbx+00000000000000A8h]
  0000000140803FF7: mov         rdx,qword ptr [rbx+0000000000000090h]
  0000000140803FFE: movsxd      rcx,dword ptr [rax+rdi*4]
  0000000140804002: test        rdx,rdx
  0000000140804005: je          000000014080401C
  0000000140804007: cmp         ecx,5Ah
  000000014080400A: jge         000000014080401C
  000000014080400C: mov         rcx,qword ptr [rdx+rcx*8+10h]
  0000000140804011: test        rcx,rcx
  0000000140804014: je          000000014080401C
  0000000140804016: mov         rax,qword ptr [rcx]
  0000000140804019: call        qword ptr [rax+60h]
  000000014080401C: inc         rdi
  000000014080401F: cmp         rdi,rsi
  0000000140804022: jl          0000000140803FF0
  0000000140804024: mov         rcx,qword ptr [rbx+0000000000000090h]
  000000014080402B: test        rcx,rcx
  000000014080402E: je          000000014080403C
  0000000140804030: call        00000001400998B0
  0000000140804035: mov         qword ptr [rbx+0000000000000090h],rbp
  000000014080403C: mov         qword ptr [rbx+0000000000000098h],rbp
  0000000140804043: lea         rdi,[rbx+28h]
  0000000140804047: mov         esi,0Dh
  000000014080404C: nop         dword ptr [rax]
  0000000140804050: mov         rcx,qword ptr [rdi]
  0000000140804053: test        rcx,rcx
  0000000140804056: je          0000000140804068
  0000000140804058: call        00000001408645C0
  000000014080405D: mov         rcx,qword ptr [rdi]
  0000000140804060: call        00000001400F1520
  0000000140804065: mov         qword ptr [rdi],rbp
  0000000140804068: add         rdi,8
  000000014080406C: dec         rsi
  000000014080406F: jne         0000000140804050
  0000000140804071: test        byte ptr [rbx+00000000000000B4h],40h
  0000000140804078: jne         00000001408040BF
  000000014080407A: mov         rdi,qword ptr [rbx+00000000000000A8h]
  0000000140804081: test        rdi,rdi
  0000000140804084: je          00000001408040AE
  0000000140804086: call        00000001401548E0
  000000014080408B: lea         rdx,[0000000141AB2230h]
  0000000140804092: mov         r9,rdi
  0000000140804095: mov         r10,qword ptr [rax]
  0000000140804098: mov         r8d,1ECh
  000000014080409E: mov         rcx,rax
  00000001408040A1: mov         qword ptr [rsp+20h],10h
  00000001408040AA: call        qword ptr [r10+10h]
  00000001408040AE: and         dword ptr [rbx+00000000000000B4h],0FFh
  00000001408040B8: mov         qword ptr [rbx+00000000000000A8h],rbp
  00000001408040BF: and         dword ptr [rbx+00000000000000B0h],0FF000000h
  00000001408040C9: test        byte ptr [rbx+000000000000022Ch],40h
  00000001408040D0: jne         0000000140804117
  00000001408040D2: mov         rdi,qword ptr [rbx+0000000000000220h]
  00000001408040D9: test        rdi,rdi
  00000001408040DC: je          0000000140804106
  00000001408040DE: call        00000001401548E0
  00000001408040E3: lea         rdx,[0000000141AB2230h]
  00000001408040EA: mov         r9,rdi
  00000001408040ED: mov         r10,qword ptr [rax]
  00000001408040F0: mov         r8d,1ECh
  00000001408040F6: mov         rcx,rax
  00000001408040F9: mov         qword ptr [rsp+20h],10h
  0000000140804102: call        qword ptr [r10+10h]
  0000000140804106: and         dword ptr [rbx+000000000000022Ch],0FFh
  0000000140804110: mov         qword ptr [rbx+0000000000000220h],rbp
  0000000140804117: and         dword ptr [rbx+0000000000000228h],0FF000000h
  0000000140804121: mov         rcx,rbx
  0000000140804124: mov         dword ptr [rbx+00000000000002F0h],ebp
  000000014080412A: mov         rbx,qword ptr [rsp+50h]
  000000014080412F: mov         rbp,qword ptr [rsp+58h]
  0000000140804134: mov         rsi,qword ptr [rsp+60h]
  0000000140804139: add         rsp,40h
  000000014080413D: pop         rdi
  000000014080413E: jmp         00000001407E7750
  0000000140804143: int         3

  Summary

     1A7D000 .text

````
