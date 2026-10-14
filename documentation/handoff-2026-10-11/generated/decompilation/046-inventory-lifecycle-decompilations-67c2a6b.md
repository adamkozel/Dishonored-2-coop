# inventory-lifecycle-decompilations.json

Original: [outputs/d2-coop-lab/analysis/ghidra/inventory-lifecycle-decompilations.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/inventory-lifecycle-decompilations.json>)

SHA-256: `56F36BEF895B2EF5EEB7534D1496FBB76ACCC2058827DC7FED8C52D8AEA4CA9F`; 44564 bytes.

## Original saved record

````json
[
  {
    "address": "140617300",
    "result": {
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140617300(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  uint uVar2;\r\n  int iVar3;\r\n  int iVar4;\r\n  int iVar5;\r\n  longlong lVar6;\r\n  undefined8 uVar7;\r\n  undefined4 *puVar8;\r\n  longlong *plVar9;\r\n  int *piVar10;\r\n  longlong lVar11;\r\n  longlong lVar12;\r\n  longlong lVar13;\r\n  undefined *puStackX_8;\r\n  undefined8 uStackX_10;\r\n  undefined8 uStack_90;\r\n  longlong lStack_88;\r\n  undefined4 uStack_80;\r\n  undefined4 uStack_7c;\r\n  longlong lStack_78;\r\n  undefined4 uStack_70;\r\n  undefined8 uStack_68;\r\n  undefined8 uStack_60;\r\n  undefined8 uStack_58;\r\n  undefined1 uStack_50;\r\n  undefined4 uStack_4c;\r\n  undefined1 uStack_43;\r\n  \r\n  func_0x0001407e7400();\r\n  uStackX_10 = *(undefined8 *)(param_1 + 0x10);\r\n  lVar1 = *(longlong *)(param_1 + 8);\r\n  uStack_90 = *(longlong *)(lVar1 + 0x68);\r\n  lVar13 = 0;\r\n  lStack_88 = 0;\r\n  uStack_80 = 0;\r\n  uStack_7c = 0xffffffff;\r\n  FUN_1406136e0(&uStack_90);\r\n  while (lStack_88 != 0) {\r\n    if ((*(int *)(lStack_88 + 0x60) != 1) && (*(int *)(lStack_88 + 0x60) != 0xf)) {\r\n      if (0 < (longlong)*(int *)(uStack_90 + 0x70)) {\r\n        plVar9 = (longlong *)(*(longlong *)(uStack_90 + 0x68) + 4);\r\n        lVar6 = lVar13;\r\n        do {\r\n          if (*plVar9 == lStack_88) {\r\n            if (lVar6 != -1) {\r\n              uVar2 = *(uint *)(*(longlong *)(uStack_90 + 0x68) + 0xc + lVar6 * 0x14);\r\n              goto code_r0x0001406173c9;\r\n            }\r\n            break;\r\n          }\r\n          lVar6 = lVar6 + 1;\r\n          plVar9 = (longlong *)((longlong)plVar9 + 0x14);\r\n        } while (lVar6 < *(int *)(uStack_90 + 0x70));\r\n      }\r\n      uVar2 = (uint)(*(int *)(uStack_90 + 0x60) == 1);\r\ncode_r0x0001406173c9:\r\n      uVar7 = func_0x00014063c630(&lStack_78,lStack_88,uVar2);\r\n      FUN_140613e60(param_1 + 0x38,uVar7);\r\n    }\r\n    FUN_1406136e0(&uStack_90);\r\n  }\r\n  if ((*(byte *)(param_1 + 0x34) & 0x40) == 0) {\r\n    lVar6 = *(longlong *)(param_1 + 0x28);\r\n    if (lVar6 != 0) {\r\n      plVar9 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar9 + 0x10))(plVar9,&UNK_141ab2230,0x1ec,lVar6,0x10);\r\n    }\r\n    *(undefined8 *)(param_1 + 0x28) = 0;\r\n    *(uint *)(param_1 + 0x34) = *(uint *)(param_1 + 0x34) & 0xff;\r\n  }\r\n  *(uint *)(param_1 + 0x30) = *(uint *)(param_1 + 0x30) & 0xff000000;\r\n  puStackX_8 = &UNK_141b9a7f0;\r\n  if (*(longlong *)(param_1 + 0x38) != 0) {\r\n    FUN_140618f10(&puStackX_8,*(longlong *)(param_1 + 0x38),*(uint *)(param_1 + 0x40) & 0xffffff);\r\n  }\r\n  if ((*(longlong *)(param_1 + 0x20) != 0) &&\r\n     (uVar2 = *(uint *)(param_1 + 0x40) & 0xffffff, lVar12 = (longlong)(int)uVar2, lVar6 = lVar13,\r\n     uVar2 != 0)) {\r\n    do {\r\n      plVar9 = (longlong *)(*(longlong *)(param_1 + 0x38) + lVar6);\r\n      lVar11 = *(longlong *)(param_1 + 0x20);\r\n      iVar3 = func_0x0001401431a0(*(undefined8 *)(*plVar9 + 8));\r\n      uVar2 = *(uint *)(lVar11 + 0x10) & 0xffffff;\r\n      if (uVar2 != 0) {\r\n        piVar10 = *(int **)(lVar11 + 8);\r\n        lVar11 = lVar13;\r\n        do {\r\n          if (*piVar10 == iVar3) {\r\n            if (piVar10 != (int *)0x0) {\r\n              puStackX_8 = (undefined *)CONCAT44(puStackX_8._4_4_,piVar10[6]);\r\n              lVar11 = func_0x0001407b5e00(_D2_GameSingletonPtr + 0x3130b0,&puStackX_8);\r\n              if (lVar11 != 0) {\r\n                D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,plVar9,lVar11);\r\n              }\r\n            }\r\n            break;\r\n          }\r\n          lVar11 = lVar11 + 1;\r\n          piVar10 = piVar10 + 8;\r\n        } while (lVar11 < (int)uVar2);\r\n      }\r\n      lVar12 = lVar12 + -1;\r\n      lVar6 = lVar6 + 0x38;\r\n    } while (lVar12 != 0);\r\n  }\r\n  if ((*(longlong *)(lVar1 + 0x70) != 0) &&\r\n     (uVar2 = *(uint *)(*(longlong *)(lVar1 + 0x70) + 0x68) & 0xffffff, lVar6 = (longlong)(int)uVar2\r\n     , uVar2 != 0)) {\r\n    do {\r\n      plVar9 = (longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x70) + 0x60) + lVar13);\r\n      if ((*plVar9 != 0) && ((plVar9[1] != 0 && (plVar9[2] != 0)))) {\r\n        puStackX_8 = &UNK_141b9a840;\r\n        uStack_70 = 0x3fff0010;\r\n        uStack_68 = 0;\r\n        uStack_60 = 0;\r\n        uStack_58 = 0;\r\n        uStack_50 = 0;\r\n        uStack_4c = 0;\r\n        uStack_43 = 1;\r\n        lStack_78 = *plVar9;\r\n        iVar3 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),\r\n                              *(uint *)(param_1 + 0x40) & 0xffffff,&lStack_78);\r\n        lStack_78 = plVar9[1];\r\n        puStackX_8 = &UNK_141b9a840;\r\n        uStack_70 = 0x3fff0010;\r\n        uStack_68 = 0;\r\n        uStack_60 = 0;\r\n        uStack_58 = 0;\r\n        uStack_50 = 0;\r\n        uStack_4c = 0;\r\n        uStack_43 = 1;\r\n        iVar4 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),\r\n                              *(uint *)(param_1 + 0x40) & 0xffffff,&lStack_78);\r\n        lStack_78 = plVar9[2];\r\n        puStackX_8 = &UNK_141b9a840;\r\n        uStack_70 = 0x3fff0010;\r\n        uStack_68 = 0;\r\n        uStack_60 = 0;\r\n        uStack_58 = 0;\r\n        uStack_50 = 0;\r\n        uStack_4c = 0;\r\n        uStack_43 = 1;\r\n        iVar5 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),\r\n                              *(uint *)(param_1 + 0x40) & 0xffffff,&lStack_78);\r\n        uStack_90 = CONCAT44(iVar4,iVar3);\r\n        lStack_88 = CONCAT44(lStack_88._4_4_,iVar5);\r\n        if ((-1 < iVar3) && ((-1 < iVar4 && (-1 < iVar5)))) {\r\n          FUN_140613d00(param_1 + 0x48,&uStack_90);\r\n        }\r\n      }\r\n      lVar13 = lVar13 + 0x18;\r\n      lVar6 = lVar6 + -1;\r\n    } while (lVar6 != 0);\r\n  }\r\n  uVar7 = FUN_140877720(uStackX_10,0x1439edb90,1);\r\n  *(undefined8 *)(param_1 + 0xf0) = uVar7;\r\n  puVar8 = (undefined4 *)FUN_14060ce50(uVar7,&puStackX_8,0x143987a90);\r\n  if ((undefined4 *)(param_1 + 0x174) != puVar8) {\r\n    *(undefined4 *)(param_1 + 0x174) = *puVar8;\r\n  }\r\n  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987a98);\r\n  if ((undefined4 *)(param_1 + 0x178) != puVar8) {\r\n    *(undefined4 *)(param_1 + 0x178) = *puVar8;\r\n  }\r\n  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987aa0);\r\n  if ((undefined4 *)(param_1 + 0x17c) != puVar8) {\r\n    *(undefined4 *)(param_1 + 0x17c) = *puVar8;\r\n  }\r\n  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987aa8);\r\n  if ((undefined4 *)(param_1 + 0x180) != puVar8) {\r\n    *(undefined4 *)(param_1 + 0x180) = *puVar8;\r\n  }\r\n  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987ab0);\r\n  if ((undefined4 *)(param_1 + 0x184) != puVar8) {\r\n    *(undefined4 *)(param_1 + 0x184) = *puVar8;\r\n  }\r\n  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987ab8);\r\n  if ((undefined4 *)(param_1 + 0x188) != puVar8) {\r\n    *(undefined4 *)(param_1 + 0x188) = *puVar8;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140613e60",
    "result": {
      "result": "\r\nuint FUN_140613e60(longlong *param_1,undefined8 *param_2)\r\n\r\n{\r\n  char cVar1;\r\n  uint uVar2;\r\n  int iVar3;\r\n  undefined8 *puVar4;\r\n  uint uVar5;\r\n  \r\n  uVar2 = *(uint *)((longlong)param_1 + 0xc);\r\n  uVar5 = uVar2 >> 8;\r\n  if ((*(uint *)(param_1 + 1) & 0xffffff) == uVar5) {\r\n    if (((byte)uVar2 & 0xc0) == 0xc0) {\r\n      if (DAT_1431aee98 == '\\0') {\r\n        if ((uVar2 & 0x3f) == 0x3f) {\r\n          if (uVar5 == 0) {\r\n            iVar3 = 1;\r\n          }\r\n          else if (uVar5 < 0x40000000) {\r\n            iVar3 = uVar5 * 2;\r\n          }\r\n          else {\r\n            iVar3 = 0x7fffffff;\r\n          }\r\n        }\r\n        else {\r\n          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);\r\n          uVar5 = iVar3 + uVar5;\r\n          iVar3 = uVar5 - (iVar3 - 1U & uVar5);\r\n        }\r\n      }\r\n      else {\r\n        iVar3 = uVar5 + 1;\r\n      }\r\n      FUN_140616120(param_1,iVar3);\r\n    }\r\n    else {\r\n      if (DAT_1431aee98 == '\\0') {\r\n        if ((uVar2 & 0x3f) == 0x3f) {\r\n          if (uVar5 == 0) {\r\n            iVar3 = 1;\r\n          }\r\n          else if (uVar5 < 0x40000000) {\r\n            iVar3 = uVar5 * 2;\r\n          }\r\n          else {\r\n            iVar3 = 0x7fffffff;\r\n          }\r\n        }\r\n        else {\r\n          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);\r\n          uVar5 = iVar3 + uVar5;\r\n          iVar3 = uVar5 - (iVar3 - 1U & uVar5);\r\n        }\r\n      }\r\n      else {\r\n        iVar3 = uVar5 + 1;\r\n      }\r\n      cVar1 = FUN_1406181c0(param_1,iVar3);\r\n      if (cVar1 == '\\0') {\r\n        return 0xffffffff;\r\n      }\r\n    }\r\n  }\r\n  uVar2 = *(uint *)(param_1 + 1) & 0xffffff;\r\n  if (uVar2 < *(uint *)((longlong)param_1 + 0xc) >> 8) {\r\n    puVar4 = (undefined8 *)((ulonglong)(*(uint *)(param_1 + 1) & 0xffffff) * 0x38 + *param_1);\r\n    *puVar4 = *param_2;\r\n    *(undefined4 *)(puVar4 + 1) = *(undefined4 *)(param_2 + 1);\r\n    puVar4[2] = param_2[2];\r\n    puVar4[3] = param_2[3];\r\n    *(undefined4 *)(puVar4 + 4) = *(undefined4 *)(param_2 + 4);\r\n    *(undefined4 *)((longlong)puVar4 + 0x24) = *(undefined4 *)((longlong)param_2 + 0x24);\r\n    *(undefined1 *)(puVar4 + 5) = *(undefined1 *)(param_2 + 5);\r\n    *(undefined4 *)((longlong)puVar4 + 0x2c) = *(undefined4 *)((longlong)param_2 + 0x2c);\r\n    *(undefined4 *)(puVar4 + 6) = *(undefined4 *)(param_2 + 6);\r\n    *(undefined1 *)((longlong)puVar4 + 0x34) = *(undefined1 *)((longlong)param_2 + 0x34);\r\n    *(undefined1 *)((longlong)puVar4 + 0x35) = *(undefined1 *)((longlong)param_2 + 0x35);\r\n    uVar2 = *(uint *)(param_1 + 1);\r\n    *(uint *)(param_1 + 1) = (uVar2 + 1 ^ uVar2) & 0xffffff ^ uVar2;\r\n    return uVar2 & 0xffffff;\r\n  }\r\n  return uVar2 - 1;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "14063c630",
    "result": {
      "result": "\r\nundefined8 * FUN_14063c630(undefined8 *param_1,undefined8 param_2,undefined4 param_3)\r\n\r\n{\r\n  *param_1 = param_2;\r\n  *(undefined4 *)(param_1 + 1) = 0x3fff0010;\r\n  param_1[2] = 0;\r\n  param_1[3] = 0;\r\n  *(undefined4 *)(param_1 + 4) = 0;\r\n  *(undefined1 *)(param_1 + 5) = 0;\r\n  *(undefined8 *)((longlong)param_1 + 0x2c) = 0;\r\n  *(undefined4 *)((longlong)param_1 + 0x24) = param_3;\r\n  *(undefined2 *)((longlong)param_1 + 0x34) = 0x100;\r\n  return param_1;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140618af0",
    "result": {
      "result": "\r\n/* WARNING: Removing unreachable block (ram,0x000140618b70) */\r\n\r\nint FUN_140618af0(undefined8 param_1,longlong param_2,int param_3,longlong *param_4)\r\n\r\n{\r\n  longlong lVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  int iVar4;\r\n  \r\n  if ((param_3 == 0) || (param_2 == 0)) {\r\n    return -1;\r\n  }\r\n  iVar4 = 0;\r\n  if (0 < param_3) {\r\n    iVar4 = 0;\r\n    do {\r\n      iVar2 = param_3 >> 1;\r\n      lVar1 = *(longlong *)(((longlong)iVar4 + (longlong)iVar2) * 0x38 + param_2);\r\n      lVar3 = lVar1 - *param_4;\r\n      if ((lVar3 < 0) || (lVar1 == *param_4 || lVar3 < 0)) {\r\n        iVar4 = iVar4 + iVar2;\r\n      }\r\n      param_3 = param_3 - iVar2;\r\n    } while (0 < iVar2);\r\n  }\r\n  lVar1 = *(longlong *)((longlong)iVar4 * 0x38 + param_2);\r\n  lVar3 = lVar1 - *param_4;\r\n  if (lVar3 < 0) {\r\n    return -1;\r\n  }\r\n  iVar2 = -1;\r\n  if (lVar1 == *param_4 || lVar3 < 0) {\r\n    iVar2 = iVar4;\r\n  }\r\n  return iVar2;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140618f10",
    "result": {
      "result": "\r\nvoid FUN_140618f10(undefined8 param_1,longlong param_2,uint param_3)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  undefined8 uVar2;\r\n  undefined8 uVar3;\r\n  undefined8 uVar4;\r\n  undefined8 uVar5;\r\n  undefined8 uVar6;\r\n  undefined8 uVar7;\r\n  longlong *plVar8;\r\n  longlong lVar9;\r\n  longlong lVar10;\r\n  longlong *plVar11;\r\n  longlong lVar12;\r\n  longlong lVar13;\r\n  longlong lVar14;\r\n  ulonglong uVar15;\r\n  ulonglong uVar16;\r\n  undefined8 *puVar17;\r\n  longlong lVar18;\r\n  ulonglong uVar19;\r\n  undefined8 *puVar20;\r\n  longlong lVar21;\r\n  longlong lStack_8a8;\r\n  undefined4 uStack_874;\r\n  undefined4 uStack_86c;\r\n  undefined1 uStack_864;\r\n  undefined1 uStack_863;\r\n  undefined4 uStack_83c;\r\n  undefined4 uStack_834;\r\n  undefined1 uStack_82c;\r\n  undefined1 uStack_82b;\r\n  longlong alStack_828 [128];\r\n  ulonglong auStack_428 [128];\r\n  \r\n  if (param_3 < 2) {\r\n    return;\r\n  }\r\n  lStack_8a8 = 0;\r\n  lVar21 = 0;\r\n  uVar19 = (ulonglong)(param_3 - 1);\r\n  do {\r\n    while (((longlong)(uVar19 - lStack_8a8) < 4 || (0x7e < lVar21))) {\r\n      if (lStack_8a8 < (longlong)uVar19) {\r\n        lVar13 = lStack_8a8 + 1;\r\n        puVar17 = (undefined8 *)(param_2 + 0x10 + uVar19 * 0x38);\r\n        do {\r\n          lVar10 = lStack_8a8;\r\n          if (lVar13 <= (longlong)uVar19) {\r\n            lVar14 = lStack_8a8 * 0x38;\r\n            lVar12 = lVar13 * 0x38;\r\n            lVar9 = lVar13;\r\n            do {\r\n              lVar18 = *(longlong *)(lVar12 + param_2) - *(longlong *)(lVar14 + param_2);\r\n              if ((-1 < lVar18) &&\r\n                 (*(longlong *)(lVar12 + param_2) != *(longlong *)(lVar14 + param_2) && -1 < lVar18)\r\n                 ) {\r\n                lVar10 = lVar9;\r\n                lVar14 = lVar12;\r\n              }\r\n              lVar9 = lVar9 + 1;\r\n              lVar12 = lVar12 + 0x38;\r\n            } while (lVar9 <= (longlong)uVar19);\r\n          }\r\n          lVar10 = lVar10 * 0x38;\r\n          uVar19 = uVar19 - 1;\r\n          uVar1 = *(undefined8 *)(lVar10 + 0x20 + param_2);\r\n          uVar2 = *(undefined8 *)(lVar10 + 0x28 + param_2);\r\n          uVar3 = *(undefined8 *)(lVar10 + 0x30 + param_2);\r\n          uVar4 = *(undefined8 *)(lVar10 + param_2);\r\n          uVar5 = *(undefined8 *)(lVar10 + 8 + param_2);\r\n          uVar6 = *(undefined8 *)(lVar10 + 0x10 + param_2);\r\n          uVar7 = *(undefined8 *)(lVar10 + 0x18 + param_2);\r\n          *(undefined8 *)(lVar10 + param_2) = puVar17[-2];\r\n          *(undefined4 *)(lVar10 + 8 + param_2) = *(undefined4 *)(puVar17 + -1);\r\n          *(undefined8 *)(lVar10 + 0x10 + param_2) = *puVar17;\r\n          *(undefined8 *)(lVar10 + 0x18 + param_2) = puVar17[1];\r\n          *(undefined4 *)(lVar10 + 0x20 + param_2) = *(undefined4 *)(puVar17 + 2);\r\n          *(undefined4 *)(lVar10 + 0x24 + param_2) = *(undefined4 *)((longlong)puVar17 + 0x14);\r\n          *(undefined1 *)(lVar10 + 0x28 + param_2) = *(undefined1 *)(puVar17 + 3);\r\n          *(undefined4 *)(lVar10 + 0x2c + param_2) = *(undefined4 *)((longlong)puVar17 + 0x1c);\r\n          *(undefined4 *)(lVar10 + 0x30 + param_2) = *(undefined4 *)(puVar17 + 4);\r\n          *(undefined1 *)(lVar10 + 0x34 + param_2) = *(undefined1 *)((longlong)puVar17 + 0x24);\r\n          *(undefined1 *)(lVar10 + 0x35 + param_2) = *(undefined1 *)((longlong)puVar17 + 0x25);\r\n          uStack_83c = (undefined4)((ulonglong)uVar1 >> 0x20);\r\n          puVar17[-2] = uVar4;\r\n          *(int *)(puVar17 + -1) = (int)uVar5;\r\n          *(undefined4 *)((longlong)puVar17 + 0x14) = uStack_83c;\r\n          uStack_834 = (undefined4)((ulonglong)uVar2 >> 0x20);\r\n          *(undefined4 *)((longlong)puVar17 + 0x1c) = uStack_834;\r\n          uStack_82c = (undefined1)((ulonglong)uVar3 >> 0x20);\r\n          *puVar17 = uVar6;\r\n          *(undefined1 *)((longlong)puVar17 + 0x24) = uStack_82c;\r\n          uStack_82b = (undefined1)((ulonglong)uVar3 >> 0x28);\r\n          puVar17[1] = uVar7;\r\n          *(int *)(puVar17 + 2) = (int)uVar1;\r\n          *(char *)(puVar17 + 3) = (char)uVar2;\r\n          *(int *)(puVar17 + 4) = (int)uVar3;\r\n          *(undefined1 *)((longlong)puVar17 + 0x25) = uStack_82b;\r\n          puVar17 = puVar17 + -7;\r\n        } while (lStack_8a8 < (longlong)uVar19);\r\n      }\r\n      lVar21 = lVar21 + -1;\r\n      if (lVar21 < 0) {\r\n        return;\r\n      }\r\n      uVar19 = auStack_428[lVar21];\r\n      lStack_8a8 = alStack_828[lVar21];\r\n    }\r\n    plVar8 = (longlong *)(uVar19 * 0x38 + param_2);\r\n    FUN_140613350(plVar8,param_2 + ((longlong)(uVar19 + lStack_8a8) / 2) * 0x38);\r\n    uVar16 = uVar19 - 1;\r\n    lVar13 = lStack_8a8;\r\n    do {\r\n      lVar10 = *plVar8;\r\n      plVar11 = (longlong *)(lVar13 * 0x38 + param_2);\r\n      do {\r\n        lVar9 = *plVar11 - lVar10;\r\n        if ((-1 < lVar9) && (lVar14 = lVar13, -1 < (int)(uint)(*plVar11 != lVar10 && -1 < lVar9)))\r\n        break;\r\n        lVar13 = lVar13 + 1;\r\n        plVar11 = plVar11 + 7;\r\n        lVar14 = lVar13;\r\n      } while (lVar13 < (longlong)uVar16);\r\n      plVar11 = (longlong *)(uVar16 * 0x38 + param_2);\r\n      uVar15 = uVar16;\r\n      while( true ) {\r\n        lVar13 = *plVar11 - lVar10;\r\n        if ((lVar13 < 0) || (*plVar11 == lVar10 || lVar13 < 0)) break;\r\n        uVar16 = uVar15 - 1;\r\n        plVar11 = plVar11 + -7;\r\n        uVar15 = uVar16;\r\n        if ((longlong)uVar16 <= lVar14) goto code_r0x0001406190f9;\r\n      }\r\n      uVar16 = uVar15;\r\n      if ((longlong)uVar15 <= lVar14) break;\r\n      lVar13 = lVar14 + 1;\r\n      uVar16 = uVar15 - 1;\r\n      puVar20 = (undefined8 *)(uVar15 * 0x38 + param_2);\r\n      puVar17 = (undefined8 *)(lVar14 * 0x38 + param_2);\r\n      uVar1 = puVar17[4];\r\n      uVar2 = puVar17[5];\r\n      uVar3 = puVar17[6];\r\n      uVar4 = *puVar17;\r\n      uVar5 = puVar17[1];\r\n      uVar6 = puVar17[2];\r\n      uVar7 = puVar17[3];\r\n      *puVar17 = *puVar20;\r\n      *(undefined4 *)(puVar17 + 1) = *(undefined4 *)(puVar20 + 1);\r\n      puVar17[2] = puVar20[2];\r\n      puVar17[3] = puVar20[3];\r\n      *(undefined4 *)(puVar17 + 4) = *(undefined4 *)(puVar20 + 4);\r\n      *(undefined4 *)((longlong)puVar17 + 0x24) = *(undefined4 *)((longlong)puVar20 + 0x24);\r\n      *(undefined1 *)(puVar17 + 5) = *(undefined1 *)(puVar20 + 5);\r\n      *(undefined4 *)((longlong)puVar17 + 0x2c) = *(undefined4 *)((longlong)puVar20 + 0x2c);\r\n      *(undefined4 *)(puVar17 + 6) = *(undefined4 *)(puVar20 + 6);\r\n      *(undefined1 *)((longlong)puVar17 + 0x34) = *(undefined1 *)((longlong)puVar20 + 0x34);\r\n      *(undefined1 *)((longlong)puVar17 + 0x35) = *(undefined1 *)((longlong)puVar20 + 0x35);\r\n      uStack_874 = (undefined4)((ulonglong)uVar1 >> 0x20);\r\n      *puVar20 = uVar4;\r\n      *(int *)(puVar20 + 1) = (int)uVar5;\r\n      *(undefined4 *)((longlong)puVar20 + 0x24) = uStack_874;\r\n      uStack_86c = (undefined4)((ulonglong)uVar2 >> 0x20);\r\n      *(undefined4 *)((longlong)puVar20 + 0x2c) = uStack_86c;\r\n      uStack_864 = (undefined1)((ulonglong)uVar3 >> 0x20);\r\n      puVar20[2] = uVar6;\r\n      *(undefined1 *)((longlong)puVar20 + 0x34) = uStack_864;\r\n      uStack_863 = (undefined1)((ulonglong)uVar3 >> 0x28);\r\n      puVar20[3] = uVar7;\r\n      *(int *)(puVar20 + 4) = (int)uVar1;\r\n      *(char *)(puVar20 + 5) = (char)uVar2;\r\n      *(int *)(puVar20 + 6) = (int)uVar3;\r\n      *(undefined1 *)((longlong)puVar20 + 0x35) = uStack_863;\r\n      lVar14 = lVar13;\r\n    } while (lVar13 < (longlong)uVar16);\r\ncode_r0x0001406190f9:\r\n    lVar13 = *plVar8;\r\n    plVar11 = (longlong *)(uVar16 * 0x38 + param_2);\r\n    for (; (-1 < *plVar11 - lVar13 && (lStack_8a8 < (longlong)uVar16)); uVar16 = uVar16 - 1) {\r\n      plVar11 = plVar11 + -7;\r\n    }\r\n    plVar11 = (longlong *)(lVar14 * 0x38 + param_2);\r\n    while( true ) {\r\n      lVar10 = *plVar11 - lVar13;\r\n      if (((-1 < lVar10) && (*plVar11 != lVar13 && -1 < lVar10)) || ((longlong)uVar19 <= lVar14))\r\n      break;\r\n      lVar14 = lVar14 + 1;\r\n      plVar11 = plVar11 + 7;\r\n    }\r\n    FUN_140613350(plVar8,lVar14 * 0x38 + param_2);\r\n    if ((longlong)(uVar16 - lStack_8a8) < (longlong)(uVar19 - lVar14)) {\r\n      auStack_428[lVar21] = uVar19;\r\n      alStack_828[lVar21] = lVar14;\r\n      lVar21 = lVar21 + 1;\r\n      uVar19 = uVar16;\r\n    }\r\n    else {\r\n      alStack_828[lVar21] = lStack_8a8;\r\n      auStack_428[lVar21] = uVar16;\r\n      lVar21 = lVar21 + 1;\r\n      lStack_8a8 = lVar14;\r\n    }\r\n  } while( true );\r\n}\r\n\r\n"
    }
  },
  {
    "address": "1406177b0",
    "result": {
      "result": "\r\nvoid FUN_1406177b0(longlong param_1)\r\n\r\n{\r\n  uint uVar1;\r\n  longlong lVar2;\r\n  longlong *plVar3;\r\n  longlong lVar4;\r\n  undefined1 auStack_28 [32];\r\n  \r\n  lVar4 = 0;\r\n  uVar1 = *(uint *)(param_1 + 0x60) & 0xffffff;\r\n  if (uVar1 != 0) {\r\n    do {\r\n      lVar2 = FUN_1406dc560(*(undefined4 *)\r\n                             (*(longlong *)(*(longlong *)(param_1 + 0x58) + lVar4 * 8) + 8));\r\n      if (lVar2 != 0) {\r\n        FUN_1408847f0(lVar2,auStack_28,0x1439a6f30);\r\n        FUN_140882fc0(auStack_28);\r\n      }\r\n      lVar4 = lVar4 + 1;\r\n    } while (lVar4 < (int)uVar1);\r\n  }\r\n  if ((*(byte *)(param_1 + 100) & 0x40) == 0) {\r\n    lVar4 = *(longlong *)(param_1 + 0x58);\r\n    if (lVar4 != 0) {\r\n      plVar3 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141ab2230,0x1ec,lVar4,0x10);\r\n    }\r\n    *(uint *)(param_1 + 100) = *(uint *)(param_1 + 100) & 0xff;\r\n    *(undefined8 *)(param_1 + 0x58) = 0;\r\n  }\r\n  *(uint *)(param_1 + 0x60) = *(uint *)(param_1 + 0x60) & 0xff000000;\r\n  if ((*(byte *)(param_1 + 0x44) & 0x40) == 0) {\r\n    lVar4 = *(longlong *)(param_1 + 0x38);\r\n    if (lVar4 != 0) {\r\n      plVar3 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141ab2230,0x1ec,lVar4,0x10);\r\n    }\r\n    *(uint *)(param_1 + 0x44) = *(uint *)(param_1 + 0x44) & 0xff;\r\n    *(undefined8 *)(param_1 + 0x38) = 0;\r\n  }\r\n  *(uint *)(param_1 + 0x40) = *(uint *)(param_1 + 0x40) & 0xff000000;\r\n  if ((*(byte *)(param_1 + 0x164) & 0x40) == 0) {\r\n    lVar4 = *(longlong *)(param_1 + 0x158);\r\n    if (lVar4 != 0) {\r\n      plVar3 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141ab2230,0x1ec,lVar4,0x10);\r\n    }\r\n    *(uint *)(param_1 + 0x164) = *(uint *)(param_1 + 0x164) & 0xff;\r\n    *(undefined8 *)(param_1 + 0x158) = 0;\r\n  }\r\n  *(uint *)(param_1 + 0x160) = *(uint *)(param_1 + 0x160) & 0xff000000;\r\n  FUN_140878e80(*(undefined8 *)(param_1 + 0x10),param_1);\r\n  *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffffc;\r\n  *(undefined8 *)(param_1 + 8) = 0;\r\n  *(undefined8 *)(param_1 + 0x10) = 0;\r\n  return;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140617930",
    "result": {
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140617930(longlong param_1)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  longlong lVar2;\r\n  bool bVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  longlong lVar7;\r\n  longlong lVar8;\r\n  longlong lStack_28;\r\n  code *pcStack_20;\r\n  \r\n  FUN_1407e7670();\r\n  puVar1 = *(undefined8 **)(param_1 + 0x10);\r\n  lVar4 = (**(code **)*puVar1)(puVar1);\r\n  if ((*(int *)(lVar4 + 0x60) < _DAT_1439ce350) || (_DAT_1439ce354 < *(int *)(lVar4 + 0x60))) {\r\n    bVar3 = false;\r\n  }\r\n  else {\r\n    bVar3 = true;\r\n  }\r\n  uVar5 = FUN_140877720(puVar1,0x1439ae8e0,1);\r\n  func_0x0001403dab80(param_1 + 0xf8,uVar5);\r\n  uVar5 = FUN_140877720(puVar1,0x1439d13b0,1);\r\n  func_0x0001403dab80(param_1 + 0x108,uVar5);\r\n  uVar5 = FUN_140877720(puVar1,0x1439ae6a0,1);\r\n  func_0x0001403dab80(param_1 + 0x118,uVar5);\r\n  *(byte *)(param_1 + 400) = *(byte *)(param_1 + 400) & 0xfe;\r\n  if (*(longlong *)(param_1 + 0x20) == 0) {\r\n    lVar4 = *(longlong *)(*(longlong *)(param_1 + 8) + 0x78);\r\n    lVar8 = 0;\r\n    if (lVar4 != 0) {\r\n      FUN_140614450(param_1);\r\n      *(byte *)(param_1 + 400) =\r\n           *(byte *)(param_1 + 400) ^ (*(byte *)(lVar4 + 0x61) ^ *(byte *)(param_1 + 400)) & 1;\r\n      lVar6 = (longlong)*(int *)(lVar4 + 0x70);\r\n      lVar7 = lVar8;\r\n      if (0 < lVar6) {\r\n        do {\r\n          lVar2 = *(longlong *)(lVar7 + 4 + *(longlong *)(lVar4 + 0x68));\r\n          if (lVar2 != 0) {\r\n            FUN_140618de0(param_1,lVar2,*(undefined4 *)(lVar7 + 0xc + *(longlong *)(lVar4 + 0x68)));\r\n          }\r\n          lVar6 = lVar6 + -1;\r\n          lVar7 = lVar7 + 0x14;\r\n        } while (lVar6 != 0);\r\n      }\r\n    }\r\n    if ((bVar3) &&\r\n       (lVar4 = *(longlong *)(*(longlong *)(_D2_GameSingletonPtr + 0x1e8948) + 0x388), lVar4 != 0))\r\n    {\r\n      FUN_140613ff0(param_1,lVar4,0);\r\n    }\r\n    lVar4 = *(longlong *)(param_1 + 0xe8);\r\n    if (lVar4 != 0) {\r\n      if (*(char *)(lVar4 + 0x60) != '\\0') {\r\n        FUN_140614450(param_1);\r\n      }\r\n      *(byte *)(param_1 + 400) =\r\n           *(byte *)(param_1 + 400) ^ (*(byte *)(lVar4 + 0x61) ^ *(byte *)(param_1 + 400)) & 1;\r\n      lVar7 = (longlong)*(int *)(lVar4 + 0x70);\r\n      if (0 < lVar7) {\r\n        do {\r\n          lVar6 = *(longlong *)(*(longlong *)(lVar4 + 0x68) + 4 + lVar8);\r\n          if (lVar6 != 0) {\r\n            FUN_140618de0(param_1,lVar6,*(undefined4 *)(*(longlong *)(lVar4 + 0x68) + 0xc + lVar8));\r\n          }\r\n          lVar8 = lVar8 + 0x14;\r\n          lVar7 = lVar7 + -1;\r\n        } while (lVar7 != 0);\r\n      }\r\n    }\r\n  }\r\n  else {\r\n    D2_Inventory_ApplySavedItems_Hypothesis(param_1);\r\n    lVar4 = *(longlong *)(param_1 + 0x20);\r\n    if (lVar4 != 0) {\r\n      FUN_14061a0a0(lVar4 + 8);\r\n      FUN_1400f1520(lVar4);\r\n    }\r\n    *(undefined8 *)(param_1 + 0x20) = 0;\r\n  }\r\n  pcStack_20 = (code *)&UNK_140619da0;\r\n  lStack_28 = param_1;\r\n  uVar5 = func_0x000140810240();\r\n  func_0x000140810810(uVar5,puVar1,0x9a,&lStack_28);\r\n  pcStack_20 = (code *)&UNK_140619d90;\r\n  lStack_28 = param_1;\r\n  uVar5 = func_0x000140810240();\r\n  func_0x000140810810(uVar5,puVar1,0xb6,&lStack_28);\r\n  pcStack_20 = FUN_140619dc0;\r\n  lStack_28 = param_1;\r\n  uVar5 = func_0x000140810240();\r\n  func_0x000140810810(uVar5,puVar1,0xec,&lStack_28);\r\n  pcStack_20 = (code *)&UNK_140619d80;\r\n  lStack_28 = param_1;\r\n  uVar5 = func_0x000140810240();\r\n  func_0x000140810810(uVar5,puVar1,0xd6,&lStack_28);\r\n  return;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140617c10",
    "result": {
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140617c10(longlong *param_1)\r\n\r\n{\r\n  uint uVar1;\r\n  undefined8 uVar2;\r\n  longlong *plVar3;\r\n  longlong lVar4;\r\n  longlong *plStack_18;\r\n  code *pcStack_10;\r\n  \r\n  lVar4 = param_1[2];\r\n  pcStack_10 = (code *)&UNK_140619da0;\r\n  plStack_18 = param_1;\r\n  uVar2 = func_0x000140810240();\r\n  func_0x000140811160(uVar2,lVar4,0x9a,&plStack_18);\r\n  pcStack_10 = (code *)&UNK_140619d90;\r\n  plStack_18 = param_1;\r\n  uVar2 = func_0x000140810240();\r\n  func_0x000140811160(uVar2,lVar4,0xb6,&plStack_18);\r\n  pcStack_10 = FUN_140619dc0;\r\n  plStack_18 = param_1;\r\n  uVar2 = func_0x000140810240();\r\n  func_0x000140811160(uVar2,lVar4,0xec,&plStack_18);\r\n  pcStack_10 = (code *)&UNK_140619d80;\r\n  plStack_18 = param_1;\r\n  uVar2 = func_0x000140810240();\r\n  func_0x000140811160(uVar2,lVar4,0xd6,&plStack_18);\r\n  plVar3 = param_1 + 0xd;\r\n  lVar4 = 0x10;\r\n  do {\r\n    if (*plVar3 != 0) {\r\n      D2_Inventory_Unequip(param_1);\r\n    }\r\n    plVar3 = plVar3 + 1;\r\n    lVar4 = lVar4 + -1;\r\n  } while (lVar4 != 0);\r\n  func_0x0001403dab80(param_1 + 0x23,0);\r\n  param_1[0x1e] = 0;\r\n  func_0x0001403dab80(param_1 + 0x1f,0);\r\n  func_0x0001403dab80(param_1 + 0x21,0);\r\n  *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xfffffff3;\r\n  if ((*(uint *)((longlong)param_1 + 0x1c) >> 0x1c & 1) == 0) {\r\n    if (((byte)*(uint *)((longlong)param_1 + 0x1c) & 0x30) < 0x20) {\r\n      uVar2 = (**(code **)(*param_1 + 0x88))();\r\n      FUN_14048ff60(uVar2,param_1);\r\n    }\r\n    uVar1 = *(uint *)((longlong)param_1 + 0x1c) >> 4 & 3;\r\n    if (uVar1 == 1) {\r\n      FUN_1407ec3e0(*(undefined8 *)(_D2_GameSingletonPtr + 0x1e8960),param_1);\r\n      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xf800000f;\r\n      return;\r\n    }\r\n    if (uVar1 == 3) {\r\n      FUN_1407ec450(*(undefined8 *)(_D2_GameSingletonPtr + 0x1e8960),param_1);\r\n    }\r\n    *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xf800000f;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140618de0",
    "result": {
      "result": "\r\nundefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  undefined *puStackX_8;\r\n  undefined8 uStack_40;\r\n  undefined4 uStack_38;\r\n  undefined8 uStack_30;\r\n  undefined8 uStack_28;\r\n  undefined8 uStack_20;\r\n  undefined1 uStack_18;\r\n  undefined4 uStack_14;\r\n  undefined1 uStack_b;\r\n  \r\n  puStackX_8 = &UNK_141b9a840;\r\n  uStack_38 = 0x3fff0010;\r\n  uStack_30 = 0;\r\n  uStack_28 = 0;\r\n  uStack_20 = 0;\r\n  uStack_18 = 0;\r\n  uStack_14 = 0;\r\n  uStack_b = 1;\r\n  uStack_40 = param_2;\r\n  iVar2 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),\r\n                        *(uint *)(param_1 + 0x40) & 0xffffff,&uStack_40,0xfffffffffffffffe);\r\n  puStackX_8 = &UNK_141b9a808;\r\n  if (iVar2 < 0) {\r\n    uVar3 = 0;\r\n  }\r\n  else {\r\n    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);\r\n    uVar1 = *(undefined4 *)(lVar4 + 0x20);\r\n    uVar3 = func_0x00014063cc00(lVar4,param_3);\r\n    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140617e70",
    "result": {
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140617e70(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined8 uVar2;\r\n  longlong lVar3;\r\n  undefined8 *puVar4;\r\n  \r\n  if (*(longlong *)(param_1 + 0x20) == 0) {\r\n    puVar4 = *(undefined8 **)(param_1 + 0x10);\r\n    lVar3 = (**(code **)*puVar4)(puVar4);\r\n    if ((_DAT_1439ce350 <= *(int *)(lVar3 + 0x60)) && (*(int *)(lVar3 + 0x60) <= _DAT_1439ce354)) {\r\n      uVar2 = *param_2;\r\n      uVar1 = *(undefined4 *)(puVar4 + 0x15);\r\n      lVar3 = func_0x000140810240();\r\n      puVar4 = (undefined8 *)FUN_1408112c0(lVar3 + 0xc8e8,0x16b,8,uVar1);\r\n      if (puVar4 != (undefined8 *)0x0) {\r\n        *puVar4 = uVar2;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140759e70",
    "result": {
      "result": "\r\nvoid FUN_140759e70(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  undefined4 uVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  longlong *plVar7;\r\n  undefined4 *puVar8;\r\n  int iVar9;\r\n  undefined8 *puVar10;\r\n  int iVar11;\r\n  longlong lVar12;\r\n  longlong lVar13;\r\n  int aiStackX_8 [2];\r\n  \r\n  plVar1 = (longlong *)(param_2 + 0x240);\r\n  lVar5 = FUN_1408f2e50(param_2);\r\n  func_0x000140783e00(lVar5);\r\n  lVar3 = *(longlong *)(param_1 + 1000);\r\n  if (lVar3 != 0) {\r\n    aiStackX_8[0] = 0;\r\n    if (0 < *(int *)(param_2 + 0x248)) {\r\n      do {\r\n        iVar9 = aiStackX_8[0];\r\n        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);\r\n        if ((plVar7 != (longlong *)0x0) &&\r\n           (lVar6 = (**(code **)(*plVar7 + 0xb8))(), lVar6 == 0x143a269d0)) {\r\n          *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;\r\n          FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);\r\n          goto code_r0x000140759f8a;\r\n        }\r\n        aiStackX_8[0] = iVar9 + 1;\r\n      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));\r\n    }\r\n    do {\r\n      uVar4 = FUN_1400fdb80();\r\n      lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);\r\n    } while (lVar6 != 0);\r\n    iVar9 = *(int *)(param_2 + 0x24c);\r\n    iVar2 = *(int *)(param_2 + 0x248);\r\n    if (iVar9 == 0) {\r\n      iVar11 = 1;\r\ncode_r0x000140759f6d:\r\n      FUN_14075ac80(plVar1,iVar11);\r\n    }\r\n    else if ((iVar2 == iVar9) && (iVar11 = iVar9 * 2, iVar9 < iVar11)) goto code_r0x000140759f6d;\r\n    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;\r\n    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;\r\n    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;\r\n  }\r\ncode_r0x000140759f8a:\r\n  lVar3 = *(longlong *)(param_1 + 0x3f0);\r\n  if ((lVar3 == 0) || ((*(byte *)(lVar3 + 0x60) & 1) == 0)) goto code_r0x00014075a07a;\r\n  aiStackX_8[0] = 0;\r\n  if (0 < *(int *)(param_2 + 0x248)) {\r\n    do {\r\n      iVar9 = aiStackX_8[0];\r\n      plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);\r\n      if (((plVar7 != (longlong *)0x0) &&\r\n          (lVar6 = (**(code **)(*plVar7 + 0xb8))(plVar7), lVar6 == 0x143a10dd0)) &&\r\n         ((*(byte *)(plVar7 + 0xc) & 1) != 0)) {\r\n        *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;\r\n        FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);\r\n        goto code_r0x00014075a07a;\r\n      }\r\n      aiStackX_8[0] = iVar9 + 1;\r\n    } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));\r\n  }\r\n  do {\r\n    uVar4 = FUN_1400fdb80();\r\n    lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);\r\n  } while (lVar6 != 0);\r\n  iVar9 = *(int *)(param_2 + 0x24c);\r\n  iVar2 = *(int *)(param_2 + 0x248);\r\n  if (iVar9 == 0) {\r\n    iVar11 = 1;\r\ncode_r0x00014075a05d:\r\n    FUN_14075ac80(plVar1,iVar11);\r\n  }\r\n  else if ((iVar2 == iVar9) && (iVar11 = iVar9 * 2, iVar9 < iVar11)) goto code_r0x00014075a05d;\r\n  *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;\r\n  *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;\r\n  *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;\r\ncode_r0x00014075a07a:\r\n  lVar3 = *(longlong *)(param_1 + 0x400);\r\n  if (lVar3 != 0) {\r\n    aiStackX_8[0] = 0;\r\n    if (0 < *(int *)(param_2 + 0x248)) {\r\n      do {\r\n        iVar9 = aiStackX_8[0];\r\n        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);\r\n        if (((plVar7 != (longlong *)0x0) &&\r\n            (lVar6 = (**(code **)(*plVar7 + 0xb8))(plVar7), lVar6 == 0x143a193d0)) &&\r\n           ((*(byte *)(plVar7 + 0xc) & 1) != 0)) {\r\n          *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;\r\n          FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);\r\n          goto code_r0x00014075a224;\r\n        }\r\n        aiStackX_8[0] = iVar9 + 1;\r\n      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));\r\n    }\r\n    do {\r\n      uVar4 = FUN_1400fdb80();\r\n      lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);\r\n    } while (lVar6 != 0);\r\n    iVar9 = *(int *)(param_2 + 0x24c);\r\n    iVar2 = *(int *)(param_2 + 0x248);\r\n    if (iVar9 == 0) {\r\n      lVar6 = *plVar1;\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      puVar8 = (undefined4 *)\r\n               (**(code **)(*plVar7 + 8))\r\n                         (plVar7,&UNK_141b38b60,0x226,&UNK_141ab3808,0xc,0x41,1,0x10,0xffffffff);\r\n      *puVar8 = 0;\r\n      iVar9 = *(int *)(param_2 + 0x248);\r\n      if (1 < *(int *)(param_2 + 0x248)) {\r\n        iVar9 = 1;\r\n      }\r\n      lVar12 = (longlong)iVar9;\r\n      if (0 < iVar9) {\r\n        lVar13 = lVar6 - (longlong)puVar8;\r\n        puVar10 = (undefined8 *)(puVar8 + 1);\r\n        do {\r\n          *(undefined4 *)((longlong)puVar10 + -4) = *(undefined4 *)(lVar13 + -4 + (longlong)puVar10)\r\n          ;\r\n          *puVar10 = *(undefined8 *)(lVar13 + -0xc + (longlong)puVar10 + 0xc);\r\n          lVar12 = lVar12 + -1;\r\n          puVar10 = (undefined8 *)((longlong)puVar10 + 0xc);\r\n        } while (lVar12 != 0);\r\n      }\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar7 + 0x10))(plVar7,&UNK_141b38b60,0x239,lVar6,0x10);\r\n      *(undefined4 *)(param_2 + 0x24c) = 1;\r\n      *plVar1 = (longlong)puVar8;\r\n    }\r\n    else if ((iVar2 == iVar9) && (iVar9 < iVar9 * 2)) {\r\n      FUN_14075ac80(plVar1);\r\n    }\r\n    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;\r\n    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;\r\n    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;\r\n  }\r\ncode_r0x00014075a224:\r\n  lVar3 = *(longlong *)(param_1 + 0x408);\r\n  if (lVar3 != 0) {\r\n    aiStackX_8[0] = 0;\r\n    if (0 < *(int *)(param_2 + 0x248)) {\r\n      do {\r\n        iVar9 = aiStackX_8[0];\r\n        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);\r\n        if ((plVar7 != (longlong *)0x0) &&\r\n           (lVar6 = (**(code **)(*plVar7 + 0xb8))(), lVar6 == 0x143a1e3d0)) {\r\n          *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;\r\n          FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);\r\n          goto code_r0x00014075a3c8;\r\n        }\r\n        aiStackX_8[0] = iVar9 + 1;\r\n      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));\r\n    }\r\n    do {\r\n      uVar4 = FUN_1400fdb80();\r\n      lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);\r\n    } while (lVar6 != 0);\r\n    iVar9 = *(int *)(param_2 + 0x24c);\r\n    iVar2 = *(int *)(param_2 + 0x248);\r\n    if (iVar9 == 0) {\r\n      lVar6 = *plVar1;\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      puVar8 = (undefined4 *)\r\n               (**(code **)(*plVar7 + 8))\r\n                         (plVar7,&UNK_141b38b60,0x226,&UNK_141ab3808,0xc,0x41,1,0x10,0xffffffff);\r\n      *puVar8 = 0;\r\n      iVar9 = *(int *)(param_2 + 0x248);\r\n      if (1 < *(int *)(param_2 + 0x248)) {\r\n        iVar9 = 1;\r\n      }\r\n      lVar12 = (longlong)iVar9;\r\n      if (0 < iVar9) {\r\n        lVar13 = lVar6 - (longlong)puVar8;\r\n        puVar10 = (undefined8 *)(puVar8 + 1);\r\n        do {\r\n          *(undefined4 *)((longlong)puVar10 + -4) = *(undefined4 *)(lVar13 + -4 + (longlong)puVar10)\r\n          ;\r\n          *puVar10 = *(undefined8 *)(lVar13 + -0xc + (longlong)puVar10 + 0xc);\r\n          lVar12 = lVar12 + -1;\r\n          puVar10 = (undefined8 *)((longlong)puVar10 + 0xc);\r\n        } while (lVar12 != 0);\r\n      }\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar7 + 0x10))(plVar7,&UNK_141b38b60,0x239,lVar6,0x10);\r\n      *(undefined4 *)(param_2 + 0x24c) = 1;\r\n      *plVar1 = (longlong)puVar8;\r\n    }\r\n    else if ((iVar2 == iVar9) && (iVar9 < iVar9 * 2)) {\r\n      FUN_14075ac80(plVar1);\r\n    }\r\n    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;\r\n    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;\r\n    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;\r\n  }\r\ncode_r0x00014075a3c8:\r\n  lVar3 = *(longlong *)(param_1 + 0x410);\r\n  if (lVar3 != 0) {\r\n    aiStackX_8[0] = 0;\r\n    if (0 < *(int *)(param_2 + 0x248)) {\r\n      do {\r\n        iVar9 = aiStackX_8[0];\r\n        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);\r\n        if (plVar7 != (longlong *)0x0) {\r\n          for (lVar6 = (**(code **)(*plVar7 + 0xb8))(); lVar6 != 0;\r\n              lVar6 = *(longlong *)(lVar6 + 0xa0)) {\r\n            if (lVar6 == 0x143986f50) {\r\n              *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;\r\n              FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);\r\n              return;\r\n            }\r\n          }\r\n        }\r\n        aiStackX_8[0] = iVar9 + 1;\r\n      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));\r\n    }\r\n    do {\r\n      uVar4 = FUN_1400fdb80();\r\n      lVar5 = func_0x0001400fdd10(plVar1,uVar4,8);\r\n    } while (lVar5 != 0);\r\n    iVar9 = *(int *)(param_2 + 0x24c);\r\n    iVar2 = *(int *)(param_2 + 0x248);\r\n    if (iVar9 == 0) {\r\n      lVar5 = *plVar1;\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      puVar8 = (undefined4 *)\r\n               (**(code **)(*plVar7 + 8))\r\n                         (plVar7,&UNK_141b38b60,0x226,&UNK_141ab3808,0xc,0x41,1,0x10,0xffffffff);\r\n      *puVar8 = 0;\r\n      iVar9 = *(int *)(param_2 + 0x248);\r\n      if (1 < iVar9) {\r\n        iVar9 = 1;\r\n      }\r\n      lVar6 = (longlong)iVar9;\r\n      if (0 < iVar9) {\r\n        lVar12 = lVar5 - (longlong)puVar8;\r\n        puVar10 = (undefined8 *)(puVar8 + 1);\r\n        do {\r\n          *(undefined4 *)((longlong)puVar10 + -4) = *(undefined4 *)(lVar12 + -4 + (longlong)puVar10)\r\n          ;\r\n          *puVar10 = *(undefined8 *)(lVar12 + -0xc + (longlong)puVar10 + 0xc);\r\n          lVar6 = lVar6 + -1;\r\n          puVar10 = (undefined8 *)((longlong)puVar10 + 0xc);\r\n        } while (lVar6 != 0);\r\n      }\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar7 + 0x10))(plVar7,&UNK_141b38b60,0x239,lVar5,0x10);\r\n      *plVar1 = (longlong)puVar8;\r\n      *(undefined4 *)(param_2 + 0x24c) = 1;\r\n    }\r\n    else if ((iVar2 == iVar9) && (iVar9 < iVar9 * 2)) {\r\n      FUN_14075ac80(plVar1);\r\n    }\r\n    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;\r\n    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;\r\n    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "14001d6b0",
    "result": {
      "result": "\r\nuint FUN_14001d6b0(longlong *param_1,undefined4 *param_2)\r\n\r\n{\r\n  char cVar1;\r\n  uint uVar2;\r\n  int iVar3;\r\n  uint uVar4;\r\n  \r\n  uVar2 = *(uint *)((longlong)param_1 + 0xc);\r\n  uVar4 = uVar2 >> 8;\r\n  if ((*(uint *)(param_1 + 1) & 0xffffff) == uVar4) {\r\n    if (((byte)uVar2 & 0xc0) == 0xc0) {\r\n      if (DAT_1431aee98 == '\\0') {\r\n        if ((uVar2 & 0x3f) == 0x3f) {\r\n          if (uVar4 == 0) {\r\n            iVar3 = 1;\r\n          }\r\n          else if (uVar4 < 0x40000000) {\r\n            iVar3 = uVar4 * 2;\r\n          }\r\n          else {\r\n            iVar3 = 0x7fffffff;\r\n          }\r\n        }\r\n        else {\r\n          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);\r\n          uVar4 = iVar3 + uVar4;\r\n          iVar3 = uVar4 - (iVar3 - 1U & uVar4);\r\n        }\r\n      }\r\n      else {\r\n        iVar3 = uVar4 + 1;\r\n      }\r\n      FUN_14003a140(param_1,iVar3);\r\n    }\r\n    else {\r\n      if (DAT_1431aee98 == '\\0') {\r\n        if ((uVar2 & 0x3f) == 0x3f) {\r\n          if (uVar4 == 0) {\r\n            iVar3 = 1;\r\n          }\r\n          else if (uVar4 < 0x40000000) {\r\n            iVar3 = uVar4 * 2;\r\n          }\r\n          else {\r\n            iVar3 = 0x7fffffff;\r\n          }\r\n        }\r\n        else {\r\n          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);\r\n          uVar4 = iVar3 + uVar4;\r\n          iVar3 = uVar4 - (iVar3 - 1U & uVar4);\r\n        }\r\n      }\r\n      else {\r\n        iVar3 = uVar4 + 1;\r\n      }\r\n      cVar1 = FUN_140384fe0(param_1,iVar3);\r\n      if (cVar1 == '\\0') {\r\n        return 0xffffffff;\r\n      }\r\n    }\r\n  }\r\n  uVar2 = *(uint *)(param_1 + 1) & 0xffffff;\r\n  if (uVar2 < *(uint *)((longlong)param_1 + 0xc) >> 8) {\r\n    *(undefined4 *)(*param_1 + (ulonglong)(*(uint *)(param_1 + 1) & 0xffffff) * 4) = *param_2;\r\n    uVar2 = *(uint *)(param_1 + 1);\r\n    *(uint *)(param_1 + 1) = (uVar2 + 1 ^ uVar2) & 0xffffff ^ uVar2;\r\n    return uVar2 & 0xffffff;\r\n  }\r\n  return uVar2 - 1;\r\n}\r\n\r\n"
    }
  },
  {
    "address": "140199350",
    "result": {
      "result": "\r\nvoid FUN_140199350(undefined8 *param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong *plVar2;\r\n  undefined1 auStack_28 [32];\r\n  \r\n  lVar1 = (**(code **)*param_1)();\r\n  if (*(longlong *)(lVar1 + 0x48) != 0) {\r\n    plVar2 = (longlong *)FUN_1401993c0(auStack_28,param_1);\r\n    if (*plVar2 == *(longlong *)(lVar1 + 0x30)) {\r\n      if (*plVar2 == 0) {\r\n        return;\r\n      }\r\n      if ((((int)plVar2[1] == (int)*(undefined8 *)(lVar1 + 0x38)) &&\r\n          ((int)((ulonglong)plVar2[1] >> 0x20) ==\r\n           (int)((ulonglong)*(undefined8 *)(lVar1 + 0x38) >> 0x20))) &&\r\n         ((int)plVar2[2] == *(int *)(lVar1 + 0x40))) {\r\n        return;\r\n      }\r\n    }\r\n  }\r\n  (**(code **)(lVar1 + 0x30))((longlong)*(int *)(lVar1 + 0x38) + (longlong)param_1);\r\n  return;\r\n}\r\n\r\n"
    }
  }
]
````

## Unescaped text fields for reading

### result

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140617300(longlong param_1)

{
  longlong lVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  longlong lVar6;
  undefined8 uVar7;
  undefined4 *puVar8;
  longlong *plVar9;
  int *piVar10;
  longlong lVar11;
  longlong lVar12;
  longlong lVar13;
  undefined *puStackX_8;
  undefined8 uStackX_10;
  undefined8 uStack_90;
  longlong lStack_88;
  undefined4 uStack_80;
  undefined4 uStack_7c;
  longlong lStack_78;
  undefined4 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined1 uStack_50;
  undefined4 uStack_4c;
  undefined1 uStack_43;
  
  func_0x0001407e7400();
  uStackX_10 = *(undefined8 *)(param_1 + 0x10);
  lVar1 = *(longlong *)(param_1 + 8);
  uStack_90 = *(longlong *)(lVar1 + 0x68);
  lVar13 = 0;
  lStack_88 = 0;
  uStack_80 = 0;
  uStack_7c = 0xffffffff;
  FUN_1406136e0(&uStack_90);
  while (lStack_88 != 0) {
    if ((*(int *)(lStack_88 + 0x60) != 1) && (*(int *)(lStack_88 + 0x60) != 0xf)) {
      if (0 < (longlong)*(int *)(uStack_90 + 0x70)) {
        plVar9 = (longlong *)(*(longlong *)(uStack_90 + 0x68) + 4);
        lVar6 = lVar13;
        do {
          if (*plVar9 == lStack_88) {
            if (lVar6 != -1) {
              uVar2 = *(uint *)(*(longlong *)(uStack_90 + 0x68) + 0xc + lVar6 * 0x14);
              goto code_r0x0001406173c9;
            }
            break;
          }
          lVar6 = lVar6 + 1;
          plVar9 = (longlong *)((longlong)plVar9 + 0x14);
        } while (lVar6 < *(int *)(uStack_90 + 0x70));
      }
      uVar2 = (uint)(*(int *)(uStack_90 + 0x60) == 1);
code_r0x0001406173c9:
      uVar7 = func_0x00014063c630(&lStack_78,lStack_88,uVar2);
      FUN_140613e60(param_1 + 0x38,uVar7);
    }
    FUN_1406136e0(&uStack_90);
  }
  if ((*(byte *)(param_1 + 0x34) & 0x40) == 0) {
    lVar6 = *(longlong *)(param_1 + 0x28);
    if (lVar6 != 0) {
      plVar9 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar9 + 0x10))(plVar9,&UNK_141ab2230,0x1ec,lVar6,0x10);
    }
    *(undefined8 *)(param_1 + 0x28) = 0;
    *(uint *)(param_1 + 0x34) = *(uint *)(param_1 + 0x34) & 0xff;
  }
  *(uint *)(param_1 + 0x30) = *(uint *)(param_1 + 0x30) & 0xff000000;
  puStackX_8 = &UNK_141b9a7f0;
  if (*(longlong *)(param_1 + 0x38) != 0) {
    FUN_140618f10(&puStackX_8,*(longlong *)(param_1 + 0x38),*(uint *)(param_1 + 0x40) & 0xffffff);
  }
  if ((*(longlong *)(param_1 + 0x20) != 0) &&
     (uVar2 = *(uint *)(param_1 + 0x40) & 0xffffff, lVar12 = (longlong)(int)uVar2, lVar6 = lVar13,
     uVar2 != 0)) {
    do {
      plVar9 = (longlong *)(*(longlong *)(param_1 + 0x38) + lVar6);
      lVar11 = *(longlong *)(param_1 + 0x20);
      iVar3 = func_0x0001401431a0(*(undefined8 *)(*plVar9 + 8));
      uVar2 = *(uint *)(lVar11 + 0x10) & 0xffffff;
      if (uVar2 != 0) {
        piVar10 = *(int **)(lVar11 + 8);
        lVar11 = lVar13;
        do {
          if (*piVar10 == iVar3) {
            if (piVar10 != (int *)0x0) {
              puStackX_8 = (undefined *)CONCAT44(puStackX_8._4_4_,piVar10[6]);
              lVar11 = func_0x0001407b5e00(_D2_GameSingletonPtr + 0x3130b0,&puStackX_8);
              if (lVar11 != 0) {
                D2_Inventory_EnsureEquipmentEntity_Hypothesis(param_1,plVar9,lVar11);
              }
            }
            break;
          }
          lVar11 = lVar11 + 1;
          piVar10 = piVar10 + 8;
        } while (lVar11 < (int)uVar2);
      }
      lVar12 = lVar12 + -1;
      lVar6 = lVar6 + 0x38;
    } while (lVar12 != 0);
  }
  if ((*(longlong *)(lVar1 + 0x70) != 0) &&
     (uVar2 = *(uint *)(*(longlong *)(lVar1 + 0x70) + 0x68) & 0xffffff, lVar6 = (longlong)(int)uVar2
     , uVar2 != 0)) {
    do {
      plVar9 = (longlong *)(*(longlong *)(*(longlong *)(lVar1 + 0x70) + 0x60) + lVar13);
      if ((*plVar9 != 0) && ((plVar9[1] != 0 && (plVar9[2] != 0)))) {
        puStackX_8 = &UNK_141b9a840;
        uStack_70 = 0x3fff0010;
        uStack_68 = 0;
        uStack_60 = 0;
        uStack_58 = 0;
        uStack_50 = 0;
        uStack_4c = 0;
        uStack_43 = 1;
        lStack_78 = *plVar9;
        iVar3 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),
                              *(uint *)(param_1 + 0x40) & 0xffffff,&lStack_78);
        lStack_78 = plVar9[1];
        puStackX_8 = &UNK_141b9a840;
        uStack_70 = 0x3fff0010;
        uStack_68 = 0;
        uStack_60 = 0;
        uStack_58 = 0;
        uStack_50 = 0;
        uStack_4c = 0;
        uStack_43 = 1;
        iVar4 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),
                              *(uint *)(param_1 + 0x40) & 0xffffff,&lStack_78);
        lStack_78 = plVar9[2];
        puStackX_8 = &UNK_141b9a840;
        uStack_70 = 0x3fff0010;
        uStack_68 = 0;
        uStack_60 = 0;
        uStack_58 = 0;
        uStack_50 = 0;
        uStack_4c = 0;
        uStack_43 = 1;
        iVar5 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),
                              *(uint *)(param_1 + 0x40) & 0xffffff,&lStack_78);
        uStack_90 = CONCAT44(iVar4,iVar3);
        lStack_88 = CONCAT44(lStack_88._4_4_,iVar5);
        if ((-1 < iVar3) && ((-1 < iVar4 && (-1 < iVar5)))) {
          FUN_140613d00(param_1 + 0x48,&uStack_90);
        }
      }
      lVar13 = lVar13 + 0x18;
      lVar6 = lVar6 + -1;
    } while (lVar6 != 0);
  }
  uVar7 = FUN_140877720(uStackX_10,0x1439edb90,1);
  *(undefined8 *)(param_1 + 0xf0) = uVar7;
  puVar8 = (undefined4 *)FUN_14060ce50(uVar7,&puStackX_8,0x143987a90);
  if ((undefined4 *)(param_1 + 0x174) != puVar8) {
    *(undefined4 *)(param_1 + 0x174) = *puVar8;
  }
  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987a98);
  if ((undefined4 *)(param_1 + 0x178) != puVar8) {
    *(undefined4 *)(param_1 + 0x178) = *puVar8;
  }
  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987aa0);
  if ((undefined4 *)(param_1 + 0x17c) != puVar8) {
    *(undefined4 *)(param_1 + 0x17c) = *puVar8;
  }
  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987aa8);
  if ((undefined4 *)(param_1 + 0x180) != puVar8) {
    *(undefined4 *)(param_1 + 0x180) = *puVar8;
  }
  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987ab0);
  if ((undefined4 *)(param_1 + 0x184) != puVar8) {
    *(undefined4 *)(param_1 + 0x184) = *puVar8;
  }
  puVar8 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0xf0),&puStackX_8,0x143987ab8);
  if ((undefined4 *)(param_1 + 0x188) != puVar8) {
    *(undefined4 *)(param_1 + 0x188) = *puVar8;
  }
  return;
}


