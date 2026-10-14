# native-victim-midnight-research.json

Original: [work/natural-moveset/carry-peer-research-v1/native-victim-midnight-research.json](<../../../../work/natural-moveset/carry-peer-research-v1/native-victim-midnight-research.json>)

SHA-256: `E27CA391A705D2FE408FC1B1574361A56014DE36AF2578DF35BBA953611D2DE3`; 33673 bytes.

## Original saved record

````json
{
  "exeSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "confirmed": [
    "v1/v2 native retargeting moved the corpse about 2.1 m as the carrier moved about 2 m, but preserved an approximately 11.5 m initial offset.",
    "v2 owner-space flag did not correct that initial offset.",
    "v3 one native initial SetTransform reduced maximum root separation to 1.281 m for Corvo and 1.202 m for Emily; both passed bounded attachment and release ownership checks.",
    "Visual inspection of both v3 recordings shows a ragdoll dragging/falling beside the carrier, not an acceptable shoulder carry."
  ],
  "pending": [
    "Victim-only native Midnight scene and ragdoll handoff",
    "Shoulder orientation/anchor, walk and crouch matching, native release presentation"
  ],
  "functions": {
    "1408542b0": {
      "result": "\r\n/* WARNING: Removing unreachable block (ram,0x00014085480a) */\r\n/* WARNING: Removing unreachable block (ram,0x000140854810) */\r\n/* WARNING: Removing unreachable block (ram,0x000140854822) */\r\n/* WARNING: Removing unreachable block (ram,0x000140854816) */\r\n/* WARNING: Removing unreachable block (ram,0x000140854820) */\r\n\r\nvoid FUN_1408542b0(longlong param_1,ulonglong param_2,longlong param_3,undefined8 param_4)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  int iVar4;\r\n  ulonglong uVar5;\r\n  ulonglong uVar6;\r\n  ulonglong uVar7;\r\n  int local_res10 [2];\r\n  undefined8 local_90;\r\n  undefined4 local_88;\r\n  undefined8 *local_80;\r\n  uint local_78;\r\n  undefined4 local_74;\r\n  undefined8 local_70;\r\n  undefined1 local_68;\r\n  undefined8 local_60;\r\n  undefined4 local_58;\r\n  undefined8 *local_50;\r\n  uint local_48;\r\n  undefined4 local_44;\r\n  undefined8 local_40;\r\n  undefined1 local_38;\r\n  \r\n  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x300);\r\n  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x328);\r\n  uVar6 = 0;\r\n  if (((param_2 & 1) != 0) && (lVar1 = FUN_140770dd0(lVar2), lVar1 != 0)) {\r\n    local_60 = DAT_1432ce5a0;\r\n    local_58 = 0;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    local_44 = 0x2ff;\r\n    local_40 = 0x3fff00103fff0010;\r\n    local_38 = 0;\r\n    lVar1 = FUN_140770dd0(lVar2);\r\n    local_res10[0] = *(int *)(lVar1 + 0xa8);\r\n    iVar4 = (local_48 & 0xffffff) - 1;\r\n    uVar5 = uVar6;\r\n    uVar7 = uVar6;\r\n    if (-1 < iVar4) {\r\n      do {\r\n        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {\r\n          if (-1 < (int)uVar7) goto LAB_1408543a5;\r\n          break;\r\n        }\r\n        uVar5 = uVar5 + 1;\r\n        uVar7 = (ulonglong)((int)uVar7 + 1);\r\n      } while ((longlong)uVar5 <= (longlong)iVar4);\r\n    }\r\n    FUN_14052e460(&local_50,local_res10);\r\nLAB_1408543a5:\r\n    FUN_1407c6840(param_4,&local_60);\r\n    local_90 = DAT_1432ce5b8;\r\n    local_88 = 0;\r\n    local_78 = 0x5000000;\r\n    local_80 = &local_70;\r\n    local_74 = 0x2ff;\r\n    local_70 = 0x3fff00103fff0010;\r\n    local_68 = 0;\r\n    lVar1 = FUN_140770dd0(lVar2);\r\n    local_res10[0] = *(int *)(lVar1 + 0xa8);\r\n    iVar4 = (local_78 & 0xffffff) - 1;\r\n    uVar5 = uVar6;\r\n    uVar7 = uVar6;\r\n    if (-1 < iVar4) {\r\n      do {\r\n        if (*(int *)((longlong)local_80 + uVar5 * 4) == local_res10[0]) {\r\n          if (-1 < (int)uVar7) goto LAB_14085443c;\r\n          break;\r\n        }\r\n        uVar5 = uVar5 + 1;\r\n        uVar7 = (ulonglong)((int)uVar7 + 1);\r\n      } while ((longlong)uVar5 <= (longlong)iVar4);\r\n    }\r\n    FUN_14052e460(&local_80,local_res10);\r\nLAB_14085443c:\r\n    FUN_1407c6840(param_4,&local_90);\r\n    FUN_14061a0a0(&local_80);\r\n    FUN_14061a0a0(&local_50);\r\n  }\r\n  if (((param_2 & 2) != 0) && (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x370)), lVar1 != 0)) {\r\n    local_60 = DAT_1432ce5a8;\r\n    local_58 = 0;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    local_44 = 0x2ff;\r\n    local_40 = 0x3fff00103fff0010;\r\n    local_38 = 0;\r\n    lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x370));\r\n    local_res10[0] = *(int *)(lVar1 + 0xa8);\r\n    iVar4 = (local_48 & 0xffffff) - 1;\r\n    uVar5 = uVar6;\r\n    uVar7 = uVar6;\r\n    if (-1 < iVar4) {\r\n      do {\r\n        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {\r\n          if (-1 < (int)uVar7) goto LAB_140854508;\r\n          break;\r\n        }\r\n        uVar5 = uVar5 + 1;\r\n        uVar7 = (ulonglong)((int)uVar7 + 1);\r\n      } while ((longlong)uVar5 <= (longlong)iVar4);\r\n    }\r\n    FUN_14052e460(&local_50,local_res10);\r\nLAB_140854508:\r\n    FUN_1407c6840(param_4,&local_60);\r\n    FUN_14061a0a0(&local_50);\r\n  }\r\n  if (((param_2 & 4) != 0) && (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x680)), lVar1 != 0)) {\r\n    local_60 = DAT_1432ce5b0;\r\n    local_58 = 0;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    local_44 = 0x2ff;\r\n    local_40 = 0x3fff00103fff0010;\r\n    local_38 = 0;\r\n    lVar2 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x680));\r\n    local_res10[0] = *(int *)(lVar2 + 0xa8);\r\n    iVar4 = (local_48 & 0xffffff) - 1;\r\n    uVar5 = uVar6;\r\n    uVar7 = uVar6;\r\n    if (-1 < iVar4) {\r\n      do {\r\n        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {\r\n          if (-1 < (int)uVar7) goto LAB_1408545ca;\r\n          break;\r\n        }\r\n        uVar5 = uVar5 + 1;\r\n        uVar7 = (ulonglong)((int)uVar7 + 1);\r\n      } while ((longlong)uVar5 <= (longlong)iVar4);\r\n    }\r\n    FUN_14052e460(&local_50,local_res10);\r\nLAB_1408545ca:\r\n    FUN_1407c6840(param_4,&local_60);\r\n    FUN_14061a0a0(&local_50);\r\n  }\r\n  if (((((param_2 & 8) != 0) && (lVar2 = *(longlong *)(lVar3 + 0x70), lVar2 != 0)) &&\r\n      (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), lVar1 != 0)) &&\r\n     (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), *(longlong *)(lVar1 + 0xf0) != 0)) {\r\n    local_60 = DAT_1432ce5c0;\r\n    local_58 = 0;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    local_44 = 0x2ff;\r\n    local_40 = 0x3fff00103fff0010;\r\n    local_38 = 0;\r\n    lVar2 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8));\r\n    local_res10[0] = *(int *)(lVar2 + 0xa8);\r\n    iVar4 = (local_48 & 0xffffff) - 1;\r\n    uVar5 = uVar6;\r\n    uVar7 = uVar6;\r\n    if (-1 < iVar4) {\r\n      do {\r\n        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {\r\n          if (-1 < (int)uVar7) goto LAB_1408546a9;\r\n          break;\r\n        }\r\n        uVar5 = uVar5 + 1;\r\n        uVar7 = (ulonglong)((int)uVar7 + 1);\r\n      } while ((longlong)uVar5 <= (longlong)iVar4);\r\n    }\r\n    FUN_14052e460(&local_50,local_res10);\r\nLAB_1408546a9:\r\n    FUN_1407c6840(param_4,&local_60);\r\n    FUN_14061a0a0(&local_50);\r\n  }\r\n  if ((((param_2 & 0x10) != 0) && (lVar2 = *(longlong *)(lVar3 + 0x68), lVar2 != 0)) &&\r\n     ((lVar3 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), lVar3 != 0 &&\r\n      (lVar3 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), *(longlong *)(lVar3 + 0xf0) != 0)))) {\r\n    local_60 = DAT_1432ce5c8;\r\n    local_58 = 0;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    local_44 = 0x2ff;\r\n    local_40 = 0x3fff00103fff0010;\r\n    local_38 = 0;\r\n    lVar2 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8));\r\n    local_res10[0] = *(int *)(lVar2 + 0xa8);\r\n    iVar4 = (local_48 & 0xffffff) - 1;\r\n    uVar5 = uVar6;\r\n    if (-1 < iVar4) {\r\n      do {\r\n        if (*(int *)((longlong)local_50 + uVar6 * 4) == local_res10[0]) {\r\n          if (-1 < (int)uVar5) goto LAB_140854788;\r\n          break;\r\n        }\r\n        uVar6 = uVar6 + 1;\r\n        uVar5 = (ulonglong)((int)uVar5 + 1);\r\n      } while ((longlong)uVar6 <= (longlong)iVar4);\r\n    }\r\n    FUN_14052e460(&local_50,local_res10);\r\nLAB_140854788:\r\n    FUN_1407c6840(param_4,&local_60);\r\n    FUN_14061a0a0(&local_50);\r\n  }\r\n  if (((param_2 & 0x20) != 0) && (param_3 != 0)) {\r\n    local_60 = DAT_1432ce5d0;\r\n    local_58 = 0;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    local_44 = 0x2ff;\r\n    local_40 = 0x3fff00103fff0010;\r\n    local_38 = 0;\r\n    local_res10[0] = *(int *)(param_3 + 0xa8);\r\n    FUN_14052e460(&local_50,local_res10);\r\n    FUN_1407c6840(param_4,&local_60);\r\n    FUN_14061a0a0(&local_50);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "1408549c0": {
      "result": "\r\nint * FUN_1408549c0(longlong *param_1,int *param_2,undefined8 param_3,undefined8 param_4,\r\n                   float param_5)\r\n\r\n{\r\n  int *piVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  int *piVar4;\r\n  int local_res8 [2];\r\n  undefined4 local_res10 [2];\r\n  undefined4 uVar5;\r\n  \r\n  lVar2 = D2_GameSingletonPtr;\r\n  uVar5 = 0;\r\n  piVar4 = (int *)FUN_1407d6040(D2_GameSingletonPtr + 0x1f5338,local_res8,param_3,param_4,\r\n                                &DAT_14398f888,0,0);\r\n  piVar1 = (int *)((longlong)param_1 + 0x24);\r\n  if (piVar1 != piVar4) {\r\n    *piVar1 = *piVar4;\r\n  }\r\n  if (*piVar1 != 0) {\r\n    (**(code **)(*param_1 + 0x88))(param_1);\r\n    local_res8[0] = (int)(param_5 * 1000.0);\r\n    local_res10[0] = 0;\r\n    cVar3 = FUN_1407d9270(lVar2 + 0x1f5338,*piVar1,local_res10,local_res8,0,CONCAT44(uVar5,0x100002)\r\n                         );\r\n    if ((cVar3 == '\\0') && (piVar1 != local_res8)) {\r\n      *piVar1 = 0;\r\n    }\r\n  }\r\n  (**(code **)(*param_1 + 0x80))(param_1,param_3);\r\n  *param_2 = *piVar1;\r\n  return param_2;\r\n}\r\n\r\n"
    },
    "1407d6040": {
      "result": "\r\nundefined4 *\r\nFUN_1407d6040(longlong param_1,undefined4 *param_2,undefined8 param_3,undefined8 param_4,\r\n             undefined4 *param_5,longlong param_6,undefined4 *param_7)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined4 *puVar2;\r\n  char cVar3;\r\n  undefined4 uVar4;\r\n  int iVar5;\r\n  longlong lVar6;\r\n  undefined1 local_58 [8];\r\n  undefined4 local_50;\r\n  undefined1 local_40 [24];\r\n  \r\n  puVar2 = param_7;\r\n  lVar1 = param_6;\r\n  cVar3 = FUN_1407d51e0(param_1,param_3,param_4,*param_5,param_6,param_7);\r\n  if (cVar3 == '\\0') {\r\n    *param_2 = 0;\r\n  }\r\n  else {\r\n    if (puVar2 != (undefined4 *)0x0) {\r\n      *puVar2 = 0;\r\n    }\r\n    uVar4 = FUN_140158200(&DAT_14398f880);\r\n    lVar6 = FUN_140652d10();\r\n    FUN_1407ce370(lVar6,param_3,uVar4,param_5,lVar1,param_4,0);\r\n    cVar3 = FUN_1407d69e0(param_1,lVar6,puVar2);\r\n    if (cVar3 == '\\0') {\r\n      if (lVar1 != 0) {\r\n        local_58[0] = 0x6d;\r\n        local_50 = 0;\r\n        FUN_140884920(lVar1,local_40,&DAT_1439a77a0,local_58);\r\n        FUN_140882fc0(local_40);\r\n      }\r\n      FUN_1407d17d0(lVar6,0xa02);\r\n      FUN_140652e70(lVar6);\r\n      *param_2 = 0;\r\n    }\r\n    else {\r\n      iVar5 = FUN_1401581c0(param_1 + 0x1b08,0,1);\r\n      while (iVar5 == 1) {\r\n        iVar5 = FUN_1401581c0(param_1 + 0x1b08,0,1);\r\n      }\r\n      param_5 = (undefined4 *)lVar6;\r\n      if (*(char *)(lVar6 + 0x2b5) == '\\0') {\r\n        FUN_1408918f0();\r\n      }\r\n      else {\r\n        FUN_140160b50(param_1,&param_5,*(undefined4 *)(param_1 + 0x11f0));\r\n        *(int *)(param_1 + 0x11f0) = *(int *)(param_1 + 0x11f0) + 1;\r\n      }\r\n      FUN_1407d6ef0(param_1,*(undefined4 *)(lVar6 + 0x158));\r\n      FUN_1401581e0(param_1 + 0x1b08,0);\r\n      *param_2 = uVar4;\r\n    }\r\n  }\r\n  return param_2;\r\n}\r\n\r\n"
    },
    "1407d9270": {
      "result": "\r\nundefined8\r\nFUN_1407d9270(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,\r\n             undefined4 *param_5,undefined4 param_6)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  undefined8 local_20;\r\n  int local_18;\r\n  \r\n  local_18 = 0;\r\n  local_20 = param_1;\r\n  lVar2 = FUN_1407d5860(param_1,param_2,&local_20,param_4,0xfffffffffffffffe);\r\n  FUN_1407cefa0(lVar2,param_6);\r\n  if (*(int *)(lVar2 + 0x27c) == 8) {\r\n    cVar1 = FUN_1407cfcd0(lVar2,0x902,1);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d17d0(lVar2,0x902);\r\n      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));\r\n    }\r\n    if (param_5 != (undefined4 *)0x0) {\r\n      *param_5 = 3;\r\n    }\r\n  }\r\n  else {\r\n    cVar1 = FUN_1407cbbe0(lVar2);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d2160(lVar2,param_3,param_4,param_5);\r\n    }\r\n    if (*(int *)(lVar2 + 0x27c) != 6) {\r\n      uVar3 = 1;\r\n      goto LAB_1407d936d;\r\n    }\r\n    cVar1 = FUN_1407cfcd0(lVar2,0x802,1);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d17d0(lVar2,0x802);\r\n      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));\r\n    }\r\n  }\r\n  uVar3 = 0;\r\nLAB_1407d936d:\r\n  if (local_18 != 0) {\r\n    FUN_1407d7050(local_20);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
    },
    "1407d51e0": {
      "result": "\r\nundefined4 FUN_1407d51e0(undefined8 param_1,longlong param_2,undefined8 param_3,int param_4)\r\n\r\n{\r\n  int *piVar1;\r\n  char cVar2;\r\n  int iVar3;\r\n  char *pcVar4;\r\n  undefined1 local_18 [16];\r\n  \r\n  cVar2 = FUN_1401975d0(param_2);\r\n  if (cVar2 != '\\0') {\r\n                    /* WARNING: Subroutine does not return */\r\n    FUN_140166830(\"arkMidnightManager::StartMdntInst: the midnight \\'%s\\' is empty.\",\r\n                  *(undefined8 *)(param_2 + 8));\r\n  }\r\n  piVar1 = (int *)FUN_1401962e0(param_2,local_18);\r\n  if (*piVar1 == 0) {\r\n    pcVar4 = \"arkMidnightManager::StartMdntInst: the midnight \\'%s\\' has nothing linked to entry.\";\r\n  }\r\n  else {\r\n    cVar2 = FUN_1407d5f40(param_1,param_2,param_3);\r\n    if (cVar2 != '\\0') {\r\n      if (param_4 == 1) {\r\n        iVar3 = FUN_140197060(param_2);\r\n        if (1 < iVar3) {\r\n          pcVar4 = \r\n          \"arkMidnightManager::StartMdntInst: the midnight \\'%s\\' can\\'t be played in reverse.\";\r\n          goto LAB_1407d5249;\r\n        }\r\n      }\r\n      return 1;\r\n    }\r\n    pcVar4 = \r\n    \"arkMidnightManager::StartMdntInst: no corresponding entity(ies) for the midnight \\'%s\\'.\";\r\n  }\r\nLAB_1407d5249:\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(pcVar4,*(undefined8 *)(param_2 + 8));\r\n}\r\n\r\n"
    },
    "1407ce370": {
      "result": "\r\nvoid FUN_1407ce370(undefined8 *param_1,undefined8 param_2,undefined4 param_3,int *param_4,\r\n                  undefined8 param_5,undefined8 param_6)\r\n\r\n{\r\n  longlong *plVar1;\r\n  byte bVar2;\r\n  int iVar3;\r\n  char cVar4;\r\n  longlong lVar5;\r\n  byte bVar6;\r\n  longlong *plVar7;\r\n  undefined4 uVar8;\r\n  undefined4 local_res18 [2];\r\n  \r\n  if (param_1 + 0x2b != (undefined8 *)local_res18) {\r\n    *(undefined4 *)(param_1 + 0x2b) = param_3;\r\n  }\r\n  *param_1 = param_2;\r\n  *(undefined1 *)(param_1 + 0x56) = 0;\r\n  param_1[0x52] = param_5;\r\n  *(char *)((longlong)param_1 + 0x2b4) = (char)param_4[1];\r\n  iVar3 = *param_4;\r\n  *(int *)(param_1 + 0x50) = iVar3;\r\n  if (iVar3 == 0) {\r\n    uVar8 = 0x3f800000;\r\n  }\r\n  else {\r\n    uVar8 = 0xbf800000;\r\n  }\r\n  *(undefined4 *)(param_1 + 0x2e) = uVar8;\r\n  *(undefined1 *)((longlong)param_1 + 0x2b6) = *(undefined1 *)((longlong)param_4 + 5);\r\n  local_res18[0] = param_3;\r\n  FUN_1407cee60(param_1);\r\n  FUN_1407ce5f0(param_1);\r\n  FUN_1407ce7e0(param_1,param_6);\r\n  *(undefined4 *)((longlong)param_1 + 0x27c) = 1;\r\n  FUN_1407cf070(param_1);\r\n  cVar4 = FUN_140195de0();\r\n  if (cVar4 == '\\0') {\r\n    if (((*(uint *)(param_1 + 0x1d) & 0xffffff) == 0) && (param_1[0x2a] != 0)) {\r\n      bVar6 = 1;\r\n    }\r\n    else {\r\n      bVar6 = 0;\r\n    }\r\n  }\r\n  else {\r\n    bVar6 = 1;\r\n  }\r\n  *(byte *)((longlong)param_1 + 0x2b2) = bVar6;\r\n  bVar2 = *(byte *)(param_4 + 1);\r\n  *(byte *)((longlong)param_1 + 0x2b2) = bVar2 | bVar6;\r\n  if ((bVar2 | bVar6) == 0) {\r\n    plVar7 = (longlong *)param_1[0x1c];\r\n    plVar1 = plVar7 + (*(uint *)(param_1 + 0x1d) & 0xffffff);\r\n    for (; plVar7 != plVar1; plVar7 = plVar7 + 1) {\r\n      lVar5 = *plVar7;\r\n      FUN_1400998b0(lVar5);\r\n      lVar5 = (**(code **)**(undefined8 **)(lVar5 + 0xd0))();\r\n      if ((*(int *)(lVar5 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar5 + 0x60))) {\r\n        bVar6 = 0;\r\n      }\r\n      else {\r\n        bVar6 = 1;\r\n      }\r\n      *(byte *)((longlong)param_1 + 0x2b2) = *(byte *)((longlong)param_1 + 0x2b2) | bVar6;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "1407d5f40": {
      "result": "\r\nbyte FUN_1407d5f40(undefined8 param_1,longlong param_2,undefined8 *param_3)\r\n\r\n{\r\n  byte bVar1;\r\n  longlong *plVar2;\r\n  ulonglong uVar3;\r\n  longlong lVar4;\r\n  ulonglong uVar5;\r\n  int iVar6;\r\n  ulonglong uVar7;\r\n  uint uVar8;\r\n  ulonglong uVar9;\r\n  byte bVar10;\r\n  ulonglong uVar11;\r\n  \r\n  uVar7 = 0;\r\n  bVar10 = 0;\r\n  iVar6 = 0;\r\n  uVar5 = uVar7;\r\n  uVar9 = uVar7;\r\n  uVar11 = uVar7;\r\n  if ((*(uint *)(param_2 + 0xb8) & 0xffffff) != 0) {\r\n    do {\r\n      lVar4 = *(longlong *)(param_2 + 0xb0) + uVar11;\r\n      bVar1 = FUN_140192d30(lVar4);\r\n      bVar10 = bVar10 | bVar1;\r\n      if ((*(uint *)(param_3 + 1) & 0xffffff) != 0) {\r\n        plVar2 = (longlong *)*param_3;\r\n        uVar3 = uVar7;\r\n        do {\r\n          if (*plVar2 == *(longlong *)(lVar4 + 0x10)) {\r\n            uVar5 = (ulonglong)((int)uVar5 + 1);\r\n            break;\r\n          }\r\n          uVar3 = uVar3 + 1;\r\n          plVar2 = plVar2 + 6;\r\n        } while ((longlong)uVar3 < (longlong)(ulonglong)(*(uint *)(param_3 + 1) & 0xffffff));\r\n      }\r\n      iVar6 = (int)uVar5;\r\n      uVar8 = (int)uVar9 + 1;\r\n      uVar9 = (ulonglong)uVar8;\r\n      uVar11 = uVar11 + 0x388;\r\n    } while ((int)uVar8 < (int)(*(uint *)(param_2 + 0xb8) & 0xffffff));\r\n  }\r\n  return bVar10 | 0 < iVar6;\r\n}\r\n\r\n"
    },
    "1407ce7e0": {
      "result": "\r\nvoid FUN_1407ce7e0(undefined **param_1,longlong *param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  uint uVar2;\r\n  undefined *puVar3;\r\n  bool bVar4;\r\n  undefined1 *puVar5;\r\n  char cVar6;\r\n  uint uVar7;\r\n  undefined *puVar8;\r\n  longlong lVar9;\r\n  longlong *plVar10;\r\n  longlong *plVar11;\r\n  uint uVar12;\r\n  int iVar13;\r\n  ulonglong uVar14;\r\n  longlong lVar15;\r\n  longlong lVar16;\r\n  ulonglong uVar17;\r\n  ulonglong uVar18;\r\n  ulonglong uVar19;\r\n  char cVar20;\r\n  longlong lVar21;\r\n  undefined **local_res8;\r\n  longlong *local_res10;\r\n  char local_res18;\r\n  undefined *local_res20;\r\n  ulonglong in_stack_fffffffffffffc08;\r\n  longlong local_3c0;\r\n  longlong local_3b8;\r\n  undefined **local_398;\r\n  undefined1 **local_390;\r\n  undefined4 local_388;\r\n  undefined8 local_378;\r\n  undefined1 *local_368;\r\n  uint local_360;\r\n  uint local_35c;\r\n  undefined1 local_358 [256];\r\n  undefined **local_258;\r\n  undefined8 local_250;\r\n  undefined4 local_248;\r\n  undefined1 *local_240;\r\n  undefined4 local_238;\r\n  undefined4 local_234;\r\n  undefined1 local_230 [384];\r\n  undefined1 *local_b0;\r\n  undefined4 local_a8;\r\n  undefined4 local_a4;\r\n  undefined1 local_a0 [64];\r\n  undefined8 local_60;\r\n  undefined4 local_58;\r\n  undefined4 local_54;\r\n  \r\n  local_378 = 0xfffffffffffffffe;\r\n  cVar20 = '\\0';\r\n  local_res18 = '\\0';\r\n  local_360 = 0x5000000;\r\n  local_368 = local_358;\r\n  local_35c = 0x20ff;\r\n  puVar3 = *param_1;\r\n  uVar7 = *(uint *)(puVar3 + 0xb8) & 0xffffff;\r\n  local_3b8 = (longlong)(int)uVar7;\r\n  local_res8 = param_1;\r\n  local_res10 = param_2;\r\n  if (uVar7 != 0) {\r\n    local_3c0 = 0;\r\n    do {\r\n      plVar11 = local_res10;\r\n      uVar17 = 0;\r\n      lVar16 = *(longlong *)(puVar3 + 0xb0) + local_3c0;\r\n      cVar6 = FUN_140190c20(lVar16 + 0x10);\r\n      uVar18 = 0xffffffffffffffff;\r\n      if ((*(uint *)(plVar11 + 1) & 0xffffff) != 0) {\r\n        plVar10 = (longlong *)*local_res10;\r\n        uVar14 = uVar17;\r\n        uVar19 = uVar17;\r\n        do {\r\n          uVar18 = uVar19;\r\n          if (*plVar10 == *(longlong *)(lVar16 + 0x10)) break;\r\n          uVar7 = (int)uVar14 + 1;\r\n          uVar14 = (ulonglong)uVar7;\r\n          uVar19 = uVar19 + 1;\r\n          plVar10 = plVar10 + 6;\r\n          uVar18 = 0xffffffffffffffff;\r\n        } while ((int)uVar7 < (int)(*(uint *)(plVar11 + 1) & 0xffffff));\r\n      }\r\n      if (cVar6 == '\\0') {\r\n        if ((longlong)uVar18 < 0) {\r\n          if ((*(longlong *)(lVar16 + 0x328) != 0) && (*(char *)((longlong)param_1 + 0x2b4) == '\\0')\r\n             ) {\r\n            in_stack_fffffffffffffc08 = 0;\r\n            lVar15 = FUN_1408ae9b0(D2_GameSingletonPtr,*(longlong *)(lVar16 + 0x328),&DAT_1431af668,\r\n                                   &DAT_14228a780,0,1,0,0,0);\r\n            *(byte *)(lVar15 + 0xd4) = *(byte *)(lVar15 + 0xd4) | 4;\r\n            local_res18 = '\\x01';\r\n            puVar8 = (undefined *)FUN_140652d60();\r\n            local_res20 = puVar8;\r\n            if (puVar8 != (undefined *)0x0) {\r\n              FUN_1407db090(puVar8,param_1,lVar16,0);\r\n            }\r\n            FUN_1407dc130(puVar8,lVar15,1);\r\n            FUN_1408918f0(&local_368,&local_res20);\r\n            cVar20 = '\\x01';\r\n          }\r\n        }\r\n        else {\r\n          lVar21 = uVar18 * 0x30;\r\n          lVar15 = *local_res10;\r\n          plVar11 = local_res10;\r\n          uVar18 = uVar17;\r\n          cVar20 = local_res18;\r\n          if ((*(uint *)(lVar15 + 0x18 + lVar21) & 0xffffff) != 0) {\r\n            do {\r\n              local_res20 = (undefined *)\r\n                            CONCAT44(local_res20._4_4_,\r\n                                     *(undefined4 *)(uVar17 + *(longlong *)(lVar15 + 0x10 + lVar21))\r\n                                    );\r\n              lVar9 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,&local_res20);\r\n              if ((lVar9 != 0) && (lVar9 = FUN_1407d5c40(lVar9,0), lVar9 != 0)) {\r\n                uVar1 = *(undefined4 *)(lVar21 + 8 + *plVar11);\r\n                puVar8 = (undefined *)FUN_140652d60();\r\n                param_1 = local_res8;\r\n                local_res20 = puVar8;\r\n                if (puVar8 != (undefined *)0x0) {\r\n                  FUN_1407db090(puVar8,local_res8,lVar16,uVar1);\r\n                }\r\n                FUN_1407dc130(puVar8,lVar9,0);\r\n                FUN_1408918f0(&local_368,&local_res20);\r\n                plVar11 = local_res10;\r\n              }\r\n              uVar7 = (int)uVar18 + 1;\r\n              uVar17 = uVar17 + 4;\r\n              uVar18 = (ulonglong)uVar7;\r\n              cVar20 = local_res18;\r\n            } while ((int)uVar7 < (int)(*(uint *)(lVar15 + 0x18 + lVar21) & 0xffffff));\r\n          }\r\n        }\r\n      }\r\n      else {\r\n        puVar8 = (undefined *)FUN_140652d60();\r\n        local_res20 = puVar8;\r\n        if (puVar8 != (undefined *)0x0) {\r\n          FUN_1407db090(puVar8,param_1,lVar16,0);\r\n        }\r\n        FUN_1407dc120(puVar8);\r\n        param_1[0x2a] = puVar8;\r\n        FUN_1408918f0(&local_368,&local_res20);\r\n      }\r\n      local_3c0 = local_3c0 + 0x388;\r\n      local_3b8 = local_3b8 + -1;\r\n    } while (local_3b8 != 0);\r\n  }\r\n  lVar16 = 0;\r\n  uVar7 = local_360 & 0xffffff;\r\n  if (((uVar7 < 5) || (DAT_14398f828 == 0)) ||\r\n     (bVar4 = true, *(char *)(D2_GameSingletonPtr + 0x337122) == '\\0')) {\r\n    bVar4 = false;\r\n  }\r\n  if (bVar4) {\r\n    local_250 = 0;\r\n    local_248 = 0x1ffff;\r\n    local_258 = &PTR_LAB_141ad4a48;\r\n    local_240 = local_230;\r\n    local_238 = 0;\r\n    local_234 = 0x80000010;\r\n    local_b0 = local_a0;\r\n    local_a8 = 0;\r\n    local_a4 = 0x80000010;\r\n    local_60 = 0;\r\n    local_58 = 0;\r\n    local_54 = 0x80000000;\r\n    local_398 = &PTR_LAB_141c6b370;\r\n    local_390 = &local_368;\r\n    local_388 = 0xffffffff;\r\n    FUN_140aa96f0(&local_258,&local_res8,&local_398,*(undefined4 *)(D2_GameSingletonPtr + 0x1f6524),\r\n                  in_stack_fffffffffffffc08 & 0xffffffff00000000);\r\n    FUN_14007a9e0(&local_258);\r\n    local_398 = &PTR_LAB_141abd918;\r\n    FUN_140090650(&local_258);\r\n  }\r\n  else {\r\n    lVar15 = lVar16;\r\n    if (uVar7 != 0) {\r\n      do {\r\n        if (((*(byte *)(*(longlong *)(local_368 + lVar15 * 8) + 0x1ac) & 8) == 0) &&\r\n           (*(longlong *)(*(longlong *)(local_368 + lVar15 * 8) + 0xd0) != 0)) {\r\n          FUN_1407dc1d0();\r\n        }\r\n        lVar15 = lVar15 + 1;\r\n      } while (lVar15 < (int)uVar7);\r\n    }\r\n  }\r\n  if (cVar20 != '\\0') {\r\n    FUN_1407cf680(param_1,&local_368);\r\n  }\r\n  if (*(uint *)((longlong)param_1 + 0x1c) >> 8 < uVar7) {\r\n    FUN_1407d01e0(param_1 + 2,uVar7);\r\n  }\r\n  if (uVar7 != 0) {\r\n    do {\r\n      lVar15 = *(longlong *)(local_368 + lVar16 * 8);\r\n      uVar2 = *(uint *)((longlong)param_1 + 0x1c);\r\n      uVar12 = uVar2 >> 8;\r\n      if ((*(uint *)(param_1 + 3) & 0xffffff) == uVar12) {\r\n        if (((byte)uVar2 & 0xc0) == 0xc0) {\r\n          if (DAT_1431aee98 == '\\0') {\r\n            if ((uVar2 & 0x3f) == 0x3f) {\r\n              if (uVar2 >> 8 == 0) {\r\n                iVar13 = 1;\r\n              }\r\n              else if (uVar12 < 0x40000000) {\r\n                iVar13 = uVar12 * 2;\r\n              }\r\n              else {\r\n                iVar13 = 0x7fffffff;\r\n              }\r\n            }\r\n            else {\r\n              iVar13 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);\r\n              uVar12 = uVar12 + iVar13;\r\n              iVar13 = uVar12 - (iVar13 - 1U & uVar12);\r\n            }\r\n          }\r\n          else {\r\n            iVar13 = uVar12 + 1;\r\n          }\r\n          FUN_1407cf300(param_1 + 2,iVar13);\r\n        }\r\n        else {\r\n          if (DAT_1431aee98 == '\\0') {\r\n            if ((uVar2 & 0x3f) == 0x3f) {\r\n              if (uVar2 >> 8 == 0) {\r\n                iVar13 = 1;\r\n              }\r\n              else if (uVar12 < 0x40000000) {\r\n                iVar13 = uVar12 * 2;\r\n              }\r\n              else {\r\n                iVar13 = 0x7fffffff;\r\n              }\r\n            }\r\n            else {\r\n              iVar13 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);\r\n              uVar12 = uVar12 + iVar13;\r\n              iVar13 = uVar12 - (iVar13 - 1U & uVar12);\r\n            }\r\n          }\r\n          else {\r\n            iVar13 = uVar12 + 1;\r\n          }\r\n          FUN_1407d01e0(param_1 + 2,iVar13);\r\n        }\r\n      }\r\n      uVar12 = *(uint *)((longlong)param_1 + 0x1c) >> 8;\r\n      uVar2 = *(uint *)(param_1 + 3);\r\n      if ((uVar2 & 0xffffff) < uVar12) {\r\n        plVar11 = (longlong *)(param_1[2] + (ulonglong)(uVar2 & 0xffffff) * 0x20);\r\n        *(uint *)(param_1 + 3) = (uVar2 + 1 ^ uVar2) & 0xffffff ^ uVar2;\r\n      }\r\n      else {\r\n        plVar11 = (longlong *)(param_1[2] + (ulonglong)(uVar12 - 1) * 0x20);\r\n      }\r\n      *plVar11 = lVar15;\r\n      plVar11[2] = *(longlong *)(lVar15 + 0xb0);\r\n      *(undefined4 *)(plVar11 + 3) = *(undefined4 *)(lVar15 + 0xa8);\r\n      if ((((*(byte *)(lVar15 + 0x1ac) & 1) == 0) && ((*(byte *)(lVar15 + 0x1ac) & 8) == 0)) &&\r\n         (*(longlong *)(lVar15 + 0xd0) != 0)) {\r\n        plVar11[1] = *(longlong *)(lVar15 + 0xd0);\r\n      }\r\n      if (*(longlong *)(lVar15 + 0xb0) == DAT_1432ce5b8) {\r\n        *(undefined1 *)((longlong)param_1 + 0x2b5) = 1;\r\n      }\r\n      lVar16 = lVar16 + 1;\r\n    } while (lVar16 < (int)uVar7);\r\n  }\r\n  local_res8 = &PTR_FUN_141c6b3b0;\r\n  if (param_1[0x1c] != (undefined *)0x0) {\r\n    FUN_1407d1e50(&local_res8,param_1[0x1c],*(uint *)(param_1 + 0x1d) & 0xffffff);\r\n  }\r\n  puVar5 = local_368;\r\n  if (((local_35c & 0x40) == 0) && (local_368 != (undefined1 *)0x0)) {\r\n    plVar11 = (longlong *)FUN_1401548e0();\r\n    (**(code **)(*plVar11 + 0x10))\r\n              (plVar11,\r\n               \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n               ,0x1ec,puVar5,0x10);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "1407d9590": {
      "result": "\r\nvoid FUN_1407d9590(undefined8 param_1,undefined8 param_2)\r\n\r\n{\r\n  FUN_1407d9460(param_1,param_2,0x600,0,1);\r\n  return;\r\n}\r\n\r\n"
    },
    "1407dc1d0": {
      "result": "\r\nvoid FUN_1407dc1d0(longlong param_1)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  byte bVar4;\r\n  int *piVar5;\r\n  undefined8 uVar6;\r\n  undefined1 *puVar7;\r\n  char *pcVar8;\r\n  undefined4 *puVar9;\r\n  int *piVar10;\r\n  char cVar11;\r\n  byte bVar12;\r\n  ushort *puVar13;\r\n  uint uVar14;\r\n  undefined8 uVar15;\r\n  ushort local_res8 [4];\r\n  ushort local_res10 [4];\r\n  ushort local_res18 [4];\r\n  undefined1 local_res20 [8];\r\n  undefined4 local_78;\r\n  undefined8 local_74;\r\n  undefined4 local_6c;\r\n  byte local_68;\r\n  ushort local_64;\r\n  undefined4 local_60;\r\n  undefined8 local_5c;\r\n  undefined4 local_54;\r\n  byte local_50;\r\n  ushort local_4c;\r\n  undefined1 local_48 [8];\r\n  undefined1 local_40 [8];\r\n  undefined1 local_38 [8];\r\n  undefined1 local_30 [8];\r\n  undefined1 local_28 [16];\r\n  \r\n  FUN_140883a10(*(undefined8 *)(param_1 + 0xd0),param_1);\r\n  FUN_1407db670(param_1);\r\n  piVar5 = (int *)FUN_140191b60(*(undefined8 *)(param_1 + 0xb8));\r\n  iVar1 = *piVar5;\r\n  if (iVar1 == 1) {\r\n    FUN_140131f90(local_48,\"Sword\",0);\r\n    uVar15 = *(undefined8 *)(param_1 + 0xf8);\r\n    uVar6 = FUN_140131ae0(local_28);\r\n    puVar7 = local_48;\r\n    puVar13 = local_res18;\r\n  }\r\n  else if (iVar1 == 2) {\r\n    FUN_140131f90(local_40,\"Empty\",0);\r\n    uVar15 = *(undefined8 *)(param_1 + 0xf8);\r\n    uVar6 = FUN_140131ae0(local_38);\r\n    puVar7 = local_40;\r\n    puVar13 = local_res10;\r\n  }\r\n  else {\r\n    if (iVar1 != 3) goto LAB_1407dc2b7;\r\n    FUN_140131f90(local_res20,\"SwordGunAim\",0);\r\n    uVar15 = *(undefined8 *)(param_1 + 0xf8);\r\n    uVar6 = FUN_140131ae0(local_30);\r\n    puVar7 = local_res20;\r\n    puVar13 = local_res8;\r\n  }\r\n  puVar7 = (undefined1 *)FUN_1405484d0(uVar15,puVar13,puVar7,uVar6);\r\n  if ((undefined1 *)(param_1 + 0x189) != puVar7) {\r\n    *(undefined1 *)(param_1 + 0x189) = *puVar7;\r\n  }\r\nLAB_1407dc2b7:\r\n  if (-1 < *(char *)(param_1 + 0x189)) {\r\n    puVar7 = (undefined1 *)\r\n             FUN_140548230(*(undefined8 *)(param_1 + 0xf8),local_res8,\r\n                           *(undefined1 *)(param_1 + 0x189));\r\n    if ((undefined1 *)(param_1 + 0x188) != puVar7) {\r\n      *(undefined1 *)(param_1 + 0x188) = *puVar7;\r\n    }\r\n  }\r\n  uVar14 = 0;\r\n  if ((-1 < *(char *)(param_1 + 0x188)) && (-1 < *(char *)(param_1 + 0x189))) {\r\n    pcVar8 = (char *)FUN_140548950(*(undefined8 *)(param_1 + 0xf8),local_res8);\r\n    if ((*pcVar8 == *(char *)(param_1 + 0x188)) &&\r\n       (pcVar8 = (char *)FUN_140548480(*(undefined8 *)(param_1 + 0xf8),local_res8),\r\n       *pcVar8 == *(char *)(param_1 + 0x189))) {\r\n      *(undefined2 *)(param_1 + 0x188) = 0xffff;\r\n    }\r\n    else {\r\n      local_68 = local_68 & 0xfe;\r\n      local_50 = local_50 & 0xfe;\r\n      local_res8[0] = (ushort)*(byte *)(param_1 + 0x188);\r\n      local_64 = local_res8[0];\r\n      local_res8[0] = (ushort)*(byte *)(param_1 + 0x189);\r\n      local_78 = 0;\r\n      local_74 = 1;\r\n      local_6c = 0;\r\n      local_60 = 0;\r\n      local_4c = local_res8[0];\r\n      local_5c = 1;\r\n      local_54 = 0;\r\n      puVar9 = (undefined4 *)FUN_140545a00(*(undefined8 *)(param_1 + 0xf8),local_res8,&local_78);\r\n      if ((undefined4 *)(param_1 + 0x17c) != puVar9) {\r\n        *(undefined4 *)(param_1 + 0x17c) = *puVar9;\r\n      }\r\n      puVar9 = (undefined4 *)FUN_140544ed0(*(undefined8 *)(param_1 + 0xf8),local_res8,&local_60);\r\n      if ((undefined4 *)(param_1 + 0x180) != puVar9) {\r\n        *(undefined4 *)(param_1 + 0x180) = *puVar9;\r\n      }\r\n    }\r\n  }\r\n  lVar2 = *(longlong *)(param_1 + 0xd0);\r\n  bVar4 = *(byte *)(lVar2 + 0xd1) >> 5 & 1;\r\n  bVar12 = *(byte *)(lVar2 + 0xd1) >> 3 & 1;\r\n  if ((bVar12 == 0) && (bVar4 == 0)) {\r\n    cVar11 = '\\0';\r\n  }\r\n  else {\r\n    cVar11 = '\\x01';\r\n  }\r\n  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) & 0xfb;\r\n  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | cVar11 << 2;\r\n  lVar3 = *(longlong *)(param_1 + 0xb8);\r\n  if ((((bVar12 != 0) && ((*(byte *)(lVar3 + 0x264) & 4) == 0)) &&\r\n      ((*(byte *)(lVar3 + 0x268) & 4) != 0)) ||\r\n     (((bVar4 != 0 && ((*(byte *)(lVar3 + 0x264) & 2) == 0)) &&\r\n      ((*(byte *)(lVar3 + 0x268) & 2) != 0)))) {\r\n    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 2;\r\n    *(undefined4 *)(param_1 + 400) = *(undefined4 *)(lVar2 + 0xa8);\r\n    *(undefined4 *)(param_1 + 0x194) = *(undefined4 *)(lVar2 + 0x300);\r\n    *(undefined4 *)(param_1 + 0x198) = *(undefined4 *)(lVar2 + 0x304);\r\n    *(undefined4 *)(param_1 + 0x19c) = *(undefined4 *)(lVar2 + 0x308);\r\n  }\r\n  piVar10 = *(int **)(lVar3 + 0x208);\r\n  piVar5 = piVar10 + (*(uint *)(lVar3 + 0x210) & 0xffffff);\r\n  for (; piVar10 != piVar5; piVar10 = piVar10 + 1) {\r\n    uVar14 = uVar14 | *(uint *)(&DAT_141c6c7a0 + (longlong)*piVar10 * 4);\r\n  }\r\n  *(uint *)(param_1 + 0x1a0) = uVar14;\r\n  if (*(longlong *)(param_1 + 0xb0) == DAT_1432ce5b8) {\r\n    *(undefined4 *)(param_1 + 0x1a8) = 2;\r\n    return;\r\n  }\r\n  if (*(longlong *)(param_1 + 0xe8) != 0) {\r\n    FUN_140767420(*(longlong *)(param_1 + 0xe8),\r\n                  *(undefined4 *)(*(longlong *)(param_1 + 200) + 0x158),0);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  }
}

````

## Unescaped text fields for reading

### result

````text

/* WARNING: Removing unreachable block (ram,0x00014085480a) */
/* WARNING: Removing unreachable block (ram,0x000140854810) */
/* WARNING: Removing unreachable block (ram,0x000140854822) */
/* WARNING: Removing unreachable block (ram,0x000140854816) */
/* WARNING: Removing unreachable block (ram,0x000140854820) */

void FUN_1408542b0(longlong param_1,ulonglong param_2,longlong param_3,undefined8 param_4)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  int iVar4;
  ulonglong uVar5;
  ulonglong uVar6;
  ulonglong uVar7;
  int local_res10 [2];
  undefined8 local_90;
  undefined4 local_88;
  undefined8 *local_80;
  uint local_78;
  undefined4 local_74;
  undefined8 local_70;
  undefined1 local_68;
  undefined8 local_60;
  undefined4 local_58;
  undefined8 *local_50;
  uint local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined1 local_38;
  
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x300);
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x328);
  uVar6 = 0;
  if (((param_2 & 1) != 0) && (lVar1 = FUN_140770dd0(lVar2), lVar1 != 0)) {
    local_60 = DAT_1432ce5a0;
    local_58 = 0;
    local_48 = 0x5000000;
    local_50 = &local_40;
    local_44 = 0x2ff;
    local_40 = 0x3fff00103fff0010;
    local_38 = 0;
    lVar1 = FUN_140770dd0(lVar2);
    local_res10[0] = *(int *)(lVar1 + 0xa8);
    iVar4 = (local_48 & 0xffffff) - 1;
    uVar5 = uVar6;
    uVar7 = uVar6;
    if (-1 < iVar4) {
      do {
        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {
          if (-1 < (int)uVar7) goto LAB_1408543a5;
          break;
        }
        uVar5 = uVar5 + 1;
        uVar7 = (ulonglong)((int)uVar7 + 1);
      } while ((longlong)uVar5 <= (longlong)iVar4);
    }
    FUN_14052e460(&local_50,local_res10);
LAB_1408543a5:
    FUN_1407c6840(param_4,&local_60);
    local_90 = DAT_1432ce5b8;
    local_88 = 0;
    local_78 = 0x5000000;
    local_80 = &local_70;
    local_74 = 0x2ff;
    local_70 = 0x3fff00103fff0010;
    local_68 = 0;
    lVar1 = FUN_140770dd0(lVar2);
    local_res10[0] = *(int *)(lVar1 + 0xa8);
    iVar4 = (local_78 & 0xffffff) - 1;
    uVar5 = uVar6;
    uVar7 = uVar6;
    if (-1 < iVar4) {
      do {
        if (*(int *)((longlong)local_80 + uVar5 * 4) == local_res10[0]) {
          if (-1 < (int)uVar7) goto LAB_14085443c;
          break;
        }
        uVar5 = uVar5 + 1;
        uVar7 = (ulonglong)((int)uVar7 + 1);
      } while ((longlong)uVar5 <= (longlong)iVar4);
    }
    FUN_14052e460(&local_80,local_res10);
LAB_14085443c:
    FUN_1407c6840(param_4,&local_90);
    FUN_14061a0a0(&local_80);
    FUN_14061a0a0(&local_50);
  }
  if (((param_2 & 2) != 0) && (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x370)), lVar1 != 0)) {
    local_60 = DAT_1432ce5a8;
    local_58 = 0;
    local_48 = 0x5000000;
    local_50 = &local_40;
    local_44 = 0x2ff;
    local_40 = 0x3fff00103fff0010;
    local_38 = 0;
    lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x370));
    local_res10[0] = *(int *)(lVar1 + 0xa8);
    iVar4 = (local_48 & 0xffffff) - 1;
    uVar5 = uVar6;
    uVar7 = uVar6;
    if (-1 < iVar4) {
      do {
        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {
          if (-1 < (int)uVar7) goto LAB_140854508;
          break;
        }
        uVar5 = uVar5 + 1;
        uVar7 = (ulonglong)((int)uVar7 + 1);
      } while ((longlong)uVar5 <= (longlong)iVar4);
    }
    FUN_14052e460(&local_50,local_res10);
