# disasm-4effd0.txt

Original: [work/disasm-4effd0.txt](<../../../../work/disasm-4effd0.txt>)

SHA-256: `099B451AC78DF32ADE5D7C80ABC261533EAD3B0B6470B156EBD33CEB496EA8A2`; 7458 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001404EFFD0: test        edx,edx
  00000001404EFFD2: je          00000001404EFFDE
  00000001404EFFD4: mov         word ptr [rcx+0000000000000116h],101h
  00000001404EFFDD: ret
  00000001404EFFDE: mov         byte ptr [rcx+0000000000000115h],1
  00000001404EFFE5: mov         byte ptr [rcx+0000000000000118h],1
  00000001404EFFEC: ret
  00000001404EFFED: int         3
  00000001404EFFEE: int         3
  00000001404EFFEF: int         3
  00000001404EFFF0: lea         rax,[00000001439814E0h]
  00000001404EFFF7: ret
  00000001404EFFF8: int         3
  00000001404EFFF9: int         3
  00000001404EFFFA: int         3
  00000001404EFFFB: int         3
  00000001404EFFFC: int         3
  00000001404EFFFD: int         3
  00000001404EFFFE: int         3
  00000001404EFFFF: int         3
  00000001404F0000: mov         qword ptr [rsp+8],rbx
  00000001404F0005: mov         rax,qword ptr [00000001432CDD70h]
  00000001404F000C: xor         r10d,r10d
  00000001404F000F: mov         r11,qword ptr [rax+10h]
  00000001404F0013: mov         r9d,r10d
  00000001404F0016: mov         rax,qword ptr [r11]
  00000001404F0019: cmp         byte ptr [rax],r9b
  00000001404F001C: je          00000001404F0042
  00000001404F001E: mov         r8,r11
  00000001404F0021: movsxd      rdx,edx
  00000001404F0024: cmp         rdx,qword ptr [r8+8]
  00000001404F0028: je          00000001404F004F
  00000001404F002A: inc         r9
  00000001404F002D: inc         r10d
  00000001404F0030: mov         r8,r9
  00000001404F0033: shl         r8,4
  00000001404F0037: add         r8,r11
  00000001404F003A: mov         rax,qword ptr [r8]
  00000001404F003D: cmp         byte ptr [rax],0
  00000001404F0040: jne         00000001404F0024
  00000001404F0042: mov         rax,qword ptr [00000001432CDD78h]
  00000001404F0049: mov         rbx,qword ptr [rsp+8]
  00000001404F004E: ret
  00000001404F004F: mov         rbx,qword ptr [rsp+8]
  00000001404F0054: movsxd      rax,r10d
  00000001404F0057: lea         rax,[rax+rax*4]
  00000001404F005B: lea         rax,[rax+11h]
  00000001404F005F: lea         rax,[rcx+rax*8]
  00000001404F0063: ret
  00000001404F0064: int         3
  00000001404F0065: int         3
  00000001404F0066: int         3
  00000001404F0067: int         3
  00000001404F0068: int         3
  00000001404F0069: int         3
  00000001404F006A: int         3
  00000001404F006B: int         3
  00000001404F006C: int         3
  00000001404F006D: int         3
  00000001404F006E: int         3
  00000001404F006F: int         3
  00000001404F0070: push        rbp
  00000001404F0072: push        rsi
  00000001404F0073: push        rdi
  00000001404F0074: push        r13
  00000001404F0076: push        r14
  00000001404F0078: sub         rsp,20h
  00000001404F007C: mov         rsi,rcx
  00000001404F007F: mov         rcx,rdx
  00000001404F0082: mov         qword ptr [rsp+58h],r12
  00000001404F0087: mov         qword ptr [rsp+60h],r15
  00000001404F008C: call        0000000140810210
  00000001404F0091: lea         rdx,[00000001439BA0D0h]
  00000001404F0098: mov         rcx,rax
  00000001404F009B: xor         r8d,r8d
  00000001404F009E: mov         r14,rax
  00000001404F00A1: call        0000000140877720
  00000001404F00A6: mov         rcx,rax
  00000001404F00A9: call        0000000140AD7580
  00000001404F00AE: mov         rcx,qword ptr [rsi+8]
  00000001404F00B2: mov         r13,rax
  00000001404F00B5: call        00000001408832B0
  00000001404F00BA: mov         r12,qword ptr [rsi+10h]
  00000001404F00BE: xor         edi,edi
  00000001404F00C0: mov         r15,rax
  00000001404F00C3: test        rax,rax
  00000001404F00C6: je          00000001404F01B6
  00000001404F00CC: mov         rcx,qword ptr [00000001432CDD70h]
  00000001404F00D3: mov         edx,edi
  00000001404F00D5: mov         qword ptr [rsp+50h],rbx
  00000001404F00DA: mov         r9,qword ptr [rcx+10h]
  00000001404F00DE: mov         r8d,edi
  00000001404F00E1: mov         rcx,qword ptr [r9]
  00000001404F00E4: cmp         byte ptr [rcx],dl
  00000001404F00E6: je          00000001404F0112
  00000001404F00E8: mov         rcx,r9
  00000001404F00EB: nop         dword ptr [rax+rax]
  00000001404F00F0: cmp         qword ptr [rcx+8],rdi
  00000001404F00F4: je          00000001404F018B
  00000001404F00FA: inc         rdx
  00000001404F00FD: inc         r8d
  00000001404F0100: mov         rcx,rdx
  00000001404F0103: shl         rcx,4
  00000001404F0107: add         rcx,r9
  00000001404F010A: mov         rax,qword ptr [rcx]
  00000001404F010D: cmp         byte ptr [rax],dil
  00000001404F0110: jne         00000001404F00F0
  00000001404F0112: mov         rcx,qword ptr [00000001432CDD78h]
  00000001404F0119: mov         rbx,qword ptr [r15]
  00000001404F011C: call        00000001401D9090
  00000001404F0121: mov         rcx,r15
  00000001404F0124: mov         rdx,rax
  00000001404F0127: call        qword ptr [rbx+0000000000000208h]
  00000001404F012D: mov         rcx,qword ptr [00000001432CDD70h]
  00000001404F0134: mov         rdx,rdi
  00000001404F0137: mov         r8,qword ptr [rcx+10h]
  00000001404F013B: mov         rbp,rax
  00000001404F013E: mov         rcx,qword ptr [r8]
  00000001404F0141: cmp         byte ptr [rcx],dl
  00000001404F0143: je          00000001404F0166
  00000001404F0145: mov         rcx,r8
  00000001404F0148: cmp         qword ptr [rcx+8],1
  00000001404F014D: je          00000001404F01A2
  00000001404F014F: inc         rdx
  00000001404F0152: inc         edi
  00000001404F0154: mov         rcx,rdx
  00000001404F0157: shl         rcx,4
  00000001404F015B: add         rcx,r8
  00000001404F015E: mov         rax,qword ptr [rcx]
  00000001404F0161: cmp         byte ptr [rax],0
  00000001404F0164: jne         00000001404F0148
  00000001404F0166: mov         rcx,qword ptr [00000001432CDD78h]
  00000001404F016D: mov         rbx,qword ptr [r15]
  00000001404F0170: call        00000001401D9090
  00000001404F0175: mov         rcx,r15
  00000001404F0178: mov         rdx,rax
  00000001404F017B: call        qword ptr [rbx+0000000000000208h]
  00000001404F0181: mov         rbx,qword ptr [rsp+50h]
  00000001404F0186: mov         rdi,rax
  00000001404F0189: jmp         00000001404F01B9
  00000001404F018B: movsxd      rax,r8d
  00000001404F018E: lea         rcx,[rax*4+0000000000000011h]
  00000001404F0196: add         rcx,rax
  00000001404F0199: lea         rcx,[r12+rcx*8]
  00000001404F019D: jmp         00000001404F0119
  00000001404F01A2: movsxd      rax,edi
  00000001404F01A5: lea         rcx,[rax*4+0000000000000011h]
  00000001404F01AD: add         rcx,rax
  00000001404F01B0: lea         rcx,[r12+rcx*8]
  00000001404F01B4: jmp         00000001404F016D
  00000001404F01B6: mov         rbp,rdi
  00000001404F01B9: mov         r15,qword ptr [rsp+60h]
  00000001404F01BE: mov         r12,qword ptr [rsp+58h]
  00000001404F01C3: test        r14,r14
  00000001404F01C6: je          00000001404F02C0
  00000001404F01CC: test        rbp,rbp
  00000001404F01CF: je          00000001404F0249

  Summary

     1A7D000 .text

````
