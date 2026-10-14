# ghidra-pickup-continuation-v786.json

Original: [work/natural-moveset/native-pickup-research-v785/ghidra-pickup-continuation-v786.json](<../../../../work/natural-moveset/native-pickup-research-v785/ghidra-pickup-continuation-v786.json>)

SHA-256: `720B32E2A2A1BAECBD546FE7A09F74B9B6680CA8020CD3011B306CAE90A5B069`; 105488 bytes.

## Original saved record

````json
{
  "pickup785-decompile-14076e2b0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14076e2b0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  longlong *plVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  \r\n  lVar6 = *(longlong *)(param_1 + 0x10);\r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  lVar3 = FUN_140877530(lVar6);\r\n  if (lVar3 != 0) {\r\n    FUN_1408762b0(lVar3,lVar6);\r\n    *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(lVar3 + 0xa8);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439d77e0,1);\r\n  if (lVar3 != 0) {\r\n    FUN_1406274d0(lVar3);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439e28e0,1);\r\n  if (lVar3 != 0) {\r\n    FUN_140491550(lVar3);\r\n  }\r\n  plVar4 = (longlong *)FUN_140877720(lVar6,&DAT_1439aee80,1);\r\n  if (plVar4 != (longlong *)0x0) {\r\n    (**(code **)(*plVar4 + 0x1a8))(plVar4);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439bfba0,2);\r\n  if (lVar3 != 0) {\r\n    FUN_14043a670(lVar3);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439c0140,2);\r\n  if (lVar3 != 0) {\r\n    FUN_14044a5f0(lVar3);\r\n  }\r\n  lVar3 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);\r\n  if (((lVar3 != 0) && (iVar1 = *(int *)(lVar3 + 0x60), (iVar1 - 5U & 0xfffffffc) == 0)) &&\r\n     (iVar1 != 6)) {\r\n    lVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n    if (lVar3 != 0) {\r\n      uVar5 = FUN_140877720(lVar3,&DAT_1439e86d0,0);\r\n      uVar5 = FUN_140777b90(uVar5,0x14);\r\n      FUN_1403c4af0(uVar5,*(undefined8 *)(param_1 + 0x10));\r\n    }\r\n    FUN_1404e3050(param_1 + 0x80);\r\n    FUN_1404e3050(param_1 + 0xa8);\r\n  }\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfb | 2;\r\n  FUN_1407e79a0(param_1);\r\n  lVar2 = *(longlong *)(lVar2 + 0xf0);\r\n  if (lVar2 != 0) {\r\n    iVar1 = *(int *)(lVar6 + 0xa8);\r\n    lVar6 = FUN_1403da910(param_1 + 0x40,2);\r\n    if (lVar6 != 0) {\r\n      FUN_140709110(lVar6,0xffffffff,lVar2,0,0,(longlong)iVar1,1,0);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14076e2b0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  longlong *plVar4;\r\n  undefined8 uVar5;\r\n  longlong lVar6;\r\n  \r\n  lVar6 = *(longlong *)(param_1 + 0x10);\r\n  lVar2 = *(longlong *)(param_1 + 8);\r\n  lVar3 = FUN_140877530(lVar6);\r\n  if (lVar3 != 0) {\r\n    FUN_1408762b0(lVar3,lVar6);\r\n    *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(lVar3 + 0xa8);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439d77e0,1);\r\n  if (lVar3 != 0) {\r\n    FUN_1406274d0(lVar3);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439e28e0,1);\r\n  if (lVar3 != 0) {\r\n    FUN_140491550(lVar3);\r\n  }\r\n  plVar4 = (longlong *)FUN_140877720(lVar6,&DAT_1439aee80,1);\r\n  if (plVar4 != (longlong *)0x0) {\r\n    (**(code **)(*plVar4 + 0x1a8))(plVar4);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439bfba0,2);\r\n  if (lVar3 != 0) {\r\n    FUN_14043a670(lVar3);\r\n  }\r\n  lVar3 = FUN_140877720(lVar6,&DAT_1439c0140,2);\r\n  if (lVar3 != 0) {\r\n    FUN_14044a5f0(lVar3);\r\n  }\r\n  lVar3 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);\r\n  if (((lVar3 != 0) && (iVar1 = *(int *)(lVar3 + 0x60), (iVar1 - 5U & 0xfffffffc) == 0)) &&\r\n     (iVar1 != 6)) {\r\n    lVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);\r\n    if (lVar3 != 0) {\r\n      uVar5 = FUN_140877720(lVar3,&DAT_1439e86d0,0);\r\n      uVar5 = FUN_140777b90(uVar5,0x14);\r\n      FUN_1403c4af0(uVar5,*(undefined8 *)(param_1 + 0x10));\r\n    }\r\n    FUN_1404e3050(param_1 + 0x80);\r\n    FUN_1404e3050(param_1 + 0xa8);\r\n  }\r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfb | 2;\r\n  FUN_1407e79a0(param_1);\r\n  lVar2 = *(longlong *)(lVar2 + 0xf0);\r\n  if (lVar2 != 0) {\r\n    iVar1 = *(int *)(lVar6 + 0xa8);\r\n    lVar6 = FUN_1403da910(param_1 + 0x40,2);\r\n    if (lVar6 != 0) {\r\n      FUN_140709110(lVar6,0xffffffff,lVar2,0,0,(longlong)iVar1,1,0);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup785-decompile-14076eeb0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14076eeb0(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong *plVar2;\r\n  undefined8 uVar3;\r\n  undefined1 local_18 [16];\r\n  \r\n  lVar1 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439dcb80,2);\r\n  if (lVar1 != 0) {\r\n    FUN_14070b320(lVar1,0x3f733333);\r\n  }\r\n  if (((*(byte *)(param_1 + 0xd0) & 1) == 0) &&\r\n     (plVar2 = (longlong *)FUN_1403da910(param_1 + 0x50,2), plVar2 != (longlong *)0x0)) {\r\n    lVar1 = *plVar2;\r\n    uVar3 = FUN_140088f70(local_18);\r\n    (**(code **)(lVar1 + 0x108))(plVar2,0x17,uVar3);\r\n    (**(code **)(*plVar2 + 0x188))(plVar2,1);\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 1;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14076eeb0(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong *plVar2;\r\n  undefined8 uVar3;\r\n  undefined1 local_18 [16];\r\n  \r\n  lVar1 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439dcb80,2);\r\n  if (lVar1 != 0) {\r\n    FUN_14070b320(lVar1,0x3f733333);\r\n  }\r\n  if (((*(byte *)(param_1 + 0xd0) & 1) == 0) &&\r\n     (plVar2 = (longlong *)FUN_1403da910(param_1 + 0x50,2), plVar2 != (longlong *)0x0)) {\r\n    lVar1 = *plVar2;\r\n    uVar3 = FUN_140088f70(local_18);\r\n    (**(code **)(lVar1 + 0x108))(plVar2,0x17,uVar3);\r\n    (**(code **)(*plVar2 + 0x188))(plVar2,1);\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 1;\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup785-decompile-14076efa0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14076efa0(longlong param_1)\r\n\r\n{\r\n  longlong *plVar1;\r\n  \r\n  if ((((*(byte *)(param_1 + 0xd0) & 4) == 0) &&\r\n      (plVar1 = (longlong *)FUN_1403da910(param_1 + 0x50,2), plVar1 != (longlong *)0x0)) &&\r\n     (FUN_14076eeb0(param_1), (*(byte *)(param_1 + 0xd0) & 1) != 0)) {\r\n    (**(code **)(*plVar1 + 0x130))(plVar1);\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 4;\r\n  }\r\n  if (((*(byte *)(param_1 + 0xd0) & 2) == 0) && ((*(byte *)(param_1 + 0xd0) & 8) == 0)) {\r\n    D2_Component_Pause(param_1,*(undefined8 *)(D2_GameSingletonPtr + 0x1e8960));\r\n    return;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14076efa0(longlong param_1)\r\n\r\n{\r\n  longlong *plVar1;\r\n  \r\n  if ((((*(byte *)(param_1 + 0xd0) & 4) == 0) &&\r\n      (plVar1 = (longlong *)FUN_1403da910(param_1 + 0x50,2), plVar1 != (longlong *)0x0)) &&\r\n     (FUN_14076eeb0(param_1), (*(byte *)(param_1 + 0xd0) & 1) != 0)) {\r\n    (**(code **)(*plVar1 + 0x130))(plVar1);\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 4;\r\n  }\r\n  if (((*(byte *)(param_1 + 0xd0) & 2) == 0) && ((*(byte *)(param_1 + 0xd0) & 8) == 0)) {\r\n    D2_Component_Pause(param_1,*(undefined8 *)(D2_GameSingletonPtr + 0x1e8960));\r\n    return;\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup785-decompile-14076ee70": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14076ee70(longlong *param_1,longlong param_2)\r\n\r\n{\r\n  FUN_1403dab80(param_1 + 6);\r\n  if (7 < ((byte)*(undefined4 *)((longlong)param_1 + 0x1c) & 0xc)) {\r\n    (**(code **)(*param_1 + 0xe8))(param_1,param_2 == 0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14076ee70(longlong *param_1,longlong param_2)\r\n\r\n{\r\n  FUN_1403dab80(param_1 + 6);\r\n  if (7 < ((byte)*(undefined4 *)((longlong)param_1 + 0x1c) & 0xc)) {\r\n    (**(code **)(*param_1 + 0xe8))(param_1,param_2 == 0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup785-decompile-14076ef60": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14076ef60(longlong param_1)\r\n\r\n{\r\n  undefined *puStack_28;\r\n  undefined *puStack_20;\r\n  undefined1 uStack_18;\r\n  \r\n  FUN_140706f30(param_1,&DAT_1439e85b0,&UNK_141c5a528);\r\n  puStack_28 = &DAT_1439d15f0;\r\n  puStack_20 = &UNK_141c5a568;\r\n  uStack_18 = 0;\r\n  FUN_140706fa0(param_1 + 0x28,&puStack_28);\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14076ef60(longlong param_1)\r\n\r\n{\r\n  undefined *puStack_28;\r\n  undefined *puStack_20;\r\n  undefined1 uStack_18;\r\n  \r\n  FUN_140706f30(param_1,&DAT_1439e85b0,&UNK_141c5a528);\r\n  puStack_28 = &DAT_1439d15f0;\r\n  puStack_20 = &UNK_141c5a568;\r\n  uStack_18 = 0;\r\n  FUN_140706fa0(param_1 + 0x28,&puStack_28);\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup785-decompile-14083f590": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14083f590(longlong param_1,longlong param_2)\r\n\r\n{\r\n  bool bVar1;\r\n  bool bVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  bool bVar5;\r\n  bool bVar6;\r\n  undefined4 *puVar7;\r\n  char cVar8;\r\n  byte bVar9;\r\n  char cVar10;\r\n  undefined8 *puVar11;\r\n  longlong lVar12;\r\n  undefined8 uVar13;\r\n  longlong lVar14;\r\n  undefined1 *puVar15;\r\n  longlong lVar16;\r\n  undefined8 uVar17;\r\n  longlong *plVar18;\r\n  uint *puVar19;\r\n  uint uVar20;\r\n  int iVar21;\r\n  float fVar22;\r\n  float fVar23;\r\n  float fVar24;\r\n  uint local_res18 [2];\r\n  char local_res20;\r\n  uint local_100;\r\n  undefined4 *local_f8;\r\n  uint local_f0;\r\n  uint local_ec;\r\n  undefined1 *local_e8;\r\n  undefined4 local_e0 [2];\r\n  ulonglong local_d8;\r\n  undefined4 *local_d0;\r\n  longlong local_c8;\r\n  longlong local_c0;\r\n  longlong local_b8;\r\n  undefined1 local_b0 [8];\r\n  uint local_a8;\r\n  undefined1 local_98 [8];\r\n  undefined4 local_90;\r\n  undefined8 local_80;\r\n  undefined1 local_78 [56];\r\n  \r\n  local_80 = 0xfffffffffffffffe;\r\n  FUN_1409ffc00();\r\n  puVar11 = (undefined8 *)FUN_1403da910(param_2 + 8,2);\r\n  if (puVar11[2] == 0) {\r\n    *(undefined4 *)(param_1 + 0x130) = 0x3fff0010;\r\n  }\r\n  else {\r\n    *(undefined4 *)(param_1 + 0x130) = *(undefined4 *)(puVar11[2] + 0xa8);\r\n  }\r\n  *(byte *)(param_1 + 0x140) =\r\n       (*(byte *)(param_2 + 0x41) & 1) * '\\x02' | *(byte *)(param_1 + 0x140) & 0xfc;\r\n  bVar6 = false;\r\n  lVar12 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));\r\n  if (((*(byte *)(lVar12 + 0xd1) & 0x20) == 0) && ((*(byte *)(lVar12 + 0xd1) & 8) == 0)) {\r\n    bVar1 = false;\r\n  }\r\n  else {\r\n    bVar1 = true;\r\n  }\r\n  lVar12 = (**(code **)*puVar11)(puVar11);\r\n  if ((DAT_1439d14a0 <= *(int *)(lVar12 + 0x60)) && (*(int *)(lVar12 + 0x60) <= DAT_1439d14a4)) {\r\n    bVar6 = true;\r\n    if (bVar1) {\r\nLAB_14083f684:\r\n      bVar9 = 0;\r\n    }\r\n    else {\r\n      uVar13 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));\r\n      cVar8 = FUN_140878c70(uVar13);\r\n      if (cVar8 != '\\0') goto LAB_14083f684;\r\n      bVar9 = 1;\r\n    }\r\n    *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) & 0xfe;\r\n    *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) | bVar9;\r\n  }\r\n  local_c8 = *(longlong *)(param_1 + 0x18);\r\n  lVar12 = FUN_140877720(puVar11[2],&DAT_1439c5880,2);\r\n  local_b8 = lVar12;\r\n  local_c0 = FUN_140877720(puVar11[2],&DAT_1439dcd30,2);\r\n  local_f8 = (undefined4 *)0x0;\r\n  FUN_1401548e0();\r\n  local_f8 = (undefined4 *)0x0;\r\n  local_100 = 0;\r\n  local_ec = 0x3f;\r\n  local_f0 = 0x5000000;\r\n  if ((*(byte *)(param_1 + 0x140) & 1) == 0) {\r\n    if ((lVar12 != 0) && (iVar21 = 0, (*(uint *)(lVar12 + 0x30) & 0xffffff) != 0)) {\r\n      lVar14 = 0;\r\n      do {\r\n        lVar16 = *(longlong *)(lVar12 + 0x28);\r\n        if ((*(char *)(lVar14 + 0x11 + lVar16) == '\\0') &&\r\n           (((*(char *)(lVar14 + 0x10 + lVar16) == '\\0' || (bVar1)) &&\r\n            (lVar16 = FUN_1406dc560(*(undefined4 *)(lVar14 + lVar16)), lVar16 != 0)))) {\r\n          local_res18[0] = *(uint *)(lVar16 + 0xa8);\r\n          FUN_14052e460(&local_f8,local_res18);\r\n        }\r\n        iVar21 = iVar21 + 1;\r\n        lVar14 = lVar14 + 0x18;\r\n      } while (iVar21 < (int)(*(uint *)(lVar12 + 0x30) & 0xffffff));\r\n    }\r\n    puVar19 = (uint *)(param_1 + 0x130);\r\n  }\r\n  else {\r\n    lVar12 = FUN_1408400b0(param_1,puVar11);\r\n    uVar20 = 0;\r\n    if (lVar12 == 0) goto LAB_14083f7dc;\r\n    local_res18[0] = *(uint *)(lVar12 + 0xa8);\r\n    puVar19 = local_res18;\r\n  }\r\n  FUN_14052e460(&local_f8,puVar19);\r\n  local_100 = local_ec;\r\n  uVar20 = local_f0;\r\nLAB_14083f7dc:\r\n  local_res18[0] = local_res18[0] & 0xffffff00;\r\n  lVar12 = *(longlong *)(param_1 + 0x10);\r\n  if (*(int *)(*(longlong *)(lVar12 + 0x2f8) + 0x5d0) != 0xe) {\r\n    cVar8 = FUN_1408241b0(*(longlong *)(lVar12 + 0x2f8) + 0x410);\r\n    local_res18[0] = CONCAT31(local_res18[0]._1_3_,cVar8 == '\\0');\r\n  }\r\n  if (*(int *)(param_1 + 0x13c) < 0) {\r\n    fVar22 = 0.0;\r\n  }\r\n  else {\r\n    fVar22 = *(float *)(*(longlong *)(*(longlong *)(lVar12 + 0x330) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x13c) * 0x28);\r\n  }\r\n  local_res20 = 0.0 < fVar22;\r\n  if ((uVar20 & 0xffffff) != 0) {\r\n    local_d8 = (ulonglong)(uVar20 & 0xffffff);\r\n    local_d0 = local_f8;\r\n    do {\r\n      lVar14 = FUN_1406dc560(*local_d0);\r\n      puVar15 = (undefined1 *)FUN_140877720(lVar14,&DAT_1439c5a30,2);\r\n      local_e8 = puVar15;\r\n      if ((puVar15 != (undefined1 *)0x0) && ((puVar15[0xd0] & 2) == 0)) {\r\n        lVar16 = FUN_140854880(param_1);\r\n        fVar23 = *(float *)(lVar14 + 0x344) - *(float *)(lVar16 + 0x344);\r\n        fVar22 = *(float *)(lVar14 + 0x340) - *(float *)(lVar16 + 0x340);\r\n        fVar24 = *(float *)(lVar14 + 0x33c) - *(float *)(lVar16 + 0x33c);\r\n        lVar16 = *(longlong *)(puVar15 + 8);\r\n        if ((*(char *)(lVar16 + 0xa0) == '\\0') ||\r\n           ((cVar8 = FUN_140803180(lVar12,0x31), cVar8 == '\\0' &&\r\n            (cVar8 = FUN_140803180(lVar12,0x32), cVar8 == '\\0')))) {\r\n          bVar1 = false;\r\n        }\r\n        else {\r\n          bVar1 = true;\r\n        }\r\n        if ((((*(char *)(param_2 + 0x40) != '\\0') || (bVar6)) ||\r\n            (fVar24 * fVar24 + fVar22 * fVar22 + fVar23 * fVar23 <=\r\n             *(float *)(local_c8 + 0xa0) * *(float *)(local_c8 + 0xa0))) ||\r\n           (cVar8 = FUN_14061c8f0(), cVar8 != '\\0')) {\r\n          bVar3 = false;\r\n        }\r\n        else {\r\n          bVar3 = true;\r\n        }\r\n        if (((*(char *)(lVar16 + 0xa0) == '\\0') && (*(longlong *)(lVar16 + 0xf8) != 0)) &&\r\n           ((0 < *(int *)(lVar16 + 0x100) &&\r\n            ((cVar8 = FUN_140615d40(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x328)),\r\n             cVar8 != '\\0' && ((*(byte *)(param_1 + 0x140) & 1) == 0)))))) {\r\n          bVar2 = true;\r\n        }\r\n        else {\r\n          bVar2 = false;\r\n        }\r\n        cVar8 = FUN_14076e800(lVar16);\r\n        cVar10 = FUN_14076e830();\r\n        if ((cVar8 == '\\0') || (local_res20 == '\\0')) {\r\n          bVar4 = false;\r\n        }\r\n        else {\r\n          bVar4 = true;\r\n        }\r\n        if (((cVar8 == '\\0') && (cVar10 == '\\0')) || ((char)local_res18[0] == '\\0')) {\r\n          bVar5 = false;\r\n        }\r\n        else {\r\n          bVar5 = true;\r\n        }\r\n        if (((bVar1) || (bVar2)) || ((bVar3 || ((bVar4 || (bVar5)))))) {\r\n          local_b0[0] = 0x69;\r\n          local_a8 = (uint)(*(byte *)(param_1 + 0x140) >> 1 & 1);\r\n          local_98[0] = 0x69;\r\n          local_90 = 0;\r\n          FUN_1408848a0(lVar14,local_78,&DAT_1439acd40,local_98,local_b0);\r\n          FUN_140882fc0(local_78);\r\n          if ((*(longlong *)(lVar16 + 0xf8) != 0) &&\r\n             (((bVar2 && (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0)) &&\r\n              (lVar14 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0xd8),\r\n              lVar14 != 0)))) {\r\n            switch(*(undefined4 *)(*(longlong *)(lVar16 + 0xf8) + 0x60)) {\r\n            case 4:\r\n              uVar13 = 4;\r\n              break;\r\n            default:\r\n              goto switchD_14083fb69_caseD_5;\r\n            case 0xe:\r\n              uVar13 = 5;\r\n              break;\r\n            case 0x16:\r\n              uVar13 = 0xe;\r\n            }\r\n            FUN_14038e820(lVar14,uVar13,0);\r\n            FUN_140841490(param_1);\r\n          }\r\nswitchD_14083fb69_caseD_5:\r\n        }\r\n        else {\r\n          local_e0[0] = *(undefined4 *)(lVar14 + 0xa8);\r\n          FUN_14052e460(param_1 + 0xf0,local_e0);\r\n          *(byte *)(lVar14 + 0xd1) = *(byte *)(lVar14 + 0xd1) & 0x7f;\r\n          FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(lVar12 + 0x10),lVar14);\r\n          if (local_c0 != 0) {\r\n            FUN_140617250(local_c0,lVar14);\r\n          }\r\n          if (local_b8 != 0) {\r\n            FUN_140744a40(local_b8,lVar14);\r\n          }\r\n          puVar15 = local_e8;\r\n          if ((*(longlong *)(lVar16 + 0xf8) == 0) ||\r\n             (*(int *)(*(longlong *)(lVar16 + 0xf8) + 0x60) != 0x18)) {\r\n            FUN_14076e2b0(local_e8);\r\n          }\r\n          local_e8 = local_98;\r\n          uVar13 = FUN_140882fa0(local_98,*(undefined8 *)(puVar15 + 0x10));\r\n          uVar17 = FUN_140854880(param_1);\r\n          FUN_140884920(uVar17,local_b0,&DAT_1439abb70,uVar13);\r\n          FUN_140882fc0(local_b0);\r\n        }\r\n      }\r\n      local_d0 = local_d0 + 1;\r\n      local_d8 = local_d8 - 1;\r\n    } while (local_d8 != 0);\r\n  }\r\n  if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {\r\n    FUN_140854190(param_1);\r\n  }\r\n  if ((*(longlong *)(lVar12 + 0x90) != 0) &&\r\n     (*(longlong *)(*(longlong *)(lVar12 + 0x90) + 0x68) != 0)) {\r\n    FUN_140888240();\r\n  }\r\n  puVar7 = local_f8;\r\n  if (((local_100 & 0x40) == 0) && (local_f8 != (undefined4 *)0x0)) {\r\n    plVar18 = (longlong *)FUN_1401548e0();\r\n    (**(code **)(*plVar18 + 0x10))\r\n              (plVar18,\r\n               \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n               ,0x1ec,puVar7,0x10);\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14083f590(longlong param_1,longlong param_2)\r\n\r\n{\r\n  bool bVar1;\r\n  bool bVar2;\r\n  bool bVar3;\r\n  bool bVar4;\r\n  bool bVar5;\r\n  bool bVar6;\r\n  undefined4 *puVar7;\r\n  char cVar8;\r\n  byte bVar9;\r\n  char cVar10;\r\n  undefined8 *puVar11;\r\n  longlong lVar12;\r\n  undefined8 uVar13;\r\n  longlong lVar14;\r\n  undefined1 *puVar15;\r\n  longlong lVar16;\r\n  undefined8 uVar17;\r\n  longlong *plVar18;\r\n  uint *puVar19;\r\n  uint uVar20;\r\n  int iVar21;\r\n  float fVar22;\r\n  float fVar23;\r\n  float fVar24;\r\n  uint local_res18 [2];\r\n  char local_res20;\r\n  uint local_100;\r\n  undefined4 *local_f8;\r\n  uint local_f0;\r\n  uint local_ec;\r\n  undefined1 *local_e8;\r\n  undefined4 local_e0 [2];\r\n  ulonglong local_d8;\r\n  undefined4 *local_d0;\r\n  longlong local_c8;\r\n  longlong local_c0;\r\n  longlong local_b8;\r\n  undefined1 local_b0 [8];\r\n  uint local_a8;\r\n  undefined1 local_98 [8];\r\n  undefined4 local_90;\r\n  undefined8 local_80;\r\n  undefined1 local_78 [56];\r\n  \r\n  local_80 = 0xfffffffffffffffe;\r\n  FUN_1409ffc00();\r\n  puVar11 = (undefined8 *)FUN_1403da910(param_2 + 8,2);\r\n  if (puVar11[2] == 0) {\r\n    *(undefined4 *)(param_1 + 0x130) = 0x3fff0010;\r\n  }\r\n  else {\r\n    *(undefined4 *)(param_1 + 0x130) = *(undefined4 *)(puVar11[2] + 0xa8);\r\n  }\r\n  *(byte *)(param_1 + 0x140) =\r\n       (*(byte *)(param_2 + 0x41) & 1) * '\\x02' | *(byte *)(param_1 + 0x140) & 0xfc;\r\n  bVar6 = false;\r\n  lVar12 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));\r\n  if (((*(byte *)(lVar12 + 0xd1) & 0x20) == 0) && ((*(byte *)(lVar12 + 0xd1) & 8) == 0)) {\r\n    bVar1 = false;\r\n  }\r\n  else {\r\n    bVar1 = true;\r\n  }\r\n  lVar12 = (**(code **)*puVar11)(puVar11);\r\n  if ((DAT_1439d14a0 <= *(int *)(lVar12 + 0x60)) && (*(int *)(lVar12 + 0x60) <= DAT_1439d14a4)) {\r\n    bVar6 = true;\r\n    if (bVar1) {\r\nLAB_14083f684:\r\n      bVar9 = 0;\r\n    }\r\n    else {\r\n      uVar13 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));\r\n      cVar8 = FUN_140878c70(uVar13);\r\n      if (cVar8 != '\\0') goto LAB_14083f684;\r\n      bVar9 = 1;\r\n    }\r\n    *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) & 0xfe;\r\n    *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) | bVar9;\r\n  }\r\n  local_c8 = *(longlong *)(param_1 + 0x18);\r\n  lVar12 = FUN_140877720(puVar11[2],&DAT_1439c5880,2);\r\n  local_b8 = lVar12;\r\n  local_c0 = FUN_140877720(puVar11[2],&DAT_1439dcd30,2);\r\n  local_f8 = (undefined4 *)0x0;\r\n  FUN_1401548e0();\r\n  local_f8 = (undefined4 *)0x0;\r\n  local_100 = 0;\r\n  local_ec = 0x3f;\r\n  local_f0 = 0x5000000;\r\n  if ((*(byte *)(param_1 + 0x140) & 1) == 0) {\r\n    if ((lVar12 != 0) && (iVar21 = 0, (*(uint *)(lVar12 + 0x30) & 0xffffff) != 0)) {\r\n      lVar14 = 0;\r\n      do {\r\n        lVar16 = *(longlong *)(lVar12 + 0x28);\r\n        if ((*(char *)(lVar14 + 0x11 + lVar16) == '\\0') &&\r\n           (((*(char *)(lVar14 + 0x10 + lVar16) == '\\0' || (bVar1)) &&\r\n            (lVar16 = FUN_1406dc560(*(undefined4 *)(lVar14 + lVar16)), lVar16 != 0)))) {\r\n          local_res18[0] = *(uint *)(lVar16 + 0xa8);\r\n          FUN_14052e460(&local_f8,local_res18);\r\n        }\r\n        iVar21 = iVar21 + 1;\r\n        lVar14 = lVar14 + 0x18;\r\n      } while (iVar21 < (int)(*(uint *)(lVar12 + 0x30) & 0xffffff));\r\n    }\r\n    puVar19 = (uint *)(param_1 + 0x130);\r\n  }\r\n  else {\r\n    lVar12 = FUN_1408400b0(param_1,puVar11);\r\n    uVar20 = 0;\r\n    if (lVar12 == 0) goto LAB_14083f7dc;\r\n    local_res18[0] = *(uint *)(lVar12 + 0xa8);\r\n    puVar19 = local_res18;\r\n  }\r\n  FUN_14052e460(&local_f8,puVar19);\r\n  local_100 = local_ec;\r\n  uVar20 = local_f0;\r\nLAB_14083f7dc:\r\n  local_res18[0] = local_res18[0] & 0xffffff00;\r\n  lVar12 = *(longlong *)(param_1 + 0x10);\r\n  if (*(int *)(*(longlong *)(lVar12 + 0x2f8) + 0x5d0) != 0xe) {\r\n    cVar8 = FUN_1408241b0(*(longlong *)(lVar12 + 0x2f8) + 0x410);\r\n    local_res18[0] = CONCAT31(local_res18[0]._1_3_,cVar8 == '\\0');\r\n  }\r\n  if (*(int *)(param_1 + 0x13c) < 0) {\r\n    fVar22 = 0.0;\r\n  }\r\n  else {\r\n    fVar22 = *(float *)(*(longlong *)(*(longlong *)(lVar12 + 0x330) + 0x48) + 0x24 +\r\n                       (longlong)*(int *)(param_1 + 0x13c) * 0x28);\r\n  }\r\n  local_res20 = 0.0 < fVar22;\r\n  if ((uVar20 & 0xffffff) != 0) {\r\n    local_d8 = (ulonglong)(uVar20 & 0xffffff);\r\n    local_d0 = local_f8;\r\n    do {\r\n      lVar14 = FUN_1406dc560(*local_d0);\r\n      puVar15 = (undefined1 *)FUN_140877720(lVar14,&DAT_1439c5a30,2);\r\n      local_e8 = puVar15;\r\n      if ((puVar15 != (undefined1 *)0x0) && ((puVar15[0xd0] & 2) == 0)) {\r\n        lVar16 = FUN_140854880(param_1);\r\n        fVar23 = *(float *)(lVar14 + 0x344) - *(float *)(lVar16 + 0x344);\r\n        fVar22 = *(float *)(lVar14 + 0x340) - *(float *)(lVar16 + 0x340);\r\n        fVar24 = *(float *)(lVar14 + 0x33c) - *(float *)(lVar16 + 0x33c);\r\n        lVar16 = *(longlong *)(puVar15 + 8);\r\n        if ((*(char *)(lVar16 + 0xa0) == '\\0') ||\r\n           ((cVar8 = FUN_140803180(lVar12,0x31), cVar8 == '\\0' &&\r\n            (cVar8 = FUN_140803180(lVar12,0x32), cVar8 == '\\0')))) {\r\n          bVar1 = false;\r\n        }\r\n        else {\r\n          bVar1 = true;\r\n        }\r\n        if ((((*(char *)(param_2 + 0x40) != '\\0') || (bVar6)) ||\r\n            (fVar24 * fVar24 + fVar22 * fVar22 + fVar23 * fVar23 <=\r\n             *(float *)(local_c8 + 0xa0) * *(float *)(local_c8 + 0xa0))) ||\r\n           (cVar8 = FUN_14061c8f0(), cVar8 != '\\0')) {\r\n          bVar3 = false;\r\n        }\r\n        else {\r\n          bVar3 = true;\r\n        }\r\n        if (((*(char *)(lVar16 + 0xa0) == '\\0') && (*(longlong *)(lVar16 + 0xf8) != 0)) &&\r\n           ((0 < *(int *)(lVar16 + 0x100) &&\r\n            ((cVar8 = FUN_140615d40(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x328)),\r\n             cVar8 != '\\0' && ((*(byte *)(param_1 + 0x140) & 1) == 0)))))) {\r\n          bVar2 = true;\r\n        }\r\n        else {\r\n          bVar2 = false;\r\n        }\r\n        cVar8 = FUN_14076e800(lVar16);\r\n        cVar10 = FUN_14076e830();\r\n        if ((cVar8 == '\\0') || (local_res20 == '\\0')) {\r\n          bVar4 = false;\r\n        }\r\n        else {\r\n          bVar4 = true;\r\n        }\r\n        if (((cVar8 == '\\0') && (cVar10 == '\\0')) || ((char)local_res18[0] == '\\0')) {\r\n          bVar5 = false;\r\n        }\r\n        else {\r\n          bVar5 = true;\r\n        }\r\n        if (((bVar1) || (bVar2)) || ((bVar3 || ((bVar4 || (bVar5)))))) {\r\n          local_b0[0] = 0x69;\r\n          local_a8 = (uint)(*(byte *)(param_1 + 0x140) >> 1 & 1);\r\n          local_98[0] = 0x69;\r\n          local_90 = 0;\r\n          FUN_1408848a0(lVar14,local_78,&DAT_1439acd40,local_98,local_b0);\r\n          FUN_140882fc0(local_78);\r\n          if ((*(longlong *)(lVar16 + 0xf8) != 0) &&\r\n             (((bVar2 && (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0)) &&\r\n              (lVar14 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0xd8),\r\n              lVar14 != 0)))) {\r\n            switch(*(undefined4 *)(*(longlong *)(lVar16 + 0xf8) + 0x60)) {\r\n            case 4:\r\n              uVar13 = 4;\r\n              break;\r\n            default:\r\n              goto switchD_14083fb69_caseD_5;\r\n            case 0xe:\r\n              uVar13 = 5;\r\n              break;\r\n            case 0x16:\r\n              uVar13 = 0xe;\r\n            }\r\n            FUN_14038e820(lVar14,uVar13,0);\r\n            FUN_140841490(param_1);\r\n          }\r\nswitchD_14083fb69_caseD_5:\r\n        }\r\n        else {\r\n          local_e0[0] = *(undefined4 *)(lVar14 + 0xa8);\r\n          FUN_14052e460(param_1 + 0xf0,local_e0);\r\n          *(byte *)(lVar14 + 0xd1) = *(byte *)(lVar14 + 0xd1) & 0x7f;\r\n          FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(lVar12 + 0x10),lVar14);\r\n          if (local_c0 != 0) {\r\n            FUN_140617250(local_c0,lVar14);\r\n          }\r\n          if (local_b8 != 0) {\r\n            FUN_140744a40(local_b8,lVar14);\r\n          }\r\n          puVar15 = local_e8;\r\n          if ((*(longlong *)(lVar16 + 0xf8) == 0) ||\r\n             (*(int *)(*(longlong *)(lVar16 + 0xf8) + 0x60) != 0x18)) {\r\n            FUN_14076e2b0(local_e8);\r\n          }\r\n          local_e8 = local_98;\r\n          uVar13 = FUN_140882fa0(local_98,*(undefined8 *)(puVar15 + 0x10));\r\n          uVar17 = FUN_140854880(param_1);\r\n          FUN_140884920(uVar17,local_b0,&DAT_1439abb70,uVar13);\r\n          FUN_140882fc0(local_b0);\r\n        }\r\n      }\r\n      local_d0 = local_d0 + 1;\r\n      local_d8 = local_d8 - 1;\r\n    } while (local_d8 != 0);\r\n  }\r\n  if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {\r\n    FUN_140854190(param_1);\r\n  }\r\n  if ((*(longlong *)(lVar12 + 0x90) != 0) &&\r\n     (*(longlong *)(*(longlong *)(lVar12 + 0x90) + 0x68) != 0)) {\r\n    FUN_140888240();\r\n  }\r\n  puVar7 = local_f8;\r\n  if (((local_100 & 0x40) == 0) && (local_f8 != (undefined4 *)0x0)) {\r\n    plVar18 = (longlong *)FUN_1401548e0();\r\n    (**(code **)(*plVar18 + 0x10))\r\n              (plVar18,\r\n               \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n               ,0x1ec,puVar7,0x10);\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-14083fd00": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14083fd00(longlong param_1)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  undefined8 uVar2;\r\n  undefined8 uVar3;\r\n  undefined1 auStackX_8 [8];\r\n  \r\n  FUN_140854fd0();\r\n  uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330);\r\n  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939a0);\r\n  if ((undefined4 *)(param_1 + 0x134) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x134) = *puVar1;\r\n  }\r\n  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939a8);\r\n  if ((undefined4 *)(param_1 + 0x138) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x138) = *puVar1;\r\n  }\r\n  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939b0);\r\n  if ((undefined4 *)(param_1 + 0x13c) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x13c) = *puVar1;\r\n  }\r\n  uVar2 = FUN_140854880(param_1);\r\n  uVar3 = FUN_140877720(uVar2,&DAT_1439c5d00,0);\r\n  FUN_1403dab80(param_1 + 0x100,uVar3);\r\n  uVar3 = FUN_140877720(uVar2,&DAT_1439b9e90,0);\r\n  FUN_1403dab80(param_1 + 0x110,uVar3);\r\n  uVar2 = FUN_140877720(uVar2,&DAT_1439d15f0,0);\r\n  FUN_1403dab80(param_1 + 0x120,uVar2);\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14083fd00(longlong param_1)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  undefined8 uVar2;\r\n  undefined8 uVar3;\r\n  undefined1 auStackX_8 [8];\r\n  \r\n  FUN_140854fd0();\r\n  uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330);\r\n  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939a0);\r\n  if ((undefined4 *)(param_1 + 0x134) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x134) = *puVar1;\r\n  }\r\n  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939a8);\r\n  if ((undefined4 *)(param_1 + 0x138) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x138) = *puVar1;\r\n  }\r\n  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939b0);\r\n  if ((undefined4 *)(param_1 + 0x13c) != puVar1) {\r\n    *(undefined4 *)(param_1 + 0x13c) = *puVar1;\r\n  }\r\n  uVar2 = FUN_140854880(param_1);\r\n  uVar3 = FUN_140877720(uVar2,&DAT_1439c5d00,0);\r\n  FUN_1403dab80(param_1 + 0x100,uVar3);\r\n  uVar3 = FUN_140877720(uVar2,&DAT_1439b9e90,0);\r\n  FUN_1403dab80(param_1 + 0x110,uVar3);\r\n  uVar2 = FUN_140877720(uVar2,&DAT_1439d15f0,0);\r\n  FUN_1403dab80(param_1 + 0x120,uVar2);\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-140840b20": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nulonglong FUN_140840b20(longlong param_1,longlong param_2)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  bool bVar2;\r\n  char cVar3;\r\n  int iVar4;\r\n  longlong lVar5;\r\n  longlong *plVar6;\r\n  longlong lVar7;\r\n  ulonglong uVar8;\r\n  float extraout_XMM0_Da;\r\n  float fVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  \r\n  lVar5 = FUN_1403da910(param_2 + 8,2);\r\n  if (lVar5 == 0) {\r\n    return 0;\r\n  }\r\n  uVar1 = *(undefined8 *)(param_1 + 0x10);\r\n  plVar6 = (longlong *)FUN_1403da910(param_2 + 8,2);\r\n  lVar5 = (**(code **)*plVar6)(plVar6);\r\n  if ((*(int *)(lVar5 + 0x60) < DAT_1439d14a0) || (DAT_1439d14a4 < *(int *)(lVar5 + 0x60))) {\r\n    bVar2 = false;\r\n  }\r\n  else {\r\n    bVar2 = true;\r\n  }\r\n  lVar5 = plVar6[2];\r\n  if ((*(char *)(param_2 + 0x40) == '\\0') &&\r\n     ((!bVar2 || (((*(byte *)(lVar5 + 0xd1) & 0x20) == 0 && ((*(byte *)(lVar5 + 0xd1) & 8) == 0)))))\r\n     ) {\r\n    lVar7 = FUN_140854880(param_1);\r\n    fVar10 = *(float *)(lVar5 + 0x300) - *(float *)(lVar7 + 0x300);\r\n    fVar11 = *(float *)(lVar5 + 0x304) - *(float *)(lVar7 + 0x304);\r\n    fVar9 = *(float *)(lVar5 + 0x308) - *(float *)(lVar7 + 0x308);\r\n    uVar8 = FUN_140840610(param_1);\r\n    if (extraout_XMM0_Da * extraout_XMM0_Da < fVar11 * fVar11 + fVar10 * fVar10 + fVar9 * fVar9)\r\n    goto LAB_140840dcb;\r\n  }\r\n  if (bVar2) {\r\n    lVar5 = FUN_140877720(plVar6[2],&DAT_1439dcd30,2);\r\n    if ((((lVar5 == 0) || (lVar5 = FUN_140615290(lVar5), lVar5 == 0)) ||\r\n        (cVar3 = FUN_140878c60(lVar5), cVar3 != '\\0')) ||\r\n       (uVar8 = FUN_140877720(lVar5,&DAT_1439c5a30,2), uVar8 == 0)) {\r\n      lVar5 = FUN_140877720(plVar6[2],&DAT_1439c5880,2);\r\n      uVar8 = 0;\r\n      if (lVar5 == 0) goto LAB_140840dcb;\r\n      lVar5 = FUN_140744880(lVar5);\r\n      uVar8 = 0;\r\n      if ((lVar5 == 0) || (uVar8 = FUN_140878c60(lVar5), (char)uVar8 != '\\0')) goto LAB_140840dcb;\r\n      goto LAB_140840d00;\r\n    }\r\n  }\r\n  else {\r\n    uVar8 = (**(code **)(*plVar6 + 0xe0))(plVar6);\r\n    if ((char)uVar8 == '\\0') goto LAB_140840dcb;\r\n    lVar5 = plVar6[2];\r\nLAB_140840d00:\r\n    uVar8 = FUN_140877720(lVar5,&DAT_1439c5a30,2);\r\n    if (uVar8 == 0) goto LAB_140840dcb;\r\n  }\r\n  if ((*(byte *)(uVar8 + 0xd0) & 2) != 0) {\r\nLAB_140840dcb:\r\n    return uVar8 & 0xffffffffffffff00;\r\n  }\r\n  lVar5 = *(longlong *)(uVar8 + 8);\r\n  if (*(char *)(lVar5 + 0xa0) != '\\0') {\r\n    cVar3 = FUN_140803180(uVar1,0x2d);\r\n    if ((cVar3 == '\\0') && (cVar3 = FUN_140803180(uVar1,0x2e), cVar3 == '\\0')) {\r\n      bVar2 = false;\r\n    }\r\n    else {\r\n      bVar2 = true;\r\n    }\r\n    iVar4 = FUN_140802de0(uVar1);\r\n    uVar8 = FUN_140803180(uVar1,0x31);\r\n    if (((char)uVar8 != '\\0') || (uVar8 = FUN_140803180(uVar1,0x32), (char)uVar8 != '\\0')) {\r\n      uVar8 = CONCAT71((int7)(uVar8 >> 8),1);\r\n    }\r\n    if (((bVar2) || (iVar4 == 2)) || ((char)uVar8 != '\\0')) goto LAB_140840dcb;\r\n  }\r\n  lVar5 = *(longlong *)(lVar5 + 0xf8);\r\n  if ((lVar5 != 0) && (*(int *)(lVar5 + 0x60) == 5)) {\r\n    FUN_140840ff0(param_1,lVar5 + 0xd0);\r\n  }\r\n  uVar8 = FUN_140854910(param_1,param_2);\r\n  return uVar8;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nulonglong FUN_140840b20(longlong param_1,longlong param_2)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  bool bVar2;\r\n  char cVar3;\r\n  int iVar4;\r\n  longlong lVar5;\r\n  longlong *plVar6;\r\n  longlong lVar7;\r\n  ulonglong uVar8;\r\n  float extraout_XMM0_Da;\r\n  float fVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  \r\n  lVar5 = FUN_1403da910(param_2 + 8,2);\r\n  if (lVar5 == 0) {\r\n    return 0;\r\n  }\r\n  uVar1 = *(undefined8 *)(param_1 + 0x10);\r\n  plVar6 = (longlong *)FUN_1403da910(param_2 + 8,2);\r\n  lVar5 = (**(code **)*plVar6)(plVar6);\r\n  if ((*(int *)(lVar5 + 0x60) < DAT_1439d14a0) || (DAT_1439d14a4 < *(int *)(lVar5 + 0x60))) {\r\n    bVar2 = false;\r\n  }\r\n  else {\r\n    bVar2 = true;\r\n  }\r\n  lVar5 = plVar6[2];\r\n  if ((*(char *)(param_2 + 0x40) == '\\0') &&\r\n     ((!bVar2 || (((*(byte *)(lVar5 + 0xd1) & 0x20) == 0 && ((*(byte *)(lVar5 + 0xd1) & 8) == 0)))))\r\n     ) {\r\n    lVar7 = FUN_140854880(param_1);\r\n    fVar10 = *(float *)(lVar5 + 0x300) - *(float *)(lVar7 + 0x300);\r\n    fVar11 = *(float *)(lVar5 + 0x304) - *(float *)(lVar7 + 0x304);\r\n    fVar9 = *(float *)(lVar5 + 0x308) - *(float *)(lVar7 + 0x308);\r\n    uVar8 = FUN_140840610(param_1);\r\n    if (extraout_XMM0_Da * extraout_XMM0_Da < fVar11 * fVar11 + fVar10 * fVar10 + fVar9 * fVar9)\r\n    goto LAB_140840dcb;\r\n  }\r\n  if (bVar2) {\r\n    lVar5 = FUN_140877720(plVar6[2],&DAT_1439dcd30,2);\r\n    if ((((lVar5 == 0) || (lVar5 = FUN_140615290(lVar5), lVar5 == 0)) ||\r\n        (cVar3 = FUN_140878c60(lVar5), cVar3 != '\\0')) ||\r\n       (uVar8 = FUN_140877720(lVar5,&DAT_1439c5a30,2), uVar8 == 0)) {\r\n      lVar5 = FUN_140877720(plVar6[2],&DAT_1439c5880,2);\r\n      uVar8 = 0;\r\n      if (lVar5 == 0) goto LAB_140840dcb;\r\n      lVar5 = FUN_140744880(lVar5);\r\n      uVar8 = 0;\r\n      if ((lVar5 == 0) || (uVar8 = FUN_140878c60(lVar5), (char)uVar8 != '\\0')) goto LAB_140840dcb;\r\n      goto LAB_140840d00;\r\n    }\r\n  }\r\n  else {\r\n    uVar8 = (**(code **)(*plVar6 + 0xe0))(plVar6);\r\n    if ((char)uVar8 == '\\0') goto LAB_140840dcb;\r\n    lVar5 = plVar6[2];\r\nLAB_140840d00:\r\n    uVar8 = FUN_140877720(lVar5,&DAT_1439c5a30,2);\r\n    if (uVar8 == 0) goto LAB_140840dcb;\r\n  }\r\n  if ((*(byte *)(uVar8 + 0xd0) & 2) != 0) {\r\nLAB_140840dcb:\r\n    return uVar8 & 0xffffffffffffff00;\r\n  }\r\n  lVar5 = *(longlong *)(uVar8 + 8);\r\n  if (*(char *)(lVar5 + 0xa0) != '\\0') {\r\n    cVar3 = FUN_140803180(uVar1,0x2d);\r\n    if ((cVar3 == '\\0') && (cVar3 = FUN_140803180(uVar1,0x2e), cVar3 == '\\0')) {\r\n      bVar2 = false;\r\n    }\r\n    else {\r\n      bVar2 = true;\r\n    }\r\n    iVar4 = FUN_140802de0(uVar1);\r\n    uVar8 = FUN_140803180(uVar1,0x31);\r\n    if (((char)uVar8 != '\\0') || (uVar8 = FUN_140803180(uVar1,0x32), (char)uVar8 != '\\0')) {\r\n      uVar8 = CONCAT71((int7)(uVar8 >> 8),1);\r\n    }\r\n    if (((bVar2) || (iVar4 == 2)) || ((char)uVar8 != '\\0')) goto LAB_140840dcb;\r\n  }\r\n  lVar5 = *(longlong *)(lVar5 + 0xf8);\r\n  if ((lVar5 != 0) && (*(int *)(lVar5 + 0x60) == 5)) {\r\n    FUN_140840ff0(param_1,lVar5 + 0xd0);\r\n  }\r\n  uVar8 = FUN_140854910(param_1,param_2);\r\n  return uVar8;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-14083fe70": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14083fe70(longlong param_1)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  bool bVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  longlong *plVar7;\r\n  undefined4 *puVar8;\r\n  undefined1 auStack_b8 [8];\r\n  undefined4 uStack_b0;\r\n  undefined1 auStack_a0 [8];\r\n  uint uStack_98;\r\n  undefined1 auStack_88 [32];\r\n  undefined1 auStack_68 [80];\r\n  \r\n  puVar8 = *(undefined4 **)(param_1 + 0xf0);\r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  puVar1 = puVar8 + (*(uint *)(param_1 + 0xf8) & 0xffffff);\r\n  do {\r\n    if (puVar8 == puVar1) {\r\n      if ((*(byte *)(param_1 + 0xfc) & 0x40) == 0) {\r\n        lVar5 = *(longlong *)(param_1 + 0xf0);\r\n        if (lVar5 != 0) {\r\n          plVar7 = (longlong *)FUN_1401548e0();\r\n          (**(code **)(*plVar7 + 0x10))\r\n                    (plVar7,\r\n                     \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                     ,0x1ec,lVar5,0x10);\r\n        }\r\n        *(uint *)(param_1 + 0xfc) = *(uint *)(param_1 + 0xfc) & 0xff;\r\n        *(undefined8 *)(param_1 + 0xf0) = 0;\r\n      }\r\n      *(uint *)(param_1 + 0xf8) = *(uint *)(param_1 + 0xf8) & 0xff000000;\r\n      lVar2 = *(longlong *)(lVar2 + 0x90);\r\n      if ((lVar2 != 0) && (*(longlong *)(lVar2 + 0x68) != 0)) {\r\n        FUN_1408880d0();\r\n      }\r\n      FUN_1408550d0(param_1);\r\n      return;\r\n    }\r\n    lVar5 = FUN_1406dc560(*puVar8);\r\n    if (lVar5 != 0) {\r\n      lVar6 = FUN_140877720(lVar5,&DAT_1439c5a30,2);\r\n      if (lVar6 == 0) {\r\nLAB_14083ffc8:\r\n        *(byte *)(lVar5 + 0xd7) = *(byte *)(lVar5 + 0xd7) | 1;\r\n        FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar5);\r\n      }\r\n      else {\r\n        lVar3 = *(longlong *)(lVar6 + 8);\r\n        if ((*(longlong *)(lVar3 + 0xf8) == 0) ||\r\n           (*(int *)(*(longlong *)(lVar3 + 0xf8) + 0xc4) == -1)) {\r\n          bVar4 = false;\r\n        }\r\n        else {\r\n          bVar4 = true;\r\n        }\r\n        if (*(char *)(lVar3 + 0xa0) == '\\0') {\r\n          if (!bVar4) {\r\n            FUN_140841030(param_1,lVar6,*(byte *)(param_1 + 0x140) & 1);\r\n            uStack_98 = *(byte *)(param_1 + 0x140) >> 1 & 1;\r\n            auStack_a0[0] = 0x69;\r\n            auStack_b8[0] = 0x69;\r\n            uStack_b0 = 1;\r\n            FUN_1408848a0(*(undefined8 *)(lVar6 + 0x10),auStack_88,&DAT_1439acd40,auStack_b8,\r\n                          auStack_a0);\r\n            FUN_140882fc0(auStack_88);\r\n            FUN_14076ec20(lVar6,1);\r\n            goto LAB_14083ffe5;\r\n          }\r\n        }\r\n        else if (!bVar4) {\r\n          FUN_14076ec80(lVar6);\r\n          goto LAB_14083ffc8;\r\n        }\r\n        FUN_1403b7aa0(auStack_68,lVar6,lVar3 + 200,0);\r\n        FUN_1408024d0(lVar2,0x34,auStack_68);\r\n      }\r\n    }\r\nLAB_14083ffe5:\r\n    puVar8 = puVar8 + 1;\r\n  } while( true );\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14083fe70(longlong param_1)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  bool bVar4;\r\n  longlong lVar5;\r\n  longlong lVar6;\r\n  longlong *plVar7;\r\n  undefined4 *puVar8;\r\n  undefined1 auStack_b8 [8];\r\n  undefined4 uStack_b0;\r\n  undefined1 auStack_a0 [8];\r\n  uint uStack_98;\r\n  undefined1 auStack_88 [32];\r\n  undefined1 auStack_68 [80];\r\n  \r\n  puVar8 = *(undefined4 **)(param_1 + 0xf0);\r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  puVar1 = puVar8 + (*(uint *)(param_1 + 0xf8) & 0xffffff);\r\n  do {\r\n    if (puVar8 == puVar1) {\r\n      if ((*(byte *)(param_1 + 0xfc) & 0x40) == 0) {\r\n        lVar5 = *(longlong *)(param_1 + 0xf0);\r\n        if (lVar5 != 0) {\r\n          plVar7 = (longlong *)FUN_1401548e0();\r\n          (**(code **)(*plVar7 + 0x10))\r\n                    (plVar7,\r\n                     \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                     ,0x1ec,lVar5,0x10);\r\n        }\r\n        *(uint *)(param_1 + 0xfc) = *(uint *)(param_1 + 0xfc) & 0xff;\r\n        *(undefined8 *)(param_1 + 0xf0) = 0;\r\n      }\r\n      *(uint *)(param_1 + 0xf8) = *(uint *)(param_1 + 0xf8) & 0xff000000;\r\n      lVar2 = *(longlong *)(lVar2 + 0x90);\r\n      if ((lVar2 != 0) && (*(longlong *)(lVar2 + 0x68) != 0)) {\r\n        FUN_1408880d0();\r\n      }\r\n      FUN_1408550d0(param_1);\r\n      return;\r\n    }\r\n    lVar5 = FUN_1406dc560(*puVar8);\r\n    if (lVar5 != 0) {\r\n      lVar6 = FUN_140877720(lVar5,&DAT_1439c5a30,2);\r\n      if (lVar6 == 0) {\r\nLAB_14083ffc8:\r\n        *(byte *)(lVar5 + 0xd7) = *(byte *)(lVar5 + 0xd7) | 1;\r\n        FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar5);\r\n      }\r\n      else {\r\n        lVar3 = *(longlong *)(lVar6 + 8);\r\n        if ((*(longlong *)(lVar3 + 0xf8) == 0) ||\r\n           (*(int *)(*(longlong *)(lVar3 + 0xf8) + 0xc4) == -1)) {\r\n          bVar4 = false;\r\n        }\r\n        else {\r\n          bVar4 = true;\r\n        }\r\n        if (*(char *)(lVar3 + 0xa0) == '\\0') {\r\n          if (!bVar4) {\r\n            FUN_140841030(param_1,lVar6,*(byte *)(param_1 + 0x140) & 1);\r\n            uStack_98 = *(byte *)(param_1 + 0x140) >> 1 & 1;\r\n            auStack_a0[0] = 0x69;\r\n            auStack_b8[0] = 0x69;\r\n            uStack_b0 = 1;\r\n            FUN_1408848a0(*(undefined8 *)(lVar6 + 0x10),auStack_88,&DAT_1439acd40,auStack_b8,\r\n                          auStack_a0);\r\n            FUN_140882fc0(auStack_88);\r\n            FUN_14076ec20(lVar6,1);\r\n            goto LAB_14083ffe5;\r\n          }\r\n        }\r\n        else if (!bVar4) {\r\n          FUN_14076ec80(lVar6);\r\n          goto LAB_14083ffc8;\r\n        }\r\n        FUN_1403b7aa0(auStack_68,lVar6,lVar3 + 200,0);\r\n        FUN_1408024d0(lVar2,0x34,auStack_68);\r\n      }\r\n    }\r\nLAB_14083ffe5:\r\n    puVar8 = puVar8 + 1;\r\n  } while( true );\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-140841500": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140841500(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  undefined4 uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  float fVar6;\r\n  uint uVar7;\r\n  longlong lVar8;\r\n  undefined8 *puVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  undefined8 uVar12;\r\n  longlong *plVar13;\r\n  uint uVar14;\r\n  float *pfVar15;\r\n  byte bVar16;\r\n  longlong lVar17;\r\n  uint uVar18;\r\n  float fVar19;\r\n  float fVar20;\r\n  uint auStack_188 [4];\r\n  undefined1 uStack_178;\r\n  float fStack_170;\r\n  float fStack_16c;\r\n  float fStack_168;\r\n  float fStack_160;\r\n  undefined4 uStack_15c;\r\n  undefined4 uStack_158;\r\n  float fStack_154;\r\n  float fStack_150;\r\n  float fStack_14c;\r\n  float fStack_148;\r\n  float fStack_144;\r\n  float fStack_140;\r\n  float fStack_138;\r\n  float fStack_134;\r\n  float fStack_130;\r\n  undefined1 auStack_128 [8];\r\n  uint uStack_120;\r\n  undefined1 auStack_108 [200];\r\n  \r\n  FUN_1408555a0();\r\n  lVar3 = *(longlong *)(param_1 + 0x10);\r\n  if (((*(longlong *)(lVar3 + 0x90) == 0) ||\r\n      (lVar4 = *(longlong *)(*(longlong *)(lVar3 + 0x90) + 0x68), lVar4 == 0)) ||\r\n     (*(float *)(lVar4 + 0x74) <= 0.0)) {\r\n    lVar4 = *(longlong *)(param_1 + 0x18);\r\n    uVar18 = *(uint *)(param_1 + 0xf8) & 0xffffff;\r\n    if (uVar18 != 0) {\r\n      lVar17 = (longlong)(int)uVar18 * 4;\r\n      do {\r\n        uVar18 = uVar18 - 1;\r\n        lVar17 = lVar17 + -4;\r\n        lVar8 = FUN_1406dc560(*(undefined4 *)\r\n                               (*(longlong *)(param_1 + 0xf0) + (longlong)(int)uVar18 * 4));\r\n        if (lVar8 == 0) {\r\n          if (((-1 < (int)uVar18) &&\r\n              (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&\r\n             (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,\r\n             lVar8 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {\r\n            do {\r\n              uVar14 = uVar14 + 1;\r\n              lVar8 = lVar8 + 4;\r\n              *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar8) =\r\n                   *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar8);\r\n            } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));\r\n          }\r\n        }\r\n        else {\r\n          puVar9 = (undefined8 *)FUN_140877720(lVar8,&DAT_1439c5a30,2);\r\n          if (puVar9 == (undefined8 *)0x0) {\r\n            if (((-1 < (int)uVar18) &&\r\n                (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&\r\n               (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,\r\n               lVar8 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {\r\n              do {\r\n                uVar14 = uVar14 + 1;\r\n                lVar8 = lVar8 + 4;\r\n                *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar8) =\r\n                     *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar8);\r\n              } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));\r\n            }\r\n          }\r\n          else {\r\n            lVar11 = puVar9[1];\r\n            lVar5 = *(longlong *)(lVar11 + 0xf8);\r\n            FUN_1408406b0(param_1,lVar8,&fStack_170,auStack_108);\r\n            if ((*(int *)(puVar9 + 10) == -1) || ((*(byte *)(puVar9 + 0x1a) & 4) != 0)) {\r\n              if (*(char *)(lVar4 + 0xac) == '\\0') {\r\n                lVar10 = (**(code **)*puVar9)(puVar9);\r\n                if ((*(int *)(lVar10 + 0x60) < DAT_1439aeaf0) ||\r\n                   (DAT_1439aeaf4 < *(int *)(lVar10 + 0x60))) goto LAB_1408416b5;\r\n              }\r\n            }\r\n            else {\r\nLAB_1408416b5:\r\n              fVar6 = fStack_168;\r\n              fVar19 = fStack_16c;\r\n              fVar1 = fStack_170;\r\n              fVar20 = fStack_16c * fStack_16c + fStack_170 * fStack_170 + fStack_168 * fStack_168;\r\n              if ((*(float *)(lVar4 + 0x90) * *(float *)(lVar4 + 0x90) <= fVar20) &&\r\n                 (lVar10 = FUN_140854880(param_1),\r\n                 fVar19 * *(float *)(lVar10 + 0x310) + fVar1 * *(float *)(lVar10 + 0x30c) +\r\n                 fVar6 * *(float *)(lVar10 + 0x314) <= 0.0)) {\r\n                lVar11 = FUN_140877530(lVar8);\r\n                if (lVar11 == 0) {\r\n                  if ((*(int *)(puVar9 + 10) == -1) || ((*(byte *)(puVar9 + 0x1a) & 4) != 0)) {\r\n                    if (fVar20 <= 1.1754944e-38) {\r\n                      fVar20 = 1.1754944e-38;\r\n                    }\r\n                    fStack_168 = 1.0 / SQRT(fVar20);\r\n                    fStack_170 = fVar1 * fStack_168;\r\n                    fStack_16c = fVar19 * fStack_168;\r\n                    fStack_168 = fVar6 * fStack_168;\r\n                    if ((*(byte *)(param_1 + 0x140) & 2) == 0) {\r\n                      fStack_140 = *(float *)(lVar4 + 0x94);\r\n                    }\r\n                    else {\r\n                      fStack_140 = *(float *)(lVar4 + 0x98);\r\n                    }\r\n                    fStack_148 = fStack_170 * fStack_140;\r\n                    fStack_144 = fStack_16c * fStack_140;\r\n                    fStack_140 = fStack_168 * fStack_140;\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    FUN_140752870(uVar12,\"arkPlayerStateLootObject\",2,&fStack_148);\r\n                    if ((*(byte *)(param_1 + 0x140) & 2) == 0) {\r\n                      pfVar15 = &fStack_160;\r\n                      fStack_160 = DAT_1431af668;\r\n                      uStack_15c = DAT_1431af66c;\r\n                      uStack_158 = DAT_1431af670;\r\n                    }\r\n                    else {\r\n                      fVar1 = *(float *)(lVar4 + 0x9c);\r\n                      lVar11 = FUN_140854880(param_1);\r\n                      pfVar15 = &fStack_154;\r\n                      fVar19 = -DAT_14228a75c;\r\n                      fStack_154 = fVar1 * *(float *)(lVar11 + 0x318) * fVar19;\r\n                      fStack_150 = fVar1 * *(float *)(lVar11 + 0x31c) * fVar19;\r\n                      fStack_14c = fVar1 * *(float *)(lVar11 + 800) * fVar19;\r\n                    }\r\n                    fStack_138 = *pfVar15;\r\n                    fStack_134 = pfVar15[1];\r\n                    fStack_130 = pfVar15[2];\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    pfVar15 = &fStack_138;\r\n                  }\r\n                  else {\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    FUN_140752870(uVar12,\"arkPlayerStateLootObject\",2,&DAT_1431af668);\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    pfVar15 = &DAT_1431af668;\r\n                  }\r\n                  FUN_1407527e0(uVar12,\"arkPlayerStateLootObject\",2,pfVar15);\r\n                }\r\n                goto LAB_140841ce8;\r\n              }\r\n            }\r\n            FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar8);\r\n            if (((-1 < (int)uVar18) &&\r\n                (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&\r\n               (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,\r\n               lVar10 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {\r\n              do {\r\n                uVar14 = uVar14 + 1;\r\n                lVar10 = lVar10 + 4;\r\n                *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar10) =\r\n                     *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar10);\r\n              } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));\r\n            }\r\n            bVar16 = *(byte *)(param_1 + 0x140) & 1;\r\n            if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {\r\n              if ((bVar16 != 0) &&\r\n                 (lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130)), lVar10 != 0)) {\r\n                if (lVar5 != 0) {\r\n                  lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));\r\n                  uVar2 = *(undefined4 *)(lVar10 + 0xa8);\r\n                  lVar10 = FUN_140810240();\r\n                  plVar13 = (longlong *)FUN_1408112c0(lVar10 + 0xc8e8,0x1c4,8,uVar2);\r\n                  if (plVar13 != (longlong *)0x0) {\r\n                    *plVar13 = lVar5;\r\n                  }\r\n                }\r\n                *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) & 0xfe;\r\n                *(undefined4 *)(param_1 + 0x130) = 0x3fff0010;\r\n              }\r\n              FUN_140854190(param_1);\r\n            }\r\n            if ((lVar5 == 0) || (*(int *)(lVar5 + 0xc4) == -1)) {\r\n              if (*(char *)(lVar11 + 0xa0) == '\\0') {\r\n                if (*(longlong *)(lVar11 + 0xf8) == 0) {\r\n                  uStack_120 = *(byte *)(param_1 + 0x140) >> 1 & 1;\r\n                  auStack_128[0] = 0x69;\r\n                  auStack_188[0] = CONCAT31(auStack_188[0]._1_3_,0x69);\r\n                  auStack_188[2] = 0;\r\n                  FUN_1408848a0(puVar9[2],auStack_108,&DAT_1439acd40,auStack_188,auStack_128);\r\n                  FUN_140882fc0(auStack_108);\r\n                  FUN_14076ec20(puVar9,1);\r\n                }\r\n                else {\r\n                  FUN_140841030(param_1,puVar9,bVar16);\r\n                  auStack_188[2] = *(byte *)(param_1 + 0x140) >> 1 & 1;\r\n                  auStack_188[0] = CONCAT31(auStack_188[0]._1_3_,0x69);\r\n                  auStack_128[0] = 0x69;\r\n                  uStack_120 = 1;\r\n                  FUN_1408848a0(puVar9[2],auStack_108,&DAT_1439acd40,auStack_128,auStack_188);\r\n                  FUN_140882fc0(auStack_108);\r\n                  FUN_14076ec20(puVar9,1);\r\n                }\r\n              }\r\n              else {\r\n                uStack_178 = 0;\r\n                auStack_188[2] = *(uint *)(lVar8 + 0xa8);\r\n                *(byte *)(lVar8 + 0xd1) =\r\n                     *(char *)(lVar11 + 0x118) << 7 | *(byte *)(lVar8 + 0xd1) & 0x7f;\r\n                auStack_188[3] = *(undefined4 *)(lVar11 + 0xa4);\r\n                if (*(char *)(lVar11 + 0xa8) == '\\0') {\r\n                  auStack_188[1] = 0x31;\r\n                  uVar12 = 0x31;\r\n                }\r\n                else {\r\n                  auStack_188[1] = 0x32;\r\n                  uVar12 = 0x32;\r\n                }\r\n                DAT_143994a30 = DAT_143994a30 + 1 & 0x7fffffff;\r\n                if (DAT_143994a30 == 0) {\r\n                  DAT_143994a30 = 1;\r\n                }\r\n                auStack_188[0] = DAT_143994a30;\r\n                FUN_1408024d0(lVar3,uVar12,auStack_188);\r\n                uVar2 = *(undefined4 *)(lVar11 + 0xb8);\r\n                lVar8 = FUN_1403da910(param_1 + 0x100,2);\r\n                if (lVar8 == 0) goto LAB_140841a3f;\r\n                FUN_1404c2690(lVar8,uVar2);\r\n                FUN_14076ec20(puVar9,0);\r\n              }\r\n            }\r\n            else {\r\n              FUN_1403b7aa0(auStack_108,puVar9,lVar11 + 200,0);\r\n              FUN_1408024d0(lVar3,0x34,auStack_108);\r\nLAB_140841a3f:\r\n              FUN_14076ec20(puVar9,0);\r\n            }\r\n          }\r\n        }\r\nLAB_140841ce8:\r\n      } while (uVar18 != 0);\r\n    }\r\n    if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {\r\n      FUN_140854190(param_1);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140841500(longlong param_1)\r\n\r\n{\r\n  float fVar1;\r\n  undefined4 uVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  float fVar6;\r\n  uint uVar7;\r\n  longlong lVar8;\r\n  undefined8 *puVar9;\r\n  longlong lVar10;\r\n  longlong lVar11;\r\n  undefined8 uVar12;\r\n  longlong *plVar13;\r\n  uint uVar14;\r\n  float *pfVar15;\r\n  byte bVar16;\r\n  longlong lVar17;\r\n  uint uVar18;\r\n  float fVar19;\r\n  float fVar20;\r\n  uint auStack_188 [4];\r\n  undefined1 uStack_178;\r\n  float fStack_170;\r\n  float fStack_16c;\r\n  float fStack_168;\r\n  float fStack_160;\r\n  undefined4 uStack_15c;\r\n  undefined4 uStack_158;\r\n  float fStack_154;\r\n  float fStack_150;\r\n  float fStack_14c;\r\n  float fStack_148;\r\n  float fStack_144;\r\n  float fStack_140;\r\n  float fStack_138;\r\n  float fStack_134;\r\n  float fStack_130;\r\n  undefined1 auStack_128 [8];\r\n  uint uStack_120;\r\n  undefined1 auStack_108 [200];\r\n  \r\n  FUN_1408555a0();\r\n  lVar3 = *(longlong *)(param_1 + 0x10);\r\n  if (((*(longlong *)(lVar3 + 0x90) == 0) ||\r\n      (lVar4 = *(longlong *)(*(longlong *)(lVar3 + 0x90) + 0x68), lVar4 == 0)) ||\r\n     (*(float *)(lVar4 + 0x74) <= 0.0)) {\r\n    lVar4 = *(longlong *)(param_1 + 0x18);\r\n    uVar18 = *(uint *)(param_1 + 0xf8) & 0xffffff;\r\n    if (uVar18 != 0) {\r\n      lVar17 = (longlong)(int)uVar18 * 4;\r\n      do {\r\n        uVar18 = uVar18 - 1;\r\n        lVar17 = lVar17 + -4;\r\n        lVar8 = FUN_1406dc560(*(undefined4 *)\r\n                               (*(longlong *)(param_1 + 0xf0) + (longlong)(int)uVar18 * 4));\r\n        if (lVar8 == 0) {\r\n          if (((-1 < (int)uVar18) &&\r\n              (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&\r\n             (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,\r\n             lVar8 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {\r\n            do {\r\n              uVar14 = uVar14 + 1;\r\n              lVar8 = lVar8 + 4;\r\n              *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar8) =\r\n                   *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar8);\r\n            } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));\r\n          }\r\n        }\r\n        else {\r\n          puVar9 = (undefined8 *)FUN_140877720(lVar8,&DAT_1439c5a30,2);\r\n          if (puVar9 == (undefined8 *)0x0) {\r\n            if (((-1 < (int)uVar18) &&\r\n                (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&\r\n               (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,\r\n               lVar8 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {\r\n              do {\r\n                uVar14 = uVar14 + 1;\r\n                lVar8 = lVar8 + 4;\r\n                *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar8) =\r\n                     *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar8);\r\n              } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));\r\n            }\r\n          }\r\n          else {\r\n            lVar11 = puVar9[1];\r\n            lVar5 = *(longlong *)(lVar11 + 0xf8);\r\n            FUN_1408406b0(param_1,lVar8,&fStack_170,auStack_108);\r\n            if ((*(int *)(puVar9 + 10) == -1) || ((*(byte *)(puVar9 + 0x1a) & 4) != 0)) {\r\n              if (*(char *)(lVar4 + 0xac) == '\\0') {\r\n                lVar10 = (**(code **)*puVar9)(puVar9);\r\n                if ((*(int *)(lVar10 + 0x60) < DAT_1439aeaf0) ||\r\n                   (DAT_1439aeaf4 < *(int *)(lVar10 + 0x60))) goto LAB_1408416b5;\r\n              }\r\n            }\r\n            else {\r\nLAB_1408416b5:\r\n              fVar6 = fStack_168;\r\n              fVar19 = fStack_16c;\r\n              fVar1 = fStack_170;\r\n              fVar20 = fStack_16c * fStack_16c + fStack_170 * fStack_170 + fStack_168 * fStack_168;\r\n              if ((*(float *)(lVar4 + 0x90) * *(float *)(lVar4 + 0x90) <= fVar20) &&\r\n                 (lVar10 = FUN_140854880(param_1),\r\n                 fVar19 * *(float *)(lVar10 + 0x310) + fVar1 * *(float *)(lVar10 + 0x30c) +\r\n                 fVar6 * *(float *)(lVar10 + 0x314) <= 0.0)) {\r\n                lVar11 = FUN_140877530(lVar8);\r\n                if (lVar11 == 0) {\r\n                  if ((*(int *)(puVar9 + 10) == -1) || ((*(byte *)(puVar9 + 0x1a) & 4) != 0)) {\r\n                    if (fVar20 <= 1.1754944e-38) {\r\n                      fVar20 = 1.1754944e-38;\r\n                    }\r\n                    fStack_168 = 1.0 / SQRT(fVar20);\r\n                    fStack_170 = fVar1 * fStack_168;\r\n                    fStack_16c = fVar19 * fStack_168;\r\n                    fStack_168 = fVar6 * fStack_168;\r\n                    if ((*(byte *)(param_1 + 0x140) & 2) == 0) {\r\n                      fStack_140 = *(float *)(lVar4 + 0x94);\r\n                    }\r\n                    else {\r\n                      fStack_140 = *(float *)(lVar4 + 0x98);\r\n                    }\r\n                    fStack_148 = fStack_170 * fStack_140;\r\n                    fStack_144 = fStack_16c * fStack_140;\r\n                    fStack_140 = fStack_168 * fStack_140;\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    FUN_140752870(uVar12,\"arkPlayerStateLootObject\",2,&fStack_148);\r\n                    if ((*(byte *)(param_1 + 0x140) & 2) == 0) {\r\n                      pfVar15 = &fStack_160;\r\n                      fStack_160 = DAT_1431af668;\r\n                      uStack_15c = DAT_1431af66c;\r\n                      uStack_158 = DAT_1431af670;\r\n                    }\r\n                    else {\r\n                      fVar1 = *(float *)(lVar4 + 0x9c);\r\n                      lVar11 = FUN_140854880(param_1);\r\n                      pfVar15 = &fStack_154;\r\n                      fVar19 = -DAT_14228a75c;\r\n                      fStack_154 = fVar1 * *(float *)(lVar11 + 0x318) * fVar19;\r\n                      fStack_150 = fVar1 * *(float *)(lVar11 + 0x31c) * fVar19;\r\n                      fStack_14c = fVar1 * *(float *)(lVar11 + 800) * fVar19;\r\n                    }\r\n                    fStack_138 = *pfVar15;\r\n                    fStack_134 = pfVar15[1];\r\n                    fStack_130 = pfVar15[2];\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    pfVar15 = &fStack_138;\r\n                  }\r\n                  else {\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    FUN_140752870(uVar12,\"arkPlayerStateLootObject\",2,&DAT_1431af668);\r\n                    uVar12 = FUN_1408783d0(lVar8);\r\n                    pfVar15 = &DAT_1431af668;\r\n                  }\r\n                  FUN_1407527e0(uVar12,\"arkPlayerStateLootObject\",2,pfVar15);\r\n                }\r\n                goto LAB_140841ce8;\r\n              }\r\n            }\r\n            FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar8);\r\n            if (((-1 < (int)uVar18) &&\r\n                (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&\r\n               (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,\r\n               lVar10 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {\r\n              do {\r\n                uVar14 = uVar14 + 1;\r\n                lVar10 = lVar10 + 4;\r\n                *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar10) =\r\n                     *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar10);\r\n              } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));\r\n            }\r\n            bVar16 = *(byte *)(param_1 + 0x140) & 1;\r\n            if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {\r\n              if ((bVar16 != 0) &&\r\n                 (lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130)), lVar10 != 0)) {\r\n                if (lVar5 != 0) {\r\n                  lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));\r\n                  uVar2 = *(undefined4 *)(lVar10 + 0xa8);\r\n                  lVar10 = FUN_140810240();\r\n                  plVar13 = (longlong *)FUN_1408112c0(lVar10 + 0xc8e8,0x1c4,8,uVar2);\r\n                  if (plVar13 != (longlong *)0x0) {\r\n                    *plVar13 = lVar5;\r\n                  }\r\n                }\r\n                *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) & 0xfe;\r\n                *(undefined4 *)(param_1 + 0x130) = 0x3fff0010;\r\n              }\r\n              FUN_140854190(param_1);\r\n            }\r\n            if ((lVar5 == 0) || (*(int *)(lVar5 + 0xc4) == -1)) {\r\n              if (*(char *)(lVar11 + 0xa0) == '\\0') {\r\n                if (*(longlong *)(lVar11 + 0xf8) == 0) {\r\n                  uStack_120 = *(byte *)(param_1 + 0x140) >> 1 & 1;\r\n                  auStack_128[0] = 0x69;\r\n                  auStack_188[0] = CONCAT31(auStack_188[0]._1_3_,0x69);\r\n                  auStack_188[2] = 0;\r\n                  FUN_1408848a0(puVar9[2],auStack_108,&DAT_1439acd40,auStack_188,auStack_128);\r\n                  FUN_140882fc0(auStack_108);\r\n                  FUN_14076ec20(puVar9,1);\r\n                }\r\n                else {\r\n                  FUN_140841030(param_1,puVar9,bVar16);\r\n                  auStack_188[2] = *(byte *)(param_1 + 0x140) >> 1 & 1;\r\n                  auStack_188[0] = CONCAT31(auStack_188[0]._1_3_,0x69);\r\n                  auStack_128[0] = 0x69;\r\n                  uStack_120 = 1;\r\n                  FUN_1408848a0(puVar9[2],auStack_108,&DAT_1439acd40,auStack_128,auStack_188);\r\n                  FUN_140882fc0(auStack_108);\r\n                  FUN_14076ec20(puVar9,1);\r\n                }\r\n              }\r\n              else {\r\n                uStack_178 = 0;\r\n                auStack_188[2] = *(uint *)(lVar8 + 0xa8);\r\n                *(byte *)(lVar8 + 0xd1) =\r\n                     *(char *)(lVar11 + 0x118) << 7 | *(byte *)(lVar8 + 0xd1) & 0x7f;\r\n                auStack_188[3] = *(undefined4 *)(lVar11 + 0xa4);\r\n                if (*(char *)(lVar11 + 0xa8) == '\\0') {\r\n                  auStack_188[1] = 0x31;\r\n                  uVar12 = 0x31;\r\n                }\r\n                else {\r\n                  auStack_188[1] = 0x32;\r\n                  uVar12 = 0x32;\r\n                }\r\n                DAT_143994a30 = DAT_143994a30 + 1 & 0x7fffffff;\r\n                if (DAT_143994a30 == 0) {\r\n                  DAT_143994a30 = 1;\r\n                }\r\n                auStack_188[0] = DAT_143994a30;\r\n                FUN_1408024d0(lVar3,uVar12,auStack_188);\r\n                uVar2 = *(undefined4 *)(lVar11 + 0xb8);\r\n                lVar8 = FUN_1403da910(param_1 + 0x100,2);\r\n                if (lVar8 == 0) goto LAB_140841a3f;\r\n                FUN_1404c2690(lVar8,uVar2);\r\n                FUN_14076ec20(puVar9,0);\r\n              }\r\n            }\r\n            else {\r\n              FUN_1403b7aa0(auStack_108,puVar9,lVar11 + 200,0);\r\n              FUN_1408024d0(lVar3,0x34,auStack_108);\r\nLAB_140841a3f:\r\n              FUN_14076ec20(puVar9,0);\r\n            }\r\n          }\r\n        }\r\nLAB_140841ce8:\r\n      } while (uVar18 != 0);\r\n    }\r\n    if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {\r\n      FUN_140854190(param_1);\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-140841030": {
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
  "pickup786-decompile-14076ec20": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14076ec20(longlong param_1,char param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  \r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfd;\r\n  if (param_2 != '\\0') {\r\n    (**(code **)(**(longlong **)(param_1 + 0x10) + 0x80))();\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 8;\r\n    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n    *pbVar1 = *pbVar1 | 4;\r\n    return;\r\n  }\r\n  if ((*(byte *)(param_1 + 0xd0) & 4) != 0) {\r\n    FUN_1407e77e0();\r\n  }\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n  *pbVar1 = *pbVar1 | 4;\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14076ec20(longlong param_1,char param_2)\r\n\r\n{\r\n  byte *pbVar1;\r\n  \r\n  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfd;\r\n  if (param_2 != '\\0') {\r\n    (**(code **)(**(longlong **)(param_1 + 0x10) + 0x80))();\r\n    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 8;\r\n    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n    *pbVar1 = *pbVar1 | 4;\r\n    return;\r\n  }\r\n  if ((*(byte *)(param_1 + 0xd0) & 4) != 0) {\r\n    FUN_1407e77e0();\r\n  }\r\n  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);\r\n  *pbVar1 = *pbVar1 | 4;\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-14076ec80": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14076ec80(longlong param_1)\r\n\r\n{\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  \r\n  if ((*(byte *)(param_1 + 0xd0) & 1) != 0) {\r\n    plVar1 = (longlong *)FUN_1403da910(param_1 + 0x50,2);\r\n    if (plVar1 != (longlong *)0x0) {\r\n      (**(code **)(*plVar1 + 0x110))(plVar1);\r\n      *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfe;\r\n    }\r\n  }\r\n  lVar2 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439dcb80,2);\r\n  if (lVar2 != 0) {\r\n    FUN_14070b320(lVar2,0);\r\n    return;\r\n  }\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14076ec80(longlong param_1)\r\n\r\n{\r\n  longlong *plVar1;\r\n  longlong lVar2;\r\n  \r\n  if ((*(byte *)(param_1 + 0xd0) & 1) != 0) {\r\n    plVar1 = (longlong *)FUN_1403da910(param_1 + 0x50,2);\r\n    if (plVar1 != (longlong *)0x0) {\r\n      (**(code **)(*plVar1 + 0x110))(plVar1);\r\n      *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfe;\r\n    }\r\n  }\r\n  lVar2 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439dcb80,2);\r\n  if (lVar2 != 0) {\r\n    FUN_14070b320(lVar2,0);\r\n    return;\r\n  }\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-1406156e0": {
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
  },
  "pickup786-decompile-140615620": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_140615620(longlong param_1,int param_2)\r\n\r\n{\r\n  undefined1 local_38 [8];\r\n  int local_30;\r\n  undefined1 local_20 [24];\r\n  \r\n  *(int *)(param_1 + 0x16c) = *(int *)(param_1 + 0x16c) + param_2;\r\n  local_38[0] = 0x69;\r\n  local_30 = param_2;\r\n  FUN_140884920(*(undefined8 *)(param_1 + 0x10),local_20,&DAT_1439ab8f0,local_38);\r\n  FUN_140882fc0(local_20);\r\n  FUN_14067aea0(D2_GameSingletonPtr + 0x1f77c8,1,param_2);\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_140615620(longlong param_1,int param_2)\r\n\r\n{\r\n  undefined1 local_38 [8];\r\n  int local_30;\r\n  undefined1 local_20 [24];\r\n  \r\n  *(int *)(param_1 + 0x16c) = *(int *)(param_1 + 0x16c) + param_2;\r\n  local_38[0] = 0x69;\r\n  local_30 = param_2;\r\n  FUN_140884920(*(undefined8 *)(param_1 + 0x10),local_20,&DAT_1439ab8f0,local_38);\r\n  FUN_140882fc0(local_20);\r\n  FUN_14067aea0(D2_GameSingletonPtr + 0x1f77c8,1,param_2);\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup786-decompile-140615d40": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nundefined8 FUN_140615d40(undefined8 param_1,longlong param_2)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  \r\n  if ((*(uint *)(param_2 + 0x60) < 0x12) &&\r\n     ((0x38022U >> (*(uint *)(param_2 + 0x60) & 0x1f) & 1) != 0)) {\r\n    return 0;\r\n  }\r\n  iVar1 = FUN_140614f30(param_1);\r\n  if (-1 < iVar1) {\r\n    uVar2 = FUN_140615d90(param_1,iVar1);\r\n    return uVar2;\r\n  }\r\n  return 1;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nundefined8 FUN_140615d40(undefined8 param_1,longlong param_2)\r\n\r\n{\r\n  int iVar1;\r\n  undefined8 uVar2;\r\n  \r\n  if ((*(uint *)(param_2 + 0x60) < 0x12) &&\r\n     ((0x38022U >> (*(uint *)(param_2 + 0x60) & 0x1f) & 1) != 0)) {\r\n    return 0;\r\n  }\r\n  iVar1 = FUN_140614f30(param_1);\r\n  if (-1 < iVar1) {\r\n    uVar2 = FUN_140615d90(param_1,iVar1);\r\n    return uVar2;\r\n  }\r\n  return 1;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup785-consumption-xrefs": {
    "content": [
      {
        "type": "text",
        "text": "From 143f2ee34 [DATA]\nFrom 14083fa76 in FUN_14083f590 [UNCONDITIONAL_CALL]\nFrom 140844d18 [UNCONDITIONAL_CALL]"
      }
    ],
    "structuredContent": {
      "result": "From 143f2ee34 [DATA]\nFrom 14083fa76 in FUN_14083f590 [UNCONDITIONAL_CALL]\nFrom 140844d18 [UNCONDITIONAL_CALL]"
    },
    "isError": false
  },
  "pickup785-loot-error-xrefs": {
    "content": [
      {
        "type": "text",
        "text": "From 140743cba in FUN_140743c30 [DATA]"
      }
    ],
    "structuredContent": {
      "result": "From 140743cba in FUN_140743c30 [DATA]"
    },
    "isError": false
  },
  "pickup785-grant0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "From 143f3da98 [DATA]\nFrom 141c7bd20 [DATA]"
        }
      ],
      "structuredContent": {
        "result": "From 143f3da98 [DATA]\nFrom 141c7bd20 [DATA]"
      },
      "isError": false
    }
  },
  "pickup785-grant1": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_14080cd60(undefined8 param_1,longlong param_2,longlong param_3)\r\n\r\n{\r\n  byte bVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  undefined8 uVar6;\r\n  longlong *plVar7;\r\n  ulonglong uVar8;\r\n  ulonglong uVar9;\r\n  longlong local_res18;\r\n  \r\n  bVar1 = *(byte *)(param_2 + 0xd1) >> 2 & 1;\r\n  local_res18 = param_3;\r\n  if ((bVar1 != 0) || ((*(byte *)(param_3 + 0xd1) & 4) != 0)) {\r\n    if (bVar1 == 0) {\r\n      param_2 = param_3;\r\n    }\r\n    uVar5 = D2_InternedName_Resolve(param_2 + 0x20);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(\"[arkTimeManager::CreateTimeSpaceAttachmentBetweenEntities] %s doesn\\'t allow sync time space\\n\"\r\n                  ,uVar5);\r\n  }\r\n  lVar3 = FUN_140877500(param_2);\r\n  lVar4 = FUN_140877500(param_3);\r\n  if (lVar3 == lVar4) {\r\n    uVar5 = D2_InternedName_Resolve(param_3 + 0x20);\r\n    uVar6 = D2_InternedName_Resolve(param_2 + 0x20);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(\"[arkTimeManager::CreateTimeSpaceAttachmentBetweenEntities] %s and %s are already attach\\n\"\r\n                  ,uVar6,uVar5);\r\n  }\r\n  if (*(longlong *)(param_3 + 0x2b8) != 0) {\r\n    FUN_14080e050(param_1);\r\n  }\r\n  *(longlong *)(param_3 + 0x2b8) = param_2;\r\n  iVar2 = FUN_1401581c0(&DAT_1439921f4,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(&DAT_1439921f4,0,1);\r\n  }\r\n  uVar8 = 0;\r\n  iVar2 = (*(uint *)(param_2 + 0x2c8) & 0xffffff) - 1;\r\n  if (-1 < iVar2) {\r\n    plVar7 = *(longlong **)(param_2 + 0x2c0);\r\n    uVar9 = uVar8;\r\n    do {\r\n      if (*plVar7 == param_3) {\r\n        if (-1 < (int)uVar9) goto LAB_14080cde1;\r\n        break;\r\n      }\r\n      uVar8 = uVar8 + 1;\r\n      uVar9 = (ulonglong)((int)uVar9 + 1);\r\n      plVar7 = plVar7 + 1;\r\n    } while ((longlong)uVar8 <= (longlong)iVar2);\r\n  }\r\n  FUN_1408918f0(param_2 + 0x2c0,&local_res18);\r\nLAB_14080cde1:\r\n  FUN_1401581e0(&DAT_1439921f4,0);\r\n  return;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_14080cd60(undefined8 param_1,longlong param_2,longlong param_3)\r\n\r\n{\r\n  byte bVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  undefined8 uVar6;\r\n  longlong *plVar7;\r\n  ulonglong uVar8;\r\n  ulonglong uVar9;\r\n  longlong local_res18;\r\n  \r\n  bVar1 = *(byte *)(param_2 + 0xd1) >> 2 & 1;\r\n  local_res18 = param_3;\r\n  if ((bVar1 != 0) || ((*(byte *)(param_3 + 0xd1) & 4) != 0)) {\r\n    if (bVar1 == 0) {\r\n      param_2 = param_3;\r\n    }\r\n    uVar5 = D2_InternedName_Resolve(param_2 + 0x20);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(\"[arkTimeManager::CreateTimeSpaceAttachmentBetweenEntities] %s doesn\\'t allow sync time space\\n\"\r\n                  ,uVar5);\r\n  }\r\n  lVar3 = FUN_140877500(param_2);\r\n  lVar4 = FUN_140877500(param_3);\r\n  if (lVar3 == lVar4) {\r\n    uVar5 = D2_InternedName_Resolve(param_3 + 0x20);\r\n    uVar6 = D2_InternedName_Resolve(param_2 + 0x20);\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(\"[arkTimeManager::CreateTimeSpaceAttachmentBetweenEntities] %s and %s are already attach\\n\"\r\n                  ,uVar6,uVar5);\r\n  }\r\n  if (*(longlong *)(param_3 + 0x2b8) != 0) {\r\n    FUN_14080e050(param_1);\r\n  }\r\n  *(longlong *)(param_3 + 0x2b8) = param_2;\r\n  iVar2 = FUN_1401581c0(&DAT_1439921f4,0,1);\r\n  while (iVar2 == 1) {\r\n    iVar2 = FUN_1401581c0(&DAT_1439921f4,0,1);\r\n  }\r\n  uVar8 = 0;\r\n  iVar2 = (*(uint *)(param_2 + 0x2c8) & 0xffffff) - 1;\r\n  if (-1 < iVar2) {\r\n    plVar7 = *(longlong **)(param_2 + 0x2c0);\r\n    uVar9 = uVar8;\r\n    do {\r\n      if (*plVar7 == param_3) {\r\n        if (-1 < (int)uVar9) goto LAB_14080cde1;\r\n        break;\r\n      }\r\n      uVar8 = uVar8 + 1;\r\n      uVar9 = (ulonglong)((int)uVar9 + 1);\r\n      plVar7 = plVar7 + 1;\r\n    } while ((longlong)uVar8 <= (longlong)iVar2);\r\n  }\r\n  FUN_1408918f0(param_2 + 0x2c0,&local_res18);\r\nLAB_14080cde1:\r\n  FUN_1401581e0(&DAT_1439921f4,0);\r\n  return;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  "pickup785-grant2": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "From 1414b12cf in FUN_141413800 [DATA]"
        }
      ],
      "structuredContent": {
        "result": "From 1414b12cf in FUN_141413800 [DATA]"
      },
      "isError": false
    }
  },
  "pickup786-start0": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "{\"address\":\"141c7bc40\",\"length\":384,\"data\":[48,12,1,64,1,0,0,0,224,21,1,64,1,0,0,0,16,5,132,64,1,0,0,0,112,54,2,64,1,0,0,0,32,150,57,64,1,0,0,0,112,44,2,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,192,83,133,64,1,0,0,0,128,53,2,64,1,0,0,0,176,152,9,64,1,0,0,0,224,56,1,64,1,0,0,0,80,62,0,64,1,0,0,0,32,150,57,64,1,0,0,0,16,254,131,64,1,0,0,0,64,208,6,64,1,0,0,0,120,22,254,65,1,0,0,0,144,119,147,64,1,0,0,0,48,245,131,64,1,0,0,0,32,150,57,64,1,0,0,0,176,152,9,64,1,0,0,0,0,253,131,64,1,0,0,0,96,81,133,64,1,0,0,0,48,82,133,64,1,0,0,0,32,11,132,64,1,0,0,0,112,85,133,64,1,0,0,0,0,77,133,64,1,0,0,0,80,78,133,64,1,0,0,0,144,245,131,64,1,0,0,0,112,254,131,64,1,0,0,0,0,21,132,64,1,0,0,0,128,22,104,64,1,0,0,0,64,73,133,64,1,0,0,0,96,83,133,64,1,0,0,0,176,152,9,64,1,0,0,0,192,44,104,64,1,0,0,0,144,39,104,64,1,0,0,0,64,43,104,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,32,82,133,64,1,0,0,0,80,84,133,64,1,0,0,0,0,82,133,64,1,0,0,0,64,41,104,64,1,0,0,0,80,41,104,64,1,0,0,0,96,41,104,64,1,0,0,0,32,150,57,64,1,0,0,0],\"hex\":\"300c014001000000e015014001000000100584400100000070360240010000002096394001000000702c024001000000b098094001000000b098094001000000c0538540010000008035024001000000b098094001000000e038014001000000503e004001000000209639400100000010fe83400100000040d00640010000007816fe4101000000907793400100000030f58340010000002096394001000000b09809400100000000fd83400100000060518540010000003052854001000000200b8440010000007055854001000000004d854001000000504e85400100000090f583400100000070fe8340010000000015844001000000801668400100000040498540010000006053854001000000b098094001000000c02c6840010000009027684001000000402b684001000000b098094001000000b098094001000000b0980940010000002052854001000000505485400100000000528540010000004029684001000000502968400100000060296840010000002096394001000000\"}"
        }
      ],
      "structuredContent": {
        "result": "{\"address\":\"141c7bc40\",\"length\":384,\"data\":[48,12,1,64,1,0,0,0,224,21,1,64,1,0,0,0,16,5,132,64,1,0,0,0,112,54,2,64,1,0,0,0,32,150,57,64,1,0,0,0,112,44,2,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,192,83,133,64,1,0,0,0,128,53,2,64,1,0,0,0,176,152,9,64,1,0,0,0,224,56,1,64,1,0,0,0,80,62,0,64,1,0,0,0,32,150,57,64,1,0,0,0,16,254,131,64,1,0,0,0,64,208,6,64,1,0,0,0,120,22,254,65,1,0,0,0,144,119,147,64,1,0,0,0,48,245,131,64,1,0,0,0,32,150,57,64,1,0,0,0,176,152,9,64,1,0,0,0,0,253,131,64,1,0,0,0,96,81,133,64,1,0,0,0,48,82,133,64,1,0,0,0,32,11,132,64,1,0,0,0,112,85,133,64,1,0,0,0,0,77,133,64,1,0,0,0,80,78,133,64,1,0,0,0,144,245,131,64,1,0,0,0,112,254,131,64,1,0,0,0,0,21,132,64,1,0,0,0,128,22,104,64,1,0,0,0,64,73,133,64,1,0,0,0,96,83,133,64,1,0,0,0,176,152,9,64,1,0,0,0,192,44,104,64,1,0,0,0,144,39,104,64,1,0,0,0,64,43,104,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,32,82,133,64,1,0,0,0,80,84,133,64,1,0,0,0,0,82,133,64,1,0,0,0,64,41,104,64,1,0,0,0,80,41,104,64,1,0,0,0,96,41,104,64,1,0,0,0,32,150,57,64,1,0,0,0],\"hex\":\"300c014001000000e015014001000000100584400100000070360240010000002096394001000000702c024001000000b098094001000000b098094001000000c0538540010000008035024001000000b098094001000000e038014001000000503e004001000000209639400100000010fe83400100000040d00640010000007816fe4101000000907793400100000030f58340010000002096394001000000b09809400100000000fd83400100000060518540010000003052854001000000200b8440010000007055854001000000004d854001000000504e85400100000090f583400100000070fe8340010000000015844001000000801668400100000040498540010000006053854001000000b098094001000000c02c6840010000009027684001000000402b684001000000b098094001000000b098094001000000b0980940010000002052854001000000505485400100000000528540010000004029684001000000502968400100000060296840010000002096394001000000\"}"
      },
      "isError": false
    }
  },
  "pickup786-loot-xrefs": {
    "content": [
      {
        "type": "text",
        "text": "From 143f3da98 [DATA]\nFrom 141c7bd20 [DATA]"
      }
    ],
    "structuredContent": {
      "result": "From 143f3da98 [DATA]\nFrom 141c7bd20 [DATA]"
    },
    "isError": false
  }
}

````

## Unescaped text fields for reading

### text

````text

void FUN_14076e2b0(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  longlong *plVar4;
  undefined8 uVar5;
  longlong lVar6;
  
  lVar6 = *(longlong *)(param_1 + 0x10);
  lVar2 = *(longlong *)(param_1 + 8);
  lVar3 = FUN_140877530(lVar6);
  if (lVar3 != 0) {
    FUN_1408762b0(lVar3,lVar6);
    *(undefined4 *)(param_1 + 0x28) = *(undefined4 *)(lVar3 + 0xa8);
  }
  lVar3 = FUN_140877720(lVar6,&DAT_1439d77e0,1);
  if (lVar3 != 0) {
    FUN_1406274d0(lVar3);
  }
  lVar3 = FUN_140877720(lVar6,&DAT_1439e28e0,1);
  if (lVar3 != 0) {
    FUN_140491550(lVar3);
  }
  plVar4 = (longlong *)FUN_140877720(lVar6,&DAT_1439aee80,1);
  if (plVar4 != (longlong *)0x0) {
    (**(code **)(*plVar4 + 0x1a8))(plVar4);
  }
  lVar3 = FUN_140877720(lVar6,&DAT_1439bfba0,2);
  if (lVar3 != 0) {
    FUN_14043a670(lVar3);
  }
  lVar3 = FUN_140877720(lVar6,&DAT_1439c0140,2);
  if (lVar3 != 0) {
    FUN_14044a5f0(lVar3);
  }
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 8) + 0xf8);
  if (((lVar3 != 0) && (iVar1 = *(int *)(lVar3 + 0x60), (iVar1 - 5U & 0xfffffffc) == 0)) &&
     (iVar1 != 6)) {
    lVar3 = FUN_1408a8ba0(D2_GameSingletonPtr);
    if (lVar3 != 0) {
      uVar5 = FUN_140877720(lVar3,&DAT_1439e86d0,0);
      uVar5 = FUN_140777b90(uVar5,0x14);
      FUN_1403c4af0(uVar5,*(undefined8 *)(param_1 + 0x10));
    }
    FUN_1404e3050(param_1 + 0x80);
    FUN_1404e3050(param_1 + 0xa8);
  }
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfb | 2;
  FUN_1407e79a0(param_1);
  lVar2 = *(longlong *)(lVar2 + 0xf0);
  if (lVar2 != 0) {
    iVar1 = *(int *)(lVar6 + 0xa8);
    lVar6 = FUN_1403da910(param_1 + 0x40,2);
    if (lVar6 != 0) {
      FUN_140709110(lVar6,0xffffffff,lVar2,0,0,(longlong)iVar1,1,0);
    }
  }
  return;
}


````

### text

````text

void FUN_14076eeb0(longlong param_1)

{
  longlong lVar1;
  longlong *plVar2;
  undefined8 uVar3;
  undefined1 local_18 [16];
  
  lVar1 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439dcb80,2);
  if (lVar1 != 0) {
    FUN_14070b320(lVar1,0x3f733333);
  }
  if (((*(byte *)(param_1 + 0xd0) & 1) == 0) &&
     (plVar2 = (longlong *)FUN_1403da910(param_1 + 0x50,2), plVar2 != (longlong *)0x0)) {
    lVar1 = *plVar2;
    uVar3 = FUN_140088f70(local_18);
    (**(code **)(lVar1 + 0x108))(plVar2,0x17,uVar3);
    (**(code **)(*plVar2 + 0x188))(plVar2,1);
    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 1;
  }
  return;
}


````

### text

````text

void FUN_14076efa0(longlong param_1)

{
  longlong *plVar1;
  
  if ((((*(byte *)(param_1 + 0xd0) & 4) == 0) &&
      (plVar1 = (longlong *)FUN_1403da910(param_1 + 0x50,2), plVar1 != (longlong *)0x0)) &&
     (FUN_14076eeb0(param_1), (*(byte *)(param_1 + 0xd0) & 1) != 0)) {
    (**(code **)(*plVar1 + 0x130))(plVar1);
    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 4;
  }
  if (((*(byte *)(param_1 + 0xd0) & 2) == 0) && ((*(byte *)(param_1 + 0xd0) & 8) == 0)) {
    D2_Component_Pause(param_1,*(undefined8 *)(D2_GameSingletonPtr + 0x1e8960));
    return;
  }
  return;
}


````

### text

````text

void FUN_14076ee70(longlong *param_1,longlong param_2)

{
  FUN_1403dab80(param_1 + 6);
  if (7 < ((byte)*(undefined4 *)((longlong)param_1 + 0x1c) & 0xc)) {
    (**(code **)(*param_1 + 0xe8))(param_1,param_2 == 0);
  }
  return;
}


````

### text

````text

void FUN_14076ef60(longlong param_1)

{
  undefined *puStack_28;
  undefined *puStack_20;
  undefined1 uStack_18;
  
  FUN_140706f30(param_1,&DAT_1439e85b0,&UNK_141c5a528);
  puStack_28 = &DAT_1439d15f0;
  puStack_20 = &UNK_141c5a568;
  uStack_18 = 0;
  FUN_140706fa0(param_1 + 0x28,&puStack_28);
  return;
}


````

### text

````text

void FUN_14083f590(longlong param_1,longlong param_2)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  bool bVar6;
  undefined4 *puVar7;
  char cVar8;
  byte bVar9;
  char cVar10;
  undefined8 *puVar11;
  longlong lVar12;
  undefined8 uVar13;
  longlong lVar14;
  undefined1 *puVar15;
  longlong lVar16;
  undefined8 uVar17;
  longlong *plVar18;
  uint *puVar19;
  uint uVar20;
  int iVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  uint local_res18 [2];
  char local_res20;
  uint local_100;
  undefined4 *local_f8;
  uint local_f0;
  uint local_ec;
  undefined1 *local_e8;
  undefined4 local_e0 [2];
  ulonglong local_d8;
  undefined4 *local_d0;
  longlong local_c8;
  longlong local_c0;
  longlong local_b8;
  undefined1 local_b0 [8];
  uint local_a8;
  undefined1 local_98 [8];
  undefined4 local_90;
  undefined8 local_80;
  undefined1 local_78 [56];
  
  local_80 = 0xfffffffffffffffe;
  FUN_1409ffc00();
  puVar11 = (undefined8 *)FUN_1403da910(param_2 + 8,2);
  if (puVar11[2] == 0) {
    *(undefined4 *)(param_1 + 0x130) = 0x3fff0010;
  }
  else {
    *(undefined4 *)(param_1 + 0x130) = *(undefined4 *)(puVar11[2] + 0xa8);
  }
  *(byte *)(param_1 + 0x140) =
       (*(byte *)(param_2 + 0x41) & 1) * '\x02' | *(byte *)(param_1 + 0x140) & 0xfc;
  bVar6 = false;
  lVar12 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));
  if (((*(byte *)(lVar12 + 0xd1) & 0x20) == 0) && ((*(byte *)(lVar12 + 0xd1) & 8) == 0)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  lVar12 = (**(code **)*puVar11)(puVar11);
  if ((DAT_1439d14a0 <= *(int *)(lVar12 + 0x60)) && (*(int *)(lVar12 + 0x60) <= DAT_1439d14a4)) {
    bVar6 = true;
    if (bVar1) {
LAB_14083f684:
      bVar9 = 0;
    }
    else {
      uVar13 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));
      cVar8 = FUN_140878c70(uVar13);
      if (cVar8 != '\0') goto LAB_14083f684;
      bVar9 = 1;
    }
    *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) & 0xfe;
    *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) | bVar9;
  }
  local_c8 = *(longlong *)(param_1 + 0x18);
  lVar12 = FUN_140877720(puVar11[2],&DAT_1439c5880,2);
  local_b8 = lVar12;
  local_c0 = FUN_140877720(puVar11[2],&DAT_1439dcd30,2);
  local_f8 = (undefined4 *)0x0;
  FUN_1401548e0();
  local_f8 = (undefined4 *)0x0;
  local_100 = 0;
  local_ec = 0x3f;
  local_f0 = 0x5000000;
  if ((*(byte *)(param_1 + 0x140) & 1) == 0) {
    if ((lVar12 != 0) && (iVar21 = 0, (*(uint *)(lVar12 + 0x30) & 0xffffff) != 0)) {
      lVar14 = 0;
      do {
        lVar16 = *(longlong *)(lVar12 + 0x28);
        if ((*(char *)(lVar14 + 0x11 + lVar16) == '\0') &&
           (((*(char *)(lVar14 + 0x10 + lVar16) == '\0' || (bVar1)) &&
            (lVar16 = FUN_1406dc560(*(undefined4 *)(lVar14 + lVar16)), lVar16 != 0)))) {
          local_res18[0] = *(uint *)(lVar16 + 0xa8);
          FUN_14052e460(&local_f8,local_res18);
        }
        iVar21 = iVar21 + 1;
        lVar14 = lVar14 + 0x18;
      } while (iVar21 < (int)(*(uint *)(lVar12 + 0x30) & 0xffffff));
    }
    puVar19 = (uint *)(param_1 + 0x130);
  }
  else {
    lVar12 = FUN_1408400b0(param_1,puVar11);
    uVar20 = 0;
    if (lVar12 == 0) goto LAB_14083f7dc;
    local_res18[0] = *(uint *)(lVar12 + 0xa8);
    puVar19 = local_res18;
  }
  FUN_14052e460(&local_f8,puVar19);
  local_100 = local_ec;
  uVar20 = local_f0;
