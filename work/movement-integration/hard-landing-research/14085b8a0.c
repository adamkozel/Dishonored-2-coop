
void FUN_14085b8a0(longlong param_1)

{
  undefined8 uVar1;
  char cVar2;
  longlong lVar3;
  longlong lVar4;
  
  if (*(char *)(param_1 + 0x6c) != '\0') {
    lVar3 = FUN_1406dc560(*(undefined4 *)(param_1 + 0x68));
    lVar4 = FUN_1406dc560(*(undefined4 *)(param_1 + 100));
    if ((lVar3 != 0) && (lVar4 != 0)) {
      lVar3 = FUN_140877720(lVar3,&DAT_1439ee6d0,0);
      if ((lVar3 == 0) || (((byte)*(undefined4 *)(lVar3 + 0x1c) & 3) < 2)) goto LAB_14085b90f;
      FUN_1404d7030(lVar3,lVar4);
    }
    *(undefined1 *)(param_1 + 0x6c) = 0;
  }
LAB_14085b90f:
  uVar1 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 800);
  FUN_1407527e0(uVar1,"arkPlayerStateCatFall",0,&DAT_1431af668);
  FUN_140752870(uVar1,"arkPlayerStateCatFall",0,&DAT_1431af668);
  cVar2 = FUN_140854920(param_1);
  if (cVar2 == '\0') {
    return;
  }
  FUN_140854190(param_1);
  return;
}


