# partner-asset-decompilations.json

Original: [outputs/d2-coop-lab/analysis/ghidra/partner-asset-decompilations.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/partner-asset-decompilations.json>)

SHA-256: `5B89C4A38C94BF5BBDD1CAD84268E22398EF29EF5CA56FE11FABC3C277D39F85`; 21691 bytes.

## Original saved record

````json
{
  "executableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "decompilations": [
    {
      "address": "140003cc0",
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nlonglong FUN_140003cc0(ulonglong param_1,char *param_2,char param_3)\r\n\r\n{\r\n  code *pcVar1;\r\n  ulonglong uVar2;\r\n  char cVar3;\r\n  int iVar4;\r\n  char *pcVar5;\r\n  longlong lVar6;\r\n  ulonglong uVar7;\r\n  undefined8 uVar8;\r\n  ulonglong uVar9;\r\n  uint uVar10;\r\n  ulonglong uVar11;\r\n  ulonglong uVar12;\r\n  undefined2 auStackX_10 [4];\r\n  char cStackX_18;\r\n  ulonglong uStack_f8;\r\n  undefined1 auStack_f0 [40];\r\n  undefined1 auStack_c8 [40];\r\n  undefined1 auStack_a0 [8];\r\n  undefined1 auStack_98 [8];\r\n  undefined8 uStack_90;\r\n  undefined1 auStack_88 [8];\r\n  undefined1 auStack_80 [8];\r\n  undefined1 uStack_78;\r\n  char acStack_77 [63];\r\n  \r\n  if (param_2 == (char *)0x0) {\r\n    return 0;\r\n  }\r\n  uStack_90 = 0xfffffffffffffffe;\r\n  if ((param_2 == (char *)0x0) || (*param_2 == '\\0')) {\r\n    if (param_3 == '\\0') {\r\n      return 0;\r\n    }\r\n    param_2 = \"_emptyName\";\r\n  }\r\n  cStackX_18 = param_3;\r\n  FUN_14013fb30(auStack_c8);\r\n  cVar3 = func_0x000140143bd0(param_2);\r\n  pcVar5 = param_2;\r\n  if (cVar3 == '\\0') {\r\n    FUN_14013ff40(auStack_c8,param_2);\r\n    FUN_140144060(auStack_c8,1);\r\n    pcVar5 = (char *)D2_InternedName_Resolve(auStack_c8);\r\n  }\r\n  lVar6 = FUN_14005feb0(param_1,pcVar5);\r\n  if (lVar6 != 0) {\r\nLAB_14001e1ab:\r\n    FUN_14013fd10(auStack_c8);\r\n    return lVar6;\r\n  }\r\n  FUN_1401430a0(pcVar5,acStack_77,0x3f);\r\n  iVar4 = func_0x000140141460(acStack_77,&UNK_141abb784);\r\n  if ((((_DAT_1432c7ed4 == 0) && (param_3 == '\\0')) && (iVar4 == 0)) &&\r\n     (cVar3 = (**(code **)(*_DAT_14228baf0 + 0x98))(_DAT_14228baf0,pcVar5,0), cVar3 == '\\0'))\r\n  goto LAB_14001e471;\r\n  uVar12 = param_1;\r\n  if (iVar4 == 0) goto LAB_14001e492;\r\n  uVar11 = 0;\r\n  uStack_f8 = uVar11;\r\n  if (((acStack_77[0] == '\\0') ||\r\n      (uStack_f8 = 0, (~(byte)(*(uint *)(param_1 + 0xec) >> 3) & 1) == 0)) ||\r\n     (param_1 == 0x142434620)) {\r\nLAB_14001e1cf:\r\n    iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();\r\n    uVar9 = uVar11;\r\n    if (0 < iVar4) {\r\n      do {\r\n        uVar7 = (**(code **)(*_DAT_1422738a8 + 0x58))(_DAT_1422738a8,uVar9);\r\n        uVar2 = uVar7;\r\n        if (uVar7 != param_1) {\r\n          for (; uVar2 != 0; uVar2 = *(ulonglong *)(uVar2 + 0xa0)) {\r\n            if (uVar2 == param_1) {\r\n              lVar6 = FUN_14005feb0(uVar7,pcVar5);\r\n              if (lVar6 != 0) {\r\n                FUN_14013fd10(auStack_c8);\r\n                return lVar6;\r\n              }\r\n              break;\r\n            }\r\n          }\r\n        }\r\n        uVar10 = (int)uVar9 + 1;\r\n        uVar9 = (ulonglong)uVar10;\r\n        iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();\r\n      } while ((int)uVar10 < iVar4);\r\n    }\r\n  }\r\n  else {\r\n    uStack_78 = 0x2e;\r\n    uStack_f8 = (**(code **)(*_DAT_1422738a8 + 0x68))(_DAT_1422738a8,&uStack_78);\r\n    if (uStack_f8 == 0) {\r\n      func_0x000140166830(&UNK_141abba10,param_2,*(undefined8 *)(param_1 + 0x78));\r\n      goto LAB_14001e1cf;\r\n    }\r\n    lVar6 = FUN_14005feb0(uStack_f8,pcVar5);\r\n    if (lVar6 != 0) goto LAB_14001e1ab;\r\n  }\r\n  if (_DAT_1432c7ed4 < 2) {\r\n    uVar9 = FUN_14001df10(param_1,pcVar5);\r\n  }\r\n  else {\r\n    FUN_14013fb30(auStack_f0);\r\n    FUN_14001e8c0(auStack_f0,param_1,pcVar5);\r\n    uVar8 = D2_InternedName_Resolve(auStack_f0);\r\n    auStackX_10[0] = 0x7fff;\r\n    cVar3 = (**(code **)(*_DAT_142280e08 + 0x80))\r\n                      (_DAT_142280e08,uVar8,auStack_80,auStack_a0,auStack_88,auStack_98,auStackX_10,\r\n                       1);\r\n    uVar9 = (ulonglong)(cVar3 != '\\0');\r\n    FUN_14013fd10(auStack_f0);\r\n  }\r\n  param_3 = cStackX_18;\r\n  if (uVar9 != 0) goto LAB_14001e492;\r\n  if (uStack_f8 == 0) {\r\n    iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();\r\n    uVar12 = param_1;\r\n    if (0 < iVar4) {\r\n      uVar9 = 0;\r\n      do {\r\n        uStack_f8 = (**(code **)(*_DAT_1422738a8 + 0x58))(_DAT_1422738a8,uVar11);\r\n        uVar12 = uStack_f8;\r\n        if (uStack_f8 != param_1) {\r\n          for (; uVar12 != 0; uVar12 = *(ulonglong *)(uVar12 + 0xa0)) {\r\n            if (uVar12 == param_1) {\r\n              if (_DAT_1432c7ed4 < 2) {\r\n                uVar9 = FUN_14001df10(uStack_f8,pcVar5);\r\n                if (uVar9 != 0) goto LAB_14001e462;\r\n              }\r\n              else {\r\n                FUN_14013fb30(auStack_f0);\r\n                FUN_14001e8c0(auStack_f0,uStack_f8,pcVar5);\r\n                uVar8 = D2_InternedName_Resolve(auStack_f0);\r\n                cVar3 = FUN_14001dec0(uVar8);\r\n                if (cVar3 != '\\0') {\r\n                  FUN_14013fd10(auStack_f0);\r\n                  uVar12 = uStack_f8;\r\n                  param_3 = cStackX_18;\r\n                  goto LAB_14001e492;\r\n                }\r\n                FUN_14013fd10(auStack_f0);\r\n              }\r\n              break;\r\n            }\r\n          }\r\n        }\r\n        uVar10 = (int)uVar11 + 1;\r\n        uVar11 = (ulonglong)uVar10;\r\n        iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();\r\n        uStack_f8 = param_1;\r\n      } while ((int)uVar10 < iVar4);\r\n      goto LAB_14001e462;\r\n    }\r\n  }\r\n  else {\r\n    if (_DAT_1432c7ed4 < 2) {\r\n      uVar9 = FUN_14001df10(uStack_f8,pcVar5);\r\n      uVar12 = param_1;\r\n      if (uVar9 == 0) goto LAB_14001e467;\r\n    }\r\n    else {\r\n      FUN_14013fb30(auStack_f0);\r\n      FUN_14001e8c0(auStack_f0,uStack_f8,pcVar5);\r\n      uVar8 = D2_InternedName_Resolve(auStack_f0);\r\n      cVar3 = FUN_14001dec0(uVar8);\r\n      uVar12 = param_1;\r\n      if (cVar3 != '\\0') {\r\n        uVar12 = uStack_f8;\r\n      }\r\n      uStack_f8 = uVar12;\r\n      uVar9 = (ulonglong)(cVar3 != '\\0');\r\n      FUN_14013fd10(auStack_f0);\r\n    }\r\nLAB_14001e462:\r\n    uVar12 = uStack_f8;\r\n    param_3 = cStackX_18;\r\n    if (uVar9 != 0) goto LAB_14001e492;\r\n  }\r\nLAB_14001e467:\r\n  uStack_f8 = uVar12;\r\n  uVar12 = uStack_f8;\r\n  param_3 = cStackX_18;\r\n  if (cStackX_18 == '\\0') {\r\nLAB_14001e471:\r\n    FUN_14013fd10(auStack_c8);\r\n    return 0;\r\n  }\r\nLAB_14001e492:\r\n  cVar3 = (**(code **)(*_DAT_142274420 + 0x118))();\r\n  if ((cVar3 != '\\0') && (cVar3 = (**(code **)(*_DAT_142274420 + 0x138))(), cVar3 == '\\0')) {\r\n    if (_DAT_142be6cf8 == 2) {\r\n      uVar8 = FUN_140060610(param_1,1);\r\n      func_0x000140166830(&UNK_141abba88,uVar8,param_2);\r\n      pcVar1 = (code *)swi(3);\r\n      lVar6 = (*pcVar1)();\r\n      return lVar6;\r\n    }\r\n    if (_DAT_142be6cf8 == 1) {\r\n      uVar8 = FUN_140060610(param_1,1);\r\n      func_0x000140166830(&UNK_141abba88,uVar8,param_2);\r\n    }\r\n  }\r\n  lVar6 = FUN_1400607f0(uVar12,pcVar5,param_3);\r\n  FUN_14013fd10(auStack_c8);\r\n  return lVar6;\r\n}\r\n\r\n"
    },
    {
      "address": "14001ea30",
      "result": "\r\nundefined8 FUN_14001ea30(longlong param_1,char *param_2)\r\n\r\n{\r\n  int iVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  \r\n  if ((param_2 != (char *)0x0) && (*param_2 != '\\0')) {\r\n    iVar2 = 0;\r\n    if ((*(uint *)(param_1 + 0x10) & 0xffffff) != 0) {\r\n      lVar3 = 0;\r\n      do {\r\n        iVar1 = func_0x000140143650(*(undefined8 *)\r\n                                     (*(longlong *)(*(longlong *)(param_1 + 8) + lVar3) + 0x78),\r\n                                    param_2);\r\n        if (iVar1 == 0) {\r\n          return *(undefined8 *)(*(longlong *)(param_1 + 8) + (longlong)iVar2 * 8);\r\n        }\r\n        iVar2 = iVar2 + 1;\r\n        lVar3 = lVar3 + 8;\r\n      } while (iVar2 < (int)(*(uint *)(param_1 + 0x10) & 0xffffff));\r\n    }\r\n    return 0;\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
    },
    {
      "address": "140021a40",
      "result": "\r\n/* WARNING: Possible PIC construction at 0x000140021a5b: Changing call to branch */\r\n/* WARNING: Possible PIC construction at 0x000140021a96: Changing call to branch */\r\n/* WARNING: Possible PIC construction at 0x000140021b07: Changing call to branch */\r\n/* WARNING: Possible PIC construction at 0x000140021b85: Changing call to branch */\r\n/* WARNING: Possible PIC construction at 0x0001400121c4: Changing call to branch */\r\n/* WARNING: Removing unreachable block (ram,0x000140021b8a) */\r\n/* WARNING: Removing unreachable block (ram,0x000140012160) */\r\n/* WARNING: Removing unreachable block (ram,0x0001400121a9) */\r\n/* WARNING: Removing unreachable block (ram,0x000140021b0c) */\r\n/* WARNING: Removing unreachable block (ram,0x000140021a9b) */\r\n/* WARNING: Removing unreachable block (ram,0x000140021a60) */\r\n/* WARNING: Removing unreachable block (ram,0x000140021ab0) */\r\n/* WARNING: Removing unreachable block (ram,0x000140021a7a) */\r\n/* WARNING: Removing unreachable block (ram,0x000140021a80) */\r\n/* WARNING: Removing unreachable block (ram,0x0001400121c9) */\r\n/* WARNING: Removing unreachable block (ram,0x0001400121d2) */\r\n\r\nvoid FUN_140021a40(int *param_1,undefined *param_2,undefined *param_3,undefined8 param_4)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  undefined *puVar3;\r\n  int iVar4;\r\n  longlong lVar5;\r\n  undefined8 auStack_30 [5];\r\n  \r\n  if (*param_1 < 2) {\r\n    register0x00000020 = (BADSPACEBASE *)auStack_30;\r\n  }\r\n  else {\r\n    param_2 = &UNK_141ab2848;\r\n    if (*param_1 < 2) {\r\n      puVar3 = &UNK_141ab2848;\r\n    }\r\n    else {\r\n      puVar3 = *(undefined **)(param_1 + 4);\r\n    }\r\n    auStack_30[0] = 0x140021aec;\r\n    lVar1 = FUN_14001ea30(0x1424367e0,puVar3);\r\n    if (lVar1 == 0) {\r\n      if (1 < *param_1) {\r\n        param_2 = *(undefined **)(param_1 + 4);\r\n      }\r\n      register0x00000020 = (BADSPACEBASE *)auStack_30;\r\n    }\r\n    else {\r\n      if (*param_1 != 3) {\r\n        iVar4 = 0;\r\n        if ((*(uint *)(lVar1 + 0xb8) & 0xffffff) != 0) {\r\n          lVar5 = 0;\r\n          do {\r\n            auStack_30[0] = 0x140021bda;\r\n            lVar2 = FUN_140003cc0(lVar1,*(undefined8 *)\r\n                                         (*(longlong *)(lVar5 + *(longlong *)(lVar1 + 0xb0)) + 8),0)\r\n            ;\r\n            if (lVar2 != 0) {\r\n              auStack_30[0] = 0x140021be9;\r\n              FUN_140012160(lVar2,1);\r\n            }\r\n            iVar4 = iVar4 + 1;\r\n            lVar5 = lVar5 + 8;\r\n          } while (iVar4 < (int)(*(uint *)(lVar1 + 0xb8) & 0xffffff));\r\n        }\r\n        return;\r\n      }\r\n      param_3 = (undefined *)0x0;\r\n      auStack_30[0] = 0x140021b3b;\r\n      lVar1 = FUN_140003cc0(lVar1,*(undefined8 *)(param_1 + 6));\r\n      if (lVar1 == 0) {\r\n        if (*param_1 < 3) {\r\n          param_3 = &UNK_141ab2848;\r\n        }\r\n        else {\r\n          param_3 = *(undefined **)(param_1 + 6);\r\n        }\r\n        if (1 < *param_1) {\r\n          param_2 = *(undefined **)(param_1 + 4);\r\n        }\r\n      }\r\n      else {\r\n        param_2 = *(undefined **)(lVar1 + 8);\r\n        register0x00000020 = (BADSPACEBASE *)auStack_30;\r\n      }\r\n    }\r\n  }\r\n  *(undefined **)((longlong)register0x00000020 + 0x10) = param_2;\r\n  *(undefined **)((longlong)register0x00000020 + 0x18) = param_3;\r\n  *(undefined8 *)((longlong)register0x00000020 + 0x20) = param_4;\r\n  return;\r\n}\r\n\r\n"
    },
    {
      "address": "1408a9430",
      "result": "\r\nvoid FUN_1408a9430(undefined8 param_1,longlong *param_2,longlong param_3,undefined4 *param_4)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 uVar2;\r\n  undefined8 uVar3;\r\n  undefined8 uVar4;\r\n  undefined1 auStackX_18 [8];\r\n  undefined4 uStack_1f8;\r\n  undefined4 uStack_1f4;\r\n  undefined4 uStack_1f0;\r\n  \r\n  func_0x0001408a2dc0(auStackX_18,param_1,1);\r\n  if (param_3 != 0) {\r\n    (**(code **)(*param_2 + 0x38))(param_2,param_3);\r\n    FUN_1401464d0(&uStack_1f8,0);\r\n    uVar1 = FUN_14006d040(param_3);\r\n    uVar3 = func_0x000140011780(param_3);\r\n    uVar2 = func_0x000140014520(param_3);\r\n    uVar4 = func_0x00014015bd10(param_3);\r\n    FUN_140149130(&uStack_1f8,uVar4,uVar2,uVar3,uVar1);\r\n    uStack_1f8 = *param_4;\r\n    uStack_1f4 = param_4[1];\r\n    uStack_1f0 = param_4[2];\r\n    FUN_14098df10(param_2,&uStack_1f8,1);\r\n    FUN_140146580(&uStack_1f8);\r\n  }\r\n  func_0x0001408a2de0(auStackX_18);\r\n  return;\r\n}\r\n\r\n"
    },
    {
      "address": "1408aecc0",
      "result": "\r\nvoid FUN_1408aecc0(longlong param_1,longlong param_2,ulonglong param_3,undefined8 param_4)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  undefined1 auStackX_8 [8];\r\n  undefined1 auStackX_10 [8];\r\n  \r\n  FUN_140158290(param_1 + 0x341590,1,param_3,param_4,0xfffffffffffffffe);\r\n  func_0x0001408a2dc0(auStackX_10,param_1,1);\r\n  uVar1 = FUN_140657ae0(param_1 + 0x2e75d8,auStackX_8,param_2,param_3 & 0xffffffff);\r\n  FUN_1407b5d90(param_1 + 0x3130b0,param_2,uVar1);\r\n  uVar1 = D2_InternedName_Resolve(param_2 + 0x20);\r\n  FUN_14087b780(param_2,uVar1);\r\n  FUN_1408ab180(param_1,param_2);\r\n  func_0x0001408a2de0(auStackX_10);\r\n  func_0x0001401582e0(param_1 + 0x341590);\r\n  return;\r\n}\r\n\r\n"
    },
    {
      "address": "140783b90",
      "result": "\r\nlonglong FUN_140783b90(longlong param_1,uint param_2)\r\n\r\n{\r\n  FUN_14061a0a0(param_1 + 0x380);\r\n  FUN_140874eb0(param_1);\r\n  if ((param_2 & 1) != 0) {\r\n    FUN_1400f1520(param_1);\r\n  }\r\n  return param_1;\r\n}\r\n\r\n"
    },
    {
      "address": "140874eb0",
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140874eb0(undefined8 *param_1)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  longlong *plVar3;\r\n  longlong lVar4;\r\n  undefined8 *puVar5;\r\n  longlong *plVar6;\r\n  uint uVar7;\r\n  int iVar8;\r\n  longlong lVar9;\r\n  \r\n  *param_1 = &D2_VT_Entity;\r\n  FUN_140879660(_D2_GameSingletonPtr,param_1);\r\n  uVar2 = (**(code **)(*_D2_GameSingletonPtr + 0x388))();\r\n  FUN_1401a32b0(uVar2,*(undefined4 *)(param_1 + 0x15));\r\n  FUN_1407c0f30(_D2_GameSingletonPtr + 0x3edc9,param_1);\r\n  if ((*(byte *)((longlong)param_1 + 0xd2) & 1) == 0) {\r\n    FUN_1407eac80(_D2_GameSingletonPtr[0x3d12c],param_1);\r\n  }\r\n  uVar2 = param_1[0x48];\r\n  plVar3 = (longlong *)FUN_1401548e0();\r\n  (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141b38b60,0x239,uVar2,0x10);\r\n  param_1[0x48] = 0;\r\n  param_1[0x49] = 0;\r\n  if ((*(int *)(param_1 + 0x5e) != 0) &&\r\n     (*(undefined4 *)(param_1 + 0x5e) = 0, (*(byte *)((longlong)param_1 + 0x2f4) & 1) != 0)) {\r\n    *(int *)(_D2_GameSingletonPtr + 0xc04d) = (int)_D2_GameSingletonPtr[0xc04d] + 1;\r\n  }\r\n  FUN_1408ac370(_D2_GameSingletonPtr,param_1);\r\n  func_0x0001408ac400(_D2_GameSingletonPtr,param_1);\r\n  FUN_1408ac460(_D2_GameSingletonPtr,param_1);\r\n  FUN_1408af0a0(_D2_GameSingletonPtr,param_1,param_1[0x18]);\r\n  FUN_14064cc30(_D2_GameSingletonPtr + 0x61e63,param_1);\r\n  puVar5 = param_1 + 0x1e;\r\n  if (puVar5 != (undefined8 *)0x0) {\r\n    FUN_140872bb0(puVar5);\r\n  }\r\n  FUN_140874940(_D2_GameSingletonPtr + 0xc028,param_1);\r\n  if ((*(int *)(param_1 + 0x15) != 0x3fff0010) &&\r\n     (iVar1 = FUN_140877490(_D2_GameSingletonPtr + 0xc037,param_1), iVar1 != -1)) {\r\n    func_0x0001408ac700(_D2_GameSingletonPtr,iVar1);\r\n  }\r\n  FUN_140879530(_D2_GameSingletonPtr + 0xc03a,param_1);\r\n  if ((*(byte *)((longlong)param_1 + 0xd1) & 2) != 0) {\r\n    FUN_14080db70(_D2_GameSingletonPtr + 0x3ea5d,param_1);\r\n  }\r\n  lVar4 = (**(code **)(*_D2_GameSingletonPtr + 0x88))();\r\n  if ((*(byte *)(lVar4 + 8) & 0x20) != 0) {\r\n    FUN_14080d370(_D2_GameSingletonPtr + 0x3ea5d,param_1);\r\n  }\r\n  if (*(int *)(param_1 + 0x15) != 0x3fff0010) {\r\n    FUN_140810300(_D2_GameSingletonPtr + 0x3d12d,*(undefined4 *)(param_1 + 0x15));\r\n    FUN_1408af060(_D2_GameSingletonPtr,param_1);\r\n  }\r\n  FUN_14061a0a0(param_1 + 0x5a);\r\n  FUN_14061a0a0(param_1 + 0x58);\r\n  FUN_14061a0a0(param_1 + 0x4f);\r\n  FUN_14061a0a0(param_1 + 0x4d);\r\n  FUN_14061a0a0(param_1 + 0x4b);\r\n  uVar2 = param_1[0x48];\r\n  plVar3 = (longlong *)FUN_1401548e0();\r\n  (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141b38b60,0x239,uVar2,0x10);\r\n  param_1[0x48] = 0;\r\n  param_1[0x49] = 0;\r\n  FUN_1407b94b0(param_1 + 0x3c);\r\n  FUN_140872bb0(puVar5);\r\n  FUN_14061a0a0(param_1 + 0x38);\r\n  FUN_14061a0a0(param_1 + 0x34);\r\n  FUN_14061a0a0(param_1 + 0x30);\r\n  FUN_14061a0a0(param_1 + 0x2b);\r\n  FUN_14061a0a0(param_1 + 0x1c);\r\n  FUN_14061a0a0(param_1 + 0x16);\r\n  FUN_14013fd10(param_1 + 4);\r\n  *param_1 = &UNK_141c89d48;\r\n  if (_D2_GameSingletonPtr != (longlong *)0x0) {\r\n    FUN_140885080(_D2_GameSingletonPtr + 0x5ce82);\r\n  }\r\n  plVar3 = (longlong *)param_1[2];\r\n  if (plVar3 != (longlong *)0x0) {\r\n    iVar1 = (*(uint *)(plVar3 + 1) & 0xffffff) - 1;\r\n    lVar4 = (longlong)iVar1;\r\n    if (-1 < iVar1) {\r\n      do {\r\n        plVar6 = *(longlong **)(*(longlong *)(*plVar3 + lVar4 * 8) + 8);\r\n        if (plVar6 != (longlong *)0x0) {\r\n          uVar7 = *(uint *)(plVar6 + 1);\r\n          iVar1 = (uVar7 & 0xffffff) - 1;\r\n          iVar8 = 0;\r\n          lVar9 = 0;\r\n          if (-1 < iVar1) {\r\n            puVar5 = (undefined8 *)*plVar6;\r\n            do {\r\n              if ((undefined8 *)*puVar5 == param_1) {\r\n                if (((-1 < iVar8) && (iVar8 < (int)(uVar7 & 0xffffff))) &&\r\n                   (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(plVar6 + 1) = uVar7,\r\n                   iVar8 < (int)(uVar7 & 0xffffff))) {\r\n                  lVar9 = lVar9 * 8;\r\n                  do {\r\n                    *(undefined8 *)(*plVar6 + lVar9) = *(undefined8 *)(*plVar6 + 8 + lVar9);\r\n                    iVar8 = iVar8 + 1;\r\n                    lVar9 = lVar9 + 8;\r\n                  } while (iVar8 < (int)(*(uint *)(plVar6 + 1) & 0xffffff));\r\n                }\r\n                break;\r\n              }\r\n              iVar8 = iVar8 + 1;\r\n              lVar9 = lVar9 + 1;\r\n              puVar5 = puVar5 + 1;\r\n            } while (lVar9 <= iVar1);\r\n          }\r\n        }\r\n        lVar4 = lVar4 + -1;\r\n      } while (-1 < lVar4);\r\n    }\r\n    lVar4 = param_1[2];\r\n    if (lVar4 != 0) {\r\n      FUN_14061a0a0(lVar4);\r\n      FUN_1400f1520(lVar4);\r\n    }\r\n    param_1[2] = 0;\r\n  }\r\n  plVar3 = (longlong *)param_1[1];\r\n  if (plVar3 != (longlong *)0x0) {\r\n    iVar1 = (*(uint *)(plVar3 + 1) & 0xffffff) - 1;\r\n    lVar4 = (longlong)iVar1;\r\n    if (-1 < iVar1) {\r\n      do {\r\n        plVar6 = *(longlong **)(*(longlong *)(*plVar3 + lVar4 * 8) + 0x10);\r\n        if (plVar6 != (longlong *)0x0) {\r\n          uVar7 = *(uint *)(plVar6 + 1);\r\n          iVar1 = (uVar7 & 0xffffff) - 1;\r\n          iVar8 = 0;\r\n          lVar9 = 0;\r\n          if (-1 < iVar1) {\r\n            puVar5 = (undefined8 *)*plVar6;\r\n            do {\r\n              if ((undefined8 *)*puVar5 == param_1) {\r\n                if (((-1 < iVar8) && (iVar8 < (int)(uVar7 & 0xffffff))) &&\r\n                   (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(plVar6 + 1) = uVar7,\r\n                   iVar8 < (int)(uVar7 & 0xffffff))) {\r\n                  lVar9 = lVar9 * 8;\r\n                  do {\r\n                    *(undefined8 *)(*plVar6 + lVar9) = *(undefined8 *)(*plVar6 + 8 + lVar9);\r\n                    iVar8 = iVar8 + 1;\r\n                    lVar9 = lVar9 + 8;\r\n                  } while (iVar8 < (int)(*(uint *)(plVar6 + 1) & 0xffffff));\r\n                }\r\n                break;\r\n              }\r\n              iVar8 = iVar8 + 1;\r\n              lVar9 = lVar9 + 1;\r\n              puVar5 = puVar5 + 1;\r\n            } while (lVar9 <= iVar1);\r\n          }\r\n        }\r\n        lVar4 = lVar4 + -1;\r\n      } while (-1 < lVar4);\r\n    }\r\n    plVar3 = (longlong *)param_1[1];\r\n    if (plVar3 != (longlong *)0x0) {\r\n      if ((*(byte *)((longlong)plVar3 + 0xc) & 0x40) == 0) {\r\n        lVar4 = *plVar3;\r\n        if (lVar4 != 0) {\r\n          plVar6 = (longlong *)FUN_1401548e0();\r\n          (**(code **)(*plVar6 + 0x10))(plVar6,&UNK_141ab2230,0x1ec,lVar4,0x10);\r\n        }\r\n        *plVar3 = 0;\r\n        *(uint *)((longlong)plVar3 + 0xc) = *(uint *)((longlong)plVar3 + 0xc) & 0xff;\r\n      }\r\n      *(uint *)(plVar3 + 1) = *(uint *)(plVar3 + 1) & 0xff000000;\r\n      FUN_1400f1520(plVar3);\r\n    }\r\n    param_1[1] = 0;\r\n  }\r\n  *param_1 = &UNK_141afc930;\r\n  return;\r\n}\r\n\r\n"
    },
    {
      "address": "1408fa570",
      "result": "\r\nundefined8 FUN_1408fa570(longlong param_1,ulonglong param_2)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  \r\n  if (param_2 < 0x399) {\r\n    if (param_1 == 0) {\r\n      uVar1 = 0;\r\n    }\r\n    else {\r\n      uVar1 = FUN_140783b00();\r\n    }\r\n  }\r\n  else {\r\n    uVar1 = 0;\r\n  }\r\n  return uVar1;\r\n}\r\n\r\n"
    },
    {
      "address": "1407841b0",
      "result": "\r\nvoid FUN_1407841b0(longlong param_1)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  \r\n  uVar1 = FUN_140877720(param_1,0x1439e8010,0);\r\n  *(undefined8 *)(param_1 + 0x360) = uVar1;\r\n  *(undefined8 *)(param_1 + 0x378) = 0xbff0000000000000;\r\n  return;\r\n}\r\n\r\n"
    },
    {
      "address": "1400106a0",
      "result": "\r\nundefined8 * FUN_1400106a0(undefined8 *param_1)\r\n\r\n{\r\n  FUN_14005e740();\r\n  *param_1 = &UNK_141ab5248;\r\n  param_1[6] = 0;\r\n  param_1[7] = 0;\r\n  param_1[8] = 0;\r\n  return param_1;\r\n}\r\n\r\n"
    }
  ]
}
````

