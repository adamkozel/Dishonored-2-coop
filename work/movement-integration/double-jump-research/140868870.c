void FUN_140868870(longlong param_1)

{
  longlong lVar1;
  undefined8 uVar2;
  undefined1 *puVar3;
  undefined4 *puVar4;
  undefined8 *puVar5;
  float fVar6;
  undefined4 uVar7;
  float fVar8;
  undefined4 local_res8 [2];
  
  lVar1 = *(longlong *)(param_1 + 0x18);
  uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300);
  if (**(char **)(lVar1 + 0xb0) != '\0') {
    puVar3 = (undefined1 *)FUN_1407710b0(uVar2,local_res8,lVar1 + 0xb0);
    if ((undefined1 *)(param_1 + 0x34) != puVar3) {
      *(undefined1 *)(param_1 + 0x34) = *puVar3;
    }
  }
  puVar5 = (undefined8 *)(lVar1 + 0xb8);
  if (*(char *)*puVar5 == '\0') {
    if (**(char **)(lVar1 + 0xb0) == '\0') goto LAB_140868911;
    puVar5 = (undefined8 *)(lVar1 + 0xb0);
  }
  puVar3 = (undefined1 *)FUN_140770f10(uVar2,local_res8,puVar5);
  if ((undefined1 *)(param_1 + 0x35) != puVar3) {
    *(undefined1 *)(param_1 + 0x35) = *puVar3;
  }
LAB_140868911:
  fVar8 = DAT_14228a75c * *(float *)(lVar1 + 0xa4);
  if ((fVar8 < 0.0) || (6.2831855 <= fVar8)) {
    fVar6 = (float)FUN_1411ad95c(fVar8 * 0.15915494);
    fVar8 = fVar8 - fVar6 * 6.2831855;
  }
  uVar7 = FUN_1411ade30(fVar8);
  *(undefined4 *)(param_1 + 0x40) = uVar7;
  fVar8 = DAT_14228a75c * *(float *)(lVar1 + 0xa8);
  if ((fVar8 < 0.0) || (6.2831855 <= fVar8)) {
    fVar6 = (float)FUN_1411ad95c(fVar8 * 0.15915494);
    fVar8 = fVar8 - fVar6 * 6.2831855;
  }
  uVar7 = FUN_1411ade30(fVar8);
  *(undefined4 *)(param_1 + 0x44) = uVar7;
  uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330);
  puVar4 = (undefined4 *)FUN_14060ce50(uVar2,local_res8,&DAT_143996498);
  if ((undefined4 *)(param_1 + 0x54) != puVar4) {
    *(undefined4 *)(param_1 + 0x54) = *puVar4;
  }
  puVar4 = (undefined4 *)FUN_14060ce50(uVar2,local_res8,&DAT_1439964a0);
  if ((undefined4 *)(param_1 + 0x58) != puVar4) {
    *(undefined4 *)(param_1 + 0x58) = *puVar4;
  }
  puVar4 = (undefined4 *)FUN_14060ce50(uVar2,local_res8,&DAT_1439964a8);
  if ((undefined4 *)(param_1 + 0x5c) != puVar4) {
    *(undefined4 *)(param_1 + 0x5c) = *puVar4;
  }
  if ((undefined4 *)(param_1 + 0x30) != local_res8) {
    *(undefined4 *)(param_1 + 0x30) = 0xffffffff;
  }
  if (**(char **)(lVar1 + 0x138) != '\0') {
    puVar4 = (undefined4 *)FUN_14060ce50(uVar2,local_res8);
    if ((undefined4 *)(param_1 + 0x60) != puVar4) {
      *(undefined4 *)(param_1 + 0x60) = *puVar4;
    }
  }
  *(undefined1 *)(param_1 + 0x48) = 0;
  return;
}
