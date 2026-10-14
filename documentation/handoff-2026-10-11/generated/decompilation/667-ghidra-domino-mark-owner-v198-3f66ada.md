# ghidra-domino-mark-owner-v198.json

Original: [work/ghidra-domino-mark-owner-v198.json](<../../../../work/ghidra-domino-mark-owner-v198.json>)

SHA-256: `5C635BE53C8AE9E85DFAC0C583E05BF9F1BF54DB6F876FE1B80466CAEB87E011`; 6419 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "addresses": [
    "1404e2940",
    "1404bd860"
  ],
  "results": [
    {
      "status": "fulfilled",
      "value": {
        "content": [
          {
            "type": "text",
            "text": "\r\nlonglong FUN_1404e2940(int *param_1,longlong param_2,longlong param_3,undefined8 param_4,\r\n                      undefined4 param_5,undefined8 param_6,undefined8 param_7)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  int *piVar3;\r\n  undefined4 uVar4;\r\n  undefined1 local_res8 [8];\r\n  \r\n  if (*param_1 != -1) {\r\n    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    FUN_1401a3250(uVar1,*param_1);\r\n  }\r\n  if (param_2 != 0) {\r\n    uVar4 = *(undefined4 *)(param_3 + 0xa8);\r\n    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    lVar2 = FUN_14019d100(uVar1,uVar4,param_2);\r\n    if (lVar2 != 0) {\r\n      *(byte *)(lVar2 + 0x138) = *(byte *)(lVar2 + 0x138) | 4;\r\n      *(undefined4 *)(lVar2 + 0x38) = *(undefined4 *)(param_3 + 0x300);\r\n      *(undefined4 *)(lVar2 + 0x3c) = *(undefined4 *)(param_3 + 0x304);\r\n      *(undefined4 *)(lVar2 + 0x40) = *(undefined4 *)(param_3 + 0x308);\r\n      *(undefined4 *)(lVar2 + 0x44) = *(undefined4 *)(param_3 + 0x30c);\r\n      *(undefined4 *)(lVar2 + 0x48) = *(undefined4 *)(param_3 + 0x310);\r\n      *(undefined4 *)(lVar2 + 0x4c) = *(undefined4 *)(param_3 + 0x314);\r\n      *(undefined4 *)(lVar2 + 0x50) = *(undefined4 *)(param_3 + 0x318);\r\n      *(undefined4 *)(lVar2 + 0x54) = *(undefined4 *)(param_3 + 0x31c);\r\n      *(undefined4 *)(lVar2 + 0x58) = *(undefined4 *)(param_3 + 800);\r\n      *(undefined4 *)(lVar2 + 0x5c) = *(undefined4 *)(param_3 + 0x324);\r\n      *(undefined4 *)(lVar2 + 0x60) = *(undefined4 *)(param_3 + 0x328);\r\n      *(undefined4 *)(lVar2 + 100) = *(undefined4 *)(param_3 + 0x32c);\r\n      *(undefined4 *)(lVar2 + 0x68) = *(undefined4 *)(param_3 + 0x33c);\r\n      *(undefined4 *)(lVar2 + 0x6c) = *(undefined4 *)(param_3 + 0x340);\r\n      *(undefined4 *)(lVar2 + 0x70) = *(undefined4 *)(param_3 + 0x344);\r\n      *(undefined4 *)(lVar2 + 0x74) = *(undefined4 *)(param_3 + 0x348);\r\n      *(undefined4 *)(lVar2 + 0x78) = *(undefined4 *)(param_3 + 0x34c);\r\n      *(undefined4 *)(lVar2 + 0x7c) = *(undefined4 *)(param_3 + 0x350);\r\n      uVar4 = FUN_1408780a0(param_3);\r\n      *(undefined4 *)(lVar2 + 0x118) = param_5;\r\n      *(undefined4 *)(lVar2 + 0xf8) = uVar4;\r\n      *(undefined8 *)(lVar2 + 0x110) = param_6;\r\n      *(undefined8 *)(lVar2 + 0x108) = param_4;\r\n      *(undefined8 *)(lVar2 + 0x128) = param_7;\r\n      uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n      FUN_1401a2ff0(uVar1,lVar2,0,0x1ffff,0);\r\n      param_1[1] = *(int *)(param_3 + 0xa8);\r\n      piVar3 = (int *)FUN_140167a00(lVar2,local_res8);\r\n      *param_1 = *piVar3;\r\n      return lVar2;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nlonglong FUN_1404e2940(int *param_1,longlong param_2,longlong param_3,undefined8 param_4,\r\n                      undefined4 param_5,undefined8 param_6,undefined8 param_7)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  int *piVar3;\r\n  undefined4 uVar4;\r\n  undefined1 local_res8 [8];\r\n  \r\n  if (*param_1 != -1) {\r\n    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    FUN_1401a3250(uVar1,*param_1);\r\n  }\r\n  if (param_2 != 0) {\r\n    uVar4 = *(undefined4 *)(param_3 + 0xa8);\r\n    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    lVar2 = FUN_14019d100(uVar1,uVar4,param_2);\r\n    if (lVar2 != 0) {\r\n      *(byte *)(lVar2 + 0x138) = *(byte *)(lVar2 + 0x138) | 4;\r\n      *(undefined4 *)(lVar2 + 0x38) = *(undefined4 *)(param_3 + 0x300);\r\n      *(undefined4 *)(lVar2 + 0x3c) = *(undefined4 *)(param_3 + 0x304);\r\n      *(undefined4 *)(lVar2 + 0x40) = *(undefined4 *)(param_3 + 0x308);\r\n      *(undefined4 *)(lVar2 + 0x44) = *(undefined4 *)(param_3 + 0x30c);\r\n      *(undefined4 *)(lVar2 + 0x48) = *(undefined4 *)(param_3 + 0x310);\r\n      *(undefined4 *)(lVar2 + 0x4c) = *(undefined4 *)(param_3 + 0x314);\r\n      *(undefined4 *)(lVar2 + 0x50) = *(undefined4 *)(param_3 + 0x318);\r\n      *(undefined4 *)(lVar2 + 0x54) = *(undefined4 *)(param_3 + 0x31c);\r\n      *(undefined4 *)(lVar2 + 0x58) = *(undefined4 *)(param_3 + 800);\r\n      *(undefined4 *)(lVar2 + 0x5c) = *(undefined4 *)(param_3 + 0x324);\r\n      *(undefined4 *)(lVar2 + 0x60) = *(undefined4 *)(param_3 + 0x328);\r\n      *(undefined4 *)(lVar2 + 100) = *(undefined4 *)(param_3 + 0x32c);\r\n      *(undefined4 *)(lVar2 + 0x68) = *(undefined4 *)(param_3 + 0x33c);\r\n      *(undefined4 *)(lVar2 + 0x6c) = *(undefined4 *)(param_3 + 0x340);\r\n      *(undefined4 *)(lVar2 + 0x70) = *(undefined4 *)(param_3 + 0x344);\r\n      *(undefined4 *)(lVar2 + 0x74) = *(undefined4 *)(param_3 + 0x348);\r\n      *(undefined4 *)(lVar2 + 0x78) = *(undefined4 *)(param_3 + 0x34c);\r\n      *(undefined4 *)(lVar2 + 0x7c) = *(undefined4 *)(param_3 + 0x350);\r\n      uVar4 = FUN_1408780a0(param_3);\r\n      *(undefined4 *)(lVar2 + 0x118) = param_5;\r\n      *(undefined4 *)(lVar2 + 0xf8) = uVar4;\r\n      *(undefined8 *)(lVar2 + 0x110) = param_6;\r\n      *(undefined8 *)(lVar2 + 0x108) = param_4;\r\n      *(undefined8 *)(lVar2 + 0x128) = param_7;\r\n      uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n      FUN_1401a2ff0(uVar1,lVar2,0,0x1ffff,0);\r\n      param_1[1] = *(int *)(param_3 + 0xa8);\r\n      piVar3 = (int *)FUN_140167a00(lVar2,local_res8);\r\n      *param_1 = *piVar3;\r\n      return lVar2;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
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
            "text": "\r\nundefined8 FUN_1404bd860(void)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  return *(undefined8 *)(*(longlong *)(*(longlong *)(lVar1 + 0x3d0) + 0xb8) + 8);\r\n}\r\n\r\n"
          }
        ],
        "structuredContent": {
          "result": "\r\nundefined8 FUN_1404bd860(void)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  return *(undefined8 *)(*(longlong *)(*(longlong *)(lVar1 + 0x3d0) + 0xb8) + 8);\r\n}\r\n\r\n"
        },
        "isError": false
      }
    }
  ]
}