````

### result

````text

uint FUN_140613e60(longlong *param_1,undefined8 *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined8 *puVar4;
  uint uVar5;
  
  uVar2 = *(uint *)((longlong)param_1 + 0xc);
  uVar5 = uVar2 >> 8;
  if ((*(uint *)(param_1 + 1) & 0xffffff) == uVar5) {
    if (((byte)uVar2 & 0xc0) == 0xc0) {
      if (DAT_1431aee98 == '\0') {
        if ((uVar2 & 0x3f) == 0x3f) {
          if (uVar5 == 0) {
            iVar3 = 1;
          }
          else if (uVar5 < 0x40000000) {
            iVar3 = uVar5 * 2;
          }
          else {
            iVar3 = 0x7fffffff;
          }
        }
        else {
          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
          uVar5 = iVar3 + uVar5;
          iVar3 = uVar5 - (iVar3 - 1U & uVar5);
        }
      }
      else {
        iVar3 = uVar5 + 1;
      }
      FUN_140616120(param_1,iVar3);
    }
    else {
      if (DAT_1431aee98 == '\0') {
        if ((uVar2 & 0x3f) == 0x3f) {
          if (uVar5 == 0) {
            iVar3 = 1;
          }
          else if (uVar5 < 0x40000000) {
            iVar3 = uVar5 * 2;
          }
          else {
            iVar3 = 0x7fffffff;
          }
        }
        else {
          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
          uVar5 = iVar3 + uVar5;
          iVar3 = uVar5 - (iVar3 - 1U & uVar5);
        }
      }
      else {
        iVar3 = uVar5 + 1;
      }
      cVar1 = FUN_1406181c0(param_1,iVar3);
      if (cVar1 == '\0') {
        return 0xffffffff;
      }
    }
  }
  uVar2 = *(uint *)(param_1 + 1) & 0xffffff;
  if (uVar2 < *(uint *)((longlong)param_1 + 0xc) >> 8) {
    puVar4 = (undefined8 *)((ulonglong)(*(uint *)(param_1 + 1) & 0xffffff) * 0x38 + *param_1);
    *puVar4 = *param_2;
    *(undefined4 *)(puVar4 + 1) = *(undefined4 *)(param_2 + 1);
    puVar4[2] = param_2[2];
    puVar4[3] = param_2[3];
    *(undefined4 *)(puVar4 + 4) = *(undefined4 *)(param_2 + 4);
    *(undefined4 *)((longlong)puVar4 + 0x24) = *(undefined4 *)((longlong)param_2 + 0x24);
    *(undefined1 *)(puVar4 + 5) = *(undefined1 *)(param_2 + 5);
    *(undefined4 *)((longlong)puVar4 + 0x2c) = *(undefined4 *)((longlong)param_2 + 0x2c);
    *(undefined4 *)(puVar4 + 6) = *(undefined4 *)(param_2 + 6);
    *(undefined1 *)((longlong)puVar4 + 0x34) = *(undefined1 *)((longlong)param_2 + 0x34);
    *(undefined1 *)((longlong)puVar4 + 0x35) = *(undefined1 *)((longlong)param_2 + 0x35);
    uVar2 = *(uint *)(param_1 + 1);
    *(uint *)(param_1 + 1) = (uVar2 + 1 ^ uVar2) & 0xffffff ^ uVar2;
    return uVar2 & 0xffffff;
  }
  return uVar2 - 1;
}