LAB_140854508:
    FUN_1407c6840(param_4,&local_60);
    FUN_14061a0a0(&local_50);
  }
  if (((param_2 & 4) != 0) && (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x680)), lVar1 != 0)) {
    local_60 = DAT_1432ce5b0;
    local_58 = 0;
    local_48 = 0x5000000;
    local_50 = &local_40;
    local_44 = 0x2ff;
    local_40 = 0x3fff00103fff0010;
    local_38 = 0;
    lVar2 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x680));
    local_res10[0] = *(int *)(lVar2 + 0xa8);
    iVar4 = (local_48 & 0xffffff) - 1;
    uVar5 = uVar6;
    uVar7 = uVar6;
    if (-1 < iVar4) {
      do {
        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {
          if (-1 < (int)uVar7) goto LAB_1408545ca;
          break;
        }
        uVar5 = uVar5 + 1;
        uVar7 = (ulonglong)((int)uVar7 + 1);
      } while ((longlong)uVar5 <= (longlong)iVar4);
    }
    FUN_14052e460(&local_50,local_res10);
LAB_1408545ca:
    FUN_1407c6840(param_4,&local_60);
    FUN_14061a0a0(&local_50);
  }
  if (((((param_2 & 8) != 0) && (lVar2 = *(longlong *)(lVar3 + 0x70), lVar2 != 0)) &&
      (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), lVar1 != 0)) &&
     (lVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), *(longlong *)(lVar1 + 0xf0) != 0)) {
    local_60 = DAT_1432ce5c0;
    local_58 = 0;
    local_48 = 0x5000000;
    local_50 = &local_40;
    local_44 = 0x2ff;
    local_40 = 0x3fff00103fff0010;
    local_38 = 0;
    lVar2 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8));
    local_res10[0] = *(int *)(lVar2 + 0xa8);
    iVar4 = (local_48 & 0xffffff) - 1;
    uVar5 = uVar6;
    uVar7 = uVar6;
    if (-1 < iVar4) {
      do {
        if (*(int *)((longlong)local_50 + uVar5 * 4) == local_res10[0]) {
          if (-1 < (int)uVar7) goto LAB_1408546a9;
          break;
        }
        uVar5 = uVar5 + 1;
        uVar7 = (ulonglong)((int)uVar7 + 1);
      } while ((longlong)uVar5 <= (longlong)iVar4);
    }
    FUN_14052e460(&local_50,local_res10);