LAB_14083f7dc:
  local_res18[0] = local_res18[0] & 0xffffff00;
  lVar12 = *(longlong *)(param_1 + 0x10);
  if (*(int *)(*(longlong *)(lVar12 + 0x2f8) + 0x5d0) != 0xe) {
    cVar8 = FUN_1408241b0(*(longlong *)(lVar12 + 0x2f8) + 0x410);
    local_res18[0] = CONCAT31(local_res18[0]._1_3_,cVar8 == '\0');
  }
  if (*(int *)(param_1 + 0x13c) < 0) {
    fVar22 = 0.0;
  }
  else {
    fVar22 = *(float *)(*(longlong *)(*(longlong *)(lVar12 + 0x330) + 0x48) + 0x24 +
                       (longlong)*(int *)(param_1 + 0x13c) * 0x28);
  }
  local_res20 = 0.0 < fVar22;
  if ((uVar20 & 0xffffff) != 0) {
    local_d8 = (ulonglong)(uVar20 & 0xffffff);
    local_d0 = local_f8;
    do {
      lVar14 = FUN_1406dc560(*local_d0);
      puVar15 = (undefined1 *)FUN_140877720(lVar14,&DAT_1439c5a30,2);
      local_e8 = puVar15;
      if ((puVar15 != (undefined1 *)0x0) && ((puVar15[0xd0] & 2) == 0)) {
        lVar16 = FUN_140854880(param_1);
        fVar23 = *(float *)(lVar14 + 0x344) - *(float *)(lVar16 + 0x344);
        fVar22 = *(float *)(lVar14 + 0x340) - *(float *)(lVar16 + 0x340);
        fVar24 = *(float *)(lVar14 + 0x33c) - *(float *)(lVar16 + 0x33c);
        lVar16 = *(longlong *)(puVar15 + 8);
        if ((*(char *)(lVar16 + 0xa0) == '\0') ||
           ((cVar8 = FUN_140803180(lVar12,0x31), cVar8 == '\0' &&
            (cVar8 = FUN_140803180(lVar12,0x32), cVar8 == '\0')))) {
          bVar1 = false;
        }
        else {
          bVar1 = true;
        }
        if ((((*(char *)(param_2 + 0x40) != '\0') || (bVar6)) ||
            (fVar24 * fVar24 + fVar22 * fVar22 + fVar23 * fVar23 <=
             *(float *)(local_c8 + 0xa0) * *(float *)(local_c8 + 0xa0))) ||
           (cVar8 = FUN_14061c8f0(), cVar8 != '\0')) {
          bVar3 = false;
        }
        else {
          bVar3 = true;
        }
        if (((*(char *)(lVar16 + 0xa0) == '\0') && (*(longlong *)(lVar16 + 0xf8) != 0)) &&
           ((0 < *(int *)(lVar16 + 0x100) &&
            ((cVar8 = FUN_140615d40(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x328)),
             cVar8 != '\0' && ((*(byte *)(param_1 + 0x140) & 1) == 0)))))) {
          bVar2 = true;
        }
        else {
          bVar2 = false;
        }
        cVar8 = FUN_14076e800(lVar16);
        cVar10 = FUN_14076e830();
        if ((cVar8 == '\0') || (local_res20 == '\0')) {
          bVar4 = false;
        }
        else {
          bVar4 = true;
        }
        if (((cVar8 == '\0') && (cVar10 == '\0')) || ((char)local_res18[0] == '\0')) {
          bVar5 = false;
        }
        else {
          bVar5 = true;
        }
        if (((bVar1) || (bVar2)) || ((bVar3 || ((bVar4 || (bVar5)))))) {
          local_b0[0] = 0x69;
          local_a8 = (uint)(*(byte *)(param_1 + 0x140) >> 1 & 1);
          local_98[0] = 0x69;
          local_90 = 0;
          FUN_1408848a0(lVar14,local_78,&DAT_1439acd40,local_98,local_b0);
          FUN_140882fc0(local_78);
          if ((*(longlong *)(lVar16 + 0xf8) != 0) &&
             (((bVar2 && (*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) != 0)) &&
              (lVar14 = *(longlong *)(*(longlong *)(D2_GameSingletonPtr + 0x1f6ee8) + 0xd8),
              lVar14 != 0)))) {
            switch(*(undefined4 *)(*(longlong *)(lVar16 + 0xf8) + 0x60)) {
            case 4:
              uVar13 = 4;
              break;
            default:
              goto switchD_14083fb69_caseD_5;
            case 0xe:
              uVar13 = 5;
              break;
            case 0x16:
              uVar13 = 0xe;
            }
            FUN_14038e820(lVar14,uVar13,0);
            FUN_140841490(param_1);
          }
switchD_14083fb69_caseD_5:
        }
        else {
          local_e0[0] = *(undefined4 *)(lVar14 + 0xa8);
          FUN_14052e460(param_1 + 0xf0,local_e0);
          *(byte *)(lVar14 + 0xd1) = *(byte *)(lVar14 + 0xd1) & 0x7f;
          FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,*(undefined8 *)(lVar12 + 0x10),lVar14);
          if (local_c0 != 0) {
            FUN_140617250(local_c0,lVar14);
          }
          if (local_b8 != 0) {
            FUN_140744a40(local_b8,lVar14);
          }
          puVar15 = local_e8;
          if ((*(longlong *)(lVar16 + 0xf8) == 0) ||
             (*(int *)(*(longlong *)(lVar16 + 0xf8) + 0x60) != 0x18)) {
            FUN_14076e2b0(local_e8);
          }
          local_e8 = local_98;
          uVar13 = FUN_140882fa0(local_98,*(undefined8 *)(puVar15 + 0x10));
          uVar17 = FUN_140854880(param_1);
          FUN_140884920(uVar17,local_b0,&DAT_1439abb70,uVar13);
          FUN_140882fc0(local_b0);
        }
      }
      local_d0 = local_d0 + 1;
      local_d8 = local_d8 - 1;
    } while (local_d8 != 0);
  }
  if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {
    FUN_140854190(param_1);
  }
  if ((*(longlong *)(lVar12 + 0x90) != 0) &&
     (*(longlong *)(*(longlong *)(lVar12 + 0x90) + 0x68) != 0)) {
    FUN_140888240();
  }
  puVar7 = local_f8;
  if (((local_100 & 0x40) == 0) && (local_f8 != (undefined4 *)0x0)) {
    plVar18 = (longlong *)FUN_1401548e0();
    (**(code **)(*plVar18 + 0x10))
              (plVar18,
               "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
               ,0x1ec,puVar7,0x10);
  }
  return;
}


