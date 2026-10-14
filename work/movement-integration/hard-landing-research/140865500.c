
void FUN_140865500(longlong param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined1 auStackX_8 [32];
  
  FUN_140868870();
  uVar1 = FUN_1407f8870(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x2f8),7);
  *(undefined4 *)(param_1 + 0x98) = uVar1;
  puVar2 = (undefined4 *)
           FUN_14060ce50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330),auStackX_8,
                         &DAT_143995fa0);
  if ((undefined4 *)(param_1 + 0xa4) != puVar2) {
    *(undefined4 *)(param_1 + 0xa4) = *puVar2;
  }
  puVar2 = (undefined4 *)
           FUN_14060ce50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330),auStackX_8,
                         &DAT_143995fa8);
  if ((undefined4 *)(param_1 + 0xa8) != puVar2) {
    *(undefined4 *)(param_1 + 0xa8) = *puVar2;
  }
  return;
}


