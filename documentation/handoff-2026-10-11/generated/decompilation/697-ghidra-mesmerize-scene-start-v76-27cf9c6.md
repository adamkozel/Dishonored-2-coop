# ghidra-mesmerize-scene-start-v76.json

Original: [work/ghidra-mesmerize-scene-start-v76.json](<../../../../work/ghidra-mesmerize-scene-start-v76.json>)

SHA-256: `0BB06B22F15B16F288569AF441F1BF286EEE384688FE9E1DD3438FC8785D4909`; 28793 bytes.

## Original saved record

````json
[
  {
    "address": "14072af30",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\n/* WARNING: Removing unreachable block (ram,0x00014072b030) */\r\n/* WARNING: Removing unreachable block (ram,0x00014072b043) */\r\n/* WARNING: Removing unreachable block (ram,0x00014072b036) */\r\n/* WARNING: Removing unreachable block (ram,0x00014072b041) */\r\n\r\nundefined8 FUN_14072af30(longlong param_1,undefined8 param_2,undefined8 param_3)\r\n\r\n{\r\n  uint uVar1;\r\n  undefined4 local_res8 [2];\r\n  longlong local_60;\r\n  uint local_58;\r\n  uint local_54;\r\n  undefined8 local_50;\r\n  undefined4 local_48;\r\n  undefined8 *local_40;\r\n  undefined4 local_38;\r\n  undefined4 local_34;\r\n  undefined8 local_30;\r\n  undefined1 local_28;\r\n  \r\n  local_60 = 0;\r\n  FUN_1401548e0();\r\n  local_58 = 0x5000000;\r\n  local_54 = 0x3f;\r\n  uVar1 = 0x5000000;\r\n  if (local_60 != 0) {\r\n    FUN_1409d8700(local_60,0);\r\n    uVar1 = local_58;\r\n  }\r\n  local_60 = 0;\r\n  local_54 = local_54 & 0xff;\r\n  local_58 = uVar1 & 0xff000000;\r\n  local_50 = DAT_14398dae0;\r\n  local_48 = 0;\r\n  local_38 = 0x5000000;\r\n  local_40 = &local_30;\r\n  local_34 = 0x2ff;\r\n  local_30 = 0x3fff00103fff0010;\r\n  local_28 = 0;\r\n  local_res8[0] = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n  FUN_14052e460(&local_40,local_res8);\r\n  FUN_1407c6840(&local_60,&local_50);\r\n  local_res8[0] = 0;\r\n  FUN_1407d91c0(D2_GameSingletonPtr + 0x1f5338,param_2,param_3,&local_60,0,local_res8,0);\r\n  FUN_14061a0a0(&local_40);\r\n  if (((local_54 & 0x40) == 0) && (local_60 != 0)) {\r\n    FUN_1409d8700(local_60,local_54 >> 8);\r\n  }\r\n  return param_2;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\n/* WARNING: Removing unreachable block (ram,0x00014072b030) */\r\n/* WARNING: Removing unreachable block (ram,0x00014072b043) */\r\n/* WARNING: Removing unreachable block (ram,0x00014072b036) */\r\n/* WARNING: Removing unreachable block (ram,0x00014072b041) */\r\n\r\nundefined8 FUN_14072af30(longlong param_1,undefined8 param_2,undefined8 param_3)\r\n\r\n{\r\n  uint uVar1;\r\n  undefined4 local_res8 [2];\r\n  longlong local_60;\r\n  uint local_58;\r\n  uint local_54;\r\n  undefined8 local_50;\r\n  undefined4 local_48;\r\n  undefined8 *local_40;\r\n  undefined4 local_38;\r\n  undefined4 local_34;\r\n  undefined8 local_30;\r\n  undefined1 local_28;\r\n  \r\n  local_60 = 0;\r\n  FUN_1401548e0();\r\n  local_58 = 0x5000000;\r\n  local_54 = 0x3f;\r\n  uVar1 = 0x5000000;\r\n  if (local_60 != 0) {\r\n    FUN_1409d8700(local_60,0);\r\n    uVar1 = local_58;\r\n  }\r\n  local_60 = 0;\r\n  local_54 = local_54 & 0xff;\r\n  local_58 = uVar1 & 0xff000000;\r\n  local_50 = DAT_14398dae0;\r\n  local_48 = 0;\r\n  local_38 = 0x5000000;\r\n  local_40 = &local_30;\r\n  local_34 = 0x2ff;\r\n  local_30 = 0x3fff00103fff0010;\r\n  local_28 = 0;\r\n  local_res8[0] = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);\r\n  FUN_14052e460(&local_40,local_res8);\r\n  FUN_1407c6840(&local_60,&local_50);\r\n  local_res8[0] = 0;\r\n  FUN_1407d91c0(D2_GameSingletonPtr + 0x1f5338,param_2,param_3,&local_60,0,local_res8,0);\r\n  FUN_14061a0a0(&local_40);\r\n  if (((local_54 & 0x40) == 0) && (local_60 != 0)) {\r\n    FUN_1409d8700(local_60,local_54 >> 8);\r\n  }\r\n  return param_2;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407d9270",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nundefined8\r\nFUN_1407d9270(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,\r\n             undefined4 *param_5,undefined4 param_6)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  undefined8 local_20;\r\n  int local_18;\r\n  \r\n  local_18 = 0;\r\n  local_20 = param_1;\r\n  lVar2 = FUN_1407d5860(param_1,param_2,&local_20,param_4,0xfffffffffffffffe);\r\n  FUN_1407cefa0(lVar2,param_6);\r\n  if (*(int *)(lVar2 + 0x27c) == 8) {\r\n    cVar1 = FUN_1407cfcd0(lVar2,0x902,1);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d17d0(lVar2,0x902);\r\n      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));\r\n    }\r\n    if (param_5 != (undefined4 *)0x0) {\r\n      *param_5 = 3;\r\n    }\r\n  }\r\n  else {\r\n    cVar1 = FUN_1407cbbe0(lVar2);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d2160(lVar2,param_3,param_4,param_5);\r\n    }\r\n    if (*(int *)(lVar2 + 0x27c) != 6) {\r\n      uVar3 = 1;\r\n      goto LAB_1407d936d;\r\n    }\r\n    cVar1 = FUN_1407cfcd0(lVar2,0x802,1);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d17d0(lVar2,0x802);\r\n      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));\r\n    }\r\n  }\r\n  uVar3 = 0;\r\nLAB_1407d936d:\r\n  if (local_18 != 0) {\r\n    FUN_1407d7050(local_20);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nundefined8\r\nFUN_1407d9270(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,\r\n             undefined4 *param_5,undefined4 param_6)\r\n\r\n{\r\n  char cVar1;\r\n  longlong lVar2;\r\n  undefined8 uVar3;\r\n  undefined8 local_20;\r\n  int local_18;\r\n  \r\n  local_18 = 0;\r\n  local_20 = param_1;\r\n  lVar2 = FUN_1407d5860(param_1,param_2,&local_20,param_4,0xfffffffffffffffe);\r\n  FUN_1407cefa0(lVar2,param_6);\r\n  if (*(int *)(lVar2 + 0x27c) == 8) {\r\n    cVar1 = FUN_1407cfcd0(lVar2,0x902,1);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d17d0(lVar2,0x902);\r\n      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));\r\n    }\r\n    if (param_5 != (undefined4 *)0x0) {\r\n      *param_5 = 3;\r\n    }\r\n  }\r\n  else {\r\n    cVar1 = FUN_1407cbbe0(lVar2);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d2160(lVar2,param_3,param_4,param_5);\r\n    }\r\n    if (*(int *)(lVar2 + 0x27c) != 6) {\r\n      uVar3 = 1;\r\n      goto LAB_1407d936d;\r\n    }\r\n    cVar1 = FUN_1407cfcd0(lVar2,0x802,1);\r\n    if (cVar1 != '\\0') {\r\n      FUN_1407d17d0(lVar2,0x802);\r\n      FUN_1407d7050(param_1,*(undefined4 *)(lVar2 + 0x158));\r\n    }\r\n  }\r\n  uVar3 = 0;\r\nLAB_1407d936d:\r\n  if (local_18 != 0) {\r\n    FUN_1407d7050(local_20);\r\n  }\r\n  return uVar3;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407d91c0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nint * FUN_1407d91c0(undefined8 param_1,int *param_2,undefined8 param_3,undefined8 param_4,\r\n                   undefined8 param_5,undefined8 param_6,undefined8 param_7)\r\n\r\n{\r\n  int iVar1;\r\n  char cVar2;\r\n  int local_18 [4];\r\n  \r\n  local_18[0] = 0;\r\n  FUN_1407d6040(param_1,local_18,param_3,param_4,&DAT_14398f888,param_5,param_7);\r\n  iVar1 = local_18[0];\r\n  if (local_18[0] != 0) {\r\n    local_18[0] = 0;\r\n    cVar2 = FUN_1407d9270(param_1,iVar1,local_18,param_6,param_7,0x100002);\r\n    if (cVar2 != '\\0') {\r\n      *param_2 = iVar1;\r\n      return param_2;\r\n    }\r\n  }\r\n  *param_2 = 0;\r\n  return param_2;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nint * FUN_1407d91c0(undefined8 param_1,int *param_2,undefined8 param_3,undefined8 param_4,\r\n                   undefined8 param_5,undefined8 param_6,undefined8 param_7)\r\n\r\n{\r\n  int iVar1;\r\n  char cVar2;\r\n  int local_18 [4];\r\n  \r\n  local_18[0] = 0;\r\n  FUN_1407d6040(param_1,local_18,param_3,param_4,&DAT_14398f888,param_5,param_7);\r\n  iVar1 = local_18[0];\r\n  if (local_18[0] != 0) {\r\n    local_18[0] = 0;\r\n    cVar2 = FUN_1407d9270(param_1,iVar1,local_18,param_6,param_7,0x100002);\r\n    if (cVar2 != '\\0') {\r\n      *param_2 = iVar1;\r\n      return param_2;\r\n    }\r\n  }\r\n  *param_2 = 0;\r\n  return param_2;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407cbbe0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nundefined1 FUN_1407cbbe0(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  \r\n  if (*(int *)(param_1 + 0x27c) == 2) {\r\n    cVar1 = FUN_1407cf8e0();\r\n    if (cVar1 == '\\0') {\r\n      cVar1 = FUN_1407cf870(param_1);\r\n      if (cVar1 == '\\0') {\r\n        return 1;\r\n      }\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nundefined1 FUN_1407cbbe0(longlong param_1)\r\n\r\n{\r\n  char cVar1;\r\n  \r\n  if (*(int *)(param_1 + 0x27c) == 2) {\r\n    cVar1 = FUN_1407cf8e0();\r\n    if (cVar1 == '\\0') {\r\n      cVar1 = FUN_1407cf870(param_1);\r\n      if (cVar1 == '\\0') {\r\n        return 1;\r\n      }\r\n    }\r\n  }\r\n  return 0;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407d2160",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nvoid FUN_1407d2160(longlong *param_1,int *param_2,int *param_3,int *param_4)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  char cVar2;\r\n  int iVar3;\r\n  uint uVar4;\r\n  int iVar5;\r\n  int local_res8 [2];\r\n  uint local_res10 [2];\r\n  undefined4 local_b8;\r\n  int local_b4;\r\n  undefined4 local_b0;\r\n  undefined8 local_a0;\r\n  undefined4 local_98;\r\n  undefined4 local_94;\r\n  undefined8 local_90;\r\n  undefined4 local_88;\r\n  undefined4 local_84;\r\n  undefined4 local_80;\r\n  undefined8 local_7c;\r\n  byte local_74;\r\n  undefined8 local_70;\r\n  undefined4 local_68;\r\n  undefined4 local_64;\r\n  undefined8 local_60;\r\n  undefined4 local_58;\r\n  undefined4 local_54;\r\n  undefined4 local_50;\r\n  undefined8 local_4c;\r\n  byte local_44;\r\n  \r\n  uVar4 = *(uint *)(param_1 + 0x4f) >> 0x1a;\r\n  iVar5 = 0;\r\n  local_res10[1] = 0;\r\n  local_res10[0] = uVar4;\r\n  FUN_1407cae40(param_1 + 0x45,local_res10);\r\n  iVar3 = FUN_140196fe0(*param_1,uVar4 << 0x1a);\r\n  if (*param_2 <= iVar3) {\r\n    if ((int)param_1[0x50] == 0) {\r\n      local_b4 = *param_2;\r\n      iVar5 = iVar3 + -1;\r\n    }\r\n    else {\r\n      local_b4 = (iVar3 - *param_2) + -1;\r\n    }\r\n    puVar1 = (undefined4 *)((longlong)param_1 + 0x15c);\r\n    local_b8 = 0;\r\n    local_b0 = 0;\r\n    *puVar1 = 0;\r\n    *(int *)(param_1 + 0x2c) = local_b4;\r\n    local_res8[0] = 0;\r\n    *(undefined4 *)((longlong)param_1 + 0x164) = 0;\r\n    *(undefined4 *)(param_1 + 0x2d) = 0;\r\n    cVar2 = FUN_1407cbc20(param_1,uVar4,local_res8);\r\n    if (cVar2 == '\\0') {\r\n      if ((*(char *)((longlong)param_1 + 0x2b6) == '\\0') || ((local_res8[0] - 2U & 0xfffffffb) != 0)\r\n         ) {\r\n        if (param_4 != (int *)0x0) {\r\n          *param_4 = local_res8[0];\r\n        }\r\n        *(undefined4 *)((longlong)param_1 + 0x27c) = 6;\r\n      }\r\n    }\r\n    else {\r\n      FUN_1407d2650(param_1);\r\n      FUN_1407cd480(param_1,4);\r\n      local_44 = local_44 & 0xf8;\r\n      *(undefined1 *)((longlong)param_1 + 0x2b1) = 0;\r\n      local_70 = 0xffffffffffffffff;\r\n      local_68 = 0xffffffff;\r\n      local_64 = 0;\r\n      local_60 = 0xffffffffffffffff;\r\n      local_58 = 0xffffffff;\r\n      local_54 = 0;\r\n      local_50 = 0xbf800000;\r\n      local_4c = 0;\r\n      local_res10[0] = 0;\r\n      FUN_1407ce4f0(&local_70,puVar1,(int)param_1[0x50],0,iVar5);\r\n      FUN_1407ce0f0(param_1,&local_70,0x101,1);\r\n      FUN_1407d2b80(param_1,puVar1);\r\n      if (0 < *param_3) {\r\n        FUN_1407cc2d0(param_1,local_res10);\r\n        local_74 = local_74 & 0xf8;\r\n        local_a0 = 0xffffffffffffffff;\r\n        local_98 = 0xffffffff;\r\n        local_94 = 0;\r\n        local_90 = 0xffffffffffffffff;\r\n        local_88 = 0xffffffff;\r\n        local_84 = 0;\r\n        local_80 = 0xbf800000;\r\n        local_7c = 0;\r\n        FUN_1407ce4f0(&local_a0,puVar1,(int)param_1[0x50],local_res10[0],iVar5);\r\n        FUN_1407d3130(param_1,&local_a0);\r\n      }\r\n      local_b0 = (undefined4)param_1[0x2b];\r\n      *(undefined4 *)((longlong)param_1 + 0x27c) = 3;\r\n      if (param_1[0x52] != 0) {\r\n        local_b8 = CONCAT31(local_b8._1_3_,0x6d);\r\n        FUN_140884920(param_1[0x52],&local_a0,&DAT_1439a7660,&local_b8);\r\n        FUN_140882fc0(&local_a0);\r\n      }\r\n    }\r\n    return;\r\n  }\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(\"%s: StartTime is superior to first scene length.\",*(undefined8 *)(*param_1 + 8));\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nvoid FUN_1407d2160(longlong *param_1,int *param_2,int *param_3,int *param_4)\r\n\r\n{\r\n  undefined4 *puVar1;\r\n  char cVar2;\r\n  int iVar3;\r\n  uint uVar4;\r\n  int iVar5;\r\n  int local_res8 [2];\r\n  uint local_res10 [2];\r\n  undefined4 local_b8;\r\n  int local_b4;\r\n  undefined4 local_b0;\r\n  undefined8 local_a0;\r\n  undefined4 local_98;\r\n  undefined4 local_94;\r\n  undefined8 local_90;\r\n  undefined4 local_88;\r\n  undefined4 local_84;\r\n  undefined4 local_80;\r\n  undefined8 local_7c;\r\n  byte local_74;\r\n  undefined8 local_70;\r\n  undefined4 local_68;\r\n  undefined4 local_64;\r\n  undefined8 local_60;\r\n  undefined4 local_58;\r\n  undefined4 local_54;\r\n  undefined4 local_50;\r\n  undefined8 local_4c;\r\n  byte local_44;\r\n  \r\n  uVar4 = *(uint *)(param_1 + 0x4f) >> 0x1a;\r\n  iVar5 = 0;\r\n  local_res10[1] = 0;\r\n  local_res10[0] = uVar4;\r\n  FUN_1407cae40(param_1 + 0x45,local_res10);\r\n  iVar3 = FUN_140196fe0(*param_1,uVar4 << 0x1a);\r\n  if (*param_2 <= iVar3) {\r\n    if ((int)param_1[0x50] == 0) {\r\n      local_b4 = *param_2;\r\n      iVar5 = iVar3 + -1;\r\n    }\r\n    else {\r\n      local_b4 = (iVar3 - *param_2) + -1;\r\n    }\r\n    puVar1 = (undefined4 *)((longlong)param_1 + 0x15c);\r\n    local_b8 = 0;\r\n    local_b0 = 0;\r\n    *puVar1 = 0;\r\n    *(int *)(param_1 + 0x2c) = local_b4;\r\n    local_res8[0] = 0;\r\n    *(undefined4 *)((longlong)param_1 + 0x164) = 0;\r\n    *(undefined4 *)(param_1 + 0x2d) = 0;\r\n    cVar2 = FUN_1407cbc20(param_1,uVar4,local_res8);\r\n    if (cVar2 == '\\0') {\r\n      if ((*(char *)((longlong)param_1 + 0x2b6) == '\\0') || ((local_res8[0] - 2U & 0xfffffffb) != 0)\r\n         ) {\r\n        if (param_4 != (int *)0x0) {\r\n          *param_4 = local_res8[0];\r\n        }\r\n        *(undefined4 *)((longlong)param_1 + 0x27c) = 6;\r\n      }\r\n    }\r\n    else {\r\n      FUN_1407d2650(param_1);\r\n      FUN_1407cd480(param_1,4);\r\n      local_44 = local_44 & 0xf8;\r\n      *(undefined1 *)((longlong)param_1 + 0x2b1) = 0;\r\n      local_70 = 0xffffffffffffffff;\r\n      local_68 = 0xffffffff;\r\n      local_64 = 0;\r\n      local_60 = 0xffffffffffffffff;\r\n      local_58 = 0xffffffff;\r\n      local_54 = 0;\r\n      local_50 = 0xbf800000;\r\n      local_4c = 0;\r\n      local_res10[0] = 0;\r\n      FUN_1407ce4f0(&local_70,puVar1,(int)param_1[0x50],0,iVar5);\r\n      FUN_1407ce0f0(param_1,&local_70,0x101,1);\r\n      FUN_1407d2b80(param_1,puVar1);\r\n      if (0 < *param_3) {\r\n        FUN_1407cc2d0(param_1,local_res10);\r\n        local_74 = local_74 & 0xf8;\r\n        local_a0 = 0xffffffffffffffff;\r\n        local_98 = 0xffffffff;\r\n        local_94 = 0;\r\n        local_90 = 0xffffffffffffffff;\r\n        local_88 = 0xffffffff;\r\n        local_84 = 0;\r\n        local_80 = 0xbf800000;\r\n        local_7c = 0;\r\n        FUN_1407ce4f0(&local_a0,puVar1,(int)param_1[0x50],local_res10[0],iVar5);\r\n        FUN_1407d3130(param_1,&local_a0);\r\n      }\r\n      local_b0 = (undefined4)param_1[0x2b];\r\n      *(undefined4 *)((longlong)param_1 + 0x27c) = 3;\r\n      if (param_1[0x52] != 0) {\r\n        local_b8 = CONCAT31(local_b8._1_3_,0x6d);\r\n        FUN_140884920(param_1[0x52],&local_a0,&DAT_1439a7660,&local_b8);\r\n        FUN_140882fc0(&local_a0);\r\n      }\r\n    }\r\n    return;\r\n  }\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(\"%s: StartTime is superior to first scene length.\",*(undefined8 *)(*param_1 + 8));\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407cbc20",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nundefined8 FUN_1407cbc20(longlong param_1,undefined4 param_2,undefined4 *param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  bool bVar2;\r\n  undefined8 *puVar3;\r\n  byte bVar4;\r\n  byte bVar5;\r\n  byte bVar6;\r\n  longlong *plVar7;\r\n  longlong lVar8;\r\n  undefined8 uVar9;\r\n  longlong lVar10;\r\n  undefined8 *puVar11;\r\n  undefined8 *local_148;\r\n  uint local_140;\r\n  uint uStack_13c;\r\n  undefined8 local_138;\r\n  undefined8 local_130;\r\n  undefined1 *local_128;\r\n  uint local_120;\r\n  uint local_11c;\r\n  undefined1 local_118 [224];\r\n  \r\n  local_130 = 0xfffffffffffffffe;\r\n  local_120 = 0x4f000000;\r\n  local_128 = local_118;\r\n  local_11c = 0x4ff;\r\n  FUN_1411ad700(local_118,0x38,4,&LAB_1406525e0,&LAB_14019b640);\r\n  bVar4 = FUN_1407cbed0(param_1,(longlong *)(param_1 + 0xe0),param_2,param_3,&local_128);\r\n  bVar5 = FUN_1407cbed0(param_1,param_1 + 0x130,param_2,param_3,&local_128);\r\n  bVar5 = bVar4 & 1 & bVar5;\r\n  bVar4 = 0;\r\n  lVar10 = (longlong)(int)(local_120 & 0xffffff);\r\n  if ((local_120 & 0xffffff) == 0) {\r\nLAB_1407cbd59:\r\n    if (bVar5 != 0) {\r\n      FUN_1407d6ab0(D2_GameSingletonPtr + 0x1f5338,param_1,&local_128);\r\n      bVar2 = true;\r\n      puVar11 = *(undefined8 **)(param_1 + 0xe0);\r\n      puVar1 = puVar11 + (*(uint *)(param_1 + 0xe8) & 0xffffff);\r\n      for (; puVar11 != puVar1; puVar11 = puVar11 + 1) {\r\n        local_140 = 0x5000000;\r\n        local_148 = &local_138;\r\n        uStack_13c = 0x240;\r\n        local_138 = 0;\r\n        bVar4 = FUN_1407db850(*puVar11,param_2,&local_148,param_3);\r\n        puVar3 = local_148;\r\n        bVar5 = bVar5 & bVar4;\r\n        bVar2 = (bool)(bVar2 & (local_140 & 0xffffff) == 0);\r\n        if (((uStack_13c & 0x40) == 0) && (local_148 != (undefined8 *)0x0)) {\r\n          plVar7 = (longlong *)FUN_1401548e0();\r\n          (**(code **)(*plVar7 + 0x10))\r\n                    (plVar7,\r\n                     \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                     ,0x1ec,puVar3,0x10);\r\n        }\r\n      }\r\n      if ((bVar5 != 0) && (bVar2)) {\r\n        uVar9 = 1;\r\n        goto LAB_1407cbe61;\r\n      }\r\n    }\r\n  }\r\n  else {\r\n    lVar8 = 0;\r\n    do {\r\n      bVar6 = FUN_1407d6330(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(local_128 + lVar8));\r\n      bVar4 = bVar4 | bVar6;\r\n      lVar8 = lVar8 + 0x38;\r\n      lVar10 = lVar10 + -1;\r\n    } while (lVar10 != 0);\r\n    if (bVar4 == 0) goto LAB_1407cbd59;\r\n    *param_3 = 7;\r\n  }\r\n  uVar9 = 0;\r\nLAB_1407cbe61:\r\n  FUN_1411ad55c(local_118,0x38,4,&LAB_14019b640);\r\n  if (((local_11c & 0x40) == 0) && (local_128 != (undefined1 *)0x0)) {\r\n    FUN_1409c6a10(local_128,local_11c >> 8);\r\n  }\r\n  return uVar9;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nundefined8 FUN_1407cbc20(longlong param_1,undefined4 param_2,undefined4 *param_3)\r\n\r\n{\r\n  undefined8 *puVar1;\r\n  bool bVar2;\r\n  undefined8 *puVar3;\r\n  byte bVar4;\r\n  byte bVar5;\r\n  byte bVar6;\r\n  longlong *plVar7;\r\n  longlong lVar8;\r\n  undefined8 uVar9;\r\n  longlong lVar10;\r\n  undefined8 *puVar11;\r\n  undefined8 *local_148;\r\n  uint local_140;\r\n  uint uStack_13c;\r\n  undefined8 local_138;\r\n  undefined8 local_130;\r\n  undefined1 *local_128;\r\n  uint local_120;\r\n  uint local_11c;\r\n  undefined1 local_118 [224];\r\n  \r\n  local_130 = 0xfffffffffffffffe;\r\n  local_120 = 0x4f000000;\r\n  local_128 = local_118;\r\n  local_11c = 0x4ff;\r\n  FUN_1411ad700(local_118,0x38,4,&LAB_1406525e0,&LAB_14019b640);\r\n  bVar4 = FUN_1407cbed0(param_1,(longlong *)(param_1 + 0xe0),param_2,param_3,&local_128);\r\n  bVar5 = FUN_1407cbed0(param_1,param_1 + 0x130,param_2,param_3,&local_128);\r\n  bVar5 = bVar4 & 1 & bVar5;\r\n  bVar4 = 0;\r\n  lVar10 = (longlong)(int)(local_120 & 0xffffff);\r\n  if ((local_120 & 0xffffff) == 0) {\r\nLAB_1407cbd59:\r\n    if (bVar5 != 0) {\r\n      FUN_1407d6ab0(D2_GameSingletonPtr + 0x1f5338,param_1,&local_128);\r\n      bVar2 = true;\r\n      puVar11 = *(undefined8 **)(param_1 + 0xe0);\r\n      puVar1 = puVar11 + (*(uint *)(param_1 + 0xe8) & 0xffffff);\r\n      for (; puVar11 != puVar1; puVar11 = puVar11 + 1) {\r\n        local_140 = 0x5000000;\r\n        local_148 = &local_138;\r\n        uStack_13c = 0x240;\r\n        local_138 = 0;\r\n        bVar4 = FUN_1407db850(*puVar11,param_2,&local_148,param_3);\r\n        puVar3 = local_148;\r\n        bVar5 = bVar5 & bVar4;\r\n        bVar2 = (bool)(bVar2 & (local_140 & 0xffffff) == 0);\r\n        if (((uStack_13c & 0x40) == 0) && (local_148 != (undefined8 *)0x0)) {\r\n          plVar7 = (longlong *)FUN_1401548e0();\r\n          (**(code **)(*plVar7 + 0x10))\r\n                    (plVar7,\r\n                     \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                     ,0x1ec,puVar3,0x10);\r\n        }\r\n      }\r\n      if ((bVar5 != 0) && (bVar2)) {\r\n        uVar9 = 1;\r\n        goto LAB_1407cbe61;\r\n      }\r\n    }\r\n  }\r\n  else {\r\n    lVar8 = 0;\r\n    do {\r\n      bVar6 = FUN_1407d6330(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(local_128 + lVar8));\r\n      bVar4 = bVar4 | bVar6;\r\n      lVar8 = lVar8 + 0x38;\r\n      lVar10 = lVar10 + -1;\r\n    } while (lVar10 != 0);\r\n    if (bVar4 == 0) goto LAB_1407cbd59;\r\n    *param_3 = 7;\r\n  }\r\n  uVar9 = 0;\r\nLAB_1407cbe61:\r\n  FUN_1411ad55c(local_118,0x38,4,&LAB_14019b640);\r\n  if (((local_11c & 0x40) == 0) && (local_128 != (undefined1 *)0x0)) {\r\n    FUN_1409c6a10(local_128,local_11c >> 8);\r\n  }\r\n  return uVar9;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407cbed0",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nbyte FUN_1407cbed0(undefined8 param_1,longlong *param_2,undefined4 param_3,undefined8 param_4,\r\n                  undefined8 param_5)\r\n\r\n{\r\n  longlong *plVar1;\r\n  undefined8 *puVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  byte bVar5;\r\n  undefined8 uVar6;\r\n  longlong *plVar7;\r\n  byte bVar8;\r\n  undefined8 *puVar9;\r\n  longlong *plVar10;\r\n  undefined4 local_res10 [2];\r\n  undefined4 local_res18;\r\n  undefined8 *local_50;\r\n  uint local_48;\r\n  uint uStack_44;\r\n  undefined8 local_40;\r\n  \r\n  bVar8 = 1;\r\n  plVar10 = (longlong *)*param_2;\r\n  plVar1 = plVar10 + (*(uint *)(param_2 + 1) & 0xffffff);\r\n  local_res18 = param_3;\r\n  for (; plVar10 != plVar1; plVar10 = plVar10 + 1) {\r\n    lVar4 = *plVar10;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    uStack_44 = 0x240;\r\n    local_40 = 0;\r\n    bVar5 = FUN_1407db850(lVar4,local_res18,&local_50,param_4);\r\n    bVar8 = bVar8 & bVar5;\r\n    if (((*(byte *)(lVar4 + 0x1ac) & 8) == 0) && (*(longlong *)(lVar4 + 0xd0) != 0)) {\r\n      FUN_1400998b0(lVar4);\r\n      uVar3 = *(undefined4 *)(*(longlong *)(lVar4 + 0xd0) + 0xa8);\r\n      puVar2 = (undefined8 *)((longlong)local_50 + (ulonglong)(local_48 & 0xffffff) * 4);\r\n      for (puVar9 = local_50; puVar9 != puVar2; puVar9 = (undefined8 *)((longlong)puVar9 + 4)) {\r\n        local_res10[0] = *(undefined4 *)puVar9;\r\n        uVar6 = FUN_1406dc560(uVar3);\r\n        FUN_1406526b0(param_5,local_res10,uVar6);\r\n      }\r\n    }\r\n    puVar2 = local_50;\r\n    if (((uStack_44 & 0x40) == 0) && (local_50 != (undefined8 *)0x0)) {\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar7 + 0x10))\r\n                (plVar7,\r\n                 \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                 ,0x1ec,puVar2,0x10);\r\n    }\r\n  }\r\n  return bVar8;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nbyte FUN_1407cbed0(undefined8 param_1,longlong *param_2,undefined4 param_3,undefined8 param_4,\r\n                  undefined8 param_5)\r\n\r\n{\r\n  longlong *plVar1;\r\n  undefined8 *puVar2;\r\n  undefined4 uVar3;\r\n  longlong lVar4;\r\n  byte bVar5;\r\n  undefined8 uVar6;\r\n  longlong *plVar7;\r\n  byte bVar8;\r\n  undefined8 *puVar9;\r\n  longlong *plVar10;\r\n  undefined4 local_res10 [2];\r\n  undefined4 local_res18;\r\n  undefined8 *local_50;\r\n  uint local_48;\r\n  uint uStack_44;\r\n  undefined8 local_40;\r\n  \r\n  bVar8 = 1;\r\n  plVar10 = (longlong *)*param_2;\r\n  plVar1 = plVar10 + (*(uint *)(param_2 + 1) & 0xffffff);\r\n  local_res18 = param_3;\r\n  for (; plVar10 != plVar1; plVar10 = plVar10 + 1) {\r\n    lVar4 = *plVar10;\r\n    local_48 = 0x5000000;\r\n    local_50 = &local_40;\r\n    uStack_44 = 0x240;\r\n    local_40 = 0;\r\n    bVar5 = FUN_1407db850(lVar4,local_res18,&local_50,param_4);\r\n    bVar8 = bVar8 & bVar5;\r\n    if (((*(byte *)(lVar4 + 0x1ac) & 8) == 0) && (*(longlong *)(lVar4 + 0xd0) != 0)) {\r\n      FUN_1400998b0(lVar4);\r\n      uVar3 = *(undefined4 *)(*(longlong *)(lVar4 + 0xd0) + 0xa8);\r\n      puVar2 = (undefined8 *)((longlong)local_50 + (ulonglong)(local_48 & 0xffffff) * 4);\r\n      for (puVar9 = local_50; puVar9 != puVar2; puVar9 = (undefined8 *)((longlong)puVar9 + 4)) {\r\n        local_res10[0] = *(undefined4 *)puVar9;\r\n        uVar6 = FUN_1406dc560(uVar3);\r\n        FUN_1406526b0(param_5,local_res10,uVar6);\r\n      }\r\n    }\r\n    puVar2 = local_50;\r\n    if (((uStack_44 & 0x40) == 0) && (local_50 != (undefined8 *)0x0)) {\r\n      plVar7 = (longlong *)FUN_1401548e0();\r\n      (**(code **)(*plVar7 + 0x10))\r\n                (plVar7,\r\n                 \"c:\\\\users\\\\zmiarchiving\\\\desktop\\\\dh2\\\\dishonored2\\\\voidengine\\\\core\\\\idlib\\\\containers/List.h\"\r\n                 ,0x1ec,puVar2,0x10);\r\n    }\r\n  }\r\n  return bVar8;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  },
  {
    "address": "1407db850",
    "response": {
      "content": [
        {
          "type": "text",
          "text": "\r\nuint FUN_1407db850(longlong param_1,int param_2,undefined8 param_3,undefined4 *param_4)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  uint uVar2;\r\n  uint uVar3;\r\n  \r\n  uVar2 = FUN_140197120(**(undefined8 **)(param_1 + 200),\r\n                        (*(uint *)(param_1 + 0xa8) | param_2 << 8) << 0x12);\r\n  uVar2 = ~*(uint *)(param_1 + 0x1a8) & *(uint *)(param_1 + 0x1a0) & uVar2;\r\n  if ((uVar2 != 0) && (*(uint *)(param_1 + 0x1a4) != uVar2)) {\r\n    if (((*(byte *)(param_1 + 0x1ac) & 8) != 0) || ((*(byte *)(param_1 + 0x1ac) & 2) != 0)) {\r\n      uVar2 = *(uint *)(*(longlong *)(param_1 + 0xb8) + 0x268) >> 3;\r\n      uVar3 = uVar2 & 0xffffff01;\r\n      if ((uVar2 & 1) != 0) {\r\n        return uVar3;\r\n      }\r\n      *param_4 = 4;\r\n      return uVar3;\r\n    }\r\n    if (*(longlong *)(param_1 + 0xe8) != 0) {\r\n      uVar1 = **(undefined8 **)(param_1 + 200);\r\n      uVar2 = FUN_1407662f0(*(longlong *)(param_1 + 0xe8),*(longlong *)(param_1 + 0xb8),\r\n                            CONCAT71((int7)((ulonglong)uVar1 >> 8),1),\r\n                            *(undefined4 *)(*(undefined8 **)(param_1 + 200) + 0x2b),uVar1,uVar2,\r\n                            param_3,param_4);\r\n      return uVar2;\r\n    }\r\n  }\r\n  return 1;\r\n}\r\n\r\n"
        }
      ],
      "structuredContent": {
        "result": "\r\nuint FUN_1407db850(longlong param_1,int param_2,undefined8 param_3,undefined4 *param_4)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  uint uVar2;\r\n  uint uVar3;\r\n  \r\n  uVar2 = FUN_140197120(**(undefined8 **)(param_1 + 200),\r\n                        (*(uint *)(param_1 + 0xa8) | param_2 << 8) << 0x12);\r\n  uVar2 = ~*(uint *)(param_1 + 0x1a8) & *(uint *)(param_1 + 0x1a0) & uVar2;\r\n  if ((uVar2 != 0) && (*(uint *)(param_1 + 0x1a4) != uVar2)) {\r\n    if (((*(byte *)(param_1 + 0x1ac) & 8) != 0) || ((*(byte *)(param_1 + 0x1ac) & 2) != 0)) {\r\n      uVar2 = *(uint *)(*(longlong *)(param_1 + 0xb8) + 0x268) >> 3;\r\n      uVar3 = uVar2 & 0xffffff01;\r\n      if ((uVar2 & 1) != 0) {\r\n        return uVar3;\r\n      }\r\n      *param_4 = 4;\r\n      return uVar3;\r\n    }\r\n    if (*(longlong *)(param_1 + 0xe8) != 0) {\r\n      uVar1 = **(undefined8 **)(param_1 + 200);\r\n      uVar2 = FUN_1407662f0(*(longlong *)(param_1 + 0xe8),*(longlong *)(param_1 + 0xb8),\r\n                            CONCAT71((int7)((ulonglong)uVar1 >> 8),1),\r\n                            *(undefined4 *)(*(undefined8 **)(param_1 + 200) + 0x2b),uVar1,uVar2,\r\n                            param_3,param_4);\r\n      return uVar2;\r\n    }\r\n  }\r\n  return 1;\r\n}\r\n\r\n"
      },
      "isError": false
    }
  }
]

````

## Unescaped text fields for reading

### text

````text

/* WARNING: Removing unreachable block (ram,0x00014072b030) */
/* WARNING: Removing unreachable block (ram,0x00014072b043) */
/* WARNING: Removing unreachable block (ram,0x00014072b036) */
/* WARNING: Removing unreachable block (ram,0x00014072b041) */

undefined8 FUN_14072af30(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  uint uVar1;
  undefined4 local_res8 [2];
  longlong local_60;
  uint local_58;
  uint local_54;
  undefined8 local_50;
  undefined4 local_48;
  undefined8 *local_40;
  undefined4 local_38;
  undefined4 local_34;
  undefined8 local_30;
  undefined1 local_28;
  
  local_60 = 0;
  FUN_1401548e0();
  local_58 = 0x5000000;
  local_54 = 0x3f;
  uVar1 = 0x5000000;
  if (local_60 != 0) {
    FUN_1409d8700(local_60,0);
    uVar1 = local_58;
  }
  local_60 = 0;
  local_54 = local_54 & 0xff;
  local_58 = uVar1 & 0xff000000;
  local_50 = DAT_14398dae0;
  local_48 = 0;
  local_38 = 0x5000000;
  local_40 = &local_30;
  local_34 = 0x2ff;
  local_30 = 0x3fff00103fff0010;
  local_28 = 0;
  local_res8[0] = *(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xa8);
  FUN_14052e460(&local_40,local_res8);
  FUN_1407c6840(&local_60,&local_50);
  local_res8[0] = 0;
  FUN_1407d91c0(D2_GameSingletonPtr + 0x1f5338,param_2,param_3,&local_60,0,local_res8,0);
  FUN_14061a0a0(&local_40);
  if (((local_54 & 0x40) == 0) && (local_60 != 0)) {
    FUN_1409d8700(local_60,local_54 >> 8);
  }
  return param_2;
}


````

### text

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

### text

````text

int * FUN_1407d91c0(undefined8 param_1,int *param_2,undefined8 param_3,undefined8 param_4,
                   undefined8 param_5,undefined8 param_6,undefined8 param_7)

{
  int iVar1;
  char cVar2;
  int local_18 [4];
  
  local_18[0] = 0;
  FUN_1407d6040(param_1,local_18,param_3,param_4,&DAT_14398f888,param_5,param_7);
  iVar1 = local_18[0];
  if (local_18[0] != 0) {
    local_18[0] = 0;
    cVar2 = FUN_1407d9270(param_1,iVar1,local_18,param_6,param_7,0x100002);
    if (cVar2 != '\0') {
      *param_2 = iVar1;
      return param_2;
    }
  }
  *param_2 = 0;
  return param_2;
}


````

### text

````text

undefined1 FUN_1407cbbe0(longlong param_1)

{
  char cVar1;
  
  if (*(int *)(param_1 + 0x27c) == 2) {
    cVar1 = FUN_1407cf8e0();
    if (cVar1 == '\0') {
      cVar1 = FUN_1407cf870(param_1);
      if (cVar1 == '\0') {
        return 1;
      }
    }
  }
  return 0;
}


````

### text

````text

void FUN_1407d2160(longlong *param_1,int *param_2,int *param_3,int *param_4)

{
  undefined4 *puVar1;
  char cVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int local_res8 [2];
  uint local_res10 [2];
  undefined4 local_b8;
  int local_b4;
  undefined4 local_b0;
  undefined8 local_a0;
  undefined4 local_98;
  undefined4 local_94;
  undefined8 local_90;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_80;
  undefined8 local_7c;
  byte local_74;
  undefined8 local_70;
  undefined4 local_68;
  undefined4 local_64;
  undefined8 local_60;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined8 local_4c;
  byte local_44;
  
  uVar4 = *(uint *)(param_1 + 0x4f) >> 0x1a;
  iVar5 = 0;
  local_res10[1] = 0;
  local_res10[0] = uVar4;
  FUN_1407cae40(param_1 + 0x45,local_res10);
  iVar3 = FUN_140196fe0(*param_1,uVar4 << 0x1a);
  if (*param_2 <= iVar3) {
    if ((int)param_1[0x50] == 0) {
      local_b4 = *param_2;
      iVar5 = iVar3 + -1;
    }
    else {
      local_b4 = (iVar3 - *param_2) + -1;
    }
    puVar1 = (undefined4 *)((longlong)param_1 + 0x15c);
    local_b8 = 0;
    local_b0 = 0;
    *puVar1 = 0;
    *(int *)(param_1 + 0x2c) = local_b4;
    local_res8[0] = 0;
    *(undefined4 *)((longlong)param_1 + 0x164) = 0;
    *(undefined4 *)(param_1 + 0x2d) = 0;
    cVar2 = FUN_1407cbc20(param_1,uVar4,local_res8);
    if (cVar2 == '\0') {
      if ((*(char *)((longlong)param_1 + 0x2b6) == '\0') || ((local_res8[0] - 2U & 0xfffffffb) != 0)
         ) {
        if (param_4 != (int *)0x0) {
          *param_4 = local_res8[0];
        }
        *(undefined4 *)((longlong)param_1 + 0x27c) = 6;
      }
    }
    else {
      FUN_1407d2650(param_1);
      FUN_1407cd480(param_1,4);
      local_44 = local_44 & 0xf8;
      *(undefined1 *)((longlong)param_1 + 0x2b1) = 0;
      local_70 = 0xffffffffffffffff;
      local_68 = 0xffffffff;
      local_64 = 0;
      local_60 = 0xffffffffffffffff;
      local_58 = 0xffffffff;
      local_54 = 0;
      local_50 = 0xbf800000;
      local_4c = 0;
      local_res10[0] = 0;
      FUN_1407ce4f0(&local_70,puVar1,(int)param_1[0x50],0,iVar5);
      FUN_1407ce0f0(param_1,&local_70,0x101,1);
      FUN_1407d2b80(param_1,puVar1);
      if (0 < *param_3) {
        FUN_1407cc2d0(param_1,local_res10);
        local_74 = local_74 & 0xf8;
        local_a0 = 0xffffffffffffffff;
        local_98 = 0xffffffff;
        local_94 = 0;
        local_90 = 0xffffffffffffffff;
        local_88 = 0xffffffff;
        local_84 = 0;
        local_80 = 0xbf800000;
        local_7c = 0;
        FUN_1407ce4f0(&local_a0,puVar1,(int)param_1[0x50],local_res10[0],iVar5);
        FUN_1407d3130(param_1,&local_a0);
      }
      local_b0 = (undefined4)param_1[0x2b];
      *(undefined4 *)((longlong)param_1 + 0x27c) = 3;
      if (param_1[0x52] != 0) {
        local_b8 = CONCAT31(local_b8._1_3_,0x6d);
        FUN_140884920(param_1[0x52],&local_a0,&DAT_1439a7660,&local_b8);
        FUN_140882fc0(&local_a0);
      }
    }
    return;
  }
                    /* WARNING: Subroutine does not return */
  FUN_140166830("%s: StartTime is superior to first scene length.",*(undefined8 *)(*param_1 + 8));
}


````

### text

````text

undefined8 FUN_1407cbc20(longlong param_1,undefined4 param_2,undefined4 *param_3)

{
  undefined8 *puVar1;
  bool bVar2;
  undefined8 *puVar3;
  byte bVar4;
  byte bVar5;
  byte bVar6;
  longlong *plVar7;
  longlong lVar8;
  undefined8 uVar9;
  longlong lVar10;
  undefined8 *puVar11;
  undefined8 *local_148;
  uint local_140;
  uint uStack_13c;
  undefined8 local_138;
  undefined8 local_130;
  undefined1 *local_128;
  uint local_120;
  uint local_11c;
  undefined1 local_118 [224];
  
  local_130 = 0xfffffffffffffffe;
  local_120 = 0x4f000000;
  local_128 = local_118;
  local_11c = 0x4ff;
  FUN_1411ad700(local_118,0x38,4,&LAB_1406525e0,&LAB_14019b640);
  bVar4 = FUN_1407cbed0(param_1,(longlong *)(param_1 + 0xe0),param_2,param_3,&local_128);
  bVar5 = FUN_1407cbed0(param_1,param_1 + 0x130,param_2,param_3,&local_128);
  bVar5 = bVar4 & 1 & bVar5;
  bVar4 = 0;
  lVar10 = (longlong)(int)(local_120 & 0xffffff);
  if ((local_120 & 0xffffff) == 0) {
LAB_1407cbd59:
    if (bVar5 != 0) {
      FUN_1407d6ab0(D2_GameSingletonPtr + 0x1f5338,param_1,&local_128);
      bVar2 = true;
      puVar11 = *(undefined8 **)(param_1 + 0xe0);
      puVar1 = puVar11 + (*(uint *)(param_1 + 0xe8) & 0xffffff);
      for (; puVar11 != puVar1; puVar11 = puVar11 + 1) {
        local_140 = 0x5000000;
        local_148 = &local_138;
        uStack_13c = 0x240;
        local_138 = 0;
        bVar4 = FUN_1407db850(*puVar11,param_2,&local_148,param_3);
        puVar3 = local_148;
        bVar5 = bVar5 & bVar4;
        bVar2 = (bool)(bVar2 & (local_140 & 0xffffff) == 0);
        if (((uStack_13c & 0x40) == 0) && (local_148 != (undefined8 *)0x0)) {
          plVar7 = (longlong *)FUN_1401548e0();
          (**(code **)(*plVar7 + 0x10))
                    (plVar7,
                     "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                     ,0x1ec,puVar3,0x10);
        }
      }
      if ((bVar5 != 0) && (bVar2)) {
        uVar9 = 1;
        goto LAB_1407cbe61;
      }
    }
  }
  else {
    lVar8 = 0;
    do {
      bVar6 = FUN_1407d6330(D2_GameSingletonPtr + 0x1f5338,*(undefined4 *)(local_128 + lVar8));
      bVar4 = bVar4 | bVar6;
      lVar8 = lVar8 + 0x38;
      lVar10 = lVar10 + -1;
    } while (lVar10 != 0);
    if (bVar4 == 0) goto LAB_1407cbd59;
    *param_3 = 7;
  }
  uVar9 = 0;
LAB_1407cbe61:
  FUN_1411ad55c(local_118,0x38,4,&LAB_14019b640);
  if (((local_11c & 0x40) == 0) && (local_128 != (undefined1 *)0x0)) {
    FUN_1409c6a10(local_128,local_11c >> 8);
  }
  return uVar9;
}


````

### text

````text

byte FUN_1407cbed0(undefined8 param_1,longlong *param_2,undefined4 param_3,undefined8 param_4,
                  undefined8 param_5)

{
  longlong *plVar1;
  undefined8 *puVar2;
  undefined4 uVar3;
  longlong lVar4;
  byte bVar5;
  undefined8 uVar6;
  longlong *plVar7;
  byte bVar8;
  undefined8 *puVar9;
  longlong *plVar10;
  undefined4 local_res10 [2];
  undefined4 local_res18;
  undefined8 *local_50;
  uint local_48;
  uint uStack_44;
  undefined8 local_40;
  
  bVar8 = 1;
  plVar10 = (longlong *)*param_2;
  plVar1 = plVar10 + (*(uint *)(param_2 + 1) & 0xffffff);
  local_res18 = param_3;
  for (; plVar10 != plVar1; plVar10 = plVar10 + 1) {
    lVar4 = *plVar10;
    local_48 = 0x5000000;
    local_50 = &local_40;
    uStack_44 = 0x240;
    local_40 = 0;
    bVar5 = FUN_1407db850(lVar4,local_res18,&local_50,param_4);
    bVar8 = bVar8 & bVar5;
    if (((*(byte *)(lVar4 + 0x1ac) & 8) == 0) && (*(longlong *)(lVar4 + 0xd0) != 0)) {
      FUN_1400998b0(lVar4);
      uVar3 = *(undefined4 *)(*(longlong *)(lVar4 + 0xd0) + 0xa8);
      puVar2 = (undefined8 *)((longlong)local_50 + (ulonglong)(local_48 & 0xffffff) * 4);
      for (puVar9 = local_50; puVar9 != puVar2; puVar9 = (undefined8 *)((longlong)puVar9 + 4)) {
        local_res10[0] = *(undefined4 *)puVar9;
        uVar6 = FUN_1406dc560(uVar3);
        FUN_1406526b0(param_5,local_res10,uVar6);
      }
    }
    puVar2 = local_50;
    if (((uStack_44 & 0x40) == 0) && (local_50 != (undefined8 *)0x0)) {
      plVar7 = (longlong *)FUN_1401548e0();
      (**(code **)(*plVar7 + 0x10))
                (plVar7,
                 "c:\\users\\zmiarchiving\\desktop\\dh2\\dishonored2\\voidengine\\core\\idlib\\containers/List.h"
                 ,0x1ec,puVar2,0x10);
    }
  }
  return bVar8;
}


````

### text

````text

uint FUN_1407db850(longlong param_1,int param_2,undefined8 param_3,undefined4 *param_4)

{
  undefined8 uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar2 = FUN_140197120(**(undefined8 **)(param_1 + 200),
                        (*(uint *)(param_1 + 0xa8) | param_2 << 8) << 0x12);
  uVar2 = ~*(uint *)(param_1 + 0x1a8) & *(uint *)(param_1 + 0x1a0) & uVar2;
  if ((uVar2 != 0) && (*(uint *)(param_1 + 0x1a4) != uVar2)) {
    if (((*(byte *)(param_1 + 0x1ac) & 8) != 0) || ((*(byte *)(param_1 + 0x1ac) & 2) != 0)) {
      uVar2 = *(uint *)(*(longlong *)(param_1 + 0xb8) + 0x268) >> 3;
      uVar3 = uVar2 & 0xffffff01;
      if ((uVar2 & 1) != 0) {
        return uVar3;
      }
      *param_4 = 4;
      return uVar3;
    }
    if (*(longlong *)(param_1 + 0xe8) != 0) {
      uVar1 = **(undefined8 **)(param_1 + 200);
      uVar2 = FUN_1407662f0(*(longlong *)(param_1 + 0xe8),*(longlong *)(param_1 + 0xb8),
                            CONCAT71((int7)((ulonglong)uVar1 >> 8),1),
                            *(undefined4 *)(*(undefined8 **)(param_1 + 200) + 0x2b),uVar1,uVar2,
                            param_3,param_4);
      return uVar2;
    }
  }
  return 1;
}


````