LAB_1408546a9:
    FUN_1407c6840(param_4,&local_60);
    FUN_14061a0a0(&local_50);
  }
  if ((((param_2 & 0x10) != 0) && (lVar2 = *(longlong *)(lVar3 + 0x68), lVar2 != 0)) &&
     ((lVar3 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), lVar3 != 0 &&
      (lVar3 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8)), *(longlong *)(lVar3 + 0xf0) != 0)))) {
    local_60 = DAT_1432ce5c8;
    local_58 = 0;
    local_48 = 0x5000000;
    local_50 = &local_40;
    local_44 = 0x2ff;
    local_40 = 0x3fff00103fff0010;
    local_38 = 0;
    lVar2 = FUN_1406dc560(*(undefined4 *)(lVar2 + 8));
    local_res10[0] = *(int *)(lVar2 + 0xa8);
    iVar4 = (local_48 & 0xffffff) - 1;
    uVar5 = uVar6;
    if (-1 < iVar4) {
      do {
        if (*(int *)((longlong)local_50 + uVar6 * 4) == local_res10[0]) {
          if (-1 < (int)uVar5) goto LAB_140854788;
          break;
        }
        uVar6 = uVar6 + 1;
        uVar5 = (ulonglong)((int)uVar5 + 1);
      } while ((longlong)uVar6 <= (longlong)iVar4);
    }
    FUN_14052e460(&local_50,local_res10);
LAB_140854788:
    FUN_1407c6840(param_4,&local_60);
    FUN_14061a0a0(&local_50);
  }
  if (((param_2 & 0x20) != 0) && (param_3 != 0)) {
    local_60 = DAT_1432ce5d0;
    local_58 = 0;
    local_48 = 0x5000000;
    local_50 = &local_40;
    local_44 = 0x2ff;
    local_40 = 0x3fff00103fff0010;
    local_38 = 0;
    local_res10[0] = *(int *)(param_3 + 0xa8);
    FUN_14052e460(&local_50,local_res10);
    FUN_1407c6840(param_4,&local_60);
    FUN_14061a0a0(&local_50);
  }
  return;
}