````

### text

````text

void FUN_14083fd00(longlong param_1)

{
  undefined4 *puVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  undefined1 auStackX_8 [8];
  
  FUN_140854fd0();
  uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330);
  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939a0);
  if ((undefined4 *)(param_1 + 0x134) != puVar1) {
    *(undefined4 *)(param_1 + 0x134) = *puVar1;
  }
  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939a8);
  if ((undefined4 *)(param_1 + 0x138) != puVar1) {
    *(undefined4 *)(param_1 + 0x138) = *puVar1;
  }
  puVar1 = (undefined4 *)FUN_14060ce50(uVar2,auStackX_8,&DAT_1439939b0);
  if ((undefined4 *)(param_1 + 0x13c) != puVar1) {
    *(undefined4 *)(param_1 + 0x13c) = *puVar1;
  }
  uVar2 = FUN_140854880(param_1);
  uVar3 = FUN_140877720(uVar2,&DAT_1439c5d00,0);
  FUN_1403dab80(param_1 + 0x100,uVar3);
  uVar3 = FUN_140877720(uVar2,&DAT_1439b9e90,0);
  FUN_1403dab80(param_1 + 0x110,uVar3);
  uVar2 = FUN_140877720(uVar2,&DAT_1439d15f0,0);
  FUN_1403dab80(param_1 + 0x120,uVar2);
  return;
}


