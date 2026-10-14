# native-replacement-queue-disasm.txt

Original: [work/native-replacement-queue-disasm.txt](<../../../../work/native-replacement-queue-disasm.txt>)

SHA-256: `E50D3C693C5C2265793740CA8BCC2B77D9D87F090D2ED1962DBA06F686E3A1AA`; 14101 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe


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
  00000001408A916C: movss       xmm8,dword ptr [r14+0000000000000308h]
  00000001408A9175: mov         eax,dword ptr [r14+000000000000030Ch]
  00000001408A917C: mov         dword ptr [rbp-49h],eax
  00000001408A917F: mov         eax,dword ptr [r14+0000000000000310h]
  00000001408A9186: mov         dword ptr [rbp-45h],eax
  00000001408A9189: mov         eax,dword ptr [r14+0000000000000314h]
  00000001408A9190: mov         dword ptr [rbp-41h],eax
  00000001408A9193: mov         eax,dword ptr [r14+0000000000000318h]
  00000001408A919A: mov         dword ptr [rbp-3Dh],eax
  00000001408A919D: mov         eax,dword ptr [r14+000000000000031Ch]
  00000001408A91A4: mov         dword ptr [rbp-39h],eax
  00000001408A91A7: mov         eax,dword ptr [r14+0000000000000320h]
  00000001408A91AE: mov         dword ptr [rbp-35h],eax
  00000001408A91B1: mov         eax,dword ptr [r14+0000000000000324h]
  00000001408A91B8: mov         dword ptr [rbp-31h],eax
  00000001408A91BB: mov         eax,dword ptr [r14+0000000000000328h]
  00000001408A91C2: mov         dword ptr [rbp-2Dh],eax
  00000001408A91C5: mov         eax,dword ptr [r14+000000000000032Ch]
  00000001408A91CC: mov         dword ptr [rbp-29h],eax
  00000001408A91CF: lea         rcx,[r14+20h]
  00000001408A91D3: call        00000001401D9090
  00000001408A91D8: mov         rdx,rax
  00000001408A91DB: mov         rcx,r12
  00000001408A91DE: call        000000014087B780
  00000001408A91E3: mov         rdx,r14
  00000001408A91E6: mov         rcx,r15
  00000001408A91E9: call        00000001408AC460
  00000001408A91EE: lea         rcx,[r15+0000000000060140h]
  00000001408A91F5: mov         rdx,r14
  00000001408A91F8: call        0000000140874940
  00000001408A91FD: mov         rdx,r14
  00000001408A9200: lea         rcx,[r15+00000000003130B0h]
  00000001408A9207: call        00000001407B5F10
  00000001408A920C: lea         rbx,[r15+0000000000341590h]
  00000001408A9213: mov         qword ptr [rbp+7Fh],rbx
  00000001408A9217: mov         dl,1
  00000001408A9219: mov         rcx,rbx
  00000001408A921C: call        0000000140158290
  00000001408A9221: nop
  00000001408A9222: mov         r8b,1
  00000001408A9225: mov         rdx,r15
  00000001408A9228: lea         rcx,[rbp+77h]
  00000001408A922C: call        00000001408A2DC0
  00000001408A9231: nop
  00000001408A9232: lea         r8,[rbp+6Fh]
  00000001408A9236: mov         rdx,r12
  00000001408A9239: lea         rcx,[r15+00000000003130B0h]
  00000001408A9240: call        00000001407B5D90
  00000001408A9245: lea         rcx,[r12+20h]
  00000001408A924A: call        00000001401D9090
  00000001408A924F: mov         rdx,rax
  00000001408A9252: mov         rcx,r12
  00000001408A9255: call        000000014087B780
  00000001408A925A: mov         qword ptr [rbp-71h],rbx
  00000001408A925E: mov         dl,1
  00000001408A9260: mov         rcx,rbx
  00000001408A9263: call        0000000140158290
  00000001408A9268: nop
  00000001408A9269: mov         rax,qword ptr [r15]
  00000001408A926C: mov         rcx,r15
  00000001408A926F: call        qword ptr [rax+0000000000000090h]
  00000001408A9275: cmp         eax,2
  00000001408A9278: jne         00000001408A9289
  00000001408A927A: lea         rcx,[r15+00000000002E75D8h]
  00000001408A9281: mov         rdx,r12
  00000001408A9284: call        000000014065B090
  00000001408A9289: mov         rcx,r12
  00000001408A928C: call        0000000140199350
  00000001408A9291: nop
  00000001408A9292: mov         rcx,rbx
  00000001408A9295: call        00000001401582E0
  00000001408A929A: nop
  00000001408A929B: lea         rcx,[rbp+77h]
  00000001408A929F: call        00000001408A2DE0
  00000001408A92A4: nop
  00000001408A92A5: mov         rcx,rbx
  00000001408A92A8: call        00000001401582E0
  00000001408A92AD: lea         rbx,[r15+00000000000601B8h]
  00000001408A92B4: mov         dl,1
  00000001408A92B6: mov         rcx,rbx
  00000001408A92B9: call        0000000140158290
  00000001408A92BE: mov         r15d,dword ptr [rbx+10h]
  00000001408A92C2: and         r15d,0FFFFFFh
  00000001408A92C9: mov         rcx,rbx
  00000001408A92CC: call        00000001401582E0
  00000001408A92D1: dec         r15d
  00000001408A92D4: movsxd      r14,r15d
  00000001408A92D7: test        r15d,r15d
  00000001408A92DA: jle         00000001408A9320
  00000001408A92DC: nop         dword ptr [rax]
  00000001408A92E0: mov         dl,1
  00000001408A92E2: mov         rcx,rbx
  00000001408A92E5: call        0000000140158290
  00000001408A92EA: mov         rsi,qword ptr [rbx+8]
  00000001408A92EE: mov         rcx,rbx
  00000001408A92F1: call        00000001401582E0
  00000001408A92F6: mov         eax,dword ptr [rsi+r14*4]
  00000001408A92FA: cmp         eax,dword ptr [rbp+6Fh]
  00000001408A92FD: je          00000001408A930C
  00000001408A92FF: dec         r15d
  00000001408A9302: dec         r14
  00000001408A9305: test        r14,r14
  00000001408A9308: jg          00000001408A92E0
  00000001408A930A: jmp         00000001408A9320
  00000001408A930C: mov         edx,r15d
  00000001408A930F: mov         rcx,rbx
  00000001408A9312: call        00000001408AC2B0
  00000001408A9317: or          dword ptr [r12+00000000000002F4h],2
  00000001408A9320: mov         qword ptr [r13-18h],r12
  00000001408A9324: movss       dword ptr [r13-8],xmm6
  00000001408A932A: movss       dword ptr [r13-4],xmm7
  00000001408A9330: movss       dword ptr [r13],xmm8
  00000001408A9336: lea         rdx,[rbp-69h]
  00000001408A933A: lea         rcx,[rbp-49h]
  00000001408A933E: call        000000014010FC20
  00000001408A9343: mov         ecx,dword ptr [rax]
  00000001408A9345: mov         dword ptr [r13+4],ecx
  00000001408A9349: mov         ecx,dword ptr [rax+4]
  00000001408A934C: mov         dword ptr [r13+8],ecx
  00000001408A9350: mov         eax,dword ptr [rax+8]
  00000001408A9353: mov         dword ptr [r13+0Ch],eax
  00000001408A9357: mov         ebx,dword ptr [r12+00000000000000A8h]
  00000001408A935F: call        0000000140810240
  00000001408A9364: mov         rcx,rax
  00000001408A9367: mov         r8d,ebx
  00000001408A936A: mov         edx,168h
  00000001408A936F: call        00000001408103F0
  00000001408A9374: nop
  00000001408A9375: test        byte ptr [rbp-4Dh],40h
  00000001408A9379: jne         00000001408A93BB
  00000001408A937B: mov         rbx,qword ptr [rbp-59h]
  00000001408A937F: test        rbx,rbx
  00000001408A9382: je          00000001408A93AC
  00000001408A9384: call        00000001401548E0
  00000001408A9389: mov         r10,qword ptr [rax]
  00000001408A938C: mov         qword ptr [rsp+20h],10h
  00000001408A9395: mov         r9,rbx
  00000001408A9398: mov         r8d,1ECh
  00000001408A939E: lea         rdx,[0000000141AB2230h]
  00000001408A93A5: mov         rcx,rax
  00000001408A93A8: call        qword ptr [r10+10h]
  00000001408A93AC: mov         qword ptr [rbp-59h],0
  00000001408A93B4: movzx       eax,byte ptr [rbp-4Dh]
  00000001408A93B8: mov         dword ptr [rbp-4Dh],eax
  00000001408A93BB: and         dword ptr [rbp-51h],0FF000000h
  00000001408A93C2: add         r13,28h
  00000001408A93C6: lea         rax,[r13-18h]
  00000001408A93CA: cmp         rax,rdi
  00000001408A93CD: je          00000001408A9409
  00000001408A93CF: mov         r15,qword ptr [rbp+67h]
  00000001408A93D3: jmp         00000001408A9020
  00000001408A93D8: lea         rcx,[r14+20h]
  00000001408A93DC: call        00000001401D9090
  00000001408A93E1: mov         rdx,rax
  00000001408A93E4: lea         rcx,[0000000141C95428h]
  00000001408A93EB: call        0000000140166830
  00000001408A93F0: int         3
  00000001408A93F1: lea         rcx,[r14+20h]
  00000001408A93F5: call        00000001401D9090
  00000001408A93FA: mov         rdx,rax
  00000001408A93FD: lea         rcx,[0000000141C95460h]
  00000001408A9404: call        0000000140166830
  00000001408A9409: lea         r11,[rsp+00000000000000C8h]
  00000001408A9411: movaps      xmm6,xmmword ptr [r11-18h]
  00000001408A9416: movaps      xmm7,xmmword ptr [r11-28h]
  00000001408A941B: movaps      xmm8,xmmword ptr [r11-38h]
  00000001408A9420: mov         rsp,r11
  00000001408A9423: pop         r15
  00000001408A9425: pop         r14
  00000001408A9427: pop         r13
  00000001408A9429: pop         r12
  00000001408A942B: pop         rdi
  00000001408A942C: pop         rsi
  00000001408A942D: pop         rbx
  00000001408A942E: pop         rbp
  00000001408A942F: ret
  00000001408A9430: 48

````
