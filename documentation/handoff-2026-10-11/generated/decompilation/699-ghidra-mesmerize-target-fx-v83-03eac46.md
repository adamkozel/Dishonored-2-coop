# ghidra-mesmerize-target-fx-v83.json

Original: [work/ghidra-mesmerize-target-fx-v83.json](<../../../../work/ghidra-mesmerize-target-fx-v83.json>)

SHA-256: `0B2101D6947A4B85B0FBB093DBF4194E59620467677CF967E9BB1A7C51596269`; 62315 bytes.

## Original saved record

````json
[
  {
    "address": "14072f3d0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14072f3d0(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  code *pcVar3;\r\n  bool bVar4;\r\n  byte bVar5;\r\n  char cVar6;\r\n  longlong lVar7;\r\n  float *pfVar8;\r\n  longlong lVar9;\r\n  undefined8 uVar10;\r\n  longlong lVar11;\r\n  undefined4 *puVar12;\r\n  double *pdVar13;\r\n  undefined8 *puVar14;\r\n  longlong lVar15;\r\n  longlong lVar16;\r\n  undefined4 *puVar17;\r\n  int iVar18;\r\n  float fVar19;\r\n  undefined1 auStackX_8 [8];\r\n  longlong lStackX_10;\r\n  longlong lStackX_18;\r\n  undefined8 uStackX_20;\r\n  undefined1 auStack_b8 [8];\r\n  undefined1 auStack_b0 [8];\r\n  undefined1 auStack_a8 [8];\r\n  undefined8 uStack_a0;\r\n  undefined4 uStack_98;\r\n  undefined4 uStack_94;\r\n  undefined8 auStack_90 [10];\r\n  \r\n  lVar7 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  lVar15 = *(longlong *)(param_1 + 8);\r\n  uStackX_20 = *(undefined8 *)(param_1 + 0x10);\r\n  lStackX_18 = lVar15;\r\n  pfVar8 = (float *)FUN_140877eb0(uStackX_20,auStackX_8);\r\n  fVar19 = *(float *)(param_1 + 0x1e8) - *pfVar8;\r\n  *(float *)(param_1 + 0x1e8) = fVar19;\r\n  iVar18 = *(int *)(param_1 + 0x290);\r\n  if ((iVar18 < 0) ||\r\n     (lVar16 = *(longlong *)(*(longlong *)(param_1 + 0x1c8) + 0x48),\r\n     *(float *)(lVar16 + 0x24 + (longlong)iVar18 * 0x28) ==\r\n     *(float *)(lVar16 + 8 + (longlong)iVar18 * 0x28))) {\r\n    bVar4 = false;\r\n  }\r\n  else {\r\n    bVar4 = true;\r\n  }\r\n  if ((fVar19 <= 0.0) || (bVar4)) {\r\n    uVar1 = *(undefined4 *)(lVar7 + 0xa8);\r\n    lVar15 = FUN_140810240();\r\n    FUN_1408112c0(lVar15 + 0xc8e8,0x100,0,uVar1);\r\n    return;\r\n  }\r\n  lVar7 = *(longlong *)(param_1 + 0x198);\r\n  lVar16 = (ulonglong)(*(uint *)(param_1 + 0x1a0) & 0xffffff) * 0x70 + lVar7;\r\n  for (; lVar7 != lVar16; lVar7 = lVar7 + 0x70) {\r\n    if (*(char *)(lVar7 + 0x6e) == '\\0') {\r\n      FUN_14072a280(param_1,lVar7);\r\n    }\r\n  }\r\n  bVar4 = false;\r\n  iVar18 = (*(uint *)(param_1 + 0x1b0) & 0xffffff) - 1;\r\n  if (-1 < iVar18) {\r\n    lVar7 = (longlong)iVar18 * 0x70;\r\n    lVar16 = lVar7;\r\n    do {\r\n      lStackX_10 = lVar7;\r\n      cVar6 = FUN_14072d600(param_1,iVar18);\r\n      if (cVar6 == '\\0') {\r\n        puVar17 = (undefined4 *)(*(longlong *)(param_1 + 0x1a8) + lVar16);\r\n        lVar9 = FUN_1406dc560(*puVar17);\r\n        if (*(char *)((longlong)puVar17 + 0x6d) != '\\0') {\r\n          FUN_140729350(param_1,puVar17);\r\n          if (puVar17[0xc] != -1) {\r\n            uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n            lVar11 = FUN_1401a04a0(uVar10,puVar17[0xc]);\r\n            lVar15 = lStackX_18;\r\n            if (lVar11 != 0) {\r\n              uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n              uVar10 = FUN_1401a04a0(uVar10,puVar17[0xd]);\r\n              puVar12 = (undefined4 *)FUN_140877eb0(uStackX_20,auStackX_8);\r\n              FUN_14072e5c0(param_1,iVar18,lVar11,uVar10,*puVar12);\r\n              lVar7 = lStackX_10;\r\n              lVar15 = lStackX_18;\r\n            }\r\n          }\r\n          if ((*(char *)(puVar17 + 0x1b) == '\\0') || (0.0 < (float)puVar17[0x1a])) {\r\n            bVar5 = 0;\r\n          }\r\n          else {\r\n            bVar5 = 1;\r\n          }\r\n          bVar4 = (bool)(bVar4 | bVar5);\r\n          if (((*(char *)(puVar17 + 0x1b) == '\\0') && (1.0 <= (float)puVar17[0x1a])) &&\r\n             ((*(byte *)(lVar9 + 0x368) & 0x80) != 0)) {\r\n            FUN_14072b6c0(param_1,lVar9);\r\n          }\r\n        }\r\n        uVar10 = FUN_1406dc560(*(undefined4 *)(lVar7 + *(longlong *)(param_1 + 0x1a8)));\r\n        lVar11 = FUN_1408f2e50(uVar10);\r\n        if (*(int *)(lVar11 + 0x370) == 2) {\r\n          if (0.0 <= *(double *)(puVar17 + 10)) {\r\n            pdVar13 = (double *)FUN_140878030(lVar9,auStack_b0);\r\n            if (*pdVar13 < *(double *)(puVar17 + 10)) goto code_r0x00014072f73a;\r\n            uStack_98 = 0x26;\r\n            uStack_a0 = DAT_14398dad0;\r\n            FUN_140131ae0(auStack_90);\r\n            uVar1 = *(undefined4 *)(lVar9 + 0xa8);\r\n            lVar7 = FUN_140810240();\r\n            puVar14 = (undefined8 *)FUN_1408112c0(lVar7 + 0xc8e8,0x4f,0x18,uVar1);\r\n            if (puVar14 != (undefined8 *)0x0) {\r\n              *puVar14 = uStack_a0;\r\n              puVar14[1] = CONCAT44(uStack_94,uStack_98);\r\n              puVar14[2] = auStack_90[0];\r\n            }\r\n            fVar19 = (float)FUN_1408ac040(*(undefined4 *)(lVar15 + 0x90),\r\n                                          *(undefined4 *)(lVar15 + 0x94));\r\n            pdVar13 = (double *)FUN_140878030(lVar9,auStack_a8);\r\n            lVar7 = lStackX_10;\r\n          }\r\n          else {\r\n            fVar19 = *(float *)(lVar15 + 0x98);\r\n            pdVar13 = (double *)FUN_140878030(lVar9,auStack_b8);\r\n          }\r\n          *(double *)(puVar17 + 10) = (double)fVar19 + *pdVar13;\r\n        }\r\ncode_r0x00014072f73a:\r\n        if (bVar4) {\r\n          iVar2 = *(int *)(param_1 + 0x2c);\r\n          if (iVar2 != 2) {\r\n            if ((iVar2 != *(int *)(param_1 + 0x28)) &&\r\n               (pcVar3 = *(code **)(param_1 + 0x20 + ((longlong)iVar2 + 1) * 0x18),\r\n               pcVar3 != (code *)0x0)) {\r\n              (*pcVar3)(*(undefined8 *)(param_1 + 0x20),2);\r\n            }\r\n            if ((*(int *)(param_1 + 0x28) != 2) && (*(code **)(param_1 + 0x60) != (code *)0x0)) {\r\n              (**(code **)(param_1 + 0x60))\r\n                        (*(undefined8 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x2c));\r\n            }\r\n            *(undefined4 *)(param_1 + 0x2c) = 2;\r\n          }\r\n        }\r\n      }\r\n      lVar7 = lVar7 + -0x70;\r\n      lVar16 = lVar16 + -0x70;\r\n      iVar18 = iVar18 + -1;\r\n    } while (-1 < iVar18);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14072f3d0(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  code *pcVar3;\r\n  bool bVar4;\r\n  byte bVar5;\r\n  char cVar6;\r\n  longlong lVar7;\r\n  float *pfVar8;\r\n  longlong lVar9;\r\n  undefined8 uVar10;\r\n  longlong lVar11;\r\n  undefined4 *puVar12;\r\n  double *pdVar13;\r\n  undefined8 *puVar14;\r\n  longlong lVar15;\r\n  longlong lVar16;\r\n  undefined4 *puVar17;\r\n  int iVar18;\r\n  float fVar19;\r\n  undefined1 auStackX_8 [8];\r\n  longlong lStackX_10;\r\n  longlong lStackX_18;\r\n  undefined8 uStackX_20;\r\n  undefined1 auStack_b8 [8];\r\n  undefined1 auStack_b0 [8];\r\n  undefined1 auStack_a8 [8];\r\n  undefined8 uStack_a0;\r\n  undefined4 uStack_98;\r\n  undefined4 uStack_94;\r\n  undefined8 auStack_90 [10];\r\n  \r\n  lVar7 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  lVar15 = *(longlong *)(param_1 + 8);\r\n  uStackX_20 = *(undefined8 *)(param_1 + 0x10);\r\n  lStackX_18 = lVar15;\r\n  pfVar8 = (float *)FUN_140877eb0(uStackX_20,auStackX_8);\r\n  fVar19 = *(float *)(param_1 + 0x1e8) - *pfVar8;\r\n  *(float *)(param_1 + 0x1e8) = fVar19;\r\n  iVar18 = *(int *)(param_1 + 0x290);\r\n  if ((iVar18 < 0) ||\r\n     (lVar16 = *(longlong *)(*(longlong *)(param_1 + 0x1c8) + 0x48),\r\n     *(float *)(lVar16 + 0x24 + (longlong)iVar18 * 0x28) ==\r\n     *(float *)(lVar16 + 8 + (longlong)iVar18 * 0x28))) {\r\n    bVar4 = false;\r\n  }\r\n  else {\r\n    bVar4 = true;\r\n  }\r\n  if ((fVar19 <= 0.0) || (bVar4)) {\r\n    uVar1 = *(undefined4 *)(lVar7 + 0xa8);\r\n    lVar15 = FUN_140810240();\r\n    FUN_1408112c0(lVar15 + 0xc8e8,0x100,0,uVar1);\r\n    return;\r\n  }\r\n  lVar7 = *(longlong *)(param_1 + 0x198);\r\n  lVar16 = (ulonglong)(*(uint *)(param_1 + 0x1a0) & 0xffffff) * 0x70 + lVar7;\r\n  for (; lVar7 != lVar16; lVar7 = lVar7 + 0x70) {\r\n    if (*(char *)(lVar7 + 0x6e) == '\\0') {\r\n      FUN_14072a280(param_1,lVar7);\r\n    }\r\n  }\r\n  bVar4 = false;\r\n  iVar18 = (*(uint *)(param_1 + 0x1b0) & 0xffffff) - 1;\r\n  if (-1 < iVar18) {\r\n    lVar7 = (longlong)iVar18 * 0x70;\r\n    lVar16 = lVar7;\r\n    do {\r\n      lStackX_10 = lVar7;\r\n      cVar6 = FUN_14072d600(param_1,iVar18);\r\n      if (cVar6 == '\\0') {\r\n        puVar17 = (undefined4 *)(*(longlong *)(param_1 + 0x1a8) + lVar16);\r\n        lVar9 = FUN_1406dc560(*puVar17);\r\n        if (*(char *)((longlong)puVar17 + 0x6d) != '\\0') {\r\n          FUN_140729350(param_1,puVar17);\r\n          if (puVar17[0xc] != -1) {\r\n            uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n            lVar11 = FUN_1401a04a0(uVar10,puVar17[0xc]);\r\n            lVar15 = lStackX_18;\r\n            if (lVar11 != 0) {\r\n              uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n              uVar10 = FUN_1401a04a0(uVar10,puVar17[0xd]);\r\n              puVar12 = (undefined4 *)FUN_140877eb0(uStackX_20,auStackX_8);\r\n              FUN_14072e5c0(param_1,iVar18,lVar11,uVar10,*puVar12);\r\n              lVar7 = lStackX_10;\r\n              lVar15 = lStackX_18;\r\n            }\r\n          }\r\n          if ((*(char *)(puVar17 + 0x1b) == '\\0') || (0.0 < (float)puVar17[0x1a])) {\r\n            bVar5 = 0;\r\n          }\r\n          else {\r\n            bVar5 = 1;\r\n          }\r\n          bVar4 = (bool)(bVar4 | bVar5);\r\n          if (((*(char *)(puVar17 + 0x1b) == '\\0') && (1.0 <= (float)puVar17[0x1a])) &&\r\n             ((*(byte *)(lVar9 + 0x368) & 0x80) != 0)) {\r\n            FUN_14072b6c0(param_1,lVar9);\r\n          }\r\n        }\r\n        uVar10 = FUN_1406dc560(*(undefined4 *)(lVar7 + *(longlong *)(param_1 + 0x1a8)));\r\n        lVar11 = FUN_1408f2e50(uVar10);\r\n        if (*(int *)(lVar11 + 0x370) == 2) {\r\n          if (0.0 <= *(double *)(puVar17 + 10)) {\r\n            pdVar13 = (double *)FUN_140878030(lVar9,auStack_b0);\r\n            if (*pdVar13 < *(double *)(puVar17 + 10)) goto code_r0x00014072f73a;\r\n            uStack_98 = 0x26;\r\n            uStack_a0 = DAT_14398dad0;\r\n            FUN_140131ae0(auStack_90);\r\n            uVar1 = *(undefined4 *)(lVar9 + 0xa8);\r\n            lVar7 = FUN_140810240();\r\n            puVar14 = (undefined8 *)FUN_1408112c0(lVar7 + 0xc8e8,0x4f,0x18,uVar1);\r\n            if (puVar14 != (undefined8 *)0x0) {\r\n              *puVar14 = uStack_a0;\r\n              puVar14[1] = CONCAT44(uStack_94,uStack_98);\r\n              puVar14[2] = auStack_90[0];\r\n            }\r\n            fVar19 = (float)FUN_1408ac040(*(undefined4 *)(lVar15 + 0x90),\r\n                                          *(undefined4 *)(lVar15 + 0x94));\r\n            pdVar13 = (double *)FUN_140878030(lVar9,auStack_a8);\r\n            lVar7 = lStackX_10;\r\n          }\r\n          else {\r\n            fVar19 = *(float *)(lVar15 + 0x98);\r\n            pdVar13 = (double *)FUN_140878030(lVar9,auStack_b8);\r\n          }\r\n          *(double *)(puVar17 + 10) = (double)fVar19 + *pdVar13;\r\n        }\r\ncode_r0x00014072f73a:\r\n        if (bVar4) {\r\n          iVar2 = *(int *)(param_1 + 0x2c);\r\n          if (iVar2 != 2) {\r\n            if ((iVar2 != *(int *)(param_1 + 0x28)) &&\r\n               (pcVar3 = *(code **)(param_1 + 0x20 + ((longlong)iVar2 + 1) * 0x18),\r\n               pcVar3 != (code *)0x0)) {\r\n              (*pcVar3)(*(undefined8 *)(param_1 + 0x20),2);\r\n            }\r\n            if ((*(int *)(param_1 + 0x28) != 2) && (*(code **)(param_1 + 0x60) != (code *)0x0)) {\r\n              (**(code **)(param_1 + 0x60))\r\n                        (*(undefined8 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x2c));\r\n            }\r\n            *(undefined4 *)(param_1 + 0x2c) = 2;\r\n          }\r\n        }\r\n      }\r\n      lVar7 = lVar7 + -0x70;\r\n      lVar16 = lVar16 + -0x70;\r\n      iVar18 = iVar18 + -1;\r\n    } while (-1 < iVar18);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "140729190",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140729190(longlong param_1,int param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  longlong lVar4;\r\n  undefined8 *puVar5;\r\n  uint uVar6;\r\n  int iVar7;\r\n  undefined8 uStack_70;\r\n  undefined4 uStack_68;\r\n  undefined4 uStack_64;\r\n  undefined8 uStack_60;\r\n  undefined1 auStack_58 [8];\r\n  undefined4 uStack_50;\r\n  undefined1 auStack_40 [24];\r\n  \r\n  iVar7 = 0;\r\n  if ((*(uint *)(param_1 + 0x1b0) & 0xffffff) != 0) {\r\n    do {\r\n      lVar2 = FUN_1406dc560(*(undefined4 *)((longlong)iVar7 * 0x70 + *(longlong *)(param_1 + 0x1a8))\r\n                           );\r\n      if (lVar2 != 0) {\r\n        uVar3 = FUN_1406dc560(*(undefined4 *)\r\n                               ((longlong)iVar7 * 0x70 + *(longlong *)(param_1 + 0x1a8)));\r\n        lVar4 = FUN_1408f2e50(uVar3);\r\n        uStack_70 = DAT_14398dad0;\r\n        uStack_68 = 0x28;\r\n        if (*(int *)(lVar4 + 0x370) != 2) {\r\n          uStack_68 = 0x27;\r\n        }\r\n        FUN_140131ae0(&uStack_60);\r\n        uVar1 = *(undefined4 *)(lVar2 + 0xa8);\r\n        lVar2 = FUN_140810240();\r\n        puVar5 = (undefined8 *)FUN_1408112c0(lVar2 + 0xc8e8,0x4f,0x18,uVar1);\r\n        if (puVar5 != (undefined8 *)0x0) {\r\n          *puVar5 = uStack_70;\r\n          puVar5[1] = CONCAT44(uStack_64,uStack_68);\r\n          puVar5[2] = uStack_60;\r\n        }\r\n      }\r\n      iVar7 = iVar7 + 1;\r\n    } while (iVar7 < (int)(*(uint *)(param_1 + 0x1b0) & 0xffffff));\r\n  }\r\n  if (param_2 == 2) {\r\n    uVar6 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n    while (uVar6 = uVar6 - 1, -1 < (int)uVar6) {\r\n      FUN_14072b0e0(param_1,uVar6,1);\r\n    }\r\n    *(uint *)(param_1 + 0x170) = *(uint *)(param_1 + 0x170) & 0xff000000;\r\n    *(uint *)(param_1 + 0x1a0) = *(uint *)(param_1 + 0x1a0) & 0xff000000;\r\n  }\r\n  uStack_70 = CONCAT71(uStack_70._1_7_,0x69);\r\n  uStack_68 = 0;\r\n  auStack_58[0] = 0x69;\r\n  uStack_50 = 4;\r\n  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  FUN_1408848a0(uVar3,auStack_40,&DAT_1439ab620,auStack_58,&uStack_70);\r\n  FUN_140882fc0(auStack_40);\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140729190(longlong param_1,int param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  longlong lVar4;\r\n  undefined8 *puVar5;\r\n  uint uVar6;\r\n  int iVar7;\r\n  undefined8 uStack_70;\r\n  undefined4 uStack_68;\r\n  undefined4 uStack_64;\r\n  undefined8 uStack_60;\r\n  undefined1 auStack_58 [8];\r\n  undefined4 uStack_50;\r\n  undefined1 auStack_40 [24];\r\n  \r\n  iVar7 = 0;\r\n  if ((*(uint *)(param_1 + 0x1b0) & 0xffffff) != 0) {\r\n    do {\r\n      lVar2 = FUN_1406dc560(*(undefined4 *)((longlong)iVar7 * 0x70 + *(longlong *)(param_1 + 0x1a8))\r\n                           );\r\n      if (lVar2 != 0) {\r\n        uVar3 = FUN_1406dc560(*(undefined4 *)\r\n                               ((longlong)iVar7 * 0x70 + *(longlong *)(param_1 + 0x1a8)));\r\n        lVar4 = FUN_1408f2e50(uVar3);\r\n        uStack_70 = DAT_14398dad0;\r\n        uStack_68 = 0x28;\r\n        if (*(int *)(lVar4 + 0x370) != 2) {\r\n          uStack_68 = 0x27;\r\n        }\r\n        FUN_140131ae0(&uStack_60);\r\n        uVar1 = *(undefined4 *)(lVar2 + 0xa8);\r\n        lVar2 = FUN_140810240();\r\n        puVar5 = (undefined8 *)FUN_1408112c0(lVar2 + 0xc8e8,0x4f,0x18,uVar1);\r\n        if (puVar5 != (undefined8 *)0x0) {\r\n          *puVar5 = uStack_70;\r\n          puVar5[1] = CONCAT44(uStack_64,uStack_68);\r\n          puVar5[2] = uStack_60;\r\n        }\r\n      }\r\n      iVar7 = iVar7 + 1;\r\n    } while (iVar7 < (int)(*(uint *)(param_1 + 0x1b0) & 0xffffff));\r\n  }\r\n  if (param_2 == 2) {\r\n    uVar6 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n    while (uVar6 = uVar6 - 1, -1 < (int)uVar6) {\r\n      FUN_14072b0e0(param_1,uVar6,1);\r\n    }\r\n    *(uint *)(param_1 + 0x170) = *(uint *)(param_1 + 0x170) & 0xff000000;\r\n    *(uint *)(param_1 + 0x1a0) = *(uint *)(param_1 + 0x1a0) & 0xff000000;\r\n  }\r\n  uStack_70 = CONCAT71(uStack_70._1_7_,0x69);\r\n  uStack_68 = 0;\r\n  auStack_58[0] = 0x69;\r\n  uStack_50 = 4;\r\n  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n  FUN_1408848a0(uVar3,auStack_40,&DAT_1439ab620,auStack_58,&uStack_70);\r\n  FUN_140882fc0(auStack_40);\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "140729350",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140729350(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  int iVar2;\r\n  int iVar3;\r\n  int iVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float local_118;\r\n  float local_114;\r\n  float local_110;\r\n  float local_108;\r\n  float local_104;\r\n  float local_100;\r\n  float local_f8;\r\n  float local_f4;\r\n  float local_f0;\r\n  undefined4 local_ec;\r\n  undefined4 local_e8;\r\n  undefined4 local_e4;\r\n  undefined4 local_e0;\r\n  undefined4 local_dc;\r\n  undefined4 local_d8;\r\n  float local_d0;\r\n  float local_cc;\r\n  float local_c8;\r\n  undefined4 local_c4;\r\n  undefined4 local_c0;\r\n  undefined4 local_bc;\r\n  undefined4 local_b8;\r\n  undefined4 local_b4;\r\n  undefined4 local_b0;\r\n  \r\n  local_118 = DAT_1431af668;\r\n  local_114 = DAT_1431af66c;\r\n  local_110 = DAT_1431af670;\r\n  local_d0 = DAT_14228a780;\r\n  local_cc = DAT_14228a784;\r\n  local_c8 = DAT_14228a788;\r\n  local_c4 = DAT_14228a78c;\r\n  local_c0 = DAT_14228a790;\r\n  local_bc = DAT_14228a794;\r\n  local_b8 = DAT_14228a798;\r\n  local_b4 = DAT_14228a79c;\r\n  local_b0 = DAT_14228a7a0;\r\n  if ((*(short *)(param_2 + 0x58) != -1) && (lVar1 = FUN_1403da910(param_2 + 8,2), lVar1 != 0)) {\r\n    FUN_14053dac0(lVar1,0,param_2 + 0x3c,&local_118,&local_d0);\r\n  }\r\n  iVar4 = -1;\r\n  iVar3 = -1;\r\n  iVar2 = 0;\r\n  fVar9 = 180.0;\r\n  if ((*(uint *)(param_1 + 0x138) & 0xffffff) != 0) {\r\n    do {\r\n      local_108 = DAT_1431af668;\r\n      local_104 = DAT_1431af66c;\r\n      local_100 = DAT_1431af670;\r\n      local_f8 = DAT_14228a780;\r\n      local_f4 = DAT_14228a784;\r\n      local_f0 = DAT_14228a788;\r\n      local_ec = DAT_14228a78c;\r\n      local_e8 = DAT_14228a790;\r\n      local_e4 = DAT_14228a794;\r\n      local_e0 = DAT_14228a798;\r\n      local_dc = DAT_14228a79c;\r\n      local_d8 = DAT_14228a7a0;\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)iVar2 * 0x20 + *(longlong *)(param_1 + 0x130),&local_108,&local_f8);\r\n      fVar8 = local_118 - local_108;\r\n      fVar6 = local_114 - local_104;\r\n      fVar7 = local_110 - local_100;\r\n      fVar5 = fVar6 * fVar6 + fVar8 * fVar8 + fVar7 * fVar7;\r\n      if (fVar5 <= 1.1754944e-38) {\r\n        fVar5 = 1.1754944e-38;\r\n      }\r\n      fVar5 = 1.0 / SQRT(fVar5);\r\n      fVar5 = local_f4 * fVar6 * fVar5 + local_f8 * fVar8 * fVar5 + local_f0 * fVar7 * fVar5;\r\n      if (-1.0 < fVar5) {\r\n        if (fVar5 < 1.0) {\r\n          fVar5 = (float)FUN_1411afeb4(fVar5);\r\n        }\r\n        else {\r\n          fVar5 = 0.0;\r\n        }\r\n      }\r\n      else {\r\n        fVar5 = 3.1415927;\r\n      }\r\n      fVar5 = fVar5 * 57.295776;\r\n      if ((360.0 <= fVar5) || (fVar5 < 0.0)) {\r\n        fVar6 = (float)FUN_1411ad95c(fVar5 * 0.0027777778);\r\n        fVar5 = fVar5 - fVar6 * 360.0;\r\n      }\r\n      if (180.0 < fVar5) {\r\n        fVar5 = fVar5 - 360.0;\r\n      }\r\n      iVar3 = iVar4;\r\n      if (ABS(fVar5) < fVar9) {\r\n        fVar9 = ABS(fVar5);\r\n        iVar3 = iVar2;\r\n      }\r\n      iVar2 = iVar2 + 1;\r\n      iVar4 = iVar3;\r\n    } while (iVar2 < (int)(*(uint *)(param_1 + 0x138) & 0xffffff));\r\n  }\r\n  *(int *)(param_2 + 0x60) = iVar3;\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140729350(longlong param_1,longlong param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  int iVar2;\r\n  int iVar3;\r\n  int iVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float local_118;\r\n  float local_114;\r\n  float local_110;\r\n  float local_108;\r\n  float local_104;\r\n  float local_100;\r\n  float local_f8;\r\n  float local_f4;\r\n  float local_f0;\r\n  undefined4 local_ec;\r\n  undefined4 local_e8;\r\n  undefined4 local_e4;\r\n  undefined4 local_e0;\r\n  undefined4 local_dc;\r\n  undefined4 local_d8;\r\n  float local_d0;\r\n  float local_cc;\r\n  float local_c8;\r\n  undefined4 local_c4;\r\n  undefined4 local_c0;\r\n  undefined4 local_bc;\r\n  undefined4 local_b8;\r\n  undefined4 local_b4;\r\n  undefined4 local_b0;\r\n  \r\n  local_118 = DAT_1431af668;\r\n  local_114 = DAT_1431af66c;\r\n  local_110 = DAT_1431af670;\r\n  local_d0 = DAT_14228a780;\r\n  local_cc = DAT_14228a784;\r\n  local_c8 = DAT_14228a788;\r\n  local_c4 = DAT_14228a78c;\r\n  local_c0 = DAT_14228a790;\r\n  local_bc = DAT_14228a794;\r\n  local_b8 = DAT_14228a798;\r\n  local_b4 = DAT_14228a79c;\r\n  local_b0 = DAT_14228a7a0;\r\n  if ((*(short *)(param_2 + 0x58) != -1) && (lVar1 = FUN_1403da910(param_2 + 8,2), lVar1 != 0)) {\r\n    FUN_14053dac0(lVar1,0,param_2 + 0x3c,&local_118,&local_d0);\r\n  }\r\n  iVar4 = -1;\r\n  iVar3 = -1;\r\n  iVar2 = 0;\r\n  fVar9 = 180.0;\r\n  if ((*(uint *)(param_1 + 0x138) & 0xffffff) != 0) {\r\n    do {\r\n      local_108 = DAT_1431af668;\r\n      local_104 = DAT_1431af66c;\r\n      local_100 = DAT_1431af670;\r\n      local_f8 = DAT_14228a780;\r\n      local_f4 = DAT_14228a784;\r\n      local_f0 = DAT_14228a788;\r\n      local_ec = DAT_14228a78c;\r\n      local_e8 = DAT_14228a790;\r\n      local_e4 = DAT_14228a794;\r\n      local_e0 = DAT_14228a798;\r\n      local_dc = DAT_14228a79c;\r\n      local_d8 = DAT_14228a7a0;\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)iVar2 * 0x20 + *(longlong *)(param_1 + 0x130),&local_108,&local_f8);\r\n      fVar8 = local_118 - local_108;\r\n      fVar6 = local_114 - local_104;\r\n      fVar7 = local_110 - local_100;\r\n      fVar5 = fVar6 * fVar6 + fVar8 * fVar8 + fVar7 * fVar7;\r\n      if (fVar5 <= 1.1754944e-38) {\r\n        fVar5 = 1.1754944e-38;\r\n      }\r\n      fVar5 = 1.0 / SQRT(fVar5);\r\n      fVar5 = local_f4 * fVar6 * fVar5 + local_f8 * fVar8 * fVar5 + local_f0 * fVar7 * fVar5;\r\n      if (-1.0 < fVar5) {\r\n        if (fVar5 < 1.0) {\r\n          fVar5 = (float)FUN_1411afeb4(fVar5);\r\n        }\r\n        else {\r\n          fVar5 = 0.0;\r\n        }\r\n      }\r\n      else {\r\n        fVar5 = 3.1415927;\r\n      }\r\n      fVar5 = fVar5 * 57.295776;\r\n      if ((360.0 <= fVar5) || (fVar5 < 0.0)) {\r\n        fVar6 = (float)FUN_1411ad95c(fVar5 * 0.0027777778);\r\n        fVar5 = fVar5 - fVar6 * 360.0;\r\n      }\r\n      if (180.0 < fVar5) {\r\n        fVar5 = fVar5 - 360.0;\r\n      }\r\n      iVar3 = iVar4;\r\n      if (ABS(fVar5) < fVar9) {\r\n        fVar9 = ABS(fVar5);\r\n        iVar3 = iVar2;\r\n      }\r\n      iVar2 = iVar2 + 1;\r\n      iVar4 = iVar3;\r\n    } while (iVar2 < (int)(*(uint *)(param_1 + 0x138) & 0xffffff));\r\n  }\r\n  *(int *)(param_2 + 0x60) = iVar3;\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "14072e5c0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14072e5c0(longlong param_1,int param_2,longlong param_3,longlong param_4,float param_5)\r\n\r\n{\r\n  byte bVar1;\r\n  int iVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  undefined4 *puVar7;\r\n  undefined8 uVar8;\r\n  undefined8 uVar9;\r\n  longlong lVar10;\r\n  float fVar11;\r\n  float fVar12;\r\n  float fVar13;\r\n  float fVar14;\r\n  undefined1 local_res10 [8];\r\n  undefined4 *in_stack_fffffffffffffe08;\r\n  float local_1e8;\r\n  float local_1e4;\r\n  float local_1e0;\r\n  float local_1d8;\r\n  float local_1d4;\r\n  float local_1d0;\r\n  undefined4 local_1c8;\r\n  undefined4 local_1c4;\r\n  undefined4 local_1c0;\r\n  undefined4 local_1bc;\r\n  undefined4 local_1b8;\r\n  undefined4 local_1b4;\r\n  undefined4 local_1b0;\r\n  undefined4 local_1ac;\r\n  undefined4 local_1a8;\r\n  float local_1a0;\r\n  float local_19c;\r\n  float local_198;\r\n  undefined4 local_190;\r\n  undefined4 local_18c;\r\n  undefined4 local_188;\r\n  undefined4 local_184;\r\n  undefined4 local_180;\r\n  undefined4 local_17c;\r\n  undefined4 local_178;\r\n  undefined4 local_174;\r\n  undefined4 local_170;\r\n  undefined1 local_168 [40];\r\n  undefined1 local_140 [24];\r\n  undefined1 local_128 [84];\r\n  undefined4 local_d4;\r\n  \r\n  lVar10 = (longlong)param_2 * 0x70;\r\n  lVar5 = FUN_1406dc560(*(undefined4 *)(lVar10 + *(longlong *)(param_1 + 0x1a8)));\r\n  if (lVar5 != 0) {\r\n    local_1e8 = *(float *)(lVar5 + 0x300);\r\n    local_1e4 = *(float *)(lVar5 + 0x304);\r\n    lVar6 = *(longlong *)(param_1 + 0x10);\r\n    local_1e0 = *(float *)(lVar5 + 0x308);\r\n    local_190 = *(undefined4 *)(lVar5 + 0x30c);\r\n    local_18c = *(undefined4 *)(lVar5 + 0x310);\r\n    local_188 = *(undefined4 *)(lVar5 + 0x314);\r\n    local_184 = *(undefined4 *)(lVar5 + 0x318);\r\n    local_180 = *(undefined4 *)(lVar5 + 0x31c);\r\n    local_17c = *(undefined4 *)(lVar5 + 800);\r\n    local_178 = *(undefined4 *)(lVar5 + 0x324);\r\n    local_174 = *(undefined4 *)(lVar5 + 0x328);\r\n    local_170 = *(undefined4 *)(lVar5 + 0x32c);\r\n    local_1d8 = *(float *)(lVar6 + 0x300);\r\n    local_1d4 = *(float *)(lVar6 + 0x304);\r\n    local_1d0 = *(float *)(lVar6 + 0x308);\r\n    local_1c8 = *(undefined4 *)(lVar6 + 0x30c);\r\n    local_1c4 = *(undefined4 *)(lVar6 + 0x310);\r\n    local_1c0 = *(undefined4 *)(lVar6 + 0x314);\r\n    local_1bc = *(undefined4 *)(lVar6 + 0x318);\r\n    local_1b8 = *(undefined4 *)(lVar6 + 0x31c);\r\n    local_1b4 = *(undefined4 *)(lVar6 + 800);\r\n    local_1b0 = *(undefined4 *)(lVar6 + 0x324);\r\n    local_1ac = *(undefined4 *)(lVar6 + 0x328);\r\n    local_1a8 = *(undefined4 *)(lVar6 + 0x32c);\r\n    if ((*(short *)(lVar10 + 0x58 + *(longlong *)(param_1 + 0x1a8)) != -1) &&\r\n       (lVar6 = FUN_1403da910(*(longlong *)(param_1 + 0x1a8) + 8 + lVar10,2), lVar6 != 0)) {\r\n      in_stack_fffffffffffffe08 = &local_190;\r\n      FUN_14053dac0(lVar6,0,*(longlong *)(param_1 + 0x1a8) + 0x3c + lVar10,&local_1e8,\r\n                    in_stack_fffffffffffffe08);\r\n    }\r\n    iVar2 = *(int *)(lVar10 + 0x60 + *(longlong *)(param_1 + 0x1a8));\r\n    if (iVar2 != -1) {\r\n      in_stack_fffffffffffffe08 = &local_1c8;\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)iVar2 * 0x20 + *(longlong *)(param_1 + 0x130),&local_1d8,\r\n                    in_stack_fffffffffffffe08);\r\n    }\r\n    *(float *)(lVar10 + 100 + *(longlong *)(param_1 + 0x1a8)) =\r\n         param_5 + *(float *)(lVar10 + 100 + *(longlong *)(param_1 + 0x1a8));\r\n    lVar6 = *(longlong *)(param_1 + 0x1a8);\r\n    lVar4 = *(longlong *)(param_1 + 8);\r\n    fVar11 = *(float *)(lVar10 + 100 + lVar6) / *(float *)(lVar4 + 0x160);\r\n    fVar12 = 1.0;\r\n    if (fVar11 <= 1.0) {\r\n      fVar12 = fVar11;\r\n    }\r\n    if (*(char *)(lVar10 + 0x6c + lVar6) == '\\0') {\r\n      *(float *)(lVar10 + 0x68 + lVar6) = fVar12;\r\n    }\r\n    else {\r\n      *(float *)(lVar10 + 0x68 + lVar6) = 1.0 - fVar12;\r\n    }\r\n    fVar14 = local_1d8 - local_1e8;\r\n    fVar13 = local_1d4 - local_1e4;\r\n    fVar11 = local_1d0 - local_1e0;\r\n    fVar12 = fVar13 * fVar13 + fVar14 * fVar14 + fVar11 * fVar11;\r\n    if (fVar12 <= 1.1754944e-38) {\r\n      fVar12 = 1.1754944e-38;\r\n    }\r\n    local_198 = 1.0 / SQRT(fVar12);\r\n    local_1a0 = fVar14 * local_198;\r\n    local_19c = fVar13 * local_198;\r\n    local_198 = fVar11 * local_198;\r\n    fVar12 = (1.0 - *(float *)(lVar10 + 0x68 + *(longlong *)(param_1 + 0x1a8))) *\r\n             SQRT(fVar13 * fVar13 + fVar14 * fVar14 + fVar11 * fVar11);\r\n    local_1e8 = local_1a0 * fVar12 + local_1e8;\r\n    local_1e4 = local_19c * fVar12 + local_1e4;\r\n    local_1e0 = local_198 * fVar12 + local_1e0;\r\n    *(float *)(param_3 + 0x38) = local_1e8;\r\n    *(float *)(param_3 + 0x3c) = local_1e4;\r\n    *(float *)(param_3 + 0x40) = local_1e0;\r\n    puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);\r\n    *(undefined4 *)(param_3 + 0x44) = *puVar7;\r\n    *(undefined4 *)(param_3 + 0x48) = puVar7[1];\r\n    *(undefined4 *)(param_3 + 0x4c) = puVar7[2];\r\n    *(undefined4 *)(param_3 + 0x50) = puVar7[3];\r\n    *(undefined4 *)(param_3 + 0x54) = puVar7[4];\r\n    *(undefined4 *)(param_3 + 0x58) = puVar7[5];\r\n    *(undefined4 *)(param_3 + 0x5c) = puVar7[6];\r\n    *(undefined4 *)(param_3 + 0x60) = puVar7[7];\r\n    *(undefined4 *)(param_3 + 100) = puVar7[8];\r\n    *(float *)(param_3 + 0x80) = local_1d8;\r\n    *(float *)(param_3 + 0x84) = local_1d4;\r\n    *(float *)(param_3 + 0x88) = local_1d0;\r\n    *(undefined4 *)(param_3 + 0x90) = local_1c4;\r\n    *(undefined4 *)(param_3 + 0x8c) = local_1c8;\r\n    *(undefined4 *)(param_3 + 0x94) = local_1c0;\r\n    *(undefined4 *)(param_3 + 0x98) = local_1bc;\r\n    *(undefined4 *)(param_3 + 0x9c) = local_1b8;\r\n    *(undefined4 *)(param_3 + 0xa0) = local_1b4;\r\n    *(undefined4 *)(param_3 + 0xa4) = local_1b0;\r\n    *(undefined4 *)(param_3 + 0xa8) = local_1ac;\r\n    *(undefined4 *)(param_3 + 0xac) = local_1a8;\r\n    if (param_4 != 0) {\r\n      *(float *)(param_4 + 0x38) = local_1e8;\r\n      *(float *)(param_4 + 0x3c) = local_1e4;\r\n      *(float *)(param_4 + 0x40) = local_1e0;\r\n      puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);\r\n      *(undefined4 *)(param_4 + 0x44) = *puVar7;\r\n      *(undefined4 *)(param_4 + 0x48) = puVar7[1];\r\n      *(undefined4 *)(param_4 + 0x4c) = puVar7[2];\r\n      *(undefined4 *)(param_4 + 0x50) = puVar7[3];\r\n      *(undefined4 *)(param_4 + 0x54) = puVar7[4];\r\n      *(undefined4 *)(param_4 + 0x58) = puVar7[5];\r\n      *(undefined4 *)(param_4 + 0x5c) = puVar7[6];\r\n      *(undefined4 *)(param_4 + 0x60) = puVar7[7];\r\n      *(undefined4 *)(param_4 + 100) = puVar7[8];\r\n      *(float *)(param_4 + 0x80) = local_1d8;\r\n      *(float *)(param_4 + 0x84) = local_1d4;\r\n      *(float *)(param_4 + 0x88) = local_1d0;\r\n      *(undefined4 *)(param_4 + 0x8c) = local_1c8;\r\n      *(undefined4 *)(param_4 + 0x90) = local_1c4;\r\n      *(undefined4 *)(param_4 + 0x94) = local_1c0;\r\n      *(undefined4 *)(param_4 + 0x98) = local_1bc;\r\n      *(undefined4 *)(param_4 + 0x9c) = local_1b8;\r\n      *(undefined4 *)(param_4 + 0xa0) = local_1b4;\r\n      *(undefined4 *)(param_4 + 0xa4) = local_1b0;\r\n      *(undefined4 *)(param_4 + 0xa8) = local_1ac;\r\n      *(undefined4 *)(param_4 + 0xac) = local_1a8;\r\n    }\r\n    uVar8 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    lVar6 = FUN_1401a04a0(uVar8,*(undefined4 *)\r\n                                 (*(longlong *)(param_1 + 0x140) + (longlong)iVar2 * 8));\r\n    if (lVar6 != 0) {\r\n      *(float *)(lVar6 + 0x38) = local_1d8;\r\n      *(float *)(lVar6 + 0x3c) = local_1d4;\r\n      *(float *)(lVar6 + 0x40) = local_1d0;\r\n      *(undefined4 *)(lVar6 + 0x44) = local_1c8;\r\n      *(undefined4 *)(lVar6 + 0x48) = local_1c4;\r\n      *(undefined4 *)(lVar6 + 0x4c) = local_1c0;\r\n      *(undefined4 *)(lVar6 + 0x50) = local_1bc;\r\n      *(undefined4 *)(lVar6 + 0x54) = local_1b8;\r\n      *(undefined4 *)(lVar6 + 0x58) = local_1b4;\r\n      *(undefined4 *)(lVar6 + 0x5c) = local_1b0;\r\n      *(undefined4 *)(lVar6 + 0x60) = local_1ac;\r\n      *(undefined4 *)(lVar6 + 100) = local_1a8;\r\n    }\r\n    lVar6 = *(longlong *)(param_1 + 0x1a8);\r\n    if ((1.0 <= *(float *)(lVar10 + 0x68 + lVar6)) && (*(char *)(lVar10 + 0x6c + lVar6) == '\\0')) {\r\n      if ((*(int *)(lVar10 + 0x38 + lVar6) == -1) && (*(longlong *)(lVar4 + 0x150) != 0)) {\r\n        lVar6 = FUN_14019d100(uVar8,*(undefined4 *)(lVar5 + 0xa8));\r\n        if (lVar6 != 0) {\r\n          *(byte *)(lVar6 + 0x138) = *(byte *)(lVar6 + 0x138) & 0xfb;\r\n          FUN_1401a2ff0(uVar8,lVar6,0,0x1ffff,\r\n                        (ulonglong)in_stack_fffffffffffffe08 & 0xffffffff00000000);\r\n          lVar4 = *(longlong *)(param_1 + 0x1a8);\r\n          puVar7 = (undefined4 *)FUN_140167a00(lVar6,local_res10);\r\n          *(undefined4 *)(lVar10 + 0x38 + lVar4) = *puVar7;\r\n        }\r\n        bVar1 = *(byte *)(lVar5 + 0x368);\r\n        if (*(int *)(param_1 + 0x2ac) < 0) {\r\n          fVar12 = 0.0;\r\n        }\r\n        else {\r\n          fVar12 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                             (longlong)*(int *)(param_1 + 0x2ac) * 0x28);\r\n        }\r\n        fVar11 = (float)FUN_1408ac040(0,0x42c80000);\r\n        if (((~(bVar1 >> 7) & 1) == 0) || (fVar12 <= fVar11)) {\r\n          uVar3 = *(undefined4 *)(lVar5 + 0xa8);\r\n          uVar9 = FUN_140810240();\r\n          FUN_1408103f0(uVar9,0x102,uVar3);\r\n        }\r\n        else {\r\n          D2_DamageParams_Construct(local_128,DAT_14231f350);\r\n          lVar6 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n          if (lVar6 == 0) {\r\n            local_d4 = 0x3fff0010;\r\n          }\r\n          else {\r\n            local_d4 = *(undefined4 *)(lVar6 + 0xa8);\r\n          }\r\n          uVar9 = FUN_140882e20(local_168,local_128);\r\n          FUN_140884920(lVar5,local_140,&DAT_1439a9d20,uVar9);\r\n          FUN_140882fc0(local_140);\r\n        }\r\n      }\r\n      if (param_4 != 0) {\r\n        FUN_1401a31a0(uVar8,param_4);\r\n        *(undefined4 *)(lVar10 + 0x34 + *(longlong *)(param_1 + 0x1a8)) = 0xffffffff;\r\n      }\r\n    }\r\n    lVar5 = FUN_1401a04a0(uVar8,*(undefined4 *)(lVar10 + 0x38 + *(longlong *)(param_1 + 0x1a8)));\r\n    if (lVar5 != 0) {\r\n      *(float *)(lVar5 + 0x38) = local_1e8;\r\n      *(float *)(lVar5 + 0x3c) = local_1e4;\r\n      *(float *)(lVar5 + 0x40) = local_1e0;\r\n      puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);\r\n      *(undefined4 *)(lVar5 + 0x44) = *puVar7;\r\n      *(undefined4 *)(lVar5 + 0x48) = puVar7[1];\r\n      *(undefined4 *)(lVar5 + 0x4c) = puVar7[2];\r\n      *(undefined4 *)(lVar5 + 0x50) = puVar7[3];\r\n      *(undefined4 *)(lVar5 + 0x54) = puVar7[4];\r\n      *(undefined4 *)(lVar5 + 0x58) = puVar7[5];\r\n      *(undefined4 *)(lVar5 + 0x5c) = puVar7[6];\r\n      *(undefined4 *)(lVar5 + 0x60) = puVar7[7];\r\n      *(undefined4 *)(lVar5 + 100) = puVar7[8];\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14072e5c0(longlong param_1,int param_2,longlong param_3,longlong param_4,float param_5)\r\n\r\n{\r\n  byte bVar1;\r\n  int iVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  undefined4 *puVar7;\r\n  undefined8 uVar8;\r\n  undefined8 uVar9;\r\n  longlong lVar10;\r\n  float fVar11;\r\n  float fVar12;\r\n  float fVar13;\r\n  float fVar14;\r\n  undefined1 local_res10 [8];\r\n  undefined4 *in_stack_fffffffffffffe08;\r\n  float local_1e8;\r\n  float local_1e4;\r\n  float local_1e0;\r\n  float local_1d8;\r\n  float local_1d4;\r\n  float local_1d0;\r\n  undefined4 local_1c8;\r\n  undefined4 local_1c4;\r\n  undefined4 local_1c0;\r\n  undefined4 local_1bc;\r\n  undefined4 local_1b8;\r\n  undefined4 local_1b4;\r\n  undefined4 local_1b0;\r\n  undefined4 local_1ac;\r\n  undefined4 local_1a8;\r\n  float local_1a0;\r\n  float local_19c;\r\n  float local_198;\r\n  undefined4 local_190;\r\n  undefined4 local_18c;\r\n  undefined4 local_188;\r\n  undefined4 local_184;\r\n  undefined4 local_180;\r\n  undefined4 local_17c;\r\n  undefined4 local_178;\r\n  undefined4 local_174;\r\n  undefined4 local_170;\r\n  undefined1 local_168 [40];\r\n  undefined1 local_140 [24];\r\n  undefined1 local_128 [84];\r\n  undefined4 local_d4;\r\n  \r\n  lVar10 = (longlong)param_2 * 0x70;\r\n  lVar5 = FUN_1406dc560(*(undefined4 *)(lVar10 + *(longlong *)(param_1 + 0x1a8)));\r\n  if (lVar5 != 0) {\r\n    local_1e8 = *(float *)(lVar5 + 0x300);\r\n    local_1e4 = *(float *)(lVar5 + 0x304);\r\n    lVar6 = *(longlong *)(param_1 + 0x10);\r\n    local_1e0 = *(float *)(lVar5 + 0x308);\r\n    local_190 = *(undefined4 *)(lVar5 + 0x30c);\r\n    local_18c = *(undefined4 *)(lVar5 + 0x310);\r\n    local_188 = *(undefined4 *)(lVar5 + 0x314);\r\n    local_184 = *(undefined4 *)(lVar5 + 0x318);\r\n    local_180 = *(undefined4 *)(lVar5 + 0x31c);\r\n    local_17c = *(undefined4 *)(lVar5 + 800);\r\n    local_178 = *(undefined4 *)(lVar5 + 0x324);\r\n    local_174 = *(undefined4 *)(lVar5 + 0x328);\r\n    local_170 = *(undefined4 *)(lVar5 + 0x32c);\r\n    local_1d8 = *(float *)(lVar6 + 0x300);\r\n    local_1d4 = *(float *)(lVar6 + 0x304);\r\n    local_1d0 = *(float *)(lVar6 + 0x308);\r\n    local_1c8 = *(undefined4 *)(lVar6 + 0x30c);\r\n    local_1c4 = *(undefined4 *)(lVar6 + 0x310);\r\n    local_1c0 = *(undefined4 *)(lVar6 + 0x314);\r\n    local_1bc = *(undefined4 *)(lVar6 + 0x318);\r\n    local_1b8 = *(undefined4 *)(lVar6 + 0x31c);\r\n    local_1b4 = *(undefined4 *)(lVar6 + 800);\r\n    local_1b0 = *(undefined4 *)(lVar6 + 0x324);\r\n    local_1ac = *(undefined4 *)(lVar6 + 0x328);\r\n    local_1a8 = *(undefined4 *)(lVar6 + 0x32c);\r\n    if ((*(short *)(lVar10 + 0x58 + *(longlong *)(param_1 + 0x1a8)) != -1) &&\r\n       (lVar6 = FUN_1403da910(*(longlong *)(param_1 + 0x1a8) + 8 + lVar10,2), lVar6 != 0)) {\r\n      in_stack_fffffffffffffe08 = &local_190;\r\n      FUN_14053dac0(lVar6,0,*(longlong *)(param_1 + 0x1a8) + 0x3c + lVar10,&local_1e8,\r\n                    in_stack_fffffffffffffe08);\r\n    }\r\n    iVar2 = *(int *)(lVar10 + 0x60 + *(longlong *)(param_1 + 0x1a8));\r\n    if (iVar2 != -1) {\r\n      in_stack_fffffffffffffe08 = &local_1c8;\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)iVar2 * 0x20 + *(longlong *)(param_1 + 0x130),&local_1d8,\r\n                    in_stack_fffffffffffffe08);\r\n    }\r\n    *(float *)(lVar10 + 100 + *(longlong *)(param_1 + 0x1a8)) =\r\n         param_5 + *(float *)(lVar10 + 100 + *(longlong *)(param_1 + 0x1a8));\r\n    lVar6 = *(longlong *)(param_1 + 0x1a8);\r\n    lVar4 = *(longlong *)(param_1 + 8);\r\n    fVar11 = *(float *)(lVar10 + 100 + lVar6) / *(float *)(lVar4 + 0x160);\r\n    fVar12 = 1.0;\r\n    if (fVar11 <= 1.0) {\r\n      fVar12 = fVar11;\r\n    }\r\n    if (*(char *)(lVar10 + 0x6c + lVar6) == '\\0') {\r\n      *(float *)(lVar10 + 0x68 + lVar6) = fVar12;\r\n    }\r\n    else {\r\n      *(float *)(lVar10 + 0x68 + lVar6) = 1.0 - fVar12;\r\n    }\r\n    fVar14 = local_1d8 - local_1e8;\r\n    fVar13 = local_1d4 - local_1e4;\r\n    fVar11 = local_1d0 - local_1e0;\r\n    fVar12 = fVar13 * fVar13 + fVar14 * fVar14 + fVar11 * fVar11;\r\n    if (fVar12 <= 1.1754944e-38) {\r\n      fVar12 = 1.1754944e-38;\r\n    }\r\n    local_198 = 1.0 / SQRT(fVar12);\r\n    local_1a0 = fVar14 * local_198;\r\n    local_19c = fVar13 * local_198;\r\n    local_198 = fVar11 * local_198;\r\n    fVar12 = (1.0 - *(float *)(lVar10 + 0x68 + *(longlong *)(param_1 + 0x1a8))) *\r\n             SQRT(fVar13 * fVar13 + fVar14 * fVar14 + fVar11 * fVar11);\r\n    local_1e8 = local_1a0 * fVar12 + local_1e8;\r\n    local_1e4 = local_19c * fVar12 + local_1e4;\r\n    local_1e0 = local_198 * fVar12 + local_1e0;\r\n    *(float *)(param_3 + 0x38) = local_1e8;\r\n    *(float *)(param_3 + 0x3c) = local_1e4;\r\n    *(float *)(param_3 + 0x40) = local_1e0;\r\n    puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);\r\n    *(undefined4 *)(param_3 + 0x44) = *puVar7;\r\n    *(undefined4 *)(param_3 + 0x48) = puVar7[1];\r\n    *(undefined4 *)(param_3 + 0x4c) = puVar7[2];\r\n    *(undefined4 *)(param_3 + 0x50) = puVar7[3];\r\n    *(undefined4 *)(param_3 + 0x54) = puVar7[4];\r\n    *(undefined4 *)(param_3 + 0x58) = puVar7[5];\r\n    *(undefined4 *)(param_3 + 0x5c) = puVar7[6];\r\n    *(undefined4 *)(param_3 + 0x60) = puVar7[7];\r\n    *(undefined4 *)(param_3 + 100) = puVar7[8];\r\n    *(float *)(param_3 + 0x80) = local_1d8;\r\n    *(float *)(param_3 + 0x84) = local_1d4;\r\n    *(float *)(param_3 + 0x88) = local_1d0;\r\n    *(undefined4 *)(param_3 + 0x90) = local_1c4;\r\n    *(undefined4 *)(param_3 + 0x8c) = local_1c8;\r\n    *(undefined4 *)(param_3 + 0x94) = local_1c0;\r\n    *(undefined4 *)(param_3 + 0x98) = local_1bc;\r\n    *(undefined4 *)(param_3 + 0x9c) = local_1b8;\r\n    *(undefined4 *)(param_3 + 0xa0) = local_1b4;\r\n    *(undefined4 *)(param_3 + 0xa4) = local_1b0;\r\n    *(undefined4 *)(param_3 + 0xa8) = local_1ac;\r\n    *(undefined4 *)(param_3 + 0xac) = local_1a8;\r\n    if (param_4 != 0) {\r\n      *(float *)(param_4 + 0x38) = local_1e8;\r\n      *(float *)(param_4 + 0x3c) = local_1e4;\r\n      *(float *)(param_4 + 0x40) = local_1e0;\r\n      puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);\r\n      *(undefined4 *)(param_4 + 0x44) = *puVar7;\r\n      *(undefined4 *)(param_4 + 0x48) = puVar7[1];\r\n      *(undefined4 *)(param_4 + 0x4c) = puVar7[2];\r\n      *(undefined4 *)(param_4 + 0x50) = puVar7[3];\r\n      *(undefined4 *)(param_4 + 0x54) = puVar7[4];\r\n      *(undefined4 *)(param_4 + 0x58) = puVar7[5];\r\n      *(undefined4 *)(param_4 + 0x5c) = puVar7[6];\r\n      *(undefined4 *)(param_4 + 0x60) = puVar7[7];\r\n      *(undefined4 *)(param_4 + 100) = puVar7[8];\r\n      *(float *)(param_4 + 0x80) = local_1d8;\r\n      *(float *)(param_4 + 0x84) = local_1d4;\r\n      *(float *)(param_4 + 0x88) = local_1d0;\r\n      *(undefined4 *)(param_4 + 0x8c) = local_1c8;\r\n      *(undefined4 *)(param_4 + 0x90) = local_1c4;\r\n      *(undefined4 *)(param_4 + 0x94) = local_1c0;\r\n      *(undefined4 *)(param_4 + 0x98) = local_1bc;\r\n      *(undefined4 *)(param_4 + 0x9c) = local_1b8;\r\n      *(undefined4 *)(param_4 + 0xa0) = local_1b4;\r\n      *(undefined4 *)(param_4 + 0xa4) = local_1b0;\r\n      *(undefined4 *)(param_4 + 0xa8) = local_1ac;\r\n      *(undefined4 *)(param_4 + 0xac) = local_1a8;\r\n    }\r\n    uVar8 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n    lVar6 = FUN_1401a04a0(uVar8,*(undefined4 *)\r\n                                 (*(longlong *)(param_1 + 0x140) + (longlong)iVar2 * 8));\r\n    if (lVar6 != 0) {\r\n      *(float *)(lVar6 + 0x38) = local_1d8;\r\n      *(float *)(lVar6 + 0x3c) = local_1d4;\r\n      *(float *)(lVar6 + 0x40) = local_1d0;\r\n      *(undefined4 *)(lVar6 + 0x44) = local_1c8;\r\n      *(undefined4 *)(lVar6 + 0x48) = local_1c4;\r\n      *(undefined4 *)(lVar6 + 0x4c) = local_1c0;\r\n      *(undefined4 *)(lVar6 + 0x50) = local_1bc;\r\n      *(undefined4 *)(lVar6 + 0x54) = local_1b8;\r\n      *(undefined4 *)(lVar6 + 0x58) = local_1b4;\r\n      *(undefined4 *)(lVar6 + 0x5c) = local_1b0;\r\n      *(undefined4 *)(lVar6 + 0x60) = local_1ac;\r\n      *(undefined4 *)(lVar6 + 100) = local_1a8;\r\n    }\r\n    lVar6 = *(longlong *)(param_1 + 0x1a8);\r\n    if ((1.0 <= *(float *)(lVar10 + 0x68 + lVar6)) && (*(char *)(lVar10 + 0x6c + lVar6) == '\\0')) {\r\n      if ((*(int *)(lVar10 + 0x38 + lVar6) == -1) && (*(longlong *)(lVar4 + 0x150) != 0)) {\r\n        lVar6 = FUN_14019d100(uVar8,*(undefined4 *)(lVar5 + 0xa8));\r\n        if (lVar6 != 0) {\r\n          *(byte *)(lVar6 + 0x138) = *(byte *)(lVar6 + 0x138) & 0xfb;\r\n          FUN_1401a2ff0(uVar8,lVar6,0,0x1ffff,\r\n                        (ulonglong)in_stack_fffffffffffffe08 & 0xffffffff00000000);\r\n          lVar4 = *(longlong *)(param_1 + 0x1a8);\r\n          puVar7 = (undefined4 *)FUN_140167a00(lVar6,local_res10);\r\n          *(undefined4 *)(lVar10 + 0x38 + lVar4) = *puVar7;\r\n        }\r\n        bVar1 = *(byte *)(lVar5 + 0x368);\r\n        if (*(int *)(param_1 + 0x2ac) < 0) {\r\n          fVar12 = 0.0;\r\n        }\r\n        else {\r\n          fVar12 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                             (longlong)*(int *)(param_1 + 0x2ac) * 0x28);\r\n        }\r\n        fVar11 = (float)FUN_1408ac040(0,0x42c80000);\r\n        if (((~(bVar1 >> 7) & 1) == 0) || (fVar12 <= fVar11)) {\r\n          uVar3 = *(undefined4 *)(lVar5 + 0xa8);\r\n          uVar9 = FUN_140810240();\r\n          FUN_1408103f0(uVar9,0x102,uVar3);\r\n        }\r\n        else {\r\n          D2_DamageParams_Construct(local_128,DAT_14231f350);\r\n          lVar6 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n          if (lVar6 == 0) {\r\n            local_d4 = 0x3fff0010;\r\n          }\r\n          else {\r\n            local_d4 = *(undefined4 *)(lVar6 + 0xa8);\r\n          }\r\n          uVar9 = FUN_140882e20(local_168,local_128);\r\n          FUN_140884920(lVar5,local_140,&DAT_1439a9d20,uVar9);\r\n          FUN_140882fc0(local_140);\r\n        }\r\n      }\r\n      if (param_4 != 0) {\r\n        FUN_1401a31a0(uVar8,param_4);\r\n        *(undefined4 *)(lVar10 + 0x34 + *(longlong *)(param_1 + 0x1a8)) = 0xffffffff;\r\n      }\r\n    }\r\n    lVar5 = FUN_1401a04a0(uVar8,*(undefined4 *)(lVar10 + 0x38 + *(longlong *)(param_1 + 0x1a8)));\r\n    if (lVar5 != 0) {\r\n      *(float *)(lVar5 + 0x38) = local_1e8;\r\n      *(float *)(lVar5 + 0x3c) = local_1e4;\r\n      *(float *)(lVar5 + 0x40) = local_1e0;\r\n      puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);\r\n      *(undefined4 *)(lVar5 + 0x44) = *puVar7;\r\n      *(undefined4 *)(lVar5 + 0x48) = puVar7[1];\r\n      *(undefined4 *)(lVar5 + 0x4c) = puVar7[2];\r\n      *(undefined4 *)(lVar5 + 0x50) = puVar7[3];\r\n      *(undefined4 *)(lVar5 + 0x54) = puVar7[4];\r\n      *(undefined4 *)(lVar5 + 0x58) = puVar7[5];\r\n      *(undefined4 *)(lVar5 + 0x5c) = puVar7[6];\r\n      *(undefined4 *)(lVar5 + 0x60) = puVar7[7];\r\n      *(undefined4 *)(lVar5 + 100) = puVar7[8];\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "14072a280",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14072a280(longlong param_1,longlong param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  undefined8 uVar4;\r\n  longlong lVar5;\r\n  int iVar6;\r\n  char cVar7;\r\n  uint uVar8;\r\n  undefined4 *puVar9;\r\n  longlong lStack_48;\r\n  undefined1 *puStack_40;\r\n  undefined8 auStack_30 [3];\r\n  \r\n  lVar5 = *(longlong *)(param_1 + 8);\r\n  uVar1 = FUN_140728720(param_1 + 0x1a8);\r\n  puVar9 = (undefined4 *)\r\n           ((ulonglong)((*(uint *)(param_1 + 0x1b0) & 0xffffff) - 1) * 0x70 +\r\n           *(longlong *)(param_1 + 0x1a8));\r\n  lVar3 = FUN_1406dc560(*puVar9);\r\n  if (((*(byte *)(lVar3 + 0x368) & 0x80) == 0) && (*(longlong *)(lVar5 + 0x88) != 0)) {\r\n    uVar4 = FUN_1403da910(puVar9 + 6,2);\r\n    FUN_1405888c0(uVar4,*(undefined8 *)(lVar5 + 0x88));\r\n  }\r\n  if (*(char *)(param_2 + 0x6d) != '\\0') {\r\n    FUN_140729350(param_1,param_2);\r\n    FUN_14072c8b0(param_1,uVar1);\r\n  }\r\n  if ((*(byte *)(lVar3 + 0x368) & 0x80) == 0) {\r\n    lStack_48 = CONCAT71(lStack_48._1_7_,0x69);\r\n    puStack_40 = (undefined1 *)CONCAT44(puStack_40._4_4_,1);\r\n    FUN_140884920(lVar3,auStack_30,&DAT_1439ac480,&lStack_48);\r\n    FUN_140882fc0(auStack_30);\r\n    puStack_40 = &LAB_14072f8b0;\r\n    lStack_48 = param_1;\r\n    uVar4 = FUN_140810240();\r\n    FUN_140810810(uVar4,lVar3,0xdd,&lStack_48);\r\n    puStack_40 = &LAB_14072f8c0;\r\n    lStack_48 = param_1;\r\n    uVar4 = FUN_140810240();\r\n    FUN_140810810(uVar4,lVar3,0xdb,&lStack_48);\r\n  }\r\n  auStack_30[0] = 0x14072f2a2;\r\n  uVar4 = FUN_1405bef90();\r\n  cVar7 = '\\0';\r\n  iVar6 = 0;\r\n  uVar8 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n  if (uVar8 != 0) {\r\n    do {\r\n      auStack_30[0] = 0x14072f2d5;\r\n      lVar5 = FUN_1406dc560(*(undefined4 *)((longlong)iVar6 * 0x70 + *(longlong *)(param_1 + 0x1a8))\r\n                           );\r\n      if (lVar5 != 0) {\r\n        auStack_30[0] = 0x14072f2f3;\r\n        iVar2 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,uVar4);\r\n        if (iVar2 == 0) {\r\n          cVar7 = '\\x01';\r\n          break;\r\n        }\r\n      }\r\n      iVar6 = iVar6 + 1;\r\n    } while (iVar6 < (int)uVar8);\r\n  }\r\n  if (cVar7 != *(char *)(param_1 + 0x2b1)) {\r\n    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    auStack_30[0] = 0x14072f31a;\r\n    lVar5 = FUN_140810240();\r\n    uVar4 = 0x17b;\r\n    if (cVar7 == '\\0') {\r\n      uVar4 = 0x17c;\r\n    }\r\n    auStack_30[0] = 0x14072f33e;\r\n    puVar9 = (undefined4 *)FUN_1408112c0(lVar5 + 0xc8e8,uVar4,4,uVar1);\r\n    if (puVar9 != (undefined4 *)0x0) {\r\n      *puVar9 = 3;\r\n    }\r\n    *(char *)(param_1 + 0x2b1) = cVar7;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14072a280(longlong param_1,longlong param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  undefined8 uVar4;\r\n  longlong lVar5;\r\n  int iVar6;\r\n  char cVar7;\r\n  uint uVar8;\r\n  undefined4 *puVar9;\r\n  longlong lStack_48;\r\n  undefined1 *puStack_40;\r\n  undefined8 auStack_30 [3];\r\n  \r\n  lVar5 = *(longlong *)(param_1 + 8);\r\n  uVar1 = FUN_140728720(param_1 + 0x1a8);\r\n  puVar9 = (undefined4 *)\r\n           ((ulonglong)((*(uint *)(param_1 + 0x1b0) & 0xffffff) - 1) * 0x70 +\r\n           *(longlong *)(param_1 + 0x1a8));\r\n  lVar3 = FUN_1406dc560(*puVar9);\r\n  if (((*(byte *)(lVar3 + 0x368) & 0x80) == 0) && (*(longlong *)(lVar5 + 0x88) != 0)) {\r\n    uVar4 = FUN_1403da910(puVar9 + 6,2);\r\n    FUN_1405888c0(uVar4,*(undefined8 *)(lVar5 + 0x88));\r\n  }\r\n  if (*(char *)(param_2 + 0x6d) != '\\0') {\r\n    FUN_140729350(param_1,param_2);\r\n    FUN_14072c8b0(param_1,uVar1);\r\n  }\r\n  if ((*(byte *)(lVar3 + 0x368) & 0x80) == 0) {\r\n    lStack_48 = CONCAT71(lStack_48._1_7_,0x69);\r\n    puStack_40 = (undefined1 *)CONCAT44(puStack_40._4_4_,1);\r\n    FUN_140884920(lVar3,auStack_30,&DAT_1439ac480,&lStack_48);\r\n    FUN_140882fc0(auStack_30);\r\n    puStack_40 = &LAB_14072f8b0;\r\n    lStack_48 = param_1;\r\n    uVar4 = FUN_140810240();\r\n    FUN_140810810(uVar4,lVar3,0xdd,&lStack_48);\r\n    puStack_40 = &LAB_14072f8c0;\r\n    lStack_48 = param_1;\r\n    uVar4 = FUN_140810240();\r\n    FUN_140810810(uVar4,lVar3,0xdb,&lStack_48);\r\n  }\r\n  auStack_30[0] = 0x14072f2a2;\r\n  uVar4 = FUN_1405bef90();\r\n  cVar7 = '\\0';\r\n  iVar6 = 0;\r\n  uVar8 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n  if (uVar8 != 0) {\r\n    do {\r\n      auStack_30[0] = 0x14072f2d5;\r\n      lVar5 = FUN_1406dc560(*(undefined4 *)((longlong)iVar6 * 0x70 + *(longlong *)(param_1 + 0x1a8))\r\n                           );\r\n      if (lVar5 != 0) {\r\n        auStack_30[0] = 0x14072f2f3;\r\n        iVar2 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,uVar4);\r\n        if (iVar2 == 0) {\r\n          cVar7 = '\\x01';\r\n          break;\r\n        }\r\n      }\r\n      iVar6 = iVar6 + 1;\r\n    } while (iVar6 < (int)uVar8);\r\n  }\r\n  if (cVar7 != *(char *)(param_1 + 0x2b1)) {\r\n    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n    auStack_30[0] = 0x14072f31a;\r\n    lVar5 = FUN_140810240();\r\n    uVar4 = 0x17b;\r\n    if (cVar7 == '\\0') {\r\n      uVar4 = 0x17c;\r\n    }\r\n    auStack_30[0] = 0x14072f33e;\r\n    puVar9 = (undefined4 *)FUN_1408112c0(lVar5 + 0xc8e8,uVar4,4,uVar1);\r\n    if (puVar9 != (undefined4 *)0x0) {\r\n      *puVar9 = 3;\r\n    }\r\n    *(char *)(param_1 + 0x2b1) = cVar7;\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "14072c8b0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14072c8b0(longlong param_1,int param_2)\r\n\r\n{\r\n  int *piVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  undefined8 uVar4;\r\n  longlong lVar5;\r\n  undefined4 *puVar6;\r\n  longlong lVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  undefined1 local_res8 [8];\r\n  int local_res10;\r\n  longlong local_res18;\r\n  undefined4 *in_stack_ffffffffffffff68;\r\n  ulonglong uVar10;\r\n  undefined4 local_88;\r\n  undefined4 local_84;\r\n  undefined4 local_80;\r\n  undefined4 local_78;\r\n  undefined4 local_74;\r\n  undefined4 local_70;\r\n  undefined4 local_6c;\r\n  undefined4 local_68;\r\n  undefined4 local_64;\r\n  undefined4 local_60;\r\n  undefined4 local_5c;\r\n  undefined4 local_58;\r\n  \r\n  lVar7 = *(longlong *)(param_1 + 8);\r\n  lVar3 = *(longlong *)(param_1 + 0x10);\r\n  local_res10 = param_2;\r\n  uVar4 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n  lVar9 = (longlong)param_2 * 0x70;\r\n  local_88 = DAT_1431af668;\r\n  local_84 = DAT_1431af66c;\r\n  local_80 = DAT_1431af670;\r\n  local_78 = DAT_14228a780;\r\n  local_74 = DAT_14228a784;\r\n  local_70 = DAT_14228a788;\r\n  local_6c = DAT_14228a78c;\r\n  local_68 = DAT_14228a790;\r\n  local_64 = DAT_14228a794;\r\n  local_60 = DAT_14228a798;\r\n  local_5c = DAT_14228a79c;\r\n  local_58 = DAT_14228a7a0;\r\n  iVar2 = *(int *)(*(longlong *)(param_1 + 0x1a8) + 0x60 + lVar9);\r\n  lVar8 = (longlong)iVar2;\r\n  local_res18 = lVar9;\r\n  if (iVar2 != -1) {\r\n    in_stack_ffffffffffffff68 = &local_78;\r\n    FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,lVar8 * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                  &local_88,in_stack_ffffffffffffff68);\r\n  }\r\n  if ((*(longlong *)(lVar7 + 0x130) != 0) &&\r\n     (lVar5 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar5 != 0)) {\r\n    *(undefined4 *)(lVar5 + 0x38) = local_88;\r\n    *(undefined4 *)(lVar5 + 0x3c) = local_84;\r\n    in_stack_ffffffffffffff68 =\r\n         (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);\r\n    *(undefined4 *)(lVar5 + 0x40) = local_80;\r\n    *(undefined4 *)(lVar5 + 0x44) = local_78;\r\n    *(undefined4 *)(lVar5 + 0x48) = local_74;\r\n    *(undefined4 *)(lVar5 + 0x4c) = local_70;\r\n    *(undefined4 *)(lVar5 + 0x50) = local_6c;\r\n    *(undefined4 *)(lVar5 + 0x54) = local_68;\r\n    *(undefined4 *)(lVar5 + 0x58) = local_64;\r\n    *(undefined4 *)(lVar5 + 0x5c) = local_60;\r\n    *(undefined4 *)(lVar5 + 0x60) = local_5c;\r\n    *(undefined4 *)(lVar5 + 100) = local_58;\r\n    *(byte *)(lVar5 + 0x138) = *(byte *)(lVar5 + 0x138) & 0xfb;\r\n    FUN_1401a2ff0(uVar4,lVar5,0,0x1ffff,in_stack_ffffffffffffff68);\r\n  }\r\n  if ((*(longlong *)(lVar7 + 0x128) != 0) && (iVar2 != -1)) {\r\n    if ((*(int *)(*(longlong *)(param_1 + 0x140) + lVar8 * 8) == -1) &&\r\n       (lVar5 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar9 = local_res18, lVar5 != 0)\r\n       ) {\r\n      *(undefined4 *)(lVar5 + 0x38) = local_88;\r\n      *(undefined4 *)(lVar5 + 0x3c) = local_84;\r\n      in_stack_ffffffffffffff68 =\r\n           (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);\r\n      *(undefined4 *)(lVar5 + 0x40) = local_80;\r\n      *(undefined4 *)(lVar5 + 0x44) = local_78;\r\n      *(undefined4 *)(lVar5 + 0x48) = local_74;\r\n      *(undefined4 *)(lVar5 + 0x4c) = local_70;\r\n      *(undefined4 *)(lVar5 + 0x50) = local_6c;\r\n      *(undefined4 *)(lVar5 + 0x54) = local_68;\r\n      *(undefined4 *)(lVar5 + 0x58) = local_64;\r\n      *(undefined4 *)(lVar5 + 0x5c) = local_60;\r\n      *(undefined4 *)(lVar5 + 0x60) = local_5c;\r\n      *(undefined4 *)(lVar5 + 100) = local_58;\r\n      *(byte *)(lVar5 + 0x138) = *(byte *)(lVar5 + 0x138) & 0xfb;\r\n      FUN_1401a2ff0(uVar4,lVar5,0,0x1ffff,in_stack_ffffffffffffff68);\r\n      lVar9 = *(longlong *)(param_1 + 0x140);\r\n      puVar6 = (undefined4 *)FUN_140167a00(lVar5,local_res8);\r\n      *(undefined4 *)(lVar9 + lVar8 * 8) = *puVar6;\r\n      lVar9 = local_res18;\r\n    }\r\n    piVar1 = (int *)(*(longlong *)(param_1 + 0x140) + 4 + lVar8 * 8);\r\n    *piVar1 = *piVar1 + 1;\r\n  }\r\n  lVar8 = 0;\r\n  if ((*(longlong *)(lVar7 + 0x140) != 0) &&\r\n     (lVar8 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar8 != 0)) {\r\n    *(byte *)(lVar8 + 0x138) = *(byte *)(lVar8 + 0x138) & 0xfb;\r\n    in_stack_ffffffffffffff68 =\r\n         (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);\r\n    FUN_1401a2ff0(uVar4,lVar8,0,0x1ffff,in_stack_ffffffffffffff68);\r\n    lVar5 = *(longlong *)(param_1 + 0x1a8);\r\n    puVar6 = (undefined4 *)FUN_140167a00(lVar8,local_res8);\r\n    *(undefined4 *)(lVar9 + 0x34 + lVar5) = *puVar6;\r\n  }\r\n  if ((*(longlong *)(lVar7 + 0x138) != 0) &&\r\n     (lVar7 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar7 != 0)) {\r\n    *(byte *)(lVar7 + 0x138) = *(byte *)(lVar7 + 0x138) & 0xfb;\r\n    uVar10 = (ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000;\r\n    FUN_1401a2ff0(uVar4,lVar7,0,0x1ffff,uVar10);\r\n    lVar3 = *(longlong *)(param_1 + 0x1a8);\r\n    puVar6 = (undefined4 *)FUN_140167a00(lVar7,local_res8);\r\n    *(undefined4 *)(lVar9 + 0x30 + lVar3) = *puVar6;\r\n    FUN_14072e5c0(param_1,local_res10,lVar7,lVar8,uVar10 & 0xffffffff00000000);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14072c8b0(longlong param_1,int param_2)\r\n\r\n{\r\n  int *piVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  undefined8 uVar4;\r\n  longlong lVar5;\r\n  undefined4 *puVar6;\r\n  longlong lVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  undefined1 local_res8 [8];\r\n  int local_res10;\r\n  longlong local_res18;\r\n  undefined4 *in_stack_ffffffffffffff68;\r\n  ulonglong uVar10;\r\n  undefined4 local_88;\r\n  undefined4 local_84;\r\n  undefined4 local_80;\r\n  undefined4 local_78;\r\n  undefined4 local_74;\r\n  undefined4 local_70;\r\n  undefined4 local_6c;\r\n  undefined4 local_68;\r\n  undefined4 local_64;\r\n  undefined4 local_60;\r\n  undefined4 local_5c;\r\n  undefined4 local_58;\r\n  \r\n  lVar7 = *(longlong *)(param_1 + 8);\r\n  lVar3 = *(longlong *)(param_1 + 0x10);\r\n  local_res10 = param_2;\r\n  uVar4 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n  lVar9 = (longlong)param_2 * 0x70;\r\n  local_88 = DAT_1431af668;\r\n  local_84 = DAT_1431af66c;\r\n  local_80 = DAT_1431af670;\r\n  local_78 = DAT_14228a780;\r\n  local_74 = DAT_14228a784;\r\n  local_70 = DAT_14228a788;\r\n  local_6c = DAT_14228a78c;\r\n  local_68 = DAT_14228a790;\r\n  local_64 = DAT_14228a794;\r\n  local_60 = DAT_14228a798;\r\n  local_5c = DAT_14228a79c;\r\n  local_58 = DAT_14228a7a0;\r\n  iVar2 = *(int *)(*(longlong *)(param_1 + 0x1a8) + 0x60 + lVar9);\r\n  lVar8 = (longlong)iVar2;\r\n  local_res18 = lVar9;\r\n  if (iVar2 != -1) {\r\n    in_stack_ffffffffffffff68 = &local_78;\r\n    FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,lVar8 * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                  &local_88,in_stack_ffffffffffffff68);\r\n  }\r\n  if ((*(longlong *)(lVar7 + 0x130) != 0) &&\r\n     (lVar5 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar5 != 0)) {\r\n    *(undefined4 *)(lVar5 + 0x38) = local_88;\r\n    *(undefined4 *)(lVar5 + 0x3c) = local_84;\r\n    in_stack_ffffffffffffff68 =\r\n         (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);\r\n    *(undefined4 *)(lVar5 + 0x40) = local_80;\r\n    *(undefined4 *)(lVar5 + 0x44) = local_78;\r\n    *(undefined4 *)(lVar5 + 0x48) = local_74;\r\n    *(undefined4 *)(lVar5 + 0x4c) = local_70;\r\n    *(undefined4 *)(lVar5 + 0x50) = local_6c;\r\n    *(undefined4 *)(lVar5 + 0x54) = local_68;\r\n    *(undefined4 *)(lVar5 + 0x58) = local_64;\r\n    *(undefined4 *)(lVar5 + 0x5c) = local_60;\r\n    *(undefined4 *)(lVar5 + 0x60) = local_5c;\r\n    *(undefined4 *)(lVar5 + 100) = local_58;\r\n    *(byte *)(lVar5 + 0x138) = *(byte *)(lVar5 + 0x138) & 0xfb;\r\n    FUN_1401a2ff0(uVar4,lVar5,0,0x1ffff,in_stack_ffffffffffffff68);\r\n  }\r\n  if ((*(longlong *)(lVar7 + 0x128) != 0) && (iVar2 != -1)) {\r\n    if ((*(int *)(*(longlong *)(param_1 + 0x140) + lVar8 * 8) == -1) &&\r\n       (lVar5 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar9 = local_res18, lVar5 != 0)\r\n       ) {\r\n      *(undefined4 *)(lVar5 + 0x38) = local_88;\r\n      *(undefined4 *)(lVar5 + 0x3c) = local_84;\r\n      in_stack_ffffffffffffff68 =\r\n           (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);\r\n      *(undefined4 *)(lVar5 + 0x40) = local_80;\r\n      *(undefined4 *)(lVar5 + 0x44) = local_78;\r\n      *(undefined4 *)(lVar5 + 0x48) = local_74;\r\n      *(undefined4 *)(lVar5 + 0x4c) = local_70;\r\n      *(undefined4 *)(lVar5 + 0x50) = local_6c;\r\n      *(undefined4 *)(lVar5 + 0x54) = local_68;\r\n      *(undefined4 *)(lVar5 + 0x58) = local_64;\r\n      *(undefined4 *)(lVar5 + 0x5c) = local_60;\r\n      *(undefined4 *)(lVar5 + 0x60) = local_5c;\r\n      *(undefined4 *)(lVar5 + 100) = local_58;\r\n      *(byte *)(lVar5 + 0x138) = *(byte *)(lVar5 + 0x138) & 0xfb;\r\n      FUN_1401a2ff0(uVar4,lVar5,0,0x1ffff,in_stack_ffffffffffffff68);\r\n      lVar9 = *(longlong *)(param_1 + 0x140);\r\n      puVar6 = (undefined4 *)FUN_140167a00(lVar5,local_res8);\r\n      *(undefined4 *)(lVar9 + lVar8 * 8) = *puVar6;\r\n      lVar9 = local_res18;\r\n    }\r\n    piVar1 = (int *)(*(longlong *)(param_1 + 0x140) + 4 + lVar8 * 8);\r\n    *piVar1 = *piVar1 + 1;\r\n  }\r\n  lVar8 = 0;\r\n  if ((*(longlong *)(lVar7 + 0x140) != 0) &&\r\n     (lVar8 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar8 != 0)) {\r\n    *(byte *)(lVar8 + 0x138) = *(byte *)(lVar8 + 0x138) & 0xfb;\r\n    in_stack_ffffffffffffff68 =\r\n         (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);\r\n    FUN_1401a2ff0(uVar4,lVar8,0,0x1ffff,in_stack_ffffffffffffff68);\r\n    lVar5 = *(longlong *)(param_1 + 0x1a8);\r\n    puVar6 = (undefined4 *)FUN_140167a00(lVar8,local_res8);\r\n    *(undefined4 *)(lVar9 + 0x34 + lVar5) = *puVar6;\r\n  }\r\n  if ((*(longlong *)(lVar7 + 0x138) != 0) &&\r\n     (lVar7 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar7 != 0)) {\r\n    *(byte *)(lVar7 + 0x138) = *(byte *)(lVar7 + 0x138) & 0xfb;\r\n    uVar10 = (ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000;\r\n    FUN_1401a2ff0(uVar4,lVar7,0,0x1ffff,uVar10);\r\n    lVar3 = *(longlong *)(param_1 + 0x1a8);\r\n    puVar6 = (undefined4 *)FUN_140167a00(lVar7,local_res8);\r\n    *(undefined4 *)(lVar9 + 0x30 + lVar3) = *puVar6;\r\n    FUN_14072e5c0(param_1,local_res10,lVar7,lVar8,uVar10 & 0xffffffff00000000);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_14072f3d0(longlong param_1)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  bool bVar4;
  byte bVar5;
  char cVar6;
  longlong lVar7;
  float *pfVar8;
  longlong lVar9;
  undefined8 uVar10;
  longlong lVar11;
  undefined4 *puVar12;
  double *pdVar13;
  undefined8 *puVar14;
  longlong lVar15;
  longlong lVar16;
  undefined4 *puVar17;
  int iVar18;
  float fVar19;
  undefined1 auStackX_8 [8];
  longlong lStackX_10;
  longlong lStackX_18;
  undefined8 uStackX_20;
  undefined1 auStack_b8 [8];
  undefined1 auStack_b0 [8];
  undefined1 auStack_a8 [8];
  undefined8 uStack_a0;
  undefined4 uStack_98;
  undefined4 uStack_94;
  undefined8 auStack_90 [10];
  
  lVar7 = FUN_1408a8ba0(D2_GameSingletonPtr);
  lVar15 = *(longlong *)(param_1 + 8);
  uStackX_20 = *(undefined8 *)(param_1 + 0x10);
  lStackX_18 = lVar15;
  pfVar8 = (float *)FUN_140877eb0(uStackX_20,auStackX_8);
  fVar19 = *(float *)(param_1 + 0x1e8) - *pfVar8;
  *(float *)(param_1 + 0x1e8) = fVar19;
  iVar18 = *(int *)(param_1 + 0x290);
  if ((iVar18 < 0) ||
     (lVar16 = *(longlong *)(*(longlong *)(param_1 + 0x1c8) + 0x48),
     *(float *)(lVar16 + 0x24 + (longlong)iVar18 * 0x28) ==
     *(float *)(lVar16 + 8 + (longlong)iVar18 * 0x28))) {
    bVar4 = false;
  }
  else {
    bVar4 = true;
  }
  if ((fVar19 <= 0.0) || (bVar4)) {
    uVar1 = *(undefined4 *)(lVar7 + 0xa8);
    lVar15 = FUN_140810240();
    FUN_1408112c0(lVar15 + 0xc8e8,0x100,0,uVar1);
    return;
  }
  lVar7 = *(longlong *)(param_1 + 0x198);
  lVar16 = (ulonglong)(*(uint *)(param_1 + 0x1a0) & 0xffffff) * 0x70 + lVar7;
  for (; lVar7 != lVar16; lVar7 = lVar7 + 0x70) {
    if (*(char *)(lVar7 + 0x6e) == '\0') {
      FUN_14072a280(param_1,lVar7);
    }
  }
  bVar4 = false;
  iVar18 = (*(uint *)(param_1 + 0x1b0) & 0xffffff) - 1;
  if (-1 < iVar18) {
    lVar7 = (longlong)iVar18 * 0x70;
    lVar16 = lVar7;
    do {
      lStackX_10 = lVar7;
      cVar6 = FUN_14072d600(param_1,iVar18);
      if (cVar6 == '\0') {
        puVar17 = (undefined4 *)(*(longlong *)(param_1 + 0x1a8) + lVar16);
        lVar9 = FUN_1406dc560(*puVar17);
        if (*(char *)((longlong)puVar17 + 0x6d) != '\0') {
          FUN_140729350(param_1,puVar17);
          if (puVar17[0xc] != -1) {
            uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
            lVar11 = FUN_1401a04a0(uVar10,puVar17[0xc]);
            lVar15 = lStackX_18;
            if (lVar11 != 0) {
              uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
              uVar10 = FUN_1401a04a0(uVar10,puVar17[0xd]);
              puVar12 = (undefined4 *)FUN_140877eb0(uStackX_20,auStackX_8);
              FUN_14072e5c0(param_1,iVar18,lVar11,uVar10,*puVar12);
              lVar7 = lStackX_10;
              lVar15 = lStackX_18;
            }
          }
          if ((*(char *)(puVar17 + 0x1b) == '\0') || (0.0 < (float)puVar17[0x1a])) {
            bVar5 = 0;
          }
          else {
            bVar5 = 1;
          }
          bVar4 = (bool)(bVar4 | bVar5);
          if (((*(char *)(puVar17 + 0x1b) == '\0') && (1.0 <= (float)puVar17[0x1a])) &&
             ((*(byte *)(lVar9 + 0x368) & 0x80) != 0)) {
            FUN_14072b6c0(param_1,lVar9);
          }
        }
        uVar10 = FUN_1406dc560(*(undefined4 *)(lVar7 + *(longlong *)(param_1 + 0x1a8)));
        lVar11 = FUN_1408f2e50(uVar10);
        if (*(int *)(lVar11 + 0x370) == 2) {
          if (0.0 <= *(double *)(puVar17 + 10)) {
            pdVar13 = (double *)FUN_140878030(lVar9,auStack_b0);
            if (*pdVar13 < *(double *)(puVar17 + 10)) goto code_r0x00014072f73a;
            uStack_98 = 0x26;
            uStack_a0 = DAT_14398dad0;
            FUN_140131ae0(auStack_90);
            uVar1 = *(undefined4 *)(lVar9 + 0xa8);
            lVar7 = FUN_140810240();
            puVar14 = (undefined8 *)FUN_1408112c0(lVar7 + 0xc8e8,0x4f,0x18,uVar1);
            if (puVar14 != (undefined8 *)0x0) {
              *puVar14 = uStack_a0;
              puVar14[1] = CONCAT44(uStack_94,uStack_98);
              puVar14[2] = auStack_90[0];
            }
            fVar19 = (float)FUN_1408ac040(*(undefined4 *)(lVar15 + 0x90),
                                          *(undefined4 *)(lVar15 + 0x94));
            pdVar13 = (double *)FUN_140878030(lVar9,auStack_a8);
            lVar7 = lStackX_10;
          }
          else {
            fVar19 = *(float *)(lVar15 + 0x98);
            pdVar13 = (double *)FUN_140878030(lVar9,auStack_b8);
          }
          *(double *)(puVar17 + 10) = (double)fVar19 + *pdVar13;
        }
code_r0x00014072f73a:
        if (bVar4) {
          iVar2 = *(int *)(param_1 + 0x2c);
          if (iVar2 != 2) {
            if ((iVar2 != *(int *)(param_1 + 0x28)) &&
               (pcVar3 = *(code **)(param_1 + 0x20 + ((longlong)iVar2 + 1) * 0x18),
               pcVar3 != (code *)0x0)) {
              (*pcVar3)(*(undefined8 *)(param_1 + 0x20),2);
            }
            if ((*(int *)(param_1 + 0x28) != 2) && (*(code **)(param_1 + 0x60) != (code *)0x0)) {
              (**(code **)(param_1 + 0x60))
                        (*(undefined8 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x2c));
            }
            *(undefined4 *)(param_1 + 0x2c) = 2;
          }
        }
      }
      lVar7 = lVar7 + -0x70;
      lVar16 = lVar16 + -0x70;
      iVar18 = iVar18 + -1;
    } while (-1 < iVar18);
  }
  return;
}


````

### text

````text

void FUN_140729190(longlong param_1,int param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 *puVar5;
  uint uVar6;
  int iVar7;
  undefined8 uStack_70;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined8 uStack_60;
  undefined1 auStack_58 [8];
  undefined4 uStack_50;
  undefined1 auStack_40 [24];
  
  iVar7 = 0;
  if ((*(uint *)(param_1 + 0x1b0) & 0xffffff) != 0) {
    do {
      lVar2 = FUN_1406dc560(*(undefined4 *)((longlong)iVar7 * 0x70 + *(longlong *)(param_1 + 0x1a8))
                           );
      if (lVar2 != 0) {
        uVar3 = FUN_1406dc560(*(undefined4 *)
                               ((longlong)iVar7 * 0x70 + *(longlong *)(param_1 + 0x1a8)));
        lVar4 = FUN_1408f2e50(uVar3);
        uStack_70 = DAT_14398dad0;
        uStack_68 = 0x28;
        if (*(int *)(lVar4 + 0x370) != 2) {
          uStack_68 = 0x27;
        }
        FUN_140131ae0(&uStack_60);
        uVar1 = *(undefined4 *)(lVar2 + 0xa8);
        lVar2 = FUN_140810240();
        puVar5 = (undefined8 *)FUN_1408112c0(lVar2 + 0xc8e8,0x4f,0x18,uVar1);
        if (puVar5 != (undefined8 *)0x0) {
          *puVar5 = uStack_70;
          puVar5[1] = CONCAT44(uStack_64,uStack_68);
          puVar5[2] = uStack_60;
        }
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < (int)(*(uint *)(param_1 + 0x1b0) & 0xffffff));
  }
  if (param_2 == 2) {
    uVar6 = *(uint *)(param_1 + 0x1b0) & 0xffffff;
    while (uVar6 = uVar6 - 1, -1 < (int)uVar6) {
      FUN_14072b0e0(param_1,uVar6,1);
    }
    *(uint *)(param_1 + 0x170) = *(uint *)(param_1 + 0x170) & 0xff000000;
    *(uint *)(param_1 + 0x1a0) = *(uint *)(param_1 + 0x1a0) & 0xff000000;
  }
  uStack_70 = CONCAT71(uStack_70._1_7_,0x69);
  uStack_68 = 0;
  auStack_58[0] = 0x69;
  uStack_50 = 4;
  uVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);
  FUN_1408848a0(uVar3,auStack_40,&DAT_1439ab620,auStack_58,&uStack_70);
  FUN_140882fc0(auStack_40);
  return;
}


````

### text

````text

void FUN_140729350(longlong param_1,longlong param_2)

{
  longlong lVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float local_118;
  float local_114;
  float local_110;
  float local_108;
  float local_104;
  float local_100;
  float local_f8;
  float local_f4;
  float local_f0;
  undefined4 local_ec;
  undefined4 local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 local_dc;
  undefined4 local_d8;
  float local_d0;
  float local_cc;
  float local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  undefined4 local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  
  local_118 = DAT_1431af668;
  local_114 = DAT_1431af66c;
  local_110 = DAT_1431af670;
  local_d0 = DAT_14228a780;
  local_cc = DAT_14228a784;
  local_c8 = DAT_14228a788;
  local_c4 = DAT_14228a78c;
  local_c0 = DAT_14228a790;
  local_bc = DAT_14228a794;
  local_b8 = DAT_14228a798;
  local_b4 = DAT_14228a79c;
  local_b0 = DAT_14228a7a0;
  if ((*(short *)(param_2 + 0x58) != -1) && (lVar1 = FUN_1403da910(param_2 + 8,2), lVar1 != 0)) {
    FUN_14053dac0(lVar1,0,param_2 + 0x3c,&local_118,&local_d0);
  }
  iVar4 = -1;
  iVar3 = -1;
  iVar2 = 0;
  fVar9 = 180.0;
  if ((*(uint *)(param_1 + 0x138) & 0xffffff) != 0) {
    do {
      local_108 = DAT_1431af668;
      local_104 = DAT_1431af66c;
      local_100 = DAT_1431af670;
      local_f8 = DAT_14228a780;
      local_f4 = DAT_14228a784;
      local_f0 = DAT_14228a788;
      local_ec = DAT_14228a78c;
      local_e8 = DAT_14228a790;
      local_e4 = DAT_14228a794;
      local_e0 = DAT_14228a798;
      local_dc = DAT_14228a79c;
      local_d8 = DAT_14228a7a0;
      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,
                    (longlong)iVar2 * 0x20 + *(longlong *)(param_1 + 0x130),&local_108,&local_f8);
      fVar8 = local_118 - local_108;
      fVar6 = local_114 - local_104;
      fVar7 = local_110 - local_100;
      fVar5 = fVar6 * fVar6 + fVar8 * fVar8 + fVar7 * fVar7;
      if (fVar5 <= 1.1754944e-38) {
        fVar5 = 1.1754944e-38;
      }
      fVar5 = 1.0 / SQRT(fVar5);
      fVar5 = local_f4 * fVar6 * fVar5 + local_f8 * fVar8 * fVar5 + local_f0 * fVar7 * fVar5;
      if (-1.0 < fVar5) {
        if (fVar5 < 1.0) {
          fVar5 = (float)FUN_1411afeb4(fVar5);
        }
        else {
          fVar5 = 0.0;
        }
      }
      else {
        fVar5 = 3.1415927;
      }
      fVar5 = fVar5 * 57.295776;
      if ((360.0 <= fVar5) || (fVar5 < 0.0)) {
        fVar6 = (float)FUN_1411ad95c(fVar5 * 0.0027777778);
        fVar5 = fVar5 - fVar6 * 360.0;
      }
      if (180.0 < fVar5) {
        fVar5 = fVar5 - 360.0;
      }
      iVar3 = iVar4;
      if (ABS(fVar5) < fVar9) {
        fVar9 = ABS(fVar5);
        iVar3 = iVar2;
      }
      iVar2 = iVar2 + 1;
      iVar4 = iVar3;
    } while (iVar2 < (int)(*(uint *)(param_1 + 0x138) & 0xffffff));
  }
  *(int *)(param_2 + 0x60) = iVar3;
  return;
}