````

### result

````text

int * FUN_1408549c0(longlong *param_1,int *param_2,undefined8 param_3,undefined8 param_4,
                   float param_5)

{
  int *piVar1;
  longlong lVar2;
  char cVar3;
  int *piVar4;
  int local_res8 [2];
  undefined4 local_res10 [2];
  undefined4 uVar5;
  
  lVar2 = D2_GameSingletonPtr;
  uVar5 = 0;
  piVar4 = (int *)FUN_1407d6040(D2_GameSingletonPtr + 0x1f5338,local_res8,param_3,param_4,
                                &DAT_14398f888,0,0);
  piVar1 = (int *)((longlong)param_1 + 0x24);
  if (piVar1 != piVar4) {
    *piVar1 = *piVar4;
  }
  if (*piVar1 != 0) {
    (**(code **)(*param_1 + 0x88))(param_1);
    local_res8[0] = (int)(param_5 * 1000.0);
    local_res10[0] = 0;
    cVar3 = FUN_1407d9270(lVar2 + 0x1f5338,*piVar1,local_res10,local_res8,0,CONCAT44(uVar5,0x100002)
                         );
    if ((cVar3 == '\0') && (piVar1 != local_res8)) {
      *piVar1 = 0;
    }
  }
  (**(code **)(*param_1 + 0x80))(param_1,param_3);
  *param_2 = *piVar1;
  return param_2;
}


