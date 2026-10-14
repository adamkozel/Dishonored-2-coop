
void FUN_14085b1e0(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined1 auStack_38 [48];
  
  lVar1 = *(longlong *)(param_1 + 0x10);
  if ((*(ulonglong *)(*(longlong *)(lVar1 + 0x2f8) + 0xa0) >>
       ((ulonglong)*(uint *)(*(longlong *)(lVar1 + 0x2f8) + 0x6c) & 0x3f) & 1) == 0) {
    uVar3 = 0;
  }
  else {
    uVar3 = 6;
  }
  FUN_1408022e0(lVar1,uVar3);
  lVar2 = *(longlong *)(lVar1 + 0x300);
  if (*(char *)(param_1 + 0x54) == '\0') {
    FUN_140774a10(lVar2 + 0x370,1);
  }
  if (*(char *)(param_1 + 0x55) == '\0') {
    FUN_140774a10(lVar2 + 0x680,1);
  }
  FUN_140774600(lVar2,0);
  FUN_140835590(auStack_38,0,0x46,*(undefined4 *)(param_1 + 8));
  if (*(char *)(param_1 + 0x56) != '\0') {
    FUN_1408024d0(lVar1,0x40,auStack_38);
  }
  if (*(char *)(param_1 + 0x57) != '\0') {
    FUN_1408024d0(lVar1,0x35,auStack_38);
  }
  *(undefined4 *)(param_1 + 100) = 0x3fff0010;
  *(undefined4 *)(param_1 + 0x68) = 0x3fff0010;
  *(undefined1 *)(param_1 + 0x6c) = 0;
  FUN_1408541a0(param_1);
  return;
}