````

### result

````text

undefined8 * FUN_14063c630(undefined8 *param_1,undefined8 param_2,undefined4 param_3)

{
  *param_1 = param_2;
  *(undefined4 *)(param_1 + 1) = 0x3fff0010;
  param_1[2] = 0;
  param_1[3] = 0;
  *(undefined4 *)(param_1 + 4) = 0;
  *(undefined1 *)(param_1 + 5) = 0;
  *(undefined8 *)((longlong)param_1 + 0x2c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x24) = param_3;
  *(undefined2 *)((longlong)param_1 + 0x34) = 0x100;
  return param_1;
}


````

### result

````text

/* WARNING: Removing unreachable block (ram,0x000140618b70) */

int FUN_140618af0(undefined8 param_1,longlong param_2,int param_3,longlong *param_4)

{
  longlong lVar1;
  int iVar2;
  longlong lVar3;
  int iVar4;
  
  if ((param_3 == 0) || (param_2 == 0)) {
    return -1;
  }
  iVar4 = 0;
  if (0 < param_3) {
    iVar4 = 0;
    do {
      iVar2 = param_3 >> 1;
      lVar1 = *(longlong *)(((longlong)iVar4 + (longlong)iVar2) * 0x38 + param_2);
      lVar3 = lVar1 - *param_4;
      if ((lVar3 < 0) || (lVar1 == *param_4 || lVar3 < 0)) {
        iVar4 = iVar4 + iVar2;
      }
      param_3 = param_3 - iVar2;
    } while (0 < iVar2);
  }
  lVar1 = *(longlong *)((longlong)iVar4 * 0x38 + param_2);
  lVar3 = lVar1 - *param_4;
  if (lVar3 < 0) {
    return -1;
  }
  iVar2 = -1;
  if (lVar1 == *param_4 || lVar3 < 0) {
    iVar2 = iVar4;
  }
  return iVar2;
}


