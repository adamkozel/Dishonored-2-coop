# ghidra-pickup-families-v803.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-pickup-families-v803.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-pickup-families-v803.json>)

SHA-256: `91B392438B4442488BAF9208BF6B43092E9459144C384475A4D92568B252CD60`; 23075 bytes.

## Original saved record

````json
{
  "140841030": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140841030(longlong param_1,undefined8 *param_2,undefined1 param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  bool bVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  char cVar5;\r\n  char cVar6;\r\n  undefined4 uVar7;\r\n  int iVar8;\r\n  longlong lVar9;\r\n  undefined8 uVar10;\r\n  longlong lVar11;\r\n  undefined8 uVar12;\r\n  longlong lVar13;\r\n  int iVar14;\r\n  int iVar15;\r\n  undefined4 local_res8 [2];\r\n  undefined4 local_b8 [2];\r\n  undefined1 local_b0 [40];\r\n  undefined4 local_88;\r\n  undefined8 local_80;\r\n  undefined4 local_78 [2];\r\n  undefined1 local_70 [40];\r\n  undefined4 local_48;\r\n  \r\n  local_80 = 0xfffffffffffffffe;\r\n  uVar10 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x328);\r\n  lVar11 = param_2[1];\r\n  lVar1 = *(longlong *)(lVar11 + 0xf8);\r\n  lVar9 = (**(code **)*param_2)(param_2);\r\n  if ((DAT_1439aeaf0 <= *(int *)(lVar9 + 0x60)) && (*(int *)(lVar9 + 0x60) <= DAT_1439aeaf4)) {\r\n    FUN_1408404c0(param_2,local_b8);\r\n    cVar5 = FUN_14060b3e0(local_b8);\r\n    if (cVar5 == '\\0') {\r\n      uVar10 = FUN_140854880(param_1);\r\n      lVar11 = FUN_140877720(uVar10,&DAT_1439c55b0,2);\r\n      if (lVar11 != 0) {\r\n        local_78[0] = local_b8[0];\r\n        FUN_14013f490(local_70,local_b0);\r\n        local_48 = local_88;\r\n        FUN_14060e4c0(lVar11,local_78);\r\n        uVar7 = FUN_1410701c0(local_b8);\r\n        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),\r\n           lVar11 != 0)) {\r\n          local_res8[0] = uVar7;\r\n          FUN_1406734b0(lVar11,lVar1,1,local_res8);\r\n        }\r\n        FUN_14067e200(D2_GameSingletonPtr + 0x1f77c8,lVar1,1,param_3,param_2[2]);\r\n        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200), lVar11 != 0\r\n           )) {\r\n          FUN_14045d380(lVar11,1,lVar1 + 100);\r\n        }\r\n      }\r\n    }\r\n    FUN_14013fd10(local_b0);\r\n    return;\r\n  }\r\n  cVar5 = FUN_140615d40(uVar10,lVar1);\r\n  lVar9 = FUN_1403da910(param_1 + 0x120,2);\r\n  if ((lVar9 == 0) || (cVar6 = FUN_1404c1f20(), cVar6 == '\\0')) {\r\n    bVar2 = false;\r\n  }\r\n  else {\r\n    bVar2 = true;\r\n  }\r\n  if (*(int *)(lVar1 + 0x60) - 7U < 2) {\r\n    bVar3 = true;\r\n    if (bVar2) goto LAB_14084120c;\r\n    bVar4 = true;\r\n  }\r\n  else {\r\n    bVar3 = false;\r\nLAB_14084120c:\r\n    bVar4 = false;\r\n    if ((bVar3) && (bVar2)) {\r\n      bVar2 = true;\r\n      goto LAB_14084121d;\r\n    }\r\n  }\r\n  bVar2 = false;\r\nLAB_14084121d:\r\n  if (cVar5 == '\\0') {\r\n    if (bVar2) {\r\n      iVar14 = *(int *)(lVar1 + 0xec);\r\n    }\r\n    else {\r\n      iVar14 = *(int *)(lVar11 + 0x100);\r\n    }\r\n    if (bVar4) {\r\n      FUN_140615620(uVar10,iVar14);\r\n      iVar8 = iVar14;\r\n    }\r\n    else {\r\n      lVar9 = lVar1;\r\n      if (bVar3) {\r\n        uVar12 = FUN_140854880(param_1);\r\n        lVar13 = FUN_140877720(uVar12,&DAT_1439e2bb0,2);\r\n        if (lVar13 != 0) {\r\n          lVar9 = FUN_14007b840(lVar13);\r\n        }\r\n      }\r\n      iVar8 = FUN_1406156e0(uVar10,lVar9,iVar14);\r\n    }\r\n    iVar15 = iVar8;\r\n    if (*(int *)(lVar1 + 0x60) == 0x10) {\r\n      iVar15 = 1;\r\n    }\r\n    FUN_14067e200(D2_GameSingletonPtr + 0x1f77c8,lVar1,iVar15,param_3,param_2[2]);\r\n    iVar15 = *(int *)(lVar1 + 0x60);\r\n    if (iVar15 != 6) {\r\n      if (((iVar15 == 1) || (iVar15 == 0xf)) || (bVar4)) {\r\n        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),\r\n           lVar11 != 0)) {\r\n          FUN_1406733c0(lVar11,iVar14);\r\n        }\r\n      }\r\n      else {\r\n        if (iVar15 == 0x10) {\r\n          if (1 < iVar8) {\r\n            if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n               (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),\r\n               lVar11 != 0)) {\r\n              FUN_1406733c0(lVar11,iVar8);\r\n            }\r\n            uVar10 = FUN_140854880(param_1);\r\n            lVar11 = FUN_140877720(uVar10,&DAT_1439ba310,2);\r\n            if (lVar11 == 0) {\r\n              return;\r\n            }\r\n            if (*(char *)(lVar11 + 0x6c) == '\\0') {\r\n              return;\r\n            }\r\n            *(int *)(lVar11 + 0x68) = *(int *)(lVar11 + 0x68) + iVar8;\r\n            return;\r\n          }\r\n          local_res8[0] = *(undefined4 *)(lVar11 + 200);\r\n        }\r\n        else {\r\n          local_res8[0] = 0;\r\n        }\r\n        FUN_140840fb0(param_1,lVar1,iVar8,local_res8);\r\n        FUN_140840e60(param_1,lVar1,iVar8);\r\n        FUN_140840ff0(param_1,lVar1 + 0xd0);\r\n        if (*(int *)(lVar1 + 0x60) - 7U < 2) {\r\n          if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n             (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200),\r\n             lVar11 != 0)) {\r\n            FUN_14045d380(lVar11,3,lVar1 + 100);\r\n          }\r\n        }\r\n        else if (((*(int *)(lVar1 + 0x60) == 0xb) &&\r\n                 (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0)) &&\r\n                (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200),\r\n                lVar11 != 0)) {\r\n          FUN_14045d380(lVar11,6,lVar1 + 100);\r\n        }\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140841030(longlong param_1,undefined8 *param_2,undefined1 param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  bool bVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  char cVar5;\r\n  char cVar6;\r\n  undefined4 uVar7;\r\n  int iVar8;\r\n  longlong lVar9;\r\n  undefined8 uVar10;\r\n  longlong lVar11;\r\n  undefined8 uVar12;\r\n  longlong lVar13;\r\n  int iVar14;\r\n  int iVar15;\r\n  undefined4 local_res8 [2];\r\n  undefined4 local_b8 [2];\r\n  undefined1 local_b0 [40];\r\n  undefined4 local_88;\r\n  undefined8 local_80;\r\n  undefined4 local_78 [2];\r\n  undefined1 local_70 [40];\r\n  undefined4 local_48;\r\n  \r\n  local_80 = 0xfffffffffffffffe;\r\n  uVar10 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x328);\r\n  lVar11 = param_2[1];\r\n  lVar1 = *(longlong *)(lVar11 + 0xf8);\r\n  lVar9 = (**(code **)*param_2)(param_2);\r\n  if ((DAT_1439aeaf0 <= *(int *)(lVar9 + 0x60)) && (*(int *)(lVar9 + 0x60) <= DAT_1439aeaf4)) {\r\n    FUN_1408404c0(param_2,local_b8);\r\n    cVar5 = FUN_14060b3e0(local_b8);\r\n    if (cVar5 == '\\0') {\r\n      uVar10 = FUN_140854880(param_1);\r\n      lVar11 = FUN_140877720(uVar10,&DAT_1439c55b0,2);\r\n      if (lVar11 != 0) {\r\n        local_78[0] = local_b8[0];\r\n        FUN_14013f490(local_70,local_b0);\r\n        local_48 = local_88;\r\n        FUN_14060e4c0(lVar11,local_78);\r\n        uVar7 = FUN_1410701c0(local_b8);\r\n        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),\r\n           lVar11 != 0)) {\r\n          local_res8[0] = uVar7;\r\n          FUN_1406734b0(lVar11,lVar1,1,local_res8);\r\n        }\r\n        FUN_14067e200(D2_GameSingletonPtr + 0x1f77c8,lVar1,1,param_3,param_2[2]);\r\n        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200), lVar11 != 0\r\n           )) {\r\n          FUN_14045d380(lVar11,1,lVar1 + 100);\r\n        }\r\n      }\r\n    }\r\n    FUN_14013fd10(local_b0);\r\n    return;\r\n  }\r\n  cVar5 = FUN_140615d40(uVar10,lVar1);\r\n  lVar9 = FUN_1403da910(param_1 + 0x120,2);\r\n  if ((lVar9 == 0) || (cVar6 = FUN_1404c1f20(), cVar6 == '\\0')) {\r\n    bVar2 = false;\r\n  }\r\n  else {\r\n    bVar2 = true;\r\n  }\r\n  if (*(int *)(lVar1 + 0x60) - 7U < 2) {\r\n    bVar3 = true;\r\n    if (bVar2) goto LAB_14084120c;\r\n    bVar4 = true;\r\n  }\r\n  else {\r\n    bVar3 = false;\r\nLAB_14084120c:\r\n    bVar4 = false;\r\n    if ((bVar3) && (bVar2)) {\r\n      bVar2 = true;\r\n      goto LAB_14084121d;\r\n    }\r\n  }\r\n  bVar2 = false;\r\nLAB_14084121d:\r\n  if (cVar5 == '\\0') {\r\n    if (bVar2) {\r\n      iVar14 = *(int *)(lVar1 + 0xec);\r\n    }\r\n    else {\r\n      iVar14 = *(int *)(lVar11 + 0x100);\r\n    }\r\n    if (bVar4) {\r\n      FUN_140615620(uVar10,iVar14);\r\n      iVar8 = iVar14;\r\n    }\r\n    else {\r\n      lVar9 = lVar1;\r\n      if (bVar3) {\r\n        uVar12 = FUN_140854880(param_1);\r\n        lVar13 = FUN_140877720(uVar12,&DAT_1439e2bb0,2);\r\n        if (lVar13 != 0) {\r\n          lVar9 = FUN_14007b840(lVar13);\r\n        }\r\n      }\r\n      iVar8 = FUN_1406156e0(uVar10,lVar9,iVar14);\r\n    }\r\n    iVar15 = iVar8;\r\n    if (*(int *)(lVar1 + 0x60) == 0x10) {\r\n      iVar15 = 1;\r\n    }\r\n    FUN_14067e200(D2_GameSingletonPtr + 0x1f77c8,lVar1,iVar15,param_3,param_2[2]);\r\n    iVar15 = *(int *)(lVar1 + 0x60);\r\n    if (iVar15 != 6) {\r\n      if (((iVar15 == 1) || (iVar15 == 0xf)) || (bVar4)) {\r\n        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),\r\n           lVar11 != 0)) {\r\n          FUN_1406733c0(lVar11,iVar14);\r\n        }\r\n      }\r\n      else {\r\n        if (iVar15 == 0x10) {\r\n          if (1 < iVar8) {\r\n            if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n               (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),\r\n               lVar11 != 0)) {\r\n              FUN_1406733c0(lVar11,iVar8);\r\n            }\r\n            uVar10 = FUN_140854880(param_1);\r\n            lVar11 = FUN_140877720(uVar10,&DAT_1439ba310,2);\r\n            if (lVar11 == 0) {\r\n              return;\r\n            }\r\n            if (*(char *)(lVar11 + 0x6c) == '\\0') {\r\n              return;\r\n            }\r\n            *(int *)(lVar11 + 0x68) = *(int *)(lVar11 + 0x68) + iVar8;\r\n            return;\r\n          }\r\n          local_res8[0] = *(undefined4 *)(lVar11 + 200);\r\n        }\r\n        else {\r\n          local_res8[0] = 0;\r\n        }\r\n        FUN_140840fb0(param_1,lVar1,iVar8,local_res8);\r\n        FUN_140840e60(param_1,lVar1,iVar8);\r\n        FUN_140840ff0(param_1,lVar1 + 0xd0);\r\n        if (*(int *)(lVar1 + 0x60) - 7U < 2) {\r\n          if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&\r\n             (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200),\r\n             lVar11 != 0)) {\r\n            FUN_14045d380(lVar11,3,lVar1 + 100);\r\n          }\r\n        }\r\n        else if (((*(int *)(lVar1 + 0x60) == 0xb) &&\r\n                 (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0)) &&\r\n                (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200),\r\n                lVar11 != 0)) {\r\n          FUN_14045d380(lVar11,6,lVar1 + 100);\r\n        }\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "1406156e0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nint FUN_1406156e0(longlong param_1,longlong param_2,int param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined8 *puVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  int iVar5;\r\n  longlong lVar6;\r\n  longlong lVar7;\r\n  longlong *plVar8;\r\n  longlong *plVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  undefined1 auStack_78 [8];\r\n  undefined8 uStack_70;\r\n  undefined8 auStack_60 [5];\r\n  undefined1 local_38 [8];\r\n  int local_30;\r\n  undefined4 uStack_2c;\r\n  \r\n  iVar5 = *(int *)(param_2 + 0x60);\r\n  if ((iVar5 == 1) || (iVar5 == 0xf)) {\r\n    *(int *)(param_1 + 0x16c) = *(int *)(param_1 + 0x16c) + param_3;\r\n    local_38[0] = 0x69;\r\n    _local_30 = CONCAT44(uStack_2c,param_3);\r\n    auStack_60[0] = 0x1406157fc;\r\n    FUN_140884920(*(undefined8 *)(param_1 + 0x10),&stack0xffffffffffffffe0,&DAT_1439ab8f0,local_38);\r\n    auStack_60[0] = 0x140615806;\r\n    FUN_140882fc0(&stack0xffffffffffffffe0);\r\n    auStack_60[0] = 0x140615823;\r\n    FUN_14067aea0(D2_GameSingletonPtr + 0x1f77c8,1,param_3);\r\n    return param_3;\r\n  }\r\n  if (iVar5 == 0x10) {\r\n    auStack_60[0] = 0x140615717;\r\n    iVar5 = FUN_140615310();\r\n    if (iVar5 != 0) {\r\n      auStack_60[0] = 0x140615731;\r\n      lVar6 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439ba310,2);\r\n      iVar5 = *(int *)(*(longlong *)(lVar6 + 8) + 0x68);\r\n      auStack_60[0] = 0x140615742;\r\n      FUN_140615620(param_1,iVar5);\r\n      return iVar5;\r\n    }\r\n  }\r\n  if (*(int *)(param_2 + 0x60) == 0x1a) {\r\n    auStack_60[0] = 0x14061576b;\r\n    lVar6 = FUN_1403da910(param_1 + 0x118,2);\r\n    if (lVar6 == 0) {\r\n      return param_3;\r\n    }\r\n    auStack_60[0] = 0x140615783;\r\n    func_0x0001404362a0(lVar6);\r\n    return param_3;\r\n  }\r\n  auStack_60[0] = 0x1406157a0;\r\n  iVar5 = FUN_140614f30(param_1,param_2);\r\n  if (iVar5 < 0) {\r\n    return 0;\r\n  }\r\n  puVar2 = *(undefined8 **)(param_1 + 0x10);\r\n  plVar9 = (longlong *)((longlong)iVar5 * 0x38 + *(longlong *)(param_1 + 0x38));\r\n  lVar6 = (**(code **)*puVar2)(puVar2);\r\n  if ((*(int *)(lVar6 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar6 + 0x60))) {\r\n    bVar3 = false;\r\n  }\r\n  else {\r\n    bVar3 = true;\r\n  }\r\n  lVar6 = *plVar9;\r\n  if (*(int *)(lVar6 + 0x60) == 6) {\r\n    if (!bVar3) goto LAB_140615ac4;\r\n    bVar4 = false;\r\n    if (*(int *)(param_1 + 0x174) < 0) {\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x174) * 0x28);\r\n    }\r\n    if (((float)*(int *)(lVar6 + 0x78) + fVar10 != 0.0) &&\r\n       (lVar7 = FUN_1403da910(param_1 + 0xf8,2), lVar7 != 0)) {\r\n      FUN_14071a140(lVar7);\r\n      bVar4 = true;\r\n    }\r\n    if ((0 < *(int *)(lVar6 + 0x7c)) && (lVar7 = FUN_1403da910(param_1 + 0x108,2), lVar7 != 0)) {\r\n      fVar10 = (float)FUN_140745f60(lVar7);\r\n      fVar11 = (float)FUN_140745fb0(lVar7);\r\n      if (fVar10 < fVar11) {\r\n        FUN_1407467d0(lVar7);\r\n        return 0;\r\n      }\r\n    }\r\n    if (bVar4) {\r\n      return 0;\r\n    }\r\n  }\r\n  if (((!bVar3) || (*(int *)(lVar6 + 0x60) != 4)) || (param_3 == 1)) goto LAB_140615ac4;\r\n  iVar5 = func_0x000140647ee0(D2_GameSingletonPtr + 0x1f7b90);\r\n  param_3 = iVar5 + param_3;\r\n  if (param_3 < 1) {\r\n    param_3 = 1;\r\n  }\r\n  if (*(int *)(*plVar9 + 0xc0) - 1U < 5) {\r\n    if (*(int *)(param_1 + 0x178) < 0) {\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x178) * 0x28);\r\n    }\r\n    fVar11 = (float)FUN_1408ac040(0);\r\n    if (fVar11 < fVar10) {\r\n      param_3 = param_3 + 1;\r\n    }\r\n    if (param_3 < 2) goto LAB_140615ac4;\r\n    if (*(int *)(param_1 + 0x17c) < 0) {\r\nLAB_140615aaa:\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x17c) * 0x28);\r\n    }\r\n  }\r\n  else {\r\n    if (1 < *(int *)(*plVar9 + 0xc0) - 10U) goto LAB_140615ac4;\r\n    if (*(int *)(param_1 + 0x180) < 0) {\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x180) * 0x28);\r\n    }\r\n    fVar11 = (float)FUN_1408ac040(0);\r\n    if (fVar11 < fVar10) {\r\n      param_3 = param_3 + 1;\r\n    }\r\n    if (param_3 < 2) goto LAB_140615ac4;\r\n    if (*(int *)(param_1 + 0x184) < 0) goto LAB_140615aaa;\r\n    fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x184) * 0x28);\r\n  }\r\n  fVar11 = (float)FUN_1408ac040(0);\r\n  if (fVar11 < fVar10) {\r\n    param_3 = param_3 + -1;\r\n  }\r\nLAB_140615ac4:\r\n  lVar6 = plVar9[4];\r\n  iVar5 = func_0x00014063c9f0(plVar9,param_3);\r\n  lVar7 = plVar9[4];\r\n  FUN_1406170b0(param_1,plVar9,(int)lVar6,(int)lVar7);\r\n  if (0 < iVar5) {\r\n    lVar6 = *plVar9;\r\n    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    uStack_70 = CONCAT44((int)lVar7,iVar5);\r\n    lVar7 = FUN_140810240();\r\n    plVar8 = (longlong *)FUN_1408112c0(lVar7 + 0xc8e8,0x9b,0x10,uVar1);\r\n    if (plVar8 != (longlong *)0x0) {\r\n      *plVar8 = lVar6;\r\n      plVar8[1] = uStack_70;\r\n    }\r\n    uStack_70 = *plVar9;\r\n    auStack_78[0] = 100;\r\n    FUN_140884920(puVar2,auStack_60,0x1439ab850,auStack_78);\r\n    FUN_140882fc0(auStack_60);\r\n  }\r\n  return iVar5;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nint FUN_1406156e0(longlong param_1,longlong param_2,int param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined8 *puVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  int iVar5;\r\n  longlong lVar6;\r\n  longlong lVar7;\r\n  longlong *plVar8;\r\n  longlong *plVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  undefined1 auStack_78 [8];\r\n  undefined8 uStack_70;\r\n  undefined8 auStack_60 [5];\r\n  undefined1 local_38 [8];\r\n  int local_30;\r\n  undefined4 uStack_2c;\r\n  \r\n  iVar5 = *(int *)(param_2 + 0x60);\r\n  if ((iVar5 == 1) || (iVar5 == 0xf)) {\r\n    *(int *)(param_1 + 0x16c) = *(int *)(param_1 + 0x16c) + param_3;\r\n    local_38[0] = 0x69;\r\n    _local_30 = CONCAT44(uStack_2c,param_3);\r\n    auStack_60[0] = 0x1406157fc;\r\n    FUN_140884920(*(undefined8 *)(param_1 + 0x10),&stack0xffffffffffffffe0,&DAT_1439ab8f0,local_38);\r\n    auStack_60[0] = 0x140615806;\r\n    FUN_140882fc0(&stack0xffffffffffffffe0);\r\n    auStack_60[0] = 0x140615823;\r\n    FUN_14067aea0(D2_GameSingletonPtr + 0x1f77c8,1,param_3);\r\n    return param_3;\r\n  }\r\n  if (iVar5 == 0x10) {\r\n    auStack_60[0] = 0x140615717;\r\n    iVar5 = FUN_140615310();\r\n    if (iVar5 != 0) {\r\n      auStack_60[0] = 0x140615731;\r\n      lVar6 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439ba310,2);\r\n      iVar5 = *(int *)(*(longlong *)(lVar6 + 8) + 0x68);\r\n      auStack_60[0] = 0x140615742;\r\n      FUN_140615620(param_1,iVar5);\r\n      return iVar5;\r\n    }\r\n  }\r\n  if (*(int *)(param_2 + 0x60) == 0x1a) {\r\n    auStack_60[0] = 0x14061576b;\r\n    lVar6 = FUN_1403da910(param_1 + 0x118,2);\r\n    if (lVar6 == 0) {\r\n      return param_3;\r\n    }\r\n    auStack_60[0] = 0x140615783;\r\n    func_0x0001404362a0(lVar6);\r\n    return param_3;\r\n  }\r\n  auStack_60[0] = 0x1406157a0;\r\n  iVar5 = FUN_140614f30(param_1,param_2);\r\n  if (iVar5 < 0) {\r\n    return 0;\r\n  }\r\n  puVar2 = *(undefined8 **)(param_1 + 0x10);\r\n  plVar9 = (longlong *)((longlong)iVar5 * 0x38 + *(longlong *)(param_1 + 0x38));\r\n  lVar6 = (**(code **)*puVar2)(puVar2);\r\n  if ((*(int *)(lVar6 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar6 + 0x60))) {\r\n    bVar3 = false;\r\n  }\r\n  else {\r\n    bVar3 = true;\r\n  }\r\n  lVar6 = *plVar9;\r\n  if (*(int *)(lVar6 + 0x60) == 6) {\r\n    if (!bVar3) goto LAB_140615ac4;\r\n    bVar4 = false;\r\n    if (*(int *)(param_1 + 0x174) < 0) {\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x174) * 0x28);\r\n    }\r\n    if (((float)*(int *)(lVar6 + 0x78) + fVar10 != 0.0) &&\r\n       (lVar7 = FUN_1403da910(param_1 + 0xf8,2), lVar7 != 0)) {\r\n      FUN_14071a140(lVar7);\r\n      bVar4 = true;\r\n    }\r\n    if ((0 < *(int *)(lVar6 + 0x7c)) && (lVar7 = FUN_1403da910(param_1 + 0x108,2), lVar7 != 0)) {\r\n      fVar10 = (float)FUN_140745f60(lVar7);\r\n      fVar11 = (float)FUN_140745fb0(lVar7);\r\n      if (fVar10 < fVar11) {\r\n        FUN_1407467d0(lVar7);\r\n        return 0;\r\n      }\r\n    }\r\n    if (bVar4) {\r\n      return 0;\r\n    }\r\n  }\r\n  if (((!bVar3) || (*(int *)(lVar6 + 0x60) != 4)) || (param_3 == 1)) goto LAB_140615ac4;\r\n  iVar5 = func_0x000140647ee0(D2_GameSingletonPtr + 0x1f7b90);\r\n  param_3 = iVar5 + param_3;\r\n  if (param_3 < 1) {\r\n    param_3 = 1;\r\n  }\r\n  if (*(int *)(*plVar9 + 0xc0) - 1U < 5) {\r\n    if (*(int *)(param_1 + 0x178) < 0) {\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x178) * 0x28);\r\n    }\r\n    fVar11 = (float)FUN_1408ac040(0);\r\n    if (fVar11 < fVar10) {\r\n      param_3 = param_3 + 1;\r\n    }\r\n    if (param_3 < 2) goto LAB_140615ac4;\r\n    if (*(int *)(param_1 + 0x17c) < 0) {\r\nLAB_140615aaa:\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x17c) * 0x28);\r\n    }\r\n  }\r\n  else {\r\n    if (1 < *(int *)(*plVar9 + 0xc0) - 10U) goto LAB_140615ac4;\r\n    if (*(int *)(param_1 + 0x180) < 0) {\r\n      fVar10 = 0.0;\r\n    }\r\n    else {\r\n      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                         (longlong)*(int *)(param_1 + 0x180) * 0x28);\r\n    }\r\n    fVar11 = (float)FUN_1408ac040(0);\r\n    if (fVar11 < fVar10) {\r\n      param_3 = param_3 + 1;\r\n    }\r\n    if (param_3 < 2) goto LAB_140615ac4;\r\n    if (*(int *)(param_1 + 0x184) < 0) goto LAB_140615aaa;\r\n    fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x184) * 0x28);\r\n  }\r\n  fVar11 = (float)FUN_1408ac040(0);\r\n  if (fVar11 < fVar10) {\r\n    param_3 = param_3 + -1;\r\n  }\r\nLAB_140615ac4:\r\n  lVar6 = plVar9[4];\r\n  iVar5 = func_0x00014063c9f0(plVar9,param_3);\r\n  lVar7 = plVar9[4];\r\n  FUN_1406170b0(param_1,plVar9,(int)lVar6,(int)lVar7);\r\n  if (0 < iVar5) {\r\n    lVar6 = *plVar9;\r\n    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    uStack_70 = CONCAT44((int)lVar7,iVar5);\r\n    lVar7 = FUN_140810240();\r\n    plVar8 = (longlong *)FUN_1408112c0(lVar7 + 0xc8e8,0x9b,0x10,uVar1);\r\n    if (plVar8 != (longlong *)0x0) {\r\n      *plVar8 = lVar6;\r\n      plVar8[1] = uStack_70;\r\n    }\r\n    uStack_70 = *plVar9;\r\n    auStack_78[0] = 100;\r\n    FUN_140884920(puVar2,auStack_60,0x1439ab850,auStack_78);\r\n    FUN_140882fc0(auStack_60);\r\n  }\r\n  return iVar5;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
}

