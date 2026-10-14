# doppelganger-crouch-playback-timing.json

Original: [outputs/d2-coop-lab/analysis/ghidra/doppelganger-crouch-playback-timing.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/doppelganger-crouch-playback-timing.json>)

SHA-256: `4636A493B93D85CF4E6C6E9469F280840193F00B487AC998EF7D778447C8B96C`; 16643 bytes.

## Original saved record

````json
{
  "ExactExecutableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "At": "2026-10-09T11:27:46.507Z",
  "Findings": [
    "Native durationTime_t at AnimBasicInputData+0x10 is float seconds, copied to property+0x14 and incremented by float native dt * rate in 6e4850. Prior signature incorrectly labeled int; zero callers unaffected.",
    "C5B5 attempted integer1000 start offset with rate0: both rigs stayed near standing (failed). 888A corrected float1.2-second trial passes on both rigs; full-body composition and guard/regression evidence recorded on 9165.",
    "Native node update 6e4af0 contains pdata fragments; do not hook internal fragment as standalone function."
  ],
  "NodeUpdate": {
    "result": "\r\nvoid FUN_1406e4af0(longlong param_1,char param_2)\r\n\r\n{\r\n  longlong *plVar1;\r\n  char *pcVar2;\r\n  undefined4 uVar3;\r\n  float fVar4;\r\n  longlong lVar5;\r\n  char cVar6;\r\n  int iVar7;\r\n  float fVar8;\r\n  uint uVar9;\r\n  float *pfVar10;\r\n  char *pcVar11;\r\n  float *pfVar12;\r\n  byte bVar13;\r\n  char *pcVar14;\r\n  ulonglong uVar15;\r\n  longlong lVar16;\r\n  ulonglong uVar17;\r\n  longlong lVar18;\r\n  char *pcVar19;\r\n  longlong *plVar20;\r\n  ulonglong uVar21;\r\n  int iVar22;\r\n  char *pcVar23;\r\n  ulonglong uVar24;\r\n  undefined4 *puVar25;\r\n  undefined4 uVar26;\r\n  float afStackX_10 [2];\r\n  float afStackX_18 [4];\r\n  char *pcStack_98;\r\n  undefined4 uStack_90;\r\n  undefined4 uStack_8c;\r\n  undefined4 uStack_88;\r\n  float fStack_84;\r\n  undefined4 uStack_80;\r\n  int iStack_7c;\r\n  undefined4 uStack_78;\r\n  undefined1 uStack_74;\r\n  \r\n  pcVar14 = (char *)0x0;\r\n  fVar4 = *(float *)(*(longlong *)(param_1 + 0x10) + 8);\r\n  plVar1 = (longlong *)(param_1 + 0x9b0);\r\n  pcVar23 = pcVar14;\r\n  if (param_2 != '\\0') {\r\n    if (*(char *)(param_1 + 0x99c) == '\\0') {\r\n      *(char *)(param_1 + 0xa76) = *(char *)(param_1 + 0xa76) + '\\x01';\r\n      *(undefined1 *)(param_1 + 0xa60) = 0xff;\r\n      *(undefined4 *)(param_1 + 0xa70) = 0;\r\n      *(undefined8 *)(param_1 + 0xa64) = 0;\r\n      *(undefined4 *)(param_1 + 0xa6c) = 0x3f800000;\r\n      afStackX_10[0] = 0.0;\r\n      *(undefined2 *)(param_1 + 0xa74) = 0;\r\n      pcVar23 = (char *)(param_1 + 0x988);\r\n    }\r\n    else {\r\n      if ((undefined1 *)(param_1 + 0xa60) != (undefined1 *)(param_1 + 0x988)) {\r\n        *(undefined1 *)(param_1 + 0xa60) = *(undefined1 *)(param_1 + 0x988);\r\n      }\r\n      *(undefined4 *)(param_1 + 0xa64) = *(undefined4 *)(param_1 + 0x98c);\r\n      *(undefined4 *)(param_1 + 0xa68) = *(undefined4 *)(param_1 + 0x990);\r\n      *(undefined4 *)(param_1 + 0xa6c) = *(undefined4 *)(param_1 + 0x994);\r\n      *(undefined4 *)(param_1 + 0xa70) = *(undefined4 *)(param_1 + 0x998);\r\n      *(undefined1 *)(param_1 + 0xa74) = *(undefined1 *)(param_1 + 0x99c);\r\n      *(undefined1 *)(param_1 + 0xa75) = *(undefined1 *)(param_1 + 0x99d);\r\n      *(undefined1 *)(param_1 + 0xa76) = *(undefined1 *)(param_1 + 0x99e);\r\n      pcVar23 = (char *)0x0;\r\n    }\r\n  }\r\n  afStackX_18[0] = fVar4;\r\n  if (*(char *)(param_1 + 0xa75) != '\\0') {\r\n    if ((((*(uint *)(param_1 + 0x9b8) & 0xffffff) != 0) &&\r\n        (plVar20 = (longlong *)*plVar1, *plVar20 != 0)) && (*(int *)((longlong)plVar20 + 0x1c) < 3))\r\n    {\r\n      pfVar10 = (float *)FUN_140242c90(*plVar20);\r\n      afStackX_10[0] = *(float *)((longlong)plVar20 + 0x14);\r\n      if (afStackX_18[0] * *(float *)(plVar20 + 2) + afStackX_10[0] <\r\n          *pfVar10 - fRam000000014231c520) goto DAT_1406e4c27;\r\n    }\r\n    pcVar23 = (char *)(param_1 + 0xa60);\r\n  }\r\nDAT_1406e4c27:\r\n  if (pcVar23 == (char *)0x0) goto code_r0x0001406e4cd1;\r\n  cVar6 = *pcVar23;\r\n  pcVar11 = pcVar14;\r\n  if ((-1 < cVar6) && (0.0 < *(float *)(pcVar23 + 4))) {\r\n    pcVar11 = (char *)func_0x000140242c50(param_1 + 0xa78);\r\n  }\r\n  if (cVar6 < '\\0') {\r\n    if (pcVar11 != (char *)0x0) goto code_r0x0001406e4d87;\r\n    fVar8 = *(float *)(param_1 + 0xa88);\r\ncode_r0x0001406e4d8b:\r\n    uVar21 = (ulonglong)(*(uint *)(param_1 + 0x9b8) & 0xffffff);\r\n    pcVar19 = pcVar14;\r\n    afStackX_10[0] = fVar8;\r\n    if (3 < uVar21) {\r\n      lVar16 = (uVar21 - 4 >> 2) + 1;\r\n      pcVar19 = (char *)(lVar16 * 4);\r\n      do {\r\n        lVar18 = *plVar1;\r\n        if (*(int *)(pcVar14 + lVar18 + 0x20) == 2) {\r\n          afStackX_10[0] = fVar8;\r\n          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x18);\r\n          pfVar10 = afStackX_18;\r\n          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x18)) {\r\n            pfVar10 = afStackX_10;\r\n          }\r\n          *(float *)(pcVar14 + lVar18 + 0x18) = *pfVar10;\r\n        }\r\n        else {\r\n          *(float *)(pcVar14 + lVar18 + 0x18) = fVar8;\r\n          pcVar2 = pcVar14 + lVar18 + 0x20;\r\n          pcVar2[0] = '\\x02';\r\n          pcVar2[1] = '\\0';\r\n          pcVar2[2] = '\\0';\r\n          pcVar2[3] = '\\0';\r\n        }\r\n        lVar18 = *plVar1;\r\n        if (*(int *)(pcVar14 + lVar18 + 0x48) == 2) {\r\n          afStackX_10[0] = fVar8;\r\n          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x40);\r\n          pfVar10 = afStackX_18;\r\n          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x40)) {\r\n            pfVar10 = afStackX_10;\r\n          }\r\n          *(float *)(pcVar14 + lVar18 + 0x40) = *pfVar10;\r\n        }\r\n        else {\r\n          *(float *)(pcVar14 + lVar18 + 0x40) = fVar8;\r\n          pcVar2 = pcVar14 + lVar18 + 0x48;\r\n          pcVar2[0] = '\\x02';\r\n          pcVar2[1] = '\\0';\r\n          pcVar2[2] = '\\0';\r\n          pcVar2[3] = '\\0';\r\n        }\r\n        lVar18 = *plVar1;\r\n        if (*(int *)(pcVar14 + lVar18 + 0x70) == 2) {\r\n          afStackX_10[0] = fVar8;\r\n          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x68);\r\n          pfVar10 = afStackX_18;\r\n          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x68)) {\r\n            pfVar10 = afStackX_10;\r\n          }\r\n          *(float *)(pcVar14 + lVar18 + 0x68) = *pfVar10;\r\n        }\r\n        else {\r\n          *(float *)(pcVar14 + lVar18 + 0x68) = fVar8;\r\n          pcVar2 = pcVar14 + lVar18 + 0x70;\r\n          pcVar2[0] = '\\x02';\r\n          pcVar2[1] = '\\0';\r\n          pcVar2[2] = '\\0';\r\n          pcVar2[3] = '\\0';\r\n        }\r\n        lVar18 = *plVar1;\r\n        if (*(int *)(pcVar14 + lVar18 + 0x98) == 2) {\r\n          afStackX_10[0] = fVar8;\r\n          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x90);\r\n          pfVar10 = afStackX_18;\r\n          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x90)) {\r\n            pfVar10 = afStackX_10;\r\n          }\r\n          *(float *)(pcVar14 + lVar18 + 0x90) = *pfVar10;\r\n        }\r\n        else {\r\n          *(float *)(pcVar14 + lVar18 + 0x90) = fVar8;\r\n          pcVar2 = pcVar14 + lVar18 + 0x98;\r\n          pcVar2[0] = '\\x02';\r\n          pcVar2[1] = '\\0';\r\n          pcVar2[2] = '\\0';\r\n          pcVar2[3] = '\\0';\r\n        }\r\n        pcVar14 = pcVar14 + 0xa0;\r\n        lVar16 = lVar16 + -1;\r\n      } while (lVar16 != 0);\r\n    }\r\n    if ((longlong)pcVar19 < (longlong)uVar21) {\r\n      lVar16 = (longlong)pcVar19 * 0x28;\r\n      lVar18 = uVar21 - (longlong)pcVar19;\r\n      do {\r\n        lVar5 = *plVar1;\r\n        if (*(int *)(lVar16 + 0x20 + lVar5) == 2) {\r\n          afStackX_18[0] = *(float *)(lVar16 + 0x18 + lVar5);\r\n          afStackX_10[0] = fVar8;\r\n          pfVar10 = afStackX_18;\r\n          if (fVar8 <= afStackX_18[0]) {\r\n            pfVar10 = afStackX_10;\r\n          }\r\n          *(float *)(lVar16 + 0x18 + lVar5) = *pfVar10;\r\n        }\r\n        else {\r\n          *(float *)(lVar16 + 0x18 + lVar5) = fVar8;\r\n          *(undefined4 *)(lVar16 + 0x20 + lVar5) = 2;\r\n        }\r\n        lVar16 = lVar16 + 0x28;\r\n        lVar18 = lVar18 + -1;\r\n      } while (lVar18 != 0);\r\n    }\r\n    if (pcVar11 == (char *)0x0) goto code_r0x0001406e4c75;\r\n    uVar9 = *(uint *)(param_1 + 0x9b8);\r\n    if ((uVar9 & 0xffffff) == 4) {\r\n      uVar9 = (uVar9 - 1 ^ uVar9) & 0xffffff ^ uVar9;\r\n      *(uint *)(param_1 + 0x9b8) = uVar9;\r\n      if ((uVar9 & 0xffffff) != 3) {\r\n        lVar16 = *plVar1;\r\n        uVar21 = (ulonglong)(uVar9 & 0xffffff);\r\n        *(undefined8 *)(lVar16 + 0x78) = *(undefined8 *)(lVar16 + uVar21 * 0x28);\r\n        *(undefined8 *)(lVar16 + 0x80) = *(undefined8 *)(lVar16 + 8 + uVar21 * 0x28);\r\n        *(undefined8 *)(lVar16 + 0x88) = *(undefined8 *)(lVar16 + 0x10 + uVar21 * 0x28);\r\n        *(undefined8 *)(lVar16 + 0x90) = *(undefined8 *)(lVar16 + 0x18 + uVar21 * 0x28);\r\n        *(undefined8 *)(lVar16 + 0x98) = *(undefined8 *)(lVar16 + 0x20 + uVar21 * 0x28);\r\n      }\r\n    }\r\n    afStackX_18[0] = *(float *)(pcVar23 + 0x10);\r\n    iVar22 = *(int *)(pcVar23 + 8);\r\n    uVar26 = *(undefined4 *)(pcVar23 + 0xc);\r\n    uVar3 = *(undefined4 *)(pcVar23 + 4);\r\n    pfVar12 = (float *)FUN_140242c90(pcVar11,afStackX_10);\r\n    pfVar10 = afStackX_10;\r\n    if (iVar22 != 4) {\r\n      pfVar10 = afStackX_18;\r\n    }\r\n    afStackX_10[0] = *pfVar12 - fRam000000014231c520;\r\n    fStack_84 = *pfVar10;\r\n    uStack_8c = 0;\r\n    uStack_80 = *(undefined4 *)(pcVar11 + 0x38);\r\n    uStack_78 = 1;\r\n    uStack_74 = 1 < iVar22 - 3U;\r\n    pcStack_98 = pcVar11;\r\n    uStack_90 = uVar3;\r\n    uStack_88 = uVar26;\r\n    iStack_7c = iVar22;\r\n    FUN_1406e3fa0(plVar1,&pcStack_98,0);\r\n  }\r\n  else {\r\n    if (pcVar11 != (char *)0x0) {\r\ncode_r0x0001406e4d87:\r\n      fVar8 = *(float *)(pcVar11 + 0x38);\r\n      goto code_r0x0001406e4d8b;\r\n    }\r\ncode_r0x0001406e4c75:\r\n    if (-1 < cVar6) {\r\n                    /* WARNING: Subroutine does not return */\r\n      FUN_140166830(&UNK_141c4bd58,(int)cVar6,*(undefined8 *)(*(longlong *)(param_1 + 8) + 8));\r\n    }\r\n  }\r\n  *(char *)(param_1 + 0xa76) = *(char *)(param_1 + 0xa76) + '\\x01';\r\n  afStackX_10[0] = 0.0;\r\n  *(undefined4 *)(param_1 + 0xa70) = 0;\r\n  *(undefined1 *)(param_1 + 0xa60) = 0xff;\r\n  *(undefined8 *)(param_1 + 0xa64) = 0;\r\n  *(undefined4 *)(param_1 + 0xa6c) = 0x3f800000;\r\n  *(undefined2 *)(param_1 + 0xa74) = 0;\r\ncode_r0x0001406e4cd1:\r\n  uVar21 = 0;\r\n  bVar13 = 0;\r\n  afStackX_10[0] = (float)((uint)afStackX_10[0] & 0xffffff00);\r\n  iVar22 = 0;\r\n  if ((*(uint *)(param_1 + 0x9b8) & 0xffffff) != 0) {\r\n    puVar25 = (undefined4 *)(param_1 + 0xa98);\r\n    uVar24 = uVar21;\r\n    do {\r\n      plVar20 = (longlong *)(*plVar1 + uVar21);\r\n      cVar6 = FUN_1406e4850(plVar20,fVar4);\r\n      iVar22 = (int)uVar24;\r\n      if (cVar6 == '\\0') {\r\n        if (((-1 < iVar22) && (uVar9 = *(uint *)(param_1 + 0x9b8), iVar22 < (int)(uVar9 & 0xffffff))\r\n            ) && (uVar9 = (uVar9 - 1 ^ uVar9) & 0xffffff ^ uVar9, *(uint *)(param_1 + 0x9b8) = uVar9\r\n                 , uVar15 = uVar21, uVar17 = uVar24, iVar22 < (int)(uVar9 & 0xffffff))) {\r\n          do {\r\n            lVar16 = *plVar1;\r\n            uVar9 = (int)uVar17 + 1;\r\n            *(undefined8 *)(uVar15 + lVar16) = *(undefined8 *)(uVar15 + 0x28 + lVar16);\r\n            *(undefined8 *)(uVar15 + 8 + lVar16) = *(undefined8 *)(uVar15 + 0x30 + lVar16);\r\n            *(undefined8 *)(uVar15 + 0x10 + lVar16) = *(undefined8 *)(uVar15 + 0x38 + lVar16);\r\n            *(undefined8 *)(uVar15 + 0x18 + lVar16) = *(undefined8 *)(uVar15 + 0x40 + lVar16);\r\n            *(undefined8 *)(uVar15 + 0x20 + lVar16) = *(undefined8 *)(uVar15 + 0x48 + lVar16);\r\n            uVar15 = uVar15 + 0x28;\r\n            uVar17 = (ulonglong)uVar9;\r\n          } while ((int)uVar9 < (int)(*(uint *)(param_1 + 0x9b8) & 0xffffff));\r\n        }\r\n        bVar13 = afStackX_10[0]._0_1_;\r\n      }\r\n      else {\r\n        lVar16 = *plVar20;\r\n        *(longlong *)(puVar25 + -2) = lVar16;\r\n        afStackX_18[0] = *(float *)((longlong)plVar20 + 0x14);\r\n        iVar7 = (**(code **)(**(longlong **)(lVar16 + 0x30) + 0x90))();\r\n        puVar25[1] = (float)iVar7 * afStackX_18[0];\r\n        *puVar25 = *(undefined4 *)((longlong)plVar20 + 0xc);\r\n        puVar25[2] = *(undefined4 *)(*(longlong *)(lVar16 + 0x30) + 0x3c);\r\n        if ((*(char *)((longlong)plVar20 + 0x24) == '\\0') && ((int)plVar20[4] == 0)) {\r\n          bVar13 = 0;\r\n        }\r\n        else {\r\n          bVar13 = 1;\r\n        }\r\n        uVar24 = (ulonglong)(iVar22 + 1);\r\n        puVar25 = puVar25 + 0xe;\r\n        bVar13 = afStackX_10[0]._0_1_ | bVar13;\r\n        uVar21 = uVar21 + 0x28;\r\n        afStackX_10[0] = (float)CONCAT31(afStackX_10[0]._1_3_,bVar13);\r\n      }\r\n      iVar22 = (int)uVar24;\r\n    } while (iVar22 < (int)(*(uint *)(param_1 + 0x9b8) & 0xffffff));\r\n  }\r\n  if ((*(uint *)(param_1 + 0x9b8) & 0xffffff) == 0) {\r\n    uVar26 = 0x7149f2ca;\r\n  }\r\n  else {\r\n    uVar26 = *(undefined4 *)(*plVar1 + 0x14);\r\n  }\r\n  *(byte *)(param_1 + 0x9ac) = bVar13;\r\n  lVar16 = (longlong)iVar22;\r\n  *(undefined4 *)(param_1 + 0x9a8) = uVar26;\r\n  if (lVar16 < 4) {\r\n    param_1 = lVar16 * 0x38 + 0xa9c + param_1;\r\n    lVar16 = 4 - lVar16;\r\n    do {\r\n      *(undefined8 *)(param_1 + -0xc) = 0;\r\n      *(undefined8 *)(param_1 + -4) = 0;\r\n      *(undefined4 *)(param_1 + 4) = 7;\r\n      *(undefined8 *)(param_1 + 0x10) = 0x3f800000;\r\n      *(undefined1 *)(param_1 + 0x18) = 0;\r\n      *(undefined4 *)(param_1 + 0x1c) = 4;\r\n      *(undefined8 *)(param_1 + 0x24) = 0;\r\n      param_1 = param_1 + 0x38;\r\n      lVar16 = lVar16 + -1;\r\n    } while (lVar16 != 0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
  },
  "Advance": {
    "result": "\r\nundefined8 FUN_1406e4850(longlong *param_1,float param_2)\r\n\r\n{\r\n  bool bVar1;\r\n  float *pfVar2;\r\n  ulonglong uVar3;\r\n  ulonglong uVar4;\r\n  uint7 uVar5;\r\n  int iVar6;\r\n  float fVar7;\r\n  float fVar8;\r\n  float fVar9;\r\n  float fVar10;\r\n  float fStackX_8;\r\n  \r\n  pfVar2 = (float *)FUN_140242c90(*param_1);\r\n  iVar6 = (int)param_1[4];\r\n  fVar9 = *pfVar2 - fRam000000014231c520;\r\n  if ((iVar6 == 1) && (*(float *)((longlong)param_1 + 0xc) == 0.0)) {\r\n    bVar1 = true;\r\n  }\r\n  else {\r\n    bVar1 = false;\r\n  }\r\n  if ((*(char *)((longlong)param_1 + 0x24) == '\\0') || (bVar1)) {\r\n    fStackX_8 = *(float *)((longlong)param_1 + 0x14);\r\n  }\r\n  else {\r\n    fStackX_8 = param_2 * *(float *)(param_1 + 2) + *(float *)((longlong)param_1 + 0x14);\r\n  }\r\n  uVar3 = (ulonglong)(uint)fStackX_8;\r\n  if (fVar9 < fStackX_8) {\r\n    if (*(int *)((longlong)param_1 + 0x1c) < 2) {\r\n      uVar3 = (ulonglong)(uint)fVar9;\r\n      *(undefined1 *)((longlong)param_1 + 0x24) = 0;\r\n      fStackX_8 = fVar9;\r\n    }\r\n    else if (*(int *)((longlong)param_1 + 0x1c) == 2) {\r\n      fStackX_8 = (float)FUN_1411af130(fStackX_8,fVar9);\r\n      uVar3 = (ulonglong)(uint)fStackX_8;\r\n    }\r\n  }\r\n  fVar10 = *(float *)(param_1 + 3);\r\n  if (((iVar6 != 2) && (*(int *)((longlong)param_1 + 0x1c) == 0)) &&\r\n     (fVar9 - *(float *)(*param_1 + 0x3c) < fStackX_8 + 1.1920929e-07)) {\r\n    fVar10 = fVar9 - *(float *)((longlong)param_1 + 0x14);\r\n    iVar6 = 2;\r\n  }\r\n  *(int *)((longlong)param_1 + 0x14) = (int)uVar3;\r\n  uVar4 = uVar3 & 0xffffffffffffff00;\r\n  bVar1 = false;\r\n  if (iVar6 == 0) goto code_r0x0001406e4a05;\r\n  fVar9 = *(float *)((longlong)param_1 + 0xc);\r\n  fVar7 = fVar10 - param_2;\r\n  if (iVar6 == 1) {\r\n    if (0.0 < fVar7) {\r\n      fVar8 = *(float *)(param_1 + 1);\r\n      fVar9 = ((fVar8 - fVar9) / fVar10) * param_2 + fVar9;\r\n      if (fVar9 <= fVar8) {\r\n        fVar8 = fVar9;\r\n      }\r\n    }\r\n    else {\r\n      iVar6 = 0;\r\n      fVar8 = *(float *)(param_1 + 1);\r\n    }\r\ncode_r0x0001406e49f6:\r\n    bVar1 = false;\r\n    *(float *)((longlong)param_1 + 0xc) = fVar8;\r\n    *(int *)(param_1 + 4) = iVar6;\r\n    *(float *)(param_1 + 3) = fVar7;\r\n  }\r\n  else {\r\n    uVar5 = (uint7)(uVar3 >> 8);\r\n    if (0.0 < fVar7) {\r\n      fVar9 = fVar9 - (fVar9 / fVar10) * param_2;\r\n      fVar8 = 0.0;\r\n      if (0.0 <= fVar9) {\r\n        fVar8 = fVar9;\r\n      }\r\n      if (fVar8 != 0.0) {\r\n        uVar4 = (ulonglong)uVar5 << 8;\r\n        goto code_r0x0001406e49f6;\r\n      }\r\n    }\r\n    uVar4 = CONCAT71(uVar5,1);\r\n    bVar1 = true;\r\n    *param_1 = 0;\r\n  }\r\ncode_r0x0001406e4a05:\r\n  return CONCAT71((int7)(uVar4 >> 8),!bVar1);\r\n}\r\n\r\n"
  },
  "RuntimeResolution": "888A verifies float1.2 seconds/rate0 held reference on both rigs. 9165 verifies complete crouch composition mechanically on both roles. Reports distinguish mechanical pass from unfinished visual quality."
}

