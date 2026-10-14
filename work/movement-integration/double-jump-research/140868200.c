void FUN_140868200(longlong param_1)

{
  float fVar1;
  longlong lVar2;
  char cVar3;
  longlong lVar4;
  undefined4 *puVar5;
  longlong lVar6;
  longlong lVar7;
  float *pfVar8;
  undefined8 uVar9;
  undefined1 auStackX_8 [8];
  undefined4 uStack_28;
  undefined4 uStack_24;
  float fStack_20;
  
  FUN_14083ebc0();
  lVar6 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2f8);
  lVar4 = FUN_140854880(param_1);
  lVar7 = *(longlong *)(param_1 + 0x18);
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x308);
  cVar3 = FUN_1407fdc10(lVar2);
  if (cVar3 == '\0') {
    lVar7 = FUN_1407fdaa0(lVar2);
    if (*(int *)(lVar7 + 4) == 0) {
      if ((0 < *(int *)(param_1 + 0x94)) &&
         ((*(ulonglong *)(lVar6 + 0x80) &
          1L << ((byte)*(undefined4 *)(param_1 + 0x88) & 0x3f) & *(ulonglong *)(lVar6 + 0x98)) != 0)
         ) {
        FUN_1408680a0(param_1);
        goto LAB_140868474;
      }
      goto LAB_1408683aa;
    }
    FUN_140854190(param_1);
    if (*(char *)(param_1 + 0x68) == '\0') {
      if ((*(ulonglong *)(lVar6 + 0xa0) >> ((ulonglong)*(uint *)(lVar6 + 0x6c) & 0x3f) & 1) != 0) {
        uVar9 = 6;
        goto LAB_14086846b;
      }
    }
    else if ((*(ulonglong *)(lVar6 + 0xa0) >> ((ulonglong)*(uint *)(lVar6 + 0x6c) & 0x3f) & 1) == 0)
    goto LAB_140868469;
    if (*(char *)(param_1 + 0x69) == '\0') goto LAB_140868469;
    uVar9 = 1;
  }
  else {
    if (((0 < *(int *)(param_1 + 0x94)) &&
        ((*(ulonglong *)(lVar6 + 0x80) &
         1L << ((byte)*(undefined4 *)(param_1 + 0x88) & 0x3f) & *(ulonglong *)(lVar6 + 0x98)) != 0))
       && (*(float *)(lVar7 + 0x170) <= *(float *)(param_1 + 0x9c))) {
      FUN_1408680a0(param_1);
    }
    cVar3 = FUN_1407fdc30(lVar2,*(undefined4 *)(lVar7 + 0x158));
    if (cVar3 == '\0') {
      fVar1 = *(float *)(param_1 + 0x8c);
      if ((*(float *)(param_1 + 0x90) <= fVar1) ||
         ((*(ulonglong *)(lVar6 + 0x80) &
          1L << ((byte)*(undefined4 *)(param_1 + 0x88) & 0x3f) & *(ulonglong *)(lVar6 + 0x98)) != 0)
         ) goto LAB_140868474;
      if (*(float *)(lVar4 + 0x308) - *(float *)(param_1 + 0xa0) <= fVar1) {
        FUN_1407fdca0(lVar2,*(float *)(param_1 + 0xa0) + fVar1,*(undefined4 *)(lVar7 + 0x174),
                      *(undefined4 *)(param_1 + 0xac));
        goto LAB_140868474;
      }
      FUN_1407ff1c0();
LAB_1408683aa:
      FUN_140854190(param_1);
      *(undefined1 *)(param_1 + 0xb9) = 1;
      uVar9 = 3;
    }
    else {
      puVar5 = (undefined4 *)FUN_1407fda90(lVar2);
      uStack_28 = *puVar5;
      uStack_24 = puVar5[1];
      fStack_20 = (float)puVar5[2];
      FUN_1407ff1c0(lVar2);
      if (0.0 < fStack_20) {
        fStack_20 = -(fStack_20 * *(float *)(lVar7 + 0x184));
        FUN_1407ff080(lVar2,&uStack_28);
      }
      FUN_140854190(param_1);
      if ((*(float *)(lVar2 + 0x2b0) <= 3.4028235e+38 && *(float *)(lVar2 + 0x2b0) != 3.4028235e+38)
         || (lVar6 = FUN_1407fdaa0(lVar2), *(int *)(lVar6 + 4) != 0)) {
LAB_140868469:
        uVar9 = 0;
      }
      else {
        *(undefined1 *)(param_1 + 0xb9) = 1;
        uVar9 = 3;
      }
    }
  }
LAB_14086846b:
  FUN_1408022e0(*(undefined8 *)(param_1 + 0x10),uVar9);
LAB_140868474:
  uVar9 = FUN_140854880(param_1);
  pfVar8 = (float *)FUN_140877eb0(uVar9,auStackX_8);
  *(float *)(param_1 + 0x9c) = *pfVar8 + *(float *)(param_1 + 0x9c);
  return;
}
