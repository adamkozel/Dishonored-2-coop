# native-retargeting-lifecycle.json

Original: [work/natural-moveset/carry-peer-research-v1/native-retargeting-lifecycle.json](<../../../../work/natural-moveset/carry-peer-research-v1/native-retargeting-lifecycle.json>)

SHA-256: `1F5280B71FE7E012AC6494CF8E87F209CC9642D77A6711888EC4F89FEC66B2CC`; 20771 bytes.

## Original saved record

````json
{
  "confirmed": [
    "Carry45+E8 points to a registered arkCpntRetargeting (vtable RVA1c57778); its declaration has vtable RVA1c57638 and owner+10 is the local native player.",
    "Native setup 140829180 calls 140755870 with the corpse and tag-pair settings, adjusts attachment record offsets, selects mode7, then starts 140757830. Cleanup140828e80 calls140757550.",
    "140757830 stores the retargeting owner into target reference-frame+3F0. 140757550 clears it and calls native release handling per live target.",
    "This attachment is distinct from entity+AC, which remains the authored spawner. It explains why cross-world corpse identity can remain valid while carrying."
  ],
  "notYetVerified": [
    "Applying the component on a controlled remote Doppelganger, including correct owner/target joints and authored shoulder offsets.",
    "Native Midnight takeover/exit for the target corpse, its ragdoll state and the effect of early ragdoll release.",
    "Interruption cleanup, competing carrier claims, death, reload and peer loss."
  ],
  "runtimeEvidence": "native-carry-carrier-emily-v3/native-carry-attachment-object.json",
  "functions": {
    "140755870": {
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140755870(longlong param_1,longlong param_2,longlong param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  short *psVar4;\r\n  undefined8 uVar5;\r\n  short local_res8 [4];\r\n  undefined1 local_128 [16];\r\n  undefined8 local_118;\r\n  undefined8 uStack_110;\r\n  undefined8 local_108;\r\n  undefined8 uStack_100;\r\n  undefined4 local_f8;\r\n  undefined4 uStack_f4;\r\n  undefined4 uStack_f0;\r\n  undefined4 uStack_ec;\r\n  undefined4 local_e8;\r\n  undefined4 uStack_e4;\r\n  undefined4 uStack_e0;\r\n  undefined4 uStack_dc;\r\n  undefined4 local_d8;\r\n  undefined4 local_d4;\r\n  undefined4 local_d0;\r\n  undefined4 local_cc;\r\n  undefined4 local_c8;\r\n  undefined4 local_c4;\r\n  undefined4 local_c0;\r\n  short local_bc;\r\n  undefined4 local_b8;\r\n  undefined4 local_b4;\r\n  undefined4 local_b0;\r\n  undefined4 local_ac;\r\n  undefined4 local_a8;\r\n  undefined4 local_a4;\r\n  undefined4 local_a0;\r\n  short local_9c;\r\n  undefined4 local_98;\r\n  undefined8 local_90;\r\n  undefined4 local_48;\r\n  float local_44;\r\n  byte local_2c;\r\n  \r\n  lVar1 = FUN_140878470(*(undefined8 *)(param_1 + 0x10));\r\n  lVar2 = FUN_140878470(param_2);\r\n  FUN_140755430(local_128);\r\n  local_118 = _DAT_1431af380;\r\n  uStack_110 = _DAT_1431af388;\r\n  local_108 = _DAT_1431af390;\r\n  uStack_100 = _DAT_1431af398;\r\n  local_f8 = DAT_1431af3a0;\r\n  uStack_f4 = DAT_1431af3a4;\r\n  uStack_f0 = DAT_1431af3a8;\r\n  uStack_ec = DAT_1431af3ac;\r\n  local_e8 = DAT_1431af3b0;\r\n  uStack_e4 = DAT_1431af3b4;\r\n  uStack_e0 = DAT_1431af3b8;\r\n  uStack_dc = DAT_1431af3bc;\r\n  if (lVar2 != 0) {\r\n    lVar3 = FUN_14025c0c0(*(longlong *)(lVar2 + 0x1fe8) + 0x108,&DAT_1432d07a0,param_3 + 8);\r\n    if (*(short *)(lVar3 + 0x1c) == -1) {\r\n      local_d8 = DAT_1431af668;\r\n      local_d4 = DAT_1431af66c;\r\n      local_d0 = DAT_1431af670;\r\n      local_c8 = DAT_14228a8b4;\r\n      local_cc = DAT_14228a8b0;\r\n      local_c0 = DAT_14228a8bc;\r\n      local_c4 = DAT_14228a8b8;\r\n      lVar3 = *(longlong *)(*(longlong *)(lVar2 + 0x1fe8) + 0x50);\r\n      if (lVar3 == 0) {\r\n        psVar4 = local_res8;\r\n        local_res8[0] = -1;\r\n      }\r\n      else {\r\n        psVar4 = (short *)FUN_140256080(lVar3,local_res8,*(undefined8 *)(param_3 + 8));\r\n      }\r\n      lVar3 = *(longlong *)(*(longlong *)(lVar2 + 0x1fe8) + 0x50);\r\n      if (lVar3 == 0) {\r\n        local_res8[0] = -1;\r\n        psVar4 = local_res8;\r\n      }\r\n      else {\r\n        psVar4 = (short *)FUN_140256000(lVar3,local_res8,*psVar4);\r\n      }\r\n      local_bc = *psVar4;\r\n    }\r\n    else {\r\n      FUN_14025b6a0(&local_d8,lVar3);\r\n    }\r\n  }\r\n  if (lVar1 != 0) {\r\n    lVar3 = *(longlong *)(lVar1 + 0x1fe8);\r\n    if ((lVar2 != 0) &&\r\n       (lVar2 = FUN_14025c0c0(lVar3 + 0x108,lVar2 + 0x1bb0,param_3 + 0x10),\r\n       *(short *)(lVar2 + 0x1c) != -1)) {\r\n      FUN_14025b6a0(&local_b8,lVar2);\r\n    }\r\n    if (local_9c == -1) {\r\n      lVar2 = FUN_14025c0c0(lVar3 + 0x108,&DAT_1432d07a0,param_3 + 0x10);\r\n      if (*(short *)(lVar2 + 0x1c) == -1) {\r\n        psVar4 = local_res8;\r\n        local_b8 = DAT_1431af668;\r\n        local_b4 = DAT_1431af66c;\r\n        local_b0 = DAT_1431af670;\r\n        local_a8 = DAT_14228a8b4;\r\n        local_ac = DAT_14228a8b0;\r\n        local_a0 = DAT_14228a8bc;\r\n        local_a4 = DAT_14228a8b8;\r\n        lVar2 = *(longlong *)(*(longlong *)(lVar1 + 0x1fe8) + 0x50);\r\n        if (lVar2 == 0) {\r\n          local_res8[0] = -1;\r\n        }\r\n        else {\r\n          psVar4 = (short *)FUN_140256080(lVar2,psVar4,*(undefined8 *)(param_3 + 0x10));\r\n        }\r\n        lVar1 = *(longlong *)(*(longlong *)(lVar1 + 0x1fe8) + 0x50);\r\n        if (lVar1 == 0) {\r\n          local_res8[0] = -1;\r\n          psVar4 = local_res8;\r\n        }\r\n        else {\r\n          psVar4 = (short *)FUN_140256000(lVar1,local_res8,*psVar4);\r\n        }\r\n        local_9c = *psVar4;\r\n      }\r\n      else {\r\n        FUN_14025b6a0(&local_b8,lVar2);\r\n      }\r\n    }\r\n  }\r\n  local_48 = *(undefined4 *)(param_3 + 0x18);\r\n  local_44 = 1e+30;\r\n  local_2c = (*(byte *)(param_3 + 0x20) & 1) << 2 | local_2c & 0xfa;\r\n  if (*(float *)(param_3 + 0x1c) != 1e+30) {\r\n    local_44 = *(float *)(param_3 + 0x1c) * 0.017453292;\r\n  }\r\n  if (param_2 == 0) {\r\n    local_98 = 0x3fff0010;\r\n  }\r\n  else {\r\n    local_98 = *(undefined4 *)(param_2 + 0xa8);\r\n  }\r\n  local_90 = FUN_1408783d0(param_2);\r\n  uVar5 = FUN_140877720(param_2,&DAT_1439b4630,1);\r\n  FUN_1403dab80(local_128,uVar5);\r\n  FUN_140755bf0(param_1 + 0x20,local_128);\r\n  return;\r\n}\r\n\r\n"
    },
    "140757550": {
      "result": "\r\nvoid FUN_140757550(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  longlong *plVar3;\r\n  ulonglong uVar4;\r\n  longlong lVar5;\r\n  uint uVar6;\r\n  ulonglong uVar7;\r\n  \r\n  uVar4 = 0;\r\n  *(undefined4 *)(param_1 + 0x30) = 0;\r\n  uVar7 = uVar4;\r\n  if ((*(uint *)(param_1 + 0x28) & 0xffffff) != 0) {\r\n    do {\r\n      lVar5 = (longlong)(int)uVar7 * 0x100;\r\n      lVar1 = FUN_1406dc560(*(undefined4 *)(lVar5 + 0x90 + *(longlong *)(param_1 + 0x20)));\r\n      if (lVar1 != 0) {\r\n        uVar2 = FUN_1406dc560(*(undefined4 *)(lVar5 + 0x90 + *(longlong *)(param_1 + 0x20)));\r\n        FUN_140757c10(param_1,uVar2);\r\n        *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x20) + 0x98 + uVar4) + 0x3f0) = 0;\r\n      }\r\n      uVar6 = (int)uVar7 + 1;\r\n      uVar4 = uVar4 + 0x100;\r\n      uVar7 = (ulonglong)uVar6;\r\n    } while ((int)uVar6 < (int)(*(uint *)(param_1 + 0x28) & 0xffffff));\r\n  }\r\n  if ((*(byte *)(param_1 + 0x2c) & 0x40) == 0) {\r\n    lVar1 = *(longlong *)(param_1 + 0x20);\r\n    if (lVar1 != 0) {\r\n      plVar3 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar3 + 0x10))\r\n                (plVar3,\r\n                 \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                 ,0x1ec,lVar1,0x10);\r\n    }\r\n    *(uint *)(param_1 + 0x2c) = *(uint *)(param_1 + 0x2c) & 0xff;\r\n    *(undefined8 *)(param_1 + 0x20) = 0;\r\n  }\r\n  *(uint *)(param_1 + 0x28) = *(uint *)(param_1 + 0x28) & 0xff000000;\r\n  *(undefined8 *)(param_1 + 0x68) = 0;\r\n  *(undefined8 *)(param_1 + 0x70) = 0;\r\n  *(undefined4 *)(param_1 + 0x78) = 0;\r\n  return;\r\n}\r\n\r\n"
    },
    "140757730": {
      "result": "\r\nvoid FUN_140757730(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  longlong lVar2;\r\n  undefined8 local_18;\r\n  undefined4 local_10;\r\n  \r\n  lVar2 = (**(code **)*param_2)(param_2);\r\n  if ((*(int *)(lVar2 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar2 + 0x60))) {\r\n    lVar2 = FUN_140877720(param_2,&DAT_1439d7510,2);\r\n    if (lVar2 != 0) {\r\n      FUN_1407ff170(lVar2,1);\r\n      if ((*(uint *)(param_1 + 0x3c) >> 2 & 1) != 0) {\r\n        FUN_1407fd830(lVar2);\r\n        local_18 = 0;\r\n        local_10 = 0;\r\n        FUN_1407ff080(lVar2,&local_18);\r\n      }\r\n    }\r\n  }\r\n  else {\r\n    lVar2 = FUN_140877720(param_2,&DAT_1439e2a00,1);\r\n    if (lVar2 != 0) {\r\n      local_18 = 0;\r\n      local_10 = 0;\r\n      FUN_1405fc880(lVar2,&local_18,0,1);\r\n      if (*(int *)(param_1 + 0x34) == 0) {\r\n        pbVar1 = (byte *)(*(longlong *)(*(longlong *)(lVar2 + 0xa8) + 0x98) + 0x130);\r\n        *pbVar1 = *pbVar1 & 0xfe;\r\n        return;\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140757830": {
      "result": "\r\nvoid FUN_140757830(longlong param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 uVar2;\r\n  undefined4 uVar3;\r\n  undefined4 uVar4;\r\n  undefined4 uVar5;\r\n  undefined4 uVar6;\r\n  undefined4 uVar7;\r\n  undefined4 uVar8;\r\n  undefined4 uVar9;\r\n  undefined4 uVar10;\r\n  undefined4 uVar11;\r\n  longlong lVar12;\r\n  longlong lVar13;\r\n  longlong lVar14;\r\n  undefined8 uVar15;\r\n  ulonglong uVar16;\r\n  uint uVar17;\r\n  ulonglong uVar18;\r\n  float fVar19;\r\n  float fVar20;\r\n  \r\n  lVar12 = *(longlong *)(param_1 + 8);\r\n  uVar16 = 0;\r\n  *(undefined4 *)(param_1 + 0x30) = 0;\r\n  uVar18 = uVar16;\r\n  if ((*(uint *)(param_1 + 0x28) & 0xffffff) != 0) {\r\n    do {\r\n      fVar20 = *(float *)(lVar12 + 0x68);\r\n      lVar13 = *(longlong *)(param_1 + 0x20);\r\n      if (fVar20 != 1e+30) {\r\n        fVar19 = *(float *)(uVar16 + 0xe0 + lVar13);\r\n        if (fVar20 <= fVar19) {\r\n          fVar19 = fVar20;\r\n        }\r\n        *(float *)(uVar16 + 0xe0 + lVar13) = fVar19;\r\n      }\r\n      fVar20 = *(float *)(lVar12 + 0x6c);\r\n      if (fVar20 != 1e+30) {\r\n        fVar19 = *(float *)(uVar16 + 0xe4 + lVar13);\r\n        fVar20 = fVar20 * 0.017453292;\r\n        if (fVar20 <= fVar19) {\r\n          fVar19 = fVar20;\r\n        }\r\n        *(float *)(uVar16 + 0xe4 + lVar13) = fVar19;\r\n      }\r\n      lVar14 = FUN_1406dc560(*(undefined4 *)(uVar16 + 0x90 + lVar13));\r\n      uVar1 = *(undefined4 *)(lVar14 + 0x318);\r\n      uVar2 = *(undefined4 *)(lVar14 + 0x324);\r\n      uVar3 = *(undefined4 *)(lVar14 + 0x300);\r\n      uVar4 = *(undefined4 *)(lVar14 + 0x310);\r\n      uVar5 = *(undefined4 *)(lVar14 + 0x31c);\r\n      uVar6 = *(undefined4 *)(lVar14 + 0x328);\r\n      uVar7 = *(undefined4 *)(lVar14 + 0x304);\r\n      uVar8 = *(undefined4 *)(lVar14 + 0x314);\r\n      uVar9 = *(undefined4 *)(lVar14 + 800);\r\n      uVar10 = *(undefined4 *)(lVar14 + 0x32c);\r\n      uVar11 = *(undefined4 *)(lVar14 + 0x308);\r\n      *(undefined4 *)(uVar16 + 0x10 + lVar13) = *(undefined4 *)(lVar14 + 0x30c);\r\n      *(undefined4 *)(uVar16 + 0x14 + lVar13) = uVar1;\r\n      *(undefined4 *)(uVar16 + 0x18 + lVar13) = uVar2;\r\n      *(undefined4 *)(uVar16 + 0x1c + lVar13) = uVar3;\r\n      *(undefined4 *)(uVar16 + 0x20 + lVar13) = uVar4;\r\n      *(undefined4 *)(uVar16 + 0x24 + lVar13) = uVar5;\r\n      *(undefined4 *)(uVar16 + 0x28 + lVar13) = uVar6;\r\n      *(undefined4 *)(uVar16 + 0x2c + lVar13) = uVar7;\r\n      *(undefined4 *)(uVar16 + 0x30 + lVar13) = uVar8;\r\n      *(undefined4 *)(uVar16 + 0x34 + lVar13) = uVar9;\r\n      *(undefined4 *)(uVar16 + 0x38 + lVar13) = uVar10;\r\n      *(undefined4 *)(uVar16 + 0x3c + lVar13) = uVar11;\r\n      *(undefined4 *)(uVar16 + 0x40 + lVar13) = 0;\r\n      *(undefined4 *)(uVar16 + 0x44 + lVar13) = 0;\r\n      *(undefined4 *)(uVar16 + 0x48 + lVar13) = 0;\r\n      *(undefined4 *)(uVar16 + 0x4c + lVar13) = 0x3f800000;\r\n      if (*(int *)(param_1 + 0x3c) != 0) {\r\n        *(undefined8 *)(*(longlong *)(uVar16 + 0x98 + lVar13) + 0x3f0) =\r\n             *(undefined8 *)(param_1 + 0x10);\r\n      }\r\n      uVar15 = FUN_1406dc560(*(undefined4 *)(uVar16 + 0x90 + lVar13));\r\n      FUN_140757730(param_1,uVar15);\r\n      if ((*(byte *)(uVar16 + 0xfc + lVar13) & 4) != 0) {\r\n        *(undefined4 *)(param_1 + 0x38) = 0;\r\n      }\r\n      uVar17 = (int)uVar18 + 1;\r\n      uVar16 = uVar16 + 0x100;\r\n      uVar18 = (ulonglong)uVar17;\r\n    } while ((int)uVar17 < (int)(*(uint *)(param_1 + 0x28) & 0xffffff));\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140829180": {
      "result": "\r\nvoid FUN_140829180(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  float fVar3;\r\n  float fVar4;\r\n  undefined2 uVar5;\r\n  undefined2 uVar6;\r\n  longlong lVar7;\r\n  undefined8 *puVar8;\r\n  undefined8 uVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  float *pfVar12;\r\n  undefined4 *puVar13;\r\n  float *pfVar14;\r\n  longlong lVar15;\r\n  char *pcVar16;\r\n  float fVar17;\r\n  float local_res8 [2];\r\n  undefined8 local_res10;\r\n  undefined8 local_res20;\r\n  undefined4 local_228 [2];\r\n  undefined4 local_220;\r\n  undefined4 local_21c;\r\n  float local_218;\r\n  float local_214;\r\n  float local_210;\r\n  float local_20c;\r\n  float local_208;\r\n  float local_204;\r\n  float local_200;\r\n  float local_1fc;\r\n  float local_1f8 [7];\r\n  undefined4 local_1dc;\r\n  undefined4 local_1d8;\r\n  undefined4 local_1d4;\r\n  undefined4 local_1d0;\r\n  undefined4 local_1cc;\r\n  undefined4 local_1c8;\r\n  undefined4 local_1c4;\r\n  undefined4 local_1c0;\r\n  undefined4 local_1bc;\r\n  undefined4 local_1b8;\r\n  float local_1b4;\r\n  float local_1b0;\r\n  undefined4 local_1a8;\r\n  undefined4 local_1a4;\r\n  undefined4 local_1a0;\r\n  float local_198;\r\n  float local_194;\r\n  float local_190;\r\n  float local_18c;\r\n  float local_188;\r\n  float local_184;\r\n  float local_180;\r\n  float local_17c;\r\n  float local_178;\r\n  float local_170;\r\n  undefined4 local_16c;\r\n  undefined4 local_168;\r\n  undefined4 local_164;\r\n  undefined4 local_160;\r\n  undefined4 local_15c;\r\n  undefined4 local_158;\r\n  undefined4 local_154;\r\n  undefined4 local_150;\r\n  undefined1 local_148 [8];\r\n  undefined8 local_140;\r\n  undefined8 local_138;\r\n  undefined4 local_130;\r\n  undefined4 local_12c;\r\n  undefined1 local_128;\r\n  float local_118 [7];\r\n  undefined4 local_fc;\r\n  undefined4 local_f8;\r\n  undefined4 local_f4;\r\n  undefined4 local_f0;\r\n  undefined4 local_ec;\r\n  \r\n  lVar7 = *(longlong *)(param_1 + 0x18);\r\n  pcVar16 = *(char **)(lVar7 + 0x80);\r\n  local_res10 = param_2;\r\n  if (*pcVar16 == '\\0') {\r\n    FUN_140196fc0(param_3,local_res8,0);\r\n    puVar8 = (undefined8 *)FUN_140195370(param_3,&local_res20,local_res8[0],1);\r\n    pcVar16 = (char *)*puVar8;\r\n  }\r\n  uVar9 = FUN_140854880(param_1);\r\n  lVar10 = FUN_140878470(uVar9);\r\n  lVar15 = *(longlong *)(lVar10 + 0x1fe8);\r\n  lVar11 = FUN_1401c7c80(lVar15,pcVar16,1);\r\n  local_res20 = FUN_1401c7030(lVar15,lVar11 + 8,0);\r\n  if (*(longlong *)(lVar15 + 0x50) == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,*param_4);\r\n  }\r\n  uVar5 = *(undefined2 *)pfVar12;\r\n  if (*(longlong *)(lVar15 + 0x50) == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,\r\n                                     *(undefined8 *)(lVar7 + 0x98));\r\n  }\r\n  uVar6 = *(undefined2 *)pfVar12;\r\n  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar5,local_res20,0,&local_1a8,&local_170);\r\n  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar6,local_res20,0,&local_1b8,&local_198);\r\n  local_res8[0] = local_180;\r\n  local_1f8[1] = (float)local_164;\r\n  local_res20 = CONCAT44(local_res20._4_4_,local_1b8);\r\n  local_1f8[2] = (float)local_158;\r\n  local_208 = local_194;\r\n  local_1f8[3] = (float)local_1a8;\r\n  local_200 = local_188;\r\n  local_1f8[4] = (float)local_16c;\r\n  local_204 = local_17c;\r\n  local_1f8[5] = (float)local_160;\r\n  local_210 = local_1b4;\r\n  local_1f8[6] = (float)local_154;\r\n  local_1fc = local_190;\r\n  local_1dc = local_1a4;\r\n  local_20c = local_184;\r\n  local_1d8 = local_168;\r\n  local_214 = local_178;\r\n  local_1d4 = local_15c;\r\n  local_218 = local_1b0;\r\n  local_1d0 = local_150;\r\n  local_1f8[0] = local_170;\r\n  local_1cc = local_1a0;\r\n  local_1c8 = 0;\r\n  local_1c4 = 0;\r\n  local_1c0 = 0;\r\n  local_1bc = 0x3f800000;\r\n  FUN_14010f0d0(local_1f8);\r\n  pfVar12 = local_1f8 + 2;\r\n  lVar15 = 4;\r\n  pfVar14 = local_118 + 2;\r\n  do {\r\n    fVar1 = pfVar12[-1];\r\n    fVar2 = pfVar12[-2];\r\n    fVar3 = *pfVar12;\r\n    fVar4 = pfVar12[1];\r\n    pfVar12 = pfVar12 + 4;\r\n    fVar17 = fVar4 * 0.0;\r\n    pfVar14[-2] = local_198 * fVar2 + local_208 * fVar1 + fVar3 * local_1fc + fVar17;\r\n    pfVar14[-1] = local_18c * fVar2 + local_200 * fVar1 + fVar3 * local_20c + fVar17;\r\n    *pfVar14 = local_res8[0] * fVar2 + local_204 * fVar1 + fVar3 * local_214 + fVar17;\r\n    pfVar14[1] = (float)local_res20 * fVar2 + local_210 * fVar1 + fVar3 * local_218 + fVar4;\r\n    lVar15 = lVar15 + -1;\r\n    pfVar14 = pfVar14 + 4;\r\n  } while (lVar15 != 0);\r\n  FUN_140131ae0(local_148);\r\n  FUN_140131ae0(&local_140);\r\n  FUN_140131ae0(&local_138);\r\n  uVar9 = local_res10;\r\n  local_130 = 0x7149f2ca;\r\n  local_12c = 0x7149f2ca;\r\n  local_128 = 0;\r\n  local_140 = *(undefined8 *)(lVar7 + 0x98);\r\n  local_138 = *param_4;\r\n  FUN_140755870(*(undefined8 *)(param_1 + 0xe8),local_res10,local_148);\r\n  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x20);\r\n  lVar15 = *(longlong *)(*(longlong *)(lVar10 + 0x1fe8) + 0x50);\r\n  if (lVar15 == 0) {\r\n    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);\r\n    pfVar12 = local_res8;\r\n  }\r\n  else {\r\n    pfVar12 = (float *)FUN_140256000(lVar15,local_res8,uVar5);\r\n  }\r\n  *(undefined2 *)(lVar7 + 0x8c) = *(undefined2 *)pfVar12;\r\n  *(float *)(lVar7 + 0x70) = local_118[3];\r\n  *(undefined4 *)(lVar7 + 0x74) = local_fc;\r\n  *(undefined4 *)(lVar7 + 0x78) = local_ec;\r\n  local_1f8[0] = local_118[0];\r\n  local_1f8[3] = local_118[1];\r\n  local_1f8[6] = local_118[2];\r\n  local_1f8[1] = local_118[4];\r\n  local_1f8[4] = local_118[5];\r\n  local_1dc = local_118[6];\r\n  local_1f8[2] = (float)local_f8;\r\n  local_1f8[5] = (float)local_f4;\r\n  local_1d8 = local_f0;\r\n  puVar13 = (undefined4 *)FUN_140110080(local_1f8,local_228);\r\n  *(undefined4 *)(lVar7 + 0x7c) = *puVar13;\r\n  *(undefined4 *)(lVar7 + 0x80) = puVar13[1];\r\n  *(undefined4 *)(lVar7 + 0x84) = puVar13[2];\r\n  local_228[0] = 0;\r\n  *(undefined4 *)(lVar7 + 0x88) = puVar13[3];\r\n  lVar7 = *(longlong *)(param_1 + 0xe8);\r\n  local_220 = 7;\r\n  local_21c = CONCAT13(local_21c._3_1_,0x100);\r\n  *(undefined4 *)(lVar7 + 0x34) = 0;\r\n  *(undefined4 *)(lVar7 + 0x38) = 0;\r\n  *(undefined4 *)(lVar7 + 0x3c) = 7;\r\n  *(undefined4 *)(lVar7 + 0x40) = local_21c;\r\n  FUN_140757830(*(undefined8 *)(param_1 + 0xe8));\r\n  FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10)\r\n                ,uVar9);\r\n  return;\r\n}\r\n\r\n"
    },
    "140828e80": {
      "result": "\r\nvoid FUN_140828e80(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  \r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));\r\n  if (lVar1 != 0) {\r\n    FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar1);\r\n  }\r\n  FUN_140757550(*(undefined8 *)(param_1 + 0xe8));\r\n  return;\r\n}\r\n\r\n"
    },
    "140757c10": {
      "result": "\r\nvoid FUN_140757c10(longlong param_1,undefined8 *param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  char cVar2;\r\n  longlong lVar3;\r\n  \r\n  lVar3 = (**(code **)*param_2)(param_2);\r\n  if ((*(int *)(lVar3 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar3 + 0x60))) {\r\n    lVar3 = FUN_140877720(param_2,&DAT_1439d7510,2);\r\n    if (lVar3 != 0) {\r\n      FUN_1407ff170(lVar3,0);\r\n      if ((*(uint *)(param_1 + 0x3c) >> 2 & 1) != 0) {\r\n        FUN_1407fd970(lVar3);\r\n      }\r\n    }\r\n  }\r\n  else {\r\n    lVar3 = FUN_140877720(param_2,&DAT_1439e2a00,1);\r\n    if (lVar3 != 0) {\r\n      FUN_1405fb960(lVar3);\r\n      cVar2 = FUN_1405f79f0(lVar3);\r\n      if (cVar2 != '\\0') {\r\n        FUN_1405faf90(lVar3);\r\n        if (*(int *)(param_1 + 0x34) == 0) {\r\n          pbVar1 = (byte *)(*(longlong *)(*(longlong *)(lVar3 + 0xa8) + 0x98) + 0x130);\r\n          *pbVar1 = *pbVar1 | 1;\r\n          return;\r\n        }\r\n      }\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  }
}

````

## Unescaped text fields for reading

### result

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140755870(longlong param_1,longlong param_2,longlong param_3)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  short *psVar4;
  undefined8 uVar5;
  short local_res8 [4];
  undefined1 local_128 [16];
  undefined8 local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined4 local_f8;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  undefined4 uStack_ec;
  undefined4 local_e8;
  undefined4 uStack_e4;
  undefined4 uStack_e0;
  undefined4 uStack_dc;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined4 local_cc;
  undefined4 local_c8;
  undefined4 local_c4;
  undefined4 local_c0;
  short local_bc;
  undefined4 local_b8;
  undefined4 local_b4;
  undefined4 local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined4 local_a0;
  short local_9c;
  undefined4 local_98;
  undefined8 local_90;
  undefined4 local_48;
  float local_44;
  byte local_2c;
  
  lVar1 = FUN_140878470(*(undefined8 *)(param_1 + 0x10));
  lVar2 = FUN_140878470(param_2);
  FUN_140755430(local_128);
  local_118 = _DAT_1431af380;
  uStack_110 = _DAT_1431af388;
  local_108 = _DAT_1431af390;
  uStack_100 = _DAT_1431af398;
  local_f8 = DAT_1431af3a0;
  uStack_f4 = DAT_1431af3a4;
  uStack_f0 = DAT_1431af3a8;
  uStack_ec = DAT_1431af3ac;
  local_e8 = DAT_1431af3b0;
  uStack_e4 = DAT_1431af3b4;
  uStack_e0 = DAT_1431af3b8;
  uStack_dc = DAT_1431af3bc;
  if (lVar2 != 0) {
    lVar3 = FUN_14025c0c0(*(longlong *)(lVar2 + 0x1fe8) + 0x108,&DAT_1432d07a0,param_3 + 8);
    if (*(short *)(lVar3 + 0x1c) == -1) {
      local_d8 = DAT_1431af668;
      local_d4 = DAT_1431af66c;
      local_d0 = DAT_1431af670;
      local_c8 = DAT_14228a8b4;
      local_cc = DAT_14228a8b0;
      local_c0 = DAT_14228a8bc;
      local_c4 = DAT_14228a8b8;
      lVar3 = *(longlong *)(*(longlong *)(lVar2 + 0x1fe8) + 0x50);
      if (lVar3 == 0) {
        psVar4 = local_res8;
        local_res8[0] = -1;
      }
      else {
        psVar4 = (short *)FUN_140256080(lVar3,local_res8,*(undefined8 *)(param_3 + 8));
      }
      lVar3 = *(longlong *)(*(longlong *)(lVar2 + 0x1fe8) + 0x50);
      if (lVar3 == 0) {
        local_res8[0] = -1;
        psVar4 = local_res8;
      }
      else {
        psVar4 = (short *)FUN_140256000(lVar3,local_res8,*psVar4);
      }
      local_bc = *psVar4;
    }
    else {
      FUN_14025b6a0(&local_d8,lVar3);
    }
  }
  if (lVar1 != 0) {
    lVar3 = *(longlong *)(lVar1 + 0x1fe8);
    if ((lVar2 != 0) &&
       (lVar2 = FUN_14025c0c0(lVar3 + 0x108,lVar2 + 0x1bb0,param_3 + 0x10),
       *(short *)(lVar2 + 0x1c) != -1)) {
      FUN_14025b6a0(&local_b8,lVar2);
    }
    if (local_9c == -1) {
      lVar2 = FUN_14025c0c0(lVar3 + 0x108,&DAT_1432d07a0,param_3 + 0x10);
      if (*(short *)(lVar2 + 0x1c) == -1) {
        psVar4 = local_res8;
        local_b8 = DAT_1431af668;
        local_b4 = DAT_1431af66c;
        local_b0 = DAT_1431af670;
        local_a8 = DAT_14228a8b4;
        local_ac = DAT_14228a8b0;
        local_a0 = DAT_14228a8bc;
        local_a4 = DAT_14228a8b8;
        lVar2 = *(longlong *)(*(longlong *)(lVar1 + 0x1fe8) + 0x50);
        if (lVar2 == 0) {
          local_res8[0] = -1;
        }
        else {
          psVar4 = (short *)FUN_140256080(lVar2,psVar4,*(undefined8 *)(param_3 + 0x10));
        }
        lVar1 = *(longlong *)(*(longlong *)(lVar1 + 0x1fe8) + 0x50);
        if (lVar1 == 0) {
          local_res8[0] = -1;
          psVar4 = local_res8;
        }
        else {
          psVar4 = (short *)FUN_140256000(lVar1,local_res8,*psVar4);
        }
        local_9c = *psVar4;
      }
      else {
        FUN_14025b6a0(&local_b8,lVar2);
      }
    }
  }
  local_48 = *(undefined4 *)(param_3 + 0x18);
  local_44 = 1e+30;
  local_2c = (*(byte *)(param_3 + 0x20) & 1) << 2 | local_2c & 0xfa;
  if (*(float *)(param_3 + 0x1c) != 1e+30) {
    local_44 = *(float *)(param_3 + 0x1c) * 0.017453292;
  }
  if (param_2 == 0) {
    local_98 = 0x3fff0010;
  }
  else {
    local_98 = *(undefined4 *)(param_2 + 0xa8);
  }
  local_90 = FUN_1408783d0(param_2);
  uVar5 = FUN_140877720(param_2,&DAT_1439b4630,1);
  FUN_1403dab80(local_128,uVar5);
  FUN_140755bf0(param_1 + 0x20,local_128);
  return;
}


````

### result

````text

void FUN_140757550(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  longlong *plVar3;
  ulonglong uVar4;
  longlong lVar5;
  uint uVar6;
  ulonglong uVar7;
  
  uVar4 = 0;
  *(undefined4 *)(param_1 + 0x30) = 0;
  uVar7 = uVar4;
  if ((*(uint *)(param_1 + 0x28) & 0xffffff) != 0) {
    do {
      lVar5 = (longlong)(int)uVar7 * 0x100;
      lVar1 = FUN_1406dc560(*(undefined4 *)(lVar5 + 0x90 + *(longlong *)(param_1 + 0x20)));
      if (lVar1 != 0) {
        uVar2 = FUN_1406dc560(*(undefined4 *)(lVar5 + 0x90 + *(longlong *)(param_1 + 0x20)));
        FUN_140757c10(param_1,uVar2);
        *(undefined8 *)(*(longlong *)(*(longlong *)(param_1 + 0x20) + 0x98 + uVar4) + 0x3f0) = 0;
      }
      uVar6 = (int)uVar7 + 1;
      uVar4 = uVar4 + 0x100;
      uVar7 = (ulonglong)uVar6;
    } while ((int)uVar6 < (int)(*(uint *)(param_1 + 0x28) & 0xffffff));
  }
  if ((*(byte *)(param_1 + 0x2c) & 0x40) == 0) {
    lVar1 = *(longlong *)(param_1 + 0x20);
    if (lVar1 != 0) {
      plVar3 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar3 + 0x10))
                (plVar3,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,lVar1,0x10);
    }
    *(uint *)(param_1 + 0x2c) = *(uint *)(param_1 + 0x2c) & 0xff;
    *(undefined8 *)(param_1 + 0x20) = 0;
  }
  *(uint *)(param_1 + 0x28) = *(uint *)(param_1 + 0x28) & 0xff000000;
  *(undefined8 *)(param_1 + 0x68) = 0;
  *(undefined8 *)(param_1 + 0x70) = 0;
  *(undefined4 *)(param_1 + 0x78) = 0;
  return;
}


````

### result

````text

void FUN_140757730(longlong param_1,undefined8 *param_2)

{
  byte *pbVar1;
  longlong lVar2;
  undefined8 local_18;
  undefined4 local_10;
  
  lVar2 = (**(code **)*param_2)(param_2);
  if ((*(int *)(lVar2 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar2 + 0x60))) {
    lVar2 = FUN_140877720(param_2,&DAT_1439d7510,2);
    if (lVar2 != 0) {
      FUN_1407ff170(lVar2,1);
      if ((*(uint *)(param_1 + 0x3c) >> 2 & 1) != 0) {
        FUN_1407fd830(lVar2);
        local_18 = 0;
        local_10 = 0;
        FUN_1407ff080(lVar2,&local_18);
      }
    }
  }
  else {
    lVar2 = FUN_140877720(param_2,&DAT_1439e2a00,1);
    if (lVar2 != 0) {
      local_18 = 0;
      local_10 = 0;
      FUN_1405fc880(lVar2,&local_18,0,1);
      if (*(int *)(param_1 + 0x34) == 0) {
        pbVar1 = (byte *)(*(longlong *)(*(longlong *)(lVar2 + 0xa8) + 0x98) + 0x130);
        *pbVar1 = *pbVar1 & 0xfe;
        return;
      }
    }
  }
  return;
}


````

### result

````text

void FUN_140757830(longlong param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  longlong lVar12;
  longlong lVar13;
  longlong lVar14;
  undefined8 uVar15;
  ulonglong uVar16;
  uint uVar17;
  ulonglong uVar18;
  float fVar19;
  float fVar20;
  
  lVar12 = *(longlong *)(param_1 + 8);
  uVar16 = 0;
  *(undefined4 *)(param_1 + 0x30) = 0;
  uVar18 = uVar16;
  if ((*(uint *)(param_1 + 0x28) & 0xffffff) != 0) {
    do {
      fVar20 = *(float *)(lVar12 + 0x68);
      lVar13 = *(longlong *)(param_1 + 0x20);
      if (fVar20 != 1e+30) {
        fVar19 = *(float *)(uVar16 + 0xe0 + lVar13);
        if (fVar20 <= fVar19) {
          fVar19 = fVar20;
        }
        *(float *)(uVar16 + 0xe0 + lVar13) = fVar19;
      }
      fVar20 = *(float *)(lVar12 + 0x6c);
      if (fVar20 != 1e+30) {
        fVar19 = *(float *)(uVar16 + 0xe4 + lVar13);
        fVar20 = fVar20 * 0.017453292;
        if (fVar20 <= fVar19) {
          fVar19 = fVar20;
        }
        *(float *)(uVar16 + 0xe4 + lVar13) = fVar19;
      }
      lVar14 = FUN_1406dc560(*(undefined4 *)(uVar16 + 0x90 + lVar13));
      uVar1 = *(undefined4 *)(lVar14 + 0x318);
      uVar2 = *(undefined4 *)(lVar14 + 0x324);
      uVar3 = *(undefined4 *)(lVar14 + 0x300);
      uVar4 = *(undefined4 *)(lVar14 + 0x310);
      uVar5 = *(undefined4 *)(lVar14 + 0x31c);
      uVar6 = *(undefined4 *)(lVar14 + 0x328);
      uVar7 = *(undefined4 *)(lVar14 + 0x304);
      uVar8 = *(undefined4 *)(lVar14 + 0x314);
      uVar9 = *(undefined4 *)(lVar14 + 800);
      uVar10 = *(undefined4 *)(lVar14 + 0x32c);
      uVar11 = *(undefined4 *)(lVar14 + 0x308);
      *(undefined4 *)(uVar16 + 0x10 + lVar13) = *(undefined4 *)(lVar14 + 0x30c);
      *(undefined4 *)(uVar16 + 0x14 + lVar13) = uVar1;
      *(undefined4 *)(uVar16 + 0x18 + lVar13) = uVar2;
      *(undefined4 *)(uVar16 + 0x1c + lVar13) = uVar3;
      *(undefined4 *)(uVar16 + 0x20 + lVar13) = uVar4;
      *(undefined4 *)(uVar16 + 0x24 + lVar13) = uVar5;
      *(undefined4 *)(uVar16 + 0x28 + lVar13) = uVar6;
      *(undefined4 *)(uVar16 + 0x2c + lVar13) = uVar7;
      *(undefined4 *)(uVar16 + 0x30 + lVar13) = uVar8;
      *(undefined4 *)(uVar16 + 0x34 + lVar13) = uVar9;
      *(undefined4 *)(uVar16 + 0x38 + lVar13) = uVar10;
      *(undefined4 *)(uVar16 + 0x3c + lVar13) = uVar11;
      *(undefined4 *)(uVar16 + 0x40 + lVar13) = 0;
      *(undefined4 *)(uVar16 + 0x44 + lVar13) = 0;
      *(undefined4 *)(uVar16 + 0x48 + lVar13) = 0;
      *(undefined4 *)(uVar16 + 0x4c + lVar13) = 0x3f800000;
      if (*(int *)(param_1 + 0x3c) != 0) {
        *(undefined8 *)(*(longlong *)(uVar16 + 0x98 + lVar13) + 0x3f0) =
             *(undefined8 *)(param_1 + 0x10);
      }
      uVar15 = FUN_1406dc560(*(undefined4 *)(uVar16 + 0x90 + lVar13));
      FUN_140757730(param_1,uVar15);
      if ((*(byte *)(uVar16 + 0xfc + lVar13) & 4) != 0) {
        *(undefined4 *)(param_1 + 0x38) = 0;
      }
      uVar17 = (int)uVar18 + 1;
      uVar16 = uVar16 + 0x100;
      uVar18 = (ulonglong)uVar17;
    } while ((int)uVar17 < (int)(*(uint *)(param_1 + 0x28) & 0xffffff));
  }
  return;
}


````

### result

````text

void FUN_140829180(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 *param_4)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  longlong lVar7;
  undefined8 *puVar8;
  undefined8 uVar9;
  longlong lVar10;
  longlong lVar11;
  float *pfVar12;
  undefined4 *puVar13;
  float *pfVar14;
  longlong lVar15;
  char *pcVar16;
  float fVar17;
  float local_res8 [2];
  undefined8 local_res10;
  undefined8 local_res20;
  undefined4 local_228 [2];
  undefined4 local_220;
  undefined4 local_21c;
  float local_218;
  float local_214;
  float local_210;
  float local_20c;
  float local_208;
  float local_204;
  float local_200;
  float local_1fc;
  float local_1f8 [7];
  undefined4 local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 local_1c8;
  undefined4 local_1c4;
  undefined4 local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  float local_1b4;
  float local_1b0;
  undefined4 local_1a8;
  undefined4 local_1a4;
  undefined4 local_1a0;
  float local_198;
  float local_194;
  float local_190;
  float local_18c;
  float local_188;
  float local_184;
  float local_180;
  float local_17c;
  float local_178;
  float local_170;
  undefined4 local_16c;
  undefined4 local_168;
  undefined4 local_164;
  undefined4 local_160;
  undefined4 local_15c;
  undefined4 local_158;
  undefined4 local_154;
  undefined4 local_150;
  undefined1 local_148 [8];
  undefined8 local_140;
  undefined8 local_138;
  undefined4 local_130;
  undefined4 local_12c;
  undefined1 local_128;
  float local_118 [7];
  undefined4 local_fc;
  undefined4 local_f8;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  
  lVar7 = *(longlong *)(param_1 + 0x18);
  pcVar16 = *(char **)(lVar7 + 0x80);
  local_res10 = param_2;
  if (*pcVar16 == '\0') {
    FUN_140196fc0(param_3,local_res8,0);
    puVar8 = (undefined8 *)FUN_140195370(param_3,&local_res20,local_res8[0],1);
    pcVar16 = (char *)*puVar8;
  }
  uVar9 = FUN_140854880(param_1);
  lVar10 = FUN_140878470(uVar9);
  lVar15 = *(longlong *)(lVar10 + 0x1fe8);
  lVar11 = FUN_1401c7c80(lVar15,pcVar16,1);
  local_res20 = FUN_1401c7030(lVar15,lVar11 + 8,0);
  if (*(longlong *)(lVar15 + 0x50) == 0) {
    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);
    pfVar12 = local_res8;
  }
  else {
    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,*param_4);
  }
  uVar5 = *(undefined2 *)pfVar12;
  if (*(longlong *)(lVar15 + 0x50) == 0) {
    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);
    pfVar12 = local_res8;
  }
  else {
    pfVar12 = (float *)FUN_140256080(*(longlong *)(lVar15 + 0x50),local_res8,
                                     *(undefined8 *)(lVar7 + 0x98));
  }
  uVar6 = *(undefined2 *)pfVar12;
  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar5,local_res20,0,&local_1a8,&local_170);
  FUN_14053c8a0(*(undefined8 *)(param_1 + 0xf0),uVar6,local_res20,0,&local_1b8,&local_198);
  local_res8[0] = local_180;
  local_1f8[1] = (float)local_164;
  local_res20 = CONCAT44(local_res20._4_4_,local_1b8);
  local_1f8[2] = (float)local_158;
  local_208 = local_194;
  local_1f8[3] = (float)local_1a8;
  local_200 = local_188;
  local_1f8[4] = (float)local_16c;
  local_204 = local_17c;
  local_1f8[5] = (float)local_160;
  local_210 = local_1b4;
  local_1f8[6] = (float)local_154;
  local_1fc = local_190;
  local_1dc = local_1a4;
  local_20c = local_184;
  local_1d8 = local_168;
  local_214 = local_178;
  local_1d4 = local_15c;
  local_218 = local_1b0;
  local_1d0 = local_150;
  local_1f8[0] = local_170;
  local_1cc = local_1a0;
  local_1c8 = 0;
  local_1c4 = 0;
  local_1c0 = 0;
  local_1bc = 0x3f800000;
  FUN_14010f0d0(local_1f8);
  pfVar12 = local_1f8 + 2;
  lVar15 = 4;
  pfVar14 = local_118 + 2;
  do {
    fVar1 = pfVar12[-1];
    fVar2 = pfVar12[-2];
    fVar3 = *pfVar12;
    fVar4 = pfVar12[1];
    pfVar12 = pfVar12 + 4;
    fVar17 = fVar4 * 0.0;
    pfVar14[-2] = local_198 * fVar2 + local_208 * fVar1 + fVar3 * local_1fc + fVar17;
    pfVar14[-1] = local_18c * fVar2 + local_200 * fVar1 + fVar3 * local_20c + fVar17;
    *pfVar14 = local_res8[0] * fVar2 + local_204 * fVar1 + fVar3 * local_214 + fVar17;
    pfVar14[1] = (float)local_res20 * fVar2 + local_210 * fVar1 + fVar3 * local_218 + fVar4;
    lVar15 = lVar15 + -1;
    pfVar14 = pfVar14 + 4;
  } while (lVar15 != 0);
  FUN_140131ae0(local_148);
  FUN_140131ae0(&local_140);
  FUN_140131ae0(&local_138);
  uVar9 = local_res10;
  local_130 = 0x7149f2ca;
  local_12c = 0x7149f2ca;
  local_128 = 0;
  local_140 = *(undefined8 *)(lVar7 + 0x98);
  local_138 = *param_4;
  FUN_140755870(*(undefined8 *)(param_1 + 0xe8),local_res10,local_148);
  lVar7 = *(longlong *)(*(longlong *)(param_1 + 0xe8) + 0x20);
  lVar15 = *(longlong *)(*(longlong *)(lVar10 + 0x1fe8) + 0x50);
  if (lVar15 == 0) {
    local_res8[0] = (float)CONCAT22(local_res8[0]._2_2_,0xffff);
    pfVar12 = local_res8;
  }
  else {
    pfVar12 = (float *)FUN_140256000(lVar15,local_res8,uVar5);
  }
  *(undefined2 *)(lVar7 + 0x8c) = *(undefined2 *)pfVar12;
  *(float *)(lVar7 + 0x70) = local_118[3];
  *(undefined4 *)(lVar7 + 0x74) = local_fc;
  *(undefined4 *)(lVar7 + 0x78) = local_ec;
  local_1f8[0] = local_118[0];
  local_1f8[3] = local_118[1];
  local_1f8[6] = local_118[2];
  local_1f8[1] = local_118[4];
  local_1f8[4] = local_118[5];
  local_1dc = local_118[6];
  local_1f8[2] = (float)local_f8;
  local_1f8[5] = (float)local_f4;
  local_1d8 = local_f0;
  puVar13 = (undefined4 *)FUN_140110080(local_1f8,local_228);
  *(undefined4 *)(lVar7 + 0x7c) = *puVar13;
  *(undefined4 *)(lVar7 + 0x80) = puVar13[1];
  *(undefined4 *)(lVar7 + 0x84) = puVar13[2];
  local_228[0] = 0;
  *(undefined4 *)(lVar7 + 0x88) = puVar13[3];
  lVar7 = *(longlong *)(param_1 + 0xe8);
  local_220 = 7;
  local_21c = CONCAT13(local_21c._3_1_,0x100);
  *(undefined4 *)(lVar7 + 0x34) = 0;
  *(undefined4 *)(lVar7 + 0x38) = 0;
  *(undefined4 *)(lVar7 + 0x3c) = 7;
  *(undefined4 *)(lVar7 + 0x40) = local_21c;
  FUN_140757830(*(undefined8 *)(param_1 + 0xe8));
  FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x10)
                ,uVar9);
  return;
}


