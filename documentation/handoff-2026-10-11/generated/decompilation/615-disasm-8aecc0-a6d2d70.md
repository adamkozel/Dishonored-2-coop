# disasm-8aecc0.txt

Original: [work/disasm-8aecc0.txt](<../../../../work/disasm-8aecc0.txt>)

SHA-256: `C6F92ABA847DDFD93214DA97F0253C9D66AA924B8B31339E1F0058F43F2D1F6C`; 16149 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001408AECC0: push        rbp
  00000001408AECC2: push        rsi
  00000001408AECC3: push        rdi
  00000001408AECC4: sub         rsp,30h
  00000001408AECC8: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  00000001408AECD1: mov         qword ptr [rsp+60h],rbx
  00000001408AECD6: mov         edi,r8d
  00000001408AECD9: mov         rsi,rdx
  00000001408AECDC: mov         rbp,rcx
  00000001408AECDF: lea         rbx,[rcx+0000000000341590h]
  00000001408AECE6: mov         qword ptr [rsp+68h],rbx
  00000001408AECEB: mov         dl,1
  00000001408AECED: mov         rcx,rbx
  00000001408AECF0: call        0000000140158290
  00000001408AECF5: nop
  00000001408AECF6: mov         r8b,1
  00000001408AECF9: mov         rdx,rbp
  00000001408AECFC: lea         rcx,[rsp+58h]
  00000001408AED01: call        00000001408A2DC0
  00000001408AED06: nop
  00000001408AED07: lea         rcx,[rbp+00000000002E75D8h]
  00000001408AED0E: mov         r9d,edi
  00000001408AED11: mov         r8,rsi
  00000001408AED14: lea         rdx,[rsp+50h]
  00000001408AED19: call        0000000140657AE0
  00000001408AED1E: mov         r8,rax
  00000001408AED21: lea         rcx,[rbp+00000000003130B0h]
  00000001408AED28: mov         rdx,rsi
  00000001408AED2B: call        00000001407B5D90
  00000001408AED30: lea         rcx,[rsi+20h]
  00000001408AED34: call        00000001401D9090
  00000001408AED39: mov         rdx,rax
  00000001408AED3C: mov         rcx,rsi
  00000001408AED3F: call        000000014087B780
  00000001408AED44: mov         rdx,rsi
  00000001408AED47: mov         rcx,rbp
  00000001408AED4A: call        00000001408AB180
  00000001408AED4F: nop
  00000001408AED50: lea         rcx,[rsp+58h]
  00000001408AED55: call        00000001408A2DE0
  00000001408AED5A: nop
  00000001408AED5B: mov         rcx,rbx
  00000001408AED5E: call        00000001401582E0
  00000001408AED63: mov         rbx,qword ptr [rsp+60h]
  00000001408AED68: add         rsp,30h
  00000001408AED6C: pop         rdi
  00000001408AED6D: pop         rsi
  00000001408AED6E: pop         rbp
  00000001408AED6F: ret
  00000001408AED70: sub         rsp,28h
  00000001408AED74: mov         r8d,dword ptr [00000001422813C8h]
  00000001408AED7B: add         rcx,2E7428h
  00000001408AED82: call        00000001400598D0
  00000001408AED87: add         rsp,28h
  00000001408AED8B: ret
  00000001408AED8C: int         3
  00000001408AED8D: int         3
  00000001408AED8E: int         3
  00000001408AED8F: int         3
  00000001408AED90: mov         rax,rsp
  00000001408AED93: mov         qword ptr [rax+8],rcx
  00000001408AED97: push        rdi
  00000001408AED98: push        r12
  00000001408AED9A: push        r13
  00000001408AED9C: push        r14
  00000001408AED9E: push        r15
  00000001408AEDA0: sub         rsp,0E0h
  00000001408AEDA7: mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
  00000001408AEDB0: mov         qword ptr [rax+10h],rbx
  00000001408AEDB4: mov         qword ptr [rax+18h],rbp
  00000001408AEDB8: mov         qword ptr [rax+20h],rsi
  00000001408AEDBC: mov         rbp,rdx
  00000001408AEDBF: mov         rbx,rcx
  00000001408AEDC2: cmp         dword ptr [00000001432C7ED4h],0
  00000001408AEDC9: jne         00000001408AEFBC
  00000001408AEDCF: mov         rcx,qword ptr [rcx+0000000000337110h]
  00000001408AEDD6: call        00000001400AC8E0
  00000001408AEDDB: mov         r13d,eax
  00000001408AEDDE: mov         edx,eax
  00000001408AEDE0: lea         rcx,[0000000141C94CD8h]
  00000001408AEDE7: call        0000000140166830
  00000001408AEDEC: xor         r12d,r12d
  00000001408AEDEF: mov         r15d,1
  00000001408AEDF5: cmp         r13d,r15d
  00000001408AEDF8: jle         00000001408AEFAD
  00000001408AEDFE: nop
  00000001408AEE00: mov         edx,r15d
  00000001408AEE03: mov         rcx,qword ptr [rbx+0000000000337110h]
  00000001408AEE0A: call        00000001400AC610
  00000001408AEE0F: mov         r14,rax
  00000001408AEE12: mov         ecx,dword ptr [rax+0000000000000098h]
  00000001408AEE18: and         ecx,0FFFFFFh
  00000001408AEE1E: jle         00000001408AEF99
  00000001408AEE24: xor         esi,esi
  00000001408AEE26: test        ecx,ecx
  00000001408AEE28: jle         00000001408AEF52
  00000001408AEE2E: nop
  00000001408AEE30: movsxd      rax,esi
  00000001408AEE33: lea         rcx,[rax+rax*4]
  00000001408AEE37: mov         rax,qword ptr [r14+0000000000000090h]
  00000001408AEE3E: lea         rdx,[rax+rcx*8]
  00000001408AEE42: lea         rcx,[rsp+20h]
  00000001408AEE47: call        000000014013F490
  00000001408AEE4C: nop
  00000001408AEE4D: lea         rcx,[rsp+50h]
  00000001408AEE52: call        000000014013FB30
  00000001408AEE57: nop
  00000001408AEE58: xor         edx,edx
  00000001408AEE5A: lea         r8d,[rdx+68h]
  00000001408AEE5E: lea         rcx,[rsp+78h]
  00000001408AEE63: call        00000001411AD450
  00000001408AEE68: mov         byte ptr [rsp+60h],0
  00000001408AEE6D: lea         rax,[rsp+60h]
  00000001408AEE72: mov         qword ptr [rsp+50h],rax
  00000001408AEE77: and         dword ptr [rsp+58h],0E0000000h
  00000001408AEE7F: mov         eax,dword ptr [rsp+5Ch]
  00000001408AEE83: and         eax,0C0000080h
  00000001408AEE88: or          eax,80000080h
  00000001408AEE8D: mov         dword ptr [rsp+5Ch],eax
  00000001408AEE91: lea         rdx,[rsp+20h]
  00000001408AEE96: lea         rcx,[rsp+50h]
  00000001408AEE9B: call        000000014013FD70
  00000001408AEEA0: nop
  00000001408AEEA1: mov         edi,dword ptr [rbp+8]
  00000001408AEEA4: and         edi,0FFFFFFh
  00000001408AEEAA: dec         edi
  00000001408AEEAC: mov         ebx,0
  00000001408AEEB1: js          00000001408AEEE3
  00000001408AEEB3: nop         word ptr [rax+rax+0000000000000000h]
  00000001408AEEC0: movsxd      rax,ebx
  00000001408AEEC3: lea         rcx,[rax+rax*8]
  00000001408AEEC7: shl         rcx,4
  00000001408AEECB: add         rcx,qword ptr [rbp]
  00000001408AEECF: lea         rdx,[rsp+50h]
  00000001408AEED4: call        00000001401400C0
  00000001408AEED9: test        al,al
  00000001408AEEDB: jne         00000001408AEEE6
  00000001408AEEDD: inc         ebx
  00000001408AEEDF: cmp         ebx,edi
  00000001408AEEE1: jle         00000001408AEEC0
  00000001408AEEE3: or          ebx,0FFFFFFFFh
  00000001408AEEE6: test        ebx,ebx
  00000001408AEEE8: setns       bl
  00000001408AEEEB: lea         rcx,[rsp+50h]
  00000001408AEEF0: call        000000014013FD10
  00000001408AEEF5: test        bl,bl
  00000001408AEEF7: jne         00000001408AEF46
  00000001408AEEF9: lea         rdx,[0000000141C94D10h]
  00000001408AEF00: lea         rcx,[rsp+20h]
  00000001408AEF05: call        0000000140140130
  00000001408AEF0A: test        al,al
  00000001408AEF0C: jne         00000001408AEF46
  00000001408AEF0E: lea         rdx,[0000000141C94D20h]
  00000001408AEF15: lea         rcx,[rsp+20h]
  00000001408AEF1A: call        0000000140140130
  00000001408AEF1F: nop
  00000001408AEF20: lea         rcx,[rsp+20h]
  00000001408AEF25: test        al,al
  00000001408AEF27: jne         00000001408AEF4B
  00000001408AEF29: call        000000014013FD10
  00000001408AEF2E: inc         esi
  00000001408AEF30: mov         eax,dword ptr [r14+0000000000000098h]
  00000001408AEF37: and         eax,0FFFFFFh
  00000001408AEF3C: cmp         esi,eax
  00000001408AEF3E: jl          00000001408AEE30
  00000001408AEF44: jmp         00000001408AEF52
  00000001408AEF46: lea         rcx,[rsp+20h]
  00000001408AEF4B: call        000000014013FD10
  00000001408AEF50: jmp         00000001408AEF99
  00000001408AEF52: inc         r12d
  00000001408AEF55: mov         rcx,qword ptr [00000001439A5290h]
  00000001408AEF5C: mov         rax,qword ptr [rcx]
  00000001408AEF5F: xor         r9d,r9d
  00000001408AEF62: or          r8d,0FFFFFFFFh
  00000001408AEF66: mov         rdx,qword ptr [r14+18h]
  00000001408AEF6A: call        qword ptr [rax+00000000000001D0h]
  00000001408AEF70: mov         rcx,rax
  00000001408AEF73: test        rax,rax
  00000001408AEF76: jne         00000001408AEF8E
  00000001408AEF78: mov         rdx,qword ptr [r14+18h]
  00000001408AEF7C: mov         rdx,qword ptr [rdx+8]
  00000001408AEF80: lea         rcx,[0000000141C94D40h]
  00000001408AEF87: call        0000000140166830
  00000001408AEF8C: jmp         00000001408AEF99
  00000001408AEF8E: mov         rax,qword ptr [rax]
  00000001408AEF91: mov         edx,1
  00000001408AEF96: call        qword ptr [rax+8]
  00000001408AEF99: inc         r15d
  00000001408AEF9C: cmp         r15d,r13d
  00000001408AEF9F: mov         rbx,qword ptr [rsp+0000000000000110h]
  00000001408AEFA7: jl          00000001408AEE00
  00000001408AEFAD: mov         edx,r12d
  00000001408AEFB0: lea         rcx,[0000000141C94D88h]
  00000001408AEFB7: call        0000000140166830
  00000001408AEFBC: lea         r11,[rsp+00000000000000E0h]
  00000001408AEFC4: mov         rbx,qword ptr [r11+38h]
  00000001408AEFC8: mov         rbp,qword ptr [r11+40h]
  00000001408AEFCC: mov         rsi,qword ptr [r11+48h]
  00000001408AEFD0: mov         rsp,r11
  00000001408AEFD3: pop         r15
  00000001408AEFD5: pop         r14
  00000001408AEFD7: pop         r13
  00000001408AEFD9: pop         r12
  00000001408AEFDB: pop         rdi
  00000001408AEFDC: ret
  00000001408AEFDD: int         3
  00000001408AEFDE: int         3
  00000001408AEFDF: int         3
  00000001408AEFE0: mov         qword ptr [rsp+8],rbx
  00000001408AEFE5: mov         qword ptr [rsp+10h],rsi
  00000001408AEFEA: push        rdi
  00000001408AEFEB: sub         rsp,20h
  00000001408AEFEF: mov         rsi,rcx
  00000001408AEFF2: nop         word ptr [rax+rax+0000000000000000h]
  00000001408AF000: mov         ebx,dword ptr [rsi]
  00000001408AF002: mov         edi,ebx
  00000001408AF004: and         edi,3FFFh
  00000001408AF00A: cmp         edi,2000h
  00000001408AF010: je          00000001408AF048
  00000001408AF012: mov         r8d,ebx
  00000001408AF015: mov         edx,ebx
  00000001408AF017: mov         rcx,rsi
  00000001408AF01A: and         r8d,0FFFFC000h
  00000001408AF021: add         r8d,4000h
  00000001408AF028: or          r8d,dword ptr [rsi+rdi*4+4]
  00000001408AF02D: call        00000001401581C0
  00000001408AF032: cmp         eax,ebx
  00000001408AF034: jne         00000001408AF000
  00000001408AF036: mov         eax,edi
  00000001408AF038: mov         rbx,qword ptr [rsp+30h]
  00000001408AF03D: mov         rsi,qword ptr [rsp+38h]
  00000001408AF042: add         rsp,20h
  00000001408AF046: pop         rdi
  00000001408AF047: ret
  00000001408AF048: mov         rbx,qword ptr [rsp+30h]
  00000001408AF04D: mov         rsi,qword ptr [rsp+38h]
  00000001408AF052: or          eax,0FFFFFFFFh
  00000001408AF055: add         rsp,20h
  00000001408AF059: pop         rdi
  00000001408AF05A: ret
  00000001408AF05B: int         3
  00000001408AF05C: int         3
  00000001408AF05D: int         3
  00000001408AF05E: int         3
  00000001408AF05F: int         3
  00000001408AF060: mov         qword ptr [rsp+8],rbx
  00000001408AF065: push        rdi
  00000001408AF066: sub         rsp,20h
  00000001408AF06A: mov         rbx,rcx
  00000001408AF06D: add         rcx,60140h
  00000001408AF074: mov         rdi,rdx
  00000001408AF077: call        0000000140874940
  00000001408AF07C: lea         rcx,[rbx+00000000003130B0h]
  00000001408AF083: mov         rdx,rdi
  00000001408AF086: mov         rbx,qword ptr [rsp+30h]
  00000001408AF08B: add         rsp,20h
  00000001408AF08F: pop         rdi
  00000001408AF090: jmp         00000001407B5F10
  00000001408AF095: int         3
  00000001408AF096: int         3
  00000001408AF097: int         3
  00000001408AF098: int         3
  00000001408AF099: int         3
  00000001408AF09A: int         3
  00000001408AF09B: int         3
  00000001408AF09C: int         3
  00000001408AF09D: int         3
  00000001408AF09E: int         3
  00000001408AF09F: int         3
  00000001408AF0A0: mov         qword ptr [rsp+8],rbx
  00000001408AF0A5: mov         qword ptr [rsp+10h],rbp
  00000001408AF0AA: mov         qword ptr [rsp+18h],rsi
  00000001408AF0AF: push        rdi
  00000001408AF0B0: sub         rsp,20h
  00000001408AF0B4: mov         ebx,dword ptr [rcx+0000000000060230h]
  00000001408AF0BA: mov         rdi,rcx
  00000001408AF0BD: mov         rcx,r8
  00000001408AF0C0: mov         rsi,r8
  00000001408AF0C3: mov         rbp,rdx
  00000001408AF0C6: call        00000001401431A0
  00000001408AF0CB: and         eax,ebx
  00000001408AF0CD: mov         rbx,qword ptr [rdi+0000000000060220h]
  00000001408AF0D4: cdqe
  00000001408AF0D6: mov         rbx,qword ptr [rbx+rax*8]
  00000001408AF0DA: test        rbx,rbx
  00000001408AF0DD: je          00000001408AF19C
  00000001408AF0E3: mov         rcx,rbx
  00000001408AF0E6: call        00000001401D9090
  00000001408AF0EB: mov         rdx,rsi
  00000001408AF0EE: mov         rcx,rax
  00000001408AF0F1: call        0000000140143650
  00000001408AF0F6: test        eax,eax
  00000001408AF0F8: je          00000001408AF10E
  00000001408AF0FA: jg          00000001408AF19C
  00000001408AF100: mov         rbx,qword ptr [rbx+38h]
  00000001408AF104: test        rbx,rbx
  00000001408AF107: jne         00000001408AF0E3
  00000001408AF109: jmp         00000001408AF19C
  00000001408AF10E: add         rbx,28h
  00000001408AF112: je          00000001408AF19C
  00000001408AF118: mov         eax,3FFF0010h
  00000001408AF11D: test        rbp,rbp
  00000001408AF120: je          00000001408AF128
  00000001408AF122: mov         eax,dword ptr [rbp+00000000000000A8h]
  00000001408AF128: mov         r11d,dword ptr [rbx+8]
  00000001408AF12C: xor         edx,edx
  00000001408AF12E: mov         r10d,r11d
  00000001408AF131: mov         r8d,edx
  00000001408AF134: and         r10d,0FFFFFFh
  00000001408AF13B: lea         ecx,[r10-1]
  00000001408AF13F: movsxd      r9,ecx
  00000001408AF142: test        ecx,ecx
  00000001408AF144: js          00000001408AF19C
  00000001408AF146: mov         rdi,qword ptr [rbx]
  00000001408AF149: mov         rcx,rdi
  00000001408AF14C: nop         dword ptr [rax]
  00000001408AF150: cmp         dword ptr [rcx],eax
  00000001408AF152: je          00000001408AF164
  00000001408AF154: inc         r8
  00000001408AF157: inc         edx
  00000001408AF159: add         rcx,4
  00000001408AF15D: cmp         r8,r9
  00000001408AF160: jle         00000001408AF150
  00000001408AF162: jmp         00000001408AF19C
  00000001408AF164: test        edx,edx
  00000001408AF166: js          00000001408AF19C
  00000001408AF168: cmp         edx,r10d
  00000001408AF16B: jge         00000001408AF19C
  00000001408AF16D: lea         eax,[r11-1]
  00000001408AF171: xor         eax,r11d
  00000001408AF174: and         eax,0FFFFFFh
  00000001408AF179: xor         eax,r11d
  00000001408AF17C: mov         r8d,eax
  00000001408AF17F: mov         dword ptr [rbx+8],eax
  00000001408AF182: and         eax,0FFFFFFh
  00000001408AF187: cmp         edx,eax
  00000001408AF189: je          00000001408AF19C
  00000001408AF18B: and         r8d,0FFFFFFh
  00000001408AF192: movsxd      rcx,edx
  00000001408AF195: mov         eax,dword ptr [rdi+r8*4]
  00000001408AF199: mov         dword ptr [rdi+rcx*4],eax
  00000001408AF19C: mov         rbx,qword ptr [rsp+30h]
  00000001408AF1A1: mov         rbp,qword ptr [rsp+38h]
  00000001408AF1A6: mov         rsi,qword ptr [rsp+40h]
  00000001408AF1AB: add         rsp,20h
  00000001408AF1AF: pop         rdi
  00000001408AF1B0: ret

  Summary

     1A7D000 .text

````