## Unescaped text fields for reading

### result

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

longlong FUN_140003cc0(ulonglong param_1,char *param_2,char param_3)

{
  code *pcVar1;
  ulonglong uVar2;
  char cVar3;
  int iVar4;
  char *pcVar5;
  longlong lVar6;
  ulonglong uVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  uint uVar10;
  ulonglong uVar11;
  ulonglong uVar12;
  undefined2 auStackX_10 [4];
  char cStackX_18;
  ulonglong uStack_f8;
  undefined1 auStack_f0 [40];
  undefined1 auStack_c8 [40];
  undefined1 auStack_a0 [8];
  undefined1 auStack_98 [8];
  undefined8 uStack_90;
  undefined1 auStack_88 [8];
  undefined1 auStack_80 [8];
  undefined1 uStack_78;
  char acStack_77 [63];
  
  if (param_2 == (char *)0x0) {
    return 0;
  }
  uStack_90 = 0xfffffffffffffffe;
  if ((param_2 == (char *)0x0) || (*param_2 == '\0')) {
    if (param_3 == '\0') {
      return 0;
    }
    param_2 = "_emptyName";
  }
  cStackX_18 = param_3;
  FUN_14013fb30(auStack_c8);
  cVar3 = func_0x000140143bd0(param_2);
  pcVar5 = param_2;
  if (cVar3 == '\0') {
    FUN_14013ff40(auStack_c8,param_2);
    FUN_140144060(auStack_c8,1);
    pcVar5 = (char *)D2_InternedName_Resolve(auStack_c8);
  }
  lVar6 = FUN_14005feb0(param_1,pcVar5);
  if (lVar6 != 0) {
LAB_14001e1ab:
    FUN_14013fd10(auStack_c8);
    return lVar6;
  }
  FUN_1401430a0(pcVar5,acStack_77,0x3f);
  iVar4 = func_0x000140141460(acStack_77,&UNK_141abb784);
  if ((((_DAT_1432c7ed4 == 0) && (param_3 == '\0')) && (iVar4 == 0)) &&
     (cVar3 = (**(code **)(*_DAT_14228baf0 + 0x98))(_DAT_14228baf0,pcVar5,0), cVar3 == '\0'))
  goto LAB_14001e471;
  uVar12 = param_1;
  if (iVar4 == 0) goto LAB_14001e492;
  uVar11 = 0;
  uStack_f8 = uVar11;
  if (((acStack_77[0] == '\0') ||
      (uStack_f8 = 0, (~(byte)(*(uint *)(param_1 + 0xec) >> 3) & 1) == 0)) ||
     (param_1 == 0x142434620)) {
LAB_14001e1cf:
    iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();
    uVar9 = uVar11;
    if (0 < iVar4) {
      do {
        uVar7 = (**(code **)(*_DAT_1422738a8 + 0x58))(_DAT_1422738a8,uVar9);
        uVar2 = uVar7;
        if (uVar7 != param_1) {
          for (; uVar2 != 0; uVar2 = *(ulonglong *)(uVar2 + 0xa0)) {
            if (uVar2 == param_1) {
              lVar6 = FUN_14005feb0(uVar7,pcVar5);
              if (lVar6 != 0) {
                FUN_14013fd10(auStack_c8);
                return lVar6;
              }
              break;
            }
          }
        }
        uVar10 = (int)uVar9 + 1;
        uVar9 = (ulonglong)uVar10;
        iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();
      } while ((int)uVar10 < iVar4);
    }
  }
  else {
    uStack_78 = 0x2e;
    uStack_f8 = (**(code **)(*_DAT_1422738a8 + 0x68))(_DAT_1422738a8,&uStack_78);
    if (uStack_f8 == 0) {
      func_0x000140166830(&UNK_141abba10,param_2,*(undefined8 *)(param_1 + 0x78));
      goto LAB_14001e1cf;
    }
    lVar6 = FUN_14005feb0(uStack_f8,pcVar5);
    if (lVar6 != 0) goto LAB_14001e1ab;
  }
  if (_DAT_1432c7ed4 < 2) {
    uVar9 = FUN_14001df10(param_1,pcVar5);
  }
  else {
    FUN_14013fb30(auStack_f0);
    FUN_14001e8c0(auStack_f0,param_1,pcVar5);
    uVar8 = D2_InternedName_Resolve(auStack_f0);
    auStackX_10[0] = 0x7fff;
    cVar3 = (**(code **)(*_DAT_142280e08 + 0x80))
                      (_DAT_142280e08,uVar8,auStack_80,auStack_a0,auStack_88,auStack_98,auStackX_10,
                       1);
    uVar9 = (ulonglong)(cVar3 != '\0');
    FUN_14013fd10(auStack_f0);
  }
  param_3 = cStackX_18;
  if (uVar9 != 0) goto LAB_14001e492;
  if (uStack_f8 == 0) {
    iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();
    uVar12 = param_1;
    if (0 < iVar4) {
      uVar9 = 0;
      do {
        uStack_f8 = (**(code **)(*_DAT_1422738a8 + 0x58))(_DAT_1422738a8,uVar11);
        uVar12 = uStack_f8;
        if (uStack_f8 != param_1) {
          for (; uVar12 != 0; uVar12 = *(ulonglong *)(uVar12 + 0xa0)) {
            if (uVar12 == param_1) {
              if (_DAT_1432c7ed4 < 2) {
                uVar9 = FUN_14001df10(uStack_f8,pcVar5);
                if (uVar9 != 0) goto LAB_14001e462;
              }
              else {
                FUN_14013fb30(auStack_f0);
                FUN_14001e8c0(auStack_f0,uStack_f8,pcVar5);
                uVar8 = D2_InternedName_Resolve(auStack_f0);
                cVar3 = FUN_14001dec0(uVar8);
                if (cVar3 != '\0') {
                  FUN_14013fd10(auStack_f0);
                  uVar12 = uStack_f8;
                  param_3 = cStackX_18;
                  goto LAB_14001e492;
                }
                FUN_14013fd10(auStack_f0);
              }
              break;
            }
          }
        }
        uVar10 = (int)uVar11 + 1;
        uVar11 = (ulonglong)uVar10;
        iVar4 = (**(code **)(*_DAT_1422738a8 + 0x48))();
        uStack_f8 = param_1;
      } while ((int)uVar10 < iVar4);
      goto LAB_14001e462;
    }
  }
  else {
    if (_DAT_1432c7ed4 < 2) {
      uVar9 = FUN_14001df10(uStack_f8,pcVar5);
      uVar12 = param_1;
      if (uVar9 == 0) goto LAB_14001e467;
    }
    else {
      FUN_14013fb30(auStack_f0);
      FUN_14001e8c0(auStack_f0,uStack_f8,pcVar5);
      uVar8 = D2_InternedName_Resolve(auStack_f0);
      cVar3 = FUN_14001dec0(uVar8);
      uVar12 = param_1;
      if (cVar3 != '\0') {
        uVar12 = uStack_f8;
      }
      uStack_f8 = uVar12;
      uVar9 = (ulonglong)(cVar3 != '\0');
      FUN_14013fd10(auStack_f0);
    }
LAB_14001e462:
    uVar12 = uStack_f8;
    param_3 = cStackX_18;
    if (uVar9 != 0) goto LAB_14001e492;
  }
LAB_14001e467:
  uStack_f8 = uVar12;
  uVar12 = uStack_f8;
  param_3 = cStackX_18;
  if (cStackX_18 == '\0') {
LAB_14001e471:
    FUN_14013fd10(auStack_c8);
    return 0;
  }
LAB_14001e492:
  cVar3 = (**(code **)(*_DAT_142274420 + 0x118))();
  if ((cVar3 != '\0') && (cVar3 = (**(code **)(*_DAT_142274420 + 0x138))(), cVar3 == '\0')) {
    if (_DAT_142be6cf8 == 2) {
      uVar8 = FUN_140060610(param_1,1);
      func_0x000140166830(&UNK_141abba88,uVar8,param_2);
      pcVar1 = (code *)swi(3);
      lVar6 = (*pcVar1)();
      return lVar6;
    }
    if (_DAT_142be6cf8 == 1) {
      uVar8 = FUN_140060610(param_1,1);
      func_0x000140166830(&UNK_141abba88,uVar8,param_2);
    }
  }
  lVar6 = FUN_1400607f0(uVar12,pcVar5,param_3);
  FUN_14013fd10(auStack_c8);
  return lVar6;
}