````

### text

````text

ulonglong FUN_140840b20(longlong param_1,longlong param_2)

{
  undefined8 uVar1;
  bool bVar2;
  char cVar3;
  int iVar4;
  longlong lVar5;
  longlong *plVar6;
  longlong lVar7;
  ulonglong uVar8;
  float extraout_XMM0_Da;
  float fVar9;
  float fVar10;
  float fVar11;
  
  lVar5 = FUN_1403da910(param_2 + 8,2);
  if (lVar5 == 0) {
    return 0;
  }
  uVar1 = *(undefined8 *)(param_1 + 0x10);
  plVar6 = (longlong *)FUN_1403da910(param_2 + 8,2);
  lVar5 = (**(code **)*plVar6)(plVar6);
  if ((*(int *)(lVar5 + 0x60) < DAT_1439d14a0) || (DAT_1439d14a4 < *(int *)(lVar5 + 0x60))) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  lVar5 = plVar6[2];
  if ((*(char *)(param_2 + 0x40) == '\0') &&
     ((!bVar2 || (((*(byte *)(lVar5 + 0xd1) & 0x20) == 0 && ((*(byte *)(lVar5 + 0xd1) & 8) == 0)))))
     ) {
    lVar7 = FUN_140854880(param_1);
    fVar10 = *(float *)(lVar5 + 0x300) - *(float *)(lVar7 + 0x300);
    fVar11 = *(float *)(lVar5 + 0x304) - *(float *)(lVar7 + 0x304);
    fVar9 = *(float *)(lVar5 + 0x308) - *(float *)(lVar7 + 0x308);
    uVar8 = FUN_140840610(param_1);
    if (extraout_XMM0_Da * extraout_XMM0_Da < fVar11 * fVar11 + fVar10 * fVar10 + fVar9 * fVar9)
    goto LAB_140840dcb;
  }
  if (bVar2) {
    lVar5 = FUN_140877720(plVar6[2],&DAT_1439dcd30,2);
    if ((((lVar5 == 0) || (lVar5 = FUN_140615290(lVar5), lVar5 == 0)) ||
        (cVar3 = FUN_140878c60(lVar5), cVar3 != '\0')) ||
       (uVar8 = FUN_140877720(lVar5,&DAT_1439c5a30,2), uVar8 == 0)) {
      lVar5 = FUN_140877720(plVar6[2],&DAT_1439c5880,2);
      uVar8 = 0;
      if (lVar5 == 0) goto LAB_140840dcb;
      lVar5 = FUN_140744880(lVar5);
      uVar8 = 0;
      if ((lVar5 == 0) || (uVar8 = FUN_140878c60(lVar5), (char)uVar8 != '\0')) goto LAB_140840dcb;
      goto LAB_140840d00;
    }
  }
  else {
    uVar8 = (**(code **)(*plVar6 + 0xe0))(plVar6);
    if ((char)uVar8 == '\0') goto LAB_140840dcb;
    lVar5 = plVar6[2];
LAB_140840d00:
    uVar8 = FUN_140877720(lVar5,&DAT_1439c5a30,2);
    if (uVar8 == 0) goto LAB_140840dcb;
  }
  if ((*(byte *)(uVar8 + 0xd0) & 2) != 0) {
LAB_140840dcb:
    return uVar8 & 0xffffffffffffff00;
  }
  lVar5 = *(longlong *)(uVar8 + 8);
  if (*(char *)(lVar5 + 0xa0) != '\0') {
    cVar3 = FUN_140803180(uVar1,0x2d);
    if ((cVar3 == '\0') && (cVar3 = FUN_140803180(uVar1,0x2e), cVar3 == '\0')) {
      bVar2 = false;
    }
    else {
      bVar2 = true;
    }
    iVar4 = FUN_140802de0(uVar1);
    uVar8 = FUN_140803180(uVar1,0x31);
    if (((char)uVar8 != '\0') || (uVar8 = FUN_140803180(uVar1,0x32), (char)uVar8 != '\0')) {
      uVar8 = CONCAT71((int7)(uVar8 >> 8),1);
    }
    if (((bVar2) || (iVar4 == 2)) || ((char)uVar8 != '\0')) goto LAB_140840dcb;
  }
  lVar5 = *(longlong *)(lVar5 + 0xf8);
  if ((lVar5 != 0) && (*(int *)(lVar5 + 0x60) == 5)) {
    FUN_140840ff0(param_1,lVar5 + 0xd0);
  }
  uVar8 = FUN_140854910(param_1,param_2);
  return uVar8;
}


