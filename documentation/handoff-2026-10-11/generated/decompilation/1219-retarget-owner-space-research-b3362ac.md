# retarget-owner-space-research.json

Original: [work/natural-moveset/carry-peer-research-v1/retarget-owner-space-research.json](<../../../../work/natural-moveset/carry-peer-research-v1/retarget-owner-space-research.json>)

SHA-256: `A5998FD63FE265D0B7C032FED9A136FBB06F7FCB24689313BE501FE3B7182414`; 11353 bytes.

## Original saved record

````json
{
  "confirmed": [
    "Native Corvo and Emily source carry retain the authored spawner while frame+3f0 links to the local player, and clear the link and records on release.",
    "Both controlled remote Doppelgangers include an initially empty registered arkCpntRetargeting.",
    "Native retargeting record joint fields are interned name IDs, not bone indices. Map names locally.",
    "Eight-second Corvo-peer v1 setup and stop succeeded; strict reciprocal reads observed 58 held samples. Corpse travelled about 2.1 m but retained an approximately 11.5 m offset from the carrier."
  ],
  "hypothesis": [
    "Owner-space flag experiment v2 did not correct the retained 11.5 m offset. Native initial SetTransform in v3 corrected proximity for both characters but did not provide a carried-body pose.",
    "Native victim animation/physics takeover will still be needed even after root following."
  ],
  "functions": {
    "140756210": {
      "result": "\r\nvoid FUN_140756210(undefined8 param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4,\r\n                  undefined8 *param_5,undefined8 param_6,undefined4 param_7)\r\n\r\n{\r\n  longlong lVar1;\r\n  undefined4 *puVar2;\r\n  undefined8 uStack_88;\r\n  undefined8 uStack_80;\r\n  undefined4 uStack_78;\r\n  undefined4 uStack_74;\r\n  undefined4 uStack_70;\r\n  undefined4 uStack_6c;\r\n  undefined8 uStack_48;\r\n  undefined8 uStack_40;\r\n  undefined8 uStack_38;\r\n  undefined8 uStack_30;\r\n  undefined8 uStack_28;\r\n  undefined8 uStack_20;\r\n  undefined8 uStack_18;\r\n  undefined8 uStack_10;\r\n  \r\n  FUN_140756360(param_1,&uStack_48);\r\n  lVar1 = FUN_140878470(param_3);\r\n  if ((*(short *)((longlong)param_2 + 0x1c) == -1) || (lVar1 == 0)) {\r\n    *param_5 = uStack_48;\r\n    param_5[1] = uStack_40;\r\n    param_5[2] = uStack_38;\r\n    param_5[3] = uStack_30;\r\n    param_5[4] = uStack_28;\r\n    param_5[5] = uStack_20;\r\n    param_5[6] = uStack_18;\r\n    param_5[7] = uStack_10;\r\n  }\r\n  else {\r\n    uStack_88 = *param_2;\r\n    uStack_80 = param_2[1];\r\n    uStack_78 = *(undefined4 *)(param_2 + 2);\r\n    uStack_74 = *(undefined4 *)((longlong)param_2 + 0x14);\r\n    uStack_70 = *(undefined4 *)(param_2 + 3);\r\n    uStack_6c = *(undefined4 *)((longlong)param_2 + 0x1c);\r\n    FUN_140755ee0(param_1,&uStack_88,param_3,param_5,param_6,param_7);\r\n    puVar2 = (undefined4 *)func_0x000140009030(&uStack_48,&uStack_88,param_5);\r\n    *(undefined4 *)param_5 = *puVar2;\r\n    *(undefined4 *)((longlong)param_5 + 4) = puVar2[1];\r\n    *(undefined4 *)(param_5 + 1) = puVar2[2];\r\n    *(undefined4 *)((longlong)param_5 + 0xc) = puVar2[3];\r\n    *(undefined4 *)(param_5 + 2) = puVar2[4];\r\n    *(undefined4 *)((longlong)param_5 + 0x14) = puVar2[5];\r\n    *(undefined4 *)(param_5 + 3) = puVar2[6];\r\n    *(undefined4 *)((longlong)param_5 + 0x1c) = puVar2[7];\r\n    *(undefined4 *)(param_5 + 4) = puVar2[8];\r\n    *(undefined4 *)((longlong)param_5 + 0x24) = puVar2[9];\r\n    *(undefined4 *)(param_5 + 5) = puVar2[10];\r\n    *(undefined4 *)((longlong)param_5 + 0x2c) = puVar2[0xb];\r\n    *(undefined4 *)(param_5 + 6) = puVar2[0xc];\r\n    *(undefined4 *)((longlong)param_5 + 0x34) = puVar2[0xd];\r\n    *(undefined4 *)(param_5 + 7) = puVar2[0xe];\r\n    *(undefined4 *)((longlong)param_5 + 0x3c) = puVar2[0xf];\r\n  }\r\n  return;\r\n}\r\n\r\n"
    },
    "140756360": {
      "result": "\r\nundefined8 * FUN_140756360(longlong param_1,undefined8 *param_2,longlong param_3,char param_4)\r\n\r\n{\r\n  float fVar1;\r\n  float fVar2;\r\n  undefined4 uVar3;\r\n  undefined4 uVar4;\r\n  undefined4 uVar5;\r\n  undefined4 uVar6;\r\n  undefined4 uVar7;\r\n  undefined4 uVar8;\r\n  float fVar9;\r\n  undefined4 uVar10;\r\n  float fVar11;\r\n  float fVar12;\r\n  float fVar13;\r\n  undefined4 uVar14;\r\n  undefined4 uVar15;\r\n  undefined4 uVar16;\r\n  longlong lVar17;\r\n  undefined8 *puVar18;\r\n  longlong lVar19;\r\n  float *pfVar20;\r\n  undefined8 uVar21;\r\n  undefined4 *puVar22;\r\n  undefined1 auStack_88 [112];\r\n  \r\n  if ((param_4 == '\\0') || (lVar17 = *(longlong *)(param_3 + 0x358), lVar17 == 0)) {\r\n    uVar3 = *(undefined4 *)(param_3 + 0x324);\r\n    uVar4 = *(undefined4 *)(param_3 + 0x300);\r\n    uVar5 = *(undefined4 *)(param_3 + 0x310);\r\n    uVar6 = *(undefined4 *)(param_3 + 0x328);\r\n    uVar7 = *(undefined4 *)(param_3 + 0x31c);\r\n    uVar8 = *(undefined4 *)(param_3 + 0x314);\r\n    uVar10 = *(undefined4 *)(param_3 + 0x304);\r\n    uVar14 = *(undefined4 *)(param_3 + 0x32c);\r\n    uVar15 = *(undefined4 *)(param_3 + 800);\r\n    uVar16 = *(undefined4 *)(param_3 + 0x308);\r\n    *param_2 = CONCAT44(*(undefined4 *)(param_3 + 0x318),*(undefined4 *)(param_3 + 0x30c));\r\n    param_2[1] = CONCAT44(uVar4,uVar3);\r\n    param_2[2] = CONCAT44(uVar7,uVar5);\r\n    param_2[3] = CONCAT44(uVar10,uVar6);\r\n    param_2[4] = CONCAT44(uVar15,uVar8);\r\n    param_2[5] = CONCAT44(uVar16,uVar14);\r\n    param_2[6] = 0;\r\n    param_2[7] = 0x3f80000000000000;\r\n  }\r\n  else {\r\n    puVar18 = *(undefined8 **)(param_1 + 0x10);\r\n    lVar19 = (**(code **)*puVar18)(puVar18);\r\n    if ((*(int *)(lVar19 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar19 + 0x60))) {\r\n      FUN_1407565c0(param_1,param_2,param_3);\r\n    }\r\n    else {\r\n      lVar19 = FUN_140877720(puVar18,&DAT_1439cb7a0,2);\r\n      pfVar20 = (float *)FUN_140750fe0(lVar17);\r\n      uVar21 = FUN_140750f70(lVar17);\r\n      puVar22 = (undefined4 *)FUN_14000dce0(lVar19 + 0xf18,auStack_88,uVar21);\r\n      fVar1 = *pfVar20;\r\n      fVar2 = pfVar20[1];\r\n      uVar3 = puVar22[1];\r\n      uVar4 = puVar22[6];\r\n      uVar5 = puVar22[7];\r\n      uVar6 = puVar22[4];\r\n      uVar7 = puVar22[5];\r\n      uVar8 = puVar22[2];\r\n      fVar9 = pfVar20[2];\r\n      uVar10 = puVar22[8];\r\n      lVar17 = *(longlong *)(param_1 + 0x58);\r\n      fVar11 = *(float *)(lVar17 + 0x3b8);\r\n      fVar12 = *(float *)(lVar17 + 0x3b0);\r\n      fVar13 = *(float *)(lVar17 + 0x3b4);\r\n      *param_2 = CONCAT44(puVar22[3],*puVar22);\r\n      param_2[1] = CONCAT44(fVar1 + fVar12,uVar4);\r\n      param_2[2] = CONCAT44(uVar6,uVar3);\r\n      param_2[3] = CONCAT44(fVar2 + fVar13,uVar5);\r\n      param_2[4] = CONCAT44(uVar7,uVar8);\r\n      param_2[5] = CONCAT44(fVar9 + fVar11,uVar10);\r\n      param_2[6] = 0;\r\n      param_2[7] = 0x3f80000000000000;\r\n    }\r\n  }\r\n  return param_2;\r\n}\r\n\r\n"
    },
    "140757420": {
      "result": "\r\nvoid FUN_140757420(longlong param_1,int param_2,undefined8 param_3)\r\n\r\n{\r\n  undefined8 uVar1;\r\n  longlong lVar2;\r\n  longlong lVar3;\r\n  undefined4 uStack_28;\r\n  undefined4 uStack_24;\r\n  undefined4 uStack_20;\r\n  undefined4 uStack_1c;\r\n  undefined4 uStack_18;\r\n  undefined4 uStack_14;\r\n  undefined4 uStack_10;\r\n  undefined4 uStack_c;\r\n  \r\n  lVar2 = (longlong)param_2 * 0x100 + *(longlong *)(param_1 + 0x20);\r\n  lVar3 = *(longlong *)(lVar2 + 0xe8);\r\n  if (lVar3 == 0) {\r\n    lVar3 = *(longlong *)(lVar2 + 0xf0);\r\n  }\r\n  uStack_28 = *(undefined4 *)(lVar2 + 0x50);\r\n  uStack_24 = *(undefined4 *)(lVar2 + 0x54);\r\n  uStack_20 = *(undefined4 *)(lVar2 + 0x58);\r\n  uStack_1c = *(undefined4 *)(lVar2 + 0x5c);\r\n  uStack_18 = *(undefined4 *)(lVar2 + 0x60);\r\n  uStack_14 = *(undefined4 *)(lVar2 + 100);\r\n  uStack_10 = *(undefined4 *)(lVar2 + 0x68);\r\n  uStack_c = *(undefined4 *)(lVar2 + 0x6c);\r\n  uVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x90));\r\n  FUN_140756210(param_1,&uStack_28,uVar1,0,param_3,lVar3,*(undefined4 *)(lVar2 + 0xf8));\r\n  return;\r\n}\r\n\r\n"
    },
    "1407570e0": {
      "result": "\r\nvoid FUN_1407570e0(longlong param_1,int param_2,undefined4 *param_3)\r\n\r\n{\r\n  undefined4 uVar1;\r\n  undefined4 uVar2;\r\n  undefined4 uVar3;\r\n  undefined4 uVar4;\r\n  undefined4 uVar5;\r\n  undefined4 uVar6;\r\n  undefined4 uVar7;\r\n  undefined4 uVar8;\r\n  undefined4 uVar9;\r\n  undefined4 uVar10;\r\n  undefined4 uVar11;\r\n  longlong lVar12;\r\n  undefined4 *puVar13;\r\n  longlong lVar14;\r\n  undefined4 auStack_c8 [4];\r\n  undefined8 uStack_b8;\r\n  undefined8 uStack_b0;\r\n  undefined4 uStack_a8;\r\n  undefined4 uStack_a4;\r\n  undefined4 uStack_a0;\r\n  undefined4 uStack_9c;\r\n  undefined4 uStack_98;\r\n  \r\n  lVar14 = (longlong)param_2 * 0x100 + *(longlong *)(param_1 + 0x20);\r\n  if ((*(byte *)(lVar14 + 0xfc) & 1) == 0) {\r\n    lVar12 = FUN_1403da910(param_1 + 0x48,2);\r\n    if (((*(char *)(param_1 + 0x42) == '\\0') && (lVar12 != 0)) &&\r\n       ((*(uint *)(lVar12 + 0x238) & 0xfffffffd) == 0)) {\r\n      FUN_140757420(param_1,param_2,param_3);\r\n    }\r\n    else {\r\n      lVar12 = *(longlong *)(param_1 + 0x68);\r\n      if (lVar12 == 0) {\r\n        lVar12 = *(longlong *)(param_1 + 0x70);\r\n      }\r\n      uStack_b8 = *(undefined8 *)(lVar14 + 0x70);\r\n      uStack_b0 = *(undefined8 *)(lVar14 + 0x78);\r\n      uStack_a8 = *(undefined4 *)(lVar14 + 0x80);\r\n      uStack_a4 = *(undefined4 *)(lVar14 + 0x84);\r\n      uStack_a0 = *(undefined4 *)(lVar14 + 0x88);\r\n      uStack_9c = *(undefined4 *)(lVar14 + 0x8c);\r\n      FUN_140756210(param_1,&uStack_b8,*(undefined8 *)(param_1 + 0x10),1,param_3,lVar12,\r\n                    *(undefined4 *)(param_1 + 0x78));\r\n    }\r\n  }\r\n  else {\r\n    *param_3 = *(undefined4 *)(lVar14 + 0xa0);\r\n    param_3[1] = *(undefined4 *)(lVar14 + 0xa4);\r\n    param_3[2] = *(undefined4 *)(lVar14 + 0xa8);\r\n    param_3[3] = *(undefined4 *)(lVar14 + 0xac);\r\n    param_3[4] = *(undefined4 *)(lVar14 + 0xb0);\r\n    param_3[5] = *(undefined4 *)(lVar14 + 0xb4);\r\n    param_3[6] = *(undefined4 *)(lVar14 + 0xb8);\r\n    param_3[7] = *(undefined4 *)(lVar14 + 0xbc);\r\n    param_3[8] = *(undefined4 *)(lVar14 + 0xc0);\r\n    param_3[9] = *(undefined4 *)(lVar14 + 0xc4);\r\n    param_3[10] = *(undefined4 *)(lVar14 + 200);\r\n    param_3[0xb] = *(undefined4 *)(lVar14 + 0xcc);\r\n    param_3[0xc] = *(undefined4 *)(lVar14 + 0xd0);\r\n    param_3[0xd] = *(undefined4 *)(lVar14 + 0xd4);\r\n    param_3[0xe] = *(undefined4 *)(lVar14 + 0xd8);\r\n    param_3[0xf] = *(undefined4 *)(lVar14 + 0xdc);\r\n  }\r\n  if ((*(byte *)(lVar14 + 0xfc) & 2) != 0) {\r\n    uStack_a0 = param_3[2];\r\n    uStack_a8 = param_3[5];\r\n    uStack_b8 = CONCAT44(param_3[4],*param_3);\r\n    uStack_9c = param_3[6];\r\n    uStack_b0 = CONCAT44(param_3[1],param_3[8]);\r\n    uStack_98 = param_3[10];\r\n    uStack_a4 = param_3[9];\r\n    FUN_14010fc20(&uStack_b8,auStack_c8);\r\n    uVar1 = param_3[3];\r\n    uVar2 = param_3[7];\r\n    auStack_c8[0] = 0;\r\n    uVar3 = param_3[0xb];\r\n    puVar13 = (undefined4 *)FUN_14010d490(auStack_c8,&uStack_b8);\r\n    uVar4 = puVar13[2];\r\n    uVar5 = puVar13[5];\r\n    uVar6 = puVar13[8];\r\n    uVar7 = puVar13[3];\r\n    uVar8 = puVar13[6];\r\n    uVar9 = puVar13[1];\r\n    uVar10 = puVar13[4];\r\n    uVar11 = puVar13[7];\r\n    *param_3 = *puVar13;\r\n    param_3[1] = uVar7;\r\n    param_3[2] = uVar8;\r\n    param_3[3] = uVar1;\r\n    param_3[4] = uVar9;\r\n    param_3[5] = uVar10;\r\n    param_3[6] = uVar11;\r\n    param_3[7] = uVar2;\r\n    param_3[8] = uVar4;\r\n    param_3[9] = uVar5;\r\n    param_3[10] = uVar6;\r\n    param_3[0xb] = uVar3;\r\n    param_3[0xc] = 0;\r\n    param_3[0xd] = 0;\r\n    param_3[0xe] = 0;\r\n    param_3[0xf] = 0x3f800000;\r\n  }\r\n  return;\r\n}\r\n\r\n"
    }
  }
}
````

