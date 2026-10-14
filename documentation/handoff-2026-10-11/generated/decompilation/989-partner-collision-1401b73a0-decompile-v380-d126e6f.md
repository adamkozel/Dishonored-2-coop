# partner-collision-1401b73a0-decompile-v380.txt

Original: [work/movement-integration/partner-collision-1401b73a0-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-1401b73a0-decompile-v380.txt>)

SHA-256: `3742DA7A6636A98D0BA82B925A5800CAC1836F483D7D56F64BF700E5BAA56B01`; 562 bytes.

## Original saved record

````text

void FUN_1401b73a0(longlong param_1,uint param_2)

{
  uint uVar1;
  longlong lVar2;
  
  lVar2 = *(longlong *)(*(longlong *)(param_1 + 0x138) + 400);
  uVar1 = param_2 & 0xffffff;
  if ((((uVar1 != 0xffffff) && (uVar1 < *(uint *)(lVar2 + 0x30))) &&
      (lVar2 = (longlong)(int)uVar1 * 0xb0 + *(longlong *)(lVar2 + 0x28),
      (*(byte *)(lVar2 + 0x44) & 3) != 0)) && (*(uint *)(lVar2 + 0x70) == param_2)) {
    FUN_140095730(*(longlong *)(param_1 + 0x138),
                  *(undefined4 *)(*(longlong *)(param_1 + 0x148) + 0x38),param_2);
  }
  return;
}



````