````

### text

````text

void FUN_14072e5c0(longlong param_1,int param_2,longlong param_3,longlong param_4,float param_5)

{
  byte bVar1;
  int iVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong lVar5;
  longlong lVar6;
  undefined4 *puVar7;
  undefined8 uVar8;
  undefined8 uVar9;
  longlong lVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  undefined1 local_res10 [8];
  undefined4 *in_stack_fffffffffffffe08;
  float local_1e8;
  float local_1e4;
  float local_1e0;
  float local_1d8;
  float local_1d4;
  float local_1d0;
  undefined4 local_1c8;
  undefined4 local_1c4;
  undefined4 local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  undefined4 local_1b4;
  undefined4 local_1b0;
  undefined4 local_1ac;
  undefined4 local_1a8;
  float local_1a0;
  float local_19c;
  float local_198;
  undefined4 local_190;
  undefined4 local_18c;
  undefined4 local_188;
  undefined4 local_184;
  undefined4 local_180;
  undefined4 local_17c;
  undefined4 local_178;
  undefined4 local_174;
  undefined4 local_170;
  undefined1 local_168 [40];
  undefined1 local_140 [24];
  undefined1 local_128 [84];
  undefined4 local_d4;
  
  lVar10 = (longlong)param_2 * 0x70;
  lVar5 = FUN_1406dc560(*(undefined4 *)(lVar10 + *(longlong *)(param_1 + 0x1a8)));
  if (lVar5 != 0) {
    local_1e8 = *(float *)(lVar5 + 0x300);
    local_1e4 = *(float *)(lVar5 + 0x304);
    lVar6 = *(longlong *)(param_1 + 0x10);
    local_1e0 = *(float *)(lVar5 + 0x308);
    local_190 = *(undefined4 *)(lVar5 + 0x30c);
    local_18c = *(undefined4 *)(lVar5 + 0x310);
    local_188 = *(undefined4 *)(lVar5 + 0x314);
    local_184 = *(undefined4 *)(lVar5 + 0x318);
    local_180 = *(undefined4 *)(lVar5 + 0x31c);
    local_17c = *(undefined4 *)(lVar5 + 800);
    local_178 = *(undefined4 *)(lVar5 + 0x324);
    local_174 = *(undefined4 *)(lVar5 + 0x328);
    local_170 = *(undefined4 *)(lVar5 + 0x32c);
    local_1d8 = *(float *)(lVar6 + 0x300);
    local_1d4 = *(float *)(lVar6 + 0x304);
    local_1d0 = *(float *)(lVar6 + 0x308);
    local_1c8 = *(undefined4 *)(lVar6 + 0x30c);
    local_1c4 = *(undefined4 *)(lVar6 + 0x310);
    local_1c0 = *(undefined4 *)(lVar6 + 0x314);
    local_1bc = *(undefined4 *)(lVar6 + 0x318);
    local_1b8 = *(undefined4 *)(lVar6 + 0x31c);
    local_1b4 = *(undefined4 *)(lVar6 + 800);
    local_1b0 = *(undefined4 *)(lVar6 + 0x324);
    local_1ac = *(undefined4 *)(lVar6 + 0x328);
    local_1a8 = *(undefined4 *)(lVar6 + 0x32c);
    if ((*(short *)(lVar10 + 0x58 + *(longlong *)(param_1 + 0x1a8)) != -1) &&
       (lVar6 = FUN_1403da910(*(longlong *)(param_1 + 0x1a8) + 8 + lVar10,2), lVar6 != 0)) {
      in_stack_fffffffffffffe08 = &local_190;
      FUN_14053dac0(lVar6,0,*(longlong *)(param_1 + 0x1a8) + 0x3c + lVar10,&local_1e8,
                    in_stack_fffffffffffffe08);
    }
    iVar2 = *(int *)(lVar10 + 0x60 + *(longlong *)(param_1 + 0x1a8));
    if (iVar2 != -1) {
      in_stack_fffffffffffffe08 = &local_1c8;
      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,
                    (longlong)iVar2 * 0x20 + *(longlong *)(param_1 + 0x130),&local_1d8,
                    in_stack_fffffffffffffe08);
    }
    *(float *)(lVar10 + 100 + *(longlong *)(param_1 + 0x1a8)) =
         param_5 + *(float *)(lVar10 + 100 + *(longlong *)(param_1 + 0x1a8));
    lVar6 = *(longlong *)(param_1 + 0x1a8);
    lVar4 = *(longlong *)(param_1 + 8);
    fVar11 = *(float *)(lVar10 + 100 + lVar6) / *(float *)(lVar4 + 0x160);
    fVar12 = 1.0;
    if (fVar11 <= 1.0) {
      fVar12 = fVar11;
    }
    if (*(char *)(lVar10 + 0x6c + lVar6) == '\0') {
      *(float *)(lVar10 + 0x68 + lVar6) = fVar12;
    }
    else {
      *(float *)(lVar10 + 0x68 + lVar6) = 1.0 - fVar12;
    }
    fVar14 = local_1d8 - local_1e8;
    fVar13 = local_1d4 - local_1e4;
    fVar11 = local_1d0 - local_1e0;
    fVar12 = fVar13 * fVar13 + fVar14 * fVar14 + fVar11 * fVar11;
    if (fVar12 <= 1.1754944e-38) {
      fVar12 = 1.1754944e-38;
    }
    local_198 = 1.0 / SQRT(fVar12);
    local_1a0 = fVar14 * local_198;
    local_19c = fVar13 * local_198;
    local_198 = fVar11 * local_198;
    fVar12 = (1.0 - *(float *)(lVar10 + 0x68 + *(longlong *)(param_1 + 0x1a8))) *
             SQRT(fVar13 * fVar13 + fVar14 * fVar14 + fVar11 * fVar11);
    local_1e8 = local_1a0 * fVar12 + local_1e8;
    local_1e4 = local_19c * fVar12 + local_1e4;
    local_1e0 = local_198 * fVar12 + local_1e0;
    *(float *)(param_3 + 0x38) = local_1e8;
    *(float *)(param_3 + 0x3c) = local_1e4;
    *(float *)(param_3 + 0x40) = local_1e0;
    puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);
    *(undefined4 *)(param_3 + 0x44) = *puVar7;
    *(undefined4 *)(param_3 + 0x48) = puVar7[1];
    *(undefined4 *)(param_3 + 0x4c) = puVar7[2];
    *(undefined4 *)(param_3 + 0x50) = puVar7[3];
    *(undefined4 *)(param_3 + 0x54) = puVar7[4];
    *(undefined4 *)(param_3 + 0x58) = puVar7[5];
    *(undefined4 *)(param_3 + 0x5c) = puVar7[6];
    *(undefined4 *)(param_3 + 0x60) = puVar7[7];
    *(undefined4 *)(param_3 + 100) = puVar7[8];
    *(float *)(param_3 + 0x80) = local_1d8;
    *(float *)(param_3 + 0x84) = local_1d4;
    *(float *)(param_3 + 0x88) = local_1d0;
    *(undefined4 *)(param_3 + 0x90) = local_1c4;
    *(undefined4 *)(param_3 + 0x8c) = local_1c8;
    *(undefined4 *)(param_3 + 0x94) = local_1c0;
    *(undefined4 *)(param_3 + 0x98) = local_1bc;
    *(undefined4 *)(param_3 + 0x9c) = local_1b8;
    *(undefined4 *)(param_3 + 0xa0) = local_1b4;
    *(undefined4 *)(param_3 + 0xa4) = local_1b0;
    *(undefined4 *)(param_3 + 0xa8) = local_1ac;
    *(undefined4 *)(param_3 + 0xac) = local_1a8;
    if (param_4 != 0) {
      *(float *)(param_4 + 0x38) = local_1e8;
      *(float *)(param_4 + 0x3c) = local_1e4;
      *(float *)(param_4 + 0x40) = local_1e0;
      puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);
      *(undefined4 *)(param_4 + 0x44) = *puVar7;
      *(undefined4 *)(param_4 + 0x48) = puVar7[1];
      *(undefined4 *)(param_4 + 0x4c) = puVar7[2];
      *(undefined4 *)(param_4 + 0x50) = puVar7[3];
      *(undefined4 *)(param_4 + 0x54) = puVar7[4];
      *(undefined4 *)(param_4 + 0x58) = puVar7[5];
      *(undefined4 *)(param_4 + 0x5c) = puVar7[6];
      *(undefined4 *)(param_4 + 0x60) = puVar7[7];
      *(undefined4 *)(param_4 + 100) = puVar7[8];
      *(float *)(param_4 + 0x80) = local_1d8;
      *(float *)(param_4 + 0x84) = local_1d4;
      *(float *)(param_4 + 0x88) = local_1d0;
      *(undefined4 *)(param_4 + 0x8c) = local_1c8;
      *(undefined4 *)(param_4 + 0x90) = local_1c4;
      *(undefined4 *)(param_4 + 0x94) = local_1c0;
      *(undefined4 *)(param_4 + 0x98) = local_1bc;
      *(undefined4 *)(param_4 + 0x9c) = local_1b8;
      *(undefined4 *)(param_4 + 0xa0) = local_1b4;
      *(undefined4 *)(param_4 + 0xa4) = local_1b0;
      *(undefined4 *)(param_4 + 0xa8) = local_1ac;
      *(undefined4 *)(param_4 + 0xac) = local_1a8;
    }
    uVar8 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
    lVar6 = FUN_1401a04a0(uVar8,*(undefined4 *)
                                 (*(longlong *)(param_1 + 0x140) + (longlong)iVar2 * 8));
    if (lVar6 != 0) {
      *(float *)(lVar6 + 0x38) = local_1d8;
      *(float *)(lVar6 + 0x3c) = local_1d4;
      *(float *)(lVar6 + 0x40) = local_1d0;
      *(undefined4 *)(lVar6 + 0x44) = local_1c8;
      *(undefined4 *)(lVar6 + 0x48) = local_1c4;
      *(undefined4 *)(lVar6 + 0x4c) = local_1c0;
      *(undefined4 *)(lVar6 + 0x50) = local_1bc;
      *(undefined4 *)(lVar6 + 0x54) = local_1b8;
      *(undefined4 *)(lVar6 + 0x58) = local_1b4;
      *(undefined4 *)(lVar6 + 0x5c) = local_1b0;
      *(undefined4 *)(lVar6 + 0x60) = local_1ac;
      *(undefined4 *)(lVar6 + 100) = local_1a8;
    }
    lVar6 = *(longlong *)(param_1 + 0x1a8);
    if ((1.0 <= *(float *)(lVar10 + 0x68 + lVar6)) && (*(char *)(lVar10 + 0x6c + lVar6) == '\0')) {
      if ((*(int *)(lVar10 + 0x38 + lVar6) == -1) && (*(longlong *)(lVar4 + 0x150) != 0)) {
        lVar6 = FUN_14019d100(uVar8,*(undefined4 *)(lVar5 + 0xa8));
        if (lVar6 != 0) {
          *(byte *)(lVar6 + 0x138) = *(byte *)(lVar6 + 0x138) & 0xfb;
          FUN_1401a2ff0(uVar8,lVar6,0,0x1ffff,
                        (ulonglong)in_stack_fffffffffffffe08 & 0xffffffff00000000);
          lVar4 = *(longlong *)(param_1 + 0x1a8);
          puVar7 = (undefined4 *)FUN_140167a00(lVar6,local_res10);
          *(undefined4 *)(lVar10 + 0x38 + lVar4) = *puVar7;
        }
        bVar1 = *(byte *)(lVar5 + 0x368);
        if (*(int *)(param_1 + 0x2ac) < 0) {
          fVar12 = 0.0;
        }
        else {
          fVar12 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +
                             (longlong)*(int *)(param_1 + 0x2ac) * 0x28);
        }
        fVar11 = (float)FUN_1408ac040(0,0x42c80000);
        if (((~(bVar1 >> 7) & 1) == 0) || (fVar12 <= fVar11)) {
          uVar3 = *(undefined4 *)(lVar5 + 0xa8);
          uVar9 = FUN_140810240();
          FUN_1408103f0(uVar9,0x102,uVar3);
        }
        else {
          D2_DamageParams_Construct(local_128,DAT_14231f350);
          lVar6 = FUN_1408a8ba0(D2_GameSingletonPtr);
          if (lVar6 == 0) {
            local_d4 = 0x3fff0010;
          }
          else {
            local_d4 = *(undefined4 *)(lVar6 + 0xa8);
          }
          uVar9 = FUN_140882e20(local_168,local_128);
          FUN_140884920(lVar5,local_140,&DAT_1439a9d20,uVar9);
          FUN_140882fc0(local_140);
        }
      }
      if (param_4 != 0) {
        FUN_1401a31a0(uVar8,param_4);
        *(undefined4 *)(lVar10 + 0x34 + *(longlong *)(param_1 + 0x1a8)) = 0xffffffff;
      }
    }
    lVar5 = FUN_1401a04a0(uVar8,*(undefined4 *)(lVar10 + 0x38 + *(longlong *)(param_1 + 0x1a8)));
    if (lVar5 != 0) {
      *(float *)(lVar5 + 0x38) = local_1e8;
      *(float *)(lVar5 + 0x3c) = local_1e4;
      *(float *)(lVar5 + 0x40) = local_1e0;
      puVar7 = (undefined4 *)FUN_140112000(&local_1a0,local_168);
      *(undefined4 *)(lVar5 + 0x44) = *puVar7;
      *(undefined4 *)(lVar5 + 0x48) = puVar7[1];
      *(undefined4 *)(lVar5 + 0x4c) = puVar7[2];
      *(undefined4 *)(lVar5 + 0x50) = puVar7[3];
      *(undefined4 *)(lVar5 + 0x54) = puVar7[4];
      *(undefined4 *)(lVar5 + 0x58) = puVar7[5];
      *(undefined4 *)(lVar5 + 0x5c) = puVar7[6];
      *(undefined4 *)(lVar5 + 0x60) = puVar7[7];
      *(undefined4 *)(lVar5 + 100) = puVar7[8];
    }
  }
  return;
}