````

### result

````text

void FUN_140828e80(longlong param_1)

{
  longlong lVar1;
  
  lVar1 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));
  if (lVar1 != 0) {
    FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar1);
  }
  FUN_140757550(*(undefined8 *)(param_1 + 0xe8));
  return;
}


````

### result

````text

void FUN_140757c10(longlong param_1,undefined8 *param_2)

{
  byte *pbVar1;
  char cVar2;
  longlong lVar3;
  
  lVar3 = (**(code **)*param_2)(param_2);
  if ((*(int *)(lVar3 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar3 + 0x60))) {
    lVar3 = FUN_140877720(param_2,&DAT_1439d7510,2);
    if (lVar3 != 0) {
      FUN_1407ff170(lVar3,0);
      if ((*(uint *)(param_1 + 0x3c) >> 2 & 1) != 0) {
        FUN_1407fd970(lVar3);
      }
    }
  }
  else {
    lVar3 = FUN_140877720(param_2,&DAT_1439e2a00,1);
    if (lVar3 != 0) {
      FUN_1405fb960(lVar3);
      cVar2 = FUN_1405f79f0(lVar3);
      if (cVar2 != '\0') {
        FUN_1405faf90(lVar3);
        if (*(int *)(param_1 + 0x34) == 0) {
          pbVar1 = (byte *)(*(longlong *)(*(longlong *)(lVar3 + 0xa8) + 0x98) + 0x130);
          *pbVar1 = *pbVar1 | 1;
          return;
        }
      }
    }
  }
  return;
}


````