````

### result

````text

undefined4 *
FUN_1407d6040(longlong param_1,undefined4 *param_2,undefined8 param_3,undefined8 param_4,
             undefined4 *param_5,longlong param_6,undefined4 *param_7)

{
  longlong lVar1;
  undefined4 *puVar2;
  char cVar3;
  undefined4 uVar4;
  int iVar5;
  longlong lVar6;
  undefined1 local_58 [8];
  undefined4 local_50;
  undefined1 local_40 [24];
  
  puVar2 = param_7;
  lVar1 = param_6;
  cVar3 = FUN_1407d51e0(param_1,param_3,param_4,*param_5,param_6,param_7);
  if (cVar3 == '\0') {
    *param_2 = 0;
  }
  else {
    if (puVar2 != (undefined4 *)0x0) {
      *puVar2 = 0;
    }
    uVar4 = FUN_140158200(&DAT_14398f880);
    lVar6 = FUN_140652d10();
    FUN_1407ce370(lVar6,param_3,uVar4,param_5,lVar1,param_4,0);
    cVar3 = FUN_1407d69e0(param_1,lVar6,puVar2);
    if (cVar3 == '\0') {
      if (lVar1 != 0) {
        local_58[0] = 0x6d;
        local_50 = 0;
        FUN_140884920(lVar1,local_40,&DAT_1439a77a0,local_58);
        FUN_140882fc0(local_40);
      }
      FUN_1407d17d0(lVar6,0xa02);
      FUN_140652e70(lVar6);
      *param_2 = 0;
    }
    else {
      iVar5 = FUN_1401581c0(param_1 + 0x1b08,0,1);
      while (iVar5 == 1) {
        iVar5 = FUN_1401581c0(param_1 + 0x1b08,0,1);
      }
      param_5 = (undefined4 *)lVar6;
      if (*(char *)(lVar6 + 0x2b5) == '\0') {
        FUN_1408918f0();
      }
      else {
        FUN_140160b50(param_1,&param_5,*(undefined4 *)(param_1 + 0x11f0));
        *(int *)(param_1 + 0x11f0) = *(int *)(param_1 + 0x11f0) + 1;
      }
      FUN_1407d6ef0(param_1,*(undefined4 *)(lVar6 + 0x158));
      FUN_1401581e0(param_1 + 0x1b08,0);
      *param_2 = uVar4;
    }
  }
  return param_2;
}