````

### result

````text

undefined8 FUN_14001ea30(longlong param_1,char *param_2)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  
  if ((param_2 != (char *)0x0) && (*param_2 != '\0')) {
    iVar2 = 0;
    if ((*(uint *)(param_1 + 0x10) & 0xffffff) != 0) {
      lVar3 = 0;
      do {
        iVar1 = func_0x000140143650(*(undefined8 *)
                                     (*(longlong *)(*(longlong *)(param_1 + 8) + lVar3) + 0x78),
                                    param_2);
        if (iVar1 == 0) {
          return *(undefined8 *)(*(longlong *)(param_1 + 8) + (longlong)iVar2 * 8);
        }
        iVar2 = iVar2 + 1;
        lVar3 = lVar3 + 8;
      } while (iVar2 < (int)(*(uint *)(param_1 + 0x10) & 0xffffff));
    }
    return 0;
  }
  return 0;
}


````

### result

````text

/* WARNING: Possible PIC construction at 0x000140021a5b: Changing call to branch */
/* WARNING: Possible PIC construction at 0x000140021a96: Changing call to branch */
/* WARNING: Possible PIC construction at 0x000140021b07: Changing call to branch */
/* WARNING: Possible PIC construction at 0x000140021b85: Changing call to branch */
/* WARNING: Possible PIC construction at 0x0001400121c4: Changing call to branch */
/* WARNING: Removing unreachable block (ram,0x000140021b8a) */
/* WARNING: Removing unreachable block (ram,0x000140012160) */
/* WARNING: Removing unreachable block (ram,0x0001400121a9) */
/* WARNING: Removing unreachable block (ram,0x000140021b0c) */
/* WARNING: Removing unreachable block (ram,0x000140021a9b) */
/* WARNING: Removing unreachable block (ram,0x000140021a60) */
/* WARNING: Removing unreachable block (ram,0x000140021ab0) */
/* WARNING: Removing unreachable block (ram,0x000140021a7a) */
/* WARNING: Removing unreachable block (ram,0x000140021a80) */
/* WARNING: Removing unreachable block (ram,0x0001400121c9) */
/* WARNING: Removing unreachable block (ram,0x0001400121d2) */

