
ulonglong FUN_1408658c0(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  char cVar3;
  undefined1 *puVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  ulonglong uVar9;
  byte bVar10;
  float extraout_XMM0_Da;
  float fVar11;
  uint local_108 [2];
  float local_100;
  undefined1 local_f0 [24];
  undefined8 local_d8;
  undefined1 local_c8 [60];
  undefined4 local_8c;
  undefined4 local_88;
  undefined4 local_84;
  undefined4 local_74;
  
  local_d8 = 0xfffffffffffffffe;
  lVar1 = *(longlong *)(param_1 + 0x18);
  lVar2 = *(longlong *)(param_1 + 0x10);
  lVar6 = *(longlong *)(lVar2 + 0x2f8);
  fVar11 = 0.0;
  bVar10 = 0;
  puVar4 = (undefined1 *)register0x00000020;
  if ((*(longlong *)(lVar1 + 0x178) != 0) &&
     (puVar4 = (undefined1 *)FUN_1408657c0(param_1,*(undefined4 *)(param_1 + 0x88)),
     fVar11 = extraout_XMM0_Da, 0.0 < extraout_XMM0_Da)) {
    D2_DamageParams_Construct(local_c8,*(undefined8 *)(lVar1 + 0x178));
    lVar5 = FUN_140854880(param_1);
    if (lVar5 == 0) {
      local_74 = 0x3fff0010;
    }
    else {
      local_74 = *(undefined4 *)(lVar5 + 0xa8);
    }
    FUN_140780a80(local_c8,extraout_XMM0_Da,1);
    if (extraout_XMM0_Da == 3.4028235e+38) {
      FUN_140780ae0(local_c8,1);
    }
    local_8c = DAT_14228a940;
    local_88 = DAT_14228a944;
    local_84 = DAT_14228a948;
    cVar3 = FUN_140824050(lVar6 + 0x410);
    if (cVar3 == '\0') {
      uVar7 = FUN_140882e20(local_108,local_c8);
      uVar8 = FUN_140854880(param_1);
      FUN_140884920(uVar8,local_f0,&DAT_1439a9d20,uVar7);
      FUN_140882fc0(local_f0);
      puVar4 = (undefined1 *)FUN_140854880(param_1);
      bVar10 = puVar4[0xd1];
    }
    else {
      lVar6 = FUN_140824760(lVar6 + 0x410);
      puVar4 = (undefined1 *)0x0;
      if (lVar6 == 0) goto LAB_140865a79;
      uVar7 = FUN_140882e20(local_f0,local_c8);
      FUN_140884920(lVar6,local_108,&DAT_1439a9d20,uVar7);
      puVar4 = (undefined1 *)FUN_140882fc0(local_108);
      bVar10 = *(byte *)(lVar6 + 0xd1);
    }
    bVar10 = bVar10 >> 3 & 1;
  }
LAB_140865a79:
  *(undefined1 *)(param_1 + 0xa0) = 0;
  if (bVar10 == 0) {
    if ((*(float *)(lVar1 + 0x158) <= *(float *)(param_1 + 0x88) &&
         *(float *)(param_1 + 0x88) != *(float *)(lVar1 + 0x158)) &&
       (puVar4 = (undefined1 *)FUN_140803400(lVar2,4), (char)puVar4 != '\0')) {
      FUN_140854190(param_1);
      DAT_143994a30 = DAT_143994a30 + 1 & 0x7fffffff;
      if (DAT_143994a30 == 0) {
        DAT_143994a30 = 1;
      }
      local_108[1] = 4;
      local_108[0] = DAT_143994a30;
      local_100 = fVar11;
      puVar4 = (undefined1 *)FUN_1408024d0(lVar2,4,local_108);
      *(undefined1 *)(param_1 + 0xa0) = 1;
    }
  }
  else {
    lVar6 = FUN_140854880(param_1);
    puVar4 = (undefined1 *)
             FUN_140770990(*(undefined8 *)(lVar2 + 0x300),lVar6 + 0x300,
                           *(undefined8 *)(lVar1 + 0x198));
  }
  *(undefined4 *)(param_1 + 0x88) = 0;
  if ((bVar10 == 0) && (*(char *)(param_1 + 0xa0) == '\0')) {
    uVar9 = (ulonglong)puVar4 & 0xffffffffffffff00;
  }
  else {
    uVar9 = 1;
  }
  return uVar9;
}