````

## Unescaped text fields for reading

### text

````text

longlong FUN_1404e2940(int *param_1,longlong param_2,longlong param_3,undefined8 param_4,
                      undefined4 param_5,undefined8 param_6,undefined8 param_7)

{
  undefined8 uVar1;
  longlong lVar2;
  int *piVar3;
  undefined4 uVar4;
  undefined1 local_res8 [8];
  
  if (*param_1 != -1) {
    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
    FUN_1401a3250(uVar1,*param_1);
  }
  if (param_2 != 0) {
    uVar4 = *(undefined4 *)(param_3 + 0xa8);
    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
    lVar2 = FUN_14019d100(uVar1,uVar4,param_2);
    if (lVar2 != 0) {
      *(byte *)(lVar2 + 0x138) = *(byte *)(lVar2 + 0x138) | 4;
      *(undefined4 *)(lVar2 + 0x38) = *(undefined4 *)(param_3 + 0x300);
      *(undefined4 *)(lVar2 + 0x3c) = *(undefined4 *)(param_3 + 0x304);
      *(undefined4 *)(lVar2 + 0x40) = *(undefined4 *)(param_3 + 0x308);
      *(undefined4 *)(lVar2 + 0x44) = *(undefined4 *)(param_3 + 0x30c);
      *(undefined4 *)(lVar2 + 0x48) = *(undefined4 *)(param_3 + 0x310);
      *(undefined4 *)(lVar2 + 0x4c) = *(undefined4 *)(param_3 + 0x314);
      *(undefined4 *)(lVar2 + 0x50) = *(undefined4 *)(param_3 + 0x318);
      *(undefined4 *)(lVar2 + 0x54) = *(undefined4 *)(param_3 + 0x31c);
      *(undefined4 *)(lVar2 + 0x58) = *(undefined4 *)(param_3 + 800);
      *(undefined4 *)(lVar2 + 0x5c) = *(undefined4 *)(param_3 + 0x324);
      *(undefined4 *)(lVar2 + 0x60) = *(undefined4 *)(param_3 + 0x328);
      *(undefined4 *)(lVar2 + 100) = *(undefined4 *)(param_3 + 0x32c);
      *(undefined4 *)(lVar2 + 0x68) = *(undefined4 *)(param_3 + 0x33c);
      *(undefined4 *)(lVar2 + 0x6c) = *(undefined4 *)(param_3 + 0x340);
      *(undefined4 *)(lVar2 + 0x70) = *(undefined4 *)(param_3 + 0x344);
      *(undefined4 *)(lVar2 + 0x74) = *(undefined4 *)(param_3 + 0x348);
      *(undefined4 *)(lVar2 + 0x78) = *(undefined4 *)(param_3 + 0x34c);
      *(undefined4 *)(lVar2 + 0x7c) = *(undefined4 *)(param_3 + 0x350);
      uVar4 = FUN_1408780a0(param_3);
      *(undefined4 *)(lVar2 + 0x118) = param_5;
      *(undefined4 *)(lVar2 + 0xf8) = uVar4;
      *(undefined8 *)(lVar2 + 0x110) = param_6;
      *(undefined8 *)(lVar2 + 0x108) = param_4;
      *(undefined8 *)(lVar2 + 0x128) = param_7;
      uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
      FUN_1401a2ff0(uVar1,lVar2,0,0x1ffff,0);
      param_1[1] = *(int *)(param_3 + 0xa8);
      piVar3 = (int *)FUN_140167a00(lVar2,local_res8);
      *param_1 = *piVar3;
      return lVar2;
    }
  }
  return 0;
}


````

### text

````text

undefined8 FUN_1404bd860(void)

{
  longlong lVar1;
  
  lVar1 = FUN_1408a8ba0(D2_GameSingletonPtr);
  return *(undefined8 *)(*(longlong *)(*(longlong *)(lVar1 + 0x3d0) + 0xb8) + 8);
}


````
