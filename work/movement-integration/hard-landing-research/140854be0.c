
undefined8
FUN_140854be0(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined4 param_4,
             undefined4 param_5)

{
  undefined4 uVar1;
  undefined1 *local_1f8;
  undefined4 local_1f0;
  uint local_1ec;
  undefined1 local_1e8 [480];
  
  local_1f0 = 0x5000000;
  local_1f8 = local_1e8;
  local_1ec = 0xa40;
  uVar1 = 1;
  FUN_1411ad700(local_1e8,0x30,10,FUN_140389210,&LAB_1400ecab0);
  FUN_1408542b0(param_1,param_4,0,&local_1f8);
  FUN_1408549c0(param_1,param_2,param_3,&local_1f8,CONCAT44(uVar1,param_5));
  FUN_1411ad55c(local_1e8,0x30,10,&LAB_1400ecab0);
  if (((local_1ec & 0x40) == 0) && (local_1f8 != (undefined1 *)0x0)) {
    FUN_1409d8700(local_1f8,local_1ec >> 8);
  }
  return param_2;
}


