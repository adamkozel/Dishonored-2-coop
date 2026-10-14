# ghidra-mesmerize-world-states-v73.json

Original: [work/ghidra-mesmerize-world-states-v73.json](<../../../../work/ghidra-mesmerize-world-states-v73.json>)

SHA-256: `B39A1FAE0A9F52CEAEEDBA1589FB920D0961D4F1801A5C7740D50F8F7DDDD70B`; 8741 bytes.

## Original saved record

````json
[
  {
    "address": "140728f50",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140728f50(longlong param_1)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  undefined1 auStackX_8 [32];\r\n  undefined1 auStack_28 [32];\r\n  \r\n  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xb0) != 0) {\r\n    puVar1 = (undefined4 *)FUN_14072af30(param_1,auStackX_8);\r\n    if ((undefined4 *)(param_1 + 0x1e4) != puVar1) {\r\n      *(undefined4 *)(param_1 + 0x1e4) = *puVar1;\r\n    }\r\n  }\r\n  if (*(int *)(param_1 + 0x1e4) == 0) {\r\n    FUN_1408847f0(*(undefined8 *)(param_1 + 0x10),auStack_28,&DAT_1439a6f30);\r\n    FUN_140882fc0(auStack_28);\r\n  }\r\n  *(uint *)(param_1 + 0x180) = *(uint *)(param_1 + 0x180) & 0xff000000;\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140728f50(longlong param_1)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  undefined1 auStackX_8 [32];\r\n  undefined1 auStack_28 [32];\r\n  \r\n  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xb0) != 0) {\r\n    puVar1 = (undefined4 *)FUN_14072af30(param_1,auStackX_8);\r\n    if ((undefined4 *)(param_1 + 0x1e4) != puVar1) {\r\n      *(undefined4 *)(param_1 + 0x1e4) = *puVar1;\r\n    }\r\n  }\r\n  if (*(int *)(param_1 + 0x1e4) == 0) {\r\n    FUN_1408847f0(*(undefined8 *)(param_1 + 0x10),auStack_28,&DAT_1439a6f30);\r\n    FUN_140882fc0(auStack_28);\r\n  }\r\n  *(uint *)(param_1 + 0x180) = *(uint *)(param_1 + 0x180) & 0xff000000;\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "140728fc0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140728fc0(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 *puVar2;\r\n  undefined8 uVar3;\r\n  undefined1 *puStackX_8;\r\n  undefined1 auStack_50 [8];\r\n  undefined4 uStack_48;\r\n  undefined1 auStack_38 [8];\r\n  undefined4 uStack_30;\r\n  undefined1 auStack_20 [24];\r\n  \r\n  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xa8) != 0) {\r\n    puVar2 = (undefined4 *)FUN_14072af30(param_1,&puStackX_8);\r\n    if ((undefined4 *)(param_1 + 0x1e0) != puVar2) {\r\n      *(undefined4 *)(param_1 + 0x1e0) = *puVar2;\r\n    }\r\n  }\r\n  puStackX_8 = auStack_50;\r\n  auStack_50[0] = 0x69;\r\n  uStack_48 = 1;\r\n  auStack_38[0] = 0x69;\r\n  uStack_30 = 4;\r\n  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  FUN_1408848a0(uVar3,auStack_20,&DAT_1439ab620,auStack_38,auStack_50);\r\n  FUN_140882fc0(auStack_20);\r\n  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n  uVar3 = FUN_140810240();\r\n  FUN_1408103f0(uVar3,0x109,uVar1);\r\n  *(undefined4 *)(param_1 + 0x128) = 0x3f800000;\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140728fc0(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 *puVar2;\r\n  undefined8 uVar3;\r\n  undefined1 *puStackX_8;\r\n  undefined1 auStack_50 [8];\r\n  undefined4 uStack_48;\r\n  undefined1 auStack_38 [8];\r\n  undefined4 uStack_30;\r\n  undefined1 auStack_20 [24];\r\n  \r\n  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xa8) != 0) {\r\n    puVar2 = (undefined4 *)FUN_14072af30(param_1,&puStackX_8);\r\n    if ((undefined4 *)(param_1 + 0x1e0) != puVar2) {\r\n      *(undefined4 *)(param_1 + 0x1e0) = *puVar2;\r\n    }\r\n  }\r\n  puStackX_8 = auStack_50;\r\n  auStack_50[0] = 0x69;\r\n  uStack_48 = 1;\r\n  auStack_38[0] = 0x69;\r\n  uStack_30 = 4;\r\n  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  FUN_1408848a0(uVar3,auStack_20,&DAT_1439ab620,auStack_38,auStack_50);\r\n  FUN_140882fc0(auStack_20);\r\n  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n  uVar3 = FUN_140810240();\r\n  FUN_1408103f0(uVar3,0x109,uVar1);\r\n  *(undefined4 *)(param_1 + 0x128) = 0x3f800000;\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407290c0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_1407290c0(longlong param_1)\r\n\r\n{\r\n  byte *pbVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  undefined4 *puVar4;\r\n  undefined4 auStackX_8 [8];\r\n  \r\n  FUN_14072c770();\r\n  (**(code **)(**(longlong **)(param_1 + 0x10) + 0x88))();\r\n  lVar2 = *(longlong *)(*(longlong *)(param_1 + 8) + 200);\r\n  if (lVar2 != 0) {\r\n    uVar3 = FUN_1404e2940(param_1 + 0xb4,lVar2,*(undefined8 *)(param_1 + 0x10),0,0,0,0);\r\n    *(undefined8 *)(param_1 + 0xe0) = uVar3;\r\n    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd2);\r\n    *pbVar1 = *pbVar1 | 6;\r\n  }\r\n  if (*(int *)(param_1 + 0x294) < 0) {\r\n    auStackX_8[0] = 0;\r\n  }\r\n  else {\r\n    auStackX_8[0] =\r\n         *(undefined4 *)\r\n          (*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n          (longlong)*(int *)(param_1 + 0x294) * 0x28);\r\n  }\r\n  *(undefined1 *)(param_1 + 0x2b0) = 0;\r\n  *(undefined4 *)(param_1 + 0x1e8) = auStackX_8[0];\r\n  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xa0) != 0) {\r\n    puVar4 = (undefined4 *)FUN_14072af30(param_1,auStackX_8);\r\n    if ((undefined4 *)(param_1 + 0x1dc) != puVar4) {\r\n      *(undefined4 *)(param_1 + 0x1dc) = *puVar4;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_1407290c0(longlong param_1)\r\n\r\n{\r\n  byte *pbVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  undefined4 *puVar4;\r\n  undefined4 auStackX_8 [8];\r\n  \r\n  FUN_14072c770();\r\n  (**(code **)(**(longlong **)(param_1 + 0x10) + 0x88))();\r\n  lVar2 = *(longlong *)(*(longlong *)(param_1 + 8) + 200);\r\n  if (lVar2 != 0) {\r\n    uVar3 = FUN_1404e2940(param_1 + 0xb4,lVar2,*(undefined8 *)(param_1 + 0x10),0,0,0,0);\r\n    *(undefined8 *)(param_1 + 0xe0) = uVar3;\r\n    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd2);\r\n    *pbVar1 = *pbVar1 | 6;\r\n  }\r\n  if (*(int *)(param_1 + 0x294) < 0) {\r\n    auStackX_8[0] = 0;\r\n  }\r\n  else {\r\n    auStackX_8[0] =\r\n         *(undefined4 *)\r\n          (*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n          (longlong)*(int *)(param_1 + 0x294) * 0x28);\r\n  }\r\n  *(undefined1 *)(param_1 + 0x2b0) = 0;\r\n  *(undefined4 *)(param_1 + 0x1e8) = auStackX_8[0];\r\n  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xa0) != 0) {\r\n    puVar4 = (undefined4 *)FUN_14072af30(param_1,auStackX_8);\r\n    if ((undefined4 *)(param_1 + 0x1dc) != puVar4) {\r\n      *(undefined4 *)(param_1 + 0x1dc) = *puVar4;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "14072f820",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14072f820(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  code *pcVar2;\r\n  char cVar3;\r\n  \r\n  if ((*(int *)(param_1 + 0x1dc) != 0) &&\r\n     (cVar3 = FUN_1407d6450(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x1dc)),\r\n     cVar3 == '\\0')) {\r\n    return;\r\n  }\r\n  *(undefined4 *)(param_1 + 0x1dc) = 0;\r\n  iVar1 = *(int *)(param_1 + 0x2c);\r\n  if (iVar1 != 3) {\r\n    if ((iVar1 != *(int *)(param_1 + 0x28)) &&\r\n       (pcVar2 = *(code **)(param_1 + 0x20 + ((longlong)iVar1 + 1) * 0x18), pcVar2 != (code *)0x0))\r\n    {\r\n      (*pcVar2)(*(undefined8 *)(param_1 + 0x20),3);\r\n    }\r\n    if ((*(int *)(param_1 + 0x28) != 3) && (*(code **)(param_1 + 0x78) != (code *)0x0)) {\r\n      (**(code **)(param_1 + 0x78))(*(undefined8 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x2c))\r\n      ;\r\n    }\r\n    *(undefined4 *)(param_1 + 0x2c) = 3;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14072f820(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  code *pcVar2;\r\n  char cVar3;\r\n  \r\n  if ((*(int *)(param_1 + 0x1dc) != 0) &&\r\n     (cVar3 = FUN_1407d6450(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x1dc)),\r\n     cVar3 == '\\0')) {\r\n    return;\r\n  }\r\n  *(undefined4 *)(param_1 + 0x1dc) = 0;\r\n  iVar1 = *(int *)(param_1 + 0x2c);\r\n  if (iVar1 != 3) {\r\n    if ((iVar1 != *(int *)(param_1 + 0x28)) &&\r\n       (pcVar2 = *(code **)(param_1 + 0x20 + ((longlong)iVar1 + 1) * 0x18), pcVar2 != (code *)0x0))\r\n    {\r\n      (*pcVar2)(*(undefined8 *)(param_1 + 0x20),3);\r\n    }\r\n    if ((*(int *)(param_1 + 0x28) != 3) && (*(code **)(param_1 + 0x78) != (code *)0x0)) {\r\n      (**(code **)(param_1 + 0x78))(*(undefined8 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x2c))\r\n      ;\r\n    }\r\n    *(undefined4 *)(param_1 + 0x2c) = 3;\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_140728f50(longlong param_1)

