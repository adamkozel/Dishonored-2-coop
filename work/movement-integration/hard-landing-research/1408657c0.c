
float FUN_1408657c0(longlong param_1,float param_2)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  
  lVar2 = *(longlong *)(param_1 + 0x18);
  iVar1 = *(int *)(param_1 + 0xa4);
  fVar7 = *(float *)(lVar2 + 0x15c);
  fVar6 = *(float *)(lVar2 + 0x164);
  if ((-1 < iVar1) &&
     (lVar3 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x330) + 0x48),
     *(float *)(lVar3 + 0x24 + (longlong)iVar1 * 0x28) !=
     *(float *)(lVar3 + 8 + (longlong)iVar1 * 0x28))) {
    fVar7 = *(float *)(lVar2 + 0x160);
    fVar6 = *(float *)(lVar2 + 0x168);
  }
  if (param_2 - fVar7 < fVar6) {
    fVar5 = (float)(**(code **)(**(longlong **)(*(longlong *)(param_1 + 0x10) + 0x348) + 0xd8))();
    if (*(int *)(param_1 + 0xa8) < 0) {
      fVar4 = 1.0;
    }
    else {
      fVar4 = *(float *)(*(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x330) + 0x48)
                         + 0x24 + (longlong)*(int *)(param_1 + 0xa8) * 0x28);
    }
    fVar4 = fVar4 * fVar5 * ((param_2 - fVar7) / (fVar6 - fVar7));
  }
  else {
    fVar4 = 3.4028235e+38;
  }
  return fVar4;
}


