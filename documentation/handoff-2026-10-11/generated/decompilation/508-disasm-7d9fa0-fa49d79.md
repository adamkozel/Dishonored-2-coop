# disasm-7d9fa0.txt

Original: [work/disasm-7d9fa0.txt](<../../../../work/disasm-7d9fa0.txt>)

SHA-256: `8A172D0CD9DD2BA0CC424BED93AE809ED8C47A166AE0EC3D40CF513238C92095`; 8970 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001407D9FA0: mov         qword ptr [rsp+18h],rbx
  00000001407D9FA5: push        rbp
  00000001407D9FA6: push        rsi
  00000001407D9FA7: push        rdi
  00000001407D9FA8: push        r14
  00000001407D9FAA: push        r15
  00000001407D9FAC: sub         rsp,20h
  00000001407D9FB0: mov         ebx,dword ptr [rcx+0000000000000158h]
  00000001407D9FB6: mov         r14,qword ptr [00000001439A5290h]
  00000001407D9FBD: mov         rbp,rcx
  00000001407D9FC0: lea         rcx,[r14+00000000001F5338h]
  00000001407D9FC7: mov         edx,ebx
  00000001407D9FC9: call        00000001407D6E50
  00000001407D9FCE: xor         esi,esi
  00000001407D9FD0: cmp         dword ptr [rbp+000000000000027Ch],2
  00000001407D9FD7: jne         00000001407DA002
  00000001407D9FD9: mov         rcx,rbp
  00000001407D9FDC: call        00000001407CBBE0
  00000001407D9FE1: test        al,al
  00000001407D9FE3: je          00000001407DA002
  00000001407D9FE5: lea         r8,[rsp+50h]
  00000001407D9FEA: lea         rdx,[rsp+58h]
  00000001407D9FEF: xor         r9d,r9d
  00000001407D9FF2: mov         rcx,rbp
  00000001407D9FF5: mov         dword ptr [rsp+50h],esi
  00000001407D9FF9: mov         dword ptr [rsp+58h],esi
  00000001407D9FFD: call        00000001407D2160
  00000001407DA002: mov         eax,dword ptr [rbp+000000000000027Ch]
  00000001407DA008: add         eax,0FFFFFFFDh
  00000001407DA00B: test        eax,0FFFFFFFBh
  00000001407DA010: jne         00000001407DA01A
  00000001407DA012: mov         rcx,rbp
  00000001407DA015: call        00000001407D2DA0
  00000001407DA01A: mov         edi,dword ptr [rbp+000000000000027Ch]
  00000001407DA020: movzx       r15d,byte ptr [rbp+00000000000002B1h]
  00000001407DA028: mov         rcx,rbp
  00000001407DA02B: call        00000001407CF580
  00000001407DA030: movzx       eax,byte ptr [rbp+00000000000002B1h]
  00000001407DA037: cmp         edi,5
  00000001407DA03A: jne         00000001407DA052
  00000001407DA03C: cmp         r15b,1
  00000001407DA040: jne         00000001407DA060
  00000001407DA042: test        al,al
  00000001407DA044: jne         00000001407DA060
  00000001407DA046: xor         edx,edx
  00000001407DA048: mov         rcx,rbp
  00000001407DA04B: call        00000001407D06A0
  00000001407DA050: jmp         00000001407DA060
  00000001407DA052: cmp         al,1
  00000001407DA054: jne         00000001407DA060
  00000001407DA056: xor         edx,edx
  00000001407DA058: mov         rcx,rbp
  00000001407DA05B: call        00000001407CFA40
  00000001407DA060: mov         eax,dword ptr [r14+00000000001F5B60h]
  00000001407DA067: and         eax,0FFFFFFh
  00000001407DA06C: movsxd      rdi,eax
  00000001407DA06F: jle         00000001407DA094
  00000001407DA071: mov         rax,qword ptr [r14+00000000001F5B58h]
  00000001407DA078: cmp         dword ptr [rax+rsi*8],ebx
  00000001407DA07B: jne         00000001407DA08C
  00000001407DA07D: lea         rcx,[rax+4]
  00000001407DA081: xor         edx,edx
  00000001407DA083: lea         rcx,[rcx+rsi*8]
  00000001407DA087: call        00000001401581E0
  00000001407DA08C: inc         rsi
  00000001407DA08F: cmp         rsi,rdi
  00000001407DA092: jl          00000001407DA071
  00000001407DA094: mov         rbx,qword ptr [rsp+60h]
  00000001407DA099: add         rsp,20h
  00000001407DA09D: pop         r15
  00000001407DA09F: pop         r14
  00000001407DA0A1: pop         rdi
  00000001407DA0A2: pop         rsi
  00000001407DA0A3: pop         rbp
  00000001407DA0A4: ret
  00000001407DA0A5: int         3
  00000001407DA0A6: int         3
  00000001407DA0A7: int         3
  00000001407DA0A8: int         3
  00000001407DA0A9: int         3
  00000001407DA0AA: int         3
  00000001407DA0AB: int         3
  00000001407DA0AC: int         3
  00000001407DA0AD: int         3
  00000001407DA0AE: int         3
  00000001407DA0AF: int         3
  00000001407DA0B0: push        rbx
  00000001407DA0B2: push        rbp
  00000001407DA0B3: sub         rsp,38h
  00000001407DA0B7: mov         rbp,rcx
  00000001407DA0BA: call        0000000140166670
  00000001407DA0BF: test        al,al
  00000001407DA0C1: je          00000001407DA21B
  00000001407DA0C7: mov         eax,dword ptr [rbp+18h]
  00000001407DA0CA: test        eax,eax
  00000001407DA0CC: jle         00000001407DA21B
  00000001407DA0D2: cmp         dword ptr [rbp+1Ch],eax
  00000001407DA0D5: jge         00000001407DA21B
  00000001407DA0DB: mov         qword ptr [rsp+58h],rsi
  00000001407DA0E0: mov         qword ptr [rsp+68h],rdi
  00000001407DA0E5: mov         qword ptr [rsp+30h],r12
  00000001407DA0EA: mov         qword ptr [rsp+28h],r14
  00000001407DA0EF: xor         r12d,r12d
  00000001407DA0F2: mov         qword ptr [rsp+20h],r15
  00000001407DA0F7: nop         word ptr [rax+rax+0000000000000000h]
  00000001407DA100: movsxd      rdx,dword ptr [rbp+1Ch]
  00000001407DA104: mov         rax,qword ptr [rbp+8]
  00000001407DA108: mov         r14,qword ptr [00000001439A5290h]
  00000001407DA10F: mov         rcx,qword ptr [rax]
  00000001407DA112: mov         rsi,qword ptr [rcx+rdx*8]
  00000001407DA116: lea         rcx,[r14+00000000001F5338h]
  00000001407DA11D: mov         ebx,dword ptr [rsi+0000000000000158h]
  00000001407DA123: mov         edx,ebx
  00000001407DA125: call        00000001407D6E50
  00000001407DA12A: cmp         dword ptr [rsi+000000000000027Ch],2
  00000001407DA131: jne         00000001407DA15E
  00000001407DA133: mov         rcx,rsi
  00000001407DA136: call        00000001407CBBE0
  00000001407DA13B: test        al,al
  00000001407DA13D: je          00000001407DA15E
  00000001407DA13F: lea         r8,[rsp+50h]
  00000001407DA144: lea         rdx,[rsp+60h]
  00000001407DA149: xor         r9d,r9d
  00000001407DA14C: mov         rcx,rsi
  00000001407DA14F: mov         dword ptr [rsp+50h],r12d
  00000001407DA154: mov         dword ptr [rsp+60h],r12d
  00000001407DA159: call        00000001407D2160
  00000001407DA15E: mov         eax,dword ptr [rsi+000000000000027Ch]
  00000001407DA164: add         eax,0FFFFFFFDh
  00000001407DA167: test        eax,0FFFFFFFBh
  00000001407DA16C: jne         00000001407DA176
  00000001407DA16E: mov         rcx,rsi
  00000001407DA171: call        00000001407D2DA0
  00000001407DA176: mov         edi,dword ptr [rsi+000000000000027Ch]
  00000001407DA17C: movzx       r15d,byte ptr [rsi+00000000000002B1h]
  00000001407DA184: mov         rcx,rsi
  00000001407DA187: call        00000001407CF580
  00000001407DA18C: movzx       eax,byte ptr [rsi+00000000000002B1h]
  00000001407DA193: cmp         edi,5
  00000001407DA196: jne         00000001407DA1AE
  00000001407DA198: cmp         r15b,1
  00000001407DA19C: jne         00000001407DA1BC
  00000001407DA19E: test        al,al
  00000001407DA1A0: jne         00000001407DA1BC
  00000001407DA1A2: xor         edx,edx
  00000001407DA1A4: mov         rcx,rsi
  00000001407DA1A7: call        00000001407D06A0
  00000001407DA1AC: jmp         00000001407DA1BC
  00000001407DA1AE: cmp         al,1
  00000001407DA1B0: jne         00000001407DA1BC
  00000001407DA1B2: xor         edx,edx
  00000001407DA1B4: mov         rcx,rsi
  00000001407DA1B7: call        00000001407CFA40
  00000001407DA1BC: mov         eax,dword ptr [r14+00000000001F5B60h]
  00000001407DA1C3: mov         rdi,r12
  00000001407DA1C6: and         eax,0FFFFFFh
  00000001407DA1CB: movsxd      rsi,eax
  00000001407DA1CE: jle         00000001407DA1F3
  00000001407DA1D0: mov         rax,qword ptr [r14+00000000001F5B58h]
  00000001407DA1D7: cmp         dword ptr [rax+rdi*8],ebx
  00000001407DA1DA: jne         00000001407DA1EB
  00000001407DA1DC: lea         rcx,[rax+4]
  00000001407DA1E0: xor         edx,edx
  00000001407DA1E2: lea         rcx,[rcx+rdi*8]
  00000001407DA1E6: call        00000001401581E0
  00000001407DA1EB: inc         rdi
  00000001407DA1EE: cmp         rdi,rsi
  00000001407DA1F1: jl          00000001407DA1D0
  00000001407DA1F3: inc         dword ptr [rbp+1Ch]
  00000001407DA1F6: mov         eax,dword ptr [rbp+1Ch]
  00000001407DA1F9: cmp         eax,dword ptr [rbp+18h]
  00000001407DA1FC: jl          00000001407DA100
  00000001407DA202: mov         r15,qword ptr [rsp+20h]
  00000001407DA207: mov         r14,qword ptr [rsp+28h]
  00000001407DA20C: mov         r12,qword ptr [rsp+30h]
  00000001407DA211: mov         rdi,qword ptr [rsp+68h]
  00000001407DA216: mov         rsi,qword ptr [rsp+58h]
  00000001407DA21B: mov         rax,qword ptr [rbp+8]
  00000001407DA21F: 48
  00000001407DA220: 8B

  Summary

     1A7D000 .text

````