````

### result

````text

undefined8
FUN_1407d9270(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
             undefined4 *param_5,undefined4 param_6)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 local_20;
  int local_18;
  
  local_18 = 0;
  local_20 = param_1;
  lVar2 = FUN_1407d5860(param_1,param_2,&local_20,param_4,0xfffffffffffffffe);
  FUN_1407cefa0(lVar2,param_6);
  if (*(int *)(lVar2 + 0x27c) == 8) {
    cVar1 = FUN_1407cfcd0(lVar2,0x902,1);
    if (cVar1 != '\0') {
      FUN_1407d17d0(lVar2,0x902);
      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));
    }
    if (param_5 != (undefined4 *)0x0) {
      *param_5 = 3;
    }
  }
  else {
    cVar1 = FUN_1407cbbe0(lVar2);
    if (cVar1 != '\0') {
      FUN_1407d2160(lVar2,param_3,param_4,param_5);
    }
    if (*(int *)(lVar2 + 0x27c) != 6) {
      uVar3 = 1;
      goto LAB_1407d936d;
    }
    cVar1 = FUN_1407cfcd0(lVar2,0x802,1);
    if (cVar1 != '\0') {
      FUN_1407d17d0(lVar2,0x802);
      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));
    }
  }
  uVar3 = 0;
LAB_1407d936d:
  if (local_18 != 0) {
    FUN_1407d7050(local_20);
  }
  return uVar3;
}


````

### result

````text

undefined4 FUN_1407d51e0(undefined8 param_1,longlong param_2,undefined8 param_3,int param_4)

{
  int *piVar1;
  char cVar2;
  int iVar3;
  char *pcVar4;
  undefined1 local_18 [16];
  
  cVar2 = FUN_1401975d0(param_2);
  if (cVar2 != '\0') {
                    /* WARNING: Subroutine does not return */
    FUN_140166830("arkMidnightManager::StartMdntInst: the midnight \'%s\' is empty.",
                  *(undefined8 *)(param_2 + 8));
  }
  piVar1 = (int *)FUN_1401962e0(param_2,local_18);
  if (*piVar1 == 0) {
    pcVar4 = "arkMidnightManager::StartMdntInst: the midnight \'%s\' has nothing linked to entry.";
  }
  else {
    cVar2 = FUN_1407d5f40(param_1,param_2,param_3);
    if (cVar2 != '\0') {
      if (param_4 == 1) {
        iVar3 = FUN_140197060(param_2);
        if (1 < iVar3) {
          pcVar4 = 
          "arkMidnightManager::StartMdntInst: the midnight \'%s\' can\'t be played in reverse.";
          goto LAB_1407d5249;
        }
      }
      return 1;
    }
    pcVar4 = 
    "arkMidnightManager::StartMdntInst: no corresponding entity(ies) for the midnight \'%s\'.";
  }
LAB_1407d5249:
                    /* WARNING: Subroutine does not return */
  FUN_140166830(pcVar4,*(undefined8 *)(param_2 + 8));
}


````

### result

````text

void FUN_1407ce370(undefined8 *param_1,undefined8 param_2,undefined4 param_3,int *param_4,
                  undefined8 param_5,undefined8 param_6)

{
  longlong *plVar1;
  byte bVar2;
  int iVar3;
  char cVar4;
  longlong lVar5;
  byte bVar6;
  longlong *plVar7;
  undefined4 uVar8;
  undefined4 local_res18 [2];
  
  if (param_1 + 0x2b != (undefined8 *)local_res18) {
    *(undefined4 *)(param_1 + 0x2b) = param_3;
  }
  *param_1 = param_2;
  *(undefined1 *)(param_1 + 0x56) = 0;
  param_1[0x52] = param_5;
  *(char *)((longlong)param_1 + 0x2b4) = (char)param_4[1];
  iVar3 = *param_4;
  *(int *)(param_1 + 0x50) = iVar3;
  if (iVar3 == 0) {
    uVar8 = 0x3f800000;
  }
  else {
    uVar8 = 0xbf800000;
  }
  *(undefined4 *)(param_1 + 0x2e) = uVar8;
  *(undefined1 *)((longlong)param_1 + 0x2b6) = *(undefined1 *)((longlong)param_4 + 5);
  local_res18[0] = param_3;
  FUN_1407cee60(param_1);
  FUN_1407ce5f0(param_1);
  FUN_1407ce7e0(param_1,param_6);
  *(undefined4 *)((longlong)param_1 + 0x27c) = 1;
  FUN_1407cf070(param_1);
  cVar4 = FUN_140195de0();
  if (cVar4 == '\0') {
    if (((*(uint *)(param_1 + 0x1d) & 0xffffff) == 0) && (param_1[0x2a] != 0)) {
      bVar6 = 1;
    }
    else {
      bVar6 = 0;
    }
  }
  else {
    bVar6 = 1;
  }
  *(byte *)((longlong)param_1 + 0x2b2) = bVar6;
  bVar2 = *(byte *)(param_4 + 1);
  *(byte *)((longlong)param_1 + 0x2b2) = bVar2 | bVar6;
  if ((bVar2 | bVar6) == 0) {
    plVar7 = (longlong *)param_1[0x1c];
    plVar1 = plVar7 + (*(uint *)(param_1 + 0x1d) & 0xffffff);
    for (; plVar7 != plVar1; plVar7 = plVar7 + 1) {
      lVar5 = *plVar7;
      FUN_1400998b0(lVar5);
      lVar5 = (**(code **)**(undefined8 **)(lVar5 + 0xd0))();
      if ((*(int *)(lVar5 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar5 + 0x60))) {
        bVar6 = 0;
      }
      else {
        bVar6 = 1;
      }
      *(byte *)((longlong)param_1 + 0x2b2) = *(byte *)((longlong)param_1 + 0x2b2) | bVar6;
    }
  }
  return;
}


