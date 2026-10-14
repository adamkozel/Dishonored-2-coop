# ghidra-pickup-v785.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-pickup-v785.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-pickup-v785.json>)

SHA-256: `0DDA72780A11412C3C0A288765B993A084EC1A4B5382CF104A9F679B09660E5B`; 60007 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "scope": "Read-only exact coin identity and native component analysis. No pickup receiver call tested; no shared pickup implementation.",
  "vtables": [
    {
      "content": [
        {
          "type": "text",
          "text": "{\"address\":\"141c55bb8\",\"length\":320,\"data\":[224,181,142,64,1,0,0,0,176,59,116,64,1,0,0,0,48,117,126,64,1,0,0,0,48,75,116,64,1,0,0,0,240,78,116,64,1,0,0,0,144,79,116,64,1,0,0,0,80,119,126,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,117,126,64,1,0,0,0,16,118,126,64,1,0,0,0,240,115,126,64,1,0,0,0,96,117,126,64,1,0,0,0,176,152,9,64,1,0,0,0,16,66,116,64,1,0,0,0,80,72,116,64,1,0,0,0,48,72,116,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,16,116,126,64,1,0,0,0,176,152,9,64,1,0,0,0,240,86,116,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,98,101,108,116,0,0,0,0,98,101,108,116,48,50,0,0,116,111,32,98,101,32,97,98,108,101,32,116,111,32,114,101,109,111,118,101,32,98,111,110,101,99,104,97,114,109,115,32,97,108,114,101,97,100,121,32,111,119,110,101,100,32,98,121,32,116,104,101,32,112,108,97,121,101,114,0,0,0,0,0,110,101,101,100,32,97,32,115,116,97,114,116,101,100,32,114,105,103,105,100,98,111,100,121,32,102,111,114,32,115,112,101],\"hex\":\"e0b58e4001000000b03b74400100000030757e4001000000304b744001000000f04e744001000000904f74400100000050777e4001000000b098094001000000b098094001000000b098094001000000b0757e400100000010767e4001000000f0737e400100000060757e4001000000b098094001000000104274400100000050487440010000003048744001000000b098094001000000b098094001000000b09809400100000010747e4001000000b098094001000000f056744001000000b098094001000000b09809400100000062656c740000000062656c7430320000746f2062652061626c6520746f2072656d6f766520626f6e65636861726d7320616c7265616479206f776e65642062792074686520706c6179657200000000006e65656420612073746172746564207269676964626f647920666f7220737065\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"address\":\"141c55bb8\",\"length\":320,\"data\":[224,181,142,64,1,0,0,0,176,59,116,64,1,0,0,0,48,117,126,64,1,0,0,0,48,75,116,64,1,0,0,0,240,78,116,64,1,0,0,0,144,79,116,64,1,0,0,0,80,119,126,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,117,126,64,1,0,0,0,16,118,126,64,1,0,0,0,240,115,126,64,1,0,0,0,96,117,126,64,1,0,0,0,176,152,9,64,1,0,0,0,16,66,116,64,1,0,0,0,80,72,116,64,1,0,0,0,48,72,116,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,16,116,126,64,1,0,0,0,176,152,9,64,1,0,0,0,240,86,116,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,98,101,108,116,0,0,0,0,98,101,108,116,48,50,0,0,116,111,32,98,101,32,97,98,108,101,32,116,111,32,114,101,109,111,118,101,32,98,111,110,101,99,104,97,114,109,115,32,97,108,114,101,97,100,121,32,111,119,110,101,100,32,98,121,32,116,104,101,32,112,108,97,121,101,114,0,0,0,0,0,110,101,101,100,32,97,32,115,116,97,114,116,101,100,32,114,105,103,105,100,98,111,100,121,32,102,111,114,32,115,112,101],\"hex\":\"e0b58e4001000000b03b74400100000030757e4001000000304b744001000000f04e744001000000904f74400100000050777e4001000000b098094001000000b098094001000000b098094001000000b0757e400100000010767e4001000000f0737e400100000060757e4001000000b098094001000000104274400100000050487440010000003048744001000000b098094001000000b098094001000000b09809400100000010747e4001000000b098094001000000f056744001000000b098094001000000b09809400100000062656c740000000062656c7430320000746f2062652061626c6520746f2072656d6f766520626f6e65636861726d7320616c7265616479206f776e65642062792074686520706c6179657200000000006e65656420612073746172746564207269676964626f647920666f7220737065\"}"
      },
      "isError": false
    },
    {
      "content": [
        {
          "type": "text",
          "text": "{\"address\":\"141c5a3b8\",\"length\":320,\"data\":[16,182,142,64,1,0,0,0,80,226,118,64,1,0,0,0,48,117,126,64,1,0,0,0,96,232,118,64,1,0,0,0,64,104,77,64,1,0,0,0,176,232,118,64,1,0,0,0,240,234,118,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,117,126,64,1,0,0,0,16,118,126,64,1,0,0,0,240,115,126,64,1,0,0,0,96,117,126,64,1,0,0,0,176,152,9,64,1,0,0,0,112,230,118,64,1,0,0,0,32,231,118,64,1,0,0,0,0,231,118,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,16,116,126,64,1,0,0,0,176,152,9,64,1,0,0,0,240,236,118,64,1,0,0,0,128,237,118,64,1,0,0,0,176,152,9,64,1,0,0,0,80,231,118,64,1,0,0,0,0,198,119,64,1,0,0,0,16,197,119,64,1,0,0,0,32,240,118,64,1,0,0,0,0,0,0,0,0,0,0,0,109,111,100,101,108,115,47,105,110,116,101,114,97,99,116,105,118,101,47,112,105,99,107,117,112,115,47,119,104,97,108,101,95,98,111,110,101,95,102,114,97,103,109,101,110,116,95,115,101,116,95,48,49,47,119,104,97,108,101,95,98,111,110,101,95,102,114,97,103,109,101,110],\"hex\":\"10b68e400100000050e276400100000030757e400100000060e876400100000040684d4001000000b0e8764001000000f0ea764001000000b098094001000000b098094001000000b098094001000000b0757e400100000010767e4001000000f0737e400100000060757e4001000000b09809400100000070e676400100000020e776400100000000e7764001000000b098094001000000b098094001000000b09809400100000010747e4001000000b098094001000000f0ec76400100000080ed764001000000b09809400100000050e776400100000000c677400100000010c577400100000020f076400100000000000000000000006d6f64656c732f696e7465726163746976652f7069636b7570732f7768616c655f626f6e655f667261676d656e745f7365745f30312f7768616c655f626f6e655f667261676d656e\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"address\":\"141c5a3b8\",\"length\":320,\"data\":[16,182,142,64,1,0,0,0,80,226,118,64,1,0,0,0,48,117,126,64,1,0,0,0,96,232,118,64,1,0,0,0,64,104,77,64,1,0,0,0,176,232,118,64,1,0,0,0,240,234,118,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,117,126,64,1,0,0,0,16,118,126,64,1,0,0,0,240,115,126,64,1,0,0,0,96,117,126,64,1,0,0,0,176,152,9,64,1,0,0,0,112,230,118,64,1,0,0,0,32,231,118,64,1,0,0,0,0,231,118,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,16,116,126,64,1,0,0,0,176,152,9,64,1,0,0,0,240,236,118,64,1,0,0,0,128,237,118,64,1,0,0,0,176,152,9,64,1,0,0,0,80,231,118,64,1,0,0,0,0,198,119,64,1,0,0,0,16,197,119,64,1,0,0,0,32,240,118,64,1,0,0,0,0,0,0,0,0,0,0,0,109,111,100,101,108,115,47,105,110,116,101,114,97,99,116,105,118,101,47,112,105,99,107,117,112,115,47,119,104,97,108,101,95,98,111,110,101,95,102,114,97,103,109,101,110,116,95,115,101,116,95,48,49,47,119,104,97,108,101,95,98,111,110,101,95,102,114,97,103,109,101,110],\"hex\":\"10b68e400100000050e276400100000030757e400100000060e876400100000040684d4001000000b0e8764001000000f0ea764001000000b098094001000000b098094001000000b098094001000000b0757e400100000010767e4001000000f0737e400100000060757e4001000000b09809400100000070e676400100000020e776400100000000e7764001000000b098094001000000b098094001000000b09809400100000010747e4001000000b098094001000000f0ec76400100000080ed764001000000b09809400100000050e776400100000000c677400100000010c577400100000020f076400100000000000000000000006d6f64656c732f696e7465726163746976652f7069636b7570732f7768616c655f626f6e655f667261676d656e745f7365745f30312f7768616c655f626f6e655f667261676d656e\"}"
      },
      "isError": false
    },
    {
      "content": [
        {
          "type": "text",
          "text": "{\"address\":\"141c5e258\",\"length\":384,\"data\":[96,177,139,64,1,0,0,0,128,90,120,64,1,0,0,0,144,104,139,64,1,0,0,0,16,0,145,64,1,0,0,0,208,253,139,64,1,0,0,0,64,139,135,64,1,0,0,0,32,150,57,64,1,0,0,0,16,133,135,64,1,0,0,0,0,128,135,64,1,0,0,0,32,150,57,64,1,0,0,0,176,152,9,64,1,0,0,0,240,127,135,64,1,0,0,0,64,195,135,64,1,0,0,0,32,183,135,64,1,0,0,0,16,183,135,64,1,0,0,0,48,125,135,64,1,0,0,0,176,132,135,64,1,0,0,0,144,197,135,64,1,0,0,0,208,132,135,64,1,0,0,0,208,197,135,64,1,0,0,0,16,127,135,64,1,0,0,0,48,133,135,64,1,0,0,0,48,126,135,64,1,0,0,0,32,191,135,64,1,0,0,0,128,83,135,64,1,0,0,0,176,152,9,64,1,0,0,0,208,97,135,64,1,0,0,0,176,197,135,64,1,0,0,0,240,219,135,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,112,158,135,64,1,0,0,0,64,169,135,64,1,0,0,0,192,155,135,64,1,0,0,0,112,156,135,64,1,0,0,0,112,162,135,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,148,135,64,1,0,0,0,112,146,135,64,1,0,0,0,34,5,147,25,1,0,0,0,24,255,19,2,0,0,0,0,0,0,0,0,3,0,0,0,220,0,20,2,32,0,0,0,0,0,0,0,1,0,0,0,34,5,147,25,1,0,0,0,240,80,15,2,0,0,0,0],\"hex\":\"60b18b4001000000805a78400100000090688b40010000001000914001000000d0fd8b4001000000408b8740010000002096394001000000108587400100000000808740010000002096394001000000b098094001000000f07f87400100000040c387400100000020b787400100000010b7874001000000307d874001000000b08487400100000090c5874001000000d084874001000000d0c5874001000000107f8740010000003085874001000000307e87400100000020bf8740010000008053874001000000b098094001000000d061874001000000b0c5874001000000f0db874001000000b098094001000000b098094001000000709e87400100000040a9874001000000c09b874001000000709c87400100000070a2874001000000b098094001000000b098094001000000b098094001000000b0948740010000007092874001000000220593190100000018ff1302000000000000000003000000dc0014022000000000000000010000002205931901000000f0500f0200000000\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"address\":\"141c5e258\",\"length\":384,\"data\":[96,177,139,64,1,0,0,0,128,90,120,64,1,0,0,0,144,104,139,64,1,0,0,0,16,0,145,64,1,0,0,0,208,253,139,64,1,0,0,0,64,139,135,64,1,0,0,0,32,150,57,64,1,0,0,0,16,133,135,64,1,0,0,0,0,128,135,64,1,0,0,0,32,150,57,64,1,0,0,0,176,152,9,64,1,0,0,0,240,127,135,64,1,0,0,0,64,195,135,64,1,0,0,0,32,183,135,64,1,0,0,0,16,183,135,64,1,0,0,0,48,125,135,64,1,0,0,0,176,132,135,64,1,0,0,0,144,197,135,64,1,0,0,0,208,132,135,64,1,0,0,0,208,197,135,64,1,0,0,0,16,127,135,64,1,0,0,0,48,133,135,64,1,0,0,0,48,126,135,64,1,0,0,0,32,191,135,64,1,0,0,0,128,83,135,64,1,0,0,0,176,152,9,64,1,0,0,0,208,97,135,64,1,0,0,0,176,197,135,64,1,0,0,0,240,219,135,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,112,158,135,64,1,0,0,0,64,169,135,64,1,0,0,0,192,155,135,64,1,0,0,0,112,156,135,64,1,0,0,0,112,162,135,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,148,135,64,1,0,0,0,112,146,135,64,1,0,0,0,34,5,147,25,1,0,0,0,24,255,19,2,0,0,0,0,0,0,0,0,3,0,0,0,220,0,20,2,32,0,0,0,0,0,0,0,1,0,0,0,34,5,147,25,1,0,0,0,240,80,15,2,0,0,0,0],\"hex\":\"60b18b4001000000805a78400100000090688b40010000001000914001000000d0fd8b4001000000408b8740010000002096394001000000108587400100000000808740010000002096394001000000b098094001000000f07f87400100000040c387400100000020b787400100000010b7874001000000307d874001000000b08487400100000090c5874001000000d084874001000000d0c5874001000000107f8740010000003085874001000000307e87400100000020bf8740010000008053874001000000b098094001000000d061874001000000b0c5874001000000f0db874001000000b098094001000000b098094001000000709e87400100000040a9874001000000c09b874001000000709c87400100000070a2874001000000b098094001000000b098094001000000b098094001000000b0948740010000007092874001000000220593190100000018ff1302000000000000000003000000dc0014022000000000000000010000002205931901000000f0500f0200000000\"}"
      },
      "isError": false
    }
  ],
  "functions": [
    {
      "address": "1407456f0",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_1407456f0(longlong param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  uint uVar3;\r\n  ulonglong uVar4;\r\n  int iVar5;\r\n  uint uVar6;\r\n  ulonglong uVar7;\r\n  longlong lVar8;\r\n  ulonglong uVar9;\r\n  longlong lVar10;\r\n  uint auStackX_10 [2];\r\n  \r\n  if (*(char *)(param_2 + 1) == '\\0') {\r\n    uVar4 = FUN_1400f13f0(0x10);\r\n    uVar7 = 0;\r\n    uVar9 = uVar7;\r\n    if (uVar4 != 0) {\r\n      FUN_140500c30(uVar4);\r\n      uVar9 = uVar4;\r\n    }\r\n    *(ulonglong *)(param_1 + 0x20) = uVar9;\r\n    (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,auStackX_10,4);\r\n    uVar6 = auStackX_10[0];\r\n    lVar10 = *(longlong *)(param_1 + 0x20);\r\n    if (((int)auStackX_10[0] <= (int)(*(uint *)(lVar10 + 0xc) >> 8)) ||\r\n       (cVar2 = FUN_140745470(lVar10,auStackX_10[0]), cVar2 != '\\0')) {\r\n      uVar3 = *(uint *)(lVar10 + 0xc) >> 8;\r\n      *(uint *)(lVar10 + 8) = *(uint *)(lVar10 + 8) & 0xff000000;\r\n      if ((int)uVar6 < (int)uVar3) {\r\n        uVar3 = uVar6;\r\n      }\r\n      *(uint *)(lVar10 + 8) = *(uint *)(lVar10 + 8) | uVar3 & 0xffffff;\r\n    }\r\n    if (0 < (int)auStackX_10[0]) {\r\n      do {\r\n        lVar10 = **(longlong **)(param_1 + 0x20) + (longlong)(int)uVar7 * 0x18;\r\n        lVar1 = *(longlong *)*param_2;\r\n        if (*(char *)(param_2 + 1) == '\\0') {\r\n          (**(code **)(lVar1 + 0x28))((longlong *)*param_2,lVar10,4);\r\n        }\r\n        else {\r\n          (**(code **)(lVar1 + 0x30))();\r\n        }\r\n        FUN_140131df0(param_2,lVar10 + 8);\r\n        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,lVar10 + 0x10,1);\r\n        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,lVar10 + 0x11,1);\r\n        uVar6 = (int)uVar7 + 1;\r\n        uVar7 = (ulonglong)uVar6;\r\n      } while ((int)uVar6 < (int)auStackX_10[0]);\r\n    }\r\n  }\r\n  else {\r\n    auStackX_10[0] = *(uint *)(param_1 + 0x30) & 0xffffff;\r\n    (**(code **)(*(longlong *)*param_2 + 0x30))\r\n              ((longlong *)*param_2,auStackX_10,4,param_4,0xfffffffffffffffe);\r\n    iVar5 = 0;\r\n    if ((*(uint *)(param_1 + 0x30) & 0xffffff) != 0) {\r\n      lVar10 = 0;\r\n      do {\r\n        lVar8 = *(longlong *)(param_1 + 0x28) + lVar10;\r\n        lVar1 = *(longlong *)*param_2;\r\n        if (*(char *)(param_2 + 1) == '\\0') {\r\n          (**(code **)(lVar1 + 0x28))((longlong *)*param_2,lVar8,4);\r\n        }\r\n        else {\r\n          (**(code **)(lVar1 + 0x30))();\r\n        }\r\n        FUN_140131fb0(param_2,*(undefined8 *)(lVar8 + 8));\r\n        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,lVar8 + 0x10,1);\r\n        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,lVar8 + 0x11,1);\r\n        iVar5 = iVar5 + 1;\r\n        lVar10 = lVar10 + 0x18;\r\n      } while (iVar5 < (int)(*(uint *)(param_1 + 0x30) & 0xffffff));\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_1407456f0(longlong param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  uint uVar3;\r\n  ulonglong uVar4;\r\n  int iVar5;\r\n  uint uVar6;\r\n  ulonglong uVar7;\r\n  longlong lVar8;\r\n  ulonglong uVar9;\r\n  longlong lVar10;\r\n  uint auStackX_10 [2];\r\n  \r\n  if (*(char *)(param_2 + 1) == '\\0') {\r\n    uVar4 = FUN_1400f13f0(0x10);\r\n    uVar7 = 0;\r\n    uVar9 = uVar7;\r\n    if (uVar4 != 0) {\r\n      FUN_140500c30(uVar4);\r\n      uVar9 = uVar4;\r\n    }\r\n    *(ulonglong *)(param_1 + 0x20) = uVar9;\r\n    (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,auStackX_10,4);\r\n    uVar6 = auStackX_10[0];\r\n    lVar10 = *(longlong *)(param_1 + 0x20);\r\n    if (((int)auStackX_10[0] <= (int)(*(uint *)(lVar10 + 0xc) >> 8)) ||\r\n       (cVar2 = FUN_140745470(lVar10,auStackX_10[0]), cVar2 != '\\0')) {\r\n      uVar3 = *(uint *)(lVar10 + 0xc) >> 8;\r\n      *(uint *)(lVar10 + 8) = *(uint *)(lVar10 + 8) & 0xff000000;\r\n      if ((int)uVar6 < (int)uVar3) {\r\n        uVar3 = uVar6;\r\n      }\r\n      *(uint *)(lVar10 + 8) = *(uint *)(lVar10 + 8) | uVar3 & 0xffffff;\r\n    }\r\n    if (0 < (int)auStackX_10[0]) {\r\n      do {\r\n        lVar10 = **(longlong **)(param_1 + 0x20) + (longlong)(int)uVar7 * 0x18;\r\n        lVar1 = *(longlong *)*param_2;\r\n        if (*(char *)(param_2 + 1) == '\\0') {\r\n          (**(code **)(lVar1 + 0x28))((longlong *)*param_2,lVar10,4);\r\n        }\r\n        else {\r\n          (**(code **)(lVar1 + 0x30))();\r\n        }\r\n        FUN_140131df0(param_2,lVar10 + 8);\r\n        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,lVar10 + 0x10,1);\r\n        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,lVar10 + 0x11,1);\r\n        uVar6 = (int)uVar7 + 1;\r\n        uVar7 = (ulonglong)uVar6;\r\n      } while ((int)uVar6 < (int)auStackX_10[0]);\r\n    }\r\n  }\r\n  else {\r\n    auStackX_10[0] = *(uint *)(param_1 + 0x30) & 0xffffff;\r\n    (**(code **)(*(longlong *)*param_2 + 0x30))\r\n              ((longlong *)*param_2,auStackX_10,4,param_4,0xfffffffffffffffe);\r\n    iVar5 = 0;\r\n    if ((*(uint *)(param_1 + 0x30) & 0xffffff) != 0) {\r\n      lVar10 = 0;\r\n      do {\r\n        lVar8 = *(longlong *)(param_1 + 0x28) + lVar10;\r\n        lVar1 = *(longlong *)*param_2;\r\n        if (*(char *)(param_2 + 1) == '\\0') {\r\n          (**(code **)(lVar1 + 0x28))((longlong *)*param_2,lVar8,4);\r\n        }\r\n        else {\r\n          (**(code **)(lVar1 + 0x30))();\r\n        }\r\n        FUN_140131fb0(param_2,*(undefined8 *)(lVar8 + 8));\r\n        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,lVar8 + 0x10,1);\r\n        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,lVar8 + 0x11,1);\r\n        iVar5 = iVar5 + 1;\r\n        lVar10 = lVar10 + 0x18;\r\n      } while (iVar5 < (int)(*(uint *)(param_1 + 0x30) & 0xffffff));\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076ecf0",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076ecf0(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  byte local_res10 [24];\r\n  \r\n  FUN_1405ed5e0();\r\n  if (*(char *)(param_2 + 1) != '\\0') {\r\n    local_res10[0] = *(byte *)(param_1 + 0xd0) >> 2 & 2 | *(byte *)(param_1 + 0xd0) & 1;\r\n    (**(code **)(*(longlong *)*param_2 + 0x30))();\r\n    return;\r\n  }\r\n  local_res10[0] = 0;\r\n  (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,local_res10);\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xf6;\r\n  *(byte *)(param_1 + 0xd0) =\r\n       *(byte *)(param_1 + 0xd0) | (local_res10[0] & 2) << 2 | local_res10[0] & 1;\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076ecf0(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  byte local_res10 [24];\r\n  \r\n  FUN_1405ed5e0();\r\n  if (*(char *)(param_2 + 1) != '\\0') {\r\n    local_res10[0] = *(byte *)(param_1 + 0xd0) >> 2 & 2 | *(byte *)(param_1 + 0xd0) & 1;\r\n    (**(code **)(*(longlong *)*param_2 + 0x30))();\r\n    return;\r\n  }\r\n  local_res10[0] = 0;\r\n  (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,local_res10);\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xf6;\r\n  *(byte *)(param_1 + 0xd0) =\r\n       *(byte *)(param_1 + 0xd0) | (local_res10[0] & 2) << 2 | local_res10[0] & 1;\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076e750",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076e750(longlong param_1,undefined8 param_2,undefined8 *param_3,undefined8 *param_4)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  undefined4 *puVar3;\r\n  \r\n  *param_3 = 0;\r\n  *param_4 = 0;\r\n  if ((*(byte *)(param_1 + 0xd0) & 2) == 0) {\r\n    lVar2 = FUN_140877530(*(undefined8 *)(param_1 + 0x10));\r\n    if ((lVar2 != 0) && (cVar1 = FUN_1406dd500(lVar2), cVar1 != '\\0')) {\r\n      return;\r\n    }\r\n    lVar2 = *(longlong *)(param_1 + 8);\r\n    puVar3 = (undefined4 *)(param_1 + 0xd4);\r\n    FUN_140681fb0(puVar3,lVar2 + 200);\r\n    lVar2 = *(longlong *)(lVar2 + 0xf8);\r\n    if ((lVar2 != 0) && ((*(int *)(lVar2 + 0x60) == 0x19 || (*(int *)(lVar2 + 0xc4) != -1)))) {\r\n      *puVar3 = *(undefined4 *)(lVar2 + 100);\r\n    }\r\n    *param_3 = puVar3;\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076e750(longlong param_1,undefined8 param_2,undefined8 *param_3,undefined8 *param_4)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  undefined4 *puVar3;\r\n  \r\n  *param_3 = 0;\r\n  *param_4 = 0;\r\n  if ((*(byte *)(param_1 + 0xd0) & 2) == 0) {\r\n    lVar2 = FUN_140877530(*(undefined8 *)(param_1 + 0x10));\r\n    if ((lVar2 != 0) && (cVar1 = FUN_1406dd500(lVar2), cVar1 != '\\0')) {\r\n      return;\r\n    }\r\n    lVar2 = *(longlong *)(param_1 + 8);\r\n    puVar3 = (undefined4 *)(param_1 + 0xd4);\r\n    FUN_140681fb0(puVar3,lVar2 + 200);\r\n    lVar2 = *(longlong *)(lVar2 + 0xf8);\r\n    if ((lVar2 != 0) && ((*(int *)(lVar2 + 0x60) == 0x19 || (*(int *)(lVar2 + 0xc4) != -1)))) {\r\n      *puVar3 = *(undefined4 *)(lVar2 + 100);\r\n    }\r\n    *param_3 = puVar3;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "140744210",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_140744210(undefined8 param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  \r\n  lVar3 = FUN_1407e7380();\r\n  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  }\r\n  FUN_14048ff20(*(undefined8 *)(lVar3 + 0x7b0),param_1);\r\n  FUN_1401581e0(lVar3 + 0xc0c,0);\r\n  uVar1 = FUN_1407e7380();\r\n  FUN_1407ec5a0(uVar1,param_1);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_140744210(undefined8 param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  \r\n  lVar3 = FUN_1407e7380();\r\n  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  }\r\n  FUN_14048ff20(*(undefined8 *)(lVar3 + 0x7b0),param_1);\r\n  FUN_1401581e0(lVar3 + 0xc0c,0);\r\n  uVar1 = FUN_1407e7380();\r\n  FUN_1407ec5a0(uVar1,param_1);\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076ed80",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076ed80(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  int iVar1;\r\n  int *piVar2;\r\n  longlong lVar3;\r\n  bool abStackX_8 [8];\r\n  \r\n  FUN_1403da950(param_1 + 0x30);\r\n  lVar3 = 0;\r\n  if (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) {\r\n    lVar3 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x80);\r\n  }\r\n  abStackX_8[0] = false;\r\n  if (*(char *)(param_2 + 1) == '\\0') {\r\n    (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,abStackX_8,1);\r\n    if ((abStackX_8[0] != false) && (lVar3 != 0)) {\r\n      FUN_1406709a0(lVar3,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8));\r\n    }\r\n    return;\r\n  }\r\n  if (lVar3 != 0) {\r\n    iVar1 = *(int *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    piVar2 = (int *)FUN_14066dd00(lVar3);\r\n    abStackX_8[0] = *piVar2 == iVar1;\r\n  }\r\n  lVar3 = *(longlong *)*param_2;\r\n  if (*(char *)(param_2 + 1) != '\\0') {\r\n    (**(code **)(lVar3 + 0x30))();\r\n    return;\r\n  }\r\n  (**(code **)(lVar3 + 0x28))((longlong *)*param_2,abStackX_8,1);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076ed80(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  int iVar1;\r\n  int *piVar2;\r\n  longlong lVar3;\r\n  bool abStackX_8 [8];\r\n  \r\n  FUN_1403da950(param_1 + 0x30);\r\n  lVar3 = 0;\r\n  if (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) {\r\n    lVar3 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x80);\r\n  }\r\n  abStackX_8[0] = false;\r\n  if (*(char *)(param_2 + 1) == '\\0') {\r\n    (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,abStackX_8,1);\r\n    if ((abStackX_8[0] != false) && (lVar3 != 0)) {\r\n      FUN_1406709a0(lVar3,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8));\r\n    }\r\n    return;\r\n  }\r\n  if (lVar3 != 0) {\r\n    iVar1 = *(int *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    piVar2 = (int *)FUN_14066dd00(lVar3);\r\n    abStackX_8[0] = *piVar2 == iVar1;\r\n  }\r\n  lVar3 = *(longlong *)*param_2;\r\n  if (*(char *)(param_2 + 1) != '\\0') {\r\n    (**(code **)(lVar3 + 0x30))();\r\n    return;\r\n  }\r\n  (**(code **)(lVar3 + 0x28))((longlong *)*param_2,abStackX_8,1);\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076f020",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076f020(longlong param_1,byte param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  longlong *plVar2;\r\n  char cVar3;\r\n  longlong lVar4;\r\n  byte bVar5;\r\n  \r\n  bVar5 = 1;\r\n  lVar4 = FUN_1403da910(param_1 + 0x30,1);\r\n  param_2 = param_2 & lVar4 == 0;\r\n  lVar4 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);\r\n  if ((lVar4 == 0) || (*(int *)(lVar4 + 0x60) != 0x1a)) goto LAB_14076f0bd;\r\n  lVar4 = FUN_1403da910(param_1 + 0x70,2);\r\n  if (lVar4 == 0) {\r\nLAB_14076f080:\r\n    bVar5 = 0;\r\n  }\r\n  else {\r\n    cVar3 = FUN_1404c1f20(lVar4);\r\n    if (cVar3 == '\\0') goto LAB_14076f080;\r\n  }\r\n  param_2 = param_2 & bVar5;\r\n  if (param_2 == 0) {\r\n    FUN_14087b6f0(*(undefined8 *)(param_1 + 0x10),DAT_14231ed30);\r\n    FUN_140235990(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0xf0));\r\n    plVar2 = *(longlong **)(*(longlong *)(param_1 + 0x10) + 0xf0);\r\n    (**(code **)(*plVar2 + 0x28))(plVar2,4);\r\n  }\r\nLAB_14076f0bd:\r\n  lVar4 = *(longlong *)(param_1 + 0x10);\r\n  pbVar1 = (byte *)(lVar4 + 0xd7);\r\n  *pbVar1 = *pbVar1 & 0xfe;\r\n  pbVar1 = (byte *)(lVar4 + 0xd7);\r\n  *pbVar1 = *pbVar1 | param_2;\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076f020(longlong param_1,byte param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  longlong *plVar2;\r\n  char cVar3;\r\n  longlong lVar4;\r\n  byte bVar5;\r\n  \r\n  bVar5 = 1;\r\n  lVar4 = FUN_1403da910(param_1 + 0x30,1);\r\n  param_2 = param_2 & lVar4 == 0;\r\n  lVar4 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);\r\n  if ((lVar4 == 0) || (*(int *)(lVar4 + 0x60) != 0x1a)) goto LAB_14076f0bd;\r\n  lVar4 = FUN_1403da910(param_1 + 0x70,2);\r\n  if (lVar4 == 0) {\r\nLAB_14076f080:\r\n    bVar5 = 0;\r\n  }\r\n  else {\r\n    cVar3 = FUN_1404c1f20(lVar4);\r\n    if (cVar3 == '\\0') goto LAB_14076f080;\r\n  }\r\n  param_2 = param_2 & bVar5;\r\n  if (param_2 == 0) {\r\n    FUN_14087b6f0(*(undefined8 *)(param_1 + 0x10),DAT_14231ed30);\r\n    FUN_140235990(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0xf0));\r\n    plVar2 = *(longlong **)(*(longlong *)(param_1 + 0x10) + 0xf0);\r\n    (**(code **)(*plVar2 + 0x28))(plVar2,4);\r\n  }\r\nLAB_14076f0bd:\r\n  lVar4 = *(longlong *)(param_1 + 0x10);\r\n  pbVar1 = (byte *)(lVar4 + 0xd7);\r\n  *pbVar1 = *pbVar1 & 0xfe;\r\n  pbVar1 = (byte *)(lVar4 + 0xd7);\r\n  *pbVar1 = *pbVar1 | param_2;\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "140744b30",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_140744b30(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  undefined8 uVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  longlong *plVar6;\r\n  uint uVar7;\r\n  undefined4 *puVar8;\r\n  int iVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  longlong lStackX_10;\r\n  undefined1 auStack_88 [24];\r\n  undefined4 auStack_70 [2];\r\n  undefined1 auStack_68 [40];\r\n  undefined4 uStack_40;\r\n  \r\n  FUN_1407e7400();\r\n  lVar1 = *(longlong *)(param_1 + 8);\r\n  uVar7 = *(uint *)(lVar1 + 0x70) & 0xffffff;\r\n  lStackX_10 = 0;\r\n  if (*(longlong *)(param_1 + 0x20) == 0) {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));\r\n    lVar4 = FUN_1408dcad0(uVar3);\r\n    if ((((lVar4 != 0) && (lStackX_10 = *(longlong *)(lVar4 + 0x418), lStackX_10 != 0)) &&\r\n        (lVar4 = FUN_1408ae9b0(D2_GameSingletonPtr,lStackX_10,&DAT_1431af668,&DAT_14228a780,\r\n                               *(undefined8 *)(param_1 + 0x10),1,0,0,0), lVar4 != 0)) &&\r\n       (cVar2 = FUN_140743c30(param_1,lVar4,lVar1 + 0x78,0), cVar2 == '\\0')) {\r\n      FUN_1408847f0(lVar4,auStack_88,&DAT_1439a6f30);\r\n      FUN_140882fc0(auStack_88);\r\n    }\r\n  }\r\n  plVar6 = *(longlong **)(param_1 + 0x20);\r\n  iVar9 = 0;\r\n  lVar4 = (longlong)(int)uVar7;\r\n  if (plVar6 == (longlong *)0x0) {\r\n    if (uVar7 != 0) {\r\n      lVar10 = 0;\r\n      do {\r\n        if ((lStackX_10 == 0) ||\r\n           (*(longlong *)(lVar10 + 8 + *(longlong *)(lVar1 + 0x68)) != *(longlong *)(lVar1 + 0x78)))\r\n        {\r\n          lVar11 = FUN_1408ae9b0(D2_GameSingletonPtr,\r\n                                 *(undefined8 *)(lVar10 + *(longlong *)(lVar1 + 0x68)),\r\n                                 &DAT_1431af668,&DAT_14228a780,*(undefined8 *)(param_1 + 0x10),1,0,0\r\n                                 ,0);\r\n          if (lVar11 != 0) {\r\n            lVar5 = FUN_140877720(lVar11,&DAT_1439aea90,0);\r\n            uVar3 = 0;\r\n            if (lVar5 == 0) {\r\nLAB_140744e6c:\r\n              cVar2 = FUN_140743c30(param_1,lVar11,\r\n                                    (longlong)iVar9 * 0x10 + 8 + *(longlong *)(lVar1 + 0x68),uVar3);\r\n              if (cVar2 != '\\0') goto LAB_140744eb0;\r\n            }\r\n            else {\r\n              lVar5 = *(longlong *)(lVar5 + 8);\r\n              cVar2 = FUN_14060b3e0(lVar5 + 0x120);\r\n              if (cVar2 != '\\0') {\r\nLAB_140744e69:\r\n                uVar3 = 1;\r\n                goto LAB_140744e6c;\r\n              }\r\n              uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n              uVar3 = FUN_140877720(uVar3,&DAT_1439c55b0,0);\r\n              auStack_70[0] = *(undefined4 *)(lVar5 + 0x120);\r\n              FUN_14013f490(auStack_68,lVar5 + 0x128);\r\n              uStack_40 = *(undefined4 *)(lVar5 + 0x150);\r\n              cVar2 = FUN_1406101f0(uVar3,auStack_70);\r\n              if (cVar2 == '\\0') goto LAB_140744e69;\r\n            }\r\n            FUN_1408847f0(lVar11,auStack_88,&DAT_1439a6f30);\r\n            FUN_140882fc0(auStack_88);\r\n          }\r\n        }\r\nLAB_140744eb0:\r\n        iVar9 = iVar9 + 1;\r\n        lVar10 = lVar10 + 0x10;\r\n        lVar4 = lVar4 + -1;\r\n      } while (lVar4 != 0);\r\n    }\r\n  }\r\n  else {\r\n    lVar4 = 0;\r\n    if ((*(uint *)(plVar6 + 1) & 0xffffff) != 0) {\r\n      lVar10 = 0;\r\n      lVar11 = 0;\r\n      do {\r\n        puVar8 = (undefined4 *)(*plVar6 + lVar11);\r\n        if ((((*(char *)((longlong)puVar8 + 0x11) == '\\0') &&\r\n             ((lStackX_10 == 0 || (*(longlong *)(puVar8 + 2) != *(longlong *)(lVar1 + 0x78))))) &&\r\n            ((lVar5 = FUN_1406dc560(*puVar8), lVar5 != 0 ||\r\n             ((lVar4 < (int)uVar7 &&\r\n              (lVar5 = FUN_1408ae9b0(D2_GameSingletonPtr,\r\n                                     *(undefined8 *)(lVar10 + *(longlong *)(lVar1 + 0x68)),\r\n                                     &DAT_1431af668,&DAT_14228a780,*(undefined8 *)(param_1 + 0x10),1\r\n                                     ,0,0,0), lVar5 != 0)))))) &&\r\n           (cVar2 = FUN_140743c30(param_1,lVar5,puVar8 + 2,*(undefined1 *)(puVar8 + 4)),\r\n           cVar2 == '\\0')) {\r\n          FUN_1408847f0(lVar5,auStack_88,&DAT_1439a6f30);\r\n          FUN_140882fc0(auStack_88);\r\n        }\r\n        plVar6 = *(longlong **)(param_1 + 0x20);\r\n        iVar9 = iVar9 + 1;\r\n        lVar4 = lVar4 + 1;\r\n        lVar11 = lVar11 + 0x18;\r\n        lVar10 = lVar10 + 0x10;\r\n      } while (iVar9 < (int)(*(uint *)(plVar6 + 1) & 0xffffff));\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_140744b30(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  undefined8 uVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  longlong *plVar6;\r\n  uint uVar7;\r\n  undefined4 *puVar8;\r\n  int iVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  longlong lStackX_10;\r\n  undefined1 auStack_88 [24];\r\n  undefined4 auStack_70 [2];\r\n  undefined1 auStack_68 [40];\r\n  undefined4 uStack_40;\r\n  \r\n  FUN_1407e7400();\r\n  lVar1 = *(longlong *)(param_1 + 8);\r\n  uVar7 = *(uint *)(lVar1 + 0x70) & 0xffffff;\r\n  lStackX_10 = 0;\r\n  if (*(longlong *)(param_1 + 0x20) == 0) {\r\n    uVar3 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));\r\n    lVar4 = FUN_1408dcad0(uVar3);\r\n    if ((((lVar4 != 0) && (lStackX_10 = *(longlong *)(lVar4 + 0x418), lStackX_10 != 0)) &&\r\n        (lVar4 = FUN_1408ae9b0(D2_GameSingletonPtr,lStackX_10,&DAT_1431af668,&DAT_14228a780,\r\n                               *(undefined8 *)(param_1 + 0x10),1,0,0,0), lVar4 != 0)) &&\r\n       (cVar2 = FUN_140743c30(param_1,lVar4,lVar1 + 0x78,0), cVar2 == '\\0')) {\r\n      FUN_1408847f0(lVar4,auStack_88,&DAT_1439a6f30);\r\n      FUN_140882fc0(auStack_88);\r\n    }\r\n  }\r\n  plVar6 = *(longlong **)(param_1 + 0x20);\r\n  iVar9 = 0;\r\n  lVar4 = (longlong)(int)uVar7;\r\n  if (plVar6 == (longlong *)0x0) {\r\n    if (uVar7 != 0) {\r\n      lVar10 = 0;\r\n      do {\r\n        if ((lStackX_10 == 0) ||\r\n           (*(longlong *)(lVar10 + 8 + *(longlong *)(lVar1 + 0x68)) != *(longlong *)(lVar1 + 0x78)))\r\n        {\r\n          lVar11 = FUN_1408ae9b0(D2_GameSingletonPtr,\r\n                                 *(undefined8 *)(lVar10 + *(longlong *)(lVar1 + 0x68)),\r\n                                 &DAT_1431af668,&DAT_14228a780,*(undefined8 *)(param_1 + 0x10),1,0,0\r\n                                 ,0);\r\n          if (lVar11 != 0) {\r\n            lVar5 = FUN_140877720(lVar11,&DAT_1439aea90,0);\r\n            uVar3 = 0;\r\n            if (lVar5 == 0) {\r\nLAB_140744e6c:\r\n              cVar2 = FUN_140743c30(param_1,lVar11,\r\n                                    (longlong)iVar9 * 0x10 + 8 + *(longlong *)(lVar1 + 0x68),uVar3);\r\n              if (cVar2 != '\\0') goto LAB_140744eb0;\r\n            }\r\n            else {\r\n              lVar5 = *(longlong *)(lVar5 + 8);\r\n              cVar2 = FUN_14060b3e0(lVar5 + 0x120);\r\n              if (cVar2 != '\\0') {\r\nLAB_140744e69:\r\n                uVar3 = 1;\r\n                goto LAB_140744e6c;\r\n              }\r\n              uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n              uVar3 = FUN_140877720(uVar3,&DAT_1439c55b0,0);\r\n              auStack_70[0] = *(undefined4 *)(lVar5 + 0x120);\r\n              FUN_14013f490(auStack_68,lVar5 + 0x128);\r\n              uStack_40 = *(undefined4 *)(lVar5 + 0x150);\r\n              cVar2 = FUN_1406101f0(uVar3,auStack_70);\r\n              if (cVar2 == '\\0') goto LAB_140744e69;\r\n            }\r\n            FUN_1408847f0(lVar11,auStack_88,&DAT_1439a6f30);\r\n            FUN_140882fc0(auStack_88);\r\n          }\r\n        }\r\nLAB_140744eb0:\r\n        iVar9 = iVar9 + 1;\r\n        lVar10 = lVar10 + 0x10;\r\n        lVar4 = lVar4 + -1;\r\n      } while (lVar4 != 0);\r\n    }\r\n  }\r\n  else {\r\n    lVar4 = 0;\r\n    if ((*(uint *)(plVar6 + 1) & 0xffffff) != 0) {\r\n      lVar10 = 0;\r\n      lVar11 = 0;\r\n      do {\r\n        puVar8 = (undefined4 *)(*plVar6 + lVar11);\r\n        if ((((*(char *)((longlong)puVar8 + 0x11) == '\\0') &&\r\n             ((lStackX_10 == 0 || (*(longlong *)(puVar8 + 2) != *(longlong *)(lVar1 + 0x78))))) &&\r\n            ((lVar5 = FUN_1406dc560(*puVar8), lVar5 != 0 ||\r\n             ((lVar4 < (int)uVar7 &&\r\n              (lVar5 = FUN_1408ae9b0(D2_GameSingletonPtr,\r\n                                     *(undefined8 *)(lVar10 + *(longlong *)(lVar1 + 0x68)),\r\n                                     &DAT_1431af668,&DAT_14228a780,*(undefined8 *)(param_1 + 0x10),1\r\n                                     ,0,0,0), lVar5 != 0)))))) &&\r\n           (cVar2 = FUN_140743c30(param_1,lVar5,puVar8 + 2,*(undefined1 *)(puVar8 + 4)),\r\n           cVar2 == '\\0')) {\r\n          FUN_1408847f0(lVar5,auStack_88,&DAT_1439a6f30);\r\n          FUN_140882fc0(auStack_88);\r\n        }\r\n        plVar6 = *(longlong **)(param_1 + 0x20);\r\n        iVar9 = iVar9 + 1;\r\n        lVar4 = lVar4 + 1;\r\n        lVar11 = lVar11 + 0x18;\r\n        lVar10 = lVar10 + 0x10;\r\n      } while (iVar9 < (int)(*(uint *)(plVar6 + 1) & 0xffffff));\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14077c600",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14077c600(longlong *param_1,char param_2)\r\n\r\n{\r\n  if ((char)param_1[4] != param_2) {\r\n    (**(code **)(*param_1 + 0xe8))();\r\n    *(char *)(param_1 + 4) = param_2;\r\n    *(byte *)(param_1[2] + 0xd4) = *(byte *)(param_1[2] + 0xd4) | 4;\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14077c600(longlong *param_1,char param_2)\r\n\r\n{\r\n  if ((char)param_1[4] != param_2) {\r\n    (**(code **)(*param_1 + 0xe8))();\r\n    *(char *)(param_1 + 4) = param_2;\r\n    *(byte *)(param_1[2] + 0xd4) = *(byte *)(param_1[2] + 0xd4) | 4;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14077c510",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nbool FUN_14077c510(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  \r\n  cVar1 = FUN_140878c60(*(undefined8 *)(param_1 + 0x10));\r\n  return cVar1 == '\\0';\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nbool FUN_14077c510(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  \r\n  cVar1 = FUN_140878c60(*(undefined8 *)(param_1 + 0x10));\r\n  return cVar1 == '\\0';\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076e670",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076e670(undefined8 param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  \r\n  lVar3 = FUN_1407e7380();\r\n  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  }\r\n  FUN_14048ff20(*(undefined8 *)(lVar3 + 0x5b8),param_1);\r\n  FUN_1401581e0(lVar3 + 0xc0c,0);\r\n  uVar1 = FUN_1407e7380();\r\n  FUN_1407ec5a0(uVar1,param_1);\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076e670(undefined8 param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  \r\n  lVar3 = FUN_1407e7380();\r\n  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  }\r\n  FUN_14048ff20(*(undefined8 *)(lVar3 + 0x5b8),param_1);\r\n  FUN_1401581e0(lVar3 + 0xc0c,0);\r\n  uVar1 = FUN_1407e7380();\r\n  FUN_1407ec5a0(uVar1,param_1);\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076e250",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nlonglong FUN_14076e250(longlong param_1,uint param_2)\r\n\r\n{\r\n  FUN_1404e3050(param_1 + 0xa8);\r\n  FUN_1404e3050(param_1 + 0x80);\r\n  FUN_14077c350(param_1);\r\n  if ((param_2 & 1) != 0) {\r\n    FUN_1400f1520(param_1);\r\n  }\r\n  return param_1;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nlonglong FUN_14076e250(longlong param_1,uint param_2)\r\n\r\n{\r\n  FUN_1404e3050(param_1 + 0xa8);\r\n  FUN_1404e3050(param_1 + 0x80);\r\n  FUN_14077c350(param_1);\r\n  if ((param_2 & 1) != 0) {\r\n    FUN_1400f1520(param_1);\r\n  }\r\n  return param_1;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076e860",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076e860(longlong param_1)\r\n\r\n{\r\n  byte *pbVar1;\r\n  \r\n  FUN_1407e7400();\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd7);\r\n  *pbVar1 = *pbVar1 | 4;\r\n  *(byte *)(*(longlong *)(param_1 + 0x10) + 0xd1) =\r\n       *(char *)(*(longlong *)(param_1 + 8) + 0x118) << 7 |\r\n       *(byte *)(*(longlong *)(param_1 + 0x10) + 0xd1) & 0x7f;\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076e860(longlong param_1)\r\n\r\n{\r\n  byte *pbVar1;\r\n  \r\n  FUN_1407e7400();\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd7);\r\n  *pbVar1 = *pbVar1 | 4;\r\n  *(byte *)(*(longlong *)(param_1 + 0x10) + 0xd1) =\r\n       *(char *)(*(longlong *)(param_1 + 8) + 0x118) << 7 |\r\n       *(byte *)(*(longlong *)(param_1 + 0x10) + 0xd1) & 0x7f;\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076e8b0",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076e8b0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  undefined8 uVar6;\r\n  undefined4 uVar7;\r\n  undefined4 local_res8 [2];\r\n  longlong local_28;\r\n  code *local_20;\r\n  \r\n  uVar6 = *(undefined8 *)(param_1 + 0x10);\r\n  lVar4 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  uVar7 = 1;\r\n  uVar5 = 0;\r\n  if (lVar4 != 0) {\r\n    uVar5 = FUN_140877720(lVar4,&DAT_1439b9e90,1);\r\n  }\r\n  FUN_1403dab80(param_1 + 0x40,uVar5);\r\n  uVar5 = FUN_140877720(uVar6,&DAT_1439e85b0,1);\r\n  FUN_1403dab80(param_1 + 0x50,uVar5);\r\n  uVar6 = FUN_140877720(uVar6,&DAT_1439b4630,1);\r\n  FUN_1403dab80(param_1 + 0x60,uVar6);\r\n  if (lVar4 == 0) {\r\n    uVar6 = 0;\r\n  }\r\n  else {\r\n    uVar6 = FUN_140877720(lVar4,&DAT_1439d15f0,2);\r\n  }\r\n  FUN_1403dab80(param_1 + 0x70,uVar6);\r\n  FUN_14077c530(param_1);\r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  lVar3 = *(longlong *)(lVar2 + 0xf8);\r\n  if (lVar3 != 0) {\r\n    if (((*(int *)(lVar3 + 0x60) - 5U & 0xfffffffc) == 0) && (*(int *)(lVar3 + 0x60) != 6)) {\r\n      if (lVar4 != 0) {\r\n        uVar6 = FUN_140877720(lVar4,&DAT_1439e86d0,0);\r\n        uVar6 = FUN_140777b90(uVar6,0x14);\r\n        iVar1 = *(int *)(lVar3 + 0x60);\r\n        if (iVar1 != 5) {\r\n          if (iVar1 == 7) {\r\n            uVar7 = 0;\r\n          }\r\n          else if (iVar1 == 8) {\r\n            uVar7 = 2;\r\n          }\r\n        }\r\n        if (*(char *)(lVar2 + 0x119) == '\\0') {\r\n          local_res8[0] = *(undefined4 *)(lVar3 + 100);\r\n          FUN_1403c4850(uVar6,*(undefined8 *)(param_1 + 0x10),uVar7,local_res8);\r\n        }\r\n      }\r\n      local_20 = FUN_14076f140;\r\n      local_28 = param_1;\r\n      uVar6 = FUN_140810240();\r\n      FUN_1408106f0(uVar6,0x123,&local_28);\r\n      local_20 = (code *)&LAB_14076f180;\r\n      local_28 = param_1;\r\n      uVar6 = FUN_140810240();\r\n      FUN_1408106f0(uVar6,0x124,&local_28);\r\n      if (*(longlong *)(lVar2 + 0x108) != 0) {\r\n        FUN_1404e2940(param_1 + 0x80,*(longlong *)(lVar2 + 0x108),*(undefined8 *)(param_1 + 0x10),0,\r\n                      0,0,0);\r\n      }\r\n    }\r\n    if ((*(uint *)(param_1 + 0x1c) >> 0x1e & 1) == 0) {\r\n      FUN_14076e4a0(param_1,lVar3);\r\n    }\r\n  }\r\n  if (((*(uint *)(param_1 + 0x1c) >> 0x1e & 1) != 0) && ((*(byte *)(param_1 + 0xd0) & 8) != 0)) {\r\n    FUN_1407e79a0(param_1);\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076e8b0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  undefined8 uVar6;\r\n  undefined4 uVar7;\r\n  undefined4 local_res8 [2];\r\n  longlong local_28;\r\n  code *local_20;\r\n  \r\n  uVar6 = *(undefined8 *)(param_1 + 0x10);\r\n  lVar4 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  uVar7 = 1;\r\n  uVar5 = 0;\r\n  if (lVar4 != 0) {\r\n    uVar5 = FUN_140877720(lVar4,&DAT_1439b9e90,1);\r\n  }\r\n  FUN_1403dab80(param_1 + 0x40,uVar5);\r\n  uVar5 = FUN_140877720(uVar6,&DAT_1439e85b0,1);\r\n  FUN_1403dab80(param_1 + 0x50,uVar5);\r\n  uVar6 = FUN_140877720(uVar6,&DAT_1439b4630,1);\r\n  FUN_1403dab80(param_1 + 0x60,uVar6);\r\n  if (lVar4 == 0) {\r\n    uVar6 = 0;\r\n  }\r\n  else {\r\n    uVar6 = FUN_140877720(lVar4,&DAT_1439d15f0,2);\r\n  }\r\n  FUN_1403dab80(param_1 + 0x70,uVar6);\r\n  FUN_14077c530(param_1);\r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  lVar3 = *(longlong *)(lVar2 + 0xf8);\r\n  if (lVar3 != 0) {\r\n    if (((*(int *)(lVar3 + 0x60) - 5U & 0xfffffffc) == 0) && (*(int *)(lVar3 + 0x60) != 6)) {\r\n      if (lVar4 != 0) {\r\n        uVar6 = FUN_140877720(lVar4,&DAT_1439e86d0,0);\r\n        uVar6 = FUN_140777b90(uVar6,0x14);\r\n        iVar1 = *(int *)(lVar3 + 0x60);\r\n        if (iVar1 != 5) {\r\n          if (iVar1 == 7) {\r\n            uVar7 = 0;\r\n          }\r\n          else if (iVar1 == 8) {\r\n            uVar7 = 2;\r\n          }\r\n        }\r\n        if (*(char *)(lVar2 + 0x119) == '\\0') {\r\n          local_res8[0] = *(undefined4 *)(lVar3 + 100);\r\n          FUN_1403c4850(uVar6,*(undefined8 *)(param_1 + 0x10),uVar7,local_res8);\r\n        }\r\n      }\r\n      local_20 = FUN_14076f140;\r\n      local_28 = param_1;\r\n      uVar6 = FUN_140810240();\r\n      FUN_1408106f0(uVar6,0x123,&local_28);\r\n      local_20 = (code *)&LAB_14076f180;\r\n      local_28 = param_1;\r\n      uVar6 = FUN_140810240();\r\n      FUN_1408106f0(uVar6,0x124,&local_28);\r\n      if (*(longlong *)(lVar2 + 0x108) != 0) {\r\n        FUN_1404e2940(param_1 + 0x80,*(longlong *)(lVar2 + 0x108),*(undefined8 *)(param_1 + 0x10),0,\r\n                      0,0,0);\r\n      }\r\n    }\r\n    if ((*(uint *)(param_1 + 0x1c) >> 0x1e & 1) == 0) {\r\n      FUN_14076e4a0(param_1,lVar3);\r\n    }\r\n  }\r\n  if (((*(uint *)(param_1 + 0x1c) >> 0x1e & 1) != 0) && ((*(byte *)(param_1 + 0xd0) & 8) != 0)) {\r\n    FUN_1407e79a0(param_1);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14076eaf0",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14076eaf0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  longlong lStack_18;\r\n  code *pcStack_10;\r\n  \r\n  FUN_14077c580();\r\n  FUN_1403dab80(param_1 + 0x40,0);\r\n  FUN_1403dab80(param_1 + 0x50,0);\r\n  FUN_1403dab80(param_1 + 0x60,0);\r\n  FUN_1403dab80(param_1 + 0x70,0);\r\n  lVar3 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);\r\n  if (lVar3 != 0) {\r\n    iVar1 = *(int *)(lVar3 + 0x60);\r\n    if (((iVar1 - 5U & 0xfffffffc) == 0) && (iVar1 != 6)) {\r\n      pcStack_10 = FUN_14076f140;\r\n      lStack_18 = param_1;\r\n      uVar2 = FUN_140810240();\r\n      FUN_140811040(uVar2,0x123,&lStack_18);\r\n      pcStack_10 = (code *)&LAB_14076f180;\r\n      lStack_18 = param_1;\r\n      uVar2 = FUN_140810240();\r\n      FUN_140811040(uVar2,0x124,&lStack_18);\r\n      lVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n      if (lVar3 != 0) {\r\n        uVar2 = FUN_140877720(lVar3,&DAT_1439e86d0,0);\r\n        uVar2 = FUN_140777b90(uVar2,0x14);\r\n        FUN_1403c4af0(uVar2,*(undefined8 *)(param_1 + 0x10));\r\n      }\r\n      FUN_1404e3050(param_1 + 0x80);\r\n      FUN_1404e3050(param_1 + 0xa8);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14076eaf0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  longlong lStack_18;\r\n  code *pcStack_10;\r\n  \r\n  FUN_14077c580();\r\n  FUN_1403dab80(param_1 + 0x40,0);\r\n  FUN_1403dab80(param_1 + 0x50,0);\r\n  FUN_1403dab80(param_1 + 0x60,0);\r\n  FUN_1403dab80(param_1 + 0x70,0);\r\n  lVar3 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);\r\n  if (lVar3 != 0) {\r\n    iVar1 = *(int *)(lVar3 + 0x60);\r\n    if (((iVar1 - 5U & 0xfffffffc) == 0) && (iVar1 != 6)) {\r\n      pcStack_10 = FUN_14076f140;\r\n      lStack_18 = param_1;\r\n      uVar2 = FUN_140810240();\r\n      FUN_140811040(uVar2,0x123,&lStack_18);\r\n      pcStack_10 = (code *)&LAB_14076f180;\r\n      lStack_18 = param_1;\r\n      uVar2 = FUN_140810240();\r\n      FUN_140811040(uVar2,0x124,&lStack_18);\r\n      lVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n      if (lVar3 != 0) {\r\n        uVar2 = FUN_140877720(lVar3,&DAT_1439e86d0,0);\r\n        uVar2 = FUN_140777b90(uVar2,0x14);\r\n        FUN_1403c4af0(uVar2,*(undefined8 *)(param_1 + 0x10));\r\n      }\r\n      FUN_1404e3050(param_1 + 0x80);\r\n      FUN_1404e3050(param_1 + 0xa8);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    },
    {
      "address": "14077b9a0",
      "result": {
        "content": [
          {
            "type": "text",
            "text": "\r\nvoid FUN_14077b9a0(longlong param_1,char param_2)\r\n\r\n{\r\n  uint uVar1;\r\n  longlong lVar2;\r\n  uint uVar3;\r\n  longlong lVar4;\r\n  \r\n  FUN_14077c600();\r\n  if ((*(char *)(*(longlong *)(param_1 + 8) + 0x84) != '\\0') &&\r\n     (uVar1 = *(uint *)(param_1 + 0x48), (uVar1 & 0xffffff) != 0xffffff)) {\r\n    lVar2 = *(longlong *)(D2_GameSingletonPtr + 0x60410);\r\n    uVar3 = uVar1 & 0xffffff;\r\n    if (param_2 == '\\0') {\r\n      if ((((uVar3 != 0xffffff) && (uVar3 < *(uint *)(lVar2 + 0x30))) &&\r\n          (lVar4 = (longlong)(int)uVar3 * 0xb0 + *(longlong *)(lVar2 + 0x28),\r\n          (*(byte *)(lVar4 + 0x44) & 3) != 0)) &&\r\n         ((*(uint *)(lVar4 + 0x70) == uVar1 &&\r\n          (*(int *)((ulonglong)(uVar1 & 0xffffff) * 0xb0 + 0x78 + *(longlong *)(lVar2 + 0x28)) != -1\r\n          )))) {\r\n        FUN_140c8f080(lVar2,param_1 + 0x48,1);\r\n      }\r\n    }\r\n    else if (((uVar3 == 0xffffff) || (*(uint *)(lVar2 + 0x30) <= uVar3)) ||\r\n            ((lVar4 = (longlong)(int)uVar3 * 0xb0 + *(longlong *)(lVar2 + 0x28),\r\n             (*(byte *)(lVar4 + 0x44) & 3) == 0 ||\r\n             ((*(uint *)(lVar4 + 0x70) != uVar1 ||\r\n              (*(int *)((ulonglong)(uVar1 & 0xffffff) * 0xb0 + 0x78 + *(longlong *)(lVar2 + 0x28))\r\n               == -1)))))) {\r\n      FUN_140c8e930(lVar2,param_1 + 0x48,1,1,1);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nvoid FUN_14077b9a0(longlong param_1,char param_2)\r\n\r\n{\r\n  uint uVar1;\r\n  longlong lVar2;\r\n  uint uVar3;\r\n  longlong lVar4;\r\n  \r\n  FUN_14077c600();\r\n  if ((*(char *)(*(longlong *)(param_1 + 8) + 0x84) != '\\0') &&\r\n     (uVar1 = *(uint *)(param_1 + 0x48), (uVar1 & 0xffffff) != 0xffffff)) {\r\n    lVar2 = *(longlong *)(D2_GameSingletonPtr + 0x60410);\r\n    uVar3 = uVar1 & 0xffffff;\r\n    if (param_2 == '\\0') {\r\n      if ((((uVar3 != 0xffffff) && (uVar3 < *(uint *)(lVar2 + 0x30))) &&\r\n          (lVar4 = (longlong)(int)uVar3 * 0xb0 + *(longlong *)(lVar2 + 0x28),\r\n          (*(byte *)(lVar4 + 0x44) & 3) != 0)) &&\r\n         ((*(uint *)(lVar4 + 0x70) == uVar1 &&\r\n          (*(int *)((ulonglong)(uVar1 & 0xffffff) * 0xb0 + 0x78 + *(longlong *)(lVar2 + 0x28)) != -1\r\n          )))) {\r\n        FUN_140c8f080(lVar2,param_1 + 0x48,1);\r\n      }\r\n    }\r\n    else if (((uVar3 == 0xffffff) || (*(uint *)(lVar2 + 0x30) <= uVar3)) ||\r\n            ((lVar4 = (longlong)(int)uVar3 * 0xb0 + *(longlong *)(lVar2 + 0x28),\r\n             (*(byte *)(lVar4 + 0x44) & 3) == 0 ||\r\n             ((*(uint *)(lVar4 + 0x70) != uVar1 ||\r\n              (*(int *)((ulonglong)(uVar1 & 0xffffff) * 0xb0 + 0x78 + *(longlong *)(lVar2 + 0x28))\r\n               == -1)))))) {\r\n      FUN_140c8e930(lVar2,param_1 + 0x48,1,1,1);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        },
        "isError": false
      }
    }
  ],
  "quantityXrefs": {
    "content": [
      {
        "type": "text",
        "text": "From 143f1a818 [DATA]\nFrom 140617a76 [UNCONDITIONAL_CALL]\nFrom 140617b06 [UNCONDITIONAL_CALL]\nFrom 140614056 in FUN_140613ff0 [UNCONDITIONAL_CALL]\nFrom 140534d63 [UNCONDITIONAL_CALL]\nFrom 1404ba864 in FUN_1404ba810 [UNCONDITIONAL_CALL]"
      }
    ],
    "structuredContent": {
      "result": "From 143f1a818 [DATA]\nFrom 140617a76 [UNCONDITIONAL_CALL]\nFrom 140617b06 [UNCONDITIONAL_CALL]\nFrom 140614056 in FUN_140613ff0 [UNCONDITIONAL_CALL]\nFrom 140534d63 [UNCONDITIONAL_CALL]\nFrom 1404ba864 in FUN_1404ba810 [UNCONDITIONAL_CALL]"
    },
    "isError": false
  },
  "pickableStatsInitializer": {
    "content": [
      {
        "type": "text",
        "text": "\r\nvoid FUN_14076e4a0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  undefined4 uVar7;\r\n  undefined8 uVar8;\r\n  \r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  lVar6 = D2_GameSingletonPtr + 0x1f77c8;\r\n  lVar4 = FUN_1403da910(param_1 + 0x70,2);\r\n  switch(*(undefined4 *)(param_2 + 0x60)) {\r\n  case 1:\r\n    uVar5 = 0x18;\r\n    break;\r\n  default:\r\n    goto switchD_14076e509_caseD_2;\r\n  case 5:\r\n    uVar5 = 0x19;\r\n    break;\r\n  case 7:\r\n    if ((lVar4 != 0) && (cVar3 = FUN_1404c1f20(lVar4), cVar3 == '\\0')) {\r\n      FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));\r\n      FUN_1404c1d70(lVar4,*(undefined4 *)(lVar2 + 0x100));\r\n    }\r\n    uVar7 = *(undefined4 *)(param_2 + 0xec);\r\n    uVar8 = 0;\r\n    uVar5 = 0x1a;\r\n    goto LAB_14076e5f9;\r\n  case 8:\r\n    if ((lVar4 != 0) && (cVar3 = FUN_1404c1f20(lVar4), cVar3 == '\\0')) {\r\n      FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));\r\n      FUN_1404c1d70(lVar4,*(undefined4 *)(lVar2 + 0x100));\r\n    }\r\n    FUN_14067aea0(lVar6,0x1a,*(undefined4 *)(param_2 + 0xec),*(undefined8 *)(param_1 + 0x10));\r\n    uVar5 = 0x1d;\r\n    uVar7 = 1;\r\n    goto LAB_14076e5f5;\r\n  case 0xf:\r\n    FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));\r\n    uVar8 = 0;\r\n    uVar5 = 0x1b;\r\n    uVar7 = 1;\r\n    goto LAB_14076e5f9;\r\n  case 0x10:\r\n    uVar5 = 0x1c;\r\n  }\r\n  uVar7 = *(undefined4 *)(lVar2 + 0x100);\r\nLAB_14076e5f5:\r\n  uVar8 = *(undefined8 *)(param_1 + 0x10);\r\nLAB_14076e5f9:\r\n  FUN_14067aea0(lVar6,uVar5,uVar7,uVar8);\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n  *pbVar1 = *pbVar1 | 4;\r\nswitchD_14076e509_caseD_2:\r\n  return;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nvoid FUN_14076e4a0(longlong param_1,longlong param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  undefined4 uVar7;\r\n  undefined8 uVar8;\r\n  \r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  lVar6 = D2_GameSingletonPtr + 0x1f77c8;\r\n  lVar4 = FUN_1403da910(param_1 + 0x70,2);\r\n  switch(*(undefined4 *)(param_2 + 0x60)) {\r\n  case 1:\r\n    uVar5 = 0x18;\r\n    break;\r\n  default:\r\n    goto switchD_14076e509_caseD_2;\r\n  case 5:\r\n    uVar5 = 0x19;\r\n    break;\r\n  case 7:\r\n    if ((lVar4 != 0) && (cVar3 = FUN_1404c1f20(lVar4), cVar3 == '\\0')) {\r\n      FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));\r\n      FUN_1404c1d70(lVar4,*(undefined4 *)(lVar2 + 0x100));\r\n    }\r\n    uVar7 = *(undefined4 *)(param_2 + 0xec);\r\n    uVar8 = 0;\r\n    uVar5 = 0x1a;\r\n    goto LAB_14076e5f9;\r\n  case 8:\r\n    if ((lVar4 != 0) && (cVar3 = FUN_1404c1f20(lVar4), cVar3 == '\\0')) {\r\n      FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));\r\n      FUN_1404c1d70(lVar4,*(undefined4 *)(lVar2 + 0x100));\r\n    }\r\n    FUN_14067aea0(lVar6,0x1a,*(undefined4 *)(param_2 + 0xec),*(undefined8 *)(param_1 + 0x10));\r\n    uVar5 = 0x1d;\r\n    uVar7 = 1;\r\n    goto LAB_14076e5f5;\r\n  case 0xf:\r\n    FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));\r\n    uVar8 = 0;\r\n    uVar5 = 0x1b;\r\n    uVar7 = 1;\r\n    goto LAB_14076e5f9;\r\n  case 0x10:\r\n    uVar5 = 0x1c;\r\n  }\r\n  uVar7 = *(undefined4 *)(lVar2 + 0x100);\r\nLAB_14076e5f5:\r\n  uVar8 = *(undefined8 *)(param_1 + 0x10);\r\nLAB_14076e5f9:\r\n  FUN_14067aea0(lVar6,uVar5,uVar7,uVar8);\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n  *pbVar1 = *pbVar1 | 4;\r\nswitchD_14076e509_caseD_2:\r\n  return;\r\n}\r\n\r\n"
    },
    "isError": false
  },
  "other": [
    {
      "content": [
        {
          "type": "text",
          "text": "From 14076e04c in FUN_14076e030 [DATA]\nFrom 14076e053 in FUN_14076e030 [DATA]"
        }
      ],
      "structuredContent": {
        "result": "From 14076e04c in FUN_14076e030 [DATA]\nFrom 14076e053 in FUN_14076e030 [DATA]"
      },
      "isError": false
    },
    {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140613ff0(longlong param_1,longlong param_2,char param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  \r\n  if ((*(char *)(param_2 + 0x60) != '\\0') || (param_3 != '\\0')) {\r\n    FUN_140614450();\r\n  }\r\n  *(byte *)(param_1 + 400) =\r\n       *(byte *)(param_1 + 400) ^ (*(byte *)(param_2 + 0x61) ^ *(byte *)(param_1 + 400)) & 1;\r\n  lVar3 = (longlong)*(int *)(param_2 + 0x70);\r\n  if (0 < lVar3) {\r\n    lVar2 = 0;\r\n    do {\r\n      lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x68) + 4 + lVar2);\r\n      if (lVar1 != 0) {\r\n        FUN_140618de0(param_1,lVar1,*(undefined4 *)(*(longlong *)(param_2 + 0x68) + 0xc + lVar2));\r\n      }\r\n      lVar2 = lVar2 + 0x14;\r\n      lVar3 = lVar3 + -1;\r\n    } while (lVar3 != 0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140613ff0(longlong param_1,longlong param_2,char param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  \r\n  if ((*(char *)(param_2 + 0x60) != '\\0') || (param_3 != '\\0')) {\r\n    FUN_140614450();\r\n  }\r\n  *(byte *)(param_1 + 400) =\r\n       *(byte *)(param_1 + 400) ^ (*(byte *)(param_2 + 0x61) ^ *(byte *)(param_1 + 400)) & 1;\r\n  lVar3 = (longlong)*(int *)(param_2 + 0x70);\r\n  if (0 < lVar3) {\r\n    lVar2 = 0;\r\n    do {\r\n      lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x68) + 4 + lVar2);\r\n      if (lVar1 != 0) {\r\n        FUN_140618de0(param_1,lVar1,*(undefined4 *)(*(longlong *)(param_2 + 0x68) + 0xc + lVar2));\r\n      }\r\n      lVar2 = lVar2 + 0x14;\r\n      lVar3 = lVar3 + -1;\r\n    } while (lVar3 != 0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    },
    {
      "content": [
        {
          "type": "text",
          "text": "From 143f2fc20 [DATA]\nFrom 141b5e530 [DATA]\nFrom 141b43dc0 [DATA]\nFrom 141b5d1c0 [DATA]\nFrom 141b5dd00 [DATA]\nFrom 141b652d0 [DATA]\nFrom 141b65900 [DATA]\nFrom 141c562d0 [DATA]\nFrom 141c56aa0 [DATA]\nFrom 141c5a490 [DATA]\nFrom 141c5c5a0 [DATA]\nFrom 141c9f830 [DATA]\nFrom 14077b9b0 [UNCONDITIONAL_CALL]"
        }
      ],
      "structuredContent": {
        "result": "From 143f2fc20 [DATA]\nFrom 141b5e530 [DATA]\nFrom 141b43dc0 [DATA]\nFrom 141b5d1c0 [DATA]\nFrom 141b5dd00 [DATA]\nFrom 141b652d0 [DATA]\nFrom 141b65900 [DATA]\nFrom 141c562d0 [DATA]\nFrom 141c56aa0 [DATA]\nFrom 141c5a490 [DATA]\nFrom 141c5c5a0 [DATA]\nFrom 141c9f830 [DATA]\nFrom 14077b9b0 [UNCONDITIONAL_CALL]"
      },
      "isError": false
    }
  ]
}

````

## Unescaped text fields for reading

### text

````text
{"address":"141c55bb8","length":320,"data":[224,181,142,64,1,0,0,0,176,59,116,64,1,0,0,0,48,117,126,64,1,0,0,0,48,75,116,64,1,0,0,0,240,78,116,64,1,0,0,0,144,79,116,64,1,0,0,0,80,119,126,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,117,126,64,1,0,0,0,16,118,126,64,1,0,0,0,240,115,126,64,1,0,0,0,96,117,126,64,1,0,0,0,176,152,9,64,1,0,0,0,16,66,116,64,1,0,0,0,80,72,116,64,1,0,0,0,48,72,116,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,16,116,126,64,1,0,0,0,176,152,9,64,1,0,0,0,240,86,116,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,98,101,108,116,0,0,0,0,98,101,108,116,48,50,0,0,116,111,32,98,101,32,97,98,108,101,32,116,111,32,114,101,109,111,118,101,32,98,111,110,101,99,104,97,114,109,115,32,97,108,114,101,97,100,121,32,111,119,110,101,100,32,98,121,32,116,104,101,32,112,108,97,121,101,114,0,0,0,0,0,110,101,101,100,32,97,32,115,116,97,114,116,101,100,32,114,105,103,105,100,98,111,100,121,32,102,111,114,32,115,112,101],"hex":"e0b58e4001000000b03b74400100000030757e4001000000304b744001000000f04e744001000000904f74400100000050777e4001000000b098094001000000b098094001000000b098094001000000b0757e400100000010767e4001000000f0737e400100000060757e4001000000b098094001000000104274400100000050487440010000003048744001000000b098094001000000b098094001000000b09809400100000010747e4001000000b098094001000000f056744001000000b098094001000000b09809400100000062656c740000000062656c7430320000746f2062652061626c6520746f2072656d6f766520626f6e65636861726d7320616c7265616479206f776e65642062792074686520706c6179657200000000006e65656420612073746172746564207269676964626f647920666f7220737065"}
````

### text

````text
{"address":"141c5a3b8","length":320,"data":[16,182,142,64,1,0,0,0,80,226,118,64,1,0,0,0,48,117,126,64,1,0,0,0,96,232,118,64,1,0,0,0,64,104,77,64,1,0,0,0,176,232,118,64,1,0,0,0,240,234,118,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,117,126,64,1,0,0,0,16,118,126,64,1,0,0,0,240,115,126,64,1,0,0,0,96,117,126,64,1,0,0,0,176,152,9,64,1,0,0,0,112,230,118,64,1,0,0,0,32,231,118,64,1,0,0,0,0,231,118,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,16,116,126,64,1,0,0,0,176,152,9,64,1,0,0,0,240,236,118,64,1,0,0,0,128,237,118,64,1,0,0,0,176,152,9,64,1,0,0,0,80,231,118,64,1,0,0,0,0,198,119,64,1,0,0,0,16,197,119,64,1,0,0,0,32,240,118,64,1,0,0,0,0,0,0,0,0,0,0,0,109,111,100,101,108,115,47,105,110,116,101,114,97,99,116,105,118,101,47,112,105,99,107,117,112,115,47,119,104,97,108,101,95,98,111,110,101,95,102,114,97,103,109,101,110,116,95,115,101,116,95,48,49,47,119,104,97,108,101,95,98,111,110,101,95,102,114,97,103,109,101,110],"hex":"10b68e400100000050e276400100000030757e400100000060e876400100000040684d4001000000b0e8764001000000f0ea764001000000b098094001000000b098094001000000b098094001000000b0757e400100000010767e4001000000f0737e400100000060757e4001000000b09809400100000070e676400100000020e776400100000000e7764001000000b098094001000000b098094001000000b09809400100000010747e4001000000b098094001000000f0ec76400100000080ed764001000000b09809400100000050e776400100000000c677400100000010c577400100000020f076400100000000000000000000006d6f64656c732f696e7465726163746976652f7069636b7570732f7768616c655f626f6e655f667261676d656e745f7365745f30312f7768616c655f626f6e655f667261676d656e"}
````

### text

````text
{"address":"141c5e258","length":384,"data":[96,177,139,64,1,0,0,0,128,90,120,64,1,0,0,0,144,104,139,64,1,0,0,0,16,0,145,64,1,0,0,0,208,253,139,64,1,0,0,0,64,139,135,64,1,0,0,0,32,150,57,64,1,0,0,0,16,133,135,64,1,0,0,0,0,128,135,64,1,0,0,0,32,150,57,64,1,0,0,0,176,152,9,64,1,0,0,0,240,127,135,64,1,0,0,0,64,195,135,64,1,0,0,0,32,183,135,64,1,0,0,0,16,183,135,64,1,0,0,0,48,125,135,64,1,0,0,0,176,132,135,64,1,0,0,0,144,197,135,64,1,0,0,0,208,132,135,64,1,0,0,0,208,197,135,64,1,0,0,0,16,127,135,64,1,0,0,0,48,133,135,64,1,0,0,0,48,126,135,64,1,0,0,0,32,191,135,64,1,0,0,0,128,83,135,64,1,0,0,0,176,152,9,64,1,0,0,0,208,97,135,64,1,0,0,0,176,197,135,64,1,0,0,0,240,219,135,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,112,158,135,64,1,0,0,0,64,169,135,64,1,0,0,0,192,155,135,64,1,0,0,0,112,156,135,64,1,0,0,0,112,162,135,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,148,135,64,1,0,0,0,112,146,135,64,1,0,0,0,34,5,147,25,1,0,0,0,24,255,19,2,0,0,0,0,0,0,0,0,3,0,0,0,220,0,20,2,32,0,0,0,0,0,0,0,1,0,0,0,34,5,147,25,1,0,0,0,240,80,15,2,0,0,0,0],"hex":"60b18b4001000000805a78400100000090688b40010000001000914001000000d0fd8b4001000000408b8740010000002096394001000000108587400100000000808740010000002096394001000000b098094001000000f07f87400100000040c387400100000020b787400100000010b7874001000000307d874001000000b08487400100000090c5874001000000d084874001000000d0c5874001000000107f8740010000003085874001000000307e87400100000020bf8740010000008053874001000000b098094001000000d061874001000000b0c5874001000000f0db874001000000b098094001000000b098094001000000709e87400100000040a9874001000000c09b874001000000709c87400100000070a2874001000000b098094001000000b098094001000000b098094001000000b0948740010000007092874001000000220593190100000018ff1302000000000000000003000000dc0014022000000000000000010000002205931901000000f0500f0200000000"}
````

### text

````text

void FUN_1407456f0(longlong param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  char cVar2;
  uint uVar3;
  ulonglong uVar4;
  int iVar5;
  uint uVar6;
  ulonglong uVar7;
  longlong lVar8;
  ulonglong uVar9;
  longlong lVar10;
  uint auStackX_10 [2];
  
  if (*(char *)(param_2 + 1) == '\0') {
    uVar4 = FUN_1400f13f0(0x10);
    uVar7 = 0;
    uVar9 = uVar7;
    if (uVar4 != 0) {
      FUN_140500c30(uVar4);
      uVar9 = uVar4;
    }
    *(ulonglong *)(param_1 + 0x20) = uVar9;
    (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,auStackX_10,4);
    uVar6 = auStackX_10[0];
    lVar10 = *(longlong *)(param_1 + 0x20);
    if (((int)auStackX_10[0] <= (int)(*(uint *)(lVar10 + 0xc) >> 8)) ||
       (cVar2 = FUN_140745470(lVar10,auStackX_10[0]), cVar2 != '\0')) {
      uVar3 = *(uint *)(lVar10 + 0xc) >> 8;
      *(uint *)(lVar10 + 8) = *(uint *)(lVar10 + 8) & 0xff000000;
      if ((int)uVar6 < (int)uVar3) {
        uVar3 = uVar6;
      }
      *(uint *)(lVar10 + 8) = *(uint *)(lVar10 + 8) | uVar3 & 0xffffff;
    }
    if (0 < (int)auStackX_10[0]) {
      do {
        lVar10 = **(longlong **)(param_1 + 0x20) + (longlong)(int)uVar7 * 0x18;
        lVar1 = *(longlong *)*param_2;
        if (*(char *)(param_2 + 1) == '\0') {
          (**(code **)(lVar1 + 0x28))((longlong *)*param_2,lVar10,4);
        }
        else {
          (**(code **)(lVar1 + 0x30))();
        }
        FUN_140131df0(param_2,lVar10 + 8);
        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,lVar10 + 0x10,1);
        (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,lVar10 + 0x11,1);
        uVar6 = (int)uVar7 + 1;
        uVar7 = (ulonglong)uVar6;
      } while ((int)uVar6 < (int)auStackX_10[0]);
    }
  }
  else {
    auStackX_10[0] = *(uint *)(param_1 + 0x30) & 0xffffff;
    (**(code **)(*(longlong *)*param_2 + 0x30))
              ((longlong *)*param_2,auStackX_10,4,param_4,0xfffffffffffffffe);
    iVar5 = 0;
    if ((*(uint *)(param_1 + 0x30) & 0xffffff) != 0) {
      lVar10 = 0;
      do {
        lVar8 = *(longlong *)(param_1 + 0x28) + lVar10;
        lVar1 = *(longlong *)*param_2;
        if (*(char *)(param_2 + 1) == '\0') {
          (**(code **)(lVar1 + 0x28))((longlong *)*param_2,lVar8,4);
        }
        else {
          (**(code **)(lVar1 + 0x30))();
        }
        FUN_140131fb0(param_2,*(undefined8 *)(lVar8 + 8));
        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,lVar8 + 0x10,1);
        (**(code **)(*(longlong *)*param_2 + 0x30))((longlong *)*param_2,lVar8 + 0x11,1);
        iVar5 = iVar5 + 1;
        lVar10 = lVar10 + 0x18;
      } while (iVar5 < (int)(*(uint *)(param_1 + 0x30) & 0xffffff));
    }
  }
  return;
}


````

### text

````text

void FUN_14076ecf0(longlong param_1,undefined8 *param_2)

{
  byte local_res10 [24];
  
  FUN_1405ed5e0();
  if (*(char *)(param_2 + 1) != '\0') {
    local_res10[0] = *(byte *)(param_1 + 0xd0) >> 2 & 2 | *(byte *)(param_1 + 0xd0) & 1;
    (**(code **)(*(longlong *)*param_2 + 0x30))();
    return;
  }
  local_res10[0] = 0;
  (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,local_res10);
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xf6;
  *(byte *)(param_1 + 0xd0) =
       *(byte *)(param_1 + 0xd0) | (local_res10[0] & 2) << 2 | local_res10[0] & 1;
  return;
}


````

### text

````text

void FUN_14076e750(longlong param_1,undefined8 param_2,undefined8 *param_3,undefined8 *param_4)

{
  char cVar1;
  longlong lVar2;
  undefined4 *puVar3;
  
  *param_3 = 0;
  *param_4 = 0;
  if ((*(byte *)(param_1 + 0xd0) & 2) == 0) {
    lVar2 = FUN_140877530(*(undefined8 *)(param_1 + 0x10));
    if ((lVar2 != 0) && (cVar1 = FUN_1406dd500(lVar2), cVar1 != '\0')) {
      return;
    }
    lVar2 = *(longlong *)(param_1 + 8);
    puVar3 = (undefined4 *)(param_1 + 0xd4);
    FUN_140681fb0(puVar3,lVar2 + 200);
    lVar2 = *(longlong *)(lVar2 + 0xf8);
    if ((lVar2 != 0) && ((*(int *)(lVar2 + 0x60) == 0x19 || (*(int *)(lVar2 + 0xc4) != -1)))) {
      *puVar3 = *(undefined4 *)(lVar2 + 100);
    }
    *param_3 = puVar3;
  }
  return;
}


````

### text

````text

void FUN_140744210(undefined8 param_1)

{
  undefined8 uVar1;
  int iVar2;
  longlong lVar3;
  
  lVar3 = FUN_1407e7380();
  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  while (iVar2 == 1) {
    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  }
  FUN_14048ff20(*(undefined8 *)(lVar3 + 0x7b0),param_1);
  FUN_1401581e0(lVar3 + 0xc0c,0);
  uVar1 = FUN_1407e7380();
  FUN_1407ec5a0(uVar1,param_1);
  return;
}


````

### text

````text

void FUN_14076ed80(longlong param_1,undefined8 *param_2)

{
  int iVar1;
  int *piVar2;
  longlong lVar3;
  bool abStackX_8 [8];
  
  FUN_1403da950(param_1 + 0x30);
  lVar3 = 0;
  if (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) {
    lVar3 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x80);
  }
  abStackX_8[0] = false;
  if (*(char *)(param_2 + 1) == '\0') {
    (**(code **)(*(longlong *)*param_2 + 0x28))((longlong *)*param_2,abStackX_8,1);
    if ((abStackX_8[0] != false) && (lVar3 != 0)) {
      FUN_1406709a0(lVar3,*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8));
    }
    return;
  }
  if (lVar3 != 0) {
    iVar1 = *(int *)(*(longlong *)(param_1 + 0x10) + 0xa8);
    piVar2 = (int *)FUN_14066dd00(lVar3);
    abStackX_8[0] = *piVar2 == iVar1;
  }
  lVar3 = *(longlong *)*param_2;
  if (*(char *)(param_2 + 1) != '\0') {
    (**(code **)(lVar3 + 0x30))();
    return;
  }
  (**(code **)(lVar3 + 0x28))((longlong *)*param_2,abStackX_8,1);
  return;
}


````

### text

````text

void FUN_14076f020(longlong param_1,byte param_2)

{
  byte *pbVar1;
  longlong *plVar2;
  char cVar3;
  longlong lVar4;
  byte bVar5;
  
  bVar5 = 1;
  lVar4 = FUN_1403da910(param_1 + 0x30,1);
  param_2 = param_2 & lVar4 == 0;
  lVar4 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);
  if ((lVar4 == 0) || (*(int *)(lVar4 + 0x60) != 0x1a)) goto LAB_14076f0bd;
  lVar4 = FUN_1403da910(param_1 + 0x70,2);
  if (lVar4 == 0) {
LAB_14076f080:
    bVar5 = 0;
  }
  else {
    cVar3 = FUN_1404c1f20(lVar4);
    if (cVar3 == '\0') goto LAB_14076f080;
  }
  param_2 = param_2 & bVar5;
  if (param_2 == 0) {
    FUN_14087b6f0(*(undefined8 *)(param_1 + 0x10),DAT_14231ed30);
    FUN_140235990(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0xf0));
    plVar2 = *(longlong **)(*(longlong *)(param_1 + 0x10) + 0xf0);
    (**(code **)(*plVar2 + 0x28))(plVar2,4);
  }
LAB_14076f0bd:
  lVar4 = *(longlong *)(param_1 + 0x10);
  pbVar1 = (byte *)(lVar4 + 0xd7);
  *pbVar1 = *pbVar1 & 0xfe;
  pbVar1 = (byte *)(lVar4 + 0xd7);
  *pbVar1 = *pbVar1 | param_2;
  return;
}


````

### text

````text

void FUN_140744b30(longlong param_1)

{
  longlong lVar1;
  char cVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  longlong *plVar6;
  uint uVar7;
  undefined4 *puVar8;
  int iVar9;
  longlong lVar10;
  longlong lVar11;
  longlong lStackX_10;
  undefined1 auStack_88 [24];
  undefined4 auStack_70 [2];
  undefined1 auStack_68 [40];
  undefined4 uStack_40;
  
  FUN_1407e7400();
  lVar1 = *(longlong *)(param_1 + 8);
  uVar7 = *(uint *)(lVar1 + 0x70) & 0xffffff;
  lStackX_10 = 0;
  if (*(longlong *)(param_1 + 0x20) == 0) {
    uVar3 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));
    lVar4 = FUN_1408dcad0(uVar3);
    if ((((lVar4 != 0) && (lStackX_10 = *(longlong *)(lVar4 + 0x418), lStackX_10 != 0)) &&
        (lVar4 = FUN_1408ae9b0(D2_GameSingletonPtr,lStackX_10,&DAT_1431af668,&DAT_14228a780,
                               *(undefined8 *)(param_1 + 0x10),1,0,0,0), lVar4 != 0)) &&
       (cVar2 = FUN_140743c30(param_1,lVar4,lVar1 + 0x78,0), cVar2 == '\0')) {
      FUN_1408847f0(lVar4,auStack_88,&DAT_1439a6f30);
      FUN_140882fc0(auStack_88);
    }
  }
  plVar6 = *(longlong **)(param_1 + 0x20);
  iVar9 = 0;
  lVar4 = (longlong)(int)uVar7;
  if (plVar6 == (longlong *)0x0) {
    if (uVar7 != 0) {
      lVar10 = 0;
      do {
        if ((lStackX_10 == 0) ||
           (*(longlong *)(lVar10 + 8 + *(longlong *)(lVar1 + 0x68)) != *(longlong *)(lVar1 + 0x78)))
        {
          lVar11 = FUN_1408ae9b0(D2_GameSingletonPtr,
                                 *(undefined8 *)(lVar10 + *(longlong *)(lVar1 + 0x68)),
                                 &DAT_1431af668,&DAT_14228a780,*(undefined8 *)(param_1 + 0x10),1,0,0
                                 ,0);
          if (lVar11 != 0) {
            lVar5 = FUN_140877720(lVar11,&DAT_1439aea90,0);
            uVar3 = 0;
            if (lVar5 == 0) {
LAB_140744e6c:
              cVar2 = FUN_140743c30(param_1,lVar11,
                                    (longlong)iVar9 * 0x10 + 8 + *(longlong *)(lVar1 + 0x68),uVar3);
              if (cVar2 != '\0') goto LAB_140744eb0;
            }
            else {
              lVar5 = *(longlong *)(lVar5 + 8);
              cVar2 = FUN_14060b3e0(lVar5 + 0x120);
              if (cVar2 != '\0') {
LAB_140744e69:
                uVar3 = 1;
                goto LAB_140744e6c;
              }
              uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);
              uVar3 = FUN_140877720(uVar3,&DAT_1439c55b0,0);
              auStack_70[0] = *(undefined4 *)(lVar5 + 0x120);
              FUN_14013f490(auStack_68,lVar5 + 0x128);
              uStack_40 = *(undefined4 *)(lVar5 + 0x150);
              cVar2 = FUN_1406101f0(uVar3,auStack_70);
              if (cVar2 == '\0') goto LAB_140744e69;
            }
            FUN_1408847f0(lVar11,auStack_88,&DAT_1439a6f30);
            FUN_140882fc0(auStack_88);
          }
        }
LAB_140744eb0:
        iVar9 = iVar9 + 1;
        lVar10 = lVar10 + 0x10;
        lVar4 = lVar4 + -1;
      } while (lVar4 != 0);
    }
  }
  else {
    lVar4 = 0;
    if ((*(uint *)(plVar6 + 1) & 0xffffff) != 0) {
      lVar10 = 0;
      lVar11 = 0;
      do {
        puVar8 = (undefined4 *)(*plVar6 + lVar11);
        if ((((*(char *)((longlong)puVar8 + 0x11) == '\0') &&
             ((lStackX_10 == 0 || (*(longlong *)(puVar8 + 2) != *(longlong *)(lVar1 + 0x78))))) &&
            ((lVar5 = FUN_1406dc560(*puVar8), lVar5 != 0 ||
             ((lVar4 < (int)uVar7 &&
              (lVar5 = FUN_1408ae9b0(D2_GameSingletonPtr,
                                     *(undefined8 *)(lVar10 + *(longlong *)(lVar1 + 0x68)),
                                     &DAT_1431af668,&DAT_14228a780,*(undefined8 *)(param_1 + 0x10),1
                                     ,0,0,0), lVar5 != 0)))))) &&
           (cVar2 = FUN_140743c30(param_1,lVar5,puVar8 + 2,*(undefined1 *)(puVar8 + 4)),
           cVar2 == '\0')) {
          FUN_1408847f0(lVar5,auStack_88,&DAT_1439a6f30);
          FUN_140882fc0(auStack_88);
        }
        plVar6 = *(longlong **)(param_1 + 0x20);
        iVar9 = iVar9 + 1;
        lVar4 = lVar4 + 1;
        lVar11 = lVar11 + 0x18;
        lVar10 = lVar10 + 0x10;
      } while (iVar9 < (int)(*(uint *)(plVar6 + 1) & 0xffffff));
    }
  }
  return;
}


````

### text

````text

void FUN_14077c600(longlong *param_1,char param_2)

{
  if ((char)param_1[4] != param_2) {
    (**(code **)(*param_1 + 0xe8))();
    *(char *)(param_1 + 4) = param_2;
    *(byte *)(param_1[2] + 0xd4) = *(byte *)(param_1[2] + 0xd4) | 4;
  }
  return;
}


````

### text

````text

bool FUN_14077c510(longlong param_1)

{
  char cVar1;
  
  cVar1 = FUN_140878c60(*(undefined8 *)(param_1 + 0x10));
  return cVar1 == '\0';
}


````

### text

````text

void FUN_14076e670(undefined8 param_1)

{
  undefined8 uVar1;
  int iVar2;
  longlong lVar3;
  
  lVar3 = FUN_1407e7380();
  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  while (iVar2 == 1) {
    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  }
  FUN_14048ff20(*(undefined8 *)(lVar3 + 0x5b8),param_1);
  FUN_1401581e0(lVar3 + 0xc0c,0);
  uVar1 = FUN_1407e7380();
  FUN_1407ec5a0(uVar1,param_1);
  return;
}


````

### text

````text

longlong FUN_14076e250(longlong param_1,uint param_2)

{
  FUN_1404e3050(param_1 + 0xa8);
  FUN_1404e3050(param_1 + 0x80);
  FUN_14077c350(param_1);
  if ((param_2 & 1) != 0) {
    FUN_1400f1520(param_1);
  }
  return param_1;
}


````

### text

````text

void FUN_14076e860(longlong param_1)

{
  byte *pbVar1;
  
  FUN_1407e7400();
  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd7);
  *pbVar1 = *pbVar1 | 4;
  *(byte *)(*(longlong *)(param_1 + 0x10) + 0xd1) =
       *(char *)(*(longlong *)(param_1 + 8) + 0x118) << 7 |
       *(byte *)(*(longlong *)(param_1 + 0x10) + 0xd1) & 0x7f;
  return;
}


````

### text

````text

void FUN_14076e8b0(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined4 uVar7;
  undefined4 local_res8 [2];
  longlong local_28;
  code *local_20;
  
  uVar6 = *(undefined8 *)(param_1 + 0x10);
  lVar4 = FUN_1408a8ba0(D2_GameSingletonPtr);
  uVar7 = 1;
  uVar5 = 0;
  if (lVar4 != 0) {
    uVar5 = FUN_140877720(lVar4,&DAT_1439b9e90,1);
  }
  FUN_1403dab80(param_1 + 0x40,uVar5);
  uVar5 = FUN_140877720(uVar6,&DAT_1439e85b0,1);
  FUN_1403dab80(param_1 + 0x50,uVar5);
  uVar6 = FUN_140877720(uVar6,&DAT_1439b4630,1);
  FUN_1403dab80(param_1 + 0x60,uVar6);
  if (lVar4 == 0) {
    uVar6 = 0;
  }
  else {
    uVar6 = FUN_140877720(lVar4,&DAT_1439d15f0,2);
  }
  FUN_1403dab80(param_1 + 0x70,uVar6);
  FUN_14077c530(param_1);
  lVar2 = *(longlong *)(param_1 + 8);
  lVar3 = *(longlong *)(lVar2 + 0xf8);
  if (lVar3 != 0) {
    if (((*(int *)(lVar3 + 0x60) - 5U & 0xfffffffc) == 0) && (*(int *)(lVar3 + 0x60) != 6)) {
      if (lVar4 != 0) {
        uVar6 = FUN_140877720(lVar4,&DAT_1439e86d0,0);
        uVar6 = FUN_140777b90(uVar6,0x14);
        iVar1 = *(int *)(lVar3 + 0x60);
        if (iVar1 != 5) {
          if (iVar1 == 7) {
            uVar7 = 0;
          }
          else if (iVar1 == 8) {
            uVar7 = 2;
          }
        }
        if (*(char *)(lVar2 + 0x119) == '\0') {
          local_res8[0] = *(undefined4 *)(lVar3 + 100);
          FUN_1403c4850(uVar6,*(undefined8 *)(param_1 + 0x10),uVar7,local_res8);
        }
      }
      local_20 = FUN_14076f140;
      local_28 = param_1;
      uVar6 = FUN_140810240();
      FUN_1408106f0(uVar6,0x123,&local_28);
      local_20 = (code *)&LAB_14076f180;
      local_28 = param_1;
      uVar6 = FUN_140810240();
      FUN_1408106f0(uVar6,0x124,&local_28);
      if (*(longlong *)(lVar2 + 0x108) != 0) {
        FUN_1404e2940(param_1 + 0x80,*(longlong *)(lVar2 + 0x108),*(undefined8 *)(param_1 + 0x10),0,
                      0,0,0);
      }
    }
    if ((*(uint *)(param_1 + 0x1c) >> 0x1e & 1) == 0) {
      FUN_14076e4a0(param_1,lVar3);
    }
  }
  if (((*(uint *)(param_1 + 0x1c) >> 0x1e & 1) != 0) && ((*(byte *)(param_1 + 0xd0) & 8) != 0)) {
    FUN_1407e79a0(param_1);
  }
  return;
}


````

### text

````text

void FUN_14076eaf0(longlong param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong lVar3;
  longlong lStack_18;
  code *pcStack_10;
  
  FUN_14077c580();
  FUN_1403dab80(param_1 + 0x40,0);
  FUN_1403dab80(param_1 + 0x50,0);
  FUN_1403dab80(param_1 + 0x60,0);
  FUN_1403dab80(param_1 + 0x70,0);
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);
  if (lVar3 != 0) {
    iVar1 = *(int *)(lVar3 + 0x60);
    if (((iVar1 - 5U & 0xfffffffc) == 0) && (iVar1 != 6)) {
      pcStack_10 = FUN_14076f140;
      lStack_18 = param_1;
      uVar2 = FUN_140810240();
      FUN_140811040(uVar2,0x123,&lStack_18);
      pcStack_10 = (code *)&LAB_14076f180;
      lStack_18 = param_1;
      uVar2 = FUN_140810240();
      FUN_140811040(uVar2,0x124,&lStack_18);
      lVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);
      if (lVar3 != 0) {
        uVar2 = FUN_140877720(lVar3,&DAT_1439e86d0,0);
        uVar2 = FUN_140777b90(uVar2,0x14);
        FUN_1403c4af0(uVar2,*(undefined8 *)(param_1 + 0x10));
      }
      FUN_1404e3050(param_1 + 0x80);
      FUN_1404e3050(param_1 + 0xa8);
      return;
    }
  }
  return;
}


````

### text

````text

void FUN_14077b9a0(longlong param_1,char param_2)

{
  uint uVar1;
  longlong lVar2;
  uint uVar3;
  longlong lVar4;
  
  FUN_14077c600();
  if ((*(char *)(*(longlong *)(param_1 + 8) + 0x84) != '\0') &&
     (uVar1 = *(uint *)(param_1 + 0x48), (uVar1 & 0xffffff) != 0xffffff)) {
    lVar2 = *(longlong *)(D2_GameSingletonPtr + 0x60410);
    uVar3 = uVar1 & 0xffffff;
    if (param_2 == '\0') {
      if ((((uVar3 != 0xffffff) && (uVar3 < *(uint *)(lVar2 + 0x30))) &&
          (lVar4 = (longlong)(int)uVar3 * 0xb0 + *(longlong *)(lVar2 + 0x28),
          (*(byte *)(lVar4 + 0x44) & 3) != 0)) &&
         ((*(uint *)(lVar4 + 0x70) == uVar1 &&
          (*(int *)((ulonglong)(uVar1 & 0xffffff) * 0xb0 + 0x78 + *(longlong *)(lVar2 + 0x28)) != -1
          )))) {
        FUN_140c8f080(lVar2,param_1 + 0x48,1);
      }
    }
    else if (((uVar3 == 0xffffff) || (*(uint *)(lVar2 + 0x30) <= uVar3)) ||
            ((lVar4 = (longlong)(int)uVar3 * 0xb0 + *(longlong *)(lVar2 + 0x28),
             (*(byte *)(lVar4 + 0x44) & 3) == 0 ||
             ((*(uint *)(lVar4 + 0x70) != uVar1 ||
              (*(int *)((ulonglong)(uVar1 & 0xffffff) * 0xb0 + 0x78 + *(longlong *)(lVar2 + 0x28))
               == -1)))))) {
      FUN_140c8e930(lVar2,param_1 + 0x48,1,1,1);
      return;
    }
  }
  return;
}


````

### text

````text
From 143f1a818 [DATA]
From 140617a76 [UNCONDITIONAL_CALL]
From 140617b06 [UNCONDITIONAL_CALL]
From 140614056 in FUN_140613ff0 [UNCONDITIONAL_CALL]
From 140534d63 [UNCONDITIONAL_CALL]
From 1404ba864 in FUN_1404ba810 [UNCONDITIONAL_CALL]
````

### text

````text

void FUN_14076e4a0(longlong param_1,longlong param_2)

{
  byte *pbVar1;
  longlong lVar2;
  char cVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong lVar6;
  undefined4 uVar7;
  undefined8 uVar8;
  
  lVar2 = *(longlong *)(param_1 + 8);
  lVar6 = D2_GameSingletonPtr + 0x1f77c8;
  lVar4 = FUN_1403da910(param_1 + 0x70,2);
  switch(*(undefined4 *)(param_2 + 0x60)) {
  case 1:
    uVar5 = 0x18;
    break;
  default:
    goto switchD_14076e509_caseD_2;
  case 5:
    uVar5 = 0x19;
    break;
  case 7:
    if ((lVar4 != 0) && (cVar3 = FUN_1404c1f20(lVar4), cVar3 == '\0')) {
      FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));
      FUN_1404c1d70(lVar4,*(undefined4 *)(lVar2 + 0x100));
    }
    uVar7 = *(undefined4 *)(param_2 + 0xec);
    uVar8 = 0;
    uVar5 = 0x1a;
    goto LAB_14076e5f9;
  case 8:
    if ((lVar4 != 0) && (cVar3 = FUN_1404c1f20(lVar4), cVar3 == '\0')) {
      FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));
      FUN_1404c1d70(lVar4,*(undefined4 *)(lVar2 + 0x100));
    }
    FUN_14067aea0(lVar6,0x1a,*(undefined4 *)(param_2 + 0xec),*(undefined8 *)(param_1 + 0x10));
    uVar5 = 0x1d;
    uVar7 = 1;
    goto LAB_14076e5f5;
  case 0xf:
    FUN_14067aea0(lVar6,0x18,*(undefined4 *)(lVar2 + 0x100),*(undefined8 *)(param_1 + 0x10));
    uVar8 = 0;
    uVar5 = 0x1b;
    uVar7 = 1;
    goto LAB_14076e5f9;
  case 0x10:
    uVar5 = 0x1c;
  }
  uVar7 = *(undefined4 *)(lVar2 + 0x100);
