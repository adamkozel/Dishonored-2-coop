# loading-commit-disasm.txt

Original: [work/loading-commit-disasm.txt](<../../../../work/loading-commit-disasm.txt>)

SHA-256: `16A3FA5DF6CD977045C0391AC5C476327C52AF9315E3E7241F2E77A5718C37AA`; 7365 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe


  0000000140787510: push        rsi
  0000000140787512: push        rdi
  0000000140787513: push        r14
  0000000140787515: sub         rsp,30h
  0000000140787519: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  0000000140787522: mov         qword ptr [rsp+58h],rbx
  0000000140787527: mov         qword ptr [rsp+60h],rbp
  000000014078752C: mov         rsi,rcx
  000000014078752F: mov         rdx,qword ptr [rcx+8]
  0000000140787533: test        rdx,rdx
  0000000140787536: je          000000014078773D
  000000014078753C: mov         r8b,1
  000000014078753F: lea         rcx,[rsp+50h]
  0000000140787544: call        00000001408A2DC0
  0000000140787549: nop
  000000014078754A: test        dword ptr [rsi+0000000000000898h],0FFFFFFh
  0000000140787554: jle         000000014078755E
  0000000140787556: mov         rcx,rsi
  0000000140787559: call        0000000140787BF0
  000000014078755E: movsxd      rdx,dword ptr [rsi+0000000000000538h]
  0000000140787565: test        edx,edx
  0000000140787567: je          0000000140787731
  000000014078756D: test        dword ptr [rsi+0000000000000530h],0FFFFFFh
  0000000140787577: jle         0000000140787593
  0000000140787579: mov         ecx,dword ptr [rsi+0000000000000530h]
  000000014078757F: and         ecx,0FFFFFFh
  0000000140787585: dec         ecx
  0000000140787587: mov         rax,qword ptr [rsi+0000000000000528h]
  000000014078758E: mov         ebp,dword ptr [rax+rcx*4]
  0000000140787591: jmp         0000000140787595
  0000000140787593: xor         ebp,ebp
  0000000140787595: mov         r8,rdx
  0000000140787598: movsxd      r14,ebp
  000000014078759B: mov         rax,r14
  000000014078759E: imul        rax,rax,11h
  00000001407875A2: add         rax,rdx
  00000001407875A5: lea         rdi,[rsi+rax*4]
  00000001407875A9: cmp         ebp,edx
  00000001407875AB: jne         00000001407875E4
  00000001407875AD: cmp         dword ptr [000000014398E878h],0
  00000001407875B4: setne       bl
  00000001407875B7: mov         r8,r14
  00000001407875BA: lea         rdx,[0000000141C5F628h]
  00000001407875C1: mov         rcx,qword ptr [0000000142294408h]
  00000001407875C8: call        000000014014BEF0
  00000001407875CD: mov         r8,rax
  00000001407875D0: lea         rdx,[0000000141C5F840h]
  00000001407875D7: movzx       ecx,bl
  00000001407875DA: call        00000001401668E0
  00000001407875DF: jmp         0000000140787727
  00000001407875E4: cmp         byte ptr [rdi+00000000000000A0h],0
  00000001407875EB: je          00000001407876DA
  00000001407875F1: cmp         byte ptr [rdi+00000000000000A1h],0
  00000001407875F8: je          000000014078761D
  00000001407875FA: cmp         dword ptr [000000014398E878h],0
  0000000140787601: setne       al
  0000000140787604: lea         rdx,[0000000141C5F890h]
  000000014078760B: movzx       ecx,al
  000000014078760E: call        00000001401668E0
  0000000140787613: mov         rcx,rsi
  0000000140787616: call        0000000140787F50
  000000014078761B: jmp         000000014078764F
  000000014078761D: cmp         byte ptr [rdi+00000000000000A2h],0
  0000000140787624: je          000000014078764F
  0000000140787626: cmp         dword ptr [000000014398E878h],0
  000000014078762D: setne       al
  0000000140787630: lea         rdx,[0000000141C5F8E0h]
  0000000140787637: movzx       ecx,al
  000000014078763A: call        00000001401668E0
  000000014078763F: mov         rcx,rsi
  0000000140787642: call        00000001407881A0
  0000000140787647: mov         rcx,rsi
  000000014078764A: call        0000000140787BF0
  000000014078764F: mov         eax,dword ptr [rsi+0000000000000530h]
  0000000140787655: and         eax,0FFFFFFh
  000000014078765A: xor         edx,edx
  000000014078765C: xor         ecx,ecx
  000000014078765E: dec         eax
  0000000140787660: movsxd      r9,eax
  0000000140787663: js          00000001407876BC
  0000000140787665: movsxd      r8,dword ptr [rsi+0000000000000538h]
  000000014078766C: mov         rax,qword ptr [rsi+0000000000000528h]
  0000000140787673: cmp         dword ptr [rax],r8d
  0000000140787676: je          0000000140787688
  0000000140787678: inc         edx
  000000014078767A: inc         rcx
  000000014078767D: add         rax,4
  0000000140787681: cmp         rcx,r9
  0000000140787684: jle         0000000140787673
  0000000140787686: jmp         00000001407876BC
  0000000140787688: cmp         edx,0FFFFFFFFh
  000000014078768B: je          00000001407876BC
  000000014078768D: cmp         dword ptr [000000014398E878h],0
  0000000140787694: setne       bl
  0000000140787697: lea         rdx,[0000000141C5F628h]
  000000014078769E: mov         rcx,qword ptr [0000000142294408h]
  00000001407876A5: call        000000014014BEF0
  00000001407876AA: mov         r8,rax
  00000001407876AD: lea         rdx,[0000000141C5F930h]
  00000001407876B4: movzx       ecx,bl
  00000001407876B7: call        00000001401668E0
  00000001407876BC: movzx       r8d,byte ptr [rdi+00000000000000A3h]
  00000001407876C4: mov         edx,dword ptr [rsi+0000000000000538h]
  00000001407876CA: mov         rcx,rsi
  00000001407876CD: call        0000000140787FC0
  00000001407876D2: mov         dword ptr [rsi+000000000000053Ch],ebp
  00000001407876D8: jmp         0000000140787727
  00000001407876DA: cmp         dword ptr [000000014398E878h],0
  00000001407876E1: setne       dil
  00000001407876E5: lea         rdx,[0000000141C5F628h]
  00000001407876EC: mov         rcx,qword ptr [0000000142294408h]
  00000001407876F3: call        000000014014BEF0
  00000001407876F8: mov         rbx,rax
  00000001407876FB: mov         r8,r14
  00000001407876FE: lea         rdx,[0000000141C5F628h]
  0000000140787705: mov         rcx,qword ptr [0000000142294408h]
  000000014078770C: call        000000014014BEF0
  0000000140787711: mov         r8,rax
  0000000140787714: mov         r9,rbx
  0000000140787717: lea         rdx,[0000000141C5F980h]
  000000014078771E: movzx       ecx,dil
  0000000140787722: call        00000001401668E0
  0000000140787727: mov         dword ptr [rsi+0000000000000538h],0
  0000000140787731: lea         rcx,[rsp+50h]
  0000000140787736: call        00000001408A2DE0
  000000014078773B: jmp         0000000140787756
  000000014078773D: cmp         dword ptr [000000014398E878h],0
  0000000140787744: setne       al
  0000000140787747: lea         rdx,[0000000141C5F9D0h]
  000000014078774E: movzx       ecx,al
  0000000140787751: call        00000001401668E0
  0000000140787756: mov         rbx,qword ptr [rsp+58h]
  000000014078775B: mov         rbp,qword ptr [rsp+60h]
  0000000140787760: add         rsp,30h
  0000000140787764: pop         r14
  0000000140787766: pop         rdi
  0000000140787767: pop         rsi
  0000000140787768: ret
  0000000140787769: int         3
  000000014078776A: int         3
  000000014078776B: int         3
  000000014078776C: int         3
  000000014078776D: int         3
  000000014078776E: int         3
  000000014078776F: int         3
  0000000140787770: 48

````