void FUN_140021a40(int *param_1,undefined *param_2,undefined *param_3,undefined8 param_4)

{
  longlong lVar1;
  longlong lVar2;
  undefined *puVar3;
  int iVar4;
  longlong lVar5;
  undefined8 auStack_30 [5];
  
  if (*param_1 < 2) {
    register0x00000020 = (BADSPACEBASE *)auStack_30;
  }
  else {
    param_2 = &UNK_141ab2848;
    if (*param_1 < 2) {
      puVar3 = &UNK_141ab2848;
    }
    else {
      puVar3 = *(undefined **)(param_1 + 4);
    }
    auStack_30[0] = 0x140021aec;
    lVar1 = FUN_14001ea30(0x1424367e0,puVar3);
    if (lVar1 == 0) {
      if (1 < *param_1) {
        param_2 = *(undefined **)(param_1 + 4);
      }
      register0x00000020 = (BADSPACEBASE *)auStack_30;
    }
    else {
      if (*param_1 != 3) {
        iVar4 = 0;
        if ((*(uint *)(lVar1 + 0xb8) & 0xffffff) != 0) {
          lVar5 = 0;
          do {
            auStack_30[0] = 0x140021bda;
            lVar2 = FUN_140003cc0(lVar1,*(undefined8 *)
                                         (*(longlong *)(lVar5 + *(longlong *)(lVar1 + 0xb0)) + 8),0)
            ;
            if (lVar2 != 0) {
              auStack_30[0] = 0x140021be9;
              FUN_140012160(lVar2,1);
            }
            iVar4 = iVar4 + 1;
            lVar5 = lVar5 + 8;
          } while (iVar4 < (int)(*(uint *)(lVar1 + 0xb8) & 0xffffff));
        }
        return;
      }
      param_3 = (undefined *)0x0;
      auStack_30[0] = 0x140021b3b;
      lVar1 = FUN_140003cc0(lVar1,*(undefined8 *)(param_1 + 6));
      if (lVar1 == 0) {
        if (*param_1 < 3) {
          param_3 = &UNK_141ab2848;
        }
        else {
          param_3 = *(undefined **)(param_1 + 6);
        }
        if (1 < *param_1) {
          param_2 = *(undefined **)(param_1 + 4);
        }
      }
      else {
        param_2 = *(undefined **)(lVar1 + 8);
        register0x00000020 = (BADSPACEBASE *)auStack_30;
      }
    }
  }
  *(undefined **)((longlong)register0x00000020 + 0x10) = param_2;
  *(undefined **)((longlong)register0x00000020 + 0x18) = param_3;
  *(undefined8 *)((longlong)register0x00000020 + 0x20) = param_4;
  return;
}


