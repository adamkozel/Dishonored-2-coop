# ghidra-domino-component-lifecycle-v210.json

Original: [work/ghidra-domino-component-lifecycle-v210.json](<../../../../work/ghidra-domino-component-lifecycle-v210.json>)

SHA-256: `E22FD1DBF6ECF797BFD427C6707CF42D8F4974CB05D11F783AD880C9A72576EA`; 12191 bytes.

## Original saved record

````json
[
  {
    "addresses": [
      "1404470d0",
      "140447590",
      "1404473a0",
      "140446fe0"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_1404470d0(undefined8 param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  \r\n  lVar3 = FUN_1407e7380();\r\n  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  }\r\n  FUN_14048ff20(*(undefined8 *)(lVar3 + 0xad0),param_1);\r\n  FUN_1401581e0(lVar3 + 0xc0c,0);\r\n  uVar1 = FUN_1407e7380();\r\n  FUN_1407ec5a0(uVar1,param_1);\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404470d0(undefined8 param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  \r\n  lVar3 = FUN_1407e7380();\r\n  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);\r\n  }\r\n  FUN_14048ff20(*(undefined8 *)(lVar3 + 0xad0),param_1);\r\n  FUN_1401581e0(lVar3 + 0xc0c,0);\r\n  uVar1 = FUN_1407e7380();\r\n  FUN_1407ec5a0(uVar1,param_1);\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_140447590(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong *plVar2;\r\n  longlong lVar3;\r\n  undefined8 uVar4;\r\n  bool bVar5;\r\n  undefined8 uStack_58;\r\n  longlong lStack_50;\r\n  undefined4 uStack_48;\r\n  undefined2 *puStack_20;\r\n  undefined2 *puStack_18;\r\n  longlong lStack_10;\r\n  \r\n  FUN_14061c150();\r\n  bVar5 = false;\r\n  FUN_140446de0(&uStack_58);\r\n  if (lStack_10 != 0) {\r\n    bVar5 = true;\r\n    do {\r\n      if (*(longlong *)(lStack_10 + 0x10) == 0) {\r\n        *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;\r\n      }\r\n      else {\r\n        *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(*(longlong *)(lStack_10 + 0x10) + 0xa8);\r\n      }\r\n      puStack_20 = puStack_20 + 1;\r\n      if (puStack_18 <= puStack_20) {\r\n        lStack_10 = 0;\r\n        break;\r\n      }\r\n      lStack_10 = FUN_1404900b0(uStack_58,*puStack_20);\r\n    } while (lStack_10 != 0);\r\n  }\r\n  if ((lStack_50 != 0) && (iVar1 = FUN_1401581d0(lStack_50 + -4), iVar1 == 0)) {\r\n    plVar2 = (longlong *)FUN_1401548e0();\r\n    (**(code **)(*plVar2 + 0x10))\r\n              (plVar2,\r\n               \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\dishonored2\\\\game\\\\components\\\\arkRoster.h\"\r\n               ,0x115,lStack_50 + -4,0x10);\r\n  }\r\n  lStack_50 = 0;\r\n  uStack_48 = 0;\r\n  if (!bVar5) {\r\n    lVar3 = FUN_1408ae9b0(D2_GameSingletonPtr,*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x198),\r\n                          &DAT_1431af668,&DAT_14228a780,0,0,0,0,0);\r\n    if (lVar3 == 0) {\r\n      *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;\r\n    }\r\n    else {\r\n      *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(lVar3 + 0xa8);\r\n    }\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n    *(byte *)(lVar3 + 0xd4) = *(byte *)(lVar3 + 0xd4) | 4;\r\n  }\r\n  uVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n  uVar4 = FUN_140877720(uVar4,&DAT_1439e2a90,0);\r\n  *(undefined8 *)(param_1 + 0xb8) = uVar4;\r\n  uVar4 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  uVar4 = FUN_140877720(uVar4,&DAT_1439e2b20,0);\r\n  *(undefined8 *)(param_1 + 0xc0) = uVar4;\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_140447590(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong *plVar2;\r\n  longlong lVar3;\r\n  undefined8 uVar4;\r\n  bool bVar5;\r\n  undefined8 uStack_58;\r\n  longlong lStack_50;\r\n  undefined4 uStack_48;\r\n  undefined2 *puStack_20;\r\n  undefined2 *puStack_18;\r\n  longlong lStack_10;\r\n  \r\n  FUN_14061c150();\r\n  bVar5 = false;\r\n  FUN_140446de0(&uStack_58);\r\n  if (lStack_10 != 0) {\r\n    bVar5 = true;\r\n    do {\r\n      if (*(longlong *)(lStack_10 + 0x10) == 0) {\r\n        *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;\r\n      }\r\n      else {\r\n        *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(*(longlong *)(lStack_10 + 0x10) + 0xa8);\r\n      }\r\n      puStack_20 = puStack_20 + 1;\r\n      if (puStack_18 <= puStack_20) {\r\n        lStack_10 = 0;\r\n        break;\r\n      }\r\n      lStack_10 = FUN_1404900b0(uStack_58,*puStack_20);\r\n    } while (lStack_10 != 0);\r\n  }\r\n  if ((lStack_50 != 0) && (iVar1 = FUN_1401581d0(lStack_50 + -4), iVar1 == 0)) {\r\n    plVar2 = (longlong *)FUN_1401548e0();\r\n    (**(code **)(*plVar2 + 0x10))\r\n              (plVar2,\r\n               \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\dishonored2\\\\game\\\\components\\\\arkRoster.h\"\r\n               ,0x115,lStack_50 + -4,0x10);\r\n  }\r\n  lStack_50 = 0;\r\n  uStack_48 = 0;\r\n  if (!bVar5) {\r\n    lVar3 = FUN_1408ae9b0(D2_GameSingletonPtr,*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x198),\r\n                          &DAT_1431af668,&DAT_14228a780,0,0,0,0,0);\r\n    if (lVar3 == 0) {\r\n      *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;\r\n    }\r\n    else {\r\n      *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(lVar3 + 0xa8);\r\n    }\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n    *(byte *)(lVar3 + 0xd4) = *(byte *)(lVar3 + 0xd4) | 4;\r\n  }\r\n  uVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n  uVar4 = FUN_140877720(uVar4,&DAT_1439e2a90,0);\r\n  *(undefined8 *)(param_1 + 0xb8) = uVar4;\r\n  uVar4 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  uVar4 = FUN_140877720(uVar4,&DAT_1439e2b20,0);\r\n  *(undefined8 *)(param_1 + 0xc0) = uVar4;\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nundefined4 FUN_1404473a0(void)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1407e7380();\r\n  return *(undefined4 *)(*(longlong *)(lVar1 + 0xad0) + 0xc);\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined4 FUN_1404473a0(void)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1407e7380();\r\n  return *(undefined4 *)(*(longlong *)(lVar1 + 0xad0) + 0xc);\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_140446fe0(longlong param_1)\r\n\r\n{\r\n  FUN_1407e7b10(*(undefined8 *)(param_1 + 0xb8));\r\n  *(undefined4 *)(param_1 + 0xf4) = *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x1b0);\r\n  FUN_14061bc20(param_1);\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_140446fe0(longlong param_1)\r\n\r\n{\r\n  FUN_1407e7b10(*(undefined8 *)(param_1 + 0xb8));\r\n  *(undefined4 *)(param_1 + 0xf4) = *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x1b0);\r\n  FUN_14061bc20(param_1);\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  },
  {
    "addresses": [
      "140447760",
      "1404477d0",
      "140447830",
      "1404473d0"
    ],
    "results": [
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_140447760(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  undefined1 auStack_28 [32];\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n  if (lVar1 != 0) {\r\n    uVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n    FUN_1408847f0(uVar2,auStack_28,&DAT_1439a6f30);\r\n    FUN_140882fc0(auStack_28);\r\n    *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;\r\n  }\r\n  *(undefined8 *)(param_1 + 0xc0) = 0;\r\n  *(undefined8 *)(param_1 + 0xb8) = 0;\r\n  FUN_14061c1a0(param_1);\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_140447760(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  undefined1 auStack_28 [32];\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n  if (lVar1 != 0) {\r\n    uVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));\r\n    FUN_1408847f0(uVar2,auStack_28,&DAT_1439a6f30);\r\n    FUN_140882fc0(auStack_28);\r\n    *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;\r\n  }\r\n  *(undefined8 *)(param_1 + 0xc0) = 0;\r\n  *(undefined8 *)(param_1 + 0xb8) = 0;\r\n  FUN_14061c1a0(param_1);\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_1404477d0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong *plVar2;\r\n  \r\n  FUN_14061c1f0();\r\n  plVar2 = (longlong *)FUN_1404bd8d0(*(undefined8 *)(param_1 + 0xb8));\r\n  (**(code **)(*plVar2 + 0xd8))(plVar2,1);\r\n  *(undefined4 *)(param_1 + 200) = 0x3fff0010;\r\n  iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));\r\n  *(uint *)(param_1 + 0xf8) = (uint)(iVar1 != 0);\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_1404477d0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong *plVar2;\r\n  \r\n  FUN_14061c1f0();\r\n  plVar2 = (longlong *)FUN_1404bd8d0(*(undefined8 *)(param_1 + 0xb8));\r\n  (**(code **)(*plVar2 + 0xd8))(plVar2,1);\r\n  *(undefined4 *)(param_1 + 200) = 0x3fff0010;\r\n  iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));\r\n  *(uint *)(param_1 + 0xf8) = (uint)(iVar1 != 0);\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nvoid FUN_140447830(longlong param_1)\r\n\r\n{\r\n  longlong *plVar1;\r\n  \r\n  FUN_1404e3050(param_1 + 0xcc);\r\n  if (((byte)*(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x1c) & 0xc) < 8) {\r\n    plVar1 = (longlong *)FUN_1404bd8d0();\r\n    (**(code **)(*plVar1 + 0xd8))(plVar1,0);\r\n  }\r\n  FUN_14061c2a0(param_1);\r\n  return;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nvoid FUN_140447830(longlong param_1)\r\n\r\n{\r\n  longlong *plVar1;\r\n  \r\n  FUN_1404e3050(param_1 + 0xcc);\r\n  if (((byte)*(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x1c) & 0xc) < 8) {\r\n    plVar1 = (longlong *)FUN_1404bd8d0();\r\n    (**(code **)(*plVar1 + 0xd8))(plVar1,0);\r\n  }\r\n  FUN_14061c2a0(param_1);\r\n  return;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      },
      {
        "status": "fulfilled",
        "value": {
          "content": [
            {
              "type": "text",
              "text": "\r\nundefined8 FUN_1404473d0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  \r\n  if (*(int *)(param_1 + 0xf8) != 1) {\r\n    iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));\r\n    if (iVar1 != 0) {\r\n      uVar2 = FUN_14061c060(param_1);\r\n      return uVar2;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
            }
          ],
          "structuredContent": {
            "result": "\r\nundefined8 FUN_1404473d0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  \r\n  if (*(int *)(param_1 + 0xf8) != 1) {\r\n    iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));\r\n    if (iVar1 != 0) {\r\n      uVar2 = FUN_14061c060(param_1);\r\n      return uVar2;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          },
          "isError": false
        }
      }
    ]
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_1404470d0(undefined8 param_1)

{
  undefined8 uVar1;
  int iVar2;
  longlong lVar3;
  
  lVar3 = FUN_1407e7380();
  iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  while (iVar2 == 1) {
    iVar2 = FUN_1401581c0(lVar3 + 0xc0c,0,1);
  }
  FUN_14048ff20(*(undefined8 *)(lVar3 + 0xad0),param_1);
  FUN_1401581e0(lVar3 + 0xc0c,0);
  uVar1 = FUN_1407e7380();
  FUN_1407ec5a0(uVar1,param_1);
  return;
}


````

### text

````text

void FUN_140447590(longlong param_1)

{
  int iVar1;
  longlong *plVar2;
  longlong lVar3;
  undefined8 uVar4;
  bool bVar5;
  undefined8 uStack_58;
  longlong lStack_50;
  undefined4 uStack_48;
  undefined2 *puStack_20;
  undefined2 *puStack_18;
  longlong lStack_10;
  
  FUN_14061c150();
  bVar5 = false;
  FUN_140446de0(&uStack_58);
  if (lStack_10 != 0) {
    bVar5 = true;
    do {
      if (*(longlong *)(lStack_10 + 0x10) == 0) {
        *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;
      }
      else {
        *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(*(longlong *)(lStack_10 + 0x10) + 0xa8);
      }
      puStack_20 = puStack_20 + 1;
      if (puStack_18 <= puStack_20) {
        lStack_10 = 0;
        break;
      }
      lStack_10 = FUN_1404900b0(uStack_58,*puStack_20);
    } while (lStack_10 != 0);
  }
  if ((lStack_50 != 0) && (iVar1 = FUN_1401581d0(lStack_50 + -4), iVar1 == 0)) {
    plVar2 = (longlong *)FUN_1401548e0();
    (**(code **)(*plVar2 + 0x10))
              (plVar2,
               "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\dishonored2\\game\\components\\arkRoster.h"
               ,0x115,lStack_50 + -4,0x10);
  }
  lStack_50 = 0;
  uStack_48 = 0;
  if (!bVar5) {
    lVar3 = FUN_1408ae9b0(D2_GameSingletonPtr,*(undefined8 *)(*(longlong *)(param_1 + 8) + 0x198),
                          &DAT_1431af668,&DAT_14228a780,0,0,0,0,0);
    if (lVar3 == 0) {
      *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;
    }
    else {
      *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(lVar3 + 0xa8);
    }
    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));
    *(byte *)(lVar3 + 0xd4) = *(byte *)(lVar3 + 0xd4) | 4;
  }
  uVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));
  uVar4 = FUN_140877720(uVar4,&DAT_1439e2a90,0);
  *(undefined8 *)(param_1 + 0xb8) = uVar4;
  uVar4 = FUN_1408a8ba0(D2_GameSingletonPtr);
  uVar4 = FUN_140877720(uVar4,&DAT_1439e2b20,0);
  *(undefined8 *)(param_1 + 0xc0) = uVar4;
  return;
}


