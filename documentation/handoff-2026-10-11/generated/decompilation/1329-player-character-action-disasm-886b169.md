# player-character-action-disasm.txt

Original: [work/player-character-action-disasm.txt](<../../../../work/player-character-action-disasm.txt>)

SHA-256: `164438C6CA9CDE4CF9A1C62575C6C820242BB22920437429E6B6CD1BA7BC28F2`; 36990 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe


  0000000140533270: mov         qword ptr [rsp+18h],rbp
  0000000140533275: push        rdi
  0000000140533276: push        r14
  0000000140533278: push        r15
  000000014053327A: sub         rsp,40h
  000000014053327E: mov         rax,qword ptr [rcx+28h]
  0000000140533282: xor         r15d,r15d
  0000000140533285: mov         rbp,r8
  0000000140533288: mov         r14,rdx
  000000014053328B: mov         rdi,rcx
  000000014053328E: mov         dword ptr [rsp+20h],r15d
  0000000140533293: test        rax,rax
  0000000140533296: je          00000001405333B2
  000000014053329C: mov         qword ptr [rsp+60h],rbx
  00000001405332A1: mov         rbx,qword ptr [rax+40h]
  00000001405332A5: mov         eax,dword ptr [rax+48h]
  00000001405332A8: and         eax,0FFFFFFh
  00000001405332AD: lea         rax,[rax+rax*8]
  00000001405332B1: lea         rax,[rbx+rax*8]
  00000001405332B5: cmp         rbx,rax
  00000001405332B8: je          00000001405333AD
  00000001405332BE: nop
  00000001405332C0: mov         rcx,qword ptr [rbx+20h]
  00000001405332C4: test        rcx,rcx
  00000001405332C7: je          00000001405332D5
  00000001405332C9: cmp         qword ptr [rcx+20h],r8
  00000001405332CD: jne         00000001405332D5
  00000001405332CF: cmp         qword ptr [rcx+28h],r15
  00000001405332D3: jne         00000001405332E3
  00000001405332D5: add         rbx,48h
  00000001405332D9: cmp         rbx,rax
  00000001405332DC: jne         00000001405332C0
  00000001405332DE: jmp         00000001405333AD
  00000001405332E3: mov         rax,qword ptr [rcx+28h]
  00000001405332E7: mov         r8,r9
  00000001405332EA: mov         rdx,rbp
  00000001405332ED: mov         rcx,rdi
  00000001405332F0: mov         qword ptr [rsp+68h],rsi
  00000001405332F5: call        rax
  00000001405332F7: movzx       esi,al
  00000001405332FA: lea         rax,[00000001439A77F0h]
  0000000140533301: cmp         rbp,rax
  0000000140533304: je          0000000140533311
  0000000140533306: mov         rdx,rbx
  0000000140533309: mov         rcx,rdi
  000000014053330C: call        00000001407BEA70
  0000000140533311: test        sil,sil
  0000000140533314: jne         0000000140533343
  0000000140533316: mov         rax,qword ptr [rdi+28h]
  000000014053331A: lea         r8,[rsp+24h]
  000000014053331F: mov         r9b,1
  0000000140533322: mov         ecx,dword ptr [rax+8]
  0000000140533325: mov         eax,dword ptr [rbx+8]
  0000000140533328: mov         rdx,rdi
  000000014053332B: mov         dword ptr [rsp+2Ch],ecx
  000000014053332F: mov         rcx,qword ptr [rdi+20h]
  0000000140533333: mov         qword ptr [rsp+24h],r15
  0000000140533338: mov         dword ptr [rsp+30h],eax
  000000014053333C: call        00000001407BB9D0
  0000000140533341: jmp         00000001405333A8
  0000000140533343: cmp         sil,1
  0000000140533347: jne         0000000140533377
  0000000140533349: mov         rax,qword ptr [rdi+28h]
  000000014053334D: lea         r8,[rsp+24h]
  0000000140533352: movzx       r9d,sil
  0000000140533356: mov         ecx,dword ptr [rax+8]
  0000000140533359: mov         eax,dword ptr [rbx+8]
  000000014053335C: mov         rdx,rdi
  000000014053335F: mov         dword ptr [rsp+2Ch],ecx
  0000000140533363: mov         rcx,qword ptr [rdi+20h]
  0000000140533367: mov         qword ptr [rsp+24h],r15
  000000014053336C: mov         dword ptr [rsp+30h],eax
  0000000140533370: call        00000001407BB950
  0000000140533375: jmp         00000001405333A8
  0000000140533377: cmp         sil,3
  000000014053337B: jne         00000001405333A8
  000000014053337D: mov         rax,qword ptr [rdi+28h]
  0000000140533381: lea         r8,[rsp+24h]
  0000000140533386: mov         r9b,1
  0000000140533389: mov         ecx,dword ptr [rax+8]
  000000014053338C: mov         eax,dword ptr [rbx+8]
  000000014053338F: mov         rdx,rdi
  0000000140533392: mov         dword ptr [rsp+2Ch],ecx
  0000000140533396: mov         rcx,qword ptr [rdi+20h]
  000000014053339A: mov         qword ptr [rsp+24h],r15
  000000014053339F: mov         dword ptr [rsp+30h],eax
  00000001405333A3: call        00000001407BBB70
  00000001405333A8: mov         rsi,qword ptr [rsp+68h]
  00000001405333AD: mov         rbx,qword ptr [rsp+60h]
  00000001405333B2: mov         rbp,qword ptr [rsp+70h]
  00000001405333B7: mov         byte ptr [r14],r15b
  00000001405333BA: mov         qword ptr [r14+8],r15
  00000001405333BE: mov         rax,r14
  00000001405333C1: add         rsp,40h
  00000001405333C5: pop         r15
  00000001405333C7: pop         r14
  00000001405333C9: pop         rdi
  00000001405333CA: ret
  00000001405333CB: int         3
  00000001405333CC: int         3
  00000001405333CD: int         3
  00000001405333CE: int         3
  00000001405333CF: int         3
  00000001405333D0: push        rdi
  00000001405333D2: sub         rsp,20h
  00000001405333D6: cmp         dword ptr [rcx+30h],0
  00000001405333DA: mov         rdi,rcx
  00000001405333DD: jbe         000000014053340E
  00000001405333DF: lea         rdx,[00000001439845F8h]
  00000001405333E6: mov         qword ptr [rsp+30h],rbx
  00000001405333EB: mov         rbx,qword ptr [00000001439A5290h]
  00000001405333F2: call        0000000140522AE0
  00000001405333F7: mov         edx,dword ptr [rdi+30h]
  00000001405333FA: lea         rcx,[rbx+00000000001F5338h]
  0000000140533401: mov         r8d,dword ptr [rax]
  0000000140533404: call        00000001407D71F0
  0000000140533409: mov         rbx,qword ptr [rsp+30h]
  000000014053340E: xor         al,al
  0000000140533410: add         rsp,20h
  0000000140533414: pop         rdi
  0000000140533415: ret
  0000000140533416: int         3
  0000000140533417: int         3
  0000000140533418: int         3
  0000000140533419: int         3
  000000014053341A: int         3
  000000014053341B: int         3
  000000014053341C: int         3
  000000014053341D: int         3
  000000014053341E: int         3
  000000014053341F: int         3
  0000000140533420: sub         rsp,28h
  0000000140533424: xor         edx,edx
  0000000140533426: call        0000000140533460
  000000014053342B: xor         al,al
  000000014053342D: add         rsp,28h
  0000000140533431: ret
  0000000140533432: int         3
  0000000140533433: int         3
  0000000140533434: int         3
  0000000140533435: int         3
  0000000140533436: int         3
  0000000140533437: int         3
  0000000140533438: int         3
  0000000140533439: int         3
  000000014053343A: int         3
  000000014053343B: int         3
  000000014053343C: int         3
  000000014053343D: int         3
  000000014053343E: int         3
  000000014053343F: int         3
  0000000140533440: sub         rsp,28h
  0000000140533444: mov         edx,1
  0000000140533449: call        0000000140533460
  000000014053344E: xor         al,al
  0000000140533450: add         rsp,28h
  0000000140533454: ret
  0000000140533455: int         3
  0000000140533456: int         3
  0000000140533457: int         3
  0000000140533458: int         3
  0000000140533459: int         3
  000000014053345A: int         3
  000000014053345B: int         3
  000000014053345C: int         3
  000000014053345D: int         3
  000000014053345E: int         3
  000000014053345F: int         3
  0000000140533460: push        rbx
  0000000140533462: push        r15
  0000000140533464: sub         rsp,78h
  0000000140533468: mov         rbx,rcx
  000000014053346B: mov         rcx,qword ptr [00000001439A5290h]
  0000000140533472: mov         r15d,edx
  0000000140533475: mov         rax,qword ptr [rcx]
  0000000140533478: call        qword ptr [rax+0000000000000088h]
  000000014053347E: test        byte ptr [rax+8],40h
  0000000140533482: jne         000000014053373D
  0000000140533488: mov         qword ptr [rsp+0000000000000090h],rbp
  0000000140533490: mov         qword ptr [rsp+60h],r12
  0000000140533495: lea         rdx,[0000000143984090h]
  000000014053349C: mov         rcx,rbx
  000000014053349F: mov         qword ptr [rsp+58h],r13
  00000001405334A4: mov         qword ptr [rsp+50h],r14
  00000001405334A9: call        0000000140531EA0
  00000001405334AE: lea         rdx,[0000000143984500h]
  00000001405334B5: mov         rcx,rbx
  00000001405334B8: mov         rbp,qword ptr [rax]
  00000001405334BB: call        0000000140523610
  00000001405334C0: lea         rdx,[0000000143984898h]
  00000001405334C7: mov         rcx,rbx
  00000001405334CA: mov         r14,rax
  00000001405334CD: call        0000000140522AE0
  00000001405334D2: lea         rdx,[00000001439846E8h]
  00000001405334D9: mov         rcx,rbx
  00000001405334DC: mov         r12,rax
  00000001405334DF: call        0000000140522BB0
  00000001405334E4: mov         rcx,qword ptr [r14]
  00000001405334E7: movzx       r13d,byte ptr [rax]
  00000001405334EB: test        rcx,rcx
  00000001405334EE: jne         0000000140533514
  00000001405334F0: lea         rcx,[rbx+38h]
  00000001405334F4: lea         rdx,[0000000141B7D648h]
  00000001405334FB: call        000000014013FF40
  0000000140533500: lea         rdx,[0000000143983B50h]
  0000000140533507: mov         rcx,rbx
  000000014053350A: call        00000001407BE910
  000000014053350F: jmp         0000000140533726
  0000000140533514: cmp         rbp,rcx
  0000000140533517: je          000000014053353D
  0000000140533519: lea         rcx,[rbx+38h]
  000000014053351D: lea         rdx,[0000000141B7D680h]
  0000000140533524: call        000000014013FF40
  0000000140533529: lea         rdx,[0000000143983B50h]
  0000000140533530: mov         rcx,rbx
  0000000140533533: call        00000001407BE910
  0000000140533538: jmp         0000000140533726
  000000014053353D: cmp         dword ptr [rbx+30h],0
  0000000140533541: mov         qword ptr [rsp+70h],rsi
  0000000140533546: mov         qword ptr [rsp+68h],rdi
  000000014053354B: lea         rdi,[rbx+30h]
  000000014053354F: jbe         00000001405335B9
  0000000140533551: mov         rsi,qword ptr [00000001439A5290h]
  0000000140533558: mov         edx,dword ptr [rdi]
  000000014053355A: mov         r8d,r15d
  000000014053355D: add         rsi,1F5338h
  0000000140533564: mov         rcx,rsi
  0000000140533567: call        00000001407D8F70
  000000014053356C: test        r13b,r13b
  000000014053356F: je          000000014053357E
  0000000140533571: mov         edx,dword ptr [rdi]
  0000000140533573: mov         r8,r12
  0000000140533576: mov         rcx,rsi
  0000000140533579: call        00000001407D8220
  000000014053357E: mov         edx,dword ptr [rdi]
  0000000140533580: mov         rcx,rsi
  0000000140533583: call        00000001407D6280
  0000000140533588: test        al,al
  000000014053358A: je          00000001405335AD
  000000014053358C: lea         rdx,[00000001439845F8h]
  0000000140533593: mov         rcx,rbx
  0000000140533596: call        0000000140522AE0
  000000014053359B: mov         edx,dword ptr [rdi]
  000000014053359D: mov         rcx,rsi
  00000001405335A0: mov         r8d,dword ptr [rax]
  00000001405335A3: call        00000001407D8950
  00000001405335A8: jmp         000000014053371C
  00000001405335AD: lea         rdx,[0000000141B7D6F0h]
  00000001405335B4: jmp         0000000140533704
  00000001405335B9: mov         rcx,r14
  00000001405335BC: call        00000001407C8960
  00000001405335C1: test        al,al
  00000001405335C3: jne         00000001405335D1
  00000001405335C5: lea         rdx,[0000000141B7D718h]
  00000001405335CC: jmp         0000000140533704
  00000001405335D1: mov         rcx,rbp
  00000001405335D4: call        0000000140197140
  00000001405335D9: test        al,al
  00000001405335DB: jne         00000001405335F5
  00000001405335DD: mov         rcx,r14
  00000001405335E0: call        00000001407C8A60
  00000001405335E5: test        al,al
  00000001405335E7: jne         00000001405335F5
  00000001405335E9: lea         rdx,[0000000141B7D748h]
  00000001405335F0: jmp         0000000140533704
  00000001405335F5: mov         rsi,qword ptr [00000001439A5290h]
  00000001405335FC: lea         rax,[rsp+00000000000000A0h]
  0000000140533604: xor         r13d,r13d
  0000000140533607: mov         qword ptr [rsp+30h],rax
  000000014053360C: lea         rax,[rsp+40h]
  0000000140533611: add         rsi,1F5338h
  0000000140533618: lea         r9,[r14+8]
  000000014053361C: lea         rdx,[rsp+00000000000000A8h]
  0000000140533624: mov         r8,rbp
  0000000140533627: mov         rcx,rsi
  000000014053362A: mov         qword ptr [rsp+28h],rbx
  000000014053362F: mov         dword ptr [rsp+00000000000000A0h],r13d
  0000000140533637: mov         qword ptr [rsp+20h],rax
  000000014053363C: mov         dword ptr [rsp+40h],r15d
  0000000140533641: mov         word ptr [rsp+44h],r13w
  0000000140533647: call        00000001407D6040
  000000014053364C: cmp         rdi,rax
  000000014053364F: je          0000000140533655
  0000000140533651: mov         eax,dword ptr [rax]
  0000000140533653: mov         dword ptr [rdi],eax
  0000000140533655: cmp         dword ptr [rdi],r13d
  0000000140533658: jbe         00000001405336F5
  000000014053365E: lea         rdx,[0000000143984628h]
  0000000140533665: mov         rcx,rbx
  0000000140533668: call        0000000140522A10
  000000014053366D: movss       xmm2,dword ptr [rax]
  0000000140533671: ucomiss     xmm2,dword ptr [0000000141F697B0h]
  0000000140533678: jp          000000014053367C
  000000014053367A: je          0000000140533695
  000000014053367C: movss       xmm0,dword ptr [0000000141AB3B04h]
  0000000140533684: mov         edx,dword ptr [rdi]
  0000000140533686: mov         rcx,rsi
  0000000140533689: maxss       xmm0,xmm2
  000000014053368D: movaps      xmm2,xmm0
  0000000140533690: call        00000001407D8FE0
  0000000140533695: lea         rdx,[0000000143984608h]
  000000014053369C: mov         rcx,rbx
  000000014053369F: call        00000001405328F0
  00000001405336A4: mov         r8d,dword ptr [rax]
  00000001405336A7: cmp         r8d,3
  00000001405336AB: je          00000001405336B7
  00000001405336AD: mov         edx,dword ptr [rdi]
  00000001405336AF: mov         rcx,rsi
  00000001405336B2: call        00000001407D8EA0
  00000001405336B7: mov         edx,dword ptr [rdi]
  00000001405336B9: lea         rax,[rsp+00000000000000A0h]
  00000001405336C1: lea         r9,[rsp+00000000000000A8h]
  00000001405336C9: mov         r8,r12
  00000001405336CC: mov         rcx,rsi
  00000001405336CF: mov         dword ptr [rsp+28h],100002h
  00000001405336D7: mov         qword ptr [rsp+20h],rax
  00000001405336DC: mov         dword ptr [rsp+00000000000000A8h],r13d
  00000001405336E4: call        00000001407D9270
  00000001405336E9: test        al,al
  00000001405336EB: jne         00000001405336F0
  00000001405336ED: mov         dword ptr [rdi],r13d
  00000001405336F0: cmp         dword ptr [rdi],r13d
  00000001405336F3: ja          000000014053371C
  00000001405336F5: mov         ecx,dword ptr [rsp+00000000000000A0h]
  00000001405336FC: call        00000001407D5A90
  0000000140533701: mov         rdx,rax
  0000000140533704: lea         rcx,[rbx+38h]
  0000000140533708: call        000000014013FF40
  000000014053370D: lea         rdx,[0000000143983B50h]
  0000000140533714: mov         rcx,rbx
  0000000140533717: call        00000001407BE910
  000000014053371C: mov         rdi,qword ptr [rsp+68h]
  0000000140533721: mov         rsi,qword ptr [rsp+70h]
  0000000140533726: mov         r12,qword ptr [rsp+60h]
  000000014053372B: mov         r13,qword ptr [rsp+58h]
  0000000140533730: mov         rbp,qword ptr [rsp+0000000000000090h]
  0000000140533738: mov         r14,qword ptr [rsp+50h]
  000000014053373D: add         rsp,78h
  0000000140533741: pop         r15
  0000000140533743: pop         rbx
  0000000140533744: ret
  0000000140533745: int         3
  0000000140533746: int         3
  0000000140533747: int         3
  0000000140533748: int         3
  0000000140533749: int         3
  000000014053374A: int         3
  000000014053374B: int         3
  000000014053374C: int         3
  000000014053374D: int         3
  000000014053374E: int         3
  000000014053374F: int         3
  0000000140533750: push        rbx
  0000000140533752: sub         rsp,20h
  0000000140533756: mov         edx,2Ah
  000000014053375B: mov         rbx,rcx
  000000014053375E: lea         ecx,[rdx-1Ah]
  0000000140533761: call        00000001400F1450
  0000000140533766: test        rax,rax
  0000000140533769: je          0000000140533781
  000000014053376B: lea         rcx,[0000000141B7D908h]
  0000000140533772: mov         qword ptr [rax],rcx
  0000000140533775: mov         ecx,dword ptr [rbx+8]
  0000000140533778: mov         dword ptr [rax+8],ecx
  000000014053377B: add         rsp,20h
  000000014053377F: pop         rbx
  0000000140533780: ret
  0000000140533781: add         rsp,20h
  0000000140533785: pop         rbx
  0000000140533786: ret
  0000000140533787: int         3
  0000000140533788: int         3
  0000000140533789: int         3
  000000014053378A: int         3
  000000014053378B: int         3
  000000014053378C: int         3
  000000014053378D: int         3
  000000014053378E: int         3
  000000014053378F: int         3
  0000000140533790: push        rbx
  0000000140533792: sub         rsp,30h
  0000000140533796: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  000000014053379F: mov         edx,2Ah
  00000001405337A4: lea         ecx,[rdx-0Ah]
  00000001405337A7: call        00000001400F1450
  00000001405337AC: mov         rbx,rax
  00000001405337AF: mov         qword ptr [rsp+48h],rax
  00000001405337B4: test        rax,rax
  00000001405337B7: je          00000001405337D6
  00000001405337B9: lea         rax,[0000000141B78E10h]
  00000001405337C0: mov         qword ptr [rbx],rax
  00000001405337C3: mov         dword ptr [rbx+8],0
  00000001405337CA: lea         rcx,[rbx+10h]
  00000001405337CE: call        00000001409097F0
  00000001405337D3: nop
  00000001405337D4: jmp         00000001405337D8
  00000001405337D6: xor         ebx,ebx
  00000001405337D8: lea         r8,[0000000143984B28h]
  00000001405337DF: mov         edx,6
  00000001405337E4: mov         rcx,rbx
  00000001405337E7: call        00000001407C2500
  00000001405337EC: lea         r8,[00000001405338A0h]
  00000001405337F3: lea         rdx,[0000000143983B90h]
  00000001405337FA: mov         rcx,rbx
  00000001405337FD: call        00000001407C2A70
  0000000140533802: lea         r8,[0000000140533A70h]
  0000000140533809: lea         rdx,[0000000143984F90h]
  0000000140533810: mov         rcx,rbx
  0000000140533813: call        00000001407C2A70
  0000000140533818: lea         r8,[0000000143984F38h]
  000000014053381F: mov         edx,5
  0000000140533824: mov         rcx,rbx
  0000000140533827: call        00000001407C2500
  000000014053382C: lea         r8,[0000000143984C88h]
  0000000140533833: mov         edx,5
  0000000140533838: mov         rcx,rbx
  000000014053383B: call        00000001407C2500
  0000000140533840: lea         r8,[0000000143983B78h]
  0000000140533847: mov         edx,5
  000000014053384C: mov         rcx,rbx
  000000014053384F: call        00000001407C2500
  0000000140533854: mov         rax,rbx
  0000000140533857: add         rsp,30h
  000000014053385B: pop         rbx
  000000014053385C: ret
  000000014053385D: int         3
  000000014053385E: int         3
  000000014053385F: int         3
  0000000140533860: sub         rsp,28h
  0000000140533864: mov         edx,2Ah
  0000000140533869: lea         ecx,[rdx-1Ah]
  000000014053386C: call        00000001400F1450
  0000000140533871: test        rax,rax
  0000000140533874: je          000000014053388C
  0000000140533876: lea         rcx,[0000000141B7D908h]
  000000014053387D: mov         qword ptr [rax],rcx
  0000000140533880: mov         dword ptr [rax+8],3FFF0010h
  0000000140533887: add         rsp,28h
  000000014053388B: ret
  000000014053388C: add         rsp,28h
  0000000140533890: ret
  0000000140533891: int         3
  0000000140533892: int         3
  0000000140533893: int         3
  0000000140533894: int         3
  0000000140533895: int         3
  0000000140533896: int         3
  0000000140533897: int         3
  0000000140533898: int         3
  0000000140533899: int         3
  000000014053389A: int         3
  000000014053389B: int         3
  000000014053389C: int         3
  000000014053389D: int         3
  000000014053389E: int         3
  000000014053389F: int         3
  00000001405338A0: mov         qword ptr [rsp+18h],rbx
  00000001405338A5: push        rdi
  00000001405338A6: sub         rsp,70h
  00000001405338AA: lea         rdx,[0000000143984560h]
  00000001405338B1: mov         rbx,rcx
  00000001405338B4: call        0000000140528040
  00000001405338B9: mov         edi,dword ptr [rax]
  00000001405338BB: mov         ecx,edi
  00000001405338BD: call        00000001406DC560
  00000001405338C2: test        rax,rax
  00000001405338C5: je          00000001405339E0
  00000001405338CB: mov         ecx,edi
  00000001405338CD: mov         qword ptr [rsp+0000000000000088h],rsi
  00000001405338D5: call        00000001406DC560
  00000001405338DA: lea         rdx,[00000001439B3E50h]
  00000001405338E1: mov         r8d,2
  00000001405338E7: mov         rcx,rax
  00000001405338EA: call        0000000140877720
  00000001405338EF: mov         rsi,rax
  00000001405338F2: test        rax,rax
  00000001405338F5: jne         000000014053391F
  00000001405338F7: mov         rcx,qword ptr [rbx+20h]
  00000001405338FB: lea         rdx,[0000000141B7D938h]
  0000000140533902: call        00000001407BC8E0
  0000000140533907: mov         rsi,qword ptr [rsp+0000000000000088h]
  000000014053390F: xor         al,al
  0000000140533911: mov         rbx,qword ptr [rsp+0000000000000090h]
  0000000140533919: add         rsp,70h
  000000014053391D: pop         rdi
  000000014053391E: ret
  000000014053391F: mov         ecx,edi
  0000000140533921: mov         qword ptr [rsp+0000000000000080h],rbp
  0000000140533929: call        00000001406DC560
  000000014053392E: mov         rdx,rbx
  0000000140533931: mov         rcx,rax
  0000000140533934: call        0000000140883A10
  0000000140533939: mov         rcx,rsi
  000000014053393C: call        000000014092E460
  0000000140533941: mov         ecx,edi
  0000000140533943: mov         rbp,rax
  0000000140533946: call        00000001406DC560
  000000014053394B: mov         ecx,edi
  000000014053394D: mov         ebx,dword ptr [rax+00000000000000A8h]
  0000000140533953: call        00000001406DC560
  0000000140533958: mov         ecx,edi
  000000014053395A: mov         rsi,rax
  000000014053395D: call        00000001406DC560
  0000000140533962: lea         rcx,[rsp+30h]
  0000000140533967: mov         rdx,rax
  000000014053396A: call        00000001405BECE0
  000000014053396F: lea         r8,[rsi+0000000000000300h]
  0000000140533976: lea         rcx,[rsp+40h]
  000000014053397B: mov         r9d,ebx
  000000014053397E: mov         rdx,rax
  0000000140533981: mov         dword ptr [rsp+20h],0
  0000000140533989: call        000000014055A340
  000000014053398E: lea         rdx,[rsp+40h]
  0000000140533993: mov         rcx,rbp
  0000000140533996: call        00000001405652C0
  000000014053399B: mov         ecx,edi
  000000014053399D: call        00000001406DC560
  00000001405339A2: mov         ebx,dword ptr [rax+00000000000000A8h]
  00000001405339A8: call        0000000140810240
  00000001405339AD: mov         r9d,ebx
  00000001405339B0: lea         rcx,[rax+000000000000C8E8h]
  00000001405339B7: mov         edx,8Bh
  00000001405339BC: mov         r8d,1
  00000001405339C2: call        00000001408112C0
  00000001405339C7: mov         rbp,qword ptr [rsp+0000000000000080h]
  00000001405339CF: test        rax,rax
  00000001405339D2: je          0000000140533907
  00000001405339D8: mov         byte ptr [rax],1
  00000001405339DB: jmp         0000000140533907
  00000001405339E0: mov         rcx,qword ptr [rbx+20h]
  00000001405339E4: lea         rdx,[0000000141B7D968h]
  00000001405339EB: call        00000001407BC8E0
  00000001405339F0: mov         rbx,qword ptr [rsp+0000000000000090h]
  00000001405339F8: xor         al,al
  00000001405339FA: add         rsp,70h
  00000001405339FE: pop         rdi
  00000001405339FF: ret
  0000000140533A00: mov         qword ptr [rsp+8],rbx
  0000000140533A05: mov         qword ptr [rsp+10h],rsi
  0000000140533A0A: push        rdi
  0000000140533A0B: sub         rsp,20h
  0000000140533A0F: mov         rsi,rdx
  0000000140533A12: lea         rdx,[0000000143983B78h]
  0000000140533A19: mov         rdi,r8
  0000000140533A1C: mov         rbx,rcx
  0000000140533A1F: call        00000001407BE910
  0000000140533A24: mov         rdx,rbx
  0000000140533A27: mov         rcx,rdi
  0000000140533A2A: call        0000000140884B70
  0000000140533A2F: mov         rbx,qword ptr [rsp+30h]
  0000000140533A34: mov         rax,rsi
  0000000140533A37: mov         rsi,qword ptr [rsp+38h]
  0000000140533A3C: add         rsp,20h
  0000000140533A40: pop         rdi
  0000000140533A41: ret
  0000000140533A42: int         3
  0000000140533A43: int         3
  0000000140533A44: int         3
  0000000140533A45: int         3
  0000000140533A46: int         3
  0000000140533A47: int         3
  0000000140533A48: int         3
  0000000140533A49: int         3
  0000000140533A4A: int         3
  0000000140533A4B: int         3
  0000000140533A4C: int         3
  0000000140533A4D: int         3
  0000000140533A4E: int         3
  0000000140533A4F: int         3
  0000000140533A50: push        rbx
  0000000140533A52: sub         rsp,20h
  0000000140533A56: mov         rbx,rdx
  0000000140533A59: lea         rdx,[0000000143984C88h]
  0000000140533A60: call        00000001407BE910
  0000000140533A65: mov         rax,rbx
  0000000140533A68: add         rsp,20h
  0000000140533A6C: pop         rbx
  0000000140533A6D: ret
  0000000140533A6E: int         3
  0000000140533A6F: int         3
  0000000140533A70: mov         qword ptr [rsp+8],rbx
  0000000140533A75: push        rdi
  0000000140533A76: sub         rsp,20h
  0000000140533A7A: lea         rdx,[0000000143984560h]
  0000000140533A81: mov         rdi,rcx
  0000000140533A84: call        0000000140528040
  0000000140533A89: mov         ebx,dword ptr [rax]
  0000000140533A8B: mov         ecx,ebx
  0000000140533A8D: call        00000001406DC560
  0000000140533A92: test        rax,rax
  0000000140533A95: je          0000000140533ACD
  0000000140533A97: mov         ecx,ebx
  0000000140533A99: call        00000001406DC560
  0000000140533A9E: lea         rdx,[00000001439AE3D0h]
  0000000140533AA5: mov         r8d,1
  0000000140533AAB: mov         rcx,rax
  0000000140533AAE: call        0000000140877720
  0000000140533AB3: test        rax,rax
  0000000140533AB6: je          0000000140533AC0
  0000000140533AB8: mov         rcx,rax
  0000000140533ABB: call        00000001405987F0
  0000000140533AC0: mov         al,1
  0000000140533AC2: mov         rbx,qword ptr [rsp+30h]
  0000000140533AC7: add         rsp,20h
  0000000140533ACB: pop         rdi
  0000000140533ACC: ret
  0000000140533ACD: mov         rcx,qword ptr [rdi+20h]
  0000000140533AD1: lea         rdx,[0000000141B7D9A0h]
  0000000140533AD8: call        00000001407BC8E0
  0000000140533ADD: mov         rbx,qword ptr [rsp+30h]
  0000000140533AE2: xor         al,al
  0000000140533AE4: add         rsp,20h
  0000000140533AE8: pop         rdi
  0000000140533AE9: ret
  0000000140533AEA: int         3
  0000000140533AEB: int         3
  0000000140533AEC: int         3
  0000000140533AED: int         3
  0000000140533AEE: int         3
  0000000140533AEF: int         3
  0000000140533AF0: mov         qword ptr [rsp+8],rcx
  0000000140533AF5: push        rsi
  0000000140533AF6: push        rdi
  0000000140533AF7: push        r14
  0000000140533AF9: sub         rsp,30h
  0000000140533AFD: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  0000000140533B06: mov         qword ptr [rsp+60h],rbx
  0000000140533B0B: mov         rsi,rcx
  0000000140533B0E: call        00000001407BDAE0
  0000000140533B13: nop
  0000000140533B14: lea         rax,[0000000141B7DAE8h]
  0000000140533B1B: mov         qword ptr [rsi],rax
  0000000140533B1E: mov         dword ptr [rsi+30h],0
  0000000140533B25: mov         dword ptr [rsi+34h],0Eh
  0000000140533B2C: lea         rbx,[rsi+38h]
  0000000140533B30: mov         edi,5
  0000000140533B35: mov         rcx,rbx
  0000000140533B38: call        0000000140131AE0
  0000000140533B3D: add         rbx,8
  0000000140533B41: dec         edi
  0000000140533B43: jns         0000000140533B35
  0000000140533B45: xor         r8d,r8d
  0000000140533B48: lea         rdx,[0000000141B7DBC8h]
  0000000140533B4F: lea         rcx,[rsp+58h]
  0000000140533B54: call        0000000140131F90
  0000000140533B59: mov         rax,qword ptr [rsp+58h]
  0000000140533B5E: mov         qword ptr [rsi+38h],rax
  0000000140533B62: xor         r8d,r8d
  0000000140533B65: lea         rdx,[0000000141B7DBE0h]
  0000000140533B6C: lea         rcx,[rsp+58h]
  0000000140533B71: call        0000000140131F90
  0000000140533B76: mov         rax,qword ptr [rsp+58h]
  0000000140533B7B: mov         qword ptr [rsi+40h],rax
  0000000140533B7F: xor         r8d,r8d
  0000000140533B82: lea         rdx,[0000000141B7DBF8h]
  0000000140533B89: lea         rcx,[rsp+58h]
  0000000140533B8E: call        0000000140131F90
  0000000140533B93: mov         rax,qword ptr [rsp+58h]
  0000000140533B98: mov         qword ptr [rsi+48h],rax
  0000000140533B9C: xor         r8d,r8d
  0000000140533B9F: lea         rdx,[0000000141B7DC08h]
  0000000140533BA6: lea         rcx,[rsp+58h]
  0000000140533BAB: call        0000000140131F90
  0000000140533BB0: mov         rax,qword ptr [rsp+58h]
  0000000140533BB5: mov         qword ptr [rsi+50h],rax
  0000000140533BB9: xor         r8d,r8d
  0000000140533BBC: lea         rdx,[0000000141B7DC18h]
  0000000140533BC3: lea         rcx,[rsp+58h]
  0000000140533BC8: call        0000000140131F90
  0000000140533BCD: mov         rax,qword ptr [rsp+58h]
  0000000140533BD2: mov         qword ptr [rsi+58h],rax
  0000000140533BD6: xor         r8d,r8d
  0000000140533BD9: lea         rdx,[0000000141B7DC28h]
  0000000140533BE0: lea         rcx,[rsp+58h]
  0000000140533BE5: call        0000000140131F90
  0000000140533BEA: mov         rax,qword ptr [rsp+58h]
  0000000140533BEF: mov         qword ptr [rsi+60h],rax
  0000000140533BF3: mov         rax,rsi
  0000000140533BF6: mov         rbx,qword ptr [rsp+60h]
  0000000140533BFB: add         rsp,30h
  0000000140533BFF: pop         r14
  0000000140533C01: pop         rdi
  0000000140533C02: pop         rsi
  0000000140533C03: ret
  0000000140533C04: int         3
  0000000140533C05: int         3
  0000000140533C06: int         3
  0000000140533C07: int         3
  0000000140533C08: int         3
  0000000140533C09: int         3
  0000000140533C0A: int         3
  0000000140533C0B: int         3
  0000000140533C0C: int         3
  0000000140533C0D: int         3
  0000000140533C0E: int         3
  0000000140533C0F: int         3
  0000000140533C10: mov         qword ptr [rsp+8],rcx
  0000000140533C15: push        rbx
  0000000140533C16: sub         rsp,30h
  0000000140533C1A: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  0000000140533C23: mov         rbx,rcx
  0000000140533C26: add         rcx,10h
  0000000140533C2A: lea         r9,[00000001400ECAB0h]
  0000000140533C31: mov         edx,30h
  0000000140533C36: lea         r8d,[rdx-2Dh]
  0000000140533C3A: call        00000001411AD55C
  0000000140533C3F: nop
  0000000140533C40: mov         edx,dword ptr [rbx+0Ch]
  0000000140533C43: test        dl,40h
  0000000140533C46: jne         0000000140533C66
  0000000140533C48: mov         rcx,qword ptr [rbx]
  0000000140533C4B: test        rcx,rcx
  0000000140533C4E: je          0000000140533C58
  0000000140533C50: shr         edx,8
  0000000140533C53: call        00000001409D8700
  0000000140533C58: mov         qword ptr [rbx],0
  0000000140533C5F: and         dword ptr [rbx+0Ch],0FFh
  0000000140533C66: and         dword ptr [rbx+8],0FF000000h
  0000000140533C6D: add         rsp,30h
  0000000140533C71: pop         rbx
  0000000140533C72: ret
  0000000140533C73: int         3
  0000000140533C74: int         3
  0000000140533C75: int         3
  0000000140533C76: int         3
  0000000140533C77: int         3
  0000000140533C78: int         3
  0000000140533C79: int         3
  0000000140533C7A: int         3
  0000000140533C7B: int         3
  0000000140533C7C: int         3
  0000000140533C7D: int         3
  0000000140533C7E: int         3
  0000000140533C7F: int         3
  0000000140533C80: push        rbx
  0000000140533C82: sub         rsp,20h
  0000000140533C86: mov         rax,qword ptr [r8+20h]
  0000000140533C8A: mov         rbx,rcx
  0000000140533C8D: mov         qword ptr [rsp+30h],rax
  0000000140533C92: cmp         rax,qword ptr [rcx+38h]
  0000000140533C96: jne         0000000140533C9C
  0000000140533C98: xor         edx,edx
  0000000140533C9A: jmp         0000000140533CA7
  0000000140533C9C: cmp         rax,qword ptr [rcx+40h]
  0000000140533CA0: jne         0000000140533CBC
  0000000140533CA2: mov         edx,1
  0000000140533CA7: mov         rcx,qword ptr [00000001439A5290h]
  0000000140533CAE: mov         rcx,qword ptr [rcx+00000000001F6F50h]
  0000000140533CB5: call        00000001404EFFD0
  0000000140533CBA: jmp         0000000140533D05
  0000000140533CBC: cmp         rax,qword ptr [rcx+48h]
  0000000140533CC0: jne         0000000140533CD5
  0000000140533CC2: mov         rcx,qword ptr [00000001439A5290h]
  0000000140533CC9: mov         edx,2
  0000000140533CCE: call        00000001408A7650
  0000000140533CD3: jmp         0000000140533D05
  0000000140533CD5: cmp         rax,qword ptr [rcx+50h]
  0000000140533CD9: jne         0000000140533CEE
  0000000140533CDB: mov         rcx,qword ptr [00000001439A5290h]
  0000000140533CE2: mov         edx,1
  0000000140533CE7: call        00000001408A7650
  0000000140533CEC: jmp         0000000140533D05
  0000000140533CEE: cmp         rax,qword ptr [rcx+58h]
  0000000140533CF2: jne         0000000140533CF8
  0000000140533CF4: xor         edx,edx
  0000000140533CF6: jmp         0000000140533D00
  0000000140533CF8: cmp         rax,qword ptr [rcx+60h]
  0000000140533CFC: jne         0000000140533D05
  0000000140533CFE: mov         dl,1
  0000000140533D00: call        0000000140533F70
  0000000140533D05: cmp         dword ptr [rbx+30h],0
  0000000140533D09: jbe         0000000140533D18
  0000000140533D0B: lea         rdx,[rsp+30h]
  0000000140533D10: mov         rcx,rbx
  0000000140533D13: call        00000001407BE910
  0000000140533D18: mov         al,4
  0000000140533D1A: add         rsp,20h
  0000000140533D1E: pop         rbx
  0000000140533D1F: ret
  0000000140533D20: push        rbx
  0000000140533D22: sub         rsp,20h
  0000000140533D26: mov         edx,2Ah
  0000000140533D2B: mov         rbx,rcx
  0000000140533D2E: lea         ecx,[rdx-1Ah]
  0000000140533D31: call        00000001400F1450
  0000000140533D36: test        rax,rax
  0000000140533D39: je          0000000140533D53
  0000000140533D3B: lea         rcx,[0000000141B7DAA8h]
  0000000140533D42: mov         qword ptr [rax],rcx
  0000000140533D45: mov         rcx,qword ptr [rbx+8]
  0000000140533D49: mov         qword ptr [rax+8],rcx
  0000000140533D4D: add         rsp,20h
  0000000140533D51: pop         rbx
  0000000140533D52: ret
  0000000140533D53: add         rsp,20h
  0000000140533D57: pop         rbx
  0000000140533D58: ret
  0000000140533D59: int         3
  0000000140533D5A: int         3
  0000000140533D5B: int         3
  0000000140533D5C: int         3
  0000000140533D5D: int         3
  0000000140533D5E: int         3
  0000000140533D5F: int         3
  0000000140533D60: 40

````
