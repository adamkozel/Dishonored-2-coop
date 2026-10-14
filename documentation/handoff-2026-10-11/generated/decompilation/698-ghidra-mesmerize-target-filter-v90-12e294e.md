# ghidra-mesmerize-target-filter-v90.json

Original: [work/ghidra-mesmerize-target-filter-v90.json](<../../../../work/ghidra-mesmerize-target-filter-v90.json>)

SHA-256: `13EAB39097A2BB7440E7B3F2EE00E1E4BE3353762017401BB732FA7D83104B06`; 59959 bytes.

## Original saved record

````json
[
  {
    "address": "14072d6e0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14072d6e0(longlong param_1)\r\n\r\n{\r\n  int *piVar1;\r\n  undefined4 uVar2;\r\n  int *piVar3;\r\n  bool bVar4;\r\n  char cVar5;\r\n  uint uVar6;\r\n  longlong *plVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  undefined8 uVar10;\r\n  ulonglong uVar11;\r\n  int iVar12;\r\n  float *pfVar13;\r\n  ulonglong uVar14;\r\n  ulonglong uVar15;\r\n  int iVar16;\r\n  uint uVar17;\r\n  longlong lVar18;\r\n  ulonglong uVar19;\r\n  uint uVar20;\r\n  int *piVar21;\r\n  int iVar22;\r\n  float fVar23;\r\n  float fVar24;\r\n  float fVar25;\r\n  float fVar26;\r\n  float fVar27;\r\n  float afStackX_8 [2];\r\n  float afStackX_10 [2];\r\n  float fStack_168;\r\n  float fStack_164;\r\n  float fStack_160;\r\n  undefined8 uStack_158;\r\n  uint uStack_150;\r\n  uint uStack_14c;\r\n  longlong lStack_148;\r\n  undefined4 uStack_140;\r\n  undefined4 uStack_13c;\r\n  undefined4 uStack_138;\r\n  undefined4 uStack_134;\r\n  undefined4 uStack_130;\r\n  undefined4 uStack_12c;\r\n  undefined4 uStack_128;\r\n  undefined4 uStack_124;\r\n  undefined4 uStack_120;\r\n  undefined8 uStack_118;\r\n  undefined1 auStack_110 [80];\r\n  undefined *puStack_c0;\r\n  undefined8 uStack_98;\r\n  undefined1 *puStack_88;\r\n  ulonglong uStack_80;\r\n  undefined1 auStack_78 [64];\r\n  \r\n  uStack_118 = 0xfffffffffffffffe;\r\n  FUN_1406debc0(param_1 + 0x1f0);\r\n  FUN_140729fb0(param_1);\r\n  bVar4 = false;\r\n  if (*(int *)(param_1 + 0x2c) == 3) {\r\n    lVar8 = 0;\r\n    if (((*(uint *)(param_1 + 0x1a0) & 0xffffff) != 0) &&\r\n       (lVar8 = 0, *(char *)((longlong)*(undefined4 **)(param_1 + 0x198) + 0x6e) != '\\0')) {\r\n      lVar8 = FUN_1406dc560(**(undefined4 **)(param_1 + 0x198));\r\n    }\r\n    iVar16 = 0;\r\n    if ((*(uint *)(param_1 + 0x210) & 0xffffff) != 0) {\r\n      lVar18 = 0;\r\n      do {\r\n        afStackX_8[0] = *(float *)(lVar18 + *(longlong *)(param_1 + 0x208));\r\n        lVar9 = FUN_1407b5e00(D2_GameSingletonPtr + 0x62616,afStackX_8);\r\n        if (lVar9 == lVar8) {\r\n          cVar5 = FUN_14072a400(param_1,lVar9,0);\r\n          if (cVar5 != '\\0') {\r\n            bVar4 = true;\r\n            goto code_r0x00014072d7f0;\r\n          }\r\n          break;\r\n        }\r\n        iVar16 = iVar16 + 1;\r\n        lVar18 = lVar18 + 0x18;\r\n      } while (iVar16 < (int)(*(uint *)(param_1 + 0x210) & 0xffffff));\r\n    }\r\n    lVar8 = 0;\r\ncode_r0x00014072d7f0:\r\n    iVar16 = (*(uint *)(param_1 + 0x1a0) & 0xffffff) - 1;\r\n    if (-1 < iVar16) {\r\n      lVar18 = (longlong)iVar16 * 0x70;\r\n      do {\r\n        lVar9 = FUN_1406dc560(*(undefined4 *)(lVar18 + *(longlong *)(param_1 + 0x198)));\r\n        if ((!bVar4) || (lVar9 != lVar8)) {\r\n          FUN_14072b260(param_1 + 0x198,iVar16);\r\n        }\r\n        lVar18 = lVar18 + -0x70;\r\n        iVar16 = iVar16 + -1;\r\n      } while (-1 < iVar16);\r\n    }\r\n    uVar11 = 0;\r\n    if (((*(char *)(param_1 + 0xec) == '\\0') && (lVar8 == 0)) &&\r\n       (uVar14 = uVar11, uVar15 = uVar11, (*(uint *)(param_1 + 0x210) & 0xffffff) != 0)) {\r\n      do {\r\n        afStackX_8[0] = *(float *)(uVar15 + *(longlong *)(param_1 + 0x208));\r\n        uVar10 = FUN_1407b5e00(D2_GameSingletonPtr + 0x62616,afStackX_8);\r\n        cVar5 = FUN_14072d340(param_1,uVar10,1);\r\n        if (cVar5 != '\\0') {\r\n          afStackX_8[0] = 1e+30;\r\n          *(undefined4 *)(param_1 + 0x150) = 0x7149f2ca;\r\n          break;\r\n        }\r\n        uVar17 = (int)uVar14 + 1;\r\n        uVar14 = (ulonglong)uVar17;\r\n        uVar15 = uVar15 + 0x18;\r\n      } while ((int)uVar17 < (int)(*(uint *)(param_1 + 0x210) & 0xffffff));\r\n    }\r\n    uVar14 = uVar11;\r\n    if ((*(uint *)(param_1 + 400) & 0xffffff) != 0) {\r\n      do {\r\n        uVar10 = FUN_1406dc560(*(undefined4 *)\r\n                                (*(longlong *)(param_1 + 0x188) + (longlong)(int)uVar14 * 4));\r\n        lVar8 = FUN_1408f2e50(uVar10);\r\n        if (lVar8 != 0) {\r\n          cVar5 = FUN_14072a400(param_1,lVar8,0);\r\n          if (cVar5 == '\\0') {\r\n            if ((*(byte *)(lVar8 + 0x368) & 0x40) == 0) {\r\n              afStackX_8[0] = *(float *)(lVar8 + 0xa8);\r\n              iVar16 = (*(uint *)(param_1 + 0x180) & 0xffffff) - 1;\r\n              if (-1 < iVar16) {\r\n                pfVar13 = *(float **)(param_1 + 0x178);\r\n                uVar15 = uVar11;\r\n                uVar19 = uVar11;\r\n                do {\r\n                  if (*pfVar13 == afStackX_8[0]) {\r\n                    if ((int)uVar19 != -1) goto code_r0x00014072da86;\r\n                    break;\r\n                  }\r\n                  uVar19 = (ulonglong)((int)uVar19 + 1);\r\n                  uVar15 = uVar15 + 1;\r\n                  pfVar13 = pfVar13 + 1;\r\n                } while ((longlong)uVar15 <= (longlong)iVar16);\r\n              }\r\n              FUN_14052e460(param_1 + 0x178,afStackX_8);\r\n              uStack_158 = lRam000000014398dad8;\r\n              uStack_150 = 0x5d;\r\n              FUN_140131ae0(&lStack_148);\r\n              uVar2 = *(undefined4 *)(lVar8 + 0xa8);\r\n              lVar8 = FUN_140810240();\r\n              plVar7 = (longlong *)FUN_1408112c0(lVar8 + 0xc8e8,0x4f,0x18,uVar2);\r\n              if (plVar7 != (longlong *)0x0) {\r\n                *plVar7 = uStack_158;\r\n                plVar7[1] = CONCAT44(uStack_14c,uStack_150);\r\n                plVar7[2] = lStack_148;\r\n              }\r\n            }\r\n          }\r\n          else {\r\n            uVar10 = FUN_1407281e0(auStack_110,lVar8,0);\r\n            iVar16 = FUN_140728720(param_1 + 0x198,uVar10);\r\n            lVar18 = FUN_140878470(lVar8);\r\n            if (lVar18 != 0) {\r\n              lVar9 = *(longlong *)(param_1 + 0x198);\r\n              uVar10 = FUN_14025c0c0(*(longlong *)(lVar18 + 0x1fe8) + 0x108,&DAT_1432d07a0,\r\n                                     *(longlong *)(param_1 + 8) + 0x168);\r\n              FUN_14025b6a0((longlong)iVar16 * 0x70 + lVar9 + 0x3c,uVar10);\r\n            }\r\n            uVar10 = FUN_140877720(lVar8,&DAT_1439ba280);\r\n            FUN_140768740(uVar10,0xe);\r\n          }\r\n        }\r\ncode_r0x00014072da86:\r\n        uVar17 = (int)uVar14 + 1;\r\n        uVar14 = (ulonglong)uVar17;\r\n      } while ((int)uVar17 < (int)(*(uint *)(param_1 + 400) & 0xffffff));\r\n    }\r\n    *(uint *)(param_1 + 400) = *(uint *)(param_1 + 400) & 0xff000000;\r\n  }\r\n  uVar11 = 0;\r\n  lVar8 = *(longlong *)(param_1 + 8);\r\n  if (*(float *)(param_1 + 0x1e8) <= *(float *)(lVar8 + 0x9c)) {\r\n    fVar27 = 1.0 - *(float *)(param_1 + 0x1e8) / *(float *)(lVar8 + 0x9c);\r\n    if (0.0 <= fVar27) {\r\n      if (1.0 < fVar27) {\r\n        fVar27 = 1.0;\r\n      }\r\n    }\r\n    else {\r\n      fVar27 = 0.0;\r\n    }\r\n    *(float *)(param_1 + 0x160) = fVar27;\r\n  }\r\n  else {\r\n    *(undefined4 *)(param_1 + 0x160) = 0;\r\n  }\r\n  if (*(int *)(param_1 + 0x2c) != 3) {\r\n    FUN_14053d940(*(undefined8 *)(param_1 + 0x1c0),0,*(undefined2 *)(param_1 + 0x1d8),&fStack_168,\r\n                  &uStack_140);\r\n    *(float *)(param_1 + 0xf0) = fStack_168;\r\n    *(float *)(param_1 + 0xf4) = fStack_164;\r\n    fVar27 = fStack_160;\r\n    goto code_r0x00014072e459;\r\n  }\r\n  if (*(float *)(param_1 + 0x150) == 1e+30) {\r\n    *(undefined1 *)(param_1 + 0xec) = 0;\r\n    iVar16 = -1;\r\n    if (((*(uint *)(param_1 + 0x1a0) & 0xffffff) == 0) ||\r\n       (*(char *)(*(longlong *)(param_1 + 0x198) + 0x6e) != '\\x01')) {\r\n      uVar17 = *(uint *)(param_1 + 0x138) & 0xffffff;\r\n      iVar22 = -1;\r\n      if (uVar17 != 0) {\r\n        if ((*(uint *)(param_1 + 0x1b0) & 0xffffff) != 0) {\r\n          uStack_158 = 0;\r\n          FUN_1401548e0();\r\n          uStack_158 = 0;\r\n          uStack_14c = 0x3f;\r\n          uStack_150 = 0x5000000;\r\n          uVar17 = 0;\r\n          if ((*(uint *)(param_1 + 0x138) & 0xffffff) != 0) {\r\n            FUN_140384fe0(&uStack_158);\r\n            uVar17 = uStack_14c;\r\n          }\r\n          piVar3 = *(int **)(param_1 + 0x1a8);\r\n          uVar6 = *(uint *)(param_1 + 0x1b0);\r\n          lVar18 = uStack_158;\r\n          uVar20 = uStack_150;\r\n          if (piVar3 != piVar3 + (ulonglong)(uVar6 & 0xffffff) * 0x1c) {\r\n            piVar21 = piVar3 + 0x18;\r\n            do {\r\n              iVar16 = (uVar20 & 0xffffff) - 1;\r\n              if (-1 < iVar16) {\r\n                uVar14 = uVar11;\r\n                uVar15 = uVar11;\r\n                do {\r\n                  if (*(int *)(lVar18 + uVar14 * 4) == *piVar21) {\r\n                    if (-1 < (int)uVar15) goto code_r0x00014072dc74;\r\n                    break;\r\n                  }\r\n                  uVar15 = (ulonglong)((int)uVar15 + 1);\r\n                  uVar14 = uVar14 + 1;\r\n                } while ((longlong)uVar14 <= (longlong)iVar16);\r\n              }\r\n              FUN_14001d6b0(&uStack_158,piVar21);\r\n              lVar18 = uStack_158;\r\n              uVar20 = uStack_150;\r\ncode_r0x00014072dc74:\r\n              piVar1 = piVar21 + 4;\r\n              piVar21 = piVar21 + 0x1c;\r\n              uVar17 = uStack_14c;\r\n            } while (piVar1 != piVar3 + (ulonglong)(uVar6 & 0xffffff) * 0x1c);\r\n          }\r\n          iVar16 = (uVar20 & 0xffffff) - 1;\r\n          if (-1 < iVar16) {\r\n            uVar14 = uVar11;\r\n            uVar15 = uVar11;\r\n            do {\r\n              iVar22 = (int)uVar15;\r\n              if (*(int *)(lVar18 + uVar14 * 4) == *(int *)(param_1 + 0x15c))\r\n              goto code_r0x00014072dcb6;\r\n              uVar15 = (ulonglong)(iVar22 + 1);\r\n              uVar14 = uVar14 + 1;\r\n            } while ((longlong)uVar14 <= (longlong)iVar16);\r\n          }\r\n          iVar22 = -1;\r\ncode_r0x00014072dcb6:\r\n          uVar14 = (ulonglong)(iVar22 + 1U);\r\n          if ((int)(uVar20 & 0xffffff) <= (int)(iVar22 + 1U)) {\r\n            uVar14 = uVar11;\r\n          }\r\n          iVar16 = *(int *)(lVar18 + (longlong)(int)uVar14 * 4);\r\n          if (((uVar17 & 0x40) == 0) && (lVar18 != 0)) {\r\n            plVar7 = (longlong *)FUN_1401548e0();\r\n            (**(code **)(*plVar7 + 0x10))\r\n                      (plVar7,\r\n                       \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                       ,0x1ec,lVar18,0x10);\r\n          }\r\n          goto code_r0x00014072dd7b;\r\n        }\r\n        iVar22 = iVar16;\r\n        if (*(int *)(param_1 + 0x15c) != -1) {\r\n          uVar14 = uVar11;\r\n          if (0 < (int)(uVar17 - 2)) {\r\n            uVar6 = (int)D2_GameSingletonPtr[0xc01f] * 0x19660d + 0x3c6ef35f;\r\n            *(uint *)(D2_GameSingletonPtr + 0xc01f) = uVar6;\r\n            uVar14 = (longlong)(ulonglong)(uVar6 >> 10 & 0x7fff) % (longlong)(int)(uVar17 - 1) &\r\n                     0xffffffff;\r\n          }\r\n          iVar12 = -1;\r\n          uVar17 = *(uint *)(param_1 + 0x138) & 0xffffff;\r\n          if (uVar17 != 0) {\r\n            do {\r\n              iVar16 = (int)uVar11;\r\n              if ((iVar16 != *(int *)(param_1 + 0x15c)) &&\r\n                 (iVar12 = iVar12 + 1, iVar12 == (int)uVar14)) goto code_r0x00014072dd7b;\r\n              uVar11 = (ulonglong)(iVar16 + 1U);\r\n            } while ((int)(iVar16 + 1U) < (int)uVar17);\r\n          }\r\n        }\r\n      }\r\n    }\r\n    else {\r\n      iVar16 = *(int *)(*(longlong *)(param_1 + 0x198) + 0x60);\r\n      *(undefined1 *)(param_1 + 0xec) = 1;\r\ncode_r0x00014072dd7b:\r\n      iVar22 = iVar16;\r\n      if ((iVar16 != -1) && (iVar16 != *(int *)(param_1 + 0x15c))) {\r\n        FUN_14072cca0(param_1);\r\n      }\r\n    }\r\n    *(int *)(param_1 + 0x15c) = iVar22;\r\n    *(undefined4 *)(param_1 + 0xfc) = *(undefined4 *)(param_1 + 0xf0);\r\n    *(undefined4 *)(param_1 + 0x100) = *(undefined4 *)(param_1 + 0xf4);\r\n    *(undefined4 *)(param_1 + 0x104) = *(undefined4 *)(param_1 + 0xf8);\r\n    *(undefined4 *)(param_1 + 0x150) = 0;\r\n    fVar27 = (float)(*(uint *)(param_1 + 0x1b0) & 0xffffff);\r\n    if (*(char *)(param_1 + 0xec) == '\\0') {\r\n      fVar24 = *(float *)(lVar8 + 0xf0);\r\n      afStackX_8[0] = (float)FUN_1406dd680(lVar8 + 0xe8);\r\n      *(float *)(param_1 + 0x154) = afStackX_8[0];\r\n      fVar25 = *(float *)(param_1 + 0x154) - fVar27 * fVar24;\r\n      fVar24 = 0.0;\r\n      if (0.0 <= fVar25) {\r\n        fVar24 = fVar25;\r\n      }\r\n      *(float *)(param_1 + 0x154) = fVar24;\r\n    }\r\n    else {\r\n      afStackX_8[0] = 0.5;\r\n      *(undefined4 *)(param_1 + 0x154) = 0x3f000000;\r\n    }\r\n    lVar18 = *(longlong *)(param_1 + 0x10);\r\n    fStack_168 = *(float *)(lVar18 + 0x300);\r\n    fStack_164 = *(float *)(lVar18 + 0x304);\r\n    fStack_160 = *(float *)(lVar18 + 0x308);\r\n    uStack_140 = *(undefined4 *)(lVar18 + 0x30c);\r\n    uStack_13c = *(undefined4 *)(lVar18 + 0x310);\r\n    uStack_138 = *(undefined4 *)(lVar18 + 0x314);\r\n    uStack_134 = *(undefined4 *)(lVar18 + 0x318);\r\n    uStack_130 = *(undefined4 *)(lVar18 + 0x31c);\r\n    uStack_12c = *(undefined4 *)(lVar18 + 800);\r\n    uStack_128 = *(undefined4 *)(lVar18 + 0x324);\r\n    uStack_124 = *(undefined4 *)(lVar18 + 0x328);\r\n    uStack_120 = *(undefined4 *)(lVar18 + 0x32c);\r\n    if (*(int *)(param_1 + 0x15c) != -1) {\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                    &fStack_168,&uStack_140);\r\n    }\r\n    fVar25 = *(float *)(param_1 + 0xf0) - fStack_168;\r\n    fVar26 = *(float *)(param_1 + 0xf4) - fStack_164;\r\n    fVar24 = *(float *)(param_1 + 0xf8) - fStack_160;\r\n    afStackX_8[0] =\r\n         SQRT(fVar26 * fVar26 + fVar25 * fVar25 + fVar24 * fVar24) /\r\n         (fVar27 * *(float *)(lVar8 + 0xf8) + *(float *)(lVar8 + 0xf4));\r\n    *(float *)(param_1 + 0x158) = afStackX_8[0];\r\n  }\r\n  FUN_140877eb0(*(undefined8 *)(param_1 + 0x10),afStackX_10);\r\n  afStackX_10[0] = afStackX_10[0] + *(float *)(param_1 + 0x150);\r\n  *(float *)(param_1 + 0x150) = afStackX_10[0];\r\n  if (*(float *)(param_1 + 0x158) <= afStackX_10[0]) {\r\n    if (*(float *)(param_1 + 0x158) + *(float *)(param_1 + 0x154) <= afStackX_10[0]) {\r\n      *(undefined4 *)(param_1 + 0x150) = 0x7149f2ca;\r\n      *(undefined1 *)(param_1 + 0xec) = 0;\r\n    }\r\n    else {\r\n      if (((*(char *)(param_1 + 0xec) != '\\0') && ((*(uint *)(param_1 + 0x1a0) & 0xffffff) != 0)) &&\r\n         (*(char *)(*(longlong *)(param_1 + 0x198) + 0x6e) == '\\x01')) {\r\n        *(undefined1 *)(*(longlong *)(param_1 + 0x198) + 0x6e) = 0;\r\n      }\r\n      lVar18 = *(longlong *)(param_1 + 0x10);\r\n      fStack_168 = *(float *)(lVar18 + 0x300);\r\n      fStack_164 = *(float *)(lVar18 + 0x304);\r\n      fStack_160 = *(float *)(lVar18 + 0x308);\r\n      uStack_140 = *(undefined4 *)(lVar18 + 0x30c);\r\n      uStack_13c = *(undefined4 *)(lVar18 + 0x310);\r\n      uStack_138 = *(undefined4 *)(lVar18 + 0x314);\r\n      uStack_134 = *(undefined4 *)(lVar18 + 0x318);\r\n      uStack_130 = *(undefined4 *)(lVar18 + 0x31c);\r\n      uStack_12c = *(undefined4 *)(lVar18 + 800);\r\n      uStack_128 = *(undefined4 *)(lVar18 + 0x324);\r\n      uStack_124 = *(undefined4 *)(lVar18 + 0x328);\r\n      uStack_120 = *(undefined4 *)(lVar18 + 0x32c);\r\n      if (*(int *)(param_1 + 0x15c) != -1) {\r\n        FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                      (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                      &fStack_168,&uStack_140);\r\n      }\r\n      *(float *)(param_1 + 0xf0) = fStack_168;\r\n      *(float *)(param_1 + 0xf4) = fStack_164;\r\n      *(float *)(param_1 + 0xf8) = fStack_160;\r\n    }\r\n  }\r\n  else {\r\n    lVar18 = *(longlong *)(param_1 + 0x10);\r\n    fStack_168 = *(float *)(lVar18 + 0x300);\r\n    fStack_164 = *(float *)(lVar18 + 0x304);\r\n    fStack_160 = *(float *)(lVar18 + 0x308);\r\n    uStack_140 = *(undefined4 *)(lVar18 + 0x30c);\r\n    uStack_13c = *(undefined4 *)(lVar18 + 0x310);\r\n    uStack_138 = *(undefined4 *)(lVar18 + 0x314);\r\n    uStack_134 = *(undefined4 *)(lVar18 + 0x318);\r\n    uStack_130 = *(undefined4 *)(lVar18 + 0x31c);\r\n    uStack_12c = *(undefined4 *)(lVar18 + 800);\r\n    uStack_128 = *(undefined4 *)(lVar18 + 0x324);\r\n    uStack_124 = *(undefined4 *)(lVar18 + 0x328);\r\n    uStack_120 = *(undefined4 *)(lVar18 + 0x32c);\r\n    if (*(int *)(param_1 + 0x15c) != -1) {\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                    &fStack_168,&uStack_140);\r\n    }\r\n    fVar27 = *(float *)(param_1 + 0x150) / *(float *)(param_1 + 0x158);\r\n    if (0.0 <= fVar27) {\r\n      if (1.0 < fVar27) {\r\n        fVar27 = 1.0;\r\n      }\r\n    }\r\n    else {\r\n      fVar27 = 0.0;\r\n    }\r\n    fVar27 = fVar27 * 3.0 * fVar27 - fVar27 * 2.0 * fVar27 * fVar27;\r\n    *(float *)(param_1 + 0xf0) =\r\n         fVar27 * (fStack_168 - *(float *)(param_1 + 0xfc)) + *(float *)(param_1 + 0xfc);\r\n    *(float *)(param_1 + 0xf4) =\r\n         fVar27 * (fStack_164 - *(float *)(param_1 + 0x100)) + *(float *)(param_1 + 0x100);\r\n    *(float *)(param_1 + 0xf8) =\r\n         fVar27 * (fStack_160 - *(float *)(param_1 + 0x104)) + *(float *)(param_1 + 0x104);\r\n  }\r\n  fVar27 = *(float *)(param_1 + 0x160);\r\n  fVar24 = *(float *)(lVar8 + 0x100);\r\n  fVar25 = *(float *)(lVar8 + 0xfc);\r\n  if (ABS(fVar24) <= 1e-18) {\r\n    fVar24 = 0.0;\r\n  }\r\n  if (ABS(fVar25) <= 1e-18) {\r\n    fVar25 = 0.0;\r\n  }\r\n  fVar26 = (1.0 - fVar27) * fVar25 + fVar24 * fVar27;\r\n  fVar24 = *(float *)(lVar8 + 0x108);\r\n  fVar25 = *(float *)(lVar8 + 0x104);\r\n  if (ABS(fVar24) <= 1e-18) {\r\n    fVar24 = 0.0;\r\n  }\r\n  if (ABS(fVar25) <= 1e-18) {\r\n    fVar25 = 0.0;\r\n  }\r\n  fVar27 = ((1.0 - fVar27) * fVar25 + fVar24 * fVar27) * DAT_14228a74c * *(float *)(param_1 + 0x1e8)\r\n  ;\r\n  if ((fVar27 < 0.0) || (fVar24 = fVar27, 6.2831855 <= fVar27)) {\r\n    fVar24 = (float)FUN_1411ad95c(fVar27 * 0.15915494);\r\n    fVar24 = fVar27 - fVar24 * 6.2831855;\r\n  }\r\n  fVar24 = (float)FUN_1411ae130(fVar24);\r\n  fVar25 = fVar27 + *(float *)(lVar8 + 0x10c);\r\n  if ((fVar25 < 0.0) || (6.2831855 <= fVar25)) {\r\n    fVar23 = (float)FUN_1411ad95c(fVar25 * 0.15915494);\r\n    fVar25 = fVar25 - fVar23 * 6.2831855;\r\n  }\r\n  fVar25 = (float)FUN_1411ae130(fVar25);\r\n  fVar27 = fVar27 + *(float *)(lVar8 + 0x110);\r\n  if ((fVar27 < 0.0) || (6.2831855 <= fVar27)) {\r\n    fVar23 = (float)FUN_1411ad95c(fVar27 * 0.15915494);\r\n    fVar27 = fVar27 - fVar23 * 6.2831855;\r\n  }\r\n  fVar27 = (float)FUN_1411ae130(fVar27);\r\n  *(float *)(param_1 + 0xf0) = fVar24 * fVar26 + *(float *)(param_1 + 0xf0);\r\n  *(float *)(param_1 + 0xf4) = fVar25 * fVar26 + *(float *)(param_1 + 0xf4);\r\n  fVar27 = fVar27 * fVar26 + *(float *)(param_1 + 0xf8);\r\ncode_r0x00014072e459:\r\n  *(float *)(param_1 + 0xf8) = fVar27;\r\n  if (*(longlong *)(param_1 + 0xe0) != 0) {\r\n    FUN_140342360(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231daf8);\r\n    uStack_158 = *(undefined8 *)(param_1 + 0xf0);\r\n    uStack_150 = *(undefined4 *)(param_1 + 0xf8);\r\n    uStack_14c = 0;\r\n    FUN_140343990(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231daf8,&uStack_158);\r\n    FUN_140342360(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231db18);\r\n    uStack_150 = *(uint *)(param_1 + 0x128);\r\n    uStack_158 = CONCAT44(uStack_150,uStack_150);\r\n    uStack_14c = uStack_150;\r\n    FUN_140343990(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231db18,&uStack_158);\r\n  }\r\n  uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n  if ((*(int *)(param_1 + 0xe8) != -1) &&\r\n     (lVar8 = FUN_1401a04a0(uVar10,*(undefined4 *)(param_1 + 0xe8)), lVar8 != 0)) {\r\n    *(undefined4 *)(lVar8 + 0x38) = *(undefined4 *)(param_1 + 0xf0);\r\n    *(undefined4 *)(lVar8 + 0x3c) = *(undefined4 *)(param_1 + 0xf4);\r\n    *(undefined4 *)(lVar8 + 0x40) = *(undefined4 *)(param_1 + 0xf8);\r\n  }\r\n  uStack_98 = 0xfffffffffffffffe;\r\n  if (*(int *)(param_1 + 0xb0) != -1) {\r\n    puStack_88 = auStack_78;\r\n    uStack_80 = 0x8000000100000000;\r\n    puStack_c0 = &UNK_14072b663;\r\n    cVar5 = FUN_140092630(D2_GameSingletonPtr + 0xc050,(int *)(param_1 + 0xb0),&puStack_88);\r\n    if ((cVar5 != '\\0') && ((int)uStack_80 != 0)) {\r\n      puStack_c0 = &UNK_14072b67b;\r\n      func_0x000140621240(param_1);\r\n    }\r\n    puStack_c0 = &UNK_14072b681;\r\n    plVar7 = (longlong *)FUN_140a9de90();\r\n    uVar14 = uStack_80;\r\n    uVar11 = uStack_80 >> 0x20;\r\n    uStack_80 = uStack_80 & 0xffffffff00000000;\r\n    if (-1 < (longlong)uVar14) {\r\n      puStack_c0 = &UNK_14072b6ad;\r\n      (**(code **)(*plVar7 + 0x20))(plVar7,puStack_88,((uint)uVar11 & 0x3fffffff) * 0x70);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14072d6e0(longlong param_1)\r\n\r\n{\r\n  int *piVar1;\r\n  undefined4 uVar2;\r\n  int *piVar3;\r\n  bool bVar4;\r\n  char cVar5;\r\n  uint uVar6;\r\n  longlong *plVar7;\r\n  longlong lVar8;\r\n  longlong lVar9;\r\n  undefined8 uVar10;\r\n  ulonglong uVar11;\r\n  int iVar12;\r\n  float *pfVar13;\r\n  ulonglong uVar14;\r\n  ulonglong uVar15;\r\n  int iVar16;\r\n  uint uVar17;\r\n  longlong lVar18;\r\n  ulonglong uVar19;\r\n  uint uVar20;\r\n  int *piVar21;\r\n  int iVar22;\r\n  float fVar23;\r\n  float fVar24;\r\n  float fVar25;\r\n  float fVar26;\r\n  float fVar27;\r\n  float afStackX_8 [2];\r\n  float afStackX_10 [2];\r\n  float fStack_168;\r\n  float fStack_164;\r\n  float fStack_160;\r\n  undefined8 uStack_158;\r\n  uint uStack_150;\r\n  uint uStack_14c;\r\n  longlong lStack_148;\r\n  undefined4 uStack_140;\r\n  undefined4 uStack_13c;\r\n  undefined4 uStack_138;\r\n  undefined4 uStack_134;\r\n  undefined4 uStack_130;\r\n  undefined4 uStack_12c;\r\n  undefined4 uStack_128;\r\n  undefined4 uStack_124;\r\n  undefined4 uStack_120;\r\n  undefined8 uStack_118;\r\n  undefined1 auStack_110 [80];\r\n  undefined *puStack_c0;\r\n  undefined8 uStack_98;\r\n  undefined1 *puStack_88;\r\n  ulonglong uStack_80;\r\n  undefined1 auStack_78 [64];\r\n  \r\n  uStack_118 = 0xfffffffffffffffe;\r\n  FUN_1406debc0(param_1 + 0x1f0);\r\n  FUN_140729fb0(param_1);\r\n  bVar4 = false;\r\n  if (*(int *)(param_1 + 0x2c) == 3) {\r\n    lVar8 = 0;\r\n    if (((*(uint *)(param_1 + 0x1a0) & 0xffffff) != 0) &&\r\n       (lVar8 = 0, *(char *)((longlong)*(undefined4 **)(param_1 + 0x198) + 0x6e) != '\\0')) {\r\n      lVar8 = FUN_1406dc560(**(undefined4 **)(param_1 + 0x198));\r\n    }\r\n    iVar16 = 0;\r\n    if ((*(uint *)(param_1 + 0x210) & 0xffffff) != 0) {\r\n      lVar18 = 0;\r\n      do {\r\n        afStackX_8[0] = *(float *)(lVar18 + *(longlong *)(param_1 + 0x208));\r\n        lVar9 = FUN_1407b5e00(D2_GameSingletonPtr + 0x62616,afStackX_8);\r\n        if (lVar9 == lVar8) {\r\n          cVar5 = FUN_14072a400(param_1,lVar9,0);\r\n          if (cVar5 != '\\0') {\r\n            bVar4 = true;\r\n            goto code_r0x00014072d7f0;\r\n          }\r\n          break;\r\n        }\r\n        iVar16 = iVar16 + 1;\r\n        lVar18 = lVar18 + 0x18;\r\n      } while (iVar16 < (int)(*(uint *)(param_1 + 0x210) & 0xffffff));\r\n    }\r\n    lVar8 = 0;\r\ncode_r0x00014072d7f0:\r\n    iVar16 = (*(uint *)(param_1 + 0x1a0) & 0xffffff) - 1;\r\n    if (-1 < iVar16) {\r\n      lVar18 = (longlong)iVar16 * 0x70;\r\n      do {\r\n        lVar9 = FUN_1406dc560(*(undefined4 *)(lVar18 + *(longlong *)(param_1 + 0x198)));\r\n        if ((!bVar4) || (lVar9 != lVar8)) {\r\n          FUN_14072b260(param_1 + 0x198,iVar16);\r\n        }\r\n        lVar18 = lVar18 + -0x70;\r\n        iVar16 = iVar16 + -1;\r\n      } while (-1 < iVar16);\r\n    }\r\n    uVar11 = 0;\r\n    if (((*(char *)(param_1 + 0xec) == '\\0') && (lVar8 == 0)) &&\r\n       (uVar14 = uVar11, uVar15 = uVar11, (*(uint *)(param_1 + 0x210) & 0xffffff) != 0)) {\r\n      do {\r\n        afStackX_8[0] = *(float *)(uVar15 + *(longlong *)(param_1 + 0x208));\r\n        uVar10 = FUN_1407b5e00(D2_GameSingletonPtr + 0x62616,afStackX_8);\r\n        cVar5 = FUN_14072d340(param_1,uVar10,1);\r\n        if (cVar5 != '\\0') {\r\n          afStackX_8[0] = 1e+30;\r\n          *(undefined4 *)(param_1 + 0x150) = 0x7149f2ca;\r\n          break;\r\n        }\r\n        uVar17 = (int)uVar14 + 1;\r\n        uVar14 = (ulonglong)uVar17;\r\n        uVar15 = uVar15 + 0x18;\r\n      } while ((int)uVar17 < (int)(*(uint *)(param_1 + 0x210) & 0xffffff));\r\n    }\r\n    uVar14 = uVar11;\r\n    if ((*(uint *)(param_1 + 400) & 0xffffff) != 0) {\r\n      do {\r\n        uVar10 = FUN_1406dc560(*(undefined4 *)\r\n                                (*(longlong *)(param_1 + 0x188) + (longlong)(int)uVar14 * 4));\r\n        lVar8 = FUN_1408f2e50(uVar10);\r\n        if (lVar8 != 0) {\r\n          cVar5 = FUN_14072a400(param_1,lVar8,0);\r\n          if (cVar5 == '\\0') {\r\n            if ((*(byte *)(lVar8 + 0x368) & 0x40) == 0) {\r\n              afStackX_8[0] = *(float *)(lVar8 + 0xa8);\r\n              iVar16 = (*(uint *)(param_1 + 0x180) & 0xffffff) - 1;\r\n              if (-1 < iVar16) {\r\n                pfVar13 = *(float **)(param_1 + 0x178);\r\n                uVar15 = uVar11;\r\n                uVar19 = uVar11;\r\n                do {\r\n                  if (*pfVar13 == afStackX_8[0]) {\r\n                    if ((int)uVar19 != -1) goto code_r0x00014072da86;\r\n                    break;\r\n                  }\r\n                  uVar19 = (ulonglong)((int)uVar19 + 1);\r\n                  uVar15 = uVar15 + 1;\r\n                  pfVar13 = pfVar13 + 1;\r\n                } while ((longlong)uVar15 <= (longlong)iVar16);\r\n              }\r\n              FUN_14052e460(param_1 + 0x178,afStackX_8);\r\n              uStack_158 = lRam000000014398dad8;\r\n              uStack_150 = 0x5d;\r\n              FUN_140131ae0(&lStack_148);\r\n              uVar2 = *(undefined4 *)(lVar8 + 0xa8);\r\n              lVar8 = FUN_140810240();\r\n              plVar7 = (longlong *)FUN_1408112c0(lVar8 + 0xc8e8,0x4f,0x18,uVar2);\r\n              if (plVar7 != (longlong *)0x0) {\r\n                *plVar7 = uStack_158;\r\n                plVar7[1] = CONCAT44(uStack_14c,uStack_150);\r\n                plVar7[2] = lStack_148;\r\n              }\r\n            }\r\n          }\r\n          else {\r\n            uVar10 = FUN_1407281e0(auStack_110,lVar8,0);\r\n            iVar16 = FUN_140728720(param_1 + 0x198,uVar10);\r\n            lVar18 = FUN_140878470(lVar8);\r\n            if (lVar18 != 0) {\r\n              lVar9 = *(longlong *)(param_1 + 0x198);\r\n              uVar10 = FUN_14025c0c0(*(longlong *)(lVar18 + 0x1fe8) + 0x108,&DAT_1432d07a0,\r\n                                     *(longlong *)(param_1 + 8) + 0x168);\r\n              FUN_14025b6a0((longlong)iVar16 * 0x70 + lVar9 + 0x3c,uVar10);\r\n            }\r\n            uVar10 = FUN_140877720(lVar8,&DAT_1439ba280);\r\n            FUN_140768740(uVar10,0xe);\r\n          }\r\n        }\r\ncode_r0x00014072da86:\r\n        uVar17 = (int)uVar14 + 1;\r\n        uVar14 = (ulonglong)uVar17;\r\n      } while ((int)uVar17 < (int)(*(uint *)(param_1 + 400) & 0xffffff));\r\n    }\r\n    *(uint *)(param_1 + 400) = *(uint *)(param_1 + 400) & 0xff000000;\r\n  }\r\n  uVar11 = 0;\r\n  lVar8 = *(longlong *)(param_1 + 8);\r\n  if (*(float *)(param_1 + 0x1e8) <= *(float *)(lVar8 + 0x9c)) {\r\n    fVar27 = 1.0 - *(float *)(param_1 + 0x1e8) / *(float *)(lVar8 + 0x9c);\r\n    if (0.0 <= fVar27) {\r\n      if (1.0 < fVar27) {\r\n        fVar27 = 1.0;\r\n      }\r\n    }\r\n    else {\r\n      fVar27 = 0.0;\r\n    }\r\n    *(float *)(param_1 + 0x160) = fVar27;\r\n  }\r\n  else {\r\n    *(undefined4 *)(param_1 + 0x160) = 0;\r\n  }\r\n  if (*(int *)(param_1 + 0x2c) != 3) {\r\n    FUN_14053d940(*(undefined8 *)(param_1 + 0x1c0),0,*(undefined2 *)(param_1 + 0x1d8),&fStack_168,\r\n                  &uStack_140);\r\n    *(float *)(param_1 + 0xf0) = fStack_168;\r\n    *(float *)(param_1 + 0xf4) = fStack_164;\r\n    fVar27 = fStack_160;\r\n    goto code_r0x00014072e459;\r\n  }\r\n  if (*(float *)(param_1 + 0x150) == 1e+30) {\r\n    *(undefined1 *)(param_1 + 0xec) = 0;\r\n    iVar16 = -1;\r\n    if (((*(uint *)(param_1 + 0x1a0) & 0xffffff) == 0) ||\r\n       (*(char *)(*(longlong *)(param_1 + 0x198) + 0x6e) != '\\x01')) {\r\n      uVar17 = *(uint *)(param_1 + 0x138) & 0xffffff;\r\n      iVar22 = -1;\r\n      if (uVar17 != 0) {\r\n        if ((*(uint *)(param_1 + 0x1b0) & 0xffffff) != 0) {\r\n          uStack_158 = 0;\r\n          FUN_1401548e0();\r\n          uStack_158 = 0;\r\n          uStack_14c = 0x3f;\r\n          uStack_150 = 0x5000000;\r\n          uVar17 = 0;\r\n          if ((*(uint *)(param_1 + 0x138) & 0xffffff) != 0) {\r\n            FUN_140384fe0(&uStack_158);\r\n            uVar17 = uStack_14c;\r\n          }\r\n          piVar3 = *(int **)(param_1 + 0x1a8);\r\n          uVar6 = *(uint *)(param_1 + 0x1b0);\r\n          lVar18 = uStack_158;\r\n          uVar20 = uStack_150;\r\n          if (piVar3 != piVar3 + (ulonglong)(uVar6 & 0xffffff) * 0x1c) {\r\n            piVar21 = piVar3 + 0x18;\r\n            do {\r\n              iVar16 = (uVar20 & 0xffffff) - 1;\r\n              if (-1 < iVar16) {\r\n                uVar14 = uVar11;\r\n                uVar15 = uVar11;\r\n                do {\r\n                  if (*(int *)(lVar18 + uVar14 * 4) == *piVar21) {\r\n                    if (-1 < (int)uVar15) goto code_r0x00014072dc74;\r\n                    break;\r\n                  }\r\n                  uVar15 = (ulonglong)((int)uVar15 + 1);\r\n                  uVar14 = uVar14 + 1;\r\n                } while ((longlong)uVar14 <= (longlong)iVar16);\r\n              }\r\n              FUN_14001d6b0(&uStack_158,piVar21);\r\n              lVar18 = uStack_158;\r\n              uVar20 = uStack_150;\r\ncode_r0x00014072dc74:\r\n              piVar1 = piVar21 + 4;\r\n              piVar21 = piVar21 + 0x1c;\r\n              uVar17 = uStack_14c;\r\n            } while (piVar1 != piVar3 + (ulonglong)(uVar6 & 0xffffff) * 0x1c);\r\n          }\r\n          iVar16 = (uVar20 & 0xffffff) - 1;\r\n          if (-1 < iVar16) {\r\n            uVar14 = uVar11;\r\n            uVar15 = uVar11;\r\n            do {\r\n              iVar22 = (int)uVar15;\r\n              if (*(int *)(lVar18 + uVar14 * 4) == *(int *)(param_1 + 0x15c))\r\n              goto code_r0x00014072dcb6;\r\n              uVar15 = (ulonglong)(iVar22 + 1);\r\n              uVar14 = uVar14 + 1;\r\n            } while ((longlong)uVar14 <= (longlong)iVar16);\r\n          }\r\n          iVar22 = -1;\r\ncode_r0x00014072dcb6:\r\n          uVar14 = (ulonglong)(iVar22 + 1U);\r\n          if ((int)(uVar20 & 0xffffff) <= (int)(iVar22 + 1U)) {\r\n            uVar14 = uVar11;\r\n          }\r\n          iVar16 = *(int *)(lVar18 + (longlong)(int)uVar14 * 4);\r\n          if (((uVar17 & 0x40) == 0) && (lVar18 != 0)) {\r\n            plVar7 = (longlong *)FUN_1401548e0();\r\n            (**(code **)(*plVar7 + 0x10))\r\n                      (plVar7,\r\n                       \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                       ,0x1ec,lVar18,0x10);\r\n          }\r\n          goto code_r0x00014072dd7b;\r\n        }\r\n        iVar22 = iVar16;\r\n        if (*(int *)(param_1 + 0x15c) != -1) {\r\n          uVar14 = uVar11;\r\n          if (0 < (int)(uVar17 - 2)) {\r\n            uVar6 = (int)D2_GameSingletonPtr[0xc01f] * 0x19660d + 0x3c6ef35f;\r\n            *(uint *)(D2_GameSingletonPtr + 0xc01f) = uVar6;\r\n            uVar14 = (longlong)(ulonglong)(uVar6 >> 10 & 0x7fff) % (longlong)(int)(uVar17 - 1) &\r\n                     0xffffffff;\r\n          }\r\n          iVar12 = -1;\r\n          uVar17 = *(uint *)(param_1 + 0x138) & 0xffffff;\r\n          if (uVar17 != 0) {\r\n            do {\r\n              iVar16 = (int)uVar11;\r\n              if ((iVar16 != *(int *)(param_1 + 0x15c)) &&\r\n                 (iVar12 = iVar12 + 1, iVar12 == (int)uVar14)) goto code_r0x00014072dd7b;\r\n              uVar11 = (ulonglong)(iVar16 + 1U);\r\n            } while ((int)(iVar16 + 1U) < (int)uVar17);\r\n          }\r\n        }\r\n      }\r\n    }\r\n    else {\r\n      iVar16 = *(int *)(*(longlong *)(param_1 + 0x198) + 0x60);\r\n      *(undefined1 *)(param_1 + 0xec) = 1;\r\ncode_r0x00014072dd7b:\r\n      iVar22 = iVar16;\r\n      if ((iVar16 != -1) && (iVar16 != *(int *)(param_1 + 0x15c))) {\r\n        FUN_14072cca0(param_1);\r\n      }\r\n    }\r\n    *(int *)(param_1 + 0x15c) = iVar22;\r\n    *(undefined4 *)(param_1 + 0xfc) = *(undefined4 *)(param_1 + 0xf0);\r\n    *(undefined4 *)(param_1 + 0x100) = *(undefined4 *)(param_1 + 0xf4);\r\n    *(undefined4 *)(param_1 + 0x104) = *(undefined4 *)(param_1 + 0xf8);\r\n    *(undefined4 *)(param_1 + 0x150) = 0;\r\n    fVar27 = (float)(*(uint *)(param_1 + 0x1b0) & 0xffffff);\r\n    if (*(char *)(param_1 + 0xec) == '\\0') {\r\n      fVar24 = *(float *)(lVar8 + 0xf0);\r\n      afStackX_8[0] = (float)FUN_1406dd680(lVar8 + 0xe8);\r\n      *(float *)(param_1 + 0x154) = afStackX_8[0];\r\n      fVar25 = *(float *)(param_1 + 0x154) - fVar27 * fVar24;\r\n      fVar24 = 0.0;\r\n      if (0.0 <= fVar25) {\r\n        fVar24 = fVar25;\r\n      }\r\n      *(float *)(param_1 + 0x154) = fVar24;\r\n    }\r\n    else {\r\n      afStackX_8[0] = 0.5;\r\n      *(undefined4 *)(param_1 + 0x154) = 0x3f000000;\r\n    }\r\n    lVar18 = *(longlong *)(param_1 + 0x10);\r\n    fStack_168 = *(float *)(lVar18 + 0x300);\r\n    fStack_164 = *(float *)(lVar18 + 0x304);\r\n    fStack_160 = *(float *)(lVar18 + 0x308);\r\n    uStack_140 = *(undefined4 *)(lVar18 + 0x30c);\r\n    uStack_13c = *(undefined4 *)(lVar18 + 0x310);\r\n    uStack_138 = *(undefined4 *)(lVar18 + 0x314);\r\n    uStack_134 = *(undefined4 *)(lVar18 + 0x318);\r\n    uStack_130 = *(undefined4 *)(lVar18 + 0x31c);\r\n    uStack_12c = *(undefined4 *)(lVar18 + 800);\r\n    uStack_128 = *(undefined4 *)(lVar18 + 0x324);\r\n    uStack_124 = *(undefined4 *)(lVar18 + 0x328);\r\n    uStack_120 = *(undefined4 *)(lVar18 + 0x32c);\r\n    if (*(int *)(param_1 + 0x15c) != -1) {\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                    &fStack_168,&uStack_140);\r\n    }\r\n    fVar25 = *(float *)(param_1 + 0xf0) - fStack_168;\r\n    fVar26 = *(float *)(param_1 + 0xf4) - fStack_164;\r\n    fVar24 = *(float *)(param_1 + 0xf8) - fStack_160;\r\n    afStackX_8[0] =\r\n         SQRT(fVar26 * fVar26 + fVar25 * fVar25 + fVar24 * fVar24) /\r\n         (fVar27 * *(float *)(lVar8 + 0xf8) + *(float *)(lVar8 + 0xf4));\r\n    *(float *)(param_1 + 0x158) = afStackX_8[0];\r\n  }\r\n  FUN_140877eb0(*(undefined8 *)(param_1 + 0x10),afStackX_10);\r\n  afStackX_10[0] = afStackX_10[0] + *(float *)(param_1 + 0x150);\r\n  *(float *)(param_1 + 0x150) = afStackX_10[0];\r\n  if (*(float *)(param_1 + 0x158) <= afStackX_10[0]) {\r\n    if (*(float *)(param_1 + 0x158) + *(float *)(param_1 + 0x154) <= afStackX_10[0]) {\r\n      *(undefined4 *)(param_1 + 0x150) = 0x7149f2ca;\r\n      *(undefined1 *)(param_1 + 0xec) = 0;\r\n    }\r\n    else {\r\n      if (((*(char *)(param_1 + 0xec) != '\\0') && ((*(uint *)(param_1 + 0x1a0) & 0xffffff) != 0)) &&\r\n         (*(char *)(*(longlong *)(param_1 + 0x198) + 0x6e) == '\\x01')) {\r\n        *(undefined1 *)(*(longlong *)(param_1 + 0x198) + 0x6e) = 0;\r\n      }\r\n      lVar18 = *(longlong *)(param_1 + 0x10);\r\n      fStack_168 = *(float *)(lVar18 + 0x300);\r\n      fStack_164 = *(float *)(lVar18 + 0x304);\r\n      fStack_160 = *(float *)(lVar18 + 0x308);\r\n      uStack_140 = *(undefined4 *)(lVar18 + 0x30c);\r\n      uStack_13c = *(undefined4 *)(lVar18 + 0x310);\r\n      uStack_138 = *(undefined4 *)(lVar18 + 0x314);\r\n      uStack_134 = *(undefined4 *)(lVar18 + 0x318);\r\n      uStack_130 = *(undefined4 *)(lVar18 + 0x31c);\r\n      uStack_12c = *(undefined4 *)(lVar18 + 800);\r\n      uStack_128 = *(undefined4 *)(lVar18 + 0x324);\r\n      uStack_124 = *(undefined4 *)(lVar18 + 0x328);\r\n      uStack_120 = *(undefined4 *)(lVar18 + 0x32c);\r\n      if (*(int *)(param_1 + 0x15c) != -1) {\r\n        FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                      (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                      &fStack_168,&uStack_140);\r\n      }\r\n      *(float *)(param_1 + 0xf0) = fStack_168;\r\n      *(float *)(param_1 + 0xf4) = fStack_164;\r\n      *(float *)(param_1 + 0xf8) = fStack_160;\r\n    }\r\n  }\r\n  else {\r\n    lVar18 = *(longlong *)(param_1 + 0x10);\r\n    fStack_168 = *(float *)(lVar18 + 0x300);\r\n    fStack_164 = *(float *)(lVar18 + 0x304);\r\n    fStack_160 = *(float *)(lVar18 + 0x308);\r\n    uStack_140 = *(undefined4 *)(lVar18 + 0x30c);\r\n    uStack_13c = *(undefined4 *)(lVar18 + 0x310);\r\n    uStack_138 = *(undefined4 *)(lVar18 + 0x314);\r\n    uStack_134 = *(undefined4 *)(lVar18 + 0x318);\r\n    uStack_130 = *(undefined4 *)(lVar18 + 0x31c);\r\n    uStack_12c = *(undefined4 *)(lVar18 + 800);\r\n    uStack_128 = *(undefined4 *)(lVar18 + 0x324);\r\n    uStack_124 = *(undefined4 *)(lVar18 + 0x328);\r\n    uStack_120 = *(undefined4 *)(lVar18 + 0x32c);\r\n    if (*(int *)(param_1 + 0x15c) != -1) {\r\n      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,\r\n                    (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),\r\n                    &fStack_168,&uStack_140);\r\n    }\r\n    fVar27 = *(float *)(param_1 + 0x150) / *(float *)(param_1 + 0x158);\r\n    if (0.0 <= fVar27) {\r\n      if (1.0 < fVar27) {\r\n        fVar27 = 1.0;\r\n      }\r\n    }\r\n    else {\r\n      fVar27 = 0.0;\r\n    }\r\n    fVar27 = fVar27 * 3.0 * fVar27 - fVar27 * 2.0 * fVar27 * fVar27;\r\n    *(float *)(param_1 + 0xf0) =\r\n         fVar27 * (fStack_168 - *(float *)(param_1 + 0xfc)) + *(float *)(param_1 + 0xfc);\r\n    *(float *)(param_1 + 0xf4) =\r\n         fVar27 * (fStack_164 - *(float *)(param_1 + 0x100)) + *(float *)(param_1 + 0x100);\r\n    *(float *)(param_1 + 0xf8) =\r\n         fVar27 * (fStack_160 - *(float *)(param_1 + 0x104)) + *(float *)(param_1 + 0x104);\r\n  }\r\n  fVar27 = *(float *)(param_1 + 0x160);\r\n  fVar24 = *(float *)(lVar8 + 0x100);\r\n  fVar25 = *(float *)(lVar8 + 0xfc);\r\n  if (ABS(fVar24) <= 1e-18) {\r\n    fVar24 = 0.0;\r\n  }\r\n  if (ABS(fVar25) <= 1e-18) {\r\n    fVar25 = 0.0;\r\n  }\r\n  fVar26 = (1.0 - fVar27) * fVar25 + fVar24 * fVar27;\r\n  fVar24 = *(float *)(lVar8 + 0x108);\r\n  fVar25 = *(float *)(lVar8 + 0x104);\r\n  if (ABS(fVar24) <= 1e-18) {\r\n    fVar24 = 0.0;\r\n  }\r\n  if (ABS(fVar25) <= 1e-18) {\r\n    fVar25 = 0.0;\r\n  }\r\n  fVar27 = ((1.0 - fVar27) * fVar25 + fVar24 * fVar27) * DAT_14228a74c * *(float *)(param_1 + 0x1e8)\r\n  ;\r\n  if ((fVar27 < 0.0) || (fVar24 = fVar27, 6.2831855 <= fVar27)) {\r\n    fVar24 = (float)FUN_1411ad95c(fVar27 * 0.15915494);\r\n    fVar24 = fVar27 - fVar24 * 6.2831855;\r\n  }\r\n  fVar24 = (float)FUN_1411ae130(fVar24);\r\n  fVar25 = fVar27 + *(float *)(lVar8 + 0x10c);\r\n  if ((fVar25 < 0.0) || (6.2831855 <= fVar25)) {\r\n    fVar23 = (float)FUN_1411ad95c(fVar25 * 0.15915494);\r\n    fVar25 = fVar25 - fVar23 * 6.2831855;\r\n  }\r\n  fVar25 = (float)FUN_1411ae130(fVar25);\r\n  fVar27 = fVar27 + *(float *)(lVar8 + 0x110);\r\n  if ((fVar27 < 0.0) || (6.2831855 <= fVar27)) {\r\n    fVar23 = (float)FUN_1411ad95c(fVar27 * 0.15915494);\r\n    fVar27 = fVar27 - fVar23 * 6.2831855;\r\n  }\r\n  fVar27 = (float)FUN_1411ae130(fVar27);\r\n  *(float *)(param_1 + 0xf0) = fVar24 * fVar26 + *(float *)(param_1 + 0xf0);\r\n  *(float *)(param_1 + 0xf4) = fVar25 * fVar26 + *(float *)(param_1 + 0xf4);\r\n  fVar27 = fVar27 * fVar26 + *(float *)(param_1 + 0xf8);\r\ncode_r0x00014072e459:\r\n  *(float *)(param_1 + 0xf8) = fVar27;\r\n  if (*(longlong *)(param_1 + 0xe0) != 0) {\r\n    FUN_140342360(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231daf8);\r\n    uStack_158 = *(undefined8 *)(param_1 + 0xf0);\r\n    uStack_150 = *(undefined4 *)(param_1 + 0xf8);\r\n    uStack_14c = 0;\r\n    FUN_140343990(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231daf8,&uStack_158);\r\n    FUN_140342360(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231db18);\r\n    uStack_150 = *(uint *)(param_1 + 0x128);\r\n    uStack_158 = CONCAT44(uStack_150,uStack_150);\r\n    uStack_14c = uStack_150;\r\n    FUN_140343990(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231db18,&uStack_158);\r\n  }\r\n  uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();\r\n  if ((*(int *)(param_1 + 0xe8) != -1) &&\r\n     (lVar8 = FUN_1401a04a0(uVar10,*(undefined4 *)(param_1 + 0xe8)), lVar8 != 0)) {\r\n    *(undefined4 *)(lVar8 + 0x38) = *(undefined4 *)(param_1 + 0xf0);\r\n    *(undefined4 *)(lVar8 + 0x3c) = *(undefined4 *)(param_1 + 0xf4);\r\n    *(undefined4 *)(lVar8 + 0x40) = *(undefined4 *)(param_1 + 0xf8);\r\n  }\r\n  uStack_98 = 0xfffffffffffffffe;\r\n  if (*(int *)(param_1 + 0xb0) != -1) {\r\n    puStack_88 = auStack_78;\r\n    uStack_80 = 0x8000000100000000;\r\n    puStack_c0 = &UNK_14072b663;\r\n    cVar5 = FUN_140092630(D2_GameSingletonPtr + 0xc050,(int *)(param_1 + 0xb0),&puStack_88);\r\n    if ((cVar5 != '\\0') && ((int)uStack_80 != 0)) {\r\n      puStack_c0 = &UNK_14072b67b;\r\n      func_0x000140621240(param_1);\r\n    }\r\n    puStack_c0 = &UNK_14072b681;\r\n    plVar7 = (longlong *)FUN_140a9de90();\r\n    uVar14 = uStack_80;\r\n    uVar11 = uStack_80 >> 0x20;\r\n    uStack_80 = uStack_80 & 0xffffffff00000000;\r\n    if (-1 < (longlong)uVar14) {\r\n      puStack_c0 = &UNK_14072b6ad;\r\n      (**(code **)(*plVar7 + 0x20))(plVar7,puStack_88,((uint)uVar11 & 0x3fffffff) * 0x70);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "14072eed0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14072eed0(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined4 *puVar5;\r\n  float fVar6;\r\n  float fStack_98;\r\n  float fStack_94;\r\n  float fStack_90;\r\n  float fStack_88;\r\n  float fStack_84;\r\n  float fStack_80;\r\n  undefined4 uStack_78;\r\n  undefined4 uStack_74;\r\n  undefined4 uStack_70;\r\n  undefined4 uStack_6c;\r\n  undefined4 uStack_68;\r\n  undefined4 uStack_64;\r\n  undefined4 uStack_60;\r\n  undefined4 uStack_5c;\r\n  undefined4 uStack_58;\r\n  undefined1 auStack_50 [72];\r\n  \r\n  lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));\r\n  if (lVar3 != 0) {\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));\r\n    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));\r\n    fVar6 = *(float *)(param_1 + 0x108);\r\n    fVar1 = *(float *)(param_1 + 0x10c);\r\n    fVar2 = *(float *)(param_1 + 0x110);\r\n    fStack_98 = fVar6 * *(float *)(lVar4 + 0x30c) + fVar1 * *(float *)(lVar4 + 0x318) +\r\n                fVar2 * *(float *)(lVar4 + 0x324) + *(float *)(lVar3 + 0x300);\r\n    fStack_94 = fVar1 * *(float *)(lVar4 + 0x31c) + fVar6 * *(float *)(lVar4 + 0x310) +\r\n                fVar2 * *(float *)(lVar4 + 0x328) + *(float *)(lVar3 + 0x304);\r\n    fStack_90 = fVar1 * *(float *)(lVar4 + 800) + fVar6 * *(float *)(lVar4 + 0x314) +\r\n                fVar2 * *(float *)(lVar4 + 0x32c) + *(float *)(lVar3 + 0x308);\r\n    fStack_80 = 0.0;\r\n    fStack_84 = *(float *)(param_1 + 0x114) * *(float *)(lVar4 + 0x310) +\r\n                *(float *)(param_1 + 0x118) * *(float *)(lVar4 + 0x31c) +\r\n                *(float *)(param_1 + 0x11c) * *(float *)(lVar4 + 0x328);\r\n    fStack_88 = *(float *)(param_1 + 0x114) * *(float *)(lVar4 + 0x30c) +\r\n                *(float *)(param_1 + 0x118) * *(float *)(lVar4 + 0x318) +\r\n                *(float *)(param_1 + 0x11c) * *(float *)(lVar4 + 0x324);\r\n    fVar6 = SQRT(fStack_88 * fStack_88 + fStack_84 * fStack_84 + 0.0);\r\n    if (fVar6 <= 1.1920929e-07) {\r\n      lVar3 = *(longlong *)(param_1 + 0x10);\r\n      uStack_78 = *(undefined4 *)(lVar3 + 0x30c);\r\n      uStack_74 = *(undefined4 *)(lVar3 + 0x310);\r\n      uStack_70 = *(undefined4 *)(lVar3 + 0x314);\r\n      uStack_6c = *(undefined4 *)(lVar3 + 0x318);\r\n      uStack_68 = *(undefined4 *)(lVar3 + 0x31c);\r\n      uStack_64 = *(undefined4 *)(lVar3 + 800);\r\n      uStack_60 = *(undefined4 *)(lVar3 + 0x324);\r\n      uStack_5c = *(undefined4 *)(lVar3 + 0x328);\r\n      uStack_58 = *(undefined4 *)(lVar3 + 0x32c);\r\n    }\r\n    else {\r\n      fVar6 = 1.0 / fVar6;\r\n      fStack_88 = fStack_88 * fVar6;\r\n      fStack_84 = fVar6 * fStack_84;\r\n      fStack_80 = fVar6 * 0.0;\r\n      puVar5 = (undefined4 *)FUN_140112000(&fStack_88,auStack_50);\r\n      uStack_78 = *puVar5;\r\n      uStack_74 = puVar5[1];\r\n      uStack_70 = puVar5[2];\r\n      uStack_6c = puVar5[3];\r\n      uStack_68 = puVar5[4];\r\n      uStack_64 = puVar5[5];\r\n      uStack_60 = puVar5[6];\r\n      uStack_5c = puVar5[7];\r\n      uStack_58 = puVar5[8];\r\n    }\r\n    FUN_14087d1e0(*(undefined8 *)(param_1 + 0x10),&fStack_98,&uStack_78,1);\r\n    return;\r\n  }\r\n  if (*(char *)(param_1 + 0x2b2) != '\\0') {\r\n    func_0x000140621240(param_1);\r\n    *(undefined1 *)(param_1 + 0x2b2) = 0;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14072eed0(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined4 *puVar5;\r\n  float fVar6;\r\n  float fStack_98;\r\n  float fStack_94;\r\n  float fStack_90;\r\n  float fStack_88;\r\n  float fStack_84;\r\n  float fStack_80;\r\n  undefined4 uStack_78;\r\n  undefined4 uStack_74;\r\n  undefined4 uStack_70;\r\n  undefined4 uStack_6c;\r\n  undefined4 uStack_68;\r\n  undefined4 uStack_64;\r\n  undefined4 uStack_60;\r\n  undefined4 uStack_5c;\r\n  undefined4 uStack_58;\r\n  undefined1 auStack_50 [72];\r\n  \r\n  lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));\r\n  if (lVar3 != 0) {\r\n    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));\r\n    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));\r\n    fVar6 = *(float *)(param_1 + 0x108);\r\n    fVar1 = *(float *)(param_1 + 0x10c);\r\n    fVar2 = *(float *)(param_1 + 0x110);\r\n    fStack_98 = fVar6 * *(float *)(lVar4 + 0x30c) + fVar1 * *(float *)(lVar4 + 0x318) +\r\n                fVar2 * *(float *)(lVar4 + 0x324) + *(float *)(lVar3 + 0x300);\r\n    fStack_94 = fVar1 * *(float *)(lVar4 + 0x31c) + fVar6 * *(float *)(lVar4 + 0x310) +\r\n                fVar2 * *(float *)(lVar4 + 0x328) + *(float *)(lVar3 + 0x304);\r\n    fStack_90 = fVar1 * *(float *)(lVar4 + 800) + fVar6 * *(float *)(lVar4 + 0x314) +\r\n                fVar2 * *(float *)(lVar4 + 0x32c) + *(float *)(lVar3 + 0x308);\r\n    fStack_80 = 0.0;\r\n    fStack_84 = *(float *)(param_1 + 0x114) * *(float *)(lVar4 + 0x310) +\r\n                *(float *)(param_1 + 0x118) * *(float *)(lVar4 + 0x31c) +\r\n                *(float *)(param_1 + 0x11c) * *(float *)(lVar4 + 0x328);\r\n    fStack_88 = *(float *)(param_1 + 0x114) * *(float *)(lVar4 + 0x30c) +\r\n                *(float *)(param_1 + 0x118) * *(float *)(lVar4 + 0x318) +\r\n                *(float *)(param_1 + 0x11c) * *(float *)(lVar4 + 0x324);\r\n    fVar6 = SQRT(fStack_88 * fStack_88 + fStack_84 * fStack_84 + 0.0);\r\n    if (fVar6 <= 1.1920929e-07) {\r\n      lVar3 = *(longlong *)(param_1 + 0x10);\r\n      uStack_78 = *(undefined4 *)(lVar3 + 0x30c);\r\n      uStack_74 = *(undefined4 *)(lVar3 + 0x310);\r\n      uStack_70 = *(undefined4 *)(lVar3 + 0x314);\r\n      uStack_6c = *(undefined4 *)(lVar3 + 0x318);\r\n      uStack_68 = *(undefined4 *)(lVar3 + 0x31c);\r\n      uStack_64 = *(undefined4 *)(lVar3 + 800);\r\n      uStack_60 = *(undefined4 *)(lVar3 + 0x324);\r\n      uStack_5c = *(undefined4 *)(lVar3 + 0x328);\r\n      uStack_58 = *(undefined4 *)(lVar3 + 0x32c);\r\n    }\r\n    else {\r\n      fVar6 = 1.0 / fVar6;\r\n      fStack_88 = fStack_88 * fVar6;\r\n      fStack_84 = fVar6 * fStack_84;\r\n      fStack_80 = fVar6 * 0.0;\r\n      puVar5 = (undefined4 *)FUN_140112000(&fStack_88,auStack_50);\r\n      uStack_78 = *puVar5;\r\n      uStack_74 = puVar5[1];\r\n      uStack_70 = puVar5[2];\r\n      uStack_6c = puVar5[3];\r\n      uStack_68 = puVar5[4];\r\n      uStack_64 = puVar5[5];\r\n      uStack_60 = puVar5[6];\r\n      uStack_5c = puVar5[7];\r\n      uStack_58 = puVar5[8];\r\n    }\r\n    FUN_14087d1e0(*(undefined8 *)(param_1 + 0x10),&fStack_98,&uStack_78,1);\r\n    return;\r\n  }\r\n  if (*(char *)(param_1 + 0x2b2) != '\\0') {\r\n    func_0x000140621240(param_1);\r\n    *(undefined1 *)(param_1 + 0x2b2) = 0;\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "14072a400",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nundefined8 FUN_14072a400(longlong param_1,longlong param_2,char param_3)\r\n\r\n{\r\n  byte bVar1;\r\n  byte bVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  bool bVar5;\r\n  bool bVar6;\r\n  bool bVar7;\r\n  bool bVar8;\r\n  char cVar9;\r\n  char cVar10;\r\n  uint uVar11;\r\n  int *piVar12;\r\n  char *pcVar13;\r\n  undefined8 uVar14;\r\n  int iVar15;\r\n  longlong lVar16;\r\n  float fVar17;\r\n  float fVar18;\r\n  float fVar19;\r\n  float fVar20;\r\n  \r\n  lVar16 = 0;\r\n  uVar11 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n  if (uVar11 != 0) {\r\n    piVar12 = *(int **)(param_1 + 0x1a8);\r\n    do {\r\n      if ((*piVar12 == *(int *)(param_2 + 0xa8)) && (*(char *)((longlong)piVar12 + 0x6e) == '\\0')) {\r\n        bVar8 = true;\r\n        goto code_r0x00014072a464;\r\n      }\r\n      lVar16 = lVar16 + 1;\r\n      piVar12 = piVar12 + 0x1c;\r\n    } while (lVar16 < (int)uVar11);\r\n  }\r\n  bVar8 = false;\r\ncode_r0x00014072a464:\r\n  if (((*(byte *)(param_2 + 0xd1) & 0x20) == 0) && ((*(byte *)(param_2 + 0xd1) & 8) == 0)) {\r\n    bVar7 = false;\r\n  }\r\n  else {\r\n    bVar7 = true;\r\n  }\r\n  bVar1 = *(byte *)(param_2 + 0x368);\r\n  cVar9 = FUN_1405bfbb0(param_2);\r\n  fVar20 = 0.0;\r\n  if ((*(int *)(param_1 + 0x29c) < 0) ||\r\n     (*(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                (longlong)*(int *)(param_1 + 0x29c) * 0x28) == 0.0)) {\r\n    bVar6 = false;\r\n  }\r\n  else {\r\n    bVar6 = true;\r\n  }\r\n  bVar2 = *(byte *)(param_2 + 0x368);\r\n  if (((-1 < *(int *)(param_1 + 0x2a4)) &&\r\n      (0.0 < *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x2a4) * 0x28))) ||\r\n     (cVar10 = FUN_1406dd490(), cVar10 == '\\0')) {\r\n    bVar5 = true;\r\n  }\r\n  else {\r\n    bVar5 = false;\r\n  }\r\n  if ((param_3 == '\\0') ||\r\n     (lVar16 = *(longlong *)(param_1 + 0x10),\r\n     fVar19 = *(float *)(param_2 + 0x300) - *(float *)(lVar16 + 0x300),\r\n     fVar17 = *(float *)(param_2 + 0x304) - *(float *)(lVar16 + 0x304),\r\n     fVar18 = *(float *)(param_2 + 0x308) - *(float *)(lVar16 + 0x308),\r\n     SQRT(fVar19 * fVar19 + fVar17 * fVar17 + fVar18 * fVar18) <\r\n     *(float *)(*(longlong *)(param_1 + 8) + 0x78))) {\r\n    bVar4 = true;\r\n  }\r\n  else {\r\n    bVar4 = false;\r\n  }\r\n  if (-1 < *(int *)(param_1 + 0x2a0)) {\r\n    fVar20 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x2a0) * 0x28);\r\n  }\r\n  iVar15 = 0;\r\n  uVar11 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n  if (uVar11 != 0) {\r\n    lVar16 = (longlong)(int)uVar11;\r\n    pcVar13 = (char *)(*(longlong *)(param_1 + 0x1a8) + 0x6d);\r\n    do {\r\n      if (*pcVar13 != '\\0') {\r\n        iVar15 = iVar15 + 1;\r\n      }\r\n      pcVar13 = pcVar13 + 0x70;\r\n      lVar16 = lVar16 + -1;\r\n    } while (lVar16 != 0);\r\n  }\r\n  uVar11 = *(uint *)(param_1 + 0x1a0) & 0xffffff;\r\n  if (uVar11 != 0) {\r\n    lVar16 = (longlong)(int)uVar11;\r\n    pcVar13 = (char *)(*(longlong *)(param_1 + 0x198) + 0x6d);\r\n    do {\r\n      if (*pcVar13 != '\\0') {\r\n        iVar15 = iVar15 + 1;\r\n      }\r\n      pcVar13 = pcVar13 + 0x70;\r\n      lVar16 = lVar16 + -1;\r\n    } while (lVar16 != 0);\r\n  }\r\n  if ((param_3 == '\\0') || (iVar15 < (int)fVar20)) {\r\n    bVar3 = true;\r\n  }\r\n  else {\r\n    bVar3 = false;\r\n  }\r\n  lVar16 = FUN_140877720(param_2,&DAT_1439ae4f0,2);\r\n  if ((((((!bVar3) || (!bVar4)) || (!bVar5)) ||\r\n       (((!bVar6 && (cVar9 != '\\0')) || ((bVar8 || ((bVar7 || ((bVar1 >> 1 & 1) != 0)))))))) ||\r\n      ((bVar2 >> 6 & 1) == 0)) ||\r\n     ((((lVar16 == 0 || (lVar16 = FUN_140877720(param_2,&DAT_1439cae10,2), lVar16 == 0)) ||\r\n       (lVar16 = FUN_140877720(param_2,&DAT_1439ba280,2), lVar16 == 0)) ||\r\n      (iVar15 = FUN_1407659c0(lVar16,0,3), 0xc < iVar15)))) {\r\n    uVar14 = 0;\r\n  }\r\n  else {\r\n    uVar14 = 1;\r\n  }\r\n  return uVar14;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nundefined8 FUN_14072a400(longlong param_1,longlong param_2,char param_3)\r\n\r\n{\r\n  byte bVar1;\r\n  byte bVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  bool bVar5;\r\n  bool bVar6;\r\n  bool bVar7;\r\n  bool bVar8;\r\n  char cVar9;\r\n  char cVar10;\r\n  uint uVar11;\r\n  int *piVar12;\r\n  char *pcVar13;\r\n  undefined8 uVar14;\r\n  int iVar15;\r\n  longlong lVar16;\r\n  float fVar17;\r\n  float fVar18;\r\n  float fVar19;\r\n  float fVar20;\r\n  \r\n  lVar16 = 0;\r\n  uVar11 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n  if (uVar11 != 0) {\r\n    piVar12 = *(int **)(param_1 + 0x1a8);\r\n    do {\r\n      if ((*piVar12 == *(int *)(param_2 + 0xa8)) && (*(char *)((longlong)piVar12 + 0x6e) == '\\0')) {\r\n        bVar8 = true;\r\n        goto code_r0x00014072a464;\r\n      }\r\n      lVar16 = lVar16 + 1;\r\n      piVar12 = piVar12 + 0x1c;\r\n    } while (lVar16 < (int)uVar11);\r\n  }\r\n  bVar8 = false;\r\ncode_r0x00014072a464:\r\n  if (((*(byte *)(param_2 + 0xd1) & 0x20) == 0) && ((*(byte *)(param_2 + 0xd1) & 8) == 0)) {\r\n    bVar7 = false;\r\n  }\r\n  else {\r\n    bVar7 = true;\r\n  }\r\n  bVar1 = *(byte *)(param_2 + 0x368);\r\n  cVar9 = FUN_1405bfbb0(param_2);\r\n  fVar20 = 0.0;\r\n  if ((*(int *)(param_1 + 0x29c) < 0) ||\r\n     (*(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                (longlong)*(int *)(param_1 + 0x29c) * 0x28) == 0.0)) {\r\n    bVar6 = false;\r\n  }\r\n  else {\r\n    bVar6 = true;\r\n  }\r\n  bVar2 = *(byte *)(param_2 + 0x368);\r\n  if (((-1 < *(int *)(param_1 + 0x2a4)) &&\r\n      (0.0 < *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x2a4) * 0x28))) ||\r\n     (cVar10 = FUN_1406dd490(), cVar10 == '\\0')) {\r\n    bVar5 = true;\r\n  }\r\n  else {\r\n    bVar5 = false;\r\n  }\r\n  if ((param_3 == '\\0') ||\r\n     (lVar16 = *(longlong *)(param_1 + 0x10),\r\n     fVar19 = *(float *)(param_2 + 0x300) - *(float *)(lVar16 + 0x300),\r\n     fVar17 = *(float *)(param_2 + 0x304) - *(float *)(lVar16 + 0x304),\r\n     fVar18 = *(float *)(param_2 + 0x308) - *(float *)(lVar16 + 0x308),\r\n     SQRT(fVar19 * fVar19 + fVar17 * fVar17 + fVar18 * fVar18) <\r\n     *(float *)(*(longlong *)(param_1 + 8) + 0x78))) {\r\n    bVar4 = true;\r\n  }\r\n  else {\r\n    bVar4 = false;\r\n  }\r\n  if (-1 < *(int *)(param_1 + 0x2a0)) {\r\n    fVar20 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x2a0) * 0x28);\r\n  }\r\n  iVar15 = 0;\r\n  uVar11 = *(uint *)(param_1 + 0x1b0) & 0xffffff;\r\n  if (uVar11 != 0) {\r\n    lVar16 = (longlong)(int)uVar11;\r\n    pcVar13 = (char *)(*(longlong *)(param_1 + 0x1a8) + 0x6d);\r\n    do {\r\n      if (*pcVar13 != '\\0') {\r\n        iVar15 = iVar15 + 1;\r\n      }\r\n      pcVar13 = pcVar13 + 0x70;\r\n      lVar16 = lVar16 + -1;\r\n    } while (lVar16 != 0);\r\n  }\r\n  uVar11 = *(uint *)(param_1 + 0x1a0) & 0xffffff;\r\n  if (uVar11 != 0) {\r\n    lVar16 = (longlong)(int)uVar11;\r\n    pcVar13 = (char *)(*(longlong *)(param_1 + 0x198) + 0x6d);\r\n    do {\r\n      if (*pcVar13 != '\\0') {\r\n        iVar15 = iVar15 + 1;\r\n      }\r\n      pcVar13 = pcVar13 + 0x70;\r\n      lVar16 = lVar16 + -1;\r\n    } while (lVar16 != 0);\r\n  }\r\n  if ((param_3 == '\\0') || (iVar15 < (int)fVar20)) {\r\n    bVar3 = true;\r\n  }\r\n  else {\r\n    bVar3 = false;\r\n  }\r\n  lVar16 = FUN_140877720(param_2,&DAT_1439ae4f0,2);\r\n  if ((((((!bVar3) || (!bVar4)) || (!bVar5)) ||\r\n       (((!bVar6 && (cVar9 != '\\0')) || ((bVar8 || ((bVar7 || ((bVar1 >> 1 & 1) != 0)))))))) ||\r\n      ((bVar2 >> 6 & 1) == 0)) ||\r\n     ((((lVar16 == 0 || (lVar16 = FUN_140877720(param_2,&DAT_1439cae10,2), lVar16 == 0)) ||\r\n       (lVar16 = FUN_140877720(param_2,&DAT_1439ba280,2), lVar16 == 0)) ||\r\n      (iVar15 = FUN_1407659c0(lVar16,0,3), 0xc < iVar15)))) {\r\n    uVar14 = 0;\r\n  }\r\n  else {\r\n    uVar14 = 1;\r\n  }\r\n  return uVar14;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "14072d600",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nundefined8 FUN_14072d600(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  int *piVar2;\r\n  float *pfVar3;\r\n  longlong lVar4;\r\n  uint uVar5;\r\n  int *piVar6;\r\n  undefined4 auStackX_10 [2];\r\n  \r\n  piVar6 = (int *)((longlong)param_2 * 0x70 + *(longlong *)(param_1 + 0x1a8));\r\n  if (*(char *)((longlong)piVar6 + 0x6d) != '\\0') {\r\n    lVar1 = *(longlong *)(param_1 + 8);\r\n    lVar4 = 0;\r\n    uVar5 = *(uint *)(param_1 + 0x210) & 0xffffff;\r\n    if (uVar5 != 0) {\r\n      piVar2 = *(int **)(param_1 + 0x208);\r\n      do {\r\n        if (*piVar2 == *piVar6) {\r\n          auStackX_10[0] = 0;\r\n          piVar6[0x17] = 0;\r\n          goto code_r0x00014072d67d;\r\n        }\r\n        lVar4 = lVar4 + 1;\r\n        piVar2 = piVar2 + 6;\r\n      } while (lVar4 < (longlong)(ulonglong)uVar5);\r\n    }\r\n    pfVar3 = (float *)FUN_140877eb0(*(undefined8 *)(param_1 + 0x10),auStackX_10);\r\n    piVar6[0x17] = (int)(*pfVar3 + (float)piVar6[0x17]);\r\ncode_r0x00014072d67d:\r\n    if (*(float *)(lVar1 + 0x1b8) <= (float)piVar6[0x17]) {\r\n      FUN_14072b0e0(param_1,param_2,0);\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nundefined8 FUN_14072d600(longlong param_1,int param_2)\r\n\r\n{\r\n  longlong lVar1;\r\n  int *piVar2;\r\n  float *pfVar3;\r\n  longlong lVar4;\r\n  uint uVar5;\r\n  int *piVar6;\r\n  undefined4 auStackX_10 [2];\r\n  \r\n  piVar6 = (int *)((longlong)param_2 * 0x70 + *(longlong *)(param_1 + 0x1a8));\r\n  if (*(char *)((longlong)piVar6 + 0x6d) != '\\0') {\r\n    lVar1 = *(longlong *)(param_1 + 8);\r\n    lVar4 = 0;\r\n    uVar5 = *(uint *)(param_1 + 0x210) & 0xffffff;\r\n    if (uVar5 != 0) {\r\n      piVar2 = *(int **)(param_1 + 0x208);\r\n      do {\r\n        if (*piVar2 == *piVar6) {\r\n          auStackX_10[0] = 0;\r\n          piVar6[0x17] = 0;\r\n          goto code_r0x00014072d67d;\r\n        }\r\n        lVar4 = lVar4 + 1;\r\n        piVar2 = piVar2 + 6;\r\n      } while (lVar4 < (longlong)(ulonglong)uVar5);\r\n    }\r\n    pfVar3 = (float *)FUN_140877eb0(*(undefined8 *)(param_1 + 0x10),auStackX_10);\r\n    piVar6[0x17] = (int)(*pfVar3 + (float)piVar6[0x17]);\r\ncode_r0x00014072d67d:\r\n    if (*(float *)(lVar1 + 0x1b8) <= (float)piVar6[0x17]) {\r\n      FUN_14072b0e0(param_1,param_2,0);\r\n      return 1;\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
]

````

## Unescaped text fields for reading

### text

````text

void FUN_14072d6e0(longlong param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int *piVar3;
  bool bVar4;
  char cVar5;
  uint uVar6;
  longlong *plVar7;
  longlong lVar8;
  longlong lVar9;
  undefined8 uVar10;
  ulonglong uVar11;
  int iVar12;
  float *pfVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  int iVar16;
  uint uVar17;
  longlong lVar18;
  ulonglong uVar19;
  uint uVar20;
  int *piVar21;
  int iVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  float afStackX_8 [2];
  float afStackX_10 [2];
  float fStack_168;
  float fStack_164;
  float fStack_160;
  undefined8 uStack_158;
  uint uStack_150;
  uint uStack_14c;
  longlong lStack_148;
  undefined4 uStack_140;
  undefined4 uStack_13c;
  undefined4 uStack_138;
  undefined4 uStack_134;
  undefined4 uStack_130;
  undefined4 uStack_12c;
  undefined4 uStack_128;
  undefined4 uStack_124;
  undefined4 uStack_120;
  undefined8 uStack_118;
  undefined1 auStack_110 [80];
  undefined *puStack_c0;
  undefined8 uStack_98;
  undefined1 *puStack_88;
  ulonglong uStack_80;
  undefined1 auStack_78 [64];
  
  uStack_118 = 0xfffffffffffffffe;
  FUN_1406debc0(param_1 + 0x1f0);
  FUN_140729fb0(param_1);
  bVar4 = false;
  if (*(int *)(param_1 + 0x2c) == 3) {
    lVar8 = 0;
    if (((*(uint *)(param_1 + 0x1a0) & 0xffffff) != 0) &&
       (lVar8 = 0, *(char *)((longlong)*(undefined4 **)(param_1 + 0x198) + 0x6e) != '\0')) {
      lVar8 = FUN_1406dc560(**(undefined4 **)(param_1 + 0x198));
    }
    iVar16 = 0;
    if ((*(uint *)(param_1 + 0x210) & 0xffffff) != 0) {
      lVar18 = 0;
      do {
        afStackX_8[0] = *(float *)(lVar18 + *(longlong *)(param_1 + 0x208));
        lVar9 = FUN_1407b5e00(D2_GameSingletonPtr + 0x62616,afStackX_8);
        if (lVar9 == lVar8) {
          cVar5 = FUN_14072a400(param_1,lVar9,0);
          if (cVar5 != '\0') {
            bVar4 = true;
            goto code_r0x00014072d7f0;
          }
          break;
        }
        iVar16 = iVar16 + 1;
        lVar18 = lVar18 + 0x18;
      } while (iVar16 < (int)(*(uint *)(param_1 + 0x210) & 0xffffff));
    }
    lVar8 = 0;
code_r0x00014072d7f0:
    iVar16 = (*(uint *)(param_1 + 0x1a0) & 0xffffff) - 1;
    if (-1 < iVar16) {
      lVar18 = (longlong)iVar16 * 0x70;
      do {
        lVar9 = FUN_1406dc560(*(undefined4 *)(lVar18 + *(longlong *)(param_1 + 0x198)));
        if ((!bVar4) || (lVar9 != lVar8)) {
          FUN_14072b260(param_1 + 0x198,iVar16);
        }
        lVar18 = lVar18 + -0x70;
        iVar16 = iVar16 + -1;
      } while (-1 < iVar16);
    }
    uVar11 = 0;
    if (((*(char *)(param_1 + 0xec) == '\0') && (lVar8 == 0)) &&
       (uVar14 = uVar11, uVar15 = uVar11, (*(uint *)(param_1 + 0x210) & 0xffffff) != 0)) {
      do {
        afStackX_8[0] = *(float *)(uVar15 + *(longlong *)(param_1 + 0x208));
        uVar10 = FUN_1407b5e00(D2_GameSingletonPtr + 0x62616,afStackX_8);
        cVar5 = FUN_14072d340(param_1,uVar10,1);
        if (cVar5 != '\0') {
          afStackX_8[0] = 1e+30;
          *(undefined4 *)(param_1 + 0x150) = 0x7149f2ca;
          break;
        }
        uVar17 = (int)uVar14 + 1;
        uVar14 = (ulonglong)uVar17;
        uVar15 = uVar15 + 0x18;
      } while ((int)uVar17 < (int)(*(uint *)(param_1 + 0x210) & 0xffffff));
    }
    uVar14 = uVar11;
    if ((*(uint *)(param_1 + 400) & 0xffffff) != 0) {
      do {
        uVar10 = FUN_1406dc560(*(undefined4 *)
                                (*(longlong *)(param_1 + 0x188) + (longlong)(int)uVar14 * 4));
        lVar8 = FUN_1408f2e50(uVar10);
        if (lVar8 != 0) {
          cVar5 = FUN_14072a400(param_1,lVar8,0);
          if (cVar5 == '\0') {
            if ((*(byte *)(lVar8 + 0x368) & 0x40) == 0) {
              afStackX_8[0] = *(float *)(lVar8 + 0xa8);
              iVar16 = (*(uint *)(param_1 + 0x180) & 0xffffff) - 1;
              if (-1 < iVar16) {
                pfVar13 = *(float **)(param_1 + 0x178);
                uVar15 = uVar11;
                uVar19 = uVar11;
                do {
                  if (*pfVar13 == afStackX_8[0]) {
                    if ((int)uVar19 != -1) goto code_r0x00014072da86;
                    break;
                  }
                  uVar19 = (ulonglong)((int)uVar19 + 1);
                  uVar15 = uVar15 + 1;
                  pfVar13 = pfVar13 + 1;
                } while ((longlong)uVar15 <= (longlong)iVar16);
              }
              FUN_14052e460(param_1 + 0x178,afStackX_8);
              uStack_158 = lRam000000014398dad8;
              uStack_150 = 0x5d;
              FUN_140131ae0(&lStack_148);
              uVar2 = *(undefined4 *)(lVar8 + 0xa8);
              lVar8 = FUN_140810240();
              plVar7 = (longlong *)FUN_1408112c0(lVar8 + 0xc8e8,0x4f,0x18,uVar2);
              if (plVar7 != (longlong *)0x0) {
                *plVar7 = uStack_158;
                plVar7[1] = CONCAT44(uStack_14c,uStack_150);
                plVar7[2] = lStack_148;
              }
            }
          }
          else {
            uVar10 = FUN_1407281e0(auStack_110,lVar8,0);
            iVar16 = FUN_140728720(param_1 + 0x198,uVar10);
            lVar18 = FUN_140878470(lVar8);
            if (lVar18 != 0) {
              lVar9 = *(longlong *)(param_1 + 0x198);
              uVar10 = FUN_14025c0c0(*(longlong *)(lVar18 + 0x1fe8) + 0x108,&DAT_1432d07a0,
                                     *(longlong *)(param_1 + 8) + 0x168);
              FUN_14025b6a0((longlong)iVar16 * 0x70 + lVar9 + 0x3c,uVar10);
            }
            uVar10 = FUN_140877720(lVar8,&DAT_1439ba280);
            FUN_140768740(uVar10,0xe);
          }
        }
code_r0x00014072da86:
        uVar17 = (int)uVar14 + 1;
        uVar14 = (ulonglong)uVar17;
      } while ((int)uVar17 < (int)(*(uint *)(param_1 + 400) & 0xffffff));
    }
    *(uint *)(param_1 + 400) = *(uint *)(param_1 + 400) & 0xff000000;
  }
  uVar11 = 0;
  lVar8 = *(longlong *)(param_1 + 8);
  if (*(float *)(param_1 + 0x1e8) <= *(float *)(lVar8 + 0x9c)) {
    fVar27 = 1.0 - *(float *)(param_1 + 0x1e8) / *(float *)(lVar8 + 0x9c);
    if (0.0 <= fVar27) {
      if (1.0 < fVar27) {
        fVar27 = 1.0;
      }
    }
    else {
      fVar27 = 0.0;
    }
    *(float *)(param_1 + 0x160) = fVar27;
  }
  else {
    *(undefined4 *)(param_1 + 0x160) = 0;
  }
  if (*(int *)(param_1 + 0x2c) != 3) {
    FUN_14053d940(*(undefined8 *)(param_1 + 0x1c0),0,*(undefined2 *)(param_1 + 0x1d8),&fStack_168,
                  &uStack_140);
    *(float *)(param_1 + 0xf0) = fStack_168;
    *(float *)(param_1 + 0xf4) = fStack_164;
    fVar27 = fStack_160;
    goto code_r0x00014072e459;
  }
  if (*(float *)(param_1 + 0x150) == 1e+30) {
    *(undefined1 *)(param_1 + 0xec) = 0;
    iVar16 = -1;
    if (((*(uint *)(param_1 + 0x1a0) & 0xffffff) == 0) ||
       (*(char *)(*(longlong *)(param_1 + 0x198) + 0x6e) != '\x01')) {
      uVar17 = *(uint *)(param_1 + 0x138) & 0xffffff;
      iVar22 = -1;
      if (uVar17 != 0) {
        if ((*(uint *)(param_1 + 0x1b0) & 0xffffff) != 0) {
          uStack_158 = 0;
          FUN_1401548e0();
          uStack_158 = 0;
          uStack_14c = 0x3f;
          uStack_150 = 0x5000000;
          uVar17 = 0;
          if ((*(uint *)(param_1 + 0x138) & 0xffffff) != 0) {
            FUN_140384fe0(&uStack_158);
            uVar17 = uStack_14c;
          }
          piVar3 = *(int **)(param_1 + 0x1a8);
          uVar6 = *(uint *)(param_1 + 0x1b0);
          lVar18 = uStack_158;
          uVar20 = uStack_150;
          if (piVar3 != piVar3 + (ulonglong)(uVar6 & 0xffffff) * 0x1c) {
            piVar21 = piVar3 + 0x18;
            do {
              iVar16 = (uVar20 & 0xffffff) - 1;
              if (-1 < iVar16) {
                uVar14 = uVar11;
                uVar15 = uVar11;
                do {
                  if (*(int *)(lVar18 + uVar14 * 4) == *piVar21) {
                    if (-1 < (int)uVar15) goto code_r0x00014072dc74;
                    break;
                  }
                  uVar15 = (ulonglong)((int)uVar15 + 1);
                  uVar14 = uVar14 + 1;
                } while ((longlong)uVar14 <= (longlong)iVar16);
              }
              FUN_14001d6b0(&uStack_158,piVar21);
              lVar18 = uStack_158;
              uVar20 = uStack_150;
code_r0x00014072dc74:
              piVar1 = piVar21 + 4;
              piVar21 = piVar21 + 0x1c;
              uVar17 = uStack_14c;
            } while (piVar1 != piVar3 + (ulonglong)(uVar6 & 0xffffff) * 0x1c);
          }
          iVar16 = (uVar20 & 0xffffff) - 1;
          if (-1 < iVar16) {
            uVar14 = uVar11;
            uVar15 = uVar11;
            do {
              iVar22 = (int)uVar15;
              if (*(int *)(lVar18 + uVar14 * 4) == *(int *)(param_1 + 0x15c))
              goto code_r0x00014072dcb6;
              uVar15 = (ulonglong)(iVar22 + 1);
              uVar14 = uVar14 + 1;
            } while ((longlong)uVar14 <= (longlong)iVar16);
          }
          iVar22 = -1;
code_r0x00014072dcb6:
          uVar14 = (ulonglong)(iVar22 + 1U);
          if ((int)(uVar20 & 0xffffff) <= (int)(iVar22 + 1U)) {
            uVar14 = uVar11;
          }
          iVar16 = *(int *)(lVar18 + (longlong)(int)uVar14 * 4);
          if (((uVar17 & 0x40) == 0) && (lVar18 != 0)) {
            plVar7 = (longlong *)FUN_1401548e0();
            (**(code **)(*plVar7 + 0x10))
                      (plVar7,
                       "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                       ,0x1ec,lVar18,0x10);
          }
          goto code_r0x00014072dd7b;
        }
        iVar22 = iVar16;
        if (*(int *)(param_1 + 0x15c) != -1) {
          uVar14 = uVar11;
          if (0 < (int)(uVar17 - 2)) {
            uVar6 = (int)D2_GameSingletonPtr[0xc01f] * 0x19660d + 0x3c6ef35f;
            *(uint *)(D2_GameSingletonPtr + 0xc01f) = uVar6;
            uVar14 = (longlong)(ulonglong)(uVar6 >> 10 & 0x7fff) % (longlong)(int)(uVar17 - 1) &
                     0xffffffff;
          }
          iVar12 = -1;
          uVar17 = *(uint *)(param_1 + 0x138) & 0xffffff;
          if (uVar17 != 0) {
            do {
              iVar16 = (int)uVar11;
              if ((iVar16 != *(int *)(param_1 + 0x15c)) &&
                 (iVar12 = iVar12 + 1, iVar12 == (int)uVar14)) goto code_r0x00014072dd7b;
              uVar11 = (ulonglong)(iVar16 + 1U);
            } while ((int)(iVar16 + 1U) < (int)uVar17);
          }
        }
      }
    }
    else {
      iVar16 = *(int *)(*(longlong *)(param_1 + 0x198) + 0x60);
      *(undefined1 *)(param_1 + 0xec) = 1;
code_r0x00014072dd7b:
      iVar22 = iVar16;
      if ((iVar16 != -1) && (iVar16 != *(int *)(param_1 + 0x15c))) {
        FUN_14072cca0(param_1);
      }
    }
    *(int *)(param_1 + 0x15c) = iVar22;
    *(undefined4 *)(param_1 + 0xfc) = *(undefined4 *)(param_1 + 0xf0);
    *(undefined4 *)(param_1 + 0x100) = *(undefined4 *)(param_1 + 0xf4);
    *(undefined4 *)(param_1 + 0x104) = *(undefined4 *)(param_1 + 0xf8);
    *(undefined4 *)(param_1 + 0x150) = 0;
    fVar27 = (float)(*(uint *)(param_1 + 0x1b0) & 0xffffff);
    if (*(char *)(param_1 + 0xec) == '\0') {
      fVar24 = *(float *)(lVar8 + 0xf0);
      afStackX_8[0] = (float)FUN_1406dd680(lVar8 + 0xe8);
      *(float *)(param_1 + 0x154) = afStackX_8[0];
      fVar25 = *(float *)(param_1 + 0x154) - fVar27 * fVar24;
      fVar24 = 0.0;
      if (0.0 <= fVar25) {
        fVar24 = fVar25;
      }
      *(float *)(param_1 + 0x154) = fVar24;
    }
    else {
      afStackX_8[0] = 0.5;
      *(undefined4 *)(param_1 + 0x154) = 0x3f000000;
    }
    lVar18 = *(longlong *)(param_1 + 0x10);
    fStack_168 = *(float *)(lVar18 + 0x300);
    fStack_164 = *(float *)(lVar18 + 0x304);
    fStack_160 = *(float *)(lVar18 + 0x308);
    uStack_140 = *(undefined4 *)(lVar18 + 0x30c);
    uStack_13c = *(undefined4 *)(lVar18 + 0x310);
    uStack_138 = *(undefined4 *)(lVar18 + 0x314);
    uStack_134 = *(undefined4 *)(lVar18 + 0x318);
    uStack_130 = *(undefined4 *)(lVar18 + 0x31c);
    uStack_12c = *(undefined4 *)(lVar18 + 800);
    uStack_128 = *(undefined4 *)(lVar18 + 0x324);
    uStack_124 = *(undefined4 *)(lVar18 + 0x328);
    uStack_120 = *(undefined4 *)(lVar18 + 0x32c);
    if (*(int *)(param_1 + 0x15c) != -1) {
      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,
                    (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),
                    &fStack_168,&uStack_140);
    }
    fVar25 = *(float *)(param_1 + 0xf0) - fStack_168;
    fVar26 = *(float *)(param_1 + 0xf4) - fStack_164;
    fVar24 = *(float *)(param_1 + 0xf8) - fStack_160;
    afStackX_8[0] =
         SQRT(fVar26 * fVar26 + fVar25 * fVar25 + fVar24 * fVar24) /
         (fVar27 * *(float *)(lVar8 + 0xf8) + *(float *)(lVar8 + 0xf4));
    *(float *)(param_1 + 0x158) = afStackX_8[0];
  }
  FUN_140877eb0(*(undefined8 *)(param_1 + 0x10),afStackX_10);
  afStackX_10[0] = afStackX_10[0] + *(float *)(param_1 + 0x150);
  *(float *)(param_1 + 0x150) = afStackX_10[0];
  if (*(float *)(param_1 + 0x158) <= afStackX_10[0]) {
    if (*(float *)(param_1 + 0x158) + *(float *)(param_1 + 0x154) <= afStackX_10[0]) {
      *(undefined4 *)(param_1 + 0x150) = 0x7149f2ca;
      *(undefined1 *)(param_1 + 0xec) = 0;
    }
    else {
      if (((*(char *)(param_1 + 0xec) != '\0') && ((*(uint *)(param_1 + 0x1a0) & 0xffffff) != 0)) &&
         (*(char *)(*(longlong *)(param_1 + 0x198) + 0x6e) == '\x01')) {
        *(undefined1 *)(*(longlong *)(param_1 + 0x198) + 0x6e) = 0;
      }
      lVar18 = *(longlong *)(param_1 + 0x10);
      fStack_168 = *(float *)(lVar18 + 0x300);
      fStack_164 = *(float *)(lVar18 + 0x304);
      fStack_160 = *(float *)(lVar18 + 0x308);
      uStack_140 = *(undefined4 *)(lVar18 + 0x30c);
      uStack_13c = *(undefined4 *)(lVar18 + 0x310);
      uStack_138 = *(undefined4 *)(lVar18 + 0x314);
      uStack_134 = *(undefined4 *)(lVar18 + 0x318);
      uStack_130 = *(undefined4 *)(lVar18 + 0x31c);
      uStack_12c = *(undefined4 *)(lVar18 + 800);
      uStack_128 = *(undefined4 *)(lVar18 + 0x324);
      uStack_124 = *(undefined4 *)(lVar18 + 0x328);
      uStack_120 = *(undefined4 *)(lVar18 + 0x32c);
      if (*(int *)(param_1 + 0x15c) != -1) {
        FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,
                      (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),
                      &fStack_168,&uStack_140);
      }
      *(float *)(param_1 + 0xf0) = fStack_168;
      *(float *)(param_1 + 0xf4) = fStack_164;
      *(float *)(param_1 + 0xf8) = fStack_160;
    }
  }
  else {
    lVar18 = *(longlong *)(param_1 + 0x10);
    fStack_168 = *(float *)(lVar18 + 0x300);
    fStack_164 = *(float *)(lVar18 + 0x304);
    fStack_160 = *(float *)(lVar18 + 0x308);
    uStack_140 = *(undefined4 *)(lVar18 + 0x30c);
    uStack_13c = *(undefined4 *)(lVar18 + 0x310);
    uStack_138 = *(undefined4 *)(lVar18 + 0x314);
    uStack_134 = *(undefined4 *)(lVar18 + 0x318);
    uStack_130 = *(undefined4 *)(lVar18 + 0x31c);
    uStack_12c = *(undefined4 *)(lVar18 + 800);
    uStack_128 = *(undefined4 *)(lVar18 + 0x324);
    uStack_124 = *(undefined4 *)(lVar18 + 0x328);
    uStack_120 = *(undefined4 *)(lVar18 + 0x32c);
    if (*(int *)(param_1 + 0x15c) != -1) {
      FUN_14053dac0(*(undefined8 *)(param_1 + 0x1c0),0,
                    (longlong)*(int *)(param_1 + 0x15c) * 0x20 + *(longlong *)(param_1 + 0x130),
                    &fStack_168,&uStack_140);
    }
    fVar27 = *(float *)(param_1 + 0x150) / *(float *)(param_1 + 0x158);
    if (0.0 <= fVar27) {
      if (1.0 < fVar27) {
        fVar27 = 1.0;
      }
    }
    else {
      fVar27 = 0.0;
    }
    fVar27 = fVar27 * 3.0 * fVar27 - fVar27 * 2.0 * fVar27 * fVar27;
    *(float *)(param_1 + 0xf0) =
         fVar27 * (fStack_168 - *(float *)(param_1 + 0xfc)) + *(float *)(param_1 + 0xfc);
    *(float *)(param_1 + 0xf4) =
         fVar27 * (fStack_164 - *(float *)(param_1 + 0x100)) + *(float *)(param_1 + 0x100);
    *(float *)(param_1 + 0xf8) =
         fVar27 * (fStack_160 - *(float *)(param_1 + 0x104)) + *(float *)(param_1 + 0x104);
  }
  fVar27 = *(float *)(param_1 + 0x160);
  fVar24 = *(float *)(lVar8 + 0x100);
  fVar25 = *(float *)(lVar8 + 0xfc);
  if (ABS(fVar24) <= 1e-18) {
    fVar24 = 0.0;
  }
  if (ABS(fVar25) <= 1e-18) {
    fVar25 = 0.0;
  }
  fVar26 = (1.0 - fVar27) * fVar25 + fVar24 * fVar27;
  fVar24 = *(float *)(lVar8 + 0x108);
  fVar25 = *(float *)(lVar8 + 0x104);
  if (ABS(fVar24) <= 1e-18) {
    fVar24 = 0.0;
  }
  if (ABS(fVar25) <= 1e-18) {
    fVar25 = 0.0;
  }
  fVar27 = ((1.0 - fVar27) * fVar25 + fVar24 * fVar27) * DAT_14228a74c * *(float *)(param_1 + 0x1e8)
  ;
  if ((fVar27 < 0.0) || (fVar24 = fVar27, 6.2831855 <= fVar27)) {
    fVar24 = (float)FUN_1411ad95c(fVar27 * 0.15915494);
    fVar24 = fVar27 - fVar24 * 6.2831855;
  }
  fVar24 = (float)FUN_1411ae130(fVar24);
  fVar25 = fVar27 + *(float *)(lVar8 + 0x10c);
  if ((fVar25 < 0.0) || (6.2831855 <= fVar25)) {
    fVar23 = (float)FUN_1411ad95c(fVar25 * 0.15915494);
    fVar25 = fVar25 - fVar23 * 6.2831855;
  }
  fVar25 = (float)FUN_1411ae130(fVar25);
  fVar27 = fVar27 + *(float *)(lVar8 + 0x110);
  if ((fVar27 < 0.0) || (6.2831855 <= fVar27)) {
    fVar23 = (float)FUN_1411ad95c(fVar27 * 0.15915494);
    fVar27 = fVar27 - fVar23 * 6.2831855;
  }
  fVar27 = (float)FUN_1411ae130(fVar27);
  *(float *)(param_1 + 0xf0) = fVar24 * fVar26 + *(float *)(param_1 + 0xf0);
  *(float *)(param_1 + 0xf4) = fVar25 * fVar26 + *(float *)(param_1 + 0xf4);
  fVar27 = fVar27 * fVar26 + *(float *)(param_1 + 0xf8);
code_r0x00014072e459:
  *(float *)(param_1 + 0xf8) = fVar27;
  if (*(longlong *)(param_1 + 0xe0) != 0) {
    FUN_140342360(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231daf8);
    uStack_158 = *(undefined8 *)(param_1 + 0xf0);
    uStack_150 = *(undefined4 *)(param_1 + 0xf8);
    uStack_14c = 0;
    FUN_140343990(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231daf8,&uStack_158);
    FUN_140342360(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231db18);
    uStack_150 = *(uint *)(param_1 + 0x128);
    uStack_158 = CONCAT44(uStack_150,uStack_150);
    uStack_14c = uStack_150;
    FUN_140343990(*(longlong *)(param_1 + 0xe0) + 0xd0,uRam000000014231db18,&uStack_158);
  }
  uVar10 = (**(code **)(*D2_GameSingletonPtr + 0x388))();
  if ((*(int *)(param_1 + 0xe8) != -1) &&
     (lVar8 = FUN_1401a04a0(uVar10,*(undefined4 *)(param_1 + 0xe8)), lVar8 != 0)) {
    *(undefined4 *)(lVar8 + 0x38) = *(undefined4 *)(param_1 + 0xf0);
    *(undefined4 *)(lVar8 + 0x3c) = *(undefined4 *)(param_1 + 0xf4);
    *(undefined4 *)(lVar8 + 0x40) = *(undefined4 *)(param_1 + 0xf8);
  }
  uStack_98 = 0xfffffffffffffffe;
  if (*(int *)(param_1 + 0xb0) != -1) {
    puStack_88 = auStack_78;
    uStack_80 = 0x8000000100000000;
    puStack_c0 = &UNK_14072b663;
    cVar5 = FUN_140092630(D2_GameSingletonPtr + 0xc050,(int *)(param_1 + 0xb0),&puStack_88);
    if ((cVar5 != '\0') && ((int)uStack_80 != 0)) {
      puStack_c0 = &UNK_14072b67b;
      func_0x000140621240(param_1);
    }
    puStack_c0 = &UNK_14072b681;
    plVar7 = (longlong *)FUN_140a9de90();
    uVar14 = uStack_80;
    uVar11 = uStack_80 >> 0x20;
    uStack_80 = uStack_80 & 0xffffffff00000000;
    if (-1 < (longlong)uVar14) {
      puStack_c0 = &UNK_14072b6ad;
      (**(code **)(*plVar7 + 0x20))(plVar7,puStack_88,((uint)uVar11 & 0x3fffffff) * 0x70);
    }
  }
  return;
}


````

### text

````text

void FUN_14072eed0(longlong param_1)

{
  float fVar1;
  float fVar2;
  longlong lVar3;
  longlong lVar4;
  undefined4 *puVar5;
  float fVar6;
  float fStack_98;
  float fStack_94;
  float fStack_90;
  float fStack_88;
  float fStack_84;
  float fStack_80;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined1 auStack_50 [72];
  
  lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));
  if (lVar3 != 0) {
    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));
    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x120));
    fVar6 = *(float *)(param_1 + 0x108);
    fVar1 = *(float *)(param_1 + 0x10c);
    fVar2 = *(float *)(param_1 + 0x110);
    fStack_98 = fVar6 * *(float *)(lVar4 + 0x30c) + fVar1 * *(float *)(lVar4 + 0x318) +
                fVar2 * *(float *)(lVar4 + 0x324) + *(float *)(lVar3 + 0x300);
    fStack_94 = fVar1 * *(float *)(lVar4 + 0x31c) + fVar6 * *(float *)(lVar4 + 0x310) +
                fVar2 * *(float *)(lVar4 + 0x328) + *(float *)(lVar3 + 0x304);
    fStack_90 = fVar1 * *(float *)(lVar4 + 800) + fVar6 * *(float *)(lVar4 + 0x314) +
                fVar2 * *(float *)(lVar4 + 0x32c) + *(float *)(lVar3 + 0x308);
    fStack_80 = 0.0;
    fStack_84 = *(float *)(param_1 + 0x114) * *(float *)(lVar4 + 0x310) +
                *(float *)(param_1 + 0x118) * *(float *)(lVar4 + 0x31c) +
                *(float *)(param_1 + 0x11c) * *(float *)(lVar4 + 0x328);
    fStack_88 = *(float *)(param_1 + 0x114) * *(float *)(lVar4 + 0x30c) +
                *(float *)(param_1 + 0x118) * *(float *)(lVar4 + 0x318) +
                *(float *)(param_1 + 0x11c) * *(float *)(lVar4 + 0x324);
    fVar6 = SQRT(fStack_88 * fStack_88 + fStack_84 * fStack_84 + 0.0);
    if (fVar6 <= 1.1920929e-07) {
      lVar3 = *(longlong *)(param_1 + 0x10);
      uStack_78 = *(undefined4 *)(lVar3 + 0x30c);
      uStack_74 = *(undefined4 *)(lVar3 + 0x310);
      uStack_70 = *(undefined4 *)(lVar3 + 0x314);
      uStack_6c = *(undefined4 *)(lVar3 + 0x318);
      uStack_68 = *(undefined4 *)(lVar3 + 0x31c);
      uStack_64 = *(undefined4 *)(lVar3 + 800);
      uStack_60 = *(undefined4 *)(lVar3 + 0x324);
      uStack_5c = *(undefined4 *)(lVar3 + 0x328);
      uStack_58 = *(undefined4 *)(lVar3 + 0x32c);
    }
    else {
      fVar6 = 1.0 / fVar6;
      fStack_88 = fStack_88 * fVar6;
      fStack_84 = fVar6 * fStack_84;
      fStack_80 = fVar6 * 0.0;
      puVar5 = (undefined4 *)FUN_140112000(&fStack_88,auStack_50);
      uStack_78 = *puVar5;
      uStack_74 = puVar5[1];
      uStack_70 = puVar5[2];
      uStack_6c = puVar5[3];
      uStack_68 = puVar5[4];
      uStack_64 = puVar5[5];
      uStack_60 = puVar5[6];
      uStack_5c = puVar5[7];
      uStack_58 = puVar5[8];
    }
    FUN_14087d1e0(*(undefined8 *)(param_1 + 0x10),&fStack_98,&uStack_78,1);
    return;
  }
  if (*(char *)(param_1 + 0x2b2) != '\0') {
    func_0x000140621240(param_1);
    *(undefined1 *)(param_1 + 0x2b2) = 0;
  }
  return;
}


