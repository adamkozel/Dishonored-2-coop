# throw-selector-1-decompilation-v2.json

Original: [work/natural-moveset/carry-research/throw-selector-1-decompilation-v2.json](<../../../../work/natural-moveset/carry-research/throw-selector-1-decompilation-v2.json>)

SHA-256: `1341EDD02C4F5D0B8F3A2DC643B0473115DD2A10071CEE92A2CF2084219C3E5F`; 4871 bytes.

## Original saved record

````json
{
  "status": "fulfilled",
  "value": {
    "content": [
      {
        "type": "text",
        "text": "\r\nvoid FUN_140831fe0(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  int iVar4;\r\n  longlong lVar5;\r\n  undefined8 uVar6;\r\n  longlong lVar7;\r\n  undefined4 *puVar8;\r\n  longlong lVar9;\r\n  longlong lStack_18;\r\n  code *pcStack_10;\r\n  \r\n  FUN_140853d40();\r\n  lVar7 = *(longlong *)(param_1 + 0x10);\r\n  lVar9 = 0;\r\n  lVar5 = lVar9;\r\n  if (*(longlong *)(lVar7 + 0x90) != 0) {\r\n    lVar5 = *(longlong *)(*(longlong *)(lVar7 + 0x90) + 0x178);\r\n  }\r\n  *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar5 + 0xf8);\r\n  lVar5 = FUN_1406dc560();\r\n  if (lVar5 == 0) {\r\n    FUN_1403dab80(param_1 + 0x40,0);\r\n  }\r\n  else {\r\n    uVar6 = FUN_140877720(lVar5,&DAT_1439b4630,2);\r\n    FUN_1403dab80(param_1 + 0x40,uVar6);\r\n    uVar1 = *(undefined4 *)(lVar5 + 0xa8);\r\n    uVar6 = FUN_140810240();\r\n    FUN_1408103f0(uVar6,0x1cc,uVar1);\r\n  }\r\n  lVar2 = *(longlong *)(lVar7 + 0x2f8);\r\n  if ((((byte)(*(uint *)(lVar2 + 0x88) >> 0x13) & (byte)(*(uint *)(lVar2 + 0x80) >> 0x13) & 1) != 0)\r\n     && (*(int *)(lVar2 + 0x224) == 0)) {\r\n    lVar9 = 1;\r\n  }\r\n  *(char *)(param_1 + 0x50) = (char)lVar9;\r\n  cVar3 = FUN_140832420(param_1);\r\n  if (cVar3 == '\\0') {\r\n    *(undefined1 *)(param_1 + 0x51) = 1;\r\n  }\r\n  else {\r\n    *(undefined1 *)(param_1 + 0x51) = 0;\r\n    FUN_140832770(param_1);\r\n  }\r\n  if (lVar5 != 0) {\r\n    iVar4 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,\r\n                          *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10));\r\n    if (iVar4 == 0) {\r\n      uVar1 = *(undefined4 *)(*(longlong *)(lVar7 + 0x10) + 0xa8);\r\n      lVar7 = FUN_140810240();\r\n      puVar8 = (undefined4 *)FUN_1408112c0(lVar7 + 0xc8e8,0x17b,4,uVar1);\r\n      if (puVar8 != (undefined4 *)0x0) {\r\n        *puVar8 = 4;\r\n      }\r\n      *(undefined1 *)(param_1 + 0x52) = 1;\r\n    }\r\n  }\r\n  pcStack_10 = FUN_1400998b0;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xb7,&lStack_18);\r\n  pcStack_10 = FUN_140832b10;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xb9,&lStack_18);\r\n  pcStack_10 = FUN_140832ad0;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xbb,&lStack_18);\r\n  return;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nvoid FUN_140831fe0(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  int iVar4;\r\n  longlong lVar5;\r\n  undefined8 uVar6;\r\n  longlong lVar7;\r\n  undefined4 *puVar8;\r\n  longlong lVar9;\r\n  longlong lStack_18;\r\n  code *pcStack_10;\r\n  \r\n  FUN_140853d40();\r\n  lVar7 = *(longlong *)(param_1 + 0x10);\r\n  lVar9 = 0;\r\n  lVar5 = lVar9;\r\n  if (*(longlong *)(lVar7 + 0x90) != 0) {\r\n    lVar5 = *(longlong *)(*(longlong *)(lVar7 + 0x90) + 0x178);\r\n  }\r\n  *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar5 + 0xf8);\r\n  lVar5 = FUN_1406dc560();\r\n  if (lVar5 == 0) {\r\n    FUN_1403dab80(param_1 + 0x40,0);\r\n  }\r\n  else {\r\n    uVar6 = FUN_140877720(lVar5,&DAT_1439b4630,2);\r\n    FUN_1403dab80(param_1 + 0x40,uVar6);\r\n    uVar1 = *(undefined4 *)(lVar5 + 0xa8);\r\n    uVar6 = FUN_140810240();\r\n    FUN_1408103f0(uVar6,0x1cc,uVar1);\r\n  }\r\n  lVar2 = *(longlong *)(lVar7 + 0x2f8);\r\n  if ((((byte)(*(uint *)(lVar2 + 0x88) >> 0x13) & (byte)(*(uint *)(lVar2 + 0x80) >> 0x13) & 1) != 0)\r\n     && (*(int *)(lVar2 + 0x224) == 0)) {\r\n    lVar9 = 1;\r\n  }\r\n  *(char *)(param_1 + 0x50) = (char)lVar9;\r\n  cVar3 = FUN_140832420(param_1);\r\n  if (cVar3 == '\\0') {\r\n    *(undefined1 *)(param_1 + 0x51) = 1;\r\n  }\r\n  else {\r\n    *(undefined1 *)(param_1 + 0x51) = 0;\r\n    FUN_140832770(param_1);\r\n  }\r\n  if (lVar5 != 0) {\r\n    iVar4 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,\r\n                          *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10));\r\n    if (iVar4 == 0) {\r\n      uVar1 = *(undefined4 *)(*(longlong *)(lVar7 + 0x10) + 0xa8);\r\n      lVar7 = FUN_140810240();\r\n      puVar8 = (undefined4 *)FUN_1408112c0(lVar7 + 0xc8e8,0x17b,4,uVar1);\r\n      if (puVar8 != (undefined4 *)0x0) {\r\n        *puVar8 = 4;\r\n      }\r\n      *(undefined1 *)(param_1 + 0x52) = 1;\r\n    }\r\n  }\r\n  pcStack_10 = FUN_1400998b0;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xb7,&lStack_18);\r\n  pcStack_10 = FUN_140832b10;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xb9,&lStack_18);\r\n  pcStack_10 = FUN_140832ad0;\r\n  lStack_18 = param_1;\r\n  uVar6 = FUN_140810240();\r\n  FUN_1408106f0(uVar6,0xbb,&lStack_18);\r\n  return;\r\n}\r\n\r\n"
    },
    "isError": false
  }
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140831fe0(longlong param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  undefined8 uVar6;
  longlong lVar7;
  undefined4 *puVar8;
  longlong lVar9;
  longlong lStack_18;
  code *pcStack_10;
  
  FUN_140853d40();
  lVar7 = *(longlong *)(param_1 + 0x10);
  lVar9 = 0;
  lVar5 = lVar9;
  if (*(longlong *)(lVar7 + 0x90) != 0) {
    lVar5 = *(longlong *)(*(longlong *)(lVar7 + 0x90) + 0x178);
  }
  *(undefined4 *)(param_1 + 0x38) = *(undefined4 *)(lVar5 + 0xf8);
  lVar5 = FUN_1406dc560();
  if (lVar5 == 0) {
    FUN_1403dab80(param_1 + 0x40,0);
  }
  else {
    uVar6 = FUN_140877720(lVar5,&DAT_1439b4630,2);
    FUN_1403dab80(param_1 + 0x40,uVar6);
    uVar1 = *(undefined4 *)(lVar5 + 0xa8);
    uVar6 = FUN_140810240();
    FUN_1408103f0(uVar6,0x1cc,uVar1);
  }
  lVar2 = *(longlong *)(lVar7 + 0x2f8);
  if ((((byte)(*(uint *)(lVar2 + 0x88) >> 0x13) & (byte)(*(uint *)(lVar2 + 0x80) >> 0x13) & 1) != 0)
     && (*(int *)(lVar2 + 0x224) == 0)) {
    lVar9 = 1;
  }
  *(char *)(param_1 + 0x50) = (char)lVar9;
  cVar3 = FUN_140832420(param_1);
  if (cVar3 == '\0') {
    *(undefined1 *)(param_1 + 0x51) = 1;
  }
  else {
    *(undefined1 *)(param_1 + 0x51) = 0;
    FUN_140832770(param_1);
  }
  if (lVar5 != 0) {
    iVar4 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,
                          *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10));
    if (iVar4 == 0) {
      uVar1 = *(undefined4 *)(*(longlong *)(lVar7 + 0x10) + 0xa8);
      lVar7 = FUN_140810240();
      puVar8 = (undefined4 *)FUN_1408112c0(lVar7 + 0xc8e8,0x17b,4,uVar1);
      if (puVar8 != (undefined4 *)0x0) {
        *puVar8 = 4;
      }
      *(undefined1 *)(param_1 + 0x52) = 1;
    }
  }
  pcStack_10 = FUN_1400998b0;
  lStack_18 = param_1;
  uVar6 = FUN_140810240();
  FUN_1408106f0(uVar6,0xb7,&lStack_18);
  pcStack_10 = FUN_140832b10;
  lStack_18 = param_1;
  uVar6 = FUN_140810240();
  FUN_1408106f0(uVar6,0xb9,&lStack_18);
  pcStack_10 = FUN_140832ad0;
  lStack_18 = param_1;
  uVar6 = FUN_140810240();
  FUN_1408106f0(uVar6,0xbb,&lStack_18);
  return;
}


````
