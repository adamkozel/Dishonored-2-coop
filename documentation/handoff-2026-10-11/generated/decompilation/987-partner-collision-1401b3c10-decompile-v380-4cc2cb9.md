# partner-collision-1401b3c10-decompile-v380.txt

Original: [work/movement-integration/partner-collision-1401b3c10-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-1401b3c10-decompile-v380.txt>)

SHA-256: `83DB6EDB36E173A2469B2C49F23F56AB44ACFA6E8C39B035E98418D00EED93C2`; 1710 bytes.

## Original saved record

````text

void FUN_1401b3c10(longlong param_1,uint param_2,longlong param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  longlong lVar3;
  longlong lVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  char cVar9;
  uint uVar10;
  longlong lVar11;
  longlong lVar12;
  undefined4 *puVar13;
  
  lVar3 = *(longlong *)(param_1 + 0x148);
  lVar12 = (longlong)(int)param_2;
  if ((int)(*(uint *)(param_1 + 0x20c) >> 8) < (int)param_2) {
    cVar9 = FUN_1401b5c70((longlong *)(param_1 + 0x200));
    if (cVar9 == '\0') goto LAB_1401b3c6a;
  }
  *(uint *)(param_1 + 0x208) = *(uint *)(param_1 + 0x208) & 0xff000000;
  uVar10 = *(uint *)(param_1 + 0x20c) >> 8;
  if ((int)param_2 < (int)uVar10) {
    uVar10 = param_2;
  }
  *(uint *)(param_1 + 0x208) = *(uint *)(param_1 + 0x208) | uVar10 & 0xffffff;
LAB_1401b3c6a:
  if (0 < (int)param_2) {
    lVar11 = 0;
    puVar13 = (undefined4 *)(param_3 + 0x18);
    do {
      lVar4 = *(longlong *)(param_1 + 0x200);
      *(undefined4 *)(lVar11 + lVar4) = puVar13[4];
      fVar5 = (float)puVar13[-5];
      fVar6 = (float)puVar13[-4];
      fVar7 = *(float *)(lVar3 + 0x84);
      fVar8 = *(float *)(lVar3 + 0x88);
      *(float *)(lVar11 + 4 + lVar4) = (float)puVar13[-6] - *(float *)(lVar3 + 0x80);
      *(float *)(lVar11 + 0xc + lVar4) = fVar6 - fVar8;
      *(float *)(lVar11 + 8 + lVar4) = fVar5 - fVar7;
      uVar1 = *puVar13;
      uVar2 = puVar13[-1];
      *(undefined4 *)(lVar11 + 0x10 + lVar4) = puVar13[-2];
      *(undefined4 *)(lVar11 + 0x14 + lVar4) = uVar2;
      *(undefined4 *)(lVar11 + 0x18 + lVar4) = uVar1;
      lVar12 = lVar12 + -1;
      lVar11 = lVar11 + 0x1c;
      puVar13 = puVar13 + 0x10;
    } while (lVar12 != 0);
  }
  return;
}



````
