
void FUN_140865b90(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  
  FUN_14083eb00();
  lVar1 = *(longlong *)(param_1 + 0x10);
  if (*(char *)(param_1 + 0x6b) == '\0') {
    *(undefined4 *)(param_1 + 0x9c) = 0x7149f2ca;
  }
  else {
    lVar2 = 0;
    if (*(longlong *)(lVar1 + 0x90) != 0) {
      lVar2 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x20);
    }
    *(undefined1 *)(param_1 + 0x68) = *(undefined1 *)(lVar2 + 0x68);
    *(undefined1 *)(param_1 + 0x69) = *(undefined1 *)(lVar2 + 0x69);
    *(undefined1 *)(param_1 + 0x6a) = *(undefined1 *)(lVar2 + 0x6a);
    *(undefined4 *)(param_1 + 0x9c) = *(undefined4 *)(lVar2 + 0xa0);
  }
  lVar2 = 0;
  if (*(longlong *)(lVar1 + 0x90) != 0) {
    lVar2 = *(longlong *)(*(longlong *)(lVar1 + 0x90) + 0x70);
  }
  if (((*(char *)(param_1 + 0x6c) == '\0') ||
      (*(bool *)(param_1 + 0x68) =
            (*(ulonglong *)(*(longlong *)(lVar1 + 0x2f8) + 0xa0) >>
             ((ulonglong)*(uint *)(*(longlong *)(lVar1 + 0x2f8) + 0x6c) & 0x3f) & 1) != 0,
      *(char *)(param_1 + 0x6c) == '\0')) || ((lVar2 != 0 && ((*(byte *)(lVar2 + 0xa4) & 8) == 0))))
  {
    *(undefined4 *)(param_1 + 0x88) = 0;
  }
  return;
}


