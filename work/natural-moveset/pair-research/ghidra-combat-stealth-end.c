
void FUN_14040b2f0(longlong param_1)

{
  undefined4 uVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lStack_28;
  code *pcStack_20;
  
  uVar4 = *(undefined8 *)(param_1 + 0x20);
  pcStack_20 = FUN_14040bef0;
  lStack_28 = param_1;
  uVar2 = FUN_140810240();
  FUN_140811160(uVar2,uVar4,0xdf,&lStack_28);
  uVar4 = *(undefined8 *)(param_1 + 0x20);
  pcStack_20 = (code *)&LAB_14040bee0;
  lStack_28 = param_1;
  uVar2 = FUN_140810240();
  FUN_140811160(uVar2,uVar4,0xb5,&lStack_28);
  FUN_1405fb960(*(undefined8 *)(param_1 + 0xc0));
  if ((7 < ((byte)*(undefined4 *)(*(longlong *)(param_1 + 0xb8) + 0x1c) & 0xc)) &&
     ((*(uint *)(*(longlong *)(param_1 + 0xb8) + 0x28) & 0xffffff) != 0)) {
    FUN_140757550();
  }
  lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xb0));
  if (lVar3 != 0) {
    FUN_1408847f0(lVar3,&lStack_28,&DAT_1439a6f30);
    FUN_140882fc0(&lStack_28);
    *(undefined4 *)(param_1 + 0xb0) = 0x3fff0010;
  }
  FUN_1403dab80(param_1 + 0xd8,0);
  *(undefined8 *)(param_1 + 0xc0) = 0;
  *(undefined8 *)(param_1 + 0xb8) = 0;
  FUN_1403dab80(param_1 + 200,0);
  uVar1 = *(undefined4 *)(*(longlong *)(param_1 + 0x20) + 0xa8);
  uVar4 = FUN_140810240();
  FUN_1408103f0(uVar4,0x112,uVar1);
  FUN_140571950(param_1);
  return;
}


