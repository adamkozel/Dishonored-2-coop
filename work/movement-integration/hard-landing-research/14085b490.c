
void FUN_14085b490(longlong param_1,longlong param_2)

{
  longlong lVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  char cVar5;
  char cVar6;
  longlong lVar7;
  int *piVar8;
  undefined8 uVar9;
  longlong lVar10;
  int iVar11;
  char cVar12;
  longlong lVar13;
  undefined1 auStackX_8 [8];
  undefined1 auStack_58 [48];
  
  FUN_140854d00();
  lVar1 = *(longlong *)(param_1 + 0x10);
  *(undefined4 *)(param_1 + 0x50) = *(undefined4 *)(param_2 + 8);
  cVar4 = FUN_140803180(lVar1,0x40);
  lVar10 = 0;
  if (cVar4 == '\0') {
    lVar13 = 1;
    if (*(longlong *)(*(longlong *)(lVar1 + 0x328) + 0x70) == 0) {
      lVar13 = lVar10;
    }
  }
  else {
    lVar7 = lVar10;
    if (*(longlong *)(lVar1 + 0x90) != 0) {
      lVar7 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x210);
    }
    lVar13 = 1;
    if (*(longlong *)(lVar7 + 0x118) != 0) {
      lVar13 = 0;
    }
  }
  lVar1 = *(longlong *)(param_1 + 0x10);
  cVar4 = FUN_140803180(lVar1,0x35);
  cVar6 = '\0';
  if (cVar4 == '\0') {
    cVar4 = '\x01';
    if (*(longlong *)(*(longlong *)(lVar1 + 0x328) + 0x68) == 0) {
      cVar4 = cVar6;
    }
  }
  else {
    lVar7 = lVar10;
    if (*(longlong *)(lVar1 + 0x90) != 0) {
      lVar7 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x1b8);
    }
    cVar4 = '\x01';
    if (*(longlong *)(lVar7 + 0x118) != 0) {
      cVar4 = '\0';
    }
  }
  lVar1 = *(longlong *)(param_1 + 0x10);
  bVar3 = false;
  cVar12 = '\0';
  cVar5 = FUN_140803180(lVar1,0x2d);
  if (cVar5 == '\0') {
    cVar5 = FUN_140803180(lVar1,0x31);
    if (cVar5 == '\0') {
      cVar5 = FUN_140803180(lVar1,0x32);
      if (cVar5 == '\0') goto LAB_14085b653;
      if (*(longlong *)(lVar1 + 0x90) != 0) {
        lVar10 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x1a0);
      }
      piVar8 = (int *)FUN_14083d3d0(lVar10,auStackX_8);
      iVar11 = *piVar8;
      FUN_14083cfb0(lVar10,0);
      bVar3 = true;
      cVar12 = '\x01';
    }
    else {
      if (*(longlong *)(lVar1 + 0x90) != 0) {
        lVar10 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x198);
      }
      piVar8 = (int *)FUN_14083d3d0(lVar10,auStackX_8);
      iVar11 = *piVar8;
      FUN_14083cfb0(lVar10,0);
      cVar12 = '\x01';
      bVar3 = false;
    }
  }
  else {
    if (*(longlong *)(lVar1 + 0x90) != 0) {
      lVar10 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x178);
    }
    iVar11 = *(int *)(lVar10 + 0xf8);
    *(int *)(param_1 + 100) = iVar11;
    cVar12 = (char)lVar13;
    bVar3 = false;
  }
  if (iVar11 != 0x3fff0010) {
    uVar9 = FUN_140810240();
    FUN_140810410(uVar9,0x148,iVar11);
  }
LAB_14085b653:
  FUN_140835590(auStack_58,0,0x21,*(undefined4 *)(param_1 + 8));
  FUN_1408024d0(lVar1,0x40,auStack_58);
  if ((*(float *)(param_1 + 0x50) < *(float *)(*(longlong *)(param_1 + 0x18) + 0xa0)) ||
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0xa8) == 0)) {
    bVar2 = false;
  }
  else {
    bVar2 = true;
  }
  if ((cVar4 == '\0') || (bVar2)) {
    FUN_1408024d0(lVar1,0x35,auStack_58);
  }
  if (((char)lVar13 != '\0') || (bVar3)) {
    cVar6 = '\x01';
  }
  *(char *)(param_1 + 0x56) = cVar6;
  if ((cVar4 == '\0') && (cVar12 == '\0')) {
    *(undefined1 *)(param_1 + 0x57) = 0;
    return;
  }
  *(undefined1 *)(param_1 + 0x57) = 1;
  return;
}