````

### result

````text

void FUN_140618f10(undefined8 param_1,longlong param_2,uint param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  undefined8 uVar7;
  longlong *plVar8;
  longlong lVar9;
  longlong lVar10;
  longlong *plVar11;
  longlong lVar12;
  longlong lVar13;
  longlong lVar14;
  ulonglong uVar15;
  ulonglong uVar16;
  undefined8 *puVar17;
  longlong lVar18;
  ulonglong uVar19;
  undefined8 *puVar20;
  longlong lVar21;
  longlong lStack_8a8;
  undefined4 uStack_874;
  undefined4 uStack_86c;
  undefined1 uStack_864;
  undefined1 uStack_863;
  undefined4 uStack_83c;
  undefined4 uStack_834;
  undefined1 uStack_82c;
  undefined1 uStack_82b;
  longlong alStack_828 [128];
  ulonglong auStack_428 [128];
  
  if (param_3 < 2) {
    return;
  }
  lStack_8a8 = 0;
  lVar21 = 0;
  uVar19 = (ulonglong)(param_3 - 1);
  do {
    while (((longlong)(uVar19 - lStack_8a8) < 4 || (0x7e < lVar21))) {
      if (lStack_8a8 < (longlong)uVar19) {
        lVar13 = lStack_8a8 + 1;
        puVar17 = (undefined8 *)(param_2 + 0x10 + uVar19 * 0x38);
        do {
          lVar10 = lStack_8a8;
          if (lVar13 <= (longlong)uVar19) {
            lVar14 = lStack_8a8 * 0x38;
            lVar12 = lVar13 * 0x38;
            lVar9 = lVar13;
            do {
              lVar18 = *(longlong *)(lVar12 + param_2) - *(longlong *)(lVar14 + param_2);
              if ((-1 < lVar18) &&
                 (*(longlong *)(lVar12 + param_2) != *(longlong *)(lVar14 + param_2) && -1 < lVar18)
                 ) {
                lVar10 = lVar9;
                lVar14 = lVar12;
              }
              lVar9 = lVar9 + 1;
              lVar12 = lVar12 + 0x38;
            } while (lVar9 <= (longlong)uVar19);
          }
          lVar10 = lVar10 * 0x38;
          uVar19 = uVar19 - 1;
          uVar1 = *(undefined8 *)(lVar10 + 0x20 + param_2);
          uVar2 = *(undefined8 *)(lVar10 + 0x28 + param_2);
          uVar3 = *(undefined8 *)(lVar10 + 0x30 + param_2);
          uVar4 = *(undefined8 *)(lVar10 + param_2);
          uVar5 = *(undefined8 *)(lVar10 + 8 + param_2);
          uVar6 = *(undefined8 *)(lVar10 + 0x10 + param_2);
          uVar7 = *(undefined8 *)(lVar10 + 0x18 + param_2);
          *(undefined8 *)(lVar10 + param_2) = puVar17[-2];
          *(undefined4 *)(lVar10 + 8 + param_2) = *(undefined4 *)(puVar17 + -1);
          *(undefined8 *)(lVar10 + 0x10 + param_2) = *puVar17;
          *(undefined8 *)(lVar10 + 0x18 + param_2) = puVar17[1];
          *(undefined4 *)(lVar10 + 0x20 + param_2) = *(undefined4 *)(puVar17 + 2);
          *(undefined4 *)(lVar10 + 0x24 + param_2) = *(undefined4 *)((longlong)puVar17 + 0x14);
          *(undefined1 *)(lVar10 + 0x28 + param_2) = *(undefined1 *)(puVar17 + 3);
          *(undefined4 *)(lVar10 + 0x2c + param_2) = *(undefined4 *)((longlong)puVar17 + 0x1c);
          *(undefined4 *)(lVar10 + 0x30 + param_2) = *(undefined4 *)(puVar17 + 4);
          *(undefined1 *)(lVar10 + 0x34 + param_2) = *(undefined1 *)((longlong)puVar17 + 0x24);
          *(undefined1 *)(lVar10 + 0x35 + param_2) = *(undefined1 *)((longlong)puVar17 + 0x25);
          uStack_83c = (undefined4)((ulonglong)uVar1 >> 0x20);
          puVar17[-2] = uVar4;
          *(int *)(puVar17 + -1) = (int)uVar5;
          *(undefined4 *)((longlong)puVar17 + 0x14) = uStack_83c;
          uStack_834 = (undefined4)((ulonglong)uVar2 >> 0x20);
          *(undefined4 *)((longlong)puVar17 + 0x1c) = uStack_834;
          uStack_82c = (undefined1)((ulonglong)uVar3 >> 0x20);
          *puVar17 = uVar6;
          *(undefined1 *)((longlong)puVar17 + 0x24) = uStack_82c;
          uStack_82b = (undefined1)((ulonglong)uVar3 >> 0x28);
          puVar17[1] = uVar7;
          *(int *)(puVar17 + 2) = (int)uVar1;
          *(char *)(puVar17 + 3) = (char)uVar2;
          *(int *)(puVar17 + 4) = (int)uVar3;
          *(undefined1 *)((longlong)puVar17 + 0x25) = uStack_82b;
          puVar17 = puVar17 + -7;
        } while (lStack_8a8 < (longlong)uVar19);
      }
      lVar21 = lVar21 + -1;
      if (lVar21 < 0) {
        return;
      }
      uVar19 = auStack_428[lVar21];
      lStack_8a8 = alStack_828[lVar21];
    }
    plVar8 = (longlong *)(uVar19 * 0x38 + param_2);
    FUN_140613350(plVar8,param_2 + ((longlong)(uVar19 + lStack_8a8) / 2) * 0x38);
    uVar16 = uVar19 - 1;
    lVar13 = lStack_8a8;
    do {
      lVar10 = *plVar8;
      plVar11 = (longlong *)(lVar13 * 0x38 + param_2);
      do {
        lVar9 = *plVar11 - lVar10;
        if ((-1 < lVar9) && (lVar14 = lVar13, -1 < (int)(uint)(*plVar11 != lVar10 && -1 < lVar9)))
        break;
        lVar13 = lVar13 + 1;
        plVar11 = plVar11 + 7;
        lVar14 = lVar13;
      } while (lVar13 < (longlong)uVar16);
      plVar11 = (longlong *)(uVar16 * 0x38 + param_2);
      uVar15 = uVar16;
      while( true ) {
        lVar13 = *plVar11 - lVar10;
        if ((lVar13 < 0) || (*plVar11 == lVar10 || lVar13 < 0)) break;
        uVar16 = uVar15 - 1;
        plVar11 = plVar11 + -7;
        uVar15 = uVar16;
        if ((longlong)uVar16 <= lVar14) goto code_r0x0001406190f9;
      }
      uVar16 = uVar15;
      if ((longlong)uVar15 <= lVar14) break;
      lVar13 = lVar14 + 1;
      uVar16 = uVar15 - 1;
      puVar20 = (undefined8 *)(uVar15 * 0x38 + param_2);
      puVar17 = (undefined8 *)(lVar14 * 0x38 + param_2);
      uVar1 = puVar17[4];
      uVar2 = puVar17[5];
      uVar3 = puVar17[6];
      uVar4 = *puVar17;
      uVar5 = puVar17[1];
      uVar6 = puVar17[2];
      uVar7 = puVar17[3];
      *puVar17 = *puVar20;
      *(undefined4 *)(puVar17 + 1) = *(undefined4 *)(puVar20 + 1);
      puVar17[2] = puVar20[2];
      puVar17[3] = puVar20[3];
      *(undefined4 *)(puVar17 + 4) = *(undefined4 *)(puVar20 + 4);
      *(undefined4 *)((longlong)puVar17 + 0x24) = *(undefined4 *)((longlong)puVar20 + 0x24);
      *(undefined1 *)(puVar17 + 5) = *(undefined1 *)(puVar20 + 5);
      *(undefined4 *)((longlong)puVar17 + 0x2c) = *(undefined4 *)((longlong)puVar20 + 0x2c);
      *(undefined4 *)(puVar17 + 6) = *(undefined4 *)(puVar20 + 6);
      *(undefined1 *)((longlong)puVar17 + 0x34) = *(undefined1 *)((longlong)puVar20 + 0x34);
      *(undefined1 *)((longlong)puVar17 + 0x35) = *(undefined1 *)((longlong)puVar20 + 0x35);
      uStack_874 = (undefined4)((ulonglong)uVar1 >> 0x20);
      *puVar20 = uVar4;
      *(int *)(puVar20 + 1) = (int)uVar5;
      *(undefined4 *)((longlong)puVar20 + 0x24) = uStack_874;
      uStack_86c = (undefined4)((ulonglong)uVar2 >> 0x20);
      *(undefined4 *)((longlong)puVar20 + 0x2c) = uStack_86c;
      uStack_864 = (undefined1)((ulonglong)uVar3 >> 0x20);
      puVar20[2] = uVar6;
      *(undefined1 *)((longlong)puVar20 + 0x34) = uStack_864;
      uStack_863 = (undefined1)((ulonglong)uVar3 >> 0x28);
      puVar20[3] = uVar7;
      *(int *)(puVar20 + 4) = (int)uVar1;
      *(char *)(puVar20 + 5) = (char)uVar2;
      *(int *)(puVar20 + 6) = (int)uVar3;
      *(undefined1 *)((longlong)puVar20 + 0x35) = uStack_863;
      lVar14 = lVar13;
    } while (lVar13 < (longlong)uVar16);
code_r0x0001406190f9:
    lVar13 = *plVar8;
    plVar11 = (longlong *)(uVar16 * 0x38 + param_2);
    for (; (-1 < *plVar11 - lVar13 && (lStack_8a8 < (longlong)uVar16)); uVar16 = uVar16 - 1) {
      plVar11 = plVar11 + -7;
    }
    plVar11 = (longlong *)(lVar14 * 0x38 + param_2);
    while( true ) {
      lVar10 = *plVar11 - lVar13;
      if (((-1 < lVar10) && (*plVar11 != lVar13 && -1 < lVar10)) || ((longlong)uVar19 <= lVar14))
      break;
      lVar14 = lVar14 + 1;
      plVar11 = plVar11 + 7;
    }
    FUN_140613350(plVar8,lVar14 * 0x38 + param_2);
    if ((longlong)(uVar16 - lStack_8a8) < (longlong)(uVar19 - lVar14)) {
      auStack_428[lVar21] = uVar19;
      alStack_828[lVar21] = lVar14;
      lVar21 = lVar21 + 1;
      uVar19 = uVar16;
    }
    else {
      alStack_828[lVar21] = lStack_8a8;
      auStack_428[lVar21] = uVar16;
      lVar21 = lVar21 + 1;
      lStack_8a8 = lVar14;
    }
  } while( true );
}


