# md6-parser-order.json

Original: [outputs/d2-coop-lab/analysis/ghidra/md6-parser-order.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/md6-parser-order.json>)

SHA-256: `EB458C2E030B92623F42DF204F3E4C650D29F41D64008830C998B0AC16152035`; 33976 bytes.

## Original saved record

````json
{
  "ExactExecutable": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "Address": "1401c9630",
  "Decompilation": {
    "result": "\r\nvoid FUN_1401c9630(longlong *param_1,undefined8 param_2)\r\n\r\n{\r\n  uint *puVar1;\r\n  byte bVar2;\r\n  char cVar3;\r\n  undefined1 uVar4;\r\n  int iVar5;\r\n  int iVar6;\r\n  uint uVar7;\r\n  uint uVar8;\r\n  longlong lVar9;\r\n  longlong lVar10;\r\n  undefined8 uVar11;\r\n  undefined8 uVar12;\r\n  undefined4 *puVar13;\r\n  longlong *plVar14;\r\n  short *psVar15;\r\n  longlong *plVar16;\r\n  ulonglong uVar17;\r\n  undefined8 *puVar18;\r\n  longlong *plVar19;\r\n  longlong lVar20;\r\n  ulonglong uVar21;\r\n  char cVar22;\r\n  undefined4 uVar23;\r\n  float fVar24;\r\n  longlong lStackX_8;\r\n  longlong *plStackX_18;\r\n  longlong *plStackX_20;\r\n  longlong *plStack_f98;\r\n  undefined8 uStack_f90;\r\n  undefined1 uStack_f88;\r\n  undefined1 *puStack_f78;\r\n  uint uStack_f70;\r\n  uint uStack_f6c;\r\n  undefined1 auStack_f68 [24];\r\n  undefined1 auStack_f50 [232];\r\n  int iStack_e68;\r\n  uint uStack_e64;\r\n  undefined8 uStack_e50;\r\n  undefined4 uStack_e48;\r\n  undefined8 uStack_e40;\r\n  undefined8 uStack_e38;\r\n  undefined8 uStack_e30;\r\n  undefined8 uStack_e28;\r\n  undefined1 auStack_e20 [40];\r\n  char cStack_df8;\r\n  longlong *plStack_df0;\r\n  undefined8 uStack_de8;\r\n  undefined8 uStack_de0;\r\n  undefined4 uStack_dd8;\r\n  undefined8 uStack_dd0;\r\n  undefined8 uStack_dc8;\r\n  undefined4 uStack_dc0;\r\n  longlong *plStack_db8;\r\n  undefined8 uStack_db0;\r\n  undefined1 auStack_da8 [40];\r\n  undefined1 auStack_d80 [40];\r\n  undefined1 auStack_d58 [280];\r\n  undefined1 auStack_c40 [3080];\r\n  \r\n  uStack_db0 = 0xfffffffffffffffe;\r\n  (**(code **)(*param_1 + 0xf8))();\r\n  FUN_14013fb30(&puStack_f78);\r\n  FUN_1411ad450(auStack_f50,0,0xe8);\r\n  auStack_f68[0] = 0;\r\n  puStack_f78 = auStack_f68;\r\n  uStack_f70 = uStack_f70 & 0xe0000000;\r\n  uStack_f6c = uStack_f6c & 0x40000000 | 0x100;\r\n  uStack_e50 = 0;\r\n  uStack_e48 = 0xff7fffff;\r\n  uStack_e40 = 0;\r\n  uStack_e38 = 0;\r\n  uStack_e30 = 0;\r\n  FUN_1401c5d60(&plStack_df0);\r\n  plStack_df0 = param_1;\r\n  FUN_14013b980(param_2,&DAT_141acb8b4);\r\n  FUN_14013ba90(param_2);\r\n  cVar22 = '\\0';\r\ncode_r0x0001401c971b:\r\n  lStackX_8 = CONCAT71(lStackX_8._1_7_,cVar22);\r\ncode_r0x0001401c9722:\r\n  cVar3 = FUN_14013e730(param_2);\r\n  if ((cVar3 != '\\0') && ((iStack_e68 != 5 || (uStack_e64 != 0x2f)))) {\r\n    iVar5 = FUN_1401435f0(&puStack_f78,\"inherit\");\r\n    if (iVar5 == 0) {\r\n      FUN_14013ba90(param_2,1);\r\n      FUN_14013fd70(param_1 + 0x16,&puStack_f78);\r\n      cVar3 = FUN_1401401e0(&puStack_f78,&DAT_141ab2848);\r\n      if (cVar3 != '\\0') {\r\n        if ((*(byte *)(param_1 + 0x31) & 2) == 0) {\r\n          plStackX_18 = (longlong *)FUN_1400f13f0(400);\r\n          if (plStackX_18 == (longlong *)0x0) {\r\n            lVar10 = 0;\r\n          }\r\n          else {\r\n            lVar10 = FUN_1401c5e90(plStackX_18);\r\n          }\r\n          uVar11 = D2_InternedName_Resolve(param_1 + 0x16);\r\n          FUN_14005f6a0(lVar10,uVar11);\r\n          *(undefined4 *)(lVar10 + 0x188) = 1;\r\n          thunk_FUN_14005ede0(lVar10,0);\r\n          param_1[0x1b] = lVar10;\r\n        }\r\n        else {\r\n          lVar9 = D2_InternedName_Resolve(param_1 + 0x16);\r\n          lVar10 = 0;\r\n          if (lVar9 != 0) {\r\n            lVar10 = FUN_14001e030(&DAT_142434620,lVar9,0);\r\n          }\r\n          param_1[0x1b] = lVar10;\r\n        }\r\n        if (param_1[0x1b] == 0) {\r\n          uVar11 = D2_InternedName_Resolve(param_1 + 0x16);\r\n          uVar11 = FUN_140004b80(auStack_c40,&UNK_141b05558,uVar11);\r\n          FUN_140139910(param_2,uVar11);\r\n        }\r\n        (**(code **)(*(longlong *)PTR_DAT_1422738a8 + 0x80))(PTR_DAT_1422738a8,param_1);\r\n        lVar10 = param_1[0x1b];\r\n        param_1[10] = *(longlong *)(lVar10 + 0x50);\r\n        param_1[0xb] = *(longlong *)(lVar10 + 0x58);\r\n        param_1[0xc] = *(longlong *)(lVar10 + 0x60);\r\n        puVar18 = (undefined8 *)param_1[9];\r\n        uVar11 = (*(undefined8 **)(lVar10 + 0x48))[1];\r\n        *puVar18 = **(undefined8 **)(lVar10 + 0x48);\r\n        puVar18[1] = uVar11;\r\n        lVar10 = param_1[0x1b];\r\n        *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(lVar10 + 0xe0);\r\n        *(undefined4 *)((longlong)param_1 + 0xe4) = *(undefined4 *)(lVar10 + 0xe4);\r\n        *(undefined4 *)(param_1 + 0x1d) = *(undefined4 *)(lVar10 + 0xe8);\r\n        *(undefined4 *)((longlong)param_1 + 0xec) = *(undefined4 *)(lVar10 + 0xec);\r\n        *(undefined4 *)(param_1 + 0x1e) = *(undefined4 *)(lVar10 + 0xf0);\r\n        *(undefined4 *)((longlong)param_1 + 0xf4) = *(undefined4 *)(lVar10 + 0xf4);\r\n        lVar10 = param_1[0x1b];\r\n        *(undefined1 *)(param_1 + 0x1f) = *(undefined1 *)(lVar10 + 0xf8);\r\n        *(undefined4 *)((longlong)param_1 + 0xfc) = *(undefined4 *)(lVar10 + 0xfc);\r\n        FUN_1401c68e0(param_1 + 0xd,lVar10 + 0x68);\r\n        FUN_1409c4d50(param_1 + 0x12,param_1[0x1b] + 0x90);\r\n      }\r\n      goto code_r0x0001401c9722;\r\n    }\r\n    iVar5 = FUN_1401435f0(&puStack_f78,\"skeleton\");\r\n    if (iVar5 == 0) {\r\n      FUN_14013ba90(param_2,1);\r\n      cVar3 = FUN_140143bb0(&puStack_f78);\r\n      if (cVar3 == '\\0') {\r\n        if ((param_1[0x1b] == 0) || (iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b05588), iVar5 != 0)\r\n           ) {\r\n          uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n          lVar10 = FUN_1400607f0(0x1432d0520,uVar11,0);\r\n          param_1[10] = lVar10;\r\n        }\r\n        plVar19 = (longlong *)param_1[10];\r\n        if (plVar19 == (longlong *)0x0) {\r\n          lVar10 = param_1[1];\r\n          uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n                    /* WARNING: Subroutine does not return */\r\n          FUN_140166830(&UNK_141b055b0,uVar11,lVar10);\r\n        }\r\n        uVar11 = (**(code **)(*plVar19 + 0x38))(plVar19,&uStack_e28,1);\r\n        lVar10 = *(longlong *)PTR_DAT_1422738a8;\r\n        D2_InternedName_Resolve(uVar11);\r\n        (**(code **)(lVar10 + 0x78))(PTR_DAT_1422738a8,param_1);\r\n        FUN_14013fd10(&uStack_e28);\r\n      }\r\n      goto code_r0x0001401c9722;\r\n    }\r\n    iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141ad5998);\r\n    if (iVar5 == 0) {\r\n      FUN_14013ba90(param_2,1);\r\n      cVar3 = FUN_140143bb0(&puStack_f78);\r\n      if (cVar3 == '\\0') {\r\n        uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n        lVar10 = FUN_1400607f0(0x142bf86f0,uVar11);\r\n        param_1[0xb] = lVar10;\r\n      }\r\n      goto code_r0x0001401c9722;\r\n    }\r\n    iVar5 = FUN_1401435f0(&puStack_f78,\"offset\");\r\n    if (iVar5 != 0) {\r\n      iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b055e0);\r\n      if (iVar5 == 0) {\r\n        FUN_14013c410(param_2,3,param_1 + 0x1c);\r\n        FUN_14013c410(param_2,3);\r\n      }\r\n      else {\r\n        iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b055e8);\r\n        if (iVar5 == 0) {\r\n          uVar4 = FUN_14013c4a0(param_2);\r\n          *(undefined1 *)(param_1 + 0x1f) = uVar4;\r\n        }\r\n        else {\r\n          iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b05600);\r\n          if (iVar5 == 0) {\r\n            uVar23 = FUN_14013c930(param_2,0);\r\n            *(undefined4 *)((longlong)param_1 + 0xfc) = uVar23;\r\n          }\r\n          else {\r\n            iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b05610);\r\n            if (iVar5 == 0) {\r\n              FUN_14013ba90(param_2,1);\r\n              uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n              lVar10 = FUN_1400607f0(&DAT_1424369e0,uVar11);\r\n              param_1[0x11] = lVar10;\r\n              if (lVar10 == 0) {\r\n                uVar11 = func_0x000140023d70(0);\r\n                uVar12 = D2_InternedName_Resolve(&puStack_f78);\r\n                    /* WARNING: Subroutine does not return */\r\n                FUN_140166830(0x3e,uVar12,uVar11);\r\n              }\r\n            }\r\n            else {\r\n              iVar5 = FUN_1401435f0(&puStack_f78,&DAT_141ab1e58);\r\n              if (iVar5 == 0) {\r\n                FUN_14013ba90(param_2,3,1,&puStack_f78);\r\n                if (iStack_e68 == 3) {\r\n                  if ((uStack_e64 & 0x20000) == 0) {\r\n                    FUN_1401461c0(&puStack_f78);\r\n                  }\r\n                  iVar5 = (int)uStack_e50;\r\n                }\r\n                else {\r\n                  iVar5 = 0;\r\n                }\r\n                FUN_14013ba90(param_2);\r\n                lVar10 = (longlong)iVar5;\r\n                cVar3 = FUN_14013e730(param_2,&puStack_f78);\r\n                while ((cVar3 != '\\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {\r\n                  if ((int)(*(uint *)(param_1 + 0xe) & 0xffffff) <= iVar5) {\r\n                    plStack_f98 = (longlong *)0x0;\r\n                    FUN_1401548e0();\r\n                    plStack_f98 = (longlong *)0x0;\r\n                    uStack_f90 = (longlong *)0x3f24000000;\r\n                    uStack_f88 = 0;\r\n                    FUN_1401cb340(param_1 + 0xd,iVar5 + 1);\r\n                  }\r\n                  iVar6 = FUN_1401435f0(&puStack_f78,\"clear\");\r\n                  if (iVar6 == 0) {\r\n                    lVar9 = param_1[0xd];\r\n                    uVar8 = *(uint *)(lVar9 + 0xc + lVar10 * 0x18);\r\n                    if ((uVar8 & 0x40) == 0) {\r\n                      lVar20 = *(longlong *)(lVar9 + lVar10 * 0x18);\r\n                      if (lVar20 != 0) {\r\n                        FUN_14005b650(lVar20,uVar8 >> 8);\r\n                      }\r\n                      *(undefined8 *)(lVar9 + lVar10 * 0x18) = 0;\r\n                      puVar1 = (uint *)(lVar9 + 0xc + lVar10 * 0x18);\r\n                      *puVar1 = *puVar1 & 0xff;\r\n                    }\r\n                    puVar1 = (uint *)(lVar9 + 8 + lVar10 * 0x18);\r\n                    *puVar1 = *puVar1 & 0xff000000;\r\n                  }\r\n                  else {\r\n                    iVar6 = FUN_1401435f0(&puStack_f78,&UNK_141b05618);\r\n                    if (iVar6 == 0) {\r\n                      *(undefined1 *)(param_1[0xd] + 0x10 + lVar10 * 0x18) = 1;\r\n                    }\r\n                    else {\r\n                      iVar6 = FUN_1401435f0(&puStack_f78,&UNK_141b0562c);\r\n                      if (iVar6 == 0) {\r\n                        FUN_14013ba90(param_2,1);\r\n                        uStack_e28 = 0;\r\n                        FUN_14013fb30(auStack_e20);\r\n                        cStack_df8 = 2;\r\n                        bVar2 = *(byte *)(param_1 + 0x31);\r\n                        FUN_14013fd70(auStack_e20,&puStack_f78);\r\n                        cStack_df8 = ((bVar2 & 2) != 0) + '\\x01';\r\n                        FUN_1401c7280(param_1[0xd] + lVar10 * 0x18,&uStack_e28);\r\n                        cVar3 = FUN_1401372e0(param_2,&DAT_141ab2424);\r\n                        if ((cVar3 != '\\0') &&\r\n                           (cVar3 = FUN_14013e730(param_2,&puStack_f78), cVar3 != '\\0')) {\r\n                          do {\r\n                            if ((iStack_e68 == 5) && (uStack_e64 == 0x2f)) break;\r\n                            iVar6 = FUN_1401435f0(&puStack_f78,&UNK_141ad2c24);\r\n                            if (iVar6 == 0) {\r\n                              FUN_14013fb30(auStack_da8);\r\n                              FUN_14013fb30(auStack_d80);\r\n                              FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n                              FUN_14013fd70(auStack_da8,&puStack_f78);\r\n                              FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n                              FUN_14013fd70(auStack_d80,&puStack_f78);\r\n                              lVar9 = *(longlong *)PTR_DAT_1422738a8;\r\n                              uVar11 = D2_InternedName_Resolve(auStack_da8);\r\n                              (**(code **)(lVar9 + 0x78))(PTR_DAT_1422738a8,param_1,uVar11);\r\n                              FUN_1411ad700(auStack_d58,0x38,5,FUN_140087d50,&LAB_140062340);\r\n                              lVar9 = FUN_1401c89c0(&uStack_de8,auStack_da8,auStack_d58);\r\n                              FUN_1411ad55c(auStack_d58,0x38,5,&LAB_140062340);\r\n                              lVar20 = lVar10 * 0x38;\r\n                              *(undefined8 *)(lVar20 + lVar9) = uStack_e28;\r\n                              FUN_14013fd70(lVar9 + 8 + lVar20,auStack_e20);\r\n                              *(char *)(lVar20 + 0x30 + lVar9) = cStack_df8;\r\n                              FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n                              FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n                              FUN_14013ba90(param_2,1);\r\n                              FUN_14013fd10(auStack_d80);\r\n                              FUN_14013fd10(auStack_da8);\r\n                            }\r\n                            else {\r\n                              D2_InternedName_Resolve(&puStack_f78);\r\n                              uVar11 = FUN_140004b80(auStack_c40,&UNK_141b05638);\r\n                              FUN_140139910(param_2,uVar11);\r\n                            }\r\n                            cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n                          } while (cVar22 != '\\0');\r\n                          cVar22 = (char)lStackX_8;\r\n                        }\r\n                        FUN_14013fd10(auStack_e20);\r\n                      }\r\n                    }\r\n                  }\r\n                  cVar3 = FUN_14013e730(param_2,&puStack_f78);\r\n                }\r\n              }\r\n              else {\r\n                D2_InternedName_Resolve(&puStack_f78);\r\n                uVar11 = FUN_140004b80(auStack_c40,&UNK_141b05670);\r\n                FUN_140139910(param_2,uVar11);\r\n              }\r\n            }\r\n          }\r\n        }\r\n      }\r\n      goto code_r0x0001401c9722;\r\n    }\r\n    FUN_14013c410(param_2,3);\r\n    cVar22 = '\\x01';\r\n    goto code_r0x0001401c971b;\r\n  }\r\n  FUN_140088c10(&plStack_df0);\r\n  iVar5 = 0;\r\n  if ((*(uint *)(param_1 + 0xe) & 0xffffff) != 0) {\r\n    lVar10 = 0;\r\n    do {\r\n      iVar6 = 0;\r\n      if ((*(uint *)(param_1[0xd] + 8 + lVar10) & 0xffffff) != 0) {\r\n        do {\r\n          lVar9 = FUN_1401c8cf0((longlong)iVar6 * 0x38 + *(longlong *)(lVar10 + param_1[0xd]));\r\n          if (lVar9 == 0) {\r\n            lVar10 = param_1[1];\r\n            plVar19 = (longlong *)\r\n                      ((longlong)iVar6 * 0x38 + *(longlong *)(param_1[0xd] + (longlong)iVar5 * 0x18)\r\n                      );\r\n            lVar9 = *plVar19;\r\n            if (lVar9 == 0) {\r\n              uVar11 = D2_InternedName_Resolve(plVar19 + 1);\r\n            }\r\n            else {\r\n              uVar11 = *(undefined8 *)(lVar9 + 8);\r\n            }\r\n                    /* WARNING: Subroutine does not return */\r\n            FUN_140166830(&UNK_141b056a8,uVar11,lVar10);\r\n          }\r\n          iVar6 = iVar6 + 1;\r\n        } while (iVar6 < (int)(*(uint *)(param_1[0xd] + 8 + lVar10) & 0xffffff));\r\n      }\r\n      iVar5 = iVar5 + 1;\r\n      lVar10 = lVar10 + 0x18;\r\n    } while (iVar5 < (int)(*(uint *)(param_1 + 0xe) & 0xffffff));\r\n  }\r\n  lVar10 = param_1[0x1b];\r\n  if (lVar10 != 0) {\r\n    lVar9 = param_1[0x11];\r\n    if (lVar9 == 0) {\r\n      lVar9 = *(longlong *)(lVar10 + 0x88);\r\n    }\r\n    param_1[0x11] = lVar9;\r\n    if (cVar22 == '\\0') {\r\n      puVar13 = *(undefined4 **)(lVar10 + 0x48);\r\n    }\r\n    else {\r\n      puVar13 = (undefined4 *)param_1[9];\r\n    }\r\n    *(undefined4 *)param_1[9] = *puVar13;\r\n    if (cVar22 == '\\0') {\r\n      uVar23 = *(undefined4 *)(*(longlong *)(param_1[0x1b] + 0x48) + 4);\r\n    }\r\n    else {\r\n      uVar23 = *(undefined4 *)(param_1[9] + 4);\r\n    }\r\n    *(undefined4 *)(param_1[9] + 4) = uVar23;\r\n    if (cVar22 == '\\0') {\r\n      uVar23 = *(undefined4 *)(*(longlong *)(param_1[0x1b] + 0x48) + 8);\r\n    }\r\n    else {\r\n      uVar23 = *(undefined4 *)(param_1[9] + 8);\r\n    }\r\n    *(undefined4 *)(param_1[9] + 8) = uVar23;\r\n    plStack_f98 = (longlong *)0x0;\r\n    FUN_1401548e0();\r\n    plVar19 = plStack_f98;\r\n    uStack_f90._0_4_ = 0x5000000;\r\n    uStack_f90 = (longlong *)0x3f05000000;\r\n    if (plStack_f98 != (longlong *)0x0) {\r\n      plVar14 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar14 + 0x10))\r\n                (plVar14,\r\n                 \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                 ,0x1ec,plVar19,0x10);\r\n    }\r\n    plStack_f98 = (longlong *)0x0;\r\n    uVar17 = (ulonglong)uStack_f90._4_1_;\r\n    plVar19 = (longlong *)0x0;\r\n    uVar21 = 0;\r\n    uStack_f90 = (longlong *)\r\n                 ((ulonglong)CONCAT14(uStack_f90._4_1_,(uint)uStack_f90) & 0xffffffffff000000);\r\n    lVar10 = param_1[0x1b];\r\n    iVar5 = (*(uint *)(lVar10 + 0x130) & 0xffffff) - 1;\r\n    lVar9 = (longlong)iVar5;\r\n    if (-1 < iVar5) {\r\n      do {\r\n        iVar6 = 0;\r\n        lVar20 = 0;\r\n        iVar5 = ((uint)uVar21 & 0xffffff) - 1;\r\n        if (iVar5 < 0) {\r\ncode_r0x0001401ca277:\r\n          FUN_1408918f0();\r\n          uVar21 = (ulonglong)uStack_f90 & 0xffffffff;\r\n          plVar19 = plStack_f98;\r\n        }\r\n        else {\r\n          while (plVar19[lVar20] != *(longlong *)(*(longlong *)(lVar10 + 0x128) + lVar9 * 8)) {\r\n            iVar6 = iVar6 + 1;\r\n            lVar20 = lVar20 + 1;\r\n            if (iVar5 < lVar20) goto code_r0x0001401ca277;\r\n          }\r\n          if (iVar6 < 0) goto code_r0x0001401ca277;\r\n        }\r\n        lVar9 = lVar9 + -1;\r\n      } while (-1 < lVar9);\r\n      uVar17 = (ulonglong)uStack_f90 >> 0x20;\r\n    }\r\n    if (*(longlong *)(lVar10 + 0xd8) != 0) {\r\n      FUN_1401c87f0(*(longlong *)(lVar10 + 0xd8));\r\n      uVar17 = (ulonglong)uStack_f90 >> 0x20;\r\n      uVar21 = (ulonglong)uStack_f90 & 0xffffffff;\r\n      plVar19 = plStack_f98;\r\n    }\r\n    iVar5 = 0;\r\n    if ((uVar21 & 0xffffff) != 0) {\r\n      lVar10 = 0;\r\n      do {\r\n        FUN_1400607f0(&DAT_142436aa0,*(undefined8 *)(*(longlong *)(lVar10 + (longlong)plVar19) + 8))\r\n        ;\r\n        FUN_140023f20(param_1[0x11]);\r\n        iVar5 = iVar5 + 1;\r\n        lVar10 = lVar10 + 8;\r\n        plVar19 = plStack_f98;\r\n      } while (iVar5 < (int)((uint)uStack_f90 & 0xffffff));\r\n      uVar17 = (ulonglong)uStack_f90 >> 0x20;\r\n    }\r\n    if (((uVar17 & 0x40) == 0) && (plVar19 != (longlong *)0x0)) {\r\n      plVar14 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar14 + 0x10))\r\n                (plVar14,\r\n                 \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                 ,0x1ec,plVar19,0x10);\r\n    }\r\n  }\r\n  if ((*(uint *)(param_1[0xd] + 8) & 0xffffff) == 0) {\r\n    FUN_140004b80(auStack_c40,&UNK_141b056d8);\r\n    FUN_140139910(param_2);\r\n  }\r\n  if (param_1[10] == 0) {\r\n    FUN_140004b80(auStack_c40,&UNK_141b05700);\r\n    FUN_140139910(param_2);\r\n  }\r\n  FUN_1401cb890(param_1);\r\n  if (*(float *)((longlong)param_1 + 0xec) <= *(float *)(param_1 + 0x1c) &&\r\n      *(float *)(param_1 + 0x1c) != *(float *)((longlong)param_1 + 0xec)) {\r\n    iVar5 = 0;\r\n    if ((*(uint *)(param_1 + 0xe) & 0xffffff) != 0) {\r\n      lVar10 = 0;\r\n      do {\r\n        iVar6 = 0;\r\n        if ((*(uint *)(lVar10 + 8 + param_1[0xd]) & 0xffffff) != 0) {\r\n          lVar9 = 0;\r\n          do {\r\n            lVar20 = *(longlong *)(lVar9 + *(longlong *)(lVar10 + param_1[0xd]));\r\n            fVar24 = *(float *)(param_1 + 0x1c);\r\n            if (*(float *)(lVar20 + 0x34) <= *(float *)(param_1 + 0x1c)) {\r\n              fVar24 = *(float *)(lVar20 + 0x34);\r\n            }\r\n            *(float *)(param_1 + 0x1c) = fVar24;\r\n            fVar24 = *(float *)((longlong)param_1 + 0xe4);\r\n            if (*(float *)(lVar20 + 0x38) <= *(float *)((longlong)param_1 + 0xe4)) {\r\n              fVar24 = *(float *)(lVar20 + 0x38);\r\n            }\r\n            *(float *)((longlong)param_1 + 0xe4) = fVar24;\r\n            fVar24 = *(float *)(param_1 + 0x1d);\r\n            if (*(float *)(lVar20 + 0x3c) <= *(float *)(param_1 + 0x1d)) {\r\n              fVar24 = *(float *)(lVar20 + 0x3c);\r\n            }\r\n            *(float *)(param_1 + 0x1d) = fVar24;\r\n            fVar24 = *(float *)((longlong)param_1 + 0xec);\r\n            if (*(float *)((longlong)param_1 + 0xec) <= *(float *)(lVar20 + 0x40)) {\r\n              fVar24 = *(float *)(lVar20 + 0x40);\r\n            }\r\n            *(float *)((longlong)param_1 + 0xec) = fVar24;\r\n            fVar24 = *(float *)(param_1 + 0x1e);\r\n            if (*(float *)(param_1 + 0x1e) <= *(float *)(lVar20 + 0x44)) {\r\n              fVar24 = *(float *)(lVar20 + 0x44);\r\n            }\r\n            *(float *)(param_1 + 0x1e) = fVar24;\r\n            fVar24 = *(float *)((longlong)param_1 + 0xf4);\r\n            if (*(float *)((longlong)param_1 + 0xf4) <= *(float *)(lVar20 + 0x48)) {\r\n              fVar24 = *(float *)(lVar20 + 0x48);\r\n            }\r\n            *(float *)((longlong)param_1 + 0xf4) = fVar24;\r\n            iVar6 = iVar6 + 1;\r\n            lVar9 = lVar9 + 0x38;\r\n          } while (iVar6 < (int)(*(uint *)(lVar10 + 8 + param_1[0xd]) & 0xffffff));\r\n        }\r\n        iVar5 = iVar5 + 1;\r\n        lVar10 = lVar10 + 0x18;\r\n      } while (iVar5 < (int)(*(uint *)(param_1 + 0xe) & 0xffffff));\r\n    }\r\n    if (((*(float *)(param_1 + 0x1c) == *(float *)((longlong)param_1 + 0xec)) &&\r\n        (*(float *)((longlong)param_1 + 0xe4) == *(float *)(param_1 + 0x1e))) &&\r\n       (*(float *)(param_1 + 0x1d) == *(float *)((longlong)param_1 + 0xf4))) {\r\n      fVar24 = *(float *)((longlong)param_1 + 0xfc);\r\n      *(float *)(param_1 + 0x1c) = *(float *)(param_1 + 0x1c) - fVar24;\r\n      *(float *)((longlong)param_1 + 0xe4) = *(float *)((longlong)param_1 + 0xe4) - fVar24;\r\n      *(float *)(param_1 + 0x1d) = *(float *)(param_1 + 0x1d) - fVar24;\r\n      *(float *)((longlong)param_1 + 0xec) = fVar24 + *(float *)((longlong)param_1 + 0xec);\r\n      *(float *)(param_1 + 0x1e) = fVar24 + *(float *)(param_1 + 0x1e);\r\n      *(float *)((longlong)param_1 + 0xf4) = fVar24 + *(float *)((longlong)param_1 + 0xf4);\r\n    }\r\n  }\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b05728);\r\n  if (cVar22 != '\\0') {\r\n    FUN_14013ed20(param_2,1);\r\n  }\r\n  FUN_14013b980(param_2,&UNK_141abb5f8);\r\n  FUN_14013ba90(param_2,5,0x2e,&puStack_f78);\r\n  cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n  while ((cVar22 != '\\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {\r\n    D2_InternedName_Resolve(&puStack_f78);\r\n    lStackX_8 = FUN_1400607f0(0x142435160);\r\n    iVar6 = 0;\r\n    lVar10 = 0;\r\n    iVar5 = (*(uint *)(param_1 + 0x24) & 0xffffff) - 1;\r\n    if (iVar5 < 0) {\r\ncode_r0x0001401ca63d:\r\n      FUN_1408918f0(param_1 + 0x23,&lStackX_8);\r\n    }\r\n    else {\r\n      plVar19 = (longlong *)param_1[0x23];\r\n      while (*plVar19 != lStackX_8) {\r\n        iVar6 = iVar6 + 1;\r\n        lVar10 = lVar10 + 1;\r\n        plVar19 = plVar19 + 1;\r\n        if (iVar5 < lVar10) goto code_r0x0001401ca63d;\r\n      }\r\n      if (iVar6 < 0) goto code_r0x0001401ca63d;\r\n    }\r\n    cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n  }\r\n  FUN_14013b980(param_2,&UNK_141b05738);\r\n  FUN_14013ba90(param_2,5,0x2e,&puStack_f78);\r\n  cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n  while ((cVar22 != '\\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {\r\n    D2_InternedName_Resolve(&puStack_f78);\r\n    lStackX_8 = FUN_1400607f0(&DAT_142436aa0);\r\n    if (lStackX_8 != 0) {\r\n      iVar6 = 0;\r\n      lVar10 = 0;\r\n      iVar5 = (*(uint *)(param_1 + 0x26) & 0xffffff) - 1;\r\n      if (-1 < iVar5) {\r\n        plVar19 = (longlong *)param_1[0x25];\r\n        do {\r\n          if (*plVar19 == lStackX_8) {\r\n            if (-1 < iVar6) goto code_r0x0001401ca73d;\r\n            break;\r\n          }\r\n          iVar6 = iVar6 + 1;\r\n          lVar10 = lVar10 + 1;\r\n          plVar19 = plVar19 + 1;\r\n        } while (lVar10 <= iVar5);\r\n      }\r\n      FUN_140023f20(param_1[0x11],lStackX_8);\r\n      FUN_1408918f0(param_1 + 0x25,&lStackX_8);\r\n    }\r\ncode_r0x0001401ca73d:\r\n    cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n  }\r\n  FUN_14013b980(param_2,&UNK_141b05748);\r\n  iVar5 = FUN_14025c6e0(param_1 + 0x21,param_2);\r\n  if (0 < iVar5) {\r\n    FUN_1401c6f80(param_1,&UNK_141b05750);\r\n    *(int *)(param_1 + 0x20) = (int)param_1[0x20] + iVar5;\r\n  }\r\n  if (param_1[0x1b] != 0) {\r\n    FUN_14025be70(param_1 + 0x21,param_1[0x1b] + 0x108);\r\n  }\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b05780);\r\n  if (cVar22 != '\\0') {\r\n    FUN_14013ba90(param_2,5,0x2e,&puStack_f78);\r\n    cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n    while ((cVar22 != '\\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {\r\n      uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n      FUN_14013f9d0(&uStack_e28,uVar11);\r\n      FUN_14013e730(param_2,&puStack_f78);\r\n      lVar10 = param_1[10];\r\n      D2_InternedName_Resolve(&uStack_e28);\r\n      psVar15 = (short *)FUN_140256080(lVar10,&lStackX_8);\r\n      if (-1 < *psVar15) {\r\n        cVar22 = FUN_140143bb0(&puStack_f78);\r\n        if (cVar22 == '\\0') {\r\n          uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n          plStackX_18 = (longlong *)FUN_1400607f0(&DAT_142434e90,uVar11);\r\n          if (plStackX_18 == (longlong *)0x0) goto code_r0x0001401caa3b;\r\n          plVar16 = (longlong *)D2_InternedName_Resolve(&uStack_e28);\r\n          plVar14 = param_1 + 0x2c;\r\n          uVar8 = *(uint *)(param_1 + 0x2e);\r\n          plStackX_20 = plVar16;\r\n          plStack_db8 = plVar14;\r\n          uVar7 = FUN_1401431a0(plVar16);\r\n          plVar19 = (longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);\r\n          lVar10 = *plVar19;\r\n          while (lVar10 != 0) {\r\n            uVar11 = D2_InternedName_Resolve(lVar10);\r\n            iVar5 = FUN_140143650(uVar11,plVar16);\r\n            if (iVar5 == 0) {\r\n              *(longlong **)(lVar10 + 0x28) = plStackX_18;\r\n              goto code_r0x0001401caa3b;\r\n            }\r\n            if (0 < iVar5) break;\r\n            plVar19 = (longlong *)(lVar10 + 0x30);\r\n            lVar10 = *plVar19;\r\n          }\r\n          *(int *)((longlong)param_1 + 0x16c) = *(int *)((longlong)param_1 + 0x16c) + 1;\r\n          plVar16 = (longlong *)FUN_1400f14b0(0x38,*(undefined1 *)((longlong)param_1 + 0x174));\r\n          plStack_f98 = plVar16;\r\n          if (plVar16 == (longlong *)0x0) {\r\n            plVar16 = (longlong *)0x0;\r\n          }\r\n          else {\r\n            lVar9 = *(longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);\r\n            FUN_14013f9d0(plVar16,plStackX_20);\r\n            plVar16[5] = (longlong)plStackX_18;\r\n            plVar16[6] = lVar9;\r\n          }\r\n        }\r\n        else {\r\n          plVar16 = (longlong *)D2_InternedName_Resolve(&uStack_e28);\r\n          plVar14 = param_1 + 0x2c;\r\n          uVar8 = *(uint *)(param_1 + 0x2e);\r\n          plStackX_18 = plVar16;\r\n          plStackX_20 = plVar14;\r\n          uVar7 = FUN_1401431a0(plVar16);\r\n          plVar19 = (longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);\r\n          lVar10 = *plVar19;\r\n          while (lVar10 != 0) {\r\n            uVar11 = D2_InternedName_Resolve(lVar10);\r\n            iVar5 = FUN_140143650(uVar11,plVar16);\r\n            if (iVar5 == 0) {\r\n              *(undefined8 *)(lVar10 + 0x28) = 0;\r\n              goto code_r0x0001401caa3b;\r\n            }\r\n            if (0 < iVar5) break;\r\n            plVar19 = (longlong *)(lVar10 + 0x30);\r\n            lVar10 = *plVar19;\r\n          }\r\n          *(int *)((longlong)param_1 + 0x16c) = *(int *)((longlong)param_1 + 0x16c) + 1;\r\n          plVar16 = (longlong *)FUN_1400f14b0(0x38,*(undefined1 *)((longlong)param_1 + 0x174));\r\n          plStack_db8 = plVar16;\r\n          if (plVar16 == (longlong *)0x0) {\r\n            plVar16 = (longlong *)0x0;\r\n          }\r\n          else {\r\n            lVar9 = *(longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);\r\n            FUN_14013f9d0(plVar16,plStackX_18);\r\n            plVar16[5] = 0;\r\n            plVar16[6] = lVar9;\r\n          }\r\n        }\r\n        *plVar19 = (longlong)plVar16;\r\n        plVar16[6] = lVar10;\r\n      }\r\ncode_r0x0001401caa3b:\r\n      FUN_14013fd10(&uStack_e28);\r\n      cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n    }\r\n  }\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b05790);\r\n  if (cVar22 != '\\0') {\r\n    FUN_14013ba90(param_2,5,0x2e,&puStack_f78);\r\n    FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n    FUN_14013ba90(param_2,5,0x2f,&puStack_f78);\r\n  }\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b057b0);\r\n  if (cVar22 == '\\0') {\r\n    lVar10 = 0;\r\n    if (param_1[0x1b] != 0) {\r\n      lVar10 = *(longlong *)(param_1[0x1b] + 0x138);\r\n    }\r\n  }\r\n  else {\r\n    FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n    uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n    lVar10 = FUN_14001e030(0x143990200,uVar11,0,0);\r\n  }\r\n  param_1[0x27] = lVar10;\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b057c0);\r\n  if (cVar22 == '\\0') {\r\n    lVar10 = 0;\r\n    if (param_1[0x1b] != 0) {\r\n      lVar10 = *(longlong *)(param_1[0x1b] + 0x140);\r\n    }\r\n  }\r\n  else {\r\n    FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n    uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n    lVar10 = FUN_14001e030(0x1439907a0,uVar11,0,0);\r\n  }\r\n  param_1[0x28] = lVar10;\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b057d8);\r\n  if (cVar22 == '\\0') {\r\n    lVar10 = 0;\r\n    if (param_1[0x1b] != 0) {\r\n      lVar10 = *(longlong *)(param_1[0x1b] + 0x148);\r\n    }\r\n  }\r\n  else {\r\n    FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n    uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n    lVar10 = FUN_1400607f0(0x143990890,uVar11);\r\n  }\r\n  param_1[0x29] = lVar10;\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b057e8);\r\n  if (cVar22 == '\\0') {\r\n    lVar10 = 0;\r\n    if (param_1[0x1b] != 0) {\r\n      lVar10 = *(longlong *)(param_1[0x1b] + 0x150);\r\n    }\r\n  }\r\n  else {\r\n    FUN_14013ba90(param_2,1,0,&puStack_f78);\r\n    uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n    lVar10 = FUN_1400607f0(0x143990d40,uVar11);\r\n  }\r\n  param_1[0x2a] = lVar10;\r\n  cVar22 = FUN_1401372e0(param_2,&UNK_141b05800);\r\n  if (cVar22 == '\\0') {\r\ncode_r0x0001401cae0d:\r\n    if ((*(uint *)((undefined8 *)param_1[0xd] + 1) & 0xffffff) == 0) {\r\n      lVar10 = 0;\r\n    }\r\n    else {\r\n      lVar10 = **(longlong **)param_1[0xd];\r\n    }\r\n    if ((param_1[10] != 0) && (lVar10 != 0)) {\r\n      FUN_1402548e0(param_1 + 0x14);\r\n    }\r\n    FUN_1401c77f0(&uStack_dd0);\r\n    FUN_1400f1710(uStack_dd0);\r\n    uStack_dd0 = 0;\r\n    uStack_dc8 = 0;\r\n    uStack_dc0 = 0;\r\n    FUN_1401c7720(&uStack_de8);\r\n    FUN_1400f1710(uStack_de8);\r\n    uStack_de8 = 0;\r\n    uStack_de0 = 0;\r\n    uStack_dd8 = 0;\r\n    FUN_14013fd10(&puStack_f78);\r\n    return;\r\n  }\r\n  FUN_14013ba90(param_2,5,0x2e,&puStack_f78);\r\n  cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n  do {\r\n    if ((cVar22 == '\\0') || ((iStack_e68 == 5 && (uStack_e64 == 0x2f)))) goto code_r0x0001401cae0d;\r\n    uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n    FUN_140131f90(&plStackX_18,uVar11,0);\r\n    FUN_14013e730(param_2,&puStack_f78);\r\n    uVar11 = D2_InternedName_Resolve(&puStack_f78);\r\n    FUN_140131f90(&plStackX_20,uVar11,0);\r\n    FUN_1402563e0(param_1[10],&lStackX_8);\r\n    if (-1 < (short)lStackX_8) {\r\n      iVar5 = 0;\r\n      lVar10 = 0;\r\n      if ((*(uint *)(param_1 + 0x13) & 0xffffff) != 0) {\r\n        puVar18 = (undefined8 *)param_1[0x12];\r\n        do {\r\n          if ((longlong *)*puVar18 == plStackX_18) {\r\n            puVar18[1] = plStackX_20;\r\n            cVar22 = FUN_1401c8b10(param_1,&plStackX_18);\r\n            if ((((cVar22 == '\\0') && ((char)*plStackX_20 == '\\0')) && (-1 < iVar5)) &&\r\n               ((uVar8 = *(uint *)(param_1 + 0x13), iVar5 < (int)(uVar8 & 0xffffff) &&\r\n                (uVar8 = (uVar8 - 1 ^ uVar8) & 0xffffff ^ uVar8, *(uint *)(param_1 + 0x13) = uVar8,\r\n                iVar5 < (int)(uVar8 & 0xffffff))))) {\r\n              lVar10 = lVar10 << 4;\r\n              do {\r\n                lVar9 = param_1[0x12];\r\n                *(undefined8 *)(lVar10 + lVar9) = *(undefined8 *)(lVar10 + 0x10 + lVar9);\r\n                *(undefined8 *)(lVar10 + 8 + lVar9) = *(undefined8 *)(lVar10 + 0x18 + lVar9);\r\n                iVar5 = iVar5 + 1;\r\n                lVar10 = lVar10 + 0x10;\r\n              } while (iVar5 < (int)(*(uint *)(param_1 + 0x13) & 0xffffff));\r\n            }\r\n            goto code_r0x0001401cadf8;\r\n          }\r\n          iVar5 = iVar5 + 1;\r\n          lVar10 = lVar10 + 1;\r\n          puVar18 = puVar18 + 2;\r\n        } while (iVar5 < (int)(*(uint *)(param_1 + 0x13) & 0xffffff));\r\n      }\r\n      if ((char)*plStackX_20 != '\\0') {\r\n        plStack_f98 = plStackX_18;\r\n        uStack_f90 = plStackX_20;\r\n        FUN_1401c7120(param_1 + 0x12,&plStack_f98);\r\n      }\r\n    }\r\ncode_r0x0001401cadf8:\r\n    cVar22 = FUN_14013e730(param_2,&puStack_f78);\r\n  } while( true );\r\n}\r\n\r\n"
  },
  "Conclusion": "Fixed parser order: optional locomotionDeclNew and animBasicDecl after props. Corrected declaration initializes both clone controllers; restored foot IK succeeds."
}
````