````

## Unescaped text fields for reading

### text

````text

void FUN_140841030(longlong param_1,undefined8 *param_2,undefined1 param_3)

{
  longlong lVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  char cVar5;
  char cVar6;
  undefined4 uVar7;
  int iVar8;
  longlong lVar9;
  undefined8 uVar10;
  longlong lVar11;
  undefined8 uVar12;
  longlong lVar13;
  int iVar14;
  int iVar15;
  undefined4 local_res8 [2];
  undefined4 local_b8 [2];
  undefined1 local_b0 [40];
  undefined4 local_88;
  undefined8 local_80;
  undefined4 local_78 [2];
  undefined1 local_70 [40];
  undefined4 local_48;
  
  local_80 = 0xfffffffffffffffe;
  uVar10 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x328);
  lVar11 = param_2[1];
  lVar1 = *(longlong *)(lVar11 + 0xf8);
  lVar9 = (**(code **)*param_2)(param_2);
  if ((DAT_1439aeaf0 <= *(int *)(lVar9 + 0x60)) && (*(int *)(lVar9 + 0x60) <= DAT_1439aeaf4)) {
    FUN_1408404c0(param_2,local_b8);
    cVar5 = FUN_14060b3e0(local_b8);
    if (cVar5 == '\0') {
      uVar10 = FUN_140854880(param_1);
      lVar11 = FUN_140877720(uVar10,&DAT_1439c55b0,2);
      if (lVar11 != 0) {
        local_78[0] = local_b8[0];
        FUN_14013f490(local_70,local_b0);
        local_48 = local_88;
        FUN_14060e4c0(lVar11,local_78);
        uVar7 = FUN_1410701c0(local_b8);
        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&
           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),
           lVar11 != 0)) {
          local_res8[0] = uVar7;
          FUN_1406734b0(lVar11,lVar1,1,local_res8);
        }
        FUN_14067e200(D2_GameSingletonPtr + 0x1f77c8,lVar1,1,param_3,param_2[2]);
        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&
           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200), lVar11 != 0
           )) {
          FUN_14045d380(lVar11,1,lVar1 + 100);
        }
      }
    }
    FUN_14013fd10(local_b0);
    return;
  }
  cVar5 = FUN_140615d40(uVar10,lVar1);
  lVar9 = FUN_1403da910(param_1 + 0x120,2);
  if ((lVar9 == 0) || (cVar6 = FUN_1404c1f20(), cVar6 == '\0')) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if (*(int *)(lVar1 + 0x60) - 7U < 2) {
    bVar3 = true;
    if (bVar2) goto LAB_14084120c;
    bVar4 = true;
  }
  else {
    bVar3 = false;
LAB_14084120c:
    bVar4 = false;
    if ((bVar3) && (bVar2)) {
      bVar2 = true;
      goto LAB_14084121d;
    }
  }
  bVar2 = false;
