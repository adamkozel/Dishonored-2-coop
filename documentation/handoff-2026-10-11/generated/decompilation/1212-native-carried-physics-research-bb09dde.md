# native-carried-physics-research.json

Original: [work/natural-moveset/carry-peer-research-v1/native-carried-physics-research.json](<../../../../work/natural-moveset/carry-peer-research-v1/native-carried-physics-research.json>)

SHA-256: `2B348911ED40A09F84AC636BBA1BCF60BAB0BB4B45CD0DC9F0FCC048211D04CA`; 10584 bytes.

## Original saved record

````json
{
  "exeSha256": "B671F8249422862BF557D00AAB2F4BB671ED4515CC10AD8A8A9DECAD84516180",
  "confirmed": [
    "Native Carry45 sends target event 0xba at1408288c7.",
    "PhysicsSystemEvolved registers handler140736ef0 for0xba at140732ecc.",
    "This callback queues animated mode3. Native734a10 queues ragdoll mode0.",
    "Native source held corpse mode=3 and released mode=0 in host-physics-carry.json / host-physics-released.json.",
    "Scoped native-victim v2 reproduced carried bit0x2 and active single-corpse Midnight scene, but physics stayed0 and visuals remained ragdoll."
  ],
  "pending": [
    "Native-victim v3 callback experiment and clean ragdoll-return verification",
    "Animated victim alignment to remote carrier spine"
  ],
  "functions": {
    "140735290": {
      "result": "\r\nulonglong FUN_140735290(longlong param_1,longlong param_2,ulonglong param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  int iVar2;\r\n  longlong lVar3;\r\n  bool bVar4;\r\n  bool bVar5;\r\n  char cVar6;\r\n  undefined1 uVar7;\r\n  float *pfVar8;\r\n  undefined8 uVar9;\r\n  ulonglong uVar10;\r\n  uint7 uVar11;\r\n  bool bVar12;\r\n  float fVar13;\r\n  float fVar14;\r\n  float fVar15;\r\n  undefined1 local_res18 [16];\r\n  \r\n  uVar10 = param_3 & 0xffffffff;\r\n  if ((int)param_3 == -1) {\r\n    if (*(int *)(param_1 + 0x238) == 2) {\r\n      uVar7 = FUN_1407c7ac0(param_1 + 600);\r\n      if (*(longlong *)(param_1 + 0x2c0) == 0) {\r\n        cVar6 = FUN_1404e23a0(*(undefined8 *)(param_1 + 0x2c8),param_2,param_1 + 0x2d0,\r\n                              *(byte *)(param_1 + 0x240) >> 7,uVar7);\r\n        if (cVar6 != '\\0') {\r\n          lVar3 = *(longlong *)(param_1 + 0x2c8);\r\n          if (*(float *)(lVar3 + 0x78) == 0.0) {\r\n            return uVar10;\r\n          }\r\n          pfVar8 = (float *)FUN_140877eb0(param_2,local_res18);\r\n          fVar15 = *pfVar8 / *(float *)(lVar3 + 0x78);\r\n          fVar13 = *(float *)(param_1 + 0x2d8) -\r\n                   fVar15 * *(float *)(*(longlong *)(param_1 + 0x2c8) + 0x70);\r\n          fVar14 = 0.0;\r\n          if (0.0 <= fVar13) {\r\n            fVar14 = fVar13;\r\n          }\r\n          *(float *)(param_1 + 0x2d8) = fVar14;\r\n          fVar15 = *(float *)(param_1 + 0x2dc) -\r\n                   fVar15 * *(float *)(*(longlong *)(param_1 + 0x2c8) + 0x74);\r\n          fVar13 = 0.0;\r\n          if (0.0 <= fVar15) {\r\n            fVar13 = fVar15;\r\n          }\r\n          *(float *)(param_1 + 0x2dc) = fVar13;\r\n          if (fVar14 != 0.0) {\r\n            return uVar10;\r\n          }\r\n          if (fVar13 != 0.0) {\r\n            return uVar10;\r\n          }\r\n        }\r\n        *(undefined8 *)(param_1 + 0x2c0) = *(undefined8 *)(*(longlong *)(param_1 + 0x2c8) + 0x60);\r\n      }\r\n      cVar6 = FUN_140734800(param_1);\r\n      if (cVar6 == '\\0') {\r\n        uVar10 = 1;\r\n      }\r\n    }\r\n    else if ((*(int *)(param_1 + 0x238) == 0) &&\r\n            ((*(longlong *)(param_1 + 0x2c0) != 0 || ((*(byte *)(param_1 + 0x241) & 1) != 0)))) {\r\n      uVar10 = 2;\r\n    }\r\n  }\r\n  else {\r\n    if (((*(byte *)(param_1 + 0x241) & 1) == 0) && ((param_3 & 0xfffffffd) == 0)) {\r\n      cVar6 = FUN_140734800();\r\n      uVar10 = (ulonglong)(-(uint)(cVar6 != '\\0') & 2);\r\n    }\r\n    else if ((int)param_3 == 1) {\r\n      uVar10 = 0;\r\n    }\r\n    iVar2 = *(int *)(param_1 + 0x238);\r\n    if ((iVar2 == 2) || (iVar2 == 0)) {\r\n      bVar4 = true;\r\n    }\r\n    else {\r\n      bVar4 = false;\r\n    }\r\n    bVar12 = (int)uVar10 == 2;\r\n    if ((bVar12) || ((int)uVar10 == 0)) {\r\n      bVar5 = true;\r\n    }\r\n    else {\r\n      bVar5 = false;\r\n    }\r\n    if ((!bVar4) && (bVar5)) {\r\n      uVar1 = *(undefined4 *)(param_2 + 0x308);\r\n      *(undefined4 *)(param_1 + 0x3ac) = uVar1;\r\n      *(undefined4 *)(param_1 + 0x3b0) = uVar1;\r\n    }\r\n    if ((iVar2 == 2) && (!bVar12)) {\r\n      FUN_140734c70(param_1);\r\n    }\r\n    if ((!bVar5) || (bVar12)) {\r\n      uVar9 = 1;\r\n    }\r\n    else {\r\n      uVar9 = 0;\r\n    }\r\n    uVar11 = (uint7)(param_3 >> 8);\r\n    if ((((bVar5) || (*(int *)(param_1 + 0x238) == -1)) || ((*(byte *)(param_2 + 0xd1) & 0x20) != 0)\r\n        ) || ((*(byte *)(param_2 + 0xd1) & 8) != 0)) {\r\n      FUN_140730e60(param_1,uVar9,(ulonglong)uVar11 << 8);\r\n    }\r\n    else {\r\n      FUN_140730e60(param_1,uVar9,CONCAT71(uVar11,1));\r\n    }\r\n  }\r\n  return uVar10;\r\n}\r\n\r\n"
    },
    "140733e1e": {
      "result": "\r\nvoid FUN_140733e10(longlong param_1,int param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  bool bVar3;\r\n  uint uVar4;\r\n  longlong lVar5;\r\n  uint uVar6;\r\n  longlong lVar7;\r\n  short sVar8;\r\n  undefined8 uVar9;\r\n  \r\n  if (*(int *)(param_1 + 0x238) == param_2) {\r\n    return;\r\n  }\r\n  lVar2 = *(longlong *)(param_1 + 0x10);\r\n  lVar5 = FUN_140878470(lVar2);\r\n  uVar4 = FUN_140735290(param_1,lVar2,param_2);\r\n  uVar6 = uVar4;\r\n  if (uVar4 != 0) {\r\n    if (uVar4 == 2) {\r\n      sVar8 = 0;\r\n      if (0 < *(int *)(param_1 + 0x138)) {\r\n        do {\r\n          if (*(int *)((longlong)sVar8 * (longlong)*(int *)(param_1 + 0x1e0) + 0xb0 +\r\n                      *(longlong *)(param_1 + 0x1d8)) != 0) {\r\n            FUN_140733a20(param_1,sVar8,2);\r\n          }\r\n          sVar8 = sVar8 + 1;\r\n        } while ((int)sVar8 < *(int *)(param_1 + 0x138));\r\n      }\r\n      *(byte *)(param_1 + 0x240) = *(byte *)(param_1 + 0x240) & 0xfc;\r\n      *(byte *)(param_1 + 0x241) = *(byte *)(param_1 + 0x241) & 0xdf;\r\n      FUN_14025e430(lVar5,1);\r\n      goto LAB_14073401c;\r\n    }\r\n    uVar6 = uVar4 - 3;\r\n    if (uVar6 == 0) {\r\n      sVar8 = 0;\r\n      if (0 < *(int *)(param_1 + 0x138)) {\r\n        do {\r\n          if (*(int *)((longlong)sVar8 * (longlong)*(int *)(param_1 + 0x1e0) + 0xb0 +\r\n                      *(longlong *)(param_1 + 0x1d8)) != 0) {\r\n            FUN_140733a20(param_1,sVar8,3);\r\n          }\r\n          sVar8 = sVar8 + 1;\r\n        } while ((int)sVar8 < *(int *)(param_1 + 0x138));\r\n      }\r\n      FUN_14025e430(lVar5,*(undefined1 *)(*(longlong *)(lVar5 + 0x1fe8) + 0xf8));\r\n      goto LAB_14073401c;\r\n    }\r\n  }\r\n  sVar8 = 0;\r\n  lVar7 = (ulonglong)(uVar6 >> 8) << 8;\r\n  bVar3 = false;\r\n  if (0 < *(int *)(param_1 + 0x138)) {\r\n    do {\r\n      if (*(int *)((longlong)sVar8 * (longlong)*(int *)(param_1 + 0x1e0) + 0xb0 +\r\n                  *(longlong *)(param_1 + 0x1d8)) != 0) {\r\n        if ((uVar4 == 0) || ((uVar4 == 4 && (bVar3)))) {\r\n          bVar3 = true;\r\n        }\r\n        else {\r\n          bVar3 = false;\r\n        }\r\n        uVar9 = 3;\r\n        if (bVar3) {\r\n          uVar9 = 1;\r\n        }\r\n        FUN_140733a20(param_1,sVar8,uVar9);\r\n        lVar7 = 0;\r\n        bVar3 = true;\r\n      }\r\n      sVar8 = sVar8 + 1;\r\n    } while ((int)sVar8 < *(int *)(param_1 + 0x138));\r\n  }\r\n  *(byte *)(param_1 + 0x240) = *(byte *)(param_1 + 0x240) & 0xfc;\r\n  *(byte *)(param_1 + 0x241) = *(byte *)(param_1 + 0x241) & 0xdf;\r\n  FUN_14025e430(lVar5,CONCAT71((int7)((ulonglong)lVar7 >> 8),1));\r\n  if (((*(byte *)(lVar2 + 0xd1) & 0x20) != 0) || ((*(byte *)(lVar2 + 0xd1) & 8) != 0)) {\r\n    uVar1 = *(undefined4 *)(lVar2 + 0xa8);\r\n    uVar9 = FUN_140810240();\r\n    FUN_1408103f0(uVar9,0xb6,uVar1);\r\n  }\r\nLAB_14073401c:\r\n  *(uint *)(param_1 + 0x238) = uVar4;\r\n  return;\r\n}\r\n\r\n"
    },
    "140733e00": {
      "result": "\r\nvoid FUN_140733e00(longlong *param_1,undefined4 param_2)\r\n\r\n{\r\n  *(undefined4 *)((longlong)param_1 + 0x23c) = param_2;\r\n                    /* WARNING: Could not recover jumptable at 0x000140733e09. Too many branches */\r\n                    /* WARNING: Treating indirect jump as call */\r\n  (**(code **)(*param_1 + 0x1c0))();\r\n  return;\r\n}\r\n\r\n"
    },
    "140736ef0": {
      "result": "\r\nvoid FUN_140736ef0(longlong *param_1)\r\n\r\n{\r\n  *(byte *)(param_1 + 0x48) = *(byte *)(param_1 + 0x48) & 0xf8;\r\n  FUN_140735830();\r\n  *(undefined4 *)((longlong)param_1 + 0x23c) = 3;\r\n  (**(code **)(*param_1 + 0x1c0))(param_1);\r\n                    /* WARNING: Could not recover jumptable at 0x000140736f26. Too many branches */\r\n                    /* WARNING: Treating indirect jump as call */\r\n  (**(code **)(*param_1 + 0x130))(param_1);\r\n  return;\r\n}\r\n\r\n"
    },
    "140734a10": {
      "result": "\r\nvoid FUN_140734a10(longlong *param_1)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  int iVar3;\r\n  longlong lVar4;\r\n  undefined8 uVar5;\r\n  undefined4 *puVar6;\r\n  \r\n  if ((*(byte *)((longlong)param_1 + 0x241) & 8) == 0) {\r\n    *(undefined4 *)((longlong)param_1 + 0x23c) = 0;\r\n    (**(code **)(*param_1 + 0x1c0))();\r\n    lVar4 = FUN_1403da910(param_1 + 0x42,0);\r\n    if ((lVar4 != 0) && ((*(byte *)(lVar4 + 0x151) & 1) != 0)) {\r\n      FUN_140853220(D2_GameSingletonPtr + 0x60280,param_1,1,0);\r\n    }\r\n    lVar2 = D2_GameSingletonPtr;\r\n    lVar4 = param_1[2];\r\n    if ((*(byte *)(lVar4 + 0xd1) & 8) == 0) {\r\n      uVar5 = FUN_1405bef90();\r\n      iVar3 = FUN_140604bd0(lVar2 + 0x1f7bb8,lVar4,uVar5);\r\n      if (iVar3 == 0) {\r\n        if (*(float *)((longlong)param_1 + 0x3dc) == 0.0) {\r\n          uVar1 = *(undefined4 *)(lVar4 + 0xa8);\r\n          lVar4 = FUN_140810240();\r\n          puVar6 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17b,4,uVar1);\r\n          if (puVar6 != (undefined4 *)0x0) {\r\n            *puVar6 = 10;\r\n          }\r\n        }\r\n        *(undefined4 *)((longlong)param_1 + 0x3dc) = *(undefined4 *)(param_1[1] + 300);\r\n      }\r\n    }\r\n    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);\r\n    uVar5 = FUN_140810240();\r\n    FUN_1408103f0(uVar5,0xbb,uVar1);\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140829a32": {
      "result": "\r\nvoid FUN_140829a10(longlong param_1,char param_2)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  longlong lVar2;\r\n  undefined8 *puVar3;\r\n  uint uStack_c;\r\n  \r\n  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));\r\n  if ((lVar2 != 0) && (*(byte *)(lVar2 + 0x368) = *(byte *)(lVar2 + 0x368) & 0xf9, param_2 != '\\0'))\r\n  {\r\n    uVar1 = *(undefined4 *)(lVar2 + 0xa8);\r\n    lVar2 = FUN_140810240();\r\n    puVar3 = (undefined8 *)FUN_1408112c0(lVar2 + 0xc8e8,0xb9,0x10,uVar1);\r\n    if (puVar3 != (undefined8 *)0x0) {\r\n      *puVar3 = 0;\r\n      puVar3[1] = (ulonglong)uStack_c << 0x20;\r\n    }\r\n  }\r\n  *(undefined4 *)(param_1 + 0xf8) = 0x3fff0010;\r\n  FUN_1403dab80(param_1 + 0x100,0);\r\n  return;\r\n}\r\n\r\n"
    }
  }
}

````

## Unescaped text fields for reading

### result

````text

ulonglong FUN_140735290(longlong param_1,longlong param_2,ulonglong param_3)

{
  undefined4 uVar1;
  int iVar2;
  longlong lVar3;
  bool bVar4;
  bool bVar5;
  char cVar6;
  undefined1 uVar7;
  float *pfVar8;
  undefined8 uVar9;
  ulonglong uVar10;
  uint7 uVar11;
  bool bVar12;
  float fVar13;
  float fVar14;
  float fVar15;
  undefined1 local_res18 [16];
  
  uVar10 = param_3 & 0xffffffff;
  if ((int)param_3 == -1) {
    if (*(int *)(param_1 + 0x238) == 2) {
      uVar7 = FUN_1407c7ac0(param_1 + 600);
      if (*(longlong *)(param_1 + 0x2c0) == 0) {
        cVar6 = FUN_1404e23a0(*(undefined8 *)(param_1 + 0x2c8),param_2,param_1 + 0x2d0,
                              *(byte *)(param_1 + 0x240) >> 7,uVar7);
        if (cVar6 != '\0') {
          lVar3 = *(longlong *)(param_1 + 0x2c8);
          if (*(float *)(lVar3 + 0x78) == 0.0) {
            return uVar10;
          }
          pfVar8 = (float *)FUN_140877eb0(param_2,local_res18);
          fVar15 = *pfVar8 / *(float *)(lVar3 + 0x78);
          fVar13 = *(float *)(param_1 + 0x2d8) -
                   fVar15 * *(float *)(*(longlong *)(param_1 + 0x2c8) + 0x70);
          fVar14 = 0.0;
          if (0.0 <= fVar13) {
            fVar14 = fVar13;
          }
          *(float *)(param_1 + 0x2d8) = fVar14;
          fVar15 = *(float *)(param_1 + 0x2dc) -
                   fVar15 * *(float *)(*(longlong *)(param_1 + 0x2c8) + 0x74);
          fVar13 = 0.0;
          if (0.0 <= fVar15) {
            fVar13 = fVar15;
          }
          *(float *)(param_1 + 0x2dc) = fVar13;
          if (fVar14 != 0.0) {
            return uVar10;
          }
          if (fVar13 != 0.0) {
            return uVar10;
          }
        }
        *(undefined8 *)(param_1 + 0x2c0) = *(undefined8 *)(*(longlong *)(param_1 + 0x2c8) + 0x60);
      }
      cVar6 = FUN_140734800(param_1);
      if (cVar6 == '\0') {
        uVar10 = 1;
      }
    }
    else if ((*(int *)(param_1 + 0x238) == 0) &&
            ((*(longlong *)(param_1 + 0x2c0) != 0 || ((*(byte *)(param_1 + 0x241) & 1) != 0)))) {
      uVar10 = 2;
    }
  }
  else {
    if (((*(byte *)(param_1 + 0x241) & 1) == 0) && ((param_3 & 0xfffffffd) == 0)) {
      cVar6 = FUN_140734800();
      uVar10 = (ulonglong)(-(uint)(cVar6 != '\0') & 2);
    }
    else if ((int)param_3 == 1) {
      uVar10 = 0;
    }
    iVar2 = *(int *)(param_1 + 0x238);
    if ((iVar2 == 2) || (iVar2 == 0)) {
      bVar4 = true;
    }
    else {
      bVar4 = false;
    }
    bVar12 = (int)uVar10 == 2;
    if ((bVar12) || ((int)uVar10 == 0)) {
      bVar5 = true;
    }
    else {
      bVar5 = false;
    }
    if ((!bVar4) && (bVar5)) {
      uVar1 = *(undefined4 *)(param_2 + 0x308);
      *(undefined4 *)(param_1 + 0x3ac) = uVar1;
      *(undefined4 *)(param_1 + 0x3b0) = uVar1;
    }
    if ((iVar2 == 2) && (!bVar12)) {
      FUN_140734c70(param_1);
    }
    if ((!bVar5) || (bVar12)) {
      uVar9 = 1;
    }
    else {
      uVar9 = 0;
    }
    uVar11 = (uint7)(param_3 >> 8);
    if ((((bVar5) || (*(int *)(param_1 + 0x238) == -1)) || ((*(byte *)(param_2 + 0xd1) & 0x20) != 0)
        ) || ((*(byte *)(param_2 + 0xd1) & 8) != 0)) {
      FUN_140730e60(param_1,uVar9,(ulonglong)uVar11 << 8);
    }
    else {
      FUN_140730e60(param_1,uVar9,CONCAT71(uVar11,1));
    }
  }
  return uVar10;
}


````

### result

````text

void FUN_140733e10(longlong param_1,int param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  bool bVar3;
  uint uVar4;
  longlong lVar5;
  uint uVar6;
  longlong lVar7;
  short sVar8;
  undefined8 uVar9;
  
  if (*(int *)(param_1 + 0x238) == param_2) {
    return;
  }
  lVar2 = *(longlong *)(param_1 + 0x10);
  lVar5 = FUN_140878470(lVar2);
  uVar4 = FUN_140735290(param_1,lVar2,param_2);
  uVar6 = uVar4;
  if (uVar4 != 0) {
    if (uVar4 == 2) {
      sVar8 = 0;
      if (0 < *(int *)(param_1 + 0x138)) {
        do {
          if (*(int *)((longlong)sVar8 * (longlong)*(int *)(param_1 + 0x1e0) + 0xb0 +
                      *(longlong *)(param_1 + 0x1d8)) != 0) {
            FUN_140733a20(param_1,sVar8,2);
          }
          sVar8 = sVar8 + 1;
        } while ((int)sVar8 < *(int *)(param_1 + 0x138));
      }
      *(byte *)(param_1 + 0x240) = *(byte *)(param_1 + 0x240) & 0xfc;
      *(byte *)(param_1 + 0x241) = *(byte *)(param_1 + 0x241) & 0xdf;
      FUN_14025e430(lVar5,1);
      goto LAB_14073401c;
    }
    uVar6 = uVar4 - 3;
    if (uVar6 == 0) {
      sVar8 = 0;
      if (0 < *(int *)(param_1 + 0x138)) {
        do {
          if (*(int *)((longlong)sVar8 * (longlong)*(int *)(param_1 + 0x1e0) + 0xb0 +
                      *(longlong *)(param_1 + 0x1d8)) != 0) {
            FUN_140733a20(param_1,sVar8,3);
          }
          sVar8 = sVar8 + 1;
        } while ((int)sVar8 < *(int *)(param_1 + 0x138));
      }
      FUN_14025e430(lVar5,*(undefined1 *)(*(longlong *)(lVar5 + 0x1fe8) + 0xf8));
      goto LAB_14073401c;
    }
  }
  sVar8 = 0;
  lVar7 = (ulonglong)(uVar6 >> 8) << 8;
  bVar3 = false;
  if (0 < *(int *)(param_1 + 0x138)) {
    do {
      if (*(int *)((longlong)sVar8 * (longlong)*(int *)(param_1 + 0x1e0) + 0xb0 +
                  *(longlong *)(param_1 + 0x1d8)) != 0) {
        if ((uVar4 == 0) || ((uVar4 == 4 && (bVar3)))) {
          bVar3 = true;
        }
        else {
          bVar3 = false;
        }
        uVar9 = 3;
        if (bVar3) {
          uVar9 = 1;
        }
        FUN_140733a20(param_1,sVar8,uVar9);
        lVar7 = 0;
        bVar3 = true;
      }
      sVar8 = sVar8 + 1;
    } while ((int)sVar8 < *(int *)(param_1 + 0x138));
  }
  *(byte *)(param_1 + 0x240) = *(byte *)(param_1 + 0x240) & 0xfc;
  *(byte *)(param_1 + 0x241) = *(byte *)(param_1 + 0x241) & 0xdf;
  FUN_14025e430(lVar5,CONCAT71((int7)((ulonglong)lVar7 >> 8),1));
  if (((*(byte *)(lVar2 + 0xd1) & 0x20) != 0) || ((*(byte *)(lVar2 + 0xd1) & 8) != 0)) {
    uVar1 = *(undefined4 *)(lVar2 + 0xa8);
    uVar9 = FUN_140810240();
    FUN_1408103f0(uVar9,0xb6,uVar1);
  }
LAB_14073401c:
  *(uint *)(param_1 + 0x238) = uVar4;
  return;
}


````

### result

````text

void FUN_140733e00(longlong *param_1,undefined4 param_2)

{
  *(undefined4 *)((longlong)param_1 + 0x23c) = param_2;
                    /* WARNING: Could not recover jumptable at 0x000140733e09. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x1c0))();
  return;
}


````

### result

````text

void FUN_140736ef0(longlong *param_1)

{
  *(byte *)(param_1 + 0x48) = *(byte *)(param_1 + 0x48) & 0xf8;
  FUN_140735830();
  *(undefined4 *)((longlong)param_1 + 0x23c) = 3;
  (**(code **)(*param_1 + 0x1c0))(param_1);
                    /* WARNING: Could not recover jumptable at 0x000140736f26. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (**(code **)(*param_1 + 0x130))(param_1);
  return;
}


````

### result

````text

void FUN_140734a10(longlong *param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  int iVar3;
  longlong lVar4;
  undefined8 uVar5;
  undefined4 *puVar6;
  
  if ((*(byte *)((longlong)param_1 + 0x241) & 8) == 0) {
    *(undefined4 *)((longlong)param_1 + 0x23c) = 0;
    (**(code **)(*param_1 + 0x1c0))();
    lVar4 = FUN_1403da910(param_1 + 0x42,0);
    if ((lVar4 != 0) && ((*(byte *)(lVar4 + 0x151) & 1) != 0)) {
      FUN_140853220(D2_GameSingletonPtr + 0x60280,param_1,1,0);
    }
    lVar2 = D2_GameSingletonPtr;
    lVar4 = param_1[2];
    if ((*(byte *)(lVar4 + 0xd1) & 8) == 0) {
      uVar5 = FUN_1405bef90();
      iVar3 = FUN_140604bd0(lVar2 + 0x1f7bb8,lVar4,uVar5);
      if (iVar3 == 0) {
        if (*(float *)((longlong)param_1 + 0x3dc) == 0.0) {
          uVar1 = *(undefined4 *)(lVar4 + 0xa8);
          lVar4 = FUN_140810240();
          puVar6 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17b,4,uVar1);
          if (puVar6 != (undefined4 *)0x0) {
            *puVar6 = 10;
          }
        }
        *(undefined4 *)((longlong)param_1 + 0x3dc) = *(undefined4 *)(param_1[1] + 300);
      }
    }
    uVar1 = *(undefined4 *)(param_1[2] + 0xa8);
    uVar5 = FUN_140810240();
    FUN_1408103f0(uVar5,0xbb,uVar1);
  }
  return;
}


````

### result

````text

void FUN_140829a10(longlong param_1,char param_2)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 *puVar3;
  uint uStack_c;
  
  lVar2 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xf8));
  if ((lVar2 != 0) && (*(byte *)(lVar2 + 0x368) = *(byte *)(lVar2 + 0x368) & 0xf9, param_2 != '\0'))
  {
    uVar1 = *(undefined4 *)(lVar2 + 0xa8);
    lVar2 = FUN_140810240();
    puVar3 = (undefined8 *)FUN_1408112c0(lVar2 + 0xc8e8,0xb9,0x10,uVar1);
    if (puVar3 != (undefined8 *)0x0) {
      *puVar3 = 0;
      puVar3[1] = (ulonglong)uStack_c << 0x20;
    }
  }
  *(undefined4 *)(param_1 + 0xf8) = 0x3fff0010;
  FUN_1403dab80(param_1 + 0x100,0);
  return;
}


````
