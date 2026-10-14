# decompile-14086d060.txt

Original: [work/movement-integration/native-kick-research-v624/decompile-14086d060.txt](<../../../../work/movement-integration/native-kick-research-v624/decompile-14086d060.txt>)

SHA-256: `4049A8F7CBC52312A8B3F24D3FEDFDA6286114EFCAC0F4FE3770E5A8414D10E6`; 2016 bytes.

## Original saved record

````text

void FUN_14086d060(longlong param_1)

{
  longlong lVar1;
  longlong lVar2;
  bool bVar3;
  char cVar4;
  longlong lVar5;
  undefined8 uVar6;
  double dStackX_8;
  
  FUN_1408555a0();
  lVar5 = *(longlong *)(param_1 + 0x10);
  lVar1 = *(longlong *)(param_1 + 0x18);
  lVar2 = *(longlong *)(lVar5 + 0x2f8);
  bVar3 = false;
  if (*(char *)(param_1 + 0xd0) == '\0') {
LAB_14086d0b8:
    if (*(char *)(param_1 + 0xd1) != '\0') goto LAB_14086d0c1;
  }
  else {
    if (*(char *)(param_1 + 0xd1) == '\0') {
      if ((*(char *)(param_1 + 0xd2) == '\0') && (*(char *)(param_1 + 0xd5) == '\0')) {
        FUN_14086cf10(param_1);
      }
      goto LAB_14086d0b8;
    }
LAB_14086d0c1:
    cVar4 = FUN_140803180(lVar5,7);
    if (cVar4 != '\0') {
      FUN_14086a9a0(param_1);
      bVar3 = true;
    }
  }
  if (*(char *)(param_1 + 0xd5) == '\0') {
    if ((*(ulonglong *)(lVar2 + 0x80) &
        1L << ((byte)*(undefined4 *)(param_1 + 0x34) & 0x3f) & *(ulonglong *)(lVar2 + 0x98)) == 0) {
      *(undefined1 *)(param_1 + 0xd5) = 1;
    }
    if (*(char *)(param_1 + 0xd5) == '\0') goto LAB_14086d13f;
  }
  if ((*(char *)(param_1 + 0xd6) == '\0') &&
     ((*(ulonglong *)(*(longlong *)(lVar5 + 0x2f8) + 0x80) &
      1L << ((byte)*(undefined4 *)(param_1 + 0x34) & 0x3f) &
      *(ulonglong *)(*(longlong *)(lVar5 + 0x2f8) + 0x98)) != 0)) {
    *(undefined1 *)(param_1 + 0xd6) = 1;
  }
LAB_14086d13f:
  lVar5 = FUN_1406dc560(*(undefined4 *)(param_1 + 0xe0));
  if ((lVar5 != 0) &&
     (cVar4 = FUN_140694c30(lVar2 + 0x760,lVar5,*(undefined4 *)(lVar1 + 0xc4)), cVar4 != '\0')) {
    uVar6 = FUN_140854880(param_1);
    FUN_140878030(uVar6,&dStackX_8);
    if ((*(double *)(param_1 + 0xf8) < 0.0) ||
       ((double)*(float *)(lVar1 + 0xc0) + *(double *)(param_1 + 0xf8) < dStackX_8)) {
      *(double *)(param_1 + 0xf8) = dStackX_8;
      FUN_1407fb0c0(lVar2,lVar5);
      FUN_140854190(param_1);
      return;
    }
  }
  if (bVar3) {
    FUN_140854190(param_1);
  }
  else {
    FUN_14086d200(param_1);
  }
  return;
}



````
