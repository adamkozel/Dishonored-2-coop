
undefined8 FUN_14085b2e0(longlong param_1)

{
  char cVar1;
  char cVar2;
  
  cVar1 = *(char *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x2f8) + 0x281);
  cVar2 = FUN_140854890();
  if ((cVar2 != '\0') && (cVar1 == '\0')) {
    return 1;
  }
  return 0;
}


