void FUN_140867ef0(longlong param_1)

{
  undefined8 uVar1;
  longlong lVar2;
  char cVar3;
  
  uVar1 = *(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x308);
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x300);
  cVar3 = FUN_1407fdc10(uVar1);
  if (cVar3 != '\0') {
    FUN_1407ff1c0(uVar1);
  }
  if (*(char *)(param_1 + 0xb9) == '\0') {
    FUN_140775270(lVar2,*(undefined1 *)(lVar2 + 0xdb8),0);
  }
  else {
    *(undefined1 *)(lVar2 + 0xdc0) = 1;
  }
  *(undefined8 *)(param_1 + 0x90) = 0;
  *(undefined4 *)(param_1 + 0x98) = 0;
  *(undefined8 *)(param_1 + 0xa0) = 0;
  FUN_14083ea30(param_1);
  return;
}
