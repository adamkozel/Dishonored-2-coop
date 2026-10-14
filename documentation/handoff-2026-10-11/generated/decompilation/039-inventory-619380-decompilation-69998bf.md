# inventory-619380-decompilation.json

Original: [outputs/d2-coop-lab/analysis/ghidra/inventory-619380-decompilation.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/inventory-619380-decompilation.json>)

SHA-256: `B1BFFFAD10ECD79CDE20DD1C9CFFE683471520129EC4556615D5F74DBCFCC26E`; 6915 bytes.

## Original saved record

````json
{
  "meta": null,
  "content": [
    {
      "type": "text",
      "text": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140619380(longlong param_1,longlong *param_2,longlong param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 *puVar2;\r\n  bool bVar3;\r\n  uint uVar4;\r\n  longlong lVar5;\r\n  undefined8 uVar6;\r\n  undefined *puVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  longlong *plStackX_10;\r\n  longlong lStack_48;\r\n  undefined8 uStack_40;\r\n  undefined1 auStack_38 [32];\r\n  \r\n  lVar1 = *param_2;\r\n  if ((*(longlong *)(lVar1 + 0x80) != 0) && (lVar5 = FUN_1406dc560((int)param_2[1]), lVar5 == 0)) {\r\n    puVar2 = *(undefined8 **)(param_1 + 0x10);\r\n    lVar5 = (**(code **)*puVar2)(puVar2);\r\n    if ((*(int *)(lVar5 + 0x60) < _DAT_1439ce350) || (_DAT_1439ce354 < *(int *)(lVar5 + 0x60))) {\r\n      bVar3 = false;\r\n    }\r\n    else {\r\n      bVar3 = true;\r\n    }\r\n    if (param_3 == 0) {\r\n      lVar5 = *_D2_GameSingletonPtr;\r\n      uVar6 = func_0x000140146520(&lStack_48);\r\n      param_3 = (**(code **)(lVar5 + 0x1c0))\r\n                          (_D2_GameSingletonPtr,*(undefined8 *)(lVar1 + 0x80),uVar6);\r\n      if (param_3 == 0) {\r\n        return;\r\n      }\r\n      func_0x00014087c320(param_3,puVar2 + 0x60);\r\n      func_0x00014087c2c0(param_3,(longlong)puVar2 + 0x30c);\r\n      if (*(longlong *)(param_1 + 0x10) == 0) {\r\n        *(undefined4 *)(param_3 + 0xac) = 0x3fff0010;\r\n      }\r\n      else {\r\n        *(undefined4 *)(param_3 + 0xac) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n      }\r\n      FUN_14087c360(param_3,*(longlong *)(param_1 + 0x10) + 0xc0);\r\n      func_0x00014087c350(param_3);\r\n      if (bVar3) {\r\n        *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 8;\r\n      }\r\n      else {\r\n        *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 4;\r\n      }\r\n      FUN_1408aecc0(_D2_GameSingletonPtr,param_3,0xffffffff);\r\n    }\r\n    lVar5 = FUN_140877720(param_3,0x1439cb440,0);\r\n    if (lVar5 == 0) {\r\n      puVar7 = &UNK_141b9aa60;\r\n    }\r\n    else {\r\n      if ((*(int *)(*(longlong *)(lVar5 + 8) + 0x11c) != 0) &&\r\n         (*(int *)(*(longlong *)(lVar5 + 8) + 0x68) < 0x10)) {\r\n        plStackX_10 = param_2;\r\n        FUN_1408918f0(param_1 + 0x58,&plStackX_10);\r\n        FUN_14063cb90(param_2,param_3);\r\n        if (!bVar3) {\r\n          return;\r\n        }\r\n        lVar5 = FUN_1408e7bb0(lVar5);\r\n        if (lVar5 == 0) {\r\n          return;\r\n        }\r\n        lVar5 = *(longlong *)(lVar5 + 8);\r\n        uVar4 = *(uint *)(lVar5 + 400) & 0xffffff;\r\n        lVar9 = (longlong)(int)uVar4;\r\n        if (uVar4 == 0) {\r\n          return;\r\n        }\r\n        lVar8 = 0;\r\n        lStack_48 = lVar1;\r\n        do {\r\n          uStack_40 = *(undefined8 *)(lVar8 + *(longlong *)(lVar5 + 0x188));\r\n          FUN_14030c670(param_1 + 0x28,&lStack_48);\r\n          lVar8 = lVar8 + 0x70;\r\n          lVar9 = lVar9 + -1;\r\n        } while (lVar9 != 0);\r\n        return;\r\n      }\r\n      puVar7 = &UNK_141b9aa10;\r\n    }\r\n    func_0x000140166830(puVar7,*(undefined8 *)(*(longlong *)(lVar1 + 0x80) + 8),\r\n                        *(undefined8 *)(lVar1 + 8));\r\n    FUN_1408847f0(param_3,auStack_38,0x1439a6f30);\r\n    FUN_140882fc0(auStack_38);\r\n  }\r\n  return;\r\n}\r\n\r\n",
      "annotations": null,
      "meta": null
    }
  ],
  "structuredContent": {
    "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140619380(longlong param_1,longlong *param_2,longlong param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 *puVar2;\r\n  bool bVar3;\r\n  uint uVar4;\r\n  longlong lVar5;\r\n  undefined8 uVar6;\r\n  undefined *puVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  longlong *plStackX_10;\r\n  longlong lStack_48;\r\n  undefined8 uStack_40;\r\n  undefined1 auStack_38 [32];\r\n  \r\n  lVar1 = *param_2;\r\n  if ((*(longlong *)(lVar1 + 0x80) != 0) && (lVar5 = FUN_1406dc560((int)param_2[1]), lVar5 == 0)) {\r\n    puVar2 = *(undefined8 **)(param_1 + 0x10);\r\n    lVar5 = (**(code **)*puVar2)(puVar2);\r\n    if ((*(int *)(lVar5 + 0x60) < _DAT_1439ce350) || (_DAT_1439ce354 < *(int *)(lVar5 + 0x60))) {\r\n      bVar3 = false;\r\n    }\r\n    else {\r\n      bVar3 = true;\r\n    }\r\n    if (param_3 == 0) {\r\n      lVar5 = *_D2_GameSingletonPtr;\r\n      uVar6 = func_0x000140146520(&lStack_48);\r\n      param_3 = (**(code **)(lVar5 + 0x1c0))\r\n                          (_D2_GameSingletonPtr,*(undefined8 *)(lVar1 + 0x80),uVar6);\r\n      if (param_3 == 0) {\r\n        return;\r\n      }\r\n      func_0x00014087c320(param_3,puVar2 + 0x60);\r\n      func_0x00014087c2c0(param_3,(longlong)puVar2 + 0x30c);\r\n      if (*(longlong *)(param_1 + 0x10) == 0) {\r\n        *(undefined4 *)(param_3 + 0xac) = 0x3fff0010;\r\n      }\r\n      else {\r\n        *(undefined4 *)(param_3 + 0xac) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n      }\r\n      FUN_14087c360(param_3,*(longlong *)(param_1 + 0x10) + 0xc0);\r\n      func_0x00014087c350(param_3);\r\n      if (bVar3) {\r\n        *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 8;\r\n      }\r\n      else {\r\n        *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 4;\r\n      }\r\n      FUN_1408aecc0(_D2_GameSingletonPtr,param_3,0xffffffff);\r\n    }\r\n    lVar5 = FUN_140877720(param_3,0x1439cb440,0);\r\n    if (lVar5 == 0) {\r\n      puVar7 = &UNK_141b9aa60;\r\n    }\r\n    else {\r\n      if ((*(int *)(*(longlong *)(lVar5 + 8) + 0x11c) != 0) &&\r\n         (*(int *)(*(longlong *)(lVar5 + 8) + 0x68) < 0x10)) {\r\n        plStackX_10 = param_2;\r\n        FUN_1408918f0(param_1 + 0x58,&plStackX_10);\r\n        FUN_14063cb90(param_2,param_3);\r\n        if (!bVar3) {\r\n          return;\r\n        }\r\n        lVar5 = FUN_1408e7bb0(lVar5);\r\n        if (lVar5 == 0) {\r\n          return;\r\n        }\r\n        lVar5 = *(longlong *)(lVar5 + 8);\r\n        uVar4 = *(uint *)(lVar5 + 400) & 0xffffff;\r\n        lVar9 = (longlong)(int)uVar4;\r\n        if (uVar4 == 0) {\r\n          return;\r\n        }\r\n        lVar8 = 0;\r\n        lStack_48 = lVar1;\r\n        do {\r\n          uStack_40 = *(undefined8 *)(lVar8 + *(longlong *)(lVar5 + 0x188));\r\n          FUN_14030c670(param_1 + 0x28,&lStack_48);\r\n          lVar8 = lVar8 + 0x70;\r\n          lVar9 = lVar9 + -1;\r\n        } while (lVar9 != 0);\r\n        return;\r\n      }\r\n      puVar7 = &UNK_141b9aa10;\r\n    }\r\n    func_0x000140166830(puVar7,*(undefined8 *)(*(longlong *)(lVar1 + 0x80) + 8),\r\n                        *(undefined8 *)(lVar1 + 8));\r\n    FUN_1408847f0(param_3,auStack_38,0x1439a6f30);\r\n    FUN_140882fc0(auStack_38);\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140619380(longlong param_1,longlong *param_2,longlong param_3)

{
  longlong lVar1;
  undefined8 *puVar2;
  bool bVar3;
  uint uVar4;
  longlong lVar5;
  undefined8 uVar6;
  undefined *puVar7;
  longlong lVar8;
  longlong lVar9;
  longlong *plStackX_10;
  longlong lStack_48;
  undefined8 uStack_40;
  undefined1 auStack_38 [32];
  
  lVar1 = *param_2;
  if ((*(longlong *)(lVar1 + 0x80) != 0) && (lVar5 = FUN_1406dc560((int)param_2[1]), lVar5 == 0)) {
    puVar2 = *(undefined8 **)(param_1 + 0x10);
    lVar5 = (**(code **)*puVar2)(puVar2);
    if ((*(int *)(lVar5 + 0x60) < _DAT_1439ce350) || (_DAT_1439ce354 < *(int *)(lVar5 + 0x60))) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    if (param_3 == 0) {
      lVar5 = *_D2_GameSingletonPtr;
      uVar6 = func_0x000140146520(&lStack_48);
      param_3 = (**(code **)(lVar5 + 0x1c0))
                          (_D2_GameSingletonPtr,*(undefined8 *)(lVar1 + 0x80),uVar6);
      if (param_3 == 0) {
        return;
      }
      func_0x00014087c320(param_3,puVar2 + 0x60);
      func_0x00014087c2c0(param_3,(longlong)puVar2 + 0x30c);
      if (*(longlong *)(param_1 + 0x10) == 0) {
        *(undefined4 *)(param_3 + 0xac) = 0x3fff0010;
      }
      else {
        *(undefined4 *)(param_3 + 0xac) = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
      }
      FUN_14087c360(param_3,*(longlong *)(param_1 + 0x10) + 0xc0);
      func_0x00014087c350(param_3);
      if (bVar3) {
        *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 8;
      }
      else {
        *(byte *)(param_3 + 0xd4) = *(byte *)(param_3 + 0xd4) | 4;
      }
      FUN_1408aecc0(_D2_GameSingletonPtr,param_3,0xffffffff);
    }
    lVar5 = FUN_140877720(param_3,0x1439cb440,0);
    if (lVar5 == 0) {
      puVar7 = &UNK_141b9aa60;
    }
    else {
      if ((*(int *)(*(longlong *)(lVar5 + 8) + 0x11c) != 0) &&
         (*(int *)(*(longlong *)(lVar5 + 8) + 0x68) < 0x10)) {
        plStackX_10 = param_2;
        FUN_1408918f0(param_1 + 0x58,&plStackX_10);
        FUN_14063cb90(param_2,param_3);
        if (!bVar3) {
          return;
        }
        lVar5 = FUN_1408e7bb0(lVar5);
        if (lVar5 == 0) {
          return;
        }
        lVar5 = *(longlong *)(lVar5 + 8);
        uVar4 = *(uint *)(lVar5 + 400) & 0xffffff;
        lVar9 = (longlong)(int)uVar4;
        if (uVar4 == 0) {
          return;
        }
        lVar8 = 0;
        lStack_48 = lVar1;
        do {
          uStack_40 = *(undefined8 *)(lVar8 + *(longlong *)(lVar5 + 0x188));
          FUN_14030c670(param_1 + 0x28,&lStack_48);
          lVar8 = lVar8 + 0x70;
          lVar9 = lVar9 + -1;
        } while (lVar9 != 0);
        return;
      }
      puVar7 = &UNK_141b9aa10;
    }
    func_0x000140166830(puVar7,*(undefined8 *)(*(longlong *)(lVar1 + 0x80) + 8),
                        *(undefined8 *)(lVar1 + 8));
    FUN_1408847f0(param_3,auStack_38,0x1439a6f30);
    FUN_140882fc0(auStack_38);
  }
  return;
}


````