## Unescaped text fields for reading

### result

````text

void FUN_140756210(undefined8 param_1,undefined8 *param_2,undefined8 param_3,undefined8 param_4,
                  undefined8 *param_5,undefined8 param_6,undefined4 param_7)

{
  longlong lVar1;
  undefined4 *puVar2;
  undefined8 uStack_88;
  undefined8 uStack_80;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  FUN_140756360(param_1,&uStack_48);
  lVar1 = FUN_140878470(param_3);
  if ((*(short *)((longlong)param_2 + 0x1c) == -1) || (lVar1 == 0)) {
    *param_5 = uStack_48;
    param_5[1] = uStack_40;
    param_5[2] = uStack_38;
    param_5[3] = uStack_30;
    param_5[4] = uStack_28;
    param_5[5] = uStack_20;
    param_5[6] = uStack_18;
    param_5[7] = uStack_10;
  }
  else {
    uStack_88 = *param_2;
    uStack_80 = param_2[1];
    uStack_78 = *(undefined4 *)(param_2 + 2);
    uStack_74 = *(undefined4 *)((longlong)param_2 + 0x14);
    uStack_70 = *(undefined4 *)(param_2 + 3);
    uStack_6c = *(undefined4 *)((longlong)param_2 + 0x1c);
    FUN_140755ee0(param_1,&uStack_88,param_3,param_5,param_6,param_7);
    puVar2 = (undefined4 *)func_0x000140009030(&uStack_48,&uStack_88,param_5);
    *(undefined4 *)param_5 = *puVar2;
    *(undefined4 *)((longlong)param_5 + 4) = puVar2[1];
    *(undefined4 *)(param_5 + 1) = puVar2[2];
    *(undefined4 *)((longlong)param_5 + 0xc) = puVar2[3];
    *(undefined4 *)(param_5 + 2) = puVar2[4];
    *(undefined4 *)((longlong)param_5 + 0x14) = puVar2[5];
    *(undefined4 *)(param_5 + 3) = puVar2[6];
    *(undefined4 *)((longlong)param_5 + 0x1c) = puVar2[7];
    *(undefined4 *)(param_5 + 4) = puVar2[8];
    *(undefined4 *)((longlong)param_5 + 0x24) = puVar2[9];
    *(undefined4 *)(param_5 + 5) = puVar2[10];
    *(undefined4 *)((longlong)param_5 + 0x2c) = puVar2[0xb];
    *(undefined4 *)(param_5 + 6) = puVar2[0xc];
    *(undefined4 *)((longlong)param_5 + 0x34) = puVar2[0xd];
    *(undefined4 *)(param_5 + 7) = puVar2[0xe];
    *(undefined4 *)((longlong)param_5 + 0x3c) = puVar2[0xf];
  }
  return;
}


