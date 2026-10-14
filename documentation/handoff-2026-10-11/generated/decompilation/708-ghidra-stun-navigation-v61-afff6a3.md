# ghidra-stun-navigation-v61.json

Original: [work/ghidra-stun-navigation-v61.json](<../../../../work/ghidra-stun-navigation-v61.json>)

SHA-256: `E4CDB395982B35CEAF0C70C656E3D2DDDB75315B4A3B49B24AC869DEAD46A7E5`; 17575 bytes.

## Original saved record

````json
{
  "content": [
    {
      "type": "text",
      "text": "\r\n/* WARNING: Removing unreachable block (ram,0x000140869ab1) */\r\n\r\nvoid FUN_1408695c0(longlong *param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  bool bVar5;\r\n  longlong lVar6;\r\n  char cVar7;\r\n  longlong lVar8;\r\n  undefined8 uVar9;\r\n  float *pfVar10;\r\n  longlong lVar11;\r\n  longlong lVar12;\r\n  longlong lVar13;\r\n  bool bVar14;\r\n  float fVar15;\r\n  float fVar16;\r\n  float fVar17;\r\n  float fVar18;\r\n  undefined1 local_res8 [8];\r\n  longlong local_res10;\r\n  undefined8 in_stack_ffffffffffffff58;\r\n  undefined4 uVar19;\r\n  float local_98;\r\n  float local_94;\r\n  float local_90;\r\n  float local_88;\r\n  float local_84;\r\n  float local_80;\r\n  \r\n  uVar19 = (undefined4)((ulonglong)in_stack_ffffffffffffff58 >> 0x20);\r\n  lVar8 = FUN_140854880();\r\n  lVar13 = param_1[2];\r\n  lVar3 = param_1[3];\r\n  lVar11 = *(longlong *)(lVar13 + 0x330);\r\n  local_res10 = *(longlong *)(lVar13 + 0x308);\r\n  if (*(char *)(*(longlong *)(lVar13 + 0x2f8) + 0xdc) == '\\0') {\r\n    fVar15 = 1e+30;\r\n    uVar9 = CONCAT44(uVar19,0x7149f2ca);\r\n  }\r\n  else {\r\n    fVar17 = DAT_143991f5c;\r\n    if (DAT_143991f5c <= DAT_143991fdc) {\r\n      fVar17 = DAT_143991fdc;\r\n    }\r\n    fVar15 = fVar17 * *(float *)(lVar3 + 0x94);\r\n    uVar9 = CONCAT44(uVar19,fVar17 * *(float *)(lVar3 + 0x98));\r\n  }\r\n  FUN_1407ff0a0(local_res10,*(undefined4 *)(lVar3 + 0x8c),*(undefined4 *)(lVar3 + 0x90),fVar15,uVar9\r\n               );\r\n  if ((int)param_1[0xb] < 0) {\r\n    fVar15 = 0.0;\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 + (longlong)(int)param_1[0xb] * 0x28);\r\n  }\r\n  if ((*(longlong *)(*(longlong *)(lVar13 + 0x328) + 0x70) == 0) &&\r\n     (*(longlong *)(*(longlong *)(lVar13 + 0x328) + 0x68) == 0)) {\r\n    bVar14 = false;\r\n  }\r\n  else {\r\n    bVar14 = true;\r\n  }\r\n  if ((0.0 < fVar15) || (!bVar14)) {\r\n    bVar5 = false;\r\n  }\r\n  else {\r\n    bVar5 = true;\r\n  }\r\n  if ((bVar14) || (*(int *)((longlong)param_1 + 0x54) < 0)) {\r\n    fVar15 = 1.0;\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +\r\n                       (longlong)*(int *)((longlong)param_1 + 0x54) * 0x28);\r\n  }\r\n  if (bVar5) {\r\n    fVar17 = 0.0;\r\n  }\r\n  else {\r\n    fVar17 = *(float *)(lVar3 + 0x78) * 0.01;\r\n  }\r\n  uVar2 = *(undefined4 *)(lVar3 + 0x7c);\r\n  uVar19 = *(undefined4 *)(lVar3 + 0x80);\r\n  lVar4 = *(longlong *)(lVar13 + 0x300);\r\n  pfVar10 = &local_98;\r\n  fVar15 = (fVar17 + 1.0) * fVar15;\r\n  uVar1 = *(undefined4 *)(lVar3 + 0x74);\r\n  *(undefined4 *)(lVar4 + 0xd9c) = uVar19;\r\n  *(undefined4 *)(lVar4 + 0xd98) = uVar2;\r\n  *(undefined4 *)(lVar4 + 0xda4) = uVar19;\r\n  *(undefined4 *)(lVar4 + 0xda0) = uVar1;\r\n  (**(code **)(*param_1 + 0xc0))\r\n            (param_1,fVar15 * *(float *)(lVar3 + 0x74) + *(float *)((longlong)param_1 + 0x3c),\r\n             fVar15 * *(float *)(lVar3 + 0x7c),fVar15 * *(float *)(lVar3 + 0x80),pfVar10);\r\n  uVar19 = (undefined4)((ulonglong)pfVar10 >> 0x20);\r\n  if (*(int *)(lVar11 + 0xd8) < 0) {\r\n    fVar15 = 1.0;\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(lVar11 + 0xd8) * 0x28);\r\n  }\r\n  fVar15 = fVar15 * *(float *)(param_1 + 7);\r\n  local_98 = local_98 * fVar15;\r\n  local_94 = local_94 * fVar15;\r\n  local_90 = local_90 * fVar15;\r\n  bVar14 = (((uint)local_98 | (uint)local_94 | (uint)local_90) & 0x7fffffff) != 0;\r\n  if ((bool)(char)param_1[9] != bVar14) {\r\n    if (((-1 < *(int *)((longlong)param_1 + 0x5c)) &&\r\n        (0.0 < *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +\r\n                         (longlong)*(int *)((longlong)param_1 + 0x5c) * 0x28))) ||\r\n       ((-1 < (int)param_1[6] &&\r\n        (0.0 < *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 + (longlong)(int)param_1[6] * 0x28))))\r\n       ) {\r\n      bVar5 = true;\r\n    }\r\n    else {\r\n      bVar5 = false;\r\n    }\r\n    if ((!bVar5) || (bVar14)) {\r\n      if (*(char *)((longlong)param_1 + 0x49) != '\\0') {\r\n        FUN_1405ff2d0(*(undefined8 *)(lVar13 + 0x358));\r\n        *(undefined1 *)((longlong)param_1 + 0x49) = 0;\r\n      }\r\n    }\r\n    else if (*(char *)((longlong)param_1 + 0x49) == '\\0') {\r\n      *(undefined1 *)((longlong)param_1 + 0x49) = 1;\r\n      FUN_1405ff1f0();\r\n    }\r\n    *(bool *)(param_1 + 9) = bVar14;\r\n  }\r\n  uVar9 = FUN_140854880(param_1);\r\n  pfVar10 = (float *)FUN_140877eb0(uVar9,local_res8);\r\n  fVar15 = *pfVar10;\r\n  fVar17 = *(float *)(param_1 + 10);\r\n  *(float *)(param_1 + 10) = fVar15 + fVar17;\r\n  if (1.0 < fVar15 + fVar17) {\r\n    *(undefined4 *)(param_1 + 10) = 0;\r\n  }\r\n  local_88 = *(float *)(lVar8 + 0x348);\r\n  local_84 = *(float *)(lVar8 + 0x34c);\r\n  local_80 = *(float *)(lVar8 + 0x350);\r\n  FUN_1407f04a0(*(undefined8 *)(lVar13 + 0x310),*(undefined4 *)(lVar3 + 0x84),\r\n                *(undefined4 *)(lVar3 + 0x88),&local_88);\r\n  cVar7 = FUN_140803180(lVar13,0x27);\r\n  if (cVar7 != '\\0') {\r\n    lVar11 = 0;\r\n    if (*(longlong *)(lVar13 + 0x90) != 0) {\r\n      lVar11 = *(longlong *)(*(longlong *)(lVar13 + 0x90) + 0x148);\r\n    }\r\n    if (*(char *)(lVar11 + 0xd1) != '\\0') {\r\n      fVar15 = *(float *)(*(longlong *)(lVar11 + 0x18) + 0xa8);\r\n      local_98 = local_98 * fVar15;\r\n      local_94 = local_94 * fVar15;\r\n      local_90 = local_90 * fVar15;\r\n      fVar15 = *(float *)(*(longlong *)(lVar11 + 0x18) + 0xac);\r\n      local_88 = local_88 * fVar15;\r\n      local_80 = local_80 * fVar15;\r\n      local_84 = local_84 * fVar15;\r\n    }\r\n  }\r\n  uVar9 = *(undefined8 *)(lVar13 + 800);\r\n  FUN_140752870(uVar9,\"arkPlayerStateNavigation\",0,&local_98);\r\n  FUN_1407527e0(uVar9,\"arkPlayerStateNavigation\",0,&local_88);\r\n  lVar6 = local_res10;\r\n  lVar13 = *(longlong *)(lVar3 + 200);\r\n  fVar15 = *(float *)(lVar3 + 0xd8);\r\n  lVar11 = *(longlong *)(lVar3 + 0xd0);\r\n  lVar12 = FUN_1407fdaa0(local_res10);\r\n  fVar17 = *(float *)(lVar12 + 0xc) * *(float *)(lVar8 + 0x310) +\r\n           *(float *)(lVar12 + 8) * *(float *)(lVar8 + 0x30c) +\r\n           *(float *)(lVar12 + 0x10) * *(float *)(lVar8 + 0x314);\r\n  if (fVar17 <= 0.34) {\r\n    if (-0.34 <= fVar17) goto LAB_140869a4a;\r\n    fVar15 = *(float *)(lVar3 + 0xf0);\r\n    if (*(longlong *)(lVar3 + 0xe0) != 0) {\r\n      lVar13 = *(longlong *)(lVar3 + 0xe0);\r\n    }\r\n    lVar8 = *(longlong *)(lVar3 + 0xe8);\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(lVar3 + 0x108);\r\n    if (*(longlong *)(lVar3 + 0xf8) != 0) {\r\n      lVar13 = *(longlong *)(lVar3 + 0xf8);\r\n    }\r\n    lVar8 = *(longlong *)(lVar3 + 0x100);\r\n  }\r\n  if (lVar8 != 0) {\r\n    lVar11 = lVar8;\r\n  }\r\nLAB_140869a4a:\r\n  fVar17 = *(float *)(lVar3 + 0x7c);\r\n  if (fVar17 < *(float *)(lVar3 + 0x74)) {\r\n    fVar17 = *(float *)(lVar3 + 0x74);\r\n  }\r\n  if (fVar17 <= *(float *)(lVar3 + 0x80)) {\r\n    fVar17 = *(float *)(lVar3 + 0x80);\r\n  }\r\n  if (0.0 < fVar17) {\r\n    fVar18 = *(float *)(lVar4 + 0xdac);\r\n    if (fVar17 <= *(float *)(lVar4 + 0xdac)) {\r\n      fVar18 = fVar17;\r\n    }\r\n    fVar16 = *(float *)(lVar3 + 0x110);\r\n    if (ABS(fVar16) <= 1e-18) {\r\n      fVar16 = 0.0;\r\n    }\r\n    fVar15 = fVar15 * ((1.0 - fVar18 / fVar17) * fVar16 + (fVar18 / fVar17) * 1.0);\r\n  }\r\n  FUN_140774620(lVar4,lVar13,lVar11,fVar15,CONCAT44(uVar19,(int)param_1[1]));\r\n  FUN_140868ef0(param_1);\r\n  if ((0.0 < *(float *)(lVar3 + 0x148)) && (*(float *)((longlong)param_1 + 0x4c) <= 0.0)) {\r\n    if ((((int)param_1[1] - 6U & 0xfffffffa) == 0) && ((int)param_1[1] != 7)) {\r\n      uVar9 = 1;\r\n    }\r\n    else {\r\n      uVar9 = 0;\r\n    }\r\n    fVar15 = *(float *)(lVar6 + 0x2b0);\r\n    lVar13 = FUN_1407fda90(lVar6);\r\n    if (*(char *)((longlong)param_1 + 0x4a) == '\\0') {\r\n      if (((*(float *)(lVar3 + 0x148) <= fVar15) &&\r\n          (*(float *)(lVar13 + 8) <= -1.0 && *(float *)(lVar13 + 8) != -1.0)) &&\r\n         (*(int *)(lVar4 + 0xdb4) == 0)) {\r\n        FUN_140774f10(lVar4,uVar9,*(undefined4 *)(lVar3 + 0x14c),0);\r\n        *(undefined1 *)((longlong)param_1 + 0x4a) = 1;\r\n      }\r\n    }\r\n    else if (*(int *)(lVar4 + 0xdb4) == 3) {\r\n      FUN_140775270(lVar4,uVar9,0);\r\n      *(undefined1 *)((longlong)param_1 + 0x4a) = 0;\r\n      *(undefined4 *)((longlong)param_1 + 0x4c) = DAT_14399634c;\r\n    }\r\n  }\r\n  uVar9 = FUN_140854880(param_1);\r\n  pfVar10 = (float *)FUN_140877eb0(uVar9,local_res8);\r\n  *(float *)((longlong)param_1 + 0x4c) = *(float *)((longlong)param_1 + 0x4c) - *pfVar10;\r\n  param_1[7] = 0x3f800000;\r\n  return;\r\n}\r\n\r\n"
    }
  ],
  "structuredContent": {
    "result": "\r\n/* WARNING: Removing unreachable block (ram,0x000140869ab1) */\r\n\r\nvoid FUN_1408695c0(longlong *param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  bool bVar5;\r\n  longlong lVar6;\r\n  char cVar7;\r\n  longlong lVar8;\r\n  undefined8 uVar9;\r\n  float *pfVar10;\r\n  longlong lVar11;\r\n  longlong lVar12;\r\n  longlong lVar13;\r\n  bool bVar14;\r\n  float fVar15;\r\n  float fVar16;\r\n  float fVar17;\r\n  float fVar18;\r\n  undefined1 local_res8 [8];\r\n  longlong local_res10;\r\n  undefined8 in_stack_ffffffffffffff58;\r\n  undefined4 uVar19;\r\n  float local_98;\r\n  float local_94;\r\n  float local_90;\r\n  float local_88;\r\n  float local_84;\r\n  float local_80;\r\n  \r\n  uVar19 = (undefined4)((ulonglong)in_stack_ffffffffffffff58 >> 0x20);\r\n  lVar8 = FUN_140854880();\r\n  lVar13 = param_1[2];\r\n  lVar3 = param_1[3];\r\n  lVar11 = *(longlong *)(lVar13 + 0x330);\r\n  local_res10 = *(longlong *)(lVar13 + 0x308);\r\n  if (*(char *)(*(longlong *)(lVar13 + 0x2f8) + 0xdc) == '\\0') {\r\n    fVar15 = 1e+30;\r\n    uVar9 = CONCAT44(uVar19,0x7149f2ca);\r\n  }\r\n  else {\r\n    fVar17 = DAT_143991f5c;\r\n    if (DAT_143991f5c <= DAT_143991fdc) {\r\n      fVar17 = DAT_143991fdc;\r\n    }\r\n    fVar15 = fVar17 * *(float *)(lVar3 + 0x94);\r\n    uVar9 = CONCAT44(uVar19,fVar17 * *(float *)(lVar3 + 0x98));\r\n  }\r\n  FUN_1407ff0a0(local_res10,*(undefined4 *)(lVar3 + 0x8c),*(undefined4 *)(lVar3 + 0x90),fVar15,uVar9\r\n               );\r\n  if ((int)param_1[0xb] < 0) {\r\n    fVar15 = 0.0;\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 + (longlong)(int)param_1[0xb] * 0x28);\r\n  }\r\n  if ((*(longlong *)(*(longlong *)(lVar13 + 0x328) + 0x70) == 0) &&\r\n     (*(longlong *)(*(longlong *)(lVar13 + 0x328) + 0x68) == 0)) {\r\n    bVar14 = false;\r\n  }\r\n  else {\r\n    bVar14 = true;\r\n  }\r\n  if ((0.0 < fVar15) || (!bVar14)) {\r\n    bVar5 = false;\r\n  }\r\n  else {\r\n    bVar5 = true;\r\n  }\r\n  if ((bVar14) || (*(int *)((longlong)param_1 + 0x54) < 0)) {\r\n    fVar15 = 1.0;\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +\r\n                       (longlong)*(int *)((longlong)param_1 + 0x54) * 0x28);\r\n  }\r\n  if (bVar5) {\r\n    fVar17 = 0.0;\r\n  }\r\n  else {\r\n    fVar17 = *(float *)(lVar3 + 0x78) * 0.01;\r\n  }\r\n  uVar2 = *(undefined4 *)(lVar3 + 0x7c);\r\n  uVar19 = *(undefined4 *)(lVar3 + 0x80);\r\n  lVar4 = *(longlong *)(lVar13 + 0x300);\r\n  pfVar10 = &local_98;\r\n  fVar15 = (fVar17 + 1.0) * fVar15;\r\n  uVar1 = *(undefined4 *)(lVar3 + 0x74);\r\n  *(undefined4 *)(lVar4 + 0xd9c) = uVar19;\r\n  *(undefined4 *)(lVar4 + 0xd98) = uVar2;\r\n  *(undefined4 *)(lVar4 + 0xda4) = uVar19;\r\n  *(undefined4 *)(lVar4 + 0xda0) = uVar1;\r\n  (**(code **)(*param_1 + 0xc0))\r\n            (param_1,fVar15 * *(float *)(lVar3 + 0x74) + *(float *)((longlong)param_1 + 0x3c),\r\n             fVar15 * *(float *)(lVar3 + 0x7c),fVar15 * *(float *)(lVar3 + 0x80),pfVar10);\r\n  uVar19 = (undefined4)((ulonglong)pfVar10 >> 0x20);\r\n  if (*(int *)(lVar11 + 0xd8) < 0) {\r\n    fVar15 = 1.0;\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(lVar11 + 0xd8) * 0x28);\r\n  }\r\n  fVar15 = fVar15 * *(float *)(param_1 + 7);\r\n  local_98 = local_98 * fVar15;\r\n  local_94 = local_94 * fVar15;\r\n  local_90 = local_90 * fVar15;\r\n  bVar14 = (((uint)local_98 | (uint)local_94 | (uint)local_90) & 0x7fffffff) != 0;\r\n  if ((bool)(char)param_1[9] != bVar14) {\r\n    if (((-1 < *(int *)((longlong)param_1 + 0x5c)) &&\r\n        (0.0 < *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +\r\n                         (longlong)*(int *)((longlong)param_1 + 0x5c) * 0x28))) ||\r\n       ((-1 < (int)param_1[6] &&\r\n        (0.0 < *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 + (longlong)(int)param_1[6] * 0x28))))\r\n       ) {\r\n      bVar5 = true;\r\n    }\r\n    else {\r\n      bVar5 = false;\r\n    }\r\n    if ((!bVar5) || (bVar14)) {\r\n      if (*(char *)((longlong)param_1 + 0x49) != '\\0') {\r\n        FUN_1405ff2d0(*(undefined8 *)(lVar13 + 0x358));\r\n        *(undefined1 *)((longlong)param_1 + 0x49) = 0;\r\n      }\r\n    }\r\n    else if (*(char *)((longlong)param_1 + 0x49) == '\\0') {\r\n      *(undefined1 *)((longlong)param_1 + 0x49) = 1;\r\n      FUN_1405ff1f0();\r\n    }\r\n    *(bool *)(param_1 + 9) = bVar14;\r\n  }\r\n  uVar9 = FUN_140854880(param_1);\r\n  pfVar10 = (float *)FUN_140877eb0(uVar9,local_res8);\r\n  fVar15 = *pfVar10;\r\n  fVar17 = *(float *)(param_1 + 10);\r\n  *(float *)(param_1 + 10) = fVar15 + fVar17;\r\n  if (1.0 < fVar15 + fVar17) {\r\n    *(undefined4 *)(param_1 + 10) = 0;\r\n  }\r\n  local_88 = *(float *)(lVar8 + 0x348);\r\n  local_84 = *(float *)(lVar8 + 0x34c);\r\n  local_80 = *(float *)(lVar8 + 0x350);\r\n  FUN_1407f04a0(*(undefined8 *)(lVar13 + 0x310),*(undefined4 *)(lVar3 + 0x84),\r\n                *(undefined4 *)(lVar3 + 0x88),&local_88);\r\n  cVar7 = FUN_140803180(lVar13,0x27);\r\n  if (cVar7 != '\\0') {\r\n    lVar11 = 0;\r\n    if (*(longlong *)(lVar13 + 0x90) != 0) {\r\n      lVar11 = *(longlong *)(*(longlong *)(lVar13 + 0x90) + 0x148);\r\n    }\r\n    if (*(char *)(lVar11 + 0xd1) != '\\0') {\r\n      fVar15 = *(float *)(*(longlong *)(lVar11 + 0x18) + 0xa8);\r\n      local_98 = local_98 * fVar15;\r\n      local_94 = local_94 * fVar15;\r\n      local_90 = local_90 * fVar15;\r\n      fVar15 = *(float *)(*(longlong *)(lVar11 + 0x18) + 0xac);\r\n      local_88 = local_88 * fVar15;\r\n      local_80 = local_80 * fVar15;\r\n      local_84 = local_84 * fVar15;\r\n    }\r\n  }\r\n  uVar9 = *(undefined8 *)(lVar13 + 800);\r\n  FUN_140752870(uVar9,\"arkPlayerStateNavigation\",0,&local_98);\r\n  FUN_1407527e0(uVar9,\"arkPlayerStateNavigation\",0,&local_88);\r\n  lVar6 = local_res10;\r\n  lVar13 = *(longlong *)(lVar3 + 200);\r\n  fVar15 = *(float *)(lVar3 + 0xd8);\r\n  lVar11 = *(longlong *)(lVar3 + 0xd0);\r\n  lVar12 = FUN_1407fdaa0(local_res10);\r\n  fVar17 = *(float *)(lVar12 + 0xc) * *(float *)(lVar8 + 0x310) +\r\n           *(float *)(lVar12 + 8) * *(float *)(lVar8 + 0x30c) +\r\n           *(float *)(lVar12 + 0x10) * *(float *)(lVar8 + 0x314);\r\n  if (fVar17 <= 0.34) {\r\n    if (-0.34 <= fVar17) goto LAB_140869a4a;\r\n    fVar15 = *(float *)(lVar3 + 0xf0);\r\n    if (*(longlong *)(lVar3 + 0xe0) != 0) {\r\n      lVar13 = *(longlong *)(lVar3 + 0xe0);\r\n    }\r\n    lVar8 = *(longlong *)(lVar3 + 0xe8);\r\n  }\r\n  else {\r\n    fVar15 = *(float *)(lVar3 + 0x108);\r\n    if (*(longlong *)(lVar3 + 0xf8) != 0) {\r\n      lVar13 = *(longlong *)(lVar3 + 0xf8);\r\n    }\r\n    lVar8 = *(longlong *)(lVar3 + 0x100);\r\n  }\r\n  if (lVar8 != 0) {\r\n    lVar11 = lVar8;\r\n  }\r\nLAB_140869a4a:\r\n  fVar17 = *(float *)(lVar3 + 0x7c);\r\n  if (fVar17 < *(float *)(lVar3 + 0x74)) {\r\n    fVar17 = *(float *)(lVar3 + 0x74);\r\n  }\r\n  if (fVar17 <= *(float *)(lVar3 + 0x80)) {\r\n    fVar17 = *(float *)(lVar3 + 0x80);\r\n  }\r\n  if (0.0 < fVar17) {\r\n    fVar18 = *(float *)(lVar4 + 0xdac);\r\n    if (fVar17 <= *(float *)(lVar4 + 0xdac)) {\r\n      fVar18 = fVar17;\r\n    }\r\n    fVar16 = *(float *)(lVar3 + 0x110);\r\n    if (ABS(fVar16) <= 1e-18) {\r\n      fVar16 = 0.0;\r\n    }\r\n    fVar15 = fVar15 * ((1.0 - fVar18 / fVar17) * fVar16 + (fVar18 / fVar17) * 1.0);\r\n  }\r\n  FUN_140774620(lVar4,lVar13,lVar11,fVar15,CONCAT44(uVar19,(int)param_1[1]));\r\n  FUN_140868ef0(param_1);\r\n  if ((0.0 < *(float *)(lVar3 + 0x148)) && (*(float *)((longlong)param_1 + 0x4c) <= 0.0)) {\r\n    if ((((int)param_1[1] - 6U & 0xfffffffa) == 0) && ((int)param_1[1] != 7)) {\r\n      uVar9 = 1;\r\n    }\r\n    else {\r\n      uVar9 = 0;\r\n    }\r\n    fVar15 = *(float *)(lVar6 + 0x2b0);\r\n    lVar13 = FUN_1407fda90(lVar6);\r\n    if (*(char *)((longlong)param_1 + 0x4a) == '\\0') {\r\n      if (((*(float *)(lVar3 + 0x148) <= fVar15) &&\r\n          (*(float *)(lVar13 + 8) <= -1.0 && *(float *)(lVar13 + 8) != -1.0)) &&\r\n         (*(int *)(lVar4 + 0xdb4) == 0)) {\r\n        FUN_140774f10(lVar4,uVar9,*(undefined4 *)(lVar3 + 0x14c),0);\r\n        *(undefined1 *)((longlong)param_1 + 0x4a) = 1;\r\n      }\r\n    }\r\n    else if (*(int *)(lVar4 + 0xdb4) == 3) {\r\n      FUN_140775270(lVar4,uVar9,0);\r\n      *(undefined1 *)((longlong)param_1 + 0x4a) = 0;\r\n      *(undefined4 *)((longlong)param_1 + 0x4c) = DAT_14399634c;\r\n    }\r\n  }\r\n  uVar9 = FUN_140854880(param_1);\r\n  pfVar10 = (float *)FUN_140877eb0(uVar9,local_res8);\r\n  *(float *)((longlong)param_1 + 0x4c) = *(float *)((longlong)param_1 + 0x4c) - *pfVar10;\r\n  param_1[7] = 0x3f800000;\r\n  return;\r\n}\r\n\r\n"
  },
  "isError": false
}

