# disasm-8acdf0.txt

Original: [work/disasm-8acdf0.txt](<../../../../work/disasm-8acdf0.txt>)

SHA-256: `D98E6A1450C575E7DD58BE4214236AB40CD9B4E91E4DD4E64EE8B834F6DBD8B7`; 13480 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001408ACDF0: push        rbp
  00000001408ACDF2: push        rsi
  00000001408ACDF3: push        rdi
  00000001408ACDF4: push        r12
  00000001408ACDF6: push        r13
  00000001408ACDF8: push        r14
  00000001408ACDFA: push        r15
  00000001408ACDFC: mov         eax,1930h
  00000001408ACE01: call        00000001411AD7C0
  00000001408ACE06: sub         rsp,rax
  00000001408ACE09: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  00000001408ACE12: mov         qword ptr [rsp+0000000000001978h],rbx
  00000001408ACE1A: mov         r15,r8
  00000001408ACE1D: mov         r13,rdx
  00000001408ACE20: mov         r14,rcx
  00000001408ACE23: xor         r8d,r8d
  00000001408ACE26: mov         rdx,rcx
  00000001408ACE29: lea         rcx,[rsp+0000000000001980h]
  00000001408ACE31: call        00000001408A2DC0
  00000001408ACE36: nop
  00000001408ACE37: mov         qword ptr [rsp+0000000000001988h],r14
  00000001408ACE3F: xor         edi,edi
  00000001408ACE41: test        r15,r15
  00000001408ACE44: jne         00000001408ACEBE
  00000001408ACE46: lea         rcx,[rsp+78h]
  00000001408ACE4B: call        0000000140043650
  00000001408ACE50: nop
  00000001408ACE51: lea         rbx,[r14+0000000000340F10h]
  00000001408ACE58: mov         rdx,rax
  00000001408ACE5B: mov         rcx,rbx
  00000001408ACE5E: call        00000001408A5CE0
  00000001408ACE63: nop
  00000001408ACE64: lea         rcx,[rsp+00000000000000D0h]
  00000001408ACE6C: call        000000014061A0A0
  00000001408ACE71: nop
  00000001408ACE72: mov         edx,dword ptr [rsp+00000000000000CCh]
  00000001408ACE79: test        dl,40h
  00000001408ACE7C: jne         00000001408ACEAA
  00000001408ACE7E: mov         rcx,qword ptr [rsp+00000000000000C0h]
  00000001408ACE86: test        rcx,rcx
  00000001408ACE89: je          00000001408ACE93
  00000001408ACE8B: shr         edx,8
  00000001408ACE8E: call        00000001409A9450
  00000001408ACE93: mov         qword ptr [rsp+00000000000000C0h],rdi
  00000001408ACE9B: movzx       eax,byte ptr [rsp+00000000000000CCh]
  00000001408ACEA3: mov         dword ptr [rsp+00000000000000CCh],eax
  00000001408ACEAA: and         dword ptr [rsp+00000000000000C8h],0FF000000h
  00000001408ACEB5: mov         qword ptr [r14+0000000000340F08h],rbx
  00000001408ACEBC: jmp         00000001408ACEC5
  00000001408ACEBE: mov         qword ptr [r14+0000000000340F08h],r15
  00000001408ACEC5: mov         bpl,1
  00000001408ACEC8: mov         rax,qword ptr [r14+0000000000340F08h]
  00000001408ACECF: test        rax,rax
  00000001408ACED2: je          00000001408AD222
  00000001408ACED8: mov         dword ptr [rax+3Ch],edi
  00000001408ACEDB: mov         rax,qword ptr [r14+0000000000340F08h]
  00000001408ACEE2: mov         dword ptr [rax+40h],edi
  00000001408ACEE5: lea         rcx,[r14+0000000000340EA8h]
  00000001408ACEEC: mov         rdx,r13
  00000001408ACEEF: call        00000001408A5C60
  00000001408ACEF4: mov         rax,qword ptr [r14]
  00000001408ACEF7: mov         rcx,r14
  00000001408ACEFA: call        qword ptr [rax+0000000000000090h]
  00000001408ACF00: cmp         eax,3
  00000001408ACF03: jne         00000001408AD112
  00000001408ACF09: lea         rcx,[r14+00000000001F77C8h]
  00000001408ACF10: call        000000014067BEC0
  00000001408ACF15: test        eax,eax
  00000001408ACF17: js          00000001408AD112
  00000001408ACF1D: lea         rcx,[r14+00000000001F6EC8h]
  00000001408ACF24: call        0000000140787900
  00000001408ACF29: cmp         eax,0Dh
  00000001408ACF2C: ja          00000001408ACF3C
  00000001408ACF2E: mov         ecx,284Ch
  00000001408ACF33: bt          ecx,eax
  00000001408ACF36: jb          00000001408AD112
  00000001408ACF3C: mov         rcx,qword ptr [00000001439A5290h]
  00000001408ACF43: mov         rax,qword ptr [rcx]
  00000001408ACF46: xor         r8d,r8d
  00000001408ACF49: lea         rdx,[rsp+0000000000001970h]
  00000001408ACF51: call        qword ptr [rax+00000000000000E0h]
  00000001408ACF57: movss       xmm0,dword ptr [rax]
  00000001408ACF5B: addss       xmm0,dword ptr [r14+000000000034105Ch]
  00000001408ACF64: movss       dword ptr [r14+000000000034105Ch],xmm0
  00000001408ACF6D: comiss      xmm0,dword ptr [0000000141F697B0h]
  00000001408ACF74: jb          00000001408AD112
  00000001408ACF7A: cvttss2si   eax,xmm0
  00000001408ACF7E: mov         dword ptr [rsp+0000000000001970h],eax
  00000001408ACF85: xor         r9d,r9d
  00000001408ACF88: mov         r8d,eax
  00000001408ACF8B: lea         edx,[r9+24h]
  00000001408ACF8F: lea         rcx,[r14+00000000001F77C8h]
  00000001408ACF96: call        000000014067AEA0
  00000001408ACF9B: movss       xmm1,dword ptr [0000000141F697B0h]
  00000001408ACFA3: movss       xmm0,dword ptr [r14+000000000034105Ch]
  00000001408ACFAC: call        00000001411AF130
  00000001408ACFB1: movss       dword ptr [r14+000000000034105Ch],xmm0
  00000001408ACFBA: mov         rcx,qword ptr [0000000142287C78h]
  00000001408ACFC1: mov         rax,qword ptr [rcx]
  00000001408ACFC4: call        qword ptr [rax+38h]
  00000001408ACFC7: mov         rcx,rax
  00000001408ACFCA: call        000000014004A4E0
  00000001408ACFCF: test        rax,rax
  00000001408ACFD2: je          00000001408AD112
  00000001408ACFD8: mov         rdx,qword ptr [rax]
  00000001408ACFDB: mov         rcx,rax
  00000001408ACFDE: call        qword ptr [rdx+0000000000000088h]
  00000001408ACFE4: mov         rsi,rax
  00000001408ACFE7: test        rax,rax
  00000001408ACFEA: je          00000001408AD112
  00000001408ACFF0: call        00000001401548E0
  00000001408ACFF5: mov         r8,qword ptr [rax]
  00000001408ACFF8: xor         edx,edx
  00000001408ACFFA: mov         rcx,rax
  00000001408ACFFD: call        qword ptr [r8+48h]
  00000001408AD001: nop
  00000001408AD002: add         rsi,28h
  00000001408AD006: lea         rdi,[r14+00000000002E75D8h]
  00000001408AD00D: mov         rcx,rdi
  00000001408AD010: call        0000000140657380
  00000001408AD015: mov         r8d,eax
  00000001408AD018: lea         rdx,[0000000141B585F0h]
  00000001408AD01F: lea         rcx,[rsp+0000000000000120h]
  00000001408AD027: call        0000000140004B80
  00000001408AD02C: mov         rdx,rax
  00000001408AD02F: lea         rcx,[rsp+28h]
  00000001408AD034: call        000000014013F640
  00000001408AD039: nop
  00000001408AD03A: lea         rcx,[rsp+28h]
  00000001408AD03F: call        00000001401D9090
  00000001408AD044: mov         rdx,rax
  00000001408AD047: xor         r8d,r8d
  00000001408AD04A: mov         rcx,rsi
  00000001408AD04D: call        0000000140064480
  00000001408AD052: mov         ebx,eax
  00000001408AD054: add         ebx,dword ptr [rsp+0000000000001970h]
  00000001408AD05B: lea         rcx,[rsp+28h]
  00000001408AD060: call        00000001401D9090
  00000001408AD065: mov         rdx,rax
  00000001408AD068: mov         r8d,ebx
  00000001408AD06B: mov         rcx,rsi
  00000001408AD06E: call        0000000140069B50
  00000001408AD073: lea         rcx,[r14+00000000001F77C8h]
  00000001408AD07A: call        000000014067BEC0
  00000001408AD07F: mov         ebx,eax
  00000001408AD081: mov         rcx,rdi
  00000001408AD084: call        0000000140657380
  00000001408AD089: mov         r8d,eax
  00000001408AD08C: mov         r9d,ebx
  00000001408AD08F: lea         rdx,[0000000141B58640h]
  00000001408AD096: lea         rcx,[rsp+0000000000000D20h]
  00000001408AD09E: call        0000000140004B80
  00000001408AD0A3: mov         rdx,rax
  00000001408AD0A6: lea         rcx,[rsp+50h]
  00000001408AD0AB: call        000000014013F640
  00000001408AD0B0: nop
  00000001408AD0B1: lea         rcx,[rsp+50h]
  00000001408AD0B6: call        00000001401D9090
  00000001408AD0BB: mov         rdx,rax
  00000001408AD0BE: xor         r8d,r8d
  00000001408AD0C1: mov         rcx,rsi
  00000001408AD0C4: call        0000000140064480
  00000001408AD0C9: mov         ebx,eax
  00000001408AD0CB: add         ebx,dword ptr [rsp+0000000000001970h]
  00000001408AD0D2: lea         rcx,[rsp+50h]
  00000001408AD0D7: call        00000001401D9090
  00000001408AD0DC: mov         rdx,rax
  00000001408AD0DF: mov         r8d,ebx
  00000001408AD0E2: mov         rcx,rsi
  00000001408AD0E5: call        0000000140069B50
  00000001408AD0EA: nop
  00000001408AD0EB: lea         rcx,[rsp+50h]
  00000001408AD0F0: call        000000014013FD10
  00000001408AD0F5: nop
  00000001408AD0F6: lea         rcx,[rsp+28h]
  00000001408AD0FB: call        000000014013FD10
  00000001408AD100: nop
  00000001408AD101: call        00000001401548E0
  00000001408AD106: mov         r8,qword ptr [rax]
  00000001408AD109: xor         edx,edx
  00000001408AD10B: mov         rcx,rax
  00000001408AD10E: call        qword ptr [r8+50h]
  00000001408AD112: mov         rcx,qword ptr [0000000142287C78h]
  00000001408AD119: mov         rax,qword ptr [rcx]
  00000001408AD11C: call        qword ptr [rax+38h]
  00000001408AD11F: cmp         byte ptr [rax+10h],0
  00000001408AD123: je          00000001408AD15A
  00000001408AD125: mov         rcx,qword ptr [0000000142274420h]
  00000001408AD12C: mov         rax,qword ptr [rcx]
  00000001408AD12F: call        qword ptr [rax+0000000000000190h]
  00000001408AD135: mov         rdx,qword ptr [rax]
  00000001408AD138: mov         rcx,rax
  00000001408AD13B: call        qword ptr [rdx+40h]
  00000001408AD13E: test        al,al
  00000001408AD140: je          00000001408AD15A
  00000001408AD142: mov         rcx,qword ptr [00000001439A5290h]
  00000001408AD149: add         rcx,1F6EC8h
  00000001408AD150: mov         edx,2
  00000001408AD155: call        00000001407885B0
  00000001408AD15A: mov         rcx,qword ptr [r14+0000000000340FC0h]
  00000001408AD161: test        rcx,rcx
  00000001408AD164: je          00000001408AD16F
  00000001408AD166: lea         rdx,[r13+50h]
  00000001408AD16A: call        0000000140899210
  00000001408AD16F: mov         rax,qword ptr [00000001439A5290h]
  00000001408AD176: mov         ebx,dword ptr [rax+0000000000337130h]
  00000001408AD17C: shr         ebx,1
  00000001408AD17E: and         bl,1
  00000001408AD181: mov         rax,qword ptr [r14]
  00000001408AD184: mov         rcx,r14
  00000001408AD187: call        qword ptr [rax+00000000000000C0h]
  00000001408AD18D: cmp         eax,0Ah
  00000001408AD190: setge       dil
  00000001408AD194: mov         rax,qword ptr [r14]
  00000001408AD197: mov         rcx,r14
  00000001408AD19A: call        qword ptr [rax+00000000000000E8h]
  00000001408AD1A0: test        al,al
  00000001408AD1A2: je          00000001408AD1BA
  00000001408AD1A4: test        bl,bl
  00000001408AD1A6: jne         00000001408AD1BA
  00000001408AD1A8: test        dil,dil
  00000001408AD1AB: je          00000001408AD1BA
  00000001408AD1AD: mov         rdx,r15
  00000001408AD1B0: mov         rcx,r14
  00000001408AD1B3: call        00000001408AD580
  00000001408AD1B8: jmp         00000001408AD222
  00000001408AD1BA: cmp         qword ptr [r14+00000000001E8940h],0
  00000001408AD1C2: je          00000001408AD222
  00000001408AD1C4: mov         ecx,dword ptr [00000001439A5788h]
  00000001408AD1CA: test        ecx,ecx
  00000001408AD1CC: jle         00000001408AD1D3
  00000001408AD1CE: call        0000000140157FF0
  00000001408AD1D3: cmp         dword ptr [00000001439A0E38h],0
  00000001408AD1DA: jg          00000001408AD1E6
  00000001408AD1DC: cmp         byte ptr [r14+0000000000340EB6h],0
  00000001408AD1E4: je          00000001408AD209
  00000001408AD1E6: cmp         dword ptr [00000001439A0EB8h],0
  00000001408AD1ED: jg          00000001408AD209
  00000001408AD1EF: mov         rax,qword ptr [00000001439A5290h]
  00000001408AD1F6: test        byte ptr [rax+0000000000337130h],2
  00000001408AD1FD: jne         00000001408AD209
  00000001408AD1FF: mov         rcx,r14
  00000001408AD202: call        00000001408AD4E0
  00000001408AD207: jmp         00000001408AD214
  00000001408AD209: mov         rcx,r14
  00000001408AD20C: call        00000001408AD260
  00000001408AD211: movzx       ebp,al
  00000001408AD214: lea         rcx,[r14+00000000002E75D8h]
  00000001408AD21B: call        00000001406605E0
  00000001408AD220: jmp         00000001408AD225
  00000001408AD222: mov         bpl,1
  00000001408AD225: lea         rcx,[r14+0000000000340F10h]
  00000001408AD22C: mov         qword ptr [r14+0000000000340F08h],rcx
  00000001408AD233: lea         rcx,[rsp+0000000000001980h]
  00000001408AD23B: call        00000001408A2DE0
  00000001408AD240: movzx       eax,bpl
  00000001408AD244: mov         rbx,qword ptr [rsp+0000000000001978h]
  00000001408AD24C: add         rsp,1930h
  00000001408AD253: pop         r15
  00000001408AD255: pop         r14
  00000001408AD257: pop         r13
  00000001408AD259: pop         r12
  00000001408AD25B: pop         rdi
  00000001408AD25C: pop         rsi
  00000001408AD25D: pop         rbp
  00000001408AD25E: ret
  00000001408AD25F: int         3

  Summary

     1A7D000 .text

````