LAB_14076e5f5:
  uVar8 = *(undefined8 *)(param_1 + 0x10);
LAB_14076e5f9:
  FUN_14067aea0(lVar6,uVar5,uVar7,uVar8);
  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);
  *pbVar1 = *pbVar1 | 4;
switchD_14076e509_caseD_2:
  return;
}


````

### text

````text

void FUN_140613ff0(longlong param_1,longlong param_2,char param_3)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  
  if ((*(char *)(param_2 + 0x60) != '\0') || (param_3 != '\0')) {
    FUN_140614450();
  }
  *(byte *)(param_1 + 400) =
       *(byte *)(param_1 + 400) ^ (*(byte *)(param_2 + 0x61) ^ *(byte *)(param_1 + 400)) & 1;
  lVar3 = (longlong)*(int *)(param_2 + 0x70);
  if (0 < lVar3) {
    lVar2 = 0;
    do {
      lVar1 = *(longlong *)(*(longlong *)(param_2 + 0x68) + 4 + lVar2);
      if (lVar1 != 0) {
        FUN_140618de0(param_1,lVar1,*(undefined4 *)(*(longlong *)(param_2 + 0x68) + 0xc + lVar2));
      }
      lVar2 = lVar2 + 0x14;
      lVar3 = lVar3 + -1;
    } while (lVar3 != 0);
  }
  return;
}


````

### text

````text
From 143f2fc20 [DATA]
From 141b5e530 [DATA]
From 141b43dc0 [DATA]
From 141b5d1c0 [DATA]
From 141b5dd00 [DATA]
From 141b652d0 [DATA]
From 141b65900 [DATA]
From 141c562d0 [DATA]
From 141c56aa0 [DATA]
From 141c5a490 [DATA]
From 141c5c5a0 [DATA]
From 141c9f830 [DATA]
From 14077b9b0 [UNCONDITIONAL_CALL]
````
