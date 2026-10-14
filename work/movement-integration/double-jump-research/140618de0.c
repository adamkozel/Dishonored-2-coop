undefined4 FUN_140618de0(longlong param_1,undefined8 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  longlong lVar4;
  undefined **local_res8;
  undefined8 local_40;
  undefined4 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined1 local_18;
  undefined4 local_14;
  undefined1 local_b;
  
  local_res8 = &PTR_FUN_141b9a840;
  local_38 = 0x3fff0010;
  local_30 = 0;
  local_28 = 0;
  local_20 = 0;
  local_18 = 0;
  local_14 = 0;
  local_b = 1;
  local_40 = param_2;
  iVar2 = FUN_140618af0(&local_res8,*(undefined8 *)(param_1 + 0x38),
                        *(uint *)(param_1 + 0x40) & 0xffffff,&local_40,0xfffffffffffffffe);
  local_res8 = &PTR_FUN_141b9a808;
  if (iVar2 < 0) {
    uVar3 = 0;
  }
  else {
    lVar4 = (longlong)iVar2 * 0x38 + *(longlong *)(param_1 + 0x38);
    uVar1 = *(undefined4 *)(lVar4 + 0x20);
    uVar3 = FUN_14063cc00(lVar4,param_3);
    FUN_1406170b0(param_1,lVar4,uVar1,uVar3);
  }
  return uVar3;
}