````

## Unescaped text fields for reading

### text

````text

/* WARNING: Removing unreachable block (ram,0x000140869ab1) */

void FUN_1408695c0(longlong *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  longlong lVar3;
  longlong lVar4;
  bool bVar5;
  longlong lVar6;
  char cVar7;
  longlong lVar8;
  undefined8 uVar9;
  float *pfVar10;
  longlong lVar11;
  longlong lVar12;
  longlong lVar13;
  bool bVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  undefined1 local_res8 [8];
  longlong local_res10;
  undefined8 in_stack_ffffffffffffff58;
  undefined4 uVar19;
  float local_98;
  float local_94;
  float local_90;
  float local_88;
  float local_84;
  float local_80;
  
  uVar19 = (undefined4)((ulonglong)in_stack_ffffffffffffff58 >> 0x20);
  lVar8 = FUN_140854880();
  lVar13 = param_1[2];
  lVar3 = param_1[3];
  lVar11 = *(longlong *)(lVar13 + 0x330);
  local_res10 = *(longlong *)(lVar13 + 0x308);
  if (*(char *)(*(longlong *)(lVar13 + 0x2f8) + 0xdc) == '\0') {
    fVar15 = 1e+30;
    uVar9 = CONCAT44(uVar19,0x7149f2ca);
  }
  else {
    fVar17 = DAT_143991f5c;
    if (DAT_143991f5c <= DAT_143991fdc) {
      fVar17 = DAT_143991fdc;
    }
    fVar15 = fVar17 * *(float *)(lVar3 + 0x94);
    uVar9 = CONCAT44(uVar19,fVar17 * *(float *)(lVar3 + 0x98));
  }
  FUN_1407ff0a0(local_res10,*(undefined4 *)(lVar3 + 0x8c),*(undefined4 *)(lVar3 + 0x90),fVar15,uVar9
               );
  if ((int)param_1[0xb] < 0) {
    fVar15 = 0.0;
  }
  else {
    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 + (longlong)(int)param_1[0xb] * 0x28);
  }
  if ((*(longlong *)(*(longlong *)(lVar13 + 0x328) + 0x70) == 0) &&
     (*(longlong *)(*(longlong *)(lVar13 + 0x328) + 0x68) == 0)) {
    bVar14 = false;
  }
  else {
    bVar14 = true;
  }
  if ((0.0 < fVar15) || (!bVar14)) {
    bVar5 = false;
  }
  else {
    bVar5 = true;
  }
  if ((bVar14) || (*(int *)((longlong)param_1 + 0x54) < 0)) {
    fVar15 = 1.0;
  }
  else {
    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +
                       (longlong)*(int *)((longlong)param_1 + 0x54) * 0x28);
  }
  if (bVar5) {
    fVar17 = 0.0;
  }
  else {
    fVar17 = *(float *)(lVar3 + 0x78) * 0.01;
  }
  uVar2 = *(undefined4 *)(lVar3 + 0x7c);
  uVar19 = *(undefined4 *)(lVar3 + 0x80);
  lVar4 = *(longlong *)(lVar13 + 0x300);
  pfVar10 = &local_98;
  fVar15 = (fVar17 + 1.0) * fVar15;
  uVar1 = *(undefined4 *)(lVar3 + 0x74);
  *(undefined4 *)(lVar4 + 0xd9c) = uVar19;
  *(undefined4 *)(lVar4 + 0xd98) = uVar2;
  *(undefined4 *)(lVar4 + 0xda4) = uVar19;
  *(undefined4 *)(lVar4 + 0xda0) = uVar1;
  (**(code **)(*param_1 + 0xc0))
            (param_1,fVar15 * *(float *)(lVar3 + 0x74) + *(float *)((longlong)param_1 + 0x3c),
             fVar15 * *(float *)(lVar3 + 0x7c),fVar15 * *(float *)(lVar3 + 0x80),pfVar10);
  uVar19 = (undefined4)((ulonglong)pfVar10 >> 0x20);
  if (*(int *)(lVar11 + 0xd8) < 0) {
    fVar15 = 1.0;
  }
  else {
    fVar15 = *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +
                       (longlong)*(int *)(lVar11 + 0xd8) * 0x28);
  }
  fVar15 = fVar15 * *(float *)(param_1 + 7);
  local_98 = local_98 * fVar15;
  local_94 = local_94 * fVar15;
  local_90 = local_90 * fVar15;
  bVar14 = (((uint)local_98 | (uint)local_94 | (uint)local_90) & 0x7fffffff) != 0;
  if ((bool)(char)param_1[9] != bVar14) {
    if (((-1 < *(int *)((longlong)param_1 + 0x5c)) &&
        (0.0 < *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 +
                         (longlong)*(int *)((longlong)param_1 + 0x5c) * 0x28))) ||
       ((-1 < (int)param_1[6] &&
        (0.0 < *(float *)(*(longlong *)(lVar11 + 0x48) + 0x24 + (longlong)(int)param_1[6] * 0x28))))
       ) {
      bVar5 = true;
    }
    else {
      bVar5 = false;
    }
    if ((!bVar5) || (bVar14)) {
      if (*(char *)((longlong)param_1 + 0x49) != '\0') {
        FUN_1405ff2d0(*(undefined8 *)(lVar13 + 0x358));
        *(undefined1 *)((longlong)param_1 + 0x49) = 0;
      }
    }
    else if (*(char *)((longlong)param_1 + 0x49) == '\0') {
      *(undefined1 *)((longlong)param_1 + 0x49) = 1;
      FUN_1405ff1f0();
    }
    *(bool *)(param_1 + 9) = bVar14;
  }
  uVar9 = FUN_140854880(param_1);
  pfVar10 = (float *)FUN_140877eb0(uVar9,local_res8);
  fVar15 = *pfVar10;
  fVar17 = *(float *)(param_1 + 10);
  *(float *)(param_1 + 10) = fVar15 + fVar17;
  if (1.0 < fVar15 + fVar17) {
    *(undefined4 *)(param_1 + 10) = 0;
  }
  local_88 = *(float *)(lVar8 + 0x348);
  local_84 = *(float *)(lVar8 + 0x34c);
  local_80 = *(float *)(lVar8 + 0x350);
  FUN_1407f04a0(*(undefined8 *)(lVar13 + 0x310),*(undefined4 *)(lVar3 + 0x84),
                *(undefined4 *)(lVar3 + 0x88),&local_88);
  cVar7 = FUN_140803180(lVar13,0x27);
  if (cVar7 != '\0') {
    lVar11 = 0;
    if (*(longlong *)(lVar13 + 0x90) != 0) {
      lVar11 = *(longlong *)(*(longlong *)(lVar13 + 0x90) + 0x148);
    }
    if (*(char *)(lVar11 + 0xd1) != '\0') {
      fVar15 = *(float *)(*(longlong *)(lVar11 + 0x18) + 0xa8);
      local_98 = local_98 * fVar15;
      local_94 = local_94 * fVar15;
      local_90 = local_90 * fVar15;
      fVar15 = *(float *)(*(longlong *)(lVar11 + 0x18) + 0xac);
      local_88 = local_88 * fVar15;
      local_80 = local_80 * fVar15;
      local_84 = local_84 * fVar15;
    }
  }
  uVar9 = *(undefined8 *)(lVar13 + 800);
  FUN_140752870(uVar9,"arkPlayerStateNavigation",0,&local_98);
  FUN_1407527e0(uVar9,"arkPlayerStateNavigation",0,&local_88);
  lVar6 = local_res10;
  lVar13 = *(longlong *)(lVar3 + 200);
  fVar15 = *(float *)(lVar3 + 0xd8);
  lVar11 = *(longlong *)(lVar3 + 0xd0);
  lVar12 = FUN_1407fdaa0(local_res10);
  fVar17 = *(float *)(lVar12 + 0xc) * *(float *)(lVar8 + 0x310) +
           *(float *)(lVar12 + 8) * *(float *)(lVar8 + 0x30c) +
           *(float *)(lVar12 + 0x10) * *(float *)(lVar8 + 0x314);
  if (fVar17 <= 0.34) {
    if (-0.34 <= fVar17) goto LAB_140869a4a;
    fVar15 = *(float *)(lVar3 + 0xf0);
    if (*(longlong *)(lVar3 + 0xe0) != 0) {
      lVar13 = *(longlong *)(lVar3 + 0xe0);
    }
    lVar8 = *(longlong *)(lVar3 + 0xe8);
  }
  else {
    fVar15 = *(float *)(lVar3 + 0x108);
    if (*(longlong *)(lVar3 + 0xf8) != 0) {
      lVar13 = *(longlong *)(lVar3 + 0xf8);
    }
    lVar8 = *(longlong *)(lVar3 + 0x100);
  }
  if (lVar8 != 0) {
    lVar11 = lVar8;
  }
