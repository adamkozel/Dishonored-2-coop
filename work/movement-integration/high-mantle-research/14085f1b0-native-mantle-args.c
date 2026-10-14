
void FUN_14085f1b0(longlong param_1,undefined8 *param_2)

{
  byte bVar1;
  
  *(byte *)(param_1 + 0x118) = *(byte *)(param_1 + 0x118) | 4;
  *(undefined8 *)(param_1 + 0xac) = *param_2;
  *(undefined4 *)(param_1 + 0xb4) = *(undefined4 *)(param_2 + 1);
  *(undefined4 *)(param_1 + 0xb8) = *(undefined4 *)((longlong)param_2 + 0xc);
  *(undefined4 *)(param_1 + 0xbc) = *(undefined4 *)(param_2 + 2);
  *(undefined4 *)(param_1 + 0xc0) = *(undefined4 *)((longlong)param_2 + 0x14);
  *(undefined4 *)(param_1 + 0xc4) = *(undefined4 *)(param_2 + 3);
  *(undefined4 *)(param_1 + 200) = *(undefined4 *)((longlong)param_2 + 0x1c);
  *(undefined4 *)(param_1 + 0xcc) = *(undefined4 *)(param_2 + 4);
  *(undefined4 *)(param_1 + 0xd0) = *(undefined4 *)((longlong)param_2 + 0x24);
  *(undefined4 *)(param_1 + 0xd4) = *(undefined4 *)(param_2 + 5);
  *(undefined4 *)(param_1 + 0xd8) = *(undefined4 *)((longlong)param_2 + 0x2c);
  *(byte *)(param_1 + 0xdc) =
       *(byte *)(param_1 + 0xdc) ^ (*(byte *)(param_1 + 0xdc) ^ *(byte *)(param_2 + 6)) & 1;
  bVar1 = (*(byte *)(param_2 + 6) ^ *(byte *)(param_1 + 0xdc)) & 2 ^ *(byte *)(param_1 + 0xdc);
  *(byte *)(param_1 + 0xdc) = bVar1;
  *(byte *)(param_1 + 0xdc) = (*(byte *)(param_2 + 6) ^ bVar1) & 4 ^ bVar1;
  FUN_1409ffc00(param_1);
  return;
}