````

### text

````text

void FUN_14083fe70(longlong param_1)

{
  undefined4 *puVar1;
  longlong lVar2;
  longlong lVar3;
  bool bVar4;
  longlong lVar5;
  longlong lVar6;
  longlong *plVar7;
  undefined4 *puVar8;
  undefined1 auStack_b8 [8];
  undefined4 uStack_b0;
  undefined1 auStack_a0 [8];
  uint uStack_98;
  undefined1 auStack_88 [32];
  undefined1 auStack_68 [80];
  
  puVar8 = *(undefined4 **)(param_1 + 0xf0);
  lVar2 = *(longlong *)(param_1 + 0x10);
  puVar1 = puVar8 + (*(uint *)(param_1 + 0xf8) & 0xffffff);
  do {
    if (puVar8 == puVar1) {
      if ((*(byte *)(param_1 + 0xfc) & 0x40) == 0) {
        lVar5 = *(longlong *)(param_1 + 0xf0);
        if (lVar5 != 0) {
          plVar7 = (longlong *)FUN_1401548e0();
          (**(code **)(*plVar7 + 0x10))
                    (plVar7,
                     "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                     ,0x1ec,lVar5,0x10);
        }
        *(uint *)(param_1 + 0xfc) = *(uint *)(param_1 + 0xfc) & 0xff;
        *(undefined8 *)(param_1 + 0xf0) = 0;
      }
      *(uint *)(param_1 + 0xf8) = *(uint *)(param_1 + 0xf8) & 0xff000000;
      lVar2 = *(longlong *)(lVar2 + 0x90);
      if ((lVar2 != 0) && (*(longlong *)(lVar2 + 0x68) != 0)) {
        FUN_1408880d0();
      }
      FUN_1408550d0(param_1);
      return;
    }
    lVar5 = FUN_1406dc560(*puVar8);
    if (lVar5 != 0) {
      lVar6 = FUN_140877720(lVar5,&DAT_1439c5a30,2);
      if (lVar6 == 0) {
LAB_14083ffc8:
        *(byte *)(lVar5 + 0xd7) = *(byte *)(lVar5 + 0xd7) | 1;
        FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar5);
      }
      else {
        lVar3 = *(longlong *)(lVar6 + 8);
        if ((*(longlong *)(lVar3 + 0xf8) == 0) ||
           (*(int *)(*(longlong *)(lVar3 + 0xf8) + 0xc4) == -1)) {
          bVar4 = false;
        }
        else {
          bVar4 = true;
        }
        if (*(char *)(lVar3 + 0xa0) == '\0') {
          if (!bVar4) {
            FUN_140841030(param_1,lVar6,*(byte *)(param_1 + 0x140) & 1);
            uStack_98 = *(byte *)(param_1 + 0x140) >> 1 & 1;
            auStack_a0[0] = 0x69;
            auStack_b8[0] = 0x69;
            uStack_b0 = 1;
            FUN_1408848a0(*(undefined8 *)(lVar6 + 0x10),auStack_88,&DAT_1439acd40,auStack_b8,
                          auStack_a0);
            FUN_140882fc0(auStack_88);
            FUN_14076ec20(lVar6,1);
            goto LAB_14083ffe5;
          }
        }
        else if (!bVar4) {
          FUN_14076ec80(lVar6);
          goto LAB_14083ffc8;
        }
        FUN_1403b7aa0(auStack_68,lVar6,lVar3 + 200,0);
        FUN_1408024d0(lVar2,0x34,auStack_68);
      }
    }
LAB_14083ffe5:
    puVar8 = puVar8 + 1;
  } while( true );
}