````

## Unescaped text fields for reading

### result

````text

void FUN_1406e4af0(longlong param_1,char param_2)

{
  longlong *plVar1;
  char *pcVar2;
  undefined4 uVar3;
  float fVar4;
  longlong lVar5;
  char cVar6;
  int iVar7;
  float fVar8;
  uint uVar9;
  float *pfVar10;
  char *pcVar11;
  float *pfVar12;
  byte bVar13;
  char *pcVar14;
  ulonglong uVar15;
  longlong lVar16;
  ulonglong uVar17;
  longlong lVar18;
  char *pcVar19;
  longlong *plVar20;
  ulonglong uVar21;
  int iVar22;
  char *pcVar23;
  ulonglong uVar24;
  undefined4 *puVar25;
  undefined4 uVar26;
  float afStackX_10 [2];
  float afStackX_18 [4];
  char *pcStack_98;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined4 uStack_88;
  float fStack_84;
  undefined4 uStack_80;
  int iStack_7c;
  undefined4 uStack_78;
  undefined1 uStack_74;
  
  pcVar14 = (char *)0x0;
  fVar4 = *(float *)(*(longlong *)(param_1 + 0x10) + 8);
  plVar1 = (longlong *)(param_1 + 0x9b0);
  pcVar23 = pcVar14;
  if (param_2 != '\0') {
    if (*(char *)(param_1 + 0x99c) == '\0') {
      *(char *)(param_1 + 0xa76) = *(char *)(param_1 + 0xa76) + '\x01';
      *(undefined1 *)(param_1 + 0xa60) = 0xff;
      *(undefined4 *)(param_1 + 0xa70) = 0;
      *(undefined8 *)(param_1 + 0xa64) = 0;
      *(undefined4 *)(param_1 + 0xa6c) = 0x3f800000;
      afStackX_10[0] = 0.0;
      *(undefined2 *)(param_1 + 0xa74) = 0;
      pcVar23 = (char *)(param_1 + 0x988);
    }
    else {
      if ((undefined1 *)(param_1 + 0xa60) != (undefined1 *)(param_1 + 0x988)) {
        *(undefined1 *)(param_1 + 0xa60) = *(undefined1 *)(param_1 + 0x988);
      }
      *(undefined4 *)(param_1 + 0xa64) = *(undefined4 *)(param_1 + 0x98c);
      *(undefined4 *)(param_1 + 0xa68) = *(undefined4 *)(param_1 + 0x990);
      *(undefined4 *)(param_1 + 0xa6c) = *(undefined4 *)(param_1 + 0x994);
      *(undefined4 *)(param_1 + 0xa70) = *(undefined4 *)(param_1 + 0x998);
      *(undefined1 *)(param_1 + 0xa74) = *(undefined1 *)(param_1 + 0x99c);
      *(undefined1 *)(param_1 + 0xa75) = *(undefined1 *)(param_1 + 0x99d);
      *(undefined1 *)(param_1 + 0xa76) = *(undefined1 *)(param_1 + 0x99e);
      pcVar23 = (char *)0x0;
    }
  }
  afStackX_18[0] = fVar4;
  if (*(char *)(param_1 + 0xa75) != '\0') {
    if ((((*(uint *)(param_1 + 0x9b8) & 0xffffff) != 0) &&
        (plVar20 = (longlong *)*plVar1, *plVar20 != 0)) && (*(int *)((longlong)plVar20 + 0x1c) < 3))
    {
      pfVar10 = (float *)FUN_140242c90(*plVar20);
      afStackX_10[0] = *(float *)((longlong)plVar20 + 0x14);
      if (afStackX_18[0] * *(float *)(plVar20 + 2) + afStackX_10[0] <
          *pfVar10 - fRam000000014231c520) goto DAT_1406e4c27;
    }
    pcVar23 = (char *)(param_1 + 0xa60);
  }
DAT_1406e4c27:
  if (pcVar23 == (char *)0x0) goto code_r0x0001406e4cd1;
  cVar6 = *pcVar23;
  pcVar11 = pcVar14;
  if ((-1 < cVar6) && (0.0 < *(float *)(pcVar23 + 4))) {
    pcVar11 = (char *)func_0x000140242c50(param_1 + 0xa78);
  }
  if (cVar6 < '\0') {
    if (pcVar11 != (char *)0x0) goto code_r0x0001406e4d87;
    fVar8 = *(float *)(param_1 + 0xa88);
code_r0x0001406e4d8b:
    uVar21 = (ulonglong)(*(uint *)(param_1 + 0x9b8) & 0xffffff);
    pcVar19 = pcVar14;
    afStackX_10[0] = fVar8;
    if (3 < uVar21) {
      lVar16 = (uVar21 - 4 >> 2) + 1;
      pcVar19 = (char *)(lVar16 * 4);
      do {
        lVar18 = *plVar1;
        if (*(int *)(pcVar14 + lVar18 + 0x20) == 2) {
          afStackX_10[0] = fVar8;
          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x18);
          pfVar10 = afStackX_18;
          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x18)) {
            pfVar10 = afStackX_10;
          }
          *(float *)(pcVar14 + lVar18 + 0x18) = *pfVar10;
        }
        else {
          *(float *)(pcVar14 + lVar18 + 0x18) = fVar8;
          pcVar2 = pcVar14 + lVar18 + 0x20;
          pcVar2[0] = '\x02';
          pcVar2[1] = '\0';
          pcVar2[2] = '\0';
          pcVar2[3] = '\0';
        }
        lVar18 = *plVar1;
        if (*(int *)(pcVar14 + lVar18 + 0x48) == 2) {
          afStackX_10[0] = fVar8;
          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x40);
          pfVar10 = afStackX_18;
          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x40)) {
            pfVar10 = afStackX_10;
          }
          *(float *)(pcVar14 + lVar18 + 0x40) = *pfVar10;
        }
        else {
          *(float *)(pcVar14 + lVar18 + 0x40) = fVar8;
          pcVar2 = pcVar14 + lVar18 + 0x48;
          pcVar2[0] = '\x02';
          pcVar2[1] = '\0';
          pcVar2[2] = '\0';
          pcVar2[3] = '\0';
        }
        lVar18 = *plVar1;
        if (*(int *)(pcVar14 + lVar18 + 0x70) == 2) {
          afStackX_10[0] = fVar8;
          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x68);
          pfVar10 = afStackX_18;
          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x68)) {
            pfVar10 = afStackX_10;
          }
          *(float *)(pcVar14 + lVar18 + 0x68) = *pfVar10;
        }
        else {
          *(float *)(pcVar14 + lVar18 + 0x68) = fVar8;
          pcVar2 = pcVar14 + lVar18 + 0x70;
          pcVar2[0] = '\x02';
          pcVar2[1] = '\0';
          pcVar2[2] = '\0';
          pcVar2[3] = '\0';
        }
        lVar18 = *plVar1;
        if (*(int *)(pcVar14 + lVar18 + 0x98) == 2) {
          afStackX_10[0] = fVar8;
          afStackX_18[0] = *(float *)(pcVar14 + lVar18 + 0x90);
          pfVar10 = afStackX_18;
          if (fVar8 <= *(float *)(pcVar14 + lVar18 + 0x90)) {
            pfVar10 = afStackX_10;
          }
          *(float *)(pcVar14 + lVar18 + 0x90) = *pfVar10;
        }
        else {
          *(float *)(pcVar14 + lVar18 + 0x90) = fVar8;
          pcVar2 = pcVar14 + lVar18 + 0x98;
          pcVar2[0] = '\x02';
          pcVar2[1] = '\0';
          pcVar2[2] = '\0';
          pcVar2[3] = '\0';
        }
        pcVar14 = pcVar14 + 0xa0;
        lVar16 = lVar16 + -1;
      } while (lVar16 != 0);
    }
    if ((longlong)pcVar19 < (longlong)uVar21) {
      lVar16 = (longlong)pcVar19 * 0x28;
      lVar18 = uVar21 - (longlong)pcVar19;
      do {
        lVar5 = *plVar1;
        if (*(int *)(lVar16 + 0x20 + lVar5) == 2) {
          afStackX_18[0] = *(float *)(lVar16 + 0x18 + lVar5);
          afStackX_10[0] = fVar8;
          pfVar10 = afStackX_18;
          if (fVar8 <= afStackX_18[0]) {
            pfVar10 = afStackX_10;
          }
          *(float *)(lVar16 + 0x18 + lVar5) = *pfVar10;
        }
        else {
          *(float *)(lVar16 + 0x18 + lVar5) = fVar8;
          *(undefined4 *)(lVar16 + 0x20 + lVar5) = 2;
        }
        lVar16 = lVar16 + 0x28;
        lVar18 = lVar18 + -1;
      } while (lVar18 != 0);
    }
    if (pcVar11 == (char *)0x0) goto code_r0x0001406e4c75;
    uVar9 = *(uint *)(param_1 + 0x9b8);
    if ((uVar9 & 0xffffff) == 4) {
      uVar9 = (uVar9 - 1 ^ uVar9) & 0xffffff ^ uVar9;
      *(uint *)(param_1 + 0x9b8) = uVar9;
      if ((uVar9 & 0xffffff) != 3) {
        lVar16 = *plVar1;
        uVar21 = (ulonglong)(uVar9 & 0xffffff);
        *(undefined8 *)(lVar16 + 0x78) = *(undefined8 *)(lVar16 + uVar21 * 0x28);
        *(undefined8 *)(lVar16 + 0x80) = *(undefined8 *)(lVar16 + 8 + uVar21 * 0x28);
        *(undefined8 *)(lVar16 + 0x88) = *(undefined8 *)(lVar16 + 0x10 + uVar21 * 0x28);
        *(undefined8 *)(lVar16 + 0x90) = *(undefined8 *)(lVar16 + 0x18 + uVar21 * 0x28);
        *(undefined8 *)(lVar16 + 0x98) = *(undefined8 *)(lVar16 + 0x20 + uVar21 * 0x28);
      }
    }
    afStackX_18[0] = *(float *)(pcVar23 + 0x10);
    iVar22 = *(int *)(pcVar23 + 8);
    uVar26 = *(undefined4 *)(pcVar23 + 0xc);
    uVar3 = *(undefined4 *)(pcVar23 + 4);
    pfVar12 = (float *)FUN_140242c90(pcVar11,afStackX_10);
    pfVar10 = afStackX_10;
    if (iVar22 != 4) {
      pfVar10 = afStackX_18;
    }
    afStackX_10[0] = *pfVar12 - fRam000000014231c520;
    fStack_84 = *pfVar10;
    uStack_8c = 0;
    uStack_80 = *(undefined4 *)(pcVar11 + 0x38);
    uStack_78 = 1;
    uStack_74 = 1 < iVar22 - 3U;
    pcStack_98 = pcVar11;
    uStack_90 = uVar3;
    uStack_88 = uVar26;
    iStack_7c = iVar22;
    FUN_1406e3fa0(plVar1,&pcStack_98,0);
  }
  else {
    if (pcVar11 != (char *)0x0) {
code_r0x0001406e4d87:
      fVar8 = *(float *)(pcVar11 + 0x38);
      goto code_r0x0001406e4d8b;
    }
code_r0x0001406e4c75:
    if (-1 < cVar6) {
                    /* WARNING: Subroutine does not return */
      FUN_140166830(&UNK_141c4bd58,(int)cVar6,*(undefined8 *)(*(longlong *)(param_1 + 8) + 8));
    }
  }
  *(char *)(param_1 + 0xa76) = *(char *)(param_1 + 0xa76) + '\x01';
  afStackX_10[0] = 0.0;
  *(undefined4 *)(param_1 + 0xa70) = 0;
  *(undefined1 *)(param_1 + 0xa60) = 0xff;
  *(undefined8 *)(param_1 + 0xa64) = 0;
  *(undefined4 *)(param_1 + 0xa6c) = 0x3f800000;
  *(undefined2 *)(param_1 + 0xa74) = 0;
