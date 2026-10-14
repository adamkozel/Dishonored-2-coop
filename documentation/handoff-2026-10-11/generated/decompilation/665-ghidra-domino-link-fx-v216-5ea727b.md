# ghidra-domino-link-fx-v216.json

Original: [work/ghidra-domino-link-fx-v216.json](<../../../../work/ghidra-domino-link-fx-v216.json>)

SHA-256: `560E5080CB6EEB2A1688C698EE18CB1761AB7A00D93DD8C4BCCEA49FDDA1BE80`; 5734 bytes.

## Original saved record

````json
{
  "program": "Dishonored2.exe",
  "exeSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "functions": [
    {
      "address": "1404dd500",
      "pseudocode": "\r\nvoid FUN_1404dd500(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  longlong lVar4;\r\n  undefined1 local_88 [16];\r\n  float local_78;\r\n  float fStack_74;\r\n  float local_70;\r\n  float fStack_6c;\r\n  float local_68;\r\n  float fStack_64;\r\n  undefined8 local_60;\r\n  undefined4 local_58;\r\n  float local_50;\r\n  float fStack_4c;\r\n  undefined8 local_48;\r\n  undefined8 local_40;\r\n  undefined8 local_38;\r\n  undefined4 local_30;\r\n  \r\n  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  lVar4 = FUN_1404bd860();\r\n  lVar1 = *(longlong *)(param_1 + 8);\r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  FUN_1404db9e0(param_1,local_88);\r\n  FUN_1404db8f0(param_1,&local_50);\r\n  local_60 = local_38;\r\n  _local_78 = CONCAT44(fStack_4c * -1.0,local_50 * -1.0);\r\n  fStack_6c = (float)((ulonglong)local_48 >> 0x20);\r\n  local_70 = (float)local_48;\r\n  fStack_64 = (float)((ulonglong)local_40 >> 0x20);\r\n  _local_70 = CONCAT44(fStack_6c * -1.0,local_70 * -1.0);\r\n  local_58 = local_30;\r\n  local_68 = (float)local_40;\r\n  _local_68 = CONCAT44(fStack_64 * -1.0,local_68 * -1.0);\r\n  FUN_1404e2de0(param_1 + 0x2c,*(undefined8 *)(lVar4 + 0xb0),uVar3,lVar1 + 0x38,&local_50,local_88,\r\n                &local_78,0,0,0,0,0,0);\r\n  FUN_1404e2de0(param_1 + 0x54,*(undefined8 *)(lVar4 + 0xa8),uVar3,lVar1 + 0x38,lVar1 + 0x44,\r\n                local_88,lVar2 + 0x44,0,0,0,0,0,0);\r\n  return;\r\n}\r\n\r\n"
    },
    {
      "address": "1404e2de0",
      "pseudocode": "\r\nundefined8\r\nFUN_1404e2de0(int *param_1,longlong param_2,longlong param_3,undefined4 *param_4,undefined4 *param_5\r\n             ,undefined4 *param_6,undefined4 *param_7,undefined4 *param_8,undefined4 *param_9,\r\n             undefined8 param_10,undefined4 param_11,undefined8 param_12,undefined8 param_13)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  int *piVar3;\r\n  undefined4 uVar4;\r\n  undefined1 local_res8 [8];\r\n  \r\n  if (*param_1 != -1) {\r\n    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    FUN_1401a3250(uVar1,*param_1);\r\n  }\r\n  if (param_2 != 0) {\r\n    uVar4 = *(undefined4 *)(param_3 + 0xa8);\r\n    uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    lVar2 = FUN_14019d100(uVar1,uVar4,param_2);\r\n    if (lVar2 != 0) {\r\n      *(byte *)(lVar2 + 0x138) = *(byte *)(lVar2 + 0x138) & 0xfb;\r\n      if (param_4 == (undefined4 *)0x0) {\r\n        param_4 = (undefined4 *)(param_3 + 0x300);\r\n      }\r\n      *(undefined4 *)(lVar2 + 0x38) = *param_4;\r\n      *(undefined4 *)(lVar2 + 0x3c) = param_4[1];\r\n      *(undefined4 *)(lVar2 + 0x40) = param_4[2];\r\n      if (param_5 == (undefined4 *)0x0) {\r\n        param_5 = (undefined4 *)(param_3 + 0x30c);\r\n      }\r\n      *(undefined4 *)(lVar2 + 0x44) = *param_5;\r\n      *(undefined4 *)(lVar2 + 0x48) = param_5[1];\r\n      *(undefined4 *)(lVar2 + 0x4c) = param_5[2];\r\n      *(undefined4 *)(lVar2 + 0x50) = param_5[3];\r\n      *(undefined4 *)(lVar2 + 0x54) = param_5[4];\r\n      *(undefined4 *)(lVar2 + 0x58) = param_5[5];\r\n      *(undefined4 *)(lVar2 + 0x5c) = param_5[6];\r\n      *(undefined4 *)(lVar2 + 0x60) = param_5[7];\r\n      *(undefined4 *)(lVar2 + 100) = param_5[8];\r\n      if (param_6 == (undefined4 *)0x0) {\r\n        param_6 = (undefined4 *)(param_3 + 0x300);\r\n      }\r\n      *(undefined4 *)(lVar2 + 0x80) = *param_6;\r\n      *(undefined4 *)(lVar2 + 0x84) = param_6[1];\r\n      *(undefined4 *)(lVar2 + 0x88) = param_6[2];\r\n      if (param_7 == (undefined4 *)0x0) {\r\n        param_7 = (undefined4 *)(param_3 + 0x30c);\r\n      }\r\n      *(undefined4 *)(lVar2 + 0x8c) = *param_7;\r\n      *(undefined4 *)(lVar2 + 0x90) = param_7[1];\r\n      *(undefined4 *)(lVar2 + 0x94) = param_7[2];\r\n      *(undefined4 *)(lVar2 + 0x98) = param_7[3];\r\n      *(undefined4 *)(lVar2 + 0x9c) = param_7[4];\r\n      *(undefined4 *)(lVar2 + 0xa0) = param_7[5];\r\n      *(undefined4 *)(lVar2 + 0xa4) = param_7[6];\r\n      *(undefined4 *)(lVar2 + 0xa8) = param_7[7];\r\n      *(undefined4 *)(lVar2 + 0xac) = param_7[8];\r\n      if (param_8 == (undefined4 *)0x0) {\r\n        param_8 = (undefined4 *)(param_3 + 0x33c);\r\n      }\r\n      *(undefined4 *)(lVar2 + 0x68) = *param_8;\r\n      *(undefined4 *)(lVar2 + 0x6c) = param_8[1];\r\n      *(undefined4 *)(lVar2 + 0x70) = param_8[2];\r\n      if (param_9 == (undefined4 *)0x0) {\r\n        param_9 = (undefined4 *)(param_3 + 0x348);\r\n      }\r\n      *(undefined4 *)(lVar2 + 0x74) = *param_9;\r\n      *(undefined4 *)(lVar2 + 0x78) = param_9[1];\r\n      *(undefined4 *)(lVar2 + 0x7c) = param_9[2];\r\n      uVar4 = FUN_1408780a0(param_3);\r\n      *(undefined4 *)(lVar2 + 0x118) = param_11;\r\n      *(undefined4 *)(lVar2 + 0xf8) = uVar4;\r\n      *(undefined8 *)(lVar2 + 0x108) = param_10;\r\n      *(undefined8 *)(lVar2 + 0x110) = param_12;\r\n      *(undefined8 *)(lVar2 + 0x128) = param_13;\r\n      uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n      FUN_1401a2ff0(uVar1,lVar2,0,0x1ffff,0);\r\n      param_1[1] = *(int *)(param_3 + 0xa8);\r\n      piVar3 = (int *)FUN_140167a00(lVar2,local_res8);\r\n      *param_1 = *piVar3;\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
    },
    {
      "address": "1404bd860",
      "pseudocode": "\r\nundefined8 FUN_1404bd860(void)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  return *(undefined8 *)(*(longlong *)(*(longlong *)(lVar1 + 0x3d0) + 0xb8) + 8);\r\n}\r\n\r\n"
    }
  ]
}