````

### text

````text

void FUN_14072a280(longlong param_1,longlong param_2)

{
  undefined4 uVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  int iVar6;
  char cVar7;
  uint uVar8;
  undefined4 *puVar9;
  longlong lStack_48;
  undefined1 *puStack_40;
  undefined8 auStack_30 [3];
  
  lVar5 = *(longlong *)(param_1 + 8);
  uVar1 = FUN_140728720(param_1 + 0x1a8);
  puVar9 = (undefined4 *)
           ((ulonglong)((*(uint *)(param_1 + 0x1b0) & 0xffffff) - 1) * 0x70 +
           *(longlong *)(param_1 + 0x1a8));
  lVar3 = FUN_1406dc560(*puVar9);
  if (((*(byte *)(lVar3 + 0x368) & 0x80) == 0) && (*(longlong *)(lVar5 + 0x88) != 0)) {
    uVar4 = FUN_1403da910(puVar9 + 6,2);
    FUN_1405888c0(uVar4,*(undefined8 *)(lVar5 + 0x88));
  }
  if (*(char *)(param_2 + 0x6d) != '\0') {
    FUN_140729350(param_1,param_2);
    FUN_14072c8b0(param_1,uVar1);
  }
  if ((*(byte *)(lVar3 + 0x368) & 0x80) == 0) {
    lStack_48 = CONCAT71(lStack_48._1_7_,0x69);
    puStack_40 = (undefined1 *)CONCAT44(puStack_40._4_4_,1);
    FUN_140884920(lVar3,auStack_30,&DAT_1439ac480,&lStack_48);
    FUN_140882fc0(auStack_30);
    puStack_40 = &LAB_14072f8b0;
    lStack_48 = param_1;
    uVar4 = FUN_140810240();
    FUN_140810810(uVar4,lVar3,0xdd,&lStack_48);
    puStack_40 = &LAB_14072f8c0;
    lStack_48 = param_1;
    uVar4 = FUN_140810240();
    FUN_140810810(uVar4,lVar3,0xdb,&lStack_48);
  }
  auStack_30[0] = 0x14072f2a2;
  uVar4 = FUN_1405bef90();
  cVar7 = '\0';
  iVar6 = 0;
  uVar8 = *(uint *)(param_1 + 0x1b0) & 0xffffff;
  if (uVar8 != 0) {
    do {
      auStack_30[0] = 0x14072f2d5;
      lVar5 = FUN_1406dc560(*(undefined4 *)((longlong)iVar6 * 0x70 + *(longlong *)(param_1 + 0x1a8))
                           );
      if (lVar5 != 0) {
        auStack_30[0] = 0x14072f2f3;
        iVar2 = FUN_140604bd0(D2_GameSingletonPtr + 0x1f7bb8,lVar5,uVar4);
        if (iVar2 == 0) {
          cVar7 = '\x01';
          break;
        }
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < (int)uVar8);
  }
  if (cVar7 != *(char *)(param_1 + 0x2b1)) {
    uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
    auStack_30[0] = 0x14072f31a;
    lVar5 = FUN_140810240();
    uVar4 = 0x17b;
    if (cVar7 == '\0') {
      uVar4 = 0x17c;
    }
    auStack_30[0] = 0x14072f33e;
    puVar9 = (undefined4 *)FUN_1408112c0(lVar5 + 0xc8e8,uVar4,4,uVar1);
    if (puVar9 != (undefined4 *)0x0) {
      *puVar9 = 3;
    }
    *(char *)(param_1 + 0x2b1) = cVar7;
  }
  return;
}


````

### text

````text

void FUN_14072c8b0(longlong param_1,int param_2)

{
  int *piVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined4 *puVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lVar9;
  undefined1 local_res8 [8];
  int local_res10;
  longlong local_res18;
  undefined4 *in_stack_ffffffffffffff68;
  ulonglong uVar10;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined4 local_78;
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  
  lVar7 = *(longlong *)(param_1 + 8);
  lVar3 = *(longlong *)(param_1 + 0x10);
  local_res10 = param_2;
  uVar4 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
  lVar9 = (longlong)param_2 * 0x70;
  local_88 = DAT_1431af668;
  local_84 = DAT_1431af66c;
  local_80 = DAT_1431af670;
  local_78 = DAT_14228a780;
  local_74 = DAT_14228a784;
  local_70 = DAT_14228a788;
  local_6c = DAT_14228a78c;
  local_68 = DAT_14228a790;
  local_64 = DAT_14228a794;
  local_60 = DAT_14228a798;
  local_5c = DAT_14228a79c;
  local_58 = DAT_14228a7a0;
  iVar2 = *(int *)(*(longlong *)(param_1 + 0x1a8) + 0x60 + lVar9);
  lVar8 = (longlong)iVar2;
  local_res18 = lVar9;
  if (iVar2 != -1) {
    in_stack_ffffffffffffff68 = &local_78;
    FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,lVar8 * 0x20 + *(longlong *)(param_1 + 0x130),
                  &local_88,in_stack_ffffffffffffff68);
  }
  if ((*(longlong *)(lVar7 + 0x130) != 0) &&
     (lVar5 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar5 != 0)) {
    *(undefined4 *)(lVar5 + 0x38) = local_88;
    *(undefined4 *)(lVar5 + 0x3c) = local_84;
    in_stack_ffffffffffffff68 =
         (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);
    *(undefined4 *)(lVar5 + 0x40) = local_80;
    *(undefined4 *)(lVar5 + 0x44) = local_78;
    *(undefined4 *)(lVar5 + 0x48) = local_74;
    *(undefined4 *)(lVar5 + 0x4c) = local_70;
    *(undefined4 *)(lVar5 + 0x50) = local_6c;
    *(undefined4 *)(lVar5 + 0x54) = local_68;
    *(undefined4 *)(lVar5 + 0x58) = local_64;
    *(undefined4 *)(lVar5 + 0x5c) = local_60;
    *(undefined4 *)(lVar5 + 0x60) = local_5c;
    *(undefined4 *)(lVar5 + 100) = local_58;
    *(byte *)(lVar5 + 0x138) = *(byte *)(lVar5 + 0x138) & 0xfb;
    FUN_1401a2ff0(uVar4,lVar5,0,0x1ffff,in_stack_ffffffffffffff68);
  }
  if ((*(longlong *)(lVar7 + 0x128) != 0) && (iVar2 != -1)) {
    if ((*(int *)(*(longlong *)(param_1 + 0x140) + lVar8 * 8) == -1) &&
       (lVar5 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar9 = local_res18, lVar5 != 0)
       ) {
      *(undefined4 *)(lVar5 + 0x38) = local_88;
      *(undefined4 *)(lVar5 + 0x3c) = local_84;
      in_stack_ffffffffffffff68 =
           (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);
      *(undefined4 *)(lVar5 + 0x40) = local_80;
      *(undefined4 *)(lVar5 + 0x44) = local_78;
      *(undefined4 *)(lVar5 + 0x48) = local_74;
      *(undefined4 *)(lVar5 + 0x4c) = local_70;
      *(undefined4 *)(lVar5 + 0x50) = local_6c;
      *(undefined4 *)(lVar5 + 0x54) = local_68;
      *(undefined4 *)(lVar5 + 0x58) = local_64;
      *(undefined4 *)(lVar5 + 0x5c) = local_60;
      *(undefined4 *)(lVar5 + 0x60) = local_5c;
      *(undefined4 *)(lVar5 + 100) = local_58;
      *(byte *)(lVar5 + 0x138) = *(byte *)(lVar5 + 0x138) & 0xfb;
      FUN_1401a2ff0(uVar4,lVar5,0,0x1ffff,in_stack_ffffffffffffff68);
      lVar9 = *(longlong *)(param_1 + 0x140);
      puVar6 = (undefined4 *)FUN_140167a00(lVar5,local_res8);
      *(undefined4 *)(lVar9 + lVar8 * 8) = *puVar6;
      lVar9 = local_res18;
    }
    piVar1 = (int *)(*(longlong *)(param_1 + 0x140) + 4 + lVar8 * 8);
    *piVar1 = *piVar1 + 1;
  }
  lVar8 = 0;
  if ((*(longlong *)(lVar7 + 0x140) != 0) &&
     (lVar8 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar8 != 0)) {
    *(byte *)(lVar8 + 0x138) = *(byte *)(lVar8 + 0x138) & 0xfb;
    in_stack_ffffffffffffff68 =
         (undefined4 *)((ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000);
    FUN_1401a2ff0(uVar4,lVar8,0,0x1ffff,in_stack_ffffffffffffff68);
    lVar5 = *(longlong *)(param_1 + 0x1a8);
    puVar6 = (undefined4 *)FUN_140167a00(lVar8,local_res8);
    *(undefined4 *)(lVar9 + 0x34 + lVar5) = *puVar6;
  }
  if ((*(longlong *)(lVar7 + 0x138) != 0) &&
     (lVar7 = FUN_14019d100(uVar4,*(undefined4 *)(lVar3 + 0xa8)), lVar7 != 0)) {
    *(byte *)(lVar7 + 0x138) = *(byte *)(lVar7 + 0x138) & 0xfb;
    uVar10 = (ulonglong)in_stack_ffffffffffffff68 & 0xffffffff00000000;
    FUN_1401a2ff0(uVar4,lVar7,0,0x1ffff,uVar10);
    lVar3 = *(longlong *)(param_1 + 0x1a8);
    puVar6 = (undefined4 *)FUN_140167a00(lVar7,local_res8);
    *(undefined4 *)(lVar9 + 0x30 + lVar3) = *puVar6;
    FUN_14072e5c0(param_1,local_res10,lVar7,lVar8,uVar10 & 0xffffffff00000000);
  }
  return;
}


````