````

### result

````text

undefined8 * FUN_140756360(longlong param_1,undefined8 *param_2,longlong param_3,char param_4)

{
  float fVar1;
  float fVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  float fVar9;
  undefined4 uVar10;
  float fVar11;
  float fVar12;
  float fVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  longlong lVar17;
  undefined8 *puVar18;
  longlong lVar19;
  float *pfVar20;
  undefined8 uVar21;
  undefined4 *puVar22;
  undefined1 auStack_88 [112];
  
  if ((param_4 == '\0') || (lVar17 = *(longlong *)(param_3 + 0x358), lVar17 == 0)) {
    uVar3 = *(undefined4 *)(param_3 + 0x324);
    uVar4 = *(undefined4 *)(param_3 + 0x300);
    uVar5 = *(undefined4 *)(param_3 + 0x310);
    uVar6 = *(undefined4 *)(param_3 + 0x328);
    uVar7 = *(undefined4 *)(param_3 + 0x31c);
    uVar8 = *(undefined4 *)(param_3 + 0x314);
    uVar10 = *(undefined4 *)(param_3 + 0x304);
    uVar14 = *(undefined4 *)(param_3 + 0x32c);
    uVar15 = *(undefined4 *)(param_3 + 800);
    uVar16 = *(undefined4 *)(param_3 + 0x308);
    *param_2 = CONCAT44(*(undefined4 *)(param_3 + 0x318),*(undefined4 *)(param_3 + 0x30c));
    param_2[1] = CONCAT44(uVar4,uVar3);
    param_2[2] = CONCAT44(uVar7,uVar5);
    param_2[3] = CONCAT44(uVar10,uVar6);
    param_2[4] = CONCAT44(uVar15,uVar8);
    param_2[5] = CONCAT44(uVar16,uVar14);
    param_2[6] = 0;
    param_2[7] = 0x3f80000000000000;
  }
  else {
    puVar18 = *(undefined8 **)(param_1 + 0x10);
    lVar19 = (**(code **)*puVar18)(puVar18);
    if ((*(int *)(lVar19 + 0x60) < DAT_1439ce350) || (DAT_1439ce354 < *(int *)(lVar19 + 0x60))) {
      FUN_1407565c0(param_1,param_2,param_3);
    }
    else {
      lVar19 = FUN_140877720(puVar18,&DAT_1439cb7a0,2);
      pfVar20 = (float *)FUN_140750fe0(lVar17);
      uVar21 = FUN_140750f70(lVar17);
      puVar22 = (undefined4 *)FUN_14000dce0(lVar19 + 0xf18,auStack_88,uVar21);
      fVar1 = *pfVar20;
      fVar2 = pfVar20[1];
      uVar3 = puVar22[1];
      uVar4 = puVar22[6];
      uVar5 = puVar22[7];
      uVar6 = puVar22[4];
      uVar7 = puVar22[5];
      uVar8 = puVar22[2];
      fVar9 = pfVar20[2];
      uVar10 = puVar22[8];
      lVar17 = *(longlong *)(param_1 + 0x58);
      fVar11 = *(float *)(lVar17 + 0x3b8);
      fVar12 = *(float *)(lVar17 + 0x3b0);
      fVar13 = *(float *)(lVar17 + 0x3b4);
      *param_2 = CONCAT44(puVar22[3],*puVar22);
      param_2[1] = CONCAT44(fVar1 + fVar12,uVar4);
      param_2[2] = CONCAT44(uVar6,uVar3);
      param_2[3] = CONCAT44(fVar2 + fVar13,uVar5);
      param_2[4] = CONCAT44(uVar7,uVar8);
      param_2[5] = CONCAT44(fVar9 + fVar11,uVar10);
      param_2[6] = 0;
      param_2[7] = 0x3f80000000000000;
    }
  }
  return param_2;
}


