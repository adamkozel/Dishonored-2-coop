void FUN_140867b70(longlong param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong lVar5;
  undefined4 uVar6;
  longlong lVar7;
  float fVar8;
  
  FUN_14083e870();
  lVar7 = *(longlong *)(param_1 + 0x10);
  lVar2 = *(longlong *)(param_1 + 0x18);
  uVar3 = *(undefined8 *)(lVar7 + 0x300);
  if (*(char *)(param_1 + 0x69) == '\0') {
    if (*(char *)(param_1 + 0x68) == '\0') {
      uVar6 = *(undefined4 *)(lVar2 + 0x15c);
    }
    else {
      uVar6 = *(undefined4 *)(lVar2 + 0x164);
    }
  }
  else {
    uVar6 = *(undefined4 *)(lVar2 + 0x160);
  }
  *(undefined4 *)(param_1 + 0x8c) = uVar6;
  *(undefined4 *)(param_1 + 0x98) = 0;
  *(undefined4 *)(param_1 + 0xa8) = *(undefined4 *)(lVar2 + 0x174);
  fVar8 = 0.0;
  *(undefined4 *)(param_1 + 0xac) = *(undefined4 *)(lVar2 + 0x178);
  uVar4 = *(undefined8 *)(lVar2 + 0x188);
  *(undefined4 *)(param_1 + 0x94) = 1;
  *(float *)(param_1 + 0x90) = *(float *)(param_1 + 0x8c);
  *(undefined8 *)(param_1 + 0xb0) = uVar4;
  iVar1 = *(int *)(param_1 + 0xbc);
  if ((-1 < iVar1) &&
     (lVar5 = *(longlong *)(*(longlong *)(lVar7 + 0x330) + 0x48),
     *(float *)(lVar5 + 0x24 + (longlong)iVar1 * 0x28) !=
     *(float *)(lVar5 + 8 + (longlong)iVar1 * 0x28))) {
    fVar8 = *(float *)(lVar2 + 0x168);
    *(undefined4 *)(param_1 + 0x94) = *(undefined4 *)(lVar2 + 0x16c);
  }
  if (*(int *)(param_1 + 0x94) < 2) {
    if (0.0 < fVar8) {
      *(float *)(param_1 + 0x90) = *(float *)(param_1 + 0x8c) + fVar8;
      *(undefined4 *)(param_1 + 0xa8) = *(undefined4 *)(lVar2 + 0x17c);
      *(undefined4 *)(param_1 + 0xac) = *(undefined4 *)(lVar2 + 0x180);
      if (*(longlong *)(lVar2 + 0x198) != 0) {
        *(longlong *)(param_1 + 0xb0) = *(longlong *)(lVar2 + 0x198);
      }
    }
  }
  else {
    *(float *)(param_1 + 0x98) = fVar8;
  }
  *(undefined4 *)(param_1 + 0x9c) = 0;
  if ((*(char *)(param_1 + 0xb8) != '\0') &&
     (lVar7 = FUN_140777b90(*(undefined8 *)(lVar7 + 0x338),6), lVar7 != 0)) {
    *(float *)(param_1 + 0x90) = *(float *)(lVar7 + 0x10c) + *(float *)(param_1 + 0x90);
  }
  *(undefined1 *)(param_1 + 0xb9) = 0;
  FUN_140774f10(uVar3,*(undefined1 *)(param_1 + 0x68),0x3f800000,0);
  lVar7 = FUN_140854880(param_1);
  uVar6 = *(undefined4 *)(lVar7 + 0x308);
  *(undefined4 *)(param_1 + 0xa0) = uVar6;
  *(undefined4 *)(param_1 + 0xa4) = uVar6;
  FUN_1408680a0(param_1);
  return;
}
