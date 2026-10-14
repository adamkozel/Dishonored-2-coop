
void FUN_14023fbd0(longlong param_1,undefined8 param_2,undefined8 param_3)

{
  longlong lVar1;
  uint uVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  undefined1 local_48 [32];
  undefined8 local_28;
  
  lVar1 = *(longlong *)(param_1 + 0x50);
  lVar3 = 0;
  local_28 = 0;
  FUN_14009e910(local_48,0,*(undefined2 *)(lVar1 + 0x78),0);
  FUN_14023ff50(param_3,0,*(uint *)(lVar1 + 0xd8) & 0xffffff,0);
  uVar2 = *(uint *)(param_1 + 0xa8) & 0xffffff;
  lVar4 = (longlong)(int)uVar2;
  if (uVar2 != 0) {
    do {
      lVar5 = *(longlong *)(param_1 + 0xa0) + lVar3;
      (**(code **)(&DAT_142299718 + (longlong)*(int *)(lVar5 + 0x18) * 8))
                (lVar1 + 0xd0,param_2,lVar5,local_48,param_3);
      lVar3 = lVar3 + 0x20;
      lVar4 = lVar4 + -1;
    } while (lVar4 != 0);
  }
  return;
}


