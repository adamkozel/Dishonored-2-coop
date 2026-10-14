# disasm-87d96c.txt

Original: [work/disasm-87d96c.txt](<../../../../work/disasm-87d96c.txt>)

SHA-256: `3A90A98EA4D6B569EA108F8F90DF76278886488B8F776414C8D7B8FEE59DA3B1`; 28515 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  000000014087D200: mov         rax,rsp
  000000014087D203: mov         byte ptr [rax+20h],r9b
  000000014087D207: mov         qword ptr [rax+10h],rdx
  000000014087D20B: push        rbp
  000000014087D20C: push        rsi
  000000014087D20D: push        rdi
  000000014087D20E: lea         rbp,[rax+FFFFFFFFFFFFFF18h]
  000000014087D215: sub         rsp,1D0h
  000000014087D21C: mov         qword ptr [rbp+10h],0FFFFFFFFFFFFFFFEh
  000000014087D224: mov         qword ptr [rax+8],rbx
  000000014087D228: movaps      xmmword ptr [rax-28h],xmm6
  000000014087D22C: movaps      xmmword ptr [rax-38h],xmm7
  000000014087D230: movaps      xmmword ptr [rax-48h],xmm8
  000000014087D235: movaps      xmmword ptr [rax-58h],xmm9
  000000014087D23A: movaps      xmmword ptr [rax-68h],xmm10
  000000014087D23F: movaps      xmmword ptr [rax-78h],xmm11
  000000014087D244: movaps      xmmword ptr [rax+FFFFFFFFFFFFFF78h],xmm12
  000000014087D24C: movaps      xmmword ptr [rax+FFFFFFFFFFFFFF68h],xmm13
  000000014087D254: movaps      xmmword ptr [rax+FFFFFFFFFFFFFF58h],xmm14
  000000014087D25C: mov         rdi,r8
  000000014087D25F: mov         rbx,rdx
  000000014087D262: mov         rsi,rcx
  000000014087D265: cmp         dword ptr [rbp+0000000000000110h],8
  000000014087D26C: jl          000000014087D27B
  000000014087D26E: lea         rcx,[0000000141C88510h]
  000000014087D275: call        0000000140166830
  000000014087D27A: int         3
  000000014087D27B: movss       xmm2,dword ptr [rcx+0000000000000308h]
  000000014087D283: subss       xmm2,dword ptr [rdx+8]
  000000014087D288: movss       xmm0,dword ptr [rcx+0000000000000304h]
  000000014087D290: subss       xmm0,dword ptr [rdx+4]
  000000014087D295: movss       xmm1,dword ptr [rcx+0000000000000300h]
  000000014087D29D: subss       xmm1,dword ptr [rdx]
  000000014087D2A1: mulss       xmm1,xmm1
  000000014087D2A5: mulss       xmm0,xmm0
  000000014087D2A9: addss       xmm1,xmm0
  000000014087D2AD: mulss       xmm2,xmm2
  000000014087D2B1: addss       xmm1,xmm2
  000000014087D2B5: comiss      xmm1,dword ptr [0000000141AC9028h]
  000000014087D2BC: jbe         000000014087D31A
  000000014087D2BE: mov         rcx,qword ptr [00000001439A5290h]
  000000014087D2C5: mov         rax,qword ptr [rcx]
  000000014087D2C8: call        qword ptr [rax+48h]
  000000014087D2CB: mov         rdx,qword ptr [rax]
  000000014087D2CE: mov         rcx,rax
  000000014087D2D1: call        qword ptr [rdx+58h]
  000000014087D2D4: mov         rcx,rax
  000000014087D2D7: mov         qword ptr [rbp-38h],rax
  000000014087D2DB: mov         rax,qword ptr [rsi+00000000000000F0h]
  000000014087D2E2: mov         qword ptr [rbp-58h],rax
  000000014087D2E6: test        rax,rax
  000000014087D2E9: je          000000014087D31A
  000000014087D2EB: add         rcx,258h
  000000014087D2F2: mov         rdx,rax
  000000014087D2F5: call        0000000140383390
  000000014087D2FA: test        al,al
  000000014087D2FC: je          000000014087D30B
  000000014087D2FE: mov         rdx,qword ptr [rbp-58h]
  000000014087D302: mov         rcx,qword ptr [rbp-38h]
  000000014087D306: call        00000001402B2590
  000000014087D30B: mov         rcx,qword ptr [rbp-58h]
  000000014087D30F: mov         rax,qword ptr [rcx]
  000000014087D312: mov         edx,1
  000000014087D317: call        qword ptr [rax+28h]
  000000014087D31A: xor         r8d,r8d
  000000014087D31D: lea         rdx,[00000001439E2A00h]
  000000014087D324: mov         rcx,rsi
  000000014087D327: call        0000000140877720
  000000014087D32C: test        rax,rax
  000000014087D32F: setne       al
  000000014087D332: test        al,al
  000000014087D334: je          000000014087D360
  000000014087D336: mov         rcx,rdi
  000000014087D339: call        00000001401103C0
  000000014087D33E: mov         dword ptr [rbp-48h],0
  000000014087D345: movss       dword ptr [rbp-44h],xmm0
  000000014087D34A: mov         dword ptr [rbp-40h],0
  000000014087D351: lea         rdx,[rbp+18h]
  000000014087D355: lea         rcx,[rbp-48h]
  000000014087D359: call        000000014010D490
  000000014087D35E: jmp         000000014087D399
  000000014087D360: mov         eax,dword ptr [rdi]
  000000014087D362: mov         dword ptr [rbp-80h],eax
  000000014087D365: mov         eax,dword ptr [rdi+4]
  000000014087D368: mov         dword ptr [rbp-7Ch],eax
  000000014087D36B: mov         eax,dword ptr [rdi+8]
  000000014087D36E: mov         dword ptr [rbp-78h],eax
  000000014087D371: mov         eax,dword ptr [rdi+0Ch]
  000000014087D374: mov         dword ptr [rbp-74h],eax
  000000014087D377: mov         eax,dword ptr [rdi+10h]
  000000014087D37A: mov         dword ptr [rbp-70h],eax
  000000014087D37D: mov         eax,dword ptr [rdi+14h]
  000000014087D380: mov         dword ptr [rbp-6Ch],eax
  000000014087D383: mov         eax,dword ptr [rdi+18h]
  000000014087D386: mov         dword ptr [rbp-68h],eax
  000000014087D389: mov         eax,dword ptr [rdi+1Ch]
  000000014087D38C: mov         dword ptr [rbp-64h],eax
  000000014087D38F: mov         eax,dword ptr [rdi+20h]
  000000014087D392: mov         dword ptr [rbp-60h],eax
  000000014087D395: lea         rax,[rbp-80h]
  000000014087D399: movss       xmm0,dword ptr [rax]
  000000014087D39D: movss       dword ptr [rsp+58h],xmm0
  000000014087D3A3: movss       xmm1,dword ptr [rax+4]
  000000014087D3A8: movss       dword ptr [rsp+5Ch],xmm1
  000000014087D3AE: movss       xmm0,dword ptr [rax+8]
  000000014087D3B3: movss       dword ptr [rsp+60h],xmm0
  000000014087D3B9: movss       xmm1,dword ptr [rax+0Ch]
  000000014087D3BE: movss       dword ptr [rsp+64h],xmm1
  000000014087D3C4: movss       xmm0,dword ptr [rax+10h]
  000000014087D3C9: movss       dword ptr [rsp+68h],xmm0
  000000014087D3CF: movss       xmm1,dword ptr [rax+14h]
  000000014087D3D4: movss       dword ptr [rsp+6Ch],xmm1
  000000014087D3DA: movss       xmm0,dword ptr [rax+18h]
  000000014087D3DF: movss       dword ptr [rsp+70h],xmm0
  000000014087D3E5: movss       xmm1,dword ptr [rax+1Ch]
  000000014087D3EA: movss       dword ptr [rsp+74h],xmm1
  000000014087D3F0: movss       xmm0,dword ptr [rax+20h]
  000000014087D3F5: movss       dword ptr [rsp+78h],xmm0
  000000014087D3FB: mov         rcx,qword ptr [00000001439A5290h]
  000000014087D402: test        dword ptr [rcx+0000000000341070h],0FFFFFFh
  000000014087D40C: jle         000000014087D41E
  000000014087D40E: mov         rdx,rbx
  000000014087D411: call        00000001408A8640
  000000014087D416: mov         edx,dword ptr [rax+00000000000002E0h]
  000000014087D41C: jmp         000000014087D421
  000000014087D41E: or          edx,0FFFFFFFFh
  000000014087D421: cmp         edx,0FFFFFFFFh
  000000014087D424: je          000000014087D43B
  000000014087D426: cmp         edx,dword ptr [rsi+00000000000002E0h]
  000000014087D42C: je          000000014087D43B
  000000014087D42E: mov         rcx,rsi
  000000014087D431: call        0000000140875FC0
  000000014087D436: jmp         000000014087D4EC
  000000014087D43B: movzx       ecx,byte ptr [rsp+3Ch]
  000000014087D440: bts         ecx,9
  000000014087D444: mov         eax,dword ptr [rsp+38h]
  000000014087D448: and         eax,0FF000000h
  000000014087D44D: and         ecx,0FFFFFFC0h
  000000014087D450: and         eax,0FFFFFFh
  000000014087D455: or          eax,5000000h
  000000014087D45A: mov         dword ptr [rsp+38h],eax
  000000014087D45E: lea         rax,[rsp+40h]
  000000014087D463: mov         qword ptr [rsp+30h],rax
  000000014087D468: btr         ecx,7
  000000014087D46C: or          ecx,40h
  000000014087D46F: mov         dword ptr [rsp+3Ch],ecx
  000000014087D473: xor         r9d,r9d
  000000014087D476: lea         r8,[rsp+30h]
  000000014087D47B: lea         rdx,[00000001439BFC30h]
  000000014087D482: mov         rcx,rsi
  000000014087D485: call        0000000140877B50
  000000014087D48A: mov         rdi,qword ptr [rsp+30h]
  000000014087D48F: mov         rax,qword ptr [rsp+38h]
  000000014087D494: and         eax,0FFFFFFh
  000000014087D499: lea         rbx,[rdi+rax*8]
  000000014087D49D: cmp         rdi,rbx
  000000014087D4A0: je          000000014087D4B8
  000000014087D4A2: mov         rcx,qword ptr [rdi]
  000000014087D4A5: call        0000000140600220
  000000014087D4AA: add         rdi,8
  000000014087D4AE: cmp         rdi,rbx
  000000014087D4B1: jne         000000014087D4A2
  000000014087D4B3: mov         rdi,qword ptr [rsp+30h]
  000000014087D4B8: test        byte ptr [rsp+3Ch],40h
  000000014087D4BD: jne         000000014087D4EC
  000000014087D4BF: test        rdi,rdi
  000000014087D4C2: je          000000014087D4EC
  000000014087D4C4: call        00000001401548E0
  000000014087D4C9: mov         r10,qword ptr [rax]
  000000014087D4CC: mov         qword ptr [rsp+20h],10h
  000000014087D4D5: mov         r9,rdi
  000000014087D4D8: mov         r8d,1ECh
  000000014087D4DE: lea         rdx,[0000000141AB2230h]
  000000014087D4E5: mov         rcx,rax
  000000014087D4E8: call        qword ptr [r10+10h]
  000000014087D4EC: mov         ebx,dword ptr [rsi+00000000000000A8h]
  000000014087D4F2: call        0000000140810240
  000000014087D4F7: mov         rcx,rax
  000000014087D4FA: mov         r8d,ebx
  000000014087D4FD: mov         edx,0C3h
  000000014087D502: call        00000001408103F0
  000000014087D507: movss       xmm6,dword ptr [rsi+000000000000030Ch]
  000000014087D50F: movss       xmm7,dword ptr [rsi+0000000000000310h]
  000000014087D517: movss       xmm8,dword ptr [rsi+0000000000000314h]
  000000014087D520: movss       xmm9,dword ptr [rsi+0000000000000318h]
  000000014087D529: movss       xmm10,dword ptr [rsi+000000000000031Ch]
  000000014087D532: movss       xmm11,dword ptr [rsi+0000000000000320h]
  000000014087D53B: movss       xmm12,dword ptr [rsi+0000000000000324h]
  000000014087D544: movss       xmm13,dword ptr [rsi+0000000000000328h]
  000000014087D54D: movss       xmm14,dword ptr [rsi+000000000000032Ch]
  000000014087D556: movzx       ecx,byte ptr [rbp-24h]
  000000014087D55A: or          ecx,600h
  000000014087D560: mov         eax,dword ptr [rbp-28h]
  000000014087D563: and         eax,0FF000000h
  000000014087D568: or          ecx,3Fh
  000000014087D56B: and         eax,0FFFFFFh
  000000014087D570: or          eax,5000000h
  000000014087D575: mov         dword ptr [rbp-28h],eax
  000000014087D578: lea         rax,[rbp-20h]
  000000014087D57C: mov         qword ptr [rbp-30h],rax
  000000014087D580: or          ecx,0C0h
  000000014087D586: mov         dword ptr [rbp-24h],ecx
  000000014087D589: xor         r8d,r8d
  000000014087D58C: lea         rdx,[00000001439C00B0h]
  000000014087D593: mov         rcx,rsi
  000000014087D596: call        0000000140877720
  000000014087D59B: xor         edi,edi
  000000014087D59D: mov         qword ptr [rbp-38h],rdi
  000000014087D5A1: test        rax,rax
  000000014087D5A4: je          000000014087D613
  000000014087D5A6: mov         ecx,dword ptr [rax+1Ch]
  000000014087D5A9: and         cl,0Ch
  000000014087D5AC: cmp         cl,8
  000000014087D5AF: jb          000000014087D613
  000000014087D5B1: mov         ecx,dword ptr [rax+28h]
  000000014087D5B4: and         ecx,0FFFFFFh
  000000014087D5BA: jle         000000014087D613
  000000014087D5BC: cmp         dword ptr [rax+3Ch],edi
  000000014087D5BF: je          000000014087D613
  000000014087D5C1: movsxd      rcx,ecx
  000000014087D5C4: mov         qword ptr [rbp-58h],rcx
  000000014087D5C8: test        rcx,rcx
  000000014087D5CB: jle         000000014087D613
  000000014087D5CD: mov         ebx,edi
  000000014087D5CF: mov         rdi,rax
  000000014087D5D2: mov         rax,qword ptr [rdi+20h]
  000000014087D5D6: test        byte ptr [rbx+rax+00000000000000FCh],1
  000000014087D5DE: jne         000000014087D601
  000000014087D5E0: mov         ecx,dword ptr [rbx+rax+0000000000000090h]
  000000014087D5E7: call        00000001406DC560
  000000014087D5EC: mov         qword ptr [rbp-48h],rax
  000000014087D5F0: lea         rdx,[rbp-48h]
  000000014087D5F4: lea         rcx,[rbp-30h]
  000000014087D5F8: call        00000001408918F0
  000000014087D5FD: mov         rcx,qword ptr [rbp-58h]
  000000014087D601: add         rbx,100h
  000000014087D608: dec         rcx
  000000014087D60B: mov         qword ptr [rbp-58h],rcx
  000000014087D60F: jne         000000014087D5D2
  000000014087D611: xor         edi,edi
  000000014087D613: lea         rdx,[rsi+00000000000000B0h]
  000000014087D61A: lea         rcx,[rbp-30h]
  000000014087D61E: call        00000001400AF430
  000000014087D623: movss       xmm0,dword ptr [000000014228A780h]
  000000014087D62B: movss       dword ptr [rsp+30h],xmm0
  000000014087D631: movss       xmm1,dword ptr [000000014228A784h]
  000000014087D639: movss       dword ptr [rsp+34h],xmm1
  000000014087D63F: movss       xmm0,dword ptr [000000014228A788h]
  000000014087D647: movss       dword ptr [rsp+38h],xmm0
  000000014087D64D: movss       xmm1,dword ptr [000000014228A78Ch]
  000000014087D655: movss       dword ptr [rsp+3Ch],xmm1
  000000014087D65B: movss       xmm0,dword ptr [000000014228A790h]
  000000014087D663: movss       dword ptr [rsp+40h],xmm0
  000000014087D669: movss       xmm1,dword ptr [000000014228A794h]
  000000014087D671: movss       dword ptr [rsp+44h],xmm1
  000000014087D677: movss       xmm0,dword ptr [000000014228A798h]
  000000014087D67F: movss       dword ptr [rsp+48h],xmm0
  000000014087D685: movss       xmm1,dword ptr [000000014228A79Ch]
  000000014087D68D: movss       dword ptr [rsp+4Ch],xmm1
  000000014087D693: movss       xmm0,dword ptr [000000014228A7A0h]
  000000014087D69B: movss       dword ptr [rsp+50h],xmm0
  000000014087D6A1: test        dword ptr [rbp-28h],0FFFFFFh
  000000014087D6A8: jle         000000014087D922
  000000014087D6AE: movss       dword ptr [rbp-80h],xmm6
  000000014087D6B3: movss       dword ptr [rbp-7Ch],xmm7
  000000014087D6B8: movss       dword ptr [rbp-78h],xmm8
  000000014087D6BE: movss       dword ptr [rbp-74h],xmm9
  000000014087D6C4: movss       dword ptr [rbp-70h],xmm10
  000000014087D6CA: movss       dword ptr [rbp-6Ch],xmm11
  000000014087D6D0: movss       dword ptr [rbp-68h],xmm12
  000000014087D6D6: movss       dword ptr [rbp-64h],xmm13
  000000014087D6DC: movss       dword ptr [rbp-60h],xmm14
  000000014087D6E2: lea         rcx,[rbp-80h]
  000000014087D6E6: call        000000014010EE70
  000000014087D6EB: movss       xmm8,dword ptr [rbp-80h]
  000000014087D6F1: movss       dword ptr [rsp+30h],xmm8
  000000014087D6F8: movss       xmm7,dword ptr [rbp-7Ch]
  000000014087D6FD: movss       dword ptr [rsp+34h],xmm7
  000000014087D703: movss       xmm6,dword ptr [rbp-78h]
  000000014087D708: movss       dword ptr [rsp+38h],xmm6
  000000014087D70E: movss       xmm3,dword ptr [rbp-74h]
  000000014087D713: movss       dword ptr [rsp+3Ch],xmm3
  000000014087D719: movss       xmm4,dword ptr [rbp-70h]
  000000014087D71E: movss       dword ptr [rsp+40h],xmm4
  000000014087D724: movss       xmm5,dword ptr [rbp-6Ch]
  000000014087D729: movss       dword ptr [rsp+44h],xmm5
  000000014087D72F: movss       xmm9,dword ptr [rbp-68h]
  000000014087D735: movss       dword ptr [rsp+48h],xmm9
  000000014087D73C: movss       xmm10,dword ptr [rbp-64h]
  000000014087D742: movss       dword ptr [rsp+4Ch],xmm10
  000000014087D749: movss       xmm11,dword ptr [rbp-60h]
  000000014087D74F: movss       dword ptr [rsp+50h],xmm11
  000000014087D756: mov         rbx,qword ptr [rbp-30h]
  000000014087D75A: mov         rax,qword ptr [rbp-28h]
  000000014087D75E: and         eax,0FFFFFFh
  000000014087D763: lea         rdi,[rbx+rax*8]
  000000014087D767: cmp         rbx,rdi
  000000014087D76A: je          000000014087D920
  000000014087D770: mov         rax,qword ptr [rbx]
  000000014087D773: mov         qword ptr [rbp-48h],rax
  000000014087D777: test        rax,rax
  000000014087D77A: je          000000014087D913
  000000014087D780: movss       xmm2,dword ptr [rax+0000000000000308h]
  000000014087D788: subss       xmm2,dword ptr [rsi+0000000000000308h]
  000000014087D790: movss       xmm1,dword ptr [rax+0000000000000304h]
  000000014087D798: subss       xmm1,dword ptr [rsi+0000000000000304h]
  000000014087D7A0: movss       xmm0,dword ptr [rax+0000000000000300h]
  000000014087D7A8: subss       xmm0,dword ptr [rsi+0000000000000300h]
  000000014087D7B0: mulss       xmm6,xmm0
  000000014087D7B4: mulss       xmm5,xmm1
  000000014087D7B8: addss       xmm6,xmm5
  000000014087D7BC: mulss       xmm11,xmm2
  000000014087D7C1: addss       xmm6,xmm11
  000000014087D7C6: mulss       xmm7,xmm0
  000000014087D7CA: mulss       xmm4,xmm1
  000000014087D7CE: addss       xmm7,xmm4
  000000014087D7D2: mulss       xmm10,xmm2
  000000014087D7D7: addss       xmm7,xmm10
  000000014087D7DC: mulss       xmm8,xmm0
  000000014087D7E1: mulss       xmm3,xmm1
  000000014087D7E5: addss       xmm8,xmm3
  000000014087D7EA: mulss       xmm9,xmm2
  000000014087D7EF: addss       xmm8,xmm9
  000000014087D7F4: lea         r8,[rax+000000000000030Ch]
  000000014087D7FB: lea         rdx,[rbp-80h]
  000000014087D7FF: lea         rcx,[rsp+30h]
  000000014087D804: call        000000014000DCE0
  000000014087D809: movss       xmm4,dword ptr [rsp+60h]
  000000014087D80F: mulss       xmm4,xmm8
  000000014087D814: movss       xmm0,dword ptr [rsp+6Ch]
  000000014087D81A: mulss       xmm0,xmm7
  000000014087D81E: addss       xmm4,xmm0
  000000014087D822: movss       xmm1,dword ptr [rsp+78h]
  000000014087D828: mulss       xmm1,xmm6
  000000014087D82C: addss       xmm4,xmm1
  000000014087D830: movss       xmm3,dword ptr [rsp+5Ch]
  000000014087D836: mulss       xmm3,xmm8
  000000014087D83B: movss       xmm0,dword ptr [rsp+68h]
  000000014087D841: mulss       xmm0,xmm7
  000000014087D845: addss       xmm3,xmm0
  000000014087D849: movss       xmm1,dword ptr [rsp+74h]
  000000014087D84F: mulss       xmm1,xmm6
  000000014087D853: addss       xmm3,xmm1
  000000014087D857: movss       xmm2,dword ptr [rsp+58h]
  000000014087D85D: mulss       xmm2,xmm8
  000000014087D862: movss       xmm0,dword ptr [rsp+64h]
  000000014087D868: mulss       xmm0,xmm7
  000000014087D86C: addss       xmm2,xmm0
  000000014087D870: movss       xmm1,dword ptr [rsp+70h]
  000000014087D876: mulss       xmm1,xmm6
  000000014087D87A: addss       xmm2,xmm1
  000000014087D87E: mov         rax,qword ptr [rbp+00000000000000F8h]
  000000014087D885: addss       xmm4,dword ptr [rax+8]
  000000014087D88A: addss       xmm3,dword ptr [rax+4]
  000000014087D88F: addss       xmm2,dword ptr [rax]
  000000014087D893: movss       dword ptr [rbp-58h],xmm2
  000000014087D898: movss       dword ptr [rbp-54h],xmm3
  000000014087D89D: movss       dword ptr [rbp-50h],xmm4
  000000014087D8A2: lea         r8,[rbp-80h]
  000000014087D8A6: lea         rdx,[rbp+18h]
  000000014087D8AA: lea         rcx,[rsp+58h]
  000000014087D8AF: call        000000014000DCE0
  000000014087D8B4: mov         eax,dword ptr [rbp+0000000000000110h]
  000000014087D8BA: inc         eax
  000000014087D8BC: mov         dword ptr [rsp+20h],eax
  000000014087D8C0: movzx       r9d,byte ptr [rbp+0000000000000108h]
  000000014087D8C8: lea         r8,[rbp+18h]
  000000014087D8CC: lea         rdx,[rbp-58h]
  000000014087D8D0: mov         rcx,qword ptr [rbp-48h]
  000000014087D8D4: call        000000014087D200
  000000014087D8D9: movss       xmm11,dword ptr [rsp+50h]
  000000014087D8E0: movss       xmm10,dword ptr [rsp+4Ch]
  000000014087D8E7: movss       xmm9,dword ptr [rsp+48h]
  000000014087D8EE: movss       xmm5,dword ptr [rsp+44h]
  000000014087D8F4: movss       xmm4,dword ptr [rsp+40h]
  000000014087D8FA: movss       xmm3,dword ptr [rsp+3Ch]
  000000014087D900: movss       xmm6,dword ptr [rsp+38h]
  000000014087D906: movss       xmm7,dword ptr [rsp+34h]
  000000014087D90C: movss       xmm8,dword ptr [rsp+30h]
  000000014087D913: add         rbx,8
  000000014087D917: cmp         rbx,rdi
  000000014087D91A: jne         000000014087D770
  000000014087D920: xor         edi,edi
  000000014087D922: test        dword ptr [rsi+0000000000000270h],0FFFFFFh
  000000014087D92C: jle         000000014087D98F
  000000014087D92E: mov         rbx,rdi
  000000014087D931: mov         rdi,qword ptr [rbp+00000000000000F8h]
  000000014087D938: nop         dword ptr [rax+rax+0000000000000000h]
  000000014087D940: mov         rax,qword ptr [rsi+0000000000000268h]
  000000014087D947: mov         rcx,qword ptr [rbx+rax]
  000000014087D94B: test        rcx,rcx
  000000014087D94E: je          000000014087D972
  000000014087D950: mov         eax,dword ptr [rcx+1Ch]
  000000014087D953: and         al,0Ch
  000000014087D955: cmp         al,8
  000000014087D957: jb          000000014087D972
  000000014087D959: mov         rax,qword ptr [rcx]
  000000014087D95C: movzx       r9d,byte ptr [rbp+0000000000000108h]
  000000014087D964: lea         r8,[rsp+58h]
  000000014087D969: mov         rdx,rdi
  000000014087D96C: call        qword ptr [rax+00000000000000B0h]
  000000014087D972: mov         rcx,qword ptr [rbp-38h]
  000000014087D976: inc         ecx
  000000014087D978: mov         qword ptr [rbp-38h],rcx
  000000014087D97C: add         rbx,8
  000000014087D980: mov         eax,dword ptr [rsi+0000000000000270h]
  000000014087D986: and         eax,0FFFFFFh
  000000014087D98B: cmp         ecx,eax
  000000014087D98D: jl          000000014087D940
  000000014087D98F: lea         rcx,[rsp+58h]
  000000014087D994: call        000000014000F2C0
  000000014087D999: movss       xmm0,dword ptr [rsp+58h]
  000000014087D99F: movss       dword ptr [rsi+000000000000030Ch],xmm0
  000000014087D9A7: movss       xmm1,dword ptr [rsp+5Ch]
  000000014087D9AD: movss       dword ptr [rsi+0000000000000310h],xmm1
  000000014087D9B5: movss       xmm0,dword ptr [rsp+60h]
  000000014087D9BB: movss       dword ptr [rsi+0000000000000314h],xmm0
  000000014087D9C3: movss       xmm1,dword ptr [rsp+64h]
  000000014087D9C9: movss       dword ptr [rsi+0000000000000318h],xmm1
  000000014087D9D1: movss       xmm0,dword ptr [rsp+68h]
  000000014087D9D7: movss       dword ptr [rsi+000000000000031Ch],xmm0
  000000014087D9DF: movss       xmm1,dword ptr [rsp+6Ch]
  000000014087D9E5: movss       dword ptr [rsi+0000000000000320h],xmm1
  000000014087D9ED: movss       xmm0,dword ptr [rsp+70h]
  000000014087D9F3: movss       dword ptr [rsi+0000000000000324h],xmm0
  000000014087D9FB: movss       xmm1,dword ptr [rsp+74h]
  000000014087DA01: movss       dword ptr [rsi+0000000000000328h],xmm1
  000000014087DA09: movss       xmm0,dword ptr [rsp+78h]
  000000014087DA0F: movss       dword ptr [rsi+000000000000032Ch],xmm0
  000000014087DA17: mov         rdx,qword ptr [rbp+00000000000000F8h]
  000000014087DA1E: mov         eax,dword ptr [rdx]
  000000014087DA20: mov         dword ptr [rsi+0000000000000300h],eax
  000000014087DA26: mov         eax,dword ptr [rdx+4]
  000000014087DA29: mov         dword ptr [rsi+0000000000000304h],eax
  000000014087DA2F: mov         eax,dword ptr [rdx+8]
  000000014087DA32: mov         dword ptr [rsi+0000000000000308h],eax
  000000014087DA38: cmp         byte ptr [rbp+0000000000000108h],0
  000000014087DA3F: je          000000014087DAA1
  000000014087DA41: movss       xmm0,dword ptr [00000001431AF668h]
  000000014087DA49: movss       dword ptr [rsi+000000000000033Ch],xmm0
  000000014087DA51: movss       xmm1,dword ptr [00000001431AF66Ch]
  000000014087DA59: movss       dword ptr [rsi+0000000000000340h],xmm1
  000000014087DA61: movss       xmm0,dword ptr [00000001431AF670h]
  000000014087DA69: movss       dword ptr [rsi+0000000000000344h],xmm0
  000000014087DA71: movss       xmm1,dword ptr [00000001431AF668h]
  000000014087DA79: movss       dword ptr [rsi+0000000000000348h],xmm1
  000000014087DA81: movss       xmm0,dword ptr [00000001431AF66Ch]
  000000014087DA89: movss       dword ptr [rsi+000000000000034Ch],xmm0
  000000014087DA91: movss       xmm1,dword ptr [00000001431AF670h]
  000000014087DA99: movss       dword ptr [rsi+0000000000000350h],xmm1
  000000014087DAA1: mov         rcx,qword ptr [rsi+00000000000000F0h]
  000000014087DAA8: test        rcx,rcx
  000000014087DAAB: je          000000014087DB74
  000000014087DAB1: movzx       eax,byte ptr [rsi+00000000000000D0h]
  000000014087DAB8: test        al,1
  000000014087DABA: jne         000000014087DB6D
  000000014087DAC0: test        al,2
  000000014087DAC2: jne         000000014087DB6D
  000000014087DAC8: mov         eax,dword ptr [rdx]
  000000014087DACA: mov         dword ptr [rcx+0000000000001860h],eax
  000000014087DAD0: mov         eax,dword ptr [rdx+4]
  000000014087DAD3: mov         dword ptr [rcx+0000000000001864h],eax
  000000014087DAD9: mov         eax,dword ptr [rdx+8]
  000000014087DADC: mov         dword ptr [rcx+0000000000001868h],eax
  000000014087DAE2: movss       xmm0,dword ptr [rsp+58h]
  000000014087DAE8: movss       dword ptr [rcx+000000000000186Ch],xmm0
  000000014087DAF0: movss       xmm1,dword ptr [rsp+5Ch]
  000000014087DAF6: movss       dword ptr [rcx+0000000000001870h],xmm1
  000000014087DAFE: movss       xmm0,dword ptr [rsp+60h]
  000000014087DB04: movss       dword ptr [rcx+0000000000001874h],xmm0
  000000014087DB0C: movss       xmm1,dword ptr [rsp+64h]
  000000014087DB12: movss       dword ptr [rcx+0000000000001878h],xmm1
  000000014087DB1A: movss       xmm0,dword ptr [rsp+68h]
  000000014087DB20: movss       dword ptr [rcx+000000000000187Ch],xmm0
  000000014087DB28: movss       xmm1,dword ptr [rsp+6Ch]
  000000014087DB2E: movss       dword ptr [rcx+0000000000001880h],xmm1
  000000014087DB36: movss       xmm0,dword ptr [rsp+70h]
  000000014087DB3C: movss       dword ptr [rcx+0000000000001884h],xmm0
  000000014087DB44: movss       xmm1,dword ptr [rsp+74h]
  000000014087DB4A: movss       dword ptr [rcx+0000000000001888h],xmm1
  000000014087DB52: movss       xmm0,dword ptr [rsp+78h]
  000000014087DB58: movss       dword ptr [rcx+000000000000188Ch],xmm0
  000000014087DB60: mov         rax,qword ptr [rcx]
  000000014087DB63: mov         edx,1
  000000014087DB68: call        qword ptr [rax+28h]
  000000014087DB6B: jmp         000000014087DB74
  000000014087DB6D: mov         rax,qword ptr [rcx]
  000000014087DB70: call        qword ptr [rax+38h]
  000000014087DB73: nop
  000000014087DB74: test        byte ptr [rbp-24h],40h
  000000014087DB78: jne         000000014087DBAB
  000000014087DB7A: mov         rbx,qword ptr [rbp-30h]
  000000014087DB7E: test        rbx,rbx
  000000014087DB81: je          000000014087DBAB
  000000014087DB83: call        00000001401548E0
  000000014087DB88: mov         r10,qword ptr [rax]
  000000014087DB8B: mov         qword ptr [rsp+20h],10h
  000000014087DB94: mov         r9,rbx
  000000014087DB97: mov         r8d,1ECh
  000000014087DB9D: lea         rdx,[0000000141AB2230h]
  000000014087DBA4: mov         rcx,rax
  000000014087DBA7: call        qword ptr [r10+10h]
  000000014087DBAB: lea         r11,[rsp+00000000000001D0h]
  000000014087DBB3: mov         rbx,qword ptr [r11+20h]
  000000014087DBB7: movaps      xmm6,xmmword ptr [r11-10h]
  000000014087DBBC: movaps      xmm7,xmmword ptr [r11-20h]
  000000014087DBC1: movaps      xmm8,xmmword ptr [r11-30h]
  000000014087DBC6: movaps      xmm9,xmmword ptr [r11-40h]
  000000014087DBCB: movaps      xmm10,xmmword ptr [r11-50h]
  000000014087DBD0: movaps      xmm11,xmmword ptr [r11-60h]
  000000014087DBD5: movaps      xmm12,xmmword ptr [r11-70h]
  000000014087DBDA: movaps      xmm13,xmmword ptr [r11-80h]
  000000014087DBDF: movaps      xmm14,xmmword ptr [r11+FFFFFFFFFFFFFF70h]
  000000014087DBE7: mov         rsp,r11
  000000014087DBEA: pop         rdi
  000000014087DBEB: pop         rsi
  000000014087DBEC: pop         rbp
  000000014087DBED: ret
  000000014087DBEE: int         3

  Summary

     1A7D000 .text

````