````

### text

````text

undefined8 FUN_14072a400(longlong param_1,longlong param_2,char param_3)

{
  byte bVar1;
  byte bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  bool bVar7;
  bool bVar8;
  char cVar9;
  char cVar10;
  uint uVar11;
  int *piVar12;
  char *pcVar13;
  undefined8 uVar14;
  int iVar15;
  longlong lVar16;
  float fVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  
  lVar16 = 0;
  uVar11 = *(uint *)(param_1 + 0x1b0) & 0xffffff;
  if (uVar11 != 0) {
    piVar12 = *(int **)(param_1 + 0x1a8);
    do {
      if ((*piVar12 == *(int *)(param_2 + 0xa8)) && (*(char *)((longlong)piVar12 + 0x6e) == '\0')) {
        bVar8 = true;
        goto code_r0x00014072a464;
      }
      lVar16 = lVar16 + 1;
      piVar12 = piVar12 + 0x1c;
    } while (lVar16 < (int)uVar11);
  }
  bVar8 = false;
code_r0x00014072a464:
  if (((*(byte *)(param_2 + 0xd1) & 0x20) == 0) && ((*(byte *)(param_2 + 0xd1) & 8) == 0)) {
    bVar7 = false;
  }
  else {
    bVar7 = true;
  }
  bVar1 = *(byte *)(param_2 + 0x368);
  cVar9 = FUN_1405bfbb0(param_2);
  fVar20 = 0.0;
  if ((*(int *)(param_1 + 0x29c) < 0) ||
     (*(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +
                (longlong)*(int *)(param_1 + 0x29c) * 0x28) == 0.0)) {
    bVar6 = false;
  }
  else {
    bVar6 = true;
  }
  bVar2 = *(byte *)(param_2 + 0x368);
  if (((-1 < *(int *)(param_1 + 0x2a4)) &&
      (0.0 < *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +
                       (longlong)*(int *)(param_1 + 0x2a4) * 0x28))) ||
     (cVar10 = FUN_1406dd490(), cVar10 == '\0')) {
    bVar5 = true;
  }
  else {
    bVar5 = false;
  }
  if ((param_3 == '\0') ||
     (lVar16 = *(longlong *)(param_1 + 0x10),
     fVar19 = *(float *)(param_2 + 0x300) - *(float *)(lVar16 + 0x300),
     fVar17 = *(float *)(param_2 + 0x304) - *(float *)(lVar16 + 0x304),
     fVar18 = *(float *)(param_2 + 0x308) - *(float *)(lVar16 + 0x308),
     SQRT(fVar19 * fVar19 + fVar17 * fVar17 + fVar18 * fVar18) <
     *(float *)(*(longlong *)(param_1 + 8) + 0x78))) {
    bVar4 = true;
  }
  else {
    bVar4 = false;
  }
  if (-1 < *(int *)(param_1 + 0x2a0)) {
    fVar20 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x1d0) + 0x48) + 0x24 +
                       (longlong)*(int *)(param_1 + 0x2a0) * 0x28);
  }
  iVar15 = 0;
  uVar11 = *(uint *)(param_1 + 0x1b0) & 0xffffff;
  if (uVar11 != 0) {
    lVar16 = (longlong)(int)uVar11;
    pcVar13 = (char *)(*(longlong *)(param_1 + 0x1a8) + 0x6d);
    do {
      if (*pcVar13 != '\0') {
        iVar15 = iVar15 + 1;
      }
      pcVar13 = pcVar13 + 0x70;
      lVar16 = lVar16 + -1;
    } while (lVar16 != 0);
  }
  uVar11 = *(uint *)(param_1 + 0x1a0) & 0xffffff;
  if (uVar11 != 0) {
    lVar16 = (longlong)(int)uVar11;
    pcVar13 = (char *)(*(longlong *)(param_1 + 0x198) + 0x6d);
    do {
      if (*pcVar13 != '\0') {
        iVar15 = iVar15 + 1;
      }
      pcVar13 = pcVar13 + 0x70;
      lVar16 = lVar16 + -1;
    } while (lVar16 != 0);
  }
  if ((param_3 == '\0') || (iVar15 < (int)fVar20)) {
    bVar3 = true;
  }
  else {
    bVar3 = false;
  }
  lVar16 = FUN_140877720(param_2,&DAT_1439ae4f0,2);
  if ((((((!bVar3) || (!bVar4)) || (!bVar5)) ||
       (((!bVar6 && (cVar9 != '\0')) || ((bVar8 || ((bVar7 || ((bVar1 >> 1 & 1) != 0)))))))) ||
      ((bVar2 >> 6 & 1) == 0)) ||
     ((((lVar16 == 0 || (lVar16 = FUN_140877720(param_2,&DAT_1439cae10,2), lVar16 == 0)) ||
       (lVar16 = FUN_140877720(param_2,&DAT_1439ba280,2), lVar16 == 0)) ||
      (iVar15 = FUN_1407659c0(lVar16,0,3), 0xc < iVar15)))) {
    uVar14 = 0;
  }
  else {
    uVar14 = 1;
  }
  return uVar14;
}