LAB_140869a4a:
  fVar17 = *(float *)(lVar3 + 0x7c);
  if (fVar17 < *(float *)(lVar3 + 0x74)) {
    fVar17 = *(float *)(lVar3 + 0x74);
  }
  if (fVar17 <= *(float *)(lVar3 + 0x80)) {
    fVar17 = *(float *)(lVar3 + 0x80);
  }
  if (0.0 < fVar17) {
    fVar18 = *(float *)(lVar4 + 0xdac);
    if (fVar17 <= *(float *)(lVar4 + 0xdac)) {
      fVar18 = fVar17;
    }
    fVar16 = *(float *)(lVar3 + 0x110);
    if (ABS(fVar16) <= 1e-18) {
      fVar16 = 0.0;
    }
    fVar15 = fVar15 * ((1.0 - fVar18 / fVar17) * fVar16 + (fVar18 / fVar17) * 1.0);
  }
  FUN_140774620(lVar4,lVar13,lVar11,fVar15,CONCAT44(uVar19,(int)param_1[1]));
  FUN_140868ef0(param_1);
  if ((0.0 < *(float *)(lVar3 + 0x148)) && (*(float *)((longlong)param_1 + 0x4c) <= 0.0)) {
    if ((((int)param_1[1] - 6U & 0xfffffffa) == 0) && ((int)param_1[1] != 7)) {
      uVar9 = 1;
    }
    else {
      uVar9 = 0;
    }
    fVar15 = *(float *)(lVar6 + 0x2b0);
    lVar13 = FUN_1407fda90(lVar6);
    if (*(char *)((longlong)param_1 + 0x4a) == '\0') {
      if (((*(float *)(lVar3 + 0x148) <= fVar15) &&
          (*(float *)(lVar13 + 8) <= -1.0 && *(float *)(lVar13 + 8) != -1.0)) &&
         (*(int *)(lVar4 + 0xdb4) == 0)) {
        FUN_140774f10(lVar4,uVar9,*(undefined4 *)(lVar3 + 0x14c),0);
        *(undefined1 *)((longlong)param_1 + 0x4a) = 1;
      }
    }
    else if (*(int *)(lVar4 + 0xdb4) == 3) {
      FUN_140775270(lVar4,uVar9,0);
      *(undefined1 *)((longlong)param_1 + 0x4a) = 0;
      *(undefined4 *)((longlong)param_1 + 0x4c) = DAT_14399634c;
    }
  }
  uVar9 = FUN_140854880(param_1);
  pfVar10 = (float *)FUN_140877eb0(uVar9,local_res8);
  *(float *)((longlong)param_1 + 0x4c) = *(float *)((longlong)param_1 + 0x4c) - *pfVar10;
  param_1[7] = 0x3f800000;
  return;
}


````
