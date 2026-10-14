
undefined8 *
FUN_14040ace0(undefined8 *param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  
  uVar1 = 0xfffffffffffffffe;
  *param_1 = &PTR_LAB_141b38298;
  param_1[1] = 0;
  FUN_14013fb30(param_1 + 2);
  *param_1 = &PTR_LAB_141b4a228;
  if (DAT_14397e6f0 == 0) {
    DAT_14397e6f0 = FUN_14014bbf0(PTR_DAT_142294408,&DAT_143de2258);
  }
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  *(undefined4 *)(param_1 + 0xc) = 0x3fc00000;
  *(undefined4 *)((longlong)param_1 + 100) = 0x41700000;
  *(undefined4 *)(param_1 + 0xd) = 0x3e4ccccd;
  FUN_140131f90(param_1 + 0xe,"anchor_jnt",0,param_4,uVar1);
  return param_1;
}


