# partner-fatality-14086c470-decompile-v395.txt

Original: [work/movement-integration/partner-fatality-14086c470-decompile-v395.txt](<../../../../work/movement-integration/partner-fatality-14086c470-decompile-v395.txt>)

SHA-256: `EB885297DFD6C33DBACC8B26C89B3DE314523655E4D5568C86B41B9A26C1A6BC`; 749 bytes.

## Original saved record

````text

undefined8 FUN_14086c470(longlong param_1)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  longlong *plVar4;
  longlong lVar5;
  longlong lVar6;
  
  lVar5 = *(longlong *)(param_1 + 0x10);
  iVar2 = FUN_140802de0(lVar5);
  if (iVar2 == 2) {
    uVar3 = FUN_140855230(param_1);
    return uVar3;
  }
  lVar6 = *(longlong *)(*(longlong *)(lVar5 + 0x328) + 0x68);
  if (lVar6 == 0) {
    plVar4 = (longlong *)FUN_1407f88b0(*(undefined8 *)(lVar5 + 0x2f8),0x35);
    lVar6 = *plVar4;
  }
  cVar1 = FUN_140855230(param_1);
  if ((cVar1 != '\0') && (lVar6 != 0)) {
    lVar5 = FUN_14063c8c0(lVar6);
    if (lVar5 != 0) {
      lVar5 = FUN_14063c8c0(lVar6);
      if (*(int *)(lVar5 + 0x11c) == 1) {
        return 1;
      }
    }
  }
  return 0;
}



````
