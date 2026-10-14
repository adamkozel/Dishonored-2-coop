
undefined8 FUN_140854920(longlong param_1)

{
  undefined8 uVar1;
  
  if (*(int *)(param_1 + 0x24) != 0) {
    uVar1 = FUN_140772070(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x300),
                          *(undefined4 *)(param_1 + 0x24));
    return uVar1;
  }
  return 1;
}