````

### result

````text

void FUN_1408a9430(undefined8 param_1,longlong *param_2,longlong param_3,undefined4 *param_4)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  undefined1 auStackX_18 [8];
  undefined4 uStack_1f8;
  undefined4 uStack_1f4;
  undefined4 uStack_1f0;
  
  func_0x0001408a2dc0(auStackX_18,param_1,1);
  if (param_3 != 0) {
    (**(code **)(*param_2 + 0x38))(param_2,param_3);
    FUN_1401464d0(&uStack_1f8,0);
    uVar1 = FUN_14006d040(param_3);
    uVar3 = func_0x000140011780(param_3);
    uVar2 = func_0x000140014520(param_3);
    uVar4 = func_0x00014015bd10(param_3);
    FUN_140149130(&uStack_1f8,uVar4,uVar2,uVar3,uVar1);
    uStack_1f8 = *param_4;
    uStack_1f4 = param_4[1];
    uStack_1f0 = param_4[2];
    FUN_14098df10(param_2,&uStack_1f8,1);
    FUN_140146580(&uStack_1f8);
  }
  func_0x0001408a2de0(auStackX_18);
  return;
}


````

### result

````text

void FUN_1408aecc0(longlong param_1,longlong param_2,ulonglong param_3,undefined8 param_4)

