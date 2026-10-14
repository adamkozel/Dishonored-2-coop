# disasm-8a8f70.txt

Original: [work/disasm-8a8f70.txt](<../../../../work/disasm-8a8f70.txt>)

SHA-256: `6E25148168C57B047E9AA112C6265F6D3A516554C70A55AA3AA7A692B994F1C2`; 6970 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001408A8F70: lea         rax,[rcx+00000000001F83D8h]
  00000001408A8F77: ret
  00000001408A8F78: int         3
  00000001408A8F79: int         3
  00000001408A8F7A: int         3
  00000001408A8F7B: int         3
  00000001408A8F7C: int         3
  00000001408A8F7D: int         3
  00000001408A8F7E: int         3
  00000001408A8F7F: int         3
  00000001408A8F80: lea         rax,[rcx+000000000030F150h]
  00000001408A8F87: ret
  00000001408A8F88: int         3
  00000001408A8F89: int         3
  00000001408A8F8A: int         3
  00000001408A8F8B: int         3
  00000001408A8F8C: int         3
  00000001408A8F8D: int         3
  00000001408A8F8E: int         3
  00000001408A8F8F: int         3
  00000001408A8F90: lea         rax,[rcx+000000000030F260h]
  00000001408A8F97: ret
  00000001408A8F98: int         3
  00000001408A8F99: int         3
  00000001408A8F9A: int         3
  00000001408A8F9B: int         3
  00000001408A8F9C: int         3
  00000001408A8F9D: int         3
  00000001408A8F9E: int         3
  00000001408A8F9F: int         3
  00000001408A8FA0: add         rcx,1F6EC8h
  00000001408A8FA7: jmp         0000000140787960
  00000001408A8FAC: int         3
  00000001408A8FAD: int         3
  00000001408A8FAE: int         3
  00000001408A8FAF: int         3
  00000001408A8FB0: mov         rax,rsp
  00000001408A8FB3: mov         qword ptr [rax+8],rcx
  00000001408A8FB7: push        rbp
  00000001408A8FB8: push        rbx
  00000001408A8FB9: push        rsi
  00000001408A8FBA: push        rdi
  00000001408A8FBB: push        r12
  00000001408A8FBD: push        r13
  00000001408A8FBF: push        r14
  00000001408A8FC1: push        r15
  00000001408A8FC3: lea         rbp,[rax-5Fh]
  00000001408A8FC7: sub         rsp,0C8h
  00000001408A8FCE: mov         qword ptr [rbp-79h],0FFFFFFFFFFFFFFFEh
  00000001408A8FD6: movaps      xmmword ptr [rax-58h],xmm6
  00000001408A8FDA: movaps      xmmword ptr [rax-68h],xmm7
  00000001408A8FDE: movaps      xmmword ptr [rax-78h],xmm8
  00000001408A8FE3: mov         r15,rcx
  00000001408A8FE6: mov         r13,qword ptr [rcx+0000000000060270h]
  00000001408A8FED: mov         eax,dword ptr [rcx+0000000000060278h]
  00000001408A8FF3: and         eax,0FFFFFFh
  00000001408A8FF8: lea         rax,[rax+rax*4]
  00000001408A8FFC: lea         rdi,[rax*8+0000000000000000h]
  00000001408A9004: add         rdi,r13
  00000001408A9007: cmp         r13,rdi
  00000001408A900A: je          00000001408A9409
  00000001408A9010: add         r13,18h
  00000001408A9014: nop         word ptr [rax+rax+0000000000000000h]
  00000001408A9020: mov         r14,qword ptr [r13-18h]
  00000001408A9024: mov         rbx,qword ptr [r13-10h]
  00000001408A9028: test        rbx,rbx
  00000001408A902B: jne         00000001408A903D
  00000001408A902D: mov         rbx,qword ptr [r14+00000000000002E8h]
  00000001408A9034: test        rbx,rbx
  00000001408A9037: je          00000001408A93D8
  00000001408A903D: mov         rdx,rbx
  00000001408A9040: mov         rcx,r15
  00000001408A9043: call        00000001408A79B0
  00000001408A9048: mov         r12,rax
  00000001408A904B: test        rax,rax
  00000001408A904E: je          00000001408A93F1
  00000001408A9054: mov         r8,rbx
  00000001408A9057: mov         rdx,rax
  00000001408A905A: mov         rcx,r15
  00000001408A905D: call        00000001408B1E60
  00000001408A9062: mov         rsi,qword ptr [r15+00000000001E8960h]
  00000001408A9069: mov         qword ptr [rbp-59h],0
  00000001408A9071: call        00000001401548E0
  00000001408A9076: movzx       ecx,byte ptr [rbp-4Dh]
  00000001408A907A: mov         edx,dword ptr [rbp-51h]
  00000001408A907D: and         edx,0FF000000h
  00000001408A9083: or          ecx,3Fh
  00000001408A9086: and         edx,0FFFFFFh
  00000001408A908C: or          edx,5000000h
  00000001408A9092: mov         dword ptr [rbp-51h],edx
  00000001408A9095: and         ecx,0FFFFFF3Fh
  00000001408A909B: mov         dword ptr [rbp-4Dh],ecx
  00000001408A909E: test        cl,40h
  00000001408A90A1: jne         00000001408A90E6
  00000001408A90A3: mov         rbx,qword ptr [rbp-59h]
  00000001408A90A7: test        rbx,rbx
  00000001408A90AA: je          00000001408A90D7
  00000001408A90AC: call        00000001401548E0
  00000001408A90B1: mov         r10,qword ptr [rax]
  00000001408A90B4: mov         qword ptr [rsp+20h],10h
  00000001408A90BD: mov         r9,rbx
  00000001408A90C0: mov         r8d,1ECh
  00000001408A90C6: lea         rdx,[0000000141AB2230h]
  00000001408A90CD: mov         rcx,rax
  00000001408A90D0: call        qword ptr [r10+10h]
  00000001408A90D4: mov         edx,dword ptr [rbp-51h]
  00000001408A90D7: mov         qword ptr [rbp-59h],0
  00000001408A90DF: movzx       eax,byte ptr [rbp-4Dh]
  00000001408A90E3: mov         dword ptr [rbp-4Dh],eax
  00000001408A90E6: and         edx,0FF000000h
  00000001408A90EC: mov         dword ptr [rbp-51h],edx
  00000001408A90EF: or          byte ptr [r14+00000000000000D2h],1
  00000001408A90F7: mov         rdx,r14
  00000001408A90FA: mov         rcx,rsi
  00000001408A90FD: call        00000001407EB060
  00000001408A9102: lea         r8,[rbp-59h]
  00000001408A9106: mov         rdx,r14
  00000001408A9109: mov         rcx,rsi
  00000001408A910C: call        00000001407EADA0
  00000001408A9111: lea         rdx,[rbp-59h]
  00000001408A9115: mov         rcx,r12
  00000001408A9118: call        0000000140878630
  00000001408A911D: xor         esi,esi
  00000001408A911F: test        dword ptr [rbp-51h],0FFFFFFh
  00000001408A9126: jle         00000001408A9150
  00000001408A9128: xor         ebx,ebx
  00000001408A912A: nop         word ptr [rax+rax]
  00000001408A9130: mov         rax,qword ptr [rbp-59h]
  00000001408A9134: mov         rcx,qword ptr [rbx+rax]
  00000001408A9138: mov         rax,qword ptr [rcx]
  00000001408A913B: call        qword ptr [rax+78h]
  00000001408A913E: inc         esi
  00000001408A9140: lea         rbx,[rbx+10h]
  00000001408A9144: mov         eax,dword ptr [rbp-51h]
  00000001408A9147: and         eax,0FFFFFFh
  00000001408A914C: cmp         esi,eax
  00000001408A914E: jl          00000001408A9130
  00000001408A9150: mov         eax,dword ptr [r14+00000000000000A8h]
  00000001408A9157: mov         dword ptr [rbp+6Fh],eax
  00000001408A915A: movss       xmm6,dword ptr [r14+0000000000000300h]
  00000001408A9163: movss       xmm7,dword ptr [r14+0000000000000304h]
  00000001408A916C: F3
  00000001408A916D: 45
  00000001408A916E: 0F
  00000001408A916F: 10
  00000001408A9170: 86

  Summary

     1A7D000 .text

````