````

### text

````text

void FUN_140841500(longlong param_1)

{
  float fVar1;
  undefined4 uVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  float fVar6;
  uint uVar7;
  longlong lVar8;
  undefined8 *puVar9;
  longlong lVar10;
  longlong lVar11;
  undefined8 uVar12;
  longlong *plVar13;
  uint uVar14;
  float *pfVar15;
  byte bVar16;
  longlong lVar17;
  uint uVar18;
  float fVar19;
  float fVar20;
  uint auStack_188 [4];
  undefined1 uStack_178;
  float fStack_170;
  float fStack_16c;
  float fStack_168;
  float fStack_160;
  undefined4 uStack_15c;
  undefined4 uStack_158;
  float fStack_154;
  float fStack_150;
  float fStack_14c;
  float fStack_148;
  float fStack_144;
  float fStack_140;
  float fStack_138;
  float fStack_134;
  float fStack_130;
  undefined1 auStack_128 [8];
  uint uStack_120;
  undefined1 auStack_108 [200];
  
  FUN_1408555a0();
  lVar3 = *(longlong *)(param_1 + 0x10);
  if (((*(longlong *)(lVar3 + 0x90) == 0) ||
      (lVar4 = *(longlong *)(*(longlong *)(lVar3 + 0x90) + 0x68), lVar4 == 0)) ||
     (*(float *)(lVar4 + 0x74) <= 0.0)) {
    lVar4 = *(longlong *)(param_1 + 0x18);
    uVar18 = *(uint *)(param_1 + 0xf8) & 0xffffff;
    if (uVar18 != 0) {
      lVar17 = (longlong)(int)uVar18 * 4;
      do {
        uVar18 = uVar18 - 1;
        lVar17 = lVar17 + -4;
        lVar8 = FUN_1406dc560(*(undefined4 *)
                               (*(longlong *)(param_1 + 0xf0) + (longlong)(int)uVar18 * 4));
        if (lVar8 == 0) {
          if (((-1 < (int)uVar18) &&
              (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&
             (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,
             lVar8 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {
            do {
              uVar14 = uVar14 + 1;
              lVar8 = lVar8 + 4;
              *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar8) =
                   *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar8);
            } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));
          }
        }
        else {
          puVar9 = (undefined8 *)FUN_140877720(lVar8,&DAT_1439c5a30,2);
          if (puVar9 == (undefined8 *)0x0) {
            if (((-1 < (int)uVar18) &&
                (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&
               (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,
               lVar8 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {
              do {
                uVar14 = uVar14 + 1;
                lVar8 = lVar8 + 4;
                *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar8) =
                     *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar8);
              } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));
            }
          }
          else {
            lVar11 = puVar9[1];
            lVar5 = *(longlong *)(lVar11 + 0xf8);
            FUN_1408406b0(param_1,lVar8,&fStack_170,auStack_108);
            if ((*(int *)(puVar9 + 10) == -1) || ((*(byte *)(puVar9 + 0x1a) & 4) != 0)) {
              if (*(char *)(lVar4 + 0xac) == '\0') {
                lVar10 = (**(code **)*puVar9)(puVar9);
                if ((*(int *)(lVar10 + 0x60) < DAT_1439aeaf0) ||
                   (DAT_1439aeaf4 < *(int *)(lVar10 + 0x60))) goto LAB_1408416b5;
              }
            }
            else {
LAB_1408416b5:
              fVar6 = fStack_168;
              fVar19 = fStack_16c;
              fVar1 = fStack_170;
              fVar20 = fStack_16c * fStack_16c + fStack_170 * fStack_170 + fStack_168 * fStack_168;
              if ((*(float *)(lVar4 + 0x90) * *(float *)(lVar4 + 0x90) <= fVar20) &&
                 (lVar10 = FUN_140854880(param_1),
                 fVar19 * *(float *)(lVar10 + 0x310) + fVar1 * *(float *)(lVar10 + 0x30c) +
                 fVar6 * *(float *)(lVar10 + 0x314) <= 0.0)) {
                lVar11 = FUN_140877530(lVar8);
                if (lVar11 == 0) {
                  if ((*(int *)(puVar9 + 10) == -1) || ((*(byte *)(puVar9 + 0x1a) & 4) != 0)) {
                    if (fVar20 <= 1.1754944e-38) {
                      fVar20 = 1.1754944e-38;
                    }
                    fStack_168 = 1.0 / SQRT(fVar20);
                    fStack_170 = fVar1 * fStack_168;
                    fStack_16c = fVar19 * fStack_168;
                    fStack_168 = fVar6 * fStack_168;
                    if ((*(byte *)(param_1 + 0x140) & 2) == 0) {
                      fStack_140 = *(float *)(lVar4 + 0x94);
                    }
                    else {
                      fStack_140 = *(float *)(lVar4 + 0x98);
                    }
                    fStack_148 = fStack_170 * fStack_140;
                    fStack_144 = fStack_16c * fStack_140;
                    fStack_140 = fStack_168 * fStack_140;
                    uVar12 = FUN_1408783d0(lVar8);
                    FUN_140752870(uVar12,"arkPlayerStateLootObject",2,&fStack_148);
                    if ((*(byte *)(param_1 + 0x140) & 2) == 0) {
                      pfVar15 = &fStack_160;
                      fStack_160 = DAT_1431af668;
                      uStack_15c = DAT_1431af66c;
                      uStack_158 = DAT_1431af670;
                    }
                    else {
                      fVar1 = *(float *)(lVar4 + 0x9c);
                      lVar11 = FUN_140854880(param_1);
                      pfVar15 = &fStack_154;
                      fVar19 = -DAT_14228a75c;
                      fStack_154 = fVar1 * *(float *)(lVar11 + 0x318) * fVar19;
                      fStack_150 = fVar1 * *(float *)(lVar11 + 0x31c) * fVar19;
                      fStack_14c = fVar1 * *(float *)(lVar11 + 800) * fVar19;
                    }
                    fStack_138 = *pfVar15;
                    fStack_134 = pfVar15[1];
                    fStack_130 = pfVar15[2];
                    uVar12 = FUN_1408783d0(lVar8);
                    pfVar15 = &fStack_138;
                  }
                  else {
                    uVar12 = FUN_1408783d0(lVar8);
                    FUN_140752870(uVar12,"arkPlayerStateLootObject",2,&DAT_1431af668);
                    uVar12 = FUN_1408783d0(lVar8);
                    pfVar15 = &DAT_1431af668;
                  }
                  FUN_1407527e0(uVar12,"arkPlayerStateLootObject",2,pfVar15);
                }
                goto LAB_140841ce8;
              }
            }
            FUN_14080e050(D2_GameSingletonPtr + 0x1f52e8,lVar8);
            if (((-1 < (int)uVar18) &&
                (uVar7 = *(uint *)(param_1 + 0xf8), (int)uVar18 < (int)(uVar7 & 0xffffff))) &&
               (uVar7 = (uVar7 - 1 ^ uVar7) & 0xffffff ^ uVar7, *(uint *)(param_1 + 0xf8) = uVar7,
               lVar10 = lVar17, uVar14 = uVar18, (int)uVar18 < (int)(uVar7 & 0xffffff))) {
              do {
                uVar14 = uVar14 + 1;
                lVar10 = lVar10 + 4;
                *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + -4 + lVar10) =
                     *(undefined4 *)(*(longlong *)(param_1 + 0xf0) + lVar10);
              } while ((int)uVar14 < (int)(*(uint *)(param_1 + 0xf8) & 0xffffff));
            }
            bVar16 = *(byte *)(param_1 + 0x140) & 1;
            if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {
              if ((bVar16 != 0) &&
                 (lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130)), lVar10 != 0)) {
                if (lVar5 != 0) {
                  lVar10 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x130));
                  uVar2 = *(undefined4 *)(lVar10 + 0xa8);
                  lVar10 = FUN_140810240();
                  plVar13 = (longlong *)FUN_1408112c0(lVar10 + 0xc8e8,0x1c4,8,uVar2);
                  if (plVar13 != (longlong *)0x0) {
                    *plVar13 = lVar5;
                  }
                }
                *(byte *)(param_1 + 0x140) = *(byte *)(param_1 + 0x140) & 0xfe;
                *(undefined4 *)(param_1 + 0x130) = 0x3fff0010;
              }
              FUN_140854190(param_1);
            }
            if ((lVar5 == 0) || (*(int *)(lVar5 + 0xc4) == -1)) {
              if (*(char *)(lVar11 + 0xa0) == '\0') {
                if (*(longlong *)(lVar11 + 0xf8) == 0) {
                  uStack_120 = *(byte *)(param_1 + 0x140) >> 1 & 1;
                  auStack_128[0] = 0x69;
                  auStack_188[0] = CONCAT31(auStack_188[0]._1_3_,0x69);
                  auStack_188[2] = 0;
                  FUN_1408848a0(puVar9[2],auStack_108,&DAT_1439acd40,auStack_188,auStack_128);
                  FUN_140882fc0(auStack_108);
                  FUN_14076ec20(puVar9,1);
                }
                else {
                  FUN_140841030(param_1,puVar9,bVar16);
                  auStack_188[2] = *(byte *)(param_1 + 0x140) >> 1 & 1;
                  auStack_188[0] = CONCAT31(auStack_188[0]._1_3_,0x69);
                  auStack_128[0] = 0x69;
                  uStack_120 = 1;
                  FUN_1408848a0(puVar9[2],auStack_108,&DAT_1439acd40,auStack_128,auStack_188);
                  FUN_140882fc0(auStack_108);
                  FUN_14076ec20(puVar9,1);
                }
              }
              else {
                uStack_178 = 0;
                auStack_188[2] = *(uint *)(lVar8 + 0xa8);
                *(byte *)(lVar8 + 0xd1) =
                     *(char *)(lVar11 + 0x118) << 7 | *(byte *)(lVar8 + 0xd1) & 0x7f;
                auStack_188[3] = *(undefined4 *)(lVar11 + 0xa4);
                if (*(char *)(lVar11 + 0xa8) == '\0') {
                  auStack_188[1] = 0x31;
                  uVar12 = 0x31;
                }
                else {
                  auStack_188[1] = 0x32;
                  uVar12 = 0x32;
                }
                DAT_143994a30 = DAT_143994a30 + 1 & 0x7fffffff;
                if (DAT_143994a30 == 0) {
                  DAT_143994a30 = 1;
                }
                auStack_188[0] = DAT_143994a30;
                FUN_1408024d0(lVar3,uVar12,auStack_188);
                uVar2 = *(undefined4 *)(lVar11 + 0xb8);
                lVar8 = FUN_1403da910(param_1 + 0x100,2);
                if (lVar8 == 0) goto LAB_140841a3f;
                FUN_1404c2690(lVar8,uVar2);
                FUN_14076ec20(puVar9,0);
              }
            }
            else {
              FUN_1403b7aa0(auStack_108,puVar9,lVar11 + 200,0);
              FUN_1408024d0(lVar3,0x34,auStack_108);
LAB_140841a3f:
              FUN_14076ec20(puVar9,0);
            }
          }
        }