````

### result

````text

void FUN_140757420(longlong param_1,int param_2,undefined8 param_3)

{
  undefined8 uVar1;
  longlong lVar2;
  longlong lVar3;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  
  lVar2 = (longlong)param_2 * 0x100 + *(longlong *)(param_1 + 0x20);
  lVar3 = *(longlong *)(lVar2 + 0xe8);
  if (lVar3 == 0) {
    lVar3 = *(longlong *)(lVar2 + 0xf0);
  }
  uStack_28 = *(undefined4 *)(lVar2 + 0x50);
  uStack_24 = *(undefined4 *)(lVar2 + 0x54);
  uStack_20 = *(undefined4 *)(lVar2 + 0x58);
  uStack_1c = *(undefined4 *)(lVar2 + 0x5c);
  uStack_18 = *(undefined4 *)(lVar2 + 0x60);
  uStack_14 = *(undefined4 *)(lVar2 + 100);
  uStack_10 = *(undefined4 *)(lVar2 + 0x68);
  uStack_c = *(undefined4 *)(lVar2 + 0x6c);
  uVar1 = FUN_1406dc560(*(undefined4 *)(lVar2 + 0x90));
  FUN_140756210(param_1,&uStack_28,uVar1,0,param_3,lVar3,*(undefined4 *)(lVar2 + 0xf8));
  return;
}