{
  undefined8 uVar1;
  undefined1 auStackX_8 [8];
  undefined1 auStackX_10 [8];
  
  FUN_140158290(param_1 + 0x341590,1,param_3,param_4,0xfffffffffffffffe);
  func_0x0001408a2dc0(auStackX_10,param_1,1);
  uVar1 = FUN_140657ae0(param_1 + 0x2e75d8,auStackX_8,param_2,param_3 & 0xffffffff);
  FUN_1407b5d90(param_1 + 0x3130b0,param_2,uVar1);
  uVar1 = D2_InternedName_Resolve(param_2 + 0x20);
  FUN_14087b780(param_2,uVar1);
  FUN_1408ab180(param_1,param_2);
  func_0x0001408a2de0(auStackX_10);
  func_0x0001401582e0(param_1 + 0x341590);
  return;
}


````

### result

````text

longlong FUN_140783b90(longlong param_1,uint param_2)

{
  FUN_14061a0a0(param_1 + 0x380);
  FUN_140874eb0(param_1);
  if ((param_2 & 1) != 0) {
    FUN_1400f1520(param_1);
  }
  return param_1;
}


````

### result

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140874eb0(undefined8 *param_1)

{
  int iVar1;
  undefined8 uVar2;
  longlong *plVar3;
  longlong lVar4;
  undefined8 *puVar5;
  longlong *plVar6;
  uint uVar7;
  int iVar8;
  longlong lVar9;
  
  *param_1 = &D2_VT_Entity;
  FUN_140879660(_D2_GameSingletonPtr,param_1);
  uVar2 = (**(code **)(*_D2_GameSingletonPtr + 0x388))();
  FUN_1401a32b0(uVar2,*(undefined4 *)(param_1 + 0x15));
  FUN_1407c0f30(_D2_GameSingletonPtr + 0x3edc9,param_1);
  if ((*(byte *)((longlong)param_1 + 0xd2) & 1) == 0) {
    FUN_1407eac80(_D2_GameSingletonPtr[0x3d12c],param_1);
  }
  uVar2 = param_1[0x48];
  plVar3 = (longlong *)FUN_1401548e0();
  (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141b38b60,0x239,uVar2,0x10);
  param_1[0x48] = 0;
  param_1[0x49] = 0;
  if ((*(int *)(param_1 + 0x5e) != 0) &&
     (*(undefined4 *)(param_1 + 0x5e) = 0, (*(byte *)((longlong)param_1 + 0x2f4) & 1) != 0)) {
    *(int *)(_D2_GameSingletonPtr + 0xc04d) = (int)_D2_GameSingletonPtr[0xc04d] + 1;
  }
  FUN_1408ac370(_D2_GameSingletonPtr,param_1);
  func_0x0001408ac400(_D2_GameSingletonPtr,param_1);
  FUN_1408ac460(_D2_GameSingletonPtr,param_1);
  FUN_1408af0a0(_D2_GameSingletonPtr,param_1,param_1[0x18]);
  FUN_14064cc30(_D2_GameSingletonPtr + 0x61e63,param_1);
  puVar5 = param_1 + 0x1e;
  if (puVar5 != (undefined8 *)0x0) {
    FUN_140872bb0(puVar5);
  }
  FUN_140874940(_D2_GameSingletonPtr + 0xc028,param_1);
  if ((*(int *)(param_1 + 0x15) != 0x3fff0010) &&
     (iVar1 = FUN_140877490(_D2_GameSingletonPtr + 0xc037,param_1), iVar1 != -1)) {
    func_0x0001408ac700(_D2_GameSingletonPtr,iVar1);
  }
  FUN_140879530(_D2_GameSingletonPtr + 0xc03a,param_1);
  if ((*(byte *)((longlong)param_1 + 0xd1) & 2) != 0) {
    FUN_14080db70(_D2_GameSingletonPtr + 0x3ea5d,param_1);
  }
  lVar4 = (**(code **)(*_D2_GameSingletonPtr + 0x88))();
  if ((*(byte *)(lVar4 + 8) & 0x20) != 0) {
    FUN_14080d370(_D2_GameSingletonPtr + 0x3ea5d,param_1);
  }
  if (*(int *)(param_1 + 0x15) != 0x3fff0010) {
    FUN_140810300(_D2_GameSingletonPtr + 0x3d12d,*(undefined4 *)(param_1 + 0x15));
    FUN_1408af060(_D2_GameSingletonPtr,param_1);
  }
  FUN_14061a0a0(param_1 + 0x5a);
  FUN_14061a0a0(param_1 + 0x58);
  FUN_14061a0a0(param_1 + 0x4f);
  FUN_14061a0a0(param_1 + 0x4d);
  FUN_14061a0a0(param_1 + 0x4b);
  uVar2 = param_1[0x48];
  plVar3 = (longlong *)FUN_1401548e0();
  (**(code **)(*plVar3 + 0x10))(plVar3,&UNK_141b38b60,0x239,uVar2,0x10);
  param_1[0x48] = 0;
  param_1[0x49] = 0;
  FUN_1407b94b0(param_1 + 0x3c);
  FUN_140872bb0(puVar5);
  FUN_14061a0a0(param_1 + 0x38);
  FUN_14061a0a0(param_1 + 0x34);
  FUN_14061a0a0(param_1 + 0x30);
  FUN_14061a0a0(param_1 + 0x2b);
  FUN_14061a0a0(param_1 + 0x1c);
  FUN_14061a0a0(param_1 + 0x16);
  FUN_14013fd10(param_1 + 4);
  *param_1 = &UNK_141c89d48;
  if (_D2_GameSingletonPtr != (longlong *)0x0) {
    FUN_140885080(_D2_GameSingletonPtr + 0x5ce82);
  }
  plVar3 = (longlong *)param_1[2];
  if (plVar3 != (longlong *)0x0) {
    iVar1 = (*(uint *)(plVar3 + 1) & 0xffffff) - 1;
    lVar4 = (longlong)iVar1;
    if (-1 < iVar1) {
      do {
        plVar6 = *(longlong **)(*(longlong *)(*plVar3 + lVar4 * 8) + 8);
        if (plVar6 != (longlong *)0x0) {
          uVar7 = *(uint *)(plVar6 + 1);
          iVar1 = (uVar7 & 0xffffff) - 1;
          iVar8 = 0;
          lVar9 = 0;
          if (-1 < iVar1) {
            puVar5 = (undefined8 *)*plVar6;
            do {
              if ((undefined8 *)*puVar5 == param_1) {
                if (((-1 < iVar8) && (iVar8 < (int)(uVar7 & 0xffffff))) &&
                   (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(plVar6 + 1) = uVar7,
                   iVar8 < (int)(uVar7 & 0xffffff))) {
                  lVar9 = lVar9 * 8;
                  do {
                    *(undefined8 *)(*plVar6 + lVar9) = *(undefined8 *)(*plVar6 + 8 + lVar9);
                    iVar8 = iVar8 + 1;
                    lVar9 = lVar9 + 8;
                  } while (iVar8 < (int)(*(uint *)(plVar6 + 1) & 0xffffff));
                }
                break;
              }
              iVar8 = iVar8 + 1;
              lVar9 = lVar9 + 1;
              puVar5 = puVar5 + 1;
            } while (lVar9 <= iVar1);
          }
        }
        lVar4 = lVar4 + -1;
      } while (-1 < lVar4);
    }
    lVar4 = param_1[2];
    if (lVar4 != 0) {
      FUN_14061a0a0(lVar4);
      FUN_1400f1520(lVar4);
    }
    param_1[2] = 0;
  }
  plVar3 = (longlong *)param_1[1];
  if (plVar3 != (longlong *)0x0) {
    iVar1 = (*(uint *)(plVar3 + 1) & 0xffffff) - 1;
    lVar4 = (longlong)iVar1;
    if (-1 < iVar1) {
      do {
        plVar6 = *(longlong **)(*(longlong *)(*plVar3 + lVar4 * 8) + 0x10);
        if (plVar6 != (longlong *)0x0) {
          uVar7 = *(uint *)(plVar6 + 1);
          iVar1 = (uVar7 & 0xffffff) - 1;
          iVar8 = 0;
          lVar9 = 0;
          if (-1 < iVar1) {
            puVar5 = (undefined8 *)*plVar6;
            do {
              if ((undefined8 *)*puVar5 == param_1) {
                if (((-1 < iVar8) && (iVar8 < (int)(uVar7 & 0xffffff))) &&
                   (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(plVar6 + 1) = uVar7,
                   iVar8 < (int)(uVar7 & 0xffffff))) {
                  lVar9 = lVar9 * 8;
                  do {
                    *(undefined8 *)(*plVar6 + lVar9) = *(undefined8 *)(*plVar6 + 8 + lVar9);
                    iVar8 = iVar8 + 1;
                    lVar9 = lVar9 + 8;
                  } while (iVar8 < (int)(*(uint *)(plVar6 + 1) & 0xffffff));
                }
                break;
              }
              iVar8 = iVar8 + 1;
              lVar9 = lVar9 + 1;
              puVar5 = puVar5 + 1;
            } while (lVar9 <= iVar1);
          }
        }
        lVar4 = lVar4 + -1;
      } while (-1 < lVar4);
    }
    plVar3 = (longlong *)param_1[1];
    if (plVar3 != (longlong *)0x0) {
      if ((*(byte *)((longlong)plVar3 + 0xc) & 0x40) == 0) {
        lVar4 = *plVar3;
        if (lVar4 != 0) {
          plVar6 = (longlong *)FUN_1401548e0();
          (**(code **)(*plVar6 + 0x10))(plVar6,&UNK_141ab2230,0x1ec,lVar4,0x10);
        }
        *plVar3 = 0;
        *(uint *)((longlong)plVar3 + 0xc) = *(uint *)((longlong)plVar3 + 0xc) & 0xff;
      }
      *(uint *)(plVar3 + 1) = *(uint *)(plVar3 + 1) & 0xff000000;
      FUN_1400f1520(plVar3);
    }
    param_1[1] = 0;
  }
  *param_1 = &UNK_141afc930;
  return;
}


````

### result

````text

undefined8 FUN_1408fa570(longlong param_1,ulonglong param_2)

{
  undefined8 uVar1;
  
  if (param_2 < 0x399) {
    if (param_1 == 0) {
      uVar1 = 0;
    }
    else {
      uVar1 = FUN_140783b00();
    }
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


````

### result

````text

void FUN_1407841b0(longlong param_1)

{
  undefined8 uVar1;
  
  uVar1 = FUN_140877720(param_1,0x1439e8010,0);
  *(undefined8 *)(param_1 + 0x360) = uVar1;
  *(undefined8 *)(param_1 + 0x378) = 0xbff0000000000000;
  return;
}


````

### result

````text

undefined8 * FUN_1400106a0(undefined8 *param_1)

{
  FUN_14005e740();
  *param_1 = &UNK_141ab5248;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  return param_1;
}


````