````

### result

````text

void FUN_1406177b0(longlong param_1)

{
  uint uVar1;
  longlong lVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined1 auStack_28 [32];
  
  lVar4 = 0;
  uVar1 = *(uint *)(param_1 + 0x60) & 0xffffff;
  if (uVar1 != 0) {
    do {
      lVar2 = FUN_1406dc560(*(undefined4 *)
                             (*(longlong *)(*(longlong *)(param_1 + 0x58) + lVar4 * 8) + 8));
      if (lVar2 != 0) {
        FUN_1408847f0(lVar2,auStack_28,0x1439a6f30);
        FUN_140882fc0(auStack_28);
      }
      lVar4 = lVar4 + 1;
    } while (lVar4 < (int)uVar1);
  }
  if ((*(byte *)(param_1 + 100) & 0x40) == 0) {
    lVar4 = *(longlong *)(param_1 + 0x58);
    if (lVar4 != 0) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141ab2230,0x1ec,lVar4,0x10);
    }
    *(uint *)(param_1 + 100) = *(uint *)(param_1 + 100) & 0xff;
    *(undefined8 *)(param_1 + 0x58) = 0;
  }
  *(uint *)(param_1 + 0x60) = *(uint *)(param_1 + 0x60) & 0xff000000;
  if ((*(byte *)(param_1 + 0x44) & 0x40) == 0) {
    lVar4 = *(longlong *)(param_1 + 0x38);
    if (lVar4 != 0) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141ab2230,0x1ec,lVar4,0x10);
    }
    *(uint *)(param_1 + 0x44) = *(uint *)(param_1 + 0x44) & 0xff;
    *(undefined8 *)(param_1 + 0x38) = 0;
  }
  *(uint *)(param_1 + 0x40) = *(uint *)(param_1 + 0x40) & 0xff000000;
  if ((*(byte *)(param_1 + 0x164) & 0x40) == 0) {
    lVar4 = *(longlong *)(param_1 + 0x158);
    if (lVar4 != 0) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141ab2230,0x1ec,lVar4,0x10);
    }
    *(uint *)(param_1 + 0x164) = *(uint *)(param_1 + 0x164) & 0xff;
    *(undefined8 *)(param_1 + 0x158) = 0;
  }
  *(uint *)(param_1 + 0x160) = *(uint *)(param_1 + 0x160) & 0xff000000;
  FUN_140878e80(*(undefined8 *)(param_1 + 0x10),param_1);
  *(uint *)(param_1 + 0x1c) = *(uint *)(param_1 + 0x1c) & 0xfffffffc;
  *(undefined8 *)(param_1 + 8) = 0;
  *(undefined8 *)(param_1 + 0x10) = 0;
  return;
}


