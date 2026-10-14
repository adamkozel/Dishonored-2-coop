# disasm-4ea000.txt

Original: [work/disasm-4ea000.txt](<../../../../work/disasm-4ea000.txt>)

SHA-256: `2B6B71DDB9AE7F9DD93F476549FD1B7808DDF7160E88CC629E8BBE05028E8E5C`; 2740 bytes.

## Original saved record

````text
Microsoft (R) COFF/PE Dumper Version 14.44.35229.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Program Files (x86)\Steam\steamapps\common\Dishonored2\Dishonored2.exe

File Type: EXECUTABLE IMAGE

  00000001404EA000: pushfq
  00000001404EA001: and         al,30h
  00000001404EA003: or          al,0
  00000001404EA005: add         byte ptr [rcx-75h],cl
  00000001404EA008: pop         rbx
  00000001404EA009: adc         byte ptr [rcx-75h],cl
  00000001404EA00C: imul        esp,dword ptr [rax],49h
  00000001404EA00F: mov         esi,dword ptr [rbx+28h]
  00000001404EA012: mov         rsp,r11
  00000001404EA015: pop         rdi
  00000001404EA016: ret
  00000001404EA017: int         3
  00000001404EA018: int         3
  00000001404EA019: int         3
  00000001404EA01A: int         3
  00000001404EA01B: int         3
  00000001404EA01C: int         3
  00000001404EA01D: int         3
  00000001404EA01E: int         3
  00000001404EA01F: int         3
  00000001404EA020: mov         qword ptr [rsp+8],rbx
  00000001404EA025: push        rdi
  00000001404EA026: sub         rsp,20h
  00000001404EA02A: mov         rdi,rdx
  00000001404EA02D: mov         rbx,rcx
  00000001404EA030: call        00000001404E9D00
  00000001404EA035: mov         rdx,rdi
  00000001404EA038: mov         rcx,rbx
  00000001404EA03B: call        00000001404E9DB0
  00000001404EA040: mov         rdx,rdi
  00000001404EA043: mov         rcx,rbx
  00000001404EA046: call        00000001404EA090
  00000001404EA04B: cmp         dword ptr [rdi+0Ch],3
  00000001404EA04F: jne         00000001404EA07C
  00000001404EA051: mov         rcx,rdi
  00000001404EA054: call        000000014091EF70
  00000001404EA059: test        rax,rax
  00000001404EA05C: je          00000001404EA07C
  00000001404EA05E: cmp         byte ptr [rax+48h],0
  00000001404EA062: je          00000001404EA07C
  00000001404EA064: mov         rcx,qword ptr [00000001439A5290h]
  00000001404EA06B: mov         edx,0Bh
  00000001404EA070: add         rcx,1F6EC8h
  00000001404EA077: call        00000001407885B0
  00000001404EA07C: mov         rbx,qword ptr [rsp+30h]
  00000001404EA081: add         rsp,20h
  00000001404EA085: pop         rdi
  00000001404EA086: ret
  00000001404EA087: int         3
  00000001404EA088: int         3
  00000001404EA089: int         3
  00000001404EA08A: int         3
  00000001404EA08B: int         3
  00000001404EA08C: int         3
  00000001404EA08D: int         3
  00000001404EA08E: int         3
  00000001404EA08F: int         3
  00000001404EA090: 48
  00000001404EA091: 89
  00000001404EA092: pop         rsp

  Summary

     1A7D000 .text

````
