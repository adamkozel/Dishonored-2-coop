# native-doppelganger-fx-1407db090-decompile-v438.txt

Original: [work/movement-integration/native-doppelganger-fx-1407db090-decompile-v438.txt](<../../../../work/movement-integration/native-doppelganger-fx-1407db090-decompile-v438.txt>)

SHA-256: `9029FE6FEA4A3E9257F0020DD41C000C1FCB3F1B2CCAD01F17547CE8D83E51EF`; 1903 bytes.

## Original saved record

````text

undefined8 *
FUN_1407db090(undefined8 *param_1,undefined8 param_2,longlong param_3,undefined4 param_4)

{
  undefined8 uVar1;
  undefined4 uVar2;
  
  FUN_140883740();
  *param_1 = &PTR_LAB_141c6ca98;
  *(uint *)(param_1 + 5) = *(uint *)(param_1 + 5) & 0xff000000;
  *(undefined1 *)((longlong)param_1 + 0x2b) = 5;
  *(undefined4 *)((longlong)param_1 + 0x2c) = 0xf40;
  param_1[4] = param_1 + 6;
  *(undefined4 *)(param_1 + 0x15) = *(undefined4 *)(param_3 + 8);
  uVar1 = *(undefined8 *)(param_3 + 0x10);
  param_1[0x17] = param_3;
  param_1[0x19] = param_2;
  param_1[0x16] = uVar1;
  param_1[0x18] = 0;
  param_1[0x1a] = 0;
  param_1[0x1b] = 0;
  param_1[0x1c] = 0xffffffffffffffff;
  param_1[0x1d] = 0;
  param_1[0x1e] = 0;
  param_1[0x1f] = 0;
  param_1[0x20] = 0;
  *(undefined4 *)(param_1 + 0x21) = 0xffffffff;
  param_1[0x22] = 0;
  *(undefined4 *)(param_1 + 0x23) = 0xffffffff;
  param_1[0x24] = 0;
  param_1[0x25] = 0;
  param_1[0x26] = 0;
  param_1[0x27] = 0;
  param_1[0x28] = 0;
  param_1[0x29] = 0;
  *(undefined4 *)(param_1 + 0x2a) = 0xffffffff;
  param_1[0x2b] = 0;
  *(undefined4 *)(param_1 + 0x2c) = 0xffffffff;
  param_1[0x2d] = 0;
  param_1[0x2e] = 0;
  *(undefined4 *)(param_1 + 0x2f) = param_4;
  *(undefined8 *)((longlong)param_1 + 0x17c) = 0xffffffffffffffff;
  *(undefined4 *)((longlong)param_1 + 0x184) = 0xffffffff;
  *(undefined2 *)(param_1 + 0x31) = 0xffff;
  *(undefined4 *)((longlong)param_1 + 0x18c) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x32) = 0x3fff0010;
  *(undefined4 *)((longlong)param_1 + 0x194) = DAT_1431af620;
  *(undefined4 *)(param_1 + 0x33) = DAT_1431af624;
  uVar2 = DAT_1431af628;
  *(byte *)((longlong)param_1 + 0x1ad) = *(byte *)((longlong)param_1 + 0x1ad) & 0xfc;
  *(undefined4 *)((longlong)param_1 + 0x19c) = uVar2;
  param_1[0x34] = 0;
  *(undefined4 *)(param_1 + 0x35) = 0;
  *(undefined1 *)((longlong)param_1 + 0x1ac) = 0;
  return param_1;
}



````
