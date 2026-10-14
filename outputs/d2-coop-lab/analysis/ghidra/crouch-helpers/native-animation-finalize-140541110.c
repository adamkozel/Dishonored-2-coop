
void FUN_140541110(longlong *param_1)

{
  longlong lVar1;
  char cVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  longlong *plVar5;
  int iVar6;
  longlong lVar7;
  undefined4 auStackX_8 [2];
  
  lVar7 = *(longlong *)(param_1[2] + 0x358);
  if (lVar7 != 0) {
    uVar3 = FUN_140750f70(lVar7);
    uVar4 = FUN_140750fe0(lVar7);
    FUN_14009d560(param_1 + 8,uVar4,uVar3);
  }
  lVar7 = param_1[2];
  FUN_140877eb0(lVar7,auStackX_8);
  if (((param_1[7] != 0) && (*(longlong *)(*(longlong *)(param_1[7] + 0x1fe8) + 0x50) != 0)) &&
     (cVar2 = (**(code **)(*param_1 + 0xd8))(param_1,auStackX_8[0]), cVar2 != '\0')) {
    FUN_14053bcd0(param_1,0x40);
    lVar1 = *(longlong *)(param_1[7] + 0x1fe8);
    if (*(char *)(*(longlong *)(lVar1 + 0x68) + 0x10 +
                 (longlong)*(int *)(param_1[7] + 0x1fb8) * 0x18) == '\0') {
      FUN_14023fca0(lVar1,param_1 + 8,param_1 + 0x56);
    }
    FUN_140540a90(param_1,3);
    if (*(longlong *)(lVar7 + 0x358) == 0) {
      plVar5 = (longlong *)D2_Entity_GetChildren(lVar7);
      iVar6 = 0;
      if ((*(uint *)(plVar5 + 1) & 0xffffff) != 0) {
        lVar7 = 0;
        do {
          uVar3 = FUN_1408783d0(*(undefined8 *)(lVar7 + *plVar5));
          FUN_1407521f0(uVar3);
          iVar6 = iVar6 + 1;
          lVar7 = lVar7 + 8;
        } while (iVar6 < (int)(*(uint *)(plVar5 + 1) & 0xffffff));
      }
    }
    else {
      uVar3 = FUN_1408783d0();
      FUN_1407521f0(uVar3);
    }
  }
  *(uint *)(param_1 + 6) = *(uint *)(param_1 + 6) & 0xff000000;
  return;
}


