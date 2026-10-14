
void FUN_14085f970(longlong param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined4 *puVar4;
  undefined1 auStackX_8 [8];
  longlong lStack_18;
  undefined1 *puStack_10;
  
  uVar2 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x338);
  uVar1 = FUN_140777b90(uVar2,1);
  *(undefined8 *)(param_1 + 0xf8) = uVar1;
  uVar2 = FUN_140777b90(uVar2,2);
  *(undefined8 *)(param_1 + 0x100) = uVar2;
  lVar3 = *(longlong *)(param_1 + 0x18);
  uVar2 = FUN_140854880();
  lVar3 = FUN_1408ae9b0(D2_GameSingletonPtr,*(undefined8 *)(lVar3 + 0xd8),&DAT_1431af668,
                        &DAT_14228a780,uVar2,0,0,0,0);
  *(longlong *)(param_1 + 0xe0) = lVar3;
  FUN_1401668e0(lVar3 == 0,
                "Can\'t create dummy entity for player mantle state. Please check you picked an idEntityDef in arkDeclPlayerStateMantle."
               );
  puVar4 = (undefined4 *)
           FUN_14060ce50(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x330),auStackX_8,
                         &DAT_143995860);
  if ((undefined4 *)(param_1 + 0x110) != puVar4) {
    *(undefined4 *)(param_1 + 0x110) = *puVar4;
  }
  uVar2 = FUN_140854880(param_1);
  puStack_10 = &LAB_1408608e0;
  lStack_18 = param_1;
  uVar1 = FUN_140810240();
  FUN_140810810(uVar1,uVar2,0xec,&lStack_18);
  return;
}