````

### result

````text

byte FUN_1407d5f40(undefined8 param_1,longlong param_2,undefined8 *param_3)

{
  byte bVar1;
  longlong *plVar2;
  ulonglong uVar3;
  longlong lVar4;
  ulonglong uVar5;
  int iVar6;
  ulonglong uVar7;
  uint uVar8;
  ulonglong uVar9;
  byte bVar10;
  ulonglong uVar11;
  
  uVar7 = 0;
  bVar10 = 0;
  iVar6 = 0;
  uVar5 = uVar7;
  uVar9 = uVar7;
  uVar11 = uVar7;
  if ((*(uint *)(param_2 + 0xb8) & 0xffffff) != 0) {
    do {
      lVar4 = *(longlong *)(param_2 + 0xb0) + uVar11;
      bVar1 = FUN_140192d30(lVar4);
      bVar10 = bVar10 | bVar1;
      if ((*(uint *)(param_3 + 1) & 0xffffff) != 0) {
        plVar2 = (longlong *)*param_3;
        uVar3 = uVar7;
        do {
          if (*plVar2 == *(longlong *)(lVar4 + 0x10)) {
            uVar5 = (ulonglong)((int)uVar5 + 1);
            break;
          }
          uVar3 = uVar3 + 1;
          plVar2 = plVar2 + 6;
        } while ((longlong)uVar3 < (longlong)(ulonglong)(*(uint *)(param_3 + 1) & 0xffffff));
      }
      iVar6 = (int)uVar5;
      uVar8 = (int)uVar9 + 1;
      uVar9 = (ulonglong)uVar8;
      uVar11 = uVar11 + 0x388;
    } while ((int)uVar8 < (int)(*(uint *)(param_2 + 0xb8) & 0xffffff));
  }
  return bVar10 | 0 < iVar6;
}


````

### result

````text

void FUN_1407ce7e0(undefined **param_1,longlong *param_2)

{
  undefined4 uVar1;
  uint uVar2;
  undefined *puVar3;
  bool bVar4;
  undefined1 *puVar5;
  char cVar6;
  uint uVar7;
  undefined *puVar8;
  longlong lVar9;
  longlong *plVar10;
  longlong *plVar11;
  uint uVar12;
  int iVar13;
  ulonglong uVar14;
  longlong lVar15;
  longlong lVar16;
  ulonglong uVar17;
  ulonglong uVar18;
  ulonglong uVar19;
  char cVar20;
  longlong lVar21;
  undefined **local_res8;
  longlong *local_res10;
  char local_res18;
  undefined *local_res20;
  ulonglong in_stack_fffffffffffffc08;
  longlong local_3c0;
  longlong local_3b8;
  undefined **local_398;
  undefined1 **local_390;
  undefined4 local_388;
  undefined8 local_378;
  undefined1 *local_368;
  uint local_360;
  uint local_35c;
  undefined1 local_358 [256];
  undefined **local_258;
  undefined8 local_250;
  undefined4 local_248;
  undefined1 *local_240;
  undefined4 local_238;
  undefined4 local_234;
  undefined1 local_230 [384];
  undefined1 *local_b0;
  undefined4 local_a8;
  undefined4 local_a4;
  undefined1 local_a0 [64];
  undefined8 local_60;
  undefined4 local_58;
  undefined4 local_54;
  
  local_378 = 0xfffffffffffffffe;
  cVar20 = '\0';
  local_res18 = '\0';
  local_360 = 0x5000000;
  local_368 = local_358;
  local_35c = 0x20ff;
  puVar3 = *param_1;
  uVar7 = *(uint *)(puVar3 + 0xb8) & 0xffffff;
  local_3b8 = (longlong)(int)uVar7;
  local_res8 = param_1;
  local_res10 = param_2;
  if (uVar7 != 0) {
    local_3c0 = 0;
    do {
      plVar11 = local_res10;
      uVar17 = 0;
      lVar16 = *(longlong *)(puVar3 + 0xb0) + local_3c0;
      cVar6 = FUN_140190c20(lVar16 + 0x10);
      uVar18 = 0xffffffffffffffff;
      if ((*(uint *)(plVar11 + 1) & 0xffffff) != 0) {
        plVar10 = (longlong *)*local_res10;
        uVar14 = uVar17;
        uVar19 = uVar17;
        do {
          uVar18 = uVar19;
          if (*plVar10 == *(longlong *)(lVar16 + 0x10)) break;
          uVar7 = (int)uVar14 + 1;
          uVar14 = (ulonglong)uVar7;
          uVar19 = uVar19 + 1;
          plVar10 = plVar10 + 6;
          uVar18 = 0xffffffffffffffff;
        } while ((int)uVar7 < (int)(*(uint *)(plVar11 + 1) & 0xffffff));
      }
      if (cVar6 == '\0') {
        if ((longlong)uVar18 < 0) {
          if ((*(longlong *)(lVar16 + 0x328) != 0) && (*(char *)((longlong)param_1 + 0x2b4) == '\0')
             ) {
            in_stack_fffffffffffffc08 = 0;
            lVar15 = FUN_1408ae9b0(D2_GameSingletonPtr,*(longlong *)(lVar16 + 0x328),&DAT_1431af668,
                                   &DAT_14228a780,0,1,0,0,0);
            *(byte *)(lVar15 + 0xd4) = *(byte *)(lVar15 + 0xd4) | 4;
            local_res18 = '\x01';
            puVar8 = (undefined *)FUN_140652d60();
            local_res20 = puVar8;
            if (puVar8 != (undefined *)0x0) {
              FUN_1407db090(puVar8,param_1,lVar16,0);
            }
            FUN_1407dc130(puVar8,lVar15,1);
            FUN_1408918f0(&local_368,&local_res20);
            cVar20 = '\x01';
          }
        }
        else {
          lVar21 = uVar18 * 0x30;
          lVar15 = *local_res10;
          plVar11 = local_res10;
          uVar18 = uVar17;
          cVar20 = local_res18;
          if ((*(uint *)(lVar15 + 0x18 + lVar21) & 0xffffff) != 0) {
            do {
              local_res20 = (undefined *)
                            CONCAT44(local_res20._4_4_,
                                     *(undefined4 *)(uVar17 + *(longlong *)(lVar15 + 0x10 + lVar21))
                                    );
              lVar9 = FUN_1407b5e00(D2_GameSingletonPtr + 0x3130b0,&local_res20);
              if ((lVar9 != 0) && (lVar9 = FUN_1407d5c40(lVar9,0), lVar9 != 0)) {
                uVar1 = *(undefined4 *)(lVar21 + 8 + *plVar11);
                puVar8 = (undefined *)FUN_140652d60();
                param_1 = local_res8;
                local_res20 = puVar8;
                if (puVar8 != (undefined *)0x0) {
                  FUN_1407db090(puVar8,local_res8,lVar16,uVar1);
                }
                FUN_1407dc130(puVar8,lVar9,0);
                FUN_1408918f0(&local_368,&local_res20);
                plVar11 = local_res10;
              }
              uVar7 = (int)uVar18 + 1;
              uVar17 = uVar17 + 4;
              uVar18 = (ulonglong)uVar7;
              cVar20 = local_res18;
            } while ((int)uVar7 < (int)(*(uint *)(lVar15 + 0x18 + lVar21) & 0xffffff));
          }
        }
      }
      else {
        puVar8 = (undefined *)FUN_140652d60();
        local_res20 = puVar8;
        if (puVar8 != (undefined *)0x0) {
          FUN_1407db090(puVar8,param_1,lVar16,0);
        }
        FUN_1407dc120(puVar8);
        param_1[0x2a] = puVar8;
        FUN_1408918f0(&local_368,&local_res20);
      }
      local_3c0 = local_3c0 + 0x388;
      local_3b8 = local_3b8 + -1;
    } while (local_3b8 != 0);
  }
  lVar16 = 0;
  uVar7 = local_360 & 0xffffff;
  if (((uVar7 < 5) || (DAT_14398f828 == 0)) ||
     (bVar4 = true, *(char *)(D2_GameSingletonPtr + 0x337122) == '\0')) {
    bVar4 = false;
  }
  if (bVar4) {
    local_250 = 0;
    local_248 = 0x1ffff;
    local_258 = &PTR_LAB_141ad4a48;
    local_240 = local_230;
    local_238 = 0;
    local_234 = 0x80000010;
    local_b0 = local_a0;
    local_a8 = 0;
    local_a4 = 0x80000010;
    local_60 = 0;
    local_58 = 0;
    local_54 = 0x80000000;
    local_398 = &PTR_LAB_141c6b370;
    local_390 = &local_368;
    local_388 = 0xffffffff;
    FUN_140aa96f0(&local_258,&local_res8,&local_398,*(undefined4 *)(D2_GameSingletonPtr + 0x1f6524),
                  in_stack_fffffffffffffc08 & 0xffffffff00000000);
    FUN_14007a9e0(&local_258);
    local_398 = &PTR_LAB_141abd918;
    FUN_140090650(&local_258);
  }
  else {
    lVar15 = lVar16;
    if (uVar7 != 0) {
      do {
        if (((*(byte *)(*(longlong *)(local_368 + lVar15 * 8) + 0x1ac) & 8) == 0) &&
           (*(longlong *)(*(longlong *)(local_368 + lVar15 * 8) + 0xd0) != 0)) {
          FUN_1407dc1d0();
        }
        lVar15 = lVar15 + 1;
      } while (lVar15 < (int)uVar7);
    }
  }
  if (cVar20 != '\0') {
    FUN_1407cf680(param_1,&local_368);
  }
  if (*(uint *)((longlong)param_1 + 0x1c) >> 8 < uVar7) {
    FUN_1407d01e0(param_1 + 2,uVar7);
  }
  if (uVar7 != 0) {
    do {
      lVar15 = *(longlong *)(local_368 + lVar16 * 8);
      uVar2 = *(uint *)((longlong)param_1 + 0x1c);
      uVar12 = uVar2 >> 8;
      if ((*(uint *)(param_1 + 3) & 0xffffff) == uVar12) {
        if (((byte)uVar2 & 0xc0) == 0xc0) {
          if (DAT_1431aee98 == '\0') {
            if ((uVar2 & 0x3f) == 0x3f) {
              if (uVar2 >> 8 == 0) {
                iVar13 = 1;
              }
              else if (uVar12 < 0x40000000) {
                iVar13 = uVar12 * 2;
              }
              else {
                iVar13 = 0x7fffffff;
              }
            }
            else {
              iVar13 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
              uVar12 = uVar12 + iVar13;
              iVar13 = uVar12 - (iVar13 - 1U & uVar12);
            }
          }
          else {
            iVar13 = uVar12 + 1;
          }
          FUN_1407cf300(param_1 + 2,iVar13);
        }
        else {
          if (DAT_1431aee98 == '\0') {
            if ((uVar2 & 0x3f) == 0x3f) {
              if (uVar2 >> 8 == 0) {
                iVar13 = 1;
              }
              else if (uVar12 < 0x40000000) {
                iVar13 = uVar12 * 2;
              }
              else {
                iVar13 = 0x7fffffff;
              }
            }
            else {
              iVar13 = 1 << ((byte)(uVar2 & 0x3f) & 0x1f);
              uVar12 = uVar12 + iVar13;
              iVar13 = uVar12 - (iVar13 - 1U & uVar12);
            }
          }
          else {
            iVar13 = uVar12 + 1;
          }
          FUN_1407d01e0(param_1 + 2,iVar13);
        }
      }
      uVar12 = *(uint *)((longlong)param_1 + 0x1c) >> 8;
      uVar2 = *(uint *)(param_1 + 3);
      if ((uVar2 & 0xffffff) < uVar12) {
        plVar11 = (longlong *)(param_1[2] + (ulonglong)(uVar2 & 0xffffff) * 0x20);
        *(uint *)(param_1 + 3) = (uVar2 + 1 ^ uVar2) & 0xffffff ^ uVar2;
      }
      else {
        plVar11 = (longlong *)(param_1[2] + (ulonglong)(uVar12 - 1) * 0x20);
      }
      *plVar11 = lVar15;
      plVar11[2] = *(longlong *)(lVar15 + 0xb0);
      *(undefined4 *)(plVar11 + 3) = *(undefined4 *)(lVar15 + 0xa8);
      if ((((*(byte *)(lVar15 + 0x1ac) & 1) == 0) && ((*(byte *)(lVar15 + 0x1ac) & 8) == 0)) &&
         (*(longlong *)(lVar15 + 0xd0) != 0)) {
        plVar11[1] = *(longlong *)(lVar15 + 0xd0);
      }
      if (*(longlong *)(lVar15 + 0xb0) == DAT_1432ce5b8) {
        *(undefined1 *)((longlong)param_1 + 0x2b5) = 1;
      }
      lVar16 = lVar16 + 1;
    } while (lVar16 < (int)uVar7);
  }
  local_res8 = &PTR_FUN_141c6b3b0;
  if (param_1[0x1c] != (undefined *)0x0) {
    FUN_1407d1e50(&local_res8,param_1[0x1c],*(uint *)(param_1 + 0x1d) & 0xffffff);
  }
  puVar5 = local_368;
  if (((local_35c & 0x40) == 0) && (local_368 != (undefined1 *)0x0)) {
    plVar11 = (longlong *)FUN_1401548e0();
    (**(code **)(*plVar11 + 0x10))
              (plVar11,
               "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
               ,0x1ec,puVar5,0x10);
  }
  return;
}


