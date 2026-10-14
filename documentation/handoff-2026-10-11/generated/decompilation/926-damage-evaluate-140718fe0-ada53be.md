# damage-evaluate-140718fe0.txt

Original: [work/movement-integration/npc-combat-research-v850/damage-evaluate-140718fe0.txt](<../../../../work/movement-integration/npc-combat-research-v850/damage-evaluate-140718fe0.txt>)

SHA-256: `6C20684CB04BAF789601D9312EE9FE5DB024BF29F819C13BD25470AC4C8A4122`; 6051 bytes.

## Original saved record

````text

void FUN_140718fe0(longlong param_1,longlong param_2,float *param_3)

{
  longlong *plVar1;
  ulonglong uVar2;
  char cVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined8 uVar6;
  longlong lVar7;
  undefined8 *puVar8;
  longlong lVar9;
  double *pdVar10;
  uint uVar11;
  ulonglong uVar12;
  ulonglong uVar13;
  float fVar14;
  float fVar15;
  float fVar16;
  float fVar17;
  float fVar18;
  float afStackX_8 [2];
  undefined1 auStackX_10 [8];
  
  uVar12 = 0;
  afStackX_8[0] = 0.0;
  *(byte *)(param_3 + 6) = *(byte *)(param_3 + 6) | 8;
  FUN_140717800();
  *(byte *)(param_3 + 6) = *(byte *)(param_3 + 6) & 0xf7;
  fVar18 = *param_3;
  fVar17 = 0.0;
  uVar2 = *(ulonglong *)(param_2 + 0x10);
  if ((((byte)*(undefined4 *)(param_1 + 0x1c) & 0xc) < 8) ||
     ((*(byte *)(*(longlong *)(param_1 + 0x10) + 0xd1) & 8) != 0)) {
LAB_140719084:
    fVar18 = 0.0;
  }
  else {
    puVar5 = (undefined4 *)FUN_14039eec0(param_2,auStackX_10);
    uVar6 = FUN_1406dc560(*puVar5);
    cVar3 = FUN_14071a390(param_1 + 0x98,uVar6,*(undefined8 *)(param_2 + 0x10));
    if (cVar3 != '\0') goto LAB_140719084;
  }
  afStackX_8[0] = 1.0;
  if ((*(byte *)(param_2 + 0x10) >> 2 & 1) == 0) {
    uVar13 = uVar12;
    if ((*(uint *)(param_1 + 0x90) & 0xffffff) != 0) {
      do {
        FUN_140435200(*(undefined8 *)(uVar13 + *(longlong *)(param_1 + 0x88)),param_2,param_3,
                      afStackX_8);
        uVar11 = (int)uVar12 + 1;
        uVar12 = (ulonglong)uVar11;
        uVar13 = uVar13 + 8;
      } while ((int)uVar11 < (int)(*(uint *)(param_1 + 0x90) & 0xffffff));
    }
    if (param_3[2] == 0.0) {
      FUN_140719d20(param_1,param_2,afStackX_8);
    }
    if (*(longlong *)(param_1 + 0x68) != 0) {
      if ((uVar2 >> 10 & 1) != 0) {
        if (*(int *)(param_1 + 200) < 0) {
          fVar16 = 1.0;
        }
        else {
          fVar16 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x48) + 0x24 +
                             (longlong)*(int *)(param_1 + 200) * 0x28);
        }
        afStackX_8[0] = afStackX_8[0] + (fVar16 - 1.0);
        lVar7 = FUN_1406dc560(*(undefined4 *)(param_2 + 0x50));
        if ((lVar7 != 0) && (lVar7 = FUN_140877720(lVar7,&DAT_1439e28e0,2), lVar7 != 0)) {
          puVar5 = (undefined4 *)FUN_14039eec0(param_2,auStackX_10);
          puVar8 = (undefined8 *)FUN_1406dc560(*puVar5);
          lVar7 = FUN_1408a8ba0(D2_GameSingletonPtr);
          lVar7 = *(longlong *)(lVar7 + 0x408);
          if (puVar8 != (undefined8 *)0x0) {
            lVar9 = (**(code **)*puVar8)(puVar8);
            if ((*(int *)(lVar9 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar9 + 0x60))
               ) {
              lVar9 = (**(code **)*puVar8)(puVar8);
              if ((*(int *)(lVar9 + 0x60) < DAT_1439ce350) ||
                 (DAT_1439ce354 < *(int *)(lVar9 + 0x60))) goto LAB_140719248;
              iVar4 = *(int *)(lVar7 + 0x78);
            }
            else {
              iVar4 = *(int *)(lVar7 + 0x7c);
            }
            if (iVar4 < 0) {
              fVar16 = 1.0;
            }
            else {
              fVar16 = *(float *)(*(longlong *)(lVar7 + 0x48) + 0x24 + (longlong)iVar4 * 0x28);
            }
            afStackX_8[0] = afStackX_8[0] + (fVar16 - 1.0);
          }
        }
      }
LAB_140719248:
      if (*(int *)(param_1 + 0xc4) < 0) {
        fVar16 = 1.0;
      }
      else {
        fVar16 = *(float *)(*(longlong *)(*(longlong *)(param_1 + 0x68) + 0x48) + 0x24 +
                           (longlong)*(int *)(param_1 + 0xc4) * 0x28);
      }
      afStackX_8[0] = afStackX_8[0] + (fVar16 - 1.0);
    }
  }
  fVar18 = fVar18 * afStackX_8[0];
  if (0 < *(int *)(param_1 + 0x100)) {
    fVar18 = 0.0;
  }
  lVar7 = *(longlong *)(param_1 + 0x10);
  lVar9 = FUN_1408a8ba0(D2_GameSingletonPtr);
  if ((lVar9 == lVar7) && ((*(byte *)(param_2 + 0x10) >> 2 & 1) == 0)) {
    cVar3 = FUN_140399620(D2_GameSingletonPtr + 0x3ef72);
    if (cVar3 == '\0') {
      lVar7 = FUN_1408a8ba0(D2_GameSingletonPtr);
      iVar4 = *(int *)(*(longlong *)(lVar7 + 0x408) + 0x108);
      if (iVar4 < 0) {
        fVar16 = 0.0;
      }
      else {
        fVar16 = *(float *)(*(longlong *)(*(longlong *)(lVar7 + 0x408) + 0x48) + 0x24 +
                           (longlong)iVar4 * 0x28);
      }
      if (fVar16 <= 0.0) goto LAB_140719364;
    }
    else {
      fVar16 = 1.0;
    }
    lVar7 = FUN_140877720(*(undefined8 *)(param_1 + 0x10),&DAT_1439d13b0,2);
    if (lVar7 != 0) {
      fVar14 = (float)FUN_140745f60(lVar7);
      fVar15 = fVar16 * fVar18;
      if (fVar14 <= fVar16 * fVar18) {
        fVar15 = fVar14;
      }
      param_3[1] = fVar15;
      fVar18 = fVar18 - fVar15;
    }
  }
