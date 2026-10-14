# partner-fatality-14086c3e0-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-14086c3e0-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-14086c3e0-decompile-v395.txt>)

SHA-256: `9E0748EAEAFD79B04636153B8E813C1CC5BE3C0647BF585B5C64C5A7B9393F38`; 607 bytes.

## Original saved record

````text

undefined8 FUN_14086c3e0(longlong param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  
  lVar5 = *(longlong *)(param_1 + 0x10);
  iVar2 = FUN_140802de0(lVar5);
  if (iVar2 == 2) {
    uVar3 = FUN_140855230(param_1);
    return uVar3;
  }
  lVar5 = *(longlong *)(*(longlong *)(lVar5 + 0x328) + 0x68);
  cVar1 = FUN_140855230(param_1);
  if ((cVar1 != '\0') && (lVar5 != 0)) {
    lVar4 = FUN_14063c8c0(lVar5);
    if (lVar4 != 0) {
      lVar5 = FUN_14063c8c0(lVar5);
      if (*(int *)(lVar5 + 0x11c) == 1) {
        return 1;
      }
    }
  }
  return 0;
}



````