````

### text

````text

undefined4 FUN_1404473a0(void)

{
  longlong lVar1;
  
  lVar1 = FUN_1407e7380();
  return *(undefined4 *)(*(longlong *)(lVar1 + 0xad0) + 0xc);
}


````

### text

````text

void FUN_140446fe0(longlong param_1)

{
  FUN_1407e7b10(*(undefined8 *)(param_1 + 0xb8));
  *(undefined4 *)(param_1 + 0xf4) = *(undefined4 *)(*(longlong *)(param_1 + 8) + 0x1b0);
  FUN_14061bc20(param_1);
  return;
}


````

### text

````text

void FUN_140447760(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined1 auStack_28 [32];
  
  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));
  if (lVar1 != 0) {
    uVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));
    FUN_1408847f0(uVar2,auStack_28,&DAT_1439a6f30);
    FUN_140882fc0(auStack_28);
    *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;
  }
  *(undefined8 *)(param_1 + 0xc0) = 0;
  *(undefined8 *)(param_1 + 0xb8) = 0;
  FUN_14061c1a0(param_1);
  return;
}


````

### text

````text

void FUN_1404477d0(longlong param_1)

{
  int iVar1;
  longlong *plVar2;
  
  FUN_14061c1f0();
  plVar2 = (longlong *)FUN_1404bd8d0(*(undefined8 *)(param_1 + 0xb8));
  (**(code **)(*plVar2 + 0xd8))(plVar2,1);
  *(undefined4 *)(param_1 + 200) = 0x3fff0010;
  iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));
  *(uint *)(param_1 + 0xf8) = (uint)(iVar1 != 0);
  return;
}


````

### text

````text

void FUN_140447830(longlong param_1)

{
  longlong *plVar1;
  
  FUN_1404e3050(param_1 + 0xcc);
  if (((byte)*(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x1c) & 0xc) < 8) {
    plVar1 = (longlong *)FUN_1404bd8d0();
    (**(code **)(*plVar1 + 0xd8))(plVar1,0);
  }
  FUN_14061c2a0(param_1);
  return;
}


````

### text

````text

undefined8 FUN_1404473d0(longlong param_1)

{
  int iVar1;
  undefined8 uVar2;
  
  if (*(int *)(param_1 + 0xf8) != 1) {
    iVar1 = FUN_1404bd810(*(undefined8 *)(param_1 + 0xb8));
    if (iVar1 != 0) {
      uVar2 = FUN_14061c060(param_1);
      return uVar2;
    }
  }
  return 0;
}


````
