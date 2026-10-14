# partner-collision-140093b00-decompile-v380.txt

Original: [work/movement-integration/partner-collision-140093b00-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-140093b00-decompile-v380.txt>)

SHA-256: `4169D3E27872B7C03C9F26038DA974FD004276BF9C9D85689787C25CA56AF28B`; 1069 bytes.

## Original saved record

````text

ulonglong FUN_140093b00(longlong param_1,uint param_2,uint param_3)

{
  uint uVar1;
  longlong lVar2;
  undefined8 uVar3;
  uint uVar4;
  uint uVar5;
  ulonglong uVar6;
  uint uStackX_c;
  
  uVar4 = param_3;
  uStackX_c = param_2;
  if ((param_2 & 0xffffff) < (param_3 & 0xffffff)) {
    uVar4 = param_2;
    uStackX_c = param_3;
  }
  uVar1 = *(uint *)(param_1 + 0x2c);
  if (0 < (int)uVar1) {
    lVar2 = *(longlong *)(param_1 + 0x20);
    uVar5 = (uStackX_c & 0xffffff) * -0x61c8864f;
    while( true ) {
      uVar5 = uVar5 & uVar1;
      uVar6 = (ulonglong)uVar5;
      if (((*(uint *)(lVar2 + uVar6 * 0xc) & 0xffffff) == 0xffffff) ||
         ((*(uint *)(lVar2 + 4 + uVar6 * 0xc) & 0xffffff) == 0xffffff)) break;
      uVar3 = *(undefined8 *)(lVar2 + uVar6 * 0xc);
      if (((uint)uVar3 == uVar4) && ((uint)((ulonglong)uVar3 >> 0x20) == uStackX_c)) {
        return (ulonglong)CONCAT31((int3)(uVar5 >> 8),(int)uVar5 <= (int)uVar1);
      }
      uVar5 = uVar5 + 1;
    }
  }
  return CONCAT71((int7)(int3)(uVar1 + 1 >> 8),(int)(uVar1 + 1) <= (int)uVar1);
}



````
