# world-spawner-presentation-disasm.txt

Original: [work/world-spawner-presentation-disasm.txt](<../../../../work/world-spawner-presentation-disasm.txt>)

SHA-256: `83C7A287061C55B68BF9D520373ACD76CF24B3A037CF025D8E783DEF25EDE9AB`; 12828 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe


  0000000140759A50: push        rbx
  0000000140759A52: push        rbp
  0000000140759A53: push        rsi
  0000000140759A54: push        rdi
  0000000140759A55: push        r12
  0000000140759A57: push        r13
  0000000140759A59: push        r14
  0000000140759A5B: push        r15
  0000000140759A5D: sub         rsp,58h
  0000000140759A61: mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
  0000000140759A6A: mov         rdi,rdx
  0000000140759A6D: mov         rcx,qword ptr [rcx+10h]
  0000000140759A71: call        00000001408DCAD0
  0000000140759A76: mov         rsi,rax
  0000000140759A79: lea         r12,[rax+0000000000000368h]
  0000000140759A80: mov         rcx,r12
  0000000140759A83: call        0000000140143BB0
  0000000140759A88: test        al,al
  0000000140759A8A: jne         0000000140759AC4
  0000000140759A8C: mov         rbx,qword ptr [rdi]
  0000000140759A8F: mov         rcx,r12
  0000000140759A92: call        00000001401D9090
  0000000140759A97: mov         rdx,rax
  0000000140759A9A: mov         rcx,rdi
  0000000140759A9D: call        qword ptr [rbx+68h]
  0000000140759AA0: lea         rdx,[rsi+00000000000001C0h]
  0000000140759AA7: test        dword ptr [rdx+8],0FFFFFFh
  0000000140759AAE: je          0000000140759ABC
  0000000140759AB0: lea         rcx,[rdi+00000000000001C0h]
  0000000140759AB7: call        0000000140759660
  0000000140759ABC: mov         rcx,rdi
  0000000140759ABF: call        0000000140878830
  0000000140759AC4: mov         rax,qword ptr [rdi]
  0000000140759AC7: mov         rcx,rdi
  0000000140759ACA: call        qword ptr [rax+0000000000000090h]
  0000000140759AD0: mov         rcx,rdi
  0000000140759AD3: call        0000000140878470
  0000000140759AD8: mov         r14,rax
  0000000140759ADB: mov         qword ptr [rsp+00000000000000B8h],rax
  0000000140759AE3: xor         r8d,r8d
  0000000140759AE6: lea         rdx,[00000001439B4480h]
  0000000140759AED: mov         rcx,rdi
  0000000140759AF0: call        0000000140877720
  0000000140759AF5: mov         rdi,rax
  0000000140759AF8: mov         qword ptr [rsp+00000000000000B0h],rax
  0000000140759B00: test        r14,r14
  0000000140759B03: je          0000000140759D3F
  0000000140759B09: test        rax,rax
  0000000140759B0C: je          0000000140759D3F
  0000000140759B12: xor         r13d,r13d
  0000000140759B15: mov         rcx,qword ptr [r14+0000000000001FE8h]
  0000000140759B1C: test        dword ptr [rcx+70h],0FFFFFFh
  0000000140759B23: jle         0000000140759D05
  0000000140759B29: xor         esi,esi
  0000000140759B2B: mov         qword ptr [rsp+00000000000000A0h],rsi
  0000000140759B33: nop         word ptr [rax+rax+0000000000000000h]
  0000000140759B40: mov         r15,qword ptr [rcx+68h]
  0000000140759B44: add         r15,rsi
  0000000140759B47: mov         qword ptr [rsp+38h],0
  0000000140759B50: call        00000001401548E0
  0000000140759B55: movzx       eax,byte ptr [rsp+44h]
  0000000140759B5A: mov         ecx,dword ptr [rsp+40h]
  0000000140759B5E: and         ecx,0FF000000h
  0000000140759B64: or          eax,3Fh
  0000000140759B67: and         ecx,0FFFFFFh
  0000000140759B6D: or          ecx,5000000h
  0000000140759B73: mov         dword ptr [rsp+40h],ecx
  0000000140759B77: and         eax,0FFFFFF3Fh
  0000000140759B7C: mov         dword ptr [rsp+44h],eax
  0000000140759B80: test        al,40h
  0000000140759B82: jne         0000000140759BCC
  0000000140759B84: mov         rbx,qword ptr [rsp+38h]
  0000000140759B89: test        rbx,rbx
  0000000140759B8C: je          0000000140759BBA
  0000000140759B8E: call        00000001401548E0
  0000000140759B93: mov         r10,qword ptr [rax]
  0000000140759B96: mov         qword ptr [rsp+20h],10h
  0000000140759B9F: mov         r9,rbx
  0000000140759BA2: mov         r8d,1ECh
  0000000140759BA8: lea         rdx,[0000000141AB2230h]
  0000000140759BAF: mov         rcx,rax
  0000000140759BB2: call        qword ptr [r10+10h]
  0000000140759BB6: mov         ecx,dword ptr [rsp+40h]
  0000000140759BBA: mov         qword ptr [rsp+38h],0
  0000000140759BC3: movzx       eax,byte ptr [rsp+44h]
  0000000140759BC8: mov         dword ptr [rsp+44h],eax
  0000000140759BCC: and         ecx,0FF000000h
  0000000140759BD2: mov         dword ptr [rsp+40h],ecx
  0000000140759BD6: xor         ebp,ebp
  0000000140759BD8: test        dword ptr [r15+8],0FFFFFFh
  0000000140759BE0: jle         0000000140759C7A
  0000000140759BE6: xor         r14d,r14d
  0000000140759BE9: nop         dword ptr [rax+0000000000000000h]
  0000000140759BF0: mov         rax,qword ptr [r15]
  0000000140759BF3: mov         rsi,qword ptr [r14+rax]
  0000000140759BF7: xor         edi,edi
  0000000140759BF9: test        dword ptr [rsi+60h],0FFFFFFh
  0000000140759C00: jle         0000000140759C4F
  0000000140759C02: xor         ebx,ebx
  0000000140759C04: nop         word ptr [rax+rax+0000000000000000h]
  0000000140759C10: mov         rax,qword ptr [rsi+58h]
  0000000140759C14: mov         rcx,qword ptr [rbx+rax]
  0000000140759C18: mov         edx,dword ptr [rcx+30h]
  0000000140759C1B: mov         rcx,r12
  0000000140759C1E: call        0000000140785340
  0000000140759C23: mov         qword ptr [rsp+00000000000000A8h],rax
  0000000140759C2B: lea         rdx,[rsp+00000000000000A8h]
  0000000140759C33: lea         rcx,[rsp+38h]
  0000000140759C38: call        00000001408918F0
  0000000140759C3D: inc         edi
  0000000140759C3F: lea         rbx,[rbx+8]
  0000000140759C43: mov         eax,dword ptr [rsi+60h]
  0000000140759C46: and         eax,0FFFFFFh
  0000000140759C4B: cmp         edi,eax
  0000000140759C4D: jl          0000000140759C10
  0000000140759C4F: inc         ebp
  0000000140759C51: add         r14,38h
  0000000140759C55: mov         eax,dword ptr [r15+8]
  0000000140759C59: and         eax,0FFFFFFh
  0000000140759C5E: cmp         ebp,eax
  0000000140759C60: jl          0000000140759BF0
  0000000140759C62: mov         rdi,qword ptr [rsp+00000000000000B0h]
  0000000140759C6A: mov         rsi,qword ptr [rsp+00000000000000A0h]
  0000000140759C72: mov         r14,qword ptr [rsp+00000000000000B8h]
  0000000140759C7A: lea         r8,[rsp+38h]
  0000000140759C7F: mov         edx,r13d
  0000000140759C82: mov         rcx,rdi
  0000000140759C85: call        0000000140721840
  0000000140759C8A: nop
  0000000140759C8B: test        byte ptr [rsp+44h],40h
  0000000140759C90: jne         0000000140759CD6
  0000000140759C92: mov         rbx,qword ptr [rsp+38h]
  0000000140759C97: test        rbx,rbx
  0000000140759C9A: je          0000000140759CC4
  0000000140759C9C: call        00000001401548E0
  0000000140759CA1: mov         r10,qword ptr [rax]
  0000000140759CA4: mov         qword ptr [rsp+20h],10h
  0000000140759CAD: mov         r9,rbx
  0000000140759CB0: mov         r8d,1ECh
  0000000140759CB6: lea         rdx,[0000000141AB2230h]
  0000000140759CBD: mov         rcx,rax
  0000000140759CC0: call        qword ptr [r10+10h]
  0000000140759CC4: mov         qword ptr [rsp+38h],0
  0000000140759CCD: movzx       eax,byte ptr [rsp+44h]
  0000000140759CD2: mov         dword ptr [rsp+44h],eax
  0000000140759CD6: and         dword ptr [rsp+40h],0FF000000h
  0000000140759CDE: inc         r13d
  0000000140759CE1: add         rsi,18h
  0000000140759CE5: mov         qword ptr [rsp+00000000000000A0h],rsi
  0000000140759CED: mov         rcx,qword ptr [r14+0000000000001FE8h]
  0000000140759CF4: mov         eax,dword ptr [rcx+70h]
  0000000140759CF7: and         eax,0FFFFFFh
  0000000140759CFC: cmp         r13d,eax
  0000000140759CFF: jl          0000000140759B40
  0000000140759D05: mov         rax,qword ptr [r14+0000000000001FE8h]
  0000000140759D0C: mov         ebx,dword ptr [rax+70h]
  0000000140759D0F: and         ebx,0FFFFFFh
  0000000140759D15: cmp         ebx,1
  0000000140759D18: jle         0000000140759D3F
  0000000140759D1A: cmp         ebx,4
  0000000140759D1D: jg          0000000140759D3F
  0000000140759D1F: nop
  0000000140759D20: lea         edx,[rbx-1]
  0000000140759D23: mov         rcx,rdi
  0000000140759D26: call        0000000140720DE0
  0000000140759D2B: mov         r8,rax
  0000000140759D2E: mov         edx,ebx
  0000000140759D30: mov         rcx,rdi
  0000000140759D33: call        0000000140721840
  0000000140759D38: inc         ebx
  0000000140759D3A: cmp         ebx,4
  0000000140759D3D: jle         0000000140759D20
  0000000140759D3F: add         rsp,58h
  0000000140759D43: pop         r15
  0000000140759D45: pop         r14
  0000000140759D47: pop         r13
  0000000140759D49: pop         r12
  0000000140759D4B: pop         rdi
  0000000140759D4C: pop         rsi
  0000000140759D4D: pop         rbp
  0000000140759D4E: pop         rbx
  0000000140759D4F: ret
  0000000140759D50: mov         qword ptr [rsp+8],rbx
  0000000140759D55: mov         qword ptr [rsp+10h],rbp
  0000000140759D5A: mov         qword ptr [rsp+18h],rsi
  0000000140759D5F: mov         qword ptr [rsp+20h],rdi
  0000000140759D64: push        r14
  0000000140759D66: sub         rsp,20h
  0000000140759D6A: mov         rbx,qword ptr [rcx+00000000000003F8h]
  0000000140759D71: mov         r14,rdx
  0000000140759D74: mov         rsi,rcx
  0000000140759D77: test        rbx,rbx
  0000000140759D7A: je          0000000140759D9A
  0000000140759D7C: lea         rdx,[00000001439DCD30h]
  0000000140759D83: xor         r8d,r8d
  0000000140759D86: mov         rcx,r14
  0000000140759D89: call        0000000140877720
  0000000140759D8E: test        rax,rax
  0000000140759D91: je          0000000140759D9A
  0000000140759D93: mov         qword ptr [rax+00000000000000E8h],rbx
  0000000140759D9A: mov         rdi,qword ptr [rsi+0000000000000360h]
  0000000140759DA1: test        rdi,rdi
  0000000140759DA4: je          0000000140759E1E
  0000000140759DA6: mov         rbx,qword ptr [rdi+68h]
  0000000140759DAA: mov         edi,dword ptr [rdi+70h]
  0000000140759DAD: and         edi,0FFFFFFh
  0000000140759DB3: imul        rdi,rdi,0B8h
  0000000140759DBA: add         rdi,rbx
  0000000140759DBD: cmp         rbx,rdi
  0000000140759DC0: je          0000000140759E1E
  0000000140759DC2: lea         rcx,[rsi+0000000000000368h]
  0000000140759DC9: call        00000001401D9090
  0000000140759DCE: lea         rcx,[rbx+28h]
  0000000140759DD2: mov         rdx,rax
  0000000140759DD5: call        0000000140141420
  0000000140759DDA: test        eax,eax
  0000000140759DDC: je          0000000140759DEC
  0000000140759DDE: add         rbx,0B8h
  0000000140759DE5: cmp         rbx,rdi
  0000000140759DE8: jne         0000000140759DC2
  0000000140759DEA: jmp         0000000140759E1E
  0000000140759DEC: mov         rax,qword ptr [rbx+00000000000000B0h]
  0000000140759DF3: cmp         byte ptr [rax],0
  0000000140759DF6: je          0000000140759E1E
  0000000140759DF8: lea         rdx,[00000001439DCD30h]
  0000000140759DFF: xor         r8d,r8d
  0000000140759E02: mov         rcx,r14
  0000000140759E05: call        0000000140877720
  0000000140759E0A: test        rax,rax
  0000000140759E0D: je          0000000140759E1E
  0000000140759E0F: lea         rdx,[rbx+00000000000000B0h]
  0000000140759E16: mov         rcx,rax
  0000000140759E19: call        0000000140613A60
  0000000140759E1E: cmp         byte ptr [rsi+0000000000000478h],0
  0000000140759E25: je          0000000140759E4F
  0000000140759E27: cmp         byte ptr [rsi+0000000000000479h],0
  0000000140759E2E: je          0000000140759E4F
  0000000140759E30: lea         rdx,[00000001439CBC20h]
  0000000140759E37: xor         r8d,r8d
  0000000140759E3A: mov         rcx,r14
  0000000140759E3D: call        0000000140877720
  0000000140759E42: test        rax,rax
  0000000140759E45: je          0000000140759E4F
  0000000140759E47: mov         rcx,rax
  0000000140759E4A: call        00000001406AF1C0
  0000000140759E4F: mov         rbx,qword ptr [rsp+30h]
  0000000140759E54: mov         rbp,qword ptr [rsp+38h]
  0000000140759E59: mov         rsi,qword ptr [rsp+40h]
  0000000140759E5E: mov         rdi,qword ptr [rsp+48h]
  0000000140759E63: add         rsp,20h
  0000000140759E67: pop         r14
  0000000140759E69: ret
  0000000140759E6A: int         3
  0000000140759E6B: int         3
  0000000140759E6C: int         3
  0000000140759E6D: int         3
  0000000140759E6E: int         3
  0000000140759E6F: int         3
  0000000140759E70: 48

````
