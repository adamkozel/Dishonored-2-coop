void FUN_1408681a0(longlong param_1)

{
  longlong lVar1;
  undefined1 uVar2;
  char cVar3;
  longlong lVar4;
  
  FUN_14083eb00();
  lVar1 = *(longlong *)(param_1 + 0x10);
  uVar2 = FUN_140803180(lVar1,9);
  *(undefined1 *)(param_1 + 0xb8) = uVar2;
  cVar3 = FUN_140803180(lVar1,3);
  if (cVar3 != '\0') {
    lVar4 = 0;
    if (*(longlong *)(lVar1 + 0x90) != 0) {
      lVar4 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x28);
    }
    *(undefined1 *)(param_1 + 0x68) = *(undefined1 *)(lVar4 + 0x68);
  }
  return;
}
