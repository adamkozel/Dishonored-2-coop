void FUN_1408680a0(longlong param_1)

{
  longlong lVar1;
  float fVar2;
  
  fVar2 = *(float *)(param_1 + 0x90) + *(float *)(param_1 + 0xa4);
  *(float *)(param_1 + 0xa4) = fVar2;
  FUN_1407fdca0(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x308),fVar2,
                *(undefined4 *)(param_1 + 0xa8),*(undefined4 *)(param_1 + 0xac));
  *(int *)(param_1 + 0x94) = *(int *)(param_1 + 0x94) + -1;
  if (*(longlong *)(param_1 + 0xb0) != 0) {
    FUN_140709110(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x340),
                  (int)*(short *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x310) + 0x25198),
                  *(longlong *)(param_1 + 0xb0),0,0,0,1,0);
  }
  if (0 < *(int *)(param_1 + 0x94)) {
    lVar1 = *(longlong *)(param_1 + 0x18);
    *(undefined4 *)(param_1 + 0x90) = *(undefined4 *)(param_1 + 0x98);
    *(undefined8 *)(param_1 + 0xb0) = *(undefined8 *)(lVar1 + 400);
    *(undefined4 *)(param_1 + 0xa8) = *(undefined4 *)(lVar1 + 0x17c);
    *(undefined4 *)(param_1 + 0xac) = *(undefined4 *)(lVar1 + 0x180);
  }
  return;
}
