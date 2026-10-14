# swarm-decompile-1406bae00-v288.txt

Original: [work/movement-integration/swarm-decompile-1406bae00-v288.txt](<../../../../work/movement-integration/swarm-decompile-1406bae00-v288.txt>)

SHA-256: `753CC556F370EF145F24DA465ED77C4E635E6AC9724976D8A7FF6ED3F290CA4A`; 832 bytes.

## Original saved record

````text

void FUN_1406bae00(longlong param_1,int param_2)

{
  longlong lVar1;
  
  if (*(int *)(param_1 + 0x48) == 3) {
    FUN_1406bb6a0();
  }
  if (*(longlong *)(param_1 + 0x78) != 0) {
    FUN_14048f910();
    FUN_140540230(*(undefined8 *)(param_1 + 0x78));
  }
  *(int *)(param_1 + 0x48) = param_2;
  if ((param_2 != 1) && ((param_2 != 3 || (*(int *)(param_1 + 0x8c) == 2)))) {
    lVar1 = *(longlong *)(param_1 + 0x28);
    if (lVar1 != 0) {
      *(uint *)(lVar1 + 0x8c) =
           *(uint *)(lVar1 + 0x8c) ^
           ((int)*(short *)(lVar1 + 0x8c) - 1U ^ *(uint *)(lVar1 + 0x8c)) & 0xffff;
    }
    *(undefined8 *)(param_1 + 0x28) = 0;
    lVar1 = *(longlong *)(*(longlong *)(param_1 + 0x18) + 8);
    *(undefined2 *)(param_1 + 0x30) = 0;
    *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(lVar1 + 0x200);
  }
  return;
}



````