LAB_14084121d:
  if (cVar5 == '\0') {
    if (bVar2) {
      iVar14 = *(int *)(lVar1 + 0xec);
    }
    else {
      iVar14 = *(int *)(lVar11 + 0x100);
    }
    if (bVar4) {
      FUN_140615620(uVar10,iVar14);
      iVar8 = iVar14;
    }
    else {
      lVar9 = lVar1;
      if (bVar3) {
        uVar12 = FUN_140854880(param_1);
        lVar13 = FUN_140877720(uVar12,&DAT_1439e2bb0,2);
        if (lVar13 != 0) {
          lVar9 = FUN_14007b840(lVar13);
        }
      }
      iVar8 = FUN_1406156e0(uVar10,lVar9,iVar14);
    }
    iVar15 = iVar8;
    if (*(int *)(lVar1 + 0x60) == 0x10) {
      iVar15 = 1;
    }
    FUN_14067e200(D2_GameSingletonPtr + 0x1f77c8,lVar1,iVar15,param_3,param_2[2]);
    iVar15 = *(int *)(lVar1 + 0x60);
    if (iVar15 != 6) {
      if (((iVar15 == 1) || (iVar15 == 0xf)) || (bVar4)) {
        if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&
           (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),
           lVar11 != 0)) {
          FUN_1406733c0(lVar11,iVar14);
        }
      }
      else {
        if (iVar15 == 0x10) {
          if (1 < iVar8) {
            if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&
               (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0x58),
               lVar11 != 0)) {
              FUN_1406733c0(lVar11,iVar8);
            }
            uVar10 = FUN_140854880(param_1);
            lVar11 = FUN_140877720(uVar10,&DAT_1439ba310,2);
            if (lVar11 == 0) {
              return;
            }
            if (*(char *)(lVar11 + 0x6c) == '\0') {
              return;
            }
            *(int *)(lVar11 + 0x68) = *(int *)(lVar11 + 0x68) + iVar8;
            return;
          }
          local_res8[0] = *(undefined4 *)(lVar11 + 200);
        }
        else {
          local_res8[0] = 0;
        }
        FUN_140840fb0(param_1,lVar1,iVar8,local_res8);
        FUN_140840e60(param_1,lVar1,iVar8);
        FUN_140840ff0(param_1,lVar1 + 0xd0);
        if (*(int *)(lVar1 + 0x60) - 7U < 2) {
          if ((*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0) &&
             (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200),
             lVar11 != 0)) {
            FUN_14045d380(lVar11,3,lVar1 + 100);
          }
        }
        else if (((*(int *)(lVar1 + 0x60) == 0xb) &&
                 (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0)) &&
                (lVar11 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 200),
                lVar11 != 0)) {
          FUN_14045d380(lVar11,6,lVar1 + 100);
        }
      }
    }
  }
  return;
}


