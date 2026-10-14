void FUN_140616210(longlong param_1,longlong *param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  longlong lVar2;
  
  iVar1 = *(int *)(param_1 + 0x188);
  if ((-1 < iVar1) &&
     (lVar2 = *(longlong *)(*(longlong *)(param_1 + 0xf0) + 0x48),
     *(float *)(lVar2 + 0x24 + (longlong)iVar1 * 0x28) !=
     *(float *)(lVar2 + 8 + (longlong)iVar1 * 0x28))) {
    if ((*(uint *)(*param_2 + 0xa8) & 0xffffff) == 0) {
      return;
    }
    FUN_140616940();
  }
  if ((*(uint *)(*param_2 + 0x98) & 0xffffff) != 0) {
    FUN_140616940(param_1,*param_2 + 0x90,param_3,param_4);
  }
  return;
}