````

## Unescaped text fields for reading

### pseudocode

````text

void FUN_1404dd500(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined1 local_88 [16];
  float local_78;
  float fStack_74;
  float local_70;
  float fStack_6c;
  float local_68;
  float fStack_64;
  undefined8 local_60;
  undefined4 local_58;
  float local_50;
  float fStack_4c;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 local_38;
  undefined4 local_30;
  
  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);
  lVar4 = FUN_1404bd860();
  lVar1 = *(longlong *)(param_1 + 8);
  lVar2 = *(longlong *)(param_1 + 0x10);
  FUN_1404db9e0(param_1,local_88);
  FUN_1404db8f0(param_1,&local_50);
  local_60 = local_38;
  _local_78 = CONCAT44(fStack_4c * -1.0,local_50 * -1.0);
  fStack_6c = (float)((ulonglong)local_48 >> 0x20);
  local_70 = (float)local_48;
  fStack_64 = (float)((ulonglong)local_40 >> 0x20);
  _local_70 = CONCAT44(fStack_6c * -1.0,local_70 * -1.0);
  local_58 = local_30;
  local_68 = (float)local_40;
  _local_68 = CONCAT44(fStack_64 * -1.0,local_68 * -1.0);
  FUN_1404e2de0(param_1 + 0x2c,*(undefined8 *)(lVar4 + 0xb0),uVar3,lVar1 + 0x38,&local_50,local_88,
                &local_78,0,0,0,0,0,0);
  FUN_1404e2de0(param_1 + 0x54,*(undefined8 *)(lVar4 + 0xa8),uVar3,lVar1 + 0x38,lVar1 + 0x44,
                local_88,lVar2 + 0x44,0,0,0,0,0,0);
  return;
}