````

### result

````text

void FUN_1407570e0(longlong param_1,int param_2,undefined4 *param_3)

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
  undefined4 *puVar13;
  longlong lVar14;
  undefined4 auStack_c8 [4];
  undefined8 uStack_b8;
  undefined8 uStack_b0;
  undefined4 uStack_a8;
  undefined4 uStack_a4;
  undefined4 uStack_a0;
  undefined4 uStack_9c;
  undefined4 uStack_98;
  
  lVar14 = (longlong)param_2 * 0x100 + *(longlong *)(param_1 + 0x20);
  if ((*(byte *)(lVar14 + 0xfc) & 1) == 0) {
    lVar12 = FUN_1403da910(param_1 + 0x48,2);
    if (((*(char *)(param_1 + 0x42) == '\0') && (lVar12 != 0)) &&
       ((*(uint *)(lVar12 + 0x238) & 0xfffffffd) == 0)) {
      FUN_140757420(param_1,param_2,param_3);
    }
    else {
      lVar12 = *(longlong *)(param_1 + 0x68);
      if (lVar12 == 0) {
        lVar12 = *(longlong *)(param_1 + 0x70);
      }
      uStack_b8 = *(undefined8 *)(lVar14 + 0x70);
      uStack_b0 = *(undefined8 *)(lVar14 + 0x78);
      uStack_a8 = *(undefined4 *)(lVar14 + 0x80);
      uStack_a4 = *(undefined4 *)(lVar14 + 0x84);
      uStack_a0 = *(undefined4 *)(lVar14 + 0x88);
      uStack_9c = *(undefined4 *)(lVar14 + 0x8c);
      FUN_140756210(param_1,&uStack_b8,*(undefined8 *)(param_1 + 0x10),1,param_3,lVar12,
                    *(undefined4 *)(param_1 + 0x78));
    }
  }
  else {
    *param_3 = *(undefined4 *)(lVar14 + 0xa0);
    param_3[1] = *(undefined4 *)(lVar14 + 0xa4);
    param_3[2] = *(undefined4 *)(lVar14 + 0xa8);
    param_3[3] = *(undefined4 *)(lVar14 + 0xac);
    param_3[4] = *(undefined4 *)(lVar14 + 0xb0);
    param_3[5] = *(undefined4 *)(lVar14 + 0xb4);
    param_3[6] = *(undefined4 *)(lVar14 + 0xb8);
    param_3[7] = *(undefined4 *)(lVar14 + 0xbc);
    param_3[8] = *(undefined4 *)(lVar14 + 0xc0);
    param_3[9] = *(undefined4 *)(lVar14 + 0xc4);
    param_3[10] = *(undefined4 *)(lVar14 + 200);
    param_3[0xb] = *(undefined4 *)(lVar14 + 0xcc);
    param_3[0xc] = *(undefined4 *)(lVar14 + 0xd0);
    param_3[0xd] = *(undefined4 *)(lVar14 + 0xd4);
    param_3[0xe] = *(undefined4 *)(lVar14 + 0xd8);
    param_3[0xf] = *(undefined4 *)(lVar14 + 0xdc);
  }
  if ((*(byte *)(lVar14 + 0xfc) & 2) != 0) {
    uStack_a0 = param_3[2];
    uStack_a8 = param_3[5];
    uStack_b8 = CONCAT44(param_3[4],*param_3);
    uStack_9c = param_3[6];
    uStack_b0 = CONCAT44(param_3[1],param_3[8]);
    uStack_98 = param_3[10];
    uStack_a4 = param_3[9];
    FUN_14010fc20(&uStack_b8,auStack_c8);
    uVar1 = param_3[3];
    uVar2 = param_3[7];
    auStack_c8[0] = 0;
    uVar3 = param_3[0xb];
    puVar13 = (undefined4 *)FUN_14010d490(auStack_c8,&uStack_b8);
    uVar4 = puVar13[2];
    uVar5 = puVar13[5];
    uVar6 = puVar13[8];
    uVar7 = puVar13[3];
    uVar8 = puVar13[6];
    uVar9 = puVar13[1];
    uVar10 = puVar13[4];
    uVar11 = puVar13[7];
    *param_3 = *puVar13;
    param_3[1] = uVar7;
    param_3[2] = uVar8;
    param_3[3] = uVar1;
    param_3[4] = uVar9;
    param_3[5] = uVar10;
    param_3[6] = uVar11;
    param_3[7] = uVar2;
    param_3[8] = uVar4;
    param_3[9] = uVar5;
    param_3[10] = uVar6;
    param_3[0xb] = uVar3;
    param_3[0xc] = 0;
    param_3[0xd] = 0;
    param_3[0xe] = 0;
    param_3[0xf] = 0x3f800000;
  }
  return;
}


````
