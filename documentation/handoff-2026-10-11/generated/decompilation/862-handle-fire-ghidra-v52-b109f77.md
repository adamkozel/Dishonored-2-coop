# handle-fire-ghidra-v52.json

Original: [work/movement-integration/grenade-research/handle-fire-ghidra-v52.json](<../../../../work/movement-integration/grenade-research/handle-fire-ghidra-v52.json>)

SHA-256: `B836A14C61247F4D611FACD1EED1EB2DB091C093D51E7FE8E7728ECBD319F23E`; 19872 bytes.

## Original saved record

````json
{
  "exactExecutableSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "strings": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "{\"matches\":[{\"address\":\"141b54b40\",\"value\":\"arkCpntWeaponThrowableNPC::PrepareProjectile - %s: No settings for current context.\",\"encoding\":\"ascii\"},{\"address\":\"141b54ba0\",\"value\":\"arkCpntWeaponThrowableNPC::PrepareProjectile - %s: No prepare bind tag specified.\",\"encoding\":\"ascii\"},{\"address\":\"141b54c00\",\"value\":\"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: No settings for current context.\",\"encoding\":\"ascii\"},{\"address\":\"141b54c60\",\"value\":\"arkCpntWeaponThrowableNPC::StartProjectile : couldn't find a working homing tag for target %s\",\"encoding\":\"ascii\"},{\"address\":\"141b54cc0\",\"value\":\"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: No origin bind tag specified.\",\"encoding\":\"ascii\"},{\"address\":\"141b54d10\",\"value\":\"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: failed to recover the bind tag %s.\",\"encoding\":\"ascii\"},{\"address\":\"141b9da40\",\"value\":\"arkCpntWeaponThrowable::HandleFire - a arkCpntAudio is needed here!\",\"encoding\":\"ascii\"},{\"address\":\"141cc64a0\",\"value\":\"arkCpntWeaponThrowable\",\"encoding\":\"ascii\"},{\"address\":\"141ccde60\",\"value\":\"arkCpntWeaponThrowableNPC\",\"encoding\":\"ascii\"},{\"address\":\"141d61960\",\"value\":\"arkValidCpntPtr < const arkCpntWeaponThrowable , CF_STARTED >\",\"encoding\":\"ascii\"},{\"address\":\"141d7b0b8\",\"value\":\"arkCheckedCpntPtr < arkCpntWeaponThrowable , CF_STARTED >\",\"encoding\":\"ascii\"}],\"total\":11,\"offset\":0,\"limit\":25}"
        }
      ],
      "structuredContent": {
        "result": "{\"matches\":[{\"address\":\"141b54b40\",\"value\":\"arkCpntWeaponThrowableNPC::PrepareProjectile - %s: No settings for current context.\",\"encoding\":\"ascii\"},{\"address\":\"141b54ba0\",\"value\":\"arkCpntWeaponThrowableNPC::PrepareProjectile - %s: No prepare bind tag specified.\",\"encoding\":\"ascii\"},{\"address\":\"141b54c00\",\"value\":\"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: No settings for current context.\",\"encoding\":\"ascii\"},{\"address\":\"141b54c60\",\"value\":\"arkCpntWeaponThrowableNPC::StartProjectile : couldn't find a working homing tag for target %s\",\"encoding\":\"ascii\"},{\"address\":\"141b54cc0\",\"value\":\"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: No origin bind tag specified.\",\"encoding\":\"ascii\"},{\"address\":\"141b54d10\",\"value\":\"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: failed to recover the bind tag %s.\",\"encoding\":\"ascii\"},{\"address\":\"141b9da40\",\"value\":\"arkCpntWeaponThrowable::HandleFire - a arkCpntAudio is needed here!\",\"encoding\":\"ascii\"},{\"address\":\"141cc64a0\",\"value\":\"arkCpntWeaponThrowable\",\"encoding\":\"ascii\"},{\"address\":\"141ccde60\",\"value\":\"arkCpntWeaponThrowableNPC\",\"encoding\":\"ascii\"},{\"address\":\"141d61960\",\"value\":\"arkValidCpntPtr < const arkCpntWeaponThrowable , CF_STARTED >\",\"encoding\":\"ascii\"},{\"address\":\"141d7b0b8\",\"value\":\"arkCheckedCpntPtr < arkCpntWeaponThrowable , CF_STARTED >\",\"encoding\":\"ascii\"}],\"total\":11,\"offset\":0,\"limit\":25}"
      },
      "isError": false
    }
  },
  "functions": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "No functions matching 'Throwable'"
        }
      ],
      "structuredContent": {
        "result": "No functions matching 'Throwable'"
      },
      "isError": false
    }
  },
  "xrefsString": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "From 1406334ff in FUN_140632f70 [DATA]"
        }
      ],
      "structuredContent": {
        "result": "From 1406334ff in FUN_140632f70 [DATA]"
      },
      "isError": false
    }
  },
  "xrefsTypeName": {
    "status": "fulfilled",
    "value": {
      "content": [
        {
          "type": "text",
          "text": "From 14143ec5f in FUN_141413800 [DATA]"
        }
      ],
      "structuredContent": {
        "result": "From 14143ec5f in FUN_141413800 [DATA]"
      },
      "isError": false
    }
  },
  "decompilation": {
    "content": [
      {
        "type": "text",
        "text": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140632f70(longlong *param_1)\r\n\r\n{\r\n  float *pfVar1;\r\n  byte bVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  float *pfVar6;\r\n  float *pfVar7;\r\n  undefined8 *puVar8;\r\n  longlong lVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  float fVar12;\r\n  undefined1 local_res8 [8];\r\n  ulonglong in_stack_fffffffffffffe78;\r\n  undefined4 uVar13;\r\n  ulonglong in_stack_fffffffffffffe80;\r\n  uint in_stack_fffffffffffffe90;\r\n  float local_158;\r\n  float local_154;\r\n  float local_150;\r\n  float local_148;\r\n  float local_144;\r\n  float local_140;\r\n  float local_138;\r\n  float local_134;\r\n  float local_130;\r\n  float local_128;\r\n  float local_124;\r\n  float local_120;\r\n  undefined4 local_118;\r\n  undefined4 local_114;\r\n  undefined4 local_110;\r\n  undefined4 local_10c;\r\n  undefined4 local_108;\r\n  undefined4 local_104;\r\n  float local_100;\r\n  float local_fc;\r\n  float local_f8;\r\n  undefined4 local_e8;\r\n  undefined4 local_e4;\r\n  undefined4 local_e0;\r\n  undefined4 local_d8;\r\n  undefined4 local_d4;\r\n  undefined4 local_d0;\r\n  undefined8 local_b8;\r\n  undefined8 uStack_b0;\r\n  undefined8 local_a8;\r\n  undefined8 uStack_a0;\r\n  undefined4 local_98;\r\n  undefined4 uStack_94;\r\n  undefined4 uStack_90;\r\n  undefined4 uStack_8c;\r\n  undefined8 local_88;\r\n  undefined8 uStack_80;\r\n  \r\n  if ((param_1[0x2c] == 0) || ((*(byte *)(param_1[0x2c] + 0x38) & 8) == 0)) {\r\n    (**(code **)(*param_1 + 0x140))();\r\n    if (param_1[0x13] != 0) {\r\n      lVar9 = param_1[2];\r\n      lVar3 = FUN_14063b5f0(param_1);\r\n      if (lVar3 != 0) {\r\n        lVar5 = *(longlong *)(lVar3 + 0xf0);\r\n        lVar4 = FUN_140877720(lVar3,&DAT_1439aee80,0);\r\n        if ((lVar5 != 0) && (lVar4 != 0)) {\r\n          in_stack_fffffffffffffe78 = in_stack_fffffffffffffe78 & 0xffffffffffffff00;\r\n          lVar5 = FUN_14008c840(*(undefined8 *)(lVar5 + 0x1bb0),\r\n                                *(undefined4 *)(*(longlong *)(lVar4 + 8) + 0x8c),lVar9 + 0x330,\r\n                                0x3c23d70a,in_stack_fffffffffffffe78);\r\n          if (lVar5 != 0) {\r\n            local_b8 = _DAT_1423ccdd0;\r\n            uStack_b0 = uRam00000001423ccdd8;\r\n            local_a8 = _DAT_1423ccde0;\r\n            uStack_a0 = uRam00000001423ccde8;\r\n            local_98 = _DAT_1423ccdf0;\r\n            uStack_94 = uRam00000001423ccdf4;\r\n            uStack_90 = uRam00000001423ccdf8;\r\n            uStack_8c = uRam00000001423ccdfc;\r\n            local_88 = 0;\r\n            uStack_80 = 0;\r\n            (**(code **)(**(longlong **)(lVar5 + 0x30) + 0x20))\r\n                      (*(longlong **)(lVar5 + 0x30),&local_b8,&local_e8);\r\n            local_118 = local_e8;\r\n            local_114 = local_e4;\r\n            local_110 = local_e0;\r\n            local_10c = local_d8;\r\n            local_108 = local_d4;\r\n            local_104 = local_d0;\r\n            FUN_14063a520(param_1,&local_118);\r\n          }\r\n        }\r\n        lVar5 = param_1[9];\r\n        FUN_140639760(param_1,&local_148);\r\n        in_stack_fffffffffffffe78 = in_stack_fffffffffffffe78 & 0xffffffffffffff00;\r\n        FUN_140638ae0(param_1,&local_138,(float *)(lVar5 + 0x25288),&local_148,\r\n                      in_stack_fffffffffffffe78,in_stack_fffffffffffffe80 & 0xffffffffffffff00);\r\n        uVar13 = (undefined4)(in_stack_fffffffffffffe78 >> 0x20);\r\n        pfVar1 = (float *)(lVar9 + 0x30c);\r\n        lVar9 = param_1[1];\r\n        if (*(char *)((longlong)param_1 + 0x17c) == '\\0') {\r\n          fVar11 = *(float *)(lVar9 + 0x298);\r\n        }\r\n        else {\r\n          fVar11 = *(float *)(lVar9 + 0x2b8);\r\n        }\r\n        local_148 = local_148 - local_138;\r\n        local_144 = local_144 - local_134;\r\n        local_140 = local_140 - local_130;\r\n        fVar12 = local_144 * local_144 + local_148 * local_148 + local_140 * local_140;\r\n        if (fVar12 <= 1.1754944e-38) {\r\n          fVar12 = 1.1754944e-38;\r\n        }\r\n        local_120 = 1.0 / SQRT(fVar12);\r\n        local_128 = local_148 * local_120;\r\n        local_124 = local_144 * local_120;\r\n        local_120 = local_140 * local_120;\r\n        if (*(char *)((longlong)param_1 + 0x17c) == '\\0') {\r\n          fVar12 = *(float *)(lVar9 + 0x29c);\r\n        }\r\n        else {\r\n          fVar12 = *(float *)(lVar9 + 700);\r\n        }\r\n        fVar10 = (float)FUN_140017c70(fVar11 * 0.017453292);\r\n        fVar11 = (float)FUN_140018620(fVar11 * 0.017453292);\r\n        pfVar6 = (float *)FUN_140112000(&local_128,&local_e8);\r\n        local_154 = (fVar10 * pfVar6[1] + pfVar6[4] * 0.0 + fVar11 * pfVar6[7]) * fVar12;\r\n        local_150 = (fVar10 * pfVar6[2] + pfVar6[5] * 0.0 + fVar11 * pfVar6[8]) * fVar12;\r\n        local_158 = (fVar10 * *pfVar6 + pfVar6[3] * 0.0 + fVar11 * pfVar6[6]) * fVar12;\r\n        if ((int)param_1[0x2f] < 0) {\r\n          fVar11 = 0.0;\r\n        }\r\n        else {\r\n          fVar11 = *(float *)(*(longlong *)(param_1[6] + 0x48) + 0x24 +\r\n                             (longlong)(int)param_1[0x2f] * 0x28);\r\n        }\r\n        fVar12 = (float)FUN_1408ac040(0,0x42c80000);\r\n        if (fVar12 < fVar11) {\r\n          local_158 = local_158 * 0.1;\r\n          local_154 = local_154 * 0.1;\r\n          local_150 = local_150 * 0.1;\r\n        }\r\n        FUN_14087c2a0(lVar3,&local_158);\r\n        pfVar6 = pfVar1;\r\n        if (*(char *)(lVar9 + 0x2ac) != '\\0') {\r\n          pfVar6 = (float *)(lVar5 + 0x25288);\r\n        }\r\n        pfVar7 = (float *)FUN_14010d3d0(lVar9 + 0x2a0,&local_118);\r\n        fVar11 = pfVar7[1];\r\n        fVar12 = *pfVar7;\r\n        fVar10 = pfVar7[2];\r\n        local_f8 = fVar12 * pfVar6[2] + fVar11 * pfVar6[5] + fVar10 * pfVar6[8];\r\n        local_fc = fVar12 * pfVar6[1] + fVar11 * pfVar6[4] + fVar10 * pfVar6[7];\r\n        local_100 = fVar11 * pfVar6[3] + fVar12 * *pfVar6 + fVar10 * pfVar6[6];\r\n        FUN_14087c280(lVar3,&local_100);\r\n        puVar8 = (undefined8 *)FUN_140131ae0(local_res8);\r\n        in_stack_fffffffffffffe90 = 0xffffffff;\r\n        FUN_140639970(param_1,lVar3,&local_138,pfVar1,CONCAT44(uVar13,0x3f800000),0,*puVar8,\r\n                      0xffffffff,0);\r\n        if ((param_1[0x2c] != 0) && (lVar9 = FUN_140877720(lVar3,&DAT_1439e28e0,0), lVar9 != 0)) {\r\n          FUN_140490f70(lVar9,param_1[0x2c]);\r\n          bVar2 = *(byte *)(lVar9 + 0x38);\r\n          *(byte *)(lVar9 + 0x38) =\r\n               ((*(byte *)((longlong)param_1 + 0xb9) >> 2 | bVar2) ^ bVar2) & 4 ^ bVar2;\r\n        }\r\n      }\r\n    }\r\n    if (-1 < *(int *)((longlong)param_1 + 0x174)) {\r\n      if (param_1[0x2d] == 0) {\r\n                    /* WARNING: Subroutine does not return */\r\n        FUN_140166830(\"arkCpntWeaponThrowable::HandleFire - a arkCpntAudio is needed here!\");\r\n      }\r\n      FUN_140709110(param_1[0x2d],0xffffffff,\r\n                    *(undefined8 *)\r\n                     (*(longlong *)(param_1[1] + 600) + 0x10 +\r\n                     (longlong)*(int *)((longlong)param_1 + 0x174) * 0x18),0,0,0,1,\r\n                    in_stack_fffffffffffffe90 & 0xffffff00);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
      }
    ],
    "structuredContent": {
      "result": "\r\n/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */\r\n\r\nvoid FUN_140632f70(longlong *param_1)\r\n\r\n{\r\n  float *pfVar1;\r\n  byte bVar2;\r\n  longlong lVar3;\r\n  longlong lVar4;\r\n  longlong lVar5;\r\n  float *pfVar6;\r\n  float *pfVar7;\r\n  undefined8 *puVar8;\r\n  longlong lVar9;\r\n  float fVar10;\r\n  float fVar11;\r\n  float fVar12;\r\n  undefined1 local_res8 [8];\r\n  ulonglong in_stack_fffffffffffffe78;\r\n  undefined4 uVar13;\r\n  ulonglong in_stack_fffffffffffffe80;\r\n  uint in_stack_fffffffffffffe90;\r\n  float local_158;\r\n  float local_154;\r\n  float local_150;\r\n  float local_148;\r\n  float local_144;\r\n  float local_140;\r\n  float local_138;\r\n  float local_134;\r\n  float local_130;\r\n  float local_128;\r\n  float local_124;\r\n  float local_120;\r\n  undefined4 local_118;\r\n  undefined4 local_114;\r\n  undefined4 local_110;\r\n  undefined4 local_10c;\r\n  undefined4 local_108;\r\n  undefined4 local_104;\r\n  float local_100;\r\n  float local_fc;\r\n  float local_f8;\r\n  undefined4 local_e8;\r\n  undefined4 local_e4;\r\n  undefined4 local_e0;\r\n  undefined4 local_d8;\r\n  undefined4 local_d4;\r\n  undefined4 local_d0;\r\n  undefined8 local_b8;\r\n  undefined8 uStack_b0;\r\n  undefined8 local_a8;\r\n  undefined8 uStack_a0;\r\n  undefined4 local_98;\r\n  undefined4 uStack_94;\r\n  undefined4 uStack_90;\r\n  undefined4 uStack_8c;\r\n  undefined8 local_88;\r\n  undefined8 uStack_80;\r\n  \r\n  if ((param_1[0x2c] == 0) || ((*(byte *)(param_1[0x2c] + 0x38) & 8) == 0)) {\r\n    (**(code **)(*param_1 + 0x140))();\r\n    if (param_1[0x13] != 0) {\r\n      lVar9 = param_1[2];\r\n      lVar3 = FUN_14063b5f0(param_1);\r\n      if (lVar3 != 0) {\r\n        lVar5 = *(longlong *)(lVar3 + 0xf0);\r\n        lVar4 = FUN_140877720(lVar3,&DAT_1439aee80,0);\r\n        if ((lVar5 != 0) && (lVar4 != 0)) {\r\n          in_stack_fffffffffffffe78 = in_stack_fffffffffffffe78 & 0xffffffffffffff00;\r\n          lVar5 = FUN_14008c840(*(undefined8 *)(lVar5 + 0x1bb0),\r\n                                *(undefined4 *)(*(longlong *)(lVar4 + 8) + 0x8c),lVar9 + 0x330,\r\n                                0x3c23d70a,in_stack_fffffffffffffe78);\r\n          if (lVar5 != 0) {\r\n            local_b8 = _DAT_1423ccdd0;\r\n            uStack_b0 = uRam00000001423ccdd8;\r\n            local_a8 = _DAT_1423ccde0;\r\n            uStack_a0 = uRam00000001423ccde8;\r\n            local_98 = _DAT_1423ccdf0;\r\n            uStack_94 = uRam00000001423ccdf4;\r\n            uStack_90 = uRam00000001423ccdf8;\r\n            uStack_8c = uRam00000001423ccdfc;\r\n            local_88 = 0;\r\n            uStack_80 = 0;\r\n            (**(code **)(**(longlong **)(lVar5 + 0x30) + 0x20))\r\n                      (*(longlong **)(lVar5 + 0x30),&local_b8,&local_e8);\r\n            local_118 = local_e8;\r\n            local_114 = local_e4;\r\n            local_110 = local_e0;\r\n            local_10c = local_d8;\r\n            local_108 = local_d4;\r\n            local_104 = local_d0;\r\n            FUN_14063a520(param_1,&local_118);\r\n          }\r\n        }\r\n        lVar5 = param_1[9];\r\n        FUN_140639760(param_1,&local_148);\r\n        in_stack_fffffffffffffe78 = in_stack_fffffffffffffe78 & 0xffffffffffffff00;\r\n        FUN_140638ae0(param_1,&local_138,(float *)(lVar5 + 0x25288),&local_148,\r\n                      in_stack_fffffffffffffe78,in_stack_fffffffffffffe80 & 0xffffffffffffff00);\r\n        uVar13 = (undefined4)(in_stack_fffffffffffffe78 >> 0x20);\r\n        pfVar1 = (float *)(lVar9 + 0x30c);\r\n        lVar9 = param_1[1];\r\n        if (*(char *)((longlong)param_1 + 0x17c) == '\\0') {\r\n          fVar11 = *(float *)(lVar9 + 0x298);\r\n        }\r\n        else {\r\n          fVar11 = *(float *)(lVar9 + 0x2b8);\r\n        }\r\n        local_148 = local_148 - local_138;\r\n        local_144 = local_144 - local_134;\r\n        local_140 = local_140 - local_130;\r\n        fVar12 = local_144 * local_144 + local_148 * local_148 + local_140 * local_140;\r\n        if (fVar12 <= 1.1754944e-38) {\r\n          fVar12 = 1.1754944e-38;\r\n        }\r\n        local_120 = 1.0 / SQRT(fVar12);\r\n        local_128 = local_148 * local_120;\r\n        local_124 = local_144 * local_120;\r\n        local_120 = local_140 * local_120;\r\n        if (*(char *)((longlong)param_1 + 0x17c) == '\\0') {\r\n          fVar12 = *(float *)(lVar9 + 0x29c);\r\n        }\r\n        else {\r\n          fVar12 = *(float *)(lVar9 + 700);\r\n        }\r\n        fVar10 = (float)FUN_140017c70(fVar11 * 0.017453292);\r\n        fVar11 = (float)FUN_140018620(fVar11 * 0.017453292);\r\n        pfVar6 = (float *)FUN_140112000(&local_128,&local_e8);\r\n        local_154 = (fVar10 * pfVar6[1] + pfVar6[4] * 0.0 + fVar11 * pfVar6[7]) * fVar12;\r\n        local_150 = (fVar10 * pfVar6[2] + pfVar6[5] * 0.0 + fVar11 * pfVar6[8]) * fVar12;\r\n        local_158 = (fVar10 * *pfVar6 + pfVar6[3] * 0.0 + fVar11 * pfVar6[6]) * fVar12;\r\n        if ((int)param_1[0x2f] < 0) {\r\n          fVar11 = 0.0;\r\n        }\r\n        else {\r\n          fVar11 = *(float *)(*(longlong *)(param_1[6] + 0x48) + 0x24 +\r\n                             (longlong)(int)param_1[0x2f] * 0x28);\r\n        }\r\n        fVar12 = (float)FUN_1408ac040(0,0x42c80000);\r\n        if (fVar12 < fVar11) {\r\n          local_158 = local_158 * 0.1;\r\n          local_154 = local_154 * 0.1;\r\n          local_150 = local_150 * 0.1;\r\n        }\r\n        FUN_14087c2a0(lVar3,&local_158);\r\n        pfVar6 = pfVar1;\r\n        if (*(char *)(lVar9 + 0x2ac) != '\\0') {\r\n          pfVar6 = (float *)(lVar5 + 0x25288);\r\n        }\r\n        pfVar7 = (float *)FUN_14010d3d0(lVar9 + 0x2a0,&local_118);\r\n        fVar11 = pfVar7[1];\r\n        fVar12 = *pfVar7;\r\n        fVar10 = pfVar7[2];\r\n        local_f8 = fVar12 * pfVar6[2] + fVar11 * pfVar6[5] + fVar10 * pfVar6[8];\r\n        local_fc = fVar12 * pfVar6[1] + fVar11 * pfVar6[4] + fVar10 * pfVar6[7];\r\n        local_100 = fVar11 * pfVar6[3] + fVar12 * *pfVar6 + fVar10 * pfVar6[6];\r\n        FUN_14087c280(lVar3,&local_100);\r\n        puVar8 = (undefined8 *)FUN_140131ae0(local_res8);\r\n        in_stack_fffffffffffffe90 = 0xffffffff;\r\n        FUN_140639970(param_1,lVar3,&local_138,pfVar1,CONCAT44(uVar13,0x3f800000),0,*puVar8,\r\n                      0xffffffff,0);\r\n        if ((param_1[0x2c] != 0) && (lVar9 = FUN_140877720(lVar3,&DAT_1439e28e0,0), lVar9 != 0)) {\r\n          FUN_140490f70(lVar9,param_1[0x2c]);\r\n          bVar2 = *(byte *)(lVar9 + 0x38);\r\n          *(byte *)(lVar9 + 0x38) =\r\n               ((*(byte *)((longlong)param_1 + 0xb9) >> 2 | bVar2) ^ bVar2) & 4 ^ bVar2;\r\n        }\r\n      }\r\n    }\r\n    if (-1 < *(int *)((longlong)param_1 + 0x174)) {\r\n      if (param_1[0x2d] == 0) {\r\n                    /* WARNING: Subroutine does not return */\r\n        FUN_140166830(\"arkCpntWeaponThrowable::HandleFire - a arkCpntAudio is needed here!\");\r\n      }\r\n      FUN_140709110(param_1[0x2d],0xffffffff,\r\n                    *(undefined8 *)\r\n                     (*(longlong *)(param_1[1] + 600) + 0x10 +\r\n                     (longlong)*(int *)((longlong)param_1 + 0x174) * 0x18),0,0,0,1,\r\n                    in_stack_fffffffffffffe90 & 0xffffff00);\r\n      return;\r\n    }\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "isError": false
  },
  "xrefsFunction": {
    "content": [
      {
        "type": "text",
        "text": "From 143f1bff4 [DATA]\nFrom 14211cee8 [DATA]\nFrom 14211cfa8 [DATA]\nFrom 14211cfb8 [DATA]\nFrom 140633a60 [UNCONDITIONAL_CALL]"
      }
    ],
    "structuredContent": {
      "result": "From 143f1bff4 [DATA]\nFrom 14211cee8 [DATA]\nFrom 14211cfa8 [DATA]\nFrom 14211cfb8 [DATA]\nFrom 140633a60 [UNCONDITIONAL_CALL]"
    },
    "isError": false
  },
  "scope": "Read-only static investigation. Native HandleFire creates/launches gameplay projectile; must not be replayed for remote cosmetic animation. ABI and field typing provisional."
}

````

## Unescaped text fields for reading

### text

````text
{"matches":[{"address":"141b54b40","value":"arkCpntWeaponThrowableNPC::PrepareProjectile - %s: No settings for current context.","encoding":"ascii"},{"address":"141b54ba0","value":"arkCpntWeaponThrowableNPC::PrepareProjectile - %s: No prepare bind tag specified.","encoding":"ascii"},{"address":"141b54c00","value":"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: No settings for current context.","encoding":"ascii"},{"address":"141b54c60","value":"arkCpntWeaponThrowableNPC::StartProjectile : couldn't find a working homing tag for target %s","encoding":"ascii"},{"address":"141b54cc0","value":"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: No origin bind tag specified.","encoding":"ascii"},{"address":"141b54d10","value":"arkCpntWeaponThrowableNPC::LaunchProjectile - %s: failed to recover the bind tag %s.","encoding":"ascii"},{"address":"141b9da40","value":"arkCpntWeaponThrowable::HandleFire - a arkCpntAudio is needed here!","encoding":"ascii"},{"address":"141cc64a0","value":"arkCpntWeaponThrowable","encoding":"ascii"},{"address":"141ccde60","value":"arkCpntWeaponThrowableNPC","encoding":"ascii"},{"address":"141d61960","value":"arkValidCpntPtr < const arkCpntWeaponThrowable , CF_STARTED >","encoding":"ascii"},{"address":"141d7b0b8","value":"arkCheckedCpntPtr < arkCpntWeaponThrowable , CF_STARTED >","encoding":"ascii"}],"total":11,"offset":0,"limit":25}
````

### text

````text

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140632f70(longlong *param_1)

{
  float *pfVar1;
  byte bVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  float *pfVar6;
  float *pfVar7;
  undefined8 *puVar8;
  longlong lVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  undefined1 local_res8 [8];
  ulonglong in_stack_fffffffffffffe78;
  undefined4 uVar13;
  ulonglong in_stack_fffffffffffffe80;
  uint in_stack_fffffffffffffe90;
  float local_158;
  float local_154;
  float local_150;
  float local_148;
  float local_144;
  float local_140;
  float local_138;
  float local_134;
  float local_130;
  float local_128;
  float local_124;
  float local_120;
  undefined4 local_118;
  undefined4 local_114;
  undefined4 local_110;
  undefined4 local_10c;
  undefined4 local_108;
  undefined4 local_104;
  float local_100;
  float local_fc;
  float local_f8;
  undefined4 local_e8;
  undefined4 local_e4;
  undefined4 local_e0;
  undefined4 local_d8;
  undefined4 local_d4;
  undefined4 local_d0;
  undefined8 local_b8;
  undefined8 uStack_b0;
  undefined8 local_a8;
  undefined8 uStack_a0;
  undefined4 local_98;
  undefined4 uStack_94;
  undefined4 uStack_90;
  undefined4 uStack_8c;
  undefined8 local_88;
  undefined8 uStack_80;
  
  if ((param_1[0x2c] == 0) || ((*(byte *)(param_1[0x2c] + 0x38) & 8) == 0)) {
    (**(code **)(*param_1 + 0x140))();
    if (param_1[0x13] != 0) {
      lVar9 = param_1[2];
      lVar3 = FUN_14063b5f0(param_1);
      if (lVar3 != 0) {
        lVar5 = *(longlong *)(lVar3 + 0xf0);
        lVar4 = FUN_140877720(lVar3,&DAT_1439aee80,0);
        if ((lVar5 != 0) && (lVar4 != 0)) {
          in_stack_fffffffffffffe78 = in_stack_fffffffffffffe78 & 0xffffffffffffff00;
          lVar5 = FUN_14008c840(*(undefined8 *)(lVar5 + 0x1bb0),
                                *(undefined4 *)(*(longlong *)(lVar4 + 8) + 0x8c),lVar9 + 0x330,
                                0x3c23d70a,in_stack_fffffffffffffe78);
          if (lVar5 != 0) {
            local_b8 = _DAT_1423ccdd0;
            uStack_b0 = uRam00000001423ccdd8;
            local_a8 = _DAT_1423ccde0;
            uStack_a0 = uRam00000001423ccde8;
            local_98 = _DAT_1423ccdf0;
            uStack_94 = uRam00000001423ccdf4;
            uStack_90 = uRam00000001423ccdf8;
            uStack_8c = uRam00000001423ccdfc;
            local_88 = 0;
            uStack_80 = 0;
            (**(code **)(**(longlong **)(lVar5 + 0x30) + 0x20))
                      (*(longlong **)(lVar5 + 0x30),&local_b8,&local_e8);
            local_118 = local_e8;
            local_114 = local_e4;
            local_110 = local_e0;
            local_10c = local_d8;
            local_108 = local_d4;
            local_104 = local_d0;
            FUN_14063a520(param_1,&local_118);
          }
        }
        lVar5 = param_1[9];
        FUN_140639760(param_1,&local_148);
        in_stack_fffffffffffffe78 = in_stack_fffffffffffffe78 & 0xffffffffffffff00;
        FUN_140638ae0(param_1,&local_138,(float *)(lVar5 + 0x25288),&local_148,
                      in_stack_fffffffffffffe78,in_stack_fffffffffffffe80 & 0xffffffffffffff00);
        uVar13 = (undefined4)(in_stack_fffffffffffffe78 >> 0x20);
        pfVar1 = (float *)(lVar9 + 0x30c);
        lVar9 = param_1[1];
        if (*(char *)((longlong)param_1 + 0x17c) == '\0') {
          fVar11 = *(float *)(lVar9 + 0x298);
        }
        else {
          fVar11 = *(float *)(lVar9 + 0x2b8);
        }
        local_148 = local_148 - local_138;
        local_144 = local_144 - local_134;
        local_140 = local_140 - local_130;
        fVar12 = local_144 * local_144 + local_148 * local_148 + local_140 * local_140;
        if (fVar12 <= 1.1754944e-38) {
          fVar12 = 1.1754944e-38;
        }
        local_120 = 1.0 / SQRT(fVar12);
        local_128 = local_148 * local_120;
        local_124 = local_144 * local_120;
        local_120 = local_140 * local_120;
        if (*(char *)((longlong)param_1 + 0x17c) == '\0') {
          fVar12 = *(float *)(lVar9 + 0x29c);
        }
        else {
          fVar12 = *(float *)(lVar9 + 700);
        }
        fVar10 = (float)FUN_140017c70(fVar11 * 0.017453292);
        fVar11 = (float)FUN_140018620(fVar11 * 0.017453292);
        pfVar6 = (float *)FUN_140112000(&local_128,&local_e8);
        local_154 = (fVar10 * pfVar6[1] + pfVar6[4] * 0.0 + fVar11 * pfVar6[7]) * fVar12;
        local_150 = (fVar10 * pfVar6[2] + pfVar6[5] * 0.0 + fVar11 * pfVar6[8]) * fVar12;
        local_158 = (fVar10 * *pfVar6 + pfVar6[3] * 0.0 + fVar11 * pfVar6[6]) * fVar12;
        if ((int)param_1[0x2f] < 0) {
          fVar11 = 0.0;
        }
        else {
          fVar11 = *(float *)(*(longlong *)(param_1[6] + 0x48) + 0x24 +
                             (longlong)(int)param_1[0x2f] * 0x28);
        }
        fVar12 = (float)FUN_1408ac040(0,0x42c80000);
        if (fVar12 < fVar11) {
          local_158 = local_158 * 0.1;
          local_154 = local_154 * 0.1;
          local_150 = local_150 * 0.1;
        }
        FUN_14087c2a0(lVar3,&local_158);
        pfVar6 = pfVar1;
        if (*(char *)(lVar9 + 0x2ac) != '\0') {
          pfVar6 = (float *)(lVar5 + 0x25288);
        }
        pfVar7 = (float *)FUN_14010d3d0(lVar9 + 0x2a0,&local_118);
        fVar11 = pfVar7[1];
        fVar12 = *pfVar7;
        fVar10 = pfVar7[2];
        local_f8 = fVar12 * pfVar6[2] + fVar11 * pfVar6[5] + fVar10 * pfVar6[8];
        local_fc = fVar12 * pfVar6[1] + fVar11 * pfVar6[4] + fVar10 * pfVar6[7];
        local_100 = fVar11 * pfVar6[3] + fVar12 * *pfVar6 + fVar10 * pfVar6[6];
        FUN_14087c280(lVar3,&local_100);
        puVar8 = (undefined8 *)FUN_140131ae0(local_res8);
        in_stack_fffffffffffffe90 = 0xffffffff;
        FUN_140639970(param_1,lVar3,&local_138,pfVar1,CONCAT44(uVar13,0x3f800000),0,*puVar8,
                      0xffffffff,0);
        if ((param_1[0x2c] != 0) && (lVar9 = FUN_140877720(lVar3,&DAT_1439e28e0,0), lVar9 != 0)) {
          FUN_140490f70(lVar9,param_1[0x2c]);
          bVar2 = *(byte *)(lVar9 + 0x38);
          *(byte *)(lVar9 + 0x38) =
               ((*(byte *)((longlong)param_1 + 0xb9) >> 2 | bVar2) ^ bVar2) & 4 ^ bVar2;
        }
      }
    }
    if (-1 < *(int *)((longlong)param_1 + 0x174)) {
      if (param_1[0x2d] == 0) {
                    /* WARNING: Subroutine does not return */
        FUN_140166830("arkCpntWeaponThrowable::HandleFire - a arkCpntAudio is needed here!");
      }
      FUN_140709110(param_1[0x2d],0xffffffff,
                    *(undefined8 *)
                     (*(longlong *)(param_1[1] + 600) + 0x10 +
                     (longlong)*(int *)((longlong)param_1 + 0x174) * 0x18),0,0,0,1,
                    in_stack_fffffffffffffe90 & 0xffffff00);
      return;
    }
  }
  return;
}


````
