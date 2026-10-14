# disasm-4f90c0.txt

Original: [work/disasm-4f90c0.txt](<../../../../work/disasm-4f90c0.txt>)

SHA-256: `EF1B79CF07DE4EF5BB4FBDE064C3A8C030EA9B1E55303DDAD5B93EE6A4B81ECA`; 11529 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001404F90C0: push        rbp
  00000001404F90C2: push        rsi
  00000001404F90C3: push        rdi
  00000001404F90C4: push        r12
  00000001404F90C6: push        r13
  00000001404F90C8: push        r14
  00000001404F90CA: push        r15
  00000001404F90CC: lea         rbp,[rsp+FFFFFFFFFFFFFF50h]
  00000001404F90D4: sub         rsp,1B0h
  00000001404F90DB: mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
  00000001404F90E4: mov         qword ptr [rsp+00000000000001F8h],rbx
  00000001404F90EC: mov         r13,rcx
  00000001404F90EF: add         rcx,60h
  00000001404F90F3: mov         edx,2
  00000001404F90F8: call        00000001403DA910
  00000001404F90FD: mov         rsi,rax
  00000001404F9100: test        rax,rax
  00000001404F9103: je          00000001404F9462
  00000001404F9109: mov         rcx,rax
  00000001404F910C: call        0000000140724160
  00000001404F9111: mov         edx,dword ptr [rax]
  00000001404F9113: mov         dword ptr [rbp+0000000000000108h],edx
  00000001404F9119: mov         rcx,qword ptr [r13+8]
  00000001404F911D: call        00000001408832B0
  00000001404F9122: mov         r15,rax
  00000001404F9125: test        rax,rax
  00000001404F9128: je          00000001404F9456
  00000001404F912E: lea         rcx,[rax+00000000001F7780h]
  00000001404F9135: lea         rdx,[rbp+0000000000000108h]
  00000001404F913C: call        00000001404E9140
  00000001404F9141: mov         rcx,rax
  00000001404F9144: call        000000014091EF70
  00000001404F9149: test        rax,rax
  00000001404F914C: je          00000001404F944D
  00000001404F9152: mov         rdi,qword ptr [rax+40h]
  00000001404F9156: test        rdi,rdi
  00000001404F9159: je          00000001404F943E
  00000001404F915F: lea         rcx,[r15+00000000001F77C8h]
  00000001404F9166: mov         edx,13h
  00000001404F916B: lea         r8d,[rdx-12h]
  00000001404F916F: call        000000014067C730
  00000001404F9174: mov         r12d,eax
  00000001404F9177: xor         bl,bl
  00000001404F9179: lea         r14,[r15+0000000000312740h]
  00000001404F9180: mov         byte ptr [rbp+00000000000000F0h],bl
  00000001404F9186: mov         rcx,r14
  00000001404F9189: call        0000000140515B60
  00000001404F918E: test        al,al
  00000001404F9190: je          00000001404F9285
  00000001404F9196: lea         rcx,[rdi+0000000000000090h]
  00000001404F919D: call        00000001401D9090
  00000001404F91A2: mov         rbx,rax
  00000001404F91A5: lea         rcx,[rsp+70h]
  00000001404F91AA: call        000000014013FB30
  00000001404F91AF: nop
  00000001404F91B0: xor         edx,edx
  00000001404F91B2: mov         r8d,0E8h
  00000001404F91B8: lea         rcx,[rbp-68h]
  00000001404F91BC: call        00000001411AD450
  00000001404F91C1: mov         byte ptr [rbp-80h],0
  00000001404F91C5: lea         rax,[rbp-80h]
  00000001404F91C9: mov         qword ptr [rsp+70h],rax
  00000001404F91CE: and         dword ptr [rsp+78h],0E0000000h
  00000001404F91D6: mov         ecx,dword ptr [rsp+7Ch]
  00000001404F91DA: and         ecx,0C0000100h
  00000001404F91E0: or          ecx,80000100h
  00000001404F91E6: mov         dword ptr [rsp+7Ch],ecx
  00000001404F91EA: mov         rdx,rbx
  00000001404F91ED: lea         rcx,[rsp+70h]
  00000001404F91F2: call        000000014013FF40
  00000001404F91F7: nop
  00000001404F91F8: lea         rdx,[0000000141AB3BC8h]
  00000001404F91FF: lea         rcx,[rsp+70h]
  00000001404F9204: call        0000000140145120
  00000001404F9209: lea         rcx,[rsp+70h]
  00000001404F920E: call        0000000140145000
  00000001404F9213: lea         rcx,[rsp+70h]
  00000001404F9218: call        0000000140140DA0
  00000001404F921D: lea         rcx,[rsp+70h]
  00000001404F9222: call        00000001401D9090
  00000001404F9227: mov         rcx,rax
  00000001404F922A: lea         rdx,[0000000141B58688h]
  00000001404F9231: call        00000001411ADC40
  00000001404F9236: test        eax,eax
  00000001404F9238: jne         00000001404F923E
  00000001404F923A: mov         bl,1
  00000001404F923C: jmp         00000001404F927B
  00000001404F923E: mov         ebx,dword ptr [rdi+000000000000020Ch]
  00000001404F9244: mov         edx,ebx
  00000001404F9246: mov         rcx,r14
  00000001404F9249: call        0000000140515950
  00000001404F924E: mov         rdi,rax
  00000001404F9251: mov         rcx,r14
  00000001404F9254: call        0000000140515980
  00000001404F9259: mov         r12d,eax
  00000001404F925C: mov         edx,ebx
  00000001404F925E: mov         rcx,r14
  00000001404F9261: call        0000000140516090
  00000001404F9266: mov         byte ptr [rbp+00000000000000F0h],al
  00000001404F926C: xor         r8d,r8d
  00000001404F926F: mov         edx,ebx
  00000001404F9271: mov         rcx,r14
  00000001404F9274: call        00000001405160C0
  00000001404F9279: xor         bl,bl
  00000001404F927B: lea         rcx,[rsp+70h]
  00000001404F9280: call        000000014013FD10
  00000001404F9285: lea         rcx,[rsp+38h]
  00000001404F928A: call        000000014013FB30
  00000001404F928F: nop
  00000001404F9290: test        r12d,r12d
  00000001404F9293: je          00000001404F92AA
  00000001404F9295: lea         rcx,[rdi+00000000000000B8h]
  00000001404F929C: call        00000001401D9090
  00000001404F92A1: lea         rdx,[rdi+0000000000000108h]
  00000001404F92A8: jmp         00000001404F92BD
  00000001404F92AA: lea         rcx,[rdi+0000000000000090h]
  00000001404F92B1: call        00000001401D9090
  00000001404F92B6: lea         rdx,[rdi+00000000000000E0h]
  00000001404F92BD: mov         r12,rax
  00000001404F92C0: lea         rcx,[rsp+38h]
  00000001404F92C5: call        000000014013FD70
  00000001404F92CA: mov         rcx,r14
  00000001404F92CD: call        0000000140515B60
  00000001404F92D2: test        al,al
  00000001404F92D4: je          00000001404F9311
  00000001404F92D6: cmp         dword ptr [rdi+000000000000020Ch],9
  00000001404F92DD: jne         00000001404F9311
  00000001404F92DF: test        bl,bl
  00000001404F92E1: jne         00000001404F9311
  00000001404F92E3: lea         rdx,[0000000141B6C468h]
  00000001404F92EA: lea         rcx,[rbp+0000000000000080h]
  00000001404F92F1: call        000000014013F9D0
  00000001404F92F6: nop
  00000001404F92F7: mov         rdx,rax
  00000001404F92FA: lea         rcx,[rsp+38h]
  00000001404F92FF: call        000000014013FD70
  00000001404F9304: nop
  00000001404F9305: lea         rcx,[rbp+0000000000000080h]
  00000001404F930C: call        000000014013FD10
  00000001404F9311: mov         rcx,r13
  00000001404F9314: call        0000000140789710
  00000001404F9319: mov         r14,rax
  00000001404F931C: mov         rcx,rsi
  00000001404F931F: call        000000014092E460
  00000001404F9324: cmp         dword ptr [rax],0
  00000001404F9327: je          00000001404F9349
  00000001404F9329: mov         rcx,rsi
  00000001404F932C: call        000000014092E460
  00000001404F9331: mov         ecx,dword ptr [rax]
  00000001404F9333: mov         dword ptr [rsp+30h],ecx
  00000001404F9337: lea         rdx,[rsp+30h]
  00000001404F933C: mov         rcx,qword ptr [rsi+8]
  00000001404F9340: call        0000000140723DB0
  00000001404F9345: mov         ebx,eax
  00000001404F9347: jmp         00000001404F934F
  00000001404F9349: mov         ebx,dword ptr [rdi+000000000000020Ch]
  00000001404F934F: mov         rcx,r14
  00000001404F9352: call        0000000140465D80
  00000001404F9357: mov         esi,ebx
  00000001404F9359: or          ecx,0FFFFFFFFh
  00000001404F935C: test        al,al
  00000001404F935E: cmovne      esi,ecx
  00000001404F9361: mov         rcx,qword ptr [0000000142274420h]
  00000001404F9368: mov         rax,qword ptr [rcx]
  00000001404F936B: call        qword ptr [rax+0000000000000120h]
  00000001404F9371: test        al,al
  00000001404F9373: je          00000001404F937A
  00000001404F9375: cmp         ebx,3
  00000001404F9378: jg          00000001404F93A4
  00000001404F937A: lea         rcx,[rsp+38h]
  00000001404F937F: call        00000001401D9090
  00000001404F9384: mov         r8,rax
  00000001404F9387: movzx       eax,byte ptr [rbp+00000000000000F0h]
  00000001404F938E: mov         byte ptr [rsp+28h],al
  00000001404F9392: mov         dword ptr [rsp+20h],esi
  00000001404F9396: mov         r9,rdi
  00000001404F9399: mov         rdx,r12
  00000001404F939C: mov         rcx,r14
  00000001404F939F: call        0000000140466660
  00000001404F93A4: mov         rcx,r15
  00000001404F93A7: call        00000001408A8BA0
  00000001404F93AC: mov         r8d,2
  00000001404F93B2: lea         rdx,[00000001439BA310h]
  00000001404F93B9: mov         rcx,rax
  00000001404F93BC: call        0000000140877720
  00000001404F93C1: test        rax,rax
  00000001404F93C4: je          00000001404F93D3
  00000001404F93C6: cmp         byte ptr [rax+6Ch],0
  00000001404F93CA: je          00000001404F93D3
  00000001404F93CC: mov         dword ptr [rax+68h],0
  00000001404F93D3: mov         rcx,qword ptr [0000000142274420h]
  00000001404F93DA: mov         rax,qword ptr [rcx]
  00000001404F93DD: call        qword ptr [rax+0000000000000120h]
  00000001404F93E3: test        al,al
  00000001404F93E5: je          00000001404F9432
  00000001404F93E7: cmp         ebx,3
  00000001404F93EA: jle         00000001404F9432
  00000001404F93EC: mov         rax,qword ptr [00000001439A5290h]
  00000001404F93F3: mov         rbx,qword ptr [rax+00000000001F6F60h]
  00000001404F93FA: lea         rcx,[rsp+38h]
  00000001404F93FF: call        00000001401D9090
  00000001404F9404: mov         r8,rax
  00000001404F9407: mov         dword ptr [rsp+20h],esi
  00000001404F940B: mov         r9,rdi
  00000001404F940E: mov         rdx,r12
  00000001404F9411: mov         rcx,rbx
  00000001404F9414: call        00000001404F1A90
  00000001404F9419: mov         rcx,qword ptr [00000001439A5290h]
  00000001404F9420: add         rcx,1F6EC8h
  00000001404F9427: mov         edx,10h
  00000001404F942C: call        00000001407885B0
  00000001404F9431: nop
  00000001404F9432: lea         rcx,[rsp+38h]
  00000001404F9437: call        000000014013FD10
  00000001404F943C: jmp         00000001404F9462
  00000001404F943E: mov         rcx,qword ptr [0000000142274420h]
  00000001404F9445: mov         rax,qword ptr [rcx]
  00000001404F9448: call        qword ptr [rax+40h]
  00000001404F944B: jmp         00000001404F9462
  00000001404F944D: lea         rcx,[0000000141B6C480h]
  00000001404F9454: jmp         00000001404F945D
  00000001404F9456: lea         rcx,[0000000141B6C4D0h]
  00000001404F945D: call        0000000140166830
  00000001404F9462: mov         rbx,qword ptr [rsp+00000000000001F8h]
  00000001404F946A: add         rsp,1B0h
  00000001404F9471: pop         r15
  00000001404F9473: pop         r14
  00000001404F9475: pop         r13
  00000001404F9477: pop         r12
  00000001404F9479: pop         rdi
  00000001404F947A: pop         rsi
  00000001404F947B: pop         rbp
  00000001404F947C: ret
  00000001404F947D: int         3

  Summary

     1A7D000 .text

````