code_r0x0001406e4cd1:
  uVar21 = 0;
  bVar13 = 0;
  afStackX_10[0] = (float)((uint)afStackX_10[0] & 0xffffff00);
  iVar22 = 0;
  if ((*(uint *)(param_1 + 0x9b8) & 0xffffff) != 0) {
    puVar25 = (undefined4 *)(param_1 + 0xa98);
    uVar24 = uVar21;
    do {
      plVar20 = (longlong *)(*plVar1 + uVar21);
      cVar6 = FUN_1406e4850(plVar20,fVar4);
      iVar22 = (int)uVar24;
      if (cVar6 == '\0') {
        if (((-1 < iVar22) && (uVar9 = *(uint *)(param_1 + 0x9b8), iVar22 < (int)(uVar9 & 0xffffff))
            ) && (uVar9 = (uVar9 - 1 ^ uVar9) & 0xffffff ^ uVar9, *(uint *)(param_1 + 0x9b8) = uVar9
                 , uVar15 = uVar21, uVar17 = uVar24, iVar22 < (int)(uVar9 & 0xffffff))) {
          do {
            lVar16 = *plVar1;
            uVar9 = (int)uVar17 + 1;
            *(undefined8 *)(uVar15 + lVar16) = *(undefined8 *)(uVar15 + 0x28 + lVar16);
            *(undefined8 *)(uVar15 + 8 + lVar16) = *(undefined8 *)(uVar15 + 0x30 + lVar16);
            *(undefined8 *)(uVar15 + 0x10 + lVar16) = *(undefined8 *)(uVar15 + 0x38 + lVar16);
            *(undefined8 *)(uVar15 + 0x18 + lVar16) = *(undefined8 *)(uVar15 + 0x40 + lVar16);
            *(undefined8 *)(uVar15 + 0x20 + lVar16) = *(undefined8 *)(uVar15 + 0x48 + lVar16);
            uVar15 = uVar15 + 0x28;
            uVar17 = (ulonglong)uVar9;
          } while ((int)uVar9 < (int)(*(uint *)(param_1 + 0x9b8) & 0xffffff));
        }
        bVar13 = afStackX_10[0]._0_1_;
      }
      else {
        lVar16 = *plVar20;
        *(longlong *)(puVar25 + -2) = lVar16;
        afStackX_18[0] = *(float *)((longlong)plVar20 + 0x14);
        iVar7 = (**(code **)(**(longlong **)(lVar16 + 0x30) + 0x90))();
        puVar25[1] = (float)iVar7 * afStackX_18[0];
        *puVar25 = *(undefined4 *)((longlong)plVar20 + 0xc);
        puVar25[2] = *(undefined4 *)(*(longlong *)(lVar16 + 0x30) + 0x3c);
        if ((*(char *)((longlong)plVar20 + 0x24) == '\0') && ((int)plVar20[4] == 0)) {
          bVar13 = 0;
        }
        else {
          bVar13 = 1;
        }
        uVar24 = (ulonglong)(iVar22 + 1);
        puVar25 = puVar25 + 0xe;
        bVar13 = afStackX_10[0]._0_1_ | bVar13;
        uVar21 = uVar21 + 0x28;
        afStackX_10[0] = (float)CONCAT31(afStackX_10[0]._1_3_,bVar13);
      }
      iVar22 = (int)uVar24;
    } while (iVar22 < (int)(*(uint *)(param_1 + 0x9b8) & 0xffffff));
  }
  if ((*(uint *)(param_1 + 0x9b8) & 0xffffff) == 0) {
    uVar26 = 0x7149f2ca;
  }
  else {
    uVar26 = *(undefined4 *)(*plVar1 + 0x14);
  }
  *(byte *)(param_1 + 0x9ac) = bVar13;
  lVar16 = (longlong)iVar22;
  *(undefined4 *)(param_1 + 0x9a8) = uVar26;
  if (lVar16 < 4) {
    param_1 = lVar16 * 0x38 + 0xa9c + param_1;
    lVar16 = 4 - lVar16;
    do {
      *(undefined8 *)(param_1 + -0xc) = 0;
      *(undefined8 *)(param_1 + -4) = 0;
      *(undefined4 *)(param_1 + 4) = 7;
      *(undefined8 *)(param_1 + 0x10) = 0x3f800000;
      *(undefined1 *)(param_1 + 0x18) = 0;
      *(undefined4 *)(param_1 + 0x1c) = 4;
      *(undefined8 *)(param_1 + 0x24) = 0;
      param_1 = param_1 + 0x38;
      lVar16 = lVar16 + -1;
    } while (lVar16 != 0);
  }
  return;
}