{
  undefined4 *puVar1;
  undefined1 auStackX_8 [32];
  undefined1 auStack_28 [32];
  
  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xb0) != 0) {
    puVar1 = (undefined4 *)FUN_14072af30(param_1,auStackX_8);
    if ((undefined4 *)(param_1 + 0x1e4) != puVar1) {
      *(undefined4 *)(param_1 + 0x1e4) = *puVar1;
    }
  }
  if (*(int *)(param_1 + 0x1e4) == 0) {
    FUN_1408847f0(*(undefined8 *)(param_1 + 0x10),auStack_28,&DAT_1439a6f30);
    FUN_140882fc0(auStack_28);
  }
  *(uint *)(param_1 + 0x180) = *(uint *)(param_1 + 0x180) & 0xff000000;
  return;
}


````

### text

````text

void FUN_140728fc0(longlong param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined8 uVar3;
  undefined1 *puStackX_8;
  undefined1 auStack_50 [8];
  undefined4 uStack_48;
  undefined1 auStack_38 [8];
  undefined4 uStack_30;
  undefined1 auStack_20 [24];
  
  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xa8) != 0) {
    puVar2 = (undefined4 *)FUN_14072af30(param_1,&puStackX_8);
    if ((undefined4 *)(param_1 + 0x1e0) != puVar2) {
      *(undefined4 *)(param_1 + 0x1e0) = *puVar2;
    }
  }
  puStackX_8 = auStack_50;
  auStack_50[0] = 0x69;
  uStack_48 = 1;
  auStack_38[0] = 0x69;
  uStack_30 = 4;
  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);
  FUN_1408848a0(uVar3,auStack_20,&DAT_1439ab620,auStack_38,auStack_50);
  FUN_140882fc0(auStack_20);
  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
  uVar3 = FUN_140810240();
  FUN_1408103f0(uVar3,0x109,uVar1);
  *(undefined4 *)(param_1 + 0x128) = 0x3f800000;
  return;
}


