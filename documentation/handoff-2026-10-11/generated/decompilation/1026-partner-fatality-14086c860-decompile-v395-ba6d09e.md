# partner-fatality-14086c860-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-14086c860-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-14086c860-decompile-v395.txt>)

SHA-256: `D731D416B84F3418A3DBBCF544CA2150B897453946083CAF507073B2DD89A4FA`; 1203 bytes.

## Original saved record

````text

void FUN_14086c860(longlong param_1)

{
  byte bVar1;
  char cVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  
  *(undefined1 *)(param_1 + 0xdb) = 0;
  uVar3 = FUN_140777b90(*(undefined8 *)(*(longlong *)(param_1 + 0x10) + 0x338),0x21);
  lVar4 = FUN_1403b8020(uVar3,0);
  if (lVar4 != 0) {
    cVar2 = FUN_140803180(*(undefined8 *)(param_1 + 0x10),0xc);
    if (cVar2 != '\0') {
      *(undefined1 *)(param_1 + 0xdb) = 1;
      lVar5 = 0;
      lVar4 = *(longlong *)(*(longlong *)(param_1 + 0x10) + 0x90);
      if (lVar4 != 0) {
        lVar5 = *(longlong *)(lVar4 + 0x70);
      }
      bVar1 = *(byte *)(lVar5 + 0xa5);
      if (-1 < *(int *)(param_1 + 0x10c)) {
        if (0.0 < *(float *)(*(longlong *)
                              (*(longlong *)(*(longlong *)(param_1 + 0x10) + 0x330) + 0x48) + 0x24 +
                            (longlong)*(int *)(param_1 + 0x10c) * 0x28)) {
          *(undefined1 *)(param_1 + 0xd3) = 1;
          *(byte *)(param_1 + 0xd4) = bVar1 >> 2 & 1;
          FUN_140855570(param_1);
          return;
        }
      }
      goto LAB_14086c91c;
    }
  }
  *(undefined2 *)(param_1 + 0xd3) = 0;
LAB_14086c91c:
  FUN_140855570(param_1);
  return;
}



````
