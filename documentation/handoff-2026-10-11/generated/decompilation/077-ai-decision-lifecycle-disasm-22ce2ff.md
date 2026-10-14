# ai-decision-lifecycle-disasm.txt

Original: [work/ai-decision-lifecycle-disasm.txt](<../../../../work/ai-decision-lifecycle-disasm.txt>)

SHA-256: `DDD2D6743AC99123200E2A03D6D577B566C84B6431E202AC829E184290CF6DB5`; 12008 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe


  00000001405DE400: push        rbx
  00000001405DE402: sub         rsp,20h
  00000001405DE406: mov         rbx,rcx
  00000001405DE409: call        0000000140588300
  00000001405DE40E: mov         rcx,qword ptr [rbx+10h]
  00000001405DE412: lea         rdx,[00000001439B3E50h]
  00000001405DE419: xor         r8d,r8d
  00000001405DE41C: call        0000000140877720
  00000001405DE421: mov         qword ptr [rbx+38h],rax
  00000001405DE425: add         rsp,20h
  00000001405DE429: pop         rbx
  00000001405DE42A: ret
  00000001405DE42B: int         3
  00000001405DE42C: int         3
  00000001405DE42D: int         3
  00000001405DE42E: int         3
  00000001405DE42F: int         3
  00000001405DE430: mov         qword ptr [rcx+38h],0
  00000001405DE438: jmp         00000001404D6840
  00000001405DE43D: int         3
  00000001405DE43E: int         3
  00000001405DE43F: int         3
  00000001405DE440: mov         qword ptr [rsp+8],rcx
  00000001405DE445: push        rbx
  00000001405DE446: push        rbp
  00000001405DE447: push        rsi
  00000001405DE448: push        rdi
  00000001405DE449: push        r12
  00000001405DE44B: push        r13
  00000001405DE44D: push        r14
  00000001405DE44F: push        r15
  00000001405DE451: sub         rsp,78h
  00000001405DE455: mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
  00000001405DE45E: mov         rbx,rcx
  00000001405DE461: call        0000000140588310
  00000001405DE466: mov         r13,qword ptr [rbx+10h]
  00000001405DE46A: xor         r8d,r8d
  00000001405DE46D: lea         rdx,[00000001439E8010h]
  00000001405DE474: mov         rcx,r13
  00000001405DE477: call        0000000140877720
  00000001405DE47C: mov         qword ptr [rsp+00000000000000D0h],rax
  00000001405DE484: test        rax,rax
  00000001405DE487: je          00000001405DE749
  00000001405DE48D: mov         r15,qword ptr [rbx+8]
  00000001405DE491: lea         rcx,[rsp+40h]
  00000001405DE496: call        00000001403EF1B0
  00000001405DE49B: nop
  00000001405DE49C: mov         r14d,1
  00000001405DE4A2: mov         esi,r14d
  00000001405DE4A5: mov         edi,r14d
  00000001405DE4A8: mov         r12d,4
  00000001405DE4AE: mov         qword ptr [rsp+00000000000000D8h],r12
  00000001405DE4B6: mov         ebp,r12d
  00000001405DE4B9: mov         r12,qword ptr [rsp+00000000000000D0h]
  00000001405DE4C1: nop         word ptr [rax+rax+0000000000000000h]
  00000001405DE4D0: xor         r8d,r8d
  00000001405DE4D3: xor         edx,edx
  00000001405DE4D5: mov         rax,qword ptr [0000000143987068h]
  00000001405DE4DC: mov         r9,qword ptr [rax+10h]
  00000001405DE4E0: mov         rax,qword ptr [r9]
  00000001405DE4E3: cmp         byte ptr [rax],dl
  00000001405DE4E5: je          00000001405DE51D
  00000001405DE4E7: mov         rcx,r9
  00000001405DE4EA: nop         word ptr [rax+rax]
  00000001405DE4F0: cmp         rdi,qword ptr [rcx+8]
  00000001405DE4F4: je          00000001405DE510
  00000001405DE4F6: inc         r8d
  00000001405DE4F9: inc         rdx
  00000001405DE4FC: mov         rcx,rdx
  00000001405DE4FF: shl         rcx,4
  00000001405DE503: add         rcx,r9
  00000001405DE506: mov         rax,qword ptr [rcx]
  00000001405DE509: cmp         byte ptr [rax],0
  00000001405DE50C: jne         00000001405DE4F0
  00000001405DE50E: jmp         00000001405DE51D
  00000001405DE510: movsxd      rbx,r8d
  00000001405DE513: add         rbx,23h
  00000001405DE517: lea         rbx,[r15+rbx*8]
  00000001405DE51B: jmp         00000001405DE524
  00000001405DE51D: mov         rbx,qword ptr [0000000143987070h]
  00000001405DE524: mov         rax,qword ptr [rbx]
  00000001405DE527: cmp         byte ptr [rax],0
  00000001405DE52A: je          00000001405DE597
  00000001405DE52C: lea         rcx,[rsp+38h]
  00000001405DE531: call        0000000140131AE0
  00000001405DE536: mov         r9,rax
  00000001405DE539: mov         r8,rbx
  00000001405DE53C: lea         rdx,[rsp+00000000000000C8h]
  00000001405DE544: mov         rcx,r12
  00000001405DE547: call        0000000140548990
  00000001405DE54C: mov         rcx,qword ptr [rsp+40h]
  00000001405DE551: add         rcx,rsi
  00000001405DE554: lea         rax,[rsp+00000000000000C8h]
  00000001405DE55C: cmp         rcx,rax
  00000001405DE55F: je          00000001405DE56B
  00000001405DE561: movzx       eax,byte ptr [rsp+00000000000000C8h]
  00000001405DE569: mov         byte ptr [rcx],al
  00000001405DE56B: cmp         byte ptr [rsp+00000000000000C8h],0FFh
  00000001405DE573: jg          00000001405DE597
  00000001405DE575: mov         rbx,qword ptr [rbx]
  00000001405DE578: mov         rcx,r13
  00000001405DE57B: call        00000001408E1770
  00000001405DE580: mov         r9,rax
  00000001405DE583: mov         r8,rbx
  00000001405DE586: lea         rdx,[0000000141B95294h]
  00000001405DE58D: mov         ecx,6Eh
  00000001405DE592: call        0000000140166830
  00000001405DE597: inc         rdi
  00000001405DE59A: inc         rsi
  00000001405DE59D: dec         rbp
  00000001405DE5A0: jne         00000001405DE4D0
  00000001405DE5A6: mov         rdi,r14
  00000001405DE5A9: mov         r12,qword ptr [rsp+00000000000000D8h]
  00000001405DE5B1: nop         word ptr [rax+rax+0000000000000000h]
  00000001405DE5C0: xor         r8d,r8d
  00000001405DE5C3: xor         edx,edx
  00000001405DE5C5: mov         rax,qword ptr [0000000143987078h]
  00000001405DE5CC: mov         r9,qword ptr [rax+10h]
  00000001405DE5D0: mov         rax,qword ptr [r9]
  00000001405DE5D3: cmp         byte ptr [rax],dl
  00000001405DE5D5: je          00000001405DE60D
  00000001405DE5D7: mov         rax,r9
  00000001405DE5DA: nop         word ptr [rax+rax]
  00000001405DE5E0: cmp         rdi,qword ptr [rax+8]
  00000001405DE5E4: je          00000001405DE600
  00000001405DE5E6: inc         r8d
  00000001405DE5E9: inc         rdx
  00000001405DE5EC: mov         rax,rdx
  00000001405DE5EF: shl         rax,4
  00000001405DE5F3: add         rax,r9
  00000001405DE5F6: mov         rcx,qword ptr [rax]
  00000001405DE5F9: cmp         byte ptr [rcx],0
  00000001405DE5FC: jne         00000001405DE5E0
  00000001405DE5FE: jmp         00000001405DE60D
  00000001405DE600: movsxd      rbx,r8d
  00000001405DE603: add         rbx,29h
  00000001405DE607: lea         rbx,[r15+rbx*8]
  00000001405DE60B: jmp         00000001405DE614
  00000001405DE60D: 48
  00000001405DE60E: 8B
  00000001405DE60F: 1D
  00000001405DE610: ins         byte ptr [rdi],dx
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe


  00000001405883F0: mov         qword ptr [rsp+8],rbx
  00000001405883F5: push        rdi
  00000001405883F6: sub         rsp,30h
  00000001405883FA: mov         rdi,rcx
  00000001405883FD: mov         rcx,qword ptr [rcx+30h]
  0000000140588401: call        00000001405D8190
  0000000140588406: mov         rax,qword ptr [rdi]
  0000000140588409: mov         rdx,qword ptr [rdi+20h]
  000000014058840D: mov         rcx,rdi
  0000000140588410: call        qword ptr [rax+00000000000000E8h]
  0000000140588416: mov         rbx,qword ptr [rdi+30h]
  000000014058841A: mov         qword ptr [rdi+20h],0
  0000000140588422: test        rbx,rbx
  0000000140588425: je          0000000140588437
  0000000140588427: mov         rcx,rbx
  000000014058842A: call        00000001404E81F0
  000000014058842F: mov         rcx,rbx
  0000000140588432: call        00000001400F1520
  0000000140588437: mov         rbx,qword ptr [rdi+10h]
  000000014058843B: xor         eax,eax
  000000014058843D: mov         qword ptr [rdi+30h],0
  0000000140588445: mov         qword ptr [rsp+20h],rax
  000000014058844A: mov         qword ptr [rsp+28h],rax
  000000014058844F: lea         rax,[0000000140588A90h]
  0000000140588456: mov         qword ptr [rsp+28h],rax
  000000014058845B: mov         qword ptr [rsp+20h],rdi
  0000000140588460: call        0000000140810240
  0000000140588465: lea         r9,[rsp+20h]
  000000014058846A: mov         r8d,25h
  0000000140588470: mov         rdx,rbx
  0000000140588473: mov         rcx,rax
  0000000140588476: call        0000000140811160
  000000014058847B: mov         eax,dword ptr [rdi+1Ch]
  000000014058847E: and         al,30h
  0000000140588480: cmp         al,20h
  0000000140588482: jae         0000000140588490
  0000000140588484: mov         rax,qword ptr [rdi]
  0000000140588487: mov         rcx,rdi
  000000014058848A: call        qword ptr [rax+00000000000000D8h]
  0000000140588490: 48
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe


  00000001405A75A0: push        rbx
  00000001405A75A2: sub         rsp,20h
  00000001405A75A6: mov         rbx,rcx
  00000001405A75A9: call        00000001407E75B0
  00000001405A75AE: mov         rax,qword ptr [rbx]
  00000001405A75B1: mov         rcx,rbx
  00000001405A75B4: add         rsp,20h
  00000001405A75B8: pop         rbx
  00000001405A75B9: jmp         qword ptr [rax+00000000000000D8h]
  00000001405A75C0: push        rbx
  00000001405A75C2: sub         rsp,20h
  00000001405A75C6: mov         rbx,rcx
  00000001405A75C9: call        00000001407E7610
  00000001405A75CE: mov         rax,qword ptr [rbx]
  00000001405A75D1: mov         rcx,rbx
  00000001405A75D4: add         rsp,20h
  00000001405A75D8: pop         rbx
  00000001405A75D9: jmp         qword ptr [rax+00000000000000D0h]
  00000001405A75E0: mov         qword ptr [rsp+8],rbx
  00000001405A75E5: push        rdi
  00000001405A75E6: sub         rsp,30h
  00000001405A75EA: mov         rdi,rcx
  00000001405A75ED: call        00000001407E7670
  00000001405A75F2: mov         rbx,qword ptr [rdi+10h]
  00000001405A75F6: xor         eax,eax
  00000001405A75F8: mov         qword ptr [rsp+20h],rax
  00000001405A75FD: mov         qword ptr [rsp+28h],rax
  00000001405A7602: lea         rax,[00000001405A77B0h]
  00000001405A7609: mov         qword ptr [rsp+28h],rax
  00000001405A760E: mov         qword ptr [rsp+20h],rdi
  00000001405A7613: call        0000000140810240
  00000001405A7618: lea         r9,[rsp+20h]
  00000001405A761D: mov         r8d,2Fh
  00000001405A7623: mov         rdx,rbx
  00000001405A7626: mov         rcx,rax
  00000001405A7629: call        0000000140810810
  00000001405A762E: mov         eax,dword ptr [rdi+1Ch]
  00000001405A7631: and         al,30h
  00000001405A7633: cmp         al,20h
  00000001405A7635: jae         00000001405A7643
  00000001405A7637: mov         rax,qword ptr [rdi]
  00000001405A763A: mov         rcx,rdi
  00000001405A763D: call        qword ptr [rax+00000000000000D0h]
  00000001405A7643: mov         rbx,qword ptr [rsp+40h]
  00000001405A7648: add         rsp,30h
  00000001405A764C: pop         rdi
  00000001405A764D: ret
  00000001405A764E: int         3
  00000001405A764F: int         3
  00000001405A7650: mov         qword ptr [rsp+8],rbx
  00000001405A7655: push        rdi
  00000001405A7656: sub         rsp,30h
  00000001405A765A: mov         eax,dword ptr [rcx+1Ch]
  00000001405A765D: mov         rdi,rcx
  00000001405A7660: and         al,30h
  00000001405A7662: cmp         al,20h
  00000001405A7664: jae         00000001405A766F
  00000001405A7666: mov         rax,qword ptr [rcx]
  00000001405A7669: call        qword ptr [rax+00000000000000D8h]
  00000001405A766F: 48
  00000001405A7670: 8B

````