````

### pseudocode

````text

undefined8
FUN_1404e2de0(int *param_1,longlong param_2,longlong param_3,undefined4 *param_4,undefined4 *param_5
             ,undefined4 *param_6,undefined4 *param_7,undefined4 *param_8,undefined4 *param_9,
             undefined8 param_10,undefined4 param_11,undefined8 param_12,undefined8 param_13)

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
      *(byte *)(lVar2 + 0x138) = *(byte *)(lVar2 + 0x138) & 0xfb;
      if (param_4 == (undefined4 *)0x0) {
        param_4 = (undefined4 *)(param_3 + 0x300);
      }
      *(undefined4 *)(lVar2 + 0x38) = *param_4;
      *(undefined4 *)(lVar2 + 0x3c) = param_4[1];
      *(undefined4 *)(lVar2 + 0x40) = param_4[2];
      if (param_5 == (undefined4 *)0x0) {
        param_5 = (undefined4 *)(param_3 + 0x30c);
      }
      *(undefined4 *)(lVar2 + 0x44) = *param_5;
      *(undefined4 *)(lVar2 + 0x48) = param_5[1];
      *(undefined4 *)(lVar2 + 0x4c) = param_5[2];
      *(undefined4 *)(lVar2 + 0x50) = param_5[3];
      *(undefined4 *)(lVar2 + 0x54) = param_5[4];
      *(undefined4 *)(lVar2 + 0x58) = param_5[5];
      *(undefined4 *)(lVar2 + 0x5c) = param_5[6];
      *(undefined4 *)(lVar2 + 0x60) = param_5[7];
      *(undefined4 *)(lVar2 + 100) = param_5[8];
      if (param_6 == (undefined4 *)0x0) {
        param_6 = (undefined4 *)(param_3 + 0x300);
      }
      *(undefined4 *)(lVar2 + 0x80) = *param_6;
      *(undefined4 *)(lVar2 + 0x84) = param_6[1];
      *(undefined4 *)(lVar2 + 0x88) = param_6[2];
      if (param_7 == (undefined4 *)0x0) {
        param_7 = (undefined4 *)(param_3 + 0x30c);
      }
      *(undefined4 *)(lVar2 + 0x8c) = *param_7;
      *(undefined4 *)(lVar2 + 0x90) = param_7[1];
      *(undefined4 *)(lVar2 + 0x94) = param_7[2];
      *(undefined4 *)(lVar2 + 0x98) = param_7[3];
      *(undefined4 *)(lVar2 + 0x9c) = param_7[4];
      *(undefined4 *)(lVar2 + 0xa0) = param_7[5];
      *(undefined4 *)(lVar2 + 0xa4) = param_7[6];
      *(undefined4 *)(lVar2 + 0xa8) = param_7[7];
      *(undefined4 *)(lVar2 + 0xac) = param_7[8];
      if (param_8 == (undefined4 *)0x0) {
        param_8 = (undefined4 *)(param_3 + 0x33c);
      }
      *(undefined4 *)(lVar2 + 0x68) = *param_8;
      *(undefined4 *)(lVar2 + 0x6c) = param_8[1];
      *(undefined4 *)(lVar2 + 0x70) = param_8[2];
      if (param_9 == (undefined4 *)0x0) {
        param_9 = (undefined4 *)(param_3 + 0x348);
      }
      *(undefined4 *)(lVar2 + 0x74) = *param_9;
      *(undefined4 *)(lVar2 + 0x78) = param_9[1];
      *(undefined4 *)(lVar2 + 0x7c) = param_9[2];
      uVar4 = FUN_1408780a0(param_3);
      *(undefined4 *)(lVar2 + 0x118) = param_11;
      *(undefined4 *)(lVar2 + 0xf8) = uVar4;
      *(undefined8 *)(lVar2 + 0x108) = param_10;
      *(undefined8 *)(lVar2 + 0x110) = param_12;
      *(undefined8 *)(lVar2 + 0x128) = param_13;
      uVar1 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
      FUN_1401a2ff0(uVar1,lVar2,0,0x1ffff,0);
      param_1[1] = *(int *)(param_3 + 0xa8);
      piVar3 = (int *)FUN_140167a00(lVar2,local_res8);
      *param_1 = *piVar3;
      return 1;
    }
  }
  return 0;
}


````

### pseudocode

````text

undefined8 FUN_1404bd860(void)

{
  longlong lVar1;
  
  lVar1 = FUN_1408a8ba0(D2_GameSingletonPtr);
  return *(undefined8 *)(*(longlong *)(*(longlong *)(lVar1 + 0x3d0) + 0xb8) + 8);
}


````
