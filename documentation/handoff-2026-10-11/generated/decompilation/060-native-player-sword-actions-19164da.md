# native-player-sword-actions.json

Original: [outputs/d2-coop-lab/analysis/ghidra/native-player-sword-actions.json](<../../../../outputs/d2-coop-lab/analysis/ghidra/native-player-sword-actions.json>)

SHA-256: `666289C387759E0D06443F8171D51F3780BBFFE2D446EC0CCDEE43213490C545`; 5068 bytes.

## Original saved record

````json
{
  "capturedAt": "2026-10-09",
  "exeSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "confirmed": {
    "slash": {
      "id": 39,
      "vtableRva": "1c85d28"
    },
    "block": {
      "id": 44,
      "vtableRva": "1c81a68"
    },
    "runtimeEvidence": "research/doppelganger-player-actions-observation.json"
  },
  "notes": "Classes confirmed on both characters; damage flag meanings and state 55 remain hypotheses.",
  "functions": {
    "140630180": {
      "result": "\r\nvoid FUN_140630180(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined8 uVar2;\r\n  undefined4 *puVar3;\r\n  undefined1 auStackX_8 [8];\r\n  \r\n  FUN_1406360a0();\r\n  lVar1 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));\r\n  if (lVar1 != 0) {\r\n    uVar2 = FUN_140877720(lVar1,&DAT_1439aed60,0);\r\n    *(undefined8 *)(param_1 + 0x3430) = uVar2;\r\n    uVar2 = FUN_140877720(lVar1,&DAT_1439d7480,0);\r\n    *(undefined8 *)(param_1 + 0x3438) = uVar2;\r\n    uVar2 = FUN_140877720(lVar1,&DAT_1439e86d0,0);\r\n    *(undefined8 *)(param_1 + 0x3440) = uVar2;\r\n    uVar2 = FUN_140877720(lVar1,&DAT_1439ee130,0);\r\n    *(undefined8 *)(param_1 + 0x3448) = uVar2;\r\n    puVar3 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),auStackX_8,&DAT_143987f88);\r\n    if ((undefined4 *)(param_1 + 0x3450) != puVar3) {\r\n      *(undefined4 *)(param_1 + 0x3450) = *puVar3;\r\n    }\r\n    puVar3 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),auStackX_8,&DAT_143987f90);\r\n    if ((undefined4 *)(param_1 + 0x3454) != puVar3) {\r\n      *(undefined4 *)(param_1 + 0x3454) = *puVar3;\r\n    }\r\n    FUN_140632820(param_1 + 0x33a8,param_1,*(longlong *)(param_1 + 8) + 0x1c0);\r\n    return;\r\n  }\r\n  uVar2 = D2_InternedName_Resolve(*(longlong *)(param_1 + 0x10) + 0x20);\r\n                    /* WARNING: Subroutine does not return */\r\n  FUN_140166830(\"[ arkCpntWeaponMeleePlayer::OnStarted ] %s has no spawner entity.\\n\",uVar2);\r\n}\r\n\r\n"
    },
    "14062f160": {
      "result": "\r\nundefined8 FUN_14062f160(longlong param_1)\r\n\r\n{\r\n  longlong lVar1;\r\n  longlong lVar2;\r\n  char cVar3;\r\n  undefined8 uVar4;\r\n  \r\n  uVar4 = FUN_140635090();\r\n  lVar1 = *(longlong *)(param_1 + 0x3430);\r\n  cVar3 = FUN_140803180(lVar1,0x27);\r\n  if (cVar3 == '\\0') {\r\n    cVar3 = FUN_140803180(lVar1,0x37);\r\n    if (cVar3 != '\\0') {\r\n      uVar4 = *(undefined8 *)(*(longlong *)(param_1 + 8) + 0x1f0);\r\n    }\r\n  }\r\n  else {\r\n    lVar2 = *(longlong *)(param_1 + 8);\r\n    if ((*(longlong *)(lVar1 + 0x90) != 0) &&\r\n       (lVar1 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x148), lVar1 != 0)) {\r\n      if (*(char *)(lVar1 + 0xd2) != '\\0') {\r\n        if (*(char *)(lVar1 + 0xd3) != '\\0') {\r\n          return *(undefined8 *)(lVar2 + 0x1e8);\r\n        }\r\n        return *(undefined8 *)(lVar2 + 0x1d0);\r\n      }\r\n      if (*(char *)(lVar1 + 0xd3) != '\\0') {\r\n        return *(undefined8 *)(lVar2 + 0x1e0);\r\n      }\r\n      if (*(char *)(lVar1 + 0xd9) != '\\0') {\r\n        return *(undefined8 *)(lVar2 + 0x1f8);\r\n      }\r\n    }\r\n  }\r\n  return uVar4;\r\n}\r\n\r\n"
    },
    "14062f3e0": {
      "result": "\r\nvoid FUN_14062f3e0(longlong param_1,undefined8 param_2,undefined8 param_3)\r\n\r\n{\r\n  longlong lVar1;\r\n  char cVar2;\r\n  \r\n  FUN_140780600(param_3);\r\n  lVar1 = *(longlong *)(param_1 + 0x3430);\r\n  cVar2 = FUN_140803180(lVar1,0x27);\r\n  if (((cVar2 != '\\0') && (lVar1 = *(longlong *)(lVar1 + 0x90), lVar1 != 0)) &&\r\n     (lVar1 = *(longlong *)(lVar1 + 0x148), lVar1 != 0)) {\r\n    FUN_14086c020(lVar1,param_2,param_3);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "14086c020": {
      "result": "\r\nvoid FUN_14086c020(longlong param_1,longlong param_2,int *param_3)\r\n\r\n{\r\n  int iVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  float fVar4;\r\n  float fVar5;\r\n  float fVar6;\r\n  float fVar7;\r\n  \r\n  FUN_1403cc8a0(*(undefined8 *)(param_1 + 0x98));\r\n  if ((*param_3 == 0) && (*(char *)(param_1 + 0xd3) != '\\0')) {\r\n    lVar2 = *(longlong *)(param_1 + 0x18);\r\n    lVar3 = FUN_140854880(param_1);\r\n    fVar7 = *(float *)(param_2 + 0x300) - *(float *)(lVar3 + 0x300);\r\n    fVar6 = *(float *)(param_2 + 0x304) - *(float *)(lVar3 + 0x304);\r\n    fVar4 = fVar7 * fVar7 + fVar6 * fVar6;\r\n    if (fVar4 <= 1.1754944e-38) {\r\n      fVar4 = 1.1754944e-38;\r\n    }\r\n    fVar7 = fVar7 * (1.0 / SQRT(fVar4));\r\n    fVar6 = fVar6 * (1.0 / SQRT(fVar4));\r\n    fVar4 = (float)FUN_1411af8ac(*(float *)(lVar2 + 0xe4) * 0.017453292);\r\n    iVar1 = *(int *)(lVar2 + 0xe0);\r\n    fVar5 = fVar7 * fVar7 + fVar6 * fVar6 + fVar4 * fVar4;\r\n    if (fVar5 <= 1.1754944e-38) {\r\n      fVar5 = 1.1754944e-38;\r\n    }\r\n    *param_3 = 6;\r\n    fVar5 = 1.0 / SQRT(fVar5);\r\n    param_3[1] = (int)(fVar7 * fVar5);\r\n    param_3[2] = (int)(fVar6 * fVar5);\r\n    param_3[3] = (int)(fVar4 * fVar5);\r\n    param_3[7] = iVar1;\r\n    param_3[0xb] = 0x3fff0010;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  }
}
````

## Unescaped text fields for reading

### result

````text

void FUN_140630180(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined4 *puVar3;
  undefined1 auStackX_8 [8];
  
  FUN_1406360a0();
  lVar1 = FUN_1406dc560(*(undefined4 *)(*(longlong *)(param_1 + 0x10) + 0xac));
  if (lVar1 != 0) {
    uVar2 = FUN_140877720(lVar1,&DAT_1439aed60,0);
    *(undefined8 *)(param_1 + 0x3430) = uVar2;
    uVar2 = FUN_140877720(lVar1,&DAT_1439d7480,0);
    *(undefined8 *)(param_1 + 0x3438) = uVar2;
    uVar2 = FUN_140877720(lVar1,&DAT_1439e86d0,0);
    *(undefined8 *)(param_1 + 0x3440) = uVar2;
    uVar2 = FUN_140877720(lVar1,&DAT_1439ee130,0);
    *(undefined8 *)(param_1 + 0x3448) = uVar2;
    puVar3 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),auStackX_8,&DAT_143987f88);
    if ((undefined4 *)(param_1 + 0x3450) != puVar3) {
      *(undefined4 *)(param_1 + 0x3450) = *puVar3;
    }
    puVar3 = (undefined4 *)FUN_14060ce50(*(undefined8 *)(param_1 + 0x30),auStackX_8,&DAT_143987f90);
    if ((undefined4 *)(param_1 + 0x3454) != puVar3) {
      *(undefined4 *)(param_1 + 0x3454) = *puVar3;
    }
    FUN_140632820(param_1 + 0x33a8,param_1,*(longlong *)(param_1 + 8) + 0x1c0);
    return;
  }
  uVar2 = D2_InternedName_Resolve(*(longlong *)(param_1 + 0x10) + 0x20);
                    /* WARNING: Subroutine does not return */
  FUN_140166830("[ arkCpntWeaponMeleePlayer::OnStarted ] %s has no spawner entity.\n",uVar2);
}


