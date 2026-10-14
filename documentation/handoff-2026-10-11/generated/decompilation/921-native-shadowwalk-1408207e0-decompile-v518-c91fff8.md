# native-shadowwalk-1408207e0-decompile-v518.txt

Original: [work/movement-integration/native-shadowwalk-1408207e0-decompile-v518.txt](<../../../../work/movement-integration/native-shadowwalk-1408207e0-decompile-v518.txt>)

SHA-256: `DC81BE1B94DFF4DFD3A9A673A4AAD9F0D13F405981F8AACA51B41ED5ED963D0E`; 3096 bytes.

## Original saved record

````text
void FUN_1408207e0(longlong param_1)

{
  undefined4 uVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  bool bVar5;
  char cVar6;
  undefined4 *puVar7;
  float afStackX_8 [2];
  float fStack_38;
  float fStack_34;
  float fStack_30;
  
  lVar2 = *(longlong *)(param_1 + 0x10);
  lVar3 = *(longlong *)(param_1 + 0x18);
  if (*(int *)(param_1 + 0x28) != 0) {
    lVar4 = *(longlong *)(lVar3 + 0xc0);
    if (*(int *)(param_1 + 0x2c) != -1) {
      FUN_140812290(lVar2,*(undefined4 *)(param_1 + 0x2c));
    }
    *(undefined4 *)(param_1 + 0x28) = 0;
    if (lVar4 != 0) {
      puVar7 = (undefined4 *)FUN_1408120b0(*(undefined8 *)(param_1 + 0x10),afStackX_8,lVar4);
      *(undefined4 *)(param_1 + 0x2c) = *puVar7;
    }
  }
  if ((*(int *)(param_1 + 0x28) == 0) && (*(longlong **)(param_1 + 0x20) != (longlong *)0x0)) {
    cVar6 = (**(code **)(**(longlong **)(param_1 + 0x20) + 0xd0))();
    if (cVar6 != '\0') {
      if ((*(char *)(param_1 + 0x38) == '\0') && (*(longlong *)(lVar3 + 0x100) != 0)) {
        puVar7 = (undefined4 *)FUN_1408120b0(*(undefined8 *)(param_1 + 0x10),afStackX_8);
        uVar1 = *puVar7;
        *(undefined1 *)(param_1 + 0x38) = 1;
        *(undefined4 *)(param_1 + 0x34) = uVar1;
      }
      goto LAB_14082089f;
    }
  }
  if (*(char *)(param_1 + 0x38) != '\0') {
    FUN_140812290(*(undefined8 *)(param_1 + 0x10),*(undefined4 *)(param_1 + 0x34));
    *(undefined1 *)(param_1 + 0x38) = 0;
  }
LAB_14082089f:
  cVar6 = *(char *)(*(longlong *)(lVar2 + 0x20) + 0x172);
  lVar4 = *(longlong *)(*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x10) + 0x388);
  if ((((cVar6 == '\0') && (lVar4 != 0)) && (*(int *)(param_1 + 0x30) == -1)) &&
     (*(longlong *)(lVar3 + 0xe0) != 0)) {
    afStackX_8[0] = *(float *)(lVar4 + 0xec);
    if (afStackX_8[0] <= *(float *)(lVar3 + 0xf8)) {
      puVar7 = (undefined4 *)FUN_1408120b0(*(longlong *)(param_1 + 0x10),afStackX_8);
      *(undefined4 *)(param_1 + 0x30) = *puVar7;
    }
  }
  FUN_1407fdb50(*(undefined8 *)(*(longlong *)(lVar2 + 0x28) + 0x308),&fStack_38);
  bVar5 = *(float *)(lVar3 + 0x118) <=
          SQRT(fStack_38 * fStack_38 + fStack_34 * fStack_34 + fStack_30 * fStack_30);
  if (cVar6 == '\0') {
    if (bVar5) {
      if (*(char *)(param_1 + 0x39) == '\0') {
        FUN_14081e7d0(param_1 + 0x50,lVar2);
      }
    }
    else if (*(char *)(param_1 + 0x39) != '\0') {
      FUN_14081e850(param_1 + 0x50,lVar2);
    }
    if (*(char *)(param_1 + 0x3a) != '\0') {
      if (*(longlong *)(lVar3 + 0xf0) != 0) {
        FUN_1408120b0(*(undefined8 *)(param_1 + 0x10),afStackX_8);
      }
      FUN_14081e7d0(param_1 + 0x40,lVar2);
      if (bVar5) {
        FUN_14081e7d0(param_1 + 0x50,lVar2);
      }
    }
  }
  else if (*(char *)(param_1 + 0x3a) == '\0') {
    if (*(longlong *)(lVar3 + 0xe8) != 0) {
      FUN_1408120b0(*(undefined8 *)(param_1 + 0x10),afStackX_8);
    }
    FUN_14081e850(param_1 + 0x40,lVar2);
    if (*(char *)(param_1 + 0x39) != '\0') {
      FUN_14081e850(param_1 + 0x50,lVar2);
    }
  }
  *(bool *)(param_1 + 0x39) = bVar5;
  *(char *)(param_1 + 0x3a) = cVar6;
  return;
}

````
