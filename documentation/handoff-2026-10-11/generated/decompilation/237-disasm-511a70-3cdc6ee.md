# disasm-511a70.txt

Original: [work/disasm-511a70.txt](<../../../../work/disasm-511a70.txt>)

SHA-256: `62DC056BC8868E6C9C0747697674305E63A273C12B8169D487373CD06B45467F`; 19298 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  0000000140511A70: mov         qword ptr [rsp+10h],rbx
  0000000140511A75: push        rbp
  0000000140511A76: push        rsi
  0000000140511A77: push        rdi
  0000000140511A78: push        r14
  0000000140511A7A: push        r15
  0000000140511A7C: sub         rsp,20h
  0000000140511A80: mov         rax,qword ptr [00000001432CDCE8h]
  0000000140511A87: mov         r15,qword ptr [rcx+78h]
  0000000140511A8B: mov         rbp,qword ptr [00000001439833E0h]
  0000000140511A92: mov         r10,qword ptr [rax+10h]
  0000000140511A96: xor         r9d,r9d
  0000000140511A99: movsxd      rsi,edx
  0000000140511A9C: mov         rax,qword ptr [r10]
  0000000140511A9F: mov         rbx,rcx
  0000000140511AA2: mov         r11d,r9d
  0000000140511AA5: movzx       r14d,byte ptr [rax]
  0000000140511AA9: mov         r8d,r9d
  0000000140511AAC: test        r14b,r14b
  0000000140511AAF: je          0000000140511AD2
  0000000140511AB1: mov         rcx,r10
  0000000140511AB4: cmp         rsi,qword ptr [rcx+8]
  0000000140511AB8: je          0000000140511B07
  0000000140511ABA: inc         r8
  0000000140511ABD: inc         r11d
  0000000140511AC0: mov         rcx,r8
  0000000140511AC3: shl         rcx,4
  0000000140511AC7: add         rcx,r10
  0000000140511ACA: mov         rax,qword ptr [rcx]
  0000000140511ACD: cmp         byte ptr [rax],r9b
  0000000140511AD0: jne         0000000140511AB4
  0000000140511AD2: mov         rdi,rbp
  0000000140511AD5: add         rdi,90h
  0000000140511ADC: mov         rdx,r9
  0000000140511ADF: test        r14b,r14b
  0000000140511AE2: je          0000000140511B33
  0000000140511AE4: mov         rcx,r10
  0000000140511AE7: cmp         rsi,qword ptr [rcx+8]
  0000000140511AEB: je          0000000140511B1E
  0000000140511AED: inc         rdx
  0000000140511AF0: inc         r9d
  0000000140511AF3: mov         rcx,rdx
  0000000140511AF6: shl         rcx,4
  0000000140511AFA: add         rcx,r10
  0000000140511AFD: mov         rax,qword ptr [rcx]
  0000000140511B00: cmp         byte ptr [rax],0
  0000000140511B03: jne         0000000140511AE7
  0000000140511B05: jmp         0000000140511B33
  0000000140511B07: movsxd      rax,r11d
  0000000140511B0A: lea         rdi,[r15+00000000000000A0h]
  0000000140511B11: lea         rcx,[rax+rax*8]
  0000000140511B15: shl         rcx,5
  0000000140511B19: add         rdi,rcx
  0000000140511B1C: jmp         0000000140511AD5
  0000000140511B1E: movsxd      rax,r9d
  0000000140511B21: lea         rbp,[r15+00000000000000A0h]
  0000000140511B28: lea         rcx,[rax+rax*8]
  0000000140511B2C: shl         rcx,5
  0000000140511B30: add         rbp,rcx
  0000000140511B33: mov         rax,qword ptr [rbx]
  0000000140511B36: mov         rdx,rbp
  0000000140511B39: mov         rcx,rbx
  0000000140511B3C: call        qword ptr [rax+68h]
  0000000140511B3F: test        al,al
  0000000140511B41: je          0000000140511F73
  0000000140511B47: cmp         esi,1Eh
  0000000140511B4A: ja          0000000140511F73
  0000000140511B50: lea         rcx,[0000000140000000h]
  0000000140511B57: mov         eax,dword ptr [rcx+rsi*4+0000000000511F84h]
  0000000140511B5E: add         rax,rcx
  0000000140511B61: jmp         rax
  0000000140511B63: xor         r9d,r9d
  0000000140511B66: lea         r8,[00000001439374E0h]
  0000000140511B6D: jmp         0000000140511F68
  0000000140511B72: xor         r9d,r9d
  0000000140511B75: lea         r8,[00000001432FB820h]
  0000000140511B7C: jmp         0000000140511F68
  0000000140511B81: xor         r8d,r8d
  0000000140511B84: mov         rdx,rdi
  0000000140511B87: mov         rcx,rbx
  0000000140511B8A: mov         rbx,qword ptr [rsp+58h]
  0000000140511B8F: add         rsp,20h
  0000000140511B93: pop         r15
  0000000140511B95: pop         r14
  0000000140511B97: pop         rdi
  0000000140511B98: pop         rsi
  0000000140511B99: pop         rbp
  0000000140511B9A: jmp         0000000140512990
  0000000140511B9F: mov         rsi,qword ptr [00000001422FCD70h]
  0000000140511BA6: mov         edx,dword ptr [rdi+8]
  0000000140511BA9: lea         r9,[rsp+50h]
  0000000140511BAE: mov         rax,qword ptr [rsi]
  0000000140511BB1: lea         r8,[rsp+60h]
  0000000140511BB6: mov         rcx,rsi
  0000000140511BB9: call        qword ptr [rax+0000000000000190h]
  0000000140511BBF: test        al,al
  0000000140511BC1: je          0000000140511F73
  0000000140511BC7: mov         r9d,dword ptr [rsp+50h]
  0000000140511BCC: mov         r8d,dword ptr [rsp+60h]
  0000000140511BD1: mov         edx,dword ptr [rdi+8]
  0000000140511BD4: mov         rcx,rsi
  0000000140511BD7: call        0000000140345890
  0000000140511BDC: mov         byte ptr [rbx+0000000000000308h],1
  0000000140511BE3: mov         rbx,qword ptr [rsp+58h]
  0000000140511BE8: add         rsp,20h
  0000000140511BEC: pop         r15
  0000000140511BEE: pop         r14
  0000000140511BF0: pop         rdi
  0000000140511BF1: pop         rsi
  0000000140511BF2: pop         rbp
  0000000140511BF3: ret
  0000000140511BF4: mov         edx,dword ptr [rdi+8]
  0000000140511BF7: lea         rcx,[00000001432FB1D0h]
  0000000140511BFE: mov         r8b,1
  0000000140511C01: call        00000001401028E0
  0000000140511C06: mov         byte ptr [rbx+0000000000000308h],1
  0000000140511C0D: mov         rbx,qword ptr [rsp+58h]
  0000000140511C12: add         rsp,20h
  0000000140511C16: pop         r15
  0000000140511C18: pop         r14
  0000000140511C1A: pop         rdi
  0000000140511C1B: pop         rsi
  0000000140511C1C: pop         rbp
  0000000140511C1D: ret
  0000000140511C1E: xor         r9d,r9d
  0000000140511C21: lea         r8,[0000000143937260h]
  0000000140511C28: jmp         0000000140511F68
  0000000140511C2D: xor         r9d,r9d
  0000000140511C30: lea         r8,[00000001432FA8E0h]
  0000000140511C37: jmp         0000000140511F68
  0000000140511C3C: mov         rax,qword ptr [rbx]
  0000000140511C3F: mov         edx,18h
  0000000140511C44: mov         rcx,rbx
  0000000140511C47: call        qword ptr [rax+0000000000000080h]
  0000000140511C4D: test        eax,eax
  0000000140511C4F: jne         0000000140511F73
  0000000140511C55: movd        xmm1,dword ptr [rdi+8]
  0000000140511C5A: lea         rcx,[00000001425B1E00h]
  0000000140511C61: mov         r8b,1
  0000000140511C64: cvtdq2ps    xmm1,xmm1
  0000000140511C67: mulss       xmm1,dword ptr [0000000141ABF15Ch]
  0000000140511C6F: addss       xmm1,dword ptr [0000000141ADFD4Ch]
  0000000140511C77: divss       xmm1,dword ptr [0000000141ADFD4Ch]
  0000000140511C7F: mov         rbx,qword ptr [rsp+58h]
  0000000140511C84: add         rsp,20h
  0000000140511C88: pop         r15
  0000000140511C8A: pop         r14
  0000000140511C8C: pop         rdi
  0000000140511C8D: pop         rsi
  0000000140511C8E: pop         rbp
  0000000140511C8F: jmp         0000000140102880
  0000000140511C94: xor         r8d,r8d
  0000000140511C97: mov         rdx,rdi
  0000000140511C9A: mov         rcx,rbx
  0000000140511C9D: mov         rbx,qword ptr [rsp+58h]
  0000000140511CA2: add         rsp,20h
  0000000140511CA6: pop         r15
  0000000140511CA8: pop         r14
  0000000140511CAA: pop         rdi
  0000000140511CAB: pop         rsi
  0000000140511CAC: pop         rbp
  0000000140511CAD: jmp         0000000140512CA0
  0000000140511CB2: xor         r9d,r9d
  0000000140511CB5: lea         r8,[00000001432FB450h]
  0000000140511CBC: jmp         0000000140511F68
  0000000140511CC1: xor         r9d,r9d
  0000000140511CC4: lea         r8,[00000001439A40A0h]
  0000000140511CCB: jmp         0000000140511F68
  0000000140511CD0: lea         r8,[0000000143982DF0h]
  0000000140511CD7: mov         edx,9
  0000000140511CDC: mov         rcx,rbx
  0000000140511CDF: mov         rbx,qword ptr [rsp+58h]
  0000000140511CE4: add         rsp,20h
  0000000140511CE8: pop         r15
  0000000140511CEA: pop         r14
  0000000140511CEC: pop         rdi
  0000000140511CED: pop         rsi
  0000000140511CEE: pop         rbp
  0000000140511CEF: jmp         0000000140512BF0
  0000000140511CF4: lea         r8,[0000000143982EF0h]
  0000000140511CFB: mov         edx,0Fh
  0000000140511D00: mov         rcx,rbx
  0000000140511D03: mov         rbx,qword ptr [rsp+58h]
  0000000140511D08: add         rsp,20h
  0000000140511D0C: pop         r15
  0000000140511D0E: pop         r14
  0000000140511D10: pop         rdi
  0000000140511D11: pop         rsi
  0000000140511D12: pop         rbp
  0000000140511D13: jmp         0000000140512BF0
  0000000140511D18: mov         edx,dword ptr [rdi+8]
  0000000140511D1B: lea         rcx,[00000001432F88C0h]
  0000000140511D22: mov         r8b,1
  0000000140511D25: mov         rbx,qword ptr [rsp+58h]
  0000000140511D2A: add         rsp,20h
  0000000140511D2E: pop         r15
  0000000140511D30: pop         r14
  0000000140511D32: pop         rdi
  0000000140511D33: pop         rsi
  0000000140511D34: pop         rbp
  0000000140511D35: jmp         00000001401028E0
  0000000140511D3A: lea         r8,[0000000143982F70h]
  0000000140511D41: mov         edx,8
  0000000140511D46: mov         rcx,rbx
  0000000140511D49: mov         rbx,qword ptr [rsp+58h]
  0000000140511D4E: add         rsp,20h
  0000000140511D52: pop         r15
  0000000140511D54: pop         r14
  0000000140511D56: pop         rdi
  0000000140511D57: pop         rsi
  0000000140511D58: pop         rbp
  0000000140511D59: jmp         0000000140512BF0
  0000000140511D5E: lea         r8,[0000000143982D70h]
  0000000140511D65: mov         edx,0Ah
  0000000140511D6A: mov         rcx,rbx
  0000000140511D6D: mov         rbx,qword ptr [rsp+58h]
  0000000140511D72: add         rsp,20h
  0000000140511D76: pop         r15
  0000000140511D78: pop         r14
  0000000140511D7A: pop         rdi
  0000000140511D7B: pop         rsi
  0000000140511D7C: pop         rbp
  0000000140511D7D: jmp         0000000140512BF0
  0000000140511D82: lea         r8,[0000000143982CF0h]
  0000000140511D89: mov         edx,0Ch
  0000000140511D8E: mov         rcx,rbx
  0000000140511D91: mov         rbx,qword ptr [rsp+58h]
  0000000140511D96: add         rsp,20h
  0000000140511D9A: pop         r15
  0000000140511D9C: pop         r14
  0000000140511D9E: pop         rdi
  0000000140511D9F: pop         rsi
  0000000140511DA0: pop         rbp
  0000000140511DA1: jmp         0000000140512BF0
  0000000140511DA6: lea         r8,[0000000143982E70h]
  0000000140511DAD: mov         edx,0Bh
  0000000140511DB2: mov         rcx,rbx
  0000000140511DB5: mov         rbx,qword ptr [rsp+58h]
  0000000140511DBA: add         rsp,20h
  0000000140511DBE: pop         r15
  0000000140511DC0: pop         r14
  0000000140511DC2: pop         rdi
  0000000140511DC3: pop         rsi
  0000000140511DC4: pop         rbp
  0000000140511DC5: jmp         0000000140512BF0
  0000000140511DCA: cmp         dword ptr [rdi+8],0
  0000000140511DCE: mov         r8b,1
  0000000140511DD1: lea         rcx,[00000001439830F0h]
  0000000140511DD8: je          0000000140511E08
  0000000140511DDA: mov         edx,1
  0000000140511DDF: call        00000001401028E0
  0000000140511DE4: lea         rcx,[000000014397A750h]
  0000000140511DEB: mov         r8b,1
  0000000140511DEE: mov         edx,1
  0000000140511DF3: mov         rbx,qword ptr [rsp+58h]
  0000000140511DF8: add         rsp,20h
  0000000140511DFC: pop         r15
  0000000140511DFE: pop         r14
  0000000140511E00: pop         rdi
  0000000140511E01: pop         rsi
  0000000140511E02: pop         rbp
  0000000140511E03: jmp         00000001401028E0
  0000000140511E08: xor         edx,edx
  0000000140511E0A: call        00000001401028E0
  0000000140511E0F: lea         rcx,[000000014397A750h]
  0000000140511E16: mov         r8b,1
  0000000140511E19: mov         edx,2
  0000000140511E1E: mov         rbx,qword ptr [rsp+58h]
  0000000140511E23: add         rsp,20h
  0000000140511E27: pop         r15
  0000000140511E29: pop         r14
  0000000140511E2B: pop         rdi
  0000000140511E2C: pop         rsi
  0000000140511E2D: pop         rbp
  0000000140511E2E: jmp         00000001401028E0
  0000000140511E33: lea         r8,[0000000143982FF0h]
  0000000140511E3A: mov         edx,0Dh
  0000000140511E3F: mov         rcx,rbx
  0000000140511E42: mov         rbx,qword ptr [rsp+58h]
  0000000140511E47: add         rsp,20h
  0000000140511E4B: pop         r15
  0000000140511E4D: pop         r14
  0000000140511E4F: pop         rdi
  0000000140511E50: pop         rsi
  0000000140511E51: pop         rbp
  0000000140511E52: jmp         0000000140512BF0
  0000000140511E57: xor         r9d,r9d
  0000000140511E5A: lea         r8,[000000014398CA10h]
  0000000140511E61: jmp         0000000140511F68
  0000000140511E66: xor         r9d,r9d
  0000000140511E69: lea         r8,[000000014398CA90h]
  0000000140511E70: jmp         0000000140511F68
  0000000140511E75: xor         r8d,r8d
  0000000140511E78: mov         rdx,rdi
  0000000140511E7B: mov         rcx,rbx
  0000000140511E7E: mov         rbx,qword ptr [rsp+58h]
  0000000140511E83: add         rsp,20h
  0000000140511E87: pop         r15
  0000000140511E89: pop         r14
  0000000140511E8B: pop         rdi
  0000000140511E8C: pop         rsi
  0000000140511E8D: pop         rbp
  0000000140511E8E: jmp         0000000140512AC0
  0000000140511E93: movsxd      rax,dword ptr [rdi+8]
  0000000140511E97: mov         r8b,1
  0000000140511E9A: movss       xmm1,dword ptr [rcx+rax*4+0000000001B70DE0h]
  0000000140511EA3: lea         rcx,[000000014397BF40h]
  0000000140511EAA: mov         rbx,qword ptr [rsp+58h]
  0000000140511EAF: add         rsp,20h
  0000000140511EB3: pop         r15
  0000000140511EB5: pop         r14
  0000000140511EB7: pop         rdi
  0000000140511EB8: pop         rsi
  0000000140511EB9: pop         rbp
  0000000140511EBA: jmp         0000000140102880
  0000000140511EBF: mov         rax,qword ptr [rbx]
  0000000140511EC2: mov         edx,19h
  0000000140511EC7: mov         rcx,rbx
  0000000140511ECA: call        qword ptr [rax+0000000000000080h]
  0000000140511ED0: mov         r8b,1
  0000000140511ED3: lea         rcx,[000000014397C4C0h]
  0000000140511EDA: cmp         eax,3
  0000000140511EDD: jne         0000000140511EFC
  0000000140511EDF: movss       xmm1,dword ptr [0000000141AB3828h]
  0000000140511EE7: mov         rbx,qword ptr [rsp+58h]
  0000000140511EEC: add         rsp,20h
  0000000140511EF0: pop         r15
  0000000140511EF2: pop         r14
  0000000140511EF4: pop         rdi
  0000000140511EF5: pop         rsi
  0000000140511EF6: pop         rbp
  0000000140511EF7: jmp         0000000140102880
  0000000140511EFC: movd        xmm1,dword ptr [rdi+8]
  0000000140511F01: cvtdq2ps    xmm1,xmm1
  0000000140511F04: mov         rbx,qword ptr [rsp+58h]
  0000000140511F09: add         rsp,20h
  0000000140511F0D: pop         r15
  0000000140511F0F: pop         r14
  0000000140511F11: pop         rdi
  0000000140511F12: pop         rsi
  0000000140511F13: pop         rbp
  0000000140511F14: jmp         0000000140102880
  0000000140511F19: xor         r9d,r9d
  0000000140511F1C: lea         r8,[00000001432EF770h]
  0000000140511F23: jmp         0000000140511F68
  0000000140511F25: lea         r8,[0000000143983070h]
  0000000140511F2C: jmp         0000000140511F65
  0000000140511F2E: lea         r8,[00000001432F7F40h]
  0000000140511F35: jmp         0000000140511F65
  0000000140511F37: lea         r8,[00000001439A4220h]
  0000000140511F3E: jmp         0000000140511F65
  0000000140511F40: lea         r8,[00000001432F7FC0h]
  0000000140511F47: jmp         0000000140511F65
  0000000140511F49: lea         r8,[000000014397AD50h]
  0000000140511F50: mov         r9b,1
  0000000140511F53: mov         rdx,rdi
  0000000140511F56: mov         rcx,rbx
  0000000140511F59: call        0000000140501B00
  0000000140511F5E: lea         r8,[00000001432EF880h]
  0000000140511F65: mov         r9b,1
  0000000140511F68: mov         rdx,rdi
  0000000140511F6B: mov         rcx,rbx
  0000000140511F6E: call        0000000140501B00
  0000000140511F73: mov         rbx,qword ptr [rsp+58h]
  0000000140511F78: add         rsp,20h
  0000000140511F7C: pop         r15
  0000000140511F7E: pop         r14
  0000000140511F80: pop         rdi
  0000000140511F81: pop         rsi
  0000000140511F82: pop         rbp
  0000000140511F83: ret
  0000000140511F84: sbb         dword ptr [rbx],1B9F0051h
  0000000140511F8A: push        rcx
  0000000140511F8B: add         ah,dh
  0000000140511F8D: sbb         edx,dword ptr [rcx]
  0000000140511F90: movsxd      ebx,dword ptr [rbx]
  0000000140511F92: push        rcx
  0000000140511F93: add         cl,al
  0000000140511F95: sbb         al,51h
  0000000140511F97: add         byte ptr [rdx+1Bh],dh
  0000000140511F9A: push        rcx
  0000000140511F9B: add         byte ptr [rsp+rbx+000000001CB20051h],dl
  0000000140511FA2: push        rcx
  0000000140511FA3: add         byte ptr [rdx],bh
  0000000140511FA5: sbb         eax,1CD00051h
  0000000140511FAA: push        rcx
  0000000140511FAB: add         byte ptr [rsi+1Dh],bl
  0000000140511FAE: push        rcx
  0000000140511FAF: add         byte ptr [rsi+FFFFFFFF8200511Dh],ah
  0000000140511FB5: sbb         eax,1E330051h
  0000000140511FBA: push        rcx
  0000000140511FBB: add         dl,cl
  0000000140511FBD: sbb         eax,1CF40051h
  0000000140511FC2: push        rcx
  0000000140511FC3: add         byte ptr [rax],bl
  0000000140511FC5: sbb         eax,1F2E0051h
  0000000140511FCA: push        rcx
  0000000140511FCB: add         byte ptr [rdi],dh
  0000000140511FCD: 1F
  0000000140511FCE: push        rcx
  0000000140511FCF: add         byte ptr [00000001805170F4h],ah
  0000000140511FD5: 1F
  0000000140511FD6: push        rcx
  0000000140511FD7: add         byte ptr [rcx+1Fh],cl
  0000000140511FDA: push        rcx
  0000000140511FDB: add         byte ptr [rdi+1Eh],dl
  0000000140511FDE: push        rcx
  0000000140511FDF: add         byte ptr [rsi+1Eh],ah
  0000000140511FE2: push        rcx
  0000000140511FE3: add         byte ptr [rsi],bl
  0000000140511FE5: sbb         al,51h
  0000000140511FE7: add         byte ptr [rbp+1Eh],dh
  0000000140511FEA: push        rcx
  0000000140511FEB: add         byte ptr [rdi+FFFFFFFF9300511Eh],bh
  0000000140511FF1: 1E
  0000000140511FF2: push        rcx
  0000000140511FF3: add         byte ptr [rsp+rbx],bh
  0000000140511FF6: push        rcx
  0000000140511FF7: add         byte ptr [0000000159517119h],ch
  0000000140511FFD: 1F
  0000000140511FFE: push        rcx
  0000000140511FFF: 00
  0000000140512000: 40

  Summary

     1A7D000 .text

````