LAB_140719364:
  puVar8 = *(undefined8 **)(param_1 + 0x10);
  fVar16 = *(float *)(param_1 + 0x20);
  if (((uVar2 & 1) == 0) && ((uVar2 & 2) == 0)) {
LAB_140719398:
    if ((uVar2 & 0x100000000000) == 0) {
      pdVar10 = (double *)
                (**(code **)(*D2_GameSingletonPtr + 0xd0))(D2_GameSingletonPtr,auStackX_10,3);
      if (*(double *)(param_1 + 0x148) < *pdVar10 || *(double *)(param_1 + 0x148) == *pdVar10)
      goto LAB_140719432;
      puVar5 = (undefined4 *)FUN_14039eec0(param_2,auStackX_10);
      puVar8 = (undefined8 *)FUN_1406dc560(*puVar5);
      if (puVar8 == (undefined8 *)0x0) goto LAB_140719432;
      lVar7 = (**(code **)*puVar8)(puVar8);
      if ((*(int *)(lVar7 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar7 + 0x60)))
      goto LAB_140719432;
    }
    else {
      plVar1 = D2_GameSingletonPtr + 0x3ef77;
      uVar6 = FUN_1408a8ba0();
      iVar4 = FUN_140604bd0(plVar1,puVar8,uVar6);
      if (iVar4 != 0) goto LAB_140719432;
      *(byte *)(param_3 + 6) = *(byte *)(param_3 + 6) | 4;
    }
  }
  else {
    lVar7 = (**(code **)*puVar8)(puVar8);
    if ((*(int *)(lVar7 + 0x60) < DAT_1439ce2c0) || (DAT_1439ce2c4 < *(int *)(lVar7 + 0x60)))
    goto LAB_140719398;
  }
  fVar17 = 1.0;
LAB_140719432:
  if (fVar17 <= fVar16 - fVar18) {
    *param_3 = fVar18;
  }
  else {
    *param_3 = fVar16 - fVar17;
  }
  return;
}



````