````

### result

````text

void FUN_1407dc1d0(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  byte bVar4;
  int *piVar5;
  undefined8 uVar6;
  undefined1 *puVar7;
  char *pcVar8;
  undefined4 *puVar9;
  int *piVar10;
  char cVar11;
  byte bVar12;
  ushort *puVar13;
  uint uVar14;
  undefined8 uVar15;
  ushort local_res8 [4];
  ushort local_res10 [4];
  ushort local_res18 [4];
  undefined1 local_res20 [8];
  undefined4 local_78;
  undefined8 local_74;
  undefined4 local_6c;
  byte local_68;
  ushort local_64;
  undefined4 local_60;
  undefined8 local_5c;
  undefined4 local_54;
  byte local_50;
  ushort local_4c;
  undefined1 local_48 [8];
  undefined1 local_40 [8];
  undefined1 local_38 [8];
  undefined1 local_30 [8];
  undefined1 local_28 [16];
  
  FUN_140883a10(*(undefined8 *)(param_1 + 0xd0),param_1);
  FUN_1407db670(param_1);
  piVar5 = (int *)FUN_140191b60(*(undefined8 *)(param_1 + 0xb8));
  iVar1 = *piVar5;
  if (iVar1 == 1) {
    FUN_140131f90(local_48,"Sword",0);
    uVar15 = *(undefined8 *)(param_1 + 0xf8);
    uVar6 = FUN_140131ae0(local_28);
    puVar7 = local_48;
    puVar13 = local_res18;
  }
  else if (iVar1 == 2) {
    FUN_140131f90(local_40,"Empty",0);
    uVar15 = *(undefined8 *)(param_1 + 0xf8);
    uVar6 = FUN_140131ae0(local_38);
    puVar7 = local_40;
    puVar13 = local_res10;
  }
  else {
    if (iVar1 != 3) goto LAB_1407dc2b7;
    FUN_140131f90(local_res20,"SwordGunAim",0);
    uVar15 = *(undefined8 *)(param_1 + 0xf8);
    uVar6 = FUN_140131ae0(local_30);
    puVar7 = local_res20;
    puVar13 = local_res8;
  }
  puVar7 = (undefined1 *)FUN_1405484d0(uVar15,puVar13,puVar7,uVar6);
  if ((undefined1 *)(param_1 + 0x189) != puVar7) {
    *(undefined1 *)(param_1 + 0x189) = *puVar7;
  }
LAB_1407dc2b7:
  if (-1 < *(char *)(param_1 + 0x189)) {
    puVar7 = (undefined1 *)
             FUN_140548230(*(undefined8 *)(param_1 + 0xf8),local_res8,
                           *(undefined1 *)(param_1 + 0x189));
    if ((undefined1 *)(param_1 + 0x188) != puVar7) {
      *(undefined1 *)(param_1 + 0x188) = *puVar7;
    }
  }
  uVar14 = 0;
  if ((-1 < *(char *)(param_1 + 0x188)) && (-1 < *(char *)(param_1 + 0x189))) {
    pcVar8 = (char *)FUN_140548950(*(undefined8 *)(param_1 + 0xf8),local_res8);
    if ((*pcVar8 == *(char *)(param_1 + 0x188)) &&
       (pcVar8 = (char *)FUN_140548480(*(undefined8 *)(param_1 + 0xf8),local_res8),
       *pcVar8 == *(char *)(param_1 + 0x189))) {
      *(undefined2 *)(param_1 + 0x188) = 0xffff;
    }
    else {
      local_68 = local_68 & 0xfe;
      local_50 = local_50 & 0xfe;
      local_res8[0] = (ushort)*(byte *)(param_1 + 0x188);
      local_64 = local_res8[0];
      local_res8[0] = (ushort)*(byte *)(param_1 + 0x189);
      local_78 = 0;
      local_74 = 1;
      local_6c = 0;
      local_60 = 0;
      local_4c = local_res8[0];
      local_5c = 1;
      local_54 = 0;
      puVar9 = (undefined4 *)FUN_140545a00(*(undefined8 *)(param_1 + 0xf8),local_res8,&local_78);
      if ((undefined4 *)(param_1 + 0x17c) != puVar9) {
        *(undefined4 *)(param_1 + 0x17c) = *puVar9;
      }
      puVar9 = (undefined4 *)FUN_140544ed0(*(undefined8 *)(param_1 + 0xf8),local_res8,&local_60);
      if ((undefined4 *)(param_1 + 0x180) != puVar9) {
        *(undefined4 *)(param_1 + 0x180) = *puVar9;
      }
    }
  }
  lVar2 = *(longlong *)(param_1 + 0xd0);
  bVar4 = *(byte *)(lVar2 + 0xd1) >> 5 & 1;
  bVar12 = *(byte *)(lVar2 + 0xd1) >> 3 & 1;
  if ((bVar12 == 0) && (bVar4 == 0)) {
    cVar11 = '\0';
  }
  else {
    cVar11 = '\x01';
  }
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) & 0xfb;
  *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | cVar11 << 2;
  lVar3 = *(longlong *)(param_1 + 0xb8);
  if ((((bVar12 != 0) && ((*(byte *)(lVar3 + 0x264) & 4) == 0)) &&
      ((*(byte *)(lVar3 + 0x268) & 4) != 0)) ||
     (((bVar4 != 0 && ((*(byte *)(lVar3 + 0x264) & 2) == 0)) &&
      ((*(byte *)(lVar3 + 0x268) & 2) != 0)))) {
    *(byte *)(param_1 + 0x1ac) = *(byte *)(param_1 + 0x1ac) | 2;
    *(undefined4 *)(param_1 + 400) = *(undefined4 *)(lVar2 + 0xa8);
    *(undefined4 *)(param_1 + 0x194) = *(undefined4 *)(lVar2 + 0x300);
    *(undefined4 *)(param_1 + 0x198) = *(undefined4 *)(lVar2 + 0x304);
    *(undefined4 *)(param_1 + 0x19c) = *(undefined4 *)(lVar2 + 0x308);
  }
  piVar10 = *(int **)(lVar3 + 0x208);
  piVar5 = piVar10 + (*(uint *)(lVar3 + 0x210) & 0xffffff);
  for (; piVar10 != piVar5; piVar10 = piVar10 + 1) {
    uVar14 = uVar14 | *(uint *)(&DAT_141c6c7a0 + (longlong)*piVar10 * 4);
  }
  *(uint *)(param_1 + 0x1a0) = uVar14;
  if (*(longlong *)(param_1 + 0xb0) == DAT_1432ce5b8) {
    *(undefined4 *)(param_1 + 0x1a8) = 2;
    return;
  }
  if (*(longlong *)(param_1 + 0xe8) != 0) {
    FUN_140767420(*(longlong *)(param_1 + 0xe8),
                  *(undefined4 *)(*(longlong *)(param_1 + 200) + 0x158),0);
  }
  return;
}


````