````

### result

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140617930(longlong param_1)

{
  undefined8 *puVar1;
  longlong lVar2;
  bool bVar3;
  longlong lVar4;
  undefined8 uVar5;
  longlong lVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lStack_28;
  code *pcStack_20;
  
  FUN_1407e7670();
  puVar1 = *(undefined8 **)(param_1 + 0x10);
  lVar4 = (**(code **)*puVar1)(puVar1);
  if ((*(int *)(lVar4 + 0x60) < _DAT_1439ce350) || (_DAT_1439ce354 < *(int *)(lVar4 + 0x60))) {
    bVar3 = false;
  }
  else {
    bVar3 = true;
  }
  uVar5 = FUN_140877720(puVar1,0x1439ae8e0,1);
  func_0x0001403dab80(param_1 + 0xf8,uVar5);
  uVar5 = FUN_140877720(puVar1,0x1439d13b0,1);
  func_0x0001403dab80(param_1 + 0x108,uVar5);
  uVar5 = FUN_140877720(puVar1,0x1439ae6a0,1);
  func_0x0001403dab80(param_1 + 0x118,uVar5);
  *(byte *)(param_1 + 400) = *(byte *)(param_1 + 400) & 0xfe;
  if (*(longlong *)(param_1 + 0x20) == 0) {
    lVar4 = *(longlong *)(*(longlong *)(param_1 + 8) + 0x78);
    lVar8 = 0;
    if (lVar4 != 0) {
      FUN_140614450(param_1);
      *(byte *)(param_1 + 400) =
           *(byte *)(param_1 + 400) ^ (*(byte *)(lVar4 + 0x61) ^ *(byte *)(param_1 + 400)) & 1;
      lVar6 = (longlong)*(int *)(lVar4 + 0x70);
      lVar7 = lVar8;
      if (0 < lVar6) {
        do {
          lVar2 = *(longlong *)(lVar7 + 4 + *(longlong *)(lVar4 + 0x68));
          if (lVar2 != 0) {
            FUN_140618de0(param_1,lVar2,*(undefined4 *)(lVar7 + 0xc + *(longlong *)(lVar4 + 0x68)));
          }
          lVar6 = lVar6 + -1;
          lVar7 = lVar7 + 0x14;
        } while (lVar6 != 0);
      }
    }
    if ((bVar3) &&
       (lVar4 = *(longlong *)(*(longlong *)(_D2_GameSingletonPtr + 0x1e8948) + 0x388), lVar4 != 0))
    {
      FUN_140613ff0(param_1,lVar4,0);
    }
    lVar4 = *(longlong *)(param_1 + 0xe8);
    if (lVar4 != 0) {
      if (*(char *)(lVar4 + 0x60) != '\0') {
        FUN_140614450(param_1);
      }
      *(byte *)(param_1 + 400) =
           *(byte *)(param_1 + 400) ^ (*(byte *)(lVar4 + 0x61) ^ *(byte *)(param_1 + 400)) & 1;
      lVar7 = (longlong)*(int *)(lVar4 + 0x70);
      if (0 < lVar7) {
        do {
          lVar6 = *(longlong *)(*(longlong *)(lVar4 + 0x68) + 4 + lVar8);
          if (lVar6 != 0) {
            FUN_140618de0(param_1,lVar6,*(undefined4 *)(*(longlong *)(lVar4 + 0x68) + 0xc + lVar8));
          }
          lVar8 = lVar8 + 0x14;
          lVar7 = lVar7 + -1;
        } while (lVar7 != 0);
      }
    }
  }
  else {
    D2_Inventory_ApplySavedItems_Hypothesis(param_1);
    lVar4 = *(longlong *)(param_1 + 0x20);
    if (lVar4 != 0) {
      FUN_14061a0a0(lVar4 + 8);
      FUN_1400f1520(lVar4);
    }
    *(undefined8 *)(param_1 + 0x20) = 0;
  }
  pcStack_20 = (code *)&UNK_140619da0;
  lStack_28 = param_1;
  uVar5 = func_0x000140810240();
  func_0x000140810810(uVar5,puVar1,0x9a,&lStack_28);
  pcStack_20 = (code *)&UNK_140619d90;
  lStack_28 = param_1;
  uVar5 = func_0x000140810240();
  func_0x000140810810(uVar5,puVar1,0xb6,&lStack_28);
  pcStack_20 = FUN_140619dc0;
  lStack_28 = param_1;
  uVar5 = func_0x000140810240();
  func_0x000140810810(uVar5,puVar1,0xec,&lStack_28);
  pcStack_20 = (code *)&UNK_140619d80;
  lStack_28 = param_1;
  uVar5 = func_0x000140810240();
  func_0x000140810810(uVar5,puVar1,0xd6,&lStack_28);
  return;
}