````

### text

````text

void FUN_1407290c0(longlong param_1)

{
  byte *pbVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined4 *puVar4;
  undefined4 auStackX_8 [8];
  
  FUN_14072c770();
  (**(code **)(**(longlong **)(param_1 + 0x10) + 0x88))();
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 8) + 200);
  if (lVar2 != 0) {
    uVar3 = FUN_1404e2940(param_1 + 0xb4,lVar2,*(undefined8 *)(param_1 + 0x10),0,0,0,0);
    *(undefined8 *)(param_1 + 0xe0) = uVar3;
    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd2);
    *pbVar1 = *pbVar1 | 6;
  }
  if (*(int *)(param_1 + 0x294) < 0) {
    auStackX_8[0] = 0;
  }
  else {
    auStackX_8[0] =
         *(undefined4 *)
          (*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +
          (longlong)*(int *)(param_1 + 0x294) * 0x28);
  }
  *(undefined1 *)(param_1 + 0x2b0) = 0;
  *(undefined4 *)(param_1 + 0x1e8) = auStackX_8[0];
  if (*(longlong *)(*(longlong *)(param_1 + 8) + 0xa0) != 0) {
    puVar4 = (undefined4 *)FUN_14072af30(param_1,auStackX_8);
    if ((undefined4 *)(param_1 + 0x1dc) != puVar4) {
      *(undefined4 *)(param_1 + 0x1dc) = *puVar4;
    }
  }
  return;
}


````

### text

````text

void FUN_14072f820(longlong param_1)

{
  int iVar1;
  code *pcVar2;
  char cVar3;
  
  if ((*(int *)(param_1 + 0x1dc) != 0) &&
     (cVar3 = FUN_1407d6450(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(param_1 + 0x1dc)),
     cVar3 == '\0')) {
    return;
  }
  *(undefined4 *)(param_1 + 0x1dc) = 0;
  iVar1 = *(int *)(param_1 + 0x2c);
  if (iVar1 != 3) {
    if ((iVar1 != *(int *)(param_1 + 0x28)) &&
       (pcVar2 = *(code **)(param_1 + 0x20 + ((longlong)iVar1 + 1) * 0x18), pcVar2 != (code *)0x0))
    {
      (*pcVar2)(*(undefined8 *)(param_1 + 0x20),3);
    }
    if ((*(int *)(param_1 + 0x28) != 3) && (*(code **)(param_1 + 0x78) != (code *)0x0)) {
      (**(code **)(param_1 + 0x78))(*(undefined8 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x2c))
      ;
    }
    *(undefined4 *)(param_1 + 0x2c) = 3;
  }
  return;
}


````
