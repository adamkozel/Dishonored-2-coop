
void FUN_140865310(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  longlong lVar5;
  float fVar6;
  float fVar7;
  
  FUN_14083e870();
  lVar2 = *(longlong *)(param_1 + 0x10);
  lVar3 = *(longlong *)(param_1 + 0x18);
  lVar4 = *(longlong *)(lVar2 + 0x300);
  lVar5 = FUN_140854880();
  *(undefined4 *)(param_1 + 0x8c) = *(undefined4 *)(lVar5 + 0x300);
  *(undefined4 *)(param_1 + 0x90) = *(undefined4 *)(lVar5 + 0x304);
  *(undefined4 *)(param_1 + 0x94) = *(undefined4 *)(lVar5 + 0x308);
  if (*(char *)(param_1 + 0x6a) == '\0') {
    if (*(char *)(param_1 + 0x6c) == '\0') goto LAB_1408653b7;
    lVar5 = 0;
    if (*(longlong *)(lVar2 + 0x90) != 0) {
      lVar5 = *(longlong *)(*(longlong *)(lVar2 + 0x90) + 0x70);
    }
    if ((*(byte *)(lVar5 + 0xa4) >> 1 & 1) == 0) goto LAB_1408653b7;
  }
  if ((*(longlong *)(lVar2 + 0x90) != 0) &&
     (lVar5 = *(longlong *)(*(longlong *)(lVar2 + 0x90) + 0x10), lVar5 != 0)) {
    FUN_140774970(lVar4,*(undefined1 *)(lVar5 + 0x34),7);
  }
LAB_1408653b7:
  cVar1 = *(char *)(*(longlong *)(lVar2 + 0x2f8) + 0x188);
  if (*(char *)(param_1 + 0x6b) == '\0') {
    if (cVar1 == '\0') {
      fVar6 = *(float *)(*(longlong *)(lVar2 + 0x308) + 0x2fc) / *(float *)(lVar3 + 0x170);
      fVar7 = 1.0;
      if (fVar6 <= 1.0) {
        fVar7 = fVar6;
      }
      FUN_140774f10(lVar4,*(undefined1 *)(param_1 + 0x68),fVar7,0);
    }
  }
  else {
    *(undefined1 *)(lVar4 + 0xdc0) = 0;
  }
  if ((*(char *)(param_1 + 0x6d) != '\0') && (cVar1 != '\0')) {
    FUN_140800fa0(*(undefined8 *)(lVar2 + 0x308));
  }
  *(undefined2 *)(param_1 + 0xa0) = 0;
  FUN_140777bc0(*(undefined8 *)(lVar2 + 0x338),0x20,1);
  return;
}