## Unescaped text fields for reading

### result

````text

void FUN_1401c9630(longlong *param_1,undefined8 param_2)

{
  uint *puVar1;
  byte bVar2;
  char cVar3;
  undefined1 uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  longlong lVar9;
  longlong lVar10;
  undefined8 uVar11;
  undefined8 uVar12;
  undefined4 *puVar13;
  longlong *plVar14;
  short *psVar15;
  longlong *plVar16;
  ulonglong uVar17;
  undefined8 *puVar18;
  longlong *plVar19;
  longlong lVar20;
  ulonglong uVar21;
  char cVar22;
  undefined4 uVar23;
  float fVar24;
  longlong lStackX_8;
  longlong *plStackX_18;
  longlong *plStackX_20;
  longlong *plStack_f98;
  undefined8 uStack_f90;
  undefined1 uStack_f88;
  undefined1 *puStack_f78;
  uint uStack_f70;
  uint uStack_f6c;
  undefined1 auStack_f68 [24];
  undefined1 auStack_f50 [232];
  int iStack_e68;
  uint uStack_e64;
  undefined8 uStack_e50;
  undefined4 uStack_e48;
  undefined8 uStack_e40;
  undefined8 uStack_e38;
  undefined8 uStack_e30;
  undefined8 uStack_e28;
  undefined1 auStack_e20 [40];
  char cStack_df8;
  longlong *plStack_df0;
  undefined8 uStack_de8;
  undefined8 uStack_de0;
  undefined4 uStack_dd8;
  undefined8 uStack_dd0;
  undefined8 uStack_dc8;
  undefined4 uStack_dc0;
  longlong *plStack_db8;
  undefined8 uStack_db0;
  undefined1 auStack_da8 [40];
  undefined1 auStack_d80 [40];
  undefined1 auStack_d58 [280];
  undefined1 auStack_c40 [3080];
  
  uStack_db0 = 0xfffffffffffffffe;
  (**(code **)(*param_1 + 0xf8))();
  FUN_14013fb30(&puStack_f78);
  FUN_1411ad450(auStack_f50,0,0xe8);
  auStack_f68[0] = 0;
  puStack_f78 = auStack_f68;
  uStack_f70 = uStack_f70 & 0xe0000000;
  uStack_f6c = uStack_f6c & 0x40000000 | 0x100;
  uStack_e50 = 0;
  uStack_e48 = 0xff7fffff;
  uStack_e40 = 0;
  uStack_e38 = 0;
  uStack_e30 = 0;
  FUN_1401c5d60(&plStack_df0);
  plStack_df0 = param_1;
  FUN_14013b980(param_2,&DAT_141acb8b4);
  FUN_14013ba90(param_2);
  cVar22 = '\0';
code_r0x0001401c971b:
  lStackX_8 = CONCAT71(lStackX_8._1_7_,cVar22);
code_r0x0001401c9722:
  cVar3 = FUN_14013e730(param_2);
  if ((cVar3 != '\0') && ((iStack_e68 != 5 || (uStack_e64 != 0x2f)))) {
    iVar5 = FUN_1401435f0(&puStack_f78,"inherit");
    if (iVar5 == 0) {
      FUN_14013ba90(param_2,1);
      FUN_14013fd70(param_1 + 0x16,&puStack_f78);
      cVar3 = FUN_1401401e0(&puStack_f78,&DAT_141ab2848);
      if (cVar3 != '\0') {
        if ((*(byte *)(param_1 + 0x31) & 2) == 0) {
          plStackX_18 = (longlong *)FUN_1400f13f0(400);
          if (plStackX_18 == (longlong *)0x0) {
            lVar10 = 0;
          }
          else {
            lVar10 = FUN_1401c5e90(plStackX_18);
          }
          uVar11 = D2_InternedName_Resolve(param_1 + 0x16);
          FUN_14005f6a0(lVar10,uVar11);
          *(undefined4 *)(lVar10 + 0x188) = 1;
          thunk_FUN_14005ede0(lVar10,0);
          param_1[0x1b] = lVar10;
        }
        else {
          lVar9 = D2_InternedName_Resolve(param_1 + 0x16);
          lVar10 = 0;
          if (lVar9 != 0) {
            lVar10 = FUN_14001e030(&DAT_142434620,lVar9,0);
          }
          param_1[0x1b] = lVar10;
        }
        if (param_1[0x1b] == 0) {
          uVar11 = D2_InternedName_Resolve(param_1 + 0x16);
          uVar11 = FUN_140004b80(auStack_c40,&UNK_141b05558,uVar11);
          FUN_140139910(param_2,uVar11);
        }
        (**(code **)(*(longlong *)PTR_DAT_1422738a8 + 0x80))(PTR_DAT_1422738a8,param_1);
        lVar10 = param_1[0x1b];
        param_1[10] = *(longlong *)(lVar10 + 0x50);
        param_1[0xb] = *(longlong *)(lVar10 + 0x58);
        param_1[0xc] = *(longlong *)(lVar10 + 0x60);
        puVar18 = (undefined8 *)param_1[9];
        uVar11 = (*(undefined8 **)(lVar10 + 0x48))[1];
        *puVar18 = **(undefined8 **)(lVar10 + 0x48);
        puVar18[1] = uVar11;
        lVar10 = param_1[0x1b];
        *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(lVar10 + 0xe0);
        *(undefined4 *)((longlong)param_1 + 0xe4) = *(undefined4 *)(lVar10 + 0xe4);
        *(undefined4 *)(param_1 + 0x1d) = *(undefined4 *)(lVar10 + 0xe8);
        *(undefined4 *)((longlong)param_1 + 0xec) = *(undefined4 *)(lVar10 + 0xec);
        *(undefined4 *)(param_1 + 0x1e) = *(undefined4 *)(lVar10 + 0xf0);
        *(undefined4 *)((longlong)param_1 + 0xf4) = *(undefined4 *)(lVar10 + 0xf4);
        lVar10 = param_1[0x1b];
        *(undefined1 *)(param_1 + 0x1f) = *(undefined1 *)(lVar10 + 0xf8);
        *(undefined4 *)((longlong)param_1 + 0xfc) = *(undefined4 *)(lVar10 + 0xfc);
        FUN_1401c68e0(param_1 + 0xd,lVar10 + 0x68);
        FUN_1409c4d50(param_1 + 0x12,param_1[0x1b] + 0x90);
      }
      goto code_r0x0001401c9722;
    }
    iVar5 = FUN_1401435f0(&puStack_f78,"skeleton");
    if (iVar5 == 0) {
      FUN_14013ba90(param_2,1);
      cVar3 = FUN_140143bb0(&puStack_f78);
      if (cVar3 == '\0') {
        if ((param_1[0x1b] == 0) || (iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b05588), iVar5 != 0)
           ) {
          uVar11 = D2_InternedName_Resolve(&puStack_f78);
          lVar10 = FUN_1400607f0(0x1432d0520,uVar11,0);
          param_1[10] = lVar10;
        }
        plVar19 = (longlong *)param_1[10];
        if (plVar19 == (longlong *)0x0) {
          lVar10 = param_1[1];
          uVar11 = D2_InternedName_Resolve(&puStack_f78);
                    /* WARNING: Subroutine does not return */
          FUN_140166830(&UNK_141b055b0,uVar11,lVar10);
        }
        uVar11 = (**(code **)(*plVar19 + 0x38))(plVar19,&uStack_e28,1);
        lVar10 = *(longlong *)PTR_DAT_1422738a8;
        D2_InternedName_Resolve(uVar11);
        (**(code **)(lVar10 + 0x78))(PTR_DAT_1422738a8,param_1);
        FUN_14013fd10(&uStack_e28);
      }
      goto code_r0x0001401c9722;
    }
    iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141ad5998);
    if (iVar5 == 0) {
      FUN_14013ba90(param_2,1);
      cVar3 = FUN_140143bb0(&puStack_f78);
      if (cVar3 == '\0') {
        uVar11 = D2_InternedName_Resolve(&puStack_f78);
        lVar10 = FUN_1400607f0(0x142bf86f0,uVar11);
        param_1[0xb] = lVar10;
      }
      goto code_r0x0001401c9722;
    }
    iVar5 = FUN_1401435f0(&puStack_f78,"offset");
    if (iVar5 != 0) {
      iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b055e0);
      if (iVar5 == 0) {
        FUN_14013c410(param_2,3,param_1 + 0x1c);
        FUN_14013c410(param_2,3);
      }
      else {
        iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b055e8);
        if (iVar5 == 0) {
          uVar4 = FUN_14013c4a0(param_2);
          *(undefined1 *)(param_1 + 0x1f) = uVar4;
        }
        else {
          iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b05600);
          if (iVar5 == 0) {
            uVar23 = FUN_14013c930(param_2,0);
            *(undefined4 *)((longlong)param_1 + 0xfc) = uVar23;
          }
          else {
            iVar5 = FUN_1401435f0(&puStack_f78,&UNK_141b05610);
            if (iVar5 == 0) {
              FUN_14013ba90(param_2,1);
              uVar11 = D2_InternedName_Resolve(&puStack_f78);
              lVar10 = FUN_1400607f0(&DAT_1424369e0,uVar11);
              param_1[0x11] = lVar10;
              if (lVar10 == 0) {
                uVar11 = func_0x000140023d70(0);
                uVar12 = D2_InternedName_Resolve(&puStack_f78);
                    /* WARNING: Subroutine does not return */
                FUN_140166830(0x3e,uVar12,uVar11);
              }
            }
            else {
              iVar5 = FUN_1401435f0(&puStack_f78,&DAT_141ab1e58);
              if (iVar5 == 0) {
                FUN_14013ba90(param_2,3,1,&puStack_f78);
                if (iStack_e68 == 3) {
                  if ((uStack_e64 & 0x20000) == 0) {
                    FUN_1401461c0(&puStack_f78);
                  }
                  iVar5 = (int)uStack_e50;
                }
                else {
                  iVar5 = 0;
                }
                FUN_14013ba90(param_2);
                lVar10 = (longlong)iVar5;
                cVar3 = FUN_14013e730(param_2,&puStack_f78);
                while ((cVar3 != '\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {
                  if ((int)(*(uint *)(param_1 + 0xe) & 0xffffff) <= iVar5) {
                    plStack_f98 = (longlong *)0x0;
                    FUN_1401548e0();
                    plStack_f98 = (longlong *)0x0;
                    uStack_f90 = (longlong *)0x3f24000000;
                    uStack_f88 = 0;
                    FUN_1401cb340(param_1 + 0xd,iVar5 + 1);
                  }
                  iVar6 = FUN_1401435f0(&puStack_f78,"clear");
                  if (iVar6 == 0) {
                    lVar9 = param_1[0xd];
                    uVar8 = *(uint *)(lVar9 + 0xc + lVar10 * 0x18);
                    if ((uVar8 & 0x40) == 0) {
                      lVar20 = *(longlong *)(lVar9 + lVar10 * 0x18);
                      if (lVar20 != 0) {
                        FUN_14005b650(lVar20,uVar8 >> 8);
                      }
                      *(undefined8 *)(lVar9 + lVar10 * 0x18) = 0;
                      puVar1 = (uint *)(lVar9 + 0xc + lVar10 * 0x18);
                      *puVar1 = *puVar1 & 0xff;
                    }
                    puVar1 = (uint *)(lVar9 + 8 + lVar10 * 0x18);
                    *puVar1 = *puVar1 & 0xff000000;
                  }
                  else {
                    iVar6 = FUN_1401435f0(&puStack_f78,&UNK_141b05618);
                    if (iVar6 == 0) {
                      *(undefined1 *)(param_1[0xd] + 0x10 + lVar10 * 0x18) = 1;
                    }
                    else {
                      iVar6 = FUN_1401435f0(&puStack_f78,&UNK_141b0562c);
                      if (iVar6 == 0) {
                        FUN_14013ba90(param_2,1);
                        uStack_e28 = 0;
                        FUN_14013fb30(auStack_e20);
                        cStack_df8 = 2;
                        bVar2 = *(byte *)(param_1 + 0x31);
                        FUN_14013fd70(auStack_e20,&puStack_f78);
                        cStack_df8 = ((bVar2 & 2) != 0) + '\x01';
                        FUN_1401c7280(param_1[0xd] + lVar10 * 0x18,&uStack_e28);
                        cVar3 = FUN_1401372e0(param_2,&DAT_141ab2424);
                        if ((cVar3 != '\0') &&
                           (cVar3 = FUN_14013e730(param_2,&puStack_f78), cVar3 != '\0')) {
                          do {
                            if ((iStack_e68 == 5) && (uStack_e64 == 0x2f)) break;
                            iVar6 = FUN_1401435f0(&puStack_f78,&UNK_141ad2c24);
                            if (iVar6 == 0) {
                              FUN_14013fb30(auStack_da8);
                              FUN_14013fb30(auStack_d80);
                              FUN_14013ba90(param_2,1,0,&puStack_f78);
                              FUN_14013fd70(auStack_da8,&puStack_f78);
                              FUN_14013ba90(param_2,1,0,&puStack_f78);
                              FUN_14013fd70(auStack_d80,&puStack_f78);
                              lVar9 = *(longlong *)PTR_DAT_1422738a8;
                              uVar11 = D2_InternedName_Resolve(auStack_da8);
                              (**(code **)(lVar9 + 0x78))(PTR_DAT_1422738a8,param_1,uVar11);
                              FUN_1411ad700(auStack_d58,0x38,5,FUN_140087d50,&LAB_140062340);
                              lVar9 = FUN_1401c89c0(&uStack_de8,auStack_da8,auStack_d58);
                              FUN_1411ad55c(auStack_d58,0x38,5,&LAB_140062340);
                              lVar20 = lVar10 * 0x38;
                              *(undefined8 *)(lVar20 + lVar9) = uStack_e28;
                              FUN_14013fd70(lVar9 + 8 + lVar20,auStack_e20);
                              *(char *)(lVar20 + 0x30 + lVar9) = cStack_df8;
                              FUN_14013ba90(param_2,1,0,&puStack_f78);
                              FUN_14013ba90(param_2,1,0,&puStack_f78);
                              FUN_14013ba90(param_2,1);
                              FUN_14013fd10(auStack_d80);
                              FUN_14013fd10(auStack_da8);
                            }
                            else {
                              D2_InternedName_Resolve(&puStack_f78);
                              uVar11 = FUN_140004b80(auStack_c40,&UNK_141b05638);
                              FUN_140139910(param_2,uVar11);
                            }
                            cVar22 = FUN_14013e730(param_2,&puStack_f78);
                          } while (cVar22 != '\0');
                          cVar22 = (char)lStackX_8;
                        }
                        FUN_14013fd10(auStack_e20);
                      }
                    }
                  }
                  cVar3 = FUN_14013e730(param_2,&puStack_f78);
                }
              }
              else {
                D2_InternedName_Resolve(&puStack_f78);
                uVar11 = FUN_140004b80(auStack_c40,&UNK_141b05670);
                FUN_140139910(param_2,uVar11);
              }
            }
          }
        }
      }
      goto code_r0x0001401c9722;
    }
    FUN_14013c410(param_2,3);
    cVar22 = '\x01';
    goto code_r0x0001401c971b;
  }
  FUN_140088c10(&plStack_df0);
  iVar5 = 0;
  if ((*(uint *)(param_1 + 0xe) & 0xffffff) != 0) {
    lVar10 = 0;
    do {
      iVar6 = 0;
      if ((*(uint *)(param_1[0xd] + 8 + lVar10) & 0xffffff) != 0) {
        do {
          lVar9 = FUN_1401c8cf0((longlong)iVar6 * 0x38 + *(longlong *)(lVar10 + param_1[0xd]));
          if (lVar9 == 0) {
            lVar10 = param_1[1];
            plVar19 = (longlong *)
                      ((longlong)iVar6 * 0x38 + *(longlong *)(param_1[0xd] + (longlong)iVar5 * 0x18)
                      );
            lVar9 = *plVar19;
            if (lVar9 == 0) {
              uVar11 = D2_InternedName_Resolve(plVar19 + 1);
            }
            else {
              uVar11 = *(undefined8 *)(lVar9 + 8);
            }
                    /* WARNING: Subroutine does not return */
            FUN_140166830(&UNK_141b056a8,uVar11,lVar10);
          }
          iVar6 = iVar6 + 1;
        } while (iVar6 < (int)(*(uint *)(param_1[0xd] + 8 + lVar10) & 0xffffff));
      }
      iVar5 = iVar5 + 1;
      lVar10 = lVar10 + 0x18;
    } while (iVar5 < (int)(*(uint *)(param_1 + 0xe) & 0xffffff));
  }
  lVar10 = param_1[0x1b];
  if (lVar10 != 0) {
    lVar9 = param_1[0x11];
    if (lVar9 == 0) {
      lVar9 = *(longlong *)(lVar10 + 0x88);
    }
    param_1[0x11] = lVar9;
    if (cVar22 == '\0') {
      puVar13 = *(undefined4 **)(lVar10 + 0x48);
    }
    else {
      puVar13 = (undefined4 *)param_1[9];
    }
    *(undefined4 *)param_1[9] = *puVar13;
    if (cVar22 == '\0') {
      uVar23 = *(undefined4 *)(*(longlong *)(param_1[0x1b] + 0x48) + 4);
    }
    else {
      uVar23 = *(undefined4 *)(param_1[9] + 4);
    }
    *(undefined4 *)(param_1[9] + 4) = uVar23;
    if (cVar22 == '\0') {
      uVar23 = *(undefined4 *)(*(longlong *)(param_1[0x1b] + 0x48) + 8);
    }
    else {
      uVar23 = *(undefined4 *)(param_1[9] + 8);
    }
    *(undefined4 *)(param_1[9] + 8) = uVar23;
    plStack_f98 = (longlong *)0x0;
    FUN_1401548e0();
    plVar19 = plStack_f98;
    uStack_f90._0_4_ = 0x5000000;
    uStack_f90 = (longlong *)0x3f05000000;
    if (plStack_f98 != (longlong *)0x0) {
      plVar14 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar14 + 0x10))
                (plVar14,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,plVar19,0x10);
    }
    plStack_f98 = (longlong *)0x0;
    uVar17 = (ulonglong)uStack_f90._4_1_;
    plVar19 = (longlong *)0x0;
    uVar21 = 0;
    uStack_f90 = (longlong *)
                 ((ulonglong)CONCAT14(uStack_f90._4_1_,(uint)uStack_f90) & 0xffffffffff000000);
    lVar10 = param_1[0x1b];
    iVar5 = (*(uint *)(lVar10 + 0x130) & 0xffffff) - 1;
    lVar9 = (longlong)iVar5;
    if (-1 < iVar5) {
      do {
        iVar6 = 0;
        lVar20 = 0;
        iVar5 = ((uint)uVar21 & 0xffffff) - 1;
        if (iVar5 < 0) {
code_r0x0001401ca277:
          FUN_1408918f0();
          uVar21 = (ulonglong)uStack_f90 & 0xffffffff;
          plVar19 = plStack_f98;
        }
        else {
          while (plVar19[lVar20] != *(longlong *)(*(longlong *)(lVar10 + 0x128) + lVar9 * 8)) {
            iVar6 = iVar6 + 1;
            lVar20 = lVar20 + 1;
            if (iVar5 < lVar20) goto code_r0x0001401ca277;
          }
          if (iVar6 < 0) goto code_r0x0001401ca277;
        }
        lVar9 = lVar9 + -1;
      } while (-1 < lVar9);
      uVar17 = (ulonglong)uStack_f90 >> 0x20;
    }
    if (*(longlong *)(lVar10 + 0xd8) != 0) {
      FUN_1401c87f0(*(longlong *)(lVar10 + 0xd8));
      uVar17 = (ulonglong)uStack_f90 >> 0x20;
      uVar21 = (ulonglong)uStack_f90 & 0xffffffff;
      plVar19 = plStack_f98;
    }
    iVar5 = 0;
    if ((uVar21 & 0xffffff) != 0) {
      lVar10 = 0;
      do {
        FUN_1400607f0(&DAT_142436aa0,*(undefined8 *)(*(longlong *)(lVar10 + (longlong)plVar19) + 8))
        ;
        FUN_140023f20(param_1[0x11]);
        iVar5 = iVar5 + 1;
        lVar10 = lVar10 + 8;
        plVar19 = plStack_f98;
      } while (iVar5 < (int)((uint)uStack_f90 & 0xffffff));
      uVar17 = (ulonglong)uStack_f90 >> 0x20;
    }
    if (((uVar17 & 0x40) == 0) && (plVar19 != (longlong *)0x0)) {
      plVar14 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar14 + 0x10))
                (plVar14,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,plVar19,0x10);
    }
  }
  if ((*(uint *)(param_1[0xd] + 8) & 0xffffff) == 0) {
    FUN_140004b80(auStack_c40,&UNK_141b056d8);
    FUN_140139910(param_2);
  }
  if (param_1[10] == 0) {
    FUN_140004b80(auStack_c40,&UNK_141b05700);
    FUN_140139910(param_2);
  }
  FUN_1401cb890(param_1);
  if (*(float *)((longlong)param_1 + 0xec) <= *(float *)(param_1 + 0x1c) &&
      *(float *)(param_1 + 0x1c) != *(float *)((longlong)param_1 + 0xec)) {
    iVar5 = 0;
    if ((*(uint *)(param_1 + 0xe) & 0xffffff) != 0) {
      lVar10 = 0;
      do {
        iVar6 = 0;
        if ((*(uint *)(lVar10 + 8 + param_1[0xd]) & 0xffffff) != 0) {
          lVar9 = 0;
          do {
            lVar20 = *(longlong *)(lVar9 + *(longlong *)(lVar10 + param_1[0xd]));
            fVar24 = *(float *)(param_1 + 0x1c);
            if (*(float *)(lVar20 + 0x34) <= *(float *)(param_1 + 0x1c)) {
              fVar24 = *(float *)(lVar20 + 0x34);
            }
            *(float *)(param_1 + 0x1c) = fVar24;
            fVar24 = *(float *)((longlong)param_1 + 0xe4);
            if (*(float *)(lVar20 + 0x38) <= *(float *)((longlong)param_1 + 0xe4)) {
              fVar24 = *(float *)(lVar20 + 0x38);
            }
            *(float *)((longlong)param_1 + 0xe4) = fVar24;
            fVar24 = *(float *)(param_1 + 0x1d);
            if (*(float *)(lVar20 + 0x3c) <= *(float *)(param_1 + 0x1d)) {
              fVar24 = *(float *)(lVar20 + 0x3c);
            }
            *(float *)(param_1 + 0x1d) = fVar24;
            fVar24 = *(float *)((longlong)param_1 + 0xec);
            if (*(float *)((longlong)param_1 + 0xec) <= *(float *)(lVar20 + 0x40)) {
              fVar24 = *(float *)(lVar20 + 0x40);
            }
            *(float *)((longlong)param_1 + 0xec) = fVar24;
            fVar24 = *(float *)(param_1 + 0x1e);
            if (*(float *)(param_1 + 0x1e) <= *(float *)(lVar20 + 0x44)) {
              fVar24 = *(float *)(lVar20 + 0x44);
            }
            *(float *)(param_1 + 0x1e) = fVar24;
            fVar24 = *(float *)((longlong)param_1 + 0xf4);
            if (*(float *)((longlong)param_1 + 0xf4) <= *(float *)(lVar20 + 0x48)) {
              fVar24 = *(float *)(lVar20 + 0x48);
            }
            *(float *)((longlong)param_1 + 0xf4) = fVar24;
            iVar6 = iVar6 + 1;
            lVar9 = lVar9 + 0x38;
          } while (iVar6 < (int)(*(uint *)(lVar10 + 8 + param_1[0xd]) & 0xffffff));
        }
        iVar5 = iVar5 + 1;
        lVar10 = lVar10 + 0x18;
      } while (iVar5 < (int)(*(uint *)(param_1 + 0xe) & 0xffffff));
    }
    if (((*(float *)(param_1 + 0x1c) == *(float *)((longlong)param_1 + 0xec)) &&
        (*(float *)((longlong)param_1 + 0xe4) == *(float *)(param_1 + 0x1e))) &&
       (*(float *)(param_1 + 0x1d) == *(float *)((longlong)param_1 + 0xf4))) {
      fVar24 = *(float *)((longlong)param_1 + 0xfc);
      *(float *)(param_1 + 0x1c) = *(float *)(param_1 + 0x1c) - fVar24;
      *(float *)((longlong)param_1 + 0xe4) = *(float *)((longlong)param_1 + 0xe4) - fVar24;
      *(float *)(param_1 + 0x1d) = *(float *)(param_1 + 0x1d) - fVar24;
      *(float *)((longlong)param_1 + 0xec) = fVar24 + *(float *)((longlong)param_1 + 0xec);
      *(float *)(param_1 + 0x1e) = fVar24 + *(float *)(param_1 + 0x1e);
      *(float *)((longlong)param_1 + 0xf4) = fVar24 + *(float *)((longlong)param_1 + 0xf4);
    }
  }
  cVar22 = FUN_1401372e0(param_2,&UNK_141b05728);
  if (cVar22 != '\0') {
    FUN_14013ed20(param_2,1);
  }
  FUN_14013b980(param_2,&UNK_141abb5f8);
  FUN_14013ba90(param_2,5,0x2e,&puStack_f78);
  cVar22 = FUN_14013e730(param_2,&puStack_f78);
  while ((cVar22 != '\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {
    D2_InternedName_Resolve(&puStack_f78);
    lStackX_8 = FUN_1400607f0(0x142435160);
    iVar6 = 0;
    lVar10 = 0;
    iVar5 = (*(uint *)(param_1 + 0x24) & 0xffffff) - 1;
    if (iVar5 < 0) {
code_r0x0001401ca63d:
      FUN_1408918f0(param_1 + 0x23,&lStackX_8);
    }
    else {
      plVar19 = (longlong *)param_1[0x23];
      while (*plVar19 != lStackX_8) {
        iVar6 = iVar6 + 1;
        lVar10 = lVar10 + 1;
        plVar19 = plVar19 + 1;
        if (iVar5 < lVar10) goto code_r0x0001401ca63d;
      }
      if (iVar6 < 0) goto code_r0x0001401ca63d;
    }
    cVar22 = FUN_14013e730(param_2,&puStack_f78);
  }
  FUN_14013b980(param_2,&UNK_141b05738);
  FUN_14013ba90(param_2,5,0x2e,&puStack_f78);
  cVar22 = FUN_14013e730(param_2,&puStack_f78);
  while ((cVar22 != '\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {
    D2_InternedName_Resolve(&puStack_f78);
    lStackX_8 = FUN_1400607f0(&DAT_142436aa0);
    if (lStackX_8 != 0) {
      iVar6 = 0;
      lVar10 = 0;
      iVar5 = (*(uint *)(param_1 + 0x26) & 0xffffff) - 1;
      if (-1 < iVar5) {
        plVar19 = (longlong *)param_1[0x25];
        do {
          if (*plVar19 == lStackX_8) {
            if (-1 < iVar6) goto code_r0x0001401ca73d;
            break;
          }
          iVar6 = iVar6 + 1;
          lVar10 = lVar10 + 1;
          plVar19 = plVar19 + 1;
        } while (lVar10 <= iVar5);
      }
      FUN_140023f20(param_1[0x11],lStackX_8);
      FUN_1408918f0(param_1 + 0x25,&lStackX_8);
    }
code_r0x0001401ca73d:
    cVar22 = FUN_14013e730(param_2,&puStack_f78);
  }
  FUN_14013b980(param_2,&UNK_141b05748);
  iVar5 = FUN_14025c6e0(param_1 + 0x21,param_2);
  if (0 < iVar5) {
    FUN_1401c6f80(param_1,&UNK_141b05750);
    *(int *)(param_1 + 0x20) = (int)param_1[0x20] + iVar5;
  }
  if (param_1[0x1b] != 0) {
    FUN_14025be70(param_1 + 0x21,param_1[0x1b] + 0x108);
  }
  cVar22 = FUN_1401372e0(param_2,&UNK_141b05780);
  if (cVar22 != '\0') {
    FUN_14013ba90(param_2,5,0x2e,&puStack_f78);
    cVar22 = FUN_14013e730(param_2,&puStack_f78);
    while ((cVar22 != '\0' && ((iStack_e68 != 5 || (uStack_e64 != 0x2f))))) {
      uVar11 = D2_InternedName_Resolve(&puStack_f78);
      FUN_14013f9d0(&uStack_e28,uVar11);
      FUN_14013e730(param_2,&puStack_f78);
      lVar10 = param_1[10];
      D2_InternedName_Resolve(&uStack_e28);
      psVar15 = (short *)FUN_140256080(lVar10,&lStackX_8);
      if (-1 < *psVar15) {
        cVar22 = FUN_140143bb0(&puStack_f78);
        if (cVar22 == '\0') {
          uVar11 = D2_InternedName_Resolve(&puStack_f78);
          plStackX_18 = (longlong *)FUN_1400607f0(&DAT_142434e90,uVar11);
          if (plStackX_18 == (longlong *)0x0) goto code_r0x0001401caa3b;
          plVar16 = (longlong *)D2_InternedName_Resolve(&uStack_e28);
          plVar14 = param_1 + 0x2c;
          uVar8 = *(uint *)(param_1 + 0x2e);
          plStackX_20 = plVar16;
          plStack_db8 = plVar14;
          uVar7 = FUN_1401431a0(plVar16);
          plVar19 = (longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);
          lVar10 = *plVar19;
          while (lVar10 != 0) {
            uVar11 = D2_InternedName_Resolve(lVar10);
            iVar5 = FUN_140143650(uVar11,plVar16);
            if (iVar5 == 0) {
              *(longlong **)(lVar10 + 0x28) = plStackX_18;
              goto code_r0x0001401caa3b;
            }
            if (0 < iVar5) break;
            plVar19 = (longlong *)(lVar10 + 0x30);
            lVar10 = *plVar19;
          }
          *(int *)((longlong)param_1 + 0x16c) = *(int *)((longlong)param_1 + 0x16c) + 1;
          plVar16 = (longlong *)FUN_1400f14b0(0x38,*(undefined1 *)((longlong)param_1 + 0x174));
          plStack_f98 = plVar16;
          if (plVar16 == (longlong *)0x0) {
            plVar16 = (longlong *)0x0;
          }
          else {
            lVar9 = *(longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);
            FUN_14013f9d0(plVar16,plStackX_20);
            plVar16[5] = (longlong)plStackX_18;
            plVar16[6] = lVar9;
          }
        }
        else {
          plVar16 = (longlong *)D2_InternedName_Resolve(&uStack_e28);
          plVar14 = param_1 + 0x2c;
          uVar8 = *(uint *)(param_1 + 0x2e);
          plStackX_18 = plVar16;
          plStackX_20 = plVar14;
          uVar7 = FUN_1401431a0(plVar16);
          plVar19 = (longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);
          lVar10 = *plVar19;
          while (lVar10 != 0) {
            uVar11 = D2_InternedName_Resolve(lVar10);
            iVar5 = FUN_140143650(uVar11,plVar16);
            if (iVar5 == 0) {
              *(undefined8 *)(lVar10 + 0x28) = 0;
              goto code_r0x0001401caa3b;
            }
            if (0 < iVar5) break;
            plVar19 = (longlong *)(lVar10 + 0x30);
            lVar10 = *plVar19;
          }
          *(int *)((longlong)param_1 + 0x16c) = *(int *)((longlong)param_1 + 0x16c) + 1;
          plVar16 = (longlong *)FUN_1400f14b0(0x38,*(undefined1 *)((longlong)param_1 + 0x174));
          plStack_db8 = plVar16;
          if (plVar16 == (longlong *)0x0) {
            plVar16 = (longlong *)0x0;
          }
          else {
            lVar9 = *(longlong *)(*plVar14 + (longlong)(int)(uVar7 & uVar8) * 8);
            FUN_14013f9d0(plVar16,plStackX_18);
            plVar16[5] = 0;
            plVar16[6] = lVar9;
          }
        }
        *plVar19 = (longlong)plVar16;
        plVar16[6] = lVar10;
      }
code_r0x0001401caa3b:
      FUN_14013fd10(&uStack_e28);
      cVar22 = FUN_14013e730(param_2,&puStack_f78);
    }
  }
  cVar22 = FUN_1401372e0(param_2,&UNK_141b05790);
  if (cVar22 != '\0') {
    FUN_14013ba90(param_2,5,0x2e,&puStack_f78);
    FUN_14013ba90(param_2,1,0,&puStack_f78);
    FUN_14013ba90(param_2,5,0x2f,&puStack_f78);
  }
  cVar22 = FUN_1401372e0(param_2,&UNK_141b057b0);
  if (cVar22 == '\0') {
    lVar10 = 0;
    if (param_1[0x1b] != 0) {
      lVar10 = *(longlong *)(param_1[0x1b] + 0x138);
    }
  }
  else {
    FUN_14013ba90(param_2,1,0,&puStack_f78);
    uVar11 = D2_InternedName_Resolve(&puStack_f78);
    lVar10 = FUN_14001e030(0x143990200,uVar11,0,0);
  }
  param_1[0x27] = lVar10;
  cVar22 = FUN_1401372e0(param_2,&UNK_141b057c0);
  if (cVar22 == '\0') {
    lVar10 = 0;
    if (param_1[0x1b] != 0) {
      lVar10 = *(longlong *)(param_1[0x1b] + 0x140);
    }
  }
  else {
    FUN_14013ba90(param_2,1,0,&puStack_f78);
    uVar11 = D2_InternedName_Resolve(&puStack_f78);
    lVar10 = FUN_14001e030(0x1439907a0,uVar11,0,0);
  }
  param_1[0x28] = lVar10;
  cVar22 = FUN_1401372e0(param_2,&UNK_141b057d8);
  if (cVar22 == '\0') {
    lVar10 = 0;
    if (param_1[0x1b] != 0) {
      lVar10 = *(longlong *)(param_1[0x1b] + 0x148);
    }
  }
  else {
    FUN_14013ba90(param_2,1,0,&puStack_f78);
    uVar11 = D2_InternedName_Resolve(&puStack_f78);
    lVar10 = FUN_1400607f0(0x143990890,uVar11);
  }
  param_1[0x29] = lVar10;
  cVar22 = FUN_1401372e0(param_2,&UNK_141b057e8);
  if (cVar22 == '\0') {
    lVar10 = 0;
    if (param_1[0x1b] != 0) {
      lVar10 = *(longlong *)(param_1[0x1b] + 0x150);
    }
  }
  else {
    FUN_14013ba90(param_2,1,0,&puStack_f78);
    uVar11 = D2_InternedName_Resolve(&puStack_f78);
    lVar10 = FUN_1400607f0(0x143990d40,uVar11);
  }
  param_1[0x2a] = lVar10;
  cVar22 = FUN_1401372e0(param_2,&UNK_141b05800);
  if (cVar22 == '\0') {
code_r0x0001401cae0d:
    if ((*(uint *)((undefined8 *)param_1[0xd] + 1) & 0xffffff) == 0) {
      lVar10 = 0;
    }
    else {
      lVar10 = **(longlong **)param_1[0xd];
    }
    if ((param_1[10] != 0) && (lVar10 != 0)) {
      FUN_1402548e0(param_1 + 0x14);
    }
    FUN_1401c77f0(&uStack_dd0);
    FUN_1400f1710(uStack_dd0);
    uStack_dd0 = 0;
    uStack_dc8 = 0;
    uStack_dc0 = 0;
    FUN_1401c7720(&uStack_de8);
    FUN_1400f1710(uStack_de8);
    uStack_de8 = 0;
    uStack_de0 = 0;
    uStack_dd8 = 0;
    FUN_14013fd10(&puStack_f78);
    return;
  }
  FUN_14013ba90(param_2,5,0x2e,&puStack_f78);
  cVar22 = FUN_14013e730(param_2,&puStack_f78);
  do {
    if ((cVar22 == '\0') || ((iStack_e68 == 5 && (uStack_e64 == 0x2f)))) goto code_r0x0001401cae0d;
    uVar11 = D2_InternedName_Resolve(&puStack_f78);
    FUN_140131f90(&plStackX_18,uVar11,0);
    FUN_14013e730(param_2,&puStack_f78);
    uVar11 = D2_InternedName_Resolve(&puStack_f78);
    FUN_140131f90(&plStackX_20,uVar11,0);
    FUN_1402563e0(param_1[10],&lStackX_8);
    if (-1 < (short)lStackX_8) {
      iVar5 = 0;
      lVar10 = 0;
      if ((*(uint *)(param_1 + 0x13) & 0xffffff) != 0) {
        puVar18 = (undefined8 *)param_1[0x12];
        do {
          if ((longlong *)*puVar18 == plStackX_18) {
            puVar18[1] = plStackX_20;
            cVar22 = FUN_1401c8b10(param_1,&plStackX_18);
            if ((((cVar22 == '\0') && ((char)*plStackX_20 == '\0')) && (-1 < iVar5)) &&
               ((uVar8 = *(uint *)(param_1 + 0x13), iVar5 < (int)(uVar8 & 0xffffff) &&
                (uVar8 = (uVar8 - 1 ^ uVar8) & 0xffffff ^ uVar8, *(uint *)(param_1 + 0x13) = uVar8,
                iVar5 < (int)(uVar8 & 0xffffff))))) {
              lVar10 = lVar10 << 4;
              do {
                lVar9 = param_1[0x12];
                *(undefined8 *)(lVar10 + lVar9) = *(undefined8 *)(lVar10 + 0x10 + lVar9);
                *(undefined8 *)(lVar10 + 8 + lVar9) = *(undefined8 *)(lVar10 + 0x18 + lVar9);
                iVar5 = iVar5 + 1;
                lVar10 = lVar10 + 0x10;
              } while (iVar5 < (int)(*(uint *)(param_1 + 0x13) & 0xffffff));
            }
            goto code_r0x0001401cadf8;
          }
          iVar5 = iVar5 + 1;
          lVar10 = lVar10 + 1;
          puVar18 = puVar18 + 2;
        } while (iVar5 < (int)(*(uint *)(param_1 + 0x13) & 0xffffff));
      }
      if ((char)*plStackX_20 != '\0') {
        plStack_f98 = plStackX_18;
        uStack_f90 = plStackX_20;
        FUN_1401c7120(param_1 + 0x12,&plStack_f98);
      }
    }
code_r0x0001401cadf8:
    cVar22 = FUN_14013e730(param_2,&puStack_f78);
  } while( true );
}


````
