# partner-collision-1401b58c0-decompile-v380.txt

Original: [work/movement-integration/partner-collision-1401b58c0-decompile-v380.txt](<../../../../work/movement-integration/partner-collision-1401b58c0-decompile-v380.txt>)

SHA-256: `492FE33B1AB5AE800649FC8FEB84F53207B2A727A9E413E8849CAE7FC306ED29`; 781 bytes.

## Original saved record

````text

void FUN_1401b58c0(longlong param_1,longlong param_2,undefined8 *param_3)

{
  uint uVar1;
  longlong *plVar2;
  longlong lVar3;
  uint uVar4;
  longlong lVar5;
  
  lVar3 = *(longlong *)(*(longlong *)(param_1 + 0x138) + 400);
  uVar1 = *(uint *)(param_2 + 0x30);
  uVar4 = uVar1 & 0xffffff;
  if ((((uVar4 != 0xffffff) && (uVar4 < *(uint *)(lVar3 + 0x30))) &&
      (lVar5 = (longlong)(int)uVar4 * 0xb0 + *(longlong *)(lVar3 + 0x28),
      (*(byte *)(lVar5 + 0x44) & 3) != 0)) && (*(uint *)(lVar5 + 0x70) == uVar1)) {
    plVar2 = (longlong *)
             FUN_140017d10((ulonglong)(uVar1 & 0xffffff) * 0xb0 + *(longlong *)(lVar3 + 0x28));
    lVar3 = (**(code **)(*plVar2 + 0x40))(plVar2);
    if (lVar3 != 0) {
      *param_3 = 0;
      param_3[1] = 0;
    }
  }
  return;
}



````
