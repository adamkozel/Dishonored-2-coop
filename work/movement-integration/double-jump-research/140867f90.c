undefined1 FUN_140867f90(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  bool bVar4;
  char cVar5;
  
  lVar1 = *(longlong *)(param_1 + 0x10);
  lVar2 = *(longlong *)(param_1 + 0x18);
  cVar5 = FUN_140803180(lVar1,9);
  if ((*(int *)(*(longlong *)(lVar1 + 0x2f8) + 0x68) != 1) ||
     (lVar3 = *(longlong *)(lVar1 + 0x10),
     *(float *)(lVar3 + 0x33c) * *(float *)(lVar3 + 0x33c) +
     *(float *)(lVar3 + 0x340) * *(float *)(lVar3 + 0x340) +
     *(float *)(lVar3 + 0x344) * *(float *)(lVar3 + 0x344) <= 0.010000001)) {
    bVar4 = false;
  }
  else {
    bVar4 = true;
  }
  if ((((cVar5 != '\0') || (cVar5 = FUN_140869370(param_1), cVar5 != '\0')) &&
      (cVar5 = FUN_1407fdc30(*(undefined8 *)(lVar1 + 0x308),*(undefined4 *)(lVar2 + 0x158)),
      cVar5 == '\0')) && ((!bVar4 && (cVar5 = FUN_1408031b0(lVar1,9), cVar5 == '\0')))) {
    return 1;
  }
  return 0;
}