LAB_140841ce8:
      } while (uVar18 != 0);
    }
    if ((*(uint *)(param_1 + 0xf8) & 0xffffff) == 0) {
      FUN_140854190(param_1);
    }
  }
  return;
}


````

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

void FUN_14076ec20(longlong param_1,char param_2)

{
  byte *pbVar1;
  
  *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfd;
  if (param_2 != '\0') {
    (**(code **)(**(longlong **)(param_1 + 0x10) + 0x80))();
    *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) | 8;
    pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);
    *pbVar1 = *pbVar1 | 4;
    return;
  }
  if ((*(byte *)(param_1 + 0xd0) & 4) != 0) {
    FUN_1407e77e0();
  }
  pbVar1 = (byte *)(*(longlong *)(param_1 + 0x10) + 0xd4);
  *pbVar1 = *pbVar1 | 4;
  return;
}


````

### text

````text

void FUN_14076ec80(longlong param_1)

{
  longlong *plVar1;
  longlong lVar2;
  
  if ((*(byte *)(param_1 + 0xd0) & 1) != 0) {
    plVar1 = (longlong *)FUN_1403da910(param_1 + 0x50,2);
    if (plVar1 != (longlong *)0x0) {
      (**(code **)(*plVar1 + 0x110))(plVar1);
      *(byte *)(param_1 + 0xd0) = *(byte *)(param_1 + 0xd0) & 0xfe;
    }
  }
  lVar2 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439dcb80,2);
  if (lVar2 != 0) {
    FUN_14070b320(lVar2,0);
    return;
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

### text

````text

void FUN_140615620(longlong param_1,int param_2)

{
  undefined1 local_38 [8];
  int local_30;
  undefined1 local_20 [24];
  
  *(int *)(param_1 + 0x16c) = *(int *)(param_1 + 0x16c) + param_2;
  local_38[0] = 0x69;
  local_30 = param_2;
  FUN_140884920(*(undefined8 *)(param_1 + 0x10),local_20,&DAT_1439ab8f0,local_38);
  FUN_140882fc0(local_20);
  FUN_14067aea0(D2_GameSingletonPtr + 0x1f77c8,1,param_2);
  return;
}


````

### text

````text

undefined8 FUN_140615d40(undefined8 param_1,longlong param_2)

{
  int iVar1;
  undefined8 uVar2;
  
  if ((*(uint *)(param_2 + 0x60) < 0x12) &&
     ((0x38022U >> (*(uint *)(param_2 + 0x60) & 0x1f) & 1) != 0)) {
    return 0;
  }
  iVar1 = FUN_140614f30(param_1);
  if (-1 < iVar1) {
    uVar2 = FUN_140615d90(param_1,iVar1);
    return uVar2;
  }
  return 1;
}


````

### text

````text

void FUN_14080cd60(undefined8 param_1,longlong param_2,longlong param_3)

{
  byte bVar1;
  int iVar2;
  longlong lVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined8 uVar6;
  longlong *plVar7;
  ulonglong uVar8;
  ulonglong uVar9;
  longlong local_res18;
  
  bVar1 = *(byte *)(param_2 + 0xd1) >> 2 & 1;
  local_res18 = param_3;
  if ((bVar1 != 0) || ((*(byte *)(param_3 + 0xd1) & 4) != 0)) {
    if (bVar1 == 0) {
      param_2 = param_3;
    }
    uVar5 = D2_InternedName_Resolve(param_2 + 0x20);
                    /* WARNING: Subroutine does not return */
    FUN_140166830("[arkTimeManager::CreateTimeSpaceAttachmentBetweenEntities] %s doesn\'t allow sync time space\n"
                  ,uVar5);
  }
  lVar3 = FUN_140877500(param_2);
  lVar4 = FUN_140877500(param_3);
  if (lVar3 == lVar4) {
    uVar5 = D2_InternedName_Resolve(param_3 + 0x20);
    uVar6 = D2_InternedName_Resolve(param_2 + 0x20);
                    /* WARNING: Subroutine does not return */
    FUN_140166830("[arkTimeManager::CreateTimeSpaceAttachmentBetweenEntities] %s and %s are already attach\n"
                  ,uVar6,uVar5);
  }
  if (*(longlong *)(param_3 + 0x2b8) != 0) {
    FUN_14080e050(param_1);
  }
  *(longlong *)(param_3 + 0x2b8) = param_2;
  iVar2 = FUN_1401581c0(&DAT_1439921f4,0,1);
  while (iVar2 == 1) {
    iVar2 = FUN_1401581c0(&DAT_1439921f4,0,1);
  }
  uVar8 = 0;
  iVar2 = (*(uint *)(param_2 + 0x2c8) & 0xffffff) - 1;
  if (-1 < iVar2) {
    plVar7 = *(longlong **)(param_2 + 0x2c0);
    uVar9 = uVar8;
    do {
      if (*plVar7 == param_3) {
        if (-1 < (int)uVar9) goto LAB_14080cde1;
        break;
      }
      uVar8 = uVar8 + 1;
      uVar9 = (ulonglong)((int)uVar9 + 1);
      plVar7 = plVar7 + 1;
    } while ((longlong)uVar8 <= (longlong)iVar2);
  }
  FUN_1408918f0(param_2 + 0x2c0,&local_res18);
LAB_14080cde1:
  FUN_1401581e0(&DAT_1439921f4,0);
  return;
}


````

### text

````text
{"address":"141c7bc40","length":384,"data":[48,12,1,64,1,0,0,0,224,21,1,64,1,0,0,0,16,5,132,64,1,0,0,0,112,54,2,64,1,0,0,0,32,150,57,64,1,0,0,0,112,44,2,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,192,83,133,64,1,0,0,0,128,53,2,64,1,0,0,0,176,152,9,64,1,0,0,0,224,56,1,64,1,0,0,0,80,62,0,64,1,0,0,0,32,150,57,64,1,0,0,0,16,254,131,64,1,0,0,0,64,208,6,64,1,0,0,0,120,22,254,65,1,0,0,0,144,119,147,64,1,0,0,0,48,245,131,64,1,0,0,0,32,150,57,64,1,0,0,0,176,152,9,64,1,0,0,0,0,253,131,64,1,0,0,0,96,81,133,64,1,0,0,0,48,82,133,64,1,0,0,0,32,11,132,64,1,0,0,0,112,85,133,64,1,0,0,0,0,77,133,64,1,0,0,0,80,78,133,64,1,0,0,0,144,245,131,64,1,0,0,0,112,254,131,64,1,0,0,0,0,21,132,64,1,0,0,0,128,22,104,64,1,0,0,0,64,73,133,64,1,0,0,0,96,83,133,64,1,0,0,0,176,152,9,64,1,0,0,0,192,44,104,64,1,0,0,0,144,39,104,64,1,0,0,0,64,43,104,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,176,152,9,64,1,0,0,0,32,82,133,64,1,0,0,0,80,84,133,64,1,0,0,0,0,82,133,64,1,0,0,0,64,41,104,64,1,0,0,0,80,41,104,64,1,0,0,0,96,41,104,64,1,0,0,0,32,150,57,64,1,0,0,0],"hex":"300c014001000000e015014001000000100584400100000070360240010000002096394001000000702c024001000000b098094001000000b098094001000000c0538540010000008035024001000000b098094001000000e038014001000000503e004001000000209639400100000010fe83400100000040d00640010000007816fe4101000000907793400100000030f58340010000002096394001000000b09809400100000000fd83400100000060518540010000003052854001000000200b8440010000007055854001000000004d854001000000504e85400100000090f583400100000070fe8340010000000015844001000000801668400100000040498540010000006053854001000000b098094001000000c02c6840010000009027684001000000402b684001000000b098094001000000b098094001000000b0980940010000002052854001000000505485400100000000528540010000004029684001000000502968400100000060296840010000002096394001000000"}
````