````

### text

````text

undefined8 FUN_14072d600(longlong param_1,int param_2)

{
  longlong lVar1;
  int *piVar2;
  float *pfVar3;
  longlong lVar4;
  uint uVar5;
  int *piVar6;
  undefined4 auStackX_10 [2];
  
  piVar6 = (int *)((longlong)param_2 * 0x70 + *(longlong *)(param_1 + 0x1a8));
  if (*(char *)((longlong)piVar6 + 0x6d) != '\0') {
    lVar1 = *(longlong *)(param_1 + 8);
    lVar4 = 0;
    uVar5 = *(uint *)(param_1 + 0x210) & 0xffffff;
    if (uVar5 != 0) {
      piVar2 = *(int **)(param_1 + 0x208);
      do {
        if (*piVar2 == *piVar6) {
          auStackX_10[0] = 0;
          piVar6[0x17] = 0;
          goto code_r0x00014072d67d;
        }
        lVar4 = lVar4 + 1;
        piVar2 = piVar2 + 6;
      } while (lVar4 < (longlong)(ulonglong)uVar5);
    }
    pfVar3 = (float *)FUN_140877eb0(*(undefined8 *)(param_1 + 0x10),auStackX_10);
    piVar6[0x17] = (int)(*pfVar3 + (float)piVar6[0x17]);
code_r0x00014072d67d:
    if (*(float *)(lVar1 + 0x1b8) <= (float)piVar6[0x17]) {
      FUN_14072b0e0(param_1,param_2,0);
      return 1;
    }
  }
  return 0;
}


````
