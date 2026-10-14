# partner-context-1403af060-decompile-v384.txt

Original: [work/movement-integration/partner-context-1403af060-decompile-v384.txt](<../../../../work/movement-integration/partner-context-1403af060-decompile-v384.txt>)

SHA-256: `8E3F845ED826763EFE441658896068AD18473C354C0513D94E31310038FC0E20`; 3129 bytes.

## Original saved record

````text

undefined8 * FUN_1403af060(undefined8 *param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined8 uVar4;
  
  FUN_1403a3600();
  *param_1 = &PTR_LAB_141b3f998;
  FUN_1411ad700(param_1 + 8,0x38,5,FUN_1403af410,FUN_140187520);
  *(undefined4 *)(param_1 + 0x2b) = 0xffffffff;
  param_1[0x2c] = 0;
  *(undefined4 *)(param_1 + 0x33) = 0x100;
  *(undefined1 *)((longlong)param_1 + 0x1a4) = 0x3f;
  uVar4 = SUB168(ZEXT816(8) * ZEXT816(0x100),0);
  if (SUB168(ZEXT816(8) * ZEXT816(0x100),8) != 0) {
    uVar4 = 0xffffffffffffffff;
  }
  uVar4 = FUN_1400f16a0(uVar4,0x3f,0xffffffff);
  param_1[0x32] = uVar4;
  FUN_1411ad450(uVar4,0,0x800);
  *(undefined4 *)((longlong)param_1 + 0x19c) = 0;
  *(undefined4 *)(param_1 + 0x34) = 0xff;
  param_1[0x3a] = 0;
  param_1[0x3b] = 2;
  *(undefined1 *)(param_1 + 0x3c) = 0;
  *(undefined4 *)((longlong)param_1 + 0x1e4) = 0xffffffff;
  param_1[0x35] = 0;
  param_1[0x36] = 0;
  param_1[0x37] = 0;
  param_1[0x38] = 0;
  param_1[0x39] = 0;
  uVar3 = DAT_1431af670;
  uVar2 = DAT_1431af66c;
  uVar1 = DAT_1431af668;
  param_1[9] = 0;
  param_1[10] = 0;
  param_1[0xb] = 0;
  *(undefined4 *)(param_1 + 0xc) = 0x3fff0010;
  *(undefined4 *)((longlong)param_1 + 100) = uVar1;
  *(undefined4 *)(param_1 + 0xd) = uVar2;
  *(undefined4 *)((longlong)param_1 + 0x6c) = uVar3;
  *(undefined4 *)(param_1 + 0xe) = 0;
  *(undefined4 *)((longlong)param_1 + 0x74) = 1;
  uVar3 = DAT_1431af670;
  uVar2 = DAT_1431af66c;
  uVar1 = DAT_1431af668;
  param_1[0x10] = 0;
  param_1[0x11] = 0;
  param_1[0x12] = 0;
  *(undefined4 *)(param_1 + 0x13) = 0x3fff0010;
  *(undefined4 *)((longlong)param_1 + 0x9c) = uVar1;
  *(undefined4 *)(param_1 + 0x14) = uVar2;
  *(undefined4 *)((longlong)param_1 + 0xa4) = uVar3;
  *(undefined4 *)(param_1 + 0x15) = 0;
  *(undefined4 *)((longlong)param_1 + 0xac) = 1;
  uVar3 = DAT_1431af670;
  uVar2 = DAT_1431af66c;
  uVar1 = DAT_1431af668;
  param_1[0x17] = 0;
  param_1[0x18] = 0;
  param_1[0x19] = 0;
  *(undefined4 *)(param_1 + 0x1a) = 0x3fff0010;
  *(undefined4 *)((longlong)param_1 + 0xd4) = uVar1;
  *(undefined4 *)(param_1 + 0x1b) = uVar2;
  *(undefined4 *)((longlong)param_1 + 0xdc) = uVar3;
  *(undefined4 *)(param_1 + 0x1c) = 0;
  *(undefined4 *)((longlong)param_1 + 0xe4) = 1;
  uVar3 = DAT_1431af670;
  uVar2 = DAT_1431af66c;
  uVar1 = DAT_1431af668;
  param_1[0x1e] = 0;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  *(undefined4 *)(param_1 + 0x21) = 0x3fff0010;
  *(undefined4 *)((longlong)param_1 + 0x10c) = uVar1;
  *(undefined4 *)(param_1 + 0x22) = uVar2;
  *(undefined4 *)((longlong)param_1 + 0x114) = uVar3;
  *(undefined4 *)(param_1 + 0x23) = 0;
  *(undefined4 *)((longlong)param_1 + 0x11c) = 1;
  uVar3 = DAT_1431af670;
  uVar2 = DAT_1431af66c;
  uVar1 = DAT_1431af668;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  param_1[0x27] = 0;
  *(undefined4 *)(param_1 + 0x28) = 0x3fff0010;
  *(undefined4 *)((longlong)param_1 + 0x144) = uVar1;
  *(undefined4 *)(param_1 + 0x29) = uVar2;
  *(undefined4 *)((longlong)param_1 + 0x14c) = uVar3;
  *(undefined4 *)(param_1 + 0x2a) = 0;
  *(undefined4 *)((longlong)param_1 + 0x154) = 1;
  return param_1;
}



````