````

### result

````text

undefined8 FUN_1406e4850(longlong *param_1,float param_2)

{
  bool bVar1;
  float *pfVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  uint7 uVar5;
  int iVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fStackX_8;
  
  pfVar2 = (float *)FUN_140242c90(*param_1);
  iVar6 = (int)param_1[4];
  fVar9 = *pfVar2 - fRam000000014231c520;
  if ((iVar6 == 1) && (*(float *)((longlong)param_1 + 0xc) == 0.0)) {
    bVar1 = true;
  }
  else {
    bVar1 = false;
  }
  if ((*(char *)((longlong)param_1 + 0x24) == '\0') || (bVar1)) {
    fStackX_8 = *(float *)((longlong)param_1 + 0x14);
  }
  else {
    fStackX_8 = param_2 * *(float *)(param_1 + 2) + *(float *)((longlong)param_1 + 0x14);
  }
  uVar3 = (ulonglong)(uint)fStackX_8;
  if (fVar9 < fStackX_8) {
    if (*(int *)((longlong)param_1 + 0x1c) < 2) {
      uVar3 = (ulonglong)(uint)fVar9;
      *(undefined1 *)((longlong)param_1 + 0x24) = 0;
      fStackX_8 = fVar9;
    }
    else if (*(int *)((longlong)param_1 + 0x1c) == 2) {
      fStackX_8 = (float)FUN_1411af130(fStackX_8,fVar9);
      uVar3 = (ulonglong)(uint)fStackX_8;
    }
  }
  fVar10 = *(float *)(param_1 + 3);
  if (((iVar6 != 2) && (*(int *)((longlong)param_1 + 0x1c) == 0)) &&
     (fVar9 - *(float *)(*param_1 + 0x3c) < fStackX_8 + 1.1920929e-07)) {
    fVar10 = fVar9 - *(float *)((longlong)param_1 + 0x14);
    iVar6 = 2;
  }
  *(int *)((longlong)param_1 + 0x14) = (int)uVar3;
  uVar4 = uVar3 & 0xffffffffffffff00;
  bVar1 = false;
  if (iVar6 == 0) goto code_r0x0001406e4a05;
  fVar9 = *(float *)((longlong)param_1 + 0xc);
  fVar7 = fVar10 - param_2;
  if (iVar6 == 1) {
    if (0.0 < fVar7) {
      fVar8 = *(float *)(param_1 + 1);
      fVar9 = ((fVar8 - fVar9) / fVar10) * param_2 + fVar9;
      if (fVar9 <= fVar8) {
        fVar8 = fVar9;
      }
    }
    else {
      iVar6 = 0;
      fVar8 = *(float *)(param_1 + 1);
    }
code_r0x0001406e49f6:
    bVar1 = false;
    *(float *)((longlong)param_1 + 0xc) = fVar8;
    *(int *)(param_1 + 4) = iVar6;
    *(float *)(param_1 + 3) = fVar7;
  }
  else {
    uVar5 = (uint7)(uVar3 >> 8);
    if (0.0 < fVar7) {
      fVar9 = fVar9 - (fVar9 / fVar10) * param_2;
      fVar8 = 0.0;
      if (0.0 <= fVar9) {
        fVar8 = fVar9;
      }
      if (fVar8 != 0.0) {
        uVar4 = (ulonglong)uVar5 << 8;
        goto code_r0x0001406e49f6;
      }
    }
    uVar4 = CONCAT71(uVar5,1);
    bVar1 = true;
    *param_1 = 0;
  }
code_r0x0001406e4a05:
  return CONCAT71((int7)(uVar4 >> 8),!bVar1);
}


````