````

### text

````text

int FUN_1406156e0(longlong param_1,longlong param_2,int param_3)

{
  undefined4 uVar1;
  undefined8 *puVar2;
  bool bVar3;
  bool bVar4;
  int iVar5;
  longlong lVar6;
  longlong lVar7;
  longlong *plVar8;
  longlong *plVar9;
  float fVar10;
  float fVar11;
  undefined1 auStack_78 [8];
  undefined8 uStack_70;
  undefined8 auStack_60 [5];
  undefined1 local_38 [8];
  int local_30;
  undefined4 uStack_2c;
  
  iVar5 = *(int *)(param_2 + 0x60);
  if ((iVar5 == 1) || (iVar5 == 0xf)) {
    *(int *)(param_1 + 0x16c) = *(int *)(param_1 + 0x16c) + param_3;
    local_38[0] = 0x69;
    _local_30 = CONCAT44(uStack_2c,param_3);
    auStack_60[0] = 0x1406157fc;
    FUN_140884920(*(undefined8 *)(param_1 + 0x10),&stack0xffffffffffffffe0,&DAT_1439ab8f0,local_38);
    auStack_60[0] = 0x140615806;
    FUN_140882fc0(&stack0xffffffffffffffe0);
    auStack_60[0] = 0x140615823;
    FUN_14067aea0(D2_GameSingletonPtr + 0x1f77c8,1,param_3);
    return param_3;
  }
  if (iVar5 == 0x10) {
    auStack_60[0] = 0x140615717;
    iVar5 = FUN_140615310();
    if (iVar5 != 0) {
      auStack_60[0] = 0x140615731;
      lVar6 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439ba310,2);
      iVar5 = *(int *)(*(longlong *)(lVar6 + 8) + 0x68);
      auStack_60[0] = 0x140615742;
      FUN_140615620(param_1,iVar5);
      return iVar5;
    }
  }
  if (*(int *)(param_2 + 0x60) == 0x1a) {
    auStack_60[0] = 0x14061576b;
    lVar6 = FUN_1403da910(param_1 + 0x118,2);
    if (lVar6 == 0) {
      return param_3;
    }
    auStack_60[0] = 0x140615783;
    func_0x0001404362a0(lVar6);
    return param_3;
  }
  auStack_60[0] = 0x1406157a0;
  iVar5 = FUN_140614f30(param_1,param_2);
  if (iVar5 < 0) {
    return 0;
  }
  puVar2 = *(undefined8 **)(param_1 + 0x10);
  plVar9 = (longlong *)((longlong)iVar5 * 0x38 + *(longlong *)(param_1 + 0x38));
  lVar6 = (**(code **)*puVar2)(puVar2);
  if ((*(int *)(lVar6 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar6 + 0x60))) {
    bVar3 = false;
  }
  else {
    bVar3 = true;
  }
  lVar6 = *plVar9;
  if (*(int *)(lVar6 + 0x60) == 6) {
    if (!bVar3) goto LAB_140615ac4;
    bVar4 = false;
    if (*(int *)(param_1 + 0x174) < 0) {
      fVar10 = 0.0;
    }
    else {
      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +
                         (longlong)*(int *)(param_1 + 0x174) * 0x28);
    }
    if (((float)*(int *)(lVar6 + 0x78) + fVar10 != 0.0) &&
       (lVar7 = FUN_1403da910(param_1 + 0xf8,2), lVar7 != 0)) {
      FUN_14071a140(lVar7);
      bVar4 = true;
    }
    if ((0 < *(int *)(lVar6 + 0x7c)) && (lVar7 = FUN_1403da910(param_1 + 0x108,2), lVar7 != 0)) {
      fVar10 = (float)FUN_140745f60(lVar7);
      fVar11 = (float)FUN_140745fb0(lVar7);
      if (fVar10 < fVar11) {
        FUN_1407467d0(lVar7);
        return 0;
      }
    }
    if (bVar4) {
      return 0;
    }
  }
  if (((!bVar3) || (*(int *)(lVar6 + 0x60) != 4)) || (param_3 == 1)) goto LAB_140615ac4;
  iVar5 = func_0x000140647ee0(D2_GameSingletonPtr + 0x1f7b90);
  param_3 = iVar5 + param_3;
  if (param_3 < 1) {
    param_3 = 1;
  }
  if (*(int *)(*plVar9 + 0xc0) - 1U < 5) {
    if (*(int *)(param_1 + 0x178) < 0) {
      fVar10 = 0.0;
    }
    else {
      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +
                         (longlong)*(int *)(param_1 + 0x178) * 0x28);
    }
    fVar11 = (float)FUN_1408ac040(0);
    if (fVar11 < fVar10) {
      param_3 = param_3 + 1;
    }
    if (param_3 < 2) goto LAB_140615ac4;
    if (*(int *)(param_1 + 0x17c) < 0) {
LAB_140615aaa:
      fVar10 = 0.0;
    }
    else {
      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +
                         (longlong)*(int *)(param_1 + 0x17c) * 0x28);
    }
  }
  else {
    if (1 < *(int *)(*plVar9 + 0xc0) - 10U) goto LAB_140615ac4;
    if (*(int *)(param_1 + 0x180) < 0) {
      fVar10 = 0.0;
    }
    else {
      fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +
                         (longlong)*(int *)(param_1 + 0x180) * 0x28);
    }
    fVar11 = (float)FUN_1408ac040(0);
    if (fVar11 < fVar10) {
      param_3 = param_3 + 1;
    }
    if (param_3 < 2) goto LAB_140615ac4;
    if (*(int *)(param_1 + 0x184) < 0) goto LAB_140615aaa;
    fVar10 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48) + 0x24 +
                       (longlong)*(int *)(param_1 + 0x184) * 0x28);
  }
  fVar11 = (float)FUN_1408ac040(0);
  if (fVar11 < fVar10) {
    param_3 = param_3 + -1;
  }
LAB_140615ac4:
  lVar6 = plVar9[4];
  iVar5 = func_0x00014063c9f0(plVar9,param_3);
  lVar7 = plVar9[4];
  FUN_1406170b0(param_1,plVar9,(int)lVar6,(int)lVar7);
  if (0 < iVar5) {
    lVar6 = *plVar9;
    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
    uStack_70 = CONCAT44((int)lVar7,iVar5);
    lVar7 = FUN_140810240();
    plVar8 = (longlong *)FUN_1408112c0(lVar7 + 0xc8e8,0x9b,0x10,uVar1);
    if (plVar8 != (longlong *)0x0) {
      *plVar8 = lVar6;
      plVar8[1] = uStack_70;
    }
    uStack_70 = *plVar9;
    auStack_78[0] = 100;
    FUN_140884920(puVar2,auStack_60,0x1439ab850,auStack_78);
    FUN_140882fc0(auStack_60);
  }
  return iVar5;
}


````