````

### result

````text

undefined8 FUN_14062f160(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  char cVar3;
  undefined8 uVar4;
  
  uVar4 = FUN_140635090();
  lVar1 = *(longlong *)(param_1 + 0x3430);
  cVar3 = FUN_140803180(lVar1,0x27);
  if (cVar3 == '\0') {
    cVar3 = FUN_140803180(lVar1,0x37);
    if (cVar3 != '\0') {
      uVar4 = *(undefined8 *)(*(longlong *)(param_1 + 8) + 0x1f0);
    }
  }
  else {
    lVar2 = *(longlong *)(param_1 + 8);
    if ((*(longlong *)(lVar1 + 0x90) != 0) &&
       (lVar1 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x148), lVar1 != 0)) {
      if (*(char *)(lVar1 + 0xd2) != '\0') {
        if (*(char *)(lVar1 + 0xd3) != '\0') {
          return *(undefined8 *)(lVar2 + 0x1e8);
        }
        return *(undefined8 *)(lVar2 + 0x1d0);
      }
      if (*(char *)(lVar1 + 0xd3) != '\0') {
        return *(undefined8 *)(lVar2 + 0x1e0);
      }
      if (*(char *)(lVar1 + 0xd9) != '\0') {
        return *(undefined8 *)(lVar2 + 0x1f8);
      }
    }
  }
  return uVar4;
}


