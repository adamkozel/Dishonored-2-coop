# disasm-78aad0.txt

Original: [work/disasm-78aad0.txt](<../../../../work/disasm-78aad0.txt>)

SHA-256: `F833A23785B6A37E9053DB05E400043F5CA27FD12857BA30FCF2D88C4789CA5C`; 10718 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  000000014078AAD0: push        rbx
  000000014078AAD2: sub         rsp,70h
  000000014078AAD6: test        byte ptr [rcx+00000000000009C0h],4
  000000014078AADD: mov         rbx,rcx
  000000014078AAE0: je          000000014078ABC5
  000000014078AAE6: add         rcx,50h
  000000014078AAEA: mov         edx,2
  000000014078AAEF: mov         qword ptr [rsp+0000000000000088h],rdi
  000000014078AAF7: call        00000001403DA910
  000000014078AAFC: lea         r8,[rsp+58h]
  000000014078AB01: lea         rdx,[rsp+38h]
  000000014078AB06: lea         rcx,[rsp+20h]
  000000014078AB0B: mov         r9d,1
  000000014078AB11: mov         rdi,rax
  000000014078AB14: call        0000000140028980
  000000014078AB19: call        0000000140034920
  000000014078AB1E: lea         rdx,[rsp+0000000000000080h]
  000000014078AB26: lea         rcx,[rsp+20h]
  000000014078AB2B: mov         dword ptr [rsp+0000000000000080h],eax
  000000014078AB32: call        0000000140028700
  000000014078AB37: lea         rdx,[rbx+00000000000006E8h]
  000000014078AB3E: lea         rcx,[rbx+0000000000000990h]
  000000014078AB45: lea         r8,[rsp+20h]
  000000014078AB4A: call        0000000140028A90
  000000014078AB4F: test        rdi,rdi
  000000014078AB52: je          000000014078ABBD
  000000014078AB54: mov         ecx,dword ptr [rdi+0000000000000098h]
  000000014078AB5A: mov         eax,dword ptr [rdi+0000000000000080h]
  000000014078AB60: shr         rcx,16h
  000000014078AB64: shr         rax,16h
  000000014078AB68: and         cl,al
  000000014078AB6A: test        cl,1
  000000014078AB6D: je          000000014078AB91
  000000014078AB6F: mov         rcx,rbx
  000000014078AB72: call        000000014078A0A0
  000000014078AB77: test        al,al
  000000014078AB79: je          000000014078AB91
  000000014078AB7B: mov         rcx,rbx
  000000014078AB7E: call        000000014078A8C0
  000000014078AB83: mov         rdi,qword ptr [rsp+0000000000000088h]
  000000014078AB8B: add         rsp,70h
  000000014078AB8F: pop         rbx
  000000014078AB90: ret
  000000014078AB91: mov         ecx,dword ptr [rdi+0000000000000090h]
  000000014078AB97: mov         eax,dword ptr [rdi+0000000000000080h]
  000000014078AB9D: shr         rcx,16h
  000000014078ABA1: shr         rax,16h
  000000014078ABA5: and         cl,al
  000000014078ABA7: test        cl,1
  000000014078ABAA: je          000000014078ABBD
  000000014078ABAC: test        byte ptr [rbx+00000000000009C0h],1
  000000014078ABB3: je          000000014078ABBD
  000000014078ABB5: mov         rcx,rbx
  000000014078ABB8: call        000000014078A930
  000000014078ABBD: mov         rdi,qword ptr [rsp+0000000000000088h]
  000000014078ABC5: add         rsp,70h
  000000014078ABC9: pop         rbx
  000000014078ABCA: ret
  000000014078ABCB: int         3
  000000014078ABCC: int         3
  000000014078ABCD: int         3
  000000014078ABCE: int         3
  000000014078ABCF: int         3
  000000014078ABD0: mov         rax,rsp
  000000014078ABD3: mov         qword ptr [rax+8],rcx
  000000014078ABD7: push        rdi
  000000014078ABD8: sub         rsp,50h
  000000014078ABDC: mov         qword ptr [rax-18h],0FFFFFFFFFFFFFFFEh
  000000014078ABE4: mov         qword ptr [rax+10h],rbx
  000000014078ABE8: mov         qword ptr [rax+18h],rsi
  000000014078ABEC: mov         rsi,rcx
  000000014078ABEF: mov         byte ptr [rax-20h],1
  000000014078ABF3: mov         byte ptr [rax-28h],1
  000000014078ABF7: mov         edi,2
  000000014078ABFC: mov         dword ptr [rax-30h],edi
  000000014078ABFF: mov         byte ptr [rax-38h],0
  000000014078AC03: xor         r9d,r9d
  000000014078AC06: mov         r8b,1
  000000014078AC09: lea         edx,[rdi-1]
  000000014078AC0C: call        00000001407887E0
  000000014078AC11: nop
  000000014078AC12: lea         rax,[0000000141C60348h]
  000000014078AC19: mov         qword ptr [rsi],rax
  000000014078AC1C: lea         rbx,[rsi+0000000000000088h]
  000000014078AC23: cmp         qword ptr [000000014398ECB0h],0
  000000014078AC2B: jne         000000014078AC47
  000000014078AC2D: lea         rdx,[0000000143DE22D0h]
  000000014078AC34: mov         rcx,qword ptr [0000000142294408h]
  000000014078AC3B: call        000000014014BBF0
  000000014078AC40: mov         qword ptr [000000014398ECB0h],rax
  000000014078AC47: xor         eax,eax
  000000014078AC49: nop         dword ptr [rax+0000000000000000h]
  000000014078AC50: mov         qword ptr [rbx],rax
  000000014078AC53: mov         qword ptr [rbx+8],rax
  000000014078AC57: mov         qword ptr [rbx+10h],rax
  000000014078AC5B: lea         rbx,[rbx+40h]
  000000014078AC5F: mov         qword ptr [rbx-28h],rax
  000000014078AC63: mov         qword ptr [rbx-20h],rax
  000000014078AC67: mov         qword ptr [rbx-18h],rax
  000000014078AC6B: mov         qword ptr [rbx-10h],rax
  000000014078AC6F: mov         qword ptr [rbx-8],rax
  000000014078AC73: dec         rdi
  000000014078AC76: jne         000000014078AC50
  000000014078AC78: mov         qword ptr [rbx],rax
  000000014078AC7B: mov         qword ptr [rbx+8],rax
  000000014078AC7F: mov         qword ptr [rbx+10h],rax
  000000014078AC83: mov         qword ptr [rbx+18h],rax
  000000014078AC87: mov         qword ptr [rbx+20h],rax
  000000014078AC8B: mov         dword ptr [rsi+0000000000000130h],eax
  000000014078AC91: mov         dword ptr [rsi+0000000000000134h],3E4CCCCDh
  000000014078AC9B: mov         rax,rsi
  000000014078AC9E: mov         rbx,qword ptr [rsp+68h]
  000000014078ACA3: mov         rsi,qword ptr [rsp+70h]
  000000014078ACA8: add         rsp,50h
  000000014078ACAC: pop         rdi
  000000014078ACAD: ret
  000000014078ACAE: int         3
  000000014078ACAF: int         3
  000000014078ACB0: mov         qword ptr [rsp+8],rcx
  000000014078ACB5: push        rdi
  000000014078ACB6: sub         rsp,40h
  000000014078ACBA: mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
  000000014078ACC3: mov         qword ptr [rsp+58h],rbx
  000000014078ACC8: mov         qword ptr [rsp+60h],rsi
  000000014078ACCD: mov         rsi,rcx
  000000014078ACD0: call        00000001407888B0
  000000014078ACD5: nop
  000000014078ACD6: lea         rax,[0000000141C60478h]
  000000014078ACDD: mov         qword ptr [rsi],rax
  000000014078ACE0: lea         rcx,[rsi+00000000000000E8h]
  000000014078ACE7: mov         rdx,rsi
  000000014078ACEA: call        0000000140538CE0
  000000014078ACEF: nop
  000000014078ACF0: movzx       eax,byte ptr [rsi+0000000000000D80h]
  000000014078ACF7: and         al,0F3h
  000000014078ACF9: or          al,3
  000000014078ACFB: mov         byte ptr [rsi+0000000000000D80h],al
  000000014078AD01: mov         word ptr [rsi+0000000000000D81h],0
  000000014078AD0A: xor         ebx,ebx
  000000014078AD0C: mov         qword ptr [rsi+0000000000000D84h],rbx
  000000014078AD13: lea         rcx,[rsi+0000000000000D90h]
  000000014078AD1A: call        0000000140028F80
  000000014078AD1F: nop
  000000014078AD20: lea         rcx,[rsi+0000000000001038h]
  000000014078AD27: call        0000000140028F80
  000000014078AD2C: nop
  000000014078AD2D: lea         rcx,[rsi+00000000000012E0h]
  000000014078AD34: call        0000000140028FC0
  000000014078AD39: lea         rcx,[rsi+0000000000001310h]
  000000014078AD40: call        0000000140028FC0
  000000014078AD45: lea         rcx,[rsi+0000000000001340h]
  000000014078AD4C: call        0000000140028FC0
  000000014078AD51: lea         rcx,[rsi+0000000000001370h]
  000000014078AD58: call        0000000140028FC0
  000000014078AD5D: lea         rcx,[rsi+00000000000013A0h]
  000000014078AD64: xor         edx,edx
  000000014078AD66: call        00000001403DA8F0
  000000014078AD6B: lea         rcx,[rsi+00000000000013B0h]
  000000014078AD72: call        0000000140500C30
  000000014078AD77: nop
  000000014078AD78: lea         rdi,[rsi+00000000000013C0h]
  000000014078AD7F: lea         rax,[000000014019B640h]
  000000014078AD86: mov         qword ptr [rsp+20h],rax
  000000014078AD8B: lea         r9,[000000014078AE10h]
  000000014078AD92: lea         edx,[rbx+18h]
  000000014078AD95: lea         r8d,[rbx+2Fh]
  000000014078AD99: mov         rcx,rdi
  000000014078AD9C: call        00000001411AD700
  000000014078ADA1: lea         rax,[rsi+48h]
  000000014078ADA5: lea         ecx,[rbx+2]
  000000014078ADA8: nop         dword ptr [rax+rax+0000000000000000h]
  000000014078ADB0: mov         qword ptr [rax],rbx
  000000014078ADB3: mov         qword ptr [rax+8],rbx
  000000014078ADB7: mov         qword ptr [rax+10h],rbx
  000000014078ADBB: lea         rax,[rax+40h]
  000000014078ADBF: mov         qword ptr [rax-28h],rbx
  000000014078ADC3: mov         qword ptr [rax-20h],rbx
  000000014078ADC7: mov         qword ptr [rax-18h],rbx
  000000014078ADCB: mov         qword ptr [rax-10h],rbx
  000000014078ADCF: mov         qword ptr [rax-8],rbx
  000000014078ADD3: dec         rcx
  000000014078ADD6: jne         000000014078ADB0
  000000014078ADD8: mov         qword ptr [rax],rbx
  000000014078ADDB: mov         qword ptr [rax+8],rbx
  000000014078ADDF: mov         qword ptr [rax+10h],rbx
  000000014078ADE3: mov         qword ptr [rax+18h],rbx
  000000014078ADE7: nop         word ptr [rax+rax+0000000000000000h]
  000000014078ADF0: mov         dword ptr [rdi],ebx
  000000014078ADF2: inc         ebx
  000000014078ADF4: lea         rdi,[rdi+18h]
  000000014078ADF8: cmp         ebx,2Fh
  000000014078ADFB: jl          000000014078ADF0
  000000014078ADFD: mov         rax,rsi
  000000014078AE00: mov         rbx,qword ptr [rsp+58h]
  000000014078AE05: mov         rsi,qword ptr [rsp+60h]
  000000014078AE0A: add         rsp,40h
  000000014078AE0E: pop         rdi
  000000014078AE0F: ret
  000000014078AE10: push        rbx
  000000014078AE12: sub         rsp,20h
  000000014078AE16: mov         rbx,rcx
  000000014078AE19: add         rcx,8
  000000014078AE1D: call        0000000140500C30
  000000014078AE22: mov         rax,rbx
  000000014078AE25: add         rsp,20h
  000000014078AE29: pop         rbx
  000000014078AE2A: ret
  000000014078AE2B: int         3
  000000014078AE2C: int         3
  000000014078AE2D: int         3
  000000014078AE2E: int         3
  000000014078AE2F: int         3
  000000014078AE30: 48

  Summary

     1A7D000 .text

````
