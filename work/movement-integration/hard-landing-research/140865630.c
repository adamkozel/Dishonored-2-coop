
void FUN_140865630(longlong param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  undefined4 *puVar5;
  
  lVar4 = *(longlong *)(param_1 + 0x10);
  lVar2 = *(longlong *)(param_1 + 0x18);
  lVar3 = *(longlong *)(lVar4 + 0x300);
  if ((*(uint *)(lVar3 + 0xdb4) & 0xfffffffb) != 0) {
    FUN_140775270(lVar3,*(undefined1 *)(lVar3 + 0xdb8),0);
  }
  *(undefined4 *)(*(longlong *)(lVar4 + 0x308) + 0x2ac) = *(undefined4 *)(lVar2 + 0xac);
  if (*(char *)(param_1 + 0xa0) == '\0') {
    FUN_140777bc0(*(undefined8 *)(lVar4 + 0x338),0x20);
  }
  if (*(char *)(param_1 + 0xa1) != '\0') {
    lVar4 = FUN_140854880(param_1);
    uVar1 = *(undefined4 *)(lVar4 + 0xa8);
    lVar4 = FUN_140810240();
    puVar5 = (undefined4 *)FUN_1408112c0(lVar4 + 0xc8e8,0x17c,4,uVar1);
    if (puVar5 != (undefined4 *)0x0) {
      *puVar5 = 5;
    }
  }
  FUN_14083ea30(param_1);
  return;
}


