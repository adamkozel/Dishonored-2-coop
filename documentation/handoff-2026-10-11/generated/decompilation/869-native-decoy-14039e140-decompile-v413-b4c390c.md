# native-decoy-14039e140-decompile-v413.txt

Original: [work/movement-integration/native-decoy-14039e140-decompile-v413.txt](<../../../../work/movement-integration/native-decoy-14039e140-decompile-v413.txt>)

SHA-256: `81655DA3DB0CC7563E18BCF9BB3EF0D28FE35A1F2054CE78A7A1632AF2369C91`; 339 bytes.

## Original saved record

````text

void FUN_14039e140(longlong param_1)

{
  longlong lVar1;
  int iVar2;
  
  if ((1 < *(int *)(param_1 + 0xa0) - 2U) &&
     (lVar1 = *(longlong *)(*(longlong *)(param_1 + 8) + 0x78), lVar1 != 0)) {
    iVar2 = FUN_1407c7d30(param_1 + 0x38,lVar1,0);
    if (iVar2 == 0) {
      *(undefined4 *)(param_1 + 0xa0) = 2;
    }
  }
  return;
}



````