````

### result

````text

void FUN_14062f3e0(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  longlong lVar1;
  char cVar2;
  
  FUN_140780600(param_3);
  lVar1 = *(longlong *)(param_1 + 0x3430);
  cVar2 = FUN_140803180(lVar1,0x27);
  if (((cVar2 != '\0') && (lVar1 = *(longlong *)(lVar1 + 0x90), lVar1 != 0)) &&
     (lVar1 = *(longlong *)(lVar1 + 0x148), lVar1 != 0)) {
    FUN_14086c020(lVar1,param_2,param_3);
  }
  return;
}


````

### result

````text

void FUN_14086c020(longlong param_1,longlong param_2,int *param_3)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  
  FUN_1403cc8a0(*(undefined8 *)(param_1 + 0x98));
  if ((*param_3 == 0) && (*(char *)(param_1 + 0xd3) != '\0')) {
    lVar2 = *(longlong *)(param_1 + 0x18);
    lVar3 = FUN_140854880(param_1);
    fVar7 = *(float *)(param_2 + 0x300) - *(float *)(lVar3 + 0x300);
    fVar6 = *(float *)(param_2 + 0x304) - *(float *)(lVar3 + 0x304);
    fVar4 = fVar7 * fVar7 + fVar6 * fVar6;
    if (fVar4 <= 1.1754944e-38) {
      fVar4 = 1.1754944e-38;
    }
    fVar7 = fVar7 * (1.0 / SQRT(fVar4));
    fVar6 = fVar6 * (1.0 / SQRT(fVar4));
    fVar4 = (float)FUN_1411af8ac(*(float *)(lVar2 + 0xe4) * 0.017453292);
    iVar1 = *(int *)(lVar2 + 0xe0);
    fVar5 = fVar7 * fVar7 + fVar6 * fVar6 + fVar4 * fVar4;
    if (fVar5 <= 1.1754944e-38) {
      fVar5 = 1.1754944e-38;
    }
    *param_3 = 6;
    fVar5 = 1.0 / SQRT(fVar5);
    param_3[1] = (int)(fVar7 * fVar5);
    param_3[2] = (int)(fVar6 * fVar5);
    param_3[3] = (int)(fVar4 * fVar5);
    param_3[7] = iVar1;
    param_3[0xb] = 0x3fff0010;
  }
  return;
}


````