````

### result

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140617c10(longlong *param_1)

{
  uint uVar1;
  undefined8 uVar2;
  longlong *plVar3;
  longlong lVar4;
  longlong *plStack_18;
  code *pcStack_10;
  
  lVar4 = param_1[2];
  pcStack_10 = (code *)&UNK_140619da0;
  plStack_18 = param_1;
  uVar2 = func_0x000140810240();
  func_0x000140811160(uVar2,lVar4,0x9a,&plStack_18);
  pcStack_10 = (code *)&UNK_140619d90;
  plStack_18 = param_1;
  uVar2 = func_0x000140810240();
  func_0x000140811160(uVar2,lVar4,0xb6,&plStack_18);
  pcStack_10 = FUN_140619dc0;
  plStack_18 = param_1;
  uVar2 = func_0x000140810240();
  func_0x000140811160(uVar2,lVar4,0xec,&plStack_18);
  pcStack_10 = (code *)&UNK_140619d80;
  plStack_18 = param_1;
  uVar2 = func_0x000140810240();
  func_0x000140811160(uVar2,lVar4,0xd6,&plStack_18);
  plVar3 = param_1 + 0xd;
  lVar4 = 0x10;
  do {
    if (*plVar3 != 0) {
      D2_Inventory_Unequip(param_1);
    }
    plVar3 = plVar3 + 1;
    lVar4 = lVar4 + -1;
  } while (lVar4 != 0);
  func_0x0001403dab80(param_1 + 0x23,0);
  param_1[0x1e] = 0;
  func_0x0001403dab80(param_1 + 0x1f,0);
  func_0x0001403dab80(param_1 + 0x21,0);
  *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xfffffff3;
  if ((*(uint *)((longlong)param_1 + 0x1c) >> 0x1c & 1) == 0) {
    if (((byte)*(uint *)((longlong)param_1 + 0x1c) & 0x30) < 0x20) {
      uVar2 = (**(code **)(*param_1 + 0x88))();
      FUN_14048ff60(uVar2,param_1);
    }
    uVar1 = *(uint *)((longlong)param_1 + 0x1c) >> 4 & 3;
    if (uVar1 == 1) {
      FUN_1407ec3e0(*(undefined8 *)(_D2_GameSingletonPtr + 0x1e8960),param_1);
      *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xf800000f;
      return;
    }
    if (uVar1 == 3) {
      FUN_1407ec450(*(undefined8 *)(_D2_GameSingletonPtr + 0x1e8960),param_1);
    }
    *(uint *)((longlong)param_1 + 0x1c) = *(uint *)((longlong)param_1 + 0x1c) & 0xf800000f;
  }
  return;
}


````

### result

````text

undefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  longlong lVar4;
  undefined *puStackX_8;
  undefined8 uStack_40;
  undefined4 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined1 uStack_18;
  undefined4 uStack_14;
  undefined1 uStack_b;
  
  puStackX_8 = &UNK_141b9a840;
  uStack_38 = 0x3fff0010;
  uStack_30 = 0;
  uStack_28 = 0;
  uStack_20 = 0;
  uStack_18 = 0;
  uStack_14 = 0;
  uStack_b = 1;
  uStack_40 = param_2;
  iVar2 = FUN_140618af0(&puStackX_8,*(undefined8 *)(param_1 + 0x38),
                        *(uint *)(param_1 + 0x40) & 0xffffff,&uStack_40,0xfffffffffffffffe);
  puStackX_8 = &UNK_141b9a808;
  if (iVar2 < 0) {
    uVar3 = 0;
  }
  else {
    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);
    uVar1 = *(undefined4 *)(lVar4 + 0x20);
    uVar3 = func_0x00014063cc00(lVar4,param_3);
    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);
  }
  return uVar3;
}


````

### result

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140617e70(longlong param_1,undefined8 *param_2)

{
  undefined4 uVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 *puVar4;
  
  if (*(longlong *)(param_1 + 0x20) == 0) {
    puVar4 = *(undefined8 **)(param_1 + 0x10);
    lVar3 = (**(code **)*puVar4)(puVar4);
    if ((_DAT_1439ce350 <= *(int *)(lVar3 + 0x60)) && (*(int *)(lVar3 + 0x60) <= _DAT_1439ce354)) {
      uVar2 = *param_2;
      uVar1 = *(undefined4 *)(puVar4 + 0x15);
      lVar3 = func_0x000140810240();
      puVar4 = (undefined8 *)FUN_1408112c0(lVar3 + 0xc8e8,0x16b,8,uVar1);
      if (puVar4 != (undefined8 *)0x0) {
        *puVar4 = uVar2;
      }
    }
  }
  return;
}


````

### result

````text

void FUN_140759e70(longlong param_1,longlong param_2)

{
  longlong *plVar1;
  int iVar2;
  longlong lVar3;
  undefined4 uVar4;
  longlong lVar5;
  longlong lVar6;
  longlong *plVar7;
  undefined4 *puVar8;
  int iVar9;
  undefined8 *puVar10;
  int iVar11;
  longlong lVar12;
  longlong lVar13;
  int aiStackX_8 [2];
  
  plVar1 = (longlong *)(param_2 + 0x240);
  lVar5 = FUN_1408f2e50(param_2);
  func_0x000140783e00(lVar5);
  lVar3 = *(longlong *)(param_1 + 1000);
  if (lVar3 != 0) {
    aiStackX_8[0] = 0;
    if (0 < *(int *)(param_2 + 0x248)) {
      do {
        iVar9 = aiStackX_8[0];
        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);
        if ((plVar7 != (longlong *)0x0) &&
           (lVar6 = (**(code **)(*plVar7 + 0xb8))(), lVar6 == 0x143a269d0)) {
          *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;
          FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);
          goto code_r0x000140759f8a;
        }
        aiStackX_8[0] = iVar9 + 1;
      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));
    }
    do {
      uVar4 = FUN_1400fdb80();
      lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);
    } while (lVar6 != 0);
    iVar9 = *(int *)(param_2 + 0x24c);
    iVar2 = *(int *)(param_2 + 0x248);
    if (iVar9 == 0) {
      iVar11 = 1;
code_r0x000140759f6d:
      FUN_14075ac80(plVar1,iVar11);
    }
    else if ((iVar2 == iVar9) && (iVar11 = iVar9 * 2, iVar9 < iVar11)) goto code_r0x000140759f6d;
    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;
    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;
    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;
  }
