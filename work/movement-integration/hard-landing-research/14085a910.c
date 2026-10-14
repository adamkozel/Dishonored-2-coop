
void FUN_14085a910(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  char cVar3;
  undefined8 uVar4;
  longlong *plVar5;
  longlong lVar6;
  undefined4 *puVar7;
  longlong lVar8;
  longlong lVar9;
  longlong lVar10;
  longlong lVar11;
  longlong lVar12;
  bool bVar13;
  undefined4 uVar14;
  undefined8 uStackX_20;
  undefined4 uStack_88;
  undefined4 uStack_84;
  undefined4 uStack_80;
  undefined4 uStack_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined4 uStack_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  
  FUN_1409ffc00();
  lVar9 = *(longlong *)(param_1 + 0x18);
  lVar1 = *(longlong *)(param_1 + 0x10);
  uVar4 = FUN_140854880(param_1);
  FUN_1407ff0e0(*(undefined8 *)(lVar1 + 0x308),0xb,0);
  lVar12 = *(longlong *)(lVar1 + 0x310);
  FUN_1407f01e0(lVar12 + 0x251c0,lVar9 + 200);
  FUN_1407f43b0(lVar12,0xe,0xbf800000);
  *(undefined2 *)(param_1 + 0x54) = 0x101;
  plVar5 = (longlong *)FUN_140777b90(*(undefined8 *)(lVar1 + 0x338),0x20);
  if (plVar5 != (longlong *)0x0) {
    *(undefined1 *)(param_1 + 0x54) = *(undefined1 *)((longlong)plVar5 + 0x65);
    *(undefined1 *)(param_1 + 0x55) = *(undefined1 *)((longlong)plVar5 + 0x66);
  }
  (**(code **)(*plVar5 + 0xd8))(plVar5,0);
  uStackX_20 = *(undefined8 *)(lVar9 + 0x98);
  lVar12 = *(longlong *)(lVar9 + 0x70);
  lVar11 = *(longlong *)(lVar9 + 0x78);
  lVar2 = *(longlong *)(lVar9 + 0x80);
  bVar13 = (*(ulonglong *)(*(longlong *)(lVar1 + 0x2f8) + 0xa0) >>
            ((ulonglong)*(uint *)(*(longlong *)(lVar1 + 0x2f8) + 0x6c) & 0x3f) & 1) != 0;
  lVar10 = lVar2;
  if ((((*(longlong *)(*(longlong *)(lVar1 + 0x328) + 0x68) != 0) &&
       (lVar6 = FUN_14063c8c0(), *(int *)(lVar6 + 0x11c) == 1)) &&
      ((!bVar13 || (lVar10 = *(longlong *)(lVar9 + 0x90), lVar10 == 0)))) &&
     (lVar10 = lVar2, *(longlong *)(lVar9 + 0x88) != 0)) {
    lVar10 = *(longlong *)(lVar9 + 0x88);
  }
  if ((*(float *)(*(longlong *)(param_1 + 0x18) + 0xa0) <= *(float *)(param_1 + 0x50)) &&
     (*(longlong *)(*(longlong *)(param_1 + 0x18) + 0xa8) != 0)) {
    uStackX_20 = *(undefined8 *)(lVar9 + 0xc0);
    lVar12 = *(longlong *)(lVar9 + 0xa8);
    lVar11 = *(longlong *)(lVar9 + 0xb0);
    lVar10 = *(longlong *)(lVar9 + 0xb8);
    *(undefined2 *)(param_1 + 0x54) = 0x101;
  }
  lVar2 = *(longlong *)(lVar1 + 0x300);
  if (*(char *)(param_1 + 0x54) == '\0') {
    FUN_140774a10(lVar2 + 0x370,0);
  }
  if (*(char *)(param_1 + 0x55) == '\0') {
    FUN_140774a10(lVar2 + 0x680,0);
  }
  FUN_140774600(lVar2,1);
  lVar2 = *(longlong *)(lVar1 + 0x330);
  if (((-1 < *(int *)(param_1 + 0x60)) &&
      (0.0 < *(float *)(*(longlong *)(lVar2 + 0x48) + 0x24 +
                       (longlong)*(int *)(param_1 + 0x60) * 0x28))) &&
     (lVar6 = *(longlong *)(lVar9 + 0xe0), lVar6 != 0)) {
    puVar7 = (undefined4 *)FUN_140417820(*(undefined8 *)(lVar1 + 0x310));
    uStack_88 = *puVar7;
    uStack_84 = puVar7[1];
    uStack_80 = puVar7[2];
    lVar8 = FUN_140854880(param_1);
    uStack_78 = *(undefined4 *)(lVar8 + 0x30c);
    uStack_74 = *(undefined4 *)(lVar8 + 0x310);
    uStack_70 = *(undefined4 *)(lVar8 + 0x314);
    uStack_6c = *(undefined4 *)(lVar8 + 0x318);
    uStack_68 = *(undefined4 *)(lVar8 + 0x31c);
    uStack_64 = *(undefined4 *)(lVar8 + 800);
    uStack_60 = *(undefined4 *)(lVar8 + 0x324);
    uStack_5c = *(undefined4 *)(lVar8 + 0x328);
    uStack_58 = *(undefined4 *)(lVar8 + 0x32c);
    lVar6 = FUN_1408ae9b0(D2_GameSingletonPtr,lVar6,&uStack_88,&uStack_78,uVar4,0,0,0,0);
    if (lVar6 == 0) {
      *(undefined4 *)(param_1 + 0x68) = 0x3fff0010;
    }
    else {
      *(undefined4 *)(param_1 + 0x68) = *(undefined4 *)(lVar6 + 0xa8);
    }
    *(undefined1 *)(param_1 + 0x6c) = 1;
    FUN_14080cd60(D2_GameSingletonPtr + 0x1f52e8,uVar4,lVar6);
  }
  cVar3 = FUN_14060c180(lVar2,*(undefined4 *)(param_1 + 0x5c));
  if (cVar3 != '\0') {
    FUN_14071a140(*(undefined8 *)(lVar1 + 0x348),*(uint *)(lVar9 + 0xd8) ^ 0x80000000);
  }
  if (*(int *)(param_1 + 0x58) < 0) {
    uVar14 = 0x3f800000;
  }
  else {
    uVar14 = *(undefined4 *)
              (*(longlong *)(lVar2 + 0x48) + 0x24 + (longlong)*(int *)(param_1 + 0x58) * 0x28);
  }
  if (lVar12 != 0) {
    if ((lVar11 == 0) || (lVar10 == 0)) {
      uVar4 = 7;
    }
    else {
      FUN_14085b400(param_1,lVar11,2,uVar14);
      FUN_14085b400(param_1,lVar10,4,uVar14);
      uVar4 = 1;
    }
    FUN_14085b400(param_1,lVar12,uVar4,uVar14);
  }
  if (((bVar13) && (*(longlong *)(lVar1 + 0x90) != 0)) &&
     (lVar9 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x40), lVar9 != 0)) {
    FUN_140774760(*(undefined8 *)(lVar1 + 0x300),*(undefined1 *)(lVar9 + 0x34),4);
  }
  lVar9 = FUN_140854880(param_1);
  FUN_140770990(*(undefined8 *)(lVar1 + 0x300),lVar9 + 0x300,uStackX_20);
  return;
}


