
void FUN_140865c50(longlong param_1)

{
  float fVar1;
  int iVar2;
  undefined4 uVar3;
  longlong lVar4;
  longlong lVar5;
  char cVar6;
  longlong lVar7;
  longlong lVar8;
  longlong lVar9;
  undefined8 *puVar10;
  float *pfVar11;
  longlong lVar12;
  undefined4 *puVar13;
  float fVar14;
  float fVar15;
  uint local_res8 [2];
  undefined8 local_res10;
  undefined8 local_98;
  float local_90;
  undefined4 uStack_8c;
  undefined8 local_88 [10];
  
  FUN_14083ebc0();
  lVar7 = FUN_140854880(param_1);
  lVar12 = *(longlong *)(param_1 + 0x10);
  lVar4 = *(longlong *)(param_1 + 0x18);
  fVar1 = *(float *)(param_1 + 0x88);
  lVar9 = *(longlong *)(lVar12 + 0x308);
  lVar5 = *(longlong *)(lVar12 + 0x2f8);
  local_res10 = *(undefined8 *)(lVar12 + 0x300);
  puVar13 = (undefined4 *)(lVar7 + 0x300);
  *(float *)(param_1 + 0x88) = (*(float *)(param_1 + 0x94) - *(float *)(lVar7 + 0x308)) + fVar1;
  *(undefined4 *)(param_1 + 0x8c) = *puVar13;
  *(undefined4 *)(param_1 + 0x90) = *(undefined4 *)(lVar7 + 0x304);
  *(undefined4 *)(param_1 + 0x94) = *(undefined4 *)(lVar7 + 0x308);
  if (((3.4028235e+38 < *(float *)(lVar9 + 0x2b0) || *(float *)(lVar9 + 0x2b0) == 3.4028235e+38) &&
      (lVar8 = FUN_1407fdaa0(lVar9), *(int *)(lVar8 + 4) == 0)) ||
     (*(char *)(lVar5 + 0x188) != '\0')) {
    if (*(char *)(param_1 + 0x6b) != '\0') {
      puVar10 = (undefined8 *)FUN_1407fda90(lVar9);
      local_98 = *puVar10;
      if (0.0 < *(float *)(puVar10 + 1)) {
        local_90 = *(float *)(puVar10 + 1) * *(float *)(lVar4 + 0x188);
        FUN_1407ff080(lVar9,&local_98);
      }
    }
    fVar15 = *(float *)(lVar7 + 0x344);
    fVar14 = *(float *)(*(longlong *)(lVar9 + 0x48) + 0xc0);
    pfVar11 = (float *)FUN_140877eb0(lVar7,local_res8);
    *(float *)(lVar9 + 0x2ac) = fVar14 * ABS(fVar15) * *pfVar11 + *(float *)(lVar4 + 0xac);
    fVar15 = *(float *)(lVar4 + 0x164);
    iVar2 = *(int *)(param_1 + 0xa4);
    if ((-1 < iVar2) &&
       (lVar12 = *(longlong *)(*(longlong *)(lVar12 + 0x330) + 0x48),
       *(float *)(lVar12 + 0x24 + (longlong)iVar2 * 0x28) !=
       *(float *)(lVar12 + 8 + (longlong)iVar2 * 0x28))) {
      fVar15 = *(float *)(lVar4 + 0x168);
    }
    if ((*(char *)(param_1 + 0xa1) == '\0') &&
       (fVar14 = (float)FUN_1408657c0(param_1,*(float *)(lVar9 + 0x2f8) + *(float *)(param_1 + 0x88)
                                     ), 0.0 < fVar14)) {
      uVar3 = *(undefined4 *)(lVar7 + 0xa8);
      lVar12 = FUN_140810240();
      puVar13 = (undefined4 *)FUN_1408112c0(lVar12 + 0xc8e8,0x17b,4,uVar3);
      if (puVar13 != (undefined4 *)0x0) {
        *puVar13 = 5;
      }
      *(undefined1 *)(param_1 + 0xa1) = 1;
    }
    if ((fVar1 < fVar15) && (fVar15 <= *(float *)(param_1 + 0x88))) {
      local_90 = 1.16308e-43;
      local_98 = DAT_143995fb0;
      FUN_140131ae0(local_88);
      uVar3 = *(undefined4 *)(lVar7 + 0xa8);
      lVar12 = FUN_140810240();
      puVar10 = (undefined8 *)FUN_1408112c0(lVar12 + 0xc8e8,0x4f,0x18,uVar3);
      if (puVar10 != (undefined8 *)0x0) {
        *puVar10 = local_98;
        puVar10[1] = CONCAT44(uStack_8c,local_90);
        puVar10[2] = local_88[0];
      }
    }
    goto LAB_140866136;
  }
  cVar6 = FUN_1408658c0(param_1);
  if (cVar6 != '\0') goto LAB_140866136;
  FUN_140854190(param_1);
  local_res8[0] = CONCAT31(local_res8[0]._1_3_,1);
  local_90 = SQRT(*(float *)(lVar7 + 0x33c) * *(float *)(lVar7 + 0x33c) +
                  *(float *)(lVar7 + 0x340) * *(float *)(lVar7 + 0x340));
  DAT_143994a30 = DAT_143994a30 + 1 & 0x7fffffff;
  if (DAT_143994a30 == 0) {
    DAT_143994a30 = 1;
  }
  lVar7 = 0;
  lVar8 = 0;
  local_98 = CONCAT44(7,DAT_143994a30);
  lVar9 = lVar7;
  if (*(longlong *)(lVar12 + 0x90) != 0) {
    lVar9 = *(longlong *)(*(longlong *)(lVar12 + 0x90) + 0x70);
  }
  if (((*(char *)(param_1 + 0x6a) == '\0') || (*(char *)(param_1 + 0x6b) != '\0')) ||
     ((*(float *)(lVar4 + 0x174) < *(float *)(param_1 + 0x88) ||
       *(float *)(lVar4 + 0x174) == *(float *)(param_1 + 0x88) ||
      (cVar6 = FUN_1408034b0(lVar12,7,&local_98), cVar6 == '\0')))) {
    if ((((*(byte *)(param_1 + 0x6b) & *(byte *)(param_1 + 0x69)) == 0 &&
          *(char *)(param_1 + 0x6d) == '\0') ||
        ((*(ulonglong *)(lVar5 + 0x80) &
         1L << ((byte)*(undefined4 *)(param_1 + 0x98) & 0x3f) & *(ulonglong *)(lVar5 + 0x98)) == 0))
       || (cVar6 = FUN_140803400(lVar12,7), cVar6 == '\0')) {
      if ((*(ulonglong *)(lVar5 + 0xa0) >> ((ulonglong)*(uint *)(lVar5 + 0x6c) & 0x3f) & 1) == 0) {
        if ((*(char *)(param_1 + 0x69) == '\0') &&
           ((*(char *)(param_1 + 0x6c) == '\0' || ((*(byte *)(lVar9 + 0xa4) & 4) == 0)))) {
          FUN_1408022e0(lVar12,0);
          lVar7 = lVar8;
          if (*(longlong *)(lVar12 + 0x90) != 0) {
            lVar9 = *(longlong *)(*(longlong *)(lVar12 + 0x90) + 0x10);
LAB_140865ee3:
            lVar7 = lVar8;
            if (lVar9 != 0) {
              lVar7 = *(longlong *)(lVar9 + 0x18);
            }
          }
        }
        else {
          FUN_1408022e0(lVar12,1);
          lVar7 = lVar8;
          if (*(longlong *)(lVar12 + 0x90) != 0) {
            lVar9 = *(longlong *)(*(longlong *)(lVar12 + 0x90) + 0x18);
            goto LAB_140865ee3;
          }
        }
      }
      else {
        FUN_1408022e0(lVar12,6);
        if (*(longlong *)(lVar12 + 0x90) != 0) {
          lVar9 = *(longlong *)(*(longlong *)(lVar12 + 0x90) + 0x40);
          goto LAB_140865ee3;
        }
      }
    }
    else {
      FUN_1407fb2f0(lVar5);
      *(ulonglong *)(lVar5 + 0xa0) =
           *(ulonglong *)(lVar5 + 0xa0) &
           ~(1L << ((byte)*(undefined4 *)(lVar5 + 0x6c) & 0x3f) & *(ulonglong *)(lVar5 + 0x80));
      FUN_1408022e0(lVar12,7);
      local_res8[0] = local_res8[0] & 0xffffff00;
    }
  }
  else {
    FUN_1408024d0(lVar12,7,&local_98);
  }
  FUN_140775270(local_res10,
                CONCAT31((int3)(*(uint *)(lVar5 + 0x6c) >> 8),
                         (*(ulonglong *)(lVar5 + 0xa0) >>
                          ((ulonglong)*(uint *)(lVar5 + 0x6c) & 0x3f) & 1) != 0),0);
  if ((char)local_res8[0] != '\0') {
    lVar12 = *(longlong *)(lVar12 + 0x300);
    local_res8[0] = 0;
    *(undefined4 *)(lVar12 + 0xc90) = 0;
    FUN_140770990(lVar12,puVar13,*(undefined8 *)(lVar4 + 400));
    if ((lVar7 != 0) && (*(longlong *)(lVar7 + 200) != 0)) {
      FUN_140770990(lVar12,puVar13);
    }
  }
LAB_140866136:
  FUN_140866160(param_1);
  return;
}