code_r0x000140759f8a:
  lVar3 = *(longlong *)(param_1 + 0x3f0);
  if ((lVar3 == 0) || ((*(byte *)(lVar3 + 0x60) & 1) == 0)) goto code_r0x00014075a07a;
  aiStackX_8[0] = 0;
  if (0 < *(int *)(param_2 + 0x248)) {
    do {
      iVar9 = aiStackX_8[0];
      plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);
      if (((plVar7 != (longlong *)0x0) &&
          (lVar6 = (**(code **)(*plVar7 + 0xb8))(plVar7), lVar6 == 0x143a10dd0)) &&
         ((*(byte *)(plVar7 + 0xc) & 1) != 0)) {
        *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;
        FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);
        goto code_r0x00014075a07a;
      }
      aiStackX_8[0] = iVar9 + 1;
    } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));
  }
  do {
    uVar4 = FUN_1400fdb80();
    lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);
  } while (lVar6 != 0);
  iVar9 = *(int *)(param_2 + 0x24c);
  iVar2 = *(int *)(param_2 + 0x248);
  if (iVar9 == 0) {
    iVar11 = 1;
code_r0x00014075a05d:
    FUN_14075ac80(plVar1,iVar11);
  }
  else if ((iVar2 == iVar9) && (iVar11 = iVar9 * 2, iVar9 < iVar11)) goto code_r0x00014075a05d;
  *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;
  *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;
  *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;
code_r0x00014075a07a:
  lVar3 = *(longlong *)(param_1 + 0x400);
  if (lVar3 != 0) {
    aiStackX_8[0] = 0;
    if (0 < *(int *)(param_2 + 0x248)) {
      do {
        iVar9 = aiStackX_8[0];
        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);
        if (((plVar7 != (longlong *)0x0) &&
            (lVar6 = (**(code **)(*plVar7 + 0xb8))(plVar7), lVar6 == 0x143a193d0)) &&
           ((*(byte *)(plVar7 + 0xc) & 1) != 0)) {
          *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;
          FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);
          goto code_r0x00014075a224;
        }
        aiStackX_8[0] = iVar9 + 1;
      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));
    }
    do {
      uVar4 = FUN_1400fdb80();
      lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);
    } while (lVar6 != 0);
    iVar9 = *(int *)(param_2 + 0x24c);
    iVar2 = *(int *)(param_2 + 0x248);
    if (iVar9 == 0) {
      lVar6 = *plVar1;
      plVar7 = (longlong *)FUN_1401548e0();
      puVar8 = (undefined4 *)
               (**(code **)(*plVar7 + 8))
                         (plVar7,&UNK_141b38b60,0x226,&UNK_141ab3808,0xc,0x41,1,0x10,0xffffffff);
      *puVar8 = 0;
      iVar9 = *(int *)(param_2 + 0x248);
      if (1 < *(int *)(param_2 + 0x248)) {
        iVar9 = 1;
      }
      lVar12 = (longlong)iVar9;
      if (0 < iVar9) {
        lVar13 = lVar6 - (longlong)puVar8;
        puVar10 = (undefined8 *)(puVar8 + 1);
        do {
          *(undefined4 *)((longlong)puVar10 + -4) = *(undefined4 *)(lVar13 + -4 + (longlong)puVar10)
          ;
          *puVar10 = *(undefined8 *)(lVar13 + -0xc + (longlong)puVar10 + 0xc);
          lVar12 = lVar12 + -1;
          puVar10 = (undefined8 *)((longlong)puVar10 + 0xc);
        } while (lVar12 != 0);
      }
      plVar7 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar7 + 0x10))(plVar7,&UNK_141b38b60,0x239,lVar6,0x10);
      *(undefined4 *)(param_2 + 0x24c) = 1;
      *plVar1 = (longlong)puVar8;
    }
    else if ((iVar2 == iVar9) && (iVar9 < iVar9 * 2)) {
      FUN_14075ac80(plVar1);
    }
    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;
    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;
    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;
  }
code_r0x00014075a224:
  lVar3 = *(longlong *)(param_1 + 0x408);
  if (lVar3 != 0) {
    aiStackX_8[0] = 0;
    if (0 < *(int *)(param_2 + 0x248)) {
      do {
        iVar9 = aiStackX_8[0];
        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);
        if ((plVar7 != (longlong *)0x0) &&
           (lVar6 = (**(code **)(*plVar7 + 0xb8))(), lVar6 == 0x143a1e3d0)) {
          *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;
          FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);
          goto code_r0x00014075a3c8;
        }
        aiStackX_8[0] = iVar9 + 1;
      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));
    }
    do {
      uVar4 = FUN_1400fdb80();
      lVar6 = func_0x0001400fdd10(plVar1,uVar4,8);
    } while (lVar6 != 0);
    iVar9 = *(int *)(param_2 + 0x24c);
    iVar2 = *(int *)(param_2 + 0x248);
    if (iVar9 == 0) {
      lVar6 = *plVar1;
      plVar7 = (longlong *)FUN_1401548e0();
      puVar8 = (undefined4 *)
               (**(code **)(*plVar7 + 8))
                         (plVar7,&UNK_141b38b60,0x226,&UNK_141ab3808,0xc,0x41,1,0x10,0xffffffff);
      *puVar8 = 0;
      iVar9 = *(int *)(param_2 + 0x248);
      if (1 < *(int *)(param_2 + 0x248)) {
        iVar9 = 1;
      }
      lVar12 = (longlong)iVar9;
      if (0 < iVar9) {
        lVar13 = lVar6 - (longlong)puVar8;
        puVar10 = (undefined8 *)(puVar8 + 1);
        do {
          *(undefined4 *)((longlong)puVar10 + -4) = *(undefined4 *)(lVar13 + -4 + (longlong)puVar10)
          ;
          *puVar10 = *(undefined8 *)(lVar13 + -0xc + (longlong)puVar10 + 0xc);
          lVar12 = lVar12 + -1;
          puVar10 = (undefined8 *)((longlong)puVar10 + 0xc);
        } while (lVar12 != 0);
      }
      plVar7 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar7 + 0x10))(plVar7,&UNK_141b38b60,0x239,lVar6,0x10);
      *(undefined4 *)(param_2 + 0x24c) = 1;
      *plVar1 = (longlong)puVar8;
    }
    else if ((iVar2 == iVar9) && (iVar9 < iVar9 * 2)) {
      FUN_14075ac80(plVar1);
    }
    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;
    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;
    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;
  }
code_r0x00014075a3c8:
  lVar3 = *(longlong *)(param_1 + 0x410);
  if (lVar3 != 0) {
    aiStackX_8[0] = 0;
    if (0 < *(int *)(param_2 + 0x248)) {
      do {
        iVar9 = aiStackX_8[0];
        plVar7 = *(longlong **)(*plVar1 + 4 + (longlong)aiStackX_8[0] * 0xc);
        if (plVar7 != (longlong *)0x0) {
          for (lVar6 = (**(code **)(*plVar7 + 0xb8))(); lVar6 != 0;
              lVar6 = *(longlong *)(lVar6 + 0xa0)) {
            if (lVar6 == 0x143986f50) {
              *(longlong *)(*plVar1 + 4 + (longlong)iVar9 * 0xc) = lVar3;
              FUN_14001d6b0(lVar5 + 0x380,aiStackX_8);
              return;
            }
          }
        }
        aiStackX_8[0] = iVar9 + 1;
      } while (aiStackX_8[0] < *(int *)(param_2 + 0x248));
    }
    do {
      uVar4 = FUN_1400fdb80();
      lVar5 = func_0x0001400fdd10(plVar1,uVar4,8);
    } while (lVar5 != 0);
    iVar9 = *(int *)(param_2 + 0x24c);
    iVar2 = *(int *)(param_2 + 0x248);
    if (iVar9 == 0) {
      lVar5 = *plVar1;
      plVar7 = (longlong *)FUN_1401548e0();
      puVar8 = (undefined4 *)
               (**(code **)(*plVar7 + 8))
                         (plVar7,&UNK_141b38b60,0x226,&UNK_141ab3808,0xc,0x41,1,0x10,0xffffffff);
      *puVar8 = 0;
      iVar9 = *(int *)(param_2 + 0x248);
      if (1 < iVar9) {
        iVar9 = 1;
      }
      lVar6 = (longlong)iVar9;
      if (0 < iVar9) {
        lVar12 = lVar5 - (longlong)puVar8;
        puVar10 = (undefined8 *)(puVar8 + 1);
        do {
          *(undefined4 *)((longlong)puVar10 + -4) = *(undefined4 *)(lVar12 + -4 + (longlong)puVar10)
          ;
          *puVar10 = *(undefined8 *)(lVar12 + -0xc + (longlong)puVar10 + 0xc);
          lVar6 = lVar6 + -1;
          puVar10 = (undefined8 *)((longlong)puVar10 + 0xc);
        } while (lVar6 != 0);
      }
      plVar7 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar7 + 0x10))(plVar7,&UNK_141b38b60,0x239,lVar5,0x10);
      *plVar1 = (longlong)puVar8;
      *(undefined4 *)(param_2 + 0x24c) = 1;
    }
    else if ((iVar2 == iVar9) && (iVar9 < iVar9 * 2)) {
      FUN_14075ac80(plVar1);
    }
    *(undefined4 *)(*plVar1 + (longlong)iVar2 * 0xc) = uVar4;
    *(longlong *)(*plVar1 + 4 + (longlong)iVar2 * 0xc) = lVar3;
    *(int *)(param_2 + 0x248) = *(int *)(param_2 + 0x248) + 1;
  }
  return;
}


````

### result

````text

uint FUN_14001d6b0(longlong *param_1,undefined4 *param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  
  uVar2 = *(uint *)((longlong)param_1 + 0xc);
  uVar4 = uVar2 >> 8;
  if ((*(uint *)(param_1 + 1) & 0xffffff) == uVar4) {
    if (((byte)uVar2 & 0xc0) == 0xc0) {
      if (DAT_1431aee98 == '\0') {
        if ((uVar2 & 0x3f) == 0x3f) {
          if (uVar4 == 0) {
            iVar3 = 1;
          }
          else if (uVar4 < 0x40000000) {
            iVar3 = uVar4 * 2;
          }
          else {
            iVar3 = 0x7fffffff;
          }
        }
        else {
          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
          uVar4 = iVar3 + uVar4;
          iVar3 = uVar4 - (iVar3 - 1U & uVar4);
        }
      }
      else {
        iVar3 = uVar4 + 1;
      }
      FUN_14003a140(param_1,iVar3);
    }
    else {
      if (DAT_1431aee98 == '\0') {
        if ((uVar2 & 0x3f) == 0x3f) {
          if (uVar4 == 0) {
            iVar3 = 1;
          }
          else if (uVar4 < 0x40000000) {
            iVar3 = uVar4 * 2;
          }
          else {
            iVar3 = 0x7fffffff;
          }
        }
        else {
          iVar3 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
          uVar4 = iVar3 + uVar4;
          iVar3 = uVar4 - (iVar3 - 1U & uVar4);
        }
      }
      else {
        iVar3 = uVar4 + 1;
      }
      cVar1 = FUN_140384fe0(param_1,iVar3);
      if (cVar1 == '\0') {
        return 0xffffffff;
      }
    }
  }
  uVar2 = *(uint *)(param_1 + 1) & 0xffffff;
  if (uVar2 < *(uint *)((longlong)param_1 + 0xc) >> 8) {
    *(undefined4 *)(*param_1 + (ulonglong)(*(uint *)(param_1 + 1) & 0xffffff) * 4) = *param_2;
    uVar2 = *(uint *)(param_1 + 1);
    *(uint *)(param_1 + 1) = (uVar2 + 1 ^ uVar2) & 0xffffff ^ uVar2;
    return uVar2 & 0xffffff;
  }
  return uVar2 - 1;
}


````

### result

````text

void FUN_140199350(undefined8 *param_1)

{
  longlong lVar1;
  longlong *plVar2;
  undefined1 auStack_28 [32];
  
  lVar1 = (**(code **)*param_1)();
  if (*(longlong *)(lVar1 + 0x48) != 0) {
    plVar2 = (longlong *)FUN_1401993c0(auStack_28,param_1);
    if (*plVar2 == *(longlong *)(lVar1 + 0x30)) {
      if (*plVar2 == 0) {
        return;
      }
      if ((((int)plVar2[1] == (int)*(undefined8 *)(lVar1 + 0x38)) &&
          ((int)((ulonglong)plVar2[1] >> 0x20) ==
           (int)((ulonglong)*(undefined8 *)(lVar1 + 0x38) >> 0x20))) &&
         ((int)plVar2[2] == *(int *)(lVar1 + 0x40))) {
        return;
      }
    }
  }
  (**(code **)(lVar1 + 0x30))((longlong)*(int *)(lVar1 + 0x38) + (longlong)param_1);
  return;
}


````
