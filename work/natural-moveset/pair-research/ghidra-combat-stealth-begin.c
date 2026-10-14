
void FUN_14040b190(longlong param_1)

{
  undefined8 uVar1;
  undefined8 uVar2;
  longlong lVar3;
  longlong lStack_18;
  code *pcStack_10;
  
  FUN_140571920();
  uVar1 = *(undefined8 *)(param_1 + 0x20);
  uVar2 = FUN_140877720(uVar1,&DAT_1439e2a00,2);
  *(undefined8 *)(param_1 + 0xc0) = uVar2;
  lVar3 = FUN_140877720(uVar1,&DAT_1439dcd30,2);
  if ((lVar3 != 0) && (*(longlong *)(lVar3 + 0x68) != 0)) {
    uVar2 = FUN_14063c890();
    uVar2 = FUN_1408d04d0(uVar2);
    FUN_1403dab80(param_1 + 0xd8,uVar2);
  }
  lVar3 = (**(code **)(*D2_GameSingletonPtr + 0x1c8))
                    (D2_GameSingletonPtr,&DAT_1439bdeb0,0,&DAT_1431af668,&DAT_14228a780,0);
  uVar2 = FUN_14040abe0(lVar3,1);
  *(undefined8 *)(param_1 + 0xb8) = uVar2;
  if (lVar3 == 0) {
    *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;
  }
  else {
    *(undefined4 *)(param_1 + 0xb0) = *(undefined4 *)(lVar3 + 0xa8);
  }
  pcStack_10 = (code *)&LAB_14040bee0;
  lStack_18 = param_1;
  uVar2 = FUN_140810240();
  FUN_140810810(uVar2,uVar1,0xb5,&lStack_18);
  pcStack_10 = FUN_14040bef0;
  lStack_18 = param_1;
  uVar2 = FUN_140810240();
  FUN_140810810(uVar2,uVar1,0xdf,&lStack_18);
  return;
}


