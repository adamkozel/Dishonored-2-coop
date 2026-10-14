# native-throw-140734a10-decompilation-v32.json

Original: [work/natural-moveset/carry-research/native-throw-140734a10-decompilation-v32.json](<../../../../work/natural-moveset/carry-research/native-throw-140734a10-decompilation-v32.json>)

SHA-256: `D4F610D3D0ADB687878B98AD5E91A945A484BB1B4B8F00CDBCEE24BFC2079360`; 2953 bytes.

## Original saved record

````json
{
  "status": "fulfilled",
  "value": {
    "content": [
      {
        "type": "text",
        "text": "\r\nvoid FUN_140734a10(longlong *param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  int iVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  undefined4 *puVar6;\r\n  \r\n  if ((*(byte *)((longlong)param_1 + 0x241) & 8) == 0) {\r\n    *(undefined4 *)((longlong)param_1 + 0x23c) = 0;\r\n    (**(code **)(*param_1 + 0x1c0))();\r\n    lVar4 = FUN_1403da910(param_1 + 0x42,0);\r\n    if ((lVar4 != 0) && ((*(byte *)(lVar4 + 0x151) & 1) != 0)) {\r\n      FUN_140853220(D2_GameSingletonPtr + 0x60280,param_1,1,0);\r\n    }\r\n    lVar2 = D2_GameSingletonPtr;\r\n    lVar4 = param_1[2];\r\n    if ((*(byte *)(lVar4 + 0xd1) & 8) == 0) {\r\n      uVar5 = FUN_1405bef90();\r\n      iVar3 = FUN_140604bd0(lVar2 + 0x1f7bb8,lVar4,uVar5);\r\n      if (iVar3 == 0) {\r\n        if (*(float *)((longlong)param_1 + 0x3dc) == 0.0) {\r\n          uVar1 = *(undefined4 *)(lVar4 + 0xa8);\r\n          lVar4 = FUN_140810240();\r\n          puVar6 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17b,4,uVar1);\r\n          if (puVar6 != (undefined4 *)0x0) {\r\n            *puVar6 = 10;\r\n          }\r\n        }\r\n        *(undefined4 *)((longlong)param_1 + 0x3dc) = *(undefined4 *)(param_1[1] + 300);\r\n      }\r\n    }\r\n    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);\r\n    uVar5 = FUN_140810240();\r\n    FUN_1408103f0(uVar5,0xbb,uVar1);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\nvoid FUN_140734a10(longlong *param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  int iVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  undefined4 *puVar6;\r\n  \r\n  if ((*(byte *)((longlong)param_1 + 0x241) & 8) == 0) {\r\n    *(undefined4 *)((longlong)param_1 + 0x23c) = 0;\r\n    (**(code **)(*param_1 + 0x1c0))();\r\n    lVar4 = FUN_1403da910(param_1 + 0x42,0);\r\n    if ((lVar4 != 0) && ((*(byte *)(lVar4 + 0x151) & 1) != 0)) {\r\n      FUN_140853220(D2_GameSingletonPtr + 0x60280,param_1,1,0);\r\n    }\r\n    lVar2 = D2_GameSingletonPtr;\r\n    lVar4 = param_1[2];\r\n    if ((*(byte *)(lVar4 + 0xd1) & 8) == 0) {\r\n      uVar5 = FUN_1405bef90();\r\n      iVar3 = FUN_140604bd0(lVar2 + 0x1f7bb8,lVar4,uVar5);\r\n      if (iVar3 == 0) {\r\n        if (*(float *)((longlong)param_1 + 0x3dc) == 0.0) {\r\n          uVar1 = *(undefined4 *)(lVar4 + 0xa8);\r\n          lVar4 = FUN_140810240();\r\n          puVar6 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17b,4,uVar1);\r\n          if (puVar6 != (undefined4 *)0x0) {\r\n            *puVar6 = 10;\r\n          }\r\n        }\r\n        *(undefined4 *)((longlong)param_1 + 0x3dc) = *(undefined4 *)(param_1[1] + 300);\r\n      }\r\n    }\r\n    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);\r\n    uVar5 = FUN_140810240();\r\n    FUN_1408103f0(uVar5,0xbb,uVar1);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "isError": false
  }
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140734a10(longlong *param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined4 *puVar6;
  
  if ((*(byte *)((longlong)param_1 + 0x241) & 8) == 0) {
    *(undefined4 *)((longlong)param_1 + 0x23c) = 0;
    (**(code **)(*param_1 + 0x1c0))();
    lVar4 = FUN_1403da910(param_1 + 0x42,0);
    if ((lVar4 != 0) && ((*(byte *)(lVar4 + 0x151) & 1) != 0)) {
      FUN_140853220(D2_GameSingletonPtr + 0x60280,param_1,1,0);
    }
    lVar2 = D2_GameSingletonPtr;
    lVar4 = param_1[2];
    if ((*(byte *)(lVar4 + 0xd1) & 8) == 0) {
      uVar5 = FUN_1405bef90();
      iVar3 = FUN_140604bd0(lVar2 + 0x1f7bb8,lVar4,uVar5);
      if (iVar3 == 0) {
        if (*(float *)((longlong)param_1 + 0x3dc) == 0.0) {
          uVar1 = *(undefined4 *)(lVar4 + 0xa8);
          lVar4 = FUN_140810240();
          puVar6 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17b,4,uVar1);
          if (puVar6 != (undefined4 *)0x0) {
            *puVar6 = 10;
          }
        }
        *(undefined4 *)((longlong)param_1 + 0x3dc) = *(undefined4 *)(param_1[1] + 300);
      }
    }
    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);
    uVar5 = FUN_140810240();
    FUN_1408103f0(uVar5,0xbb,uVar1);
  }
  return;
}


````
